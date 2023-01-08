// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

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

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareButton::SoftwareButton(QWidget *parent)
  :QWidget(parent)
  ,onButton(false)
  ,onOptionButton(false)
  ,onShowSoundQualityButton(false)
  ,onShowVideoQualityButton(false)
  ,onShowSoundButton(false)
  ,onShowSoundCacheButton(false)
  ,onShowPublicModeButton(false)
  ,onShowVideoFPSButton(false)
  ,currentFrameRate(0.0)
  ,currentDataRate(0.0)
  ,fontSize(32)
  ,penWidth(2)
  ,alpha(255)
  // for DEBUG
  ,outputLog(false)
{
  // set widget attributes
  setAttribute(Qt::WA_NoSystemBackground, true); // NOT fill background

#if defined(QTB_DEV_DESKTOP)
  // mouse tracking on
  setMouseTracking(true);
#endif // defined(QTB_DEV_DESKTOP)

  // reset flag
  for(int i = ID_BUTTON_1; i < ID_BUTTON_NUM; i++){
	layout[i].pushed = false;
	layout[i].selected = false;
  }

  // for Info
  layout[ID_BUTTON_6].pushed = true;

  // initialize layout
  calculateLayout(5, 5);
}

// reset size
QSize SoftwareButton::resetSize()
{
  // initialize layout
  calculateLayout(5, 5);
  resize(buttonSize);
  return buttonSize;
}

// size hint
QSize SoftwareButton::sizeHint() const
{
  QSize size = buttonSize + QSize(1, 1);
  return size;
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------

// paint event
void SoftwareButton::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  // panel color
  QColor panelColor = QColor::fromRgb(0, 0, 0, alpha);
  // pen color
  QColor penColor = QColor::fromRgb(61, 124, 250, alpha);
  // create pen
  QPen pen = QPen(penColor);
  pen.setWidth(penWidth);

  // painter
  QPainter painter(this);

  // change font size
  QFont font = painter.font();
  font.setPixelSize(fontSize);
  painter.setFont(font);

  // draw all buttons
  for(int i = ID_BUTTON_1; i < ID_BUTTON_NUM; i++){
	//if (true){
	if (buttonTopTable[i].visible){
	  QRect rect = layout[i].rect;
	  if (layout[i].pushed){
		painter.setBrush(penColor);
	  }
	  else {
		painter.setBrush(panelColor);
	  }
	  painter.setPen(pen);
#if QT_VERSION >= 0x050d00 // Qt 5.13.0
	  painter.drawRoundedRect(rect, 25, 25);
#else // QT_VERSION >= 0x050d00
	  painter.drawRoundRect(rect);
#endif // QT_VERSION >= 0x050d00
	  //painter.drawRect(rect);
	  if (layout[i].pushed){
		painter.setPen(panelColor);
	  }
	  else {
		painter.setPen(penColor);
	  }
	  if (i == ID_BUTTON_6){ // Info
		QString str = QString("%1 fps\n\n%2 Mbps").
		  arg(currentFrameRate, 4, 'f', 1, ' ').
		  arg(currentDataRate, 4, 'f', 1, ' ');
		painter.drawText(rect,
						 Qt::AlignCenter,
						 str
						 );
	  }
	  else {
		painter.drawText(rect,
						 Qt::AlignCenter,
						 buttonTopTable[i].buttonTop
						 );
	  }
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
	//std::cout << "resize! : (W, H) = (" << size.width() << "," << size.height() << ")" << std::endl << std::flush;
	qreal xFactor = (qreal)size.width()/WIDTH;
	qreal yFactor = (qreal)size.height()/HEIGHT;
	// recalculate layout
	calculateLayout(xFactor, yFactor);
	update();
  }
  else {
	//std::cout << "resize! : same size" << std::endl << std::flush;
  }
}

// is on button
bool SoftwareButton::isOnButton() const
{
  return onButton;
}

// mouse event
void SoftwareButton::mousePressEvent(QMouseEvent *event)
{
  if (outputLog)
	std::cout << "Press  : (" << event->pos().x() << "," << event->pos().y() << ")" << std::endl << std::flush;
  ID_BUTTON id = getID(event->pos());
  if (id != ID_BUTTON_NULL){
	pressedButton(id);
	onButton = true;
  }
  else {
	QWidget::mousePressEvent(event);
	onButton = false;
  }
}

void SoftwareButton::mouseReleaseEvent(QMouseEvent *event)
{
  if (outputLog)
	std::cout << "Release: (" << event->pos().x() << "," << event->pos().y() << ")" << std::endl << std::flush;
  ID_BUTTON id = getID(event->pos());
  if (id != ID_BUTTON_NULL){
	releasedButton(id);
	onButton = true;
  }
  else {
	QWidget::mouseReleaseEvent(event);
	onButton = false;
  }
}

void SoftwareButton::mouseMoveEvent(QMouseEvent *event)
{
  ID_BUTTON id = getID(event->pos());
  if (id != ID_BUTTON_NULL){
	onButton = true;
  }
  else {
	QWidget::mouseMoveEvent(event);
	onButton = false;
  }
}

// pressed button
void SoftwareButton::pressedButton(SoftwareButton::ID_BUTTON id)
{
  if (outputLog)
	std::cout << "Pressed : ID_BUTTON = " << id << std::endl << std::flush;

  switch (id){
  case ID_BUTTON_1:
	// Fn
	break;
  case ID_BUTTON_2:
	// clear buttons
	clearButtons();
	// Monitor
	break;
  case ID_BUTTON_3:
	if (!onShowSoundButton){
	  // clear buttons
	  clearButtons();
	}
	// Sound
	toggleShowSoundButton();
	break;
  case ID_BUTTON_4:
  case ID_BUTTON_5:
	// Sound ON/OFF
	for(int i = ID_BUTTON_4; i <= ID_BUTTON_5; i++){
	  layout[i].pushed = false;
	  layout[i].selected = false;
	}
	layout[id].selected = true;
	break;
  case ID_BUTTON_6:
	// Info button
	// Nothing to do
	return;
	break;
  case ID_BUTTON_7:
	if (!onShowSoundCacheButton){
	  // clear buttons
	  clearButtons();
	}
	// Sound Cache
	toggleShowSoundCacheButton();
	break;
  case ID_BUTTON_8:
  case ID_BUTTON_9:
  case ID_BUTTON_10:
  case ID_BUTTON_11:
  case ID_BUTTON_12:
	for(int i = ID_BUTTON_8; i <= ID_BUTTON_12; i++){
	  layout[i].pushed = false;
	  layout[i].selected = false;
	}
	layout[id].selected = true;
	break;
  case ID_BUTTON_13:
	// Option
	toggleOptionButton();
	break;
  case ID_BUTTON_14:
	if (!onShowSoundQualityButton){
	  // clear buttons
	  clearButtons();
	}
	// Sound Quality
	toggleShowSoundQualityButton();
	break;
  case ID_BUTTON_15:
	// Sound Quality (Lowest)
  case ID_BUTTON_16:
	// Sound Quality (Low)
  case ID_BUTTON_17:
	// Sound Quality (Standard)
  case ID_BUTTON_18:
	// Sound Quality (High)
  case ID_BUTTON_19:
	// Sound Quality (Highest)
	for(int i = ID_BUTTON_15; i <= ID_BUTTON_19; i++){
	  layout[i].pushed = false;
	  layout[i].selected = false;
	}
	layout[id].selected = true;
	break;
  case ID_BUTTON_20:
	// Wheel +
	break;
  case ID_BUTTON_21:
	if (!onShowPublicModeButton){
	  // clear buttons
	  clearButtons();
	}
	// Public Mode
	toggleShowPublicModeButton();
	break;
  case ID_BUTTON_22:
  case ID_BUTTON_23:
	// Public Mode
	for(int i = ID_BUTTON_22; i <= ID_BUTTON_23; i++){
	  layout[i].pushed = false;
	  layout[i].selected = false;
	}
	layout[id].selected = true;
	break;
  case ID_BUTTON_24:
	// Wheel -
	break;
  case ID_BUTTON_25:
	if (!onShowVideoQualityButton){
	  // clear buttons
	  clearButtons();
	}
	// Video Quality
	toggleShowVideoQualityButton();
	break;
  case ID_BUTTON_26:
	// Video Quality (Lowest)
  case ID_BUTTON_27:
	// Video Quality (Low)
  case ID_BUTTON_28:
	// Video Quality (Standard)
  case ID_BUTTON_29:
	// Video Quality (High)
  case ID_BUTTON_30:
	// Video Quality (Highest)
	for(int i = ID_BUTTON_26; i <= ID_BUTTON_30; i++){
	  layout[i].pushed = false;
	  layout[i].selected = false;
	}
	layout[id].selected = true;
	break;
  case ID_BUTTON_31:
	// Mouse Left Button
	break;
  case ID_BUTTON_32:
	// Mouse Right Button
	break;
  case ID_BUTTON_33:
	if (!onShowVideoFPSButton){
	  // clear buttons
	  clearButtons();
	}
	// Video FPS
	toggleShowVideoFPSButton();
	break;
  case ID_BUTTON_34:
	// Video FPS (Minimum)
  case ID_BUTTON_35:
	// Video FPS (10)
  case ID_BUTTON_36:
	// Video FPS (30)
  case ID_BUTTON_37:
	// Video FPS (60)
  case ID_BUTTON_38:
	// Video FPS (Maximum)
	for(int i = ID_BUTTON_34; i <= ID_BUTTON_38; i++){
	  layout[i].pushed = false;
	  layout[i].selected = false;
	}
	layout[id].selected = true;
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
	std::cout << "Released: ID_BUTTON = " << id << std::endl << std::flush;

  switch (id){
  case ID_BUTTON_1:
	// Fn
	break;
  case ID_BUTTON_6:
	// Info button
	// Nothing to do
	return;
	break;
  default:
	// fall through
	break;
  }

  // update buttons
  if (!layout[id].selected){
	layout[id].pushed = false;
	update();
  }
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
  //  std::cout << "Button Layout! : (W, H) = (" << buttonSize.width() << "," << buttonSize.height() << ")" << std::endl << std::flush;;
  // font ,pen, etc
  //  setFontSize(32);
  setFontSize((int)((double)buttonSize.height()/1080*32));
  setPenWidth(1);
  setOpacity(0.8);
}

 // get ID
SoftwareButton::ID_BUTTON SoftwareButton::getID(QPoint pos)
{
  for(int i = ID_BUTTON_1; i < ID_BUTTON_NUM; i++){
	if (buttonTopTable[i].visible){
	  QRect rect = layout[i].rect;
	  if (rect.contains(pos, true)){ // inside the rectangle
		return (ID_BUTTON)i;
	  }
	}
  }

  return ID_BUTTON_NULL;
}

// clear buttons
void SoftwareButton::clearButtons()
{
  // clear buttons
  if (onShowSoundButton)
	toggleShowSoundButton();
  if (onShowSoundQualityButton)
	toggleShowSoundQualityButton();
  if (onShowVideoQualityButton)
	toggleShowVideoQualityButton();
  if (onShowSoundCacheButton)
	toggleShowSoundCacheButton();
  if (onShowPublicModeButton)
	toggleShowPublicModeButton();
  if (onShowVideoFPSButton)
	toggleShowVideoFPSButton();
}

// toggle option button
void SoftwareButton::toggleOptionButton()
{
  onOptionButton = !onOptionButton;

  layout[ID_BUTTON_13].pushed = onOptionButton;
  layout[ID_BUTTON_13].selected = onOptionButton;

  // monitor option button
  buttonTopTable[ID_BUTTON_2].visible = onOptionButton;
  // sound option button
  buttonTopTable[ID_BUTTON_3].visible = onOptionButton;
  // sound cache button
  buttonTopTable[ID_BUTTON_7].visible = onOptionButton;
  // sound quality button
  buttonTopTable[ID_BUTTON_14].visible = onOptionButton;
  // publilc mode button
  buttonTopTable[ID_BUTTON_21].visible = onOptionButton;
  // video quality button
  buttonTopTable[ID_BUTTON_25].visible = onOptionButton;
  // video FPS button
  buttonTopTable[ID_BUTTON_33].visible = onOptionButton;

  if(!onOptionButton){
	// clear buttons
	clearButtons();
  }

  update();
}

// toggle sound button
void SoftwareButton::toggleShowSoundButton()
{
  onShowSoundButton = !onShowSoundButton;

  layout[ID_BUTTON_3].pushed = onShowSoundButton;
  layout[ID_BUTTON_3].selected = onShowSoundButton;

  buttonTopTable[ID_BUTTON_4].visible = onShowSoundButton;
  buttonTopTable[ID_BUTTON_5].visible = onShowSoundButton;

  update();
}

// toggle sound quality button
void SoftwareButton::toggleShowSoundQualityButton()
{
  onShowSoundQualityButton = !onShowSoundQualityButton;

  layout[ID_BUTTON_14].pushed = onShowSoundQualityButton;
  layout[ID_BUTTON_14].selected = onShowSoundQualityButton;

  buttonTopTable[ID_BUTTON_15].visible = onShowSoundQualityButton;
  buttonTopTable[ID_BUTTON_16].visible = onShowSoundQualityButton;
  buttonTopTable[ID_BUTTON_17].visible = onShowSoundQualityButton;
  buttonTopTable[ID_BUTTON_18].visible = onShowSoundQualityButton;
  buttonTopTable[ID_BUTTON_19].visible = onShowSoundQualityButton;

  update();
}

// toggle video quality button
void SoftwareButton::toggleShowVideoQualityButton()
{
  onShowVideoQualityButton = !onShowVideoQualityButton;

  layout[ID_BUTTON_25].pushed = onShowVideoQualityButton;
  layout[ID_BUTTON_25].selected = onShowVideoQualityButton;

  buttonTopTable[ID_BUTTON_26].visible = onShowVideoQualityButton;
  buttonTopTable[ID_BUTTON_27].visible = onShowVideoQualityButton;
  buttonTopTable[ID_BUTTON_28].visible = onShowVideoQualityButton;
  buttonTopTable[ID_BUTTON_29].visible = onShowVideoQualityButton;
  buttonTopTable[ID_BUTTON_30].visible = onShowVideoQualityButton;

  update();
}

// toggle sound cache button
void SoftwareButton::toggleShowSoundCacheButton()
{
  onShowSoundCacheButton = !onShowSoundCacheButton;

  layout[ID_BUTTON_7].pushed = onShowSoundCacheButton;
  layout[ID_BUTTON_7].selected = onShowSoundCacheButton;

  buttonTopTable[ID_BUTTON_8].visible = onShowSoundCacheButton;
  buttonTopTable[ID_BUTTON_9].visible = onShowSoundCacheButton;
  buttonTopTable[ID_BUTTON_10].visible = onShowSoundCacheButton;
  buttonTopTable[ID_BUTTON_11].visible = onShowSoundCacheButton;
  buttonTopTable[ID_BUTTON_12].visible = onShowSoundCacheButton;

  update();
}

// toggle public mode button
void SoftwareButton::toggleShowPublicModeButton()
{
  onShowPublicModeButton = !onShowPublicModeButton;

  layout[ID_BUTTON_21].pushed = onShowPublicModeButton;
  layout[ID_BUTTON_21].selected = onShowPublicModeButton;

  buttonTopTable[ID_BUTTON_22].visible = onShowPublicModeButton;
  buttonTopTable[ID_BUTTON_23].visible = onShowPublicModeButton;

  update();
}

// toggle video FPS button
void SoftwareButton::toggleShowVideoFPSButton()
{
  onShowVideoFPSButton = !onShowVideoFPSButton;

  layout[ID_BUTTON_33].pushed = onShowVideoFPSButton;
  layout[ID_BUTTON_33].selected = onShowVideoFPSButton;

  buttonTopTable[ID_BUTTON_34].visible = onShowVideoFPSButton;
  buttonTopTable[ID_BUTTON_35].visible = onShowVideoFPSButton;
  buttonTopTable[ID_BUTTON_36].visible = onShowVideoFPSButton;
  buttonTopTable[ID_BUTTON_37].visible = onShowVideoFPSButton;
  buttonTopTable[ID_BUTTON_38].visible = onShowVideoFPSButton;

  update();
}

} // end of namespace qtbrynhildr
