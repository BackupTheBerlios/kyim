/****************************************************************************
** Form interface generated from reading ui file './OfflinesView.ui'
**
** Created: Sun May 23 11:31:05 2004
**      by: The User Interface Compiler ($Id: OfflinesView.h,v 1.1 2004/05/23 10:13:01 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef OFFLINESVIEW_H
#define OFFLINESVIEW_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QTable;
class QPushButton;
class KTextBrowser;

class OfflinesView : public QDialog
{
    Q_OBJECT

public:
    OfflinesView( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~OfflinesView();

    QTable* messagesTable;
    QPushButton* closeButton;
    KTextBrowser* messagesText;

public slots:
    virtual void itemClicked(int,int,int,const QPoint&);

protected:
    QGridLayout* OfflinesViewLayout;

protected slots:
    virtual void languageChange();

};

#endif // OFFLINESVIEW_H
