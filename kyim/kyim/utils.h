/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/**
   Converts parameter string from upper/lower case to all upper case.
   Return string is allocated on heap and needs to be freed.

   @param in String to be lower cased.
   @returns new string all in lower case.
*/
char * strlower( const char *in);

int add_proxy_auth_str( const char *user, const char *passwd );
