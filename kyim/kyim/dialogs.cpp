/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <stdlib.h> /* for rand() */
#include <qdialog.h>
#include <qmultilinedit.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qpoint.h>
#include <qfiledlg.h>
#include <kfiledialog.h>
#include <klocale.h>

extern "C" {
#include "yahoo2.h"
#include "yahoo_list.h"
}
#include "dialogs.h"
#include "kyim.h"
#include "IMState.h"

extern KYahoo* main_window;
extern struct yahoo_context *context;
extern IMState* imstate; 

static int conf_id = 1;

/* start a new conference dialog */
Conference_Invite_Window::Conference_Invite_Window()
  : QDialog( 0, "New Conference Dialog", false /* not modal */ )
{

  // set the window title to this.
  this->setCaption( i18n("KYIM: Start New Conference") );
  this->resize( 500, 300 );
  
  // conference type, only allow text chats for now
  type = 0;

  // holds the user list
  userlist = new QStrList();

  QVBoxLayout *vlayout = new QVBoxLayout( this, 5, 5 );
  QVBoxLayout *vlayout1 = new QVBoxLayout(); 
  QVBoxLayout *vlayout2 = new QVBoxLayout(); 
  QHBoxLayout *hlayout1 = new QHBoxLayout();
  QHBoxLayout *hlayout2 = new QHBoxLayout();

  vlayout->addLayout( hlayout1, 1 ); 
  hlayout1->addLayout( vlayout1 ); 
  hlayout1->addLayout( vlayout2 ); 

  QLabel* title2a = new QLabel(i18n("Currently Online Buddies:"), this);
  title2a->setFixedHeight( title2a->sizeHint().height() );
  vlayout1->addWidget( title2a );

  buddies = new QListView( this );  // list of buddies online now
  buddies->addColumn(i18n("Name"));
  QListViewItem* item = NULL; 

  QStringList list =  imstate->BuddiesOnline(); 
  
  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
    main_window->debugmsg("adding %s to list", (*it).latin1() );
    item = new QListViewItem(buddies, (*it).latin1());
    item->setOpen( true );      
  }
  QObject::connect( buddies, SIGNAL( doubleClicked(QListViewItem *)),
		    this, SLOT( name_clicked(QListViewItem *)));
  buddies->resize( buddies->sizeHint());
  buddies->show();
  vlayout1->addWidget( buddies, 1 );

  QLabel* title2 = new QLabel(i18n("Additional users:"), this);
  title2->setFixedHeight( title2->sizeHint().height() ); 
  vlayout1->addWidget( title2 );

  invite_line = new QLineEdit( this );
  invite_line->setFixedHeight( invite_line->sizeHint().height() );
  invite_line->setFocus();
  QObject::connect( invite_line, SIGNAL(returnPressed()),
		    this, SLOT( add_invite() ) );
  vlayout1->addWidget( invite_line );

  QPushButton* add_invite_button = new QPushButton(i18n("Add to invitation"), this);
  add_invite_button->setFixedSize( add_invite_button->sizeHint() );
  QObject::connect( add_invite_button, SIGNAL(clicked()),
		    this, SLOT( add_invite() ) );
  vlayout1->addWidget( add_invite_button );

  QLabel* title1 = new QLabel( i18n("Invited Users:"), this );
  title1->setFixedHeight( title1->sizeHint().height() ); 
  vlayout2->addWidget( title1 );

  invited_list = new QMultiLineEdit( this );
  invited_list->setReadOnly( true );
  vlayout2->addWidget( invited_list, 1 );

  QLabel* title3 = new QLabel(i18n("Invitation message:"), this );
  title3->setFixedHeight( title2->sizeHint().height() );
  vlayout->addWidget( title3 );

  msg_line = new QLineEdit( this );
  msg_line->setFixedHeight( msg_line->sizeHint().height() );
  msg_line->setText(i18n("Join My Conference...!"));
  vlayout->addWidget( msg_line );

  vlayout->addLayout( hlayout2 );

  hlayout2->addStretch(); 
  QPushButton* done_button = new QPushButton(i18n("Done"), this);
  done_button->setFixedSize( done_button->sizeHint() );
  QObject::connect( done_button, SIGNAL( clicked() ),
		    this, SLOT( done_clicked() ) );
  hlayout2->addWidget( done_button );
  
  //  hlayout2->addStretch(); 

  QPushButton* cancel_button = new QPushButton(i18n("Cancel"), this);
  cancel_button->setFixedSize( cancel_button->sizeHint() );
  QObject::connect( cancel_button, SIGNAL( clicked() ),
		    this, SLOT( reject() ) );  
  hlayout2->addWidget( cancel_button );
  //  hlayout2->addStretch(); 

  vlayout->activate();
  
  buddies->setColumnWidth( 0, buddies->width() );
}

void Conference_Invite_Window::add_invite()
{
  char* user = strdup(invite_line->text());
  //  main_window->debugmsg("entered_text length = %d", s.length());
  //  main_window->debugmsg("chat_text width = %d", chat_text->width());
  
  // need to clear entered_text and append to msg window
  if ( user )
    {
      invited_list->append( user );
      invited_list->setCursorPosition( invited_list->numLines(), 0);

      userlist->append( user ); 

    }

  invite_line->clear();
  free(user);

} /* add_invite_clicked() */


void Conference_Invite_Window::done_clicked()
{
  unsigned int i = 0;
  YList* inviteList = 0; 
  QString confId; 
  
  for( i = 0; i < userlist->count(); i++ )
    {
      inviteList = y_list_append(inviteList, strdup( userlist->at(i)) );
    }
  confId.sprintf("%s-%.3d",  (const char*)main_window->current_user(), 
		 conf_id++ );

  yahoo_conference_invite(context->id, NULL,
			  inviteList, confId, msg_line->text() );

  // add yourself to list
  inviteList = y_list_append(inviteList, 
			     strdup((const char*)main_window->current_user()));

  imstate->start_conf( confId, inviteList ); 
  // conference session free's the inviteList

  this->accept();

} /* done_clicked() */


void Conference_Invite_Window::name_clicked(QListViewItem *item)
{
  //
  // make sure not a flaky signal
  //
  // if parent is null, then this is a group heading, not 
  // a buddy
  //
  if ( item != NULL )  
    {
      char *user = strdup( item->text(0));
      main_window->debugmsg ( "%s was double clicked", user);

      if ( user )
	{
	  invited_list->append( user );
	  invited_list->setCursorPosition( invited_list->numLines(), 0);
	  userlist->append( user ); 
	  
	  delete item; 
	  
	}
    }

} /* name_clicked() */


/****************************************************************************
 *
 * Debug_Msg_Dialog
 *
 *
 ***************************************************************************/
Debug_Msg_Dialog::Debug_Msg_Dialog()
  : QDialog( 0, "Debug Messages Dialog", false /* not modal */ )
{
  // set the window title to this.
  this->setCaption( i18n("KYIM: Debug Messages") );
  this->resize(500, 150);
 
  QVBoxLayout* vlayout = new QVBoxLayout(this);

  debug_msgs = new QMultiLineEdit( this );
  vlayout->addWidget( debug_msgs  );

  vlayout->activate();

}

void Debug_Msg_Dialog::closeEvent( QCloseEvent *e )
{
  main_window->Ptr_Dbg_Dialog = NULL; 
  e->accept();                            // hides the widget
}

void Debug_Msg_Dialog::appendmsg( char *msg )
{
  if ( !debug_msgs )
    return;

  if ( !msg )
    return;

  debug_msgs->append( msg );
  debug_msgs->setCursorPosition( debug_msgs->numLines(), 0);

}

