/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qpopupmenu.h>
#include <kpopupmenu.h>
#include <kmenubar.h>
#include <qaccel.h>
#include <kiconloader.h>
#include <ktextbrowser.h>
#include <qfile.h>
#include <qtextstream.h>
#include <kfiledialog.h>
#include <qdatetime.h>
#include <klocale.h>
#include "ChatBase.h"
#include "IMState.h"
#include "kyim.h"

extern KYahoo* main_window;
extern IMState* imstate; 

ChatBase::ChatBase( QWidget* parent, const char *name )
  : KMainWindow(parent, name, WDestructiveClose)
{
  initMenuBar();

	QMimeSourceFactory::setDefaultFactory((QMimeSourceFactory *)(kapp->mimeSourceFactory()));
}


void ChatBase::initMenuBar()
{
  // Icons: /opt/kde2/share/icons/hicolor/16x16/actions

  menuBar()->clear();

  KPopupMenu* fileMenu = new KPopupMenu(menuBar(), "FileMenu");
  fileMenu->insertItem(SmallIcon("filesave"), i18n("&Save (Formatted)"), this, SLOT(saveHTML()),
		       ALT+Key_S );
  fileMenu->insertItem(SmallIcon("filesave"), i18n("S&ave (Unformatted)"), this, SLOT(saveText()), ALT+Key_A );
  fileMenu->insertItem( i18n("&Close"), this, SLOT(quit()) );
  menuBar()->insertItem( i18n("&File"), fileMenu );
  
  KPopupMenu* editMenu = new KPopupMenu(menuBar(), "EditMenu");
  editMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(copy()), CTRL+Key_C );
  editMenu->insertItem(SmallIcon("editcut"), i18n("C&ut"), this, SLOT(cut()), CTRL+Key_X );
  editMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(paste()), CTRL+Key_V );
  menuBar()->insertItem( i18n("&Edit"), editMenu);


#if 0
	KPopupMenu* buddyMenu = new KPopupMenu(menuBar(), "BuddyMenu");
	buddyMenu->insertItem("&Profile",this,SLOT(profile()),CTRL+Key_P);
  menuBar()->insertItem("&Buddy", buddyMenu);

  KPopupMenu viewMenu = new KPopupMenu(menuBar, "ViewMenu");
  menuBar->insertItem("&View", viewMenu);

  KPopupMenu insertMenu = new KPopupMenu(menuBar, "InsertMenu");
  menuBar->insertItem("&Insert", insertMenu);

  KPopupMenu insertFaceMenu = new KPopupMenu(insertMenu, "FaceMenu");

  KPopupMenu helpMenu = new KPopupMenu(kmenuBar, "HelpMenu");
  menuBar->insertItem("&Help", helpMenu);
	
#endif

} // initMenuBar

void ChatBase::resetIdle()
{
  imstate->reset_idle();
}

/* register emoticons */
/*void ChatBase::registerIcons()
{
  QMimeSourceFactory::defaultFactory()->setPixmap( "smiley_icon", QPixmap(smiley_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "tongue_icon", QPixmap(tongue_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "blink_icon", QPixmap(blink_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "surp_icon", QPixmap(surp_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "sad_icon", QPixmap(sad_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "angry_icon", QPixmap(angry_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "bigsm_icon", QPixmap(bigsm_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "laugh_icon", QPixmap(laugh_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "devil_icon", QPixmap(devil_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "glass_icon", QPixmap(glass_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "sungl_icon", QPixmap(sungl_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "cry_icon", QPixmap(cry_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "quest_icon", QPixmap(quest_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "love_icon", QPixmap(love_xpm) );
  QMimeSourceFactory::defaultFactory()->setPixmap( "sarc_icon", QPixmap(sarc_xpm) );

}
*/

void ChatBase::saveText()
{
  QString fname = KFileDialog::getSaveFileName( 0, "*.txt", this, "Enter filename to save capture file." );
  if ( fname.isEmpty() ) 
    {
      qWarning("ChatSession::saveText filename is NULL");
      return;
    }
  
  QFile* outfile = new QFile( fname );
  outfile->open( IO_WriteOnly );
  
  QTextStream outstream ( outfile );
  outstream << "Chat Log taken on " << QDate::currentDate().toString() << endl;
  outstream << "============================================" << endl;

  outstream << text();
  outfile->close();
}

void ChatBase::saveHTML()
{
  QString fname = KFileDialog::getSaveFileName( 0, "*.html", this, "Enter filename to save capture file." );
  if ( fname.isEmpty() ) 
    {
      qWarning("ChatSession::saveHTML filename is NULL");
      return;
    }

  QFile* outfile = new QFile( fname );
  outfile->open( IO_WriteOnly );

  QTextStream outstream ( outfile );
  outstream << "Chat Log taken on " << QDate::currentDate().toString() << endl;
  outstream << "=============================================================" << endl;

  outstream << HTML();
  outfile->close();
}


