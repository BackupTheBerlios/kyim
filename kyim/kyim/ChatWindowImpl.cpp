
#include <qdatetime.h>
#include <qlineedit.h>
#include <qevent.h>
#include <ktextbrowser.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <stdio.h>

#include "ChatWindowImpl.h"

#include "kyim.h"
#include "Parse.h"

extern KYahoo* main_window;

/*
 *  Constructs a ChatWindowImpl which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
ChatWindowImpl::ChatWindowImpl( QWidget* parent,
				const char* name, WFlags fl )
  : ChatWindow( parent, name, fl )
{
  // message parser
  parse = new MsgParser();

  me = main_window->current_user();
  enteredText->setFocus();

  // clear log
  _textLog.truncate(0);
  lbreak = false;
}

/*
 *  Destroys the object and frees any allocated resources
 */
ChatWindowImpl::~ChatWindowImpl()
{
    // no need to delete child widgets, Qt does it all for us

  delete parse;
}


void ChatWindowImpl::appendMessage( const QString& msg, const QString& from )
{
  QString newText(msg);
  QString newLog(msg);

  parse->formatLog( newLog );

  _textLog += "[" + from + " " + QTime::currentTime().toString()+ "] " + newLog + "\n";

  parse->formatMsg( newText );

  newText.insert( 0, "<b><font color=\"blue\">[" + from + " "
		  + QTime::currentTime().toString()
		  + "]</font></b> ");

  text->setText( text->text() + newText );
  text->scrollToBottom();
}


/*
 * public slot
 */
void ChatWindowImpl::newMessage()
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

	text->setText( text->text() + newText );
	text->scrollToBottom();

	enteredText->clear();
  }
}

void ChatWindowImpl::textPressed()
{
  if (! enteredText->text().stripWhiteSpace().isEmpty() && ! lbreak)
	enteredText->doKeyboardAction(QTextEdit::ActionBackspace);

   if (enteredText->text().stripWhiteSpace().isEmpty() && ! lbreak)
	enteredText->clear();

	newMessage();
}

void ChatWindowImpl::copy()
{
  if (enteredText->hasFocus())
    enteredText->copy();
  else
    text->copy();
}

void ChatWindowImpl::cut()
{
  if (enteredText->hasFocus())
    enteredText->cut();
}

void ChatWindowImpl::paste()
{
  if (enteredText->hasFocus())
    enteredText->paste();
}

void ChatWindowImpl::forceDefaultFont( bool flag )
{
  main_window->debugmsg("Default font forced ? %d", flag);
  parse->setForceDefaultFont(flag);
}

void ChatWindowImpl::showEvent ( QShowEvent *e )
{

  text->setText( text->text());
  text->ensureVisible( 0, text->contentsHeight(), 5, 5 );
  text->viewport()->repaint(true);

  ChatWindow::showEvent( e );
}

void ChatWindowImpl::keyPressEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_Shift)
		lbreak = true;
}

void ChatWindowImpl::keyReleaseEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_Shift)
		lbreak = false;
}
