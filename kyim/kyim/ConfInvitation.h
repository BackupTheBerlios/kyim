/****************************************************************************
** Form interface generated from reading ui file './ConfInvitation.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: ConfInvitation.h,v 1.1 2004/05/23 10:12:42 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONFINVITATION_H
#define CONFINVITATION_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QFrame;
class QLineEdit;
class QPushButton;

class ConfInvitation : public QDialog
{
    Q_OBJECT

public:
    ConfInvitation( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConfInvitation();

    QLabel* Inviter;
    QLabel* TextLabel1;
    QLabel* TextLabel2;
    QLabel* Message;
    QLabel* TextLabel5;
    QFrame* Frame3;
    QLabel* TextLabel6;
    QLineEdit* Reason;
    QPushButton* PushButton1;
    QPushButton* PushButton2;

protected:
    QVBoxLayout* ConfInvitationLayout;
    QHBoxLayout* Layout1;
    QVBoxLayout* Frame3Layout;
    QVBoxLayout* Layout5;
    QHBoxLayout* Layout3;

protected slots:
    virtual void languageChange();

    virtual void decline();


};

#endif // CONFINVITATION_H
