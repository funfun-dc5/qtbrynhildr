// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QCursor>
#include <QEnterEvent>
#include <QPainter>
#include <QPoint>
#include <QSize>
#include <QtGui>

// Local Header
#include "mainwindow/desktopwindow.h"

namespace qtbrynhildr {

// constructor
DesktopWindow::DesktopWindow(QtBrynhildr *qtbrynhildr, QWidget *parent)
  :QWidget(parent)  
  ,DesktopPanel(qtbrynhildr)
  // for DEBUG
  ,outputLog(false)
{
  //  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true); // mouse tracking mode on
}

// destructor
DesktopWindow::~DesktopWindow()
{
}

// resize desktop
void DesktopWindow::resizeDesktop(int width, int height)
{
  resize(width, height);
}

// refresh desktop
void DesktopWindow::updateDesktop()
{
  update();
}

// minimum size hint
QSize DesktopWindow::minimumSizeHint() const
{
  return currentSize;
}

// size hint
QSize DesktopWindow::sizeHint() const
{
  return currentSize;
}

// for event handling
#if defined(QTB_DEV_TOUCHPANEL)
  // event
bool DesktopWindow::event(QEvent *event){
  if (DesktopPanel::event(event))
	return true;
  else
	return QWidget::event(event);
}
#endif // defined(QTB_DEV_TOUCHPANEL)

// paint event
void DesktopWindow::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  if (image.isNull()){
	return;
  }

  QPainter painter(this);
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

// widget enter event
void DesktopWindow::enterEvent(QEvent *event)
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	// taskbar assist
	if (settings->getOnTaskbarAssist()){
	  QPoint pos = ((QEnterEvent*)event)->pos();
	  const int areaWidth = settings->getTaskbarAssistAreaWidth(); // width()/100;
	  const int areaHeight = settings->getTaskbarAssistAreaHeight(); // height()/100;
	  int windowWidth = width();
	  int windowHeight = height();
	  if (outputLog) qDebug() << "EtnerEvent->pos() : " << pos;
	  // check top side
	  if (pos.y() <= areaHeight){
		if (outputLog) qDebug() << "top size";
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MOVE_TOPSIDE, MOUSE_BUTTON_UP);
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MOVE_TOPSIDE, MOUSE_BUTTON_UP);
	  }
	  // check bottom side
	  else if (pos.y() >= windowHeight - areaHeight){
		if (outputLog) qDebug() << "bottom side";
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MOVE_BOTTOMSIDE, MOUSE_BUTTON_UP);
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MOVE_BOTTOMSIDE, MOUSE_BUTTON_UP);
	  }
	  // check left side
	  else if (pos.x() <= areaWidth){
		if (outputLog) qDebug() << "left side";
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MOVE_LEFTSIDE, MOUSE_BUTTON_UP);
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MOVE_LEFTSIDE, MOUSE_BUTTON_UP);
	  }
	  // check right side
	  else if (pos.x() >= windowWidth - areaWidth){
		if (outputLog) qDebug() << "right side";
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MOVE_RIGHTSIDE, MOUSE_BUTTON_UP);
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MOVE_RIGHTSIDE, MOUSE_BUTTON_UP);
	  }
	}
  }
}

// widget leave event
void DesktopWindow::leaveEvent(QEvent *event)
{
  Q_UNUSED(event);

  if (settings->getOnClipCursor()){
	QCursor::setPos(mapToGlobal(currentMousePos));
  }
}

// mouse event
void DesktopWindow::mousePressEvent(QMouseEvent *event)
{
  DesktopPanel::mousePressEvent(event);
}
void DesktopWindow::mouseReleaseEvent(QMouseEvent *event)
{
  DesktopPanel::mouseReleaseEvent(event);
}
void DesktopWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
  DesktopPanel::mouseDoubleClickEvent(event);
}
void DesktopWindow::mouseMoveEvent(QMouseEvent *event)
{
  DesktopPanel::mouseMoveEvent(event);
}
void DesktopWindow::wheelEvent(QWheelEvent *event)
{
  DesktopPanel::wheelEvent(event);
}

// keyboard event
void DesktopWindow::keyPressEvent(QKeyEvent *event)
{
  DesktopPanel::keyPressEvent(event);
}
void DesktopWindow::keyReleaseEvent(QKeyEvent *event)
{
  DesktopPanel::keyReleaseEvent(event);
}

#if QTB_DRAG_AND_DROP_SUPPORT
// drag and drop
void DesktopWindow::dragEnterEvent(QDragEnterEvent *event)
{
  DesktopPanel::dragEnterEvent(event);
}
void DesktopWindow::dropEvent(QDropEvent *event)
{
  DesktopPanel::dropEvent(event);
}
#endif // QTB_DRAG_AND_DROP_SUPPORT

//----------------------------------------------------------------------
// native event filter
//----------------------------------------------------------------------
#if defined(Q_OS_WIN)
bool DesktopWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
  return DesktopPanel::nativeEventFilter(eventType, message, result);
}
#endif // defined(Q_OS_WIN)

} // end of namespace qtbrynhildr
