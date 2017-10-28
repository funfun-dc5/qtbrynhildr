// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "mainwindow/desktopimage.h"

namespace qtbrynhildr {

// constructor
DesktopImage::DesktopImage(Settings *settings, QtBrynhildr *qtbrynhildr, QGraphicsItem *parent)
  :
  DesktopWindow(settings, qtbrynhildr),
  QGraphicsObject(parent),
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
  return QRectF(0,0,0,0);
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

  //  cout << "leave paint()" << endl << flush;
}

} // end of namespace qtbrynhildr
