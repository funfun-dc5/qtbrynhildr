// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <iostream> // for TEST

// Qt Header

// Local Header
#include "qtbrynhildr.h"
#include "sb.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SB::SB(MouseBuffer *mouseBuffer, QtBrynhildr *qtbrynhildr, QWidget *parent)
  :SoftwareButton(parent)
  ,mouseBuffer(mouseBuffer)
  ,qtbrynhildr(qtbrynhildr)
  ,settings(qtbrynhildr->getSettings())
#if defined(QTB_DEV_TOUCHPANEL)
  ,desktopPanel(qtbrynhildr->getDesktopPanel())
#endif // defined(QTB_DEV_TOUCHPANEL)
  ,previousClickButton(MouseBuffer::MOUSE_BUTTON_INVALID)
  ,previousClickTime(QDateTime::currentDateTime())
  ,pressedMouseLeftButton(false)
  ,pressedMouseRightButton(false)
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
// pressed button
void SB::pressedButton(SoftwareButton::ID_BUTTON id)
{
  if (outputLog){
	std::cout << "SB: Pressed : ID_BUTTON = " << id << std::endl << std::flush;
  }

  bool updated = false;
  MOUSE_BUTTON value = MOUSE_BUTTON_DOWN;

  switch (id){
  case ID_BUTTON_1:
	// Fn
	qtbrynhildr->toggleSoftwareButton();
	break;
  case ID_BUTTON_2:
	// Monitor
	{
	  // set next monitor number
	  MONITOR_COUNT monitorCount = settings->getMonitorCount();
	  MONITOR_NO monitorNo = settings->getMonitorNo();
	  monitorNo =  (monitorNo < monitorCount) ? monitorNo + 1 : 1;
	  settings->setMonitorNo(monitorNo);
	}
	break;
  case ID_BUTTON_3:
	// Sound
	break;
  case ID_BUTTON_4:
	// Sound OFF
	settings->setOnSound(false);
	break;
  case ID_BUTTON_5:
	// Sound ON
	settings->setOnSound(true);
	break;
  case ID_BUTTON_6:
	// Info
	break;
  case ID_BUTTON_7:
	// Sound Cache
	break;
  case ID_BUTTON_8:
	// Sound Cache (100 ms)
	settings->setSoundCacheTime(100);
	break;
  case ID_BUTTON_9:
	// Sound Cache (200 ms)
	settings->setSoundCacheTime(200);
	break;
  case ID_BUTTON_10:
	// Sound Cache (300 ms)
	settings->setSoundCacheTime(300);
	break;
  case ID_BUTTON_11:
	// Sound Cache (400 ms)
	settings->setSoundCacheTime(400);
	break;
  case ID_BUTTON_12:
	// Sound Cache (500 ms)
	settings->setSoundCacheTime(500);
	break;
  case ID_BUTTON_13:
	// Option Button
	break;
  case ID_BUTTON_14:
	// Sound Quality
	break;
  case ID_BUTTON_15:
	// Sound Quality (Lowest)
	settings->setSoundQuality(SOUND_QUALITY_MINIMUM);
	updated = true;
	break;
  case ID_BUTTON_16:
	// Sound Quality (Low)
	settings->setSoundQuality(SOUND_QUALITY_LOW);
	updated = true;
	break;
  case ID_BUTTON_17:
	// Sound Quality (Standard)
	settings->setSoundQuality(SOUND_QUALITY_STANDARD);
	updated = true;
	break;
  case ID_BUTTON_18:
	// Sound Quality (High)
	settings->setSoundQuality(SOUND_QUALITY_HIGH);
	updated = true;
	break;
  case ID_BUTTON_19:
	// Sound Quality (Highest)
	settings->setSoundQuality(SOUND_QUALITY_MAXIMUM);
	updated = true;
	break;
  case ID_BUTTON_20:
	// Wheel +
	if (settings->getConnected()){
	  mouseBuffer->putWheel(15); // + 1 tick
	}
	break;
  case ID_BUTTON_21:
	// Public Mode
	break;
  case ID_BUTTON_22:
	// Public Mode 5
	settings->setPublicModeVersion(PUBLICMODE_VERSION5);
	break;
  case ID_BUTTON_23:
	// Public Mode 7
	settings->setPublicModeVersion(PUBLICMODE_VERSION7);
	break;
  case ID_BUTTON_24:
	// Wheel -
	if (settings->getConnected()){
	  mouseBuffer->putWheel(-15); // - 1 tick
	}
	break;
  case ID_BUTTON_25:
	// Video Quality
	break;
  case ID_BUTTON_26:
	// Video Quality (Lowest)
	settings->setVideoQuality(VIDEO_QUALITY_MINIMUM);
	updated = true;
	break;
  case ID_BUTTON_27:
	// Video Quality (Low)
	settings->setVideoQuality(VIDEO_QUALITY_LOW);
	updated = true;
	break;
  case ID_BUTTON_28:
	// Video Quality (Standard)
	settings->setVideoQuality(VIDEO_QUALITY_STANDARD);
	updated = true;
	break;
  case ID_BUTTON_29:
	// Video Quality (High)
	settings->setVideoQuality(VIDEO_QUALITY_HIGH);
	updated = true;
	break;
  case ID_BUTTON_30:
	// Video Quality (Highest)
	settings->setVideoQuality(VIDEO_QUALITY_MAXIMUM);
	updated = true;
	break;
  case ID_BUTTON_31:
	// Mouse Left Button
	if (settings->getConnected()){
	  mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_LEFT, value);
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 diffMSeconds = currentTime.toMSecsSinceEpoch() - previousClickTime.toMSecsSinceEpoch();
	  if (previousClickButton == MouseBuffer::MOUSE_BUTTON_LEFT &&
		  diffMSeconds < settings->getDoubleClickThreshold()){
		value = MOUSE_BUTTON_DBLCLK;
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_LEFT, value);
		if (outputLog){
		  std::cout << "Double Click!!" << std::endl << std::flush;
		}
	  }
	  previousClickButton = MouseBuffer::MOUSE_BUTTON_LEFT;
	  previousClickTime = currentTime;
	  pressedMouseLeftButton = true;
	}
	break;
  case ID_BUTTON_32:
	// Mouse Right Button
	if (settings->getConnected()){
	  mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_RIGHT, value);
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 diffMSeconds = currentTime.toMSecsSinceEpoch() - previousClickTime.toMSecsSinceEpoch();
	  if (previousClickButton == MouseBuffer::MOUSE_BUTTON_RIGHT &&
		  diffMSeconds < settings->getDoubleClickThreshold()){
		value = MOUSE_BUTTON_DBLCLK;
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_RIGHT, value);
		if (outputLog){
		  std::cout << "Double Click!!" << std::endl << std::flush;
		}
	  }
	  previousClickButton = MouseBuffer::MOUSE_BUTTON_RIGHT;
	  previousClickTime = currentTime;
	  pressedMouseRightButton = true;
	}
	break;
  case ID_BUTTON_33:
	// Video FPS
	break;
  case ID_BUTTON_34:
	// Video FPS (Minimum)
	settings->setFrameRate(5);
	break;
  case ID_BUTTON_35:
	// Video FPS (10)
	settings->setFrameRate(10);
	break;
  case ID_BUTTON_36:
	// Video FPS (30)
	settings->setFrameRate(30);
	break;
  case ID_BUTTON_37:
	// Video FPS (60)
	settings->setFrameRate(60);
	break;
  case ID_BUTTON_38:
	// Video FPS (Maximum)
	settings->setFrameRate(FRAMERATE_MAXIMUM);
	break;
  default:
	// error
	break;
  }
  if (updated){
#if USE_KERO_KEYBOARD
	emit refreshMenu();
#endif // USE_KERO_KEYBOARD
  }

  SoftwareButton::pressedButton(id);
}

// released button
void SB::releasedButton(SoftwareButton::ID_BUTTON id)
{
  if (outputLog){
	std::cout << "SB: Released: ID_BUTTON = " << id << std::endl << std::flush;
  }

  MOUSE_BUTTON value = MOUSE_BUTTON_UP;

  switch (id){
  case ID_BUTTON_1:
	// Fn
	break;
  case ID_BUTTON_31:
	// Mouse Left Button
	if (settings->getConnected()){
	  mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_LEFT, value);
	}
	pressedMouseLeftButton = false;
	break;
  case ID_BUTTON_32:
	// Mouse Right Button
	if (settings->getConnected()){
	  mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_RIGHT, value);
	}
	pressedMouseRightButton = false;
	break;
  default:
	// Nothing to do
	break;
  }

  SoftwareButton::releasedButton(id);
}

// for event handling
#if defined(QTB_DEV_TOUCHPANEL)
// event
bool SB::event(QEvent *event)
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

	  // touch point id for mouse button
	  static int idMouseButton = 0;

	  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
	  int touchPointCount = touchPoints.count();
	  if (touchPointCount == 1){ // 1 finger

		const QTouchEvent::TouchPoint &touchPoint = touchPoints.first();
		if (outputLog){
		  qDebug() << "id = " << touchPoint.id();
		}

		if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
		  if (outputLog){
			qDebug() << "SB: 1 Pressed!";
		  }
		  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonPress,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);
		  // left mouse button press
		  mousePressEvent(newEvent);
		  delete newEvent;

		  // save id for mouse button
		  if (pressedMouseLeftButton || pressedMouseRightButton){
			idMouseButton = touchPoint.id();
		  }
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
		  if (outputLog){
			qDebug() << "SB: 1 Released!";
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
			qDebug() << "SB: 1 Moved!";
		  }
		  QPoint currentPos = touchPoint.pos().toPoint();
		  QPoint lastPos = touchPoint.lastPos().toPoint();
		  QPoint move = currentPos - lastPos;
		  if (outputLog){
			qDebug() << "currentPos = " << touchPoint.pos();
			qDebug() << "lastPos = " << touchPoint.lastPos();
			qDebug() << "move = " << move;
		  }
		  if (move != QPoint(0,0))
			desktopPanel->mouseMoveRelatively(move);
		}
	  }
	  else if (touchPointCount == 2){ // 2 fingers
		if (outputLog){
		  const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
		  const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
		  qDebug() << "Point0.id = " << touchPoint0.id();
		  qDebug() << "Point1.id = " << touchPoint1.id();
		}
		if (touchEvent->touchPointStates() & Qt::TouchPointPressed){ // Press
		  if (outputLog){
			qDebug() << "SB: 2 Pressed!";
		  }
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
		  if (outputLog){
			qDebug() << "SB: 2 Released!";
		  }
		  const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
		  const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
		  // check first point
		  if (touchPoint0.state() & Qt::TouchPointReleased){
			if (outputLog){
			  qDebug() << "first point release id = " << touchPoint0.id();
			}
			if (touchPoint0.id() == idMouseButton){
			  // release mouse button
			  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													  touchPoint0.pos(),
													  Qt::LeftButton,
													  Qt::LeftButton,
													  Qt::NoModifier);
			  // left mouse button release
			  mouseReleaseEvent(newEvent);
			  delete newEvent;

			  // clear button id
			  idMouseButton = 0;
			}
		  }
		  // check second point
		  else if (touchPoint1.state() & Qt::TouchPointReleased){
			if (outputLog){
			  qDebug() << "second point release id = " << touchPoint1.id();
			}
			if (touchPoint1.id() == idMouseButton){
			  // release mouse button
			  QMouseEvent *newEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													  touchPoint1.pos(),
													  Qt::LeftButton,
													  Qt::LeftButton,
													  Qt::NoModifier);
			  // left mouse button release
			  mouseReleaseEvent(newEvent);
			  delete newEvent;

			  // clear button id
			  idMouseButton = 0;
			}
		  }
		}
		else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){ // Move
		  if (outputLog){
			qDebug() << "SB: 2 Moved!";
		  }
		  if (pressedMouseLeftButton || pressedMouseRightButton){
			// drag
			const QTouchEvent::TouchPoint &touchPoint = touchPoints.last();
			QPoint currentPos = touchPoint.pos().toPoint();
			QPoint lastPos = touchPoint.lastPos().toPoint();
			QPoint move = currentPos - lastPos;
			if (outputLog){
			  qDebug() << "move = " << move;
			}
			if (move != QPoint(0,0))
			  desktopPanel->mouseMoveRelatively(move);
		  }
		  else {
			// pinch in/out
			const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
			const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
			qreal currentScalingFactor =
			  QLineF(touchPoint0.pos(), touchPoint1.pos()).length() /
			  QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
			if (outputLog){
			  qDebug() << "currentScalingFactor = " << currentScalingFactor;
			}
			qreal scalingFactor = settings->getDesktopScalingFactor();
			if (currentScalingFactor < 1.0){
			  scalingFactor -= 0.02;
			}
			else {
			  scalingFactor += 0.02;
			}
			settings->setDesktopScalingFactor(scalingFactor);
			scalingFactor = settings->getDesktopScalingFactor();
			if (outputLog){
			  qDebug() << "scalingFactor = " << scalingFactor;
			}
			desktopPanel->setScale(scalingFactor);
		  }
		}
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
void SB::mousePressEvent(QMouseEvent *event)
{
  SoftwareButton::mousePressEvent(event);
}

void SB::mouseReleaseEvent(QMouseEvent *event)
{
  SoftwareButton::mouseReleaseEvent(event);
}

void SB::mouseMoveEvent(QMouseEvent *event)
{
  SoftwareButton::mouseMoveEvent(event);
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

// window show event
void SB::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);

  // Sound
  if (settings->getOnSound()){
	SoftwareButton::pressedButton(ID_BUTTON_5);
  }
  else {
	SoftwareButton::pressedButton(ID_BUTTON_4);
  }

  // Sound Cache
  int soundCacheTime = settings->getSoundCacheTime();
  switch(soundCacheTime){
  case 100:
	SoftwareButton::pressedButton(ID_BUTTON_8);
	break;
  case 200:
	SoftwareButton::pressedButton(ID_BUTTON_9);
	break;
  case 300:
	SoftwareButton::pressedButton(ID_BUTTON_10);
	break;
  case 400:
	SoftwareButton::pressedButton(ID_BUTTON_11);
	break;
  case 500:
	SoftwareButton::pressedButton(ID_BUTTON_12);
	break;
  default:
	break;
  }

  // Sound Quality
  SOUND_QUALITY soundQuality = settings->getSoundQuality();
  switch(soundQuality){
  case SOUND_QUALITY_MINIMUM:
	SoftwareButton::pressedButton(ID_BUTTON_15);
	break;
  case SOUND_QUALITY_LOW:
	SoftwareButton::pressedButton(ID_BUTTON_16);
	break;
  case SOUND_QUALITY_STANDARD:
	SoftwareButton::pressedButton(ID_BUTTON_17);
	break;
  case SOUND_QUALITY_HIGH:
	SoftwareButton::pressedButton(ID_BUTTON_18);
	break;
  case SOUND_QUALITY_MAXIMUM:
	SoftwareButton::pressedButton(ID_BUTTON_19);
	break;
  }

  // Video Quality
  VIDEO_QUALITY videoQuality = settings->getVideoQuality();
  switch(videoQuality){
  case VIDEO_QUALITY_MINIMUM:
	SoftwareButton::pressedButton(ID_BUTTON_26);
	break;
  case VIDEO_QUALITY_LOW:
	SoftwareButton::pressedButton(ID_BUTTON_27);
	break;
  case VIDEO_QUALITY_STANDARD:
	SoftwareButton::pressedButton(ID_BUTTON_28);
	break;
  case VIDEO_QUALITY_HIGH:
	SoftwareButton::pressedButton(ID_BUTTON_29);
	break;
  case VIDEO_QUALITY_MAXIMUM:
	SoftwareButton::pressedButton(ID_BUTTON_30);
	break;
  }

  // Video FPS
  int frameRate = settings->getFrameRate();
  switch(frameRate){
  case 5:
	SoftwareButton::pressedButton(ID_BUTTON_34);
	break;
  case 10:
	SoftwareButton::pressedButton(ID_BUTTON_35);
	break;
  case 30:
	SoftwareButton::pressedButton(ID_BUTTON_36);
	break;
  case 60:
	SoftwareButton::pressedButton(ID_BUTTON_37);
	break;
  case 0:
	SoftwareButton::pressedButton(ID_BUTTON_38);
	break;
  }

  // Public Mode
  int publicModeVersion = settings->getPublicModeVersion();
  switch(publicModeVersion){
  case PUBLICMODE_VERSION5:
	SoftwareButton::pressedButton(ID_BUTTON_22);
	break;
  case PUBLICMODE_VERSION6:
	SoftwareButton::pressedButton(ID_BUTTON_22);
	break;
  case PUBLICMODE_VERSION7:
	SoftwareButton::pressedButton(ID_BUTTON_23);
	break;
  }
}

#if defined(QTB_DEV_TOUCHPANEL)
  // hide event
void SB::hideEvent(QHideEvent *event)
{
  Q_UNUSED(event);

  // save settings
  settings->writeSettings();
}
#endif // defined(QTB_DEV_TOUCHPANEL)

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr
