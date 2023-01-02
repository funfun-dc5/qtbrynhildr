// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <fstream> // for TEST
#include <iostream> // for TEST
#include <iomanip> // for TEST

// Qt Header
#include <QString>

#if defined(QTB_DEBUG)
#include <QDebug>
#endif // defined(QTB_DEBUG)

// Local Header

#include "eventconverter.h"
#ifdef USE_KEYLAYOUTFILE
#include "keylayout/keylayoutfile.h"
#endif // USE_KEYLAYOUTFILE

namespace qtbrynhildr {

// constructor
EventConverter::EventConverter()
  :EventConverter(KEYTOP_TYPE_JP)
{

#if defined(QTB_DEV_TOUCHPANEL)
  // check and fix key for OEM keys
  keyMap["!"] = Qt::Key_Exclam;
  keyMap["\""] = Qt::Key_QuoteDbl;
  keyMap["#"] = Qt::Key_NumberSign;
  keyMap["$"] = Qt::Key_Dollar;
  keyMap["%"] = Qt::Key_Percent;
  keyMap["&"] = Qt::Key_Ampersand;
  keyMap["'"] = Qt::Key_Apostrophe;
  keyMap["("] = Qt::Key_ParenLeft;
  keyMap[")"] = Qt::Key_ParenRight;

  keyMap["*"] = Qt::Key_Asterisk;
  keyMap["+"] = Qt::Key_Plus;
  keyMap[","] = Qt::Key_Comma;
  keyMap["-"] = Qt::Key_Minus;
  keyMap["."] = Qt::Key_Period;
  keyMap["/"] = Qt::Key_Slash;

  keyMap[":"] = Qt::Key_Colon;
  keyMap[";"] = Qt::Key_Semicolon;
  keyMap["<"] = Qt::Key_Less;
  keyMap["="] = Qt::Key_Equal;
  keyMap[">"] = Qt::Key_Greater;
  keyMap["?"] = Qt::Key_Question;
  keyMap["@"] = Qt::Key_At;
  keyMap["`"] = Qt::Key_QuoteLeft;

  keyMap["["] = Qt::Key_BracketLeft;
  keyMap["{"] = Qt::Key_BraceLeft;
  keyMap["\\"] = Qt::Key_Backslash;
  keyMap["|"] = Qt::Key_yen;
  keyMap["_"] = Qt::Key_Underscore;
  keyMap["]"] = Qt::Key_BracketRight;
  keyMap["}"] = Qt::Key_BraceRight;
  keyMap["~"] = Qt::Key_AsciiTilde;
  keyMap["^"] = Qt::Key_AsciiCircum;
#endif // defined(QTB_DEV_TOUCHPANEL)
}

EventConverter::EventConverter(KEYTOP_TYPE type)
  :keyEventTable(0)
  ,tableSize(0)
#ifdef USE_KEYLAYOUTFILE
  ,klf(0)
#endif // USE_KEYLAYOUTFILE
  ,type(KEYTOP_TYPE_UNKNOWN)
  ,shiftKeyControl(SHIFTKEY_THROUGH)
  ,outputLog(false)
{
  Q_UNUSED(outputLog);

  // set keyboard type
  setKeytopType(type);

#ifdef USE_KEYLAYOUTFILE
#if 0 // for TEST
  std::fstream file;
  file.open("keyEventTable_JP.dat", ios::out | ios::binary | ios::trunc);
  if (file.is_open()){
	file.write((char *)keyEventTable_JP, sizeof(KeyEvent)*TABLE_SIZE_JP);
	file.close();
  }
  file.open("keyEventTable_US.dat", ios::out | ios::binary | ios::trunc);
  if (file.is_open()){
	file.write((char *)keyEventTable_US, sizeof(KeyEvent)*TABLE_SIZE_US);
	file.close();
  }
  KLFHeader header;
  memset(&header, 0, sizeof(header));
  strncpy(header.magic, "KLF", 3);
  header.spec = 1;
  header.size = 3964;
  header.keynum = 139;
  header.softkeynum = 67;
  strncpy(header.name, "Japanese 109", strlen("Japanese 109"));
  strncpy(header.author, "FunFun <fu.aba.dc5@gmail.com>", strlen("FunFun <fu.aba.dc5@gmail.com>"));
  file.open("KLFHeader_JP.dat", ios::out | ios::binary | ios::trunc);
  if (file.is_open()){
	file.write((char *)&header, sizeof(header));
	file.close();
  }
  memset(&header, 0, sizeof(header));
  strncpy(header.magic, "KLF", 3);
  header.spec = 1;
  header.size = 3928;
  header.keynum = 136;
  header.softkeynum = 67;
  strncpy(header.name, "US 101", strlen("US 101"));
  strncpy(header.author, "FunFun <fu.aba.dc5@gmail.com>", strlen("FunFun <fu.aba.dc5@gmail.com>"));
  file.open("KLFHeader_US.dat", ios::out | ios::binary | ios::trunc);
  if (file.is_open()){
	file.write((char *)&header, sizeof(header));
	file.close();
  }
#endif // for TEST
#endif // USE_KEYLAYOUTFILE
}

#ifdef USE_KEYLAYOUTFILE
EventConverter::EventConverter(KeyLayoutFile *klf)
  :
  EventConverter(KEYTOP_TYPE_KLF)
{
  setKeytopType(klf);
}
#endif // USE_KEYLAYOUTFILE

// get keytop type
EventConverter::KEYTOP_TYPE EventConverter::getKeytopType()
{
  return type;
}

// set keyboard type
void EventConverter::setKeytopType(KEYTOP_TYPE type){
  switch(type){
  case KEYTOP_TYPE_JP:
	keyEventTable = keyEventTable_JP;
	tableSize= TABLE_SIZE_JP;
	break;
  case KEYTOP_TYPE_US:
	keyEventTable = keyEventTable_US;
	tableSize= TABLE_SIZE_US;
	break;
  default:
	// NO Change
	return;
	break;
  }

  this->type = type;
}

#ifdef USE_KEYLAYOUTFILE
// set keytop type by key layout file
void EventConverter::setKeytopType(KeyLayoutFile *klf)
{
  // set keyboard type
  setKeytopType(KEYTOP_TYPE_KLF);

  this->klf = klf;

  keyEventTable = klf->keyEventTable;
  tableSize= klf->keynum;
}
#endif // USE_KEYLAYOUTFILE

// get Virtual Keycode
uchar EventConverter::getVKCode(QKeyEvent *keyEvent)
{
  Key key = (Key)keyEvent->key();

#if defined(QTB_DEV_TOUCHPANEL)
  // check and fix key for OEM keys
  if (keyMap.contains(keyEvent->text())){
	key = keyMap[keyEvent->text()];
	//qDebug() << "keyEvent = " << keyEvent;
  }
#endif // defined(QTB_DEV_TOUCHPANEL)

  for(int i = 0; i < tableSize; i++){
	if (keyEventTable[i].key == key){
	  shiftKeyControl = keyEventTable[i].shiftKeyControl;
	  return keyEventTable[i].VK_Code;
	}
  }

  // unknown key
  shiftKeyControl = SHIFTKEY_THROUGH;
  return VK_NONE_00;
}

// get shift key control
EventConverter::ShiftKeyControl EventConverter::getShiftKeyControl()
{
  return shiftKeyControl;
}

// get name
QString EventConverter::getEventConverterName() const
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

// get name of virtual keycode
QString EventConverter::getVKCodeByString(uchar vkcode)
{
  return (QString)stringTableOfVKCode[(int)vkcode];
}

// for DEBUG
#if QTB_DEBUG
void EventConverter::printKeyEvent(KeyEvent *keyEvent)
{
  std::cout << "key             : " << keyEvent->key << std::endl;
  std::cout << "VK_Code         : " << stringTableOfVKCode[(int)(keyEvent->VK_Code)] << std::endl;
  std::cout << "shiftKeyControl : " << keyEvent->shiftKeyControl << std::endl << std::flush;
}
#endif // QTB_DEBUG

} // end of namespace qtbrynhildr
