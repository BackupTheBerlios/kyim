/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <qapplication.h>

#ifndef process_packet_h
#define process_packet_h

class QTimer;
class QSocketNotifier;

/**
   @short Handles socket interface
*/
class packet : public QObject
{
  Q_OBJECT

 public:
    packet();
  ~packet();

 public slots:
  /**
     Handles QSocketNotfier events
     @see QSocketNotfier
  */
  void io_read_callback( int );
  void io_write_callback( int );
 
  /** 
     Handles QTimer events
     @see QTimer
  */
  void timer_callback( void );

  /**
     Called from ycb_yahoo_add_handler
     starts the QSocketNotifier for read/write type
  */
  void startReadNotifier(void);
  void startWriteNotifier(void);

  /**
     Called from ycb_yahoo_remove_handler
     Pauses the QSocketNotifier for write type
  */
  void stopWriteNotifier(void);

 private slots:
  void ping_timeout();

 private:
  /* register libyahoo2 callbacks */
  void register_callbacks( void );

  QTimer *ping_timer;

  QSocketNotifier* ReadSocketNotifier;
  QSocketNotifier* WriteSocketNotifier;

};


#endif

