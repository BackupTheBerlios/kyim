/*

simple kyahoo dcop interface implementation
created by Frank Dekervel (2001)


*/


#include "KYahooIface.h"
#include "ChatSession.h"
#include "IMState.h"

extern "C" {
#include "yahoo2.h"
}

extern struct yahoo_context *context;
extern KYahoo* main_window;
extern IMState* imstate; 


KYahooIface::KYahooIface() 
       :DCOPObject("KYahooIface") 
{

}

KYahooIface::~KYahooIface()
{

// destruction goes in here

}


void KYahooIface::setCustomStatus(QString s) {
  imstate->MyStatusChanged(s);
}

void KYahooIface::newChatSession(QString buddy) {
  imstate->start_chat( buddy );
}

void KYahooIface::sendMessageTo(QString buddy,QString message) {
  imstate->chat_message( buddy, message, 0, 0, 0);
}
 
QStringList KYahooIface::getOnlineBuddies() {
     return imstate->BuddiesOnline();
}

QStringList KYahooIface::getOfflineBuddies() {
     return imstate->BuddiesOffline();
}


QStringList KYahooIface::getAllBuddies() {
     return imstate->Buddies();
}

QStringList KYahooIface::getBuddyStatusList() {
     return imstate->BuddyStatusList();
}

