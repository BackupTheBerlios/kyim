#include "AddFriendDialog.h"
#include "AddFriend.h"

#include <kdialogbase.h>
#include <klocale.h>
#include <kcombobox.h>
#include <klineedit.h>

AddFriendDialog::AddFriendDialog( QWidget* parent, const char* name ):
  KDialogBase::KDialogBase( parent, name, true, i18n( "Add Friend" ),
    KDialogBase::Ok|KDialogBase::Cancel )
{
  AddFriendWidget* mainWidget = new AddFriendWidget( this );
  setMainWidget( mainWidget );
  m_yahooId = mainWidget->yahooId;
  m_groupName = mainWidget->groupName;
}

void AddFriendDialog::setGroupList( const QStringList& groups )
{
  m_groupName->insertStringList( groups );  
}

QString AddFriendDialog::groupName() const
{
  return m_groupName->currentText();
}

QString AddFriendDialog::yahooId() const
{
  return m_yahooId->text();
}
