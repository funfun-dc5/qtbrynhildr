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

// Local Header


namespace qtbrynhildr {

// DesktopImage
class DesktopImage : public QGraphicsObject
{
  Q_OBJECT

public:
  // constructor
  DesktopImage();
  // destructor
  ~DesktopImage();

  QRectF boundingRect() const override;
  //  QRectF sceneBoundingRect() const;
  //  QPainterPath shape() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

  // set image
  void setImage(QImage image);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
  // desktop image
  QImage image;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // DESKTOPIMAGE_H
