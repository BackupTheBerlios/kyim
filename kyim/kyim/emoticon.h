#ifndef EMOTICON_H
#define EMOTICON_H

#include <qframe.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qwidget.h>

class QGridLayout;
class QEvent;
class QMouseEvent;
class QPaintEvent;
class EmoticonPanelPrivate;
class EmoticonButtonPrivate;

class EmoticonMenu : public QPopupMenu
{ 
  Q_OBJECT

public:
  EmoticonMenu( QWidget* parent = 0, const char* name = 0 );
  ~EmoticonMenu();

signals:
  void emoticonSelected( int id );

public slots:
  void updateSize();
};

class EmoticonPanel : public QWidget
{ 
  Q_OBJECT

public:

  EmoticonPanel( QWidget* parent = 0, const char* name = 0 );
  ~EmoticonPanel();

signals:
  void sizeChanged();
  void emoticonSelected( int id );

protected slots:
  void buttonSelected( int id );

private:

  EmoticonPanelPrivate* d;
  QGridLayout* layout;

  virtual void mouseReleaseEvent( QMouseEvent* );
  virtual void showEvent( QShowEvent *e );

  void fillEmoticons();
  void insertEmoticon( int id, const QString& name, const QString& filename );

  // no copy or assign
  EmoticonPanel( const EmoticonPanel& );
  EmoticonPanel& operator=( const EmoticonPanel& );

};

class EmoticonButton: public QFrame
{ 
  Q_OBJECT

public:
  EmoticonButton( int id, const QPixmap&, QWidget* parent, const char* name = 0 );
  ~EmoticonButton();

signals:
  void selected( int id );

protected:
  virtual void paintEvent( QPaintEvent* );
  virtual void enterEvent( QEvent* );
  virtual void leaveEvent( QEvent* );
  virtual void mouseReleaseEvent( QMouseEvent* );

private:

  EmoticonButtonPrivate* d;

  // no copy or assign
  EmoticonButton( const EmoticonButton& );
  EmoticonButton& operator=( const EmoticonButton& );
};

#endif // EMOTICON_H
