// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QCursor>
#include <QPainter>
#if defined(QTB_DEV_TOUCHPANEL)
#include <QScrollBar>
#include <QTouchEvent>
#endif // defined(QTB_DEV_TOUCHPANEL)
#include <QTransform>

#if defined(QTB_DEBUG)
#include <QDebug>
#endif // defined(QTB_DEBUG)

// Local Header
#include "mainwindow/graphicsview.h"
#include "qtbrynhildr.h"

namespace qtbrynhildr {

// constructor
GraphicsView::GraphicsView(QGraphicsScene *scene, QtBrynhildr *qtbrynhildr, QWidget *parent)
  :QGraphicsView(scene, parent)
  ,qtbrynhildr(qtbrynhildr)
  ,settings(qtbrynhildr->getSettings())
  ,desktopPanel(qtbrynhildr->getDesktopPanel())
  ,keyBuffer(qtbrynhildr->getDesktopPanel()->getKeyBuffer())
#if defined(QTB_DEV_TOUCHPANEL)
  ,scalingFactor(1.0)
#endif // defined(QTB_DEV_TOUCHPANEL)
  // for DEBUG
  ,outputLog(false)
{
#if defined(QTB_DEV_TOUCHPANEL)
  setAttribute(Qt::WA_AcceptTouchEvents, true);
#endif // defined(QTB_DEV_TOUCHPANEL)
  setMouseTracking(true);
  setRenderHint(QPainter::Antialiasing, false);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  //  setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  //  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setDragMode(QGraphicsView::NoDrag);
  setResizeAnchor(QGraphicsView::AnchorViewCenter);
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  setAlignment(Qt::AlignCenter);
#if defined(QTB_DEV_TOUCHPANEL)
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#endif // defined(QTB_DEV_TOUCHPANEL)
  setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  setViewportMargins(0, 0, 0, 0);
}

// destructor
GraphicsView::~GraphicsView()
{
}

// size hint
QSize GraphicsView::sizeHint() const
{
  return desktopPanel->getSize();
}

// scale
void GraphicsView::setScale(qreal scalingFactor)
{
  QTransform transform;

  qreal tmpScalingFactor = scalingFactor/getScalingFactorForFullScreen();
  transform.scale(tmpScalingFactor, tmpScalingFactor);
  setTransform(transform);

#if defined(QTB_DEBUG)
  // get window size
  QSize windowSize = size();
  QSize desktopImageSize = settings->getDesktopImageSize();
  QSize currentDesktopSize = desktopImageSize * scalingFactor;
  QSize diffSize = currentDesktopSize - windowSize;
  if (outputLog){
	qDebug() << "--- setScale() ---";
	qDebug() << "windowSize = " << windowSize;
	qDebug() << "desktopPanel->getDesktopSize() = " << desktopImageSize;
	qDebug() << "currentDesktopSize = " << currentDesktopSize;
	qDebug() << "diffSize = " << diffSize;
  }
#if defined(QTB_DEV_TOUCHPANEL)
  verticalScrollBar()->setValue(verticalScrollBar()->minimum());
  horizontalScrollBar()->setValue(horizontalScrollBar()->minimum());
  if (outputLog){
	qDebug() << "verticalScrollBar.minimum   = " << verticalScrollBar()->minimum();
	qDebug() << "verticalScrollBar.maximum   = " << verticalScrollBar()->maximum();
	qDebug() << "verticalScrollBar.value     = " << verticalScrollBar()->value();
	qDebug() << "horizontalScrollBar.minimum = " << horizontalScrollBar()->minimum();
	qDebug() << "horizontalScrollBar.maximum = " << horizontalScrollBar()->maximum();
	qDebug() << "horizontalScrollBar.value   = " << horizontalScrollBar()->value();
  }
#endif // defined(QTB_DEV_TOUCHPANEL)
#endif // defined(QTB_DEBUG)

#if defined(QTB_DEV_TOUCHPANEL)
  // save scaling factor
  this->scalingFactor = scalingFactor;
#endif // defined(QTB_DEV_TOUCHPANEL)
}

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
// mouse press event software panel
void GraphicsView::mousePressEventForSP(QMouseEvent *event)
{
  mousePressEvent(event);
}

// mouse release event software panel
void GraphicsView::mouseReleaseEventForSP(QMouseEvent *event)
{
  mouseReleaseEvent(event);
}

// mouse move event software panel
void GraphicsView::mouseMoveEventForSP(QMouseEvent *event)
{
  mouseMoveEvent(event);
}

// mouse move
void GraphicsView::mouseMove(QPoint mousePos, bool marker)
{
  desktopPanel->mouseMove(mousePos, marker);
}

// mouse move relatively
void GraphicsView::mouseMoveRelatively(QPoint mousePos, bool marker)
{
  mousePos /= scalingFactor;
  desktopPanel->mouseMoveRelatively(mousePos, marker);
}
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

#if defined(QTB_DEV_TOUCHPANEL)
// viewport event
bool GraphicsView::viewportEvent(QEvent *event){
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

	  // flags
	  static TOP_TYPE topType = TOP_TYPE_UNKNOWN;

	  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
	  int touchPointCount = touchPoints.count();
	  if (touchPointCount == 1){ // 1 finger
		// check software panel
		if (settings->getOnShowSoftwareButton()){
		  // Nothing to do
		  return true;
		}

		// -----------------------------------------------------------------------------------
		// KeroRemote Compatible Operation (1 point tap)
		// -----------------------------------------------------------------------------------
		if (settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_KEROREMOTE){
		  // last position of mouse cursor
		  static QPointF lastPos;

		  // open software panel check flags
		  static bool inCheckingButtonOpen = false;
		  static bool inCheckingKeyboardOpen = false;
		  // for zoom
		  static bool inZooming = false;
		  static QDateTime pressTimeInZooming;

		  const QTouchEvent::TouchPoint &touchPoint = touchPoints.first();

		  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
			if (outputLog){
			  qDebug() << "GV: 1 Pressed!";
			}
			topType = TOP_TYPE_1POINT;
			if (softwareButtonRect.contains(touchPoint.pos().toPoint())){
			  if (outputLog){
				qDebug() << "GV: into software button open area";
			  }
			  inCheckingButtonOpen = true;
			}
			else if (softwareKeyboardRect.contains(touchPoint.pos().toPoint())){
			  if (outputLog){
				qDebug() << "GV: into software keyboard open area";
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
		  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
			if (outputLog){
			  qDebug() << "GV: 1 Released!";
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
		  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
			if (outputLog){
			  qDebug() << "GV: 1 Moved!";
			}
			if (inCheckingButtonOpen || inCheckingKeyboardOpen){
			  // Nothing to do
			}
			else if (inZooming){
			  // scroll graphics view
			  QPoint currentPos = touchPoint.pos().toPoint();
			  QPoint lastPos = touchPoint.lastPos().toPoint();
			  QPoint move = lastPos - currentPos;
			  horizontalScrollBar()->setValue(horizontalScrollBar()->value() + move.x());
			  verticalScrollBar()->setValue(verticalScrollBar()->value() + move.y());
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
		}
		// -----------------------------------------------------------------------------------
		// Qt Brynhildr Operation (1 point tap)
		// -----------------------------------------------------------------------------------
		else if (settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_QTBRYNHILDR){
		  // open software panel check flags
		  static bool inCheckingButtonOpen = false;
		  static bool inCheckingKeyboardOpen = false;
		  // for zoom
		  static bool inZooming = false;

		  const QTouchEvent::TouchPoint &touchPoint = touchPoints.first();

		  QPoint currentPos = touchPoint.pos().toPoint();
		  QPoint lastPos = touchPoint.lastPos().toPoint();

		  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
			if (outputLog){
			  qDebug() << "GV: 1 Pressed!";
			}
			topType = TOP_TYPE_1POINT;
			// check for software keyboard/button
			if (softwareButtonRect.contains(currentPos)){
			  if (outputLog){
				qDebug() << "GV: into software button open area";
			  }
			  inCheckingButtonOpen = true;
			}
			else if (softwareKeyboardRect.contains(currentPos)){
			  if (outputLog){
				qDebug() << "GV: into software keyboard open area";
			  }
			  inCheckingKeyboardOpen = true;
			}
		  }
		  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
			if (outputLog){
			  qDebug() << "GV: 1 Released!";
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
		  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
			if (outputLog){
			  qDebug() << "GV: 1 Moved!";
			}

			if (inCheckingButtonOpen || inCheckingKeyboardOpen){
			  // Nothing to do
			}
			else if (inZooming){
			  // scroll graphics view
			  QPoint move = lastPos - currentPos;
			  horizontalScrollBar()->setValue(horizontalScrollBar()->value() + move.x());
			  verticalScrollBar()->setValue(verticalScrollBar()->value() + move.y());
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
		}
		// -----------------------------------------------------------------------------------
		// Unknown Touchpanel Operation
		// -----------------------------------------------------------------------------------
		else {
		  qDebug() << "Unknown Touchpanel Operation!";
		}
	  }
	  else if (touchPointCount == 2){ // 2 fingers
		// check software panel
		if (settings->getOnShowSoftwareKeyboard() || settings->getOnShowSoftwareButton()){
		  // Nothing to do
		  return true;
		}

		if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
		  if (outputLog){
			qDebug() << "GV: 2 Pressed!";
		  }
		  topType = TOP_TYPE_2POINT;
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
		  if (outputLog){
			qDebug() << "GV: 2 Released!";
		  }
		  if (topType != TOP_TYPE_2POINT){
			// Nothing to do
		  }
		  else {
			if (settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_QTBRYNHILDR){
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
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
		  if (outputLog){
			qDebug() << "GV: 2 Moved!";
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
		  }
		  settings->setDesktopScalingFactor(scalingFactor);
		  scalingFactor = settings->getDesktopScalingFactor();
		  setScale(scalingFactor);
		}
	  }
	  else { // touchPointCount >= 3 fingers
		if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
		  topType = TOP_TYPE_3POINT;
		  // press
		  //keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN); // Windows key press
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
		  if (topType == TOP_TYPE_3POINT){
			// press
			keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN); // Windows key press
			// release
			keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYUP); // Windows key release
		  }
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
		  // Nothing to do
		}
	  }
	  return true;
	}
	break;
  default:
	break;
  }

  return QGraphicsView::viewportEvent(event);
}
#endif // defined(QTB_DEV_TOUCHPANEL)

// widget leave event
void GraphicsView::leaveEvent(QEvent *event)
{
  Q_UNUSED(event);

  if (settings->getOnClipCursor()){
	QCursor::setPos(mapToGlobal(desktopPanel->getCurrentMousePos()));
  }
}

// mouse event
void GraphicsView::mousePressEvent(QMouseEvent *event)
{
  //  cout << "mousePressEvent" << endl << flush;
  QPoint pos = mapToScene(event->pos()).toPoint();
  //  qDebug() << "pos of scene = " << pos;
  if (convertToDesktop(pos)){
	//	qDebug() << "pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanel->mousePressEvent(newEvent);
	// delete newEvent;
  }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  //  cout << "mouseReleaseEvent" << endl << flush;
  QPoint pos = mapToScene(event->pos()).toPoint();
  //  qDebug() << "pos of scene = " << pos;
  if (convertToDesktop(pos)){
	//	qDebug() << "pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanel->mouseReleaseEvent(newEvent);
	// delete newEvent;
  }
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
  //  cout << "mouseDoubleClicEvent" << endl << flush;
  QPoint pos = mapToScene(event->pos()).toPoint();
  //  qDebug() << "pos of scene = " << pos;
  if (convertToDesktop(pos)){
	//	qDebug() << "pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanel->mouseDoubleClickEvent(newEvent);
	// delete newEvent;
  }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
  //  cout << "mouseMoveEvent" << endl << flush;
  QPoint pos = mapToScene(event->pos()).toPoint();
  //  qDebug() << "pos of scene = " << pos;
  if (convertToDesktop(pos)){
	//	qDebug() << "pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanel->mouseMoveEvent(newEvent);
	// delete newEvent;
  }
  else {
	QGraphicsView::mouseMoveEvent(event);
  }
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
  //  cout << "wheelEvent" << endl << flush;
  desktopPanel->wheelEvent(event);
}

// keyboard event
void GraphicsView::keyPressEvent(QKeyEvent *event)
{
  //  cout << "keyPressEvent" << endl << flush;
  //qDebug() << "KeyPress : " << event;
  desktopPanel->keyPressEvent(event);
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
  //  cout << "keyReleaseEvent" << endl << flush;
  //qDebug() << "KeyRelease : " << event;
  desktopPanel->keyReleaseEvent(event);
}

#if QTB_DRAG_AND_DROP_SUPPORT
// drag and drop
void GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
  desktopPanel->dragEnterEvent(event);
}
void GraphicsView::dropEvent(QDropEvent *event)
{
  desktopPanel->dropEvent(event);
}
#endif // QTB_DRAG_AND_DROP_SUPPORT

// viewport size hint
QSize GraphicsView::viewportSizeHint() const
{
  return desktopPanel->getSize();
}

// convert to desktop
bool GraphicsView::convertToDesktop(QPoint &point)
{
  QSize size = desktopPanel->getSize();
  QRect rect(-size.width()/2, -size.height()/2, size.width(), size.height());
  if (rect.contains(point)){
	point.setX(point.x() + size.width()/2);
	point.setY(point.y() + size.height()/2);
	return true;
  }
  else {
	return false;
  }
}

//----------------------------------------------------------------------
// native event filter
//----------------------------------------------------------------------
#if defined(Q_OS_WIN)
bool GraphicsView::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
  return desktopPanel->nativeEventFilter(eventType, message, result);
}
#endif // defined(Q_OS_WIN)

} // end of namespace qtbrynhildr
