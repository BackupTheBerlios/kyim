#include <kdialogbase.h>
#include <qstringlist.h>

class KLineEdit;
class KComboBox;

class AddFriendDialog : public KDialogBase
{
public:
  AddFriendDialog( QWidget* parent = 0, const char* name = 0 );

  void setGroupList( const QStringList& groups );

  QString groupName() const;

  QString yahooId() const;
  
private:
  KLineEdit* m_yahooId;
  KComboBox* m_groupName;
};

