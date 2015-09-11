// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// qtbrynhildr.cpp

// Common Header
#include "common/common.h"

// System Header
#include <cstring>
#if 1 // copy for TEST
#include <cstdio>
#endif
//#include <iostream>

// Qt Header
#include <QByteArray>
#include <QCloseEvent>
#include <QDir>
#if QTB_RECORDER
#include <QFileDialog>
#endif // QTB_RECORDER
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

namespace qtbrynhildr {

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_OSX)
// socket for control
SOCKET sock_control = INVALID_SOCKET;
// socket for graphics
SOCKET sock_graphics = INVALID_SOCKET;
// socket for sound
SOCKET sock_sound = INVALID_SOCKET;
#endif // defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_OSX)

// counter for control
int counter_control = 0;
// counter for graphics
int counter_graphics = 0;

// date format
const QString dateFormat = QTB_LOG_DATE_FORMAT;

// constructor
QtBrynhildr::QtBrynhildr(int argc, char *argv[])
  :
  desktopScalingDialog(0),
  softwareKeyboard(0),
  softwareButton(0),
  softwareKeyboardDockWidget(0),
  softwareButtonDockWidget(0),
  frameCounter(0),
  currentFrameRate(0),
  option(0),
  iniFileName(0),
  settings(0),
#if QTB_RECORDER
  recorder(0),
#endif // QTB_RECORDER
#if QTB_CRYPTOGRAM
  cipher(0),
#endif // QTB_CRYPTGRAM
  logMessage(new LogMessage(this)),
  controlThread(0),
  graphicsThread(0),
  soundThread(0),
  fullScreenMode(false),
  heightOfMenuBar(0),
  heightOfStatusBar(0)
{
  // initialize platform
  if (!initPlatform()){
	// Failed to initialize platform
	logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "error: initPlatform()");
  }

  // open Log File
  if (!logMessage->openLogFile(QTB_LOG_FILENAME)){
	// Failed to open log file
#if 1 // for DEBUG
	QMessageBox::warning(this,
						 tr("warning"),
						 tr("Failed to open log file"),
						 QMessageBox::Ok,
						 QMessageBox::Ok);
#endif
  }

  // bootTime
  bootTime = QDateTime::currentDateTime();
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, tr("Bootup."));

#if QTB_CRYPTOGRAM
  // create cipher
#if 1 // simple description
  cipher = new Cipher(qPrintable(bootTime.toString()));
#else
  QByteArray bootTimeStr = bootTime.toString().toUtf8();
  cipher = new Cipher(bootTimeStr.constData());
#endif
#endif // QTB_CRYPTOGRAM

  // analyze command line options
  option = new Option(argc, argv);
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

  // set option to settings
  if (option->getServerName() != 0){
	settings->setServerName(option->getServerName());
  }
  if (option->getPortNo() > 0){
	settings->setPortNo(option->getPortNo());
  }
  if (option->getPassword() != 0){
	settings->setPassword(option->getPassword());
  }
  if (option->getDebug() != 0){
	if (strncmp(option->getDebug(), "on", sizeof("on")) == 0){
	  settings->setOutputLog(true);
	}
	else {
	  settings->setOutputLog(false);
	}
  }

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

  // version
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, "Version    : v" QTB_VERSION);
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

  //------------------------------------------------------------
  // create window
  //------------------------------------------------------------
#if 1 // for scroll area TEST
  // Scroll Area
  scrollArea = new QScrollArea;
  scrollArea->setWidgetResizable(true);
  //  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  //  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  // Main Window Widget
  mainWindow = new MainWindow(settings, this);
  // set Widget
  scrollArea->setWidget(mainWindow);
  setCentralWidget(scrollArea);
#else
  // Main Window Widget
  mainWindow = new MainWindow(settings, this);
  setCentralWidget(mainWindow);
#endif

  // create Main Window
  createActions();
  createMenus();
  createContextMenu();
  createToolBars();
  createStatusBar();
  // show or hide
  menuBar()->setVisible(settings->getOnShowMenuBar());
  statusBar()->setVisible(settings->getOnShowStatusBar());

  // set icon
  //  setWindowIcon(QIcon(QTB_ICON_FILENAME));

  // set window title
  setWindowTitle(tr(QTB_APPLICATION));

  // set window flags
  if (settings->getOnFrameLessWindow()){
	setWindowFlags(Qt::FramelessWindowHint);
  }

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

  // set up connect to server dialog
  connectToServerDialog = new ConnectToServerDialog(settings, this);
  connect(connectToServerDialog->buttonBox, SIGNAL(accepted()), SLOT(connectToServer()));

  // set up desktop scaling dialog
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog = new DesktopScalingDialog(settings, this);
  }

  // Software Button
  if (QTB_SOFTWARE_KEYBOARD_AND_BUTTON){
	softwareButton = new SoftwareButton(settings, mainWindow->getMouseBuffer(), this);
	softwareButton->setVisible(false);
	connect(softwareButton, SIGNAL(refreshMenu()), SLOT(refreshMenu()));
  }

  // for Event Converter TEST
  if (QTB_DEBUG_KEYBOARD || QTB_DEBUG_MOUSE){
	mainWindow->setEventConverter(new EventConverter_JP());
  }

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

  // control thread
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
		  SIGNAL(networkError()),
		  SLOT(onNetworkError()));

  connect(controlThread,
		  SIGNAL(exitApplication()),
		  SLOT(exitApplication()));

  // graphics thread
  connect(graphicsThread,
		  SIGNAL(desktopChanged(QImage)),
		  SLOT(onDesktopChanged(QImage)));
  connect(graphicsThread,
		  SIGNAL(desktopClear()),
		  SLOT(onDesktopClear()));

  // bootTime
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
}

// destructor
QtBrynhildr::~QtBrynhildr()
{
  // option
  if (option != 0){
	delete option;
	option = 0;
  }
  if (settings != 0){
	// disconnect to server
	if (settings->getConnected()){
	  disconnectToServer();
	}

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

  // close Log File
  logMessage->closeLogFile();

#if QTB_CRYPTOGRAM
  // delete cipher
  if (cipher != 0){
	delete cipher;
	cipher = 0;
  }
#endif

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
}

// get main window
MainWindow *QtBrynhildr::getMainWindow() const
{
  return mainWindow;
}

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
	mainWindow->refreshDesktop();
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

// get shutdown flag
bool QtBrynhildr::getShutdownFlag() const
{
  return option->getShutdownFlag();
}

// desktop Changed
void QtBrynhildr::onDesktopChanged(QImage image)
{
  // update desktop
  mainWindow->refreshDesktop(image);

  // update current frame rate
  if (settings->getOnShowFrameRate()){
	static int refreshCounter = 1;
	if (refreshCounter > QTB_STATUS_REFRESH_COUNT){
	  currentFrameRate = graphicsThread->getFrameRate();
	  updateFrameRate();
	  refreshCounter = 1;
	}
	refreshCounter++;
  }
}

// desktop clear
void QtBrynhildr::onDesktopClear()
{
  mainWindow->clearDesktop();
  refreshWindow();
}

// network error handler
void QtBrynhildr::onNetworkError()
{
  settings->setConnected(false);
  mainWindow->clearDesktop();
  refreshWindow();
}

// exit applilcation
void QtBrynhildr::exitApplication()
{
  exit();
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
  connectToServer_Action = new QAction(tr("&Connect"), this);
  connectToServer_Action->setStatusTip(tr("Connect to Brynhildr Server"));
  //  connectToServer_Action->setShortcut(tr("Ctrl+C"));
  connect(connectToServer_Action, SIGNAL(triggered()), this, SLOT(popUpConnectToServer()));

  // output log Action
  outputLog_Action = new QAction(tr("output Log ON/OFF"), this);
  outputLog_Action->setCheckable(true);
  outputLog_Action->setChecked(settings->getOutputLog());
  outputLog_Action->setStatusTip(tr("output Log ON/OFF"));
  connect(outputLog_Action, SIGNAL(triggered()), this, SLOT(toggleOutputLog()));

  // exit Action
  exit_Action = new QAction(tr("&Exit"), this);
  exit_Action->setStatusTip(tr("Exit Qt Brynhildr"));
  //  exit_Action->setShortcut(tr("Ctrl+E"));
  connect(exit_Action, SIGNAL(triggered()), this, SLOT(exit()));

  // about Action
  about_Action = new QAction(tr("About"), this);
  about_Action->setStatusTip(tr("Show Qt Brynhildr"));
  connect(about_Action, SIGNAL(triggered()), this, SLOT(about()));

  // Show Menu Bar
  showMenuBar_Action = new QAction(tr("Show Menu Bar"), this);
  showMenuBar_Action->setStatusTip(tr("Show Menu Bar"));
  showMenuBar_Action->setEnabled(false); // for TEST
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
  showFrameRate_Action = new QAction(tr("Show FrameRate"), this);
  showFrameRate_Action->setStatusTip(tr("Show FrameRate"));
  showFrameRate_Action->setCheckable(true);
  showFrameRate_Action->setChecked(settings->getOnShowFrameRate());
  connect(showFrameRate_Action, SIGNAL(triggered()), this, SLOT(toggleShowFrameRate()));

  // Full Screen
  if (QTB_DESKTOP_FULL_SCREEN){
	fullScreen_Action = new QAction(tr("Full Screen"), this);
	fullScreen_Action->setStatusTip(tr("Full Screen"));
	fullScreen_Action->setCheckable(true);
	fullScreen_Action->setChecked(false);
	connect(fullScreen_Action, SIGNAL(triggered()), this, SLOT(fullScreen()));
  }

  // Stays On Top
  if (QTB_DESKTOP_STAYS_ON_TOP){
	staysOnTop_Action = new QAction(tr("Stays On Top"), this);
	staysOnTop_Action->setStatusTip(tr("Stays On Top"));
	staysOnTop_Action->setCheckable(true);
	staysOnTop_Action->setChecked(settings->getOnStaysOnTop());
	connect(staysOnTop_Action, SIGNAL(triggered()), this, SLOT(toggleStaysOnTop()));
  }

  // Video Quality Action MINIMUM
  videoQuality_MINIMUM_Action = new QAction(tr("Video Quality: Minimum"), this);
  videoQuality_MINIMUM_Action->setCheckable(true);
  videoQuality_MINIMUM_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_MINIMUM);
  videoQuality_MINIMUM_Action->setStatusTip(tr("Video Quality: Minimum"));
  connect(videoQuality_MINIMUM_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_MINIMUM()));

  // Video Quality Action LOW
  videoQuality_LOW_Action = new QAction(tr("Video Quality: Low"), this);
  videoQuality_LOW_Action->setCheckable(true);
  videoQuality_LOW_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_LOW);
  videoQuality_LOW_Action->setStatusTip(tr("Video Quality: Low"));
  connect(videoQuality_LOW_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_LOW()));

  // Video Quality Action STANDARD
  videoQuality_STANDARD_Action = new QAction(tr("Video Quality: Standard"), this);
  videoQuality_STANDARD_Action->setCheckable(true);
  videoQuality_STANDARD_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_STANDARD);
  videoQuality_STANDARD_Action->setStatusTip(tr("Video Quality: Standard"));
  connect(videoQuality_STANDARD_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_STANDARD()));

  // Video Quality Action HIGH
  videoQuality_HIGH_Action = new QAction(tr("Video Quality: High"), this);
  videoQuality_HIGH_Action->setCheckable(true);
  videoQuality_HIGH_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_HIGH);
  videoQuality_HIGH_Action->setStatusTip(tr("Video Quality: High"));
  connect(videoQuality_HIGH_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_HIGH()));

  // Video Quality Action MAXIMUM
  videoQuality_MAXIMUM_Action = new QAction(tr("Video Quality: Maximum"), this);
  videoQuality_MAXIMUM_Action->setCheckable(true);
  videoQuality_MAXIMUM_Action->setChecked(settings->getVideoQuality() == VIDEO_QUALITY_MAXIMUM);
  videoQuality_MAXIMUM_Action->setStatusTip(tr("Video Quality: Maximum"));
  connect(videoQuality_MAXIMUM_Action, SIGNAL(triggered()), this, SLOT(setVideoQuality_MAXIMUM()));

  // Deskop Scaling Action
  if (QTB_DESKTOP_IMAGE_SCALING){
	desktopScalingDialog_Action = new QAction(tr("Desktop Scaling"), this);
	desktopScalingDialog_Action->setStatusTip(tr("Desktop Scaling"));
	desktopScalingDialog_Action->setEnabled(false);
	connect(desktopScalingDialog_Action, SIGNAL(triggered()), this, SLOT(desktopScaling()));
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

  selectFrameRateMaximum_Action = new QAction(tr("maximum FPS"), this);
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
  soundQuality_MINIMUM_Action = new QAction(tr("Sound Quality: Minimum"), this);
  soundQuality_MINIMUM_Action->setCheckable(true);
  soundQuality_MINIMUM_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_MINIMUM);
  soundQuality_MINIMUM_Action->setStatusTip(tr("Sound Quality: Minimum"));
  connect(soundQuality_MINIMUM_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_MINIMUM()));

  // Sound Quality Action LOW
  soundQuality_LOW_Action = new QAction(tr("Sound Quality: Low"), this);
  soundQuality_LOW_Action->setCheckable(true);
  soundQuality_LOW_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_LOW);
  soundQuality_LOW_Action->setStatusTip(tr("Sound Quality: Low"));
  connect(soundQuality_LOW_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_LOW()));

  // Sound Quality Action STANDARD
  soundQuality_STANDARD_Action = new QAction(tr("Sound Quality: Standard"), this);
  soundQuality_STANDARD_Action->setCheckable(true);
  soundQuality_STANDARD_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_STANDARD);
  soundQuality_STANDARD_Action->setStatusTip(tr("Sound Quality: Standard"));
  connect(soundQuality_STANDARD_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_STANDARD()));

  // Sound Quality Action HIGH
  soundQuality_HIGH_Action = new QAction(tr("Sound Quality: High"), this);
  soundQuality_HIGH_Action->setCheckable(true);
  soundQuality_HIGH_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_HIGH);
  soundQuality_HIGH_Action->setStatusTip(tr("Sound Quality: High"));
  connect(soundQuality_HIGH_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_HIGH()));

  // Sound Quality Action MAXIMUM
  soundQuality_MAXIMUM_Action = new QAction(tr("Sound Quality: Maximum"), this);
  soundQuality_MAXIMUM_Action->setCheckable(true);
  soundQuality_MAXIMUM_Action->setChecked(settings->getSoundQuality() == SOUND_QUALITY_MAXIMUM);
  soundQuality_MAXIMUM_Action->setStatusTip(tr("Sound Quality: Maximum"));
  connect(soundQuality_MAXIMUM_Action, SIGNAL(triggered()), this, SLOT(setSoundQuality_MAXIMUM()));

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

#if QTB_RECORDER
  // start recording control
  startRecordingControl_Action = new QAction(tr("Start Recodrding"), this);
  startRecordingControl_Action->setCheckable(true);
  startRecordingControl_Action->setChecked(settings->getOnRecordingControl());
  startRecordingControl_Action->setStatusTip(tr("Start Recodrding"));
  connect(startRecordingControl_Action, SIGNAL(triggered()), this, SLOT(startRecordingControl()));

  // stop recording control
  stopRecordingControl_Action = new QAction(tr("Stop Recodrding"), this);
  stopRecordingControl_Action->setStatusTip(tr("Stop Recodrding"));
  connect(stopRecordingControl_Action, SIGNAL(triggered()), this, SLOT(stopRecordingControl()));

  // replay recorded control
  replayRecordingControl_Action = new QAction(tr("Replay"), this);
  replayRecordingControl_Action->setStatusTip(tr("Replay"));
  connect(replayRecordingControl_Action, SIGNAL(triggered()), this, SLOT(replayRecordingControl()));
#endif // QTB_RECORDER

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

  // on Scroll Mode Action
  onScrollMode_Action = new QAction(tr("Scroll Mode"), this);
  onScrollMode_Action->setEnabled(false);
  onScrollMode_Action->setCheckable(true);
  onScrollMode_Action->setChecked(settings->getOnScrollMode());
  onScrollMode_Action->setStatusTip(tr("Scroll Mode"));
  connect(onScrollMode_Action, SIGNAL(triggered()), this, SLOT(toggleOnScrollMode()));
}

// create Menus
void QtBrynhildr::createMenus()
{
  // file menu
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(connectToServer_Action);
  fileMenu->addSeparator();
  fileMenu->addAction(exit_Action);

  // display menu
  displayMenu = menuBar()->addMenu(tr("&Display"));
  displayMenu->addAction(showMenuBar_Action);
  displayMenu->addAction(showStatusBar_Action);
  displayMenu->addAction(showFrameRate_Action);
  // for stays on top
  if (QTB_DESKTOP_STAYS_ON_TOP){
	displayMenu->addSeparator();
	displayMenu->addAction(staysOnTop_Action);
  }
  // for full screen
  if (QTB_DESKTOP_FULL_SCREEN){
	displayMenu->addSeparator();
	displayMenu->addAction(fullScreen_Action);
  }

  // video menu
  videoMenu = menuBar()->addMenu(tr("&Video"));
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

  // sound menu
  soundMenu = menuBar()->addMenu(tr("&Sound"));
  soundMenu->addAction(soundQuality_MINIMUM_Action);
  soundMenu->addAction(soundQuality_LOW_Action);
  soundMenu->addAction(soundQuality_STANDARD_Action);
  soundMenu->addAction(soundQuality_HIGH_Action);
  soundMenu->addAction(soundQuality_MAXIMUM_Action);

  // control menu
  controlMenu = menuBar()->addMenu(tr("&Control"));
  // for send key
  sendKeySubMenu = controlMenu->addMenu(tr("Send Key"));
#if 0 // for TEST
  sendKeySubMenu->addAction(sendKey1_Action);
#endif
  sendKeySubMenu->addAction(sendKey2_Action);
  sendKeySubMenu->addAction(sendKey3_Action);
  sendKeySubMenu->addAction(sendKey4_Action);

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

#if QTB_RECORDER
  // for record and replay
  controlMenu->addSeparator();
  recordAndReplaySubMenu = controlMenu->addMenu(tr("Record and Replay"));
  recordAndReplaySubMenu->addAction(startRecordingControl_Action);
  recordAndReplaySubMenu->addAction(stopRecordingControl_Action);
  recordAndReplaySubMenu->addAction(replayRecordingControl_Action);
#endif // QTB_RECORDER

  // option menu
  optionMenu = menuBar()->addMenu(tr("&Option"));
  modeSubMenu = optionMenu->addMenu(tr("Mode"));
  modeSubMenu->addAction(onScrollMode_Action);
  optionMenu->addSeparator();
  optionMenu->addAction(outputLog_Action);

  // in testing sub menu
  if (QTB_IN_TESTING){
	optionMenu->addSeparator();
	inTestingSubMenu = optionMenu->addMenu(tr("In Testing"));
	// in Testing Menu
  }

  menuBar()->addSeparator();

  // help menu
  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(about_Action);
}

// create Context Menu
void QtBrynhildr::createContextMenu()
{
}

// create Tool Bar
void QtBrynhildr::createToolBars()
{
  // fileToolBar = addToolBar(tr("&File"));
}

// create Status Bar
void QtBrynhildr::createStatusBar()
{
  connectionLabel = new QLabel;
  connectionLabel->setAlignment(Qt::AlignHCenter);

  frameRateLabel = new QLabel;
  frameRateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignLeft);
  frameRateLabel->setText(tr("FrameRate: ")+"00.00");
  // set initial size
  //  frameRateLabel->setMinimumSize(frameRateLabel->sizeHint());
  frameRateLabel->setMinimumSize(QSize(frameRateLabel->sizeHint().width() * 2,
									   frameRateLabel->sizeHint().height()));
  //  cout << "size.width  = " << frameRateLabel->sizeHint().width() << endl << flush;
  //  cout << "size.height = " << frameRateLabel->sizeHint().height() << endl << flush;

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

  if (settings->getConnected()){
	// connection
	connectionLabel->setText(tr("connected : ") + settings->getServerName());
  }
  else {
	// connection
	connectionLabel->setText(tr("connected : ") + "None");
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
  if (settings->getOnShowFrameRate()){
	if (settings->getConnected()){
	  frameRateLabel->setText(tr("FrameRate: ")+QString::number(currentFrameRate, 'f', 2));
	}
	else {
	  frameRateLabel->setText(tr("FrameRate: ")+"00.00");
	}
  }
  else {
	frameRateLabel->clear();
  }
}

// connected
void QtBrynhildr::connected()
{
  // enabled send key
  if (sendKey1_Action != 0)
    sendKey1_Action->setEnabled(true);
  if (sendKey2_Action != 0)
    sendKey2_Action->setEnabled(true);
  if (sendKey3_Action != 0)
    sendKey3_Action->setEnabled(true);
  if (sendKey4_Action != 0)
    sendKey4_Action->setEnabled(true);

  // enabled scaling dialog
  desktopScalingDialog_Action->setEnabled(true);

  // enable scroll mode
  onScrollMode_Action->setEnabled(true);

  // reset frameCounter
  frameCounter = 0;
}

// connected
void QtBrynhildr::disconnected()
{
  // disabled send key
  if (sendKey1_Action != 0)
    sendKey1_Action->setEnabled(false);
  if (sendKey2_Action != 0)
    sendKey2_Action->setEnabled(false);
  if (sendKey3_Action != 0)
    sendKey3_Action->setEnabled(false);
  if (sendKey4_Action != 0)
    sendKey4_Action->setEnabled(false);

  // disabled scaling dialog
  desktopScalingDialog_Action->setEnabled(false);

  // disabled scroll mode
  onScrollMode_Action->setEnabled(false);
}

// close event by window close
void QtBrynhildr::closeEvent(QCloseEvent *event)
{
  event->ignore();

  exit();
}

// load settings from setting files or registry
void QtBrynhildr::readSettings()
{
  // read global settings
  settings->readSettings();

  // restore geometry
  QRect rect = settings->getSettings()->value(QTB_GEOMETRY,
											 QRect(200, 200, 800, 600)).toRect();
  move(rect.topLeft());
  resize(rect.size());

  // set maximum width & height
  int targetWidth = rect.size().width() + settings->getDesktop()->getCorrectWindowWidth();
  setMaximumWidth(targetWidth);
  int targetHeight = rect.size().height() + heightOfMenuBar + heightOfStatusBar + settings->getDesktop()->getCorrectWindowHeight();
  setMaximumHeight(targetHeight);
}

// save settings to setting file or registry
void QtBrynhildr::writeSettings()
{
  // write global settings
  settings->writeSettings();

  // save geometry
  QRect geometryInfo(QPoint(frameGeometry().x(), frameGeometry().y()),
					 QSize(geometry().width(), geometry().height()));
  settings->getSettings()->setValue(QTB_GEOMETRY, geometryInfo);

#if 0 // for DEBUG
  qDebug() << "frameGeometry : " << frameGeometry(); // for DEBUG
  qDebug() << "geometry      : " << geometry(); // for DEBUG
#endif // for DEBUG

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
					 QTB_VERSION
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
  switch(settings->getKeyboardType()){
  case KEYBOARD_TYPE_JP:
	eventConverter = new EventConverter_JP();
	break;
  case KEYBOARD_TYPE_US:
	eventConverter = new EventConverter_US();
	break;
  default:
	// unknown keyboard type
	ABORT();
	break;
  }
  if (eventConverter == 0){
	return;
  }
  mainWindow->setEventConverter(eventConverter);

  // Software Keyboard and Button
  if (QTB_SOFTWARE_KEYBOARD_AND_BUTTON){
	// software keyboard
	if (softwareKeyboard != 0){
	  delete softwareKeyboard;
	  softwareKeyboard = 0;
	}
	switch(settings->getKeyboardType()){
	case KEYBOARD_TYPE_JP:
	  softwareKeyboard = new SoftwareKeyboard_JP(settings, mainWindow->getKeyBuffer(), this);
	  break;
	case KEYBOARD_TYPE_US:
	  softwareKeyboard = new SoftwareKeyboard_US(settings, mainWindow->getKeyBuffer(), this);
	  break;
	default:
	  // unknown keyboard type
	  ABORT();
	  break;
	}
	softwareKeyboard->setVisible(false);
	if (softwareKeyboardDockWidget != 0){
	  removeDockWidget(softwareKeyboardDockWidget);
	  delete softwareKeyboardDockWidget;
	  softwareKeyboardDockWidget = 0;
	}
	softwareKeyboardDockWidget = new QDockWidget(tr("Software Keyboard"));
	softwareKeyboardDockWidget->setWidget(softwareKeyboard);
	softwareKeyboardDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
	connect(softwareKeyboardDockWidget,
			SIGNAL(visibilityChanged(bool)), this, SLOT(visibilityChangedSoftwareKeyboard(bool)));
	addDockWidget(Qt::BottomDockWidgetArea, softwareKeyboardDockWidget);
	softwareKeyboardDockWidget->setVisible(false);

	// software button
	if (softwareButtonDockWidget == 0){
	  softwareButtonDockWidget = new QDockWidget(tr("Software Button"));
	  softwareButtonDockWidget->setWidget(softwareButton);
	  softwareButtonDockWidget->setAllowedAreas(Qt::TopDockWidgetArea);
	  connect(softwareButtonDockWidget,
			  SIGNAL(visibilityChanged(bool)), this, SLOT(visibilityChangedSoftwareButton(bool)));
	  addDockWidget(Qt::TopDockWidgetArea, softwareButtonDockWidget);
	}
	softwareButtonDockWidget->setVisible(false);
  }

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

  // connected
  connected();
}

// disconnect to server
void QtBrynhildr::disconnectToServer()
{
  // connect check
  if (!settings->getConnected()){
	return;
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

  // close socket
  closeSocket();

  // disconnected
  disconnected();

  // disconnect
  settings->setConnected(false);

  // refresh window
  refreshWindow();
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

  // disconnected
  if (settings->getConnected()){
	disconnectToServer();
  }

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

  // normal screen
  if (QTB_DESKTOP_FULL_SCREEN){
	if (fullScreenMode){
	  // to Normal Screen
	  showNormal();
	  fullScreenMode = false;
	}
  }

  // save settings
  writeSettings();

  // log
  QDateTime shutdownTime = QDateTime::currentDateTime();
  if (settings->getOutputLog()){
	logMessage->outputLogMessage(PHASE_DEBUG,
								 "Shutdown : " +
								 shutdownTime.toString(dateFormat));
  }
  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, tr("Shutdown."));

  // output average of frame rate
  // get frame counter
  frameCounter = graphicsThread->getFrameCounter();
  if (frameCounter != 0){
	//	cout << "frameCounter = " << frameCounter << endl << flush;
	uint diffSeconds = shutdownTime.toTime_t() - bootTime.toTime_t();
	if (diffSeconds != 0){
	  float averageFrameRate = (float)frameCounter/diffSeconds;
	  //	  cout << "FPS of last session: " << averageFrameRate << endl << flush;
	  QString averageFrameRateString;
	  averageFrameRateString.setNum(averageFrameRate);
	  logMessage->outputLogMessage(PHASE_QTBRYNHILDR, tr("FPS of last session: ") + averageFrameRateString + " FPS");
	}
  }

  // quit
  qApp->quit();
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

// toggle onControl
void QtBrynhildr::toggleOnControl()
{
  if (settings->getOnControl()){
	settings->setOnControl(false);
	onControl_Action->setChecked(false);
	// clear device buffer
	mainWindow->getKeyBuffer()->clear();
	mainWindow->getMouseBuffer()->clear();
  }
  else {
	settings->setOnControl(true);
	onControl_Action->setChecked(true);
  }
}

// toggle onGraphics
void QtBrynhildr::toggleOnGraphics()
{
  if (settings->getOnGraphics()){
	settings->setOnGraphics(false);
	onGraphics_Action->setChecked(false);

	// onControl Off
	if (settings->getOnControlOffWithGraphicsOff()){
	  settings->setOnControl(false);
	  onControl_Action->setChecked(false);
	}
  }
  else {
	settings->setOnGraphics(true);
	onGraphics_Action->setChecked(true);
  }
}

// toggle onSound
void QtBrynhildr::toggleOnSound()
{
  if (settings->getOnSound()){
	settings->setOnSound(false);
	onSound_Action->setChecked(false);
  }
  else {
	settings->setOnSound(true);
	onSound_Action->setChecked(true);
  }
}

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
  // stop to record
  recorder->stopRecording(qPrintable(fileName));

  // checked
  startRecordingControl_Action->setChecked(false);
}

// replay recorded control
void QtBrynhildr::replayRecordingControl()
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
  strncpy(filename, qPrintable(fileName), QTB_MAXPATHLEN);
  settings->setReplayingControlFileName(filename);

  // start to replay
  recorder->startReplaying();
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

// toggle show menu bar
void QtBrynhildr::toggleShowMenuBar()
{
  if (settings->getOnShowMenuBar()){
	settings->setOnShowMenuBar(false);
	menuBar()->setVisible(false);
  }
  else {
	settings->setOnShowMenuBar(true);
	menuBar()->setVisible(true);
  }
  refreshWindow();
}

// toggle show status bar
void QtBrynhildr::toggleShowStatusBar()
{
  if (settings->getOnShowStatusBar()){
	settings->setOnShowStatusBar(false);
	statusBar()->setVisible(false);
  }
  else {
	settings->setOnShowStatusBar(true);
	statusBar()->setVisible(true);
  }
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
  fullScreenMode = !fullScreenMode;
  if (fullScreenMode){
	showFullScreen();
  }
  else {
	showNormal();
  }
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
#if defined(Q_OS_LINUX) || defined(Q_OS_OSX)
  QPoint topLeft = QPoint(geometry().x(), geometry().y() + 28);
#endif // defined(Q_OS_LINUX) || defined(Q_OS_OSX)
  setWindowFlags(flags);
#if defined(Q_OS_LINUX) || defined(Q_OS_OSX)
  move(topLeft);
#endif // defined(Q_OS_LINUX) || defined(Q_OS_OSX)
  show();
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
  if (settings->getOnScrollMode()){
	settings->setOnScrollMode(false);
  }
  else {
	settings->setOnScrollMode(true);
  }
}

// visibility changed software keyboard
void QtBrynhildr::visibilityChangedSoftwareKeyboard(bool visible)
{
  settings->setOnShowSoftwareKeyboard(visible);
}

// visibility changed software button
void QtBrynhildr::visibilityChangedSoftwareButton(bool visible)
{
  settings->setOnShowSoftwareButton(visible);
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

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_OSX)
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
#endif // defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_OSX)

// for Windows
#if defined(Q_OS_WIN)
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
#endif // defined(Q_OS_WIN)

// for Linux and Darwin
#if defined(Q_OS_LINUX) || defined(Q_OS_OSX)
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
#endif // defined(Q_OS_LINUX) || defined(Q_OS_OSX)

} // end of namespace qtbrynhildr
