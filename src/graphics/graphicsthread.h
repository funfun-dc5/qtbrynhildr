// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// graphicsthread.h

#ifndef GRAPHICSTHREAD_H
#define GRAPHICSTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QImage>

// Local Header
#include "common/netthread.h"

namespace qtbrynhildr {

// GraphicsThread
class GraphicsThread : public NetThread
{
  Q_OBJECT

public:
  // constructor
  GraphicsThread(Settings *settings, MainWindow *mainWindow = 0);
  // destructor
  ~GraphicsThread();

  // get frame counter
  unsigned int getFrameCounter() const
  {
	return frameCounter;
  };

  // get frame rate
  double getFrameRate();

protected:
  // connect to server
  CONNECT_RESULT connectToServer();

  // process for header
  PROCESS_RESULT processForHeader();

  // transmit local buffer to global buffer
  TRANSMIT_RESULT transmitBuffer();

  // connected
  void connectedToServer();

  // shutdown connection
  void shutdownConnection();

private:
  // image for desktop
  QImage *image;

  // frame counter
  unsigned int frameCounter;

  // clearDesktop
  bool onClearDesktop;

private:
  // local buffer
  char *buffer;

signals:
  // desktop Changed
  void desktopChanged(QImage image);

  // desktop Clear
  void desktopClear();
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSTHREAD_H
