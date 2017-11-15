// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QtGui>

// Local Header
#include "mainwindow/desktoppanelobject.h"

namespace qtbrynhildr {

// constructor
DesktopPanelObject::DesktopPanelObject(QtBrynhildr *qtbrynhildr, QGraphicsItem *parent)
  :
  QGraphicsObject(parent),
  DesktopPanel(qtbrynhildr)
{
  setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

// destructor
DesktopPanelObject::~DesktopPanelObject()
{
}

// resize desktop
void DesktopPanelObject::resizeDesktop(int width, int height)
{
  Q_UNUSED(width);
  Q_UNUSED(height);

  //  cout << "resizeDesktop()" << endl << flush;
  //  cout << "(width, height) = (" << width << ", " << height << ")" << endl << flush;
}

// refresh desktop
void DesktopPanelObject::updateDesktop()
{
  //  cout << "updateDesktop()" << endl << flush;
  update();
}

QRectF DesktopPanelObject::boundingRect() const
{
  return QRectF(-image.width()/2, -image.height()/2, image.width(), image.height());
}

void DesktopPanelObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  // set half width, half height
  int hwidth = image.width()/2;
  int hheight = image.height()/2;

  // draw image
  painter->drawImage(-hwidth, -hheight, image);

  // draw marker for mouse cursor
  if (drawMarkerCounter > 0){
	int length = drawMarkerCounter*10;
	int x = currentMousePos.x() - length/2 - hwidth;
	int y = currentMousePos.y() - length/2 - hheight;

	painter->setRenderHint(QPainter::Antialiasing, false);
	painter->setPen(QPen(Qt::green, 4));
	painter->drawArc(x, y, length, length, 0*360, 16*360);

	drawMarkerCounter--;
  }
}

} // end of namespace qtbrynhildr
