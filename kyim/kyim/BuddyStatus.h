
extern "C" {
#include "yahoo2.h"
}


#ifndef BUDDYSTATUS_H
#define BUDDYSTATUS_H


/**
 * BuddyStatus
 * 
 * Maintains each individual buddy's status and custom status message if
 * available. Each buddy in the list will be represented by an object of
 * this class. 
 *
 * @short Buddy Status Element
 *
 */

class BuddyStatus
{
 public:
  BuddyStatus( const char *newid, const char* realname,
	       const char *buddygroup, 
	       int newstatus = YAHOO_STATUS_OFFLINE )
    : id(newid), realName(realname), group(buddygroup), status(newstatus) 
    {
      updateTime();
    }

  bool available( void ) { 
    if ( status == YAHOO_STATUS_OFFLINE ) 
      return false;
    else
      return true;
  } /* available() */
 
  void updateTime( void ) { loginTime = QTime::currentTime(); }

  QString id; // yahoo login
  QString realName; // real name
  QString group;

  int status; // buddy status
  QString custStatus; // custom status

  QTime loginTime; // time buddy status changed

};

#endif
