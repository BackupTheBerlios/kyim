/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef CONFINVITATIONIMPL_H
#define CONFINVITATIONIMPL_H

#include "ConfInvitation.h"

/**
   Implementation class of the Conference Invitation dialog. This one inherits
   the automatically generated ConfInvitation class.
   @short Conference Invitation Dialog
*/

class ConfInvitationImpl : public ConfInvitation
{ 
    Q_OBJECT

public:
    /**
       ConfInvitationImpl constructor. This is a modal dialog.

       @param inviter YahooID of person who invited you to conference.
       @param msg Message they sent with the invitation.
       @param reason Pointer to QString object where decline message will be passed back.
    */
    ConfInvitationImpl( const char* inviter, const char* msg, QString* reason, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConfInvitationImpl();

protected slots:
    /**
       User declined to join conference.
       Needs to set reason for why they declined. 
       This will call the reject() method so that the calling app will know
       the user declined to join the conference.
    */
    void decline();

private:
    QString* ReasonMsg;

};

#endif // CONFINVITATIONIMPL_H
