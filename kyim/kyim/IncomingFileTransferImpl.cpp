#include "IncomingFileTransferImpl.h"
#include "kfiledialog.h"
#include "qlineedit.h"
#include <qmultilineedit.h>
#include "kio/netaccess.h"
#include "kurl.h"

IncomingFileTransferImpl::IncomingFileTransferImpl(char* URL, char* FROM, char* DESCRIPTION, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : IncomingFileTransfer( parent, name, modal, fl ) {

  url=new QString(URL);
  lbl_desc->setText(DESCRIPTION);
  lbl_frm->setText(FROM);

}

IncomingFileTransferImpl::~IncomingFileTransferImpl() {
 delete(url);
}


void IncomingFileTransferImpl::cancel_clicked() {
  close(true);
}


void IncomingFileTransferImpl::okay_clicked() {
  if (edit_dest->text().isEmpty()) return;
  KURL test((const QString)*url);
  KURL test2(edit_dest->text());

  if (KIO::NetAccess::copy(test,test2))
      close(true);
}


void IncomingFileTransferImpl::browse_clicked() {
  KURL temp((const QString)*url);

  QString fname = KFileDialog::getSaveFileName( 0, temp.fileName(false), this );
  if ( !fname.isEmpty() ) {
    edit_dest->setText( fname );
  }

}
