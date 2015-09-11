// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_button.cpp

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt header

// Local Header
#include "software_button.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareButton::SoftwareButton(Settings *settings, MouseBuffer *mouseBuffer, QWidget *parent)
  :
  QWidget(parent),
  settings(settings),
  mouseBuffer(mouseBuffer),
  onOptionButton(true),
  onShowMonitorButton(true),
  onShowSoundQualityButton(true),
  onShowVideoQualityButton(true),
  onShowSoundButton(true),
  previousClick(TYPE_MOUSE_INVALID),
  previousClickTime(QDateTime::currentDateTime()),
  // for DEBUG
  outputLog(true)
{
  setupUi(this);

  // connect signals
  // pressed signals
  connect(pushButton_1, SIGNAL(pressed()), this, SLOT(pressedButton_1()));
  connect(pushButton_2, SIGNAL(pressed()), this, SLOT(pressedButton_2()));
  connect(pushButton_3, SIGNAL(pressed()), this, SLOT(pressedButton_3()));
  connect(pushButton_4, SIGNAL(pressed()), this, SLOT(pressedButton_4()));
  connect(pushButton_5, SIGNAL(pressed()), this, SLOT(pressedButton_5()));
  connect(pushButton_6, SIGNAL(pressed()), this, SLOT(pressedButton_6()));
  connect(pushButton_7, SIGNAL(pressed()), this, SLOT(pressedButton_7()));
  connect(pushButton_8, SIGNAL(pressed()), this, SLOT(pressedButton_8()));
  connect(pushButton_9, SIGNAL(pressed()), this, SLOT(pressedButton_9()));
  connect(pushButton_10, SIGNAL(pressed()), this, SLOT(pressedButton_10()));
  connect(pushButton_11, SIGNAL(pressed()), this, SLOT(pressedButton_11()));
  connect(pushButton_12, SIGNAL(pressed()), this, SLOT(pressedButton_12()));
  connect(pushButton_13, SIGNAL(pressed()), this, SLOT(pressedButton_13()));
  connect(pushButton_14, SIGNAL(pressed()), this, SLOT(pressedButton_14()));
  connect(pushButton_15, SIGNAL(pressed()), this, SLOT(pressedButton_15()));
  connect(pushButton_16, SIGNAL(pressed()), this, SLOT(pressedButton_16()));
  connect(pushButton_17, SIGNAL(pressed()), this, SLOT(pressedButton_17()));
  connect(pushButton_18, SIGNAL(pressed()), this, SLOT(pressedButton_18()));
  connect(pushButton_19, SIGNAL(pressed()), this, SLOT(pressedButton_19()));
  connect(pushButton_20, SIGNAL(pressed()), this, SLOT(pressedButton_20()));
  connect(pushButton_21, SIGNAL(pressed()), this, SLOT(pressedButton_21()));
  connect(pushButton_22, SIGNAL(pressed()), this, SLOT(pressedButton_22()));
  connect(pushButton_23, SIGNAL(pressed()), this, SLOT(pressedButton_23()));
  connect(pushButton_24, SIGNAL(pressed()), this, SLOT(pressedButton_24()));
  connect(pushButton_25, SIGNAL(pressed()), this, SLOT(pressedButton_25()));
  connect(pushButton_26, SIGNAL(pressed()), this, SLOT(pressedButton_26()));
  connect(pushButton_27, SIGNAL(pressed()), this, SLOT(pressedButton_27()));
  connect(pushButton_28, SIGNAL(pressed()), this, SLOT(pressedButton_28()));
  connect(pushButton_29, SIGNAL(pressed()), this, SLOT(pressedButton_29()));
  connect(pushButton_30, SIGNAL(pressed()), this, SLOT(pressedButton_30()));
  connect(pushButton_31, SIGNAL(pressed()), this, SLOT(pressedButton_31()));

  // released signals
  connect(pushButton_1, SIGNAL(released()), this, SLOT(releasedButton_1()));
  connect(pushButton_2, SIGNAL(released()), this, SLOT(releasedButton_2()));
  connect(pushButton_3, SIGNAL(released()), this, SLOT(releasedButton_3()));
  connect(pushButton_4, SIGNAL(released()), this, SLOT(releasedButton_4()));
  connect(pushButton_5, SIGNAL(released()), this, SLOT(releasedButton_5()));
  connect(pushButton_6, SIGNAL(released()), this, SLOT(releasedButton_6()));
  connect(pushButton_7, SIGNAL(released()), this, SLOT(releasedButton_7()));
  connect(pushButton_8, SIGNAL(released()), this, SLOT(releasedButton_8()));
  connect(pushButton_9, SIGNAL(released()), this, SLOT(releasedButton_9()));
  connect(pushButton_10, SIGNAL(released()), this, SLOT(releasedButton_10()));
  connect(pushButton_11, SIGNAL(released()), this, SLOT(releasedButton_11()));
  connect(pushButton_12, SIGNAL(released()), this, SLOT(releasedButton_12()));
  connect(pushButton_13, SIGNAL(released()), this, SLOT(releasedButton_13()));
  connect(pushButton_14, SIGNAL(released()), this, SLOT(releasedButton_14()));
  connect(pushButton_15, SIGNAL(released()), this, SLOT(releasedButton_15()));
  connect(pushButton_16, SIGNAL(released()), this, SLOT(releasedButton_16()));
  connect(pushButton_17, SIGNAL(released()), this, SLOT(releasedButton_17()));
  connect(pushButton_18, SIGNAL(released()), this, SLOT(releasedButton_18()));
  connect(pushButton_19, SIGNAL(released()), this, SLOT(releasedButton_19()));
  connect(pushButton_20, SIGNAL(released()), this, SLOT(releasedButton_20()));
  connect(pushButton_21, SIGNAL(released()), this, SLOT(releasedButton_21()));
  connect(pushButton_22, SIGNAL(released()), this, SLOT(releasedButton_22()));
  connect(pushButton_23, SIGNAL(released()), this, SLOT(releasedButton_23()));
  connect(pushButton_24, SIGNAL(released()), this, SLOT(releasedButton_24()));
  connect(pushButton_25, SIGNAL(released()), this, SLOT(releasedButton_25()));
  connect(pushButton_26, SIGNAL(released()), this, SLOT(releasedButton_26()));
  connect(pushButton_27, SIGNAL(released()), this, SLOT(releasedButton_27()));
  connect(pushButton_28, SIGNAL(released()), this, SLOT(releasedButton_28()));
  connect(pushButton_29, SIGNAL(released()), this, SLOT(releasedButton_29()));
  connect(pushButton_30, SIGNAL(released()), this, SLOT(releasedButton_30()));
  connect(pushButton_31, SIGNAL(released()), this, SLOT(releasedButton_31()));

  // initialization
  toggleOptionButton();

  // update size
  updateSize();
}

// size hint
QSize SoftwareButton::sizeHint() const
{
  return size();
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
// toggle option button
void SoftwareButton::toggleOptionButton()
{
  onOptionButton = !onOptionButton;

  // monitor option button
  pushButton_2->setVisible(onOptionButton);
  // sound quality option button
  pushButton_13->setVisible(onOptionButton);
  // sound video option button
  pushButton_20->setVisible(onOptionButton);
  // sound option button
  pushButton_29->setVisible(onOptionButton);

  if(!onOptionButton){
	// clear buttons
	if (onShowMonitorButton)
	  toggleShowMonitorButton();
	if (onShowSoundQualityButton)
	  toggleShowSoundQualityButton();
	if (onShowVideoQualityButton)
	  toggleShowVideoQualityButton();
	if (onShowSoundButton)
	  toggleShowSoundButton();
  }
}

// toggle monitor button
void SoftwareButton::toggleShowMonitorButton()
{
  onShowMonitorButton = !onShowMonitorButton;
  pushButton_3->setVisible(onShowMonitorButton);
  pushButton_4->setVisible(onShowMonitorButton);
  pushButton_5->setVisible(onShowMonitorButton);
  pushButton_6->setVisible(onShowMonitorButton);
  pushButton_7->setVisible(onShowMonitorButton);
  pushButton_8->setVisible(onShowMonitorButton);
  pushButton_9->setVisible(onShowMonitorButton);
  pushButton_10->setVisible(onShowMonitorButton);
  pushButton_11->setVisible(onShowMonitorButton);
}

// toggle sound quality button
void SoftwareButton::toggleShowSoundQualityButton()
{
  onShowSoundQualityButton = !onShowSoundQualityButton;
  pushButton_14->setVisible(onShowSoundQualityButton);
  pushButton_15->setVisible(onShowSoundQualityButton);
  pushButton_16->setVisible(onShowSoundQualityButton);
  pushButton_17->setVisible(onShowSoundQualityButton);
  pushButton_18->setVisible(onShowSoundQualityButton);
}

// toggle video quality button
void SoftwareButton::toggleShowVideoQualityButton()
{
  onShowVideoQualityButton = !onShowVideoQualityButton;
  pushButton_21->setVisible(onShowVideoQualityButton);
  pushButton_22->setVisible(onShowVideoQualityButton);
  pushButton_23->setVisible(onShowVideoQualityButton);
  pushButton_24->setVisible(onShowVideoQualityButton);
  pushButton_25->setVisible(onShowVideoQualityButton);
}

// toggle sound button
void SoftwareButton::toggleShowSoundButton()
{
  onShowSoundButton = !onShowSoundButton;
  pushButton_30->setVisible(onShowSoundButton);
  pushButton_31->setVisible(onShowSoundButton);
}

// update size
void SoftwareButton::updateSize()
{
}

// pressed button
void SoftwareButton::pressedButton(ID_BUTTON id)
{
  if (outputLog){
	cout << "Pressed  Button! id = " << id << endl << flush;
  }

  // check id
  if (id <= 0 || id > ID_BUTTON_NUM){
	// error
	return;
  }

  bool updated = false;
  MouseInfoValue value;
  value.button = MOUSE_BUTTON_DOWN;

  switch(id){
  case ID_BUTTON_1:
	// Option
	toggleOptionButton();
	break;
  case ID_BUTTON_2:
	// Monitor
	toggleShowMonitorButton();
	break;
  case ID_BUTTON_3: // monitor No.1
  case ID_BUTTON_4: // monitor No.2
  case ID_BUTTON_5: // monitor No.3
  case ID_BUTTON_6: // monitor No.4
  case ID_BUTTON_7: // monitor No.5
  case ID_BUTTON_8: // monitor No.6
  case ID_BUTTON_9: // monitor No.7
  case ID_BUTTON_10: // monitor No.8
  case ID_BUTTON_11: // monitor No.9
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
	// Wheel +
	if (settings->getConnected()){
	  MouseInfoValue value;
	  value.wheel = 15; // + 1 tick
	  mouseBuffer->put(TYPE_MOUSE_WHEEL, value);
	}
	break;
  case ID_BUTTON_13:
	// Sound Quality
	toggleShowSoundQualityButton();
	break;
  case ID_BUTTON_14:
	// Sound Quality (Lowest)
	settings->setSoundQuality(SOUND_QUALITY_MINIMUM);
	updated = true;
	break;
  case ID_BUTTON_15:
	// Sound Quality (Low)
	settings->setSoundQuality(SOUND_QUALITY_LOW);
	updated = true;
	break;
  case ID_BUTTON_16:
	// Sound Quality (Standard)
	settings->setSoundQuality(SOUND_QUALITY_STANDARD);
	updated = true;
	break;
  case ID_BUTTON_17:
	// Sound Quality (High)
	settings->setSoundQuality(SOUND_QUALITY_HIGH);
	updated = true;
	break;
  case ID_BUTTON_18:
	// Sound Quality (Highest)
	settings->setSoundQuality(SOUND_QUALITY_MAXIMUM);
	updated = true;
	break;
  case ID_BUTTON_19:
	// Wheel -
	if (settings->getConnected()){
	  MouseInfoValue value;
	  value.wheel = -15; // - 1 tick
	  mouseBuffer->put(TYPE_MOUSE_WHEEL, value);
	}
	break;
  case ID_BUTTON_20:
	// Video Quality
	toggleShowVideoQualityButton();
	break;
  case ID_BUTTON_21:
	// Video Quality (Lowest)
	settings->setVideoQuality(VIDEO_QUALITY_MINIMUM);
	updated = true;
	break;
  case ID_BUTTON_22:
	// Video Quality (Low)
	settings->setVideoQuality(VIDEO_QUALITY_LOW);
	updated = true;
	break;
  case ID_BUTTON_23:
	// Video Quality (Standard)
	settings->setVideoQuality(VIDEO_QUALITY_STANDARD);
	updated = true;
	break;
  case ID_BUTTON_24:
	// Video Quality (High)
	settings->setVideoQuality(VIDEO_QUALITY_HIGH);
	updated = true;
	break;
  case ID_BUTTON_25:
	// Video Quality (Highest)
	settings->setVideoQuality(VIDEO_QUALITY_MAXIMUM);
	updated = true;
	break;
  case ID_BUTTON_26:
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
  case ID_BUTTON_27:
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
  case ID_BUTTON_28:
	// Info Button
	break;
  case ID_BUTTON_29:
	// Sound Button
	toggleShowSoundButton();
	break;
  case ID_BUTTON_30:
	// Sound OFF Button
	settings->setOnSound(false);
	break;
  case ID_BUTTON_31:
	// Sound ON Button
	settings->setOnSound(true);
	break;
  default:
	// error
	ABORT();
	break;
  }
  if (updated){
	emit refreshMenu();
  }
}

// released button
void SoftwareButton::releasedButton(ID_BUTTON id)
{
  if (outputLog){
	cout << "Released Button! id = " << id << endl << flush;
  }

  // check id
  if (id <= 0 || id > ID_BUTTON_NUM){
	// error
	return;
  }

  MouseInfoValue value;
  value.button = MOUSE_BUTTON_UP;

  switch(id){
  case ID_BUTTON_1:
	// Option
	break;
  case ID_BUTTON_2:
	// Monitor
	break;
  case ID_BUTTON_3: // monitor No.1
  case ID_BUTTON_4: // monitor No.2
  case ID_BUTTON_5: // monitor No.3
  case ID_BUTTON_6: // monitor No.4
  case ID_BUTTON_7: // monitor No.5
  case ID_BUTTON_8: // monitor No.6
  case ID_BUTTON_9: // monitor No.7
  case ID_BUTTON_10: // monitor No.8
  case ID_BUTTON_11: // monitor No.9
	// monitor No.1 - No.9
	break;
  case ID_BUTTON_12:
	// Wheel +
	break;
  case ID_BUTTON_13:
	// Sound Quality
	break;
  case ID_BUTTON_14:
	// Sound Quality (Lowest)
	break;
  case ID_BUTTON_15:
	// Sound Quality (Low)
	break;
  case ID_BUTTON_16:
	// Sound Quality (Standard)
	break;
  case ID_BUTTON_17:
	// Sound Quality (High)
	break;
  case ID_BUTTON_18:
	// Sound Quality (Highest)
	break;
  case ID_BUTTON_19:
	// Wheel -
	break;
  case ID_BUTTON_20:
	// Video Quality
	break;
  case ID_BUTTON_21:
	// Video Quality (Lowest)
	break;
  case ID_BUTTON_22:
	// Video Quality (Low)
	break;
  case ID_BUTTON_23:
	// Video Quality (Standard)
	break;
  case ID_BUTTON_24:
	// Video Quality (High)
	break;
  case ID_BUTTON_25:
	// Video Quality (Highest)
	break;
  case ID_BUTTON_26:
	// Mouse Right Button
	if (settings->getConnected()){
	  mouseBuffer->put(TYPE_MOUSE_RIGHT_BUTTON, value);
	}
	break;
  case ID_BUTTON_27:
	// Mouse Left Button
	if (settings->getConnected()){
	  mouseBuffer->put(TYPE_MOUSE_LEFT_BUTTON, value);
	}
	break;
  case ID_BUTTON_28:
	// Info Button
	break;
  case ID_BUTTON_29:
	// Sound Button
	break;
  case ID_BUTTON_30:
	// Sound OFF Button
	break;
  case ID_BUTTON_31:
	// Sound ON Button
	break;
  default:
	// error
	ABORT();
	break;
  }
}

// pressed slots
void SoftwareButton::pressedButton_1()
{
  pressedButton(ID_BUTTON_1);
}

void SoftwareButton::pressedButton_2()
{
  pressedButton(ID_BUTTON_2);
}

void SoftwareButton::pressedButton_3()
{
  pressedButton(ID_BUTTON_3);
}

void SoftwareButton::pressedButton_4()
{
  pressedButton(ID_BUTTON_4);
}

void SoftwareButton::pressedButton_5()
{
  pressedButton(ID_BUTTON_5);
}

void SoftwareButton::pressedButton_6()
{
  pressedButton(ID_BUTTON_6);
}

void SoftwareButton::pressedButton_7()
{
  pressedButton(ID_BUTTON_7);
}

void SoftwareButton::pressedButton_8()
{
  pressedButton(ID_BUTTON_8);
}

void SoftwareButton::pressedButton_9()
{
  pressedButton(ID_BUTTON_9);
}

void SoftwareButton::pressedButton_10()
{
  pressedButton(ID_BUTTON_10);
}

void SoftwareButton::pressedButton_11()
{
  pressedButton(ID_BUTTON_11);
}

void SoftwareButton::pressedButton_12()
{
  pressedButton(ID_BUTTON_12);
}

void SoftwareButton::pressedButton_13()
{
  pressedButton(ID_BUTTON_13);
}

void SoftwareButton::pressedButton_14()
{
  pressedButton(ID_BUTTON_14);
}

void SoftwareButton::pressedButton_15()
{
  pressedButton(ID_BUTTON_15);
}

void SoftwareButton::pressedButton_16()
{
  pressedButton(ID_BUTTON_16);
}

void SoftwareButton::pressedButton_17()
{
  pressedButton(ID_BUTTON_17);
}

void SoftwareButton::pressedButton_18()
{
  pressedButton(ID_BUTTON_18);
}

void SoftwareButton::pressedButton_19()
{
  pressedButton(ID_BUTTON_19);
}

void SoftwareButton::pressedButton_20()
{
  pressedButton(ID_BUTTON_20);
}

void SoftwareButton::pressedButton_21()
{
  pressedButton(ID_BUTTON_21);
}

void SoftwareButton::pressedButton_22()
{
  pressedButton(ID_BUTTON_22);
}

void SoftwareButton::pressedButton_23()
{
  pressedButton(ID_BUTTON_23);
}

void SoftwareButton::pressedButton_24()
{
  pressedButton(ID_BUTTON_24);
}

void SoftwareButton::pressedButton_25()
{
  pressedButton(ID_BUTTON_25);
}

void SoftwareButton::pressedButton_26()
{
  pressedButton(ID_BUTTON_26);
}

void SoftwareButton::pressedButton_27()
{
  pressedButton(ID_BUTTON_27);
}

void SoftwareButton::pressedButton_28()
{
  pressedButton(ID_BUTTON_28);
}

void SoftwareButton::pressedButton_29()
{
  pressedButton(ID_BUTTON_29);
}

void SoftwareButton::pressedButton_30()
{
  pressedButton(ID_BUTTON_30);
}

void SoftwareButton::pressedButton_31()
{
  pressedButton(ID_BUTTON_31);
}

// released slots
void SoftwareButton::releasedButton_1()
{
  releasedButton(ID_BUTTON_1);
}

void SoftwareButton::releasedButton_2()
{
  releasedButton(ID_BUTTON_2);
}

void SoftwareButton::releasedButton_3()
{
  releasedButton(ID_BUTTON_3);
}

void SoftwareButton::releasedButton_4()
{
  releasedButton(ID_BUTTON_4);
}

void SoftwareButton::releasedButton_5()
{
  releasedButton(ID_BUTTON_5);
}

void SoftwareButton::releasedButton_6()
{
  releasedButton(ID_BUTTON_6);
}

void SoftwareButton::releasedButton_7()
{
  releasedButton(ID_BUTTON_7);
}

void SoftwareButton::releasedButton_8()
{
  releasedButton(ID_BUTTON_8);
}

void SoftwareButton::releasedButton_9()
{
  releasedButton(ID_BUTTON_9);
}

void SoftwareButton::releasedButton_10()
{
  releasedButton(ID_BUTTON_10);
}

void SoftwareButton::releasedButton_11()
{
  releasedButton(ID_BUTTON_11);
}

void SoftwareButton::releasedButton_12()
{
  releasedButton(ID_BUTTON_12);
}

void SoftwareButton::releasedButton_13()
{
  releasedButton(ID_BUTTON_13);
}

void SoftwareButton::releasedButton_14()
{
  releasedButton(ID_BUTTON_14);
}

void SoftwareButton::releasedButton_15()
{
  releasedButton(ID_BUTTON_15);
}

void SoftwareButton::releasedButton_16()
{
  releasedButton(ID_BUTTON_16);
}

void SoftwareButton::releasedButton_17()
{
  releasedButton(ID_BUTTON_17);
}

void SoftwareButton::releasedButton_18()
{
  releasedButton(ID_BUTTON_18);
}

void SoftwareButton::releasedButton_19()
{
  releasedButton(ID_BUTTON_19);
}

void SoftwareButton::releasedButton_20()
{
  releasedButton(ID_BUTTON_20);
}

void SoftwareButton::releasedButton_21()
{
  releasedButton(ID_BUTTON_21);
}

void SoftwareButton::releasedButton_22()
{
  releasedButton(ID_BUTTON_22);
}

void SoftwareButton::releasedButton_23()
{
  releasedButton(ID_BUTTON_23);
}

void SoftwareButton::releasedButton_24()
{
  releasedButton(ID_BUTTON_24);
}

void SoftwareButton::releasedButton_25()
{
  releasedButton(ID_BUTTON_25);
}

void SoftwareButton::releasedButton_26()
{
  releasedButton(ID_BUTTON_26);
}

void SoftwareButton::releasedButton_27()
{
  releasedButton(ID_BUTTON_27);
}

void SoftwareButton::releasedButton_28()
{
  releasedButton(ID_BUTTON_28);
}

void SoftwareButton::releasedButton_29()
{
  releasedButton(ID_BUTTON_29);
}

void SoftwareButton::releasedButton_30()
{
  releasedButton(ID_BUTTON_30);
}

void SoftwareButton::releasedButton_31()
{
  releasedButton(ID_BUTTON_31);
}

} // end of namespace qtbrynhildr
