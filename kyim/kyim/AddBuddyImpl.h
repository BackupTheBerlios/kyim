/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ADDBUDDYIMPL_H
#define ADDBUDDYIMPL_H

#include "AddBuddy.h"

/**
   Implementation class of the Add Buddy dialog. This one inherits the
   automatically generated AddBuddy class.
*/
class AddBuddyImpl : public AddBuddy
{ 
  Q_OBJECT

public:

  AddBuddyImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  /** sets the ID to add in the widget to the id value */
  void setID(QString id);
  
protected:
  /**
     Calls libyahoo to add a new buddy to the buddylist.
  */
  virtual void commit();

};
#endif
