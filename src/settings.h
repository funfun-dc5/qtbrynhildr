// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// settings.h

#ifndef SETTINGS_H
#define SETTINGS_H
// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Qt Header
#include <QtCore>
#include <QSettings>
#include <QString>

// Local Header
#if QTB_CRYPTOGRAM
#include "cipher.h"
#endif // QTB_CRYPTGRAM
#include "common/protocols.h"
#include "desktop.h"

// for Settings
// author
#define QTB_AUTHOR				"FunFun"

// organization
#define QTB_ORGANIZATION		"mcz-xoxo"

// application
#define QTB_APPLICATION			"Qt Brynhildr"

// translation file path
#define QTB_TRANSLATION_FILE_PATH	":/translations/"

// translation file prefix
#define QTB_TRANSLATION_FILE_PREFIX	"qtbrynhildr_"

// -- settings --
// for serverName
#define QTB_SERVERNAME			"serverName"
#if defined(QTB_DEV_TABLET) // for TEST
#define QTB_SERVERNAME_DEFAULT	"192.168.10.5"
#else // defined(QTB_DEV_TABLET)
#define QTB_SERVERNAME_DEFAULT	"mcz.world.idol.jp"
#endif // defined(QTB_DEV_TABLET)

// for serverType
#define QTB_SERVERTYPE			"serverType"
#define QTB_SERVERTYPE_DEFAULT	SERVER_TYPE_WINDOWS_10
typedef int SERVER_TYPE;
#define	SERVER_TYPE_WINDOWS_XP		0
#define	SERVER_TYPE_WINDOWS_VISTA	1
#define	SERVER_TYPE_WINDOWS_7		2
#define	SERVER_TYPE_WINDOWS_8		3
#define	SERVER_TYPE_WINDOWS_8_1		4
#define	SERVER_TYPE_WINDOWS_10		5
#define	SERVER_TYPE_NUM				6

#define	STRING_SERVER_TYPE_WINDOWS_XP		"Windows XP"
#define	STRING_SERVER_TYPE_WINDOWS_VISTA	"Windows Vista"
#define	STRING_SERVER_TYPE_WINDOWS_7		"Windows 7"
#define	STRING_SERVER_TYPE_WINDOWS_8		"Windows 8"
#define	STRING_SERVER_TYPE_WINDOWS_8_1		"Windows 8.1"
#define	STRING_SERVER_TYPE_WINDOWS_10		"Windows 10"

// for keyboardType
#define QTB_KEYBOARDTYPE		"keyboardType"
typedef int KEYBOARD_TYPE;
#define KEYBOARD_TYPE_JP		0
#define KEYBOARD_TYPE_US		1
#define KEYBOARD_TYPE_NUM		2

#define STRING_KEYBOARD_TYPE_JP		"JP Keyboard"
#define STRING_KEYBOARD_TYPE_US		"US Keyboard"

// for portNo
#define QTB_PORTNO				"portNo"
#define QTB_PORTNO_DEFAULT		55500

// for password
#define QTB_PASSWORD			"password"
#define QTB_PASSWORD_DEFAULT	"********"

// for onControl
#define QTB_ONCONTROL			"onControl"
#define QTB_ONCONTROL_DEFAULT	true

// for onHoldMouseControl
#define QTB_ONHOLDMOUSECONTROL			"onHoldMouseControl"
#define QTB_ONHOLDMOUSECONTROL_DEFAULT	false

// for onControlOffWithGraphicsOff
#define QTB_ONCONTROLOFFWITHGRAPHICSOFF			"onControlOffWithGraphicsOff"
#define QTB_ONCONTROLOFFWITHGRAPHICSOFF_DEFAULT	true

// for onGraphics
#define QTB_ONGRAPHICS			"onGraphics"
#define QTB_ONGRAPHICS_DEFAULT	true

// for videoQuality
#define QTB_VIDEOQUALITY			"videoQuality"
#if defined(QTB_DEV_TABLET)
#define QTB_VIDEOQUALITY_DEFAULT	VIDEO_QUALITY_MINIMUM
#else // defined(QTB_DEV_TABLET)
#define QTB_VIDEOQUALITY_DEFAULT	VIDEO_QUALITY_STANDARD
#endif // defined(QTB_DEV_TABLET)

// for frameRate
#define QTB_FRAMERATE			"frameRate"
#define FRAMERATE_MAXIMUM		0
#if defined(QTB_DEV_TABLET)
#define QTB_FRAMERATE_DEFAULT	FRAMERATE_MAXIMUM
#else // defined(QTB_DEV_TABLET)
#define QTB_FRAMERATE_DEFAULT	30
#endif // defined(QTB_DEV_TABLET)

#if QTB_BRYNHILDR2_SUPPORT
// for displayCursor
#if defined(QTB_DEV_TABLET)
#define QTB_ONDISPLAYCURSOR_DEFAULT		true
#else // defined(QTB_DEV_TABLET)
#define QTB_ONDISPLAYCURSOR_DEFAULT		false
#endif // defined(QTB_DEV_TABLET)
// for onSupportGamePad
#define QTB_ONSUPPORTGAMEPAD			"onSupportGamePad"
#if defined(Q_OS_WIN)
#define QTB_ONSUPPORTGAMEPAD_DEFAULT	true
#else // defined(Q_OS_WIN)
#define QTB_ONSUPPORTGAMEPAD_DEFAULT	false
#endif // defined(Q_OS_WIN)
// for onDisableBrynhildr2Support
#define QTB_ONDISABLEBRYNHILDR2SUPPORT			"onDisableBrynhildr2Support"
#define QTB_ONDISABLEBRYNHILDR2SUPPORT_DEFAULT	false
#endif // QTB_BRYNHILDR2_SUPPORT

// for onSound
#define QTB_ONSOUND				"onSound"
#define QTB_ONSOUND_DEFAULT		true

// for soundQuality
#define QTB_SOUNDQUALITY			"soundQuality"
#if defined(QTB_DEV_TABLET)
#define QTB_SOUNDQUALITY_DEFAULT	SOUND_QUALITY_MINIMUM
#else // defined(QTB_DEV_TABLET)
#define QTB_SOUNDQUALITY_DEFAULT	SOUND_QUALITY_STANDARD
#endif // defined(QTB_DEV_TABLET)

// for soundCapture
#define QTB_SOUNDCAPTURE			"soundCapture"
#define QTB_SOUNDCAPTURE_DEFAULT	SOUND_CAPTURE_CORE_AUDIO

// for onKeepOriginalDesktopSize
#define QTB_ONKEEPORIGINALDESKTOPSIZE "onKeepOriginalDesktopSize"
#define QTB_ONKEEPORIGINALDESKTOPSIZE_DEFAULT true

// for desktopScalingFactor
#define QTB_DESKTOPSCALINGFACTOR	"desktopScalingFactor"
#define QTB_DESKTOPSCALINGFACTOR_DEFAULT 1.0

// for desktopScaringQuality
#define QTB_DESKTOPSCALINGQUALITY			"desktopScalingQuality"
#if defined(QTB_DEV_TABLET)
#define QTB_DESKTOPSCALINGQUALITY_DEFAULT	DESKTOPSCALINGQUALITY_FAST
#else // defined(QTB_DEV_TABLET)
#define QTB_DESKTOPSCALINGQUALITY_DEFAULT	DESKTOPSCALINGQUALITY_SMOOTH
#endif // defined(QTB_DEV_TABLET)
#define DESKTOPSCALINGQUALITY_SMOOTH		Qt::SmoothTransformation
#define DESKTOPSCALINGQUALITY_FAST			Qt::FastTransformation

// for scalingType
typedef int SCALING_TYPE;
#define QTB_DESKTOPSCALINGTYPE			"desktopScalingType"
#define QTB_DESKTOPSCALINGTYPE_DEFAULT	DESKTOPSCALING_TYPE_ON_CLIENT
#define DESKTOPSCALING_TYPE_ON_CLIENT	0
#define DESKTOPSCALING_TYPE_ON_SERVER	1

// for onCutDesktopBlankArea
#define QTB_ONCUTDESKTOPBLANKAREA				"onCutDesktopBlankArea"
#define QTB_ONCUTDESKTOPBLANKAREA_DEFAULT		false

// for desktopOffsetX/Y
#define QTB_DESKTOPOFFSETX				"desktopOffsetX"
#define QTB_DESKTOPOFFSETX_DEFAULT		0
#define QTB_DESKTOPOFFSETY				"desktopOffsetY"
#define QTB_DESKTOPOFFSETY_DEFAULT		0

// MAX desktop width for server
#define QTB_MAX_SERVER_DESKTOP_WIDTH	4096
// MAX desktop height for server
#define QTB_MAX_SERVER_DESKTOP_HIGHT	2048

// for monitorNo
#define QTB_MONITOR_NO					"monitorNo"
#define QTB_MONITOR_NO_DEFAULT			1
#define MONITOR_NO_MAX					9
#define MONITOR_NO_ALL					99

// for onConfirmAtExit
#define QTB_ONCONFIRMATEXIT				"onConfirmAtExit"
#define QTB_ONCONFIRMATEXIT_DEFAULT		true

// for onSaveSettingsAtExit
#define QTB_ONSAVESETTINGSATEXIT			"onSaveSettingsAtExit"
#define QTB_ONSAVESETTINGSATEXIT_DEFAULT	false

// for onExitAfterRelpay
#define QTB_ONEXITAFTERREPLAY				"onExitAfterReplay"
#define QTB_ONEXITAFTERREPLAY_DEFAULT		false

// for onStaysOnTop
#define QTB_ONSTAYSONTOP					"onStaysOnTop"
#define QTB_ONSTAYSONTOP_DEFAULT			false

// for onFrameLessWindow
#define QTB_ONFRAMELESSWINDOW				"onFrameLessWindow"
#define QTB_ONFRAMELESSWINDOW_DEFAULT		false

// for onShowMenuBar
#define QTB_ONSHOWMENUBAR					"onShowMenuBar"
#if defined(QTB_DEV_TABLET)
#define QTB_ONSHOWMENUBAR_DEFAULT			false
#else // defined(QTB_DEV_TABLET)
#define QTB_ONSHOWMENUBAR_DEFAULT			true
#endif // defined(QTB_DEV_TABLET)

// for onShowStatusBar
#define QTB_ONSHOWSTATUSBAR					"onShowStatusBar"
#define QTB_ONSHOWSTATUSBAR_DEFAULT			true

// for onShowFrameRate
#define QTB_ONSHOWFRAMERATE					"onShowFrameRate"
#define QTB_ONSHOWFRAMERATE_DEFAULT			true

// for onShowPassword
#define QTB_ONSHOWPASSWORD					"onShowPassword"
#define QTB_ONSHOWPASSWORD_DEFAULT			false

#if QTB_SUPPORT_PUBLIC_MODE6

// for onDisableClipboard
#define QTB_ONDISABLECLIPBOARD				"onDisableClipboard"
#define QTB_ONDISABLECLIPBOARD_DEFAULT		false

#endif // QTB_SUPPORT_PUBLIC_MODE6

// for graphicsBufferSize
#define QTB_GRAPHICSBUFFERSIZE				"graphicsBufferSize"
#define QTB_GRAPHICSBUFFERSIZE_DEFAULT		(1024 * 1024)

// for soundBufferSize
#define QTB_SOUNDBUFFERSIZE					"soundBufferSize"
#define QTB_SOUNDBUFFERSIZE_DEFAULT			(1024 * 1024)

// for doubleClickThreshold
#define QTB_DOUBLECLICKTHRESHOLD			"doubleClickThreshold"
#define QTB_DOUBLECLICKTHRESHOLD_DEFAULT	500

// for outputGraphicsDataToFile
#define QTB_OUTPUTGRAPHICSDATATOFILE			"outputGraphicsDataToFile"
#define QTB_OUTPUTGRAPHICSDATATOFILE_DEFAULT	false

// for outputSoundDataToFile
#define QTB_OUTPUTSOUNDDATATOFILE				"outputSoundDataToFile"
#define QTB_OUTPUTSOUNDDATATOFILE_DEFAULT		false

// for outputSoundDataToWavFile
#define QTB_OUTPUTSOUNDDATATOWAVFILE			"outputSoundDataToWavFile"
#define QTB_OUTPUTSOUNDDATATOWAVFILE_DEFAULT	false

// for outputLog
#define QTB_OUTPUTLOG							"outputLog"
#define QTB_OUTPUTLOG_DEFAULT					false

// for logFile
#define QTB_LOGFILE								"logFile"

// -- other settings --
// for geometry
#define QTB_GEOMETRY			"geometry"

namespace qtbrynhildr {

// Settings
class Settings
{
public:
  // constructor
#if QTB_CRYPTOGRAM
  Settings(const char *iniFileName, Cipher *cipher);
#else // QTB_CRYPTGRAM
  Settings();
  Settings(const char *iniFileName);
#endif // QTB_CRYPTGRAM
  // desctructor
  ~Settings();

  // settings
  QSettings *getSettings() const;

  // read settings
  void readSettings();

  // write settings
  void writeSettings();

  // analyze command line options
  bool analyzeOptions(int argc, char *argv[]);

  // output for log
  void printSettings() const;

  //---------------------------------------------------------------------------
  // getter & setter
  //---------------------------------------------------------------------------
  // get desktop information
  Desktop *getDesktop() const
  {
	return desktop;
  }

  // get server name
  QString getServerName() const
  {
	return serverName;
  }

  // set server name
  bool setServerName(QString serverName)
  {
	this->serverName = serverName;
	return true;
  }

  // get server type
  SERVER_TYPE getServerType() const
  {
	return serverType;
  }

  // set server type
  void setServerType(SERVER_TYPE serverType)
  {
	ASSERT(serverType >= SERVER_TYPE_WINDOWS_XP && serverType < SERVER_TYPE_NUM);
	this->serverType = serverType;

	switch(serverType){
	case SERVER_TYPE_WINDOWS_XP:
	  setSoundCapture(SOUND_CAPTURE_DIRECT_SOUND);
	  break;
	case SERVER_TYPE_WINDOWS_VISTA:
	case SERVER_TYPE_WINDOWS_7:
	case SERVER_TYPE_WINDOWS_8:
	case SERVER_TYPE_WINDOWS_8_1:
	case SERVER_TYPE_WINDOWS_10:
	  setSoundCapture(SOUND_CAPTURE_CORE_AUDIO);
	  break;
	default:
	  // Error
	  ABORT();
	  break;
	}
  }

  // set server type
  void setServerType(const char *hostType)
  {
	const char* hostTypeTable[SERVER_TYPE_NUM] = {
	  "xp",
	  "vista",
	  "7",
	  "8",
	  "8.1",
	  "10"
	};
	for(int i = 0; i < SERVER_TYPE_NUM; i++){
	  if (strncmp(hostTypeTable[i], hostType, strlen(hostType)) == 0){
		setServerType(i);
		return;
	  }
	}

	// Nothing to do
  }

  // get server type string
  QString getServerTypeByString() const
  {
	static const QString stringTable[SERVER_TYPE_NUM] = {
	  "SERVER_TYPE_WINDOWS_XP",
	  "SERVER_TYPE_WINDOWS_VISTA",
	  "SERVER_TYPE_WINDOWS_7",
	  "SERVER_TYPE_WINDOWS_8",
	  "SERVER_TYPE_WINDOWS_8.1",
	  "SERVER_TYPE_WINDOWS_10",
	};

	return stringTable[serverType];
  }

  // get keyboard type
  KEYBOARD_TYPE getKeyboardType() const
  {
	return keyboardType;
  }

  // set keyboard type
  void setKeyboardType(KEYBOARD_TYPE keyboardType)
  {
	ASSERT(keyboardType >= KEYBOARD_TYPE_JP && keyboardType < KEYBOARD_TYPE_NUM);
	this->keyboardType = keyboardType;
  }

  // get keyboard type string
  QString getKeyboardTypeByString() const
  {
	static const QString stringTable[KEYBOARD_TYPE_NUM] = {
	  "KEYBOARD_TYPE_WINDOWS_JP",
	  "KEYBOARD_TYPE_WINDOWS_US",
	};

	return stringTable[keyboardType];
  }

  // get port No.
  quint16 getPortNo() const
  {
	return portNo;
  }

  // set port No.
  bool setPortNo(quint16 portNo)
  {
	this->portNo = portNo;
	return true;
  }

  // get password
  QString getPassword()
  {
#if QTB_CRYPTOGRAM
	// decryption
	string strDecryptPassword = cipher->decodeString(password.toUtf8().constData()).c_str();
	QString decryptPassword = strDecryptPassword.c_str();
	return decryptPassword;
#else // QTB_CRYPTGRAM
	return password;
#endif // QTB_CRYPTGRAM
  }

  // set password
  bool setPassword(QString password)
  {
#if QTB_CRYPTOGRAM
	// encryption
	string strPassword = password.toUtf8().constData();
	QString encryptPassword = (cipher->encodeString(strPassword)).c_str();
	this->password = encryptPassword;
#else // QTB_CRYPTGRAM
	this->password = password;
#endif // QTB_CRYPTGRAM
	return true;
  }

  // get connect flag
  bool getConnected() const
  {
	return connected;
  }

  // set connect flag
  void setConnected(bool connected)
  {
	this->connected = connected;
  }

  // get control flag
  bool getOnControl() const
  {
	return onControl;
  }

  // set control flag
  bool setOnControl(bool onControl)
  {
	this->onControl = onControl;
	return true;
  }

  // get hold mouse control flag
  bool getOnHoldMouseControl() const
  {
	return onHoldMouseControl;
  }

  // set hold mouse control flag
  bool setOnHoldMouseControl(bool onHoldMouseControl)
  {
	this->onHoldMouseControl = onHoldMouseControl;
	return true;
  }

  // get control off with graphics off flag
  bool getOnControlOffWithGraphicsOff() const
  {
	return onControlOffWithGraphicsOff;
  }

  // set control off with graphics off flag
  bool setOnControlOffWithGraphicsOff(bool onControlOffWithGraphicsOff)
  {
	this->onControlOffWithGraphicsOff = onControlOffWithGraphicsOff;
	return true;
  }

  // get graphics flag
  bool getOnGraphics() const
  {
	return onGraphics;
  }

  // set graphics flag
  bool setOnGraphics(bool onGraphics)
  {
	this->onGraphics = onGraphics;
	return true;
  }

  // get sound flag
  bool getOnSound() const
  {
	return onSound;
  }

  // set sound flag
  bool setOnSound(bool onSound)
  {
	this->onSound = onSound;
	return true;
  }

  // get video quality
  VIDEO_QUALITY getVideoQuality() const
  {
	return videoQuality;
  }

  // set video quality
  void setVideoQuality(VIDEO_QUALITY videoQuality)
  {
	ASSERT(videoQuality >= VIDEO_QUALITY_MINIMUM && videoQuality <= VIDEO_QUALITY_MAXIMUM);
	this->videoQuality = videoQuality;
  }

  // get video quality string
  QString getVideoQualityByString() const
  {
	static const QString stringTable[VIDEO_QUALITY_MAXIMUM+1] = {
	  "VIDEO_QUALITY_0",
	  "VIDEO_QUALITY_MINIMUM",
	  "VIDEO_QUALITY_2",
	  "VIDEO_QUALITY_LOW",
	  "VIDEO_QUALITY_4",
	  "VIDEO_QUALITY_STANDARD",
	  "VIDEO_QUALITY_6",
	  "VIDEO_QUALITY_HIGH",
	  "VIDEO_QUALITY_8",
	  "VIDEO_QUALITY_MAXIMUM",
	};

	return stringTable[videoQuality];
  }

#if QTB_BRYNHILDR2_SUPPORT
  // get display cursor flag
  bool getOnDisplayCursor()
  {
	return onDisplayCursor;
  }

  // set display cursor flag
  void setOnDisplayCursor(bool onDisplayCursor)
  {
	this->onDisplayCursor = onDisplayCursor;
  }

  // get support gamepad flag
  bool getOnSupportGamePad()
  {
	return onSupportGamePad;
  }

  // set support gamepad flag
  void setOnSupportGamePad(bool onSupportGamePad)
  {
	this->onSupportGamePad = onSupportGamePad;
  }

  // get disable brynhildr2 flag
  bool getOnDisableBrynhildr2Support()
  {
	return onDisableBrynhildr2Support;
  }

  // set disable brynhildr2 flag
  void setOnDisableBrynhildr2Support(bool onDisableBrynhildr2Support)
  {
	this->onDisableBrynhildr2Support = onDisableBrynhildr2Support;
  }
#endif // QTB_BRYNHILDR2_SUPPORT

  // get frame rate (FPS)
  unsigned int getFrameRate()
  {
	return frameRate;
  }

  // set frame rate (FPS)
  void setFrameRate(unsigned int frameRate)
  {
	if (frameRate > 100){
	  return;
	}
	this->frameRate = frameRate;
	if (frameRate == 0){
	  frameInterval = 0;
	}
	else {
	  frameInterval = (unsigned long)1000000/frameRate; // micro second = 1000000 (1 second)
	}
  }

  // get frame interval (micro seconds)
  unsigned long getFrameInterval()
  {
	return frameInterval;
  }

  // get sound quality
  SOUND_QUALITY getSoundQuality() const
  {
	return soundQuality;
  }

  // set sound quality
  void setSoundQuality(SOUND_QUALITY soundQuality)
  {
	ASSERT(soundQuality >= SOUND_QUALITY_MINIMUM && soundQuality <= SOUND_QUALITY_MAXIMUM);
	this->soundQuality = soundQuality;
  }

  // get sound quality string
  QString getSoundQualityByString() const
  {
	static const QString stringTable[SOUND_QUALITY_MAXIMUM+1] = {
	  "SOUND_QUALITY_0",
	  "SOUND_QUALITY_MINIMUM",
	  "SOUND_QUALITY_LOW",
	  "SOUND_QUALITY_STANDARD",
	  "SOUND_QUALITY_HIGH",
	  "SOUND_QUALITY_MAXIMUM",
	};

	return stringTable[soundQuality];
  }

  // get sound capture
  SOUND_CAPTURE getSoundCapture() const
  {
	return soundCapture;
  }

private:
  // set sound capture
  void setSoundCapture(SOUND_CAPTURE soundCapture)
  {
	switch(soundCapture){
	case SOUND_CAPTURE_DIRECT_SOUND:
	case SOUND_CAPTURE_CORE_AUDIO:
	  this->soundCapture = soundCapture;
	  break;
	default:
	  // Error
	  ABORT();
	  break;
	}
  }

public:
  // get sound capture string
  QString getSoundCaptureByString() const
  {
	switch(soundCapture){
	case SOUND_CAPTURE_DIRECT_SOUND:
	  return "CAPTURE_DIRECT_SOUND";
	  break;
	case SOUND_CAPTURE_CORE_AUDIO:
	  return "CAPTURE_CORE_AUDIO";
	  break;
	default:
	  // Error
	  ABORT();
	  return "UNKNOWN_CAPTURE";
	  break;
	}
  }

  // get keep original desktop size flag
  bool getOnKeepOriginalDesktopSize() const
  {
	return onKeepOriginalDesktopSize;
  }

  // set keep original desktop size flag
  void setOnKeepOriginalDesktopSize(bool onKeepOriginalDesktopSize)
  {
	this->onKeepOriginalDesktopSize = onKeepOriginalDesktopSize;
  }

  // get desktop scaling factor
  qreal getDesktopScalingFactor() const
  {
	return desktopScalingFactor;
  }

  // get desktop scaling factor
  qreal getDesktopScalingFactorForZoom() const
  {
	return desktopScalingFactorForZoom;
  }

  // set desktop scaling factor
  void setDesktopScalingFactor(qreal desktopScalingFactor)
  {
	if (desktopScalingFactor == 0.0){
	  // ignore
	  return;
	}
	this->desktopScalingFactor = desktopScalingFactor;
	this->desktopScalingFactorForZoom = 1.0/desktopScalingFactor;
  }

  // get scaling quality
  Qt::TransformationMode getDesktopScaringQuality() const
  {
	return desktopScalingQuality;
  }

  // set scaling quality
  void setDesktopScalingQuality(Qt::TransformationMode desktopScalingQuality)
  {
	this->desktopScalingQuality = desktopScalingQuality;
  }

  // get scaling type
  SCALING_TYPE getDesktopScalingType() const
  {
	return desktopScalingType;
  }

  // set scaling type
  void setDesktopScalingType(SCALING_TYPE desktopScalingType)
  {
	switch(desktopScalingType){
	case DESKTOPSCALING_TYPE_ON_CLIENT:
	case DESKTOPSCALING_TYPE_ON_SERVER:
	  this->desktopScalingType = desktopScalingType;
	  break;
	default:
	  // Error
	  ABORT();
	  break;
	}
  }

  // get cut desktop blank area flag
  bool getOnCutDesktopBlankArea() const
  {
	return onCutDesktopBlankArea;
  }

  // set cut desktop blank area flag
  void setOnCutDesktopBlankArea(bool onCutDesktopBlankArea)
  {
	this->onCutDesktopBlankArea = onCutDesktopBlankArea;
  }

  // get desktop offset x
  POS getDesktopOffsetX() const
  {
	return desktopOffsetX;
  }

  // set desktop offset x
  bool setDesktopOffsetX(POS desktopOffsetX)
  {
	this->desktopOffsetX = desktopOffsetX;
	return true;
  }

  // get desktop offset y
  POS getDesktopOffsetY() const
  {
	return desktopOffsetY;
  }

  // set desktop offset y
  bool setDesktopOffsetY(POS desktopOffsetY)
  {
	this->desktopOffsetY = desktopOffsetY;
	return true;
  }

  // get desktop width
  SIZE getDesktopWidth() const
  {
	return desktopWidth;
  }

  // set desktop width
  bool setDesktopWidth(SIZE desktopWidth)
  {
	this->desktopWidth = desktopWidth;
	return true;
  }

  // get desktop height
  SIZE getDesktopHeight() const
  {
	return desktopHeight;
  }

  // set desktop height
  bool setDesktopHeight(SIZE desktopHeight)
  {
	this->desktopHeight = desktopHeight;
	return true;
  }

  // get monitor no
  MONITOR_NO getMonitorNo() const
  {
	return monitorNo;
  }

  // set monitor no
  void setMonitorNo(MONITOR_NO monitorNo)
  {
	switch((int)monitorNo){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case MONITOR_NO_ALL:
	  this->monitorNo = monitorNo;
	  break;
	default:
	  // Error
	  ABORT();
	  break;
	}
  }

  // get monitor count
  MONITOR_COUNT getMonitorCount() const
  {
	return monitorCount;
  }

  // set monitor count
  void setMonitorCount(MONITOR_COUNT monitorCount)
  {
	this->monitorCount = monitorCount;
  }

  // get confirm at exit flag
  bool getOnConfirmAtExit() const
  {
	return onConfirmAtExit;
  }

  // set confirm at exit flag
  void setOnConfirmAtExit(bool onConfirmAtExit)
  {
	this->onConfirmAtExit = onConfirmAtExit;
  }

  // get save settings at exit flag
  bool getOnSaveSettingsAtExit() const
  {
	return onSaveSettingsAtExit;
  }

  // set save settings at exit flag
  void setOnSaveSettingsAtExit(bool onSaveSettingsAtExit)
  {
	this->onSaveSettingsAtExit = onSaveSettingsAtExit;
  }

  // get exit after replaying flag
  bool getOnExitAfterReplay() const
  {
	return onExitAfterReplay;
  }

  // set exit after replaying flag
  void setOnExitAfterReplay(bool onExitAfterReplay)
  {
	this->onExitAfterReplay = onExitAfterReplay;
  }

  // get stays on top flag
  bool getOnStaysOnTop() const
  {
	return onStaysOnTop;
  }

  // set stays on top flag
  void setOnStaysOnTop(bool onStaysOnTop)
  {
	this->onStaysOnTop = onStaysOnTop;
  }

  // get frame less window flag
  bool getOnFrameLessWindow() const
  {
	return onFrameLessWindow;
  }

  // set frame less window flag
  void setOnFrameLessWindow(bool onFrameLessWindow)
  {
	this->onFrameLessWindow = onFrameLessWindow;
  }

  // get show menu bar flag
  bool getOnShowMenuBar() const
  {
	return onShowMenuBar;
  }

  // set show menu bar flag
  void setOnShowMenuBar(bool onShowMenuBar)
  {
	this->onShowMenuBar = onShowMenuBar;
  }

  // get show status bar flag
  bool getOnShowStatusBar() const
  {
	return onShowStatusBar;
  }

  // set show status bar flag
  void setOnShowStatusBar(bool onShowStatusBar)
  {
	this->onShowStatusBar = onShowStatusBar;
  }

  // get show frame rate flag
  bool getOnShowFrameRate() const
  {
	return onShowFrameRate;
  }

  // set show frame rate flag
  void setOnShowFrameRate(bool onShowFrameRate)
  {
	this->onShowFrameRate = onShowFrameRate;
  }

#if QTB_RECORDER
  // get on recording control flag
  bool getOnRecordingControl() const
  {
	return onRecordingControl;
  }

  // set on recording control flag
  void setOnRecordingControl(bool onRecordingControl)
  {
	this->onRecordingControl = onRecordingControl;
  }

  // get recording control filename
  const char* getRecordingControlFileName() const
  {
	return recordingControlFileName;
  }

  // set recording control filename
  void setRecordingControlFileName(const char *recordingControlFileName)
  {
	this->recordingControlFileName = recordingControlFileName;
  }

  // get on replaying control flag
  bool getOnReplayingControl() const
  {
	return onReplayingControl;
  }

  // set on replaying control flag
  void setOnReplayingControl(bool onReplayingControl)
  {
	this->onReplayingControl = onReplayingControl;
  }

  // get replaying control filename
  const char* getReplayingControlFileName() const
  {
	return replayingControlFileName;
  }

  // set replaying control filename
  void setReplayingControlFileName(const char *replayingControlFileName)
  {
	this->replayingControlFileName = replayingControlFileName;
  }
#endif // QTB_RECORDER

  // get on scroll mode flag
  bool getOnScrollMode() const
  {
	return onScrollMode;
  }

  // set on scroll mode flag
  void setOnScrollMode(bool onScrollMode)
  {
	this->onScrollMode = onScrollMode;
  }

  // get show password flag
  bool getOnShowPassword() const
  {
	return onShowPassword;
  }

  // set show password flag
  void setOnShowPassword(bool onShowPassword)
  {
	this->onShowPassword = onShowPassword;
  }

#if QTB_SUPPORT_PUBLIC_MODE6

  // get disable clilpboard flag
  bool getOnDisableClipboard() const
  {
	return onDisableClipboard;
  }

  // set disable clipboard flag
  void setOnDisableClipboard(bool onDisableClipboard)
  {
	this->onDisableClipboard = onDisableClipboard;
  }

#endif // QTB_SUPPORT_PUBLIC_MODE6

  // get on show Software Keyboard flag
  bool getOnShowSoftwareKeyboard() const
  {
	return onShowSoftwareKeyboard;
  }

  // set on show Software Keyboard flag
  void setOnShowSoftwareKeyboard(bool onShowSoftwareKeyboard)
  {
	this->onShowSoftwareKeyboard = onShowSoftwareKeyboard;
  }

  // get on show Software Button flag
  bool getOnShowSoftwareButton() const
  {
	return onShowSoftwareButton;
  }

  // set on show Software Button flag
  void setOnShowSoftwareButton(bool onShowSoftwareButton)
  {
	this->onShowSoftwareButton = onShowSoftwareButton;
  }

  // get graphics buffer size
  int getGraphicsBufferSize() const
  {
	return graphicsBufferSize;
  }

  // set graphics buffer size
  void setGraphicsBufferSize(int size)
  {
	graphicsBufferSize = size;
  }

  // get sound buffer size
  int getSoundBufferSize() const
  {
	return soundBufferSize;
  }

  // set sound buffer size
  void setSoundBufferSize(int size)
  {
	soundBufferSize = size;
  }

  // get double click threshold
  int getDoubleClickThreshold() const
  {
	return doubleClickThreshold;
  }

  // set double click threshold
  void setDoubleClickThreshold(int threshold)
  {
	doubleClickThreshold = threshold;
  }

  // get output graphics flag
  bool getOutputGraphicsDataToFile() const
  {
	return outputGraphicsDataToFile;
  }

  // set output graphics flag
  bool setOutputGraphicsDataToFile(bool outputGraphicsDataToFile)
  {
	this->outputGraphicsDataToFile = outputGraphicsDataToFile;
	return outputGraphicsDataToFile;
  }

  // get output sound flag
  bool getOutputSoundDataToFile() const
  {
	return outputSoundDataToFile;
  }

  // set output sound flag
  bool setOutputSoundDataToFile(bool outputSoundDataToFile)
  {
	this->outputSoundDataToFile = outputSoundDataToFile;
	return outputSoundDataToFile;
  }

  // get output sound flag
  bool getOutputSoundDataToWavFile() const
  {
	return outputSoundDataToWavFile;
  }

  // set output sound flag
  bool setOutputSoundDataToWavFile(bool outputSoundDataToWavFile)
  {
	this->outputSoundDataToWavFile = outputSoundDataToWavFile;
	return outputSoundDataToWavFile;
  }

  // get output log flag
  bool getOutputLog() const
  {
	return outputLog;
  }

  // set output log flag
  bool setOutputLog(bool outputLog)
  {
	this->outputLog = outputLog;
	return outputLog;
  }

  // get log file
  QString getLogFile() const
  {
	return logFile;
  }

  // set log file
  void setLogFile(QString logFile)
  {
	this->logFile = logFile;
  }

private:
  // get default keyboard type
  KEYBOARD_TYPE getDefaultKeyboardType() const;

  // get Default Log File
  QString getDefaultLogFile() const;

private:
  // settings
  QSettings *settings;

  // desktop information
  Desktop *desktop;

  // bootup flag
  volatile bool bootupFlag;

  // shutdown flag
  volatile bool shutdownFlag;

#if QTB_CRYPTOGRAM
  // cipher
  Cipher *cipher;
#endif // QTB_CRYPTGRAM

  // server name
  QString serverName;

  // server type
  volatile SERVER_TYPE serverType;

  // keyboard type
  volatile KEYBOARD_TYPE keyboardType;

  // port
  volatile quint16 portNo;

  // password
  QString password;

  // connected
  volatile bool connected;

  // Control
  volatile bool onControl;
  volatile bool onHoldMouseControl;
  volatile bool onControlOffWithGraphicsOff;

  // Graphics
  volatile bool onGraphics;
  volatile VIDEO_QUALITY videoQuality;

#if QTB_BRYNHILDR2_SUPPORT
  volatile bool onDisplayCursor;
  volatile bool onSupportGamePad;
  volatile bool onDisableBrynhildr2Support;
#endif // QTB_BRYNHILDR2_SUPPORT
  volatile unsigned int frameRate;
  volatile unsigned long frameInterval;

  // Sound
  volatile bool onSound;
  volatile SOUND_QUALITY soundQuality;
  volatile SOUND_CAPTURE soundCapture;

  // keep original desktop size flag
  volatile bool onKeepOriginalDesktopSize;

  // scaling factor
  volatile qreal desktopScalingFactor;
  volatile qreal desktopScalingFactorForZoom;

  // scaling quality
  volatile Qt::TransformationMode desktopScalingQuality;

  // scaling type
  volatile SCALING_TYPE desktopScalingType;

  // cut desktop blank area flag
  volatile bool onCutDesktopBlankArea;

  // desktop offset x
  volatile POS desktopOffsetX;

  // desktop offset y
  volatile POS desktopOffsetY;

  // desktop width
  volatile SIZE desktopWidth;

  // desktop height
  volatile SIZE desktopHeight;

  // select monitor
  volatile MONITOR_NO monitorNo;

  // monitor count
  volatile MONITOR_COUNT monitorCount;

  // confirm at exit
  volatile bool onConfirmAtExit;

  // save settings at exit
  volatile bool onSaveSettingsAtExit;

  // exit after replaying
  volatile bool onExitAfterReplay;

  // stays on top
  volatile bool onStaysOnTop;

  // frame less window
  volatile bool onFrameLessWindow;

  // show menu bar
  volatile bool onShowMenuBar;

  // show status bar
  volatile bool onShowStatusBar;

  // show frame rate
  volatile bool onShowFrameRate;

  // software keyboard
  volatile bool onShowSoftwareKeyboard;

  // software button
  volatile bool onShowSoftwareButton;

#if QTB_RECORDER
  // recording
  volatile bool onRecordingControl;
  const char *recordingControlFileName;

  // reply
  volatile bool onReplayingControl;
  const char *replayingControlFileName;
#endif // QTB_RECORDER

  // scroll mode
  volatile bool onScrollMode;

  // show password
  volatile bool onShowPassword;

#if QTB_SUPPORT_PUBLIC_MODE6

  // disable clip board
  volatile bool onDisableClipboard;

#endif // QTB_SUPPORT_PUBLIC_MODE6

  // buffer
  volatile int graphicsBufferSize;
  volatile int soundBufferSize;

  // threshold
  volatile int doubleClickThreshold;

  // Debug
  volatile bool outputGraphicsDataToFile;
  volatile bool outputSoundDataToFile;
  volatile bool outputSoundDataToWavFile;
  volatile bool outputLog;

  // for logFile
  QString logFile;
};

} // end of namespace qtbrynhildr

// for GUI
#define QTB_ICON_FILENAME			":/images/qtbrynhildr48.ico"
#define QTB_SPLASH_SCREEN_FILENAME	":/images/3Bjunior.jpg"

// for log file
#define QTB_LOG_FILENAME	"qtbrynhildr.log"

// date format for log
#define QTB_LOG_DATE_FORMAT "yyyy.MM.dd HH:mm:ss.zzz"

// for DEBUG
#define QTB_GRAPHICS_OUTPUT_FILENAME_PREFIX	"graphics_output"
#define QTB_SOUND_OUTPUT_FILENAME			"sound_output.pcm"
#define QTB_SOUND_OUTPUT_WAV_FILENAME		"sound_output.wav"

#endif // SETTINGS_H
