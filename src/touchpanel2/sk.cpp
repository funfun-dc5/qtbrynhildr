// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <iostream> // for TEST

// Qt Header

// Local Header
#include "qtbrynhildr.h"
#include "sk.h"

using namespace std; // for TEST

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SK::SK(KeyBuffer *keyBuffer, QtBrynhildr *qtbrynhildr, QWidget *parent)
  :
  SoftwareKeyboard(parent),
  keyBuffer(keyBuffer),
  qtbrynhildr(qtbrynhildr),
  settings(qtbrynhildr->getSettings()),
#if QTB_NEW_DESKTOPWINDOW
  graphicsView(qtbrynhildr->getGraphicsView()),
#endif // QTB_NEW_DESKTOPWINDOW
  // for DEBUG
  outputLog(false)
{
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// pressed key
void SK::pressedKey(ID_KEY id)
{
  SoftwareKeyboard::pressedKey(id);
}

// released key
void SK::releasedKey(ID_KEY id)
{
  SoftwareKeyboard::releasedKey(id);
  if (id == ID_KEY_Fn){
	qtbrynhildr->toggleSoftwareKeyboard();
  }
}

// key down
void SK::keyDown(uchar key)
{
  SoftwareKeyboard::keyDown(key);

  keyBuffer->put(key, KEYCODE_FLG_KEYDOWN);

  if (outputLog)
	cout << "SK:DOWN"<< endl << flush;
}
  
// key up
void SK::keyUp(uchar key)
{
  SoftwareKeyboard::keyUp(key);

  keyBuffer->put(key, KEYCODE_FLG_KEYUP);

  if (outputLog)
	cout << "SK:UP"<< endl << flush;
}

#if QTB_NEW_DESKTOPWINDOW
// mouse event
void SK::mousePressEvent(QMouseEvent *event)
{
  SoftwareKeyboard::mousePressEvent(event);
  if (!isOnButton()){
	QPoint pos = event->pos() + this->pos();
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											pos,
											event->button(),
											event->buttons(),
											event->modifiers());
	graphicsView->mousePressEventForSP(newEvent);
  }
}

void SK::mouseReleaseEvent(QMouseEvent *event)
{
  SoftwareKeyboard::mouseReleaseEvent(event);
  if (!isOnButton()){
	QPoint pos = event->pos() + this->pos();
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											pos,
											event->button(),
											event->buttons(),
											event->modifiers());
	graphicsView->mouseReleaseEventForSP(newEvent);
  }
}

void SK::mouseMoveEvent(QMouseEvent *event)
{
  SoftwareKeyboard::mouseMoveEvent(event);
  if (!isOnButton()){
	QPoint pos = event->pos() + this->pos();
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											pos,
											event->button(),
											event->buttons(),
											event->modifiers());
	graphicsView->mouseMoveEventForSP(newEvent);
  }
}
#endif // QTB_NEW_DESKTOPWINDOW

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr
