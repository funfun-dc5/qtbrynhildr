// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>
#include <QVBoxLayout>

// Local Header
#include "view.h"

namespace qtbrynhildr {

// constructor
View::View(QWidget *parent)
  :
  QWidget(parent),
  // for DEBUG
  outputLog(true)
{
  graphicsView = new GraphicsView(this);
  graphicsView->setRenderHint(QPainter::Antialiasing, false);
  graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  graphicsView->setDragMode(QGraphicsView::NoDrag);
  graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphicsView->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(graphicsView);
  setLayout(layout);
}

// destructor
View::~View()
{
}

QGraphicsView *View::view() const
{
  return (QGraphicsView *)graphicsView;
}

#if 0
// mouse event 
void View::mousePressEvent(QMouseEvent *event)
{
  cout << "mousePressEvent" << endl << flush;
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
  cout << "mouseReleaseEvent" << endl << flush;
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
  cout << "mouseDoubleClicEvent" << endl << flush;
}

void View::mouseMoveEvent(QMouseEvent *event)
{
  cout << "mouseMoveEvent" << endl << flush;
}

void View::wheelEvent(QWheelEvent *event)
{
  cout << "wheelEvent" << endl << flush;
}

// keyboard event
void View::keyPressEvent(QKeyEvent *event)
{
  cout << "keyPressEvent" << endl << flush;
}

void View::keyReleaseEvent(QKeyEvent *event)
{
  cout << "keyReleaseEvent" << endl << flush;
}
#endif

} // end of namespace qtbrynhildr
