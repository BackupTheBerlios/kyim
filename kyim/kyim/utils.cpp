/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>


#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>

extern struct yahoo_context *context;

/* basic programming utilities */

/* Take a string adn return the same string with all letters converted
   to lowercase */

char * strlower( const char *in)
{
  char *tmp;
  int len, i;

  if ( !in )
    return NULL;

  len = strlen(in);
  tmp = ( char * ) malloc ( len + 1 );

  for ( i = 0; i < len; i++ )
    tmp[i] = tolower(in[i]);

  tmp[i] = '\0';

  return tmp;

} /* strlower */


/* Add Proxy User and Proxy Passwd to yahoo_options. It will be used by Yahoo
 * Library later when yahoo_init is called
 */
int
add_proxy_auth_str( const char *user, const char *passwd )
{
#ifdef LIBYAHOO
	yahoo_options.proxy_user = strdup( user );
	yahoo_options.proxy_passwd = strdup( passwd );
#endif
	return 1;
}
