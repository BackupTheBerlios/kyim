/****************************************************************************
**
** DCOP Stub Definition created by dcopidl2cpp from KYahooIface.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#ifndef __KYAHOOIFACE_STUB__
#define __KYAHOOIFACE_STUB__

#include <dcopstub.h>
#include <dcopobject.h>
#include <dcopclient.h>
#include <qstringlist.h>
#include <qcombobox.h>


class KYahooIface_stub : virtual public DCOPStub
{
public:
    KYahooIface_stub( const QCString& app, const QCString& id );
    KYahooIface_stub( DCOPClient* client, const QCString& app, const QCString& id );
    virtual void setCustomStatus( QString status );
    virtual void newChatSession( QString buddy );
    virtual void sendMessageTo( QString buddy, QString message );
    virtual QStringList getOnlineBuddies();
    virtual QStringList getOfflineBuddies();
    virtual QStringList getAllBuddies();
    virtual QStringList getBuddyStatusList();
};


#endif
