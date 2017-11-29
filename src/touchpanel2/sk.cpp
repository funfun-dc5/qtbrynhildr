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
  desktopPanel(qtbrynhildr->getDesktopPanel()),
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
	QPoint scenePos = graphicsView->mapToScene(pos).toPoint();
	if (graphicsView->convertToDesktop(scenePos)){
		// need to convert pos
		QMouseEvent *newEvent = new QMouseEvent(event->type(),
												scenePos,
												event->button(),
												event->buttons(),
												event->modifiers());
		desktopPanel->mousePressEvent(newEvent);
	}
  }
}

void SK::mouseReleaseEvent(QMouseEvent *event)
{
  SoftwareKeyboard::mouseReleaseEvent(event);
  if (!isOnButton()){
	QPoint pos = event->pos() + this->pos();
	QPoint scenePos = graphicsView->mapToScene(pos).toPoint();
	if (graphicsView->convertToDesktop(scenePos)){
		// need to convert pos
		QMouseEvent *newEvent = new QMouseEvent(event->type(),
												scenePos,
												event->button(),
												event->buttons(),
												event->modifiers());
		desktopPanel->mouseReleaseEvent(newEvent);
	}
  }
}

void SK::mouseMoveEvent(QMouseEvent *event)
{
  SoftwareKeyboard::mouseMoveEvent(event);
#if 1
  if (!isOnButton()){
	QPoint pos = event->pos() + this->pos();
	QPoint scenePos = graphicsView->mapToScene(pos).toPoint();
	if (graphicsView->convertToDesktop(scenePos)){
		// need to convert pos
		QMouseEvent *newEvent = new QMouseEvent(event->type(),
												scenePos,
												event->button(),
												event->buttons(),
												event->modifiers());
		desktopPanel->mouseMoveEvent(newEvent);
	}
  }
#endif
}
#endif // QTB_NEW_DESKTOPWINDOW

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr
