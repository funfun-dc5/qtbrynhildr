// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <iostream> // for TEST

// Qt Header
#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPen>

// Local Header
#include "software_button.h"

using namespace std; // for TEST

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareButton::SoftwareButton(QWidget *parent)
  :
  QWidget(parent),
  onOptionButton(false),
  onShowMonitorButton(false),
  onShowSoundQualityButton(false),
  onShowVideoQualityButton(false),
  onShowSoundButton(false),
  // for DEBUG
  outputLog(false)
{
  // reset flag
  for(int i = ID_BUTTON_0; i < ID_BUTTON_NUM; i++){
	layout[i].pushed = false;
  }

  // initialize layout
  calculateLayout(5, 5);
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------

// paint event
void SoftwareButton::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event)

  QPainter painter(this);

  QColor panelColor = QColor::fromRgb(15, 31, 64);
  // change color for Pen
  QColor penColor = QColor::fromRgb(61, 124, 250);
  painter.setPen(penColor);
  // change font size
  QFont font = painter.font();
  font.setPixelSize(12);
  painter.setFont(font);

  // draw all buttons
  for(int i = ID_BUTTON_1; i < ID_BUTTON_NUM; i++){
	if (buttonTopInfo[i].visible){
	  QRect rect = layout[i].rect;
	  if (layout[i].pushed)
		painter.fillRect(rect, Qt::black);
	  else
		painter.fillRect(rect, panelColor);
	  painter.drawRect(rect);
	  painter.drawText(rect,
					   Qt::AlignCenter,
					   buttonTopInfo[i].buttonTop
					   );
	}
  }

  // restore color for Pen
  //  painter.setPen(Qt::black);
}

// resize event
void SoftwareButton::resizeEvent(QResizeEvent *event)
{
  QSize size = event->size() - QSize(1, 1);

  if (size != buttonSize){
	//cout << "resize! : (W, H) = (" << size.width() << "," << size.height() << ")" << endl << flush;
	qreal xFactor = (qreal)size.width()/WIDTH;
	qreal yFactor = (qreal)size.height()/HEIGHT;
	// recalculate layout
	calculateLayout(xFactor, yFactor);
	update();
  }
  else {
	//cout << "resize! : same size" << endl << flush;
  }
}

#if 0
// minimum size hint
QSize SoftwareButton::minimumSizeHint() const
{
  QSize size = buttonSize + QSize(1, 1);
  return size;
}
#endif

// size hint
QSize SoftwareButton::sizeHint() const
{
  QSize size = buttonSize + QSize(1, 1);
  return size;
}

// mouse event
void SoftwareButton::mousePressEvent(QMouseEvent *event)
{
  if (outputLog)
	cout << "Press  : (" << event->pos().x() << "," << event->pos().y() << ")" << endl << flush;
  ID_BUTTON id = getID(event->pos());
  pressedButton(id);
}

void SoftwareButton::mouseReleaseEvent(QMouseEvent *event)
{
  if (outputLog)
	cout << "Release: (" << event->pos().x() << "," << event->pos().y() << ")" << endl << flush;
  ID_BUTTON id = getID(event->pos());
  releasedButton(id);
}

// pressed button
void SoftwareButton::pressedButton(SoftwareButton::ID_BUTTON id)
{
  if (outputLog)
	cout << "Pressed : BUTTON_ID = " << id << endl << flush;

  switch (id){
  case ID_BUTTON_1:
	// Option
	toggleOptionButton();
	break;
  case ID_BUTTON_2:
	// Monitor
	toggleShowMonitorButton();
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
	break;
  case ID_BUTTON_12:
	// Info Button
	break;
  case ID_BUTTON_13:
	// Sound Button
	toggleShowSoundButton();
	break;
  case ID_BUTTON_14:
	// Sound OFF Button
	break;
  case ID_BUTTON_15:
	// Sound ON Button
	break;
  case ID_BUTTON_16:
	// Wheel +
	break;
  case ID_BUTTON_17:
	// Sound Quality
	toggleShowSoundQualityButton();
	break;
  case ID_BUTTON_18:
	// Sound Quality (Lowest)
	break;
  case ID_BUTTON_19:
	// Sound Quality (Low)
	break;
  case ID_BUTTON_20:
	// Sound Quality (Standard)
	break;
  case ID_BUTTON_21:
	// Sound Quality (High)
	break;
  case ID_BUTTON_22:
	// Sound Quality (Highest)
	break;
  case ID_BUTTON_23:
	// Wheel -
	break;
  case ID_BUTTON_24:
	// Video Quality
	toggleShowVideoQualityButton();
	break;
  case ID_BUTTON_25:
	// Video Quality (Lowest)
	break;
  case ID_BUTTON_26:
	// Video Quality (Low)
	break;
  case ID_BUTTON_27:
	// Video Quality (Standard)
	break;
  case ID_BUTTON_28:
	// Video Quality (High)
	break;
  case ID_BUTTON_29:
	// Video Quality (Highest)
	break;
  case ID_BUTTON_30:
	// Mouse Right Button
	break;
  case ID_BUTTON_31:
	// Mouse Left Button
	break;
  default:
	// error
	break;
  }

  // update buttons
  layout[id].pushed = true;
  update();
}

// released button
void SoftwareButton::releasedButton(SoftwareButton::ID_BUTTON id)
{
  if (outputLog)
	cout << "Released: BUTTON_ID = " << id << endl << flush;

  // update buttons
  layout[id].pushed = false;
  update();
}

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
// calculate layout
void SoftwareButton::calculateLayout(qreal xFactor, qreal yFactor)
{
  for(int i = ID_BUTTON_1; i < ID_BUTTON_NUM; i++){
	QRect rect = buttonLayout[i];
	layout[i].rect.setX(rect.x()*xFactor);
	layout[i].rect.setY(rect.y()*yFactor);
	layout[i].rect.setWidth(rect.width()*xFactor);
	layout[i].rect.setHeight(rect.height()*yFactor);
  }
  buttonSize = QSize(WIDTH * xFactor, HEIGHT * yFactor);
  //cout << "layout! : (W, H) = (" << buttonSize.width() << "," << buttonSize.height() << ")" << endl << flush;
}

 // get ID
SoftwareButton::ID_BUTTON SoftwareButton::getID(QPoint pos)
{
  for(int i = ID_BUTTON_1; i < ID_BUTTON_NUM; i++){
	if (buttonTopInfo[i].visible){
	  QRect rect = layout[i].rect;
	  if (rect.contains(pos, true)){ // inside the rectangle
		return (ID_BUTTON)i;
	  }
	}
  }

  return ID_BUTTON_0;
}

// toggle option button
void SoftwareButton::toggleOptionButton()
{
  onOptionButton = !onOptionButton;

  layout[ID_BUTTON_1].pushed = onOptionButton;

  // monitor option button
  buttonTopInfo[ID_BUTTON_2].visible = onOptionButton;
  // sound option button
  buttonTopInfo[ID_BUTTON_13].visible = onOptionButton;
  // sound quality option button
  buttonTopInfo[ID_BUTTON_17].visible = onOptionButton;
  // sound video option button
  buttonTopInfo[ID_BUTTON_24].visible = onOptionButton;

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

  update();
}

// toggle monitor button
void SoftwareButton::toggleShowMonitorButton()
{
  onShowMonitorButton = !onShowMonitorButton;

  layout[ID_BUTTON_2].pushed = onShowMonitorButton;

  buttonTopInfo[ID_BUTTON_3].visible = onShowMonitorButton;
  buttonTopInfo[ID_BUTTON_4].visible = onShowMonitorButton;
  buttonTopInfo[ID_BUTTON_5].visible = onShowMonitorButton;
  buttonTopInfo[ID_BUTTON_6].visible = onShowMonitorButton;
  buttonTopInfo[ID_BUTTON_7].visible = onShowMonitorButton;
  buttonTopInfo[ID_BUTTON_8].visible = onShowMonitorButton;
  buttonTopInfo[ID_BUTTON_9].visible = onShowMonitorButton;
  buttonTopInfo[ID_BUTTON_10].visible = onShowMonitorButton;
  buttonTopInfo[ID_BUTTON_11].visible = onShowMonitorButton;

  update();
}

// toggle sound button
void SoftwareButton::toggleShowSoundButton()
{
  onShowSoundButton = !onShowSoundButton;

  layout[ID_BUTTON_13].pushed = onShowSoundButton;

  buttonTopInfo[ID_BUTTON_14].visible = onShowSoundButton;
  buttonTopInfo[ID_BUTTON_15].visible = onShowSoundButton;

  update();
}

// toggle sound quality button
void SoftwareButton::toggleShowSoundQualityButton()
{
  onShowSoundQualityButton = !onShowSoundQualityButton;

  layout[ID_BUTTON_17].pushed = onShowSoundQualityButton;

  buttonTopInfo[ID_BUTTON_18].visible = onShowSoundQualityButton;
  buttonTopInfo[ID_BUTTON_19].visible = onShowSoundQualityButton;
  buttonTopInfo[ID_BUTTON_20].visible = onShowSoundQualityButton;
  buttonTopInfo[ID_BUTTON_21].visible = onShowSoundQualityButton;
  buttonTopInfo[ID_BUTTON_22].visible = onShowSoundQualityButton;

  update();
}

// toggle video quality button
void SoftwareButton::toggleShowVideoQualityButton()
{
  onShowVideoQualityButton = !onShowVideoQualityButton;

  layout[ID_BUTTON_24].pushed = onShowVideoQualityButton;

  buttonTopInfo[ID_BUTTON_25].visible = onShowVideoQualityButton;
  buttonTopInfo[ID_BUTTON_26].visible = onShowVideoQualityButton;
  buttonTopInfo[ID_BUTTON_27].visible = onShowVideoQualityButton;
  buttonTopInfo[ID_BUTTON_28].visible = onShowVideoQualityButton;
  buttonTopInfo[ID_BUTTON_29].visible = onShowVideoQualityButton;

  update();
}

} // end of namespace qtbrynhildr
