// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// debug.cpp

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Local Header
#include "common/protocols.h"

namespace qtbrynhildr {

// check basic parameter
bool checkProtocolHeader(bool outputLog)
{
  bool result = true;
  COM_DATA com_data;

  // check 1
  if (outputLog){
	  cout << "==== check 1 ====" << endl << flush;
  }
  if (sizeof(COM_DATA) != 256){
	cout << "sizeof(COM_DATA) != 256 : size = " << sizeof(COM_DATA) << " : ng" << endl << flush;
	result = false;
  }
  else {
	if (outputLog)
	  cout << "sizeof(COM_DATA) == 256 : ok" << endl << flush;
  }

  // check 2
  if (outputLog){
	cout << "==== check 2 ====" << endl << flush;
	cout << "sizeof(int) = " << sizeof(int) << endl << flush;
	cout << "sizeof(long) = " << sizeof(long) << endl << flush;
  }
#if __x86_64__ || __LP64__
  if (sizeof(int) != 4){
	cout << "sizeof(int) != 4" << endl << flush;
	result = false;
  }
  else {
	if (outputLog)
	  cout << "sizeof(int) == 4 : ok" << endl << flush;
  }
#else // __x86_64__ || __LP64__
  if (sizeof(long) != 4){
	cout << "sizeof(long) != 4" << endl << flush;
	result = false;
  }
  else {
	if (outputLog)
	  cout << "sizeof(long) == 4 : ok" << endl << flush;
  }
#endif // __x86_64__ || __LP64__

  // check 3
  if (outputLog)
	cout << "==== check 3 ====" << endl << flush;
  int offset;
  offset = (char*)&(com_data.dummy1[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy1 offset = " << offset;
  if (offset == 2){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy2[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy2 offset = " << offset;
  if (offset == 5){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy3[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy3 offset = " << offset;
  if (offset == 12){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy4[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy4 offset = " << offset;
#if QTB_EXTRA_BUTTON_SUPPORT
  if (offset == 65){
#else // QTB_EXTRA_BUTTON_SUPPORT
  if (offset == 78){
#endif // QTB_EXTRA_BUTTON_SUPPORT
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy5[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy5 offset = " << offset;
  if (offset == 82){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy6[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy6 offset = " << offset;
  if (offset == 86){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy7[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy7 offset = " << offset;
  if (offset == 96){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy8[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy8 offset = " << offset;
#if QTB_BRYNHILDR2_SUPPORT
  if (offset == 148){
#else // QTB_BRYNHILDR2_SUPPORT
  if (offset == 144){
#endif // QTB_BRYNHILDR2_SUPPORT
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy9[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy9 offset = " << offset;
  if (offset == 193){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy10[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy10 offset = " << offset;
  if (offset == 224){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy11[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy11 offset = " << offset;
  if (offset == 229){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  offset = (char*)&(com_data.dummy12[0]) - (char*)&com_data;
  if (outputLog)
	cout << "dummy12 offset = " << offset;
  if (offset == 236){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
	result = false;
  }
  cout << flush;

  return result;
}

} // end of namespace qtbrynhildr
