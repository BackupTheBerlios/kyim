#ifndef STARTCHAT_DIALOG_H
#define STARTCHAT_DIALOG_H

#include <kdialogbase.h>

class StartChat;
class QWidget;

class StartChatDialog : public KDialogBase
{
  Q_OBJECT

public:
  StartChatDialog(QWidget* parent=NULL);
  ~StartChatDialog();

private:
  StartChat* ui;
  virtual void slotOk();
};

#endif /* STARTCHAT_DIALOG_H */

