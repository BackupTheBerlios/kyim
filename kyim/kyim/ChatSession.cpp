/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ChatSession.h"
#include <qpopupmenu.h>
#include <kpopupmenu.h>
#include <kmenubar.h>
#include <qaccel.h>
#include <ktextbrowser.h>
#include <klocale.h>
#include <krun.h>
#include <kiconloader.h>
#include <kapplication.h>
#include <qlineedit.h>
#include <qtimer.h>

extern "C" {
#include "yahoo2.h"
}

#include "ChatWindowImpl.h"
#include "kyim.h"
#include "AddBuddyImpl.h"
#include "emoticons.h"
#include "emoticon.h"
#include "IMState.h"

extern KYahoo* main_window;
extern IMState* imstate;

extern struct yahoo_context *context; // defined in buddylist.cpp


ChatSession::ChatSession( const char* tobuddy,
			  QWidget* parent, const char *name )
  : ChatBase( parent, name )
{
  to = tobuddy;

  initMenuBar();

  // set the window title to reflect chat session
  QString caption = to + " - ";
  caption += i18n("Chat Session");
  setCaption( caption );

  _chatWindow = new ChatWindowImpl( this );
  setCentralWidget(_chatWindow);

  QObject::connect( _chatWindow, SIGNAL(sendMessage(QString )),
		    this, SLOT(typedMessage(QString)) );

  QObject::connect( _chatWindow, SIGNAL(destroyed()),
		    this, SLOT(quit()) );
  resize(400,300);

}

// destr
ChatSession::~ChatSession()
{
  main_window->debugmsg("chatwindow destructor");

  imstate->closeWindow(to);
}

void ChatSession::buzz()
{
  main_window->debugmsg("buzz !");
  int xp = x();
  int yp = y();
  QTime t;

  t.start();
  for ( int i = 32; i > 0; )
  {
    kapp->processEvents();
    if ( t.elapsed() >= 1 )
    {
      int delta = i >> 2;
      int dir = i & 3;
      int dx = ((dir==1)||(dir==2)) ? delta : -delta;
      int dy = (dir<2) ? delta : -delta;
      move( xp+dx, yp+dy );
      t.restart();
      i--;
    }
  }
  move( xp, yp );
}

void ChatSession::quit()
{
  delete this;
}

//
// slot to handle message typed by user,
// signal is from the chat window widget
//
void ChatSession::typedMessage( QString msg )
{
  /* reset idle timer */
  resetIdle();

  imstate->sendMessage( main_window->current_user(), to, msg );
}

/**
   A new message has been typed by the user and needs to be sent out
   the network. The chat window has already been updated by the Chat
   GUI widget. This public method is meant to provide access from the dcop
   interface for sending messages.
*/
void ChatSession::messageOut( const QString& msg )
{
  typedMessage( msg );
}

/**
    A new message has arrive from network and needs to be displayed in
    a chat window.
*/
void ChatSession::messageIn( const QString& msg )
{
  if ( !msg.isEmpty() )
    {
      if ( !isActiveWindow() )
	main_window->play_sound(KYahoo::NEW_MESSAGE);

      if ( msg == "<ding>" || msg == "[ding]" )  {
        QTimer::singleShot( 0, this, SLOT( buzz() ) );
	main_window->play_sound(KYahoo::BUZZ);
      }

      _chatWindow->appendMessage( msg, (const char*)to);
    }
}

// API for conference window,
// stub for chat window
void ChatSession::messageIn( const QString& msg, const QString& from)
{
  if ( !msg.isEmpty() )
    {
      if ( !isActiveWindow() )
	main_window->play_sound(KYahoo::NEW_MESSAGE);

      if ( msg == "<ding>" || msg == "[ding]" )  {
        QTimer::singleShot( 0, this, SLOT( buzz() ) );
	main_window->play_sound(KYahoo::BUZZ);
      }

      _chatWindow->appendMessage( msg, from);
    }
//  messageIn( msg );
}

void ChatSession::copy()
{
  _chatWindow->copy();
}

void ChatSession::cut()
{
  _chatWindow->cut();
}

void ChatSession::paste()
{
  _chatWindow->paste();
}

void ChatSession::profile()
{
  QString URLProfile=QString(yahoo_get_profile_url());
  URLProfile+=to;
  KRun::runURL(KURL(URLProfile),QString("text/html"));
}

void ChatSession::forceDefault()
{
  chatFontMenu->setItemChecked(CHAT_FONT_DEFAULT_ID, !chatFontMenu->
			       isItemChecked(CHAT_FONT_DEFAULT_ID));
  _chatWindow->forceDefaultFont(chatFontMenu->
				isItemChecked(CHAT_FONT_DEFAULT_ID));
}

//Load an image from the given filename and load it into
//QPixmap rp
void ChatSession::loadImage(char filename[100],QPixmap& rp){
  rp.load(filename);
	return;
}

void ChatSession::initMenuBar()
{
  KPopupMenu* buddyMenu = new KPopupMenu(menuBar(), "BuddyMenu");
  chatBuddyMenu = buddyMenu;
  buddyMenu->insertItem( i18n("&Profile"),this,SLOT(profile()),CTRL+Key_P,1);
  buddyMenu->insertItem( i18n("&Add Buddy"),this,SLOT(addbuddy()),CTRL+Key_A,2);
  buddyMenu->insertItem( i18n("Do &Not Warn Me When Buddy Offline"),this,SLOT(toggleWarnOffline()),CTRL+Key_N,CHAT_DONT_WARN_OFFLINE_ID);
  buddyMenu->setItemChecked(CHAT_DONT_WARN_OFFLINE_ID,false);
  warnWhenUserOffline=true;
  buddyMenu->setCheckable(true);
  menuBar()->insertItem( i18n("&Buddy"), buddyMenu);

  KPopupMenu* fontMenu = new KPopupMenu(menuBar(), "FontMenu");
  chatFontMenu=fontMenu;
  fontMenu->insertItem( i18n("&Force Default Font"), this,
		       SLOT(forceDefault()),CTRL+Key_F, CHAT_FONT_DEFAULT_ID);
  fontMenu->setCheckable(true);
  fontMenu->setItemChecked(CHAT_FONT_DEFAULT_ID, false);
  menuBar()->insertItem( i18n("&Font"), fontMenu);

  EmoticonMenu* emoticonMenu = new EmoticonMenu( this, "emoticonMenu" );
  connect( emoticonMenu, SIGNAL( emoticonSelected(int)), SLOT(insertSmiley(int)) );

  QPixmap rp;
  loadImage(DATADIR "/pics/emoticon_smile.png",rp);

  toolBar("chatMain")->insertButton(SmallIcon("editcopy"), 1,SIGNAL(clicked(int)), this, SLOT(copy()), true, i18n("Copy"));
  toolBar("chatMain")->insertButton(SmallIcon("editpaste"), 1,SIGNAL(clicked(int)), this, SLOT(paste()), true, i18n("Paste"));
  toolBar("chatMain")->insertButton(rp, 1, emoticonMenu, true, i18n("Emoticons"));

}

/** slot to add a buddy to the contact list */
void ChatSession::addbuddy(){
	AddBuddyImpl* addbud = new AddBuddyImpl(this);
	addbud->setID(to);
	addbud->show();
}

QString ChatSession::text()
{
  return( _chatWindow->_textLog );
}

QString ChatSession::HTML()
{
  return( _chatWindow->text->text());
}

void ChatSession::insertSmiley(int id)
{
  //printf("Smiley : %d\n",id);
  QString orig=_chatWindow->enteredText->text();

  switch(id){
    case SMILEY_ID:orig.append(STR_SMILEY1);
		  break;
    case TONGUE_ID:orig.append(STR_TONGUE1);
		  break;
    case BLINK_ID:orig.append(STR_BLINK1);
		  break;
    case SURP_ID:orig.append(STR_SURP1);
		  break;
    case SAD_ID:orig.append(STR_SAD1);
		  break;
    case ANGRY_ID:orig.append(STR_ANGRY1);
		  break;
    case BIGSM_ID:orig.append(STR_BIGSM1);
		  break;
    case LAUGH_ID:orig.append(STR_LAUGH1);
		  break;
    case CRY_ID:orig.append(STR_CRY1);
		  break;
    case DEVIL_ID:orig.append(STR_DEVIL3);
		  break;
    case GLASS_ID:orig.append(STR_GLASS1);
		  break;
    case SUNGL_ID:orig.append(STR_SUNGL);
		  break;
    case SARC_ID:orig.append(STR_SARC3);
		  break;
    case QUEST_ID:orig.append(STR_QUEST1);
		  break;
    case LOVE_ID:orig.append(STR_LOVE1);
		  break;
    case ANGEL_ID:orig.append(STR_ANGEL1);
		  break;
    case QUIET_ID:orig.append(STR_QUIET1);
		  break;
    case STRAIGHT_ID:orig.append(STR_STRAIGHT1);
		  break;
    case BLUSH_ID:orig.append(STR_BLUSH3);
		  break;
    case SLEEP_ID:orig.append(STR_SLEEP1);
		  break;
    case BYE_ID:orig.append(STR_BYE1);
		  break;
    case CLOWN_ID:orig.append(STR_CLOWN1);
		  break;
    case PISSED_ID:orig.append(STR_PISSED1);
		  break;
    case YUCK_ID:orig.append(STR_YUCK1);
		  break;
    case KISS_ID:orig.append(STR_KISS1);
                  break;
  }
  _chatWindow->enteredText->setText(orig);

}
/** No descriptions */
void ChatSession::toggleWarnOffline(){
	chatBuddyMenu->setItemChecked(CHAT_DONT_WARN_OFFLINE_ID,
																!chatBuddyMenu->isItemChecked(CHAT_DONT_WARN_OFFLINE_ID));
	warnWhenUserOffline = (!chatBuddyMenu->isItemChecked(CHAT_DONT_WARN_OFFLINE_ID));
}


bool ChatSession::findUser( const QString& user )
{
  if ( user == to ) {
    return true;
  }

  return false;
}
