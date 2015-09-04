// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// desktop_scaling_dialog.cpp

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt header
#include <QDialog>

// Local Header
#include "desktop_scaling_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
DesktopScalingDialog::DesktopScalingDialog(Settings *settings,
										   QWidget *parent)
  :
  //  QDialog(parent),
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  settings(settings),
  // for DEBUG
  outputLog(false)
{
  setupUi(this);

  // setting for slider
  horizontalSlider->setMinimum(SLIDER_MINIMUM);
  horizontalSlider->setMaximum(SLIDER_MAXIMUM);
  horizontalSlider->setSingleStep(1);
  horizontalSlider->setPageStep(1);
  horizontalSlider->setTickInterval(1);
  horizontalSlider->setTickPosition(QSlider::TicksAbove);
}

// set scale slider position from stting
void DesktopScalingDialog::setSliderPositionFromSetting()
{
  horizontalSlider->setSliderPosition((int)(settings->getDesktopScalingFactor()*SLIDER_FACTOR));
  if (outputLog)
	cout << "slider position: " << settings->getDesktopScalingFactor()*SLIDER_FACTOR << endl << flush; // for DEBUG
}

// set setting from scale slider position
void DesktopScalingDialog::setSettingFromSliderPosition()
{
  settings->setDesktopScalingFactor((qreal)horizontalSlider->value()/SLIDER_FACTOR);
  if (outputLog){
	cout << "scaling value: " << horizontalSlider->value() << endl << flush; // for DEBUG
	cout << "scaling factor: " << settings->getDesktopScalingFactor() << endl << flush; // for DEBUG
  }
}

// show Event
void DesktopScalingDialog::showEvent(QShowEvent *event)
{
  // for cancel
  originalFactor = settings->getDesktopScalingFactor();
}

//---------------------------------------------------------------------------
// private slot
//---------------------------------------------------------------------------
// scale slider
void DesktopScalingDialog::on_horizontalSlider_valueChanged()
{
  if (isActiveWindow()){
	if (outputLog)
	  cout << "slider: value Changed" << endl << flush; // for DEBUG
 	setSettingFromSliderPosition();
  }
}

// reset scale
void DesktopScalingDialog::on_resetButton_clicked()
{
  if (outputLog)
	cout << "reset button: click" << endl << flush; // for DEBUG

  // reset desktop scaling factor
  settings->setDesktopScalingFactor(1.0);
  setSliderPositionFromSetting();
}

// accept button
void DesktopScalingDialog::accept()
{
  if (outputLog)
	cout << "accept()."; // for DEBUG
  hide();
}

// reject button
void DesktopScalingDialog::reject()
{
  if (outputLog)
	cout << "reject()."; // for DEBUG
  settings->setDesktopScalingFactor(originalFactor);
  hide();
}

} // end of namespace qtbrynhildr
