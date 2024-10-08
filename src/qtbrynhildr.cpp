// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Qt Header
#if QT_VERSION < 0x060000
#include <QAudioDeviceInfo>
#else // QT_VERSION >= 0x060000
#include <QAudioDevice>
#include <QMediaDevices>
#include <QStringRef>
#endif // QT_VERSION >= 0x060000
#include <QByteArray>
#include <QCloseEvent>
#include <QDir>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLocale>
#include <QMenuBar>
#include <QMimeData>
#include <QMessageBox>
#include <QRect>
#include <QStatusBar>
#include <QSysInfo>

// Local Header
#include "graphicsthread/yuv2rgb/yuv2rgb.h"
#include "qtbrynhildr.h"
#include "parameters.h"
#include "settings.h"
#include "url.h"
#include "util/cpuinfo.h"
#include "version.h"

#if QTB_HELP_BROWSER
#include "util/helpbrowser.h"
#endif // QTB_HELP_BROWSER

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

// frame_no of server
int frameNoOfServer = 0;
// frame_no of client
int frameNoOfClient = 0;

// date format
const QString dateFormat = QTB_LOG_DATE_FORMAT;

// constructor
QtBrynhildr::QtBrynhildr(Option *option, QClipboard *clipboard)
  :desktopFrame(nullptr)
#if defined(QTB_DEV_TOUCHPANEL)
  ,desktopPanel(nullptr)
#else // !defined(QTB_DEV_TOUCHPANEL)
  ,desktopWindowWidget(nullptr)
  ,desktopWindow(nullptr)
#endif // !defined(QTB_DEV_TOUCHPANEL)
  ,connectionLabel(nullptr)
  ,frameRateLabel(nullptr)
  ,fileMenu(nullptr)
  ,displayMenu(nullptr)
  ,videoMenu(nullptr)
  ,selectMonitorNoSubMenu(nullptr)
  ,selectFrameRateSubMenu(nullptr)
  ,soundMenu(nullptr)
  ,soundCacheSubMenu(nullptr)
  ,controlMenu(nullptr)
  ,sendKeySubMenu(nullptr)
  ,selectPublicModeVersionSubMenu(nullptr)
#if QTB_RECORDER
  ,recordAndReplaySubMenu(nullptr)
#endif // QTB_RECORDER
  ,optionMenu(nullptr)
#if defined(QTB_DEV_TOUCHPANEL)
  ,touchpanelOperationTypeSubMenu(nullptr)
  ,touchpanelInterfaceTypeSubMenu(nullptr)
#endif // defined(QTB_DEV_TOUCHPANEL)
  ,inTestingSubMenu(nullptr)
#if QTB_DESKTOP_COMPRESS_MODE
  ,desktopCompressModeSubMenu(nullptr)
#endif // QTB_DESKTOP_COMPRESS_MODE
#if defined(QTB_DEV_TOUCHPANEL)
  ,decodeOptionSubMenu(nullptr)
#endif // defined(QTB_DEV_TOUCHPANEL)
  ,helpMenu(nullptr)
#if QTB_BENCHMARK
  ,benchmarkMenu(nullptr)
#endif // QTB_BENCHMARK
#if QTB_TOOLBAR
  ,toolBar(nullptr)
#endif // QTB_TOOLBAR
  ,connectToServer_Action(nullptr)
  ,disconnectToServer_Action(nullptr)
  ,initializeSettings_Action(nullptr)
  ,outputKeyboardLog_Action(nullptr)
  ,outputLog_Action(nullptr)
  ,exit_Action(nullptr)
  ,aboutQt_Action(nullptr)
  ,about_Action(nullptr)
  ,checkUpdate_Action(nullptr)
#if QTB_HELP_BROWSER
  ,helpBrowser_Action(nullptr)
#endif // QTB_HELP_BROWSER
  ,videoQuality_MINIMUM_Action(nullptr)
  ,videoQuality_LOW_Action(nullptr)
  ,videoQuality_STANDARD_Action(nullptr)
  ,videoQuality_HIGH_Action(nullptr)
  ,videoQuality_MAXIMUM_Action(nullptr)
  ,showMenuBar_Action(nullptr)
#if QTB_TOOLBAR
  ,showToolBar_Action(nullptr)
#endif // QTB_TOOLBAR
  ,showStatusBar_Action(nullptr)
  ,showFrameRate_Action(nullptr)
  ,fullScreen_Action(nullptr)
  ,staysOnTop_Action(nullptr)
  ,desktopScaleFixed_Action(nullptr)
  ,windowSizeFixed_Action(nullptr)
  ,desktopScalingDialog_Action(nullptr)
  ,desktopCapture_Action(nullptr)
  ,logViewDialog_Action(nullptr)
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  ,showSoftwareKeyboard_Action(nullptr)
  ,showSoftwareButton_Action(nullptr)
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  ,selectFrameRateMinimum_Action(nullptr)
  ,selectFrameRate5_Action(nullptr)
  ,selectFrameRate10_Action(nullptr)
  ,selectFrameRate20_Action(nullptr)
  ,selectFrameRate30_Action(nullptr)
  ,selectFrameRate40_Action(nullptr)
  ,selectFrameRate50_Action(nullptr)
  ,selectFrameRate60_Action(nullptr)
  ,selectFrameRateMaximum_Action(nullptr)
  ,selectMonitorNo1_Action(nullptr)
  ,selectMonitorNo2_Action(nullptr)
  ,selectMonitorNo3_Action(nullptr)
  ,selectMonitorNo4_Action(nullptr)
  ,selectMonitorNo5_Action(nullptr)
  ,selectMonitorNo6_Action(nullptr)
  ,selectMonitorNo7_Action(nullptr)
  ,selectMonitorNo8_Action(nullptr)
  ,selectMonitorNo9_Action(nullptr)
  ,selectMonitorNoAll_Action(nullptr)
  ,soundQuality_MINIMUM_Action(nullptr)
  ,soundQuality_LOW_Action(nullptr)
  ,soundQuality_STANDARD_Action(nullptr)
  ,soundQuality_HIGH_Action(nullptr)
  ,soundQuality_MAXIMUM_Action(nullptr)
  ,soundCache_0_Action(nullptr)
  ,soundCache_1_Action(nullptr)
  ,soundCache_2_Action(nullptr)
  ,soundCache_3_Action(nullptr)
  ,soundCache_4_Action(nullptr)
  ,soundCache_5_Action(nullptr)
  ,onControl_Action(nullptr)
  ,onGraphics_Action(nullptr)
  ,onSound_Action(nullptr)
  ,selectPublicModeVersion5_Action(nullptr)
  ,selectPublicModeVersion6_Action(nullptr)
  ,selectPublicModeVersion7_Action(nullptr)
#if QTB_RECORDER
  ,startRecordingControl_Action(nullptr)
  ,stopRecordingControl_Action(nullptr)
  ,startReplayRecordingControl_Action(nullptr)
  ,stopReplayRecordingControl_Action(nullptr)
#endif // QTB_RECORDER
#if QTB_EXTRA_BUTTON_SUPPORT
  ,onPluginsDisable_Action(nullptr)
#endif // QTB_EXTRA_BUTTON_SUPPORT
  ,sendKey1_Action(nullptr)
  ,sendKey2_Action(nullptr)
  ,sendKey3_Action(nullptr)
  ,sendKey4_Action(nullptr)
  ,sendKey5_Action(nullptr)
  ,sendKey6_Action(nullptr)
  ,sendKey7_Action(nullptr)
  ,onScrollMode_Action(nullptr)
  ,onViewerMode_Action(nullptr)
  ,onFulFulMode_Action(nullptr)
  ,onMonochromeMode_Action(nullptr)
  ,onMouseTrackingMode_Action(nullptr)
#if defined(QTB_DEV_TOUCHPANEL)
  ,touchpanelOperationTypeKeroRemote_Action(nullptr)
  ,touchpanelOperationTypeQtBrynhildr_Action(nullptr)
  ,touchpanelInterfaceTypeLeftRight_Action(nullptr)
  ,touchpanelInterfaceTypeTopBottom_Action(nullptr)
  ,touchpanelInterfaceTypeBottom_LeftCenter_Action(nullptr)
  ,touchpanelInterfaceTypeTop_LeftCenter_Action(nullptr)
#endif // defined(QTB_DEV_TOUCHPANEL)
  ,sendClipboard_Action(nullptr)
  ,sendFile_Action(nullptr)
  ,cancelFileTransferring_Action(nullptr)
#if QTB_DESKTOP_COMPRESS_MODE
  ,desktopCompressMode0_Action(nullptr)
  ,desktopCompressMode2_Action(nullptr)
  ,desktopCompressMode4_Action(nullptr)
  ,desktopCompressMode8_Action(nullptr)
#endif // QTB_DESKTOP_COMPRESS_MODE
#if defined(QTB_DEV_TOUCHPANEL)
   // decode option for touchpanel
  ,decodeOptionTypeCPP_Action(nullptr)
  ,decodeOptionTypeNEON_Action(nullptr)
  ,decodeOptionThread1_Action(nullptr)
  ,decodeOptionThread2_Action(nullptr)
  ,decodeOptionThread4_Action(nullptr)
#endif // defined(QTB_DEV_TOUCHPANEL)
#if QTB_PREFERENCE
  ,preferences_Action(nullptr)
#endif // QTB_PREFERENCE
  ,disableDrawing_Action(nullptr)
  ,disableMaxfps_Action(nullptr)
#if QTB_BENCHMARK
  ,selectBenchmarkPhase0_Action(nullptr)
  ,selectBenchmarkPhase1_Action(nullptr)
  ,selectBenchmarkPhase2_Action(nullptr)
  ,selectBenchmarkPhase3_Action(nullptr)
  ,selectBenchmarkPhase4_Action(nullptr)
#endif // QTB_BENCHMARK
  ,connectToServerDialog(nullptr)
  ,desktopScalingDialog(nullptr)
  ,logViewDialog(nullptr)
#if QTB_PREFERENCE
  ,preferenceDialog(nullptr)
#endif // QTB_PREFERENCE
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  ,softwareKeyboard(nullptr)
  ,softwareButton(nullptr)
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  ,totalFrameCounter(0)
  ,currentFrameRate(0)
  ,currentDataRate(0)
#if QTB_TEST_DRAW_FRAME
  ,drawCounter(0)
#endif // QTB_TEST_DRAW_FRAME
  ,option(option)
  ,iniFileName(nullptr)
  ,settings(nullptr)
  ,writeSettingsAtExit(true)
#if QTB_CRYPTOGRAM
  ,cipher(nullptr)
#endif // QTB_CRYPTGRAM
#if QTB_RECORDER
  ,recorder(nullptr)
#endif // QTB_RECORDER
  ,httpGetter(nullptr)
  ,logMessage(new LogMessage(this))
  ,controlThread(nullptr)
  ,graphicsThread(nullptr)
  ,soundThread(nullptr)
#ifdef USE_KEYLAYOUTFILE
  ,keyLayoutFileManager(nullptr)
  ,keyLayoutFileReader(nullptr)
#endif // USE_KEYLAYOUTFILE
  ,eventConverter(nullptr)
  ,clipboard(clipboard)
  ,fullScreenMode(false)
  ,onSetDesktopScalingFactorForFullScreen(false)
  ,onShowMenuBar(false)
#if QTB_TOOLBAR
  ,onShowToolBar(false)
#endif // QTB_TOOLBAR
  ,onShowStatusBar(false)
  ,progressBar(nullptr)
  ,heightOfTitleBar(0)
  ,heightOfMenuBar(0)
#if QTB_TOOLBAR
  ,heightOfToolBar(0)
  ,widthOfToolBar(0)
#endif // QTB_TOOLBAR
  ,heightOfStatusBar(0)
  ,widthMargin(0)
  ,heightMargin(0)
  ,onControl(true)
  ,onGraphics(true)
  ,onSound(true)
  ,savedFrameRate(0)
  ,keyBuffer(nullptr)
  ,onControlKey(false)
  ,mouseBuffer(nullptr)
  ,timer(nullptr)
  ,isExecutingToConnect(false)
  ,onClearDesktop(false)
  ,hasSIMDInstruction(false)
  ,onPopUpConnectToServer(false)
  ,onCheckUpdateInBackground(false)
#if QTB_BENCHMARK
  ,initialBenchmarkPhaseCounter(0)
  ,onBenchmarkMenu(false)
#endif // QTB_BENCHMARK
  ,hasSoundDevice(false)
  // for DEBUG
  ,outputLog(false)
{
  // bootTime
  bootTime = QDateTime::currentDateTime();

#if QTB_SIMD_SUPPORT
#if !defined(__ARM_NEON__)
  hasSIMDInstruction = CPUInfo::SSE42() || CPUInfo::AVX2();
#else // defined(__ARM_NEON__)
  hasSIMDInstruction = CPUInfo::NEON();
#endif // defined(__ARM_NEON__)
#endif // QTB_SIMD_SUPPORT

#if QTB_CRYPTOGRAM
  // create cipher
  cipher = new Cipher(qPrintable(bootTime.toString()));
#endif // QTB_CRYPTOGRAM

  // set init file
  if (option->getIniFileName() != nullptr){
	iniFileName = option->getIniFileName();
  }

  // check shutdown flag
  if (option->getShutdownFlag()){
	// nothing to do -> shutdown now
	return;
  }

#if QTB_BENCHMARK
  // benchmark flag
  onBenchmarkMenu = option->getBenchmarkFlag();
#endif // QTB_BENCHMARK

  // create setting
#if QTB_CRYPTOGRAM
  settinegs = new Settings(iniFileName, cipher);
#else // !QTB_CRYPTGRAM
  settings = new Settings(iniFileName);
#endif // !QTB_CRYPTGRAM

  // restore settings
  readSettings();

  // set settings
  logMessage->setSettings(settings);

  // initialize platform
  if (!initPlatform()){
	// Failed to initialize platform
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "error: initPlatform()");
  }

  // set option to settings
  if (option->getPublicModeVersion() > 0){
	writeSettingsAtExit = false;
	settings->setPublicModeVersion(option->getPublicModeVersion());
  }
  if (option->getServerName() != nullptr){
	writeSettingsAtExit = false;
	settings->setServerName(option->getServerName());
  }
  if (option->getPortNo() > 0){
	writeSettingsAtExit = false;
	settings->setPortNo(option->getPortNo());
  }
  if (option->getPassword() != nullptr){
	writeSettingsAtExit = false;
	settings->setPassword(option->getPassword());
  }
  if (option->getHostType() != nullptr){
	writeSettingsAtExit = false;
	settings->setServerType(option->getHostType());
  }
  if (option->getDebug() != nullptr){
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

  // viewer mode
  if (option->getViewerFlag()){
	settings->setOnViewerAtConnected(true);
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

#if QTB_UPDATECHECK
  // create http getter
  httpGetter = new HttpGetter();
  if (!httpGetter->supportsSsl()){
	std::cout << "NOT support OpenSSL" << std::endl << std::flush;
  }
#if 0 // for DEBUG
  else {
	std::cout << "support OpenSSL" << std::endl << std::flush;
  }
#endif // for DEBUG
  connect(httpGetter, SIGNAL(finished()), SLOT(finishedDownload()));
#endif // QTB_UPDATECHECK

  // version
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "Version    : v" QTB_VERSION QTB_RCNAME QTB_ARCHNAME);
  // kernel type
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "OS Type    : " + QSysInfo::kernelType());
  // kernel version
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "OS Version : " + QSysInfo::kernelVersion());
  // Qt version
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "Qt Version : " + (QString)qVersion());

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
#endif // for DEBUG

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

  // screen size information
  if (settings->getOutputLog()){
	int screenWidth = settings->getCurrentScreenWidth();
	int screenHeight = settings->getCurrentScreenHeight();
	logMessage->outputLogMessage(PHASE_DEBUG,
								 (QString)"Screen Width : " + QString::number(screenWidth));
	logMessage->outputLogMessage(PHASE_DEBUG,
								 (QString)"Screen Height: " + QString::number(screenHeight));
  }

  // Supported Sound Sample Rate List
#if QT_VERSION < 0x060000
  {
	const QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
	QList<int> sampleRatesList = deviceInfo.supportedSampleRates();
	QString str = "Supported Sampling Rate (Hz): ";
	for(QList<int>::iterator i = sampleRatesList.begin(); i != sampleRatesList.end(); i++){
	  str =  str + " " + QString::number((int)(*i));
	  hasSoundDevice = true;
	}
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, std::move(str));
  }
#else // QT_VERSION >= 0x060000
  {
	QAudioDevice defaultAudioOutput = QMediaDevices::defaultAudioOutput();
	QString str = "Supported Sampling Rate (Hz): ";
	if (defaultAudioOutput.isNull()){
	  str =  str + " None";
	  hasSoundDevice = false;
	}
	else {
	  str = str +
		QString::number(defaultAudioOutput.minimumSampleRate()) + " -  " +
		QString::number(defaultAudioOutput.maximumSampleRate());
	  hasSoundDevice = true;
	}
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, std::move(str));
  }
#endif // QT_VERSION >= 0x060000
  // sound device check
  if (!hasSoundDevice){
	// no sound device
	settings->setOnSound(false);
  }

  // set current onControl/onGraphics/onSound
  onControl = settings->getOnControl();
  onGraphics = settings->getOnGraphics();
  onSound = settings->getOnSound();

  //------------------------------------------------------------
  // create window
  //------------------------------------------------------------
#if defined(QTB_DEV_TOUCHPANEL)

  // Desktop Panel
  desktopPanel = new DesktopPanel(this);
  desktopFrame = desktopPanel->getDesktopPanelWidget();

  // set Widget
  setCentralWidget(desktopPanel);

  // initialize palette
  backgroundPalette = fullScreenBackgroundPalette = desktopPanel->palette();
  // for background of desktop
  backgroundPalette.setColor(QPalette::Window, QTB_DESKTOP_BACKGROUND_COLOR);
  desktopPanel->setPalette(backgroundPalette); // change QPalette::Window to QTB_DESKTOP_BACKGROUND_COLOR
  // for full screen
  fullScreenBackgroundPalette.setColor(QPalette::Window, Qt::black);

#else // !defined(QTB_DEV_TOUCHPANEL)

  // Desktop Window Widget
  desktopWindowWidget = new DesktopWindowWidget(this);
  desktopFrame = desktopWindowWidget;

  // Desktop Window
  desktopWindow = new DesktopWindow;
  desktopWindow->setWidgetResizable(true);
  desktopWindow->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  //desktopWindow->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  if (settings->getOnDesktopScaleFixed()){
	desktopWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	desktopWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  }
  else {
	desktopWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	desktopWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }
  // set Widget
  desktopWindow->setWidget(desktopWindowWidget);
  desktopWindow->setFocusProxy(desktopWindowWidget);
  desktopWindow->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  setCentralWidget(desktopWindow);

  // initialize palette
  backgroundPalette = fullScreenBackgroundPalette = desktopWindow->palette();
  // for background of desktop
#if (QT_VERSION >= 0x060000) && (defined(Q_OS_WIN) || defined(Q_OS_CYGWIN))
  backgroundPalette.setColor(QPalette::Base, QTB_DESKTOP_BACKGROUND_COLOR);
#else // (QT_VERSION >= 0x060000) && (defined(Q_OS_WIN) || defined(Q_OS_CYGWIN))
  backgroundPalette.setColor(QPalette::Window, QTB_DESKTOP_BACKGROUND_COLOR);
#endif // (QT_VERSION >= 0x060000) && (defined(Q_OS_WIN) || defined(Q_OS_CYGWIN))
  desktopWindow->setPalette(backgroundPalette); // change QPalette::Window to QTB_DESKTOP_BACKGROUND_COLOR
  // for full screen
  fullScreenBackgroundPalette.setColor(QPalette::Window, Qt::black);

#endif // !defined(QTB_DEV_TOUCHPANEL)

  // set key/mouse buffer
  keyBuffer = desktopFrame->getKeyBuffer();
  mouseBuffer = desktopFrame->getMouseBuffer();

  // create Main Window
  createActions();
  createMenus();
  createContextMenu();
#if QTB_TOOLBAR
  createToolBars();
#endif // QTB_TOOLBAR
  createStatusBar();
  // show or hide
#if defined(QTB_DEV_TOUCHPANEL)
  // show menu bar and status bar on touchpanel device
  settings->setOnShowMenuBar(true);
  settings->setOnShowStatusBar(true);
#endif // defined(QTB_DEV_TOUCHPANEL)
  menuBar()->setVisible(settings->getOnShowMenuBar());
#if QTB_TOOLBAR
  toolBar->setVisible(settings->getOnShowToolBar());
#endif // QTB_TOOLBAR
  statusBar()->setVisible(settings->getOnShowStatusBar());
  statusBar()->setSizeGripEnabled(false);
  // save onShow* flags
  onShowMenuBar = settings->getOnShowMenuBar();
#if QTB_TOOLBAR
  onShowToolBar = settings->getOnShowToolBar();
#endif // QTB_TOOLBAR
  onShowStatusBar = settings->getOnShowStatusBar();

  // set icon
  //  setWindowIcon(QIcon(QTB_ICON_FILENAME));

  // set window title
  setupWindowTitle();

  // set window flags
  Qt::WindowFlags flags = windowFlags();
  flags |= Qt::CustomizeWindowHint;
  flags |= Qt::WindowTitleHint;
  flags |= Qt::WindowSystemMenuHint;
  flags |= Qt::WindowCloseButtonHint;
  flags |= Qt::WindowMinimizeButtonHint;
  flags &= ~Qt::WindowMaximizeButtonHint;
  if (settings->getOnFrameLessWindow()){
	flags |= Qt::FramelessWindowHint;
  }
  flags |= Qt::MSWindowsFixedSizeDialogHint;
  setWindowFlags(flags);

  // refresh window
  refreshWindow();

  // get window information
  heightOfTitleBar = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight) + 2;
  heightOfMenuBar = menuBar()->sizeHint().height();
#if QTB_TOOLBAR
  heightOfToolBar = toolBar->sizeHint().height();
  widthOfToolBar = toolBar->sizeHint().width();
#endif // QTB_TOOLBAR
  heightOfStatusBar = statusBar()->sizeHint().height();
  if (settings->getOutputLog()){
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "menuBar height = " + QString::number(heightOfMenuBar));
#if QTB_TOOLBAR
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "toolBar height = " + QString::number(heightOfToolBar));
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "toolBar width = " + QString::number(widthOfToolBar));
#endif // QTB_TOOLBAR
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "statusBar height = " + QString::number(heightOfStatusBar));
  }
#if 0 // for TEST
  qDebug() << "titleBar height = " << heightOfTitleBar;
  qDebug() << "menuBar height = " << heightOfMenuBar;
#if QTB_TOOLBAR
  qDebug() << "toolBar height = " << heightOfToolBar;
  qDebug() << "toolBar width = " << widthOfToolBar;
#endif // QTB_TOOLBAR
  qDebug() << "statusBar height = " << heightOfStatusBar;
#endif // for TEST

#if defined(QTB_DEV_TOUCHPANEL)
  // set window information
  int screenWidth = settings->getCurrentScreenWidth();
  int screenHeight = settings->getCurrentScreenHeight();
  //  qDebug() << "screenHeight = " << screenHeight;

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // setup touchpanel interface
  // Left : Button, Right : Keyboard
  int checkWidth = settings->getCheckAreaWidth();
  touchpanelInterfaceType[QTB_TOUCHPANELINTERFACETYPE_LEFTRIGHT].softwareButtonRect =
	QRect(0, 0, checkWidth, QTB_TOUCHPANEL_HEIGHT_SUPPORT_MAX);
  touchpanelInterfaceType[QTB_TOUCHPANELINTERFACETYPE_LEFTRIGHT].softwareKeyboardRect =
	QRect(screenWidth - checkWidth, 0, checkWidth, QTB_TOUCHPANEL_HEIGHT_SUPPORT_MAX);

  // Top : Button, Bottom : Keyboard
  int checkHeight = settings->getCheckAreaHeight();
  touchpanelInterfaceType[QTB_TOUCHPANELINTERFACETYPE_TOPBOTTOM].softwareButtonRect =
	QRect(0, 0, QTB_TOUCHPANEL_WIDTH_SUPPORT_MAX, checkHeight);
  touchpanelInterfaceType[QTB_TOUCHPANELINTERFACETYPE_TOPBOTTOM].softwareKeyboardRect =
	QRect(0, screenHeight - checkHeight, QTB_TOUCHPANEL_WIDTH_SUPPORT_MAX, checkHeight);

  // Bottom Left : Button, Bottom Center : Keyboard
  checkWidth = screenWidth / 4;
  touchpanelInterfaceType[QTB_TOUCHPANELINTERFACETYPE_BOTTOM_LEFTCENTER].softwareButtonRect =
	QRect(0, screenHeight - checkHeight, checkWidth, screenHeight);
  touchpanelInterfaceType[QTB_TOUCHPANELINTERFACETYPE_BOTTOM_LEFTCENTER].softwareKeyboardRect =
	QRect(checkWidth, screenHeight - checkHeight, checkWidth*3, screenHeight);

  // Top Left : Button, Top Center : Keyboard
  touchpanelInterfaceType[QTB_TOUCHPANELINTERFACETYPE_TOP_LEFTCENTER].softwareButtonRect =
	QRect(0, 0, checkWidth, checkHeight);
  touchpanelInterfaceType[QTB_TOUCHPANELINTERFACETYPE_TOP_LEFTCENTER].softwareKeyboardRect =
	QRect(checkWidth, 0, checkWidth*3, checkHeight);

  // interface check area information
  if (settings->getOutputLog()){
	for(int i = QTB_TOUCHPANELINTERFACETYPE_LEFTRIGHT; i < QTB_TOUCHPANELINTERFACETYPE_NUM; i++){
	  QPoint topLeft =
		touchpanelInterfaceType[i].softwareButtonRect.topLeft();
	  QPoint bottomRight =
		touchpanelInterfaceType[i].softwareButtonRect.bottomRight();
	  logMessage->outputLogMessage(PHASE_DEBUG,
								   "software button check area " + QString::number(i) + ": (" +
								   QString::number(topLeft.x()) +
								   ", " +
								   QString::number(topLeft.y()) +
								   ") - (" +
								   QString::number(bottomRight.x()) +
								   ", " +
								   QString::number(bottomRight.y()) +
								   ")"
								   );
	  topLeft =
		touchpanelInterfaceType[i].softwareKeyboardRect.topLeft();
	  bottomRight =
		touchpanelInterfaceType[i].softwareKeyboardRect.bottomRight();
	  logMessage->outputLogMessage(PHASE_DEBUG,
								   "software keyboard check area " + QString::number(i) + ": (" +
								   QString::number(topLeft.x()) +
								   ", " +
								   QString::number(topLeft.y()) +
								   ") - (" +
								   QString::number(bottomRight.x()) +
								   ", " +
								   QString::number(bottomRight.y()) +
								   ")"
								   );
	}
  }
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

#endif // defined(QTB_DEV_TOUCHPANEL)

  // set up connect to server dialog
  connectToServerDialog = new ConnectToServerDialog(settings, nullptr);
  connectToServerDialog->setModal(true);
  connect(connectToServerDialog, SIGNAL(connectToServer()), SLOT(connectToServer()));

  // set up desktop scaling dialog
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog = new DesktopScalingDialog(settings, nullptr);
	desktopScalingDialog->setModal(true);
  }

  // set up log view dialog
  if (QTB_LOG_VIEW){
	logViewDialog = new LogViewDialog(settings, nullptr);
	logViewDialog->setModal(true);
  }

#if QTB_PREFERENCE
  // preference dialog
  preferenceDialog = new PreferenceDialog(settings, nullptr);
  preferenceDialog->setModal(true);
#endif // QTB_PREFERENCE

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // set up Software Button and Keyboard
  // keyboard
#if defined(QTB_DEV_TOUCHPANEL)
  softwareKeyboard = new SK(keyBuffer, this, desktopPanel);
#else // !defined(QTB_DEV_TOUCHPANEL)
  softwareKeyboard = new SK(keyBuffer, this, desktopWindowWidget);
#endif // !defined(QTB_DEV_TOUCHPANEL)
  softwareKeyboard->setVisible(false);
#if 0 // for TEST
  softwareKeyboard->setGeometry(40,350,1120,300);
  softwareKeyboard->setVisible(true);
#endif // for TEST

  // button
#if defined(QTB_DEV_TOUCHPANEL)
  softwareButton = new SB(mouseBuffer, this, desktopPanel);
#else // !defined(QTB_DEV_TOUCHPANEL)
  softwareButton = new SB(mouseBuffer, this, desktopWindowWidget);
#endif // !defined(QTB_DEV_TOUCHPANEL)
  softwareButton->setVisible(false);
  connect(softwareButton, SIGNAL(refreshMenu()), SLOT(refreshMenu()));
#if 0 // for TEST
  softwareButton->setGeometry(40,30,1188,450);
  softwareButton->setVisible(true);
#endif // for TEST

#if 0 // for TEST
  // set default font size
  softwareKeyboard->setFontSize(32);
  softwareButton->setFontSize(32);

  // set default pen width
  softwareKeyboard->setPenWidth(1);
  softwareButton->setPenWidth(1);

  // set default opacity
  softwareKeyboard->setOpacity(0.80);
  softwareButton->setOpacity(0.80);
#endif // 0 // for TEST
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
  // set keylayout file information to dialog
  connectToServerDialog->addKeyboardTypeList(keyLayoutFileReader->getKeyboardTypeList());
#if QTB_PREFERENCE
  preferenceDialog->setKeylayoutList(keyLayoutFileReader->getKeyboardTypeList());
#endif // QTB_PREFERENCE

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
  desktopFrame->setEventConverter(eventConverter);

  // clipboard dataChanged
  if (settings->getOnTransferClipboardSupport()){
	connect(clipboard, SIGNAL(dataChanged()), SLOT(sendClipboard()));
  }

#if !defined(QTB_DEV_TOUCHPANEL)
  // set margin
#if defined(Q_OS_LINUX)
  if (menuBar()->sizeHint().height() == 0){
	settings->setVSpace(2);
  }
#endif // defined(Q_OS_LINUX)
  int hspace = settings->getHSpace();
  int vspace = settings->getVSpace();

  setMargins(hspace, vspace);
  desktopFrame->setMargins(hspace, vspace);
#endif // !defined(QTB_DEV_TOUCHPANEL)

  //------------------------------------------------------------
  // create threads
  //------------------------------------------------------------
#if QTB_RECORDER
  controlThread = new ControlThread(settings, desktopFrame, recorder);
#else // !QTB_RECORDER
  controlThread = new ControlThread(settings, desktopFrame);
#endif // !QTB_RECORDER
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
		  SIGNAL(refreshMenu()),
		  SLOT(refreshMenu()));
  connect(controlThread,
		  SIGNAL(finished()),
		  SLOT(finishedNetThread()));
  connect(controlThread,
		  SIGNAL(exitApplication()),
		  SLOT(exitApplication()));
  connect(controlThread,
		  SIGNAL(setClipboard(QString)),
		  SLOT(setClipboard(QString)));
  connect(controlThread,
		  SIGNAL(setFileTransferProgressBarValue(int)),
		  SLOT(setFileTransferProgressBarValue(int)));
  connect(controlThread,
		  SIGNAL(changeMouseCursor(const QCursor &)),
		  SLOT(changeMouseCursor(const QCursor &)));
  connect(controlThread,
		  SIGNAL(adjustWindow(const QSize &)),
		  SLOT(adjustWindow(const QSize &)));

  // graphics thread
  connect(graphicsThread,
		  SIGNAL(drawDesktop(QImage)),
		  SLOT(drawDesktop(QImage)));
  connect(graphicsThread,
		  SIGNAL(clearDesktop()),
		  SLOT(clearDesktop()));

  // sound thread
  connect(soundThread,
		  SIGNAL(connected()),
		  SLOT(connected()));
  connect(soundThread,
		  SIGNAL(refreshWindow()),
		  SLOT(refreshWindow()));

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

  // initialize timer for GUI
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), SLOT(timerExpired()));
  timer->start(QTB_WINDOW_UPDATE_DURATION);

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

  // sound device check (output message)
  if (!hasSoundDevice && !settings->getOnSoundCriticalMessageDisable()){
	// no sound device
	logMessage->outputMessage(QTB_MSG_NOT_FOUND_SOUND_DEVICE);
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

#if QTB_UPDATECHECK
  // check update
  if (settings->getOnCheckUpdateAtBootup()){
	// check update
	onCheckUpdateInBackground = true;
	checkUpdate();
  }
#endif // QTB_UPDATECHECK

  // for test mode
  disableDrawing_Action->setChecked(!graphicsThread->getOnDrawing());
  disableMaxfps_Action->setChecked(!controlThread->getOnMaxfps());

#if 0 // for TEST
  std::cout << "heightOfTitleBar = " <<  heightOfTitleBar << std::endl;
  std::cout << "heightOfMenuBar = " << heightOfMenuBar << std::endl;
  std::cout << "heightOfStatusBar = " << heightOfStatusBar << std::endl;
  std::cout << "widthMargin = " << widthMargin << std::endl;
  std::cout << "heightMargin = " << heightMargin << std::endl;

  QSize currentSize = size();
  std::cout << "(width, height) = (" <<
	currentSize.width() << ", " << currentSize.height() << ")" << std::endl << std::flush;
#endif // for TEST

#if QTB_SIMD_SUPPORT
  // SIMD decoder name
  QStringList list = graphicsThread->getSIMDDecoderNameList();
  QString str = list.join(", ");
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "VP8 decoder : " + str);
#if QTB_PREFERENCE
  // set SIMD decoder name list
  preferenceDialog->setDecoderNameList(graphicsThread->getSIMDDecoderNameList());
#endif // QTB_PREFERENCE
#endif // QTB_SIMD_SUPPORT
}

// destructor
QtBrynhildr::~QtBrynhildr()
{
  // shutdown timer for main thread
  if (timer != nullptr){
	timer->stop();
	delete timer;
	timer = nullptr;
  }
  if (settings != nullptr){
	// disconnect to server
	if (settings->getConnected())
	  disconnectToServer();

	// disconnect signal slots
	disconnect();
  }

  // stop & delete threads
  if (controlThread != nullptr){
	// delete
	delete controlThread;
	controlThread = nullptr;
  }
  if (graphicsThread != nullptr){
	// delete
	delete graphicsThread;
	graphicsThread = nullptr;
  }
  if (soundThread != nullptr){
	// delete
	delete soundThread;
	soundThread = nullptr;
  }

  // Event Converter
  if (eventConverter != nullptr){
	delete eventConverter;
	eventConverter = nullptr;
  }

#ifdef USE_KEYLAYOUTFILE
  // Key Layout File Reader
  if (keyLayoutFileReader != nullptr){
	delete keyLayoutFileReader;
	keyLayoutFileReader = nullptr;
  }

  // Key Layout File Manager
  if (keyLayoutFileManager != nullptr){
	delete keyLayoutFileManager;
	keyLayoutFileManager = nullptr;
  }
#endif // USE_KEYLAYOUTFILE

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // Software Button and Keyboard
  // keyboard
  if (softwareKeyboard != nullptr){
	delete softwareKeyboard;
	softwareKeyboard = nullptr;
  }

  // button
  if (softwareButton != nullptr){
	delete softwareButton;
	softwareButton = nullptr;
  }
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

#if QTB_CRYPTOGRAM
  // delete cipher
  if (cipher != nullptr){
	delete cipher;
	cipher = nullptr;
  }
#endif

  // http getter
  if (httpGetter != nullptr){
	delete httpGetter;
	httpGetter = nullptr;
  }

#if QTB_RECORDER
  // delete recorder
  if (recorder != nullptr){
	delete recorder;
	recorder = nullptr;
  }
#endif // QTB_RECORDER

  // dialogs
  if (connectToServerDialog != nullptr){
	// disconnect
	disconnect(connectToServerDialog, SIGNAL(connectToServer()), this, SLOT(connectToServer()));
	delete connectToServerDialog;
	connectToServerDialog = nullptr;
  }
  if (desktopScalingDialog != nullptr){
	delete desktopScalingDialog;
	desktopScalingDialog = nullptr;
  }
  if (logViewDialog != nullptr){
	delete logViewDialog;
	logViewDialog = nullptr;
  }
#if QTB_PREFERENCE
  if (preferenceDialog != nullptr){
	delete preferenceDialog;
	preferenceDialog = nullptr;
  }
#endif // QTB_PREFERENCE

  // progress bar
  if (progressBar != nullptr){
	delete progressBar;
	progressBar = nullptr;
  }

#if defined(QTB_DEV_TOUCHPANEL)

  // desktop frame
  if (desktopFrame != nullptr){
	delete desktopFrame;
	desktopFrame = nullptr;
  }

#else // !defined(QTB_DEV_TOUCHPANEL)

  // desktop window widget
  if (desktopWindowWidget != nullptr){
	delete desktopWindowWidget;
	desktopWindowWidget = nullptr;
	desktopFrame = 0;
  }
  // scroll area
  if (desktopWindow != nullptr){
	delete desktopWindow;
	desktopWindow = nullptr;
  }

#endif // !defined(QTB_DEV_TOUCHPANEL)

  // settings
  if (settings != nullptr){
	delete settings;
	settings = nullptr;
  }

  // shutdown platform
  if (!shutdownPlatform()){
	// Failed to shutdown platform
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "error: shutdownPlatform()");
  }
}

#if defined(QTB_DEV_TOUCHPANEL)
// get desktop panel
DesktopPanel *QtBrynhildr::getDesktopPanel() const
{
  return desktopPanel;
}
#else // !defined(QTB_DEV_TOUCHPANEL)
// get desktop window widget
DesktopWindowWidget *QtBrynhildr::getDesktopWindowWidget() const
{
  return desktopWindowWidget;
}
#endif // !defined(QTB_DEV_TOUCHPANEL)

// get desktop frame
DesktopFrame *QtBrynhildr::getDesktopFrame() const
{
  return desktopFrame;
}

// exit full screen
void QtBrynhildr::exitFullScreen()
{
  if (fullScreenMode)
	fullScreen();
}

// get height of title bar
int QtBrynhildr::getHeightOfTitleBar()
{
  return heightOfTitleBar;
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
	return 0;
  }
}

#if QTB_TOOLBAR
// get height of tool bar
int QtBrynhildr::getHeightOfToolBar()
{
  if (toolBar == nullptr)
	return 0;

  if (settings->getOnShowToolBar() &&
	  toolBar->orientation() == Qt::Horizontal &&
	  !toolBar->isFloating()){
	return toolBar->sizeHint().height();
  }
  else {
	return 0;
  }
}

// get width of tool bar
int QtBrynhildr::getWidthOfToolBar()
{
  if (toolBar == nullptr)
	return 0;

  if (settings->getOnShowToolBar() &&
	  toolBar->orientation() == Qt::Vertical &&
	  !toolBar->isFloating()){
	return toolBar->sizeHint().width();;
  }
  else {
	return 0;
  }
}
#endif // QTB_TOOLBAR

// get height of status bar
int QtBrynhildr::getHeightOfStatusBar()
{
  if (settings->getOnShowStatusBar()){
	return heightOfStatusBar;
  }
  else {
	return 0;
  }
}

#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
// set cursor point color
void QtBrynhildr::setCursorPointColor(QRgb cursorPointColor)
{
  controlThread->setCursorPointColor(cursorPointColor);
}
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)

// refresh window
void QtBrynhildr::refreshWindow()
{
  // output log for Window Size
  if (settings->getOutputLog()){
	QSize size = desktopFrame->getSize();
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "WindowSize : (" +
								 QString::number(size.width())  + "," +
								 QString::number(size.height()) +
								 ")");
  }

  // refresh desktop
  if (settings->getConnected()){
	desktopFrame->resizeWindow();
  }

  // update status bar
  updateStatusBar();
}

// adjust window
void QtBrynhildr::adjustWindow(const QSize &size)
{
  QRect desktop = settings->getCurrentScreen();
  QRect newWindow = QRect(pos(), size);
  if (!desktop.contains(newWindow)){
	const QPoint newPos =
	  QPoint((settings->getCurrentScreenWidth() - size.width() - 64)/2,
			 (settings->getCurrentScreenHeight() - size.height() - 64)/2);
	move(newPos);
  }
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

  // public mode
  refreshPublicMode();

  // other menu
  refreshOtherMenu();
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
  //  std::cout << "[QtBrynhildr] monitor_count=" << monitorCount << std::endl << std::flush;

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

// draw desktop
void QtBrynhildr::drawDesktop(QImage image)
{
  if (!settings->getConnected())
	return;

  // update desktop
  desktopFrame->refreshDesktop(image);

#if QTB_TEST_DRAW_FRAME
  // count up draw counter
  drawCounter++;
#endif // QTB_TEST_DRAW_FRAME

  // set desktop scaling factor for full screen mode
  if (onSetDesktopScalingFactorForFullScreen){
	onSetDesktopScalingFactorForFullScreen = false;

	QSize screenSize = settings->getCurrentScreenSize();
	setDesktopScalingFactor(screenSize);

#if defined(QTB_DEV_TOUCHPANEL)
	// set desktop scaling limit for touchpanel
	settings->setDesktopScalingFactorLimit(settings->getDesktopScalingFactor());
#endif // defined(QTB_DEV_TOUCHPANEL)
  }

  // update MainWindow
  update();
}

// clear desktop
void QtBrynhildr::clearDesktop()
{
  desktopFrame->clearDesktop();
  refreshWindow();
}

// change mouse cursor
void QtBrynhildr::changeMouseCursor(const QCursor &cursor)
{
  setCursor(cursor);
}

// network error handler
void QtBrynhildr::onNetworkError(bool doRetry)
{
  if (doRetry){
	reconnectToServer();
  }
  else {
	bool flag = onPopUpConnectToServer;
	disconnected();
	if (flag){
	  popUpConnectToServer();
	}
  }
}

// exit applilcation
void QtBrynhildr::exitApplication()
{
  exit();
}

// set clipboard
void QtBrynhildr::setClipboard(QString clipboardString)
{
  // check support
  if (!settings->getOnTransferClipboardSupport()){
	// Nothing to do
	return;
  }

  // check connected
  if (!settings->getConnected() ||
	  !settings->getOnControl()){
	// Nothing to do
	return;
  }

  // check clipboard
  if (clipboard == nullptr){
	// Nothing to do
	return;
  }

  //	std::cout << "setClipboard = " << qPrintable(clipboardString) << std::endl << std::flush;
  // set clipboard
  clipboard->setText(clipboardString);
  //	std::cout << "Clipboard = " << qPrintable(clipboard->text()) << std::endl << std::flush;
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
  connectToServer_Action->setEnabled(true);
  connect(connectToServer_Action, SIGNAL(triggered()), this, SLOT(popUpConnectToServer()));

  // disconnect to server
  disconnectToServer_Action = new QAction(tr("Disconnect"), this);
  disconnectToServer_Action->setStatusTip(tr("Disconnect to Brynhildr Server"));
  //  connectToServer_Action->setShortcut(tr("Ctrl+D"));
  disconnectToServer_Action->setEnabled(false);
  connect(disconnectToServer_Action, SIGNAL(triggered()), this, SLOT(popUpDisconnectToServer()));

  //   initialize settings
  initializeSettings_Action = new QAction(tr("Initialize Settings..."), this);
  initializeSettings_Action->setStatusTip(tr("Initialize Settings..."));
  initializeSettings_Action->setEnabled(true);
  connect(initializeSettings_Action, SIGNAL(triggered()), this, SLOT(initializeSettings()));

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

  // about Qt Action
  aboutQt_Action = new QAction(tr("About Qt"), this);
  aboutQt_Action->setStatusTip(tr("About Qt"));
  //  aboutQt_Action->setMenuRole(QAction::AboutRole);
  connect(aboutQt_Action, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

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

#if QTB_HELP_BROWSER
  // help browser
  helpBrowser_Action = new QAction(tr("Help Browser"), this);
  helpBrowser_Action->setStatusTip(tr("Help Browser"));
  helpBrowser_Action->setEnabled(httpGetter->supportsSsl());
  connect(helpBrowser_Action, SIGNAL(triggered()), this, SLOT(helpBrowser()));
#endif // QTB_HELP_BROWSER

  // Show Menu Bar
  showMenuBar_Action = new QAction(tr("Show Menu Bar"), this);
  showMenuBar_Action->setStatusTip(tr("Show Menu Bar"));
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
  showMenuBar_Action->setEnabled(true);
#else // !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))
  showMenuBar_Action->setEnabled(false);
#endif // !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))
  showMenuBar_Action->setCheckable(true);
  showMenuBar_Action->setChecked(settings->getOnShowMenuBar());
  connect(showMenuBar_Action, SIGNAL(triggered()), this, SLOT(toggleShowMenuBar()));

#if QTB_TOOLBAR
  // Show Tool Bar
  showToolBar_Action = new QAction(tr("Show Tool Bar"), this);
  showToolBar_Action->setStatusTip(tr("Show Tool Bar"));
  showToolBar_Action->setEnabled(true);
  showToolBar_Action->setCheckable(true);
  showToolBar_Action->setChecked(settings->getOnShowToolBar());
  connect(showToolBar_Action, SIGNAL(triggered()), this, SLOT(toggleShowToolBar()));
#endif // QTB_TOOLBAR

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
	toggleWindowSizeFixed();
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
  selectFrameRateMinimum_Action = new QAction(tr("Minimum FPS"), this);
  //  selectFrameRateMinimum_Action->setEnabled(false);
  selectFrameRateMinimum_Action->setCheckable(true);
  selectFrameRateMinimum_Action->setChecked(settings->getFrameRate() == FRAMERATE_MINIMUM);
  selectFrameRateMinimum_Action->setStatusTip(tr("maxfps Minimum FPS"));
  connect(selectFrameRateMinimum_Action, SIGNAL(triggered()), this, SLOT(selectFrameRateMinimum()));

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
  selectFrameRateMaximum_Action->setChecked(settings->getFrameRate() == FRAMERATE_MAXIMUM);
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
  // sound device check
  if (!hasSoundDevice){
	// no sound device
	onSound_Action->setEnabled(false);
  }

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
#if 0 // disable now
  sendKey1_Action = new QAction(tr("Ctrl + Alt + Del"), this);
  sendKey1_Action->setEnabled(false);
  sendKey1_Action->setStatusTip(tr("Send key: Ctrl + Alt + Del"));
  connect(sendKey1_Action, SIGNAL(triggered()), this, SLOT(sendKey_CTRL_ALT_DEL()));
#endif // disable now

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

  sendKey7_Action = new QAction(tr("Ctrl (ON/OFF)"), this);
  sendKey7_Action->setEnabled(false);
  sendKey7_Action->setStatusTip(tr("Send key: Ctrl (ON/OFF)"));
  connect(sendKey7_Action, SIGNAL(triggered()), this, SLOT(sendKey_CTRL_Toggle()));
  sendKey7_Action->setCheckable(true);
  sendKey7_Action->setChecked(getOnControlKey());

  // on Scroll Mode Action
  if (QTB_SCROLL_MODE){
	onScrollMode_Action = new QAction(tr("Scroll Mode"), this);
	onScrollMode_Action->setEnabled(false);
	onScrollMode_Action->setCheckable(true);
	onScrollMode_Action->setChecked(settings->getOnScrollMode());
	onScrollMode_Action->setStatusTip(tr("Scroll Mode"));
	connect(onScrollMode_Action, SIGNAL(triggered()), this, SLOT(toggleOnScrollMode()));
  }

  // on Viewer Mode Action
  if (QTB_VIEWER_MODE){
	onViewerMode_Action = new QAction(tr("Viewer Mode"), this);
	onViewerMode_Action->setEnabled(false);
	onViewerMode_Action->setCheckable(true);
	onViewerMode_Action->setChecked(settings->getOnViewerMode());
	onViewerMode_Action->setStatusTip(tr("Viewer Mode"));
	connect(onViewerMode_Action, SIGNAL(triggered()), this, SLOT(toggleOnViewerMode()));
  }

  // on FulFul Mode Action
  if (QTB_VIEWER_MODE){
	onFulFulMode_Action = new QAction(tr("Viewer Mode (FulFul)"), this);
	onFulFulMode_Action->setEnabled(false);
	onFulFulMode_Action->setCheckable(true);
	onFulFulMode_Action->setChecked(settings->getOnViewerMode());
	onFulFulMode_Action->setStatusTip(tr("Viewer Mode (FulFul)"));
	connect(onFulFulMode_Action, SIGNAL(triggered()), this, SLOT(toggleOnFulFulMode()));
  }

#if QTB_GRAY_SCALE_MODE
  // Monochrome Mode
  onMonochromeMode_Action = new QAction(tr("Monochrome Mode"), this);
  onMonochromeMode_Action->setStatusTip(tr("Monochrome Mode"));
  onMonochromeMode_Action->setEnabled(true);
  onMonochromeMode_Action->setCheckable(true);
  onMonochromeMode_Action->setChecked(settings->getOnMonochromeMode());
  connect(onMonochromeMode_Action, SIGNAL(triggered()), this, SLOT(toggleOnMonochromeMode()));
#endif // QTB_GRAY_SCALE_MODE

#if QTB_MOUSE_TRACKING_FOCUS_MODE
  // mouse tracking mode
  onMouseTrackingMode_Action = new QAction(tr("Mouse Tracking Mode"), this);
  onMouseTrackingMode_Action->setStatusTip(tr("Mouse Tracking Mode"));
  onMouseTrackingMode_Action->setEnabled(true);
  onMouseTrackingMode_Action->setCheckable(true);
  onMouseTrackingMode_Action->setChecked(settings->getOnMouseTrackingMode());
  connect(onMouseTrackingMode_Action, SIGNAL(triggered()), this, SLOT(toggleOnMouseTrackingMode()));
#endif // QTB_MOUSE_TRACKING_FOCUS_MODE

#if defined(QTB_DEV_TOUCHPANEL)
  // touchpanel operation type
  touchpanelOperationTypeKeroRemote_Action = new QAction(tr("KeroRemote Type"), this);
  touchpanelOperationTypeKeroRemote_Action->setEnabled(true);
  touchpanelOperationTypeKeroRemote_Action->setCheckable(true);
  touchpanelOperationTypeKeroRemote_Action->setChecked(
					settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_KEROREMOTE);
  touchpanelOperationTypeKeroRemote_Action->setStatusTip(tr("KeroRemote Type"));
  connect(touchpanelOperationTypeKeroRemote_Action, SIGNAL(triggered()), this,
		  SLOT(touchpanelOperationTypeKeroRemote()));

  touchpanelOperationTypeQtBrynhildr_Action = new QAction(tr("Qt Brynhildr Type"), this);
  touchpanelOperationTypeQtBrynhildr_Action->setEnabled(true);
  touchpanelOperationTypeQtBrynhildr_Action->setCheckable(true);
  touchpanelOperationTypeQtBrynhildr_Action->setChecked(
					settings->getTouchpanelOperationType() == QTB_TOUCHPANELOPERATIONTYPE_QTBRYNHILDR);
  touchpanelOperationTypeQtBrynhildr_Action->setStatusTip(tr("Qt Brynhildr Type"));
  connect(touchpanelOperationTypeQtBrynhildr_Action, SIGNAL(triggered()), this,
		  SLOT(touchpanelOperationTypeQtBrynhildr()));

  // touchpanel interface type
  touchpanelInterfaceTypeLeftRight_Action = new QAction(tr("Left/Right Type"), this);
  touchpanelInterfaceTypeLeftRight_Action->setEnabled(true);
  touchpanelInterfaceTypeLeftRight_Action->setCheckable(true);
  touchpanelInterfaceTypeLeftRight_Action->setChecked(
					settings->getTouchpanelInterfaceType() == QTB_TOUCHPANELINTERFACETYPE_LEFTRIGHT);
  touchpanelInterfaceTypeLeftRight_Action->setStatusTip(tr("Left/Right Type"));
  connect(touchpanelInterfaceTypeLeftRight_Action, SIGNAL(triggered()), this,
		  SLOT(touchpanelInterfaceTypeLeftRight()));

  touchpanelInterfaceTypeTopBottom_Action = new QAction(tr("Top/Bottom Type"), this);
  touchpanelInterfaceTypeTopBottom_Action->setEnabled(true);
  touchpanelInterfaceTypeTopBottom_Action->setCheckable(true);
  touchpanelInterfaceTypeTopBottom_Action->setChecked(
					settings->getTouchpanelInterfaceType() == QTB_TOUCHPANELINTERFACETYPE_TOPBOTTOM);
  touchpanelInterfaceTypeTopBottom_Action->setStatusTip(tr("Top/Bottom Type"));
  connect(touchpanelInterfaceTypeTopBottom_Action, SIGNAL(triggered()), this,
		  SLOT(touchpanelInterfaceTypeTopBottom()));

  touchpanelInterfaceTypeBottom_LeftCenter_Action = new QAction(tr("Bottom Left/Center Type"), this);
  touchpanelInterfaceTypeBottom_LeftCenter_Action->setEnabled(true);
  touchpanelInterfaceTypeBottom_LeftCenter_Action->setCheckable(true);
  touchpanelInterfaceTypeBottom_LeftCenter_Action->setChecked(
					settings->getTouchpanelInterfaceType() == QTB_TOUCHPANELINTERFACETYPE_BOTTOM_LEFTCENTER);
  touchpanelInterfaceTypeBottom_LeftCenter_Action->setStatusTip(tr("Bottom Left/Center Type"));
  connect(touchpanelInterfaceTypeBottom_LeftCenter_Action, SIGNAL(triggered()), this,
		  SLOT(touchpanelInterfaceTypeBottom_LeftCenter()));

  touchpanelInterfaceTypeTop_LeftCenter_Action = new QAction(tr("Top Left/Center Type"), this);
  touchpanelInterfaceTypeTop_LeftCenter_Action->setEnabled(true);
  touchpanelInterfaceTypeTop_LeftCenter_Action->setCheckable(true);
  touchpanelInterfaceTypeTop_LeftCenter_Action->setChecked(
					settings->getTouchpanelInterfaceType() == QTB_TOUCHPANELINTERFACETYPE_TOP_LEFTCENTER);
  touchpanelInterfaceTypeTop_LeftCenter_Action->setStatusTip(tr("Top Left/Center Type"));
  connect(touchpanelInterfaceTypeTop_LeftCenter_Action, SIGNAL(triggered()), this,
		  SLOT(touchpanelInterfaceTypeTop_LeftCenter()));
#endif // defined(QTB_DEV_TOUCHPANEL)

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

#if QTB_DESKTOP_COMPRESS_MODE
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
#endif // QTB_DESKTOP_COMPRESS_MODE

#if defined(QTB_DEV_TOUCHPANEL)
  // decode option for touchpanel
  decodeOptionTypeCPP_Action = new QAction("C++", this);
  decodeOptionTypeCPP_Action->setEnabled(true);
  decodeOptionTypeCPP_Action->setCheckable(true);
  decodeOptionTypeCPP_Action->setChecked(settings->getSIMDOperationTypeName() == "C++");
  decodeOptionTypeCPP_Action->setStatusTip(tr("Decode Type : C++"));
  connect(decodeOptionTypeCPP_Action, SIGNAL(triggered()), this, SLOT(decodeOptionTypeCPP()));

  decodeOptionTypeNEON_Action = new QAction("NEON", this);
  decodeOptionTypeNEON_Action->setEnabled(true);
  decodeOptionTypeNEON_Action->setCheckable(true);
  decodeOptionTypeNEON_Action->setChecked(settings->getSIMDOperationTypeName() == "NEON");
  decodeOptionTypeNEON_Action->setStatusTip(tr("Decode Type : NEON"));
  connect(decodeOptionTypeNEON_Action, SIGNAL(triggered()), this, SLOT(decodeOptionTypeNEON()));

  decodeOptionThread1_Action = new QAction(tr("1 thread"), this);
  decodeOptionThread1_Action->setEnabled(true);
  decodeOptionThread1_Action->setCheckable(true);
  decodeOptionThread1_Action->setChecked(settings->getConvertThreadCount() == 1);
  decodeOptionThread1_Action->setStatusTip(tr("Decode Thread : 1 thread"));
  connect(decodeOptionThread1_Action, SIGNAL(triggered()), this, SLOT(decodeOptionThread1()));

  decodeOptionThread2_Action = new QAction(tr("2 threads"), this);
  decodeOptionThread2_Action->setEnabled(true);
  decodeOptionThread2_Action->setCheckable(true);
  decodeOptionThread2_Action->setChecked(settings->getConvertThreadCount() == 2);
  decodeOptionThread2_Action->setStatusTip(tr("Decode Thread : 2 threads"));
  connect(decodeOptionThread2_Action, SIGNAL(triggered()), this, SLOT(decodeOptionThread2()));

  decodeOptionThread4_Action = new QAction(tr("4 threads"), this);
  decodeOptionThread4_Action->setEnabled(true);
  decodeOptionThread4_Action->setCheckable(true);
  decodeOptionThread4_Action->setChecked(settings->getConvertThreadCount() == 4);
  decodeOptionThread4_Action->setStatusTip(tr("Decode Thread : 4 threads"));
  connect(decodeOptionThread4_Action, SIGNAL(triggered()), this, SLOT(decodeOptionThread4()));
#endif // defined(QTB_DEV_TOUCHPANEL)

#if QTB_PREFERENCE
  // preferences
  preferences_Action = new QAction(tr("Preferences..."), this);
  preferences_Action->setStatusTip(tr("Preferences..."));
  preferences_Action->setEnabled(true);
  connect(preferences_Action, SIGNAL(triggered()), this, SLOT(preferences()));
#endif // QTB_PREFERENCE

  // disable drawing
  disableDrawing_Action = new QAction(tr("Disable Drawing"), this);
  disableDrawing_Action->setStatusTip(tr("Disable Drawing"));
  disableDrawing_Action->setEnabled(true);
  disableDrawing_Action->setCheckable(true);
  connect(disableDrawing_Action, SIGNAL(triggered()), this, SLOT(disableDrawing()));

  // disable maxfps
  disableMaxfps_Action = new QAction(tr("Disable Maxfps"), this);
  disableMaxfps_Action->setStatusTip(tr("Disable Maxfps"));
  disableMaxfps_Action->setEnabled(true);
  disableMaxfps_Action->setCheckable(true);
  connect(disableMaxfps_Action, SIGNAL(triggered()), this, SLOT(disableMaxfps()));

#if QTB_BENCHMARK
  if (onBenchmarkMenu){
	// select phase for benchmark
	selectBenchmarkPhase0_Action = new QAction(tr("Select Phase 0"), this);
	selectBenchmarkPhase0_Action->setStatusTip(tr("Select Phase 0"));
	selectBenchmarkPhase0_Action->setEnabled(false);
	selectBenchmarkPhase0_Action->setCheckable(false);
	connect(selectBenchmarkPhase0_Action, SIGNAL(triggered()), this, SLOT(selectBenchmarkPhase0()));
	selectBenchmarkPhase1_Action = new QAction(tr("Select Phase 1"), this);
	selectBenchmarkPhase1_Action->setStatusTip(tr("Select Phase 1"));
	selectBenchmarkPhase1_Action->setEnabled(false);
	selectBenchmarkPhase1_Action->setCheckable(false);
	connect(selectBenchmarkPhase1_Action, SIGNAL(triggered()), this, SLOT(selectBenchmarkPhase1()));
	selectBenchmarkPhase2_Action = new QAction(tr("Select Phase 2"), this);
	selectBenchmarkPhase2_Action->setStatusTip(tr("Select Phase 2"));
	selectBenchmarkPhase2_Action->setEnabled(false);
	selectBenchmarkPhase2_Action->setCheckable(false);
	connect(selectBenchmarkPhase2_Action, SIGNAL(triggered()), this, SLOT(selectBenchmarkPhase2()));
	selectBenchmarkPhase3_Action = new QAction(tr("Select Phase 3"), this);
	selectBenchmarkPhase3_Action->setStatusTip(tr("Select Phase 3"));
	selectBenchmarkPhase3_Action->setEnabled(false);
	selectBenchmarkPhase3_Action->setCheckable(false);
	connect(selectBenchmarkPhase3_Action, SIGNAL(triggered()), this, SLOT(selectBenchmarkPhase3()));
	selectBenchmarkPhase4_Action = new QAction(tr("Select Phase 4"), this);
	selectBenchmarkPhase4_Action->setStatusTip(tr("Select Phase 4"));
	selectBenchmarkPhase4_Action->setEnabled(false);
	selectBenchmarkPhase4_Action->setCheckable(false);
	connect(selectBenchmarkPhase4_Action, SIGNAL(triggered()), this, SLOT(selectBenchmarkPhase4()));
  }
#endif // QTB_BENCHMARK
}

// create Menus
void QtBrynhildr::createMenus()
{
  // menu bar setting
  if (settings->getOnNativeMenuBarDisable()){
	// disable native menu bar
	menuBar()->setNativeMenuBar(false);
  }

  // file menu
  fileMenu = menuBar()->addMenu(tr("File"));
  fileMenu->addAction(connectToServer_Action);
  fileMenu->addAction(disconnectToServer_Action);
  if (settings->getOnTransferFileSupport() || settings->getOnTransferClipboardSupport()){
	fileMenu->addSeparator();
  }
#if 0 // disable now
  if (settings->getOnTransferClipboardSupport())
	fileMenu->addAction(sendClipboard_Action);
#endif // disable now
  if (settings->getOnTransferFileSupport()){
	fileMenu->addAction(sendFile_Action);
	fileMenu->addAction(cancelFileTransferring_Action);
  }
#if QTB_PREFERENCE
  fileMenu->addSeparator();
  fileMenu->addAction(preferences_Action);
#endif // QTB_PREFERENCE
  fileMenu->addAction(initializeSettings_Action);
#if 1 // defined(QTB_DEV_DESKTOP)
  fileMenu->addSeparator();
  fileMenu->addAction(exit_Action);
#endif // defined(QTB_DEV_DESKTOP)

  // display menu
#if defined(QTB_DEV_DESKTOP)
  displayMenu = menuBar()->addMenu(tr("Display"));
#endif // defined(QTB_DEV_DESKTOP)
#if 0 // disable now
  displayMenu->addAction(showMenuBar_Action);
#endif // disable now
#if defined(QTB_DEV_DESKTOP)
#if QTB_TOOLBAR
  displayMenu->addAction(showToolBar_Action);
#endif // QTB_TOOLBAR
  displayMenu->addAction(showStatusBar_Action);
#endif // defined(QTB_DEV_DESKTOP)
#if defined(QTB_DEV_DESKTOP)
  displayMenu->addAction(showFrameRate_Action);
#endif // defined(QTB_DEV_DESKTOP)

  // software keyboard and button
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
#if defined(QTB_DEV_DESKTOP)
  displayMenu->addSeparator();
  displayMenu->addAction(showSoftwareButton_Action);
  displayMenu->addAction(showSoftwareKeyboard_Action);
#endif // defined(QTB_DEV_DESKTOP)
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
#endif // defined(QTB_DEV_DESKTOP)

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
  selectFrameRateSubMenu->addAction(selectFrameRateMinimum_Action);
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
  soundMenu->setEnabled(settings->getOnSound());
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
#if 0 // disable now
  sendKeySubMenu->addAction(sendKey1_Action);
#endif // disable now
  sendKeySubMenu->addAction(sendKey2_Action);
  sendKeySubMenu->addAction(sendKey3_Action);
  sendKeySubMenu->addAction(sendKey4_Action);
  sendKeySubMenu->addAction(sendKey5_Action);
  sendKeySubMenu->addAction(sendKey6_Action);
  if (settings->getOnSendControlKeyState()){
	sendKeySubMenu->addAction(sendKey7_Action);
  }

#if defined(QTB_DEV_DESKTOP)
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
  selectMonitorNoSubMenu->addAction(selectMonitorNoAll_Action);
#endif // defined(QTB_DEV_DESKTOP)

#if defined(QTB_DEV_DESKTOP)
  // for control
  controlMenu->addSeparator();
  controlMenu->addAction(onControl_Action);
  controlMenu->addAction(onGraphics_Action);
  controlMenu->addAction(onSound_Action);
#endif // defined(QTB_DEV_DESKTOP)

  // for select publicmode version
  controlMenu->addSeparator();
  selectPublicModeVersionSubMenu = controlMenu->addMenu(tr("Select Public Mode Version"));
  selectPublicModeVersionSubMenu->addAction(selectPublicModeVersion5_Action);
  selectPublicModeVersionSubMenu->addAction(selectPublicModeVersion6_Action);
  selectPublicModeVersionSubMenu->addAction(selectPublicModeVersion7_Action);

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

#if QTB_PLUGINS_DISABLE_SUPPORT && defined(QTB_DEV_DESKTOP)
  // for plugins disable
  controlMenu->addSeparator();
  controlMenu->addAction(onPluginsDisable_Action);
#endif // QTB_PLUGINS_DISABLE_SUPPORT && defined(QTB_DEV_DESKTOP)

  // option menu
  optionMenu = menuBar()->addMenu(tr("Option"));
#if QTB_DESKTOP_COMPRESS_MODE
  // desktop compress mode
  desktopCompressModeSubMenu = optionMenu->addMenu(tr("Desktop Compress Mode"));
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
  optionMenu->addSeparator();
#endif // QTB_DESKTOP_COMPRESS_MODE

#if defined(QTB_DEV_TOUCHPANEL)
  // decode option
  decodeOptionSubMenu = optionMenu->addMenu(tr("Decode Option"));
  decodeOptionTypeSubMenu = decodeOptionSubMenu->addMenu(tr("Type"));
  decodeOptionTypeSubMenu->addAction(decodeOptionTypeCPP_Action);
  decodeOptionTypeSubMenu->addAction(decodeOptionTypeNEON_Action);

  decodeOptionThreadSubMenu = decodeOptionSubMenu->addMenu(tr("Thread"));
  decodeOptionThreadSubMenu->addAction(decodeOptionThread1_Action);
  decodeOptionThreadSubMenu->addAction(decodeOptionThread2_Action);
  decodeOptionThreadSubMenu->addAction(decodeOptionThread4_Action);

  if (settings->getPublicModeVersion() == PUBLICMODE_VERSION7){
	decodeOptionSubMenu->setEnabled(true);
  }
  else {
	decodeOptionSubMenu->setEnabled(false);
  }
#endif // defined(QTB_DEV_TOUCHPANEL)

  if (QTB_VIEWER_MODE){
	optionMenu->addAction(onViewerMode_Action);
	optionMenu->addAction(onFulFulMode_Action);
  }

  if (QTB_SCROLL_MODE){
	optionMenu->addAction(onScrollMode_Action);
  }

#if QTB_GRAY_SCALE_MODE
  // monochrome mode
  optionMenu->addAction(onMonochromeMode_Action);
#endif // QTB_GRAY_SCALE_MODE

#if QTB_MOUSE_TRACKING_FOCUS_MODE
  // mouse tracking mode
  optionMenu->addAction(onMouseTrackingMode_Action);
#endif // QTB_MOUSE_TRACKING_FOCUS_MODE

#if defined(QTB_DEV_TOUCHPANEL)
  // touchpanel operation type
  touchpanelOperationTypeSubMenu = optionMenu->addMenu(tr("Touchpanel Operation"));
  touchpanelOperationTypeSubMenu->addAction(touchpanelOperationTypeKeroRemote_Action);
  touchpanelOperationTypeSubMenu->addAction(touchpanelOperationTypeQtBrynhildr_Action);

  // touchpanel interface type
  touchpanelInterfaceTypeSubMenu = optionMenu->addMenu(tr("Touchpanel Interface"));
  touchpanelInterfaceTypeSubMenu->addAction(touchpanelInterfaceTypeLeftRight_Action);
  touchpanelInterfaceTypeSubMenu->addAction(touchpanelInterfaceTypeTopBottom_Action);
  touchpanelInterfaceTypeSubMenu->addAction(touchpanelInterfaceTypeBottom_LeftCenter_Action);
  touchpanelInterfaceTypeSubMenu->addAction(touchpanelInterfaceTypeTop_LeftCenter_Action);
#endif // defined(QTB_DEV_TOUCHPANEL)

#if 1 // defined(QTB_DEV_DESKTOP)
  optionMenu->addSeparator();
  optionMenu->addAction(outputKeyboardLog_Action);
  optionMenu->addAction(outputLog_Action);
#endif // defined(QTB_DEV_DESKTOP)

  // in testing sub menu
  if (QTB_IN_TESTING){
	optionMenu->addSeparator();
	inTestingSubMenu = optionMenu->addMenu(tr("In Testing"));
	// in Testing Menu
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
#if QTB_HELP_BROWSER
  helpMenu->addAction(helpBrowser_Action);
  helpMenu->addSeparator();
#endif // QTB_HELP_BROWSER
  helpMenu->addAction(aboutQt_Action);
  helpMenu->addAction(about_Action);

  // test mode menu
  if (option->getTestModeFlag()){
	helpMenu->addSeparator();
	// disable drawing
	helpMenu->addAction(disableDrawing_Action);
	// disable maxfps
	helpMenu->addAction(disableMaxfps_Action);
  }

#if QTB_BENCHMARK
  if (onBenchmarkMenu){
	// benchmark menu
	benchmarkMenu = menuBar()->addMenu(tr("Benchmark"));
	benchmarkMenu->addAction(selectBenchmarkPhase0_Action);
	benchmarkMenu->addAction(selectBenchmarkPhase1_Action);
	benchmarkMenu->addAction(selectBenchmarkPhase2_Action);
	benchmarkMenu->addAction(selectBenchmarkPhase3_Action);
	benchmarkMenu->addAction(selectBenchmarkPhase4_Action);

	// refresh benchmark menu
	refreshBenchmarkMenu();
  }
#endif // QTB_BENCHMARK
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

#if QTB_TOOLBAR
// create Tool Bar
void QtBrynhildr::createToolBars()
{
  toolBar = addToolBar(tr("ToolBar"));
  toolBar->setObjectName("ToolBar");
  toolBar->setOrientation(Qt::Horizontal);
  toolBar->setFloatable(true);
  toolBar->setMovable(true);

  // Full Screen button
  toolBar->addAction(fullScreen_Action); // for TEST

  // Send Key button
  toolBar->addSeparator(); // for TEST
  toolBar->addAction(sendKey2_Action); // for TEST
  toolBar->addAction(sendKey3_Action); // for TEST
  toolBar->addAction(sendKey4_Action); // for TEST
  toolBar->addAction(sendKey5_Action); // for TEST
  toolBar->addAction(sendKey6_Action); // for TEST
  if (settings->getOnSendControlKeyState()){
	toolBar->addAction(sendKey7_Action); // for TEST
  }

  // Compress Mode button
  toolBar->addSeparator(); // for TEST
  toolBar->addAction(desktopCompressMode0_Action); // for TEST
  toolBar->addAction(desktopCompressMode2_Action); // for TEST
  toolBar->addAction(desktopCompressMode4_Action); // for TEST
  toolBar->addAction(desktopCompressMode8_Action); // for TEST

  // Select Monitor button
  toolBar->addSeparator(); // for TEST
  toolBar->addAction(selectMonitorNo1_Action); // for TEST
  toolBar->addAction(selectMonitorNo2_Action); // for TEST
  toolBar->addAction(selectMonitorNo3_Action); // for TEST
  toolBar->addAction(selectMonitorNo4_Action); // for TEST
#if 0 // for TEST
  toolBar->addAction(selectMonitorNo5_Action); // for TEST
  toolBar->addAction(selectMonitorNo6_Action); // for TEST
  toolBar->addAction(selectMonitorNo7_Action); // for TEST
  toolBar->addAction(selectMonitorNo8_Action); // for TEST
  toolBar->addAction(selectMonitorNo9_Action); // for TEST
#endif // 0 // for TEST
  toolBar->addAction(selectMonitorNoAll_Action); // for TEST

  connect(toolBar, SIGNAL(topLevelChanged(bool)), SLOT(topLevelChanged(bool)));
}
#endif // QTB_TOOLBAR

// create Status Bar
void QtBrynhildr::createStatusBar()
{
  connectionLabel = new QLabel;
  connectionLabel->setAlignment(Qt::AlignLeft);

  frameRateLabel = new QLabel;
  frameRateLabel->setAlignment(Qt::AlignRight);
  frameRateLabel->setText(" "); // dummy text

  // progress bar for transfer file
  progressBar = new QProgressBar(this);
  progressBar->setFixedWidth(this->width()/4);
  progressBar->setFixedHeight((statusBar()->height())/2);
  progressBar->setValue(80); // for TEST
  progressBar->setRange(0,100);
  progressBar->setTextVisible(true);
  progressBar->setVisible(false);
  statusBar()->addPermanentWidget(progressBar);

  statusBar()->addWidget(connectionLabel);
  statusBar()->addPermanentWidget(frameRateLabel);
}

#if QTB_TOOLBAR
// update Tool Bar
void QtBrynhildr::updateToolBar()
{
}
#endif // QTB_TOOLBAR

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
  if (connectionLabel == nullptr){
	// Nothing to do
	return;
  }

  // check executing to connect
  if (isExecutingToConnect){
	QString str = QString(tr("Connecting ... : ")+"%1").
	  arg(settings->getServerName());
	// set label
	connectionLabel->setText(str);
	// set minimum size
	connectionLabel->setMinimumSize(connectionLabel->sizeHint());
	return;
  }

  // set label
  if (settings->getConnected()){
	// connection
	QString str;
#if QTB_BENCHMARK
	if (onBenchmarkMenu){
	  if (settings->getPublicModeVersion() == PUBLICMODE_VERSION7){
		str = QString(tr("Connected : ")+"%1 [ %2x%3 ] [ SF : %4 : %5x%6 ] [ "+tr("ReCalc Rate")+" :  %7 % : %8]").
		  arg(settings->getServerName()).
		  arg(settings->getDesktopWidth(), 3).
		  arg(settings->getDesktopHeight(), 3).
		  arg(settings->getDesktopScalingFactor(), 2, 'f', 4, ' ').
		  arg(GraphicsThread::calcWidthForMode7(settings->getDesktopWidth()*settings->getDesktopScalingFactor()), 3).
		  arg(GraphicsThread::calcHeightForMode7(settings->getDesktopHeight()*settings->getDesktopScalingFactor()), 3).
		  arg(calcRate, 4, 'f', 2, ' ').
		  arg(settings->getSIMDOperationTypeName());
	  }
	  else {
		str = QString(tr("Connected : ")+"%1 [ %2x%3 ] [ SF : %4 : %5x%6 ]").
		  arg(settings->getServerName()).
		  arg(settings->getDesktopWidth(), 3).
		  arg(settings->getDesktopHeight(), 3).
		  arg(settings->getDesktopScalingFactor(), 2, 'f', 4, ' ').
		  arg(GraphicsThread::calcWidth(settings->getDesktopWidth()*settings->getDesktopScalingFactor()), 3).
		  arg(GraphicsThread::calcHeight(settings->getDesktopHeight()*settings->getDesktopScalingFactor()), 3);
	  }
	  // sample rate (sound)
	  str += QString(" : Sound %1 Hz").arg(soundThread->getSampleRate());
	}
	else {
#if defined(QTB_DEV_DESKTOP)
	  if (settings->getDesktopScalingFactor() == 1.0 && settings->getDesktopCompressMode() == 1){
		str = QString(tr("Connected : ")+"%1 [ %2x%3 ]").
		  arg(settings->getServerName()).
		  arg(settings->getDesktopWidth(), 3).
		  arg(settings->getDesktopHeight(), 3);
	  }
	  else {
		int width = settings->getDesktopImageSize().width();
		int height = settings->getDesktopImageSize().height();
		if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_SERVER){
		  if (settings->getDesktopScalingFactor() > 1.0){
			width *= settings->getDesktopScalingFactor();
			height *= settings->getDesktopScalingFactor();
		  }
		}
		else {
			width *= settings->getDesktopScalingFactor();
			height *= settings->getDesktopScalingFactor();
		}
		str = QString(tr("Connected : ")+"%1 [ %2x%3 ] >> [ %4x%5 ]").
		  arg(settings->getServerName()).
		  arg(settings->getDesktopWidth(), 3).
		  arg(settings->getDesktopHeight(), 3).
		  arg(width, 3).
		  arg(height, 3);
	  }
	  // sample rate (sound)
	  str += QString(" : %1 Hz").arg(soundThread->getSampleRate());
#else // !defined(QTB_DEV_DESKTOP)
	  str = QString(tr("Connected : ")+"%1 [ %2x%3 ]").
		arg(settings->getServerName()).
		arg(settings->getDesktopWidth(), 3).
		arg(settings->getDesktopHeight(), 3);
#endif // !defined(QTB_DEV_DESKTOP)
	}
#else // !QTB_BENCHMARK
	if (settings->getDesktopScalingFactor() == 1.0 && settings->getDesktopCompressMode() == 1){
	  str = QString(tr("Connected : ")+"%1 [ %2x%3 ]").
		arg(settings->getServerName()).
		arg(settings->getDesktopWidth(), 3).
		arg(settings->getDesktopHeight(), 3);
	}
	else {
	  int width = settings->getDesktopImageSize().width();
	  int height = settings->getDesktopImageSize().height();
	  if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_SERVER){
		if (settings->getDesktopScalingFactor() > 1.0){
		  width *= settings->getDesktopScalingFactor();
		  height *= settings->getDesktopScalingFactor();
		}
	  }
	  else {
		width *= settings->getDesktopScalingFactor();
		height *= settings->getDesktopScalingFactor();
	  }
	  str = QString(tr("Connected : ")+"%1 [ %2x%3 ] >> [ %4x%5 ]").
		arg(settings->getServerName()).
		arg(settings->getDesktopWidth(), 3).
		arg(settings->getDesktopHeight(), 3).
		arg(width, 3).
		arg(height, 3);
	}
#endif // !QTB_BENCHMARK
	// viewer mode
	if (settings->getOnViewerMode()){
	  str += " [" + tr("Viewer Mode") + "]";
	}
	if (settings->getOnFulFulMode()){
	  str += " [" + tr("FulFul Mode") + "]";
	}
	connectionLabel->setText(str);
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
  if (frameRateLabel == nullptr){
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
	if (settings->getOnShowFrameRate()){ // Status Bar
	  QString str = QString(tr("Frame Rate : ")+"%1  [%2 Mbps] ").
		arg(currentFrameRate, 3).
		arg(currentDataRate, 4, 'f', 1, ' ');
	  frameRateLabel->setText(str);
	}
	else {
	  frameRateLabel->clear();
	}
	if (settings->getOnShowSoftwareButton()){ // Software Button
	  if (softwareButton != nullptr){
		softwareButton->setCurrentFrameRate(currentFrameRate);
		softwareButton->setCurrentDataRate(currentDataRate);
	  }
	}
  }
  else {
	frameRateLabel->clear();
  }
#else // !QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  if (settings->getOnShowFrameRate()){
	if (!settings->getConnected()){
	  currentFrameRate = 0;
	  currentDataRate = 0;
	}
	QString str = QString(tr("Frame Rate : ")+"%1  [%2 Mbps] ").
	  arg(currentFrameRate, 3).
	  arg(currentDataRate, 4, 'f', 1, ' ');
	// total data counter
	if (settings->getDisplayDataCounterType() != QTB_DISPLAYDATACOUNTERTYPE_NONE){
	  qreal trd = 0.0;
	  if (controlThread != nullptr){
		switch(settings->getDisplayDataCounterType()){
		case QTB_DISPLAYDATACOUNTERTYPE_TOTAL:
		  trd += controlThread->getTotalReceivedDataCounter();
		  trd += graphicsThread->getTotalReceivedDataCounter();
		  trd += soundThread->getTotalReceivedDataCounter();
		  break;
		case QTB_DISPLAYDATACOUNTERTYPE_CONTROL:
		  trd += controlThread->getTotalReceivedDataCounter();
		  break;
		case QTB_DISPLAYDATACOUNTERTYPE_GRAPHICS:
		  trd += graphicsThread->getTotalReceivedDataCounter();
		  break;
		case QTB_DISPLAYDATACOUNTERTYPE_SOUND:
		  trd += soundThread->getTotalReceivedDataCounter();
		  break;
		default:
		  // internal error
		  // Nothing to do
		  break;
		}

		trd /= (1024*1024); // MB
	  }
	  if (trd > 1024*1024){
		// display by TB
		trd /= (1024*1024);
		str += QString(" (%1 TB) ").arg(trd, 4, 'f', 2, ' ');
	  }
	  else if (trd > 1024){
		// display by GB
		trd /= 1024;
		str += QString(" (%1 GB) ").arg(trd, 4, 'f', 2, ' ');
	  }
	  else {
		// display by MB
		str += QString(" (%1 MB) ").arg(trd, 4, 'f', 2, ' ');
	  }
	}
	frameRateLabel->setText(str);
  }
  else {
	frameRateLabel->clear();
  }
#endif // !QTB_SOFTWARE_KEYBOARD_AND_BUTTON
}

// connected
void QtBrynhildr::connected()
{
  // enabled software keyboard/button
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
#if defined(Q_OS_WIN)
  if (settings->getKeyboardType() != KEYBOARD_TYPE_NATIVE){
	if (showSoftwareKeyboard_Action != nullptr)
	  showSoftwareKeyboard_Action->setEnabled(true);

	if (showSoftwareButton_Action != nullptr)
	  showSoftwareButton_Action->setEnabled(true);
  }
#else // !defined(Q_OS_WIN)
  if (showSoftwareKeyboard_Action != nullptr)
	showSoftwareKeyboard_Action->setEnabled(true);

  if (showSoftwareButton_Action != nullptr)
	showSoftwareButton_Action->setEnabled(true);
#endif // !defined(Q_OS_WIN)
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // enabled disconnect to server
  disconnectToServer_Action->setEnabled(true);

  // enabled send key
  if (sendKey1_Action != nullptr)
    sendKey1_Action->setEnabled(true);
  if (sendKey2_Action != nullptr)
    sendKey2_Action->setEnabled(true);
  if (sendKey3_Action != nullptr)
    sendKey3_Action->setEnabled(true);
  if (sendKey4_Action != nullptr)
    sendKey4_Action->setEnabled(true);
  if (sendKey5_Action != nullptr)
    sendKey5_Action->setEnabled(true);
  if (sendKey6_Action != nullptr)
    sendKey6_Action->setEnabled(true);
  if (sendKey7_Action != nullptr)
    sendKey7_Action->setEnabled(true);

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

  // enable viewer mode
  if (QTB_VIEWER_MODE){
	onViewerMode_Action->setEnabled(true);
	onFulFulMode_Action->setEnabled(true);
  }

#if 0 //QTB_GRAY_SCALE_MODE
  // monochrome mode
  onMonochromeMode_Action->setEnabled(true);
#endif // QTB_GRAY_SCALE_MODE

#if 0 //QTB_MOUSE_TRACKING_FOCUS_MODE
  // mouse tracking mode
  onMouseTrackingMode_Action->setEnabled(true);
#endif // QTB_MOUSE_TRACKING_FOCUS_MODE

  // enable full screen
#if defined(QTB_DEV_DESKTOP)
  if (QTB_DESKTOP_FULL_SCREEN && settings->getDesktopCompressMode() == QTB_DESKTOPCOMPRESSMODE_DEFAULT){
	fullScreen_Action->setEnabled(true);
  }
#endif // defined(QTB_DEV_DESKTOP)

  // enable desktop scale fixed
  if (QTB_DESKTOP_SCALE_FIXED){
	desktopScaleFixed_Action->setEnabled(true);
  }

  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// send clipboard
	sendClipboard_Action->setEnabled(true);

	// send file
	sendFile_Action->setEnabled(true);

	// cancel file transferring
	cancelFileTransferring_Action->setEnabled(false);

	// drag and drop
#if defined(QTB_DEV_TOUCHPANEL)
#if 0 // for TEST
	desktopFrameObject->setAcceptDrops(true);
#endif // 0 // for TEST
#else // !defined(QTB_DEV_TOUCHPANEL)
	desktopWindowWidget->setAcceptDrops(true);
#endif // !defined(QTB_DEV_TOUCHPANEL)
  }

#if QTB_PLUGINS_DISABLE_SUPPORT
  // plugins disable
  onPluginsDisable_Action->setEnabled(true);
#endif // QTB_PLUGINS_DISABLE_SUPPORT

#if QTB_DESKTOP_COMPRESS_MODE
  // desktop compress mode
  if (settings->getPublicModeVersion() <= PUBLICMODE_VERSION7){
	desktopCompressModeSubMenu->setEnabled(true);
  }
  else {
	desktopCompressModeSubMenu->setEnabled(false);
  }
#endif // QTB_DESKTOP_COMPRESS_MODE

#if defined(QTB_DEV_TOUCHPANEL)
  // decode option for touchpanel
  decodeOptionSubMenu->setEnabled(false);
#endif // defined(QTB_DEV_TOUCHPANEL)

  // reset total frame counter
  totalFrameCounter = 0;

  // initialize mouse cursor
  if (settings->getOnDisplayMouseCursor()){
	menuBar()->setCursor(cursor());
	changeMouseCursor(Qt::CrossCursor);
  }

  // set window title
  setupWindowTitle();

  // refresh menu
  refreshMenu();

#if 0 // defined(QTB_DEV_TOUCHPANEL)
  // save settings
  settings->writeSettings();
#endif // defined(QTB_DEV_TOUCHPANEL)

  // full screen at connected
  fullScreenMode = false;
  onSetDesktopScalingFactorForFullScreen = false;
  if (settings->getOnFullScreenAtConnected()){
	fullScreen();
	onSetDesktopScalingFactorForFullScreen = true;
  }

  // viewer mode at connected
  if (settings->getOnViewerAtConnected()){
	toggleOnViewerMode();
  }

  // try to connect flag
  isExecutingToConnect = false;
  updateConnected();

  // disable connect to server menu
  connectToServer_Action->setEnabled(true);
  // enable disconnect to server menu
  disconnectToServer_Action->setEnabled(true);

  // disable initialize settings menu
  initializeSettings_Action->setEnabled(false);

  // reset control key status
  onControlKey = false;
}

// disconnected
void QtBrynhildr::disconnected()
{
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // disabled software keyboard/button
  if (showSoftwareKeyboard_Action != nullptr)
	showSoftwareKeyboard_Action->setEnabled(false);

  if (showSoftwareButton_Action != nullptr)
	  showSoftwareButton_Action->setEnabled(false);
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // disabled disconnect to server
  disconnectToServer_Action->setEnabled(false);

  // disabled send key
  if (sendKey1_Action != nullptr)
    sendKey1_Action->setEnabled(false);
  if (sendKey2_Action != nullptr)
    sendKey2_Action->setEnabled(false);
  if (sendKey3_Action != nullptr)
    sendKey3_Action->setEnabled(false);
  if (sendKey4_Action != nullptr)
    sendKey4_Action->setEnabled(false);
  if (sendKey5_Action != nullptr)
    sendKey5_Action->setEnabled(false);
  if (sendKey6_Action != nullptr)
    sendKey6_Action->setEnabled(false);
  if (sendKey7_Action != nullptr)
    sendKey7_Action->setEnabled(false);

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

#if 0 //QTB_GRAY_SCALE_MODE
  onMonochromeMode_Action->setEnabled(false);
#endif // QTB_GRAY_SCALE_MODE

#if 0 //QTB_MOUSE_TRACKING_FOCUS_MODE
  // mouse tracking mode
  onMouseTrackingMode_Action->setEnabled(false);
#endif // QTB_MOUSE_TRACKING_FOCUS_MODE

  // disabled viewer mode
  if (QTB_VIEWER_MODE){
	onViewerMode_Action->setEnabled(false);
	if (settings->getOnViewerMode()){
	  // viewer mode OFF
	  toggleOnViewerMode();
	}

  	onFulFulMode_Action->setEnabled(false);
	if (settings->getOnFulFulMode()){
	  // viewer mode OFF
	  toggleOnFulFulMode();
	}
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

  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// send clipboard
	sendClipboard_Action->setEnabled(false);

	// send file
	sendFile_Action->setEnabled(false);

	// cancel file transferring
	cancelFileTransferring_Action->setEnabled(false);

	// drag and drop
#if defined(QTB_DEV_TOUCHPANEL)
#if 0 // for TEST
	desktopFrameObject->setAcceptDrops(false);
#endif // 0 // for TEST
#else // !defined(QTB_DEV_TOUCHPANEL)
	desktopWindowWidget->setAcceptDrops(false);
#endif // !defined(QTB_DEV_TOUCHPANEL)
  }

#if defined(QTB_DEV_TOUCHPANEL)
  // decode option for touchpanel
  if (settings->getPublicModeVersion() == PUBLICMODE_VERSION7){
	decodeOptionSubMenu->setEnabled(true);
  }
  else {
	decodeOptionSubMenu->setEnabled(false);
  }
#endif // defined(QTB_DEV_TOUCHPANEL)

#if QTB_PLUGINS_DISABLE_SUPPORT
  // plugins disable
  settings->setOnPluginsDisable(false);
  onPluginsDisable_Action->setChecked(false);
  onPluginsDisable_Action->setEnabled(false);
#endif // QTB_PLUGINS_DISABLE_SUPPORT

  // normal screen
  if (QTB_DESKTOP_FULL_SCREEN){
	if (fullScreenMode){
	  // to Normal Screen
	  exitFullScreen();
	}
  }

  // reset pop up Connect To Server Dialog flag
  onPopUpConnectToServer = false;

  // try to connect flag
  isExecutingToConnect = false;
  updateConnected();

#if QTB_TOOLBAR
  // update tool bar
  updateToolBar();
#endif // QTB_TOOLBAR

  // enable connect to server menu
  connectToServer_Action->setEnabled(true);
  // disable disconnect to server menu
  disconnectToServer_Action->setEnabled(false);

  // disable initialize settings menu
  initializeSettings_Action->setEnabled(true);

  // reset control key status
  onControlKey = false;
}

// set desktop scaling factor
void QtBrynhildr::setDesktopScalingFactor(QSize windowSize)
{
  if (settings->getOnDesktopScaleFixed()){
	// NOT change scaling
	return;
  }
  QSize desktopImageSize = settings->getDesktopImageSize();
  if (!desktopImageSize.isValid()){
	return;
  }

  int width = windowSize.width();
  int height = windowSize.height() - getHeightOfMenuBar() - getHeightOfStatusBar();
#if QTB_TOOLBAR
  width -= getWidthOfToolBar();
  height -= getHeightOfToolBar();
#endif // QTB_TOOLBAR
#if !defined(QTB_DEV_TOUCHPANEL)
  // correct
  width  -= widthMargin;
  height -= heightMargin;
#endif // !defined(QTB_DEV_TOUCHPANEL)

#if defined(QTB_DEV_DESKTOP)
  QSize screenSize = settings->getCurrentScreenSize();

  if (desktopFrame->getSize().width() > screenSize.width()){
	width = desktopFrame->getSize().width();
  }
  if (desktopFrame->getSize().height() > screenSize.height()){
	height = desktopFrame->getSize().height();
  }
#endif // defined(QTB_DEV_DESKTOP)

  int desktopWidth = settings->getDesktopWidth();
  int desktopHeight = settings->getDesktopHeight();
  qreal widthFactor = (qreal)width/desktopWidth;
  qreal heightFactor = (qreal)height/desktopHeight;
  if (widthFactor < heightFactor){
	settings->setDesktopScalingFactor(widthFactor);
  }
  else {
	settings->setDesktopScalingFactor(heightFactor);
  }
  //std::cout << "setDesktopScalingFactor():" << settings->getDesktopScalingFactor() << std::endl << std::flush;
#if 0 // for TEST
  qDebug() << "screenSize = " << screenSize;
  qDebug() << "width = " << width;
  qDebug() << "height = " << height;
  qDebug() << "desktopWidth = " << desktopWidth;
  qDebug() << "desktopHeight = " << desktopHeight;
  qDebug() << "widthFactor = " << widthFactor;
  qDebug() << "heightFactor = " << heightFactor;
  qDebug() << "scalingFactor = " << settings->getDesktopScalingFactor();
#endif // 0 // for TEST

#if defined(QTB_DEV_TOUCHPANEL)
  desktopPanel->setScale(settings->getDesktopScalingFactor());
#endif // defined(QTB_DEV_TOUCHPANEL)
}

#if 0 // disable now
// change event
void QtBrynhildr::changeEvent(QEvent *event)
{
  QMainWindow::changeEvent(event);

  if (event->type() == QEvent::WindowStateChange){
	Qt::WindowState state = windowState();
	//  OR'ed Qt::WindowNoState, Qt::WindowMaximized, Qt::WindowMinimized, Qt::WindowActive
  }
}
#endif // disable now

// close event by window close
void QtBrynhildr::closeEvent(QCloseEvent *event)
{
  event->ignore();

  exit();
}

// window resize event
void QtBrynhildr::resizeEvent(QResizeEvent *event)
{
  QMainWindow::resizeEvent(event);

  //std::cout << "resizeEvent()" << std::endl << std::flush;

#if !defined(QTB_DEV_TOUCHPANEL)
  // rescaling desktop
#if 0 // for TEST
  std::cout << "resizeEvent() : Rescaling for (width, height) = ("
	   << event->size().width() << "," << event->size().height() << ")" << std::endl << std::flush;
#endif // for TEST
  //setDesktopScalingFactor(event->size());

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
#endif // !defined(QTB_DEV_TOUCHPANEL)
}

// window hide event
void QtBrynhildr::hideEvent(QHideEvent *event)
{
  QMainWindow::hideEvent(event);

  if (settings->getConnected()){
	//	std::cout << "hideEvent()" << std::endl << std::flush;

	// set frame rate to 1
	if (savedFrameRate == 0){
	  // save current frame rate
	  savedFrameRate = settings->getFrameRate();
	  // set temporary frame rate
	  settings->setFrameRate(1);
	}
  }
}

// window show event
void QtBrynhildr::showEvent(QShowEvent *event)
{
  QMainWindow::showEvent(event);

  if (settings->getConnected()){
	//	std::cout << "showEvent()" << std::endl << std::flush;

	// restore frame rate
	if (savedFrameRate != 0){
	  // set temporary frame rate
	  settings->setFrameRate(savedFrameRate);
	  // reset saved frame rate
	  savedFrameRate = 0;
	}
  }
}

// context menu event
void QtBrynhildr::contextMenuEvent(QContextMenuEvent *event)
{
  switch(event->reason()){
  case QContextMenuEvent::Mouse:
	//	std::cout << "Context Menu Event by Mouse (Right button)" << std::endl << std::flush;
	break;
  case QContextMenuEvent::Keyboard:
	//	std::cout << "Context Menu Event by Keyboard (Menu button)" << std::endl << std::flush;
	break;
  case QContextMenuEvent::Other:
	//	std::cout << "Context Menu Event by Other" << std::endl << std::flush;
	break;
  default:
	//	std::cout << "Context Menu Event by Unknown" << std::endl << std::flush;
	break;
  }

  QMainWindow::contextMenuEvent(event);
}

// load settings from setting files or registry
void QtBrynhildr::readSettings()
{
  // read global settings
  if (!option->getInitFlag()){
	settings->readSettings();
  }

#if defined(QTB_DEV_DESKTOP)
  // restore geometry
#if 0 // for all platform
  QRect defaultRect = QRect(200, 200, 800, 600);

  bool result = restoreGeometry(settings->getSettings()->value(QTB_GEOMETRY).toByteArray());
  if (!result){
	move(defaultRect.topLeft());
	resize(defaultRect.size());
  }
#else // 1 // for all platform
  QPoint pos = settings->getSettings()->value(QTB_WINDOWPOS, QVariant(QPoint(200, 200))).toPoint();
  QSize size = settings->getSettings()->value(QTB_WINDOWSIZE, QVariant(QSize(800, 600))).toSize();
  move(pos);
  resize(size);
#endif // 1 // for all platform

  // restore window state
  restoreState(settings->getSettings()->value(QTB_WINDOWSTATE).toByteArray());
#endif // defined(QTB_DEV_DESKTOP)
}

// save settings to setting file or registry
void QtBrynhildr::writeSettings()
{
  // write global settings
  settings->writeSettings();

#if defined(QTB_DEV_DESKTOP)
  // save geometry
#if 0 // for all platform
  settings->getSettings()->setValue(QTB_GEOMETRY, saveGeometry());
  //qDebug() << "frame geometry: " << frameGeometry();
  //qDebug() << "geometry: " << geometry();
#else // 1 // for all platform
  settings->getSettings()->setValue(QTB_WINDOWPOS, pos());
  settings->getSettings()->setValue(QTB_WINDOWSIZE, size());
#endif // 1 // for all platform

  // save window state
  settings->getSettings()->setValue(QTB_WINDOWSTATE, saveState());
#endif // defined(QTB_DEV_DESKTOP)

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
					 QTB_VERSION QTB_RCNAME QTB_ARCHNAME
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
					 "blog : " QTB_BLOG
#if QTB_MANUAL_PAGE
					 "<br><br>"
					 "manual : " QTB_MANUAL
#endif // QTB_MANUAL_PAGE
					 "<br><br>"
					 "privacy : " QTB_PRIVACY_POLICY
					 );
}

// popup connect to server dialog
void QtBrynhildr::popUpConnectToServer()
{
  onPopUpConnectToServer = true;

  // pop up connect to server dialog
  QPoint pos = getInitialDialogPos(connectToServerDialog);
  connectToServerDialog->move(pos);
  connectToServerDialog->show();
}

#if QTB_UPDATECHECK
// check update
void QtBrynhildr::checkUpdate()
{
  //  std::cout << "enter checkUpdate()" << std::endl << std::flush;

  // start downloading release page
  //  bool result = httpGetter->startDownload(QTB_URL_FOR_RELEASE, "release_page.html");
  bool result = httpGetter->startDownload(QTB_URL_FOR_RELEASE);
  if (!result){
	std::cout << "Failed to http access" << std::endl << std::flush;
  }

  //  std::cout << "leave checkUpdate()" << std::endl << std::flush;
}
#endif // QTB_UPDATECHECK

#if QTB_HELP_BROWSER
// help browser
void QtBrynhildr::helpBrowser()
{
  HelpBrowser::showPage("index.html");
}
#endif // QTB_HELP_BROWSER

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
  disconnectToServer();
  // wait for reconnect to server
  QThread::sleep(1);

  // disable connect to server menu
  connectToServer_Action->setEnabled(false);
  // disabled disconnect to server
  disconnectToServer_Action->setEnabled(true);

  // clear desktop
  desktopFrame->clearDesktop();

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
#else // !defined(Q_OS_WIN)
	ABORT(); // available for windows only
#endif // !defined(Q_OS_WIN)
	break;
  default: // key layout file
#ifdef USE_KEYLAYOUTFILE
	int index = keyboardType - KEYBOARD_TYPE_NATIVE - 1;
	// set key layout file to eventconverter
	//	std::cout << "key layout file index = " << index << std::endl << std::flush;
	eventConverter->setKeytopType(keyLayoutFileReader->getKeyLayoutFile(index));
#else // !USE_KEYLAYOUTFILE
	ABORT();
#endif //  !USE_KEYLAYOUTFILE
	break;
  }

  // mode
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "MODE : " +
							   QString::number(settings->getPublicModeVersion()));

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
#else // !defined(Q_OS_WIN)
	ABORT(); // available for windows only
#endif // !defined(Q_OS_WIN)
	break;
  default: // key layout file
#ifdef USE_KEYLAYOUTFILE
	int index = keyboardType - KEYBOARD_TYPE_NATIVE - 1;
	// set key layout file to software keyboard
	//	  std::cout << "key layout file index = " << index << std::endl << std::flush;
	KeyLayoutFile *keyLayoutFile = keyLayoutFileReader->getKeyLayoutFile(index);
	softwareKeyboard->setKeytopType(keyLayoutFile);
	settings->setKeyboardTypeName(keyLayoutFile->getName());
#else // !USE_KEYLAYOUTFILE
	ABORT();
#endif // !USE_KEYLAYOUTFILE
	break;
  }
  softwareKeyboard->setVisible(false);

#if defined(QTB_DEV_TOUCHPANEL)
  // set touchpanel interface
  desktopPanel->setSoftwareButtonRect(touchpanelInterfaceType[settings->getTouchpanelInterfaceType()].softwareButtonRect);
  desktopPanel->setSoftwareKeyboardRect(touchpanelInterfaceType[settings->getTouchpanelInterfaceType()].softwareKeyboardRect);
#endif // defined(QTB_DEV_TOUCHPANEL)
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // clear buffer for control
  desktopFrame->getKeyBuffer()->clear();
  desktopFrame->getMouseBuffer()->clear();

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

  // try to connect flag
  isExecutingToConnect = true;
  updateConnected();

#if defined(QTB_DEV_TOUCHPANEL)
  // save settings
  settings->writeSettings();
#endif // defined(QTB_DEV_TOUCHPANEL)
}

// reconnect to server
void QtBrynhildr::reconnectToServer()
{
  if (!settings->getConnected()){
	return;
  }

  // disable connect to server menu
  connectToServer_Action->setEnabled(false);
  // disabled disconnect to server
  disconnectToServer_Action->setEnabled(true);

  // clear buffer for control
  keyBuffer->clear();
  mouseBuffer->clear();

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

#if 0 // disable now
  // desktop clear
  onDesktopClear();
#endif // disable now
}

// disconnect to server
void QtBrynhildr::disconnectToServer()
{
  // check control key state
  if (getOnControlKey()){
	// release control key
	sendKey_CTRL_Toggle();
  }

#if QTB_RECORDER
  // stop record and replay
  if (settings->getOnReplayingControl()){
	recorder->stopReplaying();
  }
  if (settings->getOnRecordingControl()){
	recorder->stopRecording(settings->getRecordingControlFileName());
  }
#endif // QTB_RECORDER

  // disable connect to server menu
  connectToServer_Action->setEnabled(false);
  // disabled disconnect to server
  disconnectToServer_Action->setEnabled(false);

  // exit all threads
  controlThread->exitThread();
  graphicsThread->exitThread();
  soundThread->exitThread();

  controlThread->wait();
  graphicsThread->wait();
  soundThread->wait();

  // disconnected
  disconnected();

  // disconnect
  settings->setConnected(false);

  // clear desktop
  clearDesktop();
}

// finished thread
void QtBrynhildr::finishedNetThread()
{
  //  std::cout << "finishedNetThread()" << std::endl << std::flush;
  // disconnect
  settings->setConnected(false);

  // refresh window
  refreshWindow();
}

// exit process for Application exit
void QtBrynhildr::exit()
{
  // execute once
  static bool doneExit = false;
  if (doneExit) return;

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

  // execute once
  doneExit = true;

  // disconnected
  disconnectToServer();

  // normal screen
  if (QTB_DESKTOP_FULL_SCREEN){
	if (fullScreenMode){
	  // to Normal Screen
	  exitFullScreen();
	}
  }

#if defined(QTB_DEV_DESKTOP)
  // desktop window widget
  if (settings->getOnViewerMode()){
	desktopWindowWidget->leaveAreaMode();
  }
#endif // defined(QTB_DEV_DESKTOP)

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
#if QT_VERSION < 0x060000
  if (totalFrameCounter != 0){
	//	std::cout << "totalFrameCounter = " << totalFrameCounter << std::endl << std::flush;
	uint diffSeconds = shutdownTime.toTime_t() - bootTime.toTime_t();
	if (diffSeconds != 0){
	  float averageFrameRate = (float)totalFrameCounter/diffSeconds;
	  //	  std::cout << "FPS of last session: " << averageFrameRate << std::endl << std::flush;
	  QString averageFrameRateString;
	  averageFrameRateString.setNum(averageFrameRate);
	  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, tr("FPS of last session : ") + averageFrameRateString + " FPS");
	}
  }
#else // QT_VERSION >= 0x060000
  if (totalFrameCounter != 0){
	//	std::cout << "totalFrameCounter = " << totalFrameCounter << std::endl << std::flush;
	uint diffSeconds = (uint)(shutdownTime.toSecsSinceEpoch() - bootTime.toSecsSinceEpoch());
	if (diffSeconds != 0){
	  float averageFrameRate = (float)totalFrameCounter/diffSeconds;
	  //	  std::cout << "FPS of last session: " << averageFrameRate << std::endl << std::flush;
	  QString averageFrameRateString;
	  averageFrameRateString.setNum(averageFrameRate);
	  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, tr("FPS of last session : ") + averageFrameRateString + " FPS");
	}
  }
#endif // QT_VERSION >= 0x060000

  // quit
  qApp->exit(0);
}

// send clipboard
void QtBrynhildr::sendClipboard()
{
  // check support
  if (!settings->getOnTransferClipboardSupport()){
	// Nothing to do
	return;
  }

  // check connected
  if (!settings->getConnected() ||
	  !settings->getOnControl()){
	// Nothing to do
	return;
  }

  // check clipboard
  if (clipboard == nullptr){
	// Nothing to do
	return;
  }

  // get mime data
  const QMimeData *mimeData = clipboard->mimeData();

  if (mimeData->hasImage()){
	//qDebug() << "clipboard has a image!";
	//qDebug() << "MIME : " << mimeData->text();
  }
  else if (mimeData->hasColor()){
	//qDebug() << "clipboard has a color!";
	//qDebug() << "MIME : " << mimeData->text();
  }
  else if (mimeData->hasUrls()){
	//qDebug() << "clipboard has a urls!";
	//qDebug() << "MIME : " << mimeData->text();
  }
  else if (mimeData->hasText()){
	const QString text = clipboard->text();
	//qDebug() << "clipboard has a text! : " << text;
	//qDebug() << "MIME : " << mimeData->text();

	// send clipboard flag ON
	if (!settings->getOnSendClipboard()){
	  settings->setSendClipboardString(text);
	  settings->setOnSendClipboard(true);
	}
  }
  else if (mimeData->hasHtml()){
	const QString text = clipboard->text();
	//qDebug() << "clipboard has a html! : " << text;
	//qDebug() << "MIME : " << mimeData->text();

	// send clipboard flag ON
	if (!settings->getOnSendClipboard()){
	  settings->setSendClipboardString(text);
	  settings->setOnSendClipboard(true);
	}
  }
  else {
	std::cout << "clipboard has unknown data!";
  }
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
  settings->setSendFileCount((int)fileNames.count());
}

// cancel file transferring
void QtBrynhildr::cancelFileTransferring()
{
  // disable cancel file transferring menu
  cancelFileTransferring_Action->setEnabled(false);

  // cancel file transferring
  controlThread->exitThread();
}

#if QTB_PREFERENCE
// preferences
void QtBrynhildr::preferences()
{
  //std::cout << "enter preferences()" << std::endl << std::flush;

  QPoint pos = getInitialDialogPos(preferenceDialog);
  preferenceDialog->move(pos);
  preferenceDialog->show();

  //std::cout << "leave preferences()" << std::endl << std::flush;
}
#endif // QTB_PREFERENCE

// initialize settings
void QtBrynhildr::initializeSettings()
{
  //std::cout << "enter initializeSettings()" << std::endl << std::flush;

  int ret = QMessageBox::question(this,
								  tr("Confirm"),
								  tr("Do you initialize settings ?"),
								  QMessageBox::Ok | QMessageBox::Cancel,
								  QMessageBox::Cancel);
  if (ret == QMessageBox::Cancel){
	// cancel
	return;
  }

  // remove settings
  if (settings != nullptr)
	delete settings;

  // create setting
#if QTB_CRYPTOGRAM
  settinegs = new Settings(iniFileName, cipher);
#else // !QTB_CRYPTGRAM
  settings = new Settings(iniFileName);
#endif // !QTB_CRYPTGRAM

  // save settings
  settings->writeSettings();

  // close connect to server dialog
  if (onPopUpConnectToServer){
	connectToServerDialog->hide();
  }

  //std::cout << "leave initializeSettings()" << std::endl << std::flush;
}

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
  selectFrameRateMinimum_Action->setChecked(false);
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
  bool flag = !settings->getOnControl();

  if (flag){ // Off -> On
	// onGraphics On
	if (!settings->getOnGraphics()){
	  toggleOnGraphics();
	}
  }
  else { // On -> Off
	// clear device buffer
	keyBuffer->clear();
	mouseBuffer->clear();
  }

  settings->setOnControl(flag);
  onControl = flag;
  onControl_Action->setChecked(flag);

  // refresh menu
  refreshOtherMenu();
}

// toggle onGraphics
void QtBrynhildr::toggleOnGraphics()
{
  bool flag = !settings->getOnGraphics();

  if (!flag){ // On -> Off
	// onControl Off
	if (settings->getOnControlOffWithGraphicsOff()){
	  settings->setOnControl(false);
	  onControl = false;
	  onControl_Action->setChecked(false);
	}
  }

  settings->setOnGraphics(flag);
  onGraphics = flag;
  onGraphics_Action->setChecked(flag);

  // refresh menu
  refreshOtherMenu();
}

// toggle onSound
void QtBrynhildr::toggleOnSound()
{
  bool flag = !settings->getOnSound();

  if (flag){ // Off -> On
  }
  else { // On -> Off
  }

  settings->setOnSound(flag);
  onSound = flag;
  onSound_Action->setChecked(flag);

  // refresh menu
  refreshOtherMenu();
}

#if QTB_TOOLBAR
// for tool bar
void QtBrynhildr::topLevelChanged(bool topLevel)
{
  Q_UNUSED(topLevel);

  //  std::cout << "topLevel:" << topLevel << std::endl << std::flush;
  // refresh desktop
  if (settings->getConnected()){
	desktopFrame->resizeWindow();
  }
}
#endif // QTB_TOOLBAR

// setup window title
void QtBrynhildr::setupWindowTitle()
{
#if !QTB_PORTABLE_VERSION
#if defined(QTB_DEV_TOUCHPANEL)
  setWindowTitle(tr(QTB_APPLICATION)+"  - " + settings->getPublicModeAliasString() +" - [TOUCHPANEL]");
#else // !defined(QTB_DEV_TOUCHPANEL)
  setWindowTitle(tr(QTB_APPLICATION)+"  - " + settings->getPublicModeAliasString() +" -");
#endif // !defined(QTB_DEV_TOUCHPANEL)
#else // QTB_PORTABLE_VERSION
  setWindowTitle(tr(QTB_APPLICATION)+" Portable  - " + settings->getPublicModeAliasString() +" -");
#endif // QTB_PORTABLE_VERSION
}

// refresh public mode
void QtBrynhildr::refreshPublicMode()
{
  selectPublicModeVersion5_Action->setChecked(settings->getPublicModeVersion() == PUBLICMODE_VERSION5);
  selectPublicModeVersion6_Action->setChecked(settings->getPublicModeVersion() == PUBLICMODE_VERSION6);
  selectPublicModeVersion7_Action->setChecked(settings->getPublicModeVersion() == PUBLICMODE_VERSION7);

  // menus, drag and drop
  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// send clipboard
	sendClipboard_Action->setEnabled(true);

	// send file
	sendFile_Action->setEnabled(true);

	// cancel file transferring
	cancelFileTransferring_Action->setEnabled(false);

	// drag and drop
#if defined(QTB_DEV_TOUCHPANEL)
#if 0 // for TEST
	desktopFrameObject->setAcceptDrops(true);
#endif // 0 // for TEST
#else // !defined(QTB_DEV_TOUCHPANEL)
	desktopWindowWidget->setAcceptDrops(true);
#endif // !defined(QTB_DEV_TOUCHPANEL)
  }
  else {
	// send clipboard
	sendClipboard_Action->setEnabled(false);

	// send file
	sendFile_Action->setEnabled(false);

	// cancel file transferring
	cancelFileTransferring_Action->setEnabled(false);

	// drag and drop
#if defined(QTB_DEV_TOUCHPANEL)
#if 0 // for TEST
	desktopFrameObject->setAcceptDrops(false);
#endif // 0 // for TEST
#else // !defined(QTB_DEV_TOUCHPANEL)
	desktopWindowWidget->setAcceptDrops(false);
#endif // !defined(QTB_DEV_TOUCHPANEL)
  }

  // set window title
  setupWindowTitle();

  // mouse cursor
  if (settings->getPublicModeVersion() != PUBLICMODE_VERSION7){
	// change mouse cursor to cross cursor
	changeMouseCursor(Qt::CrossCursor);
	// change mouse cursor on menus to cross cursor
	menuBar()->setCursor(Qt::CrossCursor);
  }
  else {
	// change mouse cursor to arrow cursor
	changeMouseCursor(Qt::ArrowCursor);
	// change mouse cursor on menus to arrow cursor
	menuBar()->setCursor(Qt::ArrowCursor);
  }
#if QTB_BENCHMARK
  if (onBenchmarkMenu){
	// refresh benchmark menu
	refreshBenchmarkMenu();
  }
#endif // QTB_BENCHMARK
}

// select public mode version
void QtBrynhildr::selectPublicModeVersion5()
{
  settings->setPublicModeVersion(PUBLICMODE_VERSION5);

  refreshPublicMode();
}
void QtBrynhildr::selectPublicModeVersion6()
{
  settings->setPublicModeVersion(PUBLICMODE_VERSION6);

  refreshPublicMode();
}
void QtBrynhildr::selectPublicModeVersion7()
{
  settings->setPublicModeVersion(PUBLICMODE_VERSION7);

  refreshPublicMode();
}

// refresh other menu
void QtBrynhildr::refreshOtherMenu()
{
  bool flag;

  // enable/disable menu for control
  flag = settings->getOnControl();
  sendKeySubMenu->setEnabled(flag);
#if defined(QTB_DEV_DESKTOP)
  selectMonitorNoSubMenu->setEnabled(flag);
#endif // defined(QTB_DEV_DESKTOP)
  selectPublicModeVersionSubMenu->setEnabled(flag);
#if QTB_RECORDER
  recordAndReplaySubMenu->setEnabled(flag);
#endif // QTB_RECORDER

  // enable/disable menu for graphics
  flag = settings->getOnGraphics();
  videoMenu->setEnabled(flag);
#if QTB_DESKTOP_COMPRESS_MODE
  desktopCompressModeSubMenu->setEnabled(flag);
#endif // QTB_DESKTOP_COMPRESS_MODE
  if (QTB_SCROLL_MODE){
	onScrollMode_Action->setEnabled(flag);
  }
  if (QTB_VIEWER_MODE){
	onViewerMode_Action->setEnabled(flag);
	onFulFulMode_Action->setEnabled(flag);
  }

#if 0 //QTB_GRAY_SCALE_MODE
  // monochrome mode
  onMonochromeMode_Action->setEnabled(flag);
#endif // QTB_GRAY_SCALE_MODE

#if 0 //QTB_MOUSE_TRACKING_FOCUS_MODE
  // mouse tracking mode
  onMouseTrackingMode_Action->setEnabled(flag);
#endif // QTB_MOUSE_TRACKING_FOCUS_MODE

  // enable/disable menu for sound
  flag = settings->getOnSound();
  soundMenu->setEnabled(flag);
}


#if QTB_BENCHMARK
// refresh benchmark menu check mark
void QtBrynhildr::refreshBenchmarkMenuCheck()
{
  selectBenchmarkPhase0_Action->setChecked(false);
  selectBenchmarkPhase1_Action->setChecked(false);
  selectBenchmarkPhase2_Action->setChecked(false);
  selectBenchmarkPhase3_Action->setChecked(false);
  selectBenchmarkPhase4_Action->setChecked(false);
  switch(initialBenchmarkPhaseCounter){
  case 0:
	selectBenchmarkPhase0_Action->setChecked(true);
	break;
  case 1:
	selectBenchmarkPhase1_Action->setChecked(true);
	break;
  case 2:
	selectBenchmarkPhase2_Action->setChecked(true);
	break;
  case 3:
	selectBenchmarkPhase3_Action->setChecked(true);
	break;
  case 4:
	selectBenchmarkPhase4_Action->setChecked(true);
	break;
  default:
	// do nothing
	break;
  }
}
// refresh benchmark menu
void QtBrynhildr::refreshBenchmarkMenu()
{
  // benchmark menus
  if (settings->getPublicModeVersion() <= PUBLICMODE_VERSION6){ // MODE5/6 (MJPEG)
	selectBenchmarkPhase0_Action->setText(tr("Data Communication"));
	selectBenchmarkPhase0_Action->setEnabled(true);
	selectBenchmarkPhase0_Action->setCheckable(true);
	selectBenchmarkPhase0_Action->setVisible(true);

	selectBenchmarkPhase1_Action->setEnabled(false);
	selectBenchmarkPhase1_Action->setCheckable(false);
	selectBenchmarkPhase1_Action->setVisible(false);

	selectBenchmarkPhase2_Action->setText(tr("Create Image Object"));
	selectBenchmarkPhase2_Action->setEnabled(true);
	selectBenchmarkPhase2_Action->setCheckable(true);
	selectBenchmarkPhase2_Action->setVisible(true);

	selectBenchmarkPhase3_Action->setText(tr("Rescale Image"));
	selectBenchmarkPhase3_Action->setEnabled(true);
	selectBenchmarkPhase3_Action->setCheckable(true);
	selectBenchmarkPhase3_Action->setVisible(true);

	selectBenchmarkPhase4_Action->setText(tr("Draw JPEG Image"));
	selectBenchmarkPhase4_Action->setEnabled(true);
	selectBenchmarkPhase4_Action->setCheckable(true);
	selectBenchmarkPhase4_Action->setVisible(true);

	initialBenchmarkPhaseCounter = 4;
  }
  else if (settings->getPublicModeVersion() == PUBLICMODE_VERSION7){ // MODE7 (VP8)
	selectBenchmarkPhase0_Action->setEnabled(true);
	selectBenchmarkPhase0_Action->setText(tr("Data Communication"));
	selectBenchmarkPhase0_Action->setCheckable(true);
	selectBenchmarkPhase0_Action->setVisible(true);

	selectBenchmarkPhase1_Action->setText(tr("Decode VP8"));
	selectBenchmarkPhase1_Action->setEnabled(true);
	selectBenchmarkPhase1_Action->setCheckable(true);
	selectBenchmarkPhase1_Action->setVisible(true);

	selectBenchmarkPhase2_Action->setText(tr("Create Image Object"));
	selectBenchmarkPhase2_Action->setEnabled(true);
	selectBenchmarkPhase2_Action->setCheckable(true);
	selectBenchmarkPhase2_Action->setVisible(true);

	selectBenchmarkPhase3_Action->setText(tr("Rescale Image"));
	selectBenchmarkPhase3_Action->setEnabled(true);
	selectBenchmarkPhase3_Action->setCheckable(true);
	selectBenchmarkPhase3_Action->setVisible(true);

	selectBenchmarkPhase4_Action->setText(tr("Draw RGB32 Image"));
	selectBenchmarkPhase4_Action->setEnabled(true);
	selectBenchmarkPhase4_Action->setCheckable(true);
	selectBenchmarkPhase4_Action->setVisible(true);

	initialBenchmarkPhaseCounter = 4;
  }
  else {
	initialBenchmarkPhaseCounter = 5;
  }

  refreshBenchmarkMenuCheck();
}
#endif // QTB_BENCHMARK

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

#if 0 // disable now
// send key for CTRL + ALT + DEL
void QtBrynhildr::sendKey_CTRL_ALT_DEL()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
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
#endif // disable now

// send key for ALT + F4
void QtBrynhildr::sendKey_ALT_F4()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
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
	// press
	keyBuffer->put(VK_MENU,	KEYCODE_FLG_KEYDOWN); // ALT key press
	keyBuffer->put(VK_SNAPSHOT,	KEYCODE_FLG_KEYDOWN); // PrintScreen key press

	// release
	keyBuffer->put(VK_SNAPSHOT,	KEYCODE_FLG_KEYUP); // PrintScreen key release
	keyBuffer->put(VK_MENU,	KEYCODE_FLG_KEYUP); // ALT key press
  }
}

// send key for CTRL ON/OFF
void QtBrynhildr::sendKey_CTRL_Toggle()
{
  if (settings->getConnected() &&
	  settings->getOnControl()){
	if (!getOnControlKey()){
	  // press
	  keyBuffer->put(VK_CONTROL, KEYCODE_FLG_KEYDOWN); // CTRL key press
	}
	else {
	  // release
	  keyBuffer->put(VK_CONTROL, KEYCODE_FLG_KEYUP); // CTRL key release
	}
	setOnControlKey(!getOnControlKey());
  }
}

// toggle show menu bar
void QtBrynhildr::toggleShowMenuBar()
{
  if (settings->getOnShowMenuBar()){
	onShowMenuBar = false;
  }
  else {
	onShowMenuBar = true;
  }
  settings->setOnShowMenuBar(onShowMenuBar);
  menuBar()->setVisible(onShowMenuBar);
  refreshWindow();
}

#if QTB_TOOLBAR
// toggle show tool bar
void QtBrynhildr::toggleShowToolBar()
{
  if (settings->getOnShowToolBar()){
	onShowToolBar = false;
  }
  else {
	onShowToolBar = true;
  }
  settings->setOnShowToolBar(onShowToolBar);
  toolBar->setVisible(onShowToolBar);
  refreshWindow();
}
#endif // QTB_TOOLBAR

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
#if defined(QTB_DEV_DESKTOP)
  if (!QTB_DESKTOP_FULL_SCREEN)
	return;
#endif // defined(QTB_DEV_DESKTOP)

#if 0 // for DEBUG
  QSize size = desktopPanel->getSize();
  std::cout << "fullScreen() : width  = " << size.width() << std::endl << std::flush;
  std::cout << "fullScreen() : height = " << size.height() << std::endl << std::flush;
#endif // for DEBUG

  static qreal originalScalingFactor = 1.0;

  fullScreenMode = !fullScreenMode;
  if (fullScreenMode){
	if (settings->getOnHideMenuAndStatusBarAtFullScreen()){
	  // hide menu and status bar
	  settings->setOnShowMenuBar(false);
	  settings->setOnShowStatusBar(false);
	  menuBar()->setVisible(false);
	  statusBar()->setVisible(false);
	}
#if !defined(QTB_DEV_TOUCHPANEL)
	desktopWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	desktopWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	desktopWindow->setPalette(fullScreenBackgroundPalette); // change QPalette::Window to black
#endif // !defined(QTB_DEV_TOUCHPANEL)
	desktopFrame->setOnFullScreen(true);
	showFullScreen();
#if 0 // for TEST
	std::cout << "size(width, height) = ("
			  << size().width() << "," << size().height() << ")" << std::endl << std::flush;
	std::cout << "screen size(width, height) = ("
			  << settings->getCurrentScreenWidth() << ","
			  << settings->getCurrentScreenHeight() << ")" << std::endl << std::flush;
#endif // 0 // for TEST
#if 0 // for TEST
	originalScalingFactor = settings->getDesktopScalingFactor();
	setDesktopScalingFactor(size());
#else // 0 // for TEST
	originalScalingFactor = settings->getDesktopScalingFactor();
	QSize screenSize = settings->getCurrentScreenSize();
	setDesktopScalingFactor(screenSize);
#endif // 0 // for TEST
  }
  else {
	if (settings->getOnHideMenuAndStatusBarAtFullScreen()){
	  // restore menu and status bar
	  settings->setOnShowMenuBar(onShowMenuBar);
	  settings->setOnShowStatusBar(onShowStatusBar);
	  menuBar()->setVisible(settings->getOnShowMenuBar());
	  statusBar()->setVisible(settings->getOnShowStatusBar());
	}
#if !defined(QTB_DEV_TOUCHPANEL)
	//desktopWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	//desktopWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	desktopWindow->setPalette(backgroundPalette); // restore original QPalette::Window
#endif // !defined(QTB_DEV_TOUCHPANEL)
	desktopFrame->setOnFullScreen(false);
	showNormal();
	//std::cout << "size(width, height) = ("
	//	 << size().width() << "," << size().height() << ")" << std::endl << std::flush;
	//setDesktopScalingFactor(size());
	settings->setDesktopScalingFactor(originalScalingFactor);
  }
  // set checked flag
#if defined(QTB_DEV_DESKTOP)
  fullScreen_Action->setChecked(fullScreenMode);
#endif // defined(QTB_DEV_DESKTOP)

#if QTB_DESKTOP_COMPRESS_MODE
  // enable/disable desktop compress mode menu
  desktopCompressModeSubMenu->setEnabled(!fullScreenMode);
#endif // QTB_DESKTOP_COMPRESS_MODE
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

#if !defined(QTB_DEV_TOUCHPANEL)
	if (checked){
	  desktopWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	  desktopWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
	else {
	  desktopWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	  desktopWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}
#endif // !defined(QTB_DEV_TOUCHPANEL)

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
#if !defined(QTB_DEV_TOUCHPANEL)
	static Qt::ScrollBarPolicy hpolicy = desktopWindow->horizontalScrollBarPolicy();
	static Qt::ScrollBarPolicy vpolicy = desktopWindow->horizontalScrollBarPolicy();
#endif // !defined(QTB_DEV_TOUCHPANEL)
	if (checked){
	  // set maximum size (current size)
	  setMaximumSize(size());
	  // set minimum size (current size)
	  setMinimumSize(size());
	  // diable scroll bar
#if !defined(QTB_DEV_TOUCHPANEL)
	  if (settings->getOnScrollMode()){
		desktopWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		desktopWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	  }
	  else {
		desktopWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		desktopWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	  }
#endif // !defined(QTB_DEV_TOUCHPANEL)
	  // disable maximum button
#if defined(Q_OS_WIN)
	  bool visible = isVisible();
	  Qt::WindowFlags flags = windowFlags();
	  flags &= ~Qt::WindowMaximizeButtonHint;
	  setWindowFlags(flags);
	  if (visible)
		setVisible(true);
#endif // defined(Q_OS_WIN)
	}
	else {
	  setMaximumSize(orgMaximumSize);
	  setMinimumSize(orgMinimumSize);
	  // enable scroll bar
#if !defined(QTB_DEV_TOUCHPANEL)
	  desktopWindow->setHorizontalScrollBarPolicy(hpolicy);
	  desktopWindow->setVerticalScrollBarPolicy(vpolicy);
#endif // !defined(QTB_DEV_TOUCHPANEL)
	  // restore window flags
#if defined(Q_OS_WIN)
	  bool visible = isVisible();
	  Qt::WindowFlags flags = windowFlags();
	  flags |= Qt::WindowMaximizeButtonHint;
	  setWindowFlags(flags);
	  if (visible)
		setVisible(true);
#endif // defined(Q_OS_WIN)
	  // update window
	  refreshWindow();
	}
  }
}

// monochrome mode
void QtBrynhildr::toggleOnMonochromeMode()
{
  settings->setOnMonochromeMode(!settings->getOnMonochromeMode());
}

// mouse tracking mode
void QtBrynhildr::toggleOnMouseTrackingMode()
{
  settings->setOnMouseTrackingMode(!settings->getOnMouseTrackingMode());
}

// desktop scaling
void QtBrynhildr::desktopScaling()
{
  // display desktop scaling dialog
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog->setSliderPositionFromSetting();
	QPoint pos = getInitialDialogPos(desktopScalingDialog);
	desktopScalingDialog->move(pos);
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
	QPoint pos = getInitialDialogPos(logViewDialog);
	logViewDialog->move(pos);
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
  QSize windowSize = this->size();
  QSize size = softwareKeyboard->resetSize();
  windowSize.setHeight(windowSize.height() - getHeightOfMenuBar() - getHeightOfStatusBar());
#if QTB_TOOLBAR
  width -= getWidthOfToolBar();
  height -= getHeightOfToolBar();
#endif // QTB_TOOLBAR
#if !defined(QTB_DEV_TOUCHPANEL)
  // correct
  windowSize.setWidth(windowSize.width() - widthMargin);
  windowSize.setHeight(windowSize.height() - heightMargin);
#endif // !defined(QTB_DEV_TOUCHPANEL)

  // calc size
  int width = windowSize.width() * 0.98;
  int height = size.height() * ((double)width / size.width());

  // check screen size
  if (width > settings->getCurrentScreenWidth()){
	height = height * (double)settings->getCurrentScreenWidth()/width;
	width = settings->getCurrentScreenWidth();
  }
  if (height > settings->getCurrentScreenHeight()){
	width = width * (double)settings->getCurrentScreenHeight()/height;
	height = settings->getCurrentScreenHeight();
  }

  // calc position
  int x = (windowSize.width() - width) * 0.5;
  int y = (windowSize.height() - height) * 0.8;

  //  std::cout << "width = " << width << std::endl;
  //  std::cout << "height = " << height << std::endl << std::flush;
  //  std::cout << "x = " << x << std::endl;
  //  std::cout << "y = " << y << std::endl << std::flush;

  return QRect(x, y, width, height);
}

// calulate software button layout
QRect QtBrynhildr::calculateSoftwareButtonLayout()
{
  // calc geometry
  QSize windowSize = this->size();
  QSize size = softwareButton->resetSize();
  windowSize.setHeight(windowSize.height() - getHeightOfMenuBar() - getHeightOfStatusBar());
#if QTB_TOOLBAR
  width -= getWidthOfToolBar();
  height -= getHeightOfToolBar();
#endif // QTB_TOOLBAR
#if !defined(QTB_DEV_TOUCHPANEL)
  // correct
  windowSize.setWidth(windowSize.width() - widthMargin);
  windowSize.setHeight(windowSize.height() - heightMargin);
#endif // !defined(QTB_DEV_TOUCHPANEL)

  // calc size
  int width = windowSize.width() * 0.95;
  int height = size.height() * ((double)width / size.width());

  // check screen size
  if (width > settings->getCurrentScreenWidth()){
	height = height * (double)settings->getCurrentScreenWidth()/width;
	width = settings->getCurrentScreenWidth();
  }
  if (height > settings->getCurrentScreenHeight()){
	width = width * (double)settings->getCurrentScreenHeight()/height;
	height = settings->getCurrentScreenHeight();
  }

  // calc position
  int x = (windowSize.width() - width) * 0.5;
  int y = (windowSize.height() - height) * 0.8;

  //  std::cout << "width = " << width << std::endl;
  //  std::cout << "height = " << height << std::endl << std::flush;
  //  std::cout << "x = " << x << std::endl;
  //  std::cout << "y = " << y << std::endl << std::flush;

  return QRect(x, y, width, height);
}

// toggle show software keyboard
void QtBrynhildr::toggleShowSoftwareKeyboard()
{
  if (settings->getOnShowSoftwareKeyboard()){
	settings->setOnShowSoftwareKeyboard(false);
	softwareKeyboard->setVisible(false);
	showSoftwareKeyboard_Action->setChecked(false);
  }
  else {
	settings->setOnShowSoftwareKeyboard(true);
	QRect rect = calculateSoftwareKeyboardLayout();
	softwareKeyboard->setGeometry(rect);
	softwareKeyboard->setVisible(true);
	showSoftwareKeyboard_Action->setChecked(true);
  }
}

// toggle show software button
void QtBrynhildr::toggleShowSoftwareButton()
{
  if (settings->getOnShowSoftwareButton()){
	settings->setOnShowSoftwareButton(false);
	softwareButton->setVisible(false);
	showSoftwareButton_Action->setChecked(false);
  }
  else {
	settings->setOnShowSoftwareButton(true);
	QRect rect = calculateSoftwareButtonLayout();
	softwareButton->setGeometry(rect);
	softwareButton->setVisible(true);
	showSoftwareButton_Action->setChecked(true);
  }
}

// setup software keyboard
void QtBrynhildr::setupSoftwareKeyboard()
{
  settings->setOnShowSoftwareKeyboard(true);
  QRect rect = calculateSoftwareKeyboardLayout();
  rect.moveTop(desktopFrame->getSize().height()); // outside desktop
  softwareKeyboard->setGeometry(rect);
  softwareKeyboard->setVisible(true);
}

// setup software button
void QtBrynhildr::setupSoftwareButton()
{
  settings->setOnShowSoftwareButton(true);
  QRect rect = calculateSoftwareButtonLayout();
  rect.moveTop(desktopFrame->getSize().height()); // outside desktop
  softwareButton->setGeometry(rect);
  softwareButton->setVisible(true);
}

// toggle software keyboard
void QtBrynhildr::toggleSoftwareKeyboard()
{
  toggleShowSoftwareKeyboard();
}
// toggle software button
void QtBrynhildr::toggleSoftwareButton()
{
  toggleShowSoftwareButton();
}

#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

// select frame rate
void QtBrynhildr::selectFrameRateMinimum()
{
  settings->setFrameRate(FRAMERATE_MINIMUM);
  clearSelectFrameRateCheck();
  selectFrameRateMinimum_Action->setChecked(true);
}
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

// toggle viewer mode
void QtBrynhildr::toggleOnViewerMode()
{
  if (!QTB_VIEWER_MODE)
	return;

  if (settings->getOnViewerMode()){
	settings->setOnViewerMode(false);

	// enable menu items
#if defined(QTB_DEV_DESKTOP)
	onControl_Action->setEnabled(true);
	onGraphics_Action->setEnabled(true);
	desktopScalingDialog_Action->setEnabled(true);
#endif // defined(QTB_DEV_DESKTOP)
	// disable sub menus
	selectPublicModeVersionSubMenu->setEnabled(true);
	sendKeySubMenu->setEnabled(true);
#if defined(QTB_DEV_DESKTOP)
	selectMonitorNoSubMenu->setEnabled(true);
	recordAndReplaySubMenu->setEnabled(true);
	desktopCompressModeSubMenu->setEnabled(true);
#endif // defined(QTB_DEV_DESKTOP)
  }
  else {
	settings->setOnViewerMode(true);

	// disable menu items
#if defined(QTB_DEV_DESKTOP)
	onControl_Action->setEnabled(false);
	onGraphics_Action->setEnabled(false);
	desktopScalingDialog_Action->setEnabled(false);
#endif // defined(QTB_DEV_DESKTOP)
	// disable sub menus
	selectPublicModeVersionSubMenu->setEnabled(false);
	sendKeySubMenu->setEnabled(false);
#if defined(QTB_DEV_DESKTOP)
	selectMonitorNoSubMenu->setEnabled(false);
	recordAndReplaySubMenu->setEnabled(false);
	desktopCompressModeSubMenu->setEnabled(false);
#endif // defined(QTB_DEV_DESKTOP)
  }

  // set checked flag
  onViewerMode_Action->setChecked(settings->getOnViewerMode());

  updateStatusBar();
}

// toggle fulful mode
void QtBrynhildr::toggleOnFulFulMode()
{
  if (!QTB_VIEWER_MODE)
	return;

  if (settings->getOnFulFulMode()){
	settings->setOnFulFulMode(false);
  }
  else {
	settings->setOnFulFulMode(true);
  }

  // set checked flag
  onFulFulMode_Action->setChecked(settings->getOnFulFulMode());

  updateStatusBar();
}

#if defined(QTB_DEV_TOUCHPANEL)
// touchpanel operation type
void QtBrynhildr::touchpanelOperationTypeKeroRemote()
{
  settings->setTouchpanelOperationType(QTB_TOUCHPANELOPERATIONTYPE_KEROREMOTE);
  touchpanelOperationTypeKeroRemote_Action->setChecked(true);
  touchpanelOperationTypeQtBrynhildr_Action->setChecked(false);
}

void QtBrynhildr::touchpanelOperationTypeQtBrynhildr()
{
  settings->setTouchpanelOperationType(QTB_TOUCHPANELOPERATIONTYPE_QTBRYNHILDR);
  touchpanelOperationTypeKeroRemote_Action->setChecked(false);
  touchpanelOperationTypeQtBrynhildr_Action->setChecked(true);
}

// touchpanel interface type
void QtBrynhildr::touchpanelInterfaceTypeLeftRight()
{
  settings->setTouchpanelInterfaceType(QTB_TOUCHPANELINTERFACETYPE_LEFTRIGHT);
  touchpanelInterfaceTypeLeftRight_Action->setChecked(true);
  touchpanelInterfaceTypeTopBottom_Action->setChecked(false);
  touchpanelInterfaceTypeBottom_LeftCenter_Action->setChecked(false);
  touchpanelInterfaceTypeTop_LeftCenter_Action->setChecked(false);
}

void QtBrynhildr::touchpanelInterfaceTypeTopBottom()
{
  settings->setTouchpanelInterfaceType(QTB_TOUCHPANELINTERFACETYPE_TOPBOTTOM);
  touchpanelInterfaceTypeLeftRight_Action->setChecked(false);
  touchpanelInterfaceTypeTopBottom_Action->setChecked(true);
  touchpanelInterfaceTypeBottom_LeftCenter_Action->setChecked(false);
  touchpanelInterfaceTypeTop_LeftCenter_Action->setChecked(false);
}
void QtBrynhildr::touchpanelInterfaceTypeBottom_LeftCenter()
{
  settings->setTouchpanelInterfaceType(QTB_TOUCHPANELINTERFACETYPE_BOTTOM_LEFTCENTER);
  touchpanelInterfaceTypeLeftRight_Action->setChecked(false);
  touchpanelInterfaceTypeTopBottom_Action->setChecked(false);
  touchpanelInterfaceTypeBottom_LeftCenter_Action->setChecked(true);
  touchpanelInterfaceTypeTop_LeftCenter_Action->setChecked(false);
}
void QtBrynhildr::touchpanelInterfaceTypeTop_LeftCenter()
{
  settings->setTouchpanelInterfaceType(QTB_TOUCHPANELINTERFACETYPE_TOP_LEFTCENTER);
  touchpanelInterfaceTypeTopBottom_Action->setChecked(false);
  touchpanelInterfaceTypeBottom_LeftCenter_Action->setChecked(false);
  touchpanelInterfaceTypeTop_LeftCenter_Action->setChecked(true);
}

#endif // defined(QTB_DEV_TOUCHPANEL)

#if 0 // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
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

#if QTB_DESKTOP_COMPRESS_MODE
// desktop compress
void QtBrynhildr::desktopCompressMode0()
{
  settings->setDesktopCompressMode(1);

  desktopCompressMode0_Action->setChecked(true);
  desktopCompressMode2_Action->setChecked(false);
  desktopCompressMode4_Action->setChecked(false);
  desktopCompressMode8_Action->setChecked(false);

  if (!fullScreen_Action->isEnabled())
	fullScreen_Action->setEnabled(true);
}
void QtBrynhildr::desktopCompressMode2()
{
  settings->setDesktopCompressMode(2);

  desktopCompressMode0_Action->setChecked(false);
  desktopCompressMode2_Action->setChecked(true);
  desktopCompressMode4_Action->setChecked(false);
  desktopCompressMode8_Action->setChecked(false);

  if (fullScreen_Action->isEnabled())
	fullScreen_Action->setEnabled(false);
}
void QtBrynhildr::desktopCompressMode4()
{
  settings->setDesktopCompressMode(4);

  desktopCompressMode0_Action->setChecked(false);
  desktopCompressMode2_Action->setChecked(false);
  desktopCompressMode4_Action->setChecked(true);
  desktopCompressMode8_Action->setChecked(false);

  if (fullScreen_Action->isEnabled())
	fullScreen_Action->setEnabled(false);
}
void QtBrynhildr::desktopCompressMode8()
{
  settings->setDesktopCompressMode(8);

  desktopCompressMode0_Action->setChecked(false);
  desktopCompressMode2_Action->setChecked(false);
  desktopCompressMode4_Action->setChecked(false);
  desktopCompressMode8_Action->setChecked(true);

  if (fullScreen_Action->isEnabled())
	fullScreen_Action->setEnabled(false);
}
#endif // QTB_DESKTOP_COMPRESS_MODE

#if defined(QTB_DEV_TOUCHPANEL)
// decode type
void QtBrynhildr::decodeOptionTypeCPP()
{
  settings->setSIMDOperationTypeName("C++");

  decodeOptionTypeCPP_Action->setChecked(true);
  decodeOptionTypeNEON_Action->setChecked(false);
}
void QtBrynhildr::decodeOptionTypeNEON()
{
  settings->setSIMDOperationTypeName("NEON");

  decodeOptionTypeCPP_Action->setChecked(false);
  decodeOptionTypeNEON_Action->setChecked(true);
}
void QtBrynhildr::decodeOptionThread1()
{
  settings->setConvertThreadCount(1);

  decodeOptionThread1_Action->setChecked(true);
  decodeOptionThread2_Action->setChecked(false);
  decodeOptionThread4_Action->setChecked(false);
}
void QtBrynhildr::decodeOptionThread2()
{
  settings->setConvertThreadCount(2);

  decodeOptionThread1_Action->setChecked(false);
  decodeOptionThread2_Action->setChecked(true);
  decodeOptionThread4_Action->setChecked(false);
}
void QtBrynhildr::decodeOptionThread4()
{
  settings->setConvertThreadCount(4);

  decodeOptionThread1_Action->setChecked(false);
  decodeOptionThread2_Action->setChecked(false);
  decodeOptionThread4_Action->setChecked(true);
}
#endif // defined(QTB_DEV_TOUCHPANEL)

// disable drawing
void QtBrynhildr::disableDrawing()
{
  bool flag = graphicsThread->getOnDrawing();
  graphicsThread->setOnDrawing(!flag);
  disableDrawing_Action->setChecked(flag);
}

// disable maxfps
void QtBrynhildr::disableMaxfps()
{
  bool flag = controlThread->getOnMaxfps();
  controlThread->setOnMaxfps(!flag);
  disableMaxfps_Action->setChecked(flag);
}

#if QTB_BENCHMARK
// select phase for benchmark
void QtBrynhildr::selectBenchmarkPhase(int initialBenchmarkPhaseCounter)
{
  if (graphicsThread != nullptr){
	this->initialBenchmarkPhaseCounter = initialBenchmarkPhaseCounter;
	graphicsThread->setInitialBenchmarkPhaseCounter(initialBenchmarkPhaseCounter);
	refreshBenchmarkMenuCheck();
  }
}
void QtBrynhildr::selectBenchmarkPhase0()
{
  selectBenchmarkPhase(0);
}
void QtBrynhildr::selectBenchmarkPhase1()
{
  selectBenchmarkPhase(1);
}
void QtBrynhildr::selectBenchmarkPhase2()
{
  selectBenchmarkPhase(2);
}
void QtBrynhildr::selectBenchmarkPhase3()
{
  selectBenchmarkPhase(3);
}
void QtBrynhildr::selectBenchmarkPhase4()
{
  selectBenchmarkPhase(4);
}
#endif // QTB_BENCHMARK

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
#if !QTB_NET_WINSOCK1
  // WinSock 2
  wsaResult = WSAStartup(MAKEWORD(2,2), &wsaData);
#else // QTB_NET_WINSOCK1
  // WinSock 1
  wsaResult = WSAStartup(MAKEWORD(1,1), &wsaData);
#endif // QTB_NET_WINSOCK1
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

#if QTB_UPDATECHECK
// finished download
void QtBrynhildr::finishedDownload()
{
  // check update
  QByteArray byteArray = httpGetter->getPage();
  if (byteArray.size() == 0){
	std::cout << "finished downloading to file." << std::endl << std::flush;
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

  // check latest release
  int startIndex = releasePage.indexOf(QTB_STRING_FOR_TAGSEARCH);
  if (startIndex > 0) {
	startIndex += (int)qstrlen(QTB_STRING_FOR_TAGSEARCH);
	int lastIndex = releasePage.indexOf("\"", startIndex);
	//std::cout << "startIndex = " << startIndex << std::endl << std::flush;
	//std::cout << "lastIndex  = " << lastIndex << std::endl << std::flush;
	QStringRef tagRef(&releasePage, startIndex, lastIndex - startIndex);
	QString latestTag;
	latestTag.append(tagRef);
	//std::cout << "latest tag = v" << qPrintable(latestTag) << std::endl;

	startIndex = lastIndex + 2;
	lastIndex = releasePage.indexOf("<", startIndex);
	QStringRef verRef(&releasePage, startIndex, lastIndex - startIndex);
	QString ver;
	ver.append(verRef);
	//std::cout << " : ver = " << qPrintable(ver) << std::endl << std::flush;

	QString tag(option->getVersionString());
	if (tag != latestTag){
	  // Found new version
	  //std::cout << "Found new version" << std::endl << std::flush;
#if defined(QTB_DEV_DESKTOP)
	  int ret = QMessageBox::question(this,
									  tr("Confirm"),
									  tr("Found new release. Open release page?"),
									  QMessageBox::Ok | QMessageBox::Cancel,
									  QMessageBox::Ok);
	  if (ret == QMessageBox::Ok){
		QDesktopServices::openUrl(QUrl(QTB_URL_FOR_RELEASE));
	  }
#else // !defined(QTB_DEV_DESKTOP)
	  QMessageBox::information(this, tr("Information"),
							   tr("Found new release."));
#endif // !defined(QTB_DEV_DESKTOP)
	}
	else {
	  if (!onCheckUpdateInBackground){
		// Up-to-date
		//		std::cout << "Up-to-date!" << std::endl << std::flush;
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
	std::cout << "NOT Found tag!" << std::endl << std::flush;
  }

  // clear memory
  httpGetter->clear();
}
#endif // QTB_UPDATECHECK

void QtBrynhildr::timerExpired()
{
  //  std::cout << "timerExpired()!" << std::endl << std::flush;
#if QTB_TEST_DRAW_FRAME
  {
	static int previousDrawCounter = 0;
	if (previousDrawCounter == 0){
	  previousDrawCounter = drawCounter;
	}
	else {
	  int frameRate = drawCounter - previousDrawCounter;
	  previousDrawCounter = drawCounter;
	  std::cout << "frameRate: " << frameRate << std::endl << std::flush;
	}
  }
#endif // QTB_TEST_DRAW_FRAME

  // update current frame rate
#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  if (settings->getOnShowFrameRate() ||
	  settings->getOnShowSoftwareButton()){
#else // !QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  if (settings->getOnShowFrameRate()){
#endif // !QTB_SOFTWARE_KEYBOARD_AND_BUTTON
	// frame rate
	currentFrameRate = graphicsThread->getFrameRate();
	if (currentFrameRate < 1) currentFrameRate = 1;
	// data rate
	long controlDataRate = controlThread->getDataRate();
	long graphicsDataRate = graphicsThread->getDataRate();
	long soundDataRate = soundThread->getDataRate();
	// Mbps
	currentDataRate = ((double)(controlDataRate + graphicsDataRate + soundDataRate) * 8 / (1024*1024));
	updateStatusBar();
  }

#if 0 // for TEST
  // focus test
  if (hasFocus()){
	qDebug() << "has Focus!";
  }
  else {
	qDebug() << "has No Focus!";
  }
#endif // 0 // for TEST
}

} // end of namespace qtbrynhildr
