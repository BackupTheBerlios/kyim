/*+*************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef chatbase_h
#define chatbase_h

#include <kmainwindow.h>


/**
   @short GUI Element Management class for chat and conference windows
*/
class ChatBase : public KMainWindow
{
Q_OBJECT
  
public:
  /**    
     Creates a traditional chat session/window.
     @param myid Current user's Yahoo ID
     @param tobuddy Yahoo ID of person on other end
     @param parent pointer to parent widget
     @param name Object name
  */
 ChatBase( QWidget* parent = 0, const char *name = 0 );
 ~ChatBase() {}
  
 /**
    A new message has arrive from network and needs to be displayed in
    a chat window.
    @param msg The message text
 */
 virtual void messageIn( const QString& msg ) = 0;

 /**
    A new message has arrive from network and needs to be displayed in
    a chat window.	
    @param msg The message text
    @param who The buddy Id of the person who sent the message
 */
 virtual void messageIn( const QString& msg, const QString& who ) = 0;

 /**
    A new message has been typed by the user and needs to be sent out
    the network. The chat window has already been updated by the Chat 
    GUI widget. This public method is meant to provide access from the dcop 
    interface for sending messages.
    @param msg The message text
 */
 virtual void messageOut( const QString& msg ) = 0;

 /**
    Return the unformatted text of the current window
 */
 virtual QString text() = 0;

 /** 
    Return the formatted text of the current window
 */
 virtual QString HTML() = 0;

 //
 // Returns true if user is in chat session or conference
 //
 virtual bool findUser( const QString& user ) = 0;

public slots:
 virtual void copy() = 0;
 virtual void cut() = 0;
 virtual void paste() = 0;

protected slots:
  /** 
    A new message has been typed by the user and needs to be sent out
    the network. This method picks up the signal sent by the Chat GUI element.
  */  
  virtual void typedMessage( QString msg ) = 0;

  virtual void quit() = 0; 

  // Used to save a log of conversation
  virtual void saveText();
  virtual void saveHTML();

protected:
 virtual void initMenuBar();

 /**
    Access function to reset the idle timer
 */
 void resetIdle();

private:

};


#endif
