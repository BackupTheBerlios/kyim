/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <qlineedit.h>

#include "OfflineMsgImpl.h"
#include "kyim.h"
#include "IMState.h"

extern struct yahoo_context *context;
extern IMState* imstate;
extern KYahoo* main_window;

OfflineMsgImpl::OfflineMsgImpl( QWidget* parent, const char* name, 
				bool modal, WFlags fl) 
  : OfflineMsg( parent, name, modal, fl )
{

  Name->insertStringList(imstate->BuddiesOffline() );

  Msg->setFocus();
}

void OfflineMsgImpl::commit()
{
  main_window->debugmsg("OfflineMsgDialog::commit user %s, msg |%s|",
			(const char*)Name->currentText(), 
			(const char*)Msg->text() );

  if ( Msg->text().isEmpty()  ) {
    main_window->warnmsg("Please enter a message.");
    reject();
  }

  imstate->sendMessage( main_window->current_user(), Name->currentText(),
    Msg->text() );

  accept();
}
