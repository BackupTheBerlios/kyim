/**
   KYahoo class. Main class for application
*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <kmainwindow.h>
#include <kapp.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <kconfig.h>
#include <kstddirs.h>
#include <qfont.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <kpopupmenu.h>
#include <qpopupmenu.h>
#include <qmenudata.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <qstatusbar.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <khelpmenu.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <qstringlist.h>
#include <kiconloader.h>
#include <krun.h>
#include <kmessagebox.h>

extern "C" {
#include "yahoo2.h"
}
#include "kyim.h"
#include "process_packet.h"
#include "dialogs.h"
#include "AddBuddyImpl.h"
#include "RemoveBuddyImpl.h"
#include "OfflineMsgImpl.h"
#include "SetupImpl.h"
#include "StartChatDialog.h"
#include "Icon.h"
#include "utils.h"
#include "ProxyAuthImpl.h"
#include "KYahooIface.h"
#include "ChatSession.h"
#include "NewMailImpl.h"
#include "AddedMessageImpl.h"
#include "ConferenceSession.h"
#include "kyimicons.h"

#include "IMState.h"

#include "AddFriendDialog.h"

struct yahoo_context* context = NULL;

KYahoo* main_window;
IMState* imstate; 

extern const char * kyahooSysTray[];

// put this back as a local variable to main once the
// help menu situation is straightened out
KAboutData aboutData ( "kyim", "KYIM", VERSION,
		       I18N_NOOP("KDE Port of Yahoo Messenger"),
		       KAboutData::License_GPL_V2, 
		       "(c) 2000-2002, The KYIM Team",
		       I18N_NOOP("This software is not supported or sponsored in any way by Yahoo."),
		       "http://kyim.sourceforge.net",
		       "kyim-support@lists.sourceforge.net");

int main ( int argc, char* argv[] )
{
  /* hunt for old-style kyahoorc and move to new location, and make sure
     that kyimrc file is read/write user only */
  QString newPath, oldPath;
  newPath += getenv("HOME");
  oldPath += newPath;
  newPath += "/.kde/share/config/kyimrc";
  oldPath += "/.kde/share/config/kyahoorc";
  rename( (const char *)oldPath, (const char *)newPath);
  chmod( (const char *)newPath, (S_IREAD|S_IWRITE));

  
  aboutData.addAuthor("Jay Lubomirski",0, "jaylubo@fiam.net");
  aboutData.addAuthor("Frank Dekervel",0, "Frank.dekervel@student.kuleuven.ac.Be");
  aboutData.addAuthor("Haris Fauzi",0, "haris@trustix.com");
  aboutData.addAuthor("Praveen Karadakal",0, "praveenkv@mindtree.com");
  aboutData.addAuthor("Alexandre Aufrere",0, "alexandre.aufrere@free.fr");
  aboutData.addAuthor("Ankit Jain",0, "radical@corewars.org");
  aboutData.addAuthor("Ariya Hidayat", 0, "ariya@kde.org");
	
  aboutData.addCredit("Ankur Consul", I18N_NOOP("emoticons artwork"),"ankz_c@hotmail.com");

  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app;

  if (app.isRestored())
    {
      if ( KMainWindow::canBeRestored(1) )
	{
	  main_window = new KYahoo();
	  main_window->restore(1);
	}
    }
  else 
    {
      main_window = new KYahoo();

      startup();

      main_window->show();
    }
   

  app.dcopClient()->attach();
  app.dcopClient()->registerAs(app.name());
  
  return( app.exec() );
  
} // main()

void startup( void )
{
  if( main_window->call_setup() )
    {
      // configuration failed -- show setup dialog
      // create modal dialog for startup
      SetupImpl* sd = new SetupImpl(NULL, "startup", true);
      sd->exec();
      delete sd;

      // save back configuration we just setup
      main_window->writeConfig();
    }
}

// constructor
KYahoo::KYahoo() :
  KMainWindow()
{
  // determine whether we can read username and password from
  // .kyahoorc file
  need_setup = 0;

  Ptr_Dbg_Dialog = NULL;

  // setup the options data structure with defaults 
  initialize_option_defaults();

  // get configuration 
  config = kapp->config();
  need_setup = readConfig();

  // system tray icon
  sysTray = new Icon(this, "kyahoo icon");
  sysTray->setPixmap(kyahooSysTray);
  sysTray->insertItem( i18n("&Logon"), this, SLOT( connect() ),
			 0, ID_YAHOO_LOGON );
  sysTray->insertItem( i18n("Log&off"), this, SLOT( disconnect() ),
			 0, ID_YAHOO_LOGOFF );
  sysTray->insertItem( i18n("&New Chat Session"), this,
		       SLOT( slotNewChat() ), 0, ID_YAHOO_NEW_CHAT);
  sysTray->insertItem( i18n("New &Conference"), this, 
		       SLOT( slotNewConf() ), 0, ID_YAHOO_NEW_CONFERENCE );
  sysTray->insertItem( i18n("&Send Offline Message"), this, 
		       SLOT( slotOffLineMsg() ), 0, ID_YAHOO_OFFLINE_MSG );
  sysTray->insertItem( i18n("View &Status Bar"), this,
                       SLOT(slotShowStatusBar()), 0, ID_SYSTRAY_VIEW_STATUS);
  sysTray->insertItem( i18n("View &Menu Bar"), this,
                       SLOT(slotShowMenuBar()), 0, ID_SYSTRAY_VIEW_MENU);
  sysTray->insertItem( i18n("View &Tool Bar"), this,
                       SLOT(slotShowToolBar()), 0, ID_SYSTRAY_VIEW_TOOLBAR);

  sysTray->show();

  //
  // file menu -- options & quit
  //
  filemenu = new QPopupMenu();
  filemenu->insertItem( i18n("O&ptions"), this, SLOT( showOptions() ),
			0, ID_FILE_OPTIONS );
  filemenu->insertItem( i18n("&Save Options"), this, SLOT( writeConfig() ),
			0, ID_FILE_SAVE_OPTIONS );
  filemenu->insertItem( i18n("&Refresh"), this, SLOT( slotRefreshAll() ),
			0, ID_FILE_REFRESH );
  filemenu->insertItem( i18n("&Quit"), this, SLOT( quit() ) );

  if( need_setup )
    filemenu->setItemEnabled(ID_FILE_SAVE_OPTIONS, true);
  else
    filemenu->setItemEnabled(ID_FILE_SAVE_OPTIONS, false);

  //
  // view menu
  // 
  viewmenu = new QPopupMenu();
  viewmenu->setCheckable(true);
  viewmenu->insertItem(i18n("View &Status Bar"), this,
                       SLOT(slotShowStatusBar()), 0, ID_VIEW_STATUS);
  viewmenu->insertItem(i18n("View &Menu Bar"), this,
                       SLOT(slotShowMenuBar()), 0, ID_VIEW_MENU);
  viewmenu->insertItem(i18n("View &Toolbar"), this,
                       SLOT(slotShowToolBar()), 0, ID_VIEW_TOOLBAR);
  viewmenu->insertItem( i18n("&Debug Messages"), this, 
			SLOT( slotShowDebug() ),
			0, ID_YAHOO_DEBUG_MSG );

  //
  // pager specific commands
  //
  yahoomenu = new QPopupMenu();
  yahoomenu->insertItem( i18n("&Logon"), this, SLOT( connect() ),
			 0, ID_YAHOO_LOGON );
  yahoomenu->insertItem( i18n("Log&off"), this, SLOT( disconnect() ),
			 0, ID_YAHOO_LOGOFF );
  yahoomenu->insertItem( i18n("&New Chat Session"), this,
			 SLOT( slotNewChat() ),
			 0, ID_YAHOO_NEW_CHAT);
  yahoomenu->insertItem( i18n("New &Conference"), this, 
			 SLOT( slotNewConf() ), 
			 0, ID_YAHOO_NEW_CONFERENCE );
  yahoomenu->insertItem( i18n("&Send Offline Message"), this, 
			 SLOT( slotOffLineMsg() ), 
			 0, ID_YAHOO_OFFLINE_MSG );
  yahoomenu->insertItem( i18n("&Add Friend"), this, SLOT( slotAddUser() ),
			 0, ID_YAHOO_ADD_USER );
  yahoomenu->insertItem( i18n("&Remove Friend"), this, 
			 SLOT( slotRemoveUser() ),
			 0, ID_YAHOO_REMOVE_USER );
			
  kyimtoolbar=toolBar();
  kyimtoolbar->insertButton(QPixmap(startchat_xpm),1,SIGNAL(clicked(int)), this, SLOT(slotNewChat()), true, i18n("Chat"));
  kyimtoolbar->insertButton(QPixmap(addfriends_xpm),2,SIGNAL(clicked(int)), this, SLOT(slotAddUser()), true, i18n("Add Friend"));
  kyimtoolbar->insertButton(QPixmap(findfriends_xpm),3,SIGNAL(clicked(int)), this, SLOT(slotFindFriends()), true, i18n("Find Friends"));
  kyimtoolbar->insertButton(QPixmap(mail_empty_xpm),4,SIGNAL(clicked(int)), this, SLOT(slotGoToMail()), true, i18n("Go To Mail"));
  kyimtoolbar->enableMoving(false);

  //
  // help menu
  //	
  /*  helpmenu = new KHelpMenu( this, &aboutData, false);*/
 
  menuBar()->insertItem( i18n("&File"), filemenu );
  menuBar()->insertItem( i18n("&View"), viewmenu );
  menuBar()->insertItem( i18n("&Yahoo"), yahoomenu );
  menuBar()->insertItem( i18n("&Help"), helpMenu() );
  
  // central widget
  imstate = new IMState( this );
  setCentralWidget( imstate );
  
 
  // status bar
  if ( options.current_user )
    statusBar()->insertItem( options.current_user, STATUS_USER );
  else
    statusBar()->insertItem( i18n("None"), STATUS_USER );
  statusBar()->insertItem( i18n("Not logged on."), STATUS_MSG );
  statusBar()->setItemAlignment( STATUS_MSG, Qt::AlignRight);
  
  disableMenus();
	
  isConnected=false;
	
  if( !need_setup && (options.login_at_startup == true) )
    /* connect to yahoo */
    connect();

  if( need_setup && (options.login_at_startup == true) )
    {
      warnmsg(i18n("You will need to login manually\n the first time.\n\nAfter setting up KYIM\nselect Yahoo->Login to login.\n"));
    }
  // dcop interface
  kyahoo_interface = new KYahooIface();
  
  // maintains geometry and menu/status bar state between launches
  setAutoSaveSettings("Main");

  viewmenu->setItemChecked( ID_VIEW_STATUS, !statusBar()->isHidden());
  sysTray->setItemChecked( ID_SYSTRAY_VIEW_STATUS, !statusBar()->isHidden());
  viewmenu->setItemChecked( ID_VIEW_MENU, !menuBar()->isHidden());
  sysTray->setItemChecked( ID_SYSTRAY_VIEW_MENU, !menuBar()->isHidden());
  viewmenu->setItemChecked( ID_VIEW_TOOLBAR, !toolBar()->isHidden());
  sysTray->setItemChecked( ID_SYSTRAY_VIEW_TOOLBAR, !toolBar()->isHidden());

//  registerIcons();
}


// destructor
KYahoo::~KYahoo()
{
  quit();

  delete(kyahoo_interface);

  /* quit the user interface, log out of yahoo (if logged in originally) */
  if ( context ) {
    yahoo_logoff(context->id);
  }
}


bool KYahoo::queryExit() 
{
  if ( filemenu->isItemEnabled ( ID_FILE_SAVE_OPTIONS ) )
    writeConfig();

  return(true);
}


void KYahoo::quit()
{
  if ( filemenu->isItemEnabled ( ID_FILE_SAVE_OPTIONS ) )
    writeConfig();
  
  saveMainWindowSettings(config);

  kapp->quit();
}


void KYahoo::slotNewChat()
{
  // don't do if not logged in
  if( !context )
    return;
  
  StartChatDialog* dialog = new StartChatDialog( this );
  dialog->show();
}


void KYahoo::slotNewConf()
{
  // don't do if not logged in
  if( !context )
    return;

  Conference_Invite_Window* dialog = new Conference_Invite_Window();
  dialog->show();
}

void KYahoo::slotOffLineMsg()
{
  // don't do if not logged in
  if( !context )
    return;

  OfflineMsgImpl* dialog = new OfflineMsgImpl(this);
  dialog->show();
}

void KYahoo::slotAddUser()
{
  // don't do if not logged in
  if( !context )
    return;
    
  AddFriendDialog* dlg = new AddFriendDialog( this );
  dlg->setGroupList( imstate->Groups() );
  if( dlg->exec() )
  {
    QString yahooId = dlg->yahooId();
    QString groupName = dlg->groupName();
    
    if( yahooId.isEmpty() )
    {
      KMessageBox::sorry( 0, i18n("You don't enter a Yahoo! ID" ), i18n("Failed" ) );
      return;
    }
    
    if( groupName.isEmpty() ) groupName = "Friends";    
    
    imstate->AddNewBuddy( yahooId, groupName );
    yahoo_add_buddy( context->id, yahooId.utf8(), groupName.utf8() );  
  }
  
  delete dlg;
}

void KYahoo::slotRemoveUser()
{
  // don't do if not logged in
  if( !context )
    return;

  RemoveBuddyImpl* dialog = new RemoveBuddyImpl(this);
  dialog->show();
}


/* initialize yahoolib -- only run once */
int KYahoo::initialize()
{
  if ( context )
    return 0;

  context = (struct yahoo_context*) malloc(sizeof(struct yahoo_context));
  if ( context == NULL ) {
    return 0;
  }

  memset(context->user, 0, 255);
  memset(context->password, 0, 255);

  strcpy(context->user, options.current_user.latin1() );
  strcpy(context->password, options.current_pass.latin1() );

  setStatus("Initializing yahoolib...");
  
  return 1;
}

int KYahoo::connect()
{
  int res = 0;

  if( context )
    return 0;

  if(!initialize())
    return 0;

  packet_handler = new packet();

  setStatus(i18n("Connecting"));

  // temp debugging level
  //  yahoo_set_log_level(YAHOO_LOG_DEBUG);
  yahoo_set_log_level(YAHOO_LOG_NOTICE);

  context->id = yahoo_init(context->user, context->password);
  if ( context->id <= 0 ) {
    debugmsg("error calling yahoo_init %d", context->id );
    return 0;
  }
  
  yahoo_login( context->id,options.startup_status );

  return( res );
}


void KYahoo::connected()
{
  enableMenus();
  
  // let main widget know
  imstate->connected();
  
  // update statusbar
  setStatus(i18n("Logged in."));
  setStatusUser(options.current_user);

  setCaption( options.current_user );

  isConnected=true;
}


int KYahoo::disconnect()
{

  // weren't ever logged in
  if ( !context )
    return 0;

  // let main widget know
  imstate->disconnected();

  /* quit the user interface, log out of yahoo */
  yahoo_logoff( context->id );

  delete packet_handler;
  
  free(context);
  
  // make sure we know we aren't logged in anymore
  context = NULL;

  disableMenus();

  // update status bar
  setStatus(i18n("Not logged in."));

  setCaption( "" );

  isConnected=false;

  return 1;
}


void KYahoo::fastDisconnect()
{

  // weren't ever logged in
  if ( !context )
    return;

  // let main widget know
  imstate->disconnected();

  delete packet_handler;

  // make sure we know we aren't logged in anymore
  free(context);
  context = NULL;

  disableMenus();

  // update status bar
  setStatus(i18n("Not logged in."));

  isConnected=false;
}


void KYahoo::initialize_option_defaults()
{
  options.msg_friend_only = false; 
  options.show_online_friends_only = false;
  options.login_at_startup = true; 
  options.resize_window = true;
  options.use_emoticons = true;

  options.disable_mail_warning = false;
  options.show_status_time = false;
  options.logoff_close_windows = false;

  options.allow_idle = false;
  options.idle_time = 15;

  options.autoraise_window = true;

  options.startup_status = YAHOO_STATUS_AVAILABLE;
  
  /*
    default constructor for QString sets these NULL
    options.current_user = NULL;	
    options.current_pass = NULL;	
  */

  /* Yahoolib - Proxy mode */
  options.connect_mode = 0;

  options.connect_retries = 1;
  options.connect_delay = 1;
  /*  options.proxy_host = NULL; */
  options.proxy_port = 80;

  /* sound defaults */
  options.buddy_logon = KGlobal::dirs()->findResource("data", 
						      BUDDY_LOGON_SOUND);
  options.buddy_logoff = KGlobal::dirs()->findResource("data",
						       BUDDY_LOGOFF_SOUND);
  options.new_message = KGlobal::dirs()->findResource("data", 
						      NEW_MESSAGE_SOUND);
  options.buzz = KGlobal::dirs()->findResource("data", BUZZ_SOUND);
  
  options.play_buddy_logon = true;
  options.play_buddy_logoff = true;
  options.play_new_message = true;

}


void KYahoo::setStatus( const QString& newstatus )
{
  if ( statusBar() )
    statusBar()->changeItem(newstatus, STATUS_MSG);
}


void KYahoo::setStatusUser( const QString& user )
{
  if ( statusBar() )
    statusBar()->changeItem(user, STATUS_USER);
}


void KYahoo::disableMenus( void )
{
  yahoomenu->setItemEnabled(ID_YAHOO_NEW_CONFERENCE, false); 
  yahoomenu->setItemEnabled(ID_YAHOO_NEW_CHAT, false); 
  yahoomenu->setItemEnabled(ID_YAHOO_OFFLINE_MSG, false);
  yahoomenu->setItemEnabled(ID_YAHOO_LOGOFF, false); 
  yahoomenu->setItemEnabled(ID_YAHOO_LOGON, true); 
  yahoomenu->setItemEnabled(ID_YAHOO_ADD_USER, false);
  yahoomenu->setItemEnabled( ID_YAHOO_REMOVE_USER, false);

  filemenu->setItemEnabled(ID_FILE_REFRESH, false);
  filemenu->setItemEnabled(ID_FILE_OPTIONS, true);
  
  sysTray->disableMenus();
}


void KYahoo::enableMenus( void )
{
  yahoomenu->setItemEnabled(ID_YAHOO_NEW_CONFERENCE, true); 
  yahoomenu->setItemEnabled(ID_YAHOO_NEW_CHAT, true); 
  yahoomenu->setItemEnabled(ID_YAHOO_OFFLINE_MSG, true);
  yahoomenu->setItemEnabled(ID_YAHOO_LOGOFF, true); 
  yahoomenu->setItemEnabled(ID_YAHOO_LOGON, false); 
  yahoomenu->setItemEnabled(ID_YAHOO_ADD_USER, true);
  yahoomenu->setItemEnabled( ID_YAHOO_REMOVE_USER, true);

  filemenu->setItemEnabled(ID_FILE_REFRESH, true);

  sysTray->enableMenus();
}


void KYahoo::slotRefreshAll()
{
  refreshAll();
}


void KYahoo::refreshAll()
{
  /* resize windows */
  refresh(true);
}

/* 
   resize to pretty defaults
   called by process_packet and connect
*/
void KYahoo::refresh(bool force_resize)
{
  if ( (options.resize_window == false) &&
       (force_resize == false) )
    return;

  debugmsg("Refresh() called");

#ifdef BUDDYLIST

  // force buddylist to resize -- avoids the phantom resize event
  // you'd think resizing the top-level widget would force the child
  // widgets to resize, but i guess not.
  //  buddylist->updateGeometry(); // make sure sizeHint() up to date
  if ( this->sizeHint().width() < buddylist->sizeHint().width() )
    {
      buddylist->resize( buddylist->sizeHint() );
      buddylist->repaint();

      debugmsg("main window refresh -- calling resize, width = %d, total height = %d",  buddylist->width(),(buddylist->height() + menuBar()->height() + statusBar()->height()));
      this->resize( buddylist->width(), 
		(buddylist->height() + menuBar()->height() 
		 + statusBar()->height()) + toolBar()->height() );
    }
  else
    {
      debugmsg("refresh: not resizing buddylist");
      this->resize( buddylist->width(), 
		    (buddylist->height() + menuBar()->height() 
		     + statusBar()->height()) );
    }
#endif
}


int KYahoo::readConfig( void )
{
  int flag = 0;

  config->setGroup(QString("Connection Options"));
  options.msg_friend_only = 
    config->readBoolEntry(QString("Accept Messages From Friends Only"),
			  options.msg_friend_only); 
  options.show_online_friends_only = 
    config->readBoolEntry(QString("Show Online Friends Only"),
			  options.show_online_friends_only );
  options.login_at_startup =
    config->readBoolEntry("Login At Startup",
			  options.login_at_startup );

  options.resize_window =
    config->readBoolEntry("Dynamically Resize Buddylist",
			  options.resize_window );

  options.use_emoticons =
    config->readBoolEntry("Use Graphical Emoticons",
			   options.use_emoticons );
			
  options.disable_mail_warning =
    config->readBoolEntry("Disable Mail Warning",
			   options.disable_mail_warning );

  options.show_status_time =
    config->readBoolEntry("Show Status Time",
			   options.show_status_time);

  options.logoff_close_windows =
    config->readBoolEntry("Close All Windows on Logoff",
			   options.logoff_close_windows);  

  options.recd_path=config->readEntry("Recd files path",options.recd_path);
  options.allow_idle = 
    config->readBoolEntry("Allow Idle", options.allow_idle );
  options.idle_time =
    config->readNumEntry("Idle Time", options.idle_time );

  options.autoraise_window = config->readBoolEntry("Autoraise Window", 
						   options.autoraise_window);

  options.startup_status = config->readNumEntry("Startup Status", 
						options.startup_status );

  if( config->readEntry("User", "").isEmpty() )
    flag++;
  else
    options.current_user = config->readEntry("User", "");

  if ( config->readEntry("Password", "").isEmpty() )
    flag++;
  else
    options.current_pass = config->readEntry("Password", "");

  options.connect_mode = config->readNumEntry("Connect Mode", 0);

  options.connect_retries = config->readNumEntry("Number of Retries", 1);
  options.connect_delay = config->readNumEntry("Delay between Retries", 1);
  
  options.proxy_port = config->readNumEntry("Proxy Port", 80);
  options.proxy_host = config->readEntry("Proxy Host", "" );

  options.buddy_logon = config->readEntry("Buddy Logon Sound", 
					  options.buddy_logon );
  options.play_buddy_logon = config->readBoolEntry("Play Buddy Logon Sound",
			  options.play_buddy_logon );
  options.buddy_logoff = config->readEntry("Buddy Logoff Sound",
					   options.buddy_logoff );
  options.play_buddy_logoff = config->readBoolEntry("Play Buddy Logoff Sound",
						   options.play_buddy_logoff );
  options.buzz = config->readEntry("Buzz Sound",options.buzz);
  options.play_buzz = config->readBoolEntry("Play Buzz Sound", options.play_buzz);
  options.new_message = config->readEntry("New Message Sound",
					  options.new_message );
  options.play_new_message = config->readBoolEntry("Play New Message Sound",
						   options.play_new_message );
  return flag;

}

int KYahoo::writeConfig( void )
{
  config->setGroup("Connection Options");
  config->writeEntry("Accept Messages From Friends Only", 
		     options.msg_friend_only);
  config->writeEntry("Show Online Friends Only",
		     options.show_online_friends_only );
  config->writeEntry("Login At Startup",
		     options.login_at_startup );
  config->writeEntry("Dynamically Resize Buddylist",
		     options.resize_window );
  config->writeEntry("Use Graphical Emoticons",
		     options.use_emoticons);
  config->writeEntry("Disable Mail Warning",
		     options.disable_mail_warning);
  config->writeEntry("Show Status Time",
			   options.show_status_time);
  config->writeEntry("Close All Windows on Logoff",
		     options.logoff_close_windows);
  config->writeEntry("Recd files path",options.recd_path);
  config->writeEntry("Allow Idle", options.allow_idle );
  config->writeEntry("Idle Time", options.idle_time );

  config->writeEntry("Autoraise Window", options.autoraise_window);

  config->writeEntry("Startup Status", options.startup_status );
  
  config->writeEntry("User", options.current_user );
  config->writeEntry("Password", options.current_pass );

  config->writeEntry("Connect Mode", options.connect_mode );
  config->writeEntry("Number of Retries", options.connect_retries );
  config->writeEntry("Delay between Retries", options.connect_delay );

  config->writeEntry("Proxy Host", options.proxy_host );
  config->writeEntry("Proxy Port", options.proxy_port );

  config->writeEntry("Buddy Logon Sound", options.buddy_logon);
  config->writeEntry("Buddy Logoff Sound", options.buddy_logoff);
  config->writeEntry("Buzz Sound", options.buzz);
  config->writeEntry("New Message Sound", options.new_message);
  config->writeEntry("Play Buddy Logon Sound", options.play_buddy_logon);
  config->writeEntry("Play Buddy Logoff Sound", options.play_buddy_logoff);
  config->writeEntry("Play Buzz Sound", options.play_buzz);
  config->writeEntry("Play New Message Sound", options.play_new_message);
  
  /* make sure file is saved now */
  config->sync();
  filemenu->setItemEnabled(ID_FILE_SAVE_OPTIONS, false);

  return 0;
}

void KYahoo::showOptions()
{

  // display options dialog box 
  SetupImpl* sd = new SetupImpl(this, "setup menu", true, isConnected);
  int ret = sd->exec();

  if ( ret == QDialog::Accepted ) {
    filemenu->setItemEnabled(ID_FILE_SAVE_OPTIONS, true);
    if (isConnected) refreshAll();
  }

  delete sd;
}

void KYahoo::slotShowStatusBar()
{
  if ( viewmenu->isItemChecked( ID_VIEW_STATUS)  == true )
    {
      viewmenu->setItemChecked( ID_VIEW_STATUS, false );
      sysTray->setItemChecked( ID_SYSTRAY_VIEW_STATUS, false);
      statusBar()->setEnabled(false);
      statusBar()->hide();
    }
  else
    {
      viewmenu->setItemChecked( ID_VIEW_STATUS, true  );
      sysTray->setItemChecked( ID_SYSTRAY_VIEW_STATUS, true);
      statusBar()->setEnabled(true);
      statusBar()->show();
    }

}

void KYahoo::slotShowMenuBar()
{
  if ( viewmenu->isItemChecked( ID_VIEW_MENU)  == true )
    {
      viewmenu->setItemChecked( ID_VIEW_MENU, false );
      sysTray->setItemChecked( ID_SYSTRAY_VIEW_MENU, false);
      menuBar()->setEnabled(false);
      menuBar()->hide();
    }
  else
    {
      viewmenu->setItemChecked( ID_VIEW_MENU, true  );
      sysTray->setItemChecked( ID_SYSTRAY_VIEW_MENU, true);
      menuBar()->setEnabled(true);
      menuBar()->show();
    }

}

void KYahoo::slotShowToolBar()
{
  if ( viewmenu->isItemChecked( ID_VIEW_TOOLBAR)  == true )
    {
      viewmenu->setItemChecked( ID_VIEW_TOOLBAR, false );
      sysTray->setItemChecked( ID_SYSTRAY_VIEW_TOOLBAR, false);
      toolBar()->setEnabled(false);
      toolBar()->hide();
    }
  else
    {
      viewmenu->setItemChecked( ID_VIEW_TOOLBAR, true  );
      sysTray->setItemChecked( ID_SYSTRAY_VIEW_TOOLBAR, true);
      toolBar()->setEnabled(true);
      toolBar()->show();
    }

}

void KYahoo::infomsg( const QString& msg )
{
  QMessageBox::information ( 0, "KYIM", msg, QMessageBox::Ok);
}

void KYahoo::errormsg( const QString& msg )
{
  QMessageBox::critical( 0, "KYIM", msg );
}

void KYahoo::warnmsg( const QString& msg )
{
   QMessageBox::warning( 0, "KYIM", msg );
}

void KYahoo::slotShowDebug()
{
  Dbg_Dialog = new Debug_Msg_Dialog();
  Ptr_Dbg_Dialog = Dbg_Dialog; 
  Dbg_Dialog->show();
}

void KYahoo::debugmsg( const char* fmt, ... )
{
  va_list args;
  va_start( args, fmt);
  
  if ( this && Ptr_Dbg_Dialog )
    if ( fmt )
      {
	char s[200];
	vsnprintf( s, 200, fmt, args );
	Dbg_Dialog->appendmsg(s);
      }

  // use this to send output to stdout as well
#if 0
  vprintf(fmt, args);
  printf("\n");
#endif

  va_end( args );
} /* debugmsg */


QString KYahoo::getRecdPath()
{
  return options.recd_path;

}


void KYahoo::socket_timeout()
{
  if ( packet_handler )
    packet_handler->timer_callback();
}

void KYahoo::play_sound( enum sound_event evt )
{
  switch (evt) 
    {
    case BUDDY_LOGON:
      if ( options.play_buddy_logon )
	{
	  debugmsg("play_sound: event = BUDDY_LOGON, file = %s", 
		   (const char *)options.buddy_logon);
	  KAudioPlayer::play(options.buddy_logon);
	}
      break;
    case BUDDY_LOGOFF:
      if ( options.play_buddy_logoff )
	{
	  debugmsg("play_sound: event = BUDDY_LOGOFF, file = %s", 
		   (const char *)options.buddy_logoff);
	  KAudioPlayer::play(options.buddy_logoff);
	}
      break;
    case BUZZ:
      if (options.play_buzz )
	{
	  debugmsg("play_sound: event = BUZZ, file=%s",
		   (const char*)options.buzz);
	  KAudioPlayer::play(options.buzz);
	}
      break;
    case NEW_MESSAGE:
      if ( options.play_new_message )
	{
	  debugmsg("play_sound: event = NEW_MESSAGE, file = %s", 
		   (const char*)options.new_message);
	  KAudioPlayer::play(options.new_message);
	}
      break;
    default:
      debugmsg("play_sound: unknown event %d", evt);
    }

} /* play_sound() */
 


void KYahoo::newmail(char *from, char *subj, int cnt)
{
  //
  //TODO: modify new mail dialog to display this
  //
  (void)from;
  (void)subj;
  (void)cnt;

  if (!options.disable_mail_warning) {
    NewMailImpl* newmail = new NewMailImpl(this);
    newmail->show();
  }
}



void KYahoo::addedMessage(QString uid, QString msg){
	AddedMessageImpl* addedmessage = new AddedMessageImpl(uid,msg,this);
	addedmessage->show();
}


void KYahoo::slotFindFriends(){
  QString URLFindFriends=QString("http://search.profiles.yahoo.com/");
  KRun::runURL(KURL(URLFindFriends),QString("text/html"));
}


void KYahoo::slotGoToMail(){
  QString URLProfile=QString("http://mail.yahoo.com/");
  KRun::runURL(KURL(URLProfile),QString("text/html"));
}

