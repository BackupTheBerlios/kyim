/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ConferenceWindowImpl.h"

#include <qlineedit.h>
#include <qmultilinedit.h>
#include <ktextbrowser.h>
#include <qdatetime.h>
#include <qevent.h>
#include <qlistbox.h>
#include <klocale.h>

#include "Parse.h"
#include "kyim.h"

extern KYahoo* main_window;

/*
 *  Constructs a ConferenceWindowImpl which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
ConferenceWindowImpl::ConferenceWindowImpl( const char* conf_id, QWidget* parent,  const char* name, WFlags fl )
    : ConferenceWindow( parent, name, fl )
{
  confId = conf_id;

  /* put yourself in window */
  //  userList->insertItem( main_window->current_user() );

  // message parser
  parse = new MsgParser();

  //fill the actions list
  fillActionList();
  me = main_window->current_user();

  // clear log
  _textLog.truncate(0);

}

/*
 *  Destroys the object and frees any allocated resources
 */
ConferenceWindowImpl::~ConferenceWindowImpl()
{
    // no need to delete child widgets, Qt does it all for us

  delete parse;
}

void ConferenceWindowImpl::fillActionList()
{
  actionsList->insertItem("Yahoo!");
  actionsList->insertItem("Matrix");
  actionsList->insertItem("Source");
}

void ConferenceWindowImpl::appendMessage( const char *msg, const char* from )
{
  if ( from && msg )
    {
      QString newText(msg);
      QString newLog(msg);

      // log file
      parse->formatLog( newLog );
      _textLog += "[" + QString(from) + " " + QTime::currentTime().toString()+ "] " + newLog + "\n";

      // gui
      parse->formatMsg( newText);

      newText.insert( 0,  "<b><font color=\"blue\">[" + QString(from) + " "
			 + QTime::currentTime().toString()
			 + "]</font></b> ");

      text->append( newText );
      text->scrollToBottom();
    }
}

void ConferenceWindowImpl::addUser( QString& user )
{
  QListBoxItem *tmp;

  tmp = userList->findItem ( user, Qt::ExactMatch);
  if ( !tmp ) {
    userList->insertItem( user );
    userList->sort();
  }
}

void ConferenceWindowImpl::removeUser( QString& user )
{
  QListBoxItem *tmp;

  tmp = userList->findItem ( user, Qt::ExactMatch);
  if ( tmp ) {
    delete tmp;
  }

  userList->sort();
}


QStringList ConferenceWindowImpl::getUserList( void )
{
  QStringList t;

  return t;
}



/*
 * public slot
 */
void ConferenceWindowImpl::newMessage()
{
  if (! enteredText->text().stripWhiteSpace().isEmpty() && ! lbreak)
  {
	QString newText( enteredText->text() );

	// make sure not an empty message
	if ( newText.isEmpty() )
	return;

	// need to clear enteredText and append to msg window

	// notify the chat class
	emit sendMessage( newText );

	// add to text log
	_textLog += "[" + me + " " + QTime::currentTime().toString()+ "] " + newText + "\n";

	parse->formatReply( newText );
	newText.insert ( 0, "<b>[" + me + " "
			+ QTime::currentTime().toString()
			+ "]</b> ");

	text->append( newText );
	text->scrollToBottom();

	enteredText->clear();
   }
}

void ConferenceWindowImpl::textPressed()
{
  if (! enteredText->text().stripWhiteSpace().isEmpty() && ! lbreak)
	enteredText->doKeyboardAction(QTextEdit::ActionBackspace);

  if (enteredText->text().stripWhiteSpace().isEmpty() && ! lbreak)
	enteredText->clear();

	newMessage();
}

/*
 * public slot
 */
void ConferenceWindowImpl::invitePressed()
{
  emit( inviteUser() );
}

/*
 * public slot
 */
void ConferenceWindowImpl::quitPressed()
{
  // notify the conference chat class
  emit quitChat();
}

void ConferenceWindowImpl::copy()
{
  if (enteredText->hasFocus())
    enteredText->copy();
  else
    text->copy();
}

void ConferenceWindowImpl::cut()
{
  if (enteredText->hasFocus())
    enteredText->cut();
}

void ConferenceWindowImpl::paste()
{
  if (enteredText->hasFocus())
    enteredText->paste();
}

void ConferenceWindowImpl::showEvent ( QShowEvent *e )
{

  text->setText( text->text());
  text->ensureVisible( 0, text->contentsHeight(), 5, 5 );
  text->viewport()->repaint(true);

  ConferenceWindow::showEvent( e );
}

/*
 * This function is exactly the same as newMessage, for now!
 * In future, once the reverse parsing has been implemented
 * we can "write" the action msg in a diff color n attributes
 */
void ConferenceWindowImpl::sendAction(QString newText)
{
  // make sure not an empty message
  if ( newText.isEmpty() )
    return;

  // need to clear enteredText and append to msg window

  // notify the chat class
  emit sendMessage( newText );

  // add to text log
  _textLog += "[" + me + " " + QTime::currentTime().toString()+ "] " + newText + "\n";

  parse->formatReply( newText );
  newText.insert ( 0, "<b>[" + me + " "
		   + QTime::currentTime().toString()
		   + "]</b> ");

  text->append( newText );
  text->scrollToBottom();

//  enteredText->clear();


}

/*
 * On an item on action list being clicked, send the corresponding
 * Action msg
 */

void ConferenceWindowImpl::actionClicked(QListBoxItem* item)
{
	//num - index of the clicked item
	int num	= actionsList->index(item);
	QString str;

	// instead of this hardcoded switch statement,
	// a char arr should be used, but will QString be reqd
	// for i18n stuff?? ->Ask jay/alex
	// i18n OK - Alex
	switch (num){
		case 0:str=i18n("Yahooo's n crashes into the ground..yes he was flyin!! ");
						 break;
		case 1:str=i18n("the Matrix has YOU!");
						 break;
		case 2:str=i18n("May the SOURCE be with you!");
		       break;

	}

/*	enteredText->setText(str);
	newMessage();*/

	sendAction(str);
}

void ConferenceWindowImpl::keyPressEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_Shift)
		lbreak = true;
}

void ConferenceWindowImpl::keyReleaseEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_Shift)
		lbreak = false;
}

bool ConferenceWindowImpl::findUser( const QString& user )
{


}

