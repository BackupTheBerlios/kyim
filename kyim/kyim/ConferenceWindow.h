/****************************************************************************
** Form interface generated from reading ui file './ConferenceWindow.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: ConferenceWindow.h,v 1.1 2004/05/23 10:12:42 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONFERENCEWINDOW_H
#define CONFERENCEWINDOW_H

#include <qvariant.h>
#include <qwidget.h>
#include <ktextbrowser.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class KTextBrowser;
class QListBox;
class QListBoxItem;
class QPushButton;
class QTextEdit;

class ConferenceWindow : public QWidget
{
    Q_OBJECT

public:
    ConferenceWindow( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ConferenceWindow();

    KTextBrowser* text;
    QListBox* actionsList;
    QListBox* userList;
    QPushButton* inviteBuddy;
    QPushButton* quitButton;
    QTextEdit* enteredText;
    QPushButton* sendButton;

public slots:
    virtual void invitePressed();
    virtual void newMessage();
    virtual void actionClicked(QListBoxItem*);
    virtual void quitPressed();
    virtual void textPressed();

protected:
    QGridLayout* ConferenceWindowLayout;
    QHBoxLayout* Layout3;
    QHBoxLayout* Layout2;
    QGridLayout* layout5;
    QHBoxLayout* layout4;

protected slots:
    virtual void languageChange();

};

#endif // CONFERENCEWINDOW_H
