/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef CONFADDINVITEIMPL_H
#define CONFADDINVITEIMPL_H
#include "ConfAddInvite.h"

extern "C" {
#include "yahoo_list.h"
}


/**
   Implementation class of the Additional Invite to Conference Dialog. This
   one inherits from the automatically generated class ConfAddInvite.
   @short Additional Invitation to Conference dialog.
*/
class ConfAddInviteImpl : public ConfAddInvite
{ 
    Q_OBJECT

public:
    /**
       @param conf_id Conference id for the current conference
    */
    ConfAddInviteImpl( const char* aconf_id, YList* ylist,
		       QWidget* parent = 0, 
		       const char* name = 0, bool modal = FALSE, 
		       WFlags fl = 0 );
    ~ConfAddInviteImpl();

protected slots:
    /**
       This method calls libyahoo to invite another buddy to the conference.
    */
    void commit();

private:
 QString conf_id; 
 YList* conf_list; 
};

#endif // CONFADDINVITEIMPL_H
