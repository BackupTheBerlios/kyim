/****************************************************************************
** Form interface generated from reading ui file './StartChat.ui'
**
** Created: Sun May 23 11:31:05 2004
**      by: The User Interface Compiler ($Id: StartChat.h,v 1.1 2004/05/23 10:13:01 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef STARTCHAT_H
#define STARTCHAT_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QComboBox;

class StartChat : public QWidget
{
    Q_OBJECT

public:
    StartChat( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~StartChat();

    QLabel* TextLabel3;
    QLabel* TextLabel2;
    QLineEdit* Msg;
    QComboBox* ID;

protected:
    QGridLayout* StartChatLayout;

protected slots:
    virtual void languageChange();

};

#endif // STARTCHAT_H
