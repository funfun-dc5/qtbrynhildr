// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPFRAMEOBJECT_H
#define DESKTOPFRAMEOBJECT_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

// Local Header
#include "mainwindow/desktopframe.h"
#include "settings.h"

namespace qtbrynhildr {

class QtBrynhildr;

class DesktopPanelObject : public QGraphicsObject, public DesktopFrame
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DesktopPanelObject(QtBrynhildr *qtbrynhildr, QGraphicsItem *parent = Q_NULLPTR);
  // destructor
  ~DesktopPanelObject();

  // resize desktop
  void resizeDesktop(int width, int height);

  // update desktop
  void updateDesktop();

protected:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
};

} // end of namespace qtbrynhildr

#endif // DESKTOPFRAMEOBJECT_H
