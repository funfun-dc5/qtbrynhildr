// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QCursor>
#include <QEnterEvent>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QtGui>

// Local Header
#include "graphicsthread/yuv2rgb/yuv2rgb.h"
#include "mainwindow/desktoppanelwidget.h"

namespace qtbrynhildr {

// constructor
DesktopPanelWidget::DesktopPanelWidget(QtBrynhildr *qtbrynhildr, QWidget *parent)
  :QWidget(parent)  
  ,DesktopFrame(qtbrynhildr)
  // for DEBUG
  ,outputLog(false)
{
  //  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::StrongFocus);
  //setMouseTracking(true); // mouse tracking mode on
  //setMouseTracking(settings->getOnMouseTrackingMode());
}

// destructor
DesktopPanelWidget::~DesktopPanelWidget()
{
}

// resize desktop
void DesktopPanelWidget::resizeDesktop(int width, int height)
{
  resize(width, height);
}

// refresh desktop
void DesktopPanelWidget::updateDesktop()
{
  update();
}

// minimum size hint
QSize DesktopPanelWidget::minimumSizeHint() const
{
  return currentSize;
}

// size hint
QSize DesktopPanelWidget::sizeHint() const
{
  return currentSize;
}

// mouse event
void DesktopPanelWidget::mousePressEvent(QMouseEvent *event)
{
  DesktopFrame::mousePressEvent(event);
}
void DesktopPanelWidget::mouseReleaseEvent(QMouseEvent *event)
{
  DesktopFrame::mouseReleaseEvent(event);
}
void DesktopPanelWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
  DesktopFrame::mouseDoubleClickEvent(event);
}
void DesktopPanelWidget::mouseMoveEvent(QMouseEvent *event)
{
  DesktopFrame::mouseMoveEvent(event);
}
void DesktopPanelWidget::wheelEvent(QWheelEvent *event)
{
  DesktopFrame::wheelEvent(event);
}

// keyboard event
void DesktopPanelWidget::keyPressEvent(QKeyEvent *event)
{
  DesktopFrame::keyPressEvent(event);
}
void DesktopPanelWidget::keyReleaseEvent(QKeyEvent *event)
{
  DesktopFrame::keyReleaseEvent(event);
}

// for event handling
// event
bool DesktopPanelWidget::event(QEvent *event){
  if (DesktopFrame::event(event))
	return true;
  else
	return QWidget::event(event);
}

// paint event
void DesktopPanelWidget::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  if (image.isNull()){
	return;
  }

  QPainter painter(this);

  // draw desktop image
  painter.drawImage(0, 0, image);

  // draw marker for mouse cursor
  if (drawMarkerCounter > 0){
	int length = drawMarkerCounter*10;
	int x = currentMousePos.x() - length/2;
	int y = currentMousePos.y() - length/2;

	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setPen(QPen(Qt::green, 4));
	painter.drawArc(x, y, length, length, 0*360, 16*360);

	drawMarkerCounter--;
  }
#if 0 // for TEST
  else {
	drawMarkerCounter = 30;
  }
#endif // for TEST
}

} // end of namespace qtbrynhildr
