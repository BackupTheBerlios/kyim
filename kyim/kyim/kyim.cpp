// qhello.cpp - Qt Hello world

#include <qapplication.h>
#include <qlabel.h>

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  QString msg("Hello world !");

  QLabel txt( "Hello world !", 0 );
  txt.resize(150,50);

  app.setMainWidget( &txt );
  txt.show();
  
  return app.exec();
}

