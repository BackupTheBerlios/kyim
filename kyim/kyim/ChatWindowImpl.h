#ifndef CHATWINDOWIMPL_H
#define CHATWINDOWIMPL_H

#include "ChatWindow.h"
#include <ktextbrowser.h>

class MsgParser;
class QShowEvent;
class QCloseEvent;

class ChatWindowImpl : public ChatWindow
{
    Q_OBJECT

public:
    /**
       Chat Window widget constructor
       @param parent The parent widget
       @param name Object name
       @param flags Widget flags
     */
    ChatWindowImpl( QWidget* parent = 0,
		    const char* name = 0, WFlags fl = 0 );
    ~ChatWindowImpl();

    /**
       A new message has arrived from the network, update the chat window
       @param text Message text
       @param from Who wrote the message
    */
    void appendMessage( const QString& text, const QString& from );

    void copy();
    void cut();
    void paste();

    /**
       handle when the use default menu flag is changed
    */
    void forceDefaultFont(bool flag);

    /**
	Save plain text log file
    */
    QString _textLog;
    bool lbreak;

protected:
    virtual void showEvent ( QShowEvent * );
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);


private slots:
    /**
       Slot to handle when a new message has been typed by the user
    */
    void newMessage();
    void textPressed();

signals:
    /**
	Signal to send a message via libyahoo. Basically signal redirection
	from the newMessage() slot.
    */
    void sendMessage(QString msg);

private:
    MsgParser* parse; // message parser
    QString me; // my username (for convience )

};

#endif // CHATWINDOWIMPL_H
