/****************************************************************************
** Form interface generated from reading ui file './IncomingFileTransfer.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: IncomingFileTransfer.h,v 1.1 2004/05/23 10:12:42 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef INCOMINGFILETRANSFER_H
#define INCOMINGFILETRANSFER_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QFrame;
class QLabel;
class QMultiLineEdit;
class QLineEdit;
class QPushButton;

class IncomingFileTransfer : public QDialog
{
    Q_OBJECT

public:
    IncomingFileTransfer( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IncomingFileTransfer();

    QFrame* Frame3;
    QLabel* TextLabel4;
    QLabel* lbl_frm;
    QFrame* Frame4;
    QLabel* TextLabel1;
    QMultiLineEdit* lbl_desc;
    QFrame* Frame5;
    QLabel* TextLabel3;
    QLineEdit* edit_dest;
    QPushButton* browse_btn;
    QPushButton* btn_okay;
    QPushButton* btn_cancel;

protected:
    QVBoxLayout* IncomingFileTransferLayout;
    QVBoxLayout* Frame3Layout;
    QVBoxLayout* Frame4Layout;
    QVBoxLayout* Frame5Layout;
    QHBoxLayout* Layout4;
    QHBoxLayout* Layout6;

protected slots:
    virtual void languageChange();

    virtual void cancel_clicked();
    virtual void browse_clicked();
    virtual void okay_clicked();


};

#endif // INCOMINGFILETRANSFER_H
