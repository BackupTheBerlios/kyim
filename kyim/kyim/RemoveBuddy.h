/****************************************************************************
** Form interface generated from reading ui file './RemoveBuddy.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: RemoveBuddy.h,v 1.1 2004/05/23 10:13:01 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef REMOVEBUDDY_H
#define REMOVEBUDDY_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QFrame;
class QComboBox;
class QPushButton;

class RemoveBuddy : public QDialog
{
    Q_OBJECT

public:
    RemoveBuddy( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~RemoveBuddy();

    QLabel* TextLabel1;
    QFrame* Frame7;
    QComboBox* ID;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QComboBox* Group;
    QPushButton* PushButton13;
    QPushButton* PushButton14;

protected:
    QVBoxLayout* RemoveBuddyLayout;
    QHBoxLayout* Frame7Layout;
    QGridLayout* Layout16;
    QHBoxLayout* Layout8;

protected slots:
    virtual void languageChange();

    virtual void commit();


};

#endif // REMOVEBUDDY_H
