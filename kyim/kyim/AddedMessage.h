/****************************************************************************
** Form interface generated from reading ui file './AddedMessage.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: AddedMessage.h,v 1.1 2004/05/23 10:12:42 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ADDEDMESSAGE_H
#define ADDEDMESSAGE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QLabel;

class AddedMessage : public QDialog
{
    Q_OBJECT

public:
    AddedMessage( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AddedMessage();

    QPushButton* OK;
    QPushButton* AcceptAdd;
    QPushButton* viewProfile;
    QLabel* message;

public slots:
    virtual void acceptAdd();
    virtual void profile();

protected:

protected slots:
    virtual void languageChange();

};

#endif // ADDEDMESSAGE_H
