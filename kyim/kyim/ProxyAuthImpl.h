/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROXYAUTHIMPL_H
#define PROXYAUTHIMPL_H

#include "ProxyAuth.h"

/**
   Implementation class of the Proxy Authentication dialog. This one inherits 
   the automatically generated ProxyAuth class.
*/
class ProxyAuthImpl : public ProxyAuth
{ 
  Q_OBJECT

public:

  ProxyAuthImpl( QWidget* parent = 0, const char *msg = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  
protected:
  /**
     Calls libyahoo to add a new buddy to the buddylist.
  */
  virtual void commit();

};
#endif

