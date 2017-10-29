// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>
#include <QSize>
#include <QtGui>

// Local Header
#include "mainwindow/desktopwindow.h"

namespace qtbrynhildr {

// constructor
DesktopWindow::DesktopWindow(Settings *settings, QtBrynhildr *qtbrynhildr, QWidget *parent)
  :
  QWidget(parent),
  DesktopPanel(settings, qtbrynhildr)
{
  //  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true); // mouse tracking mode on
}

// destructor
DesktopWindow::~DesktopWindow()
{
}

// resize desktop
void DesktopWindow::resizeDesktop(int width, int height)
{
  resize(width, height);
}

// refresh desktop
void DesktopWindow::updateDesktop()
{
  update();
}

// minimum size hint
QSize DesktopWindow::minimumSizeHint() const
{
  return currentSize;
}

// size hint
QSize DesktopWindow::sizeHint() const
{
  return currentSize;
}

// for event handling
#if defined(QTB_DEV_TOUCHPANEL)
  // event
bool DesktopWindow::event(QEvent *event){
  if (DesktopPanel::event(event))
	return true;
  else
	return QWidget::event(event);
}
#endif // defined(QTB_DEV_TOUCHPANEL)

// paint event
void DesktopWindow::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  if (image.isNull()){
	return;
  }

#if 1 // for TEST
  QPainter painter(this);
  painter.drawImage(0, 0, image);
  // draw marker for mouse cursor
  if (drawMarkerCounter > 0){
	int length = drawMarkerCounter*10;
	int x = currentMousePos.x() - length/2;
	int y = currentMousePos.y() - length/2;

	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setPen(QPen(Qt::green, 4));
	painter.drawArc(x, y, length, length, 0*360, 16*360);

	drawMarkerCounter--;
  }
#if 0 // for TEST
  else {
	drawMarkerCounter = 30;
  }
#endif // for TEST
#else // for TEST
  QImage image2(image.size().width(), image.size().height(), QImage::Format_ARGB32_Premultiplied);
  QPainter imagePainter(&image2);
  imagePainter.setRenderHint(QPainter::Antialiasing, true);
  imagePainter.drawImage(0, 0, image);

  QPainter widgetPainter(this);
  widgetPainter.drawImage(0, 0, image2);
#endif // for TEST
}

// widget leave event
void DesktopWindow::leaveEvent(QEvent *event)
{
  Q_UNUSED(event);

  if (settings->getOnClipCursor()){
	QCursor::setPos(mapToGlobal(currentMousePos));
  }
}

// mouse event
void DesktopWindow::mousePressEvent(QMouseEvent *event)
{
  DesktopPanel::mousePressEvent(event);
}
void DesktopWindow::mouseReleaseEvent(QMouseEvent *event)
{
  DesktopPanel::mouseReleaseEvent(event);
}
void DesktopWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
  DesktopPanel::mouseDoubleClickEvent(event);
}
void DesktopWindow::mouseMoveEvent(QMouseEvent *event)
{
  DesktopPanel::mouseMoveEvent(event);
}
void DesktopWindow::wheelEvent(QWheelEvent *event)
{
  DesktopPanel::wheelEvent(event);
}

// keyboard event
void DesktopWindow::keyPressEvent(QKeyEvent *event)
{
  DesktopPanel::keyPressEvent(event);
}
void DesktopWindow::keyReleaseEvent(QKeyEvent *event)
{
  DesktopPanel::keyReleaseEvent(event);
}

#if QTB_DRAG_AND_DROP_SUPPORT
// drag and drop
void DesktopWindow::dragEnterEvent(QDragEnterEvent *event)
{
  DesktopPanel::dragEnterEvent(event);
}
void DesktopWindow::dropEvent(QDropEvent *event)
{
  DesktopPanel::dropEvent(event);
}
#endif // QTB_DRAG_AND_DROP_SUPPORT

//----------------------------------------------------------------------
// native event filter
//----------------------------------------------------------------------
#if defined(Q_OS_WIN)
bool DesktopWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
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
