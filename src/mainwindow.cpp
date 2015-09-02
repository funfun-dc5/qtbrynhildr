// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// mainwindow.cpp

// Common Header
#include "common.h"

// System Header
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
#include <unistd.h>
#endif // defined(Q_OS_LINUX) || defined(Q_OS_MAC)

// Qt Header
#include <QKeyEvent>
#include <QPainter>
#include <QPoint>
#include <QSize>

// Local Header
#include "config.h"
#include "mainwindow.h"
#include "parameters.h"
#include "qtbrynhildr.h"
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
#include "desktop_scaling_dialog.h"
#endif // defined(Q_OS_LINUX) || defined(Q_OS_MAC)

namespace qtbrynhildr {

// constructor
MainWindow::MainWindow(Settings *settings, QWidget *parent)
  :
  QWidget(parent),
  settings(settings),
  eventConverter(0),
  // for DEBUG
  outputLog(false),
  outputLogForKeyboard(QTB_DEBUG_KEYBOARD),
  outputLogForMouse(QTB_DEBUG_MOUSE),
  heightOfMenuBar(0),
  heightOfStatusBar(0),
  heightOfMenuBarInHiding(0),
  heightOfStatusBarInHiding(0)
{
  // save parent
  this->parent = parent;

  // setting main window
  setBackgroundRole(QPalette::Dark);
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true); // mouse tracking mode on

  // create keyboard buffer
  keyBuffer = new KeyBuffer(QTB_KEYBOARD_BUFFER_SIZE);

  // create mouse buffer
  mouseBuffer = new MouseBuffer(QTB_MOUSE_BUFFER_SIZE);

  // set parameters
  heightOfMenuBarInHiding = settings->getDesktop()->getHeightOfMenuBarInHiding();
  heightOfStatusBarInHiding = settings->getDesktop()->getHeightOfStatusBarInHiding();
}

// destructor
MainWindow::~MainWindow()
{
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
void MainWindow::setEventConverter(EventConverter *eventConverter)
{
  this->eventConverter = eventConverter;
}

// get keyboard buffer
KeyBuffer *MainWindow::getKeyBuffer() const
{
  return keyBuffer;
}

// get mouse buffer
MouseBuffer *MainWindow::getMouseBuffer() const
{
  return mouseBuffer;
}

// reflesh desktop window
void MainWindow::refreshDesktop(QImage image)
{
  // return if not initialized
  if (image.isNull()){
	return;
  }
  if (!settings->getConnected()){
	return;
  }

  // cut blank area
  if (QTB_CUT_DESKTOP_BLANK_AREA){
	// for offset
	if (settings->getOnCutDesktopBlankArea()){
	  if (settings->getDesktopOffsetX() != 0 || settings->getDesktopOffsetY() != 0){
		image = image.copy(0,0,
						   image.size().width()  - settings->getDesktopOffsetX(),
						   image.size().height() - settings->getDesktopOffsetY());
	  }
	}
  }

  // save size
  size = desktopSize = image.size();

  // scaling image
  if (QTB_DESKTOP_IMAGE_SCALING){
	if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_CLIENT){
	  qreal scalingFactor = getDesktopScalingFactor(size);
	  if (scalingFactor != 1.0){
		// scale
		size = size * scalingFactor;
		image = image.scaled(size, Qt::KeepAspectRatio, settings->getDesktopScaringQuality());
	  }
	  // save scaling factor
	  if (scalingFactor != settings->getDesktopScalingFactor()){
		settings->setDesktopScalingFactor(scalingFactor);
	  }
	}
  }

  // copy QImage
  this->image = image;

  // refresh image
  update();

  // resize window
  if (previousSize != size){
#if 0 // for DEBUG
	cout << "resize..." << endl; // for DEBUG
	cout << "image.size().width()  = " << image.size().width() << endl; // for DEBUG
	cout << "image.size().height() = " << image.size().height() << endl << flush; // for DEBUG
#endif
	previousSize = size;
	// resize main window
	resize(size.width(), size.height());

	// refresh
	refreshDesktop();
  }
}

// reflesh desktop window
void MainWindow::refreshDesktop()
{
  // check size
  if (size.width() < 0 || size.height() < 0){
	// size is null value
	// Nothing to do
	return;
  }

  // set maximum width & height
  int targetWidth = size.width() + settings->getDesktop()->getCorrectWindowWidth();
  int targetHeight = size.height() + getHeightOfMenuBar() + getHeightOfStatusBar() + settings->getDesktop()->getCorrectWindowHeight();
  QSize screenSize = settings->getDesktop()->getCurrentScreen().size();
  if (targetWidth > screenSize.width()){
	targetWidth = screenSize.width();
  }
  if (targetHeight > screenSize.height()){
	targetHeight = screenSize.height();
  }
  parent->setMaximumWidth(targetWidth);
  parent->setMaximumHeight(targetHeight);
  if (QTB_FIXED_MAINWINDOW_SIZE){
	if (settings->getOnKeepOriginalDesktopSize()){
	  parent->resize(targetWidth, targetHeight);
	}
  }

  // refresh image
  update();
}

// clear desktop window
void MainWindow::clearDesktop()
{
  //  cout << "clearDesktop()" << endl << flush;
#if 0 // for TEST
  if (settings->getConnected()){
	return;
  }
#endif
  if (!image.isNull()){
	image.fill(Qt::gray);
	update();
  }
}

// get window size
QSize MainWindow::getSize() const
{
  return size;
}

// get window size
QSize MainWindow::getDesktopSize() const
{
  return desktopSize;
}

// event handler

// paint event
void MainWindow::paintEvent(QPaintEvent *event)
{
  if (image.isNull()){
	return;
  }

#if 1 // for TEST
  QPainter painter(this);
  painter.drawImage(0, 0, image);
#else // for TEST
  QImage image2(image.size().width(), image.size().height(), QImage::Format_ARGB32_Premultiplied);
  QPainter imagePainter(&image2);
  imagePainter.setRenderHint(QPainter::Antialiasing, true);
  imagePainter.drawImage(0, 0, image);

  QPainter widgetPainter(this);
  widgetPainter.drawImage(0, 0, image2);
#endif // for TEST
}

//----------------------------------------------------------------------
// mouse events
//----------------------------------------------------------------------

// print mouse button event
void MainWindow::printMouseButtonEvent(QMouseEvent *event)
{
  switch (event->button()){
  case Qt::LeftButton:
	qDebug() << "Left Button : " << event->pos(); // for DEBUG
	break;
  case Qt::RightButton:
	qDebug() << "Right Button : " << event->pos(); // for DEBUG
	break;
  case Qt::MiddleButton:
	qDebug() << "Middle Button : " << event->pos(); // for DEBUG
	break;
  case Qt::ForwardButton:
	qDebug() << "Forward Button : " << event->pos(); // for DEBUG
	break;
  case Qt::BackButton:
	qDebug() << "Back Button : " << event->pos(); // for DEBUG
	break;
  default:
	qDebug() << "Unknown Button : " << event->pos(); // for DEBUG
	break;
  }
}

// set mouse button event
void MainWindow::setMouseButtonEvent(QMouseEvent *event, MouseInfoValue value)
{
  switch (event->button()){
  case Qt::LeftButton:
	mouseBuffer->put(TYPE_MOUSE_LEFT_BUTTON, value);
	break;
  case Qt::RightButton:
	mouseBuffer->put(TYPE_MOUSE_RIGHT_BUTTON, value);
	break;
#if 0 // for TEST
  case Qt::MiddleButton:
	mouseBuffer->put(TYPE_MOUSE_MIDDLE_BUTTON, value);
	break;
  case Qt::ForwardButton:
	mouseBuffer->put(TYPE_MOUSE_FORWARD_BUTTON, value);
	break;
  case Qt::BackButton:
	mouseBuffer->put(TYPE_MOUSE_BACK_BUTTON, value);
	break;
#endif // for TEST
  default:
	// Not support button
	break;
  }
}

// mouse press event
void MainWindow::mousePressEvent(QMouseEvent *event)
{
  if (outputLogForMouse){
	qDebug() << "[MainWindow] mousePressEvent: ";
	printMouseButtonEvent(event);
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
#if 1 // for TEST
	if (!settings->getOnShowSoftwareButton()){
	  MouseInfoValue value;
	  value.button = MOUSE_BUTTON_DOWN;
	  setMouseButtonEvent(event, value);
	}
	else {
	  if (event->button() == Qt::LeftButton){
		MOUSE_POS pos;
		pos.x = event->pos().x();
		pos.y = event->pos().y();
		mouseBuffer->setMousePos(pos);
	  }
	}
#else // for TEST
	MouseInfoValue value;
	value.button = MOUSE_BUTTON_DOWN;
	setMouseButtonEvent(event, value);
#endif // for TEST
  }
}

// mouse release event
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
	qDebug() << "[MainWindow] mouseReleaseEvent: ";
	printMouseButtonEvent(event);
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
#if 1 // for TEST
	if (!settings->getOnShowSoftwareButton()){
	  MouseInfoValue value;
	  value.button = MOUSE_BUTTON_UP;
	  setMouseButtonEvent(event, value);
	}
#else // for TEST
	MouseInfoValue value;
	value.button = MOUSE_BUTTON_UP;
	setMouseButtonEvent(event, value);
#endif // for TEST
  }
}

// mouse double click event
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
	qDebug() << "[MainWindow] mouseDoubleClickEvent: ";
	printMouseButtonEvent(event);
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
#if 1 // for TEST
	if (!settings->getOnShowSoftwareButton()){
	  MouseInfoValue value;
	  value.button = MOUSE_BUTTON_DBLCLK;
	  setMouseButtonEvent(event, value);
	}
#else // for TEST
	MouseInfoValue value;
	value.button = MOUSE_BUTTON_DBLCLK;
	setMouseButtonEvent(event, value);
#endif // for TEST
  }
}

// mouse wheel event
void MainWindow::wheelEvent(QWheelEvent *event)
{
  int degrees = event->delta() / 8;
  // for DEBUG
  if (outputLogForMouse){
	int ticks = degrees/15;
	qDebug() << "[MainWindow] wheelEvent: " << degrees << "(ticks = " << ticks << ")"; // for DEBUG
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
#if 1 // for TEST
	if (!settings->getOnShowSoftwareButton()){
	  MouseInfoValue value;
	  value.wheel = degrees;
	  mouseBuffer->put(TYPE_MOUSE_WHEEL, value);
	}
#else // for TEST
	MouseInfoValue value;
	value.wheel = degrees;
	mouseBuffer->put(TYPE_MOUSE_WHEEL, value);
#endif // for TEST
  }
}

// mouse move event
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
	qDebug() << "[MainWindow] mouseMoveEvent: " << event->pos(); // for DEBUG
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
#if 1 // for TEST
	if (!settings->getOnShowSoftwareButton()){
	  MOUSE_POS pos;
	  pos.x = event->pos().x();
	  pos.y = event->pos().y();
	  mouseBuffer->setMousePos(pos);
	}
#else // for TEST
	MOUSE_POS pos;
	pos.x = event->pos().x();
	pos.y = event->pos().y();
	mouseBuffer->setMousePos(pos);
#endif // for TEST
  }
}

//----------------------------------------------------------------------
// key events
//----------------------------------------------------------------------

// key press event
void MainWindow::keyPressEvent(QKeyEvent *event)
{
  // check event converter
  if (eventConverter == 0){
	// Nothing to do
	return;
  }

  // get VK_Code from Key_*
  uchar VK_Code = eventConverter->getVKCode((Key)event->key());
  if (outputLogForKeyboard){
	qDebug() << "[MainWindow]" << eventConverter->getEventConverterName() << // for DEBUG
	  ": Press   : VK_Code =" << eventConverter->getVKCodeByString(VK_Code) <<
	  ":" << hex << VK_Code;
  }

  // on Scroll Mode
  if (settings->getOnScrollMode()){
	if (scrollArea(VK_Code, true)){
	  return;
	}
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	// insert into KeyBuffer
	keyBuffer->put(VK_Code, KEYCODE_FLG_KEYDOWN);
  }
}

// key release event
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
  // check event converter
  if (eventConverter == 0){
	// Nothing to do
	return;
  }

  // get VK_Code from Key_*
  uchar VK_Code = eventConverter->getVKCode((Key)event->key());
  if (outputLogForKeyboard){
	qDebug() << "[MainWindow]" << eventConverter->getEventConverterName() << // for DEBUG
	  ": Release : VK_Code =" << eventConverter->getVKCodeByString(VK_Code) <<
	  ":" << hex << VK_Code;
  }

  // on Scroll Mode
  if (settings->getOnScrollMode()){
	if (scrollArea(VK_Code, false)){
	  return;
	}
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	// insert into KeyBuffer
	keyBuffer->put(VK_Code, KEYCODE_FLG_KEYUP);
  }
}

// scroll area
bool MainWindow::scrollArea(uchar VK_Code, bool onKeyPress)
{
  bool result = false;

  if (!onKeyPress){
	// Nothing to do if key release
	return false;
  }

  // change offset
  if (VK_Code == VK_UP){
	POS offsetY = settings->getDesktopOffsetY();
	offsetY -= 100;
	if (offsetY < 0) offsetY = 0;
	settings->setDesktopOffsetY(offsetY);
	//	cout << "Scroll Up : " << settings->getDesktopOffsetY() << endl << flush;
	result = true;
  }
  else if (VK_Code == VK_DOWN){
	POS offsetY = settings->getDesktopOffsetY();
	offsetY += 100;
	if (offsetY > settings->getDesktopHeight()){
	  offsetY = settings->getDesktopHeight();
	}
	settings->setDesktopOffsetY(offsetY);
	//	cout << "Scroll Down : " << settings->getDesktopOffsetY() << endl << flush;
	result = true;
  }
  else if (VK_Code == VK_LEFT){
	POS offsetX = settings->getDesktopOffsetX();
	offsetX -= 100;
	if (offsetX < 0) offsetX = 0;
	settings->setDesktopOffsetX(offsetX);
	//	cout << "Scroll Left : " << settings->getDesktopOffsetX() << endl << flush;
	result = true;
  }
  else if (VK_Code == VK_RIGHT){
	POS offsetX = settings->getDesktopOffsetX();
	offsetX += 100;
	if (offsetX > settings->getDesktopWidth()){
	  offsetX = settings->getDesktopWidth();
	}
	settings->setDesktopOffsetX(offsetX);
	//	cout << "Scroll Right : " << settings->getDesktopOffsetX() << endl << flush;
	result = true;
  }

  return result;
}

// get desktop scaling factor
qreal MainWindow::getDesktopScalingFactor(QSize size)
{
  qreal scalingFactor = settings->getDesktopScalingFactor();

  if (scalingFactor != 1.0){
	unsigned long maxImageDataSize = settings->getDesktop()->getMaxImageDataSize();
	if (maxImageDataSize == 0){
	  if (settings->getOutputLog()){
		cout << "[MainWindow] scaled... maxImageDataSize = " << maxImageDataSize << endl << flush;
	  }
	  return scalingFactor;
	}
#if defined(Q_OS_WIN)
	else {
	  // Internal Error: Unkown State
	  ABORT();
	}
#elif defined(Q_OS_LINUX)
	else {
	  // for Linux
	  unsigned long pageSizeMask = (unsigned long)getpagesize()-1;
	  qreal unitFactor = 1.0/DesktopScalingDialog::SLIDER_FACTOR;
	  while(true){ // for checking scaling factor
		QSize targetSize = size * scalingFactor;
		unsigned long imageDataSize = targetSize.width() * targetSize.height() * 4;
		imageDataSize = (imageDataSize + pageSizeMask) & ~pageSizeMask;
		if (imageDataSize <= maxImageDataSize){
		  return scalingFactor;
		}
		else {
		  // Can't shmget() in QXcbShmImage::QXcbShmImage() in qxcbbackingstore.cpp
		  if (settings->getOutputLog()){
			cout << "[MainWindow] Can't scale... imageDataSize    = " << imageDataSize << endl;
			cout << "[MainWindow] Can't scale... maxImageDataSize = " << maxImageDataSize << endl << flush;
		  }
		  // scale down
		  scalingFactor -= unitFactor;
		  if (scalingFactor < unitFactor){
			return unitFactor;
		  }
		}
	  } // end of while
	}
#elif  defined(Q_OS_MAC)
	else {
	  // Darwin
	  // Internal Error: Unkown State
	  ABORT();
	}
#endif
  }

  return scalingFactor;
}

// get height of menu bar
int MainWindow::getHeightOfMenuBar()
{
  if (settings->getOnShowMenuBar()){
	return heightOfMenuBar;
  }
  else {
	return heightOfMenuBarInHiding;
  }
}

// get height of status bar
int MainWindow::getHeightOfStatusBar()
{
  if (settings->getOnShowStatusBar()){
	return heightOfStatusBar;
  }
  else {
	return heightOfStatusBarInHiding;
  }
}

// minimum size hint
QSize MainWindow::minimumSizeHint() const
{
  //  return QSize(100, 40);
  return size;
}

// size hint
QSize MainWindow::sizeHint() const
{
  //  return QSize(100, 40);
  return size;
}

//----------------------------------------------------------------------
// native event filter
//----------------------------------------------------------------------
#if defined(Q_OS_WIN)
bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE
{
  if (!(settings->getConnected()) ||
	  !(settings->getOnControl())){
	return false;
  }
  if (eventType == "windows_generic_MSG"){
	MSG *msg = static_cast<MSG *>(message);
	// KEY
	if (msg->message == WM_KEYDOWN){
	  switch(msg->wParam){
	  case VK_OEM_AUTO:
	  case VK_OEM_ENLW:
		//		qDebug() << "[MainWindow] nativeEventFilter: KEYDOWN: " << msg->wParam; // for DEBUG
		keyBuffer->put(VK_KANJI, KEYCODE_FLG_KEYDOWN);
		return true;
		break;
	  case VK_CONVERT:
	  case VK_NONCONVERT:
	  case VK_OEM_ATTN:
	  case 229:
		//		qDebug() << "[MainWindow] nativeEventFilter: KEYDOWN: " << msg->wParam; // for DEBUG
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
		//		qDebug() << "[MainWindow] nativeEventFilter: KEYUP: " << msg->wParam; // for DEBUG
		keyBuffer->put(VK_KANJI, KEYCODE_FLG_KEYUP);
		return true;
		break;
	  case VK_CONVERT:
	  case VK_NONCONVERT:
	  case VK_OEM_ATTN:
		//		qDebug() << "[MainWindow] nativeEventFilter: KEYUP: " << msg->wParam; // for DEBUG
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
#if defined(Q_OS_LINUX)
// for X11
#include <xcb/xcb.h>

bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE
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
#endif // defined(Q_OS_LINUX)
#endif

} // end of namespace qtbrynhildr
