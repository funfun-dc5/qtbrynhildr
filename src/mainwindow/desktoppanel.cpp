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
#include <QPoint>
#include <QSize>
#if defined(QTB_DEV_TOUCHPANEL)
#include <QTouchEvent>
#endif // defined(QTB_DEV_TOUCHPANEL)
#if QTB_PUBLIC_MODE6_SUPPORT
#include <QList>
#include <QUrl>
#endif // QTB_PUBLIC_MODE6_SUPPORT
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
DesktopPanel::DesktopPanel(Settings *settings, QtBrynhildr *qtbrynhildr)
  :
  qtbrynhildr(qtbrynhildr),
  settings(settings),
  eventConverter(0),
  onShiftKey(false),
  onFullScreen(false),
  drawMarkerCounter(0),
#if defined(Q_OS_OSX)
  previous_KEYCODE_FLG(KEYCODE_FLG_KEYUP),
#endif // defined(Q_OS_OSX)
  keyboardLogFile(0),
  keyboardLogFileStream(0),
  // for DEBUG
  outputLogForKeyboard(QTB_DEBUG_KEYBOARD),
  outputLogForMouse(QTB_DEBUG_MOUSE),
  outputLog(false)
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

// reflesh desktop window
void DesktopPanel::refreshDesktop(QImage image)
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
#if defined(QTB_DEV_DESKTOP)
		image = image.scaled(currentSize, Qt::KeepAspectRatio, settings->getDesktopScaringQuality());
		//image = image.scaled(currentSize, Qt::KeepAspectRatio, Qt::FastTransformation);
		//image = image.scaled(currentSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif // defined(QTB_DEV_DESKTOP)
	  }
	  // save scaling factor
	  if (scalingFactor != settings->getDesktopScalingFactor()){
		settings->setDesktopScalingFactor(scalingFactor);
	  }
	}
#if defined(QTB_DEV_DESKTOP)
	else { // DESKTOPSCALING_TYPE_ON_SERVER
	  if (settings->getDesktopScalingFactor() > 1.0){
		// scale
		currentSize = currentSize * settings->getDesktopScalingFactor();
		image = image.scaled(currentSize, Qt::KeepAspectRatio, settings->getDesktopScaringQuality());
	  }
	}
#endif // defined(QTB_DEV_DESKTOP)
  }

#if 0 // QTB_DESKTOP_COMPRESS_MODE // for TEST
  // desktop compress mode
  if (settings->getDesktopCompressMode() > 1){
	currentSize = currentSize * settings->getDesktopCompressMode();
	image = image.scaled(currentSize, Qt::KeepAspectRatio, settings->getDesktopScaringQuality());
  }
#endif // QTB_DESKTOP_COMPRESS_MODE

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

  // resize if NOT full screen
  if (QTB_FIXED_MAINWINDOW_SIZE){
	if (!onFullScreen){
	  if (settings->getOnKeepOriginalDesktopSize() && !(qtbrynhildr->isMaximized() || qtbrynhildr->isMinimized())){
		int width = currentSize.width();
		int height = currentSize.height() + qtbrynhildr->getHeightOfMenuBar() + qtbrynhildr->getHeightOfStatusBar();
#if QTB_DESKTOPWINDOW
		// correct
		width  += settings->getDesktop()->getCorrectWindowWidth();
		height += settings->getDesktop()->getCorrectWindowHeight();
#endif // QTB_DESKTOPWINDOW

		QSize screenSize = settings->getDesktop()->getCurrentScreen().size();
		if (width > screenSize.width()){
		  width = screenSize.width();
		}
		if (height > screenSize.height()){
		  height = screenSize.height();
		}

#if QTB_DESKTOPWINDOW
		// resize
		qtbrynhildr->resize(width, height);
#endif // QTB_DESKTOPWINDOW

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
#if 0 // for TEST
  if (settings->getConnected()){
	return;
  }
#endif
  if (!image.isNull()){
	image.fill(Qt::gray);
	updateDesktop();
  }
}

// get window size
QSize DesktopPanel::getSize() const
{
  return currentSize;
}

// get window size
QSize DesktopPanel::getDesktopSize() const
{
  return desktopSize;
}

// set full screen flag
void DesktopPanel::setOnFullScreen(bool onFullScreen)
{
  if (QTB_DESKTOP_FULL_SCREEN){
	this->onFullScreen = onFullScreen;
  }
}

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
void DesktopPanel::setMouseButtonEvent(QMouseEvent *event, MouseInfoValue value)
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
void DesktopPanel::mousePressEvent(QMouseEvent *event)
{
  if (outputLogForMouse){
	cout << "[DesktopPanel] mousePressEvent: ";
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
void DesktopPanel::mouseDoubleClickEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
	cout << "[DesktopPanel] mouseDoubleClickEvent: ";
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
void DesktopPanel::wheelEvent(QWheelEvent *event)
{
  int degrees = event->delta() / 8;
  // for DEBUG
  if (outputLogForMouse){
	int ticks = degrees/15;
	cout << "[DesktopPanel] wheelEvent: " << degrees << "(ticks = " << ticks << ")"; // for DEBUG
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
void DesktopPanel::mouseMoveEvent(QMouseEvent *event)
{
  // for DEBUG
  if (outputLogForMouse){
    cout << "[DesktopPanel] mouseMoveEvent: (x, y) = (" <<
	  event->pos().x() << "," << event->pos().y() << ")" << endl << flush; // for DEBUG
  }

  if (settings->getConnected() &&
	  settings->getOnControl()){
	if (!(settings->getOnShowSoftwareKeyboard() ||
		  settings->getOnShowSoftwareButton())){
	  currentMousePos = event->pos();
	  MOUSE_POS pos;
	  pos.x = currentMousePos.x();
	  pos.y = currentMousePos.y();
	  mouseBuffer->setMousePos(pos);
	  //qtbrynhildr->moveTopOfSoftwareKeyboard(pos.y); // for TEST
#if QTB_PUBLIC_MODE7_SUPPORT && !defined(Q_OS_WIN)
	  // set cursor point color to control thread
	  qtbrynhildr->setCursorPointColor(image.pixel(currentMousePos));
#endif // QTB_PUBLIC_MODE7_SUPPORT && !defined(Q_OS_WIN)
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
	qDebug() << "[DesktopPanel]" << eventConverter->getEventConverterName() << // for DEBUG
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
qreal DesktopPanel::getDesktopScalingFactor(QSize size)
{
  Q_UNUSED(size);

  qreal scalingFactor = settings->getDesktopScalingFactor();

  if (scalingFactor != 1.0){
	unsigned long maxImageDataSize = settings->getDesktop()->getMaxImageDataSize();
	if (maxImageDataSize == 0){
	  if (settings->getOutputLog()){
		cout << "[DesktopPanel] scaled... maxImageDataSize = " << maxImageDataSize << endl << flush;
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
			cout << "[DesktopPanel] Can't scale... imageDataSize    = " << imageDataSize << endl;
			cout << "[DesktopPanel] Can't scale... maxImageDataSize = " << maxImageDataSize << endl << flush;
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
