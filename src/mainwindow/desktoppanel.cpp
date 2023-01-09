// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>

// Local Header
#include "mainwindow/desktoppanel.h"

namespace qtbrynhildr {

// constructor
DesktopPanel::DesktopPanel(QtBrynhildr *qtbrynhildr, QWidget *parent)
  :QScrollArea(parent)
  ,qtbrynhildr(qtbrynhildr)
  ,settings(qtbrynhildr->getSettings())
  ,desktopPanelWidget(0)
  ,keyBuffer(0)
  ,topType(TOP_TYPE_UNKNOWN)
  ,scalingFactor(1.0)
  ,scalingFactorForFullScreen(1.0)
  ,screenSize(settings->getCurrentScreenSize())
  // for DEBUG
  ,outputLog(true)
{
  // focus
  setFocusPolicy(Qt::StrongFocus);

  // enable touch event
  setAttribute(Qt::WA_AcceptTouchEvents, true);

  //setWidgetResizable(true);
  setWidgetResizable(false);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  //setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
  //setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  //setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);

  // scrollbar Always Off
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // desktop panel widget
  desktopPanelWidget = new DesktopPanelWidget(qtbrynhildr, this);
  // key buffer
  keyBuffer = desktopPanelWidget->getKeyBuffer();

  setScale(0.33); // for TEST
}

// destructor
DesktopPanel::~DesktopPanel()
{
  // delete objects
  if (desktopPanelWidget != 0){
	delete desktopPanelWidget;
	desktopPanelWidget = 0;
  }
}

// scale
void DesktopPanel::setScale(qreal scalingFactor)
{
  this->scalingFactor = scalingFactor;
  settings->setDesktopScalingFactor(scalingFactor);
}

// resize desktop
void DesktopPanel::resizeDesktop(int width, int height)
{
  Q_UNUSED(width);
  Q_UNUSED(height);
}

// update desktop
void DesktopPanel::updateDesktop()
{
  update();
}

// mouse press event software panel
void DesktopPanel::mousePressEventForSP(QMouseEvent *event)
{
  mousePressEvent(event);
}

// mouse release event software panel
void DesktopPanel::mouseReleaseEventForSP(QMouseEvent *event)
{
  mouseReleaseEvent(event);
}

// mouse move event software panel
void DesktopPanel::mouseMoveEventForSP(QMouseEvent *event)
{
  mouseMoveEvent(event);
}

// clear desktop window
void DesktopPanel::clearDesktop()
{
  //  std::cout << "clearDesktop()" << std::endl << std::flush;
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
		  qtbrynhildr->getHeightOfMenuBar()
		  + qtbrynhildr->getHeightOfStatusBar()
		  + heightMargin);
#if QTB_TOOLBAR
  diffSize += QSize(qtbrynhildr->getWidthOfToolBar(),
					qtbrynhildr->getHeightOfToolBar());
#endif // QTB_TOOLBAR

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

#if 0 // for TEST
// check focus
bool DesktopPanel::hasFocus() const
{
  if (qtbrynhildr != 0){
	return qtbrynhildr->hasFocus();
  }
  else {
	return false;
  }
}
#endif // 0 // for TEST

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
>>>>>>> master
// mouse move
void DesktopPanel::mouseMove(QPoint mousePos, bool marker)
{
  desktopPanelWidget->mouseMove(mousePos, marker);
}

// mouse move relatively
void DesktopPanel::mouseMoveRelatively(QPoint mousePos, bool marker)
{
  mousePos /= scalingFactor;
  desktopPanelWidget->mouseMoveRelatively(mousePos, marker);
}

// size hint
QSize DesktopPanel::sizeHint() const
{
  return QSize(1280, 736);
}

// scrollarea event for event handling (touchpanel)
bool DesktopPanel::event(QEvent *event)
{
  switch(event->type()){
  case QEvent::TouchBegin:
  case QEvent::TouchUpdate:
  case QEvent::TouchEnd:
	{
	  QTouchEvent *touchEvent = (QTouchEvent*)event;
	  if (outputLog){
		qDebug() << "event type  = " << event->type();
		qDebug() << "TouchStates = " << touchEvent->touchPointStates();
	  }

	  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
<<<<<<< HEAD
	  int touchPointCount = touchPoints.count();

	  // -----------------------------------------------------------------------------------
	  // KeroRemote Compatible Operation
	  // -----------------------------------------------------------------------------------
	  if (settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_KEROREMOTE){
		switch(touchPointCount){
		case 1: // 1 finger operation
		  return oneFingerEventForKeroRemote(touchEvent);
		  break;
		case 2: // 2 fingers operation
		  return twoFingerEventForKeroRemote(touchEvent);
		  break;
		case 3: // 3 fingers operation
		  return threeFingerEvent(touchEvent);
		  break;
		default:
		  // Nothing to do
		  break;
		}
	  }
	  // -----------------------------------------------------------------------------------
	  // Qt Brynhildr Operation
	  // -----------------------------------------------------------------------------------
	  else if (settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_QTBRYNHILDR){
		switch(touchPointCount){
		case 1: // 1 finger operation
		  return oneFingerEventForQtBrynhildr(touchEvent);
		  break;
		case 2: // 2 fingers operation
		  return twoFingerEventForQtBrynhildr(touchEvent);
		  break;
		case 3: // 3 fingers operation
		  return threeFingerEvent(touchEvent);
		  break;
		default:
		  // Nothing to do
		  break;
		}
	  }
	  else {
		// Unknown Touchpanel Operation Type
		qDebug() << "DP: Unknown Touchpanel Operation!";
	  }
	}
  default:
	// Nothing to do
	break;
  }

  return QScrollArea::event(event);
=======
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
	std::cout << "Left Button    : ";
	break;
  case Qt::RightButton:
	std::cout << "Right Button   : ";
	break;
  case Qt::MiddleButton:
	std::cout << "Middle Button  : ";
	break;
  case Qt::ForwardButton:
	std::cout << "Forward Button : ";
	break;
  case Qt::BackButton:
	std::cout << "Back Button    : ";
	break;
  default:
	std::cout << "Unknown Button : ";
	break;
  }

  std::cout << "(x, y) = (" << event->pos().x() << "," << event->pos().y() << ")" << std::endl << std::flush;
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
	std::cout << "[DesktopPanel] mousePressEvent: ";
	printMouseButtonEvent(event);
  }

  // check connected
  if (!settings->getConnected())
	return;

  // viewer mode
  if (settings->getOnViewerMode()){
	return;
  }

  // control
  if (settings->getOnControl()){
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
	std::cout << "[DesktopPanel] mouseReleaseEvent: ";
	printMouseButtonEvent(event);
  }

  // check connected
  if (!settings->getConnected())
	return;

  // viewer mode
  if (settings->getOnViewerMode()){
	return;
  }

  // control
  if (settings->getOnControl()){
	// check for filedrop
	if (event->button() == Qt::LeftButton){
	  QRect window = QRect(0, 0, currentSize.width(), currentSize.height());
	  if (!window.contains(event->pos(), false)){
		//		std::cout << "FileDrop!" << std::endl << std::flush;
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
	std::cout << "[DesktopPanel] mouseDoubleClickEvent: ";
	printMouseButtonEvent(event);
  }

  // check connected
  if (!settings->getConnected())
	return;

  // viewer mode
  if (settings->getOnViewerMode()){
	return;
  }

  // control
  if (settings->getOnControl()){
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

#if QT_VERSION >= 0x050500 // Qt 5.5.0
  if(event->source() == Qt::MouseEventNotSynthesized){
	//std::cout << "[WheelEvent] source() : " << event->source() << std::endl << std::flush;
	QPoint degrees = event->angleDelta() / 8;
	mouseWheel = degrees.y();

	// for DEBUG
	if (outputLogForMouse){
	  int ticks = mouseWheel/15;
	  std::cout << "[DesktopPanel] wheelEvent(degrees): " << mouseWheel << " (ticks = " << ticks << ")" << std::endl << std::flush; // for DEBUG
	}
  }
  else if (event->source() == Qt::MouseEventSynthesizedBySystem){
	//std::cout << "[WheelEvent] source() : " << event->source() << std::endl << std::flush;
	QPoint pixels = event->pixelDelta();
	mouseWheel = pixels.y();

	// for DEBUG
	if (outputLogForMouse){
	  std::cout << "[DesktopPanel] wheelEvent(pixels): " << (int)mouseWheel << std::endl << std::flush; // for DEBUG
	}

	// adjust
	mouseWheel *= settings->getDesktopScalingFactor();
  }
#else // QT_VERSION >= 0x050500 // Qt 5.5.0
  //std::cout << "[WheelEvent] source() : " << event->source() << std::endl << std::flush;
  QPoint degrees = event->angleDelta() / 8;
  mouseWheel = degrees.y();

  // for DEBUG
  if (outputLogForMouse){
	int ticks = mouseWheel/15;
	std::cout << "[DesktopPanel] wheelEvent(degrees): " << mouseWheel << " (ticks = " << ticks << ")" << std::endl << std::flush; // for DEBUG
  }
#endif // QT_VERSION >= 0x050500 // Qt 5.5.0

  // check connected
  if (!settings->getConnected())
	return;

  // viewer mode
  if (settings->getOnViewerMode()){
	return;
  }

  // control
  if (settings->getOnControl()){
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
    std::cout << "[DesktopPanel] mouseMoveEvent: (x, y) = (" <<
	  event->pos().x() << "," << event->pos().y() << ")" << std::endl << std::flush; // for DEBUG
  }

  // move mouse cursor
  moveMouseCursor(event, true);
}

// move mouse cursor
void DesktopPanel::moveMouseCursor(QMouseEvent *event, bool marker)
{
  // check connected
  if (!settings->getConnected())
	return;

  // viewer mode
  if (settings->getOnViewerMode()){
	return;
  }

  // control
  if (settings->getOnControl()){
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

  // check connected
  if (!settings->getConnected())
	return;

  // viewer mode
  if (settings->getOnViewerMode()){
	return;
  }

  // control
  if (settings->getOnControl()){
#if defined(QTB_DEV_DESKTOP)
	// exit full screen
	if (onFullScreen && VK_Code == VK_ESCAPE){
#if QTB_TOOLBAR
	  if (!qtbrynhildr->getToolBar()->isVisible()){
		qtbrynhildr->exitFullScreen();
		return;
	  }
#else // QTB_TOOLBAR
	  qtbrynhildr->exitFullScreen();
	  return;
#endif // QTB_TOOLBAR
	}
#endif // defined(QTB_DEV_DESKTOP)
	// check shift key status
	if (!onShiftKey && eventConverter->getShiftKeyControl() == EventConverter::SHIFTKEY_NEED){
	  // need shift key
	  keyBuffer->put(VK_SHIFT, KEYCODE_FLG_KEYDOWN);
	  // output Keyboard Log
	  if (settings->getOutputKeyboardLog()){
		outputKeyboardLog("Press", (Qt::Key)event->key(), VK_SHIFT);
	  }
	}

	// check control key status
	if (key == Qt::Key_Control){
	  // press control key
	  qtbrynhildr->setOnControlKey(true);
	}

	// insert into KeyBuffer
	keyBuffer->put(VK_Code, KEYCODE_FLG_KEYDOWN);
	// output Keyboard Log
	if (settings->getOutputKeyboardLog()){
	  outputKeyboardLog("Press", (Qt::Key)event->key(), VK_Code);
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
	outputKeyboardLog("Release", (Qt::Key)event->key(), VK_Code);
  }

  // on Scroll Mode
  if (settings->getOnScrollMode()){
	if (scrollArea(VK_Code, false)){
	  return;
	}
  }

  // check connected
  if (!settings->getConnected())
	return;

  // viewer mode
  if (settings->getOnViewerMode()){
	return;
  }

  // control
  if (settings->getOnControl()){
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
		outputKeyboardLog("Release", (Qt::Key)event->key(), VK_SHIFT);
	  }
	}

	// check control key status
	if (key == Qt::Key_Control){
	  // release control key
	  qtbrynhildr->setOnControlKey(false);
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
  // check connected
  if (!settings->getConnected() ||
	  !settings->getOnControl()){
	// Nothing to do
	return;
  }

  // check support
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
  // check connected
  if (!settings->getConnected() ||
	  !settings->getOnControl()){
	// Nothing to do
	return;
  }

  // check support
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
#if QT_VERSION < 0x060000
bool DesktopPanel::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
#else // QT_VERSION >= 0x060000
bool DesktopPanel::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
#endif // QT_VERSION >= 0x060000
{
  Q_UNUSED(result);

  if (!(settings->getConnected()) ||
	  !(settings->getOnControl()) || settings->getOnViewerMode()){
	return false;
  }
  if (eventType == "windows_generic_MSG"){
	MSG *msg = static_cast<MSG *>(message);
	// KEY
	if (settings->getKeyboardType() == KEYBOARD_TYPE_NATIVE){
	  // send All key event in nativeEventFilter
#if 0 // VK_XXXX -> VK_LXXXX/RXXXX
	  static uchar pressedShiftKey;
	  static uchar pressedControlKey;
#endif // 0 // // VK_XXXX -> VK_LXXXX/RXXXX
	  if (msg->message == WM_KEYDOWN){
#if 0 // VK_XXXX -> VK_LXXXX/RXXXX
		// CONTROL/SHIFT
		// VK_XXXX -> VK_LXXXX/RXXXX
		switch(msg->wParam){
		case VK_CONTROL:
		  if (GetAsyncKeyState(VK_RCONTROL) & 0x01){
			msg->wParam = VK_RCONTROL;
			pressedControlKey = VK_RCONTROL;
			if (settings->getOutputKeyboardLog()){
			  outputKeyboardLog("Press", Qt::Key_Control, VK_RCONTROL);
			}
		  }
		  else {
			msg->wParam = VK_LCONTROL;
			pressedControlKey = VK_LCONTROL;
			if (settings->getOutputKeyboardLog()){
			  outputKeyboardLog("Press", Qt::Key_Control, VK_LCONTROL);
			}
		  }
		  break;
		case VK_SHIFT:
		  if (GetAsyncKeyState(VK_RSHIFT) & 0x01){
			msg->wParam = VK_RSHIFT;
			pressedShiftKey = VK_RSHIFT;
			if (settings->getOutputKeyboardLog()){
			  outputKeyboardLog("Press", Qt::Key_Shift, VK_RSHIFT);
			}
		  }
		  else {
			msg->wParam = VK_LSHIFT;
			pressedShiftKey = VK_LSHIFT;
			if (settings->getOutputKeyboardLog()){
			  outputKeyboardLog("Press", Qt::Key_Shift, VK_LSHIFT);
			}
		  }
		  break;
		}
#endif // 0 // VK_XXXX -> VK_LXXXX/RXXXX
		keyBuffer->put(msg->wParam, KEYCODE_FLG_KEYDOWN);
		return true;
	  }
	  else if (msg->message == WM_KEYUP){
#if 0 // VK_XXXX -> VK_LXXXX/RXXXX
		// CONTROL/SHIFT
		switch(msg->wParam){
		case VK_CONTROL:
		  msg->wParam = pressedControlKey;
		  if (settings->getOutputKeyboardLog()){
			outputKeyboardLog("Release", Qt::Key_Control, pressedControlKey);
		  }
		  break;
		case VK_SHIFT:
		  msg->wParam = pressedShiftKey;
		  if (settings->getOutputKeyboardLog()){
			outputKeyboardLog("Release", Qt::Key_Shift, pressedShiftKey);
		  }
		  break;
		}
#endif // 0 // VK_XXXX -> VK_LXXXX/RXXXX
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
		//		std::cout << "[DesktopPanel] nativeEventFilter: KEYDOWN: " << msg->wParam << std::endl; // for DEBUG
		keyBuffer->put(VK_KANJI, KEYCODE_FLG_KEYDOWN);
		return true;
		break;
		//	  case VK_TAB:
	  case VK_CONVERT:
	  case VK_NONCONVERT:
	  case VK_OEM_ATTN:
	  case 229:
		//		std::cout << "[DesktopPanel] nativeEventFilter: KEYDOWN: " << msg->wParam << std::endl; // for DEBUG
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
		//		std::cout << "[DesktopPanel] nativeEventFilter: KEYUP: " << msg->wParam << std::endl; // for DEBUG
		keyBuffer->put(VK_KANJI, KEYCODE_FLG_KEYUP);
		return true;
		break;
		//	  case VK_TAB:
	  case VK_CONVERT:
	  case VK_NONCONVERT:
	  case VK_OEM_ATTN:
		//		std::cout << "[DesktopPanel] nativeEventFilter: KEYUP: " << msg->wParam << std::endl; // for DEBUG
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
	  !(settings->getOnControl()) || settings->getOnViewerMode()){
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
	//	std::cout << "Scroll Up : " << settings->getDesktopOffsetY() << std::endl << std::flush;
	result = true;
  }
  else if (VK_Code == VK_DOWN){
	POS offsetY = settings->getDesktopOffsetY();
	offsetY -= 100;
	if (offsetY > settings->getDesktopHeight()){
	  offsetY = settings->getDesktopHeight();
	}
	settings->setDesktopOffsetY(offsetY);
	//	std::cout << "Scroll Down : " << settings->getDesktopOffsetY() << std::endl << std::flush;
	result = true;
  }
  else if (VK_Code == VK_LEFT){
	POS offsetX = settings->getDesktopOffsetX();
	offsetX += 100;
	if (offsetX < 0) offsetX = 0;
	settings->setDesktopOffsetX(offsetX);
	//	std::cout << "Scroll Left : " << settings->getDesktopOffsetX() << std::endl << std::flush;
	result = true;
  }
  else if (VK_Code == VK_RIGHT){
	POS offsetX = settings->getDesktopOffsetX();
	offsetX -= 100;
	if (offsetX > settings->getDesktopWidth()){
	  offsetX = settings->getDesktopWidth();
	}
	settings->setDesktopOffsetX(offsetX);
	//	std::cout << "Scroll Right : " << settings->getDesktopOffsetX() << std::endl << std::flush;
	result = true;
  }

  return result;
>>>>>>> master
}

// -----------------------------------------------------------------------------------
// 1 Finger Operation
// -----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------
// KeroRemote Compatible Operation (1 finger)
// -----------------------------------------------------------------------------------
bool DesktopPanel::oneFingerEventForKeroRemote(QTouchEvent *touchEvent)
{
  // last position of mouse cursor
  static QPointF lastPos;

  // open software panel check flags
  static bool inCheckingButtonOpen = false;
  static bool inCheckingKeyboardOpen = false;
  // for zoom
  static bool inZooming = false;
  static QDateTime pressTimeInZooming;

  const QTouchEvent::TouchPoint &touchPoint = touchEvent->touchPoints().first();

  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: Kero 1 fingers Pressed!";
	}

	// set TOP_TYPE
	topType = TOP_TYPE_1POINT;

	if (softwareButtonRect.contains(touchPoint.pos().toPoint())){
	  if (outputLog){
		qDebug() << "DP: into software button open area";
	  }
	  inCheckingButtonOpen = true;
	}
	else if (softwareKeyboardRect.contains(touchPoint.pos().toPoint())){
	  if (outputLog){
		qDebug() << "DP: into software keyboard open area";
	  }
	  inCheckingKeyboardOpen = true;
	}
	else if (inZooming){
	  // for release
	  pressTimeInZooming = QDateTime::currentDateTime();
	}
	else {
	  // move mouse cursor and press left button
	  qreal distance = QLineF(lastPos, touchPoint.pos()).length();
	  //qDebug() << "distance = " << distance;
	  if (lastPos.isNull() || distance > QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												 touchPoint.pos(),
												 Qt::NoButton,
												 Qt::NoButton,
												 Qt::NoModifier);

		mouseMoveEvent(moveEvent);
		delete moveEvent;
		lastPos = touchPoint.pos();
	  }
	  QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
												touchPoint.pos(),
												Qt::LeftButton,
												Qt::LeftButton,
												Qt::NoModifier);

	  mousePressEvent(pressEvent);
	  delete pressEvent;
	}
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: Kero 1 fingers Released!";
	}

<<<<<<< HEAD
	if (topType != TOP_TYPE_1POINT){
	  // Nothing to do
	}
	else if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // display software panel check
	  QPoint currentPos = touchPoint.pos().toPoint();
	  if (inCheckingButtonOpen &&
		  !softwareButtonRect.contains(currentPos, true)){
		// open software button
		qtbrynhildr->toggleSoftwareButton();
	  }
	  if (inCheckingKeyboardOpen &&
		  !softwareKeyboardRect.contains(currentPos, true)){
		// open software keyboard
		qtbrynhildr->toggleSoftwareKeyboard();
	  }
	}
	else if (inZooming){
	  // check tap
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 tapTime = pressTimeInZooming.msecsTo(currentTime);
	  if (tapTime < QTB_TOUCHPANEL_TAP_TIME_THRESHOLD){
		// tap
		// move mouse cursor and press left button and release left button
		qreal distance = QLineF(lastPos, touchPoint.pos()).length();
		//qDebug() << "distance = " << distance;
		if (lastPos.isNull() || distance > QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		  QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												   touchPoint.pos(),
												   Qt::NoButton,
												   Qt::NoButton,
												   Qt::NoModifier);
=======
  // create QTextStrem
  QTextStream *keyboardLogFileStream = new QTextStream(keyboardLogFile);
#if QT_VERSION < 0x060000
  keyboardLogFileStream->setCodec("UTF-8");
#endif // QT_VERSION < 0x060000
>>>>>>> master

		  mouseMoveEvent(moveEvent);
		  delete moveEvent;
		  lastPos = touchPoint.pos();
		}
		QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);

		mousePressEvent(pressEvent);
		delete pressEvent;

		// release left button
		QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													touchPoint.pos(),
													Qt::LeftButton,
													Qt::LeftButton,
													Qt::NoModifier);

		mouseReleaseEvent(releaseEvent);
		delete releaseEvent;
	  }
	}
	else {
	  // release left button
	  QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
												  touchPoint.pos(),
												  Qt::LeftButton,
												  Qt::LeftButton,
												  Qt::NoModifier);

	  mouseReleaseEvent(releaseEvent);
	  delete releaseEvent;
	}

	// reset open software panel check flags
	inCheckingButtonOpen = false;
	inCheckingKeyboardOpen = false;
	inZooming = settings->getDesktopScalingFactor() > getScalingFactorForFullScreen();
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: Kero 1 finger Moved!";
	}

	if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // Nothing to do
	}
	else if (inZooming){
	  // scroll desktop
	  QPoint currentPos = touchPoint.pos().toPoint();
	  QPoint lastPos = touchPoint.lastPos().toPoint();
	  QPoint move = lastPos - currentPos;
#if 0 // Yet
	  horizontalScrollBar()->setValue(horizontalScrollBar()->value() + move.x());
	  verticalScrollBar()->setValue(verticalScrollBar()->value() + move.y());
#endif //0 // Yet
	}
	else {
	  // move mouse cursor
	  qreal distance = QLineF(lastPos, touchPoint.pos()).length();
	  //qDebug() << "distance = " << distance;
	  if (lastPos.isNull() || distance > QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												 touchPoint.pos(),
												 Qt::NoButton,
												 Qt::NoButton,
												 Qt::NoModifier);
		mouseMoveEvent(moveEvent);
		delete moveEvent;
		lastPos = touchPoint.pos();
	  }
	}
  }

  return true;
}

// -----------------------------------------------------------------------------------
// QtBrynhildr Operation (1 finger)
// -----------------------------------------------------------------------------------
bool DesktopPanel::oneFingerEventForQtBrynhildr(QTouchEvent *touchEvent)
{
  // open software panel check flags
  static bool inCheckingButtonOpen = false;
  static bool inCheckingKeyboardOpen = false;
  // for zoom
  static bool inZooming = false;

  const QTouchEvent::TouchPoint &touchPoint = touchEvent->touchPoints().first();

  QPoint currentPos = touchPoint.pos().toPoint();
  QPoint lastPos = touchPoint.lastPos().toPoint();

  // set TOP_TYPE
  topType = TOP_TYPE_1POINT;

  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: Qt 1 fingers Pressed!";
	}

	// set topType
	topType = TOP_TYPE_1POINT;

	// check for software keyboard/button
	if (softwareButtonRect.contains(currentPos)){
	  if (outputLog){
		qDebug() << "DP: into software button open area";
	  }
	  inCheckingButtonOpen = true;
	}
	else if (softwareKeyboardRect.contains(currentPos)){
	  if (outputLog){
		qDebug() << "DP: into software keyboard open area";
	  }
	  inCheckingKeyboardOpen = true;
	}
  }
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){ // Release
	if (outputLog){
	  qDebug() << "DP: 1 Released!";
	}

	if (topType != TOP_TYPE_1POINT){
	  // Nothing to do
	}
	else if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // display software panel check
	  if (inCheckingButtonOpen &&
		  !softwareButtonRect.contains(currentPos, true)){
		// open software button
		qtbrynhildr->toggleSoftwareButton();
	  }
	  if (inCheckingKeyboardOpen &&
		  !softwareKeyboardRect.contains(currentPos, true)){
		// open software keyboard
		qtbrynhildr->toggleSoftwareKeyboard();
	  }
	}
	else {
	  qreal distance = QLineF(touchPoint.startPos(), touchPoint.pos()).length();
	  if (distance < QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		if (!settings->getOnShowSoftwareButton()){
		  QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												   touchPoint.pos(),
												   Qt::NoButton,
												   Qt::NoButton,
												   Qt::NoModifier);
		  QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
													touchPoint.pos(),
													Qt::LeftButton,
													Qt::LeftButton,
													Qt::NoModifier);
		  QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													  touchPoint.pos(),
													  Qt::LeftButton,
													  Qt::LeftButton,
													  Qt::NoModifier);
		  // move + L mouse button
		  mouseMoveEvent(moveEvent);
		  mousePressEvent(pressEvent);
		  mouseReleaseEvent(releaseEvent);
		  delete moveEvent;
		  delete pressEvent;
		  delete releaseEvent;
		}
	  }
	}

	// reset open software panel check flags
	inCheckingButtonOpen = false;
	inCheckingKeyboardOpen = false;
	inZooming = settings->getDesktopScalingFactor() > getScalingFactorForFullScreen();
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: Qt 1 fingers Released!";
	}

	if (topType != TOP_TYPE_1POINT){
	  // Nothing to do
	}
	else if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // display software panel check
	  if (inCheckingButtonOpen &&
		  !softwareButtonRect.contains(currentPos, true)){
		// open software button
		qtbrynhildr->toggleSoftwareButton();
	  }
	  if (inCheckingKeyboardOpen &&
		  !softwareKeyboardRect.contains(currentPos, true)){
		// open software keyboard
		qtbrynhildr->toggleSoftwareKeyboard();
	  }
	}
	else {
	  qreal distance = QLineF(touchPoint.startPos(), touchPoint.pos()).length();
	  if (distance < QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD){
		if (!settings->getOnShowSoftwareButton()){
		  QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
												   touchPoint.pos(),
												   Qt::NoButton,
												   Qt::NoButton,
												   Qt::NoModifier);
		  QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
													touchPoint.pos(),
													Qt::LeftButton,
													Qt::LeftButton,
													Qt::NoModifier);
		  QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													  touchPoint.pos(),
													  Qt::LeftButton,
													  Qt::LeftButton,
													  Qt::NoModifier);
		  // move + L mouse button
		  mouseMoveEvent(moveEvent);
		  mousePressEvent(pressEvent);
		  mouseReleaseEvent(releaseEvent);
		  delete moveEvent;
		  delete pressEvent;
		  delete releaseEvent;
		}
	  }
	}

	// reset open software panel check flags
	inCheckingButtonOpen = false;
	inCheckingKeyboardOpen = false;
	inZooming = settings->getDesktopScalingFactor() > getScalingFactorForFullScreen();
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: Qt 1 finger Moved!";
	}

	if (inCheckingButtonOpen || inCheckingKeyboardOpen){
	  // Nothing to do
	}
	else if (inZooming){
	  // scroll graphics view
	  QPoint move = lastPos - currentPos;
#if 0 // Yet
	  horizontalScrollBar()->setValue(horizontalScrollBar()->value() + move.x());
	  verticalScrollBar()->setValue(verticalScrollBar()->value() + move.y());
#endif //0 // Yet
	}
	else {
	  // move mouse cursor
	  QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove,
											   touchPoint.pos(),
											   Qt::NoButton,
											   Qt::NoButton,
											   Qt::NoModifier);
	  mouseMoveEvent(moveEvent);
	  delete moveEvent;
	}
  }

  return true;
}

// -----------------------------------------------------------------------------------
// 2 Finger Operation
// -----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------
// KeroRemote Compatible Operation (2 finger)
// -----------------------------------------------------------------------------------
bool DesktopPanel::twoFingerEventForKeroRemote(QTouchEvent *touchEvent)
{
  // check software panel
  if (settings->getOnShowSoftwareKeyboard() || settings->getOnShowSoftwareButton()){
	// Nothing to do
	return true;
  }

  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: Kero 2 fingers Pressed!";
	}

	// set topType
	topType = TOP_TYPE_2POINT;
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: Kero 2 fingers Released!";
	}
	// Nothing to do
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: Kero 2 finger Moved!";
	}
	// change scaling factor
	const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
	const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
	qreal currentScalingFactor =
	  QLineF(touchPoint0.pos(), touchPoint1.pos()).length() /
	  QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
	if (currentScalingFactor < 1.0){
	  scalingFactor -= 0.02;
	  qreal scalingFactorForFullScreen = getScalingFactorForFullScreen();
	  if (scalingFactor < scalingFactorForFullScreen){
		scalingFactor = scalingFactorForFullScreen;
	  }
	}
	else {
	  scalingFactor += 0.02;
#if QTB_TEST
	  if (scalingFactor > 1.0) scalingFactor = 1.0;
#endif // QTB_TEST
	}
	settings->setDesktopScalingFactor(scalingFactor);
	scalingFactor = settings->getDesktopScalingFactor();
	setScale(scalingFactor);
  }

  return true;
}

// -----------------------------------------------------------------------------------
// QtBrynhildr Operation (2 finger)
// -----------------------------------------------------------------------------------
bool DesktopPanel::twoFingerEventForQtBrynhildr(QTouchEvent *touchEvent)
{
  // check software panel
  if (settings->getOnShowSoftwareKeyboard() || settings->getOnShowSoftwareButton()){
	// Nothing to do
	return true;
  }

  QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: Qt 2 fingers Pressed!";
	}
	topType = TOP_TYPE_2POINT;
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: Qt 2 fingers Released!";
	}
	if (topType != TOP_TYPE_2POINT){
	  // Nothing to do
	}
	else {
	  const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
	  const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
	  int distance0 = QLineF(touchPoint0.startPos(), touchPoint0.pos()).length();
	  int distance1 = QLineF(touchPoint1.startPos(), touchPoint1.pos()).length();
	  if (distance0 < QTB_TOUCHPANEL_2TAP_DIST_THRESHOLD &&
		  distance1 < QTB_TOUCHPANEL_2TAP_DIST_THRESHOLD){
		if (outputLog){
		  qDebug() << "DP: 2 Point Tap!!";
		}

		QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
												  touchPoint0.pos(),
												  Qt::RightButton,
												  Qt::RightButton,
												  Qt::NoModifier);
		QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,
													touchPoint0.pos(),
													Qt::RightButton,
													Qt::RightButton,
													Qt::NoModifier);
		// R mouse button
		mousePressEvent(pressEvent);
		mouseReleaseEvent(releaseEvent);
		delete pressEvent;
		delete releaseEvent;
	  }	  
	}
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: Qt 2 finger Moved!";
	}

	// change scaling factor
	const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
	const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
	qreal currentScalingFactor =
	  QLineF(touchPoint0.pos(), touchPoint1.pos()).length() /
	  QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
	if (currentScalingFactor < 1.0){
	  scalingFactor -= 0.02;
	  qreal scalingFactorForFullScreen = getScalingFactorForFullScreen();
	  if (scalingFactor < scalingFactorForFullScreen){
		scalingFactor = scalingFactorForFullScreen;
	  }
	}
	else {
	  scalingFactor += 0.02;
#if QTB_TEST
	  if (scalingFactor > 1.0) scalingFactor = 1.0;
#endif // QTB_TEST
	}
	settings->setDesktopScalingFactor(scalingFactor);
	scalingFactor = settings->getDesktopScalingFactor();
	setScale(scalingFactor);
  }

  return true;
}

// -----------------------------------------------------------------------------------
// 3 Finger Operation
// -----------------------------------------------------------------------------------
bool DesktopPanel::threeFingerEvent(QTouchEvent *touchEvent)
{
  //---------------------------------------------------------------------------------
  // Press
  //---------------------------------------------------------------------------------
  if (touchEvent->touchPointStates() & Qt::TouchPointPressed){
	if (outputLog){
	  qDebug() << "DP: 3 fingers Pressed!";
	}

	// set topType
	topType = TOP_TYPE_3POINT;

	// press
	//keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN); // Windows key press
  }
  //---------------------------------------------------------------------------------
  // Release
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointReleased){
	if (outputLog){
	  qDebug() << "DP: 3 fingers Released!";
	}

	if (topType == TOP_TYPE_3POINT){
	  // press
	  keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN); // Windows key press
	  // release
	  keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYUP); // Windows key release
	}
  }
  //---------------------------------------------------------------------------------
  // Move
  //---------------------------------------------------------------------------------
  else if (touchEvent->touchPointStates() & Qt::TouchPointMoved){
	if (outputLog){
	  qDebug() << "DP: 3 finger Moved!";
	}
	// Nothing to do
  }

  return true;
}

// mouse event 
void DesktopPanel::mousePressEvent(QMouseEvent *event)
{
  QPoint pos = event->pos();
  if (convertToDesktop(pos)){
	//	qDebug() << "DP: mousePressEvent: pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanelWidget->mousePressEvent(newEvent);
	// delete newEvent;
  }
}

void DesktopPanel::mouseReleaseEvent(QMouseEvent *event)
{
  QPoint pos = event->pos();
  if (convertToDesktop(pos)){
	//	qDebug() << "DP: mouseReleaseEvent: pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanelWidget->mouseReleaseEvent(newEvent);
	// delete newEvent;
  }
}

void DesktopPanel::mouseDoubleClickEvent(QMouseEvent *event)
{
  QPoint pos = event->pos();
  if (convertToDesktop(pos)){
	//	qDebug() << "DP: mouseDoubleClickEvent: pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanelWidget->mouseDoubleClickEvent(newEvent);
	// delete newEvent;
  }
}

void DesktopPanel::mouseMoveEvent(QMouseEvent *event)
{
  QPoint pos = event->pos();
  if (convertToDesktop(pos)){
	//	qDebug() << "DP: mouseMoveEvent: pos of desktop = " << pos;
	QMouseEvent *newEvent = new QMouseEvent(event->type(),
											QPointF(pos),
											event->button(),
											event->buttons(),
											event->modifiers());
	desktopPanelWidget->mouseMoveEvent(newEvent);
	// delete newEvent;
  }
  else {
	QScrollArea::mouseMoveEvent(event);
  }
}

void DesktopPanel::wheelEvent(QWheelEvent *event)
{
  //qDebug() << "DP: wheelEvent : " << event;
  desktopPanelWidget->wheelEvent(event);
}

// keyboard event
void DesktopPanel::keyPressEvent(QKeyEvent *event)
{
  //qDebug() << "DP: KeyPress : " << event;
  desktopPanelWidget->keyPressEvent(event);
}

void DesktopPanel::keyReleaseEvent(QKeyEvent *event)
{
  //qDebug() << "DP: KeyRelease : " << event;
  desktopPanelWidget->keyReleaseEvent(event);
}

// convert to desktop
bool DesktopPanel::convertToDesktop(QPoint &point)
{
  // Yet
}

} // end of namespace qtbrynhildr
