// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QtGui>

// Local Header
#include "mainwindow/desktoppanelobject.h"

namespace qtbrynhildr {

// constructor
DesktopPanelObject::DesktopPanelObject(Settings *settings, QtBrynhildr *qtbrynhildr, QGraphicsItem *parent)
  :
  QGraphicsObject(parent),
  DesktopPanel(settings, qtbrynhildr)
{
  setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

// destructor
DesktopPanelObject::~DesktopPanelObject()
{
}

// resize desktop
void DesktopPanelObject::resizeDesktop(int width, int height)
{
  Q_UNUSED(width);
  Q_UNUSED(height);

  //  cout << "resizeDesktop()" << endl << flush;
  //  cout << "(width, height) = (" << width << ", " << height << ")" << endl << flush;
}

// refresh desktop
void DesktopPanelObject::updateDesktop()
{
  //  cout << "updateDesktop()" << endl << flush;
  update();
}

QRectF DesktopPanelObject::boundingRect() const
{
  return QRectF(-image.width()/2, -image.height()/2, image.width(), image.height());
}

void DesktopPanelObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  // set half width, half height
  int hwidth = image.width()/2;
  int hheight = image.height()/2;

  // draw image
  painter->drawImage(-hwidth, -hheight, image);

  // draw marker for mouse cursor
  if (drawMarkerCounter > 0){
	int length = drawMarkerCounter*10;
	int x = currentMousePos.x() - length/2 - hwidth;
	int y = currentMousePos.y() - length/2 - hheight;

	painter->setRenderHint(QPainter::Antialiasing, false);
	painter->setPen(QPen(Qt::green, 4));
	painter->drawArc(x, y, length, length, 0*360, 16*360);

	drawMarkerCounter--;
  }
}

//----------------------------------------------------------------------
// native event filter
//----------------------------------------------------------------------
#if defined(Q_OS_WIN)
bool DesktopPanelObject::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
  Q_UNUSED(result);

  if (!(settings->getConnected()) ||
	  !(settings->getOnControl())){
	return false;
  }
  if (eventType == "windows_generic_MSG"){
	MSG *msg = static_cast<MSG *>(message);
	// KEY
	if (settings->getKeyboardType() == KEYBOARD_TYPE_NATIVE){
	  // send All key event in nativeEventFilter
	  if (msg->message == WM_KEYDOWN){
#if 0 // for TEST
		// VK_LXXXX/RXXXX -> VK_XXXX
		switch(msg->wParam){
		case VK_LSHIFT:
		case VK_RSHIFT:
		  msg->wParam = VK_SHIFT;
		  break;
		case VK_LCONTROL:
		case VK_RCONTROL:
		  msg->wParam = VK_CONTROL;
		  break;
		case VK_LMENU:
		case VK_RMENU:
		  msg->wParam = VK_MENU;
		  break;
		}
#endif // for TEST
		keyBuffer->put(msg->wParam, KEYCODE_FLG_KEYDOWN);
		return true;
	  }
	  else if (msg->message == WM_KEYUP){
#if 0 // for TEST
		// VK_LXXXX/RXXXX -> VK_XXXX
		switch(msg->wParam){
		case VK_LSHIFT:
		case VK_RSHIFT:
		  msg->wParam = VK_SHIFT;
		  break;
		case VK_LCONTROL:
		case VK_RCONTROL:
		  msg->wParam = VK_CONTROL;
		  break;
		case VK_LMENU:
		case VK_RMENU:
		  msg->wParam = VK_MENU;
		  break;
		}
#endif // for TEST
		keyBuffer->put(msg->wParam, KEYCODE_FLG_KEYUP);
		return true;
	  }
	  // NOTE: never fall through
	}

	// except for KEYBOARD_TYPE_NATIVE
	if (msg->message == WM_KEYDOWN){
	  switch(msg->wParam){
	  case VK_OEM_AUTO:
	  case VK_OEM_ENLW:
		//		cout << "[DesktopPanel] nativeEventFilter: KEYDOWN: " << msg->wParam << endl; // for DEBUG
		keyBuffer->put(VK_KANJI, KEYCODE_FLG_KEYDOWN);
		return true;
		break;
	  case VK_CONVERT:
	  case VK_NONCONVERT:
	  case VK_OEM_ATTN:
	  case 229:
		//		cout << "[DesktopPanel] nativeEventFilter: KEYDOWN: " << msg->wParam << endl; // for DEBUG
		keyBuffer->put(msg->wParam, KEYCODE_FLG_KEYDOWN);
		return true;
		break;
	  default:
		return false;
		break;
	  }
	}
	else if (msg->message == WM_KEYUP){
	  switch(msg->wParam){
	  case VK_OEM_AUTO:
	  case VK_OEM_ENLW:
		//		cout << "[DesktopPanel] nativeEventFilter: KEYUP: " << msg->wParam << endl; // for DEBUG
		keyBuffer->put(VK_KANJI, KEYCODE_FLG_KEYUP);
		return true;
		break;
	  case VK_CONVERT:
	  case VK_NONCONVERT:
	  case VK_OEM_ATTN:
		//		cout << "[DesktopPanel] nativeEventFilter: KEYUP: " << msg->wParam << endl; // for DEBUG
		keyBuffer->put(msg->wParam, KEYCODE_FLG_KEYUP);
		return true;
		break;
	  default:
		return false;
		break;
	  }
	}
	// SYSKEY
	else if (msg->message == WM_SYSKEYDOWN){
	  switch(msg->wParam){
	  case VK_MENU:
		keyBuffer->put(msg->wParam, KEYCODE_FLG_KEYDOWN);
		return true;
		break;
	  default:
		return false;
		break;
	  }
	}
	else if (msg->message == WM_SYSKEYUP){
	  switch(msg->wParam){
	  case VK_MENU:
		keyBuffer->put(msg->wParam, KEYCODE_FLG_KEYUP);
		return true;
		break;
	  default:
		return false;
		break;
	  }
	}
  }
  return false;
}
#endif // defined(Q_OS_WIN)

#if 0 // for TEST
#if defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD)
// for X11
#include <xcb/xcb.h>

bool DesktopPanel::nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE
{
  if (!(settings->getConnected()) ||
	  !(settings->getOnControl())){
	return false;
  }
  if (eventType == "xcb_generic_event_t"){
	xcb_generic_event_t *ev = static_cast<xcb_generic_event_t *>(message);
  }
  return false;
}
#endif // defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD)
#endif

} // end of namespace qtbrynhildr
