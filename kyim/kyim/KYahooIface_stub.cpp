/****************************************************************************
**
** DCOP Stub Implementation created by dcopidl2cpp from KYahooIface.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#include "KYahooIface_stub.h"
#include <dcopclient.h>

#include <kdatastream.h>



KYahooIface_stub::KYahooIface_stub( const QCString& app, const QCString& obj )
  : DCOPStub( app, obj )
{
}

KYahooIface_stub::KYahooIface_stub( DCOPClient* client, const QCString& app, const QCString& obj )
  : DCOPStub( client, app, obj )
{
}

void KYahooIface_stub::setCustomStatus( QString arg0 )
{
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return;
    }
    QByteArray data, replyData;
    QCString replyType;
    QDataStream arg( data, IO_WriteOnly );
    arg << arg0;
    if ( dcopClient()->call( app(), obj(), "setCustomStatus(QString)", data, replyType, replyData ) ) {
	setStatus( CallSucceeded );
    } else { 
	callFailed();
    }
}

void KYahooIface_stub::newChatSession( QString arg0 )
{
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return;
    }
    QByteArray data, replyData;
    QCString replyType;
    QDataStream arg( data, IO_WriteOnly );
    arg << arg0;
    if ( dcopClient()->call( app(), obj(), "newChatSession(QString)", data, replyType, replyData ) ) {
	setStatus( CallSucceeded );
    } else { 
	callFailed();
    }
}

void KYahooIface_stub::sendMessageTo( QString arg0, QString arg1 )
{
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return;
    }
    QByteArray data, replyData;
    QCString replyType;
    QDataStream arg( data, IO_WriteOnly );
    arg << arg0;
    arg << arg1;
    if ( dcopClient()->call( app(), obj(), "sendMessageTo(QString,QString)", data, replyType, replyData ) ) {
	setStatus( CallSucceeded );
    } else { 
	callFailed();
    }
}

QStringList KYahooIface_stub::getOnlineBuddies()
{
    QStringList result;
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return result;
    }
    QByteArray data, replyData;
    QCString replyType;
    if ( dcopClient()->call( app(), obj(), "getOnlineBuddies()", data, replyType, replyData ) ) {
	if ( replyType == "QStringList" ) {
	    QDataStream _reply_stream( replyData, IO_ReadOnly );
	    _reply_stream >> result;
	    setStatus( CallSucceeded );
	} else {
	    callFailed();
	}
    } else { 
	callFailed();
    }
    return result;
}

QStringList KYahooIface_stub::getOfflineBuddies()
{
    QStringList result;
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return result;
    }
    QByteArray data, replyData;
    QCString replyType;
    if ( dcopClient()->call( app(), obj(), "getOfflineBuddies()", data, replyType, replyData ) ) {
	if ( replyType == "QStringList" ) {
	    QDataStream _reply_stream( replyData, IO_ReadOnly );
	    _reply_stream >> result;
	    setStatus( CallSucceeded );
	} else {
	    callFailed();
	}
    } else { 
	callFailed();
    }
    return result;
}

QStringList KYahooIface_stub::getAllBuddies()
{
    QStringList result;
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return result;
    }
    QByteArray data, replyData;
    QCString replyType;
    if ( dcopClient()->call( app(), obj(), "getAllBuddies()", data, replyType, replyData ) ) {
	if ( replyType == "QStringList" ) {
	    QDataStream _reply_stream( replyData, IO_ReadOnly );
	    _reply_stream >> result;
	    setStatus( CallSucceeded );
	} else {
	    callFailed();
	}
    } else { 
	callFailed();
    }
    return result;
}

QStringList KYahooIface_stub::getBuddyStatusList()
{
    QStringList result;
    if ( !dcopClient()  ) {
	setStatus( CallFailed );
	return result;
    }
    QByteArray data, replyData;
    QCString replyType;
    if ( dcopClient()->call( app(), obj(), "getBuddyStatusList()", data, replyType, replyData ) ) {
	if ( replyType == "QStringList" ) {
	    QDataStream _reply_stream( replyData, IO_ReadOnly );
	    _reply_stream >> result;
	    setStatus( CallSucceeded );
	} else {
	    callFailed();
	}
    } else { 
	callFailed();
    }
    return result;
}


