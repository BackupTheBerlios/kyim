/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <qlineedit.h>

#include "ProxyAuthImpl.h"
#include "kyim.h"
#include "utils.h"

extern struct yahoo_context *context;
extern KYahoo* main_window;

ProxyAuthImpl::ProxyAuthImpl( QWidget* parent, const char *msg, const char* name, bool modal, WFlags fl ) :
  ProxyAuth( parent, name, modal, fl )
{
  Password->setEchoMode( Password->Password );
  Username->setFocus();

  if ( msg != NULL )
  	TextLabel1->setText( tr( msg ) );
}
  
void ProxyAuthImpl::commit()
{
  QString name, passwd;
  
  name = Username->text();
  if ( name.isEmpty() )
    reject();

  passwd = Password->text();
  if ( passwd.isEmpty() )
    reject();
  
  add_proxy_auth_str( name, passwd );
  accept();
  main_window->connect();
}

