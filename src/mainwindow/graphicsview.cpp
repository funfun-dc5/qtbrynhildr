// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QCursor>
#include <QPainter>
#if defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
#include <QScrollBar>
#include <QTouchEvent>
#endif // defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
#include <QTransform>

#include <QDebug>

// Local Header
#include "mainwindow/graphicsview.h"
#include "qtbrynhildr.h"

namespace qtbrynhildr {

// constructor
GraphicsView::GraphicsView(QGraphicsScene *scene, QtBrynhildr *qtbrynhildr, QWidget *parent)
  :
  QGraphicsView(scene, parent),
  qtbrynhildr(qtbrynhildr),
  settings(qtbrynhildr->getSettings()),
  desktopPanel(qtbrynhildr->getDesktopPanel()),
  keyBuffer(qtbrynhildr->getDesktopPanel()->getKeyBuffer()),
#if defined(QTB_DEV_TOUCHPANEL)
  scalingFactor(1.0),
  scalingFactorForFullScreen(1.0),
#endif // defined(QTB_DEV_TOUCHPANEL)
  // for DEBUG
  outputLog(true)
{
#if defined(QTB_DEV_TOUCHPANEL)
  setAttribute(Qt::WA_AcceptTouchEvents, true);
#else // defined(QTB_DEV_TOUCHPANEL)
#if !QTB_TEST_TOUCHPANEL_ON_DESKTOP
  setMouseTracking(true);
#endif // !QTB_TEST_TOUCHPANEL_ON_DESKTOP
#endif // defined(QTB_DEV_TOUCHPANEL)
  setRenderHint(QPainter::Antialiasing, false);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  //  setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  //  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setDragMode(QGraphicsView::NoDrag);
  setResizeAnchor(QGraphicsView::AnchorViewCenter);
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  setAlignment(Qt::AlignCenter);
#if defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#endif // defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
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

  transform.scale(scalingFactor, scalingFactor);
  setTransform(transform);
  // get window size
  QSize windowSize = size();
  QSize desktopSize = desktopPanel->getDesktopSize();
  QSize currentDesktopSize = desktopSize * scalingFactor;
  QSize diffSize = currentDesktopSize - windowSize;
  qDebug() << "--- setScale() ---";
  qDebug() << "windowSize = " << windowSize;
  qDebug() << "desktopPanel->getDesktopSize() = " << desktopSize;
  qDebug() << "currentDesktopSize = " << currentDesktopSize;
  qDebug() << "diffSize = " << diffSize;

#if 0 // for TEST
#if defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
#if 1
  //  verticalScrollBar()->setRange(1, diffSize.height()-1);
  //  verticalScrollBar()->setValue(diffSize.height()/2);
  //  verticalScrollBar()->setPageStep(windowSize.height());
  verticalScrollBar()->setValue(verticalScrollBar()->minimum());
  //  verticalScrollBar()->setValue(verticalScrollBar()->maximum());

  //  horizontalScrollBar()->setRange(1, diffSize.width()-1);
  //  horizontalScrollBar()->setValue(diffSize.width()/2);
  //  horizontalScrollBar()->setPageStep(windowSize.width());
  horizontalScrollBar()->setValue(horizontalScrollBar()->minimum());
  //  horizontalScrollBar()->setValue(horizontalScrollBar()->maximum());
#else
  verticalScrollBar()->setPageStep(windowSize.height());
  horizontalScrollBar()->setPageStep(windowSize.width());
  verticalScrollBar()->setRange(0, currentDesktopSize.height() - windowSize.height());
  horizontalScrollBar()->setRange(0, currentDesktopSize.width() - windowSize.width());
  verticalScrollBar()->setValue(0);
  horizontalScrollBar()->setValue(0);
#endif
  qDebug() << "verticalScrollBar.minimum   = " << verticalScrollBar()->minimum();
  qDebug() << "verticalScrollBar.maximum   = " << verticalScrollBar()->maximum();
  qDebug() << "verticalScrollBar.value     = " << verticalScrollBar()->value();
  qDebug() << "horizontalScrollBar.minimum = " << horizontalScrollBar()->minimum();
  qDebug() << "horizontalScrollBar.maximum = " << horizontalScrollBar()->maximum();
  qDebug() << "horizontalScrollBar.value   = " << horizontalScrollBar()->value();
#endif // defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
#endif
#if defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
  // save scaling factor
  this->scalingFactor = scalingFactor;
#endif // defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
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
	  qDebug() << "event type  = " << event->type();

	  QTouchEvent *touchEvent = (QTouchEvent*)event;
	  qDebug() << "TouchStates = " << touchEvent->touchPointStates();

	  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
	  if (touchPoints.count() == 1){
		// 1 finger
		qDebug() << "== 1 Point == ";
		const QTouchEvent::TouchPoint &touchPoint = touchPoints.first();
		qDebug() << "pos = " << touchPoint.pos();
		//		break; // to QGraphicsView::viewportEvent(event)
		if(touchEvent->touchPointStates() == Qt::TouchPointReleased){
		  qreal distance = QLineF(touchPoint.startPos(), touchPoint.pos()).length();
		  if (distance < 20){ // for TEST (Nexus7(2013):1920x1200)
			// tap
			qDebug() << "TAP";
			if (!settings->getOnShowSoftwareButton()){
			  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonPress,
													  touchPoint.pos(),
													  Qt::LeftButton,
													  Qt::LeftButton,
													  Qt::NoModifier);
			  // move + L mouse button
			  mouseMoveEvent(newEvent);
			  mousePressEvent(newEvent);
			  mouseReleaseEvent(newEvent);
			}
		  }
		  else {
			QPoint currentPos = touchPoint.pos().toPoint();
			QPoint startPos = touchPoint.startPos().toPoint();
#if 1 // for TEST
			qDebug() << "softwareButtonRect = " << softwareButtonRect;
			qDebug() << "softwareKeyboardRect = " << softwareKeyboardRect;
			qDebug() << "currentPos = " << currentPos;
			qDebug() << "startPos = " << startPos;
#endif // for TEST
			// show software button
			if (softwareButtonRect.contains(startPos) &&
				!softwareButtonRect.contains(currentPos, true)){
			  qtbrynhildr->toggleSoftwareButton();
			}
			// show software keyboard
			else if (softwareKeyboardRect.contains(startPos) &&
					 !softwareKeyboardRect.contains(currentPos, true)){
			  qtbrynhildr->toggleSoftwareKeyboard();
			}
		  }
		}
		else if(touchEvent->touchPointStates() == Qt::TouchPointMoved){
		  qDebug() << "Moved:GraphicsView";
		  QPoint startPos = touchPoint.startPos().toPoint();
		  QPoint currentPos = touchPoint.pos().toPoint();
		  QPoint lastPos = touchPoint.lastPos().toPoint();
		  // move mouse cursor
		  if (softwareButtonRect.contains(startPos, true) ||
			  softwareKeyboardRect.contains(startPos, true)){
			// Nothing to do
		  }
		  else if (settings->getDesktopScalingFactor() <= scalingFactorForFullScreen){
#if 1 // for TEST
			if (!settings->getOnShowSoftwareButton()){
			  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonPress,
													  touchPoint.pos(),
													  Qt::LeftButton,
													  Qt::LeftButton,
													  Qt::NoModifier);
			  // move
			  mouseMoveEvent(newEvent);
			}
			else {
			  QPoint currentPos = touchPoint.pos().toPoint();
			  QPoint lastPos = touchPoint.lastPos().toPoint();
			  QPoint move = currentPos - lastPos;
			  qDebug() << "move = " << move;
			  mouseMoveRelatively(move);
			}
#else // for TEST
			QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonPress,
													touchPoint.pos(),
													Qt::LeftButton,
													Qt::LeftButton,
													Qt::NoModifier);
			// move
			mouseMoveEvent(newEvent);
#endif // for TEST
		  }
		  // scroll desktop
		  else {
			QPoint move = lastPos - currentPos;
			qDebug() << "scroll move = " << move;
			horizontalScrollBar()->setValue(horizontalScrollBar()->value() + move.x());
			verticalScrollBar()->setValue(verticalScrollBar()->value() + move.y());
		  }
		}
	  }
	  else if (touchPoints.count() == 2){
		// 2 fingers
		qDebug() << "== 2 Point == ";
		if (!(settings->getOnShowSoftwareKeyboard() || settings->getOnShowSoftwareButton())){
		  const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
		  const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
		  qreal currentScalingFactor =
			QLineF(touchPoint0.pos(), touchPoint1.pos()).length() /
			QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
		  qDebug() << "currentScalingFactor = " << currentScalingFactor;
		  if (currentScalingFactor < 1.0){
			scalingFactor -= 0.02;
		  }
		  else {
			scalingFactor += 0.02;
		  }
		  settings->setDesktopScalingFactor(scalingFactor);
		  scalingFactor = settings->getDesktopScalingFactor();
		  qDebug() << "scalingFactor = " << scalingFactor;
		  setScale(scalingFactor);
		}
	  }
	  else {
		// others
		qDebug() << "== many Point == ";
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
#if QTB_TEST_TOUCHPANEL_ON_DESKTOP
	// Left Button -> move cursor
	if (event->button() == Qt::LeftButton){
	  desktopPanel->mouseMoveEvent(newEvent);
	}
#else // QTB_TEST_TOUCHPANEL_ON_DESKTOP
	desktopPanel->mousePressEvent(newEvent);
#endif // QTB_TEST_TOUCHPANEL_ON_DESKTOP
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
  }
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
  //  cout << "mouseDoubleClicEvent" << endl << flush;
#if defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
  Q_UNUSED(event);
#else // defined(QTB_DEV_TOUCHPANEL)
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
  }
#endif // defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
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
  }
  else {
	QGraphicsView::mouseMoveEvent(event);
  }
}

#if defined(QTB_DEV_DESKTOP) && !QTB_TEST_TOUCHPANEL_ON_DESKTOP
void GraphicsView::wheelEvent(QWheelEvent *event)
{
  //  cout << "wheelEvent" << endl << flush;
  desktopPanel->wheelEvent(event);
}
#endif // defined(QTB_DEV_DESKTOP) && !QTB_TEST_TOUCHPANEL_ON_DESKTOP

// keyboard event
void GraphicsView::keyPressEvent(QKeyEvent *event)
{
  //  cout << "keyPressEvent" << endl << flush;
  desktopPanel->keyPressEvent(event);
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
  //  cout << "keyReleaseEvent" << endl << flush;
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
