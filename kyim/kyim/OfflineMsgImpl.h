/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef OFFLINEMSGIMPL_H
#define OFFLINEMSGIMPL_H

#include "OfflineMsg.h"

/**
   The implementation class of the Offline Message dialog. This one inherits
   the automatically generated OfflineMsg class. 
   The commit() method does the work of the dialog.
   @short Send an offline message dialog.
*/
class OfflineMsgImpl : public OfflineMsg
{ 
    Q_OBJECT

public:
    OfflineMsgImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );

 protected:
    /**
       This method calls libyahoo to send the offline message
    */
    virtual void commit();
};


#endif
