// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <fstream> // for TEST

// Qt Header
#include <QString>

// Local Header

#include "eventconverter.h"
#include "keylayout/keylayoutfile.h"

namespace qtbrynhildr {

// constructor
EventConverter::EventConverter()
  :
  EventConverter(KEYTOP_TYPE_JP)
{
}

EventConverter::EventConverter(KEYTOP_TYPE type)
  :
  shiftKeyControl(SHIFTKEY_THROUGH),
  outputLog(false)
{
  // set keyboard type
  setKeytopType(type);

#if 0 // for TEST
  fstream file;
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
}

EventConverter::EventConverter(KeyLayoutFile *klf)
  :
  EventConverter(KEYTOP_TYPE_KLF)
{
  keyEventTable = klf->keyEventTable;
  tableSize= klf->keynum;
}

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

// get Virtual Keycode
uchar EventConverter::getVKCode(QKeyEvent *keyEvent)
{
  Key key = (Key)keyEvent->key();

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
void EventConverter::printKeyEvent(KeyEvent *keyEvent)
{
}

} // end of namespace qtbrynhildr
