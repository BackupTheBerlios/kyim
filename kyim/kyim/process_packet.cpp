/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
extern "C" {
#include "yahoo2.h"
#include "yahoo2_callbacks.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

#include <qsocketnotifier.h>
#include <qdialog.h>
#include <qtimer.h>
#include <time.h>
#include <qregexp.h>
#include <klocale.h>
#include <qtable.h>
#include <ktoolbarbutton.h>

#include "ChatSession.h"
#include "ConferenceSession.h"
#include "process_packet.h"
#include "kyim.h"
#include "dialogs.h"
#include "ConfInvitationImpl.h"
#include "IncomingFileTransferImpl.h"
#include "kyimicons.h"

#include "IMState.h"

extern struct yahoo_context* context;
extern IMState *imstate;
extern KYahoo* main_window;


static void ycb_yahoo_login_response(int id, int succ, char *url);
static void ycb_yahoo_got_buddies(int id, YList * buds);
static void ycb_yahoo_got_ignore(int id, YList * igns);
static void ycb_yahoo_got_identities(int id, YList * ids);
static void ycb_yahoo_got_cookies(int id);
static void ycb_yahoo_status_changed(int id, char *who, int stat, char *msg, int away);
static void ycb_yahoo_got_im(int id, char *who, char *msg, long tm, int stat, int utf8);
static void ycb_yahoo_got_conf_invite(int id, char *who, char *room, char *msg, 
			       YList *members);
static void ycb_yahoo_conf_userdecline(int id, char *who, char *room, char *msg);
static void ycb_yahoo_conf_userjoin(int id, char *who, char *room);
static void ycb_yahoo_conf_userleave(int id, char *who, char *room);
static void ycb_yahoo_chat_cat_xml(int id, char *xml);
static void ycb_yahoo_chat_join(int id, char *room, char *topic, YList *members, int fd);
static void ycb_yahoo_chat_userjoin(int id, char *room, struct yahoo_chat_member *who);
static void ycb_yahoo_chat_userleave(int id, char *room, char *who);
static void ycb_yahoo_chat_message(int id, char *who, char *room, char *msg, int msgtype, int utf8);
static void ycb_yahoo_chat_yahoologout(int id);
static void ycb_yahoo_chat_yahooerror(int id);
static void ycb_yahoo_conf_message(int id, char *who, char *room, char *msg, int utf8);
static void ycb_yahoo_got_file(int id, char *who, char *url, long expires, 
			char *msg, char *fname, unsigned long fesize);
static void ycb_yahoo_contact_added(int id, char *myid, char *who, char *msg);
static void ycb_yahoo_rejected(int id, char *who, char *msg);
static void ycb_yahoo_typing_notify(int id, char *who, int stat);
static void ycb_yahoo_game_notify(int id, char *who, int stat);
static void ycb_yahoo_mail_notify(int id, char *from, char *subj, int cnt);
static void ycb_yahoo_system_message(int id, char *msg);
static void ycb_yahoo_got_webcam_image(int id, const char * who,
				       const unsigned char *image, unsigned int image_size, 
				       unsigned int real_size, unsigned int timestamp);
static void ycb_yahoo_webcam_invite(int id, char *from);
static void ycb_yahoo_webcam_invite_reply(int id, char *from, int accept);
static void ycb_yahoo_webcam_closed(int id, char *who, int reason);
static void ycb_yahoo_got_search_result(int id, int found, int start, int total, YList *contacts);
static void ycb_yahoo_error(int id, char *err, int fatal);
static void ycb_yahoo_webcam_viewer(int id, char *who, int connect);
static void ycb_yahoo_webcam_data_request(int id, int send);
static int ycb_yahoo_log(char *fmt,...);
static int ycb_yahoo_add_handler(int id, int fd, yahoo_input_condition cond, void* data);
static void ycb_yahoo_remove_handler(int id, int tag);
static int ycb_yahoo_connect(char *host, int port);
static int ycb_yahoo_connect_async(int id, char *host, int port, 
				   yahoo_connect_callback callback, void *callback_data);

static packet* packetPtr; 

packet::packet()
{
  // ping timer (every 2 minutes )
  ping_timer = new QTimer( this, "Ping Timer" );
  QObject::connect( ping_timer, SIGNAL(timeout()),
		    this, SLOT( ping_timeout()) );

  // start ping timer
  ping_timer->start( 1000*60*5 );

  register_callbacks();

  // null out ptr to start
  ReadSocketNotifier = 0;
  WriteSocketNotifier = 0;


  packetPtr = this; 
}

packet::~packet()
{
  // only if startReadNotifier was called 
  if ( ReadSocketNotifier ) {
    
    QObject::disconnect( ReadSocketNotifier, SIGNAL(activated(int)),
			 this, SLOT( io_read_callback(int) ) );
    delete ReadSocketNotifier;
  }
  // only if startWriteNotifier was called 
  if ( WriteSocketNotifier ) {
    QObject::disconnect( WriteSocketNotifier, SIGNAL(activated(int)),
			 this, SLOT( io_write_callback(int) ) );
    delete WriteSocketNotifier;
  }
}

void packet::startReadNotifier(void)
{
  main_window->debugmsg("startReadNotifier %d", 
			context->handle[YAHOO_INPUT_READ].sockfd);
  ReadSocketNotifier = new QSocketNotifier( context->handle[YAHOO_INPUT_READ].sockfd, QSocketNotifier::Read);
  QObject::connect( ReadSocketNotifier, SIGNAL( activated( int ) ),
		    this, SLOT( io_read_callback( int ) ) );
}


void packet::startWriteNotifier(void)
{
  main_window->debugmsg("startWriteNotifier %d", context->handle[YAHOO_INPUT_WRITE].sockfd);
  if (!WriteSocketNotifier) {
    WriteSocketNotifier = new QSocketNotifier( context->handle[YAHOO_INPUT_WRITE].sockfd, QSocketNotifier::Write);
    QObject::connect( WriteSocketNotifier, SIGNAL( activated( int ) ),
		      this, SLOT( io_write_callback( int ) ) );
  }
  else {
    WriteSocketNotifier->setEnabled(TRUE);
  }
}


void packet::stopWriteNotifier(void)
{
  main_window->debugmsg("stopWriteNotifier %d", context->handle[YAHOO_INPUT_WRITE].sockfd);
  WriteSocketNotifier->setEnabled(FALSE); 
}


void packet::register_callbacks()
{
#ifdef USE_STRUCT_CALLBACKS
	static struct yahoo_callbacks yc;

	yc.ext_yahoo_login_response = ycb_yahoo_login_response;
	yc.ext_yahoo_got_buddies = ycb_yahoo_got_buddies;
	yc.ext_yahoo_got_ignore = ycb_yahoo_got_ignore;
	yc.ext_yahoo_got_identities = ycb_yahoo_got_identities;
	yc.ext_yahoo_got_cookies = ycb_yahoo_got_cookies;
	yc.ext_yahoo_status_changed = ycb_yahoo_status_changed;
	yc.ext_yahoo_got_im = ycb_yahoo_got_im;
	yc.ext_yahoo_got_conf_invite = ycb_yahoo_got_conf_invite;
	yc.ext_yahoo_conf_userdecline = ycb_yahoo_conf_userdecline;
	yc.ext_yahoo_conf_userjoin = ycb_yahoo_conf_userjoin;
	yc.ext_yahoo_conf_userleave = ycb_yahoo_conf_userleave;
	yc.ext_yahoo_chat_cat_xml = ycb_yahoo_chat_cat_xml;
	yc.ext_yahoo_chat_join = ycb_yahoo_chat_join;
	yc.ext_yahoo_chat_userjoin = ycb_yahoo_chat_userjoin;
	yc.ext_yahoo_chat_userleave = ycb_yahoo_chat_userleave;
	yc.ext_yahoo_chat_message = ycb_yahoo_chat_message;
	yc.ext_yahoo_chat_yahoologout = ycb_yahoo_chat_yahoologout;
	yc.ext_yahoo_chat_yahooerror = ycb_yahoo_chat_yahooerror;
	yc.ext_yahoo_conf_message = ycb_yahoo_conf_message;
	yc.ext_yahoo_got_file = ycb_yahoo_got_file;
	yc.ext_yahoo_contact_added = ycb_yahoo_contact_added;
	yc.ext_yahoo_rejected = ycb_yahoo_rejected;
	yc.ext_yahoo_typing_notify = ycb_yahoo_typing_notify;
	yc.ext_yahoo_game_notify = ycb_yahoo_game_notify;
	yc.ext_yahoo_mail_notify = ycb_yahoo_mail_notify;
	yc.ext_yahoo_system_message = ycb_yahoo_system_message;
	yc.ext_yahoo_got_webcam_image = ycb_yahoo_got_webcam_image;
	yc.ext_yahoo_webcam_invite = ycb_yahoo_webcam_invite;
	yc.ext_yahoo_webcam_invite_reply = ycb_yahoo_webcam_invite_reply;
	yc.ext_yahoo_webcam_closed = ycb_yahoo_webcam_closed;	yc.ext_yahoo_got_search_result = ycb_yahoo_got_search_result;
	yc.ext_yahoo_error = ycb_yahoo_error;
	yc.ext_yahoo_webcam_viewer = ycb_yahoo_webcam_viewer;
	yc.ext_yahoo_webcam_data_request = ycb_yahoo_webcam_data_request;
	yc.ext_yahoo_log = ycb_yahoo_log;
	yc.ext_yahoo_add_handler = ycb_yahoo_add_handler;
	yc.ext_yahoo_remove_handler = ycb_yahoo_remove_handler;
	yc.ext_yahoo_connect = ycb_yahoo_connect;
	yc.ext_yahoo_connect_async = ycb_yahoo_connect_async;
	yahoo_register_callbacks(&yc);
	
#endif
}

void packet::io_read_callback( int fd )
{
  int ret = 1;

  main_window->debugmsg("io_read_callback(%d)", fd);
  ret = yahoo_read_ready(context->id, fd, context->handle[YAHOO_INPUT_READ].data);
  
  if(ret == -1) {
    main_window->debugmsg("Yahoo read error (%d): %s", errno, strerror(errno));
  }
  else if(ret == 0) {
    main_window->debugmsg("Yahoo read error: Server closed socket");
  }
}

void packet::io_write_callback( int fd )
{
  int ret = 1;

  main_window->debugmsg("io_write_callback(%d)", fd);
  ret = yahoo_write_ready(context->id, fd, context->handle[YAHOO_INPUT_WRITE].data);
  
  if(ret == -1) {
    main_window->debugmsg("Yahoo write error (%d): %s", errno, strerror(errno));
  }
  else if(ret == 0) {
    main_window->debugmsg("Yahoo write error: Server closed socket");
  }
}

void packet::timer_callback( void )
{
  main_window->debugmsg("timer callback");
}


void packet::ping_timeout(void)
{
  // only ping if connected
  if ( context )
    yahoo_keepalive(context->id);
}


void ycb_yahoo_login_response(int id, int succ, char *url)
{
  main_window->debugmsg("ycb_login_response: %d %d", id, succ);

  if(succ == YAHOO_LOGIN_OK) {
    main_window->debugmsg("logged in");
    main_window->connected();
    return;  
  }
  else if(succ == YAHOO_LOGIN_PASSWD) {
    main_window->warnmsg("Could not log into Yahoo service.  Please verify that your username and password are correctly typed.");
  } 
  else if(succ == YAHOO_LOGIN_LOCK) {
    QString msg ("Could not log into Yahoo service.  Your account has been locked.\n");
    msg += "Visit ";
    msg += url;
    msg += " to reactivate it.";
    main_window->warnmsg(msg);
  } 
  else if(succ == YAHOO_LOGIN_DUPL) {
    main_window->warnmsg("You have been logged out of the yahoo service, possibly due to a duplicate login.");
  }
  
  main_window->disconnect();
}

void ycb_yahoo_got_buddies(int id, YList * buds)
{
  main_window->debugmsg("ycb_yahoo_got_buddies");

  imstate->update_buddyList(buds);

  for(; buds; buds = buds->next) {
    struct yahoo_buddy *bud = (struct yahoo_buddy *)buds->data;
    main_window->debugmsg("Buddy: Group %s | ID %s | Real Name %s", bud->group,
	     bud->id, bud->real_name );
  }
}


void ycb_yahoo_got_ignore(int id, YList * igns)
{
  main_window->debugmsg("ycb_yahoo_got_ignore");

  imstate->update_ignoreList(igns);

}


void ycb_yahoo_got_identities(int id, YList * ids)
{
  main_window->debugmsg("ycb_yahoo_got_identities");

}

void ycb_yahoo_got_cookies(int id)
{
  main_window->debugmsg("ycb_yahoo_got_cookies");
}

void ycb_yahoo_status_changed(int id, char *who, int stat, char *msg, int away)
{
  main_window->debugmsg("ycb_yahoo_status_changed id %d who %s stat %d msg %d away %d",
			id, who, stat, msg, away);
  
  imstate->update_buddyStatus( who, stat, msg, away );
}


void ycb_yahoo_got_im(int id, char *who, char *msg, long tm, int stat, int utf8)
{
  main_window->debugmsg("ycb_yahoo_got_im id %d who %s stat %d msg %d tm %d utf8 %d",
			id, who, stat, msg, tm, utf8);

  imstate->chat_message( who, msg, tm, stat, utf8 );
}

void ycb_yahoo_got_conf_invite(int id, char *who, char *room, char *msg, 
			       YList *members)
{

  main_window->debugmsg("ycb_yahoo_got_conf_invite id %d who %s room %s msg %s", 
	   id, who, room, msg);

  imstate->conf_invite( who, room, msg, members );

  for(; members; members=members->next)
    main_window->debugmsg("\tmembers: %s", (char *)members->data);

}

void ycb_yahoo_conf_userdecline(int id, char *who, char *room, char *msg)
{
  main_window->debugmsg("ycb_yahoo_conf_userdecline id %d who %s room %s msg %s",
	   id, who, room, msg);

  imstate->conf_userDecline( who, room, msg );
}


void ycb_yahoo_conf_userjoin(int id, char *who, char *room)
{
  main_window->debugmsg("ycb_yahoo_conf_userjoin id %d who %s room %s",
	   id, who, room);
  
  imstate->conf_userJoin( who, room);
}


void ycb_yahoo_conf_userleave(int id, char *who, char *room)
{
  main_window->debugmsg("ycb_yahoo_conf_userleave id %d who %s room %s",
	   id, who, room);
  imstate->conf_userLeave( who, room);
}


void ycb_yahoo_chat_cat_xml(int id, char *xml)
{
  main_window->debugmsg("ycb_yahoo_chat_cat_xml");
}


void ycb_yahoo_chat_join(int id, char *room, char *topic, YList *members, int fd)
{
  main_window->debugmsg("ycb_yahoo_chat_join");
}


void ycb_yahoo_chat_userjoin(int id, char *room, struct yahoo_chat_member *who)
{
  main_window->debugmsg("ycb_yahoo_chat_userjoin");
}


void ycb_yahoo_chat_userleave(int id, char *room, char *who)
{
  main_window->debugmsg("ycb_yahoo_chat_userleave");
}


void ycb_yahoo_chat_message(int id, char *who, char *room, char *msg, int msgtype, int utf8)
{
  main_window->debugmsg("ycb_yahoo_chat_message");
}

void ycb_yahoo_chat_yahoologout(int id)
{
  main_window->debugmsg("ycb_yahoo_chat_yahoologout");
}

void ycb_yahoo_chat_yahooerror(int id)
{
  main_window->debugmsg("ycb_yahoo_chat_yahooerror");
}

void ycb_yahoo_conf_message(int id, char *who, char *room, char *msg, int utf8)
{
  main_window->debugmsg("ycb_yahoo_conf_message id %d who %s room %s nsg %s utf8 %d", 
	   id, who, room, msg, utf8);
  imstate->conf_message( who, room, msg);
}


void ycb_yahoo_got_file(int id, char *who, char *url, long expires, 
			char *msg, char *fname, unsigned long fesize)
{
  main_window->debugmsg("ycb_yahoo_got_file id %d who %s url %s expires %d msg %s fname %s fesize %d", 
	   id, who, url, expires, msg, fname, fesize);
}


void ycb_yahoo_contact_added(int id, char *myid, char *who, char *msg)
{
  QString info; 
  main_window->debugmsg("ycb_yahoo_contact_added id %d myid %s who %s msg %s",
	   id, myid, who, msg);

  main_window->addedMessage( who, msg );
}


void ycb_yahoo_rejected(int id, char *who, char *msg)
{
  main_window->debugmsg("ycb_yahoo_rejected id %d who %s msg %s", id, who, msg);
}


void ycb_yahoo_typing_notify(int id, char *who, int stat)
{
  main_window->debugmsg("ycb_yahoo_typing_notify id %d who %s stat %d", id, who, stat);
  imstate->notify_typing( who, stat );
}


void ycb_yahoo_game_notify(int id, char *who, int stat)
{
  main_window->debugmsg("ycb_yahoo_game_notify id %d who %s stat %d", id, who, stat);
  imstate->notify_game(who, stat);
}


void ycb_yahoo_mail_notify(int id, char *from, char *subj, int cnt)
{
  main_window->debugmsg("ycb_yahoo_mail_notify id %d from %s subj %s cnt %d", 
			id, from, subj, cnt);

  // only notify if there is new mail waiting
  if (cnt) {
    main_window->newmail(from, subj, cnt);
  }
}


void ycb_yahoo_system_message(int id, char *msg)
{
  QString text("Yahoo System Message: "); 
  main_window->debugmsg("ycb_yahoo_system_message id %d msg %s", 
	   id, msg);
  
  text += msg; 
  main_window->infomsg(msg);
}


void ycb_yahoo_got_webcam_image(int id, const char * who,
				const unsigned char *image, unsigned int image_size, 
				unsigned int real_size, unsigned int timestamp)
{
  main_window->debugmsg("ycb_yahoo_got_webcam_image");
}


void ycb_yahoo_webcam_invite(int id, char *from)
{
  main_window->debugmsg("ycb_yahoo_webcam_invite");
}


void ycb_yahoo_webcam_invite_reply(int id, char *from, int accept)
{
  main_window->debugmsg("ycb_yahoo_webcam_invite_reply");
}


void ycb_yahoo_webcam_closed(int id, char *who, int reason)
{
  main_window->debugmsg("ycb_yahoo_webcam_closed");
}

void ycb_yahoo_got_search_result(int id, int found, int start, int total, YList *contacts)
{
  main_window->debugmsg("ycb_yahoo_got_search_result");
}

void ycb_yahoo_error(int id, char *err, int fatal)
{
  main_window->debugmsg("ycb_yahoo_error id %d err %s fatal %d", 
	   id, err, fatal);
	
  if(fatal) {
    main_window->errormsg(err);
    main_window->disconnect();
  }
}


void ycb_yahoo_webcam_viewer(int id, char *who, int connect)
{
  main_window->debugmsg("ycb_yahoo_webcam_viewer");
}


void ycb_yahoo_webcam_data_request(int id, int send)
{
  main_window->debugmsg("ycb_yahoo_webcam_data_request");
}


int ycb_yahoo_log(char *fmt,...)
{
  char s[200];
  va_list ap;
  
  va_start(ap, fmt);
  
  vsnprintf(s, 200, fmt, ap);
  main_window->debugmsg("ycb_yahoo_log: %s", s);
  va_end(ap);

  return 0;
}


int ycb_yahoo_add_handler(int id, int fd, yahoo_input_condition cond, void* data)
{
  main_window->debugmsg("ycb_yahoo_add_handler id %d fd %d cond %d data 0x%x", 
			id, fd, cond, data);
  context->id = id; 
  context->handle[cond].data = data;
  context->handle[cond].cond = cond;
  context->handle[cond].sockfd = fd;

  if (cond == YAHOO_INPUT_READ ) {
    packetPtr->startReadNotifier();
  }
  if (cond == YAHOO_INPUT_WRITE ) {
    packetPtr->startWriteNotifier();
  }

  return (cond);
}


void ycb_yahoo_remove_handler(int id, int tag)
{
  main_window->debugmsg("ycb_yahoo_remove_handler id %d tag %d", id, tag);
  
  if ( tag == YAHOO_INPUT_WRITE ) {
    packetPtr->stopWriteNotifier();
  }

#if 0
    context->id = 0;
    context->handle[YAHOO_INPUT_WRITE].sockfd = 0;
    context->handle[YAHOO_INPUT_READ].sockfd = 0;
#endif
}


int ycb_yahoo_connect(char *host, int port)
{
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int res;
  char **p;

  main_window->debugmsg("ycb_yahoo_connect host %s port %d", host, port);

  if(!(server = gethostbyname(host))) {
    main_window->debugmsg("failed to look up server (%s:%d)\n%d: %s", host, port,
	     h_errno, hstrerror(h_errno));
    return -1;
  }
  
  if((context->handle[YAHOO_INPUT_WRITE].sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    main_window->debugmsg("Socket create error (%d): %s", errno, 
	     strerror(errno));
    return -1;
  }
  
  for (p = server->h_addr_list; *p; p++)
    {
      memset(&serv_addr, 0, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      memcpy(&serv_addr.sin_addr.s_addr, *p, server->h_length);
      serv_addr.sin_port = htons(port);
      
      res = -1;
      res = connect(context->handle[YAHOO_INPUT_WRITE].sockfd, (struct sockaddr *) &serv_addr, 
		    sizeof(serv_addr));
      
      if(res == 0 ) {
	main_window->debugmsg("connected %d", context->handle[YAHOO_INPUT_WRITE].sockfd);
	return context->handle[YAHOO_INPUT_WRITE].sockfd;
      }
    }
  
  main_window->debugmsg("Could not connect to %s:%d\n%d:%s", host, port,
	   errno, strerror(errno));
  close(context->handle[YAHOO_INPUT_WRITE].sockfd);
  return -1;
}


int ycb_yahoo_connect_async(int id, char *host, int port, 
			    yahoo_connect_callback callback, void *data)
{
  struct sockaddr_in serv_addr;
  static struct hostent *server;
  int error;
  
  main_window->debugmsg("ycb_yahoo_connect_async");
  
  if(!(server = gethostbyname(host))) {
    errno=h_errno;
    return -1;
  }
  
  if((context->handle[YAHOO_INPUT_WRITE].sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }
  
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr.s_addr, *server->h_addr_list, server->h_length);
  serv_addr.sin_port = htons(port);
  
  error = connect(context->handle[YAHOO_INPUT_WRITE].sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  
  main_window->debugmsg("Trying to connect: fd:%d error:%d", context->handle[YAHOO_INPUT_WRITE].sockfd, error);
  if(!error) {
    callback(context->handle[YAHOO_INPUT_WRITE].sockfd, 0, data);
    return context->handle[YAHOO_INPUT_WRITE].sockfd;
  }  
  close(context->handle[YAHOO_INPUT_WRITE].sockfd);
  return -1;
}
