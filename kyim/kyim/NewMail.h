/****************************************************************************
** Form interface generated from reading ui file './NewMail.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: NewMail.h,v 1.1 2004/05/23 10:13:00 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef NEWMAIL_H
#define NEWMAIL_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;

class NewMail : public QDialog
{
    Q_OBJECT

public:
    NewMail( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~NewMail();

    QLabel* PixmapLabel1;
    QLabel* TextLabel1;
    QPushButton* OK;
    QPushButton* GoToMail;

public slots:
    virtual void goToMail();

protected:
    QVBoxLayout* NewMailLayout;
    QHBoxLayout* Layout1;
    QHBoxLayout* Layout3;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // NEWMAIL_H
