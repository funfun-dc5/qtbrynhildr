// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <iostream> // for TEST

// Qt Header

// Local Header
#include "sb.h"

using namespace std; // for TEST

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SB::SB(Settings *settings, MouseBuffer *mouseBuffer, QWidget *parent)
  :
  SoftwareButton(parent),
  settings(settings),
  mouseBuffer(mouseBuffer),
  previousClick(TYPE_MOUSE_INVALID),
  previousClickTime(QDateTime::currentDateTime()),
  // for DEBUG
  outputLog(false)
{
  buttonTopInfo[ID_BUTTON_12].buttonTop = "Ver. " QTB_VERSION QTB_RCNAME;
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// pressed button
void SB::pressedButton(SoftwareButton::ID_BUTTON id)
{
  SoftwareButton::pressedButton(id);

  if (outputLog){
	cout << "SB: Pressed : BUTTON_ID = " << id << endl << flush;
  }

  bool updated = false;
  MouseInfoValue value;
  value.button = MOUSE_BUTTON_DOWN;

  switch (id){
  case ID_BUTTON_1:
	// Option
	break;
  case ID_BUTTON_2:
	// Monitor
	break;
  case ID_BUTTON_3:
  case ID_BUTTON_4:
  case ID_BUTTON_5:
  case ID_BUTTON_6:
  case ID_BUTTON_7:
  case ID_BUTTON_8:
  case ID_BUTTON_9:
  case ID_BUTTON_10:
  case ID_BUTTON_11:
	{
	  // monitor No.1 - No.9
	  MONITOR_NO monitorNo = id - ID_BUTTON_3 + 1;
	  if (monitorNo <= settings->getMonitorCount()){
		settings->setMonitorNo(monitorNo);
		updated = true;
	  }
	}
	break;
  case ID_BUTTON_12:
	// Info Button
	break;
  case ID_BUTTON_13:
	// Sound Button
	break;
  case ID_BUTTON_14:
	// Sound OFF Button
	settings->setOnSound(false);
	break;
  case ID_BUTTON_15:
	// Sound ON Button
	settings->setOnSound(true);
	break;
  case ID_BUTTON_16:
	// Wheel +
	if (settings->getConnected()){
	  MouseInfoValue value;
	  value.wheel = 15; // + 1 tick
	  mouseBuffer->put(TYPE_MOUSE_WHEEL, value);
	}
	break;
  case ID_BUTTON_17:
	// Sound Quality
	break;
  case ID_BUTTON_18:
	// Sound Quality (Lowest)
	settings->setSoundQuality(SOUND_QUALITY_MINIMUM);
	updated = true;
	break;
  case ID_BUTTON_19:
	// Sound Quality (Low)
	settings->setSoundQuality(SOUND_QUALITY_LOW);
	updated = true;
	break;
  case ID_BUTTON_20:
	// Sound Quality (Standard)
	settings->setSoundQuality(SOUND_QUALITY_STANDARD);
	updated = true;
	break;
  case ID_BUTTON_21:
	// Sound Quality (High)
	settings->setSoundQuality(SOUND_QUALITY_HIGH);
	updated = true;
	break;
  case ID_BUTTON_22:
	// Sound Quality (Highest)
	settings->setSoundQuality(SOUND_QUALITY_MAXIMUM);
	updated = true;
	break;
  case ID_BUTTON_23:
	// Wheel -
	if (settings->getConnected()){
	  MouseInfoValue value;
	  value.wheel = -15; // - 1 tick
	  mouseBuffer->put(TYPE_MOUSE_WHEEL, value);
	}
	break;
  case ID_BUTTON_24:
	// Video Quality
	break;
  case ID_BUTTON_25:
	// Video Quality (Lowest)
	settings->setVideoQuality(VIDEO_QUALITY_MINIMUM);
	updated = true;
	break;
  case ID_BUTTON_26:
	// Video Quality (Low)
	settings->setVideoQuality(VIDEO_QUALITY_LOW);
	updated = true;
	break;
  case ID_BUTTON_27:
	// Video Quality (Standard)
	settings->setVideoQuality(VIDEO_QUALITY_STANDARD);
	updated = true;
	break;
  case ID_BUTTON_28:
	// Video Quality (High)
	settings->setVideoQuality(VIDEO_QUALITY_HIGH);
	updated = true;
	break;
  case ID_BUTTON_29:
	// Video Quality (Highest)
	settings->setVideoQuality(VIDEO_QUALITY_MAXIMUM);
	updated = true;
	break;
  case ID_BUTTON_30:
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
  default:
	// error
	break;
  }
  if (updated){
#if USE_KERO_KEYBOARD
	emit refreshMenu();
#endif // USE_KERO_KEYBOARD
  }
}

// released button
void SB::releasedButton(SoftwareButton::ID_BUTTON id)
{
  SoftwareButton::releasedButton(id);

  if (outputLog){
	cout << "SB: Released: BUTTON_ID = " << id << endl << flush;
  }

  MouseInfoValue value;
  value.button = MOUSE_BUTTON_UP;

  switch (id){
  case ID_BUTTON_0:
  case ID_BUTTON_1:
  case ID_BUTTON_2:
  case ID_BUTTON_3:
  case ID_BUTTON_4:
  case ID_BUTTON_5:
  case ID_BUTTON_6:
  case ID_BUTTON_7:
  case ID_BUTTON_8:
  case ID_BUTTON_9:
  case ID_BUTTON_10:
  case ID_BUTTON_11:
  case ID_BUTTON_12:
  case ID_BUTTON_13:
  case ID_BUTTON_14:
  case ID_BUTTON_15:
  case ID_BUTTON_16:
  case ID_BUTTON_17:
  case ID_BUTTON_18:
  case ID_BUTTON_19:
  case ID_BUTTON_20:
  case ID_BUTTON_21:
  case ID_BUTTON_22:
  case ID_BUTTON_23:
  case ID_BUTTON_24:
  case ID_BUTTON_25:
  case ID_BUTTON_26:
  case ID_BUTTON_27:
  case ID_BUTTON_28:
  case ID_BUTTON_29:
	break;
  case ID_BUTTON_30:
	// Mouse Right Button
	if (settings->getConnected()){
	  mouseBuffer->put(TYPE_MOUSE_RIGHT_BUTTON, value);
	}
	break;
  case ID_BUTTON_31:
	// Mouse Left Button
	if (settings->getConnected()){
	  mouseBuffer->put(TYPE_MOUSE_LEFT_BUTTON, value);
	}
	break;
  default:
	// error
	break;
  }
}

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr