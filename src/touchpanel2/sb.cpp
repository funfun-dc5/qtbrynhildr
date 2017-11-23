// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <iostream> // for TEST

// Qt Header

// Local Header
#include "qtbrynhildr.h"
#include "sb.h"

using namespace std; // for TEST

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SB::SB(MouseBuffer *mouseBuffer, QtBrynhildr *qtbrynhildr, QWidget *parent)
  :
  SoftwareButton(parent),
  mouseBuffer(mouseBuffer),
  qtbrynhildr(qtbrynhildr),
  settings(qtbrynhildr->getSettings()),
  previousClick(TYPE_MOUSE_INVALID),
  previousClickTime(QDateTime::currentDateTime()),
  // for DEBUG
  outputLog(false)
{
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
	break;
  case PUBLICMODE_VERSION7:
	SoftwareButton::pressedButton(ID_BUTTON_23);
	break;
  }
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// pressed button
void SB::pressedButton(SoftwareButton::ID_BUTTON id)
{
  if (outputLog){
	cout << "SB: Pressed : ID_BUTTON = " << id << endl << flush;
  }

  bool updated = false;
  MouseInfoValue value;
  value.button = MOUSE_BUTTON_DOWN;

  switch (id){
  case ID_BUTTON_1:
	// Fn
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
	  MouseInfoValue value;
	  value.wheel = 15; // + 1 tick
	  mouseBuffer->put(TYPE_MOUSE_WHEEL, value);
	}
	break;
  case ID_BUTTON_21:
	// Public Mode
	break;
  case ID_BUTTON_22:
	// Public Mode 5
	settings->setPublicModeVersion(5);
	break;
  case ID_BUTTON_23:
	// Public Mode 7
	settings->setPublicModeVersion(7);
	break;
  case ID_BUTTON_24:
	// Wheel -
	if (settings->getConnected()){
	  MouseInfoValue value;
	  value.wheel = -15; // - 1 tick
	  mouseBuffer->put(TYPE_MOUSE_WHEEL, value);
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
	  mouseBuffer->put(TYPE_MOUSE_LEFT_BUTTON, value);
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 diffMSeconds = currentTime.toMSecsSinceEpoch() - previousClickTime.toMSecsSinceEpoch();
	  if (previousClick == TYPE_MOUSE_LEFT_BUTTON && diffMSeconds < settings->getDoubleClickThreshold()){
		value.button = MOUSE_BUTTON_DBLCLK;
		mouseBuffer->put(TYPE_MOUSE_LEFT_BUTTON, value);
		if (outputLog){
		  cout << "Double Click!!" << endl << flush;
		}
	  }
	  previousClick = TYPE_MOUSE_LEFT_BUTTON;
	  previousClickTime = currentTime;
	}
	break;
  case ID_BUTTON_32:
	// Mouse Right Button
	if (settings->getConnected()){
	  mouseBuffer->put(TYPE_MOUSE_RIGHT_BUTTON, value);
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 diffMSeconds = currentTime.toMSecsSinceEpoch() - previousClickTime.toMSecsSinceEpoch();
	  if (previousClick == TYPE_MOUSE_RIGHT_BUTTON && diffMSeconds < settings->getDoubleClickThreshold()){
		value.button = MOUSE_BUTTON_DBLCLK;
		mouseBuffer->put(TYPE_MOUSE_RIGHT_BUTTON, value);
		if (outputLog){
		  cout << "Double Click!!" << endl << flush;
		}
	  }
	  previousClick = TYPE_MOUSE_RIGHT_BUTTON;
	  previousClickTime = currentTime;
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
	settings->setFrameRate(0);
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
	cout << "SB: Released: ID_BUTTON = " << id << endl << flush;
  }

  MouseInfoValue value;
  value.button = MOUSE_BUTTON_UP;

  switch (id){
  case ID_BUTTON_1:
	// Fn
	qtbrynhildr->toggleSoftwareButton();
	break;
  case ID_BUTTON_31:
	// Mouse Left Button
	if (settings->getConnected()){
	  mouseBuffer->put(TYPE_MOUSE_LEFT_BUTTON, value);
	}
	break;
  case ID_BUTTON_32:
	// Mouse Right Button
	if (settings->getConnected()){
	  mouseBuffer->put(TYPE_MOUSE_RIGHT_BUTTON, value);
	}
	break;
  default:
	// Nothing to do
	break;
  }

  SoftwareButton::releasedButton(id);
}

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr
