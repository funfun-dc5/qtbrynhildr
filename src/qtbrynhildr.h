// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef QTBRYNHILDR_H
#define QTBRYNHILDR_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QAction>
#if QTB_PUBLIC_MODE6_SUPPORT
#include <QClipboard>
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if !QTB_BRYNHILDR2_SUPPORT
#include <QCursor>
#endif // !QTB_BRYNHILDR2_SUPPORT
#include <QDateTime>
#if 0 // for TEST
#include <QDockWidget>
#endif // for TEST
#include <QHideEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QObject>
#include <QPalette>
#if QTB_PUBLIC_MODE6_SUPPORT
#include <QProgressBar>
#endif // QTB_PUBLIC_MODE6_SUPPORT
#include <QScrollArea>
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
#if QTB_CRYPTOGRAM
#include "function/cipher.h"
#endif // QTB_CRYPTGRAM
#include "logmessage.h"
#include "mainwindow.h"
#include "option.h"
#if QTB_RECORDER
#include "function/recorder.h"
#endif // QTB_RECORDER
#include "settings.h"
#include "touchpanel2/sk.h"
#include "touchpanel2/sb.h"

// thread
#include "control/controlthread.h"
#include "graphics/graphicsthread.h"
#include "sound/soundthread.h"

#ifdef USE_KEYLAYOUTFILE
// key layout
#include "keylayout/keylayoutfilemanager.h"
#include "keylayout/keylayoutfilereader.h"
#endif // USE_KEYLAYOUTFILE

// event converter
#include "windows/eventconverter.h"

#include "util/httpgetter.h"

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

// QtBrynhildr
class QtBrynhildr : public QMainWindow
{
  Q_OBJECT

public:
  // constructor
#if QTB_PUBLIC_MODE6_SUPPORT
  QtBrynhildr(Option *option, QClipboard *clipboard);
#else // QTB_PUBLIC_MODE6_SUPPORT
  QtBrynhildr(Option *option);
#endif // QTB_PUBLIC_MODE6_SUPPORT
  // destructor
  ~QtBrynhildr();

  // get main window
  MainWindow *getMainWindow() const;

  // shutdown flag
  bool getShutdownFlag() const;

  // exit full screen
  void exitFullScreen();

  // get height of menu bar
  int getHeightOfMenuBar();

  // get height of status bar
  int getHeightOfStatusBar();

public slots:
  // connect to server
  void connectToServer();

  // connected
  void connected();

  // refresh window
  void refreshWindow();

  // refresh menu
  void refreshMenu();

  // desktop Changed
  void onDesktopChanged(QImage image);

  // Desktop clear
  void onDesktopClear();

#if QTB_BRYNHILDR2_SUPPORT
  // change mouse cursor
  void changeMouseCursor(const QCursor &cursor);
#endif // QTB_BRYNHILDR2_SUPPORT

  // network error handler
  void onNetworkError(bool doRetry);

  // exit applilcation
  void exitApplication();

#if QTB_PUBLIC_MODE6_SUPPORT
  // set clipboard
  void setClipboard(QString clipboardString);

  // set progress bar value for transfer file
  void setFileTransferProgressBarValue(int value);
#endif // QTB_PUBLIC_MODE6_SUPPORT

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

#if 0 // for TEST
  // context menu event
  void contextMenuEvent(QContextMenuEvent *event);
#endif // for TEST

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

#if QTB_PUBLIC_MODE6_SUPPORT
  // send clipboard
  void sendClipboard();

  // send file
  void sendFile();

  // cancel file transferring
  void cancelFileTransferring();
#endif // QTB_PUBLIC_MODE6_SUPPORT

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

  // calulate software keyboard layout
  void calculateSoftwareKeyboardLayout();

  // calulate software button layout
  void calculateSoftwareButtonLayout();

  // toggle show software keyboard
  void toggleShowSoftwareKeyboard();

  // toggle show software button
  void toggleShowSoftwareButton();

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

  // visibility changed software keyboard
  void visibilityChangedSoftwareKeyboard(bool visible);

  // visibility changed software button
  void visibilityChangedSoftwareButton(bool visible);

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

private:
  // GUI
  // scroll area
  QScrollArea *scrollArea;
  // main window
  MainWindow *mainWindow;
  // window size
  QSize mainWindowSize;

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

#if QTB_RECORDER
  // Record and Replay Sub Menu
  QMenu *recordAndReplaySubMenu;
#endif // QTB_RECORDER

  // Option Menu
  QMenu *optionMenu;

  // Mode Sub Menu
  QMenu *modeSubMenu;

  // In Testing Sub Menu
  QMenu *inTestingSubMenu;

  // Help Menu
  QMenu *helpMenu;

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

#if QTB_UPDATECHECK
  // check update
  QAction *checkUpdate_Action;
#endif // QTB_UPDATECHECK

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

  // show software keyboard
  QAction *showSoftwareKeyboard_Action;

  // show software button
  QAction *showSoftwareButton_Action;

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

#if QTB_PUBLIC_MODE6_SUPPORT
  // send clipboard
  QAction *sendClipboard_Action;

  // send file
  QAction *sendFile_Action;

  // cancel file transferring
  QAction *cancelFileTransferring_Action;
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // connect to server dialog
  ConnectToServerDialog *connectToServerDialog;

  // desktop scling dialog
  DesktopScalingDialog *desktopScalingDialog;

  // log view dialog
  LogViewDialog *logViewDialog;

  // software keyboard and button
  SK *softwareKeyboard;
  SB *softwareButton;
#if 0 // for TEST
  QDockWidget *softwareKeyboardDockWidget;
  QDockWidget *softwareButtonDockWidget;
#endif // for TEST

private:
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

private:
  // for logging
  QDateTime bootTime;

  // total frame counter
  unsigned int totalFrameCounter;

  // current frame rate
  double currentFrameRate;

  // current data rate
  double currentDataRate;

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

#if QTB_UPDATECHECK
  // http getter
  HttpGetter *httpGetter;

private slots:
  // finished download
  void finishedDownload();

private:
#endif // QTB_UPDATECHECK

  // current path
  QString currentPath;

  // log message
  LogMessage *logMessage;

  // thread for control
  ControlThread *controlThread;

  // thread for graphics
  GraphicsThread *graphicsThread;

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

#if QTB_PUBLIC_MODE6_SUPPORT
  // clipboard
  QClipboard *clipboard;
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // full screen
  bool fullScreenMode;
  bool onSetDesktopScalingFactorForFullScreen;

  // current status
  bool onShowMenuBar;
  bool onShowStatusBar;

#if QTB_PUBLIC_MODE6_SUPPORT
  // progress bar
  QProgressBar	*progressBar;
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if 0 // for TEST
  // height of title bar
  int heightOfTitleBar;
#endif // for TEST

  // height of menu bar in hiding
  int heightOfMenuBarInHiding;

  // height of status bar in hiding
  int heightOfStatusBarInHiding;

  // height of menu bar
  int heightOfMenuBar;

  // height of status bar
  int heightOfStatusBar;

  // palettes
  QPalette originalPalette;
  QPalette fullScreenPalette;

  // current onControl/onGraphics/onSound
  bool onControl;
  bool onGraphics;
  bool onSound;

  // timer for main thread
  QTimer *timer;

private slots:
  void timerExpired();

private:
  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // QTBRYNHILDR_H
