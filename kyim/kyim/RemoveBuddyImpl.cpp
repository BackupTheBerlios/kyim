/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <qlineedit.h>
#include <qstringlist.h>

#include "RemoveBuddyImpl.h"
#include "kyim.h"
#include "IMState.h"

extern struct yahoo_context *context;
extern IMState* imstate; 
extern KYahoo* main_window;


RemoveBuddyImpl::RemoveBuddyImpl( QWidget* parent, const char* name, 
				  bool modal, WFlags fl ) :
  RemoveBuddy( parent, name, modal, fl )
{
  const QStringList& list(imstate->Groups());

  // Add current list of buddies 
  ID->insertStringList(imstate->Buddies());

  // Add current list of groups
  Group->insertStringList(list, list.count() );
}
  

void RemoveBuddyImpl::commit()
{
  QString name, group;
  
  name = ID->currentText();
  if ( name.isEmpty() ) {
    main_window->warnmsg("Please enter a buddy name.");
    reject();
  }

  group = Group->currentText();
  if ( group.isEmpty() ) {
    main_window->warnmsg("Please enter a group name.");
    reject();
  }

  imstate->RemoveBuddy(name);

  yahoo_remove_buddy( context->id, (const char*)name, (const char*)group);

  accept();
}
