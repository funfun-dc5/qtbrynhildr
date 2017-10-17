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

// Local Header


namespace qtbrynhildr {

class GraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  // constructor
  GraphicsView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
  // destructor
  ~GraphicsView();

#if 1
protected:
  // mouse event 
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  // keyboard event
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
#endif

private:
  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSVIEW_H
