// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPIMAGE_H
#define DESKTOPIMAGE_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QGraphicsObject>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QRgb>

// Local Header
#include "mainwindow/desktopwindow.h"


namespace qtbrynhildr {

// DesktopImage
  class DesktopImage : public DesktopWindow, public QGraphicsObject
{
  Q_OBJECT

public:
  // constructor
  DesktopImage(Settings *settings, QtBrynhildr *qtbrynhildr, QGraphicsItem *parent = Q_NULLPTR);
  // destructor
  ~DesktopImage();

  QRectF boundingRect() const override;
#if 0
  QRectF sceneBoundingRect() const;
  QPainterPath shape() const override;
#endif
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

private:
  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // DESKTOPIMAGE_H
