
/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qobject.h>
#include <qasciidict.h>
#include <qstring.h>
#include <qdatetime.h>

extern "C" {
#include "yahoo2.h"
}

#include <qpixmap.h>
#include "BuddyList.h"

#ifndef IMSTATE_H
#define IMSTATE_H

#define ID_COLUMN 0
#define STATUS_COLUMN 1

class BuddyStatus;
class ChatBase; 
class QTimer;

/**
 * IMState
 * 
 * Maintains each individual buddy's status, chat and conference window 
 * status. 
 *
 * @short Messenger state
 *
 */
class IMState : public BuddyList
{
  Q_OBJECT

 public:
  IMState( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~IMState();


  //
  // called from kyim class
  //
  void connected(void);
  void disconnected(void);

  //
  // called from chatbase
  //
  void closeWindow( QString& id );

  //
  // used in start new chat session dialog
  // 
  void start_chat( const QString& buddy );
  void start_conf( const QString& confid, YList* list );

  //	
  // idle timer support
  //	
  void reset_idle();

  //
  // Custom status support
  //
  void MyStatusChanged( QString custom );

  //
  // Called from AddBuddy/RemoveBuddy gui
  //
  void AddNewBuddy( const QString& buddy, const QString& group );
  void RemoveBuddy( const QString& buddy );

  //
  // Buddylist queries
  //
  QStringList Buddies(void);
  const QStringList& Groups(void);
  QStringList BuddiesOnline(void);
  QStringList BuddiesOffline(void);
  QStringList BuddyStatusList(void);
  QString BuddySelected() const;

  //
  // called from libyahoo2 callbacks
  //
  void update_buddyList( YList* );
  void update_ignoreList( YList* );
  void update_buddyStatus( char* who, int status, char* msg, int away );
  void chat_message( const char* who, const char* msg, long tm, int stat, int utf8);

  void conf_invite( char* who, char* room, char* msg, YList * members );
  void conf_userDecline( char *who, char *room, char *msg );
  void conf_userJoin( const char *who, const char *room);
  void conf_userLeave( char *who, char *room);
  void conf_message( char *who, char *room, char *msg);

  void notify_typing( char *who, int stat );
  void notify_game(char *who, int stat);

 public slots:
   // called from gui
   void MyStatusChanged( int indx );
   void buddyClicked(QListViewItem*);

   void sendMessage( const QString& sender, const QString& buddy, const QString& msg );

 private slots:
   void to_idle();

 private:
  BuddyStatus* fetchBuddy( const char *id );
  ChatBase* findUser( const char* id, ChatBase* start );
  ChatBase* findWindow( QString& id );
  ChatBase* findWindow( const char* id );

  // private gui operations
  void add_Buddy( const QString& buddy );
  void remove_Buddy( const QString& buddy );
  void set_BuddyStatus(const QString& buddy, int status, 
		       const QString& message );
  void closeAllWindows( bool exit );
  void clear_buddyList( void );

  // icon information
  QPixmap& getIcon( int status );

  // idle timer
  void from_idle();
  void start_idle();
  void stop_idle();

  //
  // private data
  //
  
  QPixmap idleIcon;
  QPixmap awayIcon;
  QPixmap busyIcon;
  QPixmap availIcon;

  QAsciiDict<ChatBase>* _windowlist; 
  QAsciiDict<BuddyStatus>* _buddylist; 
  QStringList _grouplist;

  int _mystatus;
  QString _mycustomstatus;

  // idle timer
  QTimer *idle_timer;
  int idle_save_state;
  bool forced_idle;

};


#endif
