// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

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
#if defined(QTB_DEV_TOUCHPANEL)
  ,desktopPanel(qtbrynhildr->getDesktopPanel())
#endif // defined(QTB_DEV_TOUCHPANEL)
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
  if (id == ID_KEY_Fn){
	qtbrynhildr->toggleSoftwareKeyboard();
  }
}

// released key
void SK::releasedKey(ID_KEY id)
{
  SoftwareKeyboard::releasedKey(id);
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
	  QTouchEvent *touchEvent = (QTouchEvent*)event;
	  if (outputLog){
		qDebug() << "event type  = " << event->type();
		qDebug() << "TouchStates = " << touchEvent->touchPointStates();
	  }

	  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
	  int touchPointCount = touchPoints.count();
	  if (touchPointCount == 1){ // 1 finger

		const QTouchEvent::TouchPoint &touchPoint = touchPoints.first();

		if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
		  if (outputLog){
			qDebug() << "SK: 1 Pressed!";
			qDebug() << "pos = " << touchPoint.pos().toPoint();
		  }
		  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonPress,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);
		  // left mouse button press
		  mousePressEvent(newEvent);
		  delete newEvent;
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
		  if (outputLog){
			qDebug() << "SK: 1 Released!";
			qDebug() << "pos = " << touchPoint.pos().toPoint();
		  }
		  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonRelease,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);
		  // left mouse button release
		  mouseReleaseEvent(newEvent);
		  delete newEvent;
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
		  if (outputLog){
			qDebug() << "SK: 1 Moved!";
			qDebug() << "pos = " << touchPoint.pos().toPoint();
		  }
		  // move mouse cursor
		  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseMove,
												  touchPoint.pos(),
												  Qt::NoButton,
												  Qt::NoButton,
												  Qt::NoModifier);

		  // move
		  mouseMoveEvent(newEvent);
		  delete newEvent;
		}
	  }
	  else if (touchPointCount == 2){ // 2 fingers
		// Nothing to do
	  }
	  else { // touchPointCount >= 3 fingers
		// Nothing to do
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

#if defined(QTB_DEV_TOUCHPANEL)
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
	desktopPanel->mouseMoveEventForSP(newEvent);
#endif // defined(QTB_DEV_TOUCHPANEL)
	desktopPanel->mousePressEventForSP(newEvent);
	//delete newEvent;
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
	desktopPanel->mouseReleaseEventForSP(newEvent);
	//delete newEvent;
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
	desktopPanel->mouseMoveEventForSP(newEvent);
	//delete newEvent;
  }
}
#endif // defined(QTB_DEV_TOUCHPANEL)

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr
