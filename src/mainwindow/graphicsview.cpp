// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>

// Local Header
#include "mainwindow/graphicsview.h"

namespace qtbrynhildr {

// constructor
GraphicsView::GraphicsView(QGraphicsScene *scene, DesktopPanel *desktopPanel, QWidget *parent)
  :
  QGraphicsView(scene, parent),
  desktopPanel(desktopPanel),
  // for DEBUG
  outputLog(true)
{
  setRenderHint(QPainter::Antialiasing, false);
  setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setAlignment(Qt::AlignCenter);
}

// destructor
GraphicsView::~GraphicsView()
{
}

// scale
void GraphicsView::setScale(qreal scalingFactor)
{
  QMatrix matrix;

  matrix.scale(scalingFactor, scalingFactor);
  setMatrix(matrix);
}

// mouse event 
void GraphicsView::mousePressEvent(QMouseEvent *event)
{
  //  cout << "mousePressEvent" << endl << flush;
  desktopPanel->mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  //  cout << "mouseReleaseEvent" << endl << flush;
  desktopPanel->mouseReleaseEvent(event);
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
  //  cout << "mouseDoubleClicEvent" << endl << flush;
  desktopPanel->mouseDoubleClickEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
  //  cout << "mouseMoveEvent" << endl << flush;
  desktopPanel->mouseMoveEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
  //  cout << "wheelEvent" << endl << flush;
  desktopPanel->wheelEvent(event);
}

// keyboard event
void GraphicsView::keyPressEvent(QKeyEvent *event)
{
  //  cout << "keyPressEvent" << endl << flush;
  desktopPanel->keyPressEvent(event);
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
  //  cout << "keyReleaseEvent" << endl << flush;
  desktopPanel->keyReleaseEvent(event);
}

} // end of namespace qtbrynhildr
