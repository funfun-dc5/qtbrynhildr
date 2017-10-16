// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef VIEW_H
#define VIEW_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QFrame>
#include <QGraphicsView>

// Local Header


namespace qtbrynhildr {

class View;

class GraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  // constructor
  GraphicsView(View *view)
	:
	QGraphicsView(),
	view(view)
  {
  }
  // destructor
  //  ~GraphicsView()

private:
  // view
  View *view;
};

class View : public QWidget
{
  Q_OBJECT

public:
  // constructor
  View(QWidget *parent = 0);
  // destructor
  ~View();

  QGraphicsView *view() const;

#if 0
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
  // graphics view
  GraphicsView *graphicsView;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // VIEW_H
