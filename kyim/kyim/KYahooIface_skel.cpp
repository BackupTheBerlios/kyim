/****************************************************************************
**
** DCOP Skeleton created by dcopidl2cpp from KYahooIface.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#include "KYahooIface.h"

#include <kdatastream.h>


static const char* const KYahooIface_ftable[8][3] = {
    { "void", "setCustomStatus(QString)", "setCustomStatus(QString status)" },
    { "void", "newChatSession(QString)", "newChatSession(QString buddy)" },
    { "void", "sendMessageTo(QString,QString)", "sendMessageTo(QString buddy,QString message)" },
    { "QStringList", "getOnlineBuddies()", "getOnlineBuddies()" },
    { "QStringList", "getOfflineBuddies()", "getOfflineBuddies()" },
    { "QStringList", "getAllBuddies()", "getAllBuddies()" },
    { "QStringList", "getBuddyStatusList()", "getBuddyStatusList()" },
    { 0, 0, 0 }
};

bool KYahooIface::process(const QCString &fun, const QByteArray &data, QCString& replyType, QByteArray &replyData)
{
    if ( fun == KYahooIface_ftable[0][1] ) { // void setCustomStatus(QString)
	QString arg0;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	replyType = KYahooIface_ftable[0][0]; 
	setCustomStatus(arg0 );
    } else if ( fun == KYahooIface_ftable[1][1] ) { // void newChatSession(QString)
	QString arg0;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	replyType = KYahooIface_ftable[1][0]; 
	newChatSession(arg0 );
    } else if ( fun == KYahooIface_ftable[2][1] ) { // void sendMessageTo(QString,QString)
	QString arg0;
	QString arg1;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	arg >> arg1;
	replyType = KYahooIface_ftable[2][0]; 
	sendMessageTo(arg0, arg1 );
    } else if ( fun == KYahooIface_ftable[3][1] ) { // QStringList getOnlineBuddies()
	replyType = KYahooIface_ftable[3][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << getOnlineBuddies( );
    } else if ( fun == KYahooIface_ftable[4][1] ) { // QStringList getOfflineBuddies()
	replyType = KYahooIface_ftable[4][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << getOfflineBuddies( );
    } else if ( fun == KYahooIface_ftable[5][1] ) { // QStringList getAllBuddies()
	replyType = KYahooIface_ftable[5][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << getAllBuddies( );
    } else if ( fun == KYahooIface_ftable[6][1] ) { // QStringList getBuddyStatusList()
	replyType = KYahooIface_ftable[6][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << getBuddyStatusList( );
    } else {
	return DCOPObject::process( fun, data, replyType, replyData );
    }
    return TRUE;
}

QCStringList KYahooIface::interfaces()
{
    QCStringList ifaces = DCOPObject::interfaces();
    ifaces += "KYahooIface";
    return ifaces;
}

QCStringList KYahooIface::functions()
{
    QCStringList funcs = DCOPObject::functions();
    for ( int i = 0; KYahooIface_ftable[i][2]; i++ ) {
	QCString func = KYahooIface_ftable[i][0];
	func += ' ';
	func += KYahooIface_ftable[i][2];
	funcs << func;
    }
    return funcs;
}


