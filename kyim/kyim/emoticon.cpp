
#include "emoticon.h"
#include "emoticons.h"

#include <qevent.h>
#include <qframe.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qtooltip.h>

class EmoticonPanelPrivate
{
public:
  int width;
  int row;
  int col;
  bool filled;
};

class EmoticonButtonPrivate
{
public:
  int id;
  QPixmap pixmap;
};

EmoticonMenu::EmoticonMenu( QWidget* parent, const char* name )
: QPopupMenu( parent, name )
{
  EmoticonPanel* panel = new EmoticonPanel( this, "emoticon_panel" );
  insertItem( panel );

  connect( panel, SIGNAL( emoticonSelected(int) ), SIGNAL( emoticonSelected(int) ) );
  connect( panel, SIGNAL( sizeChanged() ), SLOT( updateSize() ) );
}

EmoticonMenu::~EmoticonMenu()
{
}

void EmoticonMenu::updateSize()
{
  styleChange( style() );
}

EmoticonPanel::EmoticonPanel( QWidget* parent, const char* name )
: QWidget(parent,name)
{
  d = new EmoticonPanelPrivate;
  d->width = 5;
  d->row = d->col = 0;
  d->filled = false;

  layout = new QGridLayout( this, 0, d->width+1, 0, 0 );
  layout->setMargin( 1 );
  layout->setSpacing( 2 );

  emit sizeChanged();
}

EmoticonPanel::~EmoticonPanel()
{
  delete d;
}

void EmoticonPanel::fillEmoticons()
{
  insertEmoticon( SMILEY_ID,   "Smile",      DATADIR "/pics/emoticon_smile.png" );
  insertEmoticon( TONGUE_ID,   "Tongue",     DATADIR "/pics/emoticon_tongue.png" );
  insertEmoticon( BLINK_ID,    "Blink",      DATADIR "/pics/emoticon_blink.png" );
  insertEmoticon( SURP_ID,     "Surp",       DATADIR "/pics/emoticon_surp.png" );
  insertEmoticon( SAD_ID,      "Sad",        DATADIR "/pics/emoticon_sad.png" );
  insertEmoticon( ANGRY_ID,    "Angry",      DATADIR "/pics/emoticon_angry.png" );
  insertEmoticon( BIGSM_ID,    "Big Smile",  DATADIR "/pics/emoticon_bigsm.png" );
  insertEmoticon( LAUGH_ID,    "Laugh",      DATADIR "/pics/emoticon_laugh.png" );
  insertEmoticon( CRY_ID,      "Cry",        DATADIR "/pics/emoticon_cry.png" );
  insertEmoticon( DEVIL_ID,    "Devil",      DATADIR "/pics/emoticon_devil.png" );
  insertEmoticon( GLASS_ID,    "Glass",      DATADIR "/pics/emoticon_glass.png" );
  insertEmoticon( SUNGL_ID,    "Sunglasses", DATADIR "/pics/emoticon_sungl.png" );
  insertEmoticon( SARC_ID,     "Sarc",       DATADIR "/pics/emoticon_sarc.png" );
  insertEmoticon( QUEST_ID,    "Quest",      DATADIR "/pics/emoticon_quest.png" );
  insertEmoticon( LOVE_ID,     "Love",       DATADIR "/pics/emoticon_love.png" );
  insertEmoticon( ANGEL_ID,    "Angel",      DATADIR "/pics/emoticon_angel.png" );
  insertEmoticon( QUIET_ID,    "Quiet",      DATADIR "/pics/emoticon_quiet.png" );
  insertEmoticon( STRAIGHT_ID,  "Straight",  DATADIR "/pics/emoticon_straight.png" );
  insertEmoticon( BLUSH_ID,    "Blush",      DATADIR "/pics/emoticon_blush.png" );
  insertEmoticon( SLEEP_ID,    "Sleep",      DATADIR "/pics/emoticon_sleep.png" );
  insertEmoticon( BYE_ID,      "Bye",        DATADIR "/pics/emoticon_bye.png" );
  insertEmoticon( CLOWN_ID,    "Clown",      DATADIR "/pics/emoticon_clown.png" );
  insertEmoticon( PISSED_ID,   "Pissed",     DATADIR "/pics/emoticon_pissed.png" );
  insertEmoticon( YUCK_ID,     "Yuck",       DATADIR "/pics/emoticon_yuck.png" );
  insertEmoticon( KISS_ID,     "Kiss",       DATADIR "/pics/emoticon_kiss.png" );

  d->filled  = true;
  emit sizeChanged();
}

void EmoticonPanel::insertEmoticon( int id, const QString& name, const QString& filename )
{
  QPixmap pixmap;
  pixmap.load( filename );

  EmoticonButton* button = new EmoticonButton( id, pixmap, this );
  layout->addWidget( button, d->row, d->col );
  button->show();

  QToolTip::add( button, name );

  connect( button, SIGNAL( selected( int ) ), SLOT( buttonSelected( int ) ) );

  d->col++;
  if( d->col >= d->width )
  {
    d->col = 0;
    d->row++;
  }
}

void EmoticonPanel::buttonSelected( int id )
{
  if ( isVisible() && parentWidget() && 
    parentWidget()->inherits("QPopupMenu") )
      parentWidget()->close();

  emit emoticonSelected( id );
}

void EmoticonPanel::mouseReleaseEvent( QMouseEvent* )
{
  //reject();
}

void EmoticonPanel::showEvent( QShowEvent *e )
{
  if( !d->filled )
    fillEmoticons();
  QWidget::showEvent(e);
}

EmoticonButton::EmoticonButton( int id, const QPixmap& p, QWidget* parent, const char* name )
:  QFrame( parent, name )
{
  d = new EmoticonButtonPrivate;
  d->id = id;
  d->pixmap = p;

  setFixedSize( 20, 20 );
  setFrameStyle( NoFrame );
}


EmoticonButton::~EmoticonButton()
{
  delete d;
}

void EmoticonButton::enterEvent( QEvent* )
{
  setFrameStyle( Panel | Sunken );
}

void EmoticonButton::leaveEvent( QEvent* )
{
  setFrameStyle( NoFrame );
}

void EmoticonButton::paintEvent( QPaintEvent* e )
{
  QFrame::paintEvent( e );

  QPainter p( this, this );
  p.drawPixmap( 2, 2, d->pixmap );
  p.end();
}

void EmoticonButton::mouseReleaseEvent( QMouseEvent* )
{
  emit selected( d->id );
}

#include "emoticon.moc"
