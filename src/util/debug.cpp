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
	  std::cout << "==== check 1 ====" << std::endl << std::flush;
  }
  if (sizeof(COM_DATA) != 256){
	std::cout << "sizeof(COM_DATA) != 256 : size = " << sizeof(COM_DATA) << " : ng" << std::endl << std::flush;
	result = false;
  }
  else {
	if (outputLog)
	  std::cout << "sizeof(COM_DATA) == 256 : ok" << std::endl << std::flush;
  }

  // check 2
  if (outputLog){
	std::cout << "==== check 2 ====" << std::endl << std::flush;
	std::cout << "sizeof(int) = " << sizeof(int) << std::endl << std::flush;
	std::cout << "sizeof(long) = " << sizeof(long) << std::endl << std::flush;
	std::cout << "sizeof(long long) = " << sizeof(long long) << std::endl << std::flush;
  }
#if __x86_64__ || __LP64__
  if (sizeof(int) != 4){
	std::cout << "sizeof(int) != 4" << std::endl << std::flush;
	result = false;
  }
  else {
	if (outputLog)
	  std::cout << "sizeof(int) == 4 : ok" << std::endl << std::flush;
  }
#else // __x86_64__ || __LP64__
  if (sizeof(long) != 4){
	std::cout << "sizeof(long) != 4" << std::endl << std::flush;
	result = false;
  }
  else {
	if (outputLog)
	  std::cout << "sizeof(long) == 4 : ok" << std::endl << std::flush;
  }
#endif // __x86_64__ || __LP64__
  if (sizeof(long long) != 8){
	std::cout << "sizeof(long lolng) != 8" << std::endl << std::flush;
	result = false;
  }
  else {
	if (outputLog)
	  std::cout << "sizeof(long long) == 8 : ok" << std::endl << std::flush;
  }

  // check 3
  if (outputLog)
	std::cout << "==== check 3 ====" << std::endl << std::flush;
  size_t offset;
  offset = offsetof(COM_DATA, dummy1);
  if (outputLog)
	std::cout << "dummy1 offset = " << offset;
  if (offset == 2){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy2);
  if (outputLog)
	std::cout << "dummy2 offset = " << offset;
  if (offset == 5){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy3);
  if (outputLog)
	std::cout << "dummy3 offset = " << offset;
  if (offset == 61){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy4);
  if (outputLog)
	std::cout << "dummy4 offset = " << offset;
#if QTB_EXTRA_BUTTON_SUPPORT
  if (offset == 65){
#else // QTB_EXTRA_BUTTON_SUPPORT
  if (offset == 78){
#endif // QTB_EXTRA_BUTTON_SUPPORT
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy6);
  if (outputLog)
	std::cout << "dummy6 offset = " << offset;
  if (offset == 86){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy16);
  if (outputLog)
	std::cout << "dummy16 offset = " << offset;
  if (offset == 89){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy7);
  if (outputLog)
	std::cout << "dummy7 offset = " << offset;
  if (offset == 93){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy8);
  if (outputLog)
	std::cout << "dummy8 offset = " << offset;
  if (offset == 96){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy9);
  if (outputLog)
	std::cout << "dummy9 offset = " << offset;
  if (offset == 141){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy10);
  if (outputLog)
	std::cout << "dummy10 offset = " << offset;
  if (offset == 145){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy11);
  if (outputLog)
	std::cout << "dummy11 offset = " << offset;
  if (offset == 193){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy17);
  if (outputLog)
	std::cout << "dummy17 offset = " << offset;
  if (offset == 197){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy12);
  if (outputLog)
	std::cout << "dummy12 offset = " << offset;
  if (offset == 224){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy13);
  if (outputLog)
	std::cout << "dummy13 offset = " << offset;
  if (offset == 229){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy14);
  if (outputLog)
	std::cout << "dummy14 offset = " << offset;
  if (offset == 233){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  offset = offsetof(COM_DATA, dummy15);
  if (outputLog)
	std::cout << "dummy15 offset = " << offset;
  if (offset == 237){
	if (outputLog)
	  std::cout << " : ok" << std::endl;
  }
  else {
	if (outputLog)
	  std::cout << " : ng" << std::endl;
	result = false;
  }
  std::cout << std::flush;

  return result;
}

} // end of namespace qtbrynhildr
