/***************************************************************************
                          AddedMessageImpl.cpp  -  description
                             -------------------
    begin                : Thu Jul 26 2001
    copyright            : (C) 2001 by Jay Lubomirski
    email                : jaylubo@fiam.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "AddedMessageImpl.h"
#include <qlineedit.h>
#include "AddBuddyImpl.h"
#include <krun.h>
#include "kyim.h"

extern "C" {
#include "yahoo2.h"
}


extern KYahoo* main_window;

AddedMessageImpl::AddedMessageImpl(QString uid, QString msg,QWidget* parent, const char* name, bool modal, WFlags fl )
 : AddedMessage ( parent, name, modal, fl )
{
  QString text; 

  text = uid;
  if ( msg.isEmpty() ) {
    text += " has added you to their buddylist.";
  }
  else {
    text += " has added you to their buddylist and said ";
    text += msg; 
  }

  message->setText(text);
  new_uid=uid;
}


AddedMessageImpl::~AddedMessageImpl()
{

}


/** No descriptions */
void AddedMessageImpl::acceptAdd()
{
	AddBuddyImpl* addbud = new AddBuddyImpl(this);
	addbud->setID(new_uid);
	addbud->show();
}


/** No descriptions */
void AddedMessageImpl::profile()
{
  QString URLProfile=QString(yahoo_get_profile_url());

  URLProfile+=new_uid;

  KRun::runURL(KURL(URLProfile),QString("text/html"));
}
