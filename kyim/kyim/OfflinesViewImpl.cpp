/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "OfflinesViewImpl.h"
#include <qtable.h>
#include <ktextbrowser.h>
#include "Parse.h"

#include <stdio.h>

/* 
 *  Constructs a OfflinesViewImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
OfflinesViewImpl::OfflinesViewImpl( QWidget* parent,  const char* name, 
				    bool modal, WFlags fl )
  : OfflinesView( parent, name, modal, fl )
{
  parse = new MsgParser();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
OfflinesViewImpl::~OfflinesViewImpl()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 * public slot
 */
#define CELL_TEXT(y,x) messagesTable->text(y,x)

void OfflinesViewImpl::itemClicked(int row,int col,int button, 
				   const QPoint& mousePos)
{ 
//  printf("itemClicked!:->  %s\n",(const char*)CELL_TEXT(row,1));
  QString newText(CELL_TEXT(row,1));
  QString newLog(CELL_TEXT(row,1));

  parse->formatLog( newLog );
  parse->formatMsg( newText );

  newText.insert(0,"<b><font color=\"blue\">["+ QString(CELL_TEXT(row,0))+" "+QString(CELL_TEXT(row,2))+"]</font></b> ");
  messagesText->setText(newText);
  messagesText->ensureVisible(0,messagesText->contentsHeight());  
  messagesText->viewport()->repaint(true);

}
