/****************************************************************************
** Form interface generated from reading ui file './AddBuddy.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: AddBuddy.h,v 1.1 2004/05/23 10:12:42 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ADDBUDDY_H
#define ADDBUDDY_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QFrame;
class QComboBox;
class QLineEdit;
class QPushButton;

class AddBuddy : public QDialog
{
    Q_OBJECT

public:
    AddBuddy( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AddBuddy();

    QLabel* TextLabel1;
    QFrame* Frame5;
    QComboBox* Group;
    QLineEdit* ID;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QLineEdit* Msg;
    QPushButton* PushButton13;
    QPushButton* PushButton14;

protected:
    QVBoxLayout* AddBuddyLayout;
    QGridLayout* Frame5Layout;
    QHBoxLayout* Layout10;
    QHBoxLayout* Layout8;

protected slots:
    virtual void languageChange();

    virtual void commit();


};

#endif // ADDBUDDY_H
