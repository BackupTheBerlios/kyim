/***************************************************************************
                          newmailimpl.cpp  -  description
                             -------------------
    begin                : Tue Jul 24 2001
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

#include "NewMailImpl.h"

#include <krun.h>

NewMailImpl::NewMailImpl(QWidget* parent)
	: NewMail(parent){
}
NewMailImpl::~NewMailImpl(){
}
/** No descriptions */
void NewMailImpl::goToMail(){
  QString URLProfile=QString("http://mail.yahoo.com/");
  KRun::runURL(KURL(URLProfile),QString("text/html"));
  accept();
}
