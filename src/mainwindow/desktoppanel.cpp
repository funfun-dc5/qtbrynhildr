// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if defined(QTB_NET_UNIX)
#include <unistd.h>
#endif // defined(QTB_NET_UNIX)

// #if defined(Q_OS_WIN)
// #include <windows.h>
// #endif // defined(Q_OS_WIN)

// Qt Header
#include <QDateTime>
#include <QKeyEvent>
#include <QMimeData>
#include <QPoint>
#include <QSize>
#if defined(QTB_DEV_TOUCHPANEL)
#include <QTouchEvent>
#endif // defined(QTB_DEV_TOUCHPANEL)
#include <QList>
#include <QUrl>
#if 1 // for qDebug()
#include <QtCore>
#endif

// Local Header
#include "config.h"
#if defined(QTB_NET_UNIX)
#include "dialog/desktop_scaling_dialog.h"
#endif // defined(QTB_NET_UNIX)
#include "mainwindow/desktoppanel.h"
#include "parameters.h"
#include "qtbrynhildr.h"

namespace qtbrynhildr {

// constructor
DesktopPanel::DesktopPanel(QtBrynhildr *qtbrynhildr)
  :qtbrynhildr(qtbrynhildr)
  ,settings(qtbrynhildr->getSettings())
  ,eventConverter(0)
  ,onShiftKey(false)
  ,onFullScreen(false)
  ,drawMarkerCounter(0)
  ,previous_KEYCODE_FLG(KEYCODE_FLG_KEYUP)
  ,widthMargin(0)
  ,heightMargin(0)
  ,keyboardLogFile(0)
  ,keyboardLogFileStream(0)
  // for DEBUG
  ,outputLogForKeyboard(false)
  ,outputLogForMouse(false)
  ,outputLog(false)
{
  // create keyboard buffer
  keyBuffer = new KeyBuffer(QTB_KEYBOARD_BUFFER_SIZE);

  // create mouse buffer
  mouseBuffer = new MouseBuffer(QTB_MOUSE_BUFFER_SIZE);

  // open keyboard log file
  openKeyboardLogFile(settings->getKeyboardLogFile());
}

// destructor
DesktopPanel::~DesktopPanel()
{
  // close keyboard log file
  closeKeyboardLogFile();

  // delete objects
  if (keyBuffer != 0){
	delete keyBuffer;
	keyBuffer = 0;
  }
  if (mouseBuffer != 0){
	delete mouseBuffer;
	mouseBuffer = 0;
  }
}

// set event converter
void DesktopPanel::setEventConverter(EventConverter *eventConverter)
{
  this->eventConverter = eventConverter;
}

// get keyboard buffer
KeyBuffer *DesktopPanel::getKeyBuffer() const
{
  return keyBuffer;
}

// get mouse buffer
MouseBuffer *DesktopPanel::getMouseBuffer() const
{
  return mouseBuffer;
}

// set mouse position
void DesktopPanel::setMousePos(POS x, POS y)
{
  currentMousePos.setX(x);
  currentMousePos.setY(y);
}

// reflesh desktop window
void DesktopPanel::refreshDesktop(QImage &image)
{
  // return if not initialized
  if (image.isNull()){
	return;
  }
  if (!settings->getConnected()){
	return;
  }

  // copy image
  this->image = image;
  // save size
  currentSize = image.size();

  // resize window
  if (currentSize != previousSize){
#if 0 // for DEBUG
	cout << "resize..." << endl; // for DEBUG
	cout << "image.size().width()  = " << image.size().width() << endl; // for DEBUG
	cout << "image.size().height() = " << image.size().height() << endl << flush; // for DEBUG
#endif // for DEBUG
	previousSize = currentSize;

	// resize main window
	resizeDesktop(currentSize.width(), currentSize.height());

	resizeWindow();
  }
  else if (settings->getDesktop()->isChangedCurrentScreen()) {
	// if current screen is changed.
	// refresh
	resizeWindow();
  }
  else {
	// update image
	updateDesktop();
  }
}

// resize window
void DesktopPanel::resizeWindow()
{
  // check size
  if (!currentSize.isValid()){
	// size is null value
	// Nothing to do
	return;
  }

  if (settings->getOnWindowSizeFixed()){
	// Nothing to do
	return;
  }

  // resize if NOT full screen
  if (QTB_FIXED_MAINWINDOW_SIZE){
	if (!onFullScreen){
	  if (!(qtbrynhildr->isMaximized() || qtbrynhildr->isMinimized())){
		int width = currentSize.width();
		int height = currentSize.height() + qtbrynhildr->getHeightOfMenuBar() + qtbrynhildr->getHeightOfStatusBar();
#if !QTB_NEW_DESKTOPWINDOW
		// correct
		width  += widthMargin;
		height += heightMargin;
#endif // !QTB_NEW_DESKTOPWINDOW

		QSize screenSize = settings->getCurrentScreenSize();
		if (width > screenSize.width()){
		  width = screenSize.width();
		}
		if (height > screenSize.height()){
		  height = screenSize.height();
		}

#if !QTB_NEW_DESKTOPWINDOW
		// resize
		qtbrynhildr->resize(width, height);
#endif // !QTB_NEW_DESKTOPWINDOW

		// update image
		updateDesktop();
	  }
	}
  }
}

// clear desktop window
void DesktopPanel::clearDesktop()
{
  //  cout << "clearDesktop()" << endl << flush;
  if (!image.isNull()){
	image.fill(QTB_DESKTOP_BACKGROUND_COLOR);
	updateDesktop();
  }
}

// get size
QSize DesktopPanel::getSize() const
{
  return currentSize;
}

// get window size
QSize DesktopPanel::getWindowSize() const
{
  QSize windowSize = qtbrynhildr->size();
  QSize diffSize =
	QSize(widthMargin,
		  qtbrynhildr->getHeightOfMenuBar() + qtbrynhildr->getHeightOfStatusBar() + heightMargin);

  windowSize -= diffSize;

  return windowSize;
}

// set full screen flag
void DesktopPanel::setOnFullScreen(bool onFullScreen)
{
  if (QTB_DESKTOP_FULL_SCREEN){
	this->onFullScreen = onFullScreen;
  }
}

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
// mouse move
void DesktopPanel::mouseMove(QPoint mousePos, bool marker)
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	currentMousePos = mousePos;
	MOUSE_POS pos;
	pos.x = currentMousePos.x();
	pos.y = currentMousePos.y();
	mouseBuffer->setPos(pos);
	//qtbrynhildr->moveTopOfSoftwareKeyboard(pos.y); // for TEST
#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
	if (image.rect().contains(currentMousePos)){
	  // set cursor point color to control thread
	  qtbrynhildr->setCursorPointColor(image.pixel(currentMousePos));
	}
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
	// marker for mouse cursor
	if (settings->getOnShowMouseCursorMarker() && marker){
	  setDrawMarkerCounter(10);
	}
  }
}

// mouse move relatively
void DesktopPanel::mouseMoveRelatively(QPoint mousePos, bool marker)
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	currentMousePos += mousePos;
	MOUSE_POS pos;
	pos.x = currentMousePos.x();
	pos.y = currentMousePos.y();
	mouseBuffer->setPos(pos);
	//qtbrynhildr->moveTopOfSoftwareKeyboard(pos.y); // for TEST
#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
	if (image.rect().contains(currentMousePos)){
	  // set cursor point color to control thread
	  qtbrynhildr->setCursorPointColor(image.pixel(currentMousePos));
	}
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
	// marker for mouse cursor
	if (settings->getOnShowMouseCursorMarker() && marker){
	  setDrawMarkerCounter(10);
	}
  }
}
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

// event handler

#if defined(QTB_DEV_TOUCHPANEL)
// event
bool DesktopPanel::event(QEvent *event)
{
  switch(event->type()){
  case QEvent::TouchBegin:
  case QEvent::TouchUpdate:
  case QEvent::TouchEnd:
	{
	  QTouchEvent *touchEvent = (QTouchEvent *)(event);
	  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
	  if (touchPoints.count() == 1){
		// 1 - finger action
		//  1) tap        : move mouse cursor and push left button
		//  2) double tap : -- ( -> move mouse cursor and push right button)
		//  3) drag       : push right button and move mouse cursor (at origianl scale)
		//                : move viewport (at NOT origianl scale)
		//  4) flick      : push right button and move mouse cursor (at origianl scale)
		//                : move viewport (at NOT origianl scale)
		//  5) long tap   : --
	  }
	  else if (touchPoints.count() == 2){
		// 2 - finger action
		//  1) pinch in/out : scaling
	  }
	}
	//return true;
  default:
	break;
  }

  return false;
}
#endif // defined(QTB_DEV_TOUCHPANEL)

//----------------------------------------------------------------------
// mouse events
//----------------------------------------------------------------------

// print mouse button event
void DesktopPanel::printMouseButtonEvent(QMouseEvent *event)
{
  switch (event->button()){
  case Qt::LeftButton:
	cout << "Left Button    : ";
	break;
  case Qt::RightButton:
	cout << "Right Button   : ";
	break;
  case Qt::MiddleButton:
	cout << "Middle Button  : ";
	break;
  case Qt::ForwardButton:
	cout << "Forward Button : ";
	break;
  case Qt::BackButton:
	cout << "Back Button    : ";
	break;
  default:
	cout << "Unknown Button : ";
	break;
  }

  cout << "(x, y) = (" << event->pos().x() << "," << event->pos().y() << ")" << endl << flush;
}

// set mouse button event
void DesktopPanel::setMouseButtonEvent(QMouseEvent *event, MOUSE_BUTTON value)
{
  switch (event->button()){
  case Qt::LeftButton:
	mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_LEFT, value);
	break;
  case Qt::RightButton:
	mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_RIGHT, value);
	break;
#if QTB_EXTRA_BUTTON_SUPPORT
  case Qt::MiddleButton:
	mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_MIDDLE, value);
	break;
  case Qt::ForwardButton:
	mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_FORWARD, value);
	break;
  case Qt::BackButton:
	mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_BACK, value);
	break;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  default:
	// Not support button
	break;
  }
}

// mouse press event
void DesktopPanel::mousePressEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
	cout << "[DesktopPanel] mousePressEvent: ";
	printMouseButtonEvent(event);
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	if (!settings->getOnShowSoftwareButton()){
	  setMouseButtonEvent(event, MOUSE_BUTTON_DOWN);
	}
  }
}

// mouse release event
void DesktopPanel::mouseReleaseEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
	cout << "[DesktopPanel] mouseReleaseEvent: ";
	printMouseButtonEvent(event);
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){

	// check for filedrop
	if (event->button() == Qt::LeftButton){
	  QRect window = QRect(0, 0, currentSize.width(), currentSize.height());
	  if (!window.contains(event->pos(), false)){
		//		cout << "FileDrop!" << endl << flush;
		mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_FILEDROP, MOUSE_BUTTON_UP);
		return;
	  }
	}

	if (!settings->getOnShowSoftwareButton()){
	  setMouseButtonEvent(event, MOUSE_BUTTON_UP);
	}
  }
}

// mouse double click event
void DesktopPanel::mouseDoubleClickEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
	cout << "[DesktopPanel] mouseDoubleClickEvent: ";
	printMouseButtonEvent(event);
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	if (!settings->getOnShowSoftwareButton()){
#if defined(QTB_DEV_DESKTOP)
  setMouseButtonEvent(event, MOUSE_BUTTON_DOWN);
#else // defined(QTB_DEV_DESKTOP)
  setMouseButtonEvent(event, MOUSE_BUTTON_DBLCLK);
#endif // defined(QTB_DEV_DESKTOP)
	}
  }
}

// mouse wheel event
void DesktopPanel::wheelEvent(QWheelEvent *event)
{
  MOUSE_WHEEL mouseWheel = 0;

  if(event->source() == Qt::MouseEventNotSynthesized){
	//cout << "[WheelEvent] source() : " << event->source() << endl << flush;
	QPoint degrees = event->angleDelta() / 8;
	mouseWheel = degrees.y();

	// for DEBUG
	if (outputLogForMouse){
	  int ticks = mouseWheel/15;
	  cout << "[DesktopPanel] wheelEvent(degrees): " << mouseWheel << " (ticks = " << ticks << ")" << endl << flush; // for DEBUG
	}
  }
  else if (event->source() == Qt::MouseEventSynthesizedBySystem){
	//cout << "[WheelEvent] source() : " << event->source() << endl << flush;
	QPoint pixels = event->pixelDelta();
	mouseWheel = pixels.y();

	// for DEBUG
	if (outputLogForMouse){
	  cout << "[DesktopPanel] wheelEvent(pixels): " << (int)mouseWheel << endl << flush; // for DEBUG
	}

	// adjust
	mouseWheel *= settings->getDesktopScalingFactor();
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	if (!settings->getOnShowSoftwareButton()){
	  // put into mouse wheel buffer
	  if (mouseWheel != 0)
		mouseBuffer->putWheel(mouseWheel);
	}
  }
}

// mouse move event
void DesktopPanel::mouseMoveEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
    cout << "[DesktopPanel] mouseMoveEvent: (x, y) = (" <<
	  event->pos().x() << "," << event->pos().y() << ")" << endl << flush; // for DEBUG
  }

  // move mouse cursor
  moveMouseCursor(event, true);
}

// move mouse cursor
void DesktopPanel::moveMouseCursor(QMouseEvent *event, bool marker)
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	currentMousePos = event->pos();
	MOUSE_POS pos;
	pos.x = currentMousePos.x();
	pos.y = currentMousePos.y();
	mouseBuffer->setPos(pos);
	//qtbrynhildr->moveTopOfSoftwareKeyboard(pos.y); // for TEST
#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
	if (image.rect().contains(currentMousePos)){
	  // set cursor point color to control thread
	  qtbrynhildr->setCursorPointColor(image.pixel(currentMousePos));
	}
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
	// marker for mouse cursor
	if (settings->getOnShowMouseCursorMarker() && marker){
	  setDrawMarkerCounter(10);
	}
  }
}

//----------------------------------------------------------------------
// key events
//----------------------------------------------------------------------

// key press event
void DesktopPanel::keyPressEvent(QKeyEvent *event)
{
  // check event converter
  if (eventConverter == 0){
	// Nothing to do
	return;
  }

  // for Menu Key support
  Qt::Key key = (Qt::Key)event->key();
  if (key == Qt::Key_Menu){
	// right button down and up
	mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_DOWN);
	mouseBuffer->putButton(MouseBuffer::MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_UP);
	return;
  }

  // get VK_Code from Key_*
  uchar VK_Code = eventConverter->getVKCode(event);
  if (outputLogForKeyboard){
	qDebug() << "[DesktopPanel]" << eventConverter->getEventConverterName() << // for DEBUG
	  ": Press   : VK_Code =" << eventConverter->getVKCodeByString(VK_Code) <<
	  ":" << QTextStream_hex << VK_Code;
  }

  // on Scroll Mode
  if (settings->getOnScrollMode()){
	if (scrollArea(VK_Code, true)){
	  return;
	}
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	// exit full screen
	if (onFullScreen && VK_Code == VK_ESCAPE){
	  qtbrynhildr->exitFullScreen();
	  return;
	}
	// check shift key status
	if (!onShiftKey && eventConverter->getShiftKeyControl() == EventConverter::SHIFTKEY_NEED){
	  // need shift key
	  keyBuffer->put(VK_SHIFT, KEYCODE_FLG_KEYDOWN);
	  // output Keyboard Log
	  if (settings->getOutputKeyboardLog()){
		(*keyboardLogFileStream) << "Press   - Qt : " << (Qt::Key)event->key()
								 << ", Windows: " << eventConverter->getVKCodeByString(VK_SHIFT)
								 << " => Sent" << QTextStream_endl << QTextStream_flush;
	  }
	}

	// insert into KeyBuffer
	keyBuffer->put(VK_Code, KEYCODE_FLG_KEYDOWN);
	// output Keyboard Log
	if (settings->getOutputKeyboardLog()){
	  (*keyboardLogFileStream) << "Press   - Qt : " << (Qt::Key)event->key()
							   << ", Windows: " << eventConverter->getVKCodeByString(VK_Code)
							   << " => Sent" << QTextStream_endl << QTextStream_flush;
	}

	// set shift key status
	if (VK_Code == VK_SHIFT){
	  onShiftKey = true;
	}

	// set previous KEYCODE_FLG
	previous_KEYCODE_FLG = KEYCODE_FLG_KEYDOWN;
  }
}

// key release event
void DesktopPanel::keyReleaseEvent(QKeyEvent *event)
{
  // check event converter
  if (eventConverter == 0){
	// Nothing to do
	return;
  }

  // for Menu Key support
  Qt::Key key = (Qt::Key)event->key();
  if (key == Qt::Key_Menu){
	// Nothing to do
	return;
  }

  // get VK_Code from Key_*
  uchar VK_Code = eventConverter->getVKCode(event);
  if (outputLogForKeyboard){
	qDebug() << "[DesktopPanel]" << eventConverter->getEventConverterName() << // for DEBUG
	  ": Release : VK_Code =" << eventConverter->getVKCodeByString(VK_Code) <<
	  ":" << QTextStream_hex << VK_Code;
  }

  // output Keyboard Log
  if (settings->getOutputKeyboardLog()){
	(*keyboardLogFileStream) << "Release - Qt : " << (Qt::Key)event->key()
							 << ", Windows: " << eventConverter->getVKCodeByString(VK_Code)
							 << " => Sent" << QTextStream_endl << QTextStream_flush;
  }

  // on Scroll Mode
  if (settings->getOnScrollMode()){
	if (scrollArea(VK_Code, false)){
	  return;
	}
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	// check previous KEYCODE_FLG
	if (previous_KEYCODE_FLG == KEYCODE_FLG_KEYUP){
	  // check VK_TAB and VK_SPACE(Eisu and Kana)
	  switch(VK_Code){
#if defined(Q_OS_OSX)
	  case VK_SPACE:
		VK_Code = VK_KANJI;
		// Fall Through
#endif // defined(Q_OS_OSX)
	  case VK_TAB:
		keyBuffer->put(VK_Code, KEYCODE_FLG_KEYDOWN);
		break;
	  default:
		// Nothing to do
		break;
	  }
	}
	// insert into KeyBuffer
	keyBuffer->put(VK_Code, KEYCODE_FLG_KEYUP);

	// set shift key status
	if (VK_Code == VK_SHIFT){
	  onShiftKey = false;
	}

	// check shift key status
	if (!onShiftKey && eventConverter->getShiftKeyControl() == EventConverter::SHIFTKEY_NEED){
	  // need shift key
	  keyBuffer->put(VK_SHIFT, KEYCODE_FLG_KEYUP);
	  // output Keyboard Log
	  if (settings->getOutputKeyboardLog()){
		(*keyboardLogFileStream) << "Release - Qt : " << (Qt::Key)event->key()
								 << ", Windows: " << eventConverter->getVKCodeByString(VK_SHIFT)
								 << " => Sent" << QTextStream_endl << QTextStream_flush;
	  }
	}

	// set previous KEYCODE_FLG
	previous_KEYCODE_FLG = KEYCODE_FLG_KEYUP;
  }
}

#if QTB_DRAG_AND_DROP_SUPPORT
//----------------------------------------------------------------------
// drag and drop events
//----------------------------------------------------------------------
// drag enter event
void DesktopPanel::dragEnterEvent(QDragEnterEvent *event)
{
  if (!settings->getOnTransferFileSupport() ||
	  !settings->getOnTransferFileSupportByDragAndDrop()){
	return;
  }
  if (event->mimeData()->hasFormat("text/uri-list")){
	event->acceptProposedAction();
  }
}

// drop event
void DesktopPanel::dropEvent(QDropEvent *event)
{
  if (!settings->getOnTransferFileSupport() ||
	  !settings->getOnTransferFileSupportByDragAndDrop()){
	return;
  }

  QList<QUrl> urls = event->mimeData()->urls();
  if (urls.isEmpty()){
	// Nothing to do
	return;
  }

  // get files
  QStringList fileNames;
  for(QList<QUrl>::iterator i = urls.begin(); i != urls.end(); i++){
	fileNames.append((*i).toLocalFile());
  }
  if (fileNames.isEmpty()){
	// NOT Found file name
	return;
  }

  // send files
  settings->setSendFileNames(fileNames);
  settings->setSendFileCount(fileNames.count());
}
#endif // QTB_DRAG_AND_DROP_SUPPORT

#if defined(Q_OS_WIN)
// native event filter
bool DesktopPanel::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
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
		//	  case VK_TAB:
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
		//	  case VK_TAB:
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
#endif // for TEST

// scroll area
bool DesktopPanel::scrollArea(uchar VK_Code, bool onKeyPress)
{
  bool result = false;

  if (!onKeyPress){
	// Nothing to do if key release
	return false;
  }

  // change offset
  if (VK_Code == VK_UP){
	POS offsetY = settings->getDesktopOffsetY();
	offsetY += 100;
	if (offsetY < 0) offsetY = 0;
	settings->setDesktopOffsetY(offsetY);
	//	cout << "Scroll Up : " << settings->getDesktopOffsetY() << endl << flush;
	result = true;
  }
  else if (VK_Code == VK_DOWN){
	POS offsetY = settings->getDesktopOffsetY();
	offsetY -= 100;
	if (offsetY > settings->getDesktopHeight()){
	  offsetY = settings->getDesktopHeight();
	}
	settings->setDesktopOffsetY(offsetY);
	//	cout << "Scroll Down : " << settings->getDesktopOffsetY() << endl << flush;
	result = true;
  }
  else if (VK_Code == VK_LEFT){
	POS offsetX = settings->getDesktopOffsetX();
	offsetX += 100;
	if (offsetX < 0) offsetX = 0;
	settings->setDesktopOffsetX(offsetX);
	//	cout << "Scroll Left : " << settings->getDesktopOffsetX() << endl << flush;
	result = true;
  }
  else if (VK_Code == VK_RIGHT){
	POS offsetX = settings->getDesktopOffsetX();
	offsetX -= 100;
	if (offsetX > settings->getDesktopWidth()){
	  offsetX = settings->getDesktopWidth();
	}
	settings->setDesktopOffsetX(offsetX);
	//	cout << "Scroll Right : " << settings->getDesktopOffsetX() << endl << flush;
	result = true;
  }

  return result;
}

//----------------------------------------------------------------------
// keyboard log file
//----------------------------------------------------------------------
// open keyboard log file
bool DesktopPanel::openKeyboardLogFile(QString filename)
{
  if (keyboardLogFile != 0)
	return false;

  keyboardLogFile = new QFile(filename);

  // check
  if (!keyboardLogFile->open(QFile::WriteOnly | QFile::Append)) {
	return false;
  }

  // create QTextStrem
  keyboardLogFileStream = new QTextStream(keyboardLogFile);
  keyboardLogFileStream->setCodec("UTF-8");

  return true;
}

// close keyboard log file
bool DesktopPanel::closeKeyboardLogFile()
{
  if (keyboardLogFile == 0)
	return false;

  if (keyboardLogFileStream == 0)
	return false;

  // flush
  keyboardLogFileStream->flush();

  // close
  if (keyboardLogFile->isOpen()){
	keyboardLogFile->close();
  }

  // delete object
  delete keyboardLogFileStream;
  keyboardLogFileStream = 0;

  delete keyboardLogFile;
  keyboardLogFile = 0;

  return true;
}

} // end of namespace qtbrynhildr
