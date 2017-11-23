// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>
#if defined(QTB_DEV_TOUCHPANEL)
#include <QTouchEvent>
#endif // defined(QTB_DEV_TOUCHPANEL)
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
}

// destructor
GraphicsView::~GraphicsView()
{
}

// scale
void GraphicsView::setScale(qreal scalingFactor)
{
  QTransform transform;

  transform.scale(scalingFactor, scalingFactor);
  setTransform(transform);
}

#if defined(QTB_DEV_TOUCHPANEL)
// event
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
			QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonPress,
													touchPoint.pos(),
													Qt::LeftButton,
													Qt::LeftButton,
													Qt::NoModifier);
			mousePressEvent(newEvent);
#if 0
			QRectF rect = sceneRect();
			qDebug() << "sceneRect = " << rect;
			setSceneRect(-960, -540, 960, 540);
			rect = sceneRect();
			qDebug() << "sceneRect = " << rect;
#endif
		  }
		  else {
			QPoint currentPos = touchPoint.pos().toPoint();
			QPoint startPos = touchPoint.startPos().toPoint();
#if 0
			qDebug() << "softwareButtonRect = " << softwareButtonRect;
			qDebug() << "softwareKeyboardRect = " << softwareKeyboardRect;
			qDebug() << "currentPos = " << currentPos;
			qDebug() << "startPos = " << startPos;
#endif
			if (softwareButtonRect.contains(startPos) &&
				!softwareButtonRect.contains(currentPos)){
			  qtbrynhildr->toggleSoftwareButton();
			}
			else if (softwareKeyboardRect.contains(startPos) &&
					 !softwareKeyboardRect.contains(currentPos)){
			  qtbrynhildr->toggleSoftwareKeyboard();
			}
		  }
		}
	  }
	  else if (touchPoints.count() == 2){
		// 2 fingers
		qDebug() << "== 2 Point == ";
		const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
		const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
#if 1
		qreal currentScalingFactor =
		  QLineF(touchPoint0.pos(), touchPoint1.pos()).length() /
		  QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
		currentScalingFactor *= 0.4;
		qreal scalingFactor = settings->getDesktopScalingFactor();
		if(touchEvent->touchPointStates() & Qt::TouchPointReleased){
		  scalingFactor *= currentScalingFactor;
		  currentScalingFactor = 1.0;
		}
		settings->setDesktopScalingFactor(scalingFactor * currentScalingFactor);
		scalingFactor = settings->getDesktopScalingFactor();
		setScale(scalingFactor);
#else
		if(touchEvent->touchPointStates() & Qt::TouchPointReleased){
		  qreal currentScalingFactor =
			QLineF(touchPoint0.pos(), touchPoint1.pos()).length() /
			QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
		  qreal scalingFactor = settings->getDesktopScalingFactor();
		  scalingFactor *= currentScalingFactor;
		  settings->setDesktopScalingFactor(scalingFactor);
		  scalingFactor = settings->getDesktopScalingFactor();
		  setScale(scalingFactor);
		}
#endif
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
#if defined(QTB_DEV_TOUCHPANEL)
	desktopPanel->mouseMoveEvent(newEvent);
#else // defined(QTB_DEV_TOUCHPANEL)
#if QTB_TEST_TOUCHPANEL_ON_DESKTOP
	// Left Button -> move cursor
	if (event->button() == Qt::LeftButton){
	  desktopPanel->mouseMoveEvent(newEvent);
	}
	// Right Button -> move center of view
	else if (event->button() == Qt::RightButton){
	  QPointF posF = mapToScene(event->pos());
	  centerOn(posF);
	}
#else // QTB_TEST_TOUCHPANEL_ON_DESKTOP
	desktopPanel->mousePressEvent(newEvent);
#endif // QTB_TEST_TOUCHPANEL_ON_DESKTOP
#endif // defined(QTB_DEV_TOUCHPANEL)
  }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  //  cout << "mouseReleaseEvent" << endl << flush;
#if defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
  Q_UNUSED(event);
#else // defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
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
#endif // defined(QTB_DEV_TOUCHPANEL) || QTB_TEST_TOUCHPANEL_ON_DESKTOP
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

#if defined(QTB_DEV_DESKTOP) && !QTB_TEST_TOUCHPANEL_ON_DESKTOP
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
