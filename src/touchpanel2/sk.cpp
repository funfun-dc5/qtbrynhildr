// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <iostream> // for TEST

// Qt Header

// Local Header
#include "qtbrynhildr.h"
#include "sk.h"

using namespace std; // for TEST

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SK::SK(KeyBuffer *keyBuffer, QtBrynhildr *qtbrynhildr, QWidget *parent)
  :SoftwareKeyboard(parent)
  ,keyBuffer(keyBuffer)
  ,qtbrynhildr(qtbrynhildr)
  ,settings(qtbrynhildr->getSettings())
#if QTB_NEW_DESKTOPWINDOW
  ,graphicsView(qtbrynhildr->getGraphicsView())
#endif // QTB_NEW_DESKTOPWINDOW
  // for DEBUG
  ,outputLog(false)
{
#if defined(QTB_DEV_TOUCHPANEL)
  setAttribute(Qt::WA_AcceptTouchEvents, true);
#endif // defined(QTB_DEV_TOUCHPANEL)
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// pressed key
void SK::pressedKey(ID_KEY id)
{
  SoftwareKeyboard::pressedKey(id);
}

// released key
void SK::releasedKey(ID_KEY id)
{
  SoftwareKeyboard::releasedKey(id);
  if (id == ID_KEY_Fn){
	qtbrynhildr->toggleSoftwareKeyboard();
  }
}

// key down
void SK::keyDown(uchar key)
{
  SoftwareKeyboard::keyDown(key);

  keyBuffer->put(key, KEYCODE_FLG_KEYDOWN);

  if (outputLog)
	cout << "SK:DOWN"<< endl << flush;
}
  
// key up
void SK::keyUp(uchar key)
{
  SoftwareKeyboard::keyUp(key);

  keyBuffer->put(key, KEYCODE_FLG_KEYUP);

  if (outputLog)
	cout << "SK:UP"<< endl << flush;
}

// for event handling
#if defined(QTB_DEV_TOUCHPANEL)
// event
bool SK::event(QEvent *event)
{
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
		if(touchEvent->touchPointStates() == Qt::TouchPointPressed){
		  qDebug() << "Pressed";
		  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonPress,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);
		  // left mouse button press
		  mousePressEvent(newEvent);
		}
		else if(touchEvent->touchPointStates() == Qt::TouchPointReleased){
		  qDebug() << "Released";
		  // tap
		  qDebug() << "TAP";
		  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonRelease,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);
		  // left mouse button release
		  mouseReleaseEvent(newEvent);
		}
		else if(touchEvent->touchPointStates() == Qt::TouchPointMoved){
		  qDebug() << "Moved:SK";
		  const QTouchEvent::TouchPoint &touchPoint = touchPoints.first();
		  // move mouse cursor
		  QPoint pos = touchPoint.pos().toPoint();
		  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseMove,
												  pos,
												  Qt::NoButton,
												  Qt::NoButton,
												  Qt::NoModifier);

		  // move
		  mouseMoveEvent(newEvent);
		}
	  }
	  else if (touchPoints.count() == 2){
		// 2 fingers
	  }
	  return true;
	}
	break;
  default:
	break;
  }
  return QWidget::event(event);
}
#endif // defined(QTB_DEV_TOUCHPANEL)

#if QTB_NEW_DESKTOPWINDOW
// mouse event
void SK::mousePressEvent(QMouseEvent *event)
{
  SoftwareKeyboard::mousePressEvent(event);
  if (!isOnButton()){
	QPoint pos = event->pos() + this->pos();
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											pos,
											event->button(),
											event->buttons(),
											event->modifiers());
#if defined(QTB_DEV_TOUCHPANEL)
	graphicsView->mouseMoveEventForSP(newEvent);
#endif // defined(QTB_DEV_TOUCHPANEL)
	graphicsView->mousePressEventForSP(newEvent);
  }
}

void SK::mouseReleaseEvent(QMouseEvent *event)
{
  SoftwareKeyboard::mouseReleaseEvent(event);
  if (!isOnButton()){
	QPoint pos = event->pos() + this->pos();
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											pos,
											event->button(),
											event->buttons(),
											event->modifiers());
	graphicsView->mouseReleaseEventForSP(newEvent);
  }
}

void SK::mouseMoveEvent(QMouseEvent *event)
{
  SoftwareKeyboard::mouseMoveEvent(event);
  if (!isOnButton()){
	QPoint pos = event->pos() + this->pos();
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											pos,
											event->button(),
											event->buttons(),
											event->modifiers());
	graphicsView->mouseMoveEventForSP(newEvent);
  }
}
#endif // QTB_NEW_DESKTOPWINDOW

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr
