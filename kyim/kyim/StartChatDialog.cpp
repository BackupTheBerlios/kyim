
#include <StartChatDialog.h>
#include <StartChat.h>
#include "ChatSession.h"
#include "IMState.h"
#include "kyim.h"

extern "C" {
#include "yahoo2.h"
}

extern KYahoo* main_window;
extern IMState* imstate;
extern struct yahoo_context *context;

#include <qstring.h>
#include <qlineedit.h>
#include <qcombobox.h>

#include <kdialogbase.h>
#include <klocale.h>

StartChatDialog::StartChatDialog(QWidget* parent)
  : KDialogBase(parent, 0, true, QString::null, Ok|Cancel, No, true)
{
  ui = new StartChat( this );
  setMainWidget( ui );

  ui->ID->insertStringList(imstate->BuddiesOnline());
  QString buddy = imstate->BuddySelected();
  if( !buddy.isEmpty() )
    ui->ID->setCurrentText( buddy );
  ui->Msg->setFocus();

  setCaption( i18n("Start Chat Session" ) );
  setButtonOKText( i18n("Chat" ), i18n("Chat") );
}

StartChatDialog::~StartChatDialog()
{
}

void StartChatDialog::slotOk()
{
  /* reset idle timer */
  imstate->reset_idle();

  imstate->start_chat(ui->ID->currentText());

  imstate->sendMessage( main_window->current_user(), 
    ui->ID->currentText(), ui->Msg->text() );

  close();
}

#include "StartChatDialog.moc"
