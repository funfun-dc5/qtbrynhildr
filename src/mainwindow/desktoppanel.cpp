// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>

// Local Header
#include "mainwindow/desktoppanel.h"

namespace qtbrynhildr {

// constructor
DesktopPanel::DesktopPanel(QtBrynhildr *qtbrynhildr, QWidget *parent)
  :QScrollArea(parent)
  ,qtbrynhildr(qtbrynhildr)
  ,settings(qtbrynhildr->getSettings())
  ,desktopPanelWidget(0)
  ,keyBuffer(0)
  ,topType(TOP_TYPE_UNKNOWN)
  ,scalingFactor(1.0)
  ,scalingFactorForFullScreen(1.0)
  ,screenSize(settings->getCurrentScreenSize())
  // for DEBUG
  ,outputLog(true)
{
  // focus
  setFocusPolicy(Qt::StrongFocus);

  // enable touch event
  setAttribute(Qt::WA_AcceptTouchEvents, true);

  setWidgetResizable(true);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);

  setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
  //setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  //setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);

  // scrollbar Always Off
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // desktop panel widget
  desktopPanelWidget = new DesktopPanelWidget(qtbrynhildr, this);
  // key buffer
  keyBuffer = desktopPanelWidget->getKeyBuffer();
}

// destructor
DesktopPanel::~DesktopPanel()
{
  // delete objects
  if (desktopPanelWidget != 0){
	delete desktopPanelWidget;
	desktopPanelWidget = 0;
  }
}

// scale
void DesktopPanel::setScale(qreal scalingFactor)
{
  this->scalingFactor = scalingFactor;
}

// resize desktop
void DesktopPanel::resizeDesktop(int width, int height)
{
  Q_UNUSED(width);
  Q_UNUSED(height);
}

// update desktop
void DesktopPanel::updateDesktop()
{
  update();
}

// mouse press event software panel
void DesktopPanel::mousePressEventForSP(QMouseEvent *event)
{
  mousePressEvent(event);
}

// mouse release event software panel
void DesktopPanel::mouseReleaseEventForSP(QMouseEvent *event)
{
  mouseReleaseEvent(event);
}

// mouse move event software panel
void DesktopPanel::mouseMoveEventForSP(QMouseEvent *event)
{
  mouseMoveEvent(event);
}

// mouse move
void DesktopPanel::mouseMove(QPoint mousePos, bool marker)
{
  desktopPanelWidget->mouseMove(mousePos, marker);
}

// mouse move relatively
void DesktopPanel::mouseMoveRelatively(QPoint mousePos, bool marker)
{
  mousePos /= scalingFactor;
  desktopPanelWidget->mouseMoveRelatively(mousePos, marker);
}

// viewport size hint
QSize DesktopPanel::viewportSizeHint() const
{
  return QSize(1280, 800);
}

// viewport event for event handling (touchpanel)
bool DesktopPanel::viewportEvent(QEvent *event)
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
bool DesktopPanel::oneFingerEventForKeroRemote(QTouchEvent *touchEvent)
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
bool DesktopPanel::oneFingerEventForQtBrynhildr(QTouchEvent *touchEvent)
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
bool DesktopPanel::twoFingerEventForKeroRemote(QTouchEvent *touchEvent)
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
bool DesktopPanel::twoFingerEventForQtBrynhildr(QTouchEvent *touchEvent)
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
bool DesktopPanel::threeFingerEvent(QTouchEvent *touchEvent)
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
void DesktopPanel::mousePressEvent(QMouseEvent *event)
{
  // Yet
}

void DesktopPanel::mouseReleaseEvent(QMouseEvent *event)
{
  // Yet
}

void DesktopPanel::mouseDoubleClickEvent(QMouseEvent *event)
{
  // Yet
}

void DesktopPanel::mouseMoveEvent(QMouseEvent *event)
{
  // Yet
}

void DesktopPanel::wheelEvent(QWheelEvent *event)
{
  // Yet
}

} // end of namespace qtbrynhildr
