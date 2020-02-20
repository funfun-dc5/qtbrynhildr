// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "mousebuffer.h"

namespace qtbrynhildr {

// constructor
MouseBuffer::MouseBuffer(int size)
  :wheel(0)
  ,enabled(true)
  // for DEBUG
  //,outputLog(false)
{
  // initialize button queues
  for (int i = 0; i < MOUSE_BUTTON_NUM; i++){
	buttons[i] = new MouseButton(size);
  }

  // initialize wheel queue
  wheel = new MouseWheel(size);

  // initialize mouse position
  pos.x = 0;
  pos.y = 0;
}

// destructor
MouseBuffer::~MouseBuffer()
{
  // delete objects
  // button queues
  for (int i = 0; i < MOUSE_BUTTON_NUM; i++){
	if (buttons[i] != 0){
	  delete buttons[i];
	  buttons[i] = 0;
	}
  }
  // wheel queue
  if (wheel != 0){
	delete wheel;
	wheel = 0;
  }
}

// clear buffer
void MouseBuffer::clear()
{
  // reset
  for (int i = 0; i < MOUSE_BUTTON_NUM; i++){
	buttons[i]->clear();
  }
  wheel->clear();
}

// for button queue
int MouseBuffer::putButton(MOUSE_BUTTON_ID button, MOUSE_BUTTON value)
{
  if (!enabled){
	// Nothig to do
	return 0;
  }

  MouseButton *mouseButton = buttons[button];
  return mouseButton->putButton(value);
}

MOUSE_BUTTON MouseBuffer::getButton(MOUSE_BUTTON_ID button)
{
  if (!enabled){
	// Nothig to do
	return 0;
  }

  MouseButton *mouseButton = buttons[button];
  return mouseButton->getButton();
}

// for wheel queue
int MouseBuffer::putWheel(MOUSE_WHEEL value)
{
  if (!enabled){
	// Nothig to do
	return 0;
  }

  return wheel->putWheel(value);
}

MOUSE_WHEEL MouseBuffer::getWheel()
{
  if (!enabled){
	// Nothig to do
	return 0;
  }

  return wheel->getWheel();
}

// set mouse position
void MouseBuffer::setPos(MOUSE_POS pos)
{
  if (!enabled){
	// Nothig to do
	return;
  }

  this->pos = pos;
}

// get mouse position
MOUSE_POS MouseBuffer::getPos() const
{
  return pos;
}

} // end of namespace qtbrynhildr
