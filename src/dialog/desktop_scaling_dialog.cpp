// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QDialog>
#include <QRect>

// Local Header
#include "desktop_scaling_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
DesktopScalingDialog::DesktopScalingDialog(Settings *settings,
										   QWidget *parent)
  :QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
  ,settings(settings)
  // for DEBUG
  ,outputLog(false)
{
  setupUi(this);

  // setting for slider
  horizontalSlider->setMinimum(SLIDER_MINIMUM);
  horizontalSlider->setMaximum(SLIDER_MAXIMUM);
  horizontalSlider->setSingleStep(1);
  horizontalSlider->setPageStep(1);
  horizontalSlider->setTickInterval(1);
  horizontalSlider->setTickPosition(QSlider::TicksAbove);

  // initialize original scaling factor
  originalFactor = 1.0;

  // resetting
  resetting();

  // fix size
  setFixedSize(size());
}

// resize event
void DesktopScalingDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
}

// set scale slider position from stting
void DesktopScalingDialog::setSliderPositionFromSetting()
{
  horizontalSlider->setSliderPosition((int)(settings->getDesktopScalingFactor()*SLIDER_FACTOR));
  if (outputLog)
	std::cout << "slider position: " << settings->getDesktopScalingFactor()*SLIDER_FACTOR << std::endl << std::flush; // for DEBUG
}

// set setting from scale slider position
void DesktopScalingDialog::setSettingFromSliderPosition()
{
  if (settings->getOnDesktopScaleFixed()){
	// NOT change scaling
	return;
  }
  settings->setDesktopScalingFactor((qreal)horizontalSlider->value()/SLIDER_FACTOR);
  if (outputLog){
	std::cout << "scaling value: " << horizontalSlider->value() << std::endl << std::flush; // for DEBUG
	std::cout << "scaling factor: " << settings->getDesktopScalingFactor() << std::endl << std::flush; // for DEBUG
  }
}

// settings for Tablet
void DesktopScalingDialog::resetting()
{
#if defined(QTB_DEV_TOUCHPANEL)
  int screenWidth = settings->getCurrentScreenWidth();
  int screenHeight = settings->getCurrentScreenHeight();
  int dialogWidth = screenWidth * 0.5;
  int dialogHeight = screenHeight * 0.8;
  int fontPointSize = 14;

  // resetting dialog window size and font size
  resize(dialogWidth, dialogHeight);
  layoutWidget->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
  QFont currentFont = font();
  currentFont.setPointSize(fontPointSize);
  setFont(currentFont);
#endif // defined(QTB_DEV_TOUCHPANEL)
}

// show Event
void DesktopScalingDialog::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);

  // set enable/disable
  horizontalSlider->setEnabled(!settings->getOnDesktopScaleFixed());

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
	  std::cout << "slider: value Changed" << std::endl << std::flush; // for DEBUG
 	setSettingFromSliderPosition();
  }
}

// reset scale
void DesktopScalingDialog::on_resetButton_clicked()
{
  if (outputLog)
	std::cout << "reset button: click" << std::endl << std::flush; // for DEBUG
  if (settings->getOnDesktopScaleFixed()){
	// NOT change scaling
	return;
  }

  // reset desktop scaling factor
  settings->setDesktopScalingFactor(1.0);
  setSliderPositionFromSetting();
}

// accept button
void DesktopScalingDialog::accept()
{
  if (outputLog)
	std::cout << "accept()."; // for DEBUG
  hide();
}

// reject button
void DesktopScalingDialog::reject()
{
  if (outputLog)
	std::cout << "reject()."; // for DEBUG
  settings->setDesktopScalingFactor(originalFactor);
  hide();
}

} // end of namespace qtbrynhildr
