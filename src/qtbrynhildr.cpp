// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>
#if 1 // copy for TEST
#include <cstdio>
#endif
//#include <iostream>

// Qt Header
#if 0 // for TEST
#include <QApplication>
#endif // for TEST
#include <QByteArray>
#include <QCloseEvent>
#include <QDir>
#include <QDesktopServices>
#if QTB_RECORDER || QTB_PUBLIC_MODE6_SUPPORT
#include <QFileDialog>
#endif // QTB_RECORDER || QTB_PUBLIC_MODE6_SUPPORT
#include <QLocale>
#include <QMenuBar>
#include <QMessageBox>
#include <QRect>
#include <QStatusBar>

// Local Header
#include "qtbrynhildr.h"
#include "parameters.h"
#include "settings.h"
#include "version.h"

#if 0 // for TEST Desktop Image Capture
#include <QPixmap>
#include <QScreen>
#include <QWindow>
#include <QWidget>
#endif // for TEST

namespace qtbrynhildr {

#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
// socket for control
SOCKET sock_control = INVALID_SOCKET;
// socket for graphics
SOCKET sock_graphics = INVALID_SOCKET;
// socket for sound
SOCKET sock_sound = INVALID_SOCKET;
#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

// counter for control
int counter_control = 0;
// counter for graphics
int counter_graphics = 0;

// date format
const QString dateFormat = QTB_LOG_DATE_FORMAT;

// constructor
#if QTB_PUBLIC_MODE6_SUPPORT
QtBrynhildr::QtBrynhildr(Option *option, QClipboard *clipboard)
#else // QTB_PUBLIC_MODE6_SUPPORT
QtBrynhildr::QtBrynhildr(Option *option)
#endif // QTB_PUBLIC_MODE6_SUPPORT
  :
  scrollArea(0),
  mainWindow(0),
  connectionLabel(0),
  frameRateLabel(0),
  fileMenu(0),
  displayMenu(0),
  videoMenu(0),
  selectMonitorNoSubMenu(0),
  selectFrameRateSubMenu(0),
  soundMenu(0),
  soundCacheSubMenu(0),
  controlMenu(0),
  sendKeySubMenu(0),
  selectPublicModeVersionSubMenu(0),
#if QTB_RECORDER
  recordAndReplaySubMenu(0),
#endif // QTB_RECORDER
  optionMenu(0),
  modeSubMenu(0),
  inTestingSubMenu(0),
#if QTB_DESKTOP_COMPRESS_MODE // for TEST
  desktopCompressModeSubMenu(0),
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST
  helpMenu(0),
  connectToServer_Action(0),
  disconnectToServer_Action(0),
  outputKeyboardLog_Action(0),
  outputLog_Action(0),
  exit_Action(0),
  about_Action(0),
  checkUpdate_Action(0),
  videoQuality_MINIMUM_Action(0),
  videoQuality_LOW_Action(0),
  videoQuality_STANDARD_Action(0),
  videoQuality_HIGH_Action(0),
  videoQuality_MAXIMUM_Action(0),
  showMenuBar_Action(0),
  showStatusBar_Action(0),
  showFrameRate_Action(0),
  fullScreen_Action(0),
  staysOnTop_Action(0),
  desktopScaleFixed_Action(0),
  windowSizeFixed_Action(0),
  desktopScalingDialog_Action(0),
  desktopCapture_Action(0),
  logViewDialog_Action(0),
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  showSoftwareKeyboard_Action(0),
  showSoftwareButton_Action(0),
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  selectFrameRate5_Action(0),
  selectFrameRate10_Action(0),
  selectFrameRate20_Action(0),
  selectFrameRate30_Action(0),
  selectFrameRate40_Action(0),
  selectFrameRate50_Action(0),
  selectFrameRate60_Action(0),
  selectFrameRateMaximum_Action(0),
  selectMonitorNo1_Action(0),
  selectMonitorNo2_Action(0),
  selectMonitorNo3_Action(0),
  selectMonitorNo4_Action(0),
  selectMonitorNo5_Action(0),
  selectMonitorNo6_Action(0),
  selectMonitorNo7_Action(0),
  selectMonitorNo8_Action(0),
  selectMonitorNo9_Action(0),
  selectMonitorNoAll_Action(0),
  soundQuality_MINIMUM_Action(0),
  soundQuality_LOW_Action(0),
  soundQuality_STANDARD_Action(0),
  soundQuality_HIGH_Action(0),
  soundQuality_MAXIMUM_Action(0),
  soundCache_0_Action(0),
  soundCache_1_Action(0),
  soundCache_2_Action(0),
  soundCache_3_Action(0),
  soundCache_4_Action(0),
  soundCache_5_Action(0),
  onControl_Action(0),
  onGraphics_Action(0),
  onSound_Action(0),
  selectPublicModeVersion5_Action(0),
  selectPublicModeVersion6_Action(0),
  selectPublicModeVersion7_Action(0),
#if QTB_RECORDER
  startRecordingControl_Action(0),
  stopRecordingControl_Action(0),
  startReplayRecordingControl_Action(0),
  stopReplayRecordingControl_Action(0),
#endif // QTB_RECORDER
#if QTB_EXTRA_BUTTON_SUPPORT
  onPluginsDisable_Action(0),
#endif // QTB_EXTRA_BUTTON_SUPPORT
  sendKey1_Action(0),
  sendKey2_Action(0),
  sendKey3_Action(0),
  sendKey4_Action(0),
  sendKey5_Action(0),
  sendKey6_Action(0),
  onScrollMode_Action(0),
#if QTB_PUBLIC_MODE6_SUPPORT
  sendClipboard_Action(0),
  sendFile_Action(0),
  cancelFileTransferring_Action(0),
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if QTB_DESKTOP_COMPRESS_MODE // for TEST
  desktopCompressMode0_Action(0),
  desktopCompressMode2_Action(0),
  desktopCompressMode4_Action(0),
  desktopCompressMode8_Action(0),
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST
#if QTB_PREFERENCE
  preferences_Action(0),
#endif // QTB_PREFERENCE
  connectToServerDialog(0),
  desktopScalingDialog(0),
  logViewDialog(0),
#if QTB_PREFERENCE
  preferenceDialog(0),
#endif // QTB_PREFERENCE
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  softwareKeyboard(0),
  softwareButton(0),
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  totalFrameCounter(0),
  currentFrameRate(0),
  currentDataRate(0),
  option(option),
  iniFileName(0),
  settings(0),
  writeSettingsAtExit(true),
#if QTB_CRYPTOGRAM
  cipher(0),
#endif // QTB_CRYPTGRAM
#if QTB_RECORDER
  recorder(0),
#endif // QTB_RECORDER
  httpGetter(0),
  logMessage(new LogMessage(this)),
  controlThread(0),
  graphicsThread(0),
  soundThread(0),
#ifdef USE_KEYLAYOUTFILE
  keyLayoutFileManager(0),
  keyLayoutFileReader(0),
#endif // USE_KEYLAYOUTFILE
  eventConverter(0),
#if QTB_PUBLIC_MODE6_SUPPORT
  clipboard(clipboard),
#endif // QTB_PUBLIC_MODE6_SUPPORT
  fullScreenMode(false),
  onSetDesktopScalingFactorForFullScreen(false),
  onShowMenuBar(false),
  onShowStatusBar(false),
#if QTB_PUBLIC_MODE6_SUPPORT
  progressBar(0),
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if 0 // for TEST
  heightOfTitleBar(0),
#endif // for TEST
  heightOfMenuBarInHiding(0),
  heightOfStatusBarInHiding(0),
  heightOfMenuBar(0),
  heightOfStatusBar(0),
  onControl(true),
  onGraphics(true),
  onSound(true),
  timer(0),
  onCheckUpdateInBackground(false),
  // for DEBUG
  outputLog(false)
{
#if 0 // for TEST Desktop Image Capture
  QScreen *screen = QGuiApplication::primaryScreen();
  if (screen != 0){
	cout << "primaryScreen(): OK" << endl << flush;
  }
  else {
	cout << "primaryScreen(): NG" << endl << flush;
  }
  const QWindow *window = windowHandle();
  if (window != 0){
	cout << "windowHandle(): OK" << endl << flush;
	screen = window->screen();
  }
  else {
	cout << "windowHandle(): NG" << endl << flush;
  }
  if (screen == 0){
	cout << "capture: NG" << endl << flush;
  }
  else {
	cout << "capture: OK" << endl << flush;
	QDateTime beginTime = QDateTime::currentDateTime();
	QPixmap pixmap;
	for (int i = 0 ; i < 1000; i++){
	  pixmap = screen->grabWindow(0);
	}
	QDateTime endTime = QDateTime::currentDateTime();
	qint64 diffSeconds = endTime.toMSecsSinceEpoch() - beginTime.toMSecsSinceEpoch();
	cout << "diff time = " << diffSeconds << " msecs." << endl << flush;
	pixmap.save("jpg/desktop.jpg", "jpg", 75);
  }
#endif // for TEST

  // bootTime
  bootTime = QDateTime::currentDateTime();

#if QTB_CRYPTOGRAM
  // create cipher
#if 1 // simple description
  cipher = new Cipher(qPrintable(bootTime.toString()));
#else
  QByteArray bootTimeStr = bootTime.toString().toUtf8();
  cipher = new Cipher(bootTimeStr.constData());
#endif
#endif // QTB_CRYPTOGRAM

  // set init file
  if (option->getIniFileName() != 0){
	iniFileName = option->getIniFileName();
  }

  // check shutdown flag
  if (option->getShutdownFlag()){
	// nothing to do -> shutdown now
	return;
  }

  // create setting
#if QTB_CRYPTOGRAM
  settinegs = new Settings(iniFileName, cipher);
#else // QTB_CRYPTGRAM
  settings = new Settings(iniFileName);
#endif // QTB_CRYPTGRAM

  // restore settings
  readSettings();

  // open Log File
  if (!logMessage->openLogFile(settings->getLogFile())){
	// Failed to open log file
#if 1 // for DEBUG
	QMessageBox::warning(this,
						 tr("warning"),
						 tr("Failed to open log file"),
						 QMessageBox::Ok,
						 QMessageBox::Ok);
#endif
  }

  // initialize platform
  if (!initPlatform()){
	// Failed to initialize platform
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "error: initPlatform()");
  }

  // set option to settings
#if QTB_PUBLIC_MODE6_SUPPORT
  if (option->getPublicModeVersion() > 0){
	writeSettingsAtExit = false;
	settings->setPublicModeVersion(option->getPublicModeVersion());
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT
  if (option->getServerName() != 0){
	writeSettingsAtExit = false;
	settings->setServerName(option->getServerName());
  }
  if (option->getPortNo() > 0){
	writeSettingsAtExit = false;
	settings->setPortNo(option->getPortNo());
  }
  if (option->getPassword() != 0){
	writeSettingsAtExit = false;
	settings->setPassword(option->getPassword());
  }
  if (option->getHostType() != 0){
	writeSettingsAtExit = false;
	settings->setServerType(option->getHostType());
  }
  if (option->getDebug() != 0){
	writeSettingsAtExit = false;
	if (strncmp(option->getDebug(), "on", sizeof("on")) == 0){
	  settings->setOutputLog(true);
	}
	else {
	  settings->setOutputLog(false);
	}
  }

  // full screen
  if (option->getFullScreenFlag()){
	settings->setOnFullScreenAtConnected(true);
  }

#if QTB_CELT_SUPPORT
  // celt
  if (option->getCELTFlag()){
	settings->setSoundType(SOUND_TYPE_CELT);
  }
#endif // QTB_CELT_SUPPORT

#if QTB_RECORDER
  // record
  settings->setOnRecordingControl(option->getRecordingFlag());
  if (option->getRecordingFlag()){
	settings->setRecordingControlFileName(option->getRecordingFileName());
  }
  // replay
  settings->setOnReplayingControl(option->getReplayingFlag());
  if (option->getReplayingFlag()){
	settings->setReplayingControlFileName(option->getReplayingFileName());
  }

  // recorder
  recorder = new Recorder(settings);
  if (settings->getOnReplayingControl()){
	recorder->startReplaying();
  }
  else if (settings->getOnRecordingControl()){
	recorder->startRecording();
  }
#endif // QTB_RECORDER

  // create http getter
  httpGetter = new HttpGetter();
  if (!httpGetter->supportsSsl()){
	cout << "NOT support OpenSSL" << endl << flush;
  }
#if 0 // for DEBUG
  else {
	cout << "support OpenSSL" << endl << flush;
  }
#endif // for DEBUG
  connect(httpGetter, SIGNAL(finished()), SLOT(finishedDownload()));

  // version
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "Version    : v" QTB_VERSION QTB_RCNAME);
  // kernel type
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "OS Type    : " + QSysInfo::kernelType());
  // kernel version
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "OS Version : " + QSysInfo::kernelVersion());

  // current product name
  if (settings->getOutputLog()){
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "[QtBrynhildr()] QSysInfo::productType() : " +
								 QSysInfo::productType());
  }

  // current path
  currentPath = QDir::currentPath();
  if (settings->getOutputLog()){
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "[QtBrynhildr()] CurrentPath: " +
								 currentPath);
  }
#if 0 // for DEBUG
  logMessage->informationMessage(PHASE_DEBUG,
								 "[QtBrynhildr()] CurrentPath: " +
								 currentPath,
								 QMessageBox::Ok,
								 QMessageBox::NoButton);
#endif

  // current system name
  if (settings->getOutputLog()){
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "[QtBrynhildr()] QLocale::system().name() : " +
								 QLocale::system().name());
  }
  // settings file name
  if (settings->getOutputLog()){
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "[QtBrynhildr()] Settings: " +
								 settings->getSettings()->fileName());
  }

  // desktop size information
  if (settings->getOutputLog()){
	QRect currentScreen = settings->getDesktop()->getCurrentScreen();
	int desktopWidth = currentScreen.width();
	int desktopHeight = currentScreen.height();
	logMessage->outputLogMessage(PHASE_DEBUG,
								 (QString)"Desktop Width  = " + QString::number(desktopWidth));
	logMessage->outputLogMessage(PHASE_DEBUG,
								 (QString)"Desktop Height = " + QString::number(desktopHeight));
  }

  // set current onControl/onGraphics/onSound
  onControl = settings->getOnControl();
  onGraphics = settings->getOnGraphics();
  onSound = settings->getOnSound();

  //------------------------------------------------------------
  // create window
  //------------------------------------------------------------
  // Main Window Widget
  mainWindow = new MainWindow(settings, this);

  // Scroll Area
  scrollArea = new QScrollArea;
  scrollArea->setWidgetResizable(true);
  scrollArea->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  //  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  //  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  // set Widget
  scrollArea->setWidget(mainWindow);
  scrollArea->setFocusProxy(mainWindow);
  setCentralWidget(scrollArea);
  // initialize palette
  originalPalette = fullScreenPalette = scrollArea->palette();
  // for original
  originalPalette.setColor(QPalette::Window, Qt::gray);
  scrollArea->setPalette(originalPalette); // change QPalette::Window to black
  // for full screen
  fullScreenPalette.setColor(QPalette::Window, Qt::black);

  // create Main Window
  createActions();
  createMenus();
  createContextMenu();
  createToolBars();
  createStatusBar();
  // show or hide
  menuBar()->setVisible(settings->getOnShowMenuBar());
  statusBar()->setVisible(settings->getOnShowStatusBar());
  // save onShow* flags
  onShowMenuBar = settings->getOnShowMenuBar();
  onShowStatusBar = settings->getOnShowStatusBar();

  // set icon
  //  setWindowIcon(QIcon(QTB_ICON_FILENAME));

  // set window title
#if QTB_PUBLIC_MODE6_SUPPORT
  //  setWindowTitle(tr(QTB_APPLICATION)+"  - MODE  " + QString::number(settings->getPublicModeVersion()) +" -");
  setWindowTitle(tr(QTB_APPLICATION)+"  - " + settings->getPublicModeAliasString() +" -");
#else // QTB_PUBLIC_MODE6_SUPPORT
  setWindowTitle(tr(QTB_APPLICATION));
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if 0 // for TEST
  // set height of title bar
  heightOfTitleBar = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
#endif // for TEST

  // set window flags
  Qt::WindowFlags flags = windowFlags();
  flags |= Qt::CustomizeWindowHint;
  flags |= Qt::WindowTitleHint;
  flags |= Qt::WindowSystemMenuHint;
  flags |= Qt::WindowCloseButtonHint;
  flags |= Qt::WindowMinimizeButtonHint;
#if 1 // for TEST
  flags |= Qt::WindowMaximizeButtonHint;
#else// for TEST
  flags &= ~Qt::WindowMaximizeButtonHint; // disable Maximize Button
#endif // for TEST
  if (settings->getOnFrameLessWindow()){
	flags |= Qt::FramelessWindowHint;
  }
  setWindowFlags(flags);

  // refresh window
  refreshWindow();

  // get window information
  heightOfMenuBar = menuBar()->sizeHint().height();
  heightOfStatusBar = statusBar()->sizeHint().height();
  if (settings->getOutputLog()){
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "menuBar height = " + QString::number(heightOfMenuBar));
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "statusBar height = " + QString::number(heightOfStatusBar));
  }
  mainWindow->setHeightOfMenuBar(heightOfMenuBar);
  mainWindow->setHeightOfStatusBar(heightOfStatusBar);
  heightOfMenuBarInHiding = settings->getDesktop()->getHeightOfMenuBarInHiding();
  heightOfStatusBarInHiding = settings->getDesktop()->getHeightOfStatusBarInHiding();

  // set up connect to server dialog
  connectToServerDialog = new ConnectToServerDialog(settings, this);
  connect(connectToServerDialog, SIGNAL(connectToServer()), SLOT(connectToServer()));

  // set up desktop scaling dialog
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog = new DesktopScalingDialog(settings, this);
  }

  // set up log view dialog
  if (QTB_LOG_VIEW){
	logViewDialog = new LogViewDialog(settings, this);
  }

#if QTB_PREFERENCE
  // preference dialog
  preferenceDialog = new PreferenceDialog(settings, this);
#endif // QTB_PREFERENCE

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // set up Software Button and Keyboard
  // keyboard
  //	softwareKeyboard = new SK(settings, mainWindow->getKeyBuffer(), this);
  softwareKeyboard = new SK(settings, mainWindow->getKeyBuffer(), mainWindow);
  softwareKeyboard->setVisible(false);
#if 0 // for TEST
  softwareKeyboard->setGeometry(40,350,1120,300);
  softwareKeyboard->setVisible(true);
#endif // for TEST

  // button
  //	softwareButton = new SB(settings, mainWindow->getMouseBuffer(), this);
  softwareButton = new SB(settings, mainWindow->getMouseBuffer(), mainWindow);
  softwareButton->setVisible(false);
  connect(softwareButton, SIGNAL(refreshMenu()), SLOT(refreshMenu()));
#if 0 // for TEST
  softwareButton->setGeometry(40,30,1188,450);
  softwareButton->setVisible(true);
#endif // for TEST
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON


#ifdef USE_KEYLAYOUTFILE
  // for Key Layout File
  QString keylayoutDirPath = settings->getKeylayoutPath();
  const char *dirPath = strdup(qPrintable(QDir::toNativeSeparators(keylayoutDirPath)));

  // Key Layout File Manager
  keyLayoutFileManager = new KeyLayoutFileManager(dirPath);

  // Key Layout File Reader
  //  QString keylayoutDirPath = QString(".") + QTB_KEYLAYOUT_FILE_PATH; // for TEST
  keyLayoutFileReader = new KeyLayoutFileReader(dirPath);
  connectToServerDialog->addKeyboardTypeList(keyLayoutFileReader->getKeyboardTypeList());
  if (settings->getKeyboardType() == KEYBOARD_TYPE_KLF){
	// set keyboard type (real index)
	KEYBOARD_TYPE keyboardType =
	  keyLayoutFileReader->getIndexOfKeyboardType(settings->getKeyboardTypeName());
	if (keyboardType >= 0){
	  keyboardType += KEYBOARD_TYPE_KLF;
	  settings->setKeyboardType(keyboardType);
	  connectToServerDialog->setKeyboardType(keyboardType);
	}
	else {
	  settings->setKeyboardType(0);
	  connectToServerDialog->setKeyboardType(0);
	}
  }
#endif // USE_KEYLAYOUTFILE

  // Event Converter
  eventConverter = new EventConverter();
  mainWindow->setEventConverter(eventConverter);

#if QTB_PUBLIC_MODE6_SUPPORT
  // clipboard dataChanged
  if (settings->getOnTransferClipboardSupport()){
	connect(clipboard, SIGNAL(dataChanged()), SLOT(sendClipboard()));
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT

  //------------------------------------------------------------
  // create threads
  //------------------------------------------------------------
#if QTB_RECORDER
  controlThread = new ControlThread(settings, mainWindow, recorder);
#else // QTB_RECORDER
  controlThread = new ControlThread(settings, mainWindow);
#endif // QTB_RECORDER
  graphicsThread = new GraphicsThread(settings);
  soundThread = new SoundThread(settings);

  // connect
  // all thread
  connect(controlThread,
		  SIGNAL(outputLogMessage(int)),
		  SLOT(outputLogMessage(int)));
  connect(controlThread,
		  SIGNAL(outputLogMessage(int, const QString)),
		  SLOT(outputLogMessage(int, const QString)));
  connect(graphicsThread,
		  SIGNAL(outputLogMessage(int)),
		  SLOT(outputLogMessage(int)));
  connect(graphicsThread,
		  SIGNAL(outputLogMessage(int, const QString)),
		  SLOT(outputLogMessage(int, const QString)));
  connect(soundThread,
		  SIGNAL(outputLogMessage(int)),
		  SLOT(outputLogMessage(int)));
  connect(soundThread,
		  SIGNAL(outputLogMessage(int, const QString)),
		  SLOT(outputLogMessage(int, const QString)));
  connect(controlThread,
		  SIGNAL(networkError(bool)),
		  SLOT(onNetworkError(bool)));
  connect(graphicsThread,
		  SIGNAL(networkError(bool)),
		  SLOT(onNetworkError(bool)));
  connect(soundThread,
		  SIGNAL(networkError(bool)),
		  SLOT(onNetworkError(bool)));

  // control thread
  connect(controlThread,
		  SIGNAL(connected()),
		  SLOT(connected()));

  connect(controlThread,
		  SIGNAL(refreshWindow()),
		  SLOT(refreshWindow()));

  connect(controlThread,
		  SIGNAL(refreshMenu()),
		  SLOT(refreshMenu()));

  connect(controlThread,
		  SIGNAL(finished()),
		  SLOT(finishedNetThread()));

  connect(controlThread,
		  SIGNAL(exitApplication()),
		  SLOT(exitApplication()));
#if QTB_PUBLIC_MODE6_SUPPORT
  connect(controlThread,
		  SIGNAL(setClipboard(QString)),
		  SLOT(setClipboard(QString)));
  connect(controlThread,
		  SIGNAL(setFileTransferProgressBarValue(int)),
		  SLOT(setFileTransferProgressBarValue(int)));
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if QTB_BRYNHILDR2_SUPPORT
  connect(controlThread,
		  SIGNAL(changeMouseCursor(const QCursor &)),
		  SLOT(changeMouseCursor(const QCursor &)));
#endif // QTB_BRYNHILDR2_SUPPORT

  // graphics thread
  connect(graphicsThread,
		  SIGNAL(desktopChanged(QImage)),
		  SLOT(onDesktopChanged(QImage)));
  connect(graphicsThread,
		  SIGNAL(desktopClear()),
		  SLOT(onDesktopClear()));

  // bootTime
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, tr("Bootup."));
  if (settings->getOutputLog())
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "Boot : " +
								 bootTime.toString(dateFormat));

  // check log message for debug
  if (settings->getOutputLog()){
	// information dialog
	logMessage->informationMessage(PHASE_QTBRYNHILDR,
								   tr("You are in Debug Logging mode."),
								   LogMessage::Ok,
								   LogMessage::NoButton);
  }

  // boot up
  if (option->getBootupFlag()){
	// connect to server now
	connectToServer();
	// clear boot flag
	option->setBootupFlag(false);
  }
  else if (settings->getOnOpenConnectToServerDialogAtBootup()){
	// pop up connect to server dialog
	this->show();
	popUpConnectToServer();
  }

  // initialize timer for main thread
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), SLOT(timerExpired()));
  timer->start(100); // 0.1 second tick timer

#if 0 // for TEST
  // initialize mouse cursor
  if (settings->getOnDisplayMouseCursor()){
	menuBar()->setCursor(cursor());
	changeMouseCursor(Qt::CrossCursor);
  }
  else {
	changeMouseCursor(Qt::ArrowCursor);
  }
#endif // for TEST

  // check update
  if (settings->getOnCheckUpdateAtBootup()){
	// check update
	onCheckUpdateInBackground = true;
	checkUpdate();
  }
}

// finished download
void QtBrynhildr::finishedDownload()
{
  // check update
  QByteArray byteArray = httpGetter->getPage();
  if (byteArray.size() == 0){
	cout << "finished downloading to file." << endl << flush;
	return;
  }

#if 0 // for TEST
  // save to file
  QFile file("release_page.html");
  file.open(QIODevice::WriteOnly);
  file.write(byteArray);
  file.close();
#endif // for TEST

  QString releasePage(byteArray);
#if 1
  // check latest release
  int startIndex = releasePage.indexOf(QTB_STRING_FOR_TAGSEARCH);
  if (startIndex > 0) {
	startIndex += qstrlen(QTB_STRING_FOR_TAGSEARCH);
	int lastIndex = releasePage.indexOf("\"", startIndex);
	//  cout << "startIndex = " << startIndex << endl << flush;
	//  cout << "lastIndex  = " << lastIndex << endl << flush;
	QStringRef tagRef(&releasePage, startIndex, lastIndex - startIndex);
	QString latestTag;
	latestTag.append(tagRef);
	//	  cout << "latest tag = v" << qPrintable(latestTag);

	startIndex = lastIndex + 2;
	lastIndex = releasePage.indexOf("<", startIndex);
	QStringRef verRef(&releasePage, startIndex, lastIndex - startIndex);
	QString ver;
	ver.append(verRef);
	//	  cout << " : ver = " << qPrintable(ver) << endl << flush;

	QString tag(option->getVersionString());
	//	  tag = "169";
	//	  cout << "current tag = v" <<  qPrintable(tag) << endl << flush;
	if (tag != latestTag){
	  // Found new version
	  //		cout << "Found new version" << endl << flush;
	  int ret = QMessageBox::question(this,
									  tr("Confirm"),
									  tr("Found new release. Open release page?"),
									  QMessageBox::Ok | QMessageBox::Cancel,
									  QMessageBox::Ok);
	  if (ret == QMessageBox::Ok){
		QDesktopServices::openUrl(QUrl(QTB_URL_FOR_RELEASE));
	  }
	}
	else {
	  if (!onCheckUpdateInBackground){
		// Up-to-date
		//		cout << "Up-to-date!" << endl << flush;
		QMessageBox::information(this, tr("Information"),
								 tr("Up-to-date!"));
	  }
	  else {
		// reset background mode
		onCheckUpdateInBackground = false;
	  }
	}
  }
  else {
	cout << "NOT Found tag!" << endl << flush;
  }
#else // for TEST
  // display all tag and version in release page
  int startIndex = 0;
  while(true){
	startIndex = releasePage.indexOf(QTB_STRING_FOR_TAGSEARCH, startIndex);
	if (startIndex < 0) break;

	startIndex += qstrlen(QTB_STRING_FOR_TAGSEARCH);
	int lastIndex = releasePage.indexOf("\"", startIndex);
	//  cout << "startIndex = " << startIndex << endl << flush;
	//  cout << "lastIndex  = " << lastIndex << endl << flush;
	QStringRef tagRef(&releasePage, startIndex, lastIndex - startIndex);
	QString tag;
	tag.append(tagRef);
	cout << "Found tag = v" << qPrintable(tag);
	startIndex = lastIndex;

	startIndex = lastIndex + 2;
	lastIndex = releasePage.indexOf("<", startIndex);
	QStringRef verRef(&releasePage, startIndex, lastIndex - startIndex);
	QString ver;
	ver.append(verRef);
	cout << " : ver = " << qPrintable(ver) << endl << flush;
	startIndex = lastIndex;
  }
#endif // for TEST

  // clear memory
  httpGetter->clear();
}

// destructor
QtBrynhildr::~QtBrynhildr()
{
  // shutdown timer for main thread
  if (timer != 0){
	timer->stop();
	delete timer;
	timer = 0;
  }

  if (settings != 0){
	// disconnect to server
	disconnectToServer();

	// disconnect signal slots
	disconnect();
  }

  // stop & delete threads
  if (controlThread != 0){
	// delete
	delete controlThread;
	controlThread = 0;
  }
  if (graphicsThread != 0){
	// delete
	delete graphicsThread;
	graphicsThread = 0;
  }
  if (soundThread != 0){
	// delete
	delete soundThread;
	soundThread = 0;
  }

  // Event Converter
  if (eventConverter != 0){
	delete eventConverter;
	eventConverter = 0;
  }

#ifdef USE_KEYLAYOUTFILE
  // Key Layout File Reader
  if (keyLayoutFileReader != 0){
	delete keyLayoutFileReader;
	keyLayoutFileReader = 0;
  }

  // Key Layout File Manager
  if (keyLayoutFileManager != 0){
	delete keyLayoutFileManager;
	keyLayoutFileManager = 0;
  }
#endif // USE_KEYLAYOUTFILE

#if QTB_CRYPTOGRAM
  // delete cipher
  if (cipher != 0){
	delete cipher;
	cipher = 0;
  }
#endif

  // http getter
  if (httpGetter != 0){
	delete httpGetter;
	httpGetter = 0;
  }

#if QTB_RECORDER
  // delete recorder
  if (recorder != 0){
	delete recorder;
	recorder = 0;
  }
#endif // QTB_RECORDER

  // settings
  if (settings != 0){
	delete settings;
	settings = 0;
  }

  // shutdown platform
  if (!shutdownPlatform()){
	// Failed to shutdown platform
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "error: shutdownPlatform()");
  }

  // close Log File
  logMessage->closeLogFile();
}

// get main window
MainWindow *QtBrynhildr::getMainWindow() const
{
  return mainWindow;
}

// exit full screen
void QtBrynhildr::exitFullScreen()
{
  if (fullScreenMode)
	fullScreen();
}

// get height of menu bar
int QtBrynhildr::getHeightOfMenuBar()
{
  if (heightOfMenuBar == 0)
	return 0;

  if (settings->getOnShowMenuBar()){
	return heightOfMenuBar;
  }
  else {
	return heightOfMenuBarInHiding;
  }
}

// get height of status bar
int QtBrynhildr::getHeightOfStatusBar()
{
  if (settings->getOnShowStatusBar()){
	return heightOfStatusBar;
  }
  else {
	return heightOfStatusBarInHiding;
  }
}

#if !defined(Q_OS_WIN)
// set cursor point color
void QtBrynhildr::setCursorPointColor(QRgb cursorPointColor)
{
  controlThread->setCursorPointColor(cursorPointColor);
}
#endif // !defined(Q_OS_WIN)

// refresh window
void QtBrynhildr::refreshWindow()
{
  // output log for Window Size
  if (settings->getOutputLog()){
	QSize size = mainWindow->getSize();
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "WindowSize : (" +
								 QString::number(size.width())  + "," +
								 QString::number(size.height()) +
								 ")");
  }

  // refresh desktop
  if (settings->getConnected()){
	mainWindow->resizeWindow();
  }

  // update status bar
  updateStatusBar();
}

// refresh menu
void QtBrynhildr::refreshMenu()
{
  // video quality
  refreshVideoQualityMenu();

  // sound quality
  refreshSoundQualityMenu();

  // monitor menu
  refreshMonitorMenu();

#if QTB_RECORDER
  // recording and replaying
  refreshRecordingAndReplayMenu();
#endif // QTB_RECORDER
}

// refresh video quality menu
void QtBrynhildr::refreshVideoQualityMenu()
{
  clearVideoQualityCheck();
  VIDEO_QUALITY videoQuality = settings->getVideoQuality();
  switch(videoQuality){
  case VIDEO_QUALITY_MINIMUM:
	videoQuality_MINIMUM_Action->setChecked(true);
	break;
  case VIDEO_QUALITY_LOW:
	videoQuality_LOW_Action->setChecked(true);
	break;
  case VIDEO_QUALITY_STANDARD:
	videoQuality_STANDARD_Action->setChecked(true);
	break;
  case VIDEO_QUALITY_HIGH:
	videoQuality_HIGH_Action->setChecked(true);
	break;
  case VIDEO_QUALITY_MAXIMUM:
	videoQuality_MAXIMUM_Action->setChecked(true);
	break;
  default:
	// error
	ABORT();
	break;
  }
}

// refresh sound quality menu
void QtBrynhildr::refreshSoundQualityMenu()
{
  clearSoundQualityCheck();
  SOUND_QUALITY soundQuality = settings->getSoundQuality();
  switch(soundQuality){
  case SOUND_QUALITY_MINIMUM:
	soundQuality_MINIMUM_Action->setChecked(true);
	break;
  case SOUND_QUALITY_LOW:
	soundQuality_LOW_Action->setChecked(true);
	break;
  case SOUND_QUALITY_STANDARD:
	soundQuality_STANDARD_Action->setChecked(true);
	break;
  case SOUND_QUALITY_HIGH:
	soundQuality_HIGH_Action->setChecked(true);
	break;
  case SOUND_QUALITY_MAXIMUM:
	soundQuality_MAXIMUM_Action->setChecked(true);
	break;
  default:
	// error
	ABORT();
	break;
  }
}

// refresh monitor menu
void QtBrynhildr::refreshMonitorMenu()
{
  MONITOR_COUNT monitorCount = settings->getMonitorCount();
  //  cout << "[QtBrynhildr] monitor_count=" << monitorCount << endl << flush;

  switch(monitorCount){
  case 9:
	// set available Monitor 9
	selectMonitorNo9_Action->setEnabled(true);
	// FALL THROUGH
  case 8:
	// set available Monitor 8
	selectMonitorNo8_Action->setEnabled(true);
	// FALL THROUGH
  case 7:
	// set available Monitor 7
	selectMonitorNo7_Action->setEnabled(true);
	// FALL THROUGH
  case 6:
	// set available Monitor 6
	selectMonitorNo6_Action->setEnabled(true);
	// FALL THROUGH
  case 5:
	// set available Monitor 5
	selectMonitorNo5_Action->setEnabled(true);
	// FALL THROUGH
  case 4:
	// set available Monitor 4
	selectMonitorNo4_Action->setEnabled(true);
	// FALL THROUGH
  case 3:
	// set available Monitor 3
	selectMonitorNo3_Action->setEnabled(true);
	// FALL THROUGH
  case 2:
	// set available Monitor 2
	selectMonitorNo2_Action->setEnabled(true);
	// FALL THROUGH
  case 1:
	// set available Monitor 1
	selectMonitorNo1_Action->setEnabled(true);
	// FALL THROUGH
  case MONITOR_NO_ALL:
	// set available Monitor All
	if (monitorCount > 1){
	  selectMonitorNoAll_Action->setEnabled(true);
	}
	break;
  case 0:
	// disabled all
	disabledSelectMonitor();
	break;
  default:
	// Error
	ABORT();
	break;
  }
}

#if QTB_RECORDER
// recording and replaying
void QtBrynhildr::refreshRecordingAndReplayMenu()
{
  if (settings->getConnected()){
	if (settings->getOnRecordingControl() || settings->getOnReplayingControl()){
	  startRecordingControl_Action->setEnabled(false);
	}
	else {
	  startRecordingControl_Action->setEnabled(true);
	}
	startRecordingControl_Action->setChecked(settings->getOnRecordingControl());
	if (settings->getOnRecordingControl()){
	  stopRecordingControl_Action->setEnabled(true);
	}
	else {
	  stopRecordingControl_Action->setEnabled(false);
	}
	if (settings->getOnRecordingControl() || settings->getOnReplayingControl()){
	  startReplayRecordingControl_Action->setEnabled(false);
	}
	else {
	  startReplayRecordingControl_Action->setEnabled(true);
	}
	startReplayRecordingControl_Action->setChecked(settings->getOnReplayingControl());
	if (settings->getOnReplayingControl()){
	  stopReplayRecordingControl_Action->setEnabled(true);
	}
	else {
	  stopReplayRecordingControl_Action->setEnabled(false);
	}
  }
  else {
	startRecordingControl_Action->setEnabled(false);
	stopRecordingControl_Action->setEnabled(false);
	startReplayRecordingControl_Action->setEnabled(false);
	stopReplayRecordingControl_Action->setEnabled(false);
	startRecordingControl_Action->setChecked(false);
	startReplayRecordingControl_Action->setChecked(false);
  }
}
#endif // QTB_RECORDER

// get shutdown flag
bool QtBrynhildr::getShutdownFlag() const
{
  return option->getShutdownFlag();
}

// desktop Changed
void QtBrynhildr::onDesktopChanged(QImage image)
{
  if (!settings->getConnected())
	return;

  // update desktop
  mainWindow->refreshDesktop(image);

  // set desktop scaling factor for full screen mode
  if (onSetDesktopScalingFactorForFullScreen){
	onSetDesktopScalingFactorForFullScreen = false;
	QSize screenSize = settings->getDesktop()->getCurrentScreen().size();
	setDesktopScalingFactor(screenSize);
  }

#if 0 // for TEST
  // update current frame rate
  if (settings->getOnShowFrameRate()){
	static int refreshCounter = 1;
	if (refreshCounter > QTB_STATUS_REFRESH_COUNT){
	  // frame rate
	  currentFrameRate = graphicsThread->getFrameRate();
	  // data rate
	  long controlDataRate = controlThread->getDataRate();
	  long graphicsDataRate = graphicsThread->getDataRate();
	  long soundDataRate = soundThread->getDataRate();
	  // Mbps
	  currentDataRate = ((double)(controlDataRate + graphicsDataRate + soundDataRate) * 8 / (1024*1024));
	  updateFrameRate();
	  refreshCounter = 1;
	}
	refreshCounter++;
  }
#endif // for TEST
}

// desktop clear
void QtBrynhildr::onDesktopClear()
{
  mainWindow->clearDesktop();
  refreshWindow();
}

#if QTB_BRYNHILDR2_SUPPORT
// change mouse cursor
void QtBrynhildr::changeMouseCursor(const QCursor &cursor)
{
  setCursor(cursor);
}
#endif // QTB_BRYNHILDR2_SUPPORT

// network error handler
void QtBrynhildr::onNetworkError(bool doRetry)
{
  if (doRetry){
	reconnectToServer();
  }
  else {
	disconnected();
  }
}

// exit applilcation
void QtBrynhildr::exitApplication()
{
  exit();
}

#if QTB_PUBLIC_MODE6_SUPPORT
// set clipboard
void QtBrynhildr::setClipboard(QString clipboardString)
{
  if (clipboard != 0){
	//	cout << "setClipboard = " << qPrintable(clipboardString) << endl << flush;
	clipboard->setText(clipboardString);
	//	cout << "Clipboard = " << qPrintable(clipboard->text()) << endl << flush;
  }
#if 0 // for TEST
  else {
	cout << "clipboard is null" << endl << flush;
  }
#endif // for TEST
}

// set progress bar value for transfer file
void QtBrynhildr::setFileTransferProgressBarValue(int value)
{
  if (value > 0){
	progressBar->setVisible(true);
  }

  if (value >= 100){
	progressBar->setVisible(false);

	// enable cancel file transferring menu
	cancelFileTransferring_Action->setEnabled(false);
  }
  else {
	progressBar->setValue(value);

	// disable cancel file transferring menu
	if (!cancelFileTransferring_Action->isEnabled())
	  cancelFileTransferring_Action->setEnabled(true);
  }
}
#endif // QTB_PUBLIC_MODE6_SUPPORT

// output Log Message
void QtBrynhildr::outputLogMessage(int msgID)
{
  static int currentMsgID = -1;
  if (currentMsgID != msgID){
	currentMsgID = msgID;
	logMessage->outputMessage(msgID);
	currentMsgID = -1;
  }
}

// output Log Message
void QtBrynhildr::outputLogMessage(int id, const QString text)
{
  logMessage->outputLogMessage((PHASE_ID)id, text);
}

// create Menu Action
void QtBrynhildr::createActions()
{
  // connect to server
  connectToServer_Action = new QAction(tr("Connect"), this);
  connectToServer_Action->setStatusTip(tr("Connec to Brynhildr Server"));
  //  connectToServer_Action->setShortcut(tr("Ctrl+C"));
  connect(connectToServer_Action, SIGNAL(triggered()), this, SLOT(popUpConnectToServer()));

  // disconnect to server
  disconnectToServer_Action = new QAction(tr("Disconnect"), this);
  disconnectToServer_Action->setStatusTip(tr("Disconnect to Brynhildr Server"));
  //  connectToServer_Action->setShortcut(tr("Ctrl+D"));
  disconnectToServer_Action->setEnabled(false);
  connect(disconnectToServer_Action, SIGNAL(triggered()), this, SLOT(popUpDisconnectToServer()));

  // output keyboardlog Action
  outputKeyboardLog_Action = new QAction(tr("Output Keyboard Log"), this);
  outputKeyboardLog_Action->setCheckable(true);
  outputKeyboardLog_Action->setChecked(settings->getOutputKeyboardLog());
  outputKeyboardLog_Action->setStatusTip(tr("Output Keyboard Log"));
  connect(outputKeyboardLog_Action, SIGNAL(triggered()), this, SLOT(toggleOutputKeyboardLog()));

  // output log Action
  outputLog_Action = new QAction(tr("Output Log"), this);
  outputLog_Action->setCheckable(true);
  outputLog_Action->setChecked(settings->getOutputLog());
  outputLog_Action->setStatusTip(tr("Output Log"));
  connect(outputLog_Action, SIGNAL(triggered()), this, SLOT(toggleOutputLog()));

  // exit Action
  exit_Action = new QAction(tr("Exit"), this);
  exit_Action->setStatusTip(tr("Exit Qt Brynhildr"));
  //  exit_Action->setShortcut(tr("Ctrl+E"));
  exit_Action->setMenuRole(QAction::QuitRole);
  connect(exit_Action, SIGNAL(triggered()), this, SLOT(exit()));

  // about Action
  about_Action = new QAction(tr("About"), this);
  about_Action->setStatusTip(tr("About Qt Brynhildr"));
  //  about_Action->setMenuRole(QAction::AboutRole);
  connect(about_Action, SIGNAL(triggered()), this, SLOT(about()));

#if QTB_UPDATECHECK
  // check update Action
  checkUpdate_Action = new QAction(tr("Check Update"), this);
  checkUpdate_Action->setStatusTip(tr("Check Update"));
  checkUpdate_Action->setEnabled(httpGetter->supportsSsl());
  connect(checkUpdate_Action, SIGNAL(triggered()), this, SLOT(checkUpdate()));
#endif // QTB_UPDATECHECK

  // Show Menu Bar
  showMenuBar_Action = new QAction(tr("Show Menu Bar"), this);
  showMenuBar_Action->setStatusTip(tr("Show Menu Bar"));
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
  showMenuBar_Action->setEnabled(true);
#else // defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
  showMenuBar_Action->setEnabled(false);
#endif // defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
  showMenuBar_Action->setCheckable(true);
  showMenuBar_Action->setChecked(settings->getOnShowMenuBar());
  connect(showMenuBar_Action, SIGNAL(triggered()), this, SLOT(toggleShowMenuBar()));

  // Show Status Bar
  showStatusBar_Action = new QAction(tr("Show Status Bar"), this);
  showStatusBar_Action->setStatusTip(tr("Show Status Bar"));
  showStatusBar_Action->setCheckable(true);
  showStatusBar_Action->setChecked(settings->getOnShowStatusBar());
  connect(showStatusBar_Action, SIGNAL(triggered()), this, SLOT(toggleShowStatusBar()));

  // Show FrameRate
  showFrameRate_Action = new QAction(tr("Show Frame Rate"), this);
  showFrameRate_Action->setStatusTip(tr("Show Frame Rate"));
  showFrameRate_Action->setCheckable(true);
  showFrameRate_Action->setChecked(settings->getOnShowFrameRate());
  connect(showFrameRate_Action, SIGNAL(triggered()), this, SLOT(toggleShowFrameRate()));

  // Full Screen
#if defined(QTB_DEV_DESKTOP)
  if (QTB_DESKTOP_FULL_SCREEN){
	fullScreen_Action = new QAction(tr("Full Screen"), this);
	fullScreen_Action->setStatusTip(tr("Full Screen"));
	fullScreen_Action->setMenuRole(QAction::NoRole);
	fullScreen_Action->setEnabled(false);
	fullScreen_Action->setCheckable(true);
	fullScreen_Action->setChecked(false);
	connect(fullScreen_Action, SIGNAL(triggered()), this, SLOT(fullScreen()));
  }
#endif // defined(QTB_DEV_DESKTOP)

  // Stays On Top
  if (QTB_DESKTOP_STAYS_ON_TOP){
	staysOnTop_Action = new QAction(tr("Stays On Top"), this);
	staysOnTop_Action->setStatusTip(tr("Stays On Top"));
	staysOnTop_Action->setCheckable(true);
	staysOnTop_Action->setChecked(settings->getOnStaysOnTop());
	connect(staysOnTop_Action, SIGNAL(triggered()), this, SLOT(toggleStaysOnTop()));
  }

  // Desktop Scale Fixed
  if (QTB_DESKTOP_SCALE_FIXED){
	desktopScaleFixed_Action = new QAction(tr("Desktop Scale Fixed"), this);
	desktopScaleFixed_Action->setStatusTip(tr("Desktop Scale Fixed"));
	desktopScaleFixed_Action->setEnabled(false);
	desktopScaleFixed_Action->setCheckable(true);
	desktopScaleFixed_Action->setChecked(settings->getOnDesktopScaleFixed());
	connect(desktopScaleFixed_Action, SIGNAL(triggered()), this, SLOT(toggleDesktopScaleFixed()));
  }

  // Window Size Fixed
  if (QTB_WINDOW_SIZE_FIXED){
	windowSizeFixed_Action = new QAction(tr("Window Size Fixed"), this);
	windowSizeFixed_Action->setStatusTip(tr("Window Size Fixed"));
	windowSizeFixed_Action->setEnabled(true);
	windowSizeFixed_Action->setCheckable(true);
	windowSizeFixed_Action->setChecked(settings->getOnWindowSizeFixed());
	connect(windowSizeFixed_Action, SIGNAL(triggered()), this, SLOT(toggleWindowSizeFixed()));
  }

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // Show Software Keyboard
  showSoftwareKeyboard_Action = new QAction(tr("Show Software Keyboard"), this);
  showSoftwareKeyboard_Action->setStatusTip(tr("Show Software Keyboard"));
  showSoftwareKeyboard_Action->setEnabled(false);
  showSoftwareKeyboard_Action->setCheckable(true);
  showSoftwareKeyboard_Action->setChecked(settings->getOnShowSoftwareKeyboard());
  connect(showSoftwareKeyboard_Action, SIGNAL(triggered()), this, SLOT(toggleShowSoftwareKeyboard()));

  // Show Software Button
  showSoftwareButton_Action = new QAction(tr("Show Software Button"), this);
  showSoftwareButton_Action->setStatusTip(tr("Show Software Button"));
  showSoftwareButton_Action->setEnabled(false);
  showSoftwareButton_Action->setCheckable(true);
  showSoftwareButton_Action->setChecked(settings->getOnShowSoftwareButton());
  connect(showSoftwareButton_Action, SIGNAL(triggered()), this, SLOT(toggleShowSoftwareButton()));
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // Video Quality Action MINIMUM
  videoQuality_MINIMUM_Action = new QAction(tr("Video Quality : Minimum"), this);
  videoQuality_MINIMUM_Action->setCheckable(true);
  videoQuality_MINIMUM_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_MINIMUM);
  videoQuality_MINIMUM_Action->setStatusTip(tr("Video Quality : Minimum"));
  connect(videoQuality_MINIMUM_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_MINIMUM()));

  // Video Quality Action LOW
  videoQuality_LOW_Action = new QAction(tr("Video Quality : Low"), this);
  videoQuality_LOW_Action->setCheckable(true);
  videoQuality_LOW_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_LOW);
  videoQuality_LOW_Action->setStatusTip(tr("Video Quality : Low"));
  connect(videoQuality_LOW_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_LOW()));

  // Video Quality Action STANDARD
  videoQuality_STANDARD_Action = new QAction(tr("Video Quality : Standard"), this);
  videoQuality_STANDARD_Action->setCheckable(true);
  videoQuality_STANDARD_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_STANDARD);
  videoQuality_STANDARD_Action->setStatusTip(tr("Video Quality : Standard"));
  connect(videoQuality_STANDARD_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_STANDARD()));

  // Video Quality Action HIGH
  videoQuality_HIGH_Action = new QAction(tr("Video Quality : High"), this);
  videoQuality_HIGH_Action->setCheckable(true);
  videoQuality_HIGH_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_HIGH);
  videoQuality_HIGH_Action->setStatusTip(tr("Video Quality : High"));
  connect(videoQuality_HIGH_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_HIGH()));

  // Video Quality Action MAXIMUM
  videoQuality_MAXIMUM_Action = new QAction(tr("Video Quality : Maximum"), this);
  videoQuality_MAXIMUM_Action->setCheckable(true);
  videoQuality_MAXIMUM_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_MAXIMUM);
  videoQuality_MAXIMUM_Action->setStatusTip(tr("Video Quality : Maximum"));
  connect(videoQuality_MAXIMUM_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_MAXIMUM()));

  // Deskop Scaling Action
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog_Action = new QAction(tr("Desktop Scaling"), this);
	desktopScalingDialog_Action->setStatusTip(tr("Desktop Scaling"));
	desktopScalingDialog_Action->setEnabled(false);
	connect(desktopScalingDialog_Action, SIGNAL(triggered()), this, SLOT(desktopScaling()));
  }

  // Deskop Capture Action
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	desktopCapture_Action = new QAction(tr("Desktop Capture"), this);
	desktopCapture_Action->setStatusTip(tr("Desktop Capture"));
	desktopCapture_Action->setEnabled(false);
	connect(desktopCapture_Action, SIGNAL(triggered()), this, SLOT(desktopCapture()));
  }

  // Log Action
  if (QTB_LOG_VIEW){
	logViewDialog_Action = new QAction(tr("Log View"), this);
	logViewDialog_Action->setStatusTip(tr("Log View"));
	connect(logViewDialog_Action, SIGNAL(triggered()), this, SLOT(logView()));
  }

  // select frame rate Action
  selectFrameRate5_Action = new QAction(tr("5 FPS"), this);
  //  selectFrameRate5_Action->setEnabled(false);
  selectFrameRate5_Action->setCheckable(true);
  selectFrameRate5_Action->setChecked(settings->getFrameRate() == 5);
  selectFrameRate5_Action->setStatusTip(tr("maxfps 5 FPS"));
  connect(selectFrameRate5_Action, SIGNAL(triggered()), this, SLOT(selectFrameRate5()));

  selectFrameRate10_Action = new QAction(tr("10 FPS"), this);
  //  selectFrameRate10_Action->setEnabled(false);
  selectFrameRate10_Action->setCheckable(true);
  selectFrameRate10_Action->setChecked(settings->getFrameRate() == 10);
  selectFrameRate10_Action->setStatusTip(tr("maxfps 10 FPS"));
  connect(selectFrameRate10_Action, SIGNAL(triggered()), this, SLOT(selectFrameRate10()));

  selectFrameRate20_Action = new QAction(tr("20 FPS"), this);
  //  selectFrameRate20_Action->setEnabled(false);
  selectFrameRate20_Action->setCheckable(true);
  selectFrameRate20_Action->setChecked(settings->getFrameRate() == 20);
  selectFrameRate20_Action->setStatusTip(tr("maxfps 20 FPS"));
  connect(selectFrameRate20_Action, SIGNAL(triggered()), this, SLOT(selectFrameRate20()));

  selectFrameRate30_Action = new QAction(tr("30 FPS"), this);
  //  selectFrameRate30_Action->setEnabled(false);
  selectFrameRate30_Action->setCheckable(true);
  selectFrameRate30_Action->setChecked(settings->getFrameRate() == 30);
  selectFrameRate30_Action->setStatusTip(tr("maxfps 30 FPS"));
  connect(selectFrameRate30_Action, SIGNAL(triggered()), this, SLOT(selectFrameRate30()));

  selectFrameRate40_Action = new QAction(tr("40 FPS"), this);
  //  selectFrameRate40_Action->setEnabled(false);
  selectFrameRate40_Action->setCheckable(true);
  selectFrameRate40_Action->setChecked(settings->getFrameRate() == 40);
  selectFrameRate40_Action->setStatusTip(tr("maxfps 40 FPS"));
  connect(selectFrameRate40_Action, SIGNAL(triggered()), this, SLOT(selectFrameRate40()));

  selectFrameRate50_Action = new QAction(tr("50 FPS"), this);
  //  selectFrameRate50_Action->setEnabled(false);
  selectFrameRate50_Action->setCheckable(true);
  selectFrameRate50_Action->setChecked(settings->getFrameRate() == 50);
  selectFrameRate50_Action->setStatusTip(tr("maxfps 50 FPS"));
  connect(selectFrameRate50_Action, SIGNAL(triggered()), this, SLOT(selectFrameRate50()));

  selectFrameRate60_Action = new QAction(tr("60 FPS"), this);
  //  selectFrameRate60_Action->setEnabled(false);
  selectFrameRate60_Action->setCheckable(true);
  selectFrameRate60_Action->setChecked(settings->getFrameRate() == 60);
  selectFrameRate60_Action->setStatusTip(tr("maxfps 60 FPS"));
  connect(selectFrameRate60_Action, SIGNAL(triggered()), this, SLOT(selectFrameRate60()));

  selectFrameRateMaximum_Action = new QAction(tr("Maximum FPS"), this);
  //  selectFrameRateMaximum_Action->setEnabled(false);
  selectFrameRateMaximum_Action->setCheckable(true);
  selectFrameRateMaximum_Action->setChecked(settings->getFrameRate() == 0);
  selectFrameRateMaximum_Action->setStatusTip(tr("maxfps Maximum"));
  connect(selectFrameRateMaximum_Action, SIGNAL(triggered()), this, SLOT(selectFrameRateMaximum()));

  // select monitor Action
  selectMonitorNo1_Action =  new QAction(tr("Monitor 1"), this);
  selectMonitorNo1_Action->setEnabled(false);
  selectMonitorNo1_Action->setCheckable(true);
  selectMonitorNo1_Action->setChecked(settings->getMonitorNo() == 1);
  selectMonitorNo1_Action->setStatusTip(tr("Monitor 1"));
  connect(selectMonitorNo1_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo1()));
  selectMonitorNo2_Action =  new QAction(tr("Monitor 2"), this);
  selectMonitorNo2_Action->setEnabled(false);
  selectMonitorNo2_Action->setCheckable(true);
  selectMonitorNo2_Action->setChecked(settings->getMonitorNo() == 2);
  selectMonitorNo2_Action->setStatusTip(tr("Monitor 2"));
  connect(selectMonitorNo2_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo2()));
  selectMonitorNo3_Action =  new QAction(tr("Monitor 3"), this);
  selectMonitorNo3_Action->setEnabled(false);
  selectMonitorNo3_Action->setCheckable(true);
  selectMonitorNo3_Action->setChecked(settings->getMonitorNo() == 3);
  selectMonitorNo3_Action->setStatusTip(tr("Monitor 3"));
  connect(selectMonitorNo3_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo3()));
  selectMonitorNo4_Action =  new QAction(tr("Monitor 4"), this);
  selectMonitorNo4_Action->setEnabled(false);
  selectMonitorNo4_Action->setCheckable(true);
  selectMonitorNo4_Action->setChecked(settings->getMonitorNo() == 4);
  selectMonitorNo4_Action->setStatusTip(tr("Monitor 4"));
  connect(selectMonitorNo4_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo4()));
  selectMonitorNo5_Action =  new QAction(tr("Monitor 5"), this);
  selectMonitorNo5_Action->setEnabled(false);
  selectMonitorNo5_Action->setCheckable(true);
  selectMonitorNo5_Action->setChecked(settings->getMonitorNo() == 5);
  selectMonitorNo5_Action->setStatusTip(tr("Monitor 5"));
  connect(selectMonitorNo5_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo5()));
  selectMonitorNo6_Action =  new QAction(tr("Monitor 6"), this);
  selectMonitorNo6_Action->setEnabled(false);
  selectMonitorNo6_Action->setCheckable(true);
  selectMonitorNo6_Action->setChecked(settings->getMonitorNo() == 6);
  selectMonitorNo6_Action->setStatusTip(tr("Monitor 6"));
  connect(selectMonitorNo6_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo6()));
  selectMonitorNo7_Action =  new QAction(tr("Monitor 7"), this);
  selectMonitorNo7_Action->setEnabled(false);
  selectMonitorNo7_Action->setCheckable(true);
  selectMonitorNo7_Action->setChecked(settings->getMonitorNo() == 7);
  selectMonitorNo7_Action->setStatusTip(tr("Monitor 7"));
  connect(selectMonitorNo7_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo7()));
  selectMonitorNo8_Action =  new QAction(tr("Monitor 8"), this);
  selectMonitorNo8_Action->setEnabled(false);
  selectMonitorNo8_Action->setCheckable(true);
  selectMonitorNo8_Action->setChecked(settings->getMonitorNo() == 8);
  selectMonitorNo8_Action->setStatusTip(tr("Monitor 8"));
  connect(selectMonitorNo8_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo8()));
  selectMonitorNo9_Action =  new QAction(tr("Monitor 9"), this);
  selectMonitorNo9_Action->setEnabled(false);
  selectMonitorNo9_Action->setCheckable(true);
  selectMonitorNo9_Action->setChecked(settings->getMonitorNo() == 9);
  selectMonitorNo9_Action->setStatusTip(tr("Monitor 9"));
  connect(selectMonitorNo9_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNo9()));
  selectMonitorNoAll_Action =  new QAction(tr("Monitor All"), this);
  selectMonitorNoAll_Action->setEnabled(false);
  selectMonitorNoAll_Action->setCheckable(true);
  selectMonitorNoAll_Action->setChecked(settings->getMonitorNo() == MONITOR_NO_ALL);
  selectMonitorNoAll_Action->setStatusTip(tr("Monitor All"));
  connect(selectMonitorNoAll_Action, SIGNAL(triggered()), this, SLOT(setSelectMonitorNoAll()));

  // Sound Quality Action MINIMUM
  soundQuality_MINIMUM_Action = new QAction(tr("Sound Quality : Minimum"), this);
  soundQuality_MINIMUM_Action->setCheckable(true);
  soundQuality_MINIMUM_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_MINIMUM);
  soundQuality_MINIMUM_Action->setStatusTip(tr("Sound Quality : Minimum"));
  connect(soundQuality_MINIMUM_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_MINIMUM()));

  // Sound Quality Action LOW
  soundQuality_LOW_Action = new QAction(tr("Sound Quality : Low"), this);
  soundQuality_LOW_Action->setCheckable(true);
  soundQuality_LOW_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_LOW);
  soundQuality_LOW_Action->setStatusTip(tr("Sound Quality : Low"));
  connect(soundQuality_LOW_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_LOW()));

  // Sound Quality Action STANDARD
  soundQuality_STANDARD_Action = new QAction(tr("Sound Quality : Standard"), this);
  soundQuality_STANDARD_Action->setCheckable(true);
  soundQuality_STANDARD_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_STANDARD);
  soundQuality_STANDARD_Action->setStatusTip(tr("Sound Quality : Standard"));
  connect(soundQuality_STANDARD_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_STANDARD()));

  // Sound Quality Action HIGH
  soundQuality_HIGH_Action = new QAction(tr("Sound Quality : High"), this);
  soundQuality_HIGH_Action->setCheckable(true);
  soundQuality_HIGH_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_HIGH);
  soundQuality_HIGH_Action->setStatusTip(tr("Sound Quality : High"));
  connect(soundQuality_HIGH_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_HIGH()));

  // Sound Quality Action MAXIMUM
  soundQuality_MAXIMUM_Action = new QAction(tr("Sound Quality : Maximum"), this);
  soundQuality_MAXIMUM_Action->setCheckable(true);
  soundQuality_MAXIMUM_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_MAXIMUM);
  soundQuality_MAXIMUM_Action->setStatusTip(tr("Sound Quality : Maximum"));
  connect(soundQuality_MAXIMUM_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_MAXIMUM()));

  // Sound Cache Action 0
  soundCache_0_Action = new QAction(tr("Level 0"), this);
  soundCache_0_Action->setCheckable(true);
  soundCache_0_Action->setChecked(settings->getSoundCacheTime() == 0);
  soundCache_0_Action->setStatusTip(tr("Level 0"));
  connect(soundCache_0_Action, SIGNAL(triggered()), this, SLOT(setSoundCache_0()));

  // Sound Cache Action 1
  soundCache_1_Action = new QAction(tr("Level 1"), this);
  soundCache_1_Action->setCheckable(true);
  soundCache_1_Action->setChecked(settings->getSoundCacheTime() == 100);
  soundCache_1_Action->setStatusTip(tr("Level 1"));
  connect(soundCache_1_Action, SIGNAL(triggered()), this, SLOT(setSoundCache_1()));

  // Sound Cache Action 2
  soundCache_2_Action = new QAction(tr("Level 2"), this);
  soundCache_2_Action->setCheckable(true);
  soundCache_2_Action->setChecked(settings->getSoundCacheTime() == 200);
  soundCache_2_Action->setStatusTip(tr("Level 2"));
  connect(soundCache_2_Action, SIGNAL(triggered()), this, SLOT(setSoundCache_2()));

  // Sound Cache Action 3
  soundCache_3_Action = new QAction(tr("Level 3"), this);
  soundCache_3_Action->setCheckable(true);
  soundCache_3_Action->setChecked(settings->getSoundCacheTime() == 300);
  soundCache_3_Action->setStatusTip(tr("Level 3"));
  connect(soundCache_3_Action, SIGNAL(triggered()), this, SLOT(setSoundCache_3()));

  // Sound Cache Action 4
  soundCache_4_Action = new QAction(tr("Level 4"), this);
  soundCache_4_Action->setCheckable(true);
  soundCache_4_Action->setChecked(settings->getSoundCacheTime() == 400);
  soundCache_4_Action->setStatusTip(tr("Level 4"));
  connect(soundCache_4_Action, SIGNAL(triggered()), this, SLOT(setSoundCache_4()));

  // Sound Cache Action 5
  soundCache_5_Action = new QAction(tr("Level 5"), this);
  soundCache_5_Action->setCheckable(true);
  soundCache_5_Action->setChecked(settings->getSoundCacheTime() == 500);
  soundCache_5_Action->setStatusTip(tr("Level 5"));
  connect(soundCache_5_Action, SIGNAL(triggered()), this, SLOT(setSoundCache_5()));

  // onControl Action
  onControl_Action = new QAction(tr("Control ON/OFF"), this);
  onControl_Action->setCheckable(true);
  onControl_Action->setChecked(settings->getOnControl());
  onControl_Action->setStatusTip(tr("Control ON/OFF"));
  connect(onControl_Action, SIGNAL(triggered()), this, SLOT(toggleOnControl()));

  // onGraphics Action
  onGraphics_Action = new QAction(tr("Graphics ON/OFF"), this);
  onGraphics_Action->setCheckable(true);
  onGraphics_Action->setChecked(settings->getOnGraphics());
  onGraphics_Action->setStatusTip(tr("Graphics ON/OFF"));
  connect(onGraphics_Action, SIGNAL(triggered()), this, SLOT(toggleOnGraphics()));

  // onSound Action
  onSound_Action = new QAction(tr("Sound ON/OFF"), this);
  onSound_Action->setCheckable(true);
  onSound_Action->setChecked(settings->getOnSound());
  onSound_Action->setStatusTip(tr("Sound ON/OFF"));
  connect(onSound_Action, SIGNAL(triggered()), this, SLOT(toggleOnSound()));

#if QTB_PUBLIC_MODE6_SUPPORT
  // select public mode version Action
  selectPublicModeVersion5_Action = new QAction(tr("MODE 5"), this);
  //  selectPublicModeVersion5_Action->setEnabled(false);
  selectPublicModeVersion5_Action->setCheckable(true);
  selectPublicModeVersion5_Action->setChecked(settings->getPublicModeVersion() == PUBLICMODE_VERSION5);
  selectPublicModeVersion5_Action->setStatusTip(tr("Public Mode 5"));
  connect(selectPublicModeVersion5_Action, SIGNAL(triggered()), this, SLOT(selectPublicModeVersion5()));

  selectPublicModeVersion6_Action = new QAction(tr("MODE 6"), this);
  //  selectPublicModeVersion6_Action->setEnabled(false);
  selectPublicModeVersion6_Action->setCheckable(true);
  selectPublicModeVersion6_Action->setChecked(settings->getPublicModeVersion() == PUBLICMODE_VERSION6);
  selectPublicModeVersion6_Action->setStatusTip(tr("Public Mode 6"));
  connect(selectPublicModeVersion6_Action, SIGNAL(triggered()), this, SLOT(selectPublicModeVersion6()));

  selectPublicModeVersion7_Action = new QAction(tr("MODE 7"), this);
  //  selectPublicModeVersion7_Action->setEnabled(false);
  selectPublicModeVersion7_Action->setCheckable(true);
  selectPublicModeVersion7_Action->setChecked(settings->getPublicModeVersion() == PUBLICMODE_VERSION7);
  selectPublicModeVersion7_Action->setStatusTip(tr("Public Mode 7"));
  connect(selectPublicModeVersion7_Action, SIGNAL(triggered()), this, SLOT(selectPublicModeVersion7()));
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_RECORDER
  // start recording control
  startRecordingControl_Action = new QAction(tr("Start Recording"), this);
  startRecordingControl_Action->setEnabled(false);
  startRecordingControl_Action->setCheckable(true);
  startRecordingControl_Action->setChecked(settings->getOnRecordingControl());
  startRecordingControl_Action->setStatusTip(tr("Start Recording"));
  connect(startRecordingControl_Action, SIGNAL(triggered()), this, SLOT(startRecordingControl()));

  // stop recording control
  stopRecordingControl_Action = new QAction(tr("Stop Recording"), this);
  stopRecordingControl_Action->setEnabled(false);
  stopRecordingControl_Action->setStatusTip(tr("Stop Recording"));
  connect(stopRecordingControl_Action, SIGNAL(triggered()), this, SLOT(stopRecordingControl()));

  // replay recorded control
  startReplayRecordingControl_Action = new QAction(tr("Start Replay"), this);
  startReplayRecordingControl_Action->setEnabled(false);
  startReplayRecordingControl_Action->setCheckable(true);
  startReplayRecordingControl_Action->setStatusTip(tr("Start Replay"));
  connect(startReplayRecordingControl_Action, SIGNAL(triggered()), this, SLOT(startReplayRecordingControl()));

  // stop replay recorded control
  stopReplayRecordingControl_Action = new QAction(tr("Stop Replay"), this);
  stopReplayRecordingControl_Action->setEnabled(false);
  stopReplayRecordingControl_Action->setStatusTip(tr("Stop Replay"));
  connect(stopReplayRecordingControl_Action, SIGNAL(triggered()), this, SLOT(stopReplayRecordingControl()));
#endif // QTB_RECORDER

#if QTB_PLUGINS_DISABLE_SUPPORT
  // onPluginsDisable Action
  onPluginsDisable_Action = new QAction(tr("Disable Plugins"), this);
  onPluginsDisable_Action->setEnabled(false);
  onPluginsDisable_Action->setCheckable(true);
  onPluginsDisable_Action->setChecked(settings->getOnPluginsDisable());
  onPluginsDisable_Action->setStatusTip(tr("Disable Plugins"));
  connect(onPluginsDisable_Action, SIGNAL(triggered()), this, SLOT(setOnPluginsDisable()));
#endif // QTB_PLUGINS_DISABLE_SUPPORT

  // send key Action
#if 0 // for TEST
  sendKey1_Action = new QAction(tr("Ctrl + Alt + Del"), this);
  sendKey1_Action->setEnabled(false);
  sendKey1_Action->setStatusTip(tr("Send key: Ctrl + Alt + Del"));
  connect(sendKey1_Action, SIGNAL(triggered()), this, SLOT(sendKey_CTRL_ALT_DEL()));
#else
  sendKey1_Action = 0;
#endif

  sendKey2_Action = new QAction(tr("Alt + F4"), this);
  sendKey2_Action->setEnabled(false);
  sendKey2_Action->setStatusTip(tr("Send key: Alt + F4"));
  connect(sendKey2_Action, SIGNAL(triggered()), this, SLOT(sendKey_ALT_F4()));

  sendKey3_Action = new QAction(tr("Ctrl + Esc"), this);
  sendKey3_Action->setEnabled(false);
  sendKey3_Action->setStatusTip(tr("Send key: Ctrl + Esc"));
  connect(sendKey3_Action, SIGNAL(triggered()), this, SLOT(sendKey_CTRL_ESC()));

  sendKey4_Action = new QAction(tr("Windows"), this);
  sendKey4_Action->setEnabled(false);
  sendKey4_Action->setStatusTip(tr("Send key: Windows"));
  connect(sendKey4_Action, SIGNAL(triggered()), this, SLOT(sendKey_WINDOWS()));

  sendKey5_Action = new QAction(tr("PrintScreen"), this);
  sendKey5_Action->setEnabled(false);
  sendKey5_Action->setStatusTip(tr("Send key: PrintScreen"));
  connect(sendKey5_Action, SIGNAL(triggered()), this, SLOT(sendKey_PrintScreen()));

  sendKey6_Action = new QAction(tr("Alt + PrintScreen"), this);
  sendKey6_Action->setEnabled(false);
  sendKey6_Action->setStatusTip(tr("Send key: Alt + PrintScreen"));
  connect(sendKey6_Action, SIGNAL(triggered()), this, SLOT(sendKey_ALT_PrintScreen()));

  // on Scroll Mode Action
  if (QTB_SCROLL_MODE){
	onScrollMode_Action = new QAction(tr("Scroll Mode"), this);
	onScrollMode_Action->setEnabled(false);
	onScrollMode_Action->setCheckable(true);
	onScrollMode_Action->setChecked(settings->getOnScrollMode());
	onScrollMode_Action->setStatusTip(tr("Scroll Mode"));
	connect(onScrollMode_Action, SIGNAL(triggered()), this, SLOT(toggleOnScrollMode()));
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  // send clipboard
  sendClipboard_Action = new QAction(tr("Send Clipboard"), this);
  sendClipboard_Action->setEnabled(false);
  sendClipboard_Action->setStatusTip(tr("Send Clipboard"));
  connect(sendClipboard_Action, SIGNAL(triggered()), this, SLOT(sendClipboard()));

  // send file
  sendFile_Action = new QAction(tr("Send File"), this);
  sendFile_Action->setEnabled(false);
  sendFile_Action->setStatusTip(tr("Send File"));
  connect(sendFile_Action, SIGNAL(triggered()), this, SLOT(sendFile()));

  // cancel file transferring
  cancelFileTransferring_Action = new QAction(tr("Cancel File Transferring"), this);
  cancelFileTransferring_Action->setEnabled(false);
  cancelFileTransferring_Action->setStatusTip(tr("Cancel File Transferring"));
  connect(cancelFileTransferring_Action, SIGNAL(triggered()), this, SLOT(cancelFileTransferring()));
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_DESKTOP_COMPRESS_MODE // for TEST
  // desktop compress mode
  desktopCompressMode0_Action = new QAction(tr("None"), this);
  desktopCompressMode0_Action->setEnabled(true);
  desktopCompressMode0_Action->setCheckable(true);
  desktopCompressMode0_Action->setChecked(settings->getDesktopCompressMode() == 1);
  desktopCompressMode0_Action->setStatusTip(tr("Desktop Compress Mode : None"));
  connect(desktopCompressMode0_Action, SIGNAL(triggered()), this, SLOT(desktopCompressMode0()));
  desktopCompressMode2_Action = new QAction(tr("1/2"), this);
  desktopCompressMode2_Action->setEnabled(true);
  desktopCompressMode2_Action->setCheckable(true);
  desktopCompressMode2_Action->setChecked(settings->getDesktopCompressMode() == 2);
  desktopCompressMode2_Action->setStatusTip(tr("Desktop Compress Mode : 1/2"));
  connect(desktopCompressMode2_Action, SIGNAL(triggered()), this, SLOT(desktopCompressMode2()));
  desktopCompressMode4_Action = new QAction(tr("1/4"), this);
  desktopCompressMode4_Action->setEnabled(true);
  desktopCompressMode4_Action->setCheckable(true);
  desktopCompressMode4_Action->setChecked(settings->getDesktopCompressMode() == 4);
  desktopCompressMode4_Action->setStatusTip(tr("Desktop Compress Mode : 1/4"));
  connect(desktopCompressMode4_Action, SIGNAL(triggered()), this, SLOT(desktopCompressMode4()));
  desktopCompressMode8_Action = new QAction(tr("1/8"), this);
  desktopCompressMode8_Action->setEnabled(true);
  desktopCompressMode8_Action->setCheckable(true);
  desktopCompressMode8_Action->setChecked(settings->getDesktopCompressMode() == 8);
  desktopCompressMode8_Action->setStatusTip(tr("Desktop Compress Mode : 1/8"));
  connect(desktopCompressMode8_Action, SIGNAL(triggered()), this, SLOT(desktopCompressMode8()));
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST

#if QTB_PREFERENCE
  // preferences
  preferences_Action = new QAction(tr("Preferences..."), this);
  preferences_Action->setStatusTip(tr("Preferences..."));
  preferences_Action->setEnabled(true);
  connect(preferences_Action, SIGNAL(triggered()), this, SLOT(preferences()));
#endif // QTB_PREFERENCE
}

// create Menus
void QtBrynhildr::createMenus()
{
  // file menu
  fileMenu = menuBar()->addMenu(tr("File"));
  fileMenu->addAction(connectToServer_Action);
  fileMenu->addAction(disconnectToServer_Action);
#if QTB_PUBLIC_MODE6_SUPPORT
  if (settings->getOnTransferFileSupport() || settings->getOnTransferClipboardSupport()){
	fileMenu->addSeparator();
  }
#if 0 // for TEST
  if (settings->getOnTransferClipboardSupport())
	fileMenu->addAction(sendClipboard_Action);
#endif
  if (settings->getOnTransferFileSupport()){
	fileMenu->addAction(sendFile_Action);
	fileMenu->addAction(cancelFileTransferring_Action);
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if QTB_PREFERENCE
  fileMenu->addSeparator();
  fileMenu->addAction(preferences_Action);
#endif // QTB_PREFERENCE
#if defined(QTB_DEV_DESKTOP)
  fileMenu->addSeparator();
  fileMenu->addAction(exit_Action);
#endif // defined(QTB_DEV_DESKTOP)

  // display menu
  displayMenu = menuBar()->addMenu(tr("Display"));
#if 0 // for TEST
  displayMenu->addAction(showMenuBar_Action);
#endif // if 1
  displayMenu->addAction(showStatusBar_Action);
  displayMenu->addAction(showFrameRate_Action);

  // software keyboard and button
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  displayMenu->addSeparator();
  displayMenu->addAction(showSoftwareButton_Action);
  displayMenu->addAction(showSoftwareKeyboard_Action);
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // desktop scale fixed
#if defined(QTB_DEV_DESKTOP)
  if (QTB_DESKTOP_SCALE_FIXED){
	displayMenu->addSeparator();
	displayMenu->addAction(desktopScaleFixed_Action);
  }
#endif // defined(QTB_DEV_DESKTOP)

  // window size fixed
#if defined(QTB_DEV_DESKTOP)
  if (QTB_WINDOW_SIZE_FIXED){
	if (!QTB_DESKTOP_SCALE_FIXED){
	  displayMenu->addSeparator();
	}
	displayMenu->addAction(windowSizeFixed_Action);
  }
#endif // defined(QTB_DEV_DESKTOP)

  // stays on top
#if defined(QTB_DEV_DESKTOP)
  if (QTB_DESKTOP_STAYS_ON_TOP){
	displayMenu->addSeparator();
	displayMenu->addAction(staysOnTop_Action);
  }
#endif // defined(QTB_DEV_DESKTOP)

  // full screen
#if defined(QTB_DEV_DESKTOP)
  if (QTB_DESKTOP_FULL_SCREEN){
	displayMenu->addAction(fullScreen_Action);
  }
#endif // defined(QTB_DEV_DESKTOP)&& !defined(Q_OS_OSX)

  // video menu
  videoMenu = menuBar()->addMenu(tr("Video"));
  videoMenu->addAction(videoQuality_MINIMUM_Action);
  videoMenu->addAction(videoQuality_LOW_Action);
  videoMenu->addAction(videoQuality_STANDARD_Action);
  videoMenu->addAction(videoQuality_HIGH_Action);
  videoMenu->addAction(videoQuality_MAXIMUM_Action);

  // for select frame rate
  videoMenu->addSeparator();
  selectFrameRateSubMenu = videoMenu->addMenu(tr("Select Frame Rate"));
  selectFrameRateSubMenu->addAction(selectFrameRate5_Action);
  selectFrameRateSubMenu->addAction(selectFrameRate10_Action);
  selectFrameRateSubMenu->addAction(selectFrameRate20_Action);
  selectFrameRateSubMenu->addAction(selectFrameRate30_Action);
  selectFrameRateSubMenu->addAction(selectFrameRate40_Action);
  selectFrameRateSubMenu->addAction(selectFrameRate50_Action);
  selectFrameRateSubMenu->addAction(selectFrameRate60_Action);
  selectFrameRateSubMenu->addAction(selectFrameRateMaximum_Action);

  // for scaling
  if (QTB_DESKTOP_IMAGE_SCALING){
	videoMenu->addSeparator();
	videoMenu->addAction(desktopScalingDialog_Action);
  }

  // for capture
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	videoMenu->addSeparator();
	videoMenu->addAction(desktopCapture_Action);
  }

  // sound menu
  soundMenu = menuBar()->addMenu(tr("Sound"));
  soundMenu->addAction(soundQuality_MINIMUM_Action);
  soundMenu->addAction(soundQuality_LOW_Action);
  soundMenu->addAction(soundQuality_STANDARD_Action);
  soundMenu->addAction(soundQuality_HIGH_Action);
  soundMenu->addAction(soundQuality_MAXIMUM_Action);
  soundMenu->addSeparator();
  soundCacheSubMenu = soundMenu->addMenu(tr("Sound Cache"));
  soundCacheSubMenu->addAction(soundCache_0_Action);
  soundCacheSubMenu->addAction(soundCache_1_Action);
  soundCacheSubMenu->addAction(soundCache_2_Action);
  soundCacheSubMenu->addAction(soundCache_3_Action);
  soundCacheSubMenu->addAction(soundCache_4_Action);
  soundCacheSubMenu->addAction(soundCache_5_Action);

  // control menu
  controlMenu = menuBar()->addMenu(tr("Control"));
  // for send key
  sendKeySubMenu = controlMenu->addMenu(tr("Send Key"));
#if 0 // for TEST
  sendKeySubMenu->addAction(sendKey1_Action);
#endif
  sendKeySubMenu->addAction(sendKey2_Action);
  sendKeySubMenu->addAction(sendKey3_Action);
  sendKeySubMenu->addAction(sendKey4_Action);
  sendKeySubMenu->addAction(sendKey5_Action);
  sendKeySubMenu->addAction(sendKey6_Action);

  // for select monitor
  selectMonitorNoSubMenu = controlMenu->addMenu(tr("Select Monitor"));
  selectMonitorNoSubMenu->addAction(selectMonitorNo1_Action);
  selectMonitorNoSubMenu->addAction(selectMonitorNo2_Action);
  selectMonitorNoSubMenu->addAction(selectMonitorNo3_Action);
  selectMonitorNoSubMenu->addAction(selectMonitorNo4_Action);
  selectMonitorNoSubMenu->addAction(selectMonitorNo5_Action);
  selectMonitorNoSubMenu->addAction(selectMonitorNo6_Action);
  selectMonitorNoSubMenu->addAction(selectMonitorNo7_Action);
  selectMonitorNoSubMenu->addAction(selectMonitorNo8_Action);
  selectMonitorNoSubMenu->addAction(selectMonitorNo9_Action);
#if 0 // for TEST
  selectMonitorNoSubMenu->addAction(selectMonitorNoAll_Action);
#endif

  // for control
  controlMenu->addSeparator();
  controlMenu->addAction(onControl_Action);
  controlMenu->addAction(onGraphics_Action);
  controlMenu->addAction(onSound_Action);

#if QTB_PUBLIC_MODE6_SUPPORT
  // for select publicmode version
  controlMenu->addSeparator();
  selectPublicModeVersionSubMenu = controlMenu->addMenu(tr("Select Public Mode Version"));
  selectPublicModeVersionSubMenu->addAction(selectPublicModeVersion5_Action);
  selectPublicModeVersionSubMenu->addAction(selectPublicModeVersion6_Action);
  selectPublicModeVersionSubMenu->addAction(selectPublicModeVersion7_Action);
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_RECORDER
  // for record and replay
  controlMenu->addSeparator();
  recordAndReplaySubMenu = controlMenu->addMenu(tr("Record and Replay"));
  recordAndReplaySubMenu->addAction(startRecordingControl_Action);
  recordAndReplaySubMenu->addAction(stopRecordingControl_Action);
  recordAndReplaySubMenu->addSeparator();
  recordAndReplaySubMenu->addAction(startReplayRecordingControl_Action);
  recordAndReplaySubMenu->addAction(stopReplayRecordingControl_Action);
#endif // QTB_RECORDER

#if QTB_PLUGINS_DISABLE_SUPPORT
  // for plugins disable
  controlMenu->addSeparator();
  controlMenu->addAction(onPluginsDisable_Action);
#endif // QTB_PLUGINS_DISABLE_SUPPORT

  // option menu
  optionMenu = menuBar()->addMenu(tr("Option"));
  if (QTB_SCROLL_MODE){
	modeSubMenu = optionMenu->addMenu(tr("Mode"));
	modeSubMenu->addAction(onScrollMode_Action);
  }
  optionMenu->addSeparator();
  optionMenu->addAction(outputKeyboardLog_Action);
  optionMenu->addAction(outputLog_Action);

  // in testing sub menu
  if (QTB_IN_TESTING){
	optionMenu->addSeparator();
	inTestingSubMenu = optionMenu->addMenu(tr("In Testing"));
	// in Testing Menu
#if QTB_DESKTOP_COMPRESS_MODE // for TEST
	// desktop compress mode
	desktopCompressModeSubMenu = inTestingSubMenu->addMenu(tr("Desktop Compress Mode"));
	desktopCompressModeSubMenu->addAction(desktopCompressMode0_Action);
	desktopCompressModeSubMenu->addAction(desktopCompressMode2_Action);
	desktopCompressModeSubMenu->addAction(desktopCompressMode4_Action);
	desktopCompressModeSubMenu->addAction(desktopCompressMode8_Action);
	if (settings->getPublicModeVersion() <= PUBLICMODE_VERSION7){
	  desktopCompressModeSubMenu->setEnabled(true);
	}
	else {
	  desktopCompressModeSubMenu->setEnabled(false);
	}
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST
  }

  menuBar()->addSeparator();

  // help menu
  helpMenu = menuBar()->addMenu(tr("Help"));
  // for log view
  if (QTB_LOG_VIEW){
	helpMenu->addAction(logViewDialog_Action);
	helpMenu->addSeparator();
  }
#if QTB_UPDATECHECK
  helpMenu->addAction(checkUpdate_Action);
  helpMenu->addSeparator();
#endif // QTB_UPDATECHECK
  helpMenu->addAction(about_Action);
}

// create Context Menu
void QtBrynhildr::createContextMenu()
{
#if 0 // for TEST
  addAction(exit_Action);
  setContextMenuPolicy(Qt::ActionsContextMenu);
  //  setContextMenuPolicy(Qt::NoContextMenu);
#endif // for TEST
}

// create Tool Bar
void QtBrynhildr::createToolBars()
{
  // fileToolBar = addToolBar(tr("File"));
}

// create Status Bar
void QtBrynhildr::createStatusBar()
{
  connectionLabel = new QLabel;
  connectionLabel->setAlignment(Qt::AlignLeft);

  frameRateLabel = new QLabel;
  frameRateLabel->setAlignment(Qt::AlignRight);
  frameRateLabel->setText(" "); // dummy text

#if QTB_PUBLIC_MODE6_SUPPORT
  // progress bar for transfer file
  progressBar = new QProgressBar(this);
  progressBar->setFixedWidth(this->width()/4);
  progressBar->setFixedHeight((statusBar()->height())/2);
  progressBar->setValue(80); // for TEST
  progressBar->setRange(0,100);
  progressBar->setTextVisible(true);
  progressBar->setVisible(false);
  statusBar()->addPermanentWidget(progressBar);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  statusBar()->addWidget(connectionLabel);
  statusBar()->addPermanentWidget(frameRateLabel);
}

// update Status Bar
void QtBrynhildr::updateStatusBar()
{
  // update labels
  // update connected
  updateConnected();
  // update fps
  updateFrameRate();
}

// update connected
void QtBrynhildr::updateConnected()
{
  // check
  if (connectionLabel == 0){
	// Nothing to do
	return;
  }

  // set label
  if (settings->getConnected()){
	// connection
	connectionLabel->setText(tr("Connected : ") + settings->getServerName());
  }
  else {
	// connection
	connectionLabel->setText(tr("Not Connected"));
  }

  // set minimum size
  connectionLabel->setMinimumSize(connectionLabel->sizeHint());
}

// update frame rate
void QtBrynhildr::updateFrameRate()
{
  // check
  if (frameRateLabel == 0){
	// Nothing to do
	return;
  }

  // update fps
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  if (settings->getOnShowFrameRate() ||
	  settings->getOnShowSoftwareButton()){
	if (!settings->getConnected()){
	  currentFrameRate = 0;
	  currentDataRate = 0;
	}
	//currentFrameRate = settings->getFrameInterval(); // for TEST
	//currentFrameRate = settings->getFrameDrawTime(); // for TEST
	if (settings->getOnShowFrameRate()){ // Status Bar
	  QString str = QString(tr("Frame Rate : ")+"%1 [%2 Mbps]").
		arg(currentFrameRate, 4, 'f', 1, ' ').
		arg(currentDataRate, 4, 'f', 1, ' ');
	  frameRateLabel->setText(str);
	}
	else {
	  frameRateLabel->clear();
	}
	if (settings->getOnShowSoftwareButton()){ // Software Button
	  if (softwareButton != 0){
		softwareButton->setCurrentFrameRate(currentFrameRate);
		softwareButton->setCurrentDataRate(currentDataRate);
	  }
	}
  }
  else {
	frameRateLabel->clear();
  }
#else // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  if (settings->getOnShowFrameRate()){
	if (!settings->getConnected()){
	  currentFrameRate = 0;
	  currentDataRate = 0;
	}
	//currentFrameRate = settings->getFrameInterval(); // for TEST
	//currentFrameRate = settings->getFrameDrawTime(); // for TEST
	QString str = QString(tr("Frame Rate : ")+"%1 [%2 Mbps]").
	  arg(currentFrameRate, 4, 'f', 1, ' ').
	  arg(currentDataRate, 4, 'f', 1, ' ');
	frameRateLabel->setText(str);
  }
  else {
	frameRateLabel->clear();
  }
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
}

// connected
void QtBrynhildr::connected()
{
  // enabled software keyboard/button
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
#if defined(Q_OS_WIN)
  if (settings->getKeyboardType() != KEYBOARD_TYPE_NATIVE){
	if (showSoftwareKeyboard_Action != 0)
	  showSoftwareKeyboard_Action->setEnabled(true);

	if (showSoftwareButton_Action != 0)
	  showSoftwareButton_Action->setEnabled(true);
  }
#else // defined(Q_OS_WIN)
  if (showSoftwareKeyboard_Action != 0)
	showSoftwareKeyboard_Action->setEnabled(true);

  if (showSoftwareButton_Action != 0)
	showSoftwareButton_Action->setEnabled(true);
#endif // defined(Q_OS_WIN)
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // enabled disconnect to server
  disconnectToServer_Action->setEnabled(true);

  // enabled send key
  if (sendKey1_Action != 0)
    sendKey1_Action->setEnabled(true);
  if (sendKey2_Action != 0)
    sendKey2_Action->setEnabled(true);
  if (sendKey3_Action != 0)
    sendKey3_Action->setEnabled(true);
  if (sendKey4_Action != 0)
    sendKey4_Action->setEnabled(true);
  if (sendKey5_Action != 0)
    sendKey5_Action->setEnabled(true);
  if (sendKey6_Action != 0)
    sendKey6_Action->setEnabled(true);

  // enabled scaling dialog
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog_Action->setEnabled(true);
  }

  // enabled capture
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	desktopCapture_Action->setEnabled(true);
  }

  // enable scroll mode
  if (QTB_SCROLL_MODE){
	onScrollMode_Action->setEnabled(true);
  }

  // enable full screen
#if defined(QTB_DEV_DESKTOP)
  if (QTB_DESKTOP_FULL_SCREEN){
	fullScreen_Action->setEnabled(true);
  }
#endif // defined(QTB_DEV_DESKTOP)

  // enable desktop scale fixed
  if (QTB_DESKTOP_SCALE_FIXED){
	desktopScaleFixed_Action->setEnabled(true);
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// send clipboard
	sendClipboard_Action->setEnabled(true);

	// send file
	sendFile_Action->setEnabled(true);

	// cancel file transferring
	cancelFileTransferring_Action->setEnabled(false);

	// drag and drop
	mainWindow->setAcceptDrops(true);
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_PLUGINS_DISABLE_SUPPORT
  // plugins disable
  onPluginsDisable_Action->setEnabled(true);
#endif // QTB_PLUGINS_DISABLE_SUPPORT

#if QTB_DESKTOP_COMPRESS_MODE // for TEST
  // desktop compress mode
  if (settings->getPublicModeVersion() <= PUBLICMODE_VERSION7){
	desktopCompressModeSubMenu->setEnabled(true);
  }
  else {
	desktopCompressModeSubMenu->setEnabled(false);
  }
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST

  // reset total frame counter
  totalFrameCounter = 0;

  // initialize mouse cursor
  if (settings->getOnDisplayMouseCursor()){
	menuBar()->setCursor(cursor());
	changeMouseCursor(Qt::CrossCursor);
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  // set window title
  setWindowTitle(tr(QTB_APPLICATION)+"  - " + settings->getPublicModeAliasString() +" -");
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // refresh menu
  refreshMenu();

  // full screen at connected
  fullScreenMode = false;
  onSetDesktopScalingFactorForFullScreen = false;
  if (settings->getOnFullScreenAtConnected()){
	fullScreen();
	onSetDesktopScalingFactorForFullScreen = true;
  }
}

// disconnected
void QtBrynhildr::disconnected()
{
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // disabled software keyboard/button
  if (showSoftwareKeyboard_Action != 0)
	showSoftwareKeyboard_Action->setEnabled(false);

  if (showSoftwareButton_Action != 0)
	  showSoftwareButton_Action->setEnabled(false);
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // disabled disconnect to server
  disconnectToServer_Action->setEnabled(false);

  // disabled send key
  if (sendKey1_Action != 0)
    sendKey1_Action->setEnabled(false);
  if (sendKey2_Action != 0)
    sendKey2_Action->setEnabled(false);
  if (sendKey3_Action != 0)
    sendKey3_Action->setEnabled(false);
  if (sendKey4_Action != 0)
    sendKey4_Action->setEnabled(false);
  if (sendKey5_Action != 0)
    sendKey5_Action->setEnabled(false);
  if (sendKey6_Action != 0)
    sendKey6_Action->setEnabled(false);

  // disabled scaling dialog
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog_Action->setEnabled(false);
  }

  // disabled capture
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	desktopCapture_Action->setEnabled(false);
  }

  // disabled scroll mode
  if (QTB_SCROLL_MODE){
	onScrollMode_Action->setEnabled(false);
  }

  // disabled full screen
#if defined(QTB_DEV_DESKTOP)
  if (QTB_DESKTOP_FULL_SCREEN){
	fullScreen_Action->setEnabled(false);
  }
#endif // defined(QTB_DEV_DESKTOP)

  // disable desktop scale fixed
  if (QTB_DESKTOP_SCALE_FIXED){
	desktopScaleFixed_Action->setEnabled(false);
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// send clipboard
	sendClipboard_Action->setEnabled(false);

	// send file
	sendFile_Action->setEnabled(false);

	// cancel file transferring
	cancelFileTransferring_Action->setEnabled(false);

	// drag and drop
	mainWindow->setAcceptDrops(false);
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_PLUGINS_DISABLE_SUPPORT
  // plugins disable
  settings->setOnPluginsDisable(false);
  onPluginsDisable_Action->setChecked(false);
  onPluginsDisable_Action->setEnabled(false);
#endif // QTB_PLUGINS_DISABLE_SUPPORT
}

// set desktop scaling factor
void QtBrynhildr::setDesktopScalingFactor(QSize windowSize)
{
  if (settings->getOnDesktopScaleFixed() || !settings->getOnKeepOriginalDesktopSize()){
	// NOT change scaling
	return;
  }
  QSize desktopSize = mainWindow->getDesktopSize();
  if (!desktopSize.isValid()){
	return;
  }

  int width = windowSize.width() -  settings->getDesktop()->getCorrectWindowWidth();
  int height = windowSize.height() - getHeightOfMenuBar() - getHeightOfStatusBar() - settings->getDesktop()->getCorrectWindowHeight();

  QSize screenSize = settings->getDesktop()->getCurrentScreen().size();
  if (mainWindow->getSize().width() > screenSize.width()){
	width = mainWindow->getSize().width();
  }
  if (mainWindow->getSize().height() > screenSize.height()){
	height = mainWindow->getSize().height();
  }

  int desktopWidth = desktopSize.width();
  int desktopHeight = desktopSize.height();
  float widthFactor = (float)width/desktopWidth;
  float heightFactor = (float)height/desktopHeight;
  if (widthFactor < heightFactor){
	settings->setDesktopScalingFactor(widthFactor);
  }
  else {
	settings->setDesktopScalingFactor(heightFactor);
  }
}

#if 0 // for TEST
// change event
void QtBrynhildr::changeEvent(QEvent *event)
{
  QMainWindow::changeEvent(event);

  if (event->type() == QEvent::WindowStateChange){
	Qt::WindowState state = windowState();
	//  OR'ed Qt::WindowNoState, Qt::WindowMaximized, Qt::WindowMinimized, Qt::WindowActive
  }
}
#endif // for TEST

// close event by window close
void QtBrynhildr::closeEvent(QCloseEvent *event)
{
  event->ignore();

  exit();
}

// window resize event
void QtBrynhildr::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);

  QMainWindow::resizeEvent(event);

#if 0 // for TEST
  cout << "resizeEvent()" << endl << flush;
#endif // for TEST

  // rescaling desktop
  if (settings->getOnKeepOriginalDesktopSize() &&
	  settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_CLIENT){
#if 0 // for TEST
	cout << "resizeEvent() : Rescaling for (width, height) = ("
		 << event->size().width() << "," << event->size().height() << ")" << endl << flush;
#endif // for TEST
	setDesktopScalingFactor(event->size());
  }

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // resize software keyboard/button
  if (settings->getOnShowSoftwareKeyboard()){
	QRect rect = calculateSoftwareKeyboardLayout();
	softwareKeyboard->setGeometry(rect);
  }
  if (settings->getOnShowSoftwareButton()){
	QRect rect = calculateSoftwareButtonLayout();
	softwareButton->setGeometry(rect);
  }
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
}

// window hide event
void QtBrynhildr::hideEvent(QHideEvent *event)
{
  QMainWindow::hideEvent(event);

  if (settings->getConnected()){
	//	cout << "hideEvent()" << endl << flush;

	// save onControl/onGraphics/onSound
	onControl = settings->getOnControl();
	onGraphics = settings->getOnGraphics();
	onSound = settings->getOnSound();
	// onControl/onGraphics to OFF
	settings->setOnControl(false);
	settings->setOnGraphics(false);
  }
}

// window show event
void QtBrynhildr::showEvent(QShowEvent *event)
{
  QMainWindow::showEvent(event);

  if (settings->getConnected()){
	//	cout << "showEvent()" << endl << flush;

	// restore onControl/onGraphics/onSound
	settings->setOnControl(onControl);
	settings->setOnGraphics(onGraphics);
	settings->setOnSound(onSound);
  }
}

// context menu event
void QtBrynhildr::contextMenuEvent(QContextMenuEvent *event)
{
  switch(event->reason()){
  case QContextMenuEvent::Mouse:
	//	cout << "Context Menu Event by Mouse (Right button)" << endl << flush;
	// marker for mouse cursor
	if (settings->getOnShowMouseCursorMarker()){
	  mainWindow->setDrawMarkerCounter(30);
	}
	break;
  case QContextMenuEvent::Keyboard:
	//	cout << "Context Menu Event by Keyboard (Menu button)" << endl << flush;
	break;
  case QContextMenuEvent::Other:
	//	cout << "Context Menu Event by Other" << endl << flush;
	break;
  default:
	//	cout << "Context Menu Event by Unknown" << endl << flush;
	break;
  }

  QMainWindow::contextMenuEvent(event);
}

// load settings from setting files or registry
void QtBrynhildr::readSettings()
{
  QRect defaultRect = QRect(200, 200, 800, 600);

  // read global settings
  if (!option->getInitFlag()){
	settings->readSettings();
  }

  // restore geometry
  bool result = restoreGeometry(settings->getSettings()->value(QTB_GEOMETRY).toByteArray());
  if (!result){
	move(defaultRect.topLeft());
	resize(defaultRect.size());
  }

  // restore window state
  restoreState(settings->getSettings()->value(QTB_WINDOWSTATE).toByteArray());
}

// save settings to setting file or registry
void QtBrynhildr::writeSettings()
{
  // write global settings
  settings->writeSettings();

  // save geometry
  settings->getSettings()->setValue(QTB_GEOMETRY, saveGeometry());

  // save window state
  settings->getSettings()->setValue(QTB_WINDOWSTATE, saveState());

  // sync
  settings->getSettings()->sync();
}

// display About Dialog
void QtBrynhildr::about()
{
  // display About Message Box
  QMessageBox::about(this,
					 "About " QTB_APPLICATION,
					 "<h2>"
					 QTB_APPLICATION
					 " Ver. "
					 QTB_VERSION QTB_RCNAME
					 "</h2>"
					 "<p>Copyright &copy;"
					 " "
					 QTB_YEAR
					 " "
					 QTB_AUTHOR
					 " @ "
					 QTB_ORGANIZATION
					 " powered by MCZ."
					 "<br><br>"
					 QTB_BLOG);
}

// popup connect to server dialog
void QtBrynhildr::popUpConnectToServer()
{
  // pop up connect to server dialog
  connectToServerDialog->show();
}

// check update
void QtBrynhildr::checkUpdate()
{
  //  cout << "enter checkUpdate()" << endl << flush;

  // start downloading release page
  //  bool result = httpGetter->startDownload(QTB_URL_FOR_RELEASE, "release_page.html");
  bool result = httpGetter->startDownload(QTB_URL_FOR_RELEASE);
  if (!result){
	cout << "Failed to http access" << endl << flush;
  }

  //  cout << "leave checkUpdate()" << endl << flush;
}

// popup disconnect to server
void QtBrynhildr::popUpDisconnectToServer()
{
  // disconnect
  disconnectToServer();
}

// connect to server
void QtBrynhildr::connectToServer()
{
  // disconnected
  if (settings->getConnected()){
	disconnectToServer();
	// wait for reconnect to server
	QThread::sleep(1);
  }

  // clear desktop
  mainWindow->clearDesktop();

  // set event converter
  KEYBOARD_TYPE keyboardType = settings->getKeyboardType();
  // TODO: check keyboardType range
  switch(keyboardType){
  case KEYBOARD_TYPE_JP:
	// set to JP (built-in)
	eventConverter->setKeytopType(EventConverter::KEYTOP_TYPE_JP);
	break;
  case KEYBOARD_TYPE_US:
	// set to US (built-in)
	eventConverter->setKeytopType(EventConverter::KEYTOP_TYPE_US);
	break;
  case KEYBOARD_TYPE_NATIVE:
#if defined(Q_OS_WIN)
	// Nothing to do
#else // defined(Q_OS_WIN)
	ABORT(); // available for windows only
#endif // defined(Q_OS_WIN)
	break;
  default: // key layout file
#ifdef USE_KEYLAYOUTFILE
	int index = keyboardType - KEYBOARD_TYPE_NATIVE - 1;
	// set key layout file to eventconverter
	//	cout << "key layout file index = " << index << endl << flush;
	eventConverter->setKeytopType(keyLayoutFileReader->getKeyLayoutFile(index));
#else // USE_KEYLAYOUTFILE
	ABORT();
#endif //  USE_KEYLAYOUTFILE
	break;
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  // mode
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "MODE : " +
							   QString::number(settings->getPublicModeVersion()));
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // keyboard type
  if (settings->getKeyboardType() == KEYBOARD_TYPE_NATIVE){ // Native Keyboard
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "KeyboardType : Native");
  }
  else {
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "KeyboardType : " +
								 eventConverter->getEventConverterName());
  }

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // Software Keyboard and Button
  // software keyboard
#if 0 // for TEST
  if (softwareKeyboard != 0){
	delete softwareKeyboard;
	softwareKeyboard = 0;
  }
  softwareKeyboard = new SK(settings, mainWindow->getKeyBuffer(), this);
#endif // for TEST
  // TODO: check keyboardType range
  switch(keyboardType){
  case KEYBOARD_TYPE_JP:
	softwareKeyboard->setKeytopType(SoftwareKeyboard::KEYTOP_TYPE_JP);
	break;
  case KEYBOARD_TYPE_US:
	softwareKeyboard->setKeytopType(SoftwareKeyboard::KEYTOP_TYPE_US);
	break;
  case KEYBOARD_TYPE_NATIVE:
#if defined(Q_OS_WIN)
	// Nothing to do
#else // defined(Q_OS_WIN)
	ABORT(); // available for windows only
#endif // defined(Q_OS_WIN)
	break;
  default: // key layout file
#ifdef USE_KEYLAYOUTFILE
	int index = keyboardType - KEYBOARD_TYPE_NATIVE - 1;
	// set key layout file to software keyboard
	//	  cout << "key layout file index = " << index << endl << flush;
	KeyLayoutFile *keyLayoutFile = keyLayoutFileReader->getKeyLayoutFile(index);
	softwareKeyboard->setKeytopType(keyLayoutFile);
	settings->setKeyboardTypeName(keyLayoutFile->getName());
#else // USE_KEYLAYOUTFILE
	ABORT();
#endif // USE_KEYLAYOUTFILE
	break;
  }
  softwareKeyboard->setVisible(false);
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // clear buffer for control
  mainWindow->getKeyBuffer()->clear();
  mainWindow->getMouseBuffer()->clear();

  // initialize socket
  initSocket();

  // counter for control
  counter_control = 0;
  // counter for graphics
  counter_graphics = 0;

  // start all thread
  controlThread->start();
  graphicsThread->start();
  soundThread->start();
}

// reconnect to server
void QtBrynhildr::reconnectToServer()
{
  if (!settings->getConnected()){
	return;
  }

  // clear buffer for control
  mainWindow->getKeyBuffer()->clear();
  mainWindow->getMouseBuffer()->clear();

  // counter for control
  counter_control = 0;
  // counter for graphics
  counter_graphics = 0;

  // disconnected
  disconnected();

  // disconnect
  settings->setConnected(false);

  // update status bar
  updateStatusBar();

#if 0 // for TEST
  // desktop clear
  onDesktopClear();
#endif
}

// disconnect to server
void QtBrynhildr::disconnectToServer()
{
#if QTB_RECORDER
  // stop record and replay
  if (settings->getOnReplayingControl()){
	recorder->stopReplaying();
  }
  if (settings->getOnRecordingControl()){
	recorder->stopRecording(settings->getRecordingControlFileName());
  }
#endif // QTB_RECORDER

  // exit all threads
  controlThread->exitThread();
  graphicsThread->exitThread();
  soundThread->exitThread();

  controlThread->exit();
  controlThread->wait();
  graphicsThread->exit();
  graphicsThread->wait();
  soundThread->exit();
  soundThread->wait();

#if 0 // for TEST
  // close socket
  closeSocket();
#endif

  // disconnected
  disconnected();

  // disconnect
  settings->setConnected(false);

  // desktop clear
  onDesktopClear();
}

// finished thread
void QtBrynhildr::finishedNetThread()
{
  //  cout << "finishedNetThread()" << endl << flush;
  // disconnect
  settings->setConnected(false);

  // refresh window
  refreshWindow();
}

// exit process for Application exit
void QtBrynhildr::exit()
{
#if 1 // for TEST
  if (settings->getOnConfirmAtExit()){
	ConfirmDialog *confirmDialog =
	  new ConfirmDialog(tr("exit application?"),
						settings->getOnConfirmAtExit(),
						settings,
						this);
	if (confirmDialog->exec() == QDialog::Accepted){
	  settings->setOnConfirmAtExit(confirmDialog->getConfirmFlag());
	  delete confirmDialog;
	}
	else {
	  delete confirmDialog;
	  return;
	}
  }
#else // for TEST
  int ret;

  if (settings->getOnConfirmAtExit()){
	ret = QMessageBox::question(this,
								tr("Confirm"),
								tr("exit application?"),
								QMessageBox::Ok | QMessageBox::Cancel,
								QMessageBox::Cancel);
	if (ret == QMessageBox::Cancel){
	  // Nothing to do
	  return;
	}
  }
#endif // for TEST

  // disconnected
  disconnectToServer();

  // normal screen
  if (QTB_DESKTOP_FULL_SCREEN){
	if (fullScreenMode){
	  // to Normal Screen
	  exitFullScreen();
	}
  }

  // save settings
  if (writeSettingsAtExit || settings->getOnSaveSettingsAtExit()){
	// restore onControl/onGraphics/onSound
	settings->setOnControl(onControl);
	settings->setOnGraphics(onGraphics);
	settings->setOnSound(onSound);

	writeSettings();
  }

  // log
  QDateTime shutdownTime = QDateTime::currentDateTime();
  if (settings->getOutputLog()){
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "Shutdown : " +
								 shutdownTime.toString(dateFormat));
  }
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, tr("Shutdown."));

  // output average of frame rate
  // get total frame counter
  totalFrameCounter = graphicsThread->getTotalFrameCounter();
  if (totalFrameCounter != 0){
	//	cout << "totalFrameCounter = " << totalFrameCounter << endl << flush;
	uint diffSeconds = shutdownTime.toTime_t() - bootTime.toTime_t();
	if (diffSeconds != 0){
	  float averageFrameRate = (float)totalFrameCounter/diffSeconds;
	  //	  cout << "FPS of last session: " << averageFrameRate << endl << flush;
	  QString averageFrameRateString;
	  averageFrameRateString.setNum(averageFrameRate);
	  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, tr("FPS of last session : ") + averageFrameRateString + " FPS");
	}
  }

  // quit
  qApp->quit();
}

#if QTB_PUBLIC_MODE6_SUPPORT
// send clipboard
void QtBrynhildr::sendClipboard()
{
  QString text = clipboard->text();

  // send clipboard flag ON
  settings->setSendClipboardString(text);
  settings->setOnSendClipboard(true);
}

// send file
void QtBrynhildr::sendFile()
{
  // prepare for send file
  QStringList fileNames =
	QFileDialog::getOpenFileNames(this,
								  tr("Open file"),
								  settings->getOutputPath());
  if (fileNames.count() == 0){
	// Nothing to do
	return;
  }

  // send files
  settings->setSendFileNames(fileNames);
  settings->setSendFileCount(fileNames.count());
}

// cancel file transferring
void QtBrynhildr::cancelFileTransferring()
{
  // disable cancel file transferring menu
  cancelFileTransferring_Action->setEnabled(false);

  // cancel file transferring
  controlThread->exitThread();
}
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_PREFERENCE
// preferences
void QtBrynhildr::preferences()
{
  //cout << "enter preferences()" << endl << flush;

  preferenceDialog->show();

  //cout << "leave preferences()" << endl << flush;
}
#endif // QTB_PREFERENCE

// clear Video Quality check
void QtBrynhildr::clearVideoQualityCheck()
{
  videoQuality_MINIMUM_Action->setChecked(false);
  videoQuality_LOW_Action->setChecked(false);
  videoQuality_STANDARD_Action->setChecked(false);
  videoQuality_HIGH_Action->setChecked(false);
  videoQuality_MAXIMUM_Action->setChecked(false);
}

// clear Sound Quality check
void QtBrynhildr::clearSoundQualityCheck()
{
  soundQuality_MINIMUM_Action->setChecked(false);
  soundQuality_LOW_Action->setChecked(false);
  soundQuality_STANDARD_Action->setChecked(false);
  soundQuality_HIGH_Action->setChecked(false);
  soundQuality_MAXIMUM_Action->setChecked(false);
}

// clear Sound Cache check
void QtBrynhildr::clearSoundCacheCheck()
{
  soundCache_0_Action->setChecked(false);
  soundCache_1_Action->setChecked(false);
  soundCache_2_Action->setChecked(false);
  soundCache_3_Action->setChecked(false);
  soundCache_4_Action->setChecked(false);
  soundCache_5_Action->setChecked(false);
}

// clear Select Frame Rate
void QtBrynhildr::clearSelectFrameRateCheck()
{
  selectFrameRate5_Action->setChecked(false);
  selectFrameRate10_Action->setChecked(false);
  selectFrameRate20_Action->setChecked(false);
  selectFrameRate30_Action->setChecked(false);
  selectFrameRate40_Action->setChecked(false);
  selectFrameRate50_Action->setChecked(false);
  selectFrameRate60_Action->setChecked(false);
  selectFrameRateMaximum_Action->setChecked(false);
}

// clear Select Monitor No check
void QtBrynhildr::clearSelectMonitorNoCheck()
{
  selectMonitorNo1_Action->setChecked(false);
  selectMonitorNo2_Action->setChecked(false);
  selectMonitorNo3_Action->setChecked(false);
  selectMonitorNo4_Action->setChecked(false);
  selectMonitorNo5_Action->setChecked(false);
  selectMonitorNo6_Action->setChecked(false);
  selectMonitorNo7_Action->setChecked(false);
  selectMonitorNo8_Action->setChecked(false);
  selectMonitorNo9_Action->setChecked(false);
  selectMonitorNoAll_Action->setChecked(false);
}

// disabled Select Monitor No check
void QtBrynhildr::disabledSelectMonitor()
{
  selectMonitorNo9_Action->setEnabled(false);
  selectMonitorNo8_Action->setEnabled(false);
  selectMonitorNo7_Action->setEnabled(false);
  selectMonitorNo6_Action->setEnabled(false);
  selectMonitorNo5_Action->setEnabled(false);
  selectMonitorNo4_Action->setEnabled(false);
  selectMonitorNo3_Action->setEnabled(false);
  selectMonitorNo2_Action->setEnabled(false);
  selectMonitorNo1_Action->setEnabled(false);
  selectMonitorNoAll_Action->setEnabled(false);
}

// set video quality MINIMUM
void QtBrynhildr::setVideoQuality_MINIMUM()
{
  settings->setVideoQuality(VIDEO_QUALITY_MINIMUM);
  refreshVideoQualityMenu();
}

// set video quality LOW
void QtBrynhildr::setVideoQuality_LOW()
{
  settings->setVideoQuality(VIDEO_QUALITY_LOW);
  refreshVideoQualityMenu();
}

// set video quality STANDARD
void QtBrynhildr::setVideoQuality_STANDARD()
{
  settings->setVideoQuality(VIDEO_QUALITY_STANDARD);
  refreshVideoQualityMenu();
}

// set video quality HIGH
void QtBrynhildr::setVideoQuality_HIGH()
{
  settings->setVideoQuality(VIDEO_QUALITY_HIGH);
  refreshVideoQualityMenu();
}

// set video quality MAXIMUM
void QtBrynhildr::setVideoQuality_MAXIMUM()
{
  settings->setVideoQuality(VIDEO_QUALITY_MAXIMUM);
  refreshVideoQualityMenu();
}

// set sound quality MINIMUM
void QtBrynhildr::setSoundQuality_MINIMUM()
{
  settings->setSoundQuality(SOUND_QUALITY_MINIMUM);
  refreshSoundQualityMenu();
}

// set sound quality LOW
void QtBrynhildr::setSoundQuality_LOW()
{
  settings->setSoundQuality(SOUND_QUALITY_LOW);
  refreshSoundQualityMenu();
}

// set sound quality STANDARD
void QtBrynhildr::setSoundQuality_STANDARD()
{
  settings->setSoundQuality(SOUND_QUALITY_STANDARD);
  refreshSoundQualityMenu();
}

// set sound quality HIGH
void QtBrynhildr::setSoundQuality_HIGH()
{
  settings->setSoundQuality(SOUND_QUALITY_HIGH);
  refreshSoundQualityMenu();
}

// set sound quality MAXIMUM
void QtBrynhildr::setSoundQuality_MAXIMUM()
{
  settings->setSoundQuality(SOUND_QUALITY_MAXIMUM);
  refreshSoundQualityMenu();
}

// set sound cache 0
void QtBrynhildr::setSoundCache_0()
{
  clearSoundCacheCheck();
  soundCache_0_Action->setChecked(true);
  settings->setSoundCacheTime(0);
}

// set sound cache 1
void QtBrynhildr::setSoundCache_1()
{
  clearSoundCacheCheck();
  soundCache_1_Action->setChecked(true);
  settings->setSoundCacheTime(100);
}

// set sound cache 2
void QtBrynhildr::setSoundCache_2()
{
  clearSoundCacheCheck();
  soundCache_2_Action->setChecked(true);
  settings->setSoundCacheTime(200);
}

// set sound cache 3
void QtBrynhildr::setSoundCache_3()
{
  clearSoundCacheCheck();
  soundCache_3_Action->setChecked(true);
  settings->setSoundCacheTime(300);
}

// set sound cache 4
void QtBrynhildr::setSoundCache_4()
{
  clearSoundCacheCheck();
  soundCache_4_Action->setChecked(true);
  settings->setSoundCacheTime(400);
}

// set sound cache 5
void QtBrynhildr::setSoundCache_5()
{
  clearSoundCacheCheck();
  soundCache_5_Action->setChecked(true);
  settings->setSoundCacheTime(500);
}

// toggle onControl
void QtBrynhildr::toggleOnControl()
{
  if (settings->getOnControl()){
	settings->setOnControl(false);
	onControl = false;
	onControl_Action->setChecked(false);
	// clear device buffer
	mainWindow->getKeyBuffer()->clear();
	mainWindow->getMouseBuffer()->clear();
  }
  else {
	settings->setOnControl(true);
	onControl = true;
	onControl_Action->setChecked(true);
	// onGraphics On
	settings->setOnGraphics(true);
	onGraphics = true;
	onGraphics_Action->setChecked(true);
  }
}

// toggle onGraphics
void QtBrynhildr::toggleOnGraphics()
{
  if (settings->getOnGraphics()){
	settings->setOnGraphics(false);
	onGraphics = false;
	onGraphics_Action->setChecked(false);

	// onControl Off
	if (settings->getOnControlOffWithGraphicsOff()){
	  settings->setOnControl(false);
	  onControl = false;
	  onControl_Action->setChecked(false);
	}
  }
  else {
	settings->setOnGraphics(true);
	onGraphics = true;
	onGraphics_Action->setChecked(true);
  }
}

// toggle onSound
void QtBrynhildr::toggleOnSound()
{
  if (settings->getOnSound()){
	settings->setOnSound(false);
	onSound = false;
	onSound_Action->setChecked(false);
  }
  else {
	settings->setOnSound(true);
	onSound = true;
	onSound_Action->setChecked(true);
  }
}

#if QTB_PUBLIC_MODE6_SUPPORT
// refresh public mode
void QtBrynhildr::refreshPublicMode()
{
  // menus, drag and drop
  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// send clipboard
	sendClipboard_Action->setEnabled(true);

	// send file
	sendFile_Action->setEnabled(true);

	// cancel file transferring
	cancelFileTransferring_Action->setEnabled(false);

	// drag and drop
	mainWindow->setAcceptDrops(true);
  }
  else {
	// send clipboard
	sendClipboard_Action->setEnabled(false);

	// send file
	sendFile_Action->setEnabled(false);

	// cancel file transferring
	cancelFileTransferring_Action->setEnabled(false);

	// drag and drop
	mainWindow->setAcceptDrops(false);
  }

  // set window title
  setWindowTitle(tr(QTB_APPLICATION)+"  - " + settings->getPublicModeAliasString() +" -");

  // mouse cursor
  if (settings->getPublicModeVersion() != PUBLICMODE_VERSION7){
	// change mouse cursor
	changeMouseCursor(Qt::CrossCursor);
  }
}

// select public mode version
void QtBrynhildr::selectPublicModeVersion5()
{
  settings->setPublicModeVersion(PUBLICMODE_VERSION5);
  selectPublicModeVersion5_Action->setChecked(true);
  selectPublicModeVersion6_Action->setChecked(false);
#if QTB_PUBLIC_MODE7_SUPPORT
  selectPublicModeVersion7_Action->setChecked(false);
#endif // QTB_PUBLIC_MODE7_SUPPORT

  refreshPublicMode();
}
void QtBrynhildr::selectPublicModeVersion6()
{
  settings->setPublicModeVersion(PUBLICMODE_VERSION6);
  selectPublicModeVersion5_Action->setChecked(false);
  selectPublicModeVersion6_Action->setChecked(true);
#if QTB_PUBLIC_MODE7_SUPPORT
  selectPublicModeVersion7_Action->setChecked(false);
#endif // QTB_PUBLIC_MODE7_SUPPORT

  refreshPublicMode();
}
#if QTB_PUBLIC_MODE7_SUPPORT
void QtBrynhildr::selectPublicModeVersion7()
{
  settings->setPublicModeVersion(PUBLICMODE_VERSION7);
  selectPublicModeVersion5_Action->setChecked(false);
  selectPublicModeVersion6_Action->setChecked(false);
  selectPublicModeVersion7_Action->setChecked(true);

  refreshPublicMode();
}
#endif // QTB_PUBLIC_MODE7_SUPPORT
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_PLUGINS_DISABLE_SUPPORT
void QtBrynhildr::setOnPluginsDisable()
{
  settings->setOnPluginsDisable(true);
  onPluginsDisable_Action->setChecked(true);
  onPluginsDisable_Action->setEnabled(false);
}
#endif // QTB_PLUGINS_DISABLE_SUPPORT

#if QTB_RECORDER
// start recording control
void QtBrynhildr::startRecordingControl()
{
  // check
  if (settings->getOnRecordingControl()){
	// Now in recording
	return;
  }

  // start to record
  recorder->startRecording();

  // refresh menu
  refreshRecordingAndReplayMenu();
}

// stop recording control
void QtBrynhildr::stopRecordingControl()
{
  // check
  if (!settings->getOnRecordingControl()){
	// Now Not in recording
	return;
  }

  // prepare for save file
  QString fileName =
	QFileDialog::getSaveFileName(this,
								 tr("Save file"),
								 ".",
								 tr("Control Record File (*.qtb)"));
  if (fileName == ""){
	// Nothing to do
	return;
  }
  // stop to record
  recorder->stopRecording(qPrintable(fileName));

  // refresh menu
  refreshRecordingAndReplayMenu();
}

// replay recorded control
void QtBrynhildr::startReplayRecordingControl()
{
  static char filename[QTB_MAXPATHLEN+1];

  // check
  if (settings->getOnReplayingControl()){
	// Now in replaying
	return;
  }

  // prepare for replaying
  QString fileName =
	QFileDialog::getOpenFileName(this,
								 tr("Open file"),
								 ".",
								 tr("Control Record File (*.qtb)"));
  if (fileName == ""){
	// Nothing to do
	return;
  }
  strncpy(filename, qPrintable(fileName), QTB_MAXPATHLEN);
  settings->setReplayingControlFileName(filename);

  // start to replay
  recorder->startReplaying();

  // refresh menu
  refreshRecordingAndReplayMenu();
}

// stop replay recorded control
void QtBrynhildr::stopReplayRecordingControl()
{
  // check
  if (!settings->getOnReplayingControl()){
	// Now NOT in replaying
	return;
  }

  // stop to replay
  recorder->stopReplaying();

  // refresh menu
  refreshRecordingAndReplayMenu();
}
#endif // QTB_RECORDER

#if 0 // for TEST
// send key for CTRL + ALT + DEL
void QtBrynhildr::sendKey_CTRL_ALT_DEL()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	KeyBuffer *keyBuffer = mainWindow->getKeyBuffer();

	// press
	keyBuffer->put(VK_CONTROL, KEYCODE_FLG_KEYDOWN); // CTRL key press
	keyBuffer->put(VK_MENU, KEYCODE_FLG_KEYDOWN); // ALT key press
	keyBuffer->put(VK_DELETE, KEYCODE_FLG_KEYDOWN); // DEL key press

	// release
	keyBuffer->put(VK_DELETE, KEYCODE_FLG_KEYUP); // DEL key release
	keyBuffer->put(VK_MENU, KEYCODE_FLG_KEYUP); // ALT key release
	keyBuffer->put(VK_CONTROL, KEYCODE_FLG_KEYUP); // CTRL key release
  }
}
#endif

// send key for ALT + F4
void QtBrynhildr::sendKey_ALT_F4()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	KeyBuffer *keyBuffer = mainWindow->getKeyBuffer();

	// press
	keyBuffer->put(VK_MENU,	KEYCODE_FLG_KEYDOWN); // ALT key press
	keyBuffer->put(VK_F4,	KEYCODE_FLG_KEYDOWN); // F4 key press

	// release
	keyBuffer->put(VK_F4,	KEYCODE_FLG_KEYUP); // F4 key release
	keyBuffer->put(VK_MENU,	KEYCODE_FLG_KEYUP); // ALT key release
  }
}

// send key for CTRL + ESC
void QtBrynhildr::sendKey_CTRL_ESC()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	KeyBuffer *keyBuffer = mainWindow->getKeyBuffer();

	// press
	keyBuffer->put(VK_CONTROL,	KEYCODE_FLG_KEYDOWN); // CTRL key press
	keyBuffer->put(VK_ESCAPE,	KEYCODE_FLG_KEYDOWN); // ESC key press

	// release
	keyBuffer->put(VK_ESCAPE,	KEYCODE_FLG_KEYUP); // ESC key release
	keyBuffer->put(VK_CONTROL,	KEYCODE_FLG_KEYUP); // CTRL key release
  }
}

// send key for WINDOWS
void QtBrynhildr::sendKey_WINDOWS()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	KeyBuffer *keyBuffer = mainWindow->getKeyBuffer();

	// press
	keyBuffer->put(VK_LWIN,	KEYCODE_FLG_KEYDOWN); // Windows key press

	// release
	keyBuffer->put(VK_LWIN,	KEYCODE_FLG_KEYUP); // Windows key release
  }
}

// send key for PrintScreen
void QtBrynhildr::sendKey_PrintScreen()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	KeyBuffer *keyBuffer = mainWindow->getKeyBuffer();

	// press
	keyBuffer->put(VK_SNAPSHOT,	KEYCODE_FLG_KEYDOWN); // PrintScreen key press

	// release
	keyBuffer->put(VK_SNAPSHOT,	KEYCODE_FLG_KEYUP); // PrintScreen key release
  }
}

// send key for ALT + PrintScreen
void QtBrynhildr::sendKey_ALT_PrintScreen()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	KeyBuffer *keyBuffer = mainWindow->getKeyBuffer();

	// press
	keyBuffer->put(VK_MENU,	KEYCODE_FLG_KEYDOWN); // ALT key press
	keyBuffer->put(VK_SNAPSHOT,	KEYCODE_FLG_KEYDOWN); // PrintScreen key press

	// release
	keyBuffer->put(VK_SNAPSHOT,	KEYCODE_FLG_KEYUP); // PrintScreen key release
	keyBuffer->put(VK_MENU,	KEYCODE_FLG_KEYUP); // ALT key press
  }
}

// toggle show menu bar
void QtBrynhildr::toggleShowMenuBar()
{
  if (settings->getOnShowMenuBar()){
	onShowMenuBar = false;
  }
  else {
	settings->setOnShowMenuBar(true);
  }
  settings->setOnShowMenuBar(onShowMenuBar);
  menuBar()->setVisible(onShowMenuBar);
  refreshWindow();
}

// toggle show status bar
void QtBrynhildr::toggleShowStatusBar()
{
  if (settings->getOnShowStatusBar()){
	onShowStatusBar  = false;
  }
  else {
	onShowStatusBar  = true;
  }
  settings->setOnShowStatusBar(onShowStatusBar);
  statusBar()->setVisible(onShowStatusBar);
  refreshWindow();
}

// toggle show frame rate
void QtBrynhildr::toggleShowFrameRate()
{
  if (settings->getOnShowFrameRate()){
	settings->setOnShowFrameRate(false);
  }
  else {
	settings->setOnShowFrameRate(true);
  }
  updateStatusBar();
}

// full screen
void QtBrynhildr::fullScreen()
{
#if defined(QTB_DESKTOP)
  if (!QTB_DESKTOP_FULL_SCREEN)
	return;
#endif // defined(QTB_DESKTOP)

#if 0 // for DEBUG
  QSize size = mainWindow->getSize();
  cout << "fullScreen() : width  = " << size.width() << endl << flush;
  cout << "fullScreen() : height = " << size.height() << endl << flush;
#endif

  fullScreenMode = !fullScreenMode;
  if (fullScreenMode){
	if (settings->getOnHideMenuAndStatusBarAtFullScreen()){
	  // hide menu and status bar
	  settings->setOnShowMenuBar(false);
	  settings->setOnShowStatusBar(false);
	  menuBar()->setVisible(false);
	  statusBar()->setVisible(false);
	}
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mainWindow->setOnFullScreen(true);
	scrollArea->setPalette(fullScreenPalette); // change QPalette::Window to black
	showFullScreen();
  }
  else {
	if (settings->getOnHideMenuAndStatusBarAtFullScreen()){
	  // restore menu and status bar
	  settings->setOnShowMenuBar(onShowMenuBar);
	  settings->setOnShowStatusBar(onShowStatusBar);
	  menuBar()->setVisible(settings->getOnShowMenuBar());
	  statusBar()->setVisible(settings->getOnShowStatusBar());
	}
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mainWindow->setOnFullScreen(false);
	scrollArea->setPalette(originalPalette); // restore original QPalette::Window
	showNormal();
  }
  // set checked flag
#if defined(QTB_DEV_DESKTOP)
  fullScreen_Action->setChecked(fullScreenMode);
#endif // defined(QTB_DEV_DESKTOP)
#if 0 // for TEST
  // menu control
  desktopScalingDialog_Action->setEnabled(!fullScreenMode);
#endif
}

// toggle stays on top
void QtBrynhildr::toggleStaysOnTop()
{
  Qt::WindowFlags flags = windowFlags();
  if (settings->getOnStaysOnTop()){
	flags &= ~Qt::WindowStaysOnTopHint;
	settings->setOnStaysOnTop(false);
  }
  else {
	flags |= Qt::WindowStaysOnTopHint;
	settings->setOnStaysOnTop(true);
  }
  staysOnTop_Action->setChecked(settings->getOnStaysOnTop());
#if defined(QTB_NET_UNIX)
  QPoint topLeft = QPoint(geometry().x(), geometry().y() + 28);
#endif // defined(QTB_NET_UNIX)
  setWindowFlags(flags);
#if defined(QTB_NET_UNIX)
  move(topLeft);
#endif // defined(QTB_NET_UNIX)
  show();
}

// desktop scale fixed
void QtBrynhildr::toggleDesktopScaleFixed()
{
  if (QTB_DESKTOP_SCALE_FIXED){
	// set enable/disable desktop scaling dialog menu
	bool checked = desktopScaleFixed_Action->isChecked();
	settings->setOnDesktopScaleFixed(checked);
	static QSize orgMaximumSize = maximumSize();
	static QSize previousSize = size();
	if (checked){
	  previousSize = size();
	  // set maximum size (current size)
	  setMaximumSize(size());
	}
	else {
	  setMaximumSize(orgMaximumSize);
	  resize(previousSize);
	}
	if (settings->getConnected()){
	  desktopScalingDialog_Action->setEnabled(!checked);
	  //	  desktopScalingDialog->hide();
	}
  }
}

// window size fixed
void QtBrynhildr::toggleWindowSizeFixed()
{
  if (QTB_WINDOW_SIZE_FIXED){
	bool checked = windowSizeFixed_Action->isChecked();
	settings->setOnWindowSizeFixed(checked);
	static QSize orgMaximumSize = maximumSize();
	static QSize orgMinimumSize = minimumSize();
	static Qt::ScrollBarPolicy hpolicy = scrollArea->horizontalScrollBarPolicy();
	static Qt::ScrollBarPolicy vpolicy = scrollArea->horizontalScrollBarPolicy();
	if (checked){
	  // set maximum size (current size)
	  setMaximumSize(size());
	  // set minimum size (current size)
	  setMinimumSize(size());
	  // diable scroll bar
	  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	  // disable maximum button
#if defined(Q_OS_WIN)
	  Qt::WindowFlags flags = windowFlags();
	  flags &= ~Qt::WindowMaximizeButtonHint;
	  setWindowFlags(flags);
	  setVisible(true);
#endif // defined(Q_OS_WIN)
	}
	else {
	  setMaximumSize(orgMaximumSize);
	  setMinimumSize(orgMinimumSize);
	  // enable scroll bar
	  scrollArea->setHorizontalScrollBarPolicy(hpolicy);
	  scrollArea->setVerticalScrollBarPolicy(vpolicy);
	  // restore window flags
#if defined(Q_OS_WIN)
	  Qt::WindowFlags flags = windowFlags();
	  flags |= Qt::WindowMaximizeButtonHint;
	  setWindowFlags(flags);
	  setVisible(true);
#endif // defined(Q_OS_WIN)
	}
  }
}

// desktop scaling
void QtBrynhildr::desktopScaling()
{
  // display desktop scaling dialog
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog->setSliderPositionFromSetting();
	desktopScalingDialog->show();
  }
}

// desktop capture
void QtBrynhildr::desktopCapture()
{
  // desktop capture
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	// desktop capture flag ON
	settings->setOnDesktopCapture(true);
  }
}

// log view
void QtBrynhildr::logView()
{
  // display log view dialog
  if (QTB_LOG_VIEW){
	logViewDialog->show();
  }
}

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
// move top of software keyboard
void QtBrynhildr::moveTopOfSoftwareKeyboard(int y)
{
  QRect rect = softwareKeyboard->geometry();
  rect.moveTop(y);
  softwareKeyboard->setGeometry(rect);
}

// move top of software button
void QtBrynhildr::moveTopOfSoftwareButton(int y)
{
  QRect rect = softwareButton->geometry();
  rect.moveTop(y);
  softwareButton->setGeometry(rect);
}

// calulate software keyboard layout
QRect QtBrynhildr::calculateSoftwareKeyboardLayout()
{
  // calc geometry
  QSize desktopSize = mainWindow->size();
  QSize size = softwareKeyboard->resetSize();

  // calc size
  int width = desktopSize.width() * 0.9;
  int height = size.height() * ((double)width / size.width());

  // calc position
  int x = (desktopSize.width() - width) * 0.5;
  int y = (desktopSize.height() - height) * 0.8;

  //  cout << "width = " << width << endl;
  //  cout << "height = " << height << endl << flush;
  //  cout << "x = " << x << endl;
  //  cout << "y = " << y << endl << flush;

  return QRect(x, y, width, height);
}

// calulate software button layout
QRect QtBrynhildr::calculateSoftwareButtonLayout()
{
  // calc geometry
  QSize desktopSize = mainWindow->size();
  QSize size = softwareButton->resetSize();

  // calc size
  int width = desktopSize.width() * 0.9;
  int height = size.height() * ((double)width / size.width());

  // calc position
  int x = (desktopSize.width() - width) * 0.5;
  int y = (desktopSize.height() - height) * 0.8;

  //  cout << "width = " << width << endl;
  //  cout << "height = " << height << endl << flush;
  //  cout << "x = " << x << endl;
  //  cout << "y = " << y << endl << flush;


  return QRect(x, y, width, height);
}

// toggle show software keyboard
void QtBrynhildr::toggleShowSoftwareKeyboard()
{
  if (settings->getOnShowSoftwareKeyboard()){
	settings->setOnShowSoftwareKeyboard(false);
	softwareKeyboard->setVisible(false);
  }
  else {
	settings->setOnShowSoftwareKeyboard(true);
	QRect rect = calculateSoftwareKeyboardLayout();
	softwareKeyboard->setGeometry(rect);
	softwareKeyboard->setVisible(true);
  }
}

// toggle show software button
void QtBrynhildr::toggleShowSoftwareButton()
{
  if (settings->getOnShowSoftwareButton()){
	settings->setOnShowSoftwareButton(false);
	softwareButton->setVisible(false);
  }
  else {
	settings->setOnShowSoftwareButton(true);
	QRect rect = calculateSoftwareButtonLayout();
	softwareButton->setGeometry(rect);
	softwareButton->setVisible(true);
  }
}

// setup software keyboard
void QtBrynhildr::setupSoftwareKeyboard()
{
  settings->setOnShowSoftwareKeyboard(true);
  QRect rect = calculateSoftwareKeyboardLayout();
  rect.moveTop(mainWindow->getSize().height()); // outside desktop
  softwareKeyboard->setGeometry(rect);
  softwareKeyboard->setVisible(true);
}

// setup software button
void QtBrynhildr::setupSoftwareButton()
{
  settings->setOnShowSoftwareButton(true);
  QRect rect = calculateSoftwareButtonLayout();
  rect.moveTop(mainWindow->getSize().height()); // outside desktop
  softwareButton->setGeometry(rect);
  softwareButton->setVisible(true);
}
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

// select frame rate
void QtBrynhildr::selectFrameRate5()
{
  settings->setFrameRate(5);
  clearSelectFrameRateCheck();
  selectFrameRate5_Action->setChecked(true);
}
void QtBrynhildr::selectFrameRate10()
{
  settings->setFrameRate(10);
  clearSelectFrameRateCheck();
  selectFrameRate10_Action->setChecked(true);
}
void QtBrynhildr::selectFrameRate20()
{
  settings->setFrameRate(20);
  clearSelectFrameRateCheck();
  selectFrameRate20_Action->setChecked(true);
}
void QtBrynhildr::selectFrameRate30()
{
  settings->setFrameRate(30);
  clearSelectFrameRateCheck();
  selectFrameRate30_Action->setChecked(true);
}
void QtBrynhildr::selectFrameRate40()
{
  settings->setFrameRate(40);
  clearSelectFrameRateCheck();
  selectFrameRate40_Action->setChecked(true);
}
void QtBrynhildr::selectFrameRate50()
{
  settings->setFrameRate(50);
  clearSelectFrameRateCheck();
  selectFrameRate50_Action->setChecked(true);
}
void QtBrynhildr::selectFrameRate60()
{
  settings->setFrameRate(60);
  clearSelectFrameRateCheck();
  selectFrameRate60_Action->setChecked(true);
}
void QtBrynhildr::selectFrameRateMaximum()
{
  settings->setFrameRate(FRAMERATE_MAXIMUM);
  clearSelectFrameRateCheck();
  selectFrameRateMaximum_Action->setChecked(true);
}

// select monitor
void QtBrynhildr::setSelectMonitorNo1()
{
  settings->setMonitorNo(1);
  clearSelectMonitorNoCheck();
  selectMonitorNo1_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNo2()
{
  settings->setMonitorNo(2);
  clearSelectMonitorNoCheck();
  selectMonitorNo2_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNo3()
{
  settings->setMonitorNo(3);
  clearSelectMonitorNoCheck();
  selectMonitorNo3_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNo4()
{
  settings->setMonitorNo(4);
  clearSelectMonitorNoCheck();
  selectMonitorNo4_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNo5()
{
  settings->setMonitorNo(5);
  clearSelectMonitorNoCheck();
  selectMonitorNo5_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNo6()
{
  settings->setMonitorNo(6);
  clearSelectMonitorNoCheck();
  selectMonitorNo6_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNo7()
{
  settings->setMonitorNo(7);
  clearSelectMonitorNoCheck();
  selectMonitorNo7_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNo8()
{
  settings->setMonitorNo(8);
  clearSelectMonitorNoCheck();
  selectMonitorNo8_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNo9()
{
  settings->setMonitorNo(9);
  clearSelectMonitorNoCheck();
  selectMonitorNo9_Action->setChecked(true);
}
void QtBrynhildr::setSelectMonitorNoAll()
{
  settings->setMonitorNo(MONITOR_NO_ALL);
  clearSelectMonitorNoCheck();
  selectMonitorNoAll_Action->setChecked(true);
}

// toggle scroll mode
void QtBrynhildr::toggleOnScrollMode()
{
  if (!QTB_SCROLL_MODE)
	return;

  if (settings->getOnScrollMode()){
	settings->setOnScrollMode(false);
  }
  else {
	settings->setOnScrollMode(true);
  }
}

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
// visibility changed software keyboard
void QtBrynhildr::visibilityChangedSoftwareKeyboard(bool visible)
{
  settings->setOnShowSoftwareKeyboard(visible);
  showSoftwareKeyboard_Action->setChecked(visible);
}

// visibility changed software button
void QtBrynhildr::visibilityChangedSoftwareButton(bool visible)
{
  settings->setOnShowSoftwareButton(visible);
  showSoftwareButton_Action->setChecked(visible);
}
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

#if QTB_DESKTOP_COMPRESS_MODE // for TEST
// desktop compress
void QtBrynhildr::desktopCompressMode0()
{
  settings->setDesktopCompressMode(1);

  desktopCompressMode0_Action->setChecked(true);
  desktopCompressMode2_Action->setChecked(false);
  desktopCompressMode4_Action->setChecked(false);
  desktopCompressMode8_Action->setChecked(false);
}
void QtBrynhildr::desktopCompressMode2()
{
  settings->setDesktopCompressMode(2);

  desktopCompressMode0_Action->setChecked(false);
  desktopCompressMode2_Action->setChecked(true);
  desktopCompressMode4_Action->setChecked(false);
  desktopCompressMode8_Action->setChecked(false);
}
void QtBrynhildr::desktopCompressMode4()
{
  settings->setDesktopCompressMode(4);

  desktopCompressMode0_Action->setChecked(false);
  desktopCompressMode2_Action->setChecked(false);
  desktopCompressMode4_Action->setChecked(true);
  desktopCompressMode8_Action->setChecked(false);
}
void QtBrynhildr::desktopCompressMode8()
{
  settings->setDesktopCompressMode(8);

  desktopCompressMode0_Action->setChecked(false);
  desktopCompressMode2_Action->setChecked(false);
  desktopCompressMode4_Action->setChecked(false);
  desktopCompressMode8_Action->setChecked(true);
}
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST

// toggle outputKeyboardLog
void QtBrynhildr::toggleOutputKeyboardLog()
{
  if (settings->getOutputKeyboardLog()){
	settings->setOutputKeyboardLog(false);
	outputKeyboardLog_Action->setChecked(false);
  }
  else {
	settings->setOutputKeyboardLog(true);
	outputKeyboardLog_Action->setChecked(true);
  }
}

// toggle outputLog
void QtBrynhildr::toggleOutputLog()
{
  if (settings->getOutputLog()){
	settings->setOutputLog(false);
	outputLog_Action->setChecked(false);
  }
  else {
	settings->setOutputLog(true);
	outputLog_Action->setChecked(true);
  }
}

#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
// initialize socket
void QtBrynhildr::initSocket()
{
  sock_control = INVALID_SOCKET;
  sock_graphics = INVALID_SOCKET;
  sock_sound = INVALID_SOCKET;
}

// close socket
void QtBrynhildr::closeSocket()
{
  if (sock_control != INVALID_SOCKET){
	shutdown(sock_control, SD_BOTH);
	closesocket(sock_control);
	sock_control = INVALID_SOCKET;
  }
  if (sock_graphics != INVALID_SOCKET){
	shutdown(sock_graphics, SD_BOTH);
	closesocket(sock_graphics);
	sock_graphics = INVALID_SOCKET;
  }
  if (sock_sound != INVALID_SOCKET){
	shutdown(sock_sound, SD_BOTH);
	closesocket(sock_sound);
	sock_sound = INVALID_SOCKET;
  }
}
#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

// for Windows
#if defined(QTB_NET_WIN)
// initialize platform
bool QtBrynhildr::initPlatform()
{
  // initialize WinSock
  WSADATA wsaData;
  int wsaResult;
  wsaResult = WSAStartup(MAKEWORD(2,0), &wsaData);
  if (wsaResult != 0){
	// error
	return false;
  }

  return true;
}

// shutdown platform
bool QtBrynhildr::shutdownPlatform()
{
  // cleanup WinSock
  int wsaResult = WSACleanup();
  if (wsaResult != 0){
	// error
	return false;
  }

  return true;
}
#endif // defined(Q_NET_WIN)

// for UNIX
#if defined(QTB_NET_UNIX)
// initialize platform
bool QtBrynhildr::initPlatform()
{
  // Nothing to do
  return true;
}

// shutdown platform
bool QtBrynhildr::shutdownPlatform()
{
  // Nothing to do
  return true;
}
#endif // defined(QTB_NET_UNIX)

void QtBrynhildr::timerExpired()
{
  //  cout << "timerExpired!" << endl << flush;

  // update current frame rate
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  if (settings->getOnShowFrameRate() ||
	  settings->getOnShowSoftwareButton()){
#else // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  if (settings->getOnShowFrameRate()){
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
	static int refreshCounter = 1;
	if (refreshCounter > QTB_STATUS_REFRESH_COUNT){
	  // frame rate
	  currentFrameRate = graphicsThread->getFrameRate();
	  // data rate
	  long controlDataRate = controlThread->getDataRate();
	  long graphicsDataRate = graphicsThread->getDataRate();
	  long soundDataRate = soundThread->getDataRate();
	  // Mbps
	  currentDataRate = ((double)(controlDataRate + graphicsDataRate + soundDataRate) * 8 / (1024*1024));
	  updateFrameRate();
	  refreshCounter = 1;
	}
	refreshCounter++;
  }
}

} // end of namespace qtbrynhildr
