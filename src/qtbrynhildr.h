// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef QTBRYNHILDR_H
#define QTBRYNHILDR_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QAction>
#include <QClipboard>
#include <QDateTime>
#include <QHideEvent>
#include <QImage>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QObject>
#include <QPalette>
#include <QProgressBar>
#include <QRect>
#if QTB_NEW_DESKTOPWINDOW
#include <QGraphicsScene>
#else // QTB_NEW_DESKTOPWINDOW
#include <QScrollArea>
#endif // QTB_NEW_DESKTOPWINDOW
#include <QShowEvent>
#include <QSize>
#include <QString>
#include <QTimer>
#include <QToolBar>
#include <QtGlobal>

// Local Header
#include "dialog/confirm_dialog.h"
#include "dialog/connect_to_server_dialog.h"
#include "dialog/desktop_scaling_dialog.h"
#include "dialog/log_view_dialog.h"
#if QTB_PREFERENCE
#include "dialog/preference_dialog.h"
#endif // QTB_PREFERENCE
#if QTB_CRYPTOGRAM
#include "function/cipher.h"
#endif // QTB_CRYPTGRAM
#include "logmessage.h"
#include "mainwindow/desktoppanel.h"
#if QTB_NEW_DESKTOPWINDOW
#include "mainwindow/graphicsview.h"
#include "mainwindow/desktoppanelobject.h"
#else // QTB_NEW_DESKTOPWINDOW
#include "mainwindow/desktopwindow.h"
#endif // QTB_NEW_DESKTOPWINDOW
#include "option.h"
#if QTB_RECORDER
#include "function/recorder.h"
#endif // QTB_RECORDER
#include "settings.h"
#include "touchpanel2/sk.h"
#include "touchpanel2/sb.h"
#include "util/httpgetter.h"

// thread
#include "controlthread/controlthread.h"
#include "graphicsthread/graphicsthread.h"
#if 0 // for TEST
#include "graphicsthread/graphicsbuffer.h"
#endif // 0 // for TEST
#include "soundthread/soundthread.h"

#ifdef USE_KEYLAYOUTFILE
// key layout
#include "keylayout/keylayoutfilemanager.h"
#include "keylayout/keylayoutfilereader.h"
#endif // USE_KEYLAYOUTFILE

// event converter
#include "windows/eventconverter.h"

// for TEST
#define QTB_TEST_DRAW_FRAME 0

namespace qtbrynhildr {

// ----------------------------------------------
// global objects
// ----------------------------------------------
#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
// socket for control
extern SOCKET sock_control;
// socket for graphics
extern SOCKET sock_graphics;
// socket for sound
extern SOCKET sock_sound;
#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

// counter for control
extern int counter_control;
// counter for graphics
extern int counter_graphics;

// frame_no of server
extern int frameNoOfServer;


// QtBrynhildr
class QtBrynhildr : public QMainWindow
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // GUI
  // desktop panel
  DesktopPanel *desktopPanel;

#if QTB_NEW_DESKTOPWINDOW
  // view
  GraphicsView *graphicsView;
  // scene
  QGraphicsScene *graphicsScene;
  // desktop panel object
  DesktopPanelObject *desktopPanelObject;
#else // QTB_NEW_DESKTOPWINDOW
  // scroll area
  QScrollArea *scrollArea;
  // desktop window
  DesktopWindow *desktopWindow;
#endif // QTB_NEW_DESKTOPWINDOW

  // connection label
  QLabel *connectionLabel;
  // frame rate label
  QLabel *frameRateLabel;

  // ------------------------------------------------------------
  // Menu
  // ------------------------------------------------------------

  // File Menu
  QMenu *fileMenu;

  // Display Menu
  QMenu *displayMenu;

  // Video Menu
  QMenu *videoMenu;

  // Select Monitor No Sub Menu
  QMenu *selectMonitorNoSubMenu;

  // Select Frame Rate No Sub Menu
  QMenu *selectFrameRateSubMenu;

  // Sound Menu
  QMenu *soundMenu;

  // Sound Cache Sub Menu
  QMenu *soundCacheSubMenu;

  // Control Menu
  QMenu *controlMenu;

  // Send Key Sub Menu
  QMenu *sendKeySubMenu;

  // Select Public Mode Version Sub Menu
  QMenu *selectPublicModeVersionSubMenu;

#if QTB_RECORDER
  // Record and Replay Sub Menu
  QMenu *recordAndReplaySubMenu;
#endif // QTB_RECORDER

  // Option Menu
  QMenu *optionMenu;

#if defined(QTB_DEV_TOUCHPANEL)
  // touchpanel Interface Type Sub Menu
  QMenu *touchpanelInterfaceTypeSubMenu;
#endif // defined(QTB_DEV_TOUCHPANEL)

  // In Testing Sub Menu
  QMenu *inTestingSubMenu;

#if QTB_DESKTOP_COMPRESS_MODE
  // Desktop Compress Mode Sub Menu
  QMenu *desktopCompressModeSubMenu;
#endif // QTB_DESKTOP_COMPRESS_MODE

  // Help Menu
  QMenu *helpMenu;

#if QTB_BENCHMARK
  // Benchmark Menu
  QMenu *benchmarkMenu;
#endif // QTB_BENCHMARK

  // QToolBar *fileToolBar;

  // ------------------------------------------------------------
  // Action for Menu
  // ------------------------------------------------------------
  // connect to server
  QAction *connectToServer_Action;

  // disconnect to server
  QAction *disconnectToServer_Action;

  // output keyboard log
  QAction *outputKeyboardLog_Action;

  // output log
  QAction *outputLog_Action;

  // exit application
  QAction *exit_Action;

  // about this application
  QAction *about_Action;

  // check update
  QAction *checkUpdate_Action;

  // video Quality MINIMUM
  QAction *videoQuality_MINIMUM_Action;
  // video Quality LOW
  QAction *videoQuality_LOW_Action;
  // video Quality STANDARD
  QAction *videoQuality_STANDARD_Action;
  // video Quality HIGH
  QAction *videoQuality_HIGH_Action;
  // video Quality MAXIMUM
  QAction *videoQuality_MAXIMUM_Action;

  // show menu bar
  QAction *showMenuBar_Action;

  // show status bar
  QAction *showStatusBar_Action;

  // show frame rate
  QAction *showFrameRate_Action;

  // full screen
  QAction *fullScreen_Action;

  // stays on top
  QAction *staysOnTop_Action;

  // desktop scale fixed
  QAction *desktopScaleFixed_Action;

  // window size fixed
  QAction *windowSizeFixed_Action;

  // desktop scaling
  QAction *desktopScalingDialog_Action;

  // desktop capture
  QAction *desktopCapture_Action;

  // log view
  QAction *logViewDialog_Action;

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // show software keyboard
  QAction *showSoftwareKeyboard_Action;

  // show software button
  QAction *showSoftwareButton_Action;
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // frame rate
  QAction *selectFrameRate5_Action;
  QAction *selectFrameRate10_Action;
  QAction *selectFrameRate20_Action;
  QAction *selectFrameRate30_Action;
  QAction *selectFrameRate40_Action;
  QAction *selectFrameRate50_Action;
  QAction *selectFrameRate60_Action;
  QAction *selectFrameRateMaximum_Action;

  // select monitor
  QAction *selectMonitorNo1_Action;
  QAction *selectMonitorNo2_Action;
  QAction *selectMonitorNo3_Action;
  QAction *selectMonitorNo4_Action;
  QAction *selectMonitorNo5_Action;
  QAction *selectMonitorNo6_Action;
  QAction *selectMonitorNo7_Action;
  QAction *selectMonitorNo8_Action;
  QAction *selectMonitorNo9_Action;
  QAction *selectMonitorNoAll_Action;

  // sound Quality MINIMUM
  QAction *soundQuality_MINIMUM_Action;
  // sound Quality LOW
  QAction *soundQuality_LOW_Action;
  // sound Quality STANDARD
  QAction *soundQuality_STANDARD_Action;
  // sound Quality HIGH
  QAction *soundQuality_HIGH_Action;
  // sound Quality MAXIMUM
  QAction *soundQuality_MAXIMUM_Action;

  // sound Cache 0
  QAction *soundCache_0_Action;
  // sound Cache 1
  QAction *soundCache_1_Action;
  // sound Cache 2
  QAction *soundCache_2_Action;
  // sound Cache 3
  QAction *soundCache_3_Action;
  // sound Cache 4
  QAction *soundCache_4_Action;
  // sound Cache 5
  QAction *soundCache_5_Action;

  // toggle onControl
  QAction *onControl_Action;

  // toggle onGraphics
  QAction *onGraphics_Action;

  // toggle onSound
  QAction *onSound_Action;

  // publi mode version
  QAction *selectPublicModeVersion5_Action;
  QAction *selectPublicModeVersion6_Action;
  QAction *selectPublicModeVersion7_Action;

#if QTB_RECORDER
  // start recording control
  QAction *startRecordingControl_Action;
  // stop recording control
  QAction *stopRecordingControl_Action;
  // replay recorded control
  QAction *startReplayRecordingControl_Action;
  // stop replay recorded control
  QAction *stopReplayRecordingControl_Action;
#endif // QTB_RECORDER

#if QTB_PLUGINS_DISABLE_SUPPORT
  // set onPluginsDisable
  QAction *onPluginsDisable_Action;
#endif // QTB_PLUGINS_DISABLE_SUPPORT

  // send key
  QAction *sendKey1_Action;
  QAction *sendKey2_Action;
  QAction *sendKey3_Action;
  QAction *sendKey4_Action;
  QAction *sendKey5_Action;
  QAction *sendKey6_Action;

  // toggle scroll mode
  QAction *onScrollMode_Action;

#if defined(QTB_DEV_TOUCHPANEL)
  // touchpanel interface type KeroRemote
  QAction *touchpanelInterfaceTypeKeroRemote_Action;
  // touchpanel interface type QtBrynhildr
  QAction *touchpanelInterfaceTypeQtBrynhildr_Action;
#endif // defined(QTB_DEV_TOUCHPANEL)

  // send clipboard
  QAction *sendClipboard_Action;

  // send file
  QAction *sendFile_Action;

  // cancel file transferring
  QAction *cancelFileTransferring_Action;

#if QTB_DESKTOP_COMPRESS_MODE
  // desktop compress mode
  QAction *desktopCompressMode0_Action;
  QAction *desktopCompressMode2_Action;
  QAction *desktopCompressMode4_Action;
  QAction *desktopCompressMode8_Action;
#endif // QTB_DESKTOP_COMPRESS_MODE

#if QTB_PREFERENCE
  // preferences
  QAction *preferences_Action;
#endif // QTB_PREFERENCE

  // disable drawing
  QAction *disableDrawing_Action;

  // disable maxfps
  QAction *disableMaxfps_Action;

#if QTB_BENCHMARK
  // select phase for benchmark
  QAction *selectBenchmarkPhase0_Action;
  QAction *selectBenchmarkPhase1_Action;
  QAction *selectBenchmarkPhase2_Action;
  QAction *selectBenchmarkPhase3_Action;
  QAction *selectBenchmarkPhase4_Action;
#endif // QTB_BENCHMARK

  // connect to server dialog
  ConnectToServerDialog *connectToServerDialog;

  // desktop scling dialog
  DesktopScalingDialog *desktopScalingDialog;

  // log view dialog
  LogViewDialog *logViewDialog;

#if QTB_PREFERENCE
  // preference dialog
  PreferenceDialog *preferenceDialog;
#endif // QTB_PREFERENCE

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // software keyboard and button
  SK *softwareKeyboard;
  SB *softwareButton;

#if defined(QTB_DEV_TOUCHPANEL)
  struct {
	QRect softwareButtonRect;
	QRect softwareKeyboardRect;
  } touchpanelInterface[QTB_TOUCHPANELINTERFACETYPE_NUM];
#endif // defined(QTB_DEV_TOUCHPANEL)

#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // for logging
  QDateTime bootTime;

  // total frame counter
  unsigned int totalFrameCounter;

  // current frame rate
  int currentFrameRate;

  // current data rate
  double currentDataRate;

#if QTB_TEST_DRAW_FRAME
  // draw counter
  int drawCounter;
#endif // QTB_TEST_DRAW_FRAME

  // option
  Option *option;

  // ini filename
  const char *iniFileName;

  // setting
  Settings *settings;

  // write settingAtExit
  bool writeSettingsAtExit;

#if QTB_CRYPTOGRAM
  // cipher
  Cipher *cipher;
#endif // QTB_CRYPTGRAM

#if QTB_RECORDER
  // recorder
  Recorder *recorder;
#endif // QTB_RECORDER

  // http getter
  HttpGetter *httpGetter;

  // current path
  QString currentPath;

  // log message
  LogMessage *logMessage;

  // thread for control
  ControlThread *controlThread;

  // thread for graphics
  GraphicsThread *graphicsThread;

#if 0 // for TEST
  // graphics buffer
  GraphicsBuffer *graphicsBuffer;
#endif // 0 // for TEST

  // thread for sound
  SoundThread *soundThread;

#ifdef USE_KEYLAYOUTFILE
  // key layout file manager
  KeyLayoutFileManager *keyLayoutFileManager;

  // key layout file reader
  KeyLayoutFileReader *keyLayoutFileReader;
#endif // USE_KEYLAYOUTFILE

  // event converter
  EventConverter *eventConverter;

  // clipboard
  QClipboard *clipboard;

  // full screen
  bool fullScreenMode;
  bool onSetDesktopScalingFactorForFullScreen;

  // current status
  bool onShowMenuBar;
  bool onShowStatusBar;

  // progress bar
  QProgressBar	*progressBar;

  // height of title bar
  int heightOfTitleBar;

  // height of menu bar
  int heightOfMenuBar;

  // height of status bar
  int heightOfStatusBar;

  // margins
  int widthMargin;
  int heightMargin;

  // palettes
  QPalette backgroundPalette;
  QPalette fullScreenBackgroundPalette;

  // current onControl/onGraphics/onSound
  bool onControl;
  bool onGraphics;
  bool onSound;

  // key buffer
  KeyBuffer *keyBuffer;

  // mouse buffer
  MouseBuffer *mouseBuffer;

  // timer for GUI
  QTimer *timer;

#if 0 // for TEST
  // timer for Graphics
  QTimer *timer_Graphics;

  // image
  QImage *image;
#endif // 0 // for TEST

  // clear desktop flag
  bool onClearDesktop;

  // SIMD instruction
  bool hasSIMDInstruction;

  // pop up Connect To Server Dialog
  bool onPopUpConnectToServer;

  // check update in background mode
  bool onCheckUpdateInBackground;

#if QTB_BENCHMARK
  // initial phase counter for benchmark
  int initialBenchmarkPhaseCounter;

  // display benchmark menu
  bool onBenchmarkMenu;
#endif // QTB_BENCHMARK

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  QtBrynhildr(Option *option, QClipboard *clipboard);
  // destructor
  ~QtBrynhildr();

  // get settings
  Settings *getSettings() const
  {
	return settings;
  }

#if QTB_NEW_DESKTOPWINDOW
  // get graphics view
  GraphicsView *getGraphicsView() const;
#else // QTB_NEW_DESKTOPWINDOW
  // get desktop window
  DesktopWindow *getDesktopWindow() const;
#endif // QTB_NEW_DESKTOPWINDOW

  // get desktop panel
  DesktopPanel *getDesktopPanel() const;

  // shutdown flag
  bool getShutdownFlag() const;

  // exit full screen
  void exitFullScreen();

  // get height of title bar
  int getHeightOfTitleBar();

  // get height of menu bar
  int getHeightOfMenuBar();

  // get height of status bar
  int getHeightOfStatusBar();

#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
  // set cursor point color
  void setCursorPointColor(QRgb cursorPointColor);
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // move top of software keyboard
  void moveTopOfSoftwareKeyboard(int y);

  // move top of software button
  void moveTopOfSoftwareButton(int y);

  // toggle software keyboard
  void toggleSoftwareKeyboard();
  // toggle software button
  void toggleSoftwareButton();
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

public slots:
  // connect to server
  void connectToServer();

  // connected
  void connected();

  // refresh window
  void refreshWindow();

  // adjust window
  void adjustWindow(const QSize &size);

  // refresh menu
  void refreshMenu();

  // draw desktop
  void drawDesktop(QImage image);

  // clear desktop
  void clearDesktop();

  // change mouse cursor
  void changeMouseCursor(const QCursor &cursor);

  // network error handler
  void onNetworkError(bool doRetry);

  // exit applilcation
  void exitApplication();

  // set clipboard
  void setClipboard(QString clipboardString);

  // set progress bar value for transfer file
  void setFileTransferProgressBarValue(int value);

  // output Log Message
  void outputLogMessage(int msgID);

  // output Log Message
  void outputLogMessage(int id, const QString text);

protected:
#if 0 // for TEST
  // change event
  void changeEvent(QEvent *event);
#endif // for TEST

  // window close event
  void closeEvent(QCloseEvent *event);

  // window resize event
  void resizeEvent(QResizeEvent *event);

  // window hide event
  void hideEvent(QHideEvent *event);

  // window show event
  void showEvent(QShowEvent *event);

  // context menu event
  void contextMenuEvent(QContextMenuEvent *event);

private slots:
  // popup connect to server dialog
  void popUpConnectToServer();

  // disconnect to server
  void popUpDisconnectToServer();

  // reconnect to server
  void reconnectToServer();

  // disconnect to server
  void disconnectToServer();

  // finished thread
  void finishedNetThread();

  // toggle output keyboard log mode
  void toggleOutputKeyboardLog();

  // toggle output log mode
  void toggleOutputLog();

  // about dialog
  void about();

#if QTB_UPDATECHECK
  // check update
  void checkUpdate();
#endif // QTB_UPDATECHECK

  // exit from QtBynhildr
  void exit();

  // send clipboard
  void sendClipboard();

  // send file
  void sendFile();

  // cancel file transferring
  void cancelFileTransferring();

#if QTB_PREFERENCE
  // preferences
  void preferences();
#endif // QTB_PREFERENCE

  // set video quality
  void setVideoQuality_MINIMUM();
  void setVideoQuality_LOW();
  void setVideoQuality_STANDARD();
  void setVideoQuality_HIGH();
  void setVideoQuality_MAXIMUM();

  // set sound quality
  void setSoundQuality_MINIMUM();
  void setSoundQuality_LOW();
  void setSoundQuality_STANDARD();
  void setSoundQuality_HIGH();
  void setSoundQuality_MAXIMUM();

  // set sound cache
  void setSoundCache_0();
  void setSoundCache_1();
  void setSoundCache_2();
  void setSoundCache_3();
  void setSoundCache_4();
  void setSoundCache_5();

  // toggle onControl/onGraphics/onSound
  void toggleOnControl();
  void toggleOnGraphics();
  void toggleOnSound();

private:
  // setup window title
  void setupWindowTitle();
  // refresh public mode
  void refreshPublicMode();
  // refresh other menu
  void refreshOtherMenu();
#if QTB_BENCHMARK
  // refresh benchmark menu check mark
  void refreshBenchmarkMenuCheck();
  // refresh benchmark menu
  void refreshBenchmarkMenu();
#endif // QTB_BENCHMARK

private slots:
  // select public mode version
  void selectPublicModeVersion5();
  void selectPublicModeVersion6();
  void selectPublicModeVersion7();

#if QTB_RECORDER
  // record and replay
  void startRecordingControl();
  void stopRecordingControl();
  void startReplayRecordingControl();
  void stopReplayRecordingControl();
#endif // QTB_RECORDER

#if QTB_PLUGINS_DISABLE_SUPPORT
  // set onPluginsDisable
  void setOnPluginsDisable();
#endif // QTB_PLUGINS_DISABLE_SUPPORT

  // send key
#if 0 // for TEST
  void sendKey_CTRL_ALT_DEL(); // CTRL + ALT + DEL
#endif
  void sendKey_ALT_F4(); // ALT + F4
  void sendKey_CTRL_ESC(); // CTRL + ESC
  void sendKey_WINDOWS(); // WINDOWS
  void sendKey_PrintScreen(); // PrintScreen
  void sendKey_ALT_PrintScreen(); // ALT + PrintScreen

  // toggle show menu bar
  void toggleShowMenuBar();

  // toggle show status bar
  void toggleShowStatusBar();

  // toggle show frame rate
  void toggleShowFrameRate();

  // full screen
  void fullScreen();

  // toggle stays on top
  void toggleStaysOnTop();

  // desktop scale fixed
  void toggleDesktopScaleFixed();

  // window size fixed
  void toggleWindowSizeFixed();

  // desktop scaling
  void desktopScaling();

  // desktop capture
  void desktopCapture();

  // log view
  void logView();

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // calulate software keyboard layout
  QRect calculateSoftwareKeyboardLayout();

  // calulate software button layout
  QRect calculateSoftwareButtonLayout();

  // toggle show software keyboard
  void toggleShowSoftwareKeyboard();

  // toggle show software button
  void toggleShowSoftwareButton();

  // setup software keyboard
  void setupSoftwareKeyboard();

  // setup software button
  void setupSoftwareButton();
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // select frame rate
  void selectFrameRate5();
  void selectFrameRate10();
  void selectFrameRate20();
  void selectFrameRate30();
  void selectFrameRate40();
  void selectFrameRate50();
  void selectFrameRate60();
  void selectFrameRateMaximum();

  // select monitor
  void setSelectMonitorNo1();
  void setSelectMonitorNo2();
  void setSelectMonitorNo3();
  void setSelectMonitorNo4();
  void setSelectMonitorNo5();
  void setSelectMonitorNo6();
  void setSelectMonitorNo7();
  void setSelectMonitorNo8();
  void setSelectMonitorNo9();
  void setSelectMonitorNoAll();

  // scroll mode
  void toggleOnScrollMode();

#if defined(QTB_DEV_TOUCHPANEL)
  // touchpanel interface type
  void touchpanelInterfaceTypeKeroRemote();
  void touchpanelInterfaceTypeQtBrynhildr();
#endif // defined(QTB_DEV_TOUCHPANEL)

#if 0 // QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // visibility changed software keyboard
  void visibilityChangedSoftwareKeyboard(bool visible);

  // visibility changed software button
  void visibilityChangedSoftwareButton(bool visible);
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

#if QTB_DESKTOP_COMPRESS_MODE
  // desktop compress mode
  void desktopCompressMode0();
  void desktopCompressMode2();
  void desktopCompressMode4();
  void desktopCompressMode8();
#endif // QTB_DESKTOP_COMPRESS_MODE

  // disable drawing
  void disableDrawing();

  // disable maxfps
  void disableMaxfps();

#if QTB_BENCHMARK
  // select phase for benchmark
  void selectBenchmarkPhase(int initialBenchmarkPhaseCounter);
  void selectBenchmarkPhase0();
  void selectBenchmarkPhase1();
  void selectBenchmarkPhase2();
  void selectBenchmarkPhase3();
  void selectBenchmarkPhase4();
#endif // QTB_BENCHMARK

private:
  // build GUI
  void createActions();
  void createMenus();
  void createContextMenu();
  void createToolBars();
  void createStatusBar();

  // settings
  // read settings from user configuration (registry etc.)
  void readSettings();
  // write settings from user configuration (registry etc.)
  void writeSettings();

  // update status bar
  void updateStatusBar();

  // update connected
  void updateConnected();

  // update frame rate
  void updateFrameRate();

  // disconnected
  void disconnected();

  // set desktop scaling factor
  void setDesktopScalingFactor(QSize windowSize);

  // refresh monitor menu
  void refreshMonitorMenu();

  // video quality
  void refreshVideoQualityMenu();

  // sound quality
  void refreshSoundQualityMenu();

#if QTB_RECORDER
  // recording and replaying
  void refreshRecordingAndReplayMenu();
#endif // QTB_RECORDER

  // clear Video Quality check
  void clearVideoQualityCheck();

  // clear Sound Quality check
  void clearSoundQualityCheck();

  // clear Sound Cache check
  void clearSoundCacheCheck();

  // clear Select Frame Rate
  void clearSelectFrameRateCheck();

  // clear Select Monitor No check
  void clearSelectMonitorNoCheck();

  // disabled Select Monitor No check
  void disabledSelectMonitor();

  // initialize socket
  void initSocket();

  // close socket
  void closeSocket();

  // initialize platform
  bool initPlatform();

  // shutdown platform
  bool shutdownPlatform();

  // set margins
  void setMargins(int widthMargin, int heightMargin)
  {
	this->widthMargin = widthMargin;
	this->heightMargin = heightMargin;
  }

#if 0 // for TEST
  // start timer graphics
  void startTimer_Graphics(int frameRate);

  // initialize graphics
  void init_Graphics();

  // draw graphics
  void draw_Graphics();
#endif // 0 // for TEST

private slots:
#if QTB_UPDATECHECK
  // finished download
  void finishedDownload();
#endif // QTB_UPDATECHECK

  void timerExpired();

#if 0 // for TEST
  void timerExpired_Graphics();
#endif // 0 // for TEST
};

} // end of namespace qtbrynhildr

#endif // QTBRYNHILDR_H
