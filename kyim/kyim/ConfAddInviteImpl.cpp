/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <qlineedit.h>
#include <stdlib.h>

extern "C" {
#include "yahoo2.h"
}

#include "ConfAddInviteImpl.h"
#include "kyim.h"


extern KYahoo* main_window;
extern struct yahoo_context *context;


/* 
 *  Constructs a ConfAddInviteImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConfAddInviteImpl::ConfAddInviteImpl( const char* aconf_id, YList* ylist,  
				      QWidget* parent, const char* name, 
				      bool modal, WFlags fl )
  : ConfAddInvite( parent, name, modal, fl )
{
  conf_id = aconf_id; 
  conf_list = ylist; 

  Name->setFocus();
}


ConfAddInviteImpl::~ConfAddInviteImpl()
{
}

/* 
 * protected slot
 */
void ConfAddInviteImpl::commit()
{
  char* tmp;

  tmp = strdup( Name->text() );
  // add to list for message 
  conf_list = y_list_append(conf_list, tmp );

  yahoo_conference_addinvite(context->id, NULL, 
			     Name->text(), conf_id, conf_list,
			     "Please Join our conference...");
  // remove from list again
  // will be re-added if user accepts
  conf_list = y_list_remove(conf_list, tmp );
  free( tmp );

  accept();
}




