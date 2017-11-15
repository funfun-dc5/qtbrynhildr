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
  desktopPanel(qtbrynhildr->getDesktopPanel()),
// for DEBUG
  outputLog(true)
{
#if defined(QTB_DEV_TOUCHPANEL)
  setAttribute(Qt::WA_AcceptTouchEvents, true);
#endif // defined(QTB_DEV_TOUCHPANEL)
  setRenderHint(QPainter::Antialiasing, false);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  //  setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  //  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setDragMode(QGraphicsView::NoDrag);
  setResizeAnchor(QGraphicsView::AnchorViewCenter);
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  setAlignment(Qt::AlignCenter);
#if !QTB_DESKTOPWINDOW
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#endif // !QTB_DESKTOPWINDOW
}

// destructor
GraphicsView::~GraphicsView()
{
}

// scale
void GraphicsView::setScale(qreal scalingFactor)
{
  QMatrix matrix;

  matrix.scale(scalingFactor, scalingFactor);
  setMatrix(matrix);
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
		  qtbrynhildr->toggleSoftwareButton();
		}
	  }
	  else if (touchPoints.count() == 2){
		// 2 fingers
		qDebug() << "== 2 Point == ";
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
	desktopPanel->mouseMoveEvent(newEvent);
#if 0 // for TEST
	desktopPanel->mousePressEvent(newEvent);
#endif // 0 // for TEST
  }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  //  cout << "mouseReleaseEvent" << endl << flush;
#if 1 // for TEST
  Q_UNUSED(event);
#else // for TEST
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
#endif // for TEST
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
  //  cout << "mouseDoubleClicEvent" << endl << flush;
#if 1 // for TEST
  Q_UNUSED(event);
#else // for TEST
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
#endif // for TEST
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
  //  cout << "mouseMoveEvent" << endl << flush;
#if 1 // for TEST
  Q_UNUSED(event);
#else // for TEST
  //  cout << "mouseDoubleClicEvent" << endl << flush;
  QPoint pos = mapToScene(event->pos()).toPoint();
  //  qDebug() << "pos of scene = " << pos;
  if (convertToDesktop(pos)){
	qDebug() << "pos of desktop = " << pos;
  }
  else {
	QGraphicsView::mouseMoveEvent(event);
  }
#endif // for TEST
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
  desktopPanel->keyPressEvent(event);
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
  //  cout << "keyReleaseEvent" << endl << flush;
  desktopPanel->keyReleaseEvent(event);
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

} // end of namespace qtbrynhildr
