// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#if 0 // for TEST
#include <cstring>
#endif // for TEST
#include <fstream> // for TEST
#include <iostream> // for TEST

// Qt Header
#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPen>

// Local Header
#ifdef USE_KEYLAYOUTFILE
#include "keylayout/keylayoutfile.h"
#endif // USE_KEYLAYOUTFILE
#include "software_keyboard.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareKeyboard::SoftwareKeyboard(QWidget *parent)
  :SoftwareKeyboard(KEYTOP_TYPE_JP, parent)
{
}

// constructor
SoftwareKeyboard::SoftwareKeyboard(SoftwareKeyboard::KEYTOP_TYPE type, QWidget *parent)
  :QWidget(parent)
  ,keyTopTable(0)
#if 0 // for TEST
  ,tempKeyTopTable(0)
#endif // for TEST
#ifdef USE_KEYLAYOUTFILE
  ,klf(0)
#endif // USE_KEYLAYOUTFILE
  ,type(KEYTOP_TYPE_UNKNOWN)
  ,onButton(false)
  ,onShiftKey(false)
  ,onControlKey(false)
  ,onAltKey(false)
  ,onFnKey(false)
  ,pushedShiftKey(ID_KEY_NULL)
  ,pushedControlKey(ID_KEY_NULL)
  ,pushedAltKey(ID_KEY_NULL)
  ,pushedFnKey(ID_KEY_NULL)
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
  for(int i = ID_KEY_1; i < ID_KEY_NUM; i++){
	layout[i].pushed = false;
  }

  // initialize layout
  calculateLayout(INITIAL_XFACTOR, INITIAL_YFACTOR);

  // set keyboard type
  setKeytopType(type);

#ifdef USE_KEYLAYOUTFILE
#if 0 // for TEST
  std::fstream file;
  file.open("keyTopTable_JP.dat", ios::out | ios::binary | ios::trunc);
  if (file.is_open()){
	file.write((char *)&keyTopTable_JP[1], sizeof(KeyTop)*(ID_KEY_NUM-1));
	file.close();
  }
  file.open("keyTopTable_US.dat", ios::out | ios::binary | ios::trunc);
  if (file.is_open()){
	file.write((char *)&keyTopTable_US[1], sizeof(KeyTop)*(ID_KEY_NUM-1));
	file.close();
  }
#endif // for TEST
#endif // USE_KEYLAYOUTFILE
}

#ifdef USE_KEYLAYOUTFILE
SoftwareKeyboard::SoftwareKeyboard(KeyLayoutFile *klf, QWidget *parent)
  :
  SoftwareKeyboard(KEYTOP_TYPE_KLF, parent)
{
  setKeytopType(klf);
}
#endif // USE_KEYLAYOUTFILE

// destructor
SoftwareKeyboard::~SoftwareKeyboard()
{
  // delete objects
#if 0 // for TEST
  // temporary key top table
  if (tempKeyTopTable != 0){
	delete [] tempKeyTopTable;
	tempKeyTopTable = 0;
  }
#endif // for TEST
}

// get keytop type
SoftwareKeyboard::KEYTOP_TYPE SoftwareKeyboard::getKeytopType()
{
  return type;
}

// get keytop type name
QString SoftwareKeyboard::getKeytopTypeName()
{
  switch(type){
  case KEYTOP_TYPE_JP:
	return QString("JP");
	break;
  case KEYTOP_TYPE_US:
	return QString("US");
	break;
#ifdef USE_KEYLAYOUTFILE
  case KEYTOP_TYPE_KLF:
	return QString(klf->name);
	break;
#endif // USE_KEYLAYOUTFILE
  default:
	return QString("Unknown");
	break;
  }
}

// set keyboard type
void SoftwareKeyboard::setKeytopType(KEYTOP_TYPE type){
  switch(type){
  case KEYTOP_TYPE_JP:
	keyTopTable = keyTopTable_JP;
	break;
  case KEYTOP_TYPE_US:
	keyTopTable = keyTopTable_US;
	break;
#ifdef USE_KEYLAYOUTFILE
  case KEYTOP_TYPE_KLF:
	// set type only
	this->type = type;
	return;
	break;
#endif // USE_KEYLAYOUTFILE
  default:
	// No Change
	return;
	break;
  }

  this->type = type;

  update();
}

#ifdef USE_KEYLAYOUTFILE
// set keytop type by key layout file
void SoftwareKeyboard::setKeytopType(KeyLayoutFile *klf)
{
  // set keyboard type
  setKeytopType(KEYTOP_TYPE_KLF);

  this->klf = klf;

  keyTopTable = klf->keyTopTable;
#if 0 // for TEST
  if (klf->softkeynum < ID_KEY_NUM){
	// 1) allocate a new table (size = ID_KEY_NUM)
	tempKeyTopTable = new KeyTop[ID_KEY_NUM];

	// 2) copy US keyboard table to the new table
	memcpy(tempKeyTopTable, keyTopTable_US, sizeof(KeyTop)*ID_KEY_NUM);

	// 3) overwrite klf->keyTopTable to the new table
	memcpy(tempKeyTopTable, klf->keyTopTable, sizeof(KeyTop)*klf->softkeynum);

	// 4) set new keyTopTable
	keyTopTable = tempKeyTopTable;
  }
#endif // for TEST

  update();
}
#endif // USE_KEYLAYOUTFILE

// reset size
QSize SoftwareKeyboard::resetSize()
{
  // initialize layout
  QSize size = QSize(WIDTH*INITIAL_XFACTOR, HEIGHT*INITIAL_YFACTOR);
  resize(size);
  return size;
}

// size hint
QSize SoftwareKeyboard::sizeHint() const
{
  QSize size = keyboardSize + QSize(1, 1);
  return size;
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------

// paint event
void SoftwareKeyboard::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  //std::cout << "paint! : (W, H) = (" << keyboardSize.width() << "," << keyboardSize.height() << ")" << std::endl << std::flush;

  // panel color
  QColor panelColor = QColor::fromRgb(15, 31, 64, alpha);
  //QColor panelColor = QColor::fromRgb(30, 30, 30, alpha);
  QColor penColor = QColor::fromRgb(61, 124, 250, 255);
  //QColor penColor = QColor::fromRgb(192, 192, 192, alpha);
  // create pen
  QPen pen = QPen(penColor);
  pen.setWidth(penWidth);

  // painter
  QPainter painter(this);

  // change pen
  painter.setPen(pen);

  // change font size
  QFont font = painter.font();
  font.setFamily("Courier");
  font.setPixelSize(fontSize);
  //font.setStyleStrategy(QFont::NoAntialias);
  //  qDebug() << "family = " << font.family();
  painter.setFont(font);

  // for Shift Key Layout
  if (onShiftKey){
	for(int i = ID_KEY_1; i < ID_KEY_NUM; i++){
	  QRect rect = layout[i].rect;
	  if (layout[i].pushed){
		painter.setBrush(Qt::black);
	  }
	  else {
		painter.setBrush(panelColor);
	  }
	  painter.drawRect(rect);
	  painter.drawText(rect,
					   Qt::AlignCenter,
					   keyTopTable[i].keyTop.keyTopWithShift
					   );
	}
	return;
  }

  // for Fn Key Layout
  if (onFnKey){
	for(int i = ID_KEY_1; i < ID_KEY_NUM; i++){
	  QRect rect = layout[i].rect;
	  if (layout[i].pushed)
		painter.fillRect(rect, Qt::black);
	  else
		painter.fillRect(rect, panelColor);
	  painter.drawRect(rect);
	  painter.drawText(rect,
					   Qt::AlignCenter,
					   keyTopTable[i].keyTopWithFn.keyTop
					   );
	}
	return;
  }

  // for Normal Key Layout
  for(int i = ID_KEY_1; i < ID_KEY_NUM; i++){
	QRect rect = layout[i].rect;
	if (layout[i].pushed)
	  painter.fillRect(rect, Qt::black);
	else
	  painter.fillRect(rect, panelColor);
	painter.drawRect(rect);
	painter.drawText(rect,
					 Qt::AlignCenter,
					 keyTopTable[i].keyTop.keyTop
					 );
  }

  // restore color for Pen
  //  painter.setPen(Qt::black);
}

// resize event
void SoftwareKeyboard::resizeEvent(QResizeEvent *event)
{
  QSize size = event->size() - QSize(1, 1);
  if (size != keyboardSize){
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
bool SoftwareKeyboard::isOnButton() const
{
  return onButton;
}

// mouse event
void SoftwareKeyboard::mousePressEvent(QMouseEvent *event)
{
  if (outputLog)
	std::cout << "Press  : (" << event->pos().x() << "," << event->pos().y() << ")" << std::endl << std::flush;

  ID_KEY id = getID(event->pos());
  if (id != ID_KEY_NULL){
	pressedKey(id);
	onButton = true;
  }
  else {
	onButton = false;
  }
}

void SoftwareKeyboard::mouseReleaseEvent(QMouseEvent *event)
{
  if (outputLog)
	std::cout << "Release: (" << event->pos().x() << "," << event->pos().y() << ")" << std::endl << std::flush;

  ID_KEY id = getID(event->pos());
  if (id != ID_KEY_NULL){
	releasedKey(id);
	onButton = true;
  }
  else {
	onButton = false;
  }
}

void SoftwareKeyboard::mouseMoveEvent(QMouseEvent *event)
{
  ID_KEY id = getID(event->pos());
  if (id != ID_KEY_NULL){
	onButton = true;
  }
  else {
	QWidget::mouseMoveEvent(event);
	onButton = false;
  }
}

// key down
void SoftwareKeyboard::keyDown(uchar key)
{
  if (outputLog){
	std::cout << "DOWN: VK_Code : " << getVKCodeByString(key)  << std::endl << std::flush;
  }
}

// key up
void SoftwareKeyboard::keyUp(uchar key)
{
  if (outputLog){
	std::cout << "UP  : VK_Code : " << getVKCodeByString(key)  << std::endl << std::flush;
  }
}

// calculate layout
void SoftwareKeyboard::calculateLayout(qreal xFactor, qreal yFactor)
{
  for(int i = ID_KEY_1; i < ID_KEY_NUM; i++){
	QRect rect = keyLayout[i];
	layout[i].rect.setX(rect.x()*xFactor);
	layout[i].rect.setY(rect.y()*yFactor);
	layout[i].rect.setWidth(rect.width()*xFactor);
	layout[i].rect.setHeight(rect.height()*yFactor);
  }
  keyboardSize = QSize(WIDTH * xFactor, HEIGHT * yFactor);
  std::cout << "Keyborad Layout! : (W, H) = (" << keyboardSize.width() << "," << keyboardSize.height() << ")" << std::endl << std::flush;
  // font ,pen, etc
  //  setFontSize(32);
  setFontSize((int)((double)keyboardSize.height()/1080*32));
  setPenWidth(1);
  setOpacity(0.8);
}

// get ID
SoftwareKeyboard::ID_KEY SoftwareKeyboard::getID(QPoint pos) const
{
  for(int i = ID_KEY_1; i < ID_KEY_NUM; i++){
	QRect rect = layout[i].rect;
	if (rect.contains(pos, true)){ // inside the rectangle
	  return (ID_KEY)i;
	}
  }

  return ID_KEY_NULL;
}

// pressed key
void SoftwareKeyboard::pressedKey(ID_KEY id)
{
  if (outputLog){
	std::cout << "Pressed Key! id = " << id << std::endl << std::flush;
#if 0 // for TEST
	if (onShiftKey)
	  std::cout << "Pressed : " << keyTop[id].keyTop.keyTopWithShift << std::endl << std::flush;
	else if (onFnKey)
	  std::cout << "Pressed : " << keyTop[id].keyTopWithFn.keyTop << std::endl << std::flush;
	else
	  std::cout << "Pressed : " << keyTop[id].keyTop.keyTop << std::endl << std::flush;
#endif // for TEST
  }

  // pushed key
  uchar key = keyTopTable[id].keyTop.VK_Code;

  switch(id){
  case ID_KEY_LSHIFT:
  case ID_KEY_RSHIFT:
	// Shift
	key = pressedShiftKey(id);
	layout[ID_KEY_LSHIFT].pushed = onShiftKey;
	layout[ID_KEY_RSHIFT].pushed = onShiftKey;
	break;
  case ID_KEY_LCONTROL:
  case ID_KEY_RCONTROL:
	// Control
	key = pressedControlKey(id);
	layout[ID_KEY_LCONTROL].pushed = onControlKey;
	layout[ID_KEY_RCONTROL].pushed = onControlKey;
	break;
  case ID_KEY_LALT:
  case ID_KEY_RALT:
	// Alt
	key = pressedAltKey(id);
	layout[ID_KEY_LALT].pushed = onAltKey;
	layout[ID_KEY_RALT].pushed = onAltKey;
	break;
  case ID_KEY_LFn:
  case ID_KEY_RFn:
	// Fn
	key = pressedFnKey(id);
	layout[ID_KEY_LFn].pushed = onFnKey;
	layout[ID_KEY_RFn].pushed = onFnKey;
	break;
  default:
	if (onFnKey){
	  // Fn key
	  key = keyTopTable[id].keyTopWithFn.VK_Code;
	}
	layout[id].pushed = true;
	break;
  }

  // pressed key
  if (key != VK_NONE_00){
	// send pressed key
	keyDown(key);
  }

  // repaint keyboard
  update();
}

// released key
void SoftwareKeyboard::releasedKey(ID_KEY id)
{
  if (outputLog){
	std::cout << "Released Key! id = " << id << std::endl << std::flush;
#if 0 // for TEST
	if (onShiftKey)
	  std::cout << "Released : " << keyTopTable[id].keyTop.keyTopWithShift << std::endl << std::flush;
	else if (onFnKey)
	  std::cout << "Released : " << keyTopTable[id].keyTopWithFn.keyTop << std::endl << std::flush;
	else
	  std::cout << "Released : " << keyTopTable[id].keyTop.keyTop << std::endl << std::flush;
#endif // for TEST
  }

  // released key
  uchar key = VK_NONE_00;

  switch(id){
  case ID_KEY_LSHIFT:
  case ID_KEY_RSHIFT:
	// Shift
  case ID_KEY_LCONTROL:
  case ID_KEY_RCONTROL:
	// Control
  case ID_KEY_LALT:
  case ID_KEY_RALT:
	// Alt
  case ID_KEY_LFn:
  case ID_KEY_RFn:
	// Fn
	// Nothig to do
	break;
  default:
	key = keyTopTable[id].keyTop.VK_Code;
	layout[id].pushed = false;
	break;
  }

  // released key
  if (key != VK_NONE_00){
	// send released key
	keyUp(key);
  }

  // repaint keyboard
  update();
}

// shift key
uchar SoftwareKeyboard::pressedShiftKey(ID_KEY id)
{
  uchar key = keyTopTable[id].keyTop.VK_Code;
  onShiftKey = !onShiftKey;
  if (onShiftKey){
	pushedShiftKey = id;
  }
  else {
	keyUp(keyTopTable[pushedShiftKey].keyTop.VK_Code);
	key = VK_NONE_00; // NOT downKey
  }
  if (outputLog)
	std::cout << "onShiftKey : " << onShiftKey << std::endl << std::flush;
  return key;
}

// control key
uchar SoftwareKeyboard::pressedControlKey(ID_KEY id)
{
  uchar key = keyTopTable[id].keyTop.VK_Code;
  onControlKey = !onControlKey;
  if (onControlKey){
	pushedControlKey = id;
  }
  else {
	keyUp(keyTopTable[pushedControlKey].keyTop.VK_Code);
	key = VK_NONE_00; // NOT downKey
  }
  if (outputLog)
	std::cout << "onControlKey : " << onControlKey << std::endl << std::flush;
  return key;
}

// Alt key
uchar SoftwareKeyboard::pressedAltKey(ID_KEY id)
{
  uchar key = keyTopTable[id].keyTop.VK_Code;
  onAltKey = !onAltKey;
  if (onAltKey){
	pushedAltKey = id;
  }
  else {
	keyUp(keyTopTable[pushedAltKey].keyTop.VK_Code);
	key = VK_NONE_00; // NOT downKey
  }
  if (outputLog)
	std::cout << "onAltKey : " << onAltKey << std::endl << std::flush;
  return key;
}

// Fn key
uchar SoftwareKeyboard::pressedFnKey(ID_KEY id)
{
  uchar key = keyTopTable[id].keyTop.VK_Code;
  onFnKey = !onFnKey;
  if (onFnKey){
	pushedFnKey = id;
  }
  else {
	keyUp(keyTopTable[pushedFnKey].keyTop.VK_Code);
	key = VK_NONE_00; // NOT downKey
  }
  if (outputLog)
	std::cout << "onFnKey : " << onFnKey << std::endl << std::flush;
  return key;
}

// get name of virtual keycode
std::string SoftwareKeyboard::getVKCodeByString(uchar vkcode)
{
  return stringTableOfVKCode[(int)vkcode];
}

// for DEBUG
#if QTB_DEBUG
// print KeyTop
void SoftwareKeyboard::printKeyTop(KeyTop *keyTop)
{
  std::cout << "keyTop.keyTop          : " << keyTop->keyTop.keyTop << std::endl;
  std::cout << "keyTop.keyTopWithShift : " << keyTop->keyTop.keyTopWithShift << std::endl;
  std::cout << "keyTop.VK_Code         : " << getVKCodeByString(keyTop->keyTop.VK_Code) << std::endl << std::endl;

  std::cout << "keyTopWithFn.keyTop    : " << keyTop->keyTopWithFn.keyTop << std::endl;
  std::cout << "keyTopWithFn.VK_Code   : " << keyTop->keyTopWithFn.VK_Code << std::endl << std::flush;
}
#endif // QTB_DEBUG

} // end of namespace qtbrynhildr
