/*

Simple kyahoo dcop interface.
Created by Frank Dekervel, 2001

*/



#ifndef KYAHOOIFACE_H
#define KYAHOOIFACE_H

#include <qcombobox.h>
#include <qstringlist.h>
#include <dcopclient.h>
#include <dcopobject.h>
#include "kyim.h"

class KYahooIface: public QObject, DCOPObject

 {


Q_OBJECT
K_DCOP

public:
    KYahooIface();
    ~KYahooIface();

k_dcop:
    void setCustomStatus(QString status);
    void newChatSession(QString buddy);
    void sendMessageTo(QString buddy, QString message);

    QStringList getOnlineBuddies();
    QStringList getOfflineBuddies();
    QStringList getAllBuddies();
    QStringList getBuddyStatusList();

};




#endif /* KYAHOOIFACE_H */
