#ifndef CONFERENCEWINDOWIMPL_H
#define CONFERENCEWINDOWIMPL_H

#include "ConferenceWindow.h"

class MsgParser;
class QShowEvent;
class QListBoxItem;

class ConferenceWindowImpl : public ConferenceWindow
{ 
    Q_OBJECT

public:
    ConferenceWindowImpl( const char* conf_id, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ConferenceWindowImpl();
    
    /**
       A new message has arrived from the network, update the chat window
    */
    void appendMessage( const char *text, const char* from );

    //
    // userlist manipulation
    //
    void addUser( QString& user );
    void removeUser( QString& user );
    QStringList getUserList( void );

    /**
     * Fill the action List
     */
    void fillActionList();
    /*
     * send the Action message
     */
    void sendAction(QString);

    void copy();
    void cut();
    void paste();

    bool findUser( const QString& user );

    /**
       Save plain text log file
    */
    QString _textLog;
    bool lbreak;


public slots:
    void newMessage();
    void textPressed();
    void invitePressed();
    void quitPressed();
    void actionClicked(QListBoxItem*);

protected:
    virtual void showEvent ( QShowEvent * );
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

signals:
    void sendMessage(QString msg);
    void quitChat();
    void inviteUser(void);

private:
    MsgParser* parse; // message parser
    QString me; // my username (for convience )

    QString confId;
};

#endif // CONFERENCEWINDOWIMPL_H
