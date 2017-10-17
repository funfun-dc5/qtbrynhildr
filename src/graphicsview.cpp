// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>

// Local Header
#include "graphicsview.h"

namespace qtbrynhildr {

// constructor
GraphicsView::GraphicsView(QGraphicsScene *scene)
  :
  QGraphicsView(scene),
  // for DEBUG
  outputLog(true)
{
  setRenderHint(QPainter::Antialiasing, false);
  setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

// destructor
GraphicsView::~GraphicsView()
{
}

#if 1
// mouse event 
void GraphicsView::mousePressEvent(QMouseEvent *event)
{
  cout << "mousePressEvent" << endl << flush;
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  cout << "mouseReleaseEvent" << endl << flush;
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
  cout << "mouseDoubleClicEvent" << endl << flush;
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
  cout << "mouseMoveEvent" << endl << flush;
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
  cout << "wheelEvent" << endl << flush;
}

// keyboard event
void GraphicsView::keyPressEvent(QKeyEvent *event)
{
  cout << "keyPressEvent" << endl << flush;
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
  cout << "keyReleaseEvent" << endl << flush;
}
#endif

} // end of namespace qtbrynhildr
