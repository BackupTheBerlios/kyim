/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ConferenceSession.h"

#include <stdlib.h>

#include <ktextbrowser.h>
#include <qpopupmenu.h>
#include <kpopupmenu.h>
#include <kmenubar.h>
#include <qaccel.h>
#include <klocale.h>
#include <krun.h>
#include <kiconloader.h>
#include <qlineedit.h>
#include <kapplication.h>

extern "C" {
#include "yahoo2.h"
}

#include "ConfAddInviteImpl.h"
#include "ConferenceWindowImpl.h"
#include "kyim.h"
#include "IMState.h"
#include "emoticon.h"
#include "emoticons.h"

extern KYahoo* main_window;
extern IMState* imstate;
extern struct yahoo_context *context; // defined in buddylist.cpp

ConferenceSession::ConferenceSession( const char* conf_id, YList* list,
				      QWidget* parent, const char *name )
  : ChatBase( parent, name )
{
  
  confId = conf_id;
  YLuserList = list;
  YList * l;

  main_window->debugmsg("ConferenceSession conf_id %s list 0x%08x",
			conf_id, list);

    // set the window title to reflect chat session
  QString caption = QString::fromLatin1(conf_id) + " - ";
  caption += i18n("Conference");
  setCaption( caption );

  _conferenceWindow = new ConferenceWindowImpl(conf_id, this);
  setCentralWidget(_conferenceWindow);

  QObject::connect( _conferenceWindow, SIGNAL(sendMessage(QString)),
		    this, SLOT(typedMessage(QString)) );

  QObject::connect( _conferenceWindow, SIGNAL(quitChat()),
		    this, SLOT(quit()) );

  QObject::connect( _conferenceWindow, SIGNAL(inviteUser()),
		    this, SLOT(inviteUser()) );
  
  initMenuBar();


  for ( l = YLuserList; l; l=l->next ) {
    QString usr((const char*)l->data); 
    main_window->debugmsg("conferencesession list %s", (const char*)l->data);
    _conferenceWindow->addUser(usr);
  }

}


ConferenceSession::~ConferenceSession()
{
  imstate->closeWindow(confId);

  // need to free YLuserList
  
}


void ConferenceSession::quit()
{
  yahoo_conference_logoff( context->id, NULL,
			   YLuserList, confId );

  imstate->closeWindow(confId);

  delete this;   
} 

//
// slot to handle message typed by user,
// signal is from the chat window widget
//
void ConferenceSession::typedMessage( QString msg )
{
  /* reset idle timer */
  resetIdle();
  
  yahoo_conference_message(context->id, NULL,
			   YLuserList, confId, msg.utf8(), 1);

}

/**
   A new message has been typed by the user and needs to be sent out
   the network. The chat window has already been updated by the Chat 
   GUI widget. This public method is meant to provide access from the dcop 
   interface for sending messages.
*/
void ConferenceSession::messageOut( const QString& msg )
{
  typedMessage( msg );
}

/**
    A new message has arrive from network and needs to be displayed in
    a chat window.
*/
void ConferenceSession::messageIn( const QString& msg )
{
  messageIn( msg, "SYSTEM" );
}

// API for conference window,
void ConferenceSession::messageIn( const QString& msg, const QString& who )
{
  if ( !msg.isEmpty() )
    {
      if ( !isActiveWindow() )
	main_window->play_sound(KYahoo::NEW_MESSAGE);

      _conferenceWindow->appendMessage( msg, who);
    }
}


void ConferenceSession::inviteUser( void )
{
  ConfAddInviteImpl* dlg = new ConfAddInviteImpl( (const char*)confId, 
						  YLuserList, this );
  dlg->show();
}

void ConferenceSession::addUser( QString user )
{
  YList * l;

  for ( l = YLuserList; l; l=l->next ) {
    char* usr = (char*)l->data; 
    if ( !strcmp( usr, user ) ) {
      main_window->debugmsg("adduser %s : already in list", 
			    (const char*)user);
      return; // already in list
    }
  }

  YLuserList = y_list_append( YLuserList, strdup(user) );
  _conferenceWindow->addUser(user);
}


void ConferenceSession::removeUser( QString user )
{
  YList * l;

  for ( l = YLuserList; l; l=l->next ) {
    char* usr = (char*)l->data; 
    if ( !strcmp( usr, user ) ) {
      main_window->debugmsg("removeuser %s : found in list", 
			    (const char*)user);
      YLuserList =  y_list_remove_link(YLuserList, l);
      free( l->data );
      free( l );
      break;
    }
  }

  _conferenceWindow->removeUser(user);
}


QStringList ConferenceSession::getUserList( void )
{
  return ( _conferenceWindow->getUserList() );
}


void ConferenceSession::copy()
{
  _conferenceWindow->copy();
}

void ConferenceSession::cut()
{
  _conferenceWindow->cut();
}

void ConferenceSession::paste()
{
  _conferenceWindow->paste();
}

QString ConferenceSession::text()
{
  return ( _conferenceWindow->_textLog );
}

QString ConferenceSession::HTML()
{
  return ( _conferenceWindow->text->text() );
}

void ConferenceSession::insertSmiley(int id)
{
  //printf("Smiley : %d\n",id);
  QString orig=_conferenceWindow->enteredText->text();		

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
  }
  _conferenceWindow->enteredText->setText(orig);
		
}

//Load an image from the given filename and load it into
//QPixmap rp
void ConferenceSession::loadImage(char filename[100],QPixmap& rp){
  rp.load(filename);
	return;
}


/** No descriptions */
void ConferenceSession::initMenuBar()
{
  EmoticonMenu* emoticonMenu = new EmoticonMenu( this, "emoticonMenu" );
  connect( emoticonMenu, SIGNAL( emoticonSelected(int)), SLOT(insertSmiley(int)) );

  QPixmap rp;
  loadImage(DATADIR "/pics/emoticon_smile.png",rp);

  toolBar("confMain")->insertButton(SmallIcon("editcopy"), 1,SIGNAL(clicked(int)), this, SLOT(copy()), true, i18n("Copy"));
  toolBar("confMain")->insertButton(SmallIcon("editpaste"), 1,SIGNAL(clicked(int)), this, SLOT(paste()), true, i18n("Paste"));
  toolBar("chatMain")->insertButton(rp, 1, emoticonMenu, true, i18n("Emoticons"));
}



bool ConferenceSession::findUser( const QString& user )
{
  return ( _conferenceWindow->findUser(user) );
}
