
#include <time.h>

#include <qstringlist.h>
#include <kcombobox.h>
#include <qlistview.h>
#include <klocale.h>
#include <qtimer.h>
#include <qtable.h>
#include <qstring.h>
#include <klineeditdlg.h>

extern "C" {
#include "yahoo2.h"
}

#include "IMState.h"
#include "BuddyStatus.h"
#include "kyim.h"
#include "statusIcons.h"
#include "Icon.h"
#include "OfflinesViewImpl.h"
#include "ChatBase.h"
#include "ChatSession.h"
#include "ConferenceSession.h"
#include "ConfInvitationImpl.h"

extern struct yahoo_context* context; 
extern KYahoo* main_window;


enum yahoo_status yahoo_status_codes_index[] = {
  YAHOO_STATUS_AVAILABLE,
  YAHOO_STATUS_BRB,
  YAHOO_STATUS_BUSY,
  YAHOO_STATUS_NOTATHOME,
  YAHOO_STATUS_NOTATDESK,
  YAHOO_STATUS_NOTINOFFICE,
  YAHOO_STATUS_ONPHONE,
  YAHOO_STATUS_ONVACATION,
  YAHOO_STATUS_OUTTOLUNCH,
  YAHOO_STATUS_STEPPEDOUT,
  YAHOO_STATUS_INVISIBLE,
  YAHOO_STATUS_CUSTOM,
  YAHOO_STATUS_IDLE,
  YAHOO_STATUS_OFFLINE,
  (enum yahoo_status)-1 /* end of array */};


// don't display codes after this
static int stopDisplayIndex = 13;


static const char* yahoo_status_codes[] = {
        "I'm Available",
        "Be Right Back",
        "Busy",
        "Not at Home",
        "Not at my Desk",
        "Not in the Office",
        "On the Phone",
        "On Vacation",
        "Out to Lunch",
        "Stepped Out",
        "Invisible",
        "Custom Message",
	"Idle", 
	"Not Available",
	NULL
};


static int getsts_index( int status ) {
  int i =  0; 
  enum yahoo_status sts; 

  do {
    sts = yahoo_status_codes_index[i];
    if ( sts == status ) {
      return i; 
    }
    i++;
  } while ( sts != (enum yahoo_status)-1 );

  return 0;
}


IMState::IMState( QWidget* parent,  const char* name, WFlags fl )
  : BuddyList( parent, name, fl )
{
  int i = 0; 
  const char* ptr = yahoo_status_codes[i];

  /* initialize status code array */
  while ( ptr != NULL ) {
    MyStatus->insertItem(ptr);
    i++;
    if ( i == stopDisplayIndex ) {
      ptr = NULL;
    }
    else {
      ptr = yahoo_status_codes[i];
    }
  }

  // pretty pictures
  awayIcon = QPixmap(away_xpm);
  busyIcon = QPixmap(busy_xpm);
  availIcon = QPixmap(avail_xpm);
  idleIcon = QPixmap(idle_xpm);

  _windowlist = new QAsciiDict<ChatBase>;
  _buddylist = new QAsciiDict<BuddyStatus>;
  _buddylist->setAutoDelete(true);

  // idle timer support
  idle_timer = new QTimer( this, "Idle Timer" );
  QObject::connect( idle_timer, SIGNAL(timeout()),
		    this, SLOT(to_idle()) );
  forced_idle = false; // flag for when idle timer forces idle instead of user
  
}


IMState::~IMState()
{
  closeAllWindows(true);
}


/* 
 * public slot
 */
void IMState::MyStatusChanged( int indx )
{
  if ( context == NULL ) {
    MyStatus->setCurrentItem(0);
    return;
  }

  _mystatus = yahoo_status_codes_index[indx]; 

  // tell libyahoo2 of new status
  if ( _mystatus == YAHOO_STATUS_CUSTOM ) {

    QString msg = KLineEditDlg::getText( i18n( "Custom Status Message"),
      i18n("Enter your status message") );

   if( msg.isEmpty() ) return;
  
    _mycustomstatus = msg;

    MyStatus->setCurrentItem(getsts_index(_mystatus));
    main_window->setStatus(_mycustomstatus);

    yahoo_set_away(context->id, (enum yahoo_status)_mystatus, 
		 (const char*)_mycustomstatus, 1);

  }
  else {
    if (_mystatus != YAHOO_STATUS_AVAILABLE ) {
      yahoo_set_away(context->id,(enum yahoo_status)_mystatus, NULL, 1);
    }
    else {
      yahoo_set_away(context->id,(enum yahoo_status)_mystatus, NULL, 0);
    }
  }

  // set the status bar back to default
  main_window->setStatus(i18n("Logged In."));
}


//
// Set my status to custom message
//
void IMState::MyStatusChanged( QString custom )
{
  if ( context == NULL ) {
    return;
  }

  _mystatus = YAHOO_STATUS_CUSTOM;
  _mycustomstatus = custom;

  MyStatus->setCurrentItem(getsts_index(_mystatus));
  main_window->setStatus(_mycustomstatus);

  yahoo_set_away(context->id, (enum yahoo_status)_mystatus, 
		 (const char*)_mycustomstatus, 1);

}


/* 
 * public slot
 */
void IMState::buddyClicked(QListViewItem* it)
{
  //
  // make sure not a flaky signal
  //
  // if parent is null, then this is a group heading, not 
  // a buddy
  //
  if ( (it != NULL) && (it->parent() != NULL) ) {
    start_chat(it->text(ID_COLUMN));
  }
}

void IMState::sendMessage( const QString& sender, const QString& buddy, const QString& msg )
{
  yahoo_send_im(context->id, sender.latin1(), buddy.latin1(), msg.utf8(), 1 );
}

void IMState::clear_buddyList( void )
{
  QStringList t; 

  // clear listview
  BuddyListView->clear();

  // clear status list
  _buddylist->clear();

  MyStatus->setCurrentItem(0);

  main_window->sysTray->updateBuddyList( t );
}


//
// called from libyahoo2 callbacks
//
void IMState::update_buddyList( YList* buds )
{
  struct yahoo_buddy *tmp_ptr; 
  BuddyStatus* bstatus = NULL; 
  /*  QStringList t; */
  
  // clear old list
  _buddylist->clear();
  clear_buddyList();
  /* main_window->sysTray->updateBuddyList( t ); */

  for(; buds; buds = buds->next) {
    tmp_ptr = (struct yahoo_buddy *)buds->data;
    
    bstatus = new BuddyStatus(tmp_ptr->id, tmp_ptr->real_name,
			      tmp_ptr->group );
    _buddylist->insert( tmp_ptr->id, bstatus);

    // only add list if we show all buddies
    if ( !main_window->options.show_online_friends_only ) {
      add_Buddy( tmp_ptr->id );
    }
  }
}


void IMState::update_ignoreList( YList* lst )
{
  (void)lst;
}


void IMState::update_buddyStatus( char* who, int status, char* msg, int away )
{
  BuddyStatus* bstatus;
  ChatBase* window; 

  (void)away;

  // update private list
  bstatus = fetchBuddy( who );
  if (bstatus) {
    bstatus->status = status;
    bstatus->custStatus = msg;
    bstatus->updateTime();

    // update gui
    set_BuddyStatus( who, status, msg );

    // update any open chat windows
    window = findUser(who, NULL);
    while ( window ) {
      QString s = i18n("Changed status to ");
      if ( status == YAHOO_STATUS_CUSTOM ) {
	s += msg; 
      }
      else {
	s += yahoo_status_codes[getsts_index(status)];
      }
      s += ".";
      window->messageIn(s, who);
      
      window = findUser(who, window);
    }

    main_window->sysTray->updateBuddyList( BuddyStatusList() );
  }
}

  
void IMState::chat_message( const char* who, const char* msg, long tm, int stat, int utf8 )
{
  ChatBase* window; 

  QString message = ( !utf8 ) ? QString::fromLatin1( msg ) :
    QString::fromUtf8( msg );

  main_window->debugmsg("%s: %s", who, msg);

  if ( !tm ) {
    window = findWindow(who);
    
    if ( !window ) {
      // window closed
      start_chat(who);
      window = findWindow(who);
      window->show();
    }
    else {
      if ( main_window->options.autoraise_window ) {
	window->raise();
	window->setActiveWindow();
      }
    }

    window->messageIn( message );
  }
  else {
    // offline message
    char timestr[255];
    OfflinesViewImpl *offlineWindow;

    strncpy(timestr, ctime(&tm), sizeof(timestr));
    timestr[strlen(timestr) - 1] = '\0';
    
    main_window->debugmsg("[Offline message at %s from %s]: %s", 
			  timestr, who, msg);

    offlineWindow = new OfflinesViewImpl ( 0, "Offline Messages", TRUE);
    offlineWindow->messagesTable->setNumRows(1);
    offlineWindow->messagesTable->setText(0, 0, QString(who));
    offlineWindow->messagesTable->setText(0, 1, message);
    offlineWindow->messagesTable->setText(0, 2, QString(timestr));
    offlineWindow->messagesTable->adjustColumn(0);
    offlineWindow->messagesTable->adjustColumn(1);
    offlineWindow->messagesTable->adjustColumn(2);
    
    offlineWindow->exec();
    if (offlineWindow!=NULL)
      delete offlineWindow;
  }
}


void IMState::conf_invite( char* who, char* room, char* msg, YList * members )
{
  ConfInvitationImpl* dialog;
  QString excuse;
  int ret = 0;

  dialog = new ConfInvitationImpl( who, msg, &excuse, NULL, " ", true );

  ret = dialog->exec();
  
  if ( ret == QDialog::Accepted )
    {
      // accepted invitation
      main_window->debugmsg("accepted invite");
      yahoo_conference_logon(context->id, NULL, members, room);

      start_conf( room, members );
    }
  else
    {
      // declined conference
      main_window->debugmsg("declined invite");
      yahoo_conference_decline(context->id, NULL, 
			       members, room, excuse);
    }

  delete dialog; 
}


void IMState::conf_userDecline( char *who, char *room, char *msg )
{
  // notify conference
  ChatBase* tmp;

  tmp = findWindow(room);
  if ( tmp ) {
    QString s; 
    s = who;
    if ( msg ) {
      s += " declined to join and said ";
      s += msg; 
    }
    else {
      s += " declined to join.";
    }
    tmp->messageIn(s);
    ((ConferenceSession*)tmp)->removeUser( who );
  }
  else {
    main_window->debugmsg("can't find room %s", room);
  }
}


void IMState::conf_userJoin( const char *who, const char *room)
{
  // notify conference
  ChatBase* tmp;

  tmp = findWindow(room);

  if ( tmp ) {
    QString s; 
    s = who;
    s += " joined the conference.";

    tmp->messageIn(s);

    ((ConferenceSession*)tmp)->addUser( who );
  }
  else {
    main_window->debugmsg("can't find room %s", room);
  }
}


void IMState::conf_userLeave( char *who, char *room)
{
  // notify conference
  ChatBase* tmp;

  tmp = findWindow(room);

  if ( tmp ) {
    QString s; 
    s = who;
    s += " left the conference.";

    tmp->messageIn(s);

    ((ConferenceSession*)tmp)->removeUser( who );
  }
  else {
    main_window->debugmsg("can't find room %s", room);
  }
}


void IMState::conf_message( char *who, char *room, char *msg)
{
  // notify conference
  ChatBase* tmp;

  // ignore my messages
  if ( !main_window->current_user().compare(who) ) {
    return;
  }

  tmp = findWindow(room);

  if ( tmp ) {
    tmp->messageIn( msg, who );
  }
  else {
    main_window->debugmsg("can't find room %s", room);
  }
}


void IMState::notify_typing( char *who, int stat )
{
  // notify window	
}


void IMState::notify_game(char *who, int stat)
{
  // notify ??
}


void IMState::start_chat( const QString& buddy )
{
  ChatSession *tmp;

  main_window->debugmsg("start_chat %s", (const char*)buddy);

  tmp = new ChatSession( buddy, 0, "Chat Session" );

  _windowlist->insert( buddy, tmp );

  tmp->show();
}


void IMState::start_conf( const QString& confid, YList* list )
{
  ConferenceSession* cw = new ConferenceSession( confid, list );
  cw->show();

  _windowlist->insert( confid, cw );
}


void IMState::closeWindow( QString& id )
{
  main_window->debugmsg("removing window %s", (const char*)id);
  _windowlist->remove(id);
}


BuddyStatus* IMState::fetchBuddy( const char *id )
{
  if ( id )
    return( (*_buddylist)[id] );

  return( NULL );
}


ChatBase* IMState::findWindow( const char* id )
{
  if ( id ) {
    return( (*_windowlist)[id] );
  }
  else {
    return NULL;
  }
}


ChatBase* IMState::findUser( const char* id, ChatBase* start )
{

  /* iterate chat windows */
  QAsciiDictIterator<ChatBase> it( *_windowlist ); // iterator for dict
  
  // fast forward to last checked item
  if ( start ) {
    while ( it.current() ) {
      if ( it.current() == start ) {
	++it; 
	break;
      }
      ++it;
    }
  }

  while ( it.current() ) {
    if ( it.current()->findUser(id) ) {
      return ( it.current() );
    }
    ++it;
  }

  return NULL;
}


ChatBase* IMState::findWindow( QString& id )
{
  return( (*_windowlist)[id] );
}


void IMState::AddNewBuddy( const QString& buddy, const QString& group )
{
  BuddyStatus* bstatus = NULL; 
  bstatus = new BuddyStatus(buddy, 0, group);

  _buddylist->insert( buddy, bstatus);

  if ( !main_window->options.show_online_friends_only ) {
    add_Buddy( buddy );
  }
}


void IMState::RemoveBuddy( const QString& buddy )
{
  remove_Buddy( buddy );

  _buddylist->remove(buddy);

}


//
// Buddylist queries
//
QStringList IMState::Buddies(void)
{
  QStringList list;
  list.clear();

  QAsciiDictIterator< BuddyStatus >iterator (*_buddylist);
  BuddyStatus* bs; 

  while( iterator.current() )
    {
      bs = iterator.current(); 
      list += bs->id; 

      ++iterator; 
    }

  list.sort();
  return( list ); 
}


const QStringList& IMState::Groups(void)
{
  QListViewItemIterator it( BuddyListView );

  // not kept up to date, so rebuild
  _grouplist.clear();

  while ( it.current() != 0 ) {
    if ( it.current()->parent() == NULL ) {
      main_window->debugmsg("groupadd: %s", 
			    (const char *)it.current()->text(ID_COLUMN));
      _grouplist += it.current()->text(ID_COLUMN);
    }
    ++it;
  }
  
   _grouplist.sort();
  return ( _grouplist); 
}


QStringList IMState::BuddiesOnline(void)
{
  QStringList list;
  
  QAsciiDictIterator< BuddyStatus >iterator (*_buddylist);
  BuddyStatus* bs; 

  while( iterator.current() )
    {
      bs = iterator.current(); 
      if ( bs->available() )
	list += bs->id; 

      ++iterator; 
    }

  list.sort();
  return( list ); 
}


QStringList IMState::BuddiesOffline(void)
{
  QStringList list;
  
  QAsciiDictIterator< BuddyStatus >iterator (*_buddylist);
  BuddyStatus* bs; 

  while( iterator.current() )
    {
      bs = iterator.current(); 
      if ( !bs->available() )
	list += bs->id; 

      ++iterator; 
    }

  list.sort();
  return( list ); 
}


QStringList IMState::BuddyStatusList()
{
  QStringList list;

  QAsciiDictIterator< BuddyStatus >iterator (*_buddylist);
  BuddyStatus* bs; 

  while( iterator.current() )
    {
      bs = iterator.current(); 
      if ( bs->available() ) {
	QString line(bs->id); 

	line += " (";
	line += yahoo_status_codes[ getsts_index(bs->status) ];
	if ( main_window->options.show_status_time ) {
	  line += " ";
	  line += bs->loginTime.toString();
	}
	line += ")"; 

	list += line; 
      }

      ++iterator; 
    }

  list.sort();
  return( list );  
}

QString IMState::BuddySelected() const
{
  const QListViewItem* item = BuddyListView->selectedItem();

  if( !item ) return QString::null;

  // do no bother if it's a group
  if( !item->parent() ) {
    return QString::null;
  }

  if( (QListView*)item->parent() == item->listView() ) {
    return QString::null;
  }

  return item->text(ID_COLUMN);
}

void IMState::connected(void)
{
  // start idle timer 
  start_idle();
}


void IMState::disconnected(void)
{
  // stop idle timer 
  stop_idle();

  closeAllWindows(false);

  // clear buddylist
  clear_buddyList();
}


//
// logged out, so delete windows as removed
//
void IMState::closeAllWindows( bool exit )
{
   if (( !main_window->options.logoff_close_windows) && (!exit))
     return;
 

  /* iterate chat windows */
  QAsciiDictIterator<ChatBase> it( *_windowlist ); // iterator for dict
  
  while ( it.current() ) {
    delete it.current(); 
    ++it;
  }
}


//
// private gui functions
//
void IMState::add_Buddy( const QString& buddy )
{
  QListViewItem *gptr, *it;
  QString statusText;
  BuddyStatus* bs;
  
  bs = fetchBuddy(buddy);
  if ( !bs ) {
    main_window->debugmsg("can't find buddy %s", (const char*)buddy);
    return; 
  }

  // build status text
  if ( bs->status == YAHOO_STATUS_CUSTOM ) {
    statusText = bs->custStatus; 
  }
  else { 
    statusText = yahoo_status_codes[getsts_index(bs->status)];
  }

  // tack on time
  if ( main_window->options.show_status_time ) {
    statusText += " ";
    statusText += bs->loginTime.toString();
  }
 
  gptr = BuddyListView->findItem(bs->group, ID_COLUMN, ExactMatch );
  if ( gptr == NULL ) {
    gptr = new QListViewItem( BuddyListView, bs->group);
    gptr->setOpen( true ); // make sure the lists are visible   
  }

  it = new QListViewItem( gptr, buddy, statusText);
  it->setPixmap( ID_COLUMN, getIcon(bs->status) );

}


void IMState::set_BuddyStatus(const QString& buddy, 
			      int status, const QString& message )
{
  QListViewItem* it; 
  QString statusText; 
  BuddyStatus* bs;
  
  bs = fetchBuddy(buddy);
  if ( !bs ) {
    main_window->debugmsg("can't find buddy %s", (const char*)buddy);
    return; 
  }

  // build status text
  if ( status == YAHOO_STATUS_CUSTOM ) {
    statusText = message; 
  }
  else { 
    statusText = yahoo_status_codes[getsts_index(status)];
  }

  // tack on time
  if ( main_window->options.show_status_time ) {
    statusText += " ";
    statusText += bs->loginTime.toString();
  }


  it = BuddyListView->findItem(buddy, ID_COLUMN, ExactMatch);
  if ( it != NULL ) {
    if ( (main_window->options.show_online_friends_only) && 
	 (status == YAHOO_STATUS_OFFLINE) ) {
      remove_Buddy(buddy);
    }
    else {
      it->setText( STATUS_COLUMN, statusText );
      it->setPixmap( ID_COLUMN, getIcon(status) );
    }
  }
  else {
    if ( main_window->options.show_online_friends_only ) {
      if ( status != YAHOO_STATUS_OFFLINE ) {
	add_Buddy( buddy );
      }
    }
    else {
      main_window->debugmsg("unknown buddy %s", (const char*) buddy);
    }
  }
}


void IMState::remove_Buddy( const QString& buddy )
{
  QListViewItem* it; 
  
  it = BuddyListView->findItem(buddy, ID_COLUMN, ExactMatch);

  delete it; 
}


QPixmap& IMState::getIcon( int status )
{
  if ( status == YAHOO_STATUS_AVAILABLE ) {
    return availIcon;
  }
  else if ( status == YAHOO_STATUS_OFFLINE ) {
    return awayIcon;
  }
  else if ( status == YAHOO_STATUS_IDLE) {
    return idleIcon;
  }
  else
    return busyIcon;
}



//
// idle timer support
//
void IMState::to_idle()
{
  if ( main_window->options.allow_idle && 
       (_mystatus == YAHOO_STATUS_AVAILABLE) ) {
    // save status state and set to idle 
    
    // set flag
    forced_idle = true;
    
    idle_save_state = _mystatus;

    MyStatus->setCurrentItem(getsts_index(YAHOO_STATUS_IDLE));
    MyStatusChanged(getsts_index(YAHOO_STATUS_IDLE));
    
    // stop timer for now
    stop_idle();
    
    main_window->debugmsg("to_idle: going idle, saving state as %d", 
			  idle_save_state );
  }
}


void IMState::start_idle()
{
  if ( main_window->options.allow_idle )
    {
      // state idle timer
      main_window->debugmsg("start_idle() timeout %d", 
			    main_window->options.idle_time);

      /* reset idle timer */
      /* stops, restarts with timeout */
      idle_timer->start( 1000*60*main_window->options.idle_time, true );
    }
}


void IMState::reset_idle()
{
  if ( main_window->options.allow_idle )
    {
      main_window->debugmsg("reset_idle()");
      
      // restore normal state if we were in idle
      if ( forced_idle )
	from_idle();
      else {
	// reset idle timer
	idle_timer->changeInterval( 1000*60*main_window->options.idle_time );
      }
    } // options.allow_idle
}


void IMState::from_idle()
{
  if ( main_window->options.allow_idle ) {
    main_window->debugmsg("from_idle: restoring state to %d", idle_save_state );
    
    // reset old state
    // dont' restore state if same as is now or current state 
    //   isn't idle 
    if ( (_mystatus != idle_save_state) &&
	 (_mystatus == YAHOO_STATUS_IDLE) ) {

      MyStatus->setCurrentItem(getsts_index(idle_save_state));
      MyStatusChanged(getsts_index(idle_save_state));
    }
    
    // set flag back to false
    forced_idle = false;
    
    // start idle timer again
    start_idle();
  }
}


void IMState::stop_idle()
{
  if ( main_window->options.allow_idle ) {
    main_window->debugmsg("stop idle");
    
    // stop idle timer 
    idle_timer->stop();
  }
}
