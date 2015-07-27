// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// qtbrynhildr.h

#ifndef QTBRYNHILDR_H
#define QTBRYNHILDR_H
// Common Header
#include "common.h"

// Qt Header
#include <QAction>
#include <QDateTime>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QObject>
#include <QScrollArea>
#include <QSize>
#include <QString>
#include <QToolBar>
#include <QtGlobal>

// Local Header
#if QTB_CRYPTOGRAM
#include "cipher.h"
#endif // QTB_CRYPTGRAM
#include "connect_to_server_dialog.h"
#include "desktop_scaling_dialog.h"
#include "logmessage.h"
#include "mainwindow.h"
#include "option.h"
#include "settings.h"
#include "software_button.h"
#include "software_keyboard.h"
#include "software_keyboard_jp.h"
#if 1 // for TEST
#include "software_button_dialog.h"
#include "software_keyboard_dialog.h"
#endif

// thread
#include "controlthread.h"
#include "graphicsthread.h"
#include "soundthread.h"

// event converter
#include "windows/eventconverter_jp.h"
#include "windows/eventconverter_us.h"

namespace qtbrynhildr {

// ----------------------------------------------
// global objects
// ----------------------------------------------
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
// socket for control
extern SOCKET sock_control;
// socket for graphics
extern SOCKET sock_graphics;
// socket for sound
extern SOCKET sock_sound;
#endif // defined(Q_OS_WIN) || defined(Q_OS_LINUX)

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
  QtBrynhildr(int argc, char *argv[]);
  // destructor
  ~QtBrynhildr();

  // get main window
  MainWindow *getMainWindow() const;

  // shutdown flag
  bool getShutdownFlag() const;

public slots:
  // refresh window
  void refreshWindow();

  // set monitor count
  void setMonitorCount(int monitorCount);

  // desktop Changed
  void onDesktopChanged(QImage image);

  // Desktop clear
  void onDesktopClear();

  // network error handler
  void onNetworkError();

  // output Log Message
  void outputLogMessage(int msgID);

  // output Log Message
  void outputLogMessage(int id, const QString text);

protected:
  // window close event
  void closeEvent(QCloseEvent *event);

private slots:
  // popup connect to server dialog
  void popUpConnectToServer();

  // connect to server
  void connectToServer();

  // disconnect to server
  void disconnectToServer();

  // finished thread
  void finishedNetThread();

  // toggle output log mode
  void toggleOutputLog();

  // about dialog
  void about();

  // exit from QtBynhildr
  void exit();

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

  // toggle onControl/onGraphics/onSound
  void toggleOnControl();
  void toggleOnGraphics();
  void toggleOnSound();

  // send key
#if 0 // for TEST
  void sendKey_CTRL_ALT_DEL(); // CTRL + ALT + DEL
#endif
  void sendKey_ALT_F4(); // ALT + F4
  void sendKey_CTRL_ESC(); // CTRL + ESC
  void sendKey_WINDOWS(); // WINDOWS

  // toggle show menu bar
  void toggleShowMenuBar();

  // toggle show status bar
  void toggleShowStatusBar();

  // full screen
  void fullScreen();

  // toggle stays on top
  void toggleStaysOnTop();

  // desktop scaling
  void desktopScaling();

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

#if 1 // for TEST
  // software keyboard
  void toggleShowSoftwareKeyboard();

  // software button
  void toggleShowSoftwareButton();
#endif

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

  // connected
  void connected();

  // disconnected
  void disconnected();

private:
  // GUI
  // scroll area
  QScrollArea *scrollArea;
  // main window
  MainWindow *mainWindow;
  // window size
  QSize mainWindowSize;

  // location label
  QLabel *locationLabel;
  // formula label
  QLabel *formulaLabel;

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

  // Control Menu
  QMenu *controlMenu;

  // Send Key Sub Menu
  QMenu *sendKeySubMenu;

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

  // output log
  QAction *outputLog_Action;

  // exit application
  QAction *exit_Action;

  // about this application
  QAction *about_Action;

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

  // stays on top
  QAction *staysOnTop_Action;

  // full screen
  QAction *fullScreen_Action;

  // desktop scaling
  QAction *desktopScalingDialog_Action;

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

  // toggle onControl
  QAction *onControl_Action;

  // toggle onGraphics
  QAction *onGraphics_Action;

  // toggle onSound
  QAction *onSound_Action;

  // send key
  QAction *sendKey1_Action;
  QAction *sendKey2_Action;
  QAction *sendKey3_Action;
  QAction *sendKey4_Action;

  // connect to server dialog
  ConnectToServerDialog *connectToServerDialog;

  // desktop scling dialog
  DesktopScalingDialog *desktopScalingDialog;

  // toggle scroll mode
  QAction *onScrollMode_Action;

  // software keyboard
  SoftwareKeyboard *softwareKeyboard;
  SoftwareButton *softwareButton;
#if 1 // for TEST
  SoftwareKeyboardDialog *softwareKeyboardDialog;
  SoftwareButtonDialog *softwareButtonDialog;
  QAction *showSoftwareKeyboard_Action;
  QAction *showSoftwareButton_Action;
#endif

private:
  // clear Video Quality check
  void clearVideoQualityCheck();

  // clear Sound Quality check
  void clearSoundQualityCheck();

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

  // frameCounter
  unsigned int frameCounter;

  // option
  Option *option;

  // ini filename
  const char *iniFileName;

  // setting
  Settings *settings;

#if QTB_CRYPTOGRAM
  // cipher
  Cipher *cipher;
#endif // QTB_CRYPTGRAM

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

  // event converter
  EventConverter *eventConverter;

  // monitor count
  MONITOR_COUNT monitorCount;

  // full screen
  bool fullScreenMode;

  // height of menu bar
  int heightOfMenuBar;

  // height of status bar
  int heightOfStatusBar;
};

} // end of namespace qtbrynhildr

#endif // QTBRYNHILDR_H
