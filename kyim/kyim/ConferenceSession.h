/*+*************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef conferencesession_h
#define conferencesession_h

#include "ChatBase.h"

class ConferenceWindowImpl;

extern "C" {
#include "yahoo_list.h"
}

/**
   @short GUI Element Management class for conference windows
*/
class ConferenceSession : public ChatBase
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
 ConferenceSession( const char* confId, YList* list, 
       QWidget* parent = 0, const char *name = 0 );
 ~ConferenceSession(); 
  
 /**
    A new message has arrive from network and needs to be displayed in
    a chat window.
    @param msg The message text
 */
 virtual void messageIn( const QString& msg );

 /**
    A new message has arrive from network and needs to be displayed in
    a chat window.	
    @param msg The message text
    @param who The buddy Id of the person who sent the message
 */
 virtual void messageIn( const QString& msg, const QString& who );

 /**
    A new message has been typed by the user and needs to be sent out
    the network. The chat window has already been updated by the Chat 
    GUI widget. This public method is meant to provide access from the dcop 
    interface for sending messages.
    @param msg The message text
 */
 virtual void messageOut( const QString& msg );

 //
 // userlist manipulation
 //
 void addUser( QString user );
 void removeUser( QString user );
 QStringList getUserList( void );

 virtual QString text();
 virtual QString HTML();


 //
 // Returns true if user is in chat session or conference
 //
 virtual bool findUser( const QString& user );
 
public slots:
 virtual void copy();
 virtual void cut();
 virtual void paste();

 virtual void quit();
 void insertSmiley(int id);

   
private slots:
  /** 
    A new message has been typed by the user and needs to be sent out
    the network. This method picks up the signal sent by the Chat GUI element.
  */  
  virtual void typedMessage( QString msg );

 void inviteUser( void ); 

private:
 // yahoolib userlist
 YList* YLuserList;

 // person the chat session is with
 QString confId;

 // chat window gui widget
 ConferenceWindowImpl* _conferenceWindow;
 void loadImage(char *,QPixmap &);

protected: // Protected methods
  /** No descriptions */
  virtual void initMenuBar();
};


#endif
