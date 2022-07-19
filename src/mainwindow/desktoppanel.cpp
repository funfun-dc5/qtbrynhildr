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

  //setWidgetResizable(true);
  setWidgetResizable(false);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  //setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

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

  setScale(0.33); // for TEST
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
  settings->setDesktopScalingFactor(scalingFactor);
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

// size hint
QSize DesktopPanel::sizeHint() const
{
  return QSize(1280, 736);
}

// scrollarea event for event handling (touchpanel)
bool DesktopPanel::event(QEvent *event)
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
		qDebug() << "DP: Unknown Touchpanel Operation!";
	  }
	}
  default:
	// Nothing to do
	break;
  }

  return QScrollArea::event(event);
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

  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: Kero 1 fingers Pressed!";
	}

	// set TOP_TYPE
	topType = TOP_TYPE_1POINT;

	if (softwareButtonRect.contains(touchPoint.pos().toPoint())){
	  if (outputLog){
		qDebug() << "DP: into software button open area";
	  }
	  inCheckingButtonOpen = true;
	}
	else if (softwareKeyboardRect.contains(touchPoint.pos().toPoint())){
	  if (outputLog){
		qDebug() << "DP: into software keyboard open area";
	  }
	  inCheckingKeyboardOpen = true;
	}
	else if (inZooming){
	  // for release
	  pressTimeInZooming = QDateTime::currentDateTime();
	}
	else {
	  // move mouse cursor and press left button
	  qreal distance = QLineF(lastPos, touchPoint.pos()).length();
	  //qDebug() << "distance = " << distance;
	  if (lastPos.isNull() || distance > QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												 touchPoint.pos(),
												 Qt::NoButton,
												 Qt::NoButton,
												 Qt::NoModifier);

		mouseMoveEvent(moveEvent);
		delete moveEvent;
		lastPos = touchPoint.pos();
	  }
	  QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
												touchPoint.pos(),
												Qt::LeftButton,
												Qt::LeftButton,
												Qt::NoModifier);

	  mousePressEvent(pressEvent);
	  delete pressEvent;
	}
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: Kero 1 fingers Released!";
	}

	if (topType != TOP_TYPE_1POINT){
	  // Nothing to do
	}
	else if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // display software panel check
	  QPoint currentPos = touchPoint.pos().toPoint();
	  if (inCheckingButtonOpen &&
		  !softwareButtonRect.contains(currentPos, true)){
		// open software button
		qtbrynhildr->toggleSoftwareButton();
	  }
	  if (inCheckingKeyboardOpen &&
		  !softwareKeyboardRect.contains(currentPos, true)){
		// open software keyboard
		qtbrynhildr->toggleSoftwareKeyboard();
	  }
	}
	else if (inZooming){
	  // check tap
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 tapTime = pressTimeInZooming.msecsTo(currentTime);
	  if (tapTime < QTB_TOUCHPANEL_TAP_TIME_THRESHOLD){
		// tap
		// move mouse cursor and press left button and release left button
		qreal distance = QLineF(lastPos, touchPoint.pos()).length();
		//qDebug() << "distance = " << distance;
		if (lastPos.isNull() || distance > QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		  QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												   touchPoint.pos(),
												   Qt::NoButton,
												   Qt::NoButton,
												   Qt::NoModifier);

		  mouseMoveEvent(moveEvent);
		  delete moveEvent;
		  lastPos = touchPoint.pos();
		}
		QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);

		mousePressEvent(pressEvent);
		delete pressEvent;

		// release left button
		QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													touchPoint.pos(),
													Qt::LeftButton,
													Qt::LeftButton,
													Qt::NoModifier);

		mouseReleaseEvent(releaseEvent);
		delete releaseEvent;
	  }
	}
	else {
	  // release left button
	  QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);

	  mouseReleaseEvent(releaseEvent);
	  delete releaseEvent;
	}

	// reset open software panel check flags
	inCheckingButtonOpen = false;
	inCheckingKeyboardOpen = false;
	inZooming = settings->getDesktopScalingFactor() > getScalingFactorForFullScreen();
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: Kero 1 finger Moved!";
	}

	if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // Nothing to do
	}
	else if (inZooming){
	  // scroll desktop
	  QPoint currentPos = touchPoint.pos().toPoint();
	  QPoint lastPos = touchPoint.lastPos().toPoint();
	  QPoint move = lastPos - currentPos;
#if 0 // Yet
	  horizontalScrollBar()->setValue(horizontalScrollBar()->value() + move.x());
	  verticalScrollBar()->setValue(verticalScrollBar()->value() + move.y());
#endif //0 // Yet
	}
	else {
	  // move mouse cursor
	  qreal distance = QLineF(lastPos, touchPoint.pos()).length();
	  //qDebug() << "distance = " << distance;
	  if (lastPos.isNull() || distance > QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												 touchPoint.pos(),
												 Qt::NoButton,
												 Qt::NoButton,
												 Qt::NoModifier);
		mouseMoveEvent(moveEvent);
		delete moveEvent;
		lastPos = touchPoint.pos();
	  }
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

  // set TOP_TYPE
  topType = TOP_TYPE_1POINT;

  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: Qt 1 fingers Pressed!";
	}

	// set topType
	topType = TOP_TYPE_1POINT;

	// check for software keyboard/button
	if (softwareButtonRect.contains(currentPos)){
	  if (outputLog){
		qDebug() << "DP: into software button open area";
	  }
	  inCheckingButtonOpen = true;
	}
	else if (softwareKeyboardRect.contains(currentPos)){
	  if (outputLog){
		qDebug() << "DP: into software keyboard open area";
	  }
	  inCheckingKeyboardOpen = true;
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
	if (outputLog){
	  qDebug() << "DP: 1 Released!";
	}

	if (topType != TOP_TYPE_1POINT){
	  // Nothing to do
	}
	else if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // display software panel check
	  if (inCheckingButtonOpen &&
		  !softwareButtonRect.contains(currentPos, true)){
		// open software button
		qtbrynhildr->toggleSoftwareButton();
	  }
	  if (inCheckingKeyboardOpen &&
		  !softwareKeyboardRect.contains(currentPos, true)){
		// open software keyboard
		qtbrynhildr->toggleSoftwareKeyboard();
	  }
	}
	else {
	  qreal distance = QLineF(touchPoint.startPos(), touchPoint.pos()).length();
	  if (distance < QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		if (!settings->getOnShowSoftwareButton()){
		  QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												   touchPoint.pos(),
												   Qt::NoButton,
												   Qt::NoButton,
												   Qt::NoModifier);
		  QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
													touchPoint.pos(),
													Qt::LeftButton,
													Qt::LeftButton,
													Qt::NoModifier);
		  QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													  touchPoint.pos(),
													  Qt::LeftButton,
													  Qt::LeftButton,
													  Qt::NoModifier);
		  // move + L mouse button
		  mouseMoveEvent(moveEvent);
		  mousePressEvent(pressEvent);
		  mouseReleaseEvent(releaseEvent);
		  delete moveEvent;
		  delete pressEvent;
		  delete releaseEvent;
		}
	  }
	}

	// reset open software panel check flags
	inCheckingButtonOpen = false;
	inCheckingKeyboardOpen = false;
	inZooming = settings->getDesktopScalingFactor() > getScalingFactorForFullScreen();
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: Qt 1 fingers Released!";
	}

	if (topType != TOP_TYPE_1POINT){
	  // Nothing to do
	}
	else if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // display software panel check
	  if (inCheckingButtonOpen &&
		  !softwareButtonRect.contains(currentPos, true)){
		// open software button
		qtbrynhildr->toggleSoftwareButton();
	  }
	  if (inCheckingKeyboardOpen &&
		  !softwareKeyboardRect.contains(currentPos, true)){
		// open software keyboard
		qtbrynhildr->toggleSoftwareKeyboard();
	  }
	}
	else {
	  qreal distance = QLineF(touchPoint.startPos(), touchPoint.pos()).length();
	  if (distance < QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		if (!settings->getOnShowSoftwareButton()){
		  QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												   touchPoint.pos(),
												   Qt::NoButton,
												   Qt::NoButton,
												   Qt::NoModifier);
		  QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
													touchPoint.pos(),
													Qt::LeftButton,
													Qt::LeftButton,
													Qt::NoModifier);
		  QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													  touchPoint.pos(),
													  Qt::LeftButton,
													  Qt::LeftButton,
													  Qt::NoModifier);
		  // move + L mouse button
		  mouseMoveEvent(moveEvent);
		  mousePressEvent(pressEvent);
		  mouseReleaseEvent(releaseEvent);
		  delete moveEvent;
		  delete pressEvent;
		  delete releaseEvent;
		}
	  }
	}

	// reset open software panel check flags
	inCheckingButtonOpen = false;
	inCheckingKeyboardOpen = false;
	inZooming = settings->getDesktopScalingFactor() > getScalingFactorForFullScreen();
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: Qt 1 finger Moved!";
	}

	if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // Nothing to do
	}
	else if (inZooming){
	  // scroll graphics view
	  QPoint move = lastPos - currentPos;
#if 0 // Yet
	  horizontalScrollBar()->setValue(horizontalScrollBar()->value() + move.x());
	  verticalScrollBar()->setValue(verticalScrollBar()->value() + move.y());
#endif //0 // Yet
	}
	else {
	  // move mouse cursor
	  QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
											   touchPoint.pos(),
											   Qt::NoButton,
											   Qt::NoButton,
											   Qt::NoModifier);
	  mouseMoveEvent(moveEvent);
	  delete moveEvent;
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

  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: Kero 2 fingers Pressed!";
	}

	// set topType
	topType = TOP_TYPE_2POINT;
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: Kero 2 fingers Released!";
	}
	// Nothing to do
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: Kero 2 finger Moved!";
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

  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: Qt 2 fingers Pressed!";
	}
	topType = TOP_TYPE_2POINT;
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: Qt 2 fingers Released!";
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
		  qDebug() << "DP: 2 Point Tap!!";
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
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: Qt 2 finger Moved!";
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
  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: 3 fingers Pressed!";
	}

	// set topType
	topType = TOP_TYPE_3POINT;

	// press
	//keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN); // Windows key press
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: 3 fingers Released!";
	}

	if (topType == TOP_TYPE_3POINT){
	  // press
	  keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN); // Windows key press
	  // release
	  keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYUP); // Windows key release
	}
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: 3 finger Moved!";
	}
	// Nothing to do
  }

  return true;
}

// mouse event 
void DesktopPanel::mousePressEvent(QMouseEvent *event)
{
  QPoint pos = event->pos();
  if (convertToDesktop(pos)){
	//	qDebug() << "DP: mousePressEvent: pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanelWidget->mousePressEvent(newEvent);
	// delete newEvent;
  }
}

void DesktopPanel::mouseReleaseEvent(QMouseEvent *event)
{
  QPoint pos = event->pos();
  if (convertToDesktop(pos)){
	//	qDebug() << "DP: mouseReleaseEvent: pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanelWidget->mouseReleaseEvent(newEvent);
	// delete newEvent;
  }
}

void DesktopPanel::mouseDoubleClickEvent(QMouseEvent *event)
{
  QPoint pos = event->pos();
  if (convertToDesktop(pos)){
	//	qDebug() << "DP: mouseDoubleClickEvent: pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanelWidget->mouseDoubleClickEvent(newEvent);
	// delete newEvent;
  }
}

void DesktopPanel::mouseMoveEvent(QMouseEvent *event)
{
  QPoint pos = event->pos();
  if (convertToDesktop(pos)){
	//	qDebug() << "DP: mouseMoveEvent: pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanelWidget->mouseMoveEvent(newEvent);
	// delete newEvent;
  }
  else {
	QScrollArea::mouseMoveEvent(event);
  }
}

void DesktopPanel::wheelEvent(QWheelEvent *event)
{
  //qDebug() << "DP: wheelEvent : " << event;
  desktopPanelWidget->wheelEvent(event);
}

// keyboard event
void DesktopPanel::keyPressEvent(QKeyEvent *event)
{
  //qDebug() << "DP: KeyPress : " << event;
  desktopPanelWidget->keyPressEvent(event);
}

void DesktopPanel::keyReleaseEvent(QKeyEvent *event)
{
  //qDebug() << "DP: KeyRelease : " << event;
  desktopPanelWidget->keyReleaseEvent(event);
}

// convert to desktop
bool DesktopPanel::convertToDesktop(QPoint &point)
{
  // Yet
}

} // end of namespace qtbrynhildr
