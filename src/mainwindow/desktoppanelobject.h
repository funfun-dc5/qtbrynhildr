// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPPANELOBJECT_H
#define DESKTOPPANELOBJECT_H
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
#include "mainwindow/desktoppanel.h"
#include "settings.h"

namespace qtbrynhildr {

class QtBrynhildr;

class DesktopPanelObject : public QGraphicsObject, public DesktopPanel
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
  DesktopPanelObject(QtBrynhildr *qtbrynhildr, QGraphicsItem *parent = nullptr);
  // destructor
  ~DesktopPanelObject();

  // resize desktop
  void resizeDesktop(int width, int height);

  // update desktop
  void updateDesktop();

protected:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
};

} // end of namespace qtbrynhildr

#endif // DESKTOPPANELOBJECT_H
