/***************************************************************************
                          newmailimpl.h  -  description
                             -------------------
    begin                : Tue Jul 24 2001
    copyright            : (C) 2001 by Alexandre Aufrere
    email                : alexandre.aufrere@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NEWMAILIMPL_H
#define NEWMAILIMPL_H

#include <NewMail.h>

/**
  *@author Alexandre Aufrere
  */

class NewMailImpl : public NewMail  {
public: 
	NewMailImpl(QWidget* parent = 0);
	~NewMailImpl();
  /** No descriptions */

public slots:
  void goToMail();
};

#endif
