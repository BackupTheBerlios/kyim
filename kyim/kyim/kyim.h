/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef kyim_h
#define kyim_h

#include <kapp.h>
#include <kmainwindow.h>
#include <kaudioplayer.h>
#include <khelpmenu.h>
#include <kaboutdata.h>
#include <qpopupmenu.h>
#include <qdict.h>
#include <qlabel.h>
#include <kstatusbar.h>
#include <qtimer.h>
#include <qstring.h>
#include <ksystemtray.h>
#include <ktoolbar.h>

extern "C" {
#include "yahoo2.h"
}

class packet;
class Icon;
class KYahooIface;
class ChatSession;

#define ID_YAHOO_LOGON 1001
#define ID_YAHOO_LOGOFF 1002
#define ID_YAHOO_NEW_CHAT 1008
#define ID_YAHOO_NEW_CONFERENCE 1003
#define ID_YAHOO_ADD_USER 1004
#define ID_YAHOO_REMOVE_USER 1005
#define ID_YAHOO_DEBUG_MSG 1006
#define ID_YAHOO_OFFLINE_MSG 1007

#define ID_FILE_SAVE_OPTIONS 2004
#define ID_FILE_OPTIONS 2005
#define ID_FILE_REFRESH 2006

#define ID_VIEW_STATUS 3001
#define ID_VIEW_MENU 3002
#define ID_VIEW_TOOLBAR 3003

#define ID_SYSTRAY_VIEW_STATUS 4001
#define ID_SYSTRAY_VIEW_MENU 4002
#define ID_SYSTRAY_VIEW_TOOLBAR 4003

#define STATUS_USER 1
#define STATUS_MSG 2

#define BUDDY_LOGON_SOUND "kyim/sounds/knock.wav"
#define BUDDY_LOGOFF_SOUND "kyim/sounds/door.wav"
#define NEW_MESSAGE_SOUND "kyim/sounds/ding.wav"
#define BUZZ_SOUND "kyim/sounds/buzz.wav"

/**
   This C function checks for initial configuration setup and calls a setup dialog if one does not exist. 
   @short Simple C startup routine
*/
void startup( void );

/*
 * Maintains current connection data, much like old libyahoo context. 
 *
 */
struct yahoo_handler_data {
  int sockfd;	
  int cond; /* enum yahoo_input_condition */
  void *data;
};

struct yahoo_context {
  char user[255];
  char password[255];
  int id;  
  struct yahoo_handler_data handle[3]; 
};


/**
@short structure that maintains all configuration options
*/
struct kyahoo_options {
  bool msg_friend_only; 
  bool show_online_friends_only;
  bool login_at_startup; 
  bool resize_window; // allow automatic resizing of buddylist
  bool use_emoticons;

  bool disable_mail_warning;
  bool show_status_time;
  bool logoff_close_windows;

  bool autoraise_window; 

  int startup_status;
  
  QString current_user;
  QString current_pass;

  //default path for recd files
  QString recd_path;
  bool allow_idle;
  int  idle_time;


  /* Yahoolib - Proxy mode */
  int connect_mode;
  int connect_retries;
  int connect_delay;
  QString proxy_host;
  int proxy_port;

  /* default sound files */
  QString buddy_logon;
  QString buddy_logoff;
  QString new_message;
  QString buzz;
  /* play sound? */
  bool play_buddy_logon;
  bool play_buddy_logoff;
  bool play_buzz;
  bool play_new_message;
};

/**
   @short Main Window GUI Element
*/
class KYahoo : public KMainWindow
{
  Q_OBJECT
  friend class SetupImpl;
  friend class Debug_Msg_Dialog;
  friend class MsgParser;
  friend class IMState;
  friend void startup();

public:
  enum sound_event {
    BUDDY_LOGON,
    BUDDY_LOGOFF,
    NEW_MESSAGE,
    BUZZ
  };

  KYahoo();
  ~KYahoo();

  /**
     @returns true if there is no configuration information
  */
  int call_setup() { return need_setup; }

  // resize to "pretty" default
  void refresh(bool force_resize=false);
  void refreshAll();

  // set status and login on bottom of main window
  void setStatus( const QString& newstatus );
  void setStatusUser( const QString& user );

  /**  */
  bool isConnected;

  /**
     @returns Reference to QString object of the current user id
  */
  QString& current_user() { return options.current_user; }

  void errormsg( const QString& msg );
  void infomsg( const QString& msg );
  void warnmsg( const QString& msg);
  void debugmsg( const char* fmt, ...);

  QString getRecdPath();

  void play_sound( enum sound_event);
  /** No descriptions */
  void newmail(char *from, char *subj, int cnt);
  /** No descriptions */
  void addedMessage(QString uid, QString msg);
  
  // DCOP interface
  KYahooIface *kyahoo_interface;

  void fastDisconnect();

  KToolBar* kyimtoolbar;

  void connected();
  
protected:
  virtual bool queryExit();

public slots:
  int disconnect();
  int connect();
  /** No descriptions */

private slots:
  void slotNewChat();
  void slotNewConf();
  void slotOffLineMsg();
  void slotAddUser();
  void slotRemoveUser();
  void slotShowDebug();
  int writeConfig( void );
  void showOptions();
  void slotRefreshAll();
  void quit();
  void socket_timeout();
  void slotShowStatusBar();
  void slotShowMenuBar();
  void slotShowToolBar();
  void slotFindFriends();
  /** No descriptions */
  void slotGoToMail();

private:
  int need_setup;
  
  Icon* sysTray;

  QPopupMenu* filemenu;
  QPopupMenu* yahoomenu;
  QPopupMenu* viewmenu;
  KHelpMenu*  helpmenu;

  packet* packet_handler;

  KConfig *config;

  struct kyahoo_options options;

  int initialize();
  void initialize_option_defaults();
  void enableMenus(void);
  void disableMenus(void);
  
  int readConfig( void );

  Debug_Msg_Dialog* Dbg_Dialog;
  Debug_Msg_Dialog* Ptr_Dbg_Dialog; // flag for if window is open.

  QTimer *socket_timer;

};


#endif


