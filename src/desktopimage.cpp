// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "desktopimage.h"

namespace qtbrynhildr {

// constructor
DesktopImage::DesktopImage()
  :
  // for DEBUG
  outputLog(true)
{
}

// destructor
DesktopImage::~DesktopImage()
{
}

QRectF DesktopImage::boundingRect() const
{
  return QRectF(-image.width()/2, -image.height()/2, image.width(), image.height());
}

#if 0
QRectF DesktopImage::sceneBoundingRect() const
{
  return QRectF(-image.width()/2, -image.height()/2, image.width(), image.height());
}

QPainterPath DesktopImage::shape() const
{
  QPainterPath path;
  path.addRect(-640,-400,1280,800);
  return path;
}
#endif

void DesktopImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
  //  cout << "enter paint()" << endl << flush;

  if (image.isNull()){
	//	cout << "return paint() : image is null" << endl << flush;
	return;
  }

  painter->drawImage(-image.width()/2, -image.height()/2, image);

  //  cout << "leave paint()" << endl << flush;
}

// set image
void DesktopImage::setImage(QImage image)
{
  //  cout << "enter setImage()" << endl << flush;
  // copy
  this->image = image;

  //  cout << "leave setImage()" << endl << flush;
}

void DesktopImage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  cout << "mousePressEvent" << endl << flush;
}

void DesktopImage::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  cout << "mouseReleaseEvent" << endl << flush;
}

void DesktopImage::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  cout << "mouseMoveEvent" << endl << flush;
}

} // end of namespace qtbrynhildr
