/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef REMOVEBUDDYIMPL_H
#define REMOVEBUDDYIMPL_H

#include "RemoveBuddy.h"

/**
   Implementation class of the RemoveBuddy dialog. This one inherits the
   automatically generated RemoveBuddy class. 
   It implements a commit() method which does the actual work.
   @short Remove a Buddy from buddylist dialog.
*/
class RemoveBuddyImpl : public RemoveBuddy
{ 
  Q_OBJECT

public:

  RemoveBuddyImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  
protected:
  /**
     This method calls the yahoolib call to remove a buddy from the list
  */
  virtual void commit();

};

#endif
