// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
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

using namespace std; // for TEST

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareKeyboard::SoftwareKeyboard(QWidget *parent)
  :
  SoftwareKeyboard(KEYTOP_TYPE_JP, parent)
{
}

// constructor
SoftwareKeyboard::SoftwareKeyboard(SoftwareKeyboard::KEYTOP_TYPE type, QWidget *parent)
  :
  QWidget(parent),
  keyTopTable(0),
#ifdef USE_KEYLAYOUTFILE
  klf(0),
#endif // USE_KEYLAYOUTFILE
  type(KEYTOP_TYPE_UNKNOWN),
  onShiftKey(false),
  onControlKey(false),
  onAltKey(false),
  onFnKey(false),
  pushedShiftKey(ID_KEY_0),
  pushedControlKey(ID_KEY_0),
  pushedAltKey(ID_KEY_0),
  pushedFnKey(ID_KEY_0),
  // for DEBUG
#ifdef DEBUG
  outputLog(true)
#else // DEBUG
  outputLog(false)
#endif // DEBUG
{
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
  fstream file;
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

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------

// paint event
void SoftwareKeyboard::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  QPainter painter(this);

  // fill keyboard panel
  QRect rect = QRect(QPoint(0,0), keyboardSize);
  //QColor panelColor = QColor::fromRgb(15, 31, 64);
  QColor panelColor = QColor::fromRgb(30, 30, 30);
  painter.fillRect(rect, panelColor);
  //  painter.fillRect(rect, Qt::darkCyan);
  //cout << "paint! : (W, H) = (" << keyboardSize.width() << "," << keyboardSize.height() << ")" << endl << flush;

  // change color for Pen
  //QColor penColor = QColor::fromRgb(61, 124, 250);
  QColor penColor = QColor::fromRgb(192, 192, 192);
  painter.setPen(penColor);
  // change font size
  QFont font = painter.font();
  font.setFamily("Courier");
  font.setPixelSize(16);
  //font.setStyleStrategy(QFont::NoAntialias);
  //  qDebug() << "family = " << font.family();
  painter.setFont(font);

  // for Shift Key Layout
  if (onShiftKey){
	for(int i = ID_KEY_1; i < ID_KEY_NUM; i++){
	  QRect rect = layout[i].rect;
	  if (layout[i].pushed)
		painter.fillRect(rect, Qt::black);
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
QSize SoftwareKeyboard::minimumSizeHint() const
{
  QSize size = keyboardSize + QSize(1, 1);
  return size;
}
#endif

// size hint
QSize SoftwareKeyboard::sizeHint() const
{
  QSize size = keyboardSize + QSize(1, 1);
  return size;
}

// mouse event
void SoftwareKeyboard::mousePressEvent(QMouseEvent *event)
{
  if (outputLog)
	cout << "Press  : (" << event->pos().x() << "," << event->pos().y() << ")" << endl << flush;

  ID_KEY id = getID(event->pos());
  pressedKey(id);
}

void SoftwareKeyboard::mouseReleaseEvent(QMouseEvent *event)
{
  if (outputLog)
	cout << "Release: (" << event->pos().x() << "," << event->pos().y() << ")" << endl << flush;

  ID_KEY id = getID(event->pos());
  releasedKey(id);
}

 // key down
void SoftwareKeyboard::keyDown(uchar key)
{
  if (outputLog){
	cout << "DOWN: VK_Code : " << getVKCodeByString(key)  << endl << flush;
  }
}
  
 // key up
void SoftwareKeyboard::keyUp(uchar key)
{
  if (outputLog){
	cout << "UP  : VK_Code : " << getVKCodeByString(key)  << endl << flush;
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
  //cout << "layout! : (W, H) = (" << keyboardSize.width() << "," << keyboardSize.height() << ")" << endl << flush;
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

  return ID_KEY_0;
}

// pressed key
void SoftwareKeyboard::pressedKey(ID_KEY id)
{
  if (outputLog){
	cout << "Pressed Key! id = " << id << endl << flush;
#if 0
	if (onShiftKey)
	  cout << "Pressed : " << keyTop[id].keyTop.keyTopWithShift << endl << flush;
	else if (onFnKey)
	  cout << "Pressed : " << keyTop[id].keyTopWithFn.keyTop << endl << flush;
	else
	  cout << "Pressed : " << keyTop[id].keyTop.keyTop << endl << flush;
#endif
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
	cout << "Released Key! id = " << id << endl << flush;
#if 0
	if (onShiftKey)
	  cout << "Released : " << keyTopTable[id].keyTop.keyTopWithShift << endl << flush;
	else if (onFnKey)
	  cout << "Released : " << keyTopTable[id].keyTopWithFn.keyTop << endl << flush;
	else
	  cout << "Released : " << keyTopTable[id].keyTop.keyTop << endl << flush;
#endif
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
	cout << "onShiftKey : " << onShiftKey << endl << flush;
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
	cout << "onControlKey : " << onControlKey << endl << flush;
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
	cout << "onAltKey : " << onAltKey << endl << flush;
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
	cout << "onFnKey : " << onFnKey << endl << flush;
  return key;
}

// get name of virtual keycode
string SoftwareKeyboard::getVKCodeByString(uchar vkcode)
{
  return stringTableOfVKCode[(int)vkcode];
}

// for DEBUG
#if QTB_DEBUG
// print KeyTop
void SoftwareKeyboard::printKeyTop(KeyTop *keyTop)
{
  cout << "keyTop.keyTop          : " << keyTop->keyTop.keyTop << endl;
  cout << "keyTop.keyTopWithShift : " << keyTop->keyTop.keyTopWithShift << endl;
  cout << "keyTop.VK_Code         : " << getVKCodeByString(keyTop->keyTop.VK_Code) << endl << endl;

  cout << "keyTopWithFn.keyTop    : " << keyTop->keyTopWithFn.keyTop << endl;
  cout << "keyTopWithFn.VK_Code   : " << keyTop->keyTopWithFn.VK_Code << endl << flush;
}
#endif // QTB_DEBUG

} // end of namespace qtbrynhildr
