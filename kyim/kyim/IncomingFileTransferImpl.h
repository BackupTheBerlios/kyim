#ifndef INCOMING_FILE_IMPL
#define INCOMING_FILE_IMPL
#include "IncomingFileTransfer.h"
#include <qstring.h>
#include <qlabel.h>
class IncomingFileTransferImpl : public IncomingFileTransfer {

Q_OBJECT

public:
          IncomingFileTransferImpl(char* URL, char* FROM, char* DESCRIPTION, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl =0);
          ~IncomingFileTransferImpl();
protected:
         QString* url;
protected slots:
    void cancel_clicked();
    void okay_clicked();
    void browse_clicked();


};
#endif
