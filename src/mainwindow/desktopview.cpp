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
  ,scalingFactor(1.0)
  ,topType(TOP_TYPE_UNKNOWN)
  // for DEBUG
  ,outputLog(true)
{
}

// destructor
DesktopView::~DesktopView()
{
}

// scale
void DesktopView::setScale(qreal scalingFactor)
{
}

// resize desktop
void DesktopView::resizeDesktop(int width, int height)
{
}

// update desktop
void DesktopView::updateDesktop()
{
}

// mouse press event software panel
void DesktopView::mousePressEventForSP(QMouseEvent *event)
{
  mousePressEvent(event);
}

// mouse release event software panel
void DesktopView::mouseReleaseEventForSP(QMouseEvent *event)
{
  mouseReleaseEvent(event);
}

// mouse move event software panel
void DesktopView::mouseMoveEventForSP(QMouseEvent *event)
{
  mouseMoveEvent(event);
}

// mouse move
void DesktopView::mouseMove(QPoint mousePos, bool marker)
{
  desktopPanel->mouseMove(mousePos, marker);
}

// mouse move relatively
void DesktopView::mouseMoveRelatively(QPoint mousePos, bool marker)
{
  mousePos /= scalingFactor;
  desktopPanel->mouseMoveRelatively(mousePos, marker);
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

	  // -----------------------------------------------------------------------------------
	  // KeroRemote Compatible Operation
	  // -----------------------------------------------------------------------------------
	  if (settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_KEROREMOTE){
		switch(touchPointCount){
		case 1: // 1 finger operation
		  return oneFingerEventForKeroRemote(touchEvent);
		  break;
		case 2: // 2 fingers operation
		  return twoFingerEventForKeroRemote(touchEvent);
		  break;
		case 3: // 3 fingers operation
		  return threeFingerEvent(touchEvent);
		  break;
		default:
		  // Nothing to do
		  break;
		}
	  }
	  // -----------------------------------------------------------------------------------
	  // Qt Brynhildr Operation
	  // -----------------------------------------------------------------------------------
	  else if (settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_QTBRYNHILDR){
		switch(touchPointCount){
		case 1: // 1 finger operation
		  return oneFingerEventForQtBrynhildr(touchEvent);
		  break;
		case 2: // 2 fingers operation
		  return twoFingerEventForQtBrynhildr(touchEvent);
		  break;
		case 3: // 3 fingers operation
		  return threeFingerEvent(touchEvent);
		  break;
		default:
		  // Nothing to do
		  break;
		}
	  }
	  else {
		// Unknown Touchpanel Operation Type
		qDebug() << "DV: Unknown Touchpanel Operation!";
	  }
	}
  default:
	// Nothing to do
	break;
  }

  return QScrollArea::viewportEvent(event);
}

// -----------------------------------------------------------------------------------
// 1 Finger Operation
// -----------------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------------
// KeroRemote Compatible Operation (2 finger)
// -----------------------------------------------------------------------------------
bool DesktopView::twoFingerEventForKeroRemote(QTouchEvent *touchEvent)
{
  // check software panel
  if (settings->getOnShowSoftwareKeyboard() || settings->getOnShowSoftwareButton()){
	// Nothing to do
	return true;
  }

  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
	if (outputLog){
	  qDebug() << "DV: Kero 2 fingers Pressed!";
	}
	topType = TOP_TYPE_2POINT;
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
	if (outputLog){
	  qDebug() << "DV: Kero 2 fingers Released!";
	}
	// Nothing to do
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
	if (outputLog){
	  qDebug() << "DV: Kero 2 finger Moved!";
	}
	// change scaling factor
	const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
	const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
	qreal currentScalingFactor =
	  QLineF(touchPoint0.pos(), touchPoint1.pos()).length() /
	  QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
	if (currentScalingFactor < 1.0){
	  scalingFactor -= 0.02;
	  qreal scalingFactorForFullScreen = getScalingFactorForFullScreen();
	  if (scalingFactor < scalingFactorForFullScreen){
		scalingFactor = scalingFactorForFullScreen;
	  }
	}
	else {
	  scalingFactor += 0.02;
#if QTB_TEST
	  if (scalingFactor > 1.0) scalingFactor = 1.0;
#endif // QTB_TEST
	}
	settings->setDesktopScalingFactor(scalingFactor);
	scalingFactor = settings->getDesktopScalingFactor();
	setScale(scalingFactor);
  }

  return true;
}

// -----------------------------------------------------------------------------------
// QtBrynhildr Operation (2 finger)
// -----------------------------------------------------------------------------------
bool DesktopView::twoFingerEventForQtBrynhildr(QTouchEvent *touchEvent)
{
  // check software panel
  if (settings->getOnShowSoftwareKeyboard() || settings->getOnShowSoftwareButton()){
	// Nothing to do
	return true;
  }

  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
	if (outputLog){
	  qDebug() << "DV: Qt 2 fingers Pressed!";
	}
	topType = TOP_TYPE_2POINT;
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
	if (outputLog){
	  qDebug() << "DV: Qt 2 fingers Released!";
	}
	if (topType != TOP_TYPE_2POINT){
	  // Nothing to do
	}
	else {
	  const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
	  const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
	  int distance0 = QLineF(touchPoint0.startPos(), touchPoint0.pos()).length();
	  int distance1 = QLineF(touchPoint1.startPos(), touchPoint1.pos()).length();
	  if (distance0 < QTB_TOUCHPANEL_2TAP_DIST_THRESHOLD &&
		  distance1 < QTB_TOUCHPANEL_2TAP_DIST_THRESHOLD){
		if (outputLog){
		  qDebug() << "GV: 2 Point Tap!!";
		}

		QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
												  touchPoint0.pos(),
												  Qt::RightButton,
												  Qt::RightButton,
												  Qt::NoModifier);
		QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													touchPoint0.pos(),
													Qt::RightButton,
													Qt::RightButton,
													Qt::NoModifier);
		// R mouse button
		mousePressEvent(pressEvent);
		mouseReleaseEvent(releaseEvent);
		delete pressEvent;
		delete releaseEvent;
	  }	  
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
	if (outputLog){
	  qDebug() << "DV: Qt 2 finger Moved!";
	}
	// change scaling factor
	const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
	const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
	qreal currentScalingFactor =
	  QLineF(touchPoint0.pos(), touchPoint1.pos()).length() /
	  QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
	if (currentScalingFactor < 1.0){
	  scalingFactor -= 0.02;
	  qreal scalingFactorForFullScreen = getScalingFactorForFullScreen();
	  if (scalingFactor < scalingFactorForFullScreen){
		scalingFactor = scalingFactorForFullScreen;
	  }
	}
	else {
	  scalingFactor += 0.02;
#if QTB_TEST
	  if (scalingFactor > 1.0) scalingFactor = 1.0;
#endif // QTB_TEST
	}
	settings->setDesktopScalingFactor(scalingFactor);
	scalingFactor = settings->getDesktopScalingFactor();
	setScale(scalingFactor);
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

// mouse event 
void DesktopView::mousePressEvent(QMouseEvent *event)
{
}

void DesktopView::mouseReleaseEvent(QMouseEvent *event)
{
}

void DesktopView::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void DesktopView::mouseMoveEvent(QMouseEvent *event)
{
}

void DesktopView::wheelEvent(QWheelEvent *event)
{
}

} // end of namespace qtbrynhildr
