/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <qdialog.h>
#include <qtabdialog.h>
#include <qmultilinedit.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qspinbox.h>

#ifndef dialogs_h
#define dialogs_h

/* ***************************************************************************/

/**
   @short Start a conference dialog
*/
class Conference_Invite_Window : public QDialog
{
  Q_OBJECT

public:
  Conference_Invite_Window();

private slots:
  /**	
     Add name from typed in id to invited buddies GUI list
  */
  void add_invite();
  /**
     Close dialog and start conference
  */
  void done_clicked(); 
  /**
     Add name from buddies online to invited buddies GUI list
  */
  void name_clicked( QListViewItem* ); 
  
private:
  QListView* buddies;
  QMultiLineEdit* invited_list; 
  QLineEdit* msg_line; 
  QLineEdit* invite_line; 

  QStrList* userlist;
  int type;

};

/* *************************************************************************/

/**
   @short Simple debug message text window.
*/
class Debug_Msg_Dialog : public QDialog
{
  Q_OBJECT

public:
  Debug_Msg_Dialog();
  void appendmsg(char *msg);

private:
  void closeEvent( QCloseEvent * );
  QMultiLineEdit* debug_msgs;

};

#endif /* ifndef dialogs_h */



