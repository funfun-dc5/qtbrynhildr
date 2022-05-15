// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>

// Local Header
#include "desktopview.h"

namespace qtbrynhildr {

// constructor
DesktopView::DesktopView(QtBrynhildr *qtbrynhildr, QWidget *parent)
  :QScrollArea(parent)
  ,DesktopPanel(qtbrynhildr)
  ,topType(TOP_TYPE_UNKNOWN)
  // for DEBUG
  ,outputLog(true)
{
}

// destructor
DesktopView::~DesktopView()
{
}

// paint event
void DesktopView::paintEvent(QPaintEvent *event)
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
}

// viewport event for event handling (touchpanel)
bool DesktopView::viewportEvent(QEvent *event)
{
  switch(event->type()){
  case QEvent::TouchBegin:
  case QEvent::TouchUpdate:
  case QEvent::TouchEnd:
	{
	  QTouchEvent *touchEvent = (QTouchEvent*)event;
	  if (outputLog){
		qDebug() << "event type  = " << event->type();
		qDebug() << "TouchStates = " << touchEvent->touchPointStates();
	  }

	  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
	  int touchPointCount = touchPoints.count();

	  switch(touchPointCount){
	  case 1: // 1 finger operation
		// touch event
		switch(settings->getTouchpanelOperationType()){
		case QTB_TOUCHPANELOPERATIONTYPE_QTBRYNHILDR:
		  return oneFingerEventForQtBrynhildr(touchEvent);
		  break;
		case QTB_TOUCHPANELOPERATIONTYPE_KEROREMOTE:
		  return oneFingerEventForKeroRemote(touchEvent);
		  break;
		default:
		  // Illegal TouchpanelOperationType
		  break;
		}
		break;
	  case 2: // 2 fingers operation
		return twoFingerEvent(touchEvent);
		break;
	  case 3: // 3 fingers operation
		return threeFingerEvent(touchEvent);
		break;
	  default:
		// Nothing to do
		break;
	  }
	}
	break;
  default:
	// Nothing to do
	break;
  }

  return QScrollArea::viewportEvent(event);
}

// -----------------------------------------------------------------------------------
// KeroRemote Compatible Operation (1 finger)
// -----------------------------------------------------------------------------------
bool DesktopView::oneFingerEventForKeroRemote(QTouchEvent *touchEvent)
{
  // last position of mouse cursor
  static QPointF lastPos;

  // open software panel check flags
  static bool inCheckingButtonOpen = false;
  static bool inCheckingKeyboardOpen = false;
  // for zoom
  static bool inZooming = false;
  static QDateTime pressTimeInZooming;

  const QTouchEvent::TouchPoint &touchPoint = touchEvent->touchPoints().first();

  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
	if (outputLog){
	  qDebug() << "DV: Kero 1 fingers Pressed!";
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
	if (outputLog){
	  qDebug() << "DV: Kero 1 fingers Released!";
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
	if (outputLog){
	  qDebug() << "DV: Kero 1 finger Moved!";
	}
  }

  return true;
}

// -----------------------------------------------------------------------------------
// QtBrynhildr Operation (1 finger)
// -----------------------------------------------------------------------------------
bool DesktopView::oneFingerEventForQtBrynhildr(QTouchEvent *touchEvent)
{
  // open software panel check flags
  static bool inCheckingButtonOpen = false;
  static bool inCheckingKeyboardOpen = false;
  // for zoom
  static bool inZooming = false;

  const QTouchEvent::TouchPoint &touchPoint = touchEvent->touchPoints().first();

  QPoint currentPos = touchPoint.pos().toPoint();
  QPoint lastPos = touchPoint.lastPos().toPoint();

  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
	if (outputLog){
	  qDebug() << "DV: Qt 1 fingers Pressed!";
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
	if (outputLog){
	  qDebug() << "DV: Qt 1 fingers Released!";
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
	if (outputLog){
	  qDebug() << "DV: Qt 1 finger Moved!";
	}
  }

  return true;
}

// -----------------------------------------------------------------------------------
// 2 Finger Operation
// -----------------------------------------------------------------------------------
bool DesktopView::twoFingerEvent(QTouchEvent *touchEvent)
{
  // check software panel
  if (settings->getOnShowSoftwareKeyboard() || settings->getOnShowSoftwareButton()){
	// Nothing to do
	return true;
  }

  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
	if (outputLog){
	  qDebug() << "DV: 2 fingers Pressed!";
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
	if (outputLog){
	  qDebug() << "DV: 2 fingers Released!";
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
	if (outputLog){
	  qDebug() << "DV: 2 finger Moved!";
	}
  }

  return true;
}

// -----------------------------------------------------------------------------------
// 3 Finger Operation
// -----------------------------------------------------------------------------------
bool DesktopView::threeFingerEvent(QTouchEvent *touchEvent)
{
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
	if (outputLog){
	  qDebug() << "DV: 3 fingers Pressed!";
	}
	topType = TOP_TYPE_3POINT;
	// press
	//keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN); // Windows key press
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
	if (outputLog){
	  qDebug() << "DV: 3 fingers Released!";
	}
	if (topType == TOP_TYPE_3POINT){
	  // press
	  keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN); // Windows key press
	  // release
	  keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYUP); // Windows key release
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
	if (outputLog){
	  qDebug() << "DV: 3 finger Moved!";
	}
	// Nothing to do
  }

  return true;
}

} // end of namespace qtbrynhildr
