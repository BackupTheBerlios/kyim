/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qstring.h>
#include <qlineedit.h>
#include <qlabel.h>

#include "ConfInvitationImpl.h"

/* 
 *  Constructs a ConfInvitationImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConfInvitationImpl::ConfInvitationImpl( const char* inviter, const char* msg, QString* reason, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : ConfInvitation( parent, name, modal, fl )
{
  Inviter->setText(inviter);
  Message->setText(msg );

  ReasonMsg = reason;
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ConfInvitationImpl::~ConfInvitationImpl()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * protected slot
 */
void ConfInvitationImpl::decline()
{
    (*ReasonMsg) = Reason->text();
    reject();
}
