/****************************************************************************
** Form interface generated from reading ui file './Setup.ui'
**
** Created: Sun May 23 11:31:04 2004
**      by: The User Interface Compiler ($Id: Setup.h,v 1.1 2004/05/23 10:13:01 ariya Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SETUP_H
#define SETUP_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QTabWidget;
class QWidget;
class QFrame;
class QLabel;
class QLineEdit;
class QButtonGroup;
class QCheckBox;
class QSpinBox;
class QComboBox;
class QGroupBox;
class QPushButton;

class Setup : public QDialog
{
    Q_OBJECT

public:
    Setup( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Setup();

    QTabWidget* tabWidget;
    QWidget* Widget2;
    QFrame* Frame6;
    QLabel* TextLabel1;
    QLineEdit* username;
    QLineEdit* password;
    QLabel* TextLabel3;
    QLabel* TextLabel2;
    QButtonGroup* ButtonGroup1;
    QCheckBox* invisible_mode;
    QCheckBox* login_on_startup_box;
    QWidget* Widget3;
    QCheckBox* msg_from_friend_box;
    QCheckBox* show_online_friends_box;
    QCheckBox* resize_window;
    QCheckBox* emoticons;
    QCheckBox* mailwarning;
    QCheckBox* statusTime;
    QCheckBox* closeLogoff;
    QCheckBox* autoraise;
    QButtonGroup* ButtonGroup4;
    QLabel* TextLabel4;
    QSpinBox* idle_time;
    QCheckBox* idle_state;
    QWidget* tab;
    QLabel* TextLabel5;
    QComboBox* connect_mode;
    QGroupBox* GroupBox1;
    QLabel* TextLabel6;
    QLabel* TextLabel7;
    QLineEdit* proxy_host;
    QLineEdit* proxy_port;
    QGroupBox* GroupBox2;
    QLineEdit* recdPath;
    QPushButton* buttonRecdPath;
    QWidget* tab_2;
    QFrame* Frame13;
    QCheckBox* play_new_message;
    QLineEdit* new_message;
    QPushButton* PushButton23;
    QFrame* Frame12;
    QCheckBox* play_buddy_logoff;
    QLineEdit* buddy_logoff;
    QPushButton* PushButton22;
    QFrame* Frame11;
    QCheckBox* play_buddy_logon;
    QLineEdit* buddy_logon;
    QPushButton* PushButton21;
    QFrame* Frame8;
    QCheckBox* play_buzz;
    QLineEdit* buzz;
    QPushButton* PushButton8;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void commit();
    virtual void help_clicked();
    virtual void pathClicked();

protected:
    QVBoxLayout* SetupLayout;
    QVBoxLayout* Widget2Layout;
    QVBoxLayout* Frame6Layout;
    QGridLayout* Layout5;
    QVBoxLayout* ButtonGroup1Layout;
    QVBoxLayout* Widget3Layout;
    QHBoxLayout* Layout6;
    QVBoxLayout* tabLayout;
    QHBoxLayout* Layout8;
    QVBoxLayout* GroupBox1Layout;
    QGridLayout* Layout7;
    QHBoxLayout* GroupBox2Layout;
    QGridLayout* tabLayout_2;
    QVBoxLayout* Frame13Layout;
    QVBoxLayout* Layout12;
    QHBoxLayout* Layout11;
    QVBoxLayout* Frame12Layout;
    QVBoxLayout* Layout13;
    QHBoxLayout* Layout10;
    QVBoxLayout* Frame11Layout;
    QVBoxLayout* Layout14;
    QHBoxLayout* Layout9;
    QGridLayout* Frame8Layout;
    QVBoxLayout* Layout13_2;
    QHBoxLayout* Layout12_2;
    QHBoxLayout* Layout1;

protected slots:
    virtual void languageChange();

    virtual void buddy_logoff_clicked();
    virtual void buddy_logon_clicked();
    virtual void buzz_clicked();
    virtual void new_message_clicked();


};

#endif // SETUP_H
