/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "Icon.h"
#include <kpopupmenu.h>
#include <qpopmenu.h>
#include <kapp.h>
#include <qlist.h>
#include <qstringlist.h>
#include <qtooltip.h>

#include "kyim.h"

const char * kyahooSysTray[] =
{
"24 24 31 1",
"       c None",
".      c #FFFFFF",
"+      c #000000",
"@      c #EEEEEE",
"#      c #1C1C1C",
"$      c #383838",
"%      c #AAAAAA",
"&      c #787878",
"*      c #D5D5D5",
"=      c #FF0000",
"-      c #C80000",
";      c #E2E2E2",
">      c #232323",
",      c #550000",
"'      c #E20000",
")      c #555555",
"!      c #CACACA",
"~      c #8D0000",
"{      c #1C0000",
"]      c #9C9C9C",
"^      c #F80000",
"/      c #710000",
"(      c #EBEBEB",
"_      c #565656",
":      c #3D0000",
"<      c #C60000",
"[      c #E50000",
"}      c #E40000",
"|      c #F70202",
"1      c #AA0000",
"2      c #380000",
"                        ",
"                        ",
"   ..  ...              ",
"   ..+ .@#+             ",
"   ..#..$+              ",
"   ..%.&+               ",
"   ...*+ ===      ==-   ",
"   ..;;> ===,    =='++  ",
"   ..).!  ==~   ==={+   ",
"   ..+..] ^='+  ==/+    ",
"   ..+ (._===: ==<+     ",
"   ..+  .; ==/[=}{+     ",
"    ++   ++|='==,+      ",
"           ====1+       ",
"            =='++       ",
"            ==~+        ",
"            ==,+        ",
"           ==={         ",
"           ===+         ",
"           ==<+         ",
"            ,2+         ",
"                        ",
"                        ",
"                        "};


Icon::Icon(KMainWindow* parent, const char* name ) :
  KSystemTray( parent, name )
{
  MenuIDs.setAutoDelete(true);
  
  ((QPopupMenu*)contextMenu())->setCheckable(true);

}

int Icon::insertItem( const QString & text, const QObject * receiver,
   const char * member, int accel, int id  )
{
  MenuIDs.append(new int(id));
  
  return( ((QPopupMenu*)contextMenu())->
	  insertItem( text, (const QObject*)receiver, 
		      (const char*)member, (int)accel, (int)id));
}

void Icon::setItemChecked ( int id, bool check )
{
  ((QPopupMenu*)contextMenu())->setItemChecked( id, check);
}

bool Icon::isItemChecked ( int id )
{
  return( ((QPopupMenu*)contextMenu())->isItemChecked( id ) ); 
}


void Icon::disableMenus()
{
  QListIterator<int> i(MenuIDs);
  
  while( i.current() )
    {
      if ( (*(*i) == ID_SYSTRAY_VIEW_STATUS) ||
	   (*(*i) == ID_SYSTRAY_VIEW_MENU) )
	{
	  ++i;
	  continue;
	}

      // Logon menu has different behavior
      if ( (*(*i) == ID_YAHOO_LOGON) )
	contextMenu()->setItemEnabled( *(*i), true);
      else
	contextMenu()->setItemEnabled( *(*i), false);

      ++i;  
    }
}

void Icon::enableMenus()
{
  QListIterator<int> i(MenuIDs);
  
  while( i.current() )
    {
      if ( (*(*i) == ID_SYSTRAY_VIEW_STATUS) ||
	   (*(*i) == ID_SYSTRAY_VIEW_MENU) )
	{
	  ++i;
	  continue;
	}      

      if ( *(*i) == ID_YAHOO_LOGON )
	contextMenu()->setItemEnabled( *(*i), false);
      else
	contextMenu()->setItemEnabled( *(*i), true);

      ++i;  
    }
}

void Icon::updateBuddyList( QStringList list )
{
  QString tmp;

  list.sort();

  tmp += "Online Buddies:\n";

  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
    tmp += (*it).latin1();
    tmp += "\n";
  }

  // mask off last newline
  tmp.truncate( tmp.length() - 1 );
  QToolTip::remove( this );
  QToolTip::add( this, tmp );

}


/** No descriptions */
void Icon::mousePressEvent(QMouseEvent *event) {
	
	 if(event->button() == LeftButton)
		{
			if(parentWidget()->isVisible()) {
				mainWinPos=parentWidget()->pos();
				parentWidget()->hide();
			}
			else {
				parentWidget()->move(mainWinPos);
				parentWidget()->show();
			}
		}
		else
			KSystemTray::mousePressEvent(event);
}

