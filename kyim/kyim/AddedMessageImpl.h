/***************************************************************************
                          AddedMessageImpl.h  -  description
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

#ifndef ADDEDMESSAGEIMPL_H
#define ADDEDMESSAGEIMPL_H

#include <AddedMessage.h>

/**
  *@author Jay Lubomirski
  */

class AddedMessageImpl : public AddedMessage
 {
public: 
	AddedMessageImpl(QString uid, QString msg,QWidget* parent = 0, const char* name = 0, bool modal = false, WFlags fl = 0);
	~AddedMessageImpl();
protected: // Protected attributes
  /**  */
  QString new_uid;
public slots: // Public slots
  /** No descriptions */
  void acceptAdd();
  /** No descriptions */
  void profile();
};

#endif
