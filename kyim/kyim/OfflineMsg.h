/****************************************************************************
** Form interface generated from reading ui file './OfflineMsg.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: OfflineMsg.h,v 1.1 2004/05/23 10:13:01 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef OFFLINEMSG_H
#define OFFLINEMSG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;

class OfflineMsg : public QDialog
{
    Q_OBJECT

public:
    OfflineMsg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~OfflineMsg();

    QLabel* TextLabel1;
    QComboBox* Name;
    QLabel* TextLabel2;
    QLineEdit* Msg;
    QPushButton* PushButton3;
    QPushButton* PushButton4;

protected:
    QVBoxLayout* OfflineMsgLayout;
    QHBoxLayout* Layout7;
    QHBoxLayout* Layout5;
    QHBoxLayout* Layout6;

protected slots:
    virtual void languageChange();

    virtual void commit();


};

#endif // OFFLINEMSG_H
