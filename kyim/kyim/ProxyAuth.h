/****************************************************************************
** Form interface generated from reading ui file './ProxyAuth.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: ProxyAuth.h,v 1.1 2004/05/23 10:13:01 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PROXYAUTH_H
#define PROXYAUTH_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QFrame;
class QLineEdit;
class QPushButton;

class ProxyAuth : public QDialog
{
    Q_OBJECT

public:
    ProxyAuth( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ProxyAuth();

    QLabel* TextLabel1;
    QFrame* Frame5;
    QLabel* TextLabel3;
    QLabel* TextLabel2;
    QLineEdit* Password;
    QLineEdit* Username;
    QPushButton* PushButton13;
    QPushButton* PushButton14;

protected:
    QVBoxLayout* ProxyAuthLayout;
    QHBoxLayout* Frame5Layout;
    QGridLayout* Layout6;
    QHBoxLayout* Layout8;

protected slots:
    virtual void languageChange();

    virtual void commit();


};

#endif // PROXYAUTH_H
