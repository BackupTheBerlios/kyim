/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdlib.h>

#include <qlineedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <kfiledialog.h>

extern "C" {
#include "yahoo2.h"
}

#include "SetupImpl.h"

#include "kyim.h"
extern KYahoo* main_window;


/* 
 *  Constructs a SetupImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SetupImpl::SetupImpl( QWidget* parent,  const char* name, bool modal, bool isConnected, WFlags fl )
    : Setup( parent, name, modal, fl )
{
  if ( main_window->options.current_user )
    username->setText( main_window->options.current_user );
  
  if ( main_window->options.current_pass )
    password->setText( main_window->options.current_pass );  

  if ( main_window->options.startup_status == YAHOO_STATUS_INVISIBLE )
    invisible_mode->setChecked(true);

	if (isConnected) {
		username->setEnabled(false);
		password->setEnabled(false);
		connect_mode->setEnabled(false);
		proxy_host->setEnabled(false);
		proxy_port->setEnabled(false);
	}

  login_on_startup_box->setChecked(main_window->options.login_at_startup);

  msg_from_friend_box->setChecked(main_window->options.msg_friend_only);

  show_online_friends_box->
    setChecked( main_window->options.show_online_friends_only );

  resize_window->setChecked(main_window->options.resize_window );

  emoticons->setChecked(main_window->options.use_emoticons );

  mailwarning->setChecked(main_window->options.disable_mail_warning );

  statusTime->setChecked(main_window->options.show_status_time );

  closeLogoff->setChecked(main_window->options.logoff_close_windows );

  autoraise->setChecked( main_window->options.autoraise_window );

  recdPath->setText(main_window->options.recd_path);

  idle_state->setChecked(main_window->options.allow_idle );
  
  idle_time->setValue( main_window->options.idle_time );
  
  connect_mode->insertItem("Normal");
  connect_mode->setCurrentItem(0);

  /*  HTTP/HTTP-proxy not supported 

  connect_mode->insertItem("HTTP"); 
      
  if ( main_window->options.connect_mode == YAHOO_CONNECTION_PAGER )
   connect_mode->setCurrentItem(0);
  else 	
   connect_mode->setCurrentItem(1);
  */

  if ( main_window->options.proxy_host != NULL )
    proxy_host->setText( main_window->options.proxy_host );

  QString t_lbl4;
  t_lbl4.setNum( main_window->options.proxy_port );
  proxy_port->setText( t_lbl4 );

  play_buddy_logon->setChecked(main_window->options.play_buddy_logon );
  buddy_logon->setText( main_window->options.buddy_logon);

  play_buddy_logoff->setChecked(main_window->options.play_buddy_logoff );
  buddy_logoff->setText( main_window->options.buddy_logoff);

  play_new_message->setChecked(main_window->options.play_new_message );
  new_message->setText( main_window->options.new_message);

  play_buzz->setChecked(main_window->options.play_buzz);
  buzz->setText(main_window->options.buzz);


}

/*  
 *  Destroys the object and frees any allocated resources
 */
SetupImpl::~SetupImpl()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void SetupImpl::help_clicked()
{
  kapp->invokeHelp( "kyim/index.html", "kyim" );
}
/* 
 * public slot
 */
void SetupImpl::commit()
{
  main_window->options.current_user = username->text();
  main_window->options.current_pass = password->text();

  if ( invisible_mode->isChecked() )
    main_window->options.startup_status = YAHOO_STATUS_INVISIBLE;
  else
    main_window->options.startup_status = YAHOO_STATUS_AVAILABLE; 

  main_window->options.login_at_startup = login_on_startup_box->isChecked();  
  
  main_window->options.msg_friend_only = msg_from_friend_box->isChecked();

  main_window->options.show_online_friends_only = show_online_friends_box->isChecked();

  main_window->options.resize_window = resize_window->isChecked();
  
  main_window->options.use_emoticons = emoticons->isChecked();

  main_window->options.disable_mail_warning = mailwarning->isChecked();

  main_window->options.show_status_time = statusTime->isChecked();

  main_window->options.logoff_close_windows = closeLogoff->isChecked();

  main_window->options.autoraise_window = autoraise->isChecked();

  main_window->options.recd_path=recdPath->text();
  main_window->options.allow_idle = idle_state->isChecked();

  int tmp_idle_time = idle_time->value();
  main_window->debugmsg("idle_time set to %d\n", tmp_idle_time );
  if ( ( tmp_idle_time > 0 ) && ( tmp_idle_time < 61 ) )
    main_window->options.idle_time = tmp_idle_time;

  /*** third page ***************************/

  if ( connect_mode->currentItem() == 0 )
    main_window->options.connect_mode = YAHOO_CONNECTION_PAGER;
  else 
    main_window->options.connect_mode = YAHOO_CONNECTION_FT;

  main_window->options.proxy_host = proxy_host->text();

  main_window->options.proxy_port = atoi( (const char*)proxy_port->text() );

  /*** fourth page **************************/
  main_window->options.play_buddy_logon = play_buddy_logon->isChecked();
  main_window->options.play_buddy_logoff = play_buddy_logoff->isChecked();
  main_window->options.play_new_message = play_new_message->isChecked();
  main_window->options.play_buzz = play_buzz->isChecked();

  main_window->options.buddy_logon = buddy_logon->text();
  main_window->options.buddy_logoff = buddy_logoff->text();
  main_window->options.new_message = new_message->text();
  main_window->options.buzz = buzz->text();
  
  accept();
}

/* 
 * protected slot
 */
void SetupImpl::buddy_logoff_clicked()
{
  QString fname = KFileDialog::getOpenFileName( 0, "*.wav", this );
  if ( !fname.isEmpty() ) {
    buddy_logoff->setText( fname );
  }

}
/* 
 * protected slot
 */
void SetupImpl::buddy_logon_clicked()
{
  QString fname = KFileDialog::getOpenFileName( 0, "*.wav", this );
  if ( !fname.isEmpty() ) {
    buddy_logon->setText( fname );
  }
}
/* 
 * protected slot
 */
void SetupImpl::new_message_clicked()
{
  QString fname = KFileDialog::getOpenFileName( 0, "*.wav", this );
  if ( !fname.isEmpty() ) {
    new_message->setText( fname );
  }
}

/*
 * protected slot
 */
void SetupImpl::buzz_clicked()
{
  QString fname = KFileDialog::getOpenFileName( 0, "*.wav", this );
  if ( !fname.isEmpty() ) {
    buzz->setText( fname );
  }
}

/*
 * protected slot
 */
void SetupImpl::pathClicked()
{
  QString fname = KFileDialog::getExistingDirectory();
  if ( !fname.isEmpty() ) {
    recdPath->setText( fname );
  }
}

