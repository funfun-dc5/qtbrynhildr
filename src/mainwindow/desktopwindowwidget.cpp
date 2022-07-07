// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

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
#include "graphicsthread/yuv2rgb/yuv2rgb.h"
#include "mainwindow/desktopwindowwidget.h"

namespace qtbrynhildr {

// constructor
DesktopWindowWidget::DesktopWindowWidget(QtBrynhildr *qtbrynhildr, QWidget *parent)
  :QWidget(parent)  
  ,DesktopFrame(qtbrynhildr)
  ,onDraging(false)
  ,onAreaMode(false)
  // for DEBUG
  ,outputLog(false)
{
  //  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::StrongFocus);
  //setMouseTracking(true); // mouse tracking mode on
  setMouseTracking(settings->getOnMouseTrackingMode());
}

// destructor
DesktopWindowWidget::~DesktopWindowWidget()
{
}

// resize desktop
void DesktopWindowWidget::resizeDesktop(int width, int height)
{
  resize(width, height);
}

// refresh desktop
void DesktopWindowWidget::updateDesktop()
{
  update();
}

// minimum size hint
QSize DesktopWindowWidget::minimumSizeHint() const
{
  return currentSize;
}

// size hint
QSize DesktopWindowWidget::sizeHint() const
{
  return currentSize;
}

// for event handling
#if defined(QTB_DEV_TOUCHPANEL)
  // event
bool DesktopWindowWidget::event(QEvent *event){
  if (DesktopFrame::event(event))
	return true;
  else
	return QWidget::event(event);
}
#endif // defined(QTB_DEV_TOUCHPANEL)

// paint event
void DesktopWindowWidget::paintEvent(QPaintEvent *event)
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

  // viewer mode
  if (settings->getOnViewerMode()){
	if (onDraging){
	  static QPoint previousPos;
	  if (currentPos != previousPos){
		// draw rectangle firstPos to currentPos
		painter.setPen(QPen(Qt::black, 1, Qt::DotLine));
		painter.drawRect(QRect(firstPos, currentPos));
		previousPos = currentPos;
	  }
	}
  }
  else {
	if (onAreaMode){
	  // leave area mode
	  leaveAreaMode();
	}
  }
}

// widget enter event
void DesktopWindowWidget::enterEvent(QEvent *event)
{
  // check connected
  if (!settings->getConnected())
	return;

  // viewer mode
  if (settings->getOnViewerMode()){
	return;
  }

  // control
  if (settings->getOnControl()){
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
void DesktopWindowWidget::leaveEvent(QEvent *event)
{
  Q_UNUSED(event);

  if (settings->getOnClipCursor()){
	QCursor::setPos(mapToGlobal(currentMousePos));
  }
}

// mouse event
void DesktopWindowWidget::mousePressEvent(QMouseEvent *event)
{
  // viewer mode
  if (settings->getOnViewerMode()){
	if (event->button() == Qt::LeftButton){
	  //qDebug() << "Left Button Press : pos = " << event->pos();
	  if (onAreaMode)
		return;

	  firstPos = event->pos();
	  int x = firstPos.x() & ~0x4;
	  int y = firstPos.y() & ~0x2;
	  firstPos.setX(x);
	  firstPos.setY(y);
	  onDraging = true;
	}
	return;
  }

  DesktopFrame::mousePressEvent(event);
}
void DesktopWindowWidget::mouseReleaseEvent(QMouseEvent *event)
{
  // viewer mode
  if (settings->getOnViewerMode()){
	if (event->button() == Qt::LeftButton){
	  //qDebug() << "Left Button Release : pos = " << event->pos();
	  if (onAreaMode)
		return;

	  lastPos = event->pos();
	  int x = lastPos.x() & ~0x4;
	  int y = lastPos.y() & ~0x2;
	  lastPos.setX(x);
	  lastPos.setY(y);
	  onDraging = false;
	  // enter area mode
	  QRect rect = QRect(firstPos, lastPos).normalized();
	  if (rect.width() >= QTB_AREA_MODE_MINIMUM_WIDTH &&
		  rect.height() >= QTB_AREA_MODE_MINIMUM_HEIGHT){
		enterAreaMode(rect);
	  }
	}
	else if (event->button() == Qt::RightButton){
	  if (!onAreaMode)
		return;

	  // leave area mode
	  leaveAreaMode();
	}
	return;
  }
  DesktopFrame::mouseReleaseEvent(event);
}
void DesktopWindowWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
  DesktopFrame::mouseDoubleClickEvent(event);
}
void DesktopWindowWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (settings->getOnViewerMode()){
	//qDebug() << "pos = " << event->pos();
	currentPos = event->pos();
	int x = currentPos.x() & ~0x4;
	int y = currentPos.y() & ~0x2;
	currentPos.setX(x);
	currentPos.setY(y);
	return;
  }
  DesktopFrame::mouseMoveEvent(event);
}
void DesktopWindowWidget::wheelEvent(QWheelEvent *event)
{
  DesktopFrame::wheelEvent(event);
}

// keyboard event
void DesktopWindowWidget::keyPressEvent(QKeyEvent *event)
{
  DesktopFrame::keyPressEvent(event);
}
void DesktopWindowWidget::keyReleaseEvent(QKeyEvent *event)
{
  DesktopFrame::keyReleaseEvent(event);
}

#if QTB_DRAG_AND_DROP_SUPPORT
// drag and drop
void DesktopWindowWidget::dragEnterEvent(QDragEnterEvent *event)
{
  DesktopFrame::dragEnterEvent(event);
}
void DesktopWindowWidget::dropEvent(QDropEvent *event)
{
  DesktopFrame::dropEvent(event);
}
#endif // QTB_DRAG_AND_DROP_SUPPORT

#if QTB_MOUSE_TRACKING_FOCUS_MODE
void DesktopWindowWidget::focusInEvent(QFocusEvent *event)
{
  //cout << "Focus In" << endl << flush;
  if (!settings->getOnMouseTrackingMode()){
	QWidget::focusInEvent(event);
	setMouseTracking(true); // mouse tracking mode on
  }
  else {
	QWidget::focusInEvent(event);
  }
}
void DesktopWindowWidget::focusOutEvent(QFocusEvent *event)
{
  //cout << "Focus Out" << endl << flush;
  if (!settings->getOnMouseTrackingMode()){
	QWidget::focusOutEvent(event);
	setMouseTracking(false); // mouse tracking mode off
  }
  else {
	QWidget::focusOutEvent(event);
  }
}
#endif // QTB_MOUSE_TRACKING_FOCUS_MODE

//----------------------------------------------------------------------
// native event filter
//----------------------------------------------------------------------
#if defined(Q_OS_WIN)
bool DesktopWindowWidget::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
  return DesktopFrame::nativeEventFilter(eventType, message, result);
}
#endif // defined(Q_OS_WIN)

void DesktopWindowWidget::enterAreaMode(QRect rect)
{
  settings->setDesktopOffsetX(rect.topLeft().x());
  settings->setDesktopOffsetY(rect.topLeft().y());
  settings->setDesktopImageWidth(rect.width());
  settings->setDesktopImageHeight(rect.height());
  onAreaMode = true;
}

void DesktopWindowWidget::leaveAreaMode()
{
  // leave area mode
  settings->setDesktopOffsetX(QTB_DESKTOPOFFSETX_DEFAULT);
  settings->setDesktopOffsetY(QTB_DESKTOPOFFSETY_DEFAULT);
  settings->setDesktopImageWidth(QTB_MAX_SERVER_DESKTOP_WIDTH);
  settings->setDesktopImageHeight(QTB_MAX_SERVER_DESKTOP_HEIGHT);
  onAreaMode = false;
}

} // end of namespace qtbrynhildr
