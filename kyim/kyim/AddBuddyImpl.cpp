/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <qlineedit.h>

#include "AddBuddyImpl.h"
#include "kyim.h"
#include "IMState.h"

extern struct yahoo_context *context;
extern KYahoo* main_window;
extern IMState* imstate; 

AddBuddyImpl::AddBuddyImpl( QWidget* parent, const char* name, bool modal, WFlags fl ) :
  AddBuddy( parent, name, modal, fl )
{
  // fill in the default group for the buddylist
  Group->insertStringList( imstate->Groups() );

  ID->setFocus();
}
  
void AddBuddyImpl::commit()
{
  QString name, group, message;
  
  name = ID->text();
  if ( name.isEmpty() )
    {
      main_window->warnmsg("Please enter a buddy name.");
      reject();
    }

  group = Group->currentText();
  if ( group.isEmpty() )
    {
      main_window->warnmsg("Please enter a group name.");
      reject();
    }

  message = Msg->text();
  imstate->AddNewBuddy( name, group );

  yahoo_add_buddy( context->id, (const char*)name, (const char*)group, (const char*)message );

  accept();
}

/** sets the ID to add in the widget to the id value */
void AddBuddyImpl::setID(QString id){
	ID->setText(id);
}
