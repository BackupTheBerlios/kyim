/****************************************************************************
** Form interface generated from reading ui file './ConfAddInvite.ui'
**
** Created: Sun May 23 11:31:05 2004
**      by: The User Interface Compiler ($Id: ConfAddInvite.h,v 1.1 2004/05/23 10:12:42 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONFADDINVITE_H
#define CONFADDINVITE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class ConfAddInvite : public QDialog
{
    Q_OBJECT

public:
    ConfAddInvite( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConfAddInvite();

    QLabel* TextLabel1;
    QLineEdit* Name;
    QPushButton* PushButton1;
    QPushButton* PushButton2;

protected:
    QVBoxLayout* ConfAddInviteLayout;
    QHBoxLayout* Layout1;

protected slots:
    virtual void languageChange();

    virtual void commit();


};

#endif // CONFADDINVITE_H
