/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef SETUPIMPL_H
#define SETUPIMPL_H
#include "Setup.h"
/**
   @short Setup dialog
   This class implements the setup dialog. It inherits the GUI from the 
   automatically generated Setup class.
*/
class SetupImpl : public Setup
{ 
    Q_OBJECT

public:
    SetupImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, bool isConnected=false, WFlags fl = 0 );
    ~SetupImpl();

public slots:
    void help_clicked();
    void commit();

protected slots:
    /**
       Create file dialog to replace the logoff sound filename
    */
    void buddy_logoff_clicked();
    
    /**
       Create file dialog to replace the logon sound filename
    */
    void buddy_logon_clicked();

    /**
       Create file dialog to replace the buzz sound filename
    */
    void buzz_clicked();

    /**
       Create file dialog to replace the new message arrived sound filename
    */
    void new_message_clicked();
    
    /**
     * Create file dialog to replace the recd files path
    */
    void pathClicked();

};

#endif // SETUPIMPL_H
