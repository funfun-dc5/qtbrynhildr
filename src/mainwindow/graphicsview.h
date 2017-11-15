// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QRect>

// Local Header
#include "mainwindow/desktoppanel.h"

namespace qtbrynhildr {

class QtBrynhildr;

class GraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  // constructor
  GraphicsView(QGraphicsScene *scene, QtBrynhildr *qtbrynhildr, QWidget *parent = Q_NULLPTR);
  // destructor
  ~GraphicsView();

  // scale
  void setScale(qreal scalingFactor);

#if defined(QTB_DEV_TOUCHPANEL)
  // for software button
  void setSoftwareButtonRect(QRect rect)
  {
	softwareButtonRect = rect;
  }

  // for software keyboard
  void setSoftwareKeyboardRect(QRect rect)
  {
	softwareKeyboardRect = rect;
  }
#endif // defined(QTB_DEV_TOUCHPANEL)

protected:
  // for event handling
#if defined(QTB_DEV_TOUCHPANEL)
  // viewport event
  bool viewportEvent(QEvent *event);
#endif // defined(QTB_DEV_TOUCHPANEL)

  // mouse event 
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  // keyboard event
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

private:
  // convert to desktop
  bool convertToDesktop(QPoint &point);

private:
  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // desktop panel
  DesktopPanel *desktopPanel;

#if defined(QTB_DEV_TOUCHPANEL)
  // for software button
  QRect softwareButtonRect;

  // for software keyboard
  QRect softwareKeyboardRect;
#endif // defined(QTB_DEV_TOUCHPANEL)

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSVIEW_H
