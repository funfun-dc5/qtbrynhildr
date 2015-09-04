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
bool do_check()
{
  bool result = true;
  COM_DATA com_data;

  // check 1
  cout << "==== check 1 ====" << endl << flush;
  if (sizeof(COM_DATA) != 256){
	cout << "sizeof(COM_DATA) != 256 : size = " << sizeof(COM_DATA) << " : ng" << endl << flush;
	result = false;
  }
  else {
	cout << "sizeof(COM_DATA) == 256 : ok" << endl << flush;
  }

  // check 2
  cout << "==== check 2 ====" << endl << flush;
  cout << "sizeof(int) = " << sizeof(int) << endl << flush;
  cout << "sizeof(long) = " << sizeof(long) << endl << flush;
  if (sizeof(long) != 4){
	cout << "sizeof(long) != 4" << endl << flush;
  }
  else {
	cout << "sizeof(long) == 4 : ok" << endl << flush;
  }

  // check 3
  cout << "==== check 3 ====" << endl << flush;
  int offset;
  offset = (char*)&(com_data.dummy1[0]) - (char*)&com_data;
  cout << "dummy1 offset = " << offset;
  if (offset == 2){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy2[0]) - (char*)&com_data;
  cout << "dummy2 offset = " << offset;
  if (offset == 12){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy3[0]) - (char*)&com_data;
  cout << "dummy3 offset = " << offset;
  if (offset == 78){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy4[0]) - (char*)&com_data;
  cout << "dummy4 offset = " << offset;
  if (offset == 82){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy5[0]) - (char*)&com_data;
  cout << "dummy5 offset = " << offset;
  if (offset == 86){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy6[0]) - (char*)&com_data;
  cout << "dummy6 offset = " << offset;
  if (offset == 96){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy7[0]) - (char*)&com_data;
  cout << "dummy7 offset = " << offset;
  if (offset == 144){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy8[0]) - (char*)&com_data;
  cout << "dummy8 offset = " << offset;
  if (offset == 192){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy9[0]) - (char*)&com_data;
  cout << "dummy9 offset = " << offset;
  if (offset == 224){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  offset = (char*)&(com_data.dummy10[0]) - (char*)&com_data;
  cout << "dummy10 offset = " << offset;
  if (offset == 236){
	cout << " : ok" << endl;
  }
  else {
	cout << " : ng" << endl;
  }
  cout << flush;

  return result;
}

} // end of namespace qtbrynhildr
