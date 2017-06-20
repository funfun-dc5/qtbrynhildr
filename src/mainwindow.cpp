// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if defined(QTB_NET_UNIX)
#include <unistd.h>
#endif // defined(QTB_NET_UNIX)

// Qt Header
#include <QDateTime>
#include <QKeyEvent>
#include <QMimeData>
#include <QPainter>
#include <QPoint>
#include <QSize>
#if QTB_PUBLIC_MODE6_SUPPORT
#include <QList>
#include <QUrl>
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if 1 // for qDebug()
#include <QtCore>
#endif

// Local Header
#include "config.h"
#include "mainwindow.h"
#include "parameters.h"
#include "qtbrynhildr.h"
#if defined(QTB_NET_UNIX)
#include "dialog/desktop_scaling_dialog.h"
#endif // defined(QTB_NET_UNIX)

namespace qtbrynhildr {

// constructor
MainWindow::MainWindow(Settings *settings, QtBrynhildr *parent)
  :
  QWidget(parent),
  settings(settings),
  eventConverter(0),
  onShiftKey(false),
  heightOfMenuBar(0),
  heightOfStatusBar(0),
  onFullScreen(false),
  drawMarkerCounter(0),
#if defined(Q_OS_OSX)
  previous_KEYCODE_FLG(KEYCODE_FLG_KEYUP),
#endif // defined(Q_OS_OSX)
  keyboardLogFile(0),
  keyboardLogFileStream(0),
  // for DEBUG
  outputLog(false),
  outputLogForKeyboard(QTB_DEBUG_KEYBOARD),
  outputLogForMouse(QTB_DEBUG_MOUSE)
{
  // save parent
  this->parent = parent;

  // setting main window
  //  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true); // mouse tracking mode on

  // create keyboard buffer
  keyBuffer = new KeyBuffer(QTB_KEYBOARD_BUFFER_SIZE);

  // create mouse buffer
  mouseBuffer = new MouseBuffer(QTB_MOUSE_BUFFER_SIZE);

  // open keyboard log file
  openKeyboardLogFile(settings->getKeyboardLogFile());
}

// destructor
MainWindow::~MainWindow()
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
  currentSize = desktopSize = image.size();

  // capture desktop image for original size
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	if (settings->getOnDesktopCapture() &&
		!settings->getDesktopCaptureFormat().startsWith(".")){
	  QDateTime now = QDateTime::currentDateTime();
	  QString filename = settings->getOutputPath() +
		QString(QTB_DESKTOP_CAPTURE_FILENAME_PREFIX) +
		now.toString(QTB_DESKTOP_CAPTURE_FILENAME_DATE_FORMAT) +
		"." + settings->getDesktopCaptureFormat();

	  // save to file
	  image.save(filename);

	  // reset desktop capture flag
	  settings->setOnDesktopCapture(false);
	}
  }

  // scaling image
  if (QTB_DESKTOP_IMAGE_SCALING){
	if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_CLIENT){
	  qreal scalingFactor = getDesktopScalingFactor(currentSize);
	  if (scalingFactor != 1.0){
		// scale
		currentSize = currentSize * scalingFactor;
		image = image.scaled(currentSize, Qt::KeepAspectRatio, settings->getDesktopScaringQuality());
	  }
	  // save scaling factor
	  if (scalingFactor != settings->getDesktopScalingFactor()){
		settings->setDesktopScalingFactor(scalingFactor);
	  }
	}
  }

  // capture desktop image
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	if (settings->getOnDesktopCapture()){
	  QDateTime now = QDateTime::currentDateTime();
	  QString filename = settings->getOutputPath() +
		QString(QTB_DESKTOP_CAPTURE_FILENAME_PREFIX) +
		now.toString(QTB_DESKTOP_CAPTURE_FILENAME_DATE_FORMAT) +
		settings->getDesktopCaptureFormat();

	  // save to file
	  image.save(filename);

	  // reset desktop capture flag
	  settings->setOnDesktopCapture(false);
	}
  }

  // copy QImage
  this->image = image;

  // resize window
  if (previousSize != currentSize){
#if 0 // for DEBUG
	cout << "resize..." << endl; // for DEBUG
	cout << "image.size().width()  = " << image.size().width() << endl; // for DEBUG
	cout << "image.size().height() = " << image.size().height() << endl << flush; // for DEBUG
#endif
	previousSize = currentSize;
	// resize main window
	resize(currentSize.width(), currentSize.height());

	resizeWindow();
  }
  else if (settings->getDesktop()->isChangedCurrentScreen()) {
	// if current screen is changed.
	// refresh
	resizeWindow();
  }
  else {
	// refresh image
	update();
  }
}

// resize window
void MainWindow::resizeWindow()
{
  // check size
  if (!currentSize.isValid()){
	// size is null value
	// Nothing to do
	return;
  }

  // resize if NOT full screen
  if (QTB_FIXED_MAINWINDOW_SIZE){
	if (!onFullScreen){
	  if (settings->getOnKeepOriginalDesktopSize() && !(parent->isMaximized() || parent->isMinimized())){
		int width = currentSize.width() + settings->getDesktop()->getCorrectWindowWidth();
		int height = currentSize.height() + getHeightOfMenuBar() + getHeightOfStatusBar() + settings->getDesktop()->getCorrectWindowHeight();

		QSize screenSize = settings->getDesktop()->getCurrentScreen().size();
		if (width > screenSize.width()){
		  width = screenSize.width();
		}
		if (height > screenSize.height()){
		  height = screenSize.height();
		}

		// resize
		parent->resize(width, height);

		// refresh image
		update();
	  }
	}
  }
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
  return currentSize;
}

// get window size
QSize MainWindow::getDesktopSize() const
{
  return desktopSize;
}

// set full screen flag
void MainWindow::setOnFullScreen(bool onFullScreen)
{
  if (QTB_DESKTOP_FULL_SCREEN){
	this->onFullScreen = onFullScreen;
  }
}

// event handler

// paint event
void MainWindow::paintEvent(QPaintEvent *event)
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
void MainWindow::leaveEvent(QEvent *event)
{
  Q_UNUSED(event);

  if (settings->getOnClipCursor()){
	QCursor::setPos(mapToGlobal(currentMousePos));
  }
}

//----------------------------------------------------------------------
// mouse events
//----------------------------------------------------------------------

// print mouse button event
void MainWindow::printMouseButtonEvent(QMouseEvent *event)
{
  switch (event->button()){
  case Qt::LeftButton:
	cout << "Left Button : ";
	break;
  case Qt::RightButton:
	cout << "Right Button : ";
	break;
  case Qt::MiddleButton:
	cout << "Middle Button : ";
	break;
  case Qt::ForwardButton:
	cout << "Forward Button : ";
	break;
  case Qt::BackButton:
	cout << "Back Button : ";
	break;
  default:
	cout << "Unknown Button : ";
	break;
  }

  cout << "(x, y) = (" << event->pos().x() << "," << event->pos().y() << ")" << endl << flush;
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
#if QTB_EXTRA_BUTTON_SUPPORT
  case Qt::MiddleButton:
	mouseBuffer->put(TYPE_MOUSE_MIDDLE_BUTTON, value);
	break;
  case Qt::ForwardButton:
	mouseBuffer->put(TYPE_MOUSE_FORWARD_BUTTON, value);
	break;
  case Qt::BackButton:
	mouseBuffer->put(TYPE_MOUSE_BACK_BUTTON, value);
	break;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  default:
	// Not support button
	break;
  }
}

// mouse press event
void MainWindow::mousePressEvent(QMouseEvent *event)
{
  if (outputLogForMouse){
	cout << "[MainWindow] mousePressEvent: ";
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
	cout << "[MainWindow] mouseReleaseEvent: ";
	printMouseButtonEvent(event);
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){

	// check for filedrop
	if (event->button() == Qt::LeftButton){
	  QRect window = QRect(0, 0, currentSize.width(), currentSize.height());
	  if (!window.contains(event->pos(), false)){
		//		cout << "FileDrop!" << endl << flush;
		MouseInfoValue value;
		value.button = MOUSE_BUTTON_UP;
		mouseBuffer->put(TYPE_MOUSE_FILEDROP, value);
		return;
	  }
	}

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
	cout << "[MainWindow] mouseDoubleClickEvent: ";
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
	cout << "[MainWindow] wheelEvent: " << degrees << "(ticks = " << ticks << ")"; // for DEBUG
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
    cout << "[MainWindow] mouseMoveEvent: (x, y) = (" <<
	  event->pos().x() << "," << event->pos().y() << ")" << endl << flush; // for DEBUG
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	if (!settings->getOnShowSoftwareButton()){
	  currentMousePos = event->pos();
	  MOUSE_POS pos;
	  pos.x = currentMousePos.x();
	  pos.y = currentMousePos.y();
	  mouseBuffer->setMousePos(pos);
	}
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

  // for Menu Key support
  Qt::Key key = (Qt::Key)event->key();
  if (key == Qt::Key_Menu){
	// right button down and up
	MouseInfoValue value;
	value.button = MOUSE_BUTTON_DOWN;
	mouseBuffer->put(TYPE_MOUSE_RIGHT_BUTTON, value);
	value.button = MOUSE_BUTTON_UP;
	mouseBuffer->put(TYPE_MOUSE_RIGHT_BUTTON, value);
	return;
  }

  // get VK_Code from Key_*
  uchar VK_Code = eventConverter->getVKCode(event);
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
	// exit full screen
	if (onFullScreen && VK_Code == VK_ESCAPE){
	  parent->exitFullScreen();
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
								 << " => Sent" << endl << flush;
	  }
	}

	// insert into KeyBuffer
	keyBuffer->put(VK_Code, KEYCODE_FLG_KEYDOWN);
	// output Keyboard Log
	if (settings->getOutputKeyboardLog()){
	  (*keyboardLogFileStream) << "Press   - Qt : " << (Qt::Key)event->key()
							   << ", Windows: " << eventConverter->getVKCodeByString(VK_Code)
							   << " => Sent" << endl << flush;
	}

	// set shift key status
	if (VK_Code == VK_SHIFT){
	  onShiftKey = true;
	}

#if defined(Q_OS_OSX)
  // set previous KEYCODE_FLG
  previous_KEYCODE_FLG = KEYCODE_FLG_KEYDOWN;
#endif // defined(Q_OS_OSX)
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

  // for Menu Key support
  Qt::Key key = (Qt::Key)event->key();
  if (key == Qt::Key_Menu){
	// Nothing to do
	return;
  }

  // get VK_Code from Key_*
  uchar VK_Code = eventConverter->getVKCode(event);
  if (outputLogForKeyboard){
	qDebug() << "[MainWindow]" << eventConverter->getEventConverterName() << // for DEBUG
	  ": Release : VK_Code =" << eventConverter->getVKCodeByString(VK_Code) <<
	  ":" << hex << VK_Code;
  }

  // output Keyboard Log
  if (settings->getOutputKeyboardLog()){
	(*keyboardLogFileStream) << "Release - Qt : " << (Qt::Key)event->key()
							 << ", Windows: " << eventConverter->getVKCodeByString(VK_Code)
							 << " => Sent" << endl << flush;
  }

  // on Scroll Mode
  if (settings->getOnScrollMode()){
	if (scrollArea(VK_Code, false)){
	  return;
	}
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
#if defined(Q_OS_OSX)
	// check previous KEYCODE_FLG
	if (previous_KEYCODE_FLG == KEYCODE_FLG_KEYUP){
	  // check VK_TAB and VK_SPACE(Eisu and Kana)
	  switch(VK_Code){
	  case VK_SPACE:
		VK_Code = VK_KANJI;
		// Fall Through
	  case VK_TAB:
		keyBuffer->put(VK_Code, KEYCODE_FLG_KEYDOWN);
		break;
	  default:
		// Nothing to do
		break;
	  }
	}
#endif // defined(Q_OS_OSX)
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
								 << " => Sent" << endl << flush;
	  }
	}

#if defined(Q_OS_OSX)
	// set previous KEYCODE_FLG
	previous_KEYCODE_FLG = KEYCODE_FLG_KEYUP;
#endif // defined(Q_OS_OSX)
  }
}

#if QTB_DRAG_AND_DROP_SUPPORT
//----------------------------------------------------------------------
// drag and drop events
//----------------------------------------------------------------------
// drag enter event
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
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
void MainWindow::dropEvent(QDropEvent *event)
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
  Q_UNUSED(size);

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
#elif defined(Q_OS_FREEBSD)
	else {
	  // Yet
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
#elif  defined(Q_OS_OSX)
	else {
	  // Darwin
	  // Internal Error: Unkown State
	  ABORT();
	}
#endif
  }

  return scalingFactor;
}

// minimum size hint
QSize MainWindow::minimumSizeHint() const
{
  return currentSize;
}

// size hint
QSize MainWindow::sizeHint() const
{
  return currentSize;
}

//----------------------------------------------------------------------
// native event filter
//----------------------------------------------------------------------
#if defined(Q_OS_WIN)
bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
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
		//		cout << "[MainWindow] nativeEventFilter: KEYDOWN: " << msg->wParam << endl; // for DEBUG
		keyBuffer->put(VK_KANJI, KEYCODE_FLG_KEYDOWN);
		return true;
		break;
	  case VK_CONVERT:
	  case VK_NONCONVERT:
	  case VK_OEM_ATTN:
	  case 229:
		//		cout << "[MainWindow] nativeEventFilter: KEYDOWN: " << msg->wParam << endl; // for DEBUG
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
		//		cout << "[MainWindow] nativeEventFilter: KEYUP: " << msg->wParam << endl; // for DEBUG
		keyBuffer->put(VK_KANJI, KEYCODE_FLG_KEYUP);
		return true;
		break;
	  case VK_CONVERT:
	  case VK_NONCONVERT:
	  case VK_OEM_ATTN:
		//		cout << "[MainWindow] nativeEventFilter: KEYUP: " << msg->wParam << endl; // for DEBUG
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
#endif // defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD)
#endif

//----------------------------------------------------------------------
// keyboard log file
//----------------------------------------------------------------------
// open keyboard log file
bool MainWindow::openKeyboardLogFile(QString filename)
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
bool MainWindow::closeKeyboardLogFile()
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
