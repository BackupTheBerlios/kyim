/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ICON_H
#define ICON_H

#include <ksystemtray.h>
#include <kmainwindow.h>
#include <qlist.h>
#include <qevent.h>
#include <qpoint.h>

class QStringList;

/**
 *
 * @short The KYahoo Icon GUI element, manages the systray object. 
 *
 * This class handles the KSystemTray object. It subclassed the KSystemTray
 * class in order to customize the menu. 
 */
class Icon : public KSystemTray
{
  Q_OBJECT

public:

  /** 
      Constructor 
  */
  Icon( KMainWindow* parent = 0, const char* name  = 0 );

  ~Icon( ) { }
 
  /** 
      Insert a new menu item into the icon menu 
      @param text The menu text
      @param receiver The object that will receive the signal
      @param member The slot method of the receiver object
      @param accel Accelerator key
      @param id Menu ID
   */
  int insertItem(const QString & text, const QObject * receiver,
		  const char * member, int accel = 0, int id = -1);

  /** Set item in menu checked */
  void setItemChecked ( int id, bool check );

  /** Is item checked ? */
  bool isItemChecked ( int id );

  /** Disable the menu's when logged out */
  void disableMenus();

  /** enable the menu's when logged in */
  void enableMenus();

  /** Update the tool tip with the latest list of online buddy's */
  void updateBuddyList( QStringList qs );

  /** No descriptions */
  virtual void mousePressEvent(QMouseEvent *event) ;

private:

  QList<int> MenuIDs;

protected: // Protected attributes
  /**  */
  QPoint mainWinPos;
};


#endif


