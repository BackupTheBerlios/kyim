/****************************************************************************
** Form interface generated from reading ui file './ChatWindow.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: ChatWindow.h,v 1.1 2004/05/23 10:12:42 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <qvariant.h>
#include <qwidget.h>
#include <ktextbrowser.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class KTextBrowser;
class QTextEdit;
class QPushButton;

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    ChatWindow( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ChatWindow();

    KTextBrowser* text;
    QTextEdit* enteredText;
    QPushButton* sendButton;

public slots:
    virtual void newMessage();
    virtual void textPressed();

protected:
    QVBoxLayout* ChatWindowLayout;
    QHBoxLayout* layout3;

protected slots:
    virtual void languageChange();

};

#endif // CHATWINDOW_H
