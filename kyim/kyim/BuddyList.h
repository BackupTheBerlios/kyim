/****************************************************************************
** Form interface generated from reading ui file './BuddyList.ui'
**
** Created: Sun May 23 11:31:05 2004
**      by: The User Interface Compiler ($Id: BuddyList.h,v 1.1 2004/05/23 10:12:42 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include <qvariant.h>
#include <qwidget.h>
#include <kcombobox.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QListView;
class QListViewItem;
class QFrame;
class QLabel;
class KComboBox;

class BuddyList : public QWidget
{
    Q_OBJECT

public:
    BuddyList( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~BuddyList();

    QListView* BuddyListView;
    QFrame* Frame3;
    QLabel* TextLabel1;
    KComboBox* MyStatus;

public slots:
    virtual void MyStatusChanged( int indx );
    virtual void buddyClicked( QListViewItem * );

protected:
    QVBoxLayout* BuddyListLayout;
    QVBoxLayout* Layout1;
    QHBoxLayout* Frame3Layout;

protected slots:
    virtual void languageChange();

};

#endif // BUDDYLIST_H
