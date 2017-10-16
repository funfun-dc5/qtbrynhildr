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
  return QRectF(0, 0, image.width()-1, image.height()-1); // for TEST
}

void DesktopImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
  //  cout << "enter paint()" << endl << flush;

  if (image.isNull()){
	//	cout << "return paint() : image is null" << endl << flush;
	return;
  }

  painter->drawImage(0, 0, image);

  //  cout << "leave paint()" << endl << flush;
}

// set image
void DesktopImage::setImage(QImage image)
{
  //  cout << "enter setImage()" << endl << flush;
  this->image = image;
  setPos(QPointF(-image.width()/2, -image.height()/2));
  update();
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
