// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstddef>
#include <iostream>

// Local Header
#include "common/protocols.h"

namespace qtbrynhildr {

// check basic parameter
bool checkProtocolHeader(bool outputLog)
{
  bool result = true;

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
	cout << "sizeof(long long) = " << sizeof(long long) << endl << flush;
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
  if (sizeof(long long) != 8){
	cout << "sizeof(long lolng) != 8" << endl << flush;
	result = false;
  }
  else {
	if (outputLog)
	  cout << "sizeof(long long) == 8 : ok" << endl << flush;
  }

  // check 3
  if (outputLog)
	cout << "==== check 3 ====" << endl << flush;
  size_t offset;
  offset = offsetof(COM_DATA, dummy1);
  if (outputLog)
	cout << "dummy1 offset = " << offset;
  if (offset == 2){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy2);
  if (outputLog)
	cout << "dummy2 offset = " << offset;
  if (offset == 5){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy3);
  if (outputLog)
	cout << "dummy3 offset = " << offset;
  if (offset == 61){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy4);
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
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy6);
  if (outputLog)
	cout << "dummy6 offset = " << offset;
  if (offset == 86){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy16);
  if (outputLog)
	cout << "dummy16 offset = " << offset;
  if (offset == 89){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy7);
  if (outputLog)
	cout << "dummy7 offset = " << offset;
  if (offset == 93){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy8);
  if (outputLog)
	cout << "dummy8 offset = " << offset;
  if (offset == 96){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy9);
  if (outputLog)
	cout << "dummy9 offset = " << offset;
  if (offset == 141){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy10);
  if (outputLog)
	cout << "dummy10 offset = " << offset;
  if (offset == 145){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy11);
  if (outputLog)
	cout << "dummy11 offset = " << offset;
  if (offset == 193){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy17);
  if (outputLog)
	cout << "dummy17 offset = " << offset;
  if (offset == 197){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy12);
  if (outputLog)
	cout << "dummy12 offset = " << offset;
  if (offset == 224){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy13);
  if (outputLog)
	cout << "dummy13 offset = " << offset;
  if (offset == 229){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy14);
  if (outputLog)
	cout << "dummy14 offset = " << offset;
  if (offset == 233){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy15);
  if (outputLog)
	cout << "dummy15 offset = " << offset;
  if (offset == 237){
	if (outputLog)
	  cout << " : ok" << endl;
  }
  else {
	if (outputLog)
	  cout << " : ng" << endl;
	result = false;
  }
  cout << flush;

  return result;
}

} // end of namespace qtbrynhildr
