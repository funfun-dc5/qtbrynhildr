// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef SETTINGS_H
#define SETTINGS_H
// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Qt Header
#include <QDir>
#include <QSettings>
#include <QString>
#if QTB_AUTO_COMPLETE
#include <QStringList>
#endif // QTB_AUTO_COMPLETE

// Local Header
#if QTB_CRYPTOGRAM
#include "cipher.h"
#endif // QTB_CRYPTGRAM
#include "common/protocols.h"
#include "parameters.h"
#include "util/desktop.h"
#include "version.h"

// for Settings
// author
#define QTB_AUTHOR				"FunFun"

// organization
#define QTB_ORGANIZATION		"mcz-xoxo"

// application
#if !_WIN64
#define QTB_APPLICATION			"Qt Brynhildr"
#else // !_WIN64
#define QTB_APPLICATION			"Qt Brynhildr 64"
#endif // !_WIN64

// translation file path
#define QTB_TRANSLATION_FILE_PATH	":/translations/"

// translation file prefix
#define QTB_TRANSLATION_FILE_PREFIX	"qtbrynhildr_"

// desktop background color
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_DESKTOP_BACKGROUND_COLOR Qt::black
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_DESKTOP_BACKGROUND_COLOR Qt::gray
#endif // defined(QTB_DEV_TOUCHPANEL)

// key layout file directory
#define QTB_KEYLAYOUT_FILE_PATH		"/keylayout"
#define QTB_KEYLAYOUT_FILE_SUFFIX	".kl"
#define QTB_KEYLAYOUT_FILE_SUFFIXX	".klx"

// -- settings --
// for version information
#define QTB_GENERATEDVERSION			"generatedVersion"
#define QTB_GENERATEDVERSION_DEFAULT	QTB_VERSION_NUMBER
#define QTB_CURRENTVERSION				"currentVersion"
#define QTB_CURRENTVERSION_DEFAULT		QTB_VERSION_NUMBER

#define QTB_INFOKERNELTYPE				"infoKernelType"
#define QTB_INFOKERNELVERSION			"infoKernelVersion"
#define QTB_INFOPRODUCTTYPE				"infoProductType"
#define QTB_INFOPRODUCTVERSION			"infoProductVersion"
#define QTB_INFOBUILDABI				"infoBuildAbi"

#if QTB_PUBLIC_MODE6_SUPPORT
// for publicModeVersion
#define QTB_PUBLICMODEVERSION	"publicModeVersion"
#if QTB_PUBLIC_MODE7_SUPPORT
#define QTB_PUBLICMODEVERSION_DEFAULT	PUBLICMODE_VERSION7
#else // QTB_PUBLIC_MODE7_SUPPORT
#define QTB_PUBLICMODEVERSION_DEFAULT	PUBLICMODE_VERSION6
#endif // QTB_PUBLIC_MODE7_SUPPORT
typedef int PUBLICMODE_VERSION;
#define PUBLICMODE_VERSION5		5
#define PUBLICMODE_VERSION6		6
#define PUBLICMODE_VERSION7		7
#endif // QTB_PUBLIC_MODE6_SUPPORT

// for serverName
#define QTB_SERVERNAME			"serverName"
#if defined(QTB_DEV_TOUCHPANEL) // for TEST
#define QTB_SERVERNAME_DEFAULT	"192.168.10.3"
//#define QTB_SERVERNAME_DEFAULT	""
#else // defined(QTB_DEV_TOUCHPANEL)
//#define QTB_SERVERNAME_DEFAULT	"mcz.world.idol.jp"
#define QTB_SERVERNAME_DEFAULT	""
#endif // defined(QTB_DEV_TOUCHPANEL)

// for serverNameListSize
#define QTB_SERVERNAMELISTSIZE			"serverNameListSize"
#define QTB_SERVERNAMELISTSIZE_DEFAULT	10

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
#define KEYBOARD_TYPE_NATIVE	2
#define KEYBOARD_TYPE_KLF		3
#define KEYBOARD_TYPE_NUM		4

#define STRING_KEYBOARD_TYPE_JP		"JP Keyboard"
#define STRING_KEYBOARD_TYPE_US		"US Keyboard"
#define STRING_KEYBOARD_TYPE_NATIVE	"Native Keyboard"

#define QTB_KEYBOARDTYPENAME			"keyboardTypeName"
#define QTB_KEYBOARDTYPENAME_DEFAULT	"Unknown"

// for portNo
#define QTB_PORTNO				"portNo"
#define QTB_PORTNO_DEFAULT		55500

// for password
#define QTB_PASSWORD			"password"
#define QTB_PASSWORD_DEFAULT	"********"

// for onControl
#define QTB_ONCONTROL			"onControl"
#define QTB_ONCONTROL_DEFAULT	true

#if QTB_PLUGINS_DISABLE_SUPPORT
// for onPluginsDisable
#define QTB_ONPLUGINSDISABLE			"onPluginsDisable"
#define QTB_ONPLUGINSDISABLE_DEFAULT	false
#endif // QTB_PLUGINS_DISABLE_SUPPORT

// for onHoldMouseControl
#define QTB_ONHOLDMOUSECONTROL			"onHoldMouseControl"
#define QTB_ONHOLDMOUSECONTROL_DEFAULT	false

#if QTB_EXTRA_BUTTON_SUPPORT
// for onExtraButtonSupport
#define QTB_ONEXTRABUTTONSUPPORT			"onExtraButtonSupport"
#define QTB_ONEXTRABUTTONSUPPORT_DEFAULT	true
#endif // QTB_EXTRA_BUTTON_SUPPORT

// for onControlOffWithGraphicsOff
#define QTB_ONCONTROLOFFWITHGRAPHICSOFF			"onControlOffWithGraphicsOff"
#define QTB_ONCONTROLOFFWITHGRAPHICSOFF_DEFAULT	true

// for onGraphics
#define QTB_ONGRAPHICS			"onGraphics"
#define QTB_ONGRAPHICS_DEFAULT	true

// for videoQuality
#define QTB_VIDEOQUALITY			"videoQuality"
#define QTB_VIDEOQUALITY_DEFAULT	VIDEO_QUALITY_STANDARD

// for frameRate
#define QTB_FRAMERATE			"frameRate"
#define FRAMERATE_MAXIMUM		0
#define QTB_FRAMERATE_DEFAULT	30

// for displayMouseCursor
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONDISPLAYMOUSECURSOR_DEFAULT	true
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONDISPLAYMOUSECURSOR_DEFAULT	false
#endif // defined(QTB_DEV_TOUCHPANEL)
// for onGamePadSupport
#define QTB_ONGAMEPADSUPPORT			"onGamePadSupport"
#if defined(Q_OS_WIN)
#define QTB_ONGAMEPADSUPPORT_DEFAULT	true
#else // defined(Q_OS_WIN)
#define QTB_ONGAMEPADSUPPORT_DEFAULT	false
#endif // defined(Q_OS_WIN)
// for onBrynhildr2Support
#define QTB_ONBRYNHILDR2SUPPORT			"onBrynhildr2Support"
#define QTB_ONBRYNHILDR2SUPPORT_DEFAULT	true

// for onSound
#define QTB_ONSOUND				"onSound"
#define QTB_ONSOUND_DEFAULT		true

#if QTB_CELT_SUPPORT
// for soundType
#define QTB_SOUNDTYPE			"soundType"
#define QTB_SOUNDTYPE_DEFAULT	SOUND_TYPE_CELT
#endif // QTB_CELT_SUPPORT

// for soundQuality
#define QTB_SOUNDQUALITY			"soundQuality"
#define QTB_SOUNDQUALITY_DEFAULT	SOUND_QUALITY_STANDARD

// for soundCapture
#define QTB_SOUNDCAPTURE			"soundCapture"
#define QTB_SOUNDCAPTURE_DEFAULT	SOUND_CAPTURE_CORE_AUDIO

// for soundCacheTime (ms)
#define QTB_SOUNDCACHETIME			"soundCacheTime"
#define QTB_SOUNDCACHETIME_DEFAULT	100

// for onKeepOriginalDesktopSize
#define QTB_ONKEEPORIGINALDESKTOPSIZE "onKeepOriginalDesktopSize"
#define QTB_ONKEEPORIGINALDESKTOPSIZE_DEFAULT true

// for desktopScalingFactor
#define QTB_DESKTOPSCALINGFACTOR	"desktopScalingFactor"
#define QTB_DESKTOPSCALINGFACTOR_DEFAULT 1.0
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_DESKTOPSCALINGFACTORLIMIT_DEFAULT 1.0
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_DESKTOPSCALINGFACTORLIMIT_DEFAULT 0.1
#endif // defined(QTB_DEV_TOUCHPANEL)

// for desktopScaringQuality
#define QTB_DESKTOPSCALINGQUALITY			"desktopScalingQuality"
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_DESKTOPSCALINGQUALITY_DEFAULT	DESKTOPSCALINGQUALITY_FAST
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_DESKTOPSCALINGQUALITY_DEFAULT	DESKTOPSCALINGQUALITY_SMOOTH
#endif // defined(QTB_DEV_TOUCHPANEL)
#define DESKTOPSCALINGQUALITY_SMOOTH		Qt::SmoothTransformation
#define DESKTOPSCALINGQUALITY_FAST			Qt::FastTransformation

// for scalingType
typedef int SCALING_TYPE;
#define QTB_DESKTOPSCALINGTYPE			"desktopScalingType"
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_DESKTOPSCALINGTYPE_DEFAULT	DESKTOPSCALING_TYPE_ON_SERVER
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_DESKTOPSCALINGTYPE_DEFAULT	DESKTOPSCALING_TYPE_ON_CLIENT
#endif // defined(QTB_DEV_TOUCHPANEL)
#define DESKTOPSCALING_TYPE_ON_CLIENT	0
#define DESKTOPSCALING_TYPE_ON_SERVER	1

#if QTB_DESKTOP_COMPRESS_MODE // for TEST
// for desktopCompressMode
typedef int COMPRESS_MODE;
#define QTB_DESKTOPCOMPRESSMODE			"desktopCompressMode"
#define QTB_DESKTOPCOMPRESSMODE_DEFAULT	1
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST

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

// for monitorChangeType
typedef enum {
  MONITOR_CHANGE_TYPE_NONE,
  MONITOR_CHANGE_TYPE_SINGLE_TO_SINGLE,
  MONITOR_CHANGE_TYPE_SINGLE_TO_ALL,
  MONITOR_CHANGE_TYPE_ALL_TO_SINGLE
} MONITOR_CHANGE_TYPE;

// for monitorNo
#define QTB_MONITOR_NO					"monitorNo"
#define QTB_MONITOR_NO_DEFAULT			1
#define MONITOR_NO_MAX					9
#define MONITOR_NO_ALL					99

// for onOpenConnectToServerDialogAtBootup
#define QTB_ONOPENCONNECTTOSERVERDIALOGATBOOTUP				"onOpenConnectToServerDialogAtBootup"
#define QTB_ONOPENCONNECTTOSERVERDIALOGATBOOTUP_DEFAULT		true

// for onCheckUpdateAtBootup
#define QTB_ONCHECKUPDATEATBOOTUP				"onCheckUpdateAtBootup"
#define QTB_ONCHECKUPDATEATBOOTUP_DEFAULT		false

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

// for onDesktopScaleFixed
#define QTB_ONDESKTOPSCALEFIXED				"onDesktopScaleFixed"
#define QTB_ONDESKTOPSCALEFIXED_DEFAULT		false

// for onWindowSizeFixed
#define QTB_ONWINDOWSIZEFIXED				"onWindowSizeFixed"
#define QTB_ONWINDOWSIZEFIXED_DEFAULT		false

// for onFrameLessWindow
#define QTB_ONFRAMELESSWINDOW				"onFrameLessWindow"
#define QTB_ONFRAMELESSWINDOW_DEFAULT		false

// for onShowMenuBar
#define QTB_ONSHOWMENUBAR					"onShowMenuBar"
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONSHOWMENUBAR_DEFAULT			false
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONSHOWMENUBAR_DEFAULT			true
#endif // defined(QTB_DEV_TOUCHPANEL)

// for onShowStatusBar
#define QTB_ONSHOWSTATUSBAR					"onShowStatusBar"
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONSHOWSTATUSBAR_DEFAULT			false
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONSHOWSTATUSBAR_DEFAULT			true
#endif // defined(QTB_DEV_TOUCHPANEL)

// for onFullScreenAtConnected
#define QTB_ONFULLSCREENATCONNECTED					"onFullScreenAtConnected"
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONFULLSCREENATCONNECTED_DEFAULT			true
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONFULLSCREENATCONNECTED_DEFAULT			false
#endif // defined(QTB_DEV_TOUCHPANEL)

// for onHideMenuAndStatusBarAtFullScreen
#define QTB_ONHIDEMENUANDSTATUSBARATFULLSCREEN			"onHideMenuAndStatusBarAtFullScreen"
#define QTB_ONHIDEMENUANDSTATUSBARATFULLSCREEN_DEFAULT	true

// for onShowFrameRate
#define QTB_ONSHOWFRAMERATE					"onShowFrameRate"
#define QTB_ONSHOWFRAMERATE_DEFAULT			true

// for onShowPassword
#define QTB_ONSHOWPASSWORD					"onShowPassword"
#define QTB_ONSHOWPASSWORD_DEFAULT			false

// for onClipCursor
#define QTB_ONCLIPCURSOR					"onClipCursor"
#define QTB_ONCLIPCURSOR_DEFAULT			false

// for onShowMouseCursorMarker
#define QTB_ONSHOWMOUSECURSORMARKER			"onShowMouseCursorMarker"
#if defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONSHOWMOUSECURSORMARKER_DEFAULT	true
#else // defined(QTB_DEV_TOUCHPANEL)
#define QTB_ONSHOWMOUSECURSORMARKER_DEFAULT	false
#endif // defined(QTB_DEV_TOUCHPANEL)

#if QTB_PUBLIC_MODE6_SUPPORT
// for onTransferFileSupport
#define QTB_ONTRANSFERFILESUPPORT			"onTransferFileSupport"
#define QTB_ONTRANSFERFILESUPPORT_DEFAULT	true

// for onShowTotalProgressForTransferFile
#define QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE			"onShowTotalProgressForTransferFile"
#define QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE_DEFAULT	false

#if QTB_DRAG_AND_DROP_SUPPORT
// for onTransferFileSupportByDragAndDrop
#define QTB_ONTRANSFERFILESUPPORTBYDRAGANDDROP			"onTransferFileSupportByDragAndDrop"
#define QTB_ONTRANSFERFILESUPPORTBYDRAGANDDROP_DEFAULT	true
#endif // QTB_DRAG_AND_DROP_SUPPORT

// for onTransferClipboardSupport
#define QTB_ONTRANSFERCLIPBOARDSUPPORT			"onTransferClipboardSupport"
#define QTB_ONTRANSFERCLIPBOARDSUPPORT_DEFAULT	true
#endif // QTB_PUBLIC_MODE6_SUPPORT

// for graphicsBufferSize
#define QTB_GRAPHICSBUFFERSIZE				"graphicsBufferSize"
#define QTB_GRAPHICSBUFFERSIZE_DEFAULT		(1024 * 1024)

// for soundBufferSize
#define QTB_SOUNDBUFFERSIZE					"soundBufferSize"
#define QTB_SOUNDBUFFERSIZE_DEFAULT			(1024 * 1024)

// for doubleClickThreshold
#define QTB_DOUBLECLICKTHRESHOLD			"doubleClickThreshold"
#define QTB_DOUBLECLICKTHRESHOLD_DEFAULT	500

// for convertThreadCount
#define QTB_CONVERTTHREADCOUNT				"convertThreadCount"
#define QTB_CONVERTTHREADCOUNT_DEFAULT		2

#if defined(QTB_DEV_TOUCHPANEL)
// for touchpanelInterfaceType
#define QTB_TOUCHPANELINTERFACETYPE				"touchpanelInterfaceType"
#define QTB_TOUCHPANELINTERFACETYPE_KEROREMOTE	0
#define QTB_TOUCHPANELINTERFACETYPE_QTBRYNHILDR	1
#define QTB_TOUCHPANELINTERFACETYPE_NUM			2
#define QTB_TOUCHPANELINTERFACETYPE_DEFAULT		QTB_TOUCHPANELINTERFACETYPE_QTBRYNHILDR
#endif // defined(QTB_DEV_TOUCHPANEL)

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

// for outputKeyboardLog
#define QTB_OUTPUTKEYBOARDLOG					"outputKeyboardLog"
#define QTB_OUTPUTKEYBOARDLOG_DEFAULT			false

// for outputPath
#define QTB_OUTPUTPATH							"outputPath"

// for keylayoutPath
#define QTB_KEYLAYOUTPATH						"keylayoutPath"

// for logFile
#define QTB_LOGFILE								"logFile"

// for keyboardLogFile
#define QTB_KEYBOARDLOGFILE						"keyboardLogFile"

// for desktop capture format
#define QTB_DESKTOPCAPTUREFORMAT				"desktopCaptureFormat"
#define QTB_DESKTOPCAPTUREFORMAT_DEFAULT		".jpg"

// -- other settings --
// for geometry
#define QTB_GEOMETRY			"geometry"
// for window state
#define QTB_WINDOWSTATE			"windowState"

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

  // get generated version
  int getGenaratedVersion() const
  {
	return generatedVersion;
  }

  // set generated version
  void setGeneratedVersion(int generatedVersion)
  {
	this->generatedVersion = generatedVersion;
  }

  // get current version
  int getCurrentVersion() const
  {
	return currentVersion;
  }

  // set current version
  void setCurrentVersion(int currentVersion)
  {
	this->currentVersion = currentVersion;
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  // get public mode version
  PUBLICMODE_VERSION getPublicModeVersion() const
  {
	return publicModeVersion;
  }

  // set public mode version
  void setPublicModeVersion(PUBLICMODE_VERSION publicModeVersion)
  {
	ASSERT(publicModeVersion == PUBLICMODE_VERSION5 ||
		   publicModeVersion == PUBLICMODE_VERSION6 ||
		   publicModeVersion == PUBLICMODE_VERSION7);

	switch(publicModeVersion){
	case PUBLICMODE_VERSION5:
	case PUBLICMODE_VERSION6:
	case PUBLICMODE_VERSION7:
	  this->publicModeVersion = publicModeVersion;
	  break;
	default:
	  // Error
	  ABORT();
	  break;
	}
  }

  // get public mode version string
  QString getPublicModeVersionByString() const
  {
	switch(publicModeVersion){
	case PUBLICMODE_VERSION5:
	  return "PUBLICMODE_VERSION5";
	  break;
	case PUBLICMODE_VERSION6:
	  return "PUBLICMODE_VERSION6";
	  break;
	case PUBLICMODE_VERSION7:
	  return "PUBLICMODE_VERSION7";
	  break;
	}

	return "UNKNOWN PUBLICMODE_VERSION";
  }

  // get public mode alias string
  QString getPublicModeAliasString() const
  {
	switch(publicModeVersion){
	case PUBLICMODE_VERSION5:
	  return "MODE 5";
	  break;
	case PUBLICMODE_VERSION6:
	  return "MODE 6";
	  break;
	case PUBLICMODE_VERSION7:
	  return "MODE 7";
	  break;
	}

	return "UNKNOWN PUBLICMODE_VERSION";
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT

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

  // get server name list
  QStringList *getServerNameList() const
  {
	return serverNameList;
  }

  // set server name list
  bool setServerNameList(QStringList *serverNameList)
  {
	this->serverNameList = serverNameList;
	return true;
  }

  // get server name list size
  int getServerNameListSize() const
  {
	return serverNameListSize;
  }

  // set server name list size
  bool setServerNameListSize(int serverNameListSize)
  {
	this->serverNameListSize = serverNameListSize;
	return true;
  }

  // read server name list
  void readServerNameList();

  // write server name list
  void writeServerNameList();

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
	//	ASSERT(keyboardType >= KEYBOARD_TYPE_JP && keyboardType < KEYBOARD_TYPE_NUM);
#if !defined(Q_OS_WIN)
	if (keyboardType == KEYBOARD_TYPE_NATIVE){
	  // NOT available except for windows
	  return;
	}
#endif // !defined(Q_OS_WIN)
	this->keyboardType = keyboardType;
  }

  // set keyboard type name
  void setKeyboardTypeName(QString keyboardTypeName)
  {
	this->keyboardTypeName = keyboardTypeName;
  }

  // get keyboard type name
  QString getKeyboardTypeName() const
  {
	static const QString stringTable[KEYBOARD_TYPE_NUM] = {
	  STRING_KEYBOARD_TYPE_JP,
	  STRING_KEYBOARD_TYPE_US,
	  STRING_KEYBOARD_TYPE_NATIVE,
	  "KLF (dummy)"
	};

	if (keyboardType < KEYBOARD_TYPE_KLF){
	  return stringTable[keyboardType];
	}
	else {
	  // KLF
	  return keyboardTypeName;
	}
  }

  // get keyboard type string
  QString getKeyboardTypeByString() const
  {
	static const QString stringTable[KEYBOARD_TYPE_NUM] = {
	  "KEYBOARD_TYPE_WINDOWS_JP",
	  "KEYBOARD_TYPE_WINDOWS_US",
	  "KEYBOARD_TYPE_WINDOWS_NATIVE",
	  "KEYBOARD_TYPE_WINDOWS_KLF"
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

#if QTB_PLUGINS_DISABLE_SUPPORT
  // get plugins disable flag
  bool getOnPluginsDisable() const
  {
	return onPluginsDisable;
  }

  // set plugins disable flag
  bool setOnPluginsDisable(bool onPluginsDisable)
  {
	this->onPluginsDisable = onPluginsDisable;
	return true;
  }
#endif // QTB_PLUGINS_DISABLE_SUPPORT

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

#if QTB_EXTRA_BUTTON_SUPPORT
  // get extra button support flag
  bool getOnExtraButtonSupport() const
  {
	return onExtraButtonSupport;
  }

  // set extra button support flag
  bool setOnExtraButtonSupport(bool onExtraButtonSupport)
  {
	this->onExtraButtonSupport = onExtraButtonSupport;
	return true;
  }
#endif // QTB_EXTRA_BUTTON_SUPPORT

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

#if QTB_PUBLIC_MODE6_SUPPORT
  // get send clipboard flag
  bool getOnSendClipboard() const
  {
	return onSendClipboard;
  }

  // set send clipboard flag
  bool setOnSendClipboard(bool onSendClipboard)
  {
	this->onSendClipboard = onSendClipboard;
	return true;
  }

  // get send clipboard string
  QString getSendClipboardString() const
  {
	return sendClipboardString;
  }

  // set send clipboard string
  void setSendClipboardString(QString sendClipboardString)
  {
	this->sendClipboardString = sendClipboardString;
  }

  // get send file count
  int getSendFileCount() const
  {
	return sendFileCount;
  }

  // set send file count
  void setSendFileCount(int sendFileCount)
  {
	this->sendFileCount = sendFileCount;
  }

  // get send file name list
  QStringList getSendFileNames() const
  {
	return sendFileNames;
  }

  // set send file name list
  void setSendFileNames(QStringList sendFileNames)
  {
	this->sendFileNames = sendFileNames;
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT

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

	return stringTable[(int)videoQuality];
  }

  // get display mouse cursor flag
  bool getOnDisplayMouseCursor()
  {
#if QTB_PUBLIC_MODE7_SUPPORT
	if (getPublicModeVersion() >= PUBLICMODE_VERSION7){
	  return QTB_ONDISPLAYMOUSECURSOR_DEFAULT;
	}
	else { // MODE5/6
	  return true;
	}
#else // QTB_PUBLIC_MODE7_SUPPORT
	return true;
#endif // QTB_PUBLIC_MODE7_SUPPORT
  }

  // get gamepad support flag
  bool getOnGamePadSupport()
  {
	return onGamePadSupport;
  }

  // set gamepad support flag
  void setOnGamePadSupport(bool onGamePadSupport)
  {
	this->onGamePadSupport = onGamePadSupport;
  }

  // get brynhildr2 support flag
  bool getOnBrynhildr2Support()
  {
	return onBrynhildr2Support;
  }

  // set brynhildr2 support flag
  void setOnBrynhildr2Support(bool onBrynhildr2Support)
  {
	this->onBrynhildr2Support = onBrynhildr2Support;
  }

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

#if 0 // for TEST
  // set frame interval (micro seconds)
  void setFrameInterval(unsigned long frameInterval)
  {
	this->frameInterval = frameInterval;
  }
#endif // for TEST

#if QTB_CELT_SUPPORT
  // get sound type
  SOUND_TYPE getSoundType() const
  {
	return soundType;
  }

  // set sound type
  void setSoundType(SOUND_TYPE soundType)
  {
	this->soundType = soundType;
  }

  // get sound type string
  QString getSoundTypeByString() const
  {
	static const QString stringTable[SOUND_TYPE_CELT+1] = {
	  "SOUND_TYPE_OFF",
	  "SOUND_TYPE_PCM",
	  "SOUND_TYPE_CELT",
	};

	return stringTable[(int)soundType];
  }
#endif // QTB_CELT_SUPPORT

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

	return stringTable[(int)soundQuality];
  }

  // get sound capture
  SOUND_CAPTURE getSoundCapture() const
  {
	return soundCapture;
  }

  // get sound cache time
  int getSoundCacheTime() const
  {
	return soundCacheTime;
  }

  // set sound cache time
  void setSoundCacheTime(int soundCacheTime)
  {
	this->soundCacheTime = soundCacheTime;
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
	if (desktopScalingFactor < desktopScalingFactorLimit){
	  desktopScalingFactor = desktopScalingFactorLimit;
	}
	this->desktopScalingFactor = desktopScalingFactor;
	this->desktopScalingFactorForZoom = 1.0/desktopScalingFactor;
  }

  // set desktop scaling factor limit
  void setDesktopScalingFactorLimit(qreal desktopScalingFactorLimit)
  {
	this->desktopScalingFactorLimit = desktopScalingFactorLimit;
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

#if QTB_DESKTOP_COMPRESS_MODE // for TEST
  // get desktop compress mode
  COMPRESS_MODE getDesktopCompressMode() const
  {
	return desktopCompressMode;
  }

  // set desktop compress mode
  void setDesktopCompressMode(COMPRESS_MODE desktopCompressMode)
  {
	this->desktopCompressMode = desktopCompressMode;
  }
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST

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

  // get monitor change type
  MONITOR_CHANGE_TYPE getMonitorChangeType() const
  {
	return monitorChangeType;
  }

  // set monitor change type
  void setMonitorChangeType(MONITOR_CHANGE_TYPE monitorChangeType)
  {
	this->monitorChangeType = monitorChangeType;
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
	  if (this->monitorNo != MONITOR_NO_ALL && monitorNo == MONITOR_NO_ALL){
		// single monitor to All monitor
		setMonitorChangeType(MONITOR_CHANGE_TYPE_SINGLE_TO_ALL);
	  }
	  else if (this->monitorNo == MONITOR_NO_ALL && monitorNo != MONITOR_NO_ALL){
		// All monitor to single monitor
		setMonitorChangeType(MONITOR_CHANGE_TYPE_ALL_TO_SINGLE);
	  }
	  else {
		// single monitor to single monitor
		setMonitorChangeType(MONITOR_CHANGE_TYPE_SINGLE_TO_SINGLE);
	  }
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

  // get open connect to server dialog at bootup flag
  bool getOnOpenConnectToServerDialogAtBootup() const
  {
	return onOpenConnectToServerDialogAtBootup;
  }

  // set open connect to server dialog at bootup flag
  void setOnOpenConnectToServerDialogAtBootup(bool onOpenConnectToServerDialogAtBootup)
  {
	this->onOpenConnectToServerDialogAtBootup = onOpenConnectToServerDialogAtBootup;
  }

  // get check update at bootup flag
  bool getOnCheckUpdateAtBootup() const
  {
	return onCheckUpdateAtBootup;
  }

  // set check update at bootup flag
  void setOnCheckUpdateAtBootup(bool onCheckUpdateAtBootup)
  {
	this->onCheckUpdateAtBootup = onCheckUpdateAtBootup;
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

  // get desktop scale fixed flag
  bool getOnDesktopScaleFixed() const
  {
	return onDesktopScaleFixed;
  }

  // set desktop scale fixed flag
  void setOnDesktopScaleFixed(bool onDesktopScaleFixed)
  {
	this->onDesktopScaleFixed = onDesktopScaleFixed;
  }

  // get window size fixed flag
  bool getOnWindowSizeFixed() const
  {
	return onWindowSizeFixed;
  }

  // set window size fixed flag
  void setOnWindowSizeFixed(bool onWindowSizeFixed)
  {
	this->onWindowSizeFixed = onWindowSizeFixed;
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

  // get full screen flag
  bool getOnFullScreenAtConnected() const
  {
	return onFullScreenAtConnected;
  }

  // set full screen flag
  void setOnFullScreenAtConnected(bool onFullScreenAtConnected)
  {
	this->onFullScreenAtConnected = onFullScreenAtConnected;
  }

  // get hide menu and status bar at full screen flag
  bool getOnHideMenuAndStatusBarAtFullScreen() const
  {
	return onHideMenuAndStatusBarAtFullScreen;
  }

  // set hide menu and status bar at full screen flag
  void setOnHideMenuAndStatusBarAtFullScreen(bool onHideMenuAndStatusBarAtFullScreen)
  {
	this->onHideMenuAndStatusBarAtFullScreen = onHideMenuAndStatusBarAtFullScreen;
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

  // get on desktop capture flag
  bool getOnDesktopCapture() const
  {
	return onDesktopCapture;
  }

  // set on desktop capture flag
  void setOnDesktopCapture(bool onDesktopCapture)
  {
	this->onDesktopCapture = onDesktopCapture;
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

  // get clip cursor flag
  bool getOnClipCursor() const
  {
	return onClipCursor;
  }

  // set clip cursor flag
  void setOnClipCursor(bool onClipCursor)
  {
	this->onClipCursor = onClipCursor;
  }

  // get show mouse cursor marker flag
  bool getOnShowMouseCursorMarker() const
  {
	return onShowMouseCursorMarker;
  }

  // set show mouse cursor marker flag
  void setOnShowMouseCursorMarker(bool onShowMouseCursorMarker)
  {
	this->onShowMouseCursorMarker = onShowMouseCursorMarker;
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  // get  transfer file support flag
  bool getOnTransferFileSupport() const
  {
	return onTransferFileSupport;
  }

  // set  transfer file support flag
  void setOnTransferFileSupport(bool onTransferFileSupport)
  {
	this->onTransferFileSupport = onTransferFileSupport;
  }

  // get show total progress for transfer file flag
  bool getOnShowTotalProgressForTransferFile() const
  {
	return onShowTotalProgressForTransferFile;
  }

  // set show total progress for transfer file flag
  void setOnShowTotalProgressForTransferFile(bool onShowTotalProgressForTransferFile)
  {
	this->onShowTotalProgressForTransferFile = onShowTotalProgressForTransferFile;
  }

#if QTB_DRAG_AND_DROP_SUPPORT
  // get transfer file support by drag and drop flag
  bool getOnTransferFileSupportByDragAndDrop() const
  {
	return onTransferFileSupportByDragAndDrop;
  }

  // set transfer file support by drag and drop flag
  void setOnTransferFileSupportByDragAndDrop(bool onTransferFileSupportByDragAndDrop)
  {
	this->onTransferFileSupportByDragAndDrop = onTransferFileSupportByDragAndDrop;
  }
#endif // QTB_DRAG_AND_DROP_SUPPORT

  // get transfer clilpboard support flag
  bool getOnTransferClipboardSupport() const
  {
	return onTransferClipboardSupport;
  }

  // set transfer clipboard support flag
  void setOnTransferClipboardSupport(bool onTransferClipboardSupport)
  {
	this->onTransferClipboardSupport = onTransferClipboardSupport;
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT

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

  // get convert thread count
  int getConvertThreadCount() const
  {
	return convertThreadCount;
  }

  // set convert thread count
  void setConvertThreadCount(int convertThreadCount)
  {
	this->convertThreadCount = convertThreadCount;
  }

#if defined(QTB_DEV_TOUCHPANEL)
  // get touchpanelInterfaceType
  int getTouchpanelInterfaceType() const
  {
	return touchpanelInterfaceType;
  }

  // set convert thread count
  void setTouchpanelInterfaceType(int touchpanelInterfaceType)
  {
	this->touchpanelInterfaceType = touchpanelInterfaceType;
  }
#endif // defined(QTB_DEV_TOUCHPANEL)

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

  // get output keyboard log flag
  bool getOutputKeyboardLog() const
  {
	return outputKeyboardLog;
  }

  // set output keyboard log flag
  bool setOutputKeyboardLog(bool outputKeyboardLog)
  {
	this->outputKeyboardLog = outputKeyboardLog;
	return outputKeyboardLog;
  }

  // get output path
  QString getOutputPath() const
  {
	return outputPath;
  }

  // set output path
  void setOutputPath(QString outputPath)
  {
	if (!outputPath.endsWith(QDir::separator())){
	  outputPath.append(QDir::separator());
	}
	this->outputPath = outputPath;
  }

  // get keylayout path
  QString getKeylayoutPath() const
  {
	return keylayoutPath;
  }

  // set keylayout path
  void setKeylayoutPath(QString keylayoutPath)
  {
	if (!keylayoutPath.endsWith(QDir::separator())){
	  keylayoutPath.append(QDir::separator());
	}
	this->keylayoutPath = keylayoutPath;
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

  // get keyboard log file
  QString getKeyboardLogFile() const
  {
	return keyboardLogFile;
  }

  // set keyboard log file
  void setKeyboardLogFile(QString keyboardLogFile)
  {
	this->keyboardLogFile = keyboardLogFile;
  }

  // get desktop capture format
  QString getDesktopCaptureFormat() const
  {
	return desktopCaptureFormat;
  }

  // set desktop capture format
  void setDesktopCaptureFormat(QString desktopCaptureFormat)
  {
	this->desktopCaptureFormat = desktopCaptureFormat;
  }

private:
  // get default keyboard type
  KEYBOARD_TYPE getDefaultKeyboardType() const;

  // get default output path
  QString getDefaultOutputPath() const;

  // get default keylayout path
  QString getDefaultKeylayoutPath() const;

  // get Default Log File
  QString getDefaultLogFile() const;

  // get Default Keyboard Log File
  QString getDefaultKeyboardLogFile() const;

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

  // generated version
  volatile int generatedVersion;

  // current version
  volatile int currentVersion;

#if QTB_PUBLIC_MODE6_SUPPORT
  // public mode version
  volatile PUBLICMODE_VERSION publicModeVersion;
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // server name
  QString serverName;

  // server name list size
  volatile int serverNameListSize;

  // server name list
  QStringList *serverNameList;

  // server type
  volatile SERVER_TYPE serverType;

  // keyboard type
  volatile KEYBOARD_TYPE keyboardType;

  // keyboard type name
  QString keyboardTypeName;

  // port
  volatile quint16 portNo;

  // password
  QString password;

  // connected
  volatile bool connected;

  // Control
  volatile bool onControl;
#if QTB_PLUGINS_DISABLE_SUPPORT
  volatile bool onPluginsDisable;
#endif // QTB_PLUGINS_DISABLE_SUPPORT
  volatile bool onHoldMouseControl;
#if QTB_EXTRA_BUTTON_SUPPORT
  volatile bool onExtraButtonSupport;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  volatile bool onControlOffWithGraphicsOff;
#if QTB_PUBLIC_MODE6_SUPPORT
  // send clipboard
  volatile bool onSendClipboard;
  // send clipboard string
  QString sendClipboardString;
  // send file count
  volatile int sendFileCount;
  // send file name
  QStringList sendFileNames;
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // Graphics
  volatile bool onGraphics;
  volatile VIDEO_QUALITY videoQuality;

  volatile bool onGamePadSupport;
  volatile bool onBrynhildr2Support;
  volatile unsigned int frameRate;
  volatile unsigned long frameInterval;

  // Sound
  volatile bool onSound;
#if QTB_CELT_SUPPORT
  volatile SOUND_TYPE soundType;
#endif // QTB_CELT_SUPPORT
  volatile SOUND_QUALITY soundQuality;
  volatile SOUND_CAPTURE soundCapture;
  volatile int soundCacheTime;

  // keep original desktop size flag
  volatile bool onKeepOriginalDesktopSize;

  // scaling factor
  volatile qreal desktopScalingFactor;
  volatile qreal desktopScalingFactorForZoom;
  volatile qreal desktopScalingFactorLimit;

  // scaling quality
  volatile Qt::TransformationMode desktopScalingQuality;

  // scaling type
  volatile SCALING_TYPE desktopScalingType;

#if QTB_DESKTOP_COMPRESS_MODE // for TEST
  // compress mode
  volatile COMPRESS_MODE desktopCompressMode;
#endif // QTB_DESKTOP_COMPRESS_MODE // for TEST

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

  // monitorChangeType
  volatile MONITOR_CHANGE_TYPE monitorChangeType;

  // open connect to server dialog at bootup
  volatile bool onOpenConnectToServerDialogAtBootup;

  // check update at bootup
  volatile bool onCheckUpdateAtBootup;

  // confirm at exit
  volatile bool onConfirmAtExit;

  // save settings at exit
  volatile bool onSaveSettingsAtExit;

  // exit after replaying
  volatile bool onExitAfterReplay;

  // stays on top
  volatile bool onStaysOnTop;

  // desktop scale fixed
  volatile bool onDesktopScaleFixed;

  // window size fixed
  volatile bool onWindowSizeFixed;

  // frame less window
  volatile bool onFrameLessWindow;

  // show menu bar
  volatile bool onShowMenuBar;

  // show status bar
  volatile bool onShowStatusBar;

  // full screen
  volatile bool onFullScreenAtConnected;

  // hide menu and status bar at full screen
  volatile bool onHideMenuAndStatusBarAtFullScreen;

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

  // desktop capture
  volatile bool onDesktopCapture;

  // scroll mode
  volatile bool onScrollMode;

  // show password
  volatile bool onShowPassword;

  // clip cursor
  volatile bool onClipCursor;

  // show marker
  volatile bool onShowMouseCursorMarker;

#if QTB_PUBLIC_MODE6_SUPPORT
  // send/receive file support
  volatile bool onTransferFileSupport;

  // send/receive file
  volatile bool onShowTotalProgressForTransferFile;

  // send/receive file support by drag and drop
  volatile bool onTransferFileSupportByDragAndDrop;

  // send/receive clipboard support
  volatile bool onTransferClipboardSupport;
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // buffer
  volatile int graphicsBufferSize;
  volatile int soundBufferSize;

  // double click threshold
  volatile int doubleClickThreshold;

  // convert thread count
  volatile int convertThreadCount;

#if defined(QTB_DEV_TOUCHPANEL)
  // for touchpanelInterfaceType
  volatile int touchpanelInterfaceType;
#endif // defined(QTB_DEV_TOUCHPANEL)

  // Debug
  volatile bool outputGraphicsDataToFile;
  volatile bool outputSoundDataToFile;
  volatile bool outputSoundDataToWavFile;
  volatile bool outputLog;
  volatile bool outputKeyboardLog;

  // for outputPath
  QString outputPath;

  // for keylayoutPath
  QString keylayoutPath;

  // for logFile
  QString logFile;

  // for keyboardLogFile
  QString keyboardLogFile;

  // for desktopCaptureFormat
  QString desktopCaptureFormat;
};

} // end of namespace qtbrynhildr

// for GUI
#define QTB_ICON_FILENAME			":/images/qtbrynhildr48.ico"
#define QTB_SPLASH_SCREEN_FILENAME	":/images/3Bjunior.jpg"

// for log file
#define QTB_LOG_FILENAME	"qtbrynhildr.log"

// for keyboard log file
#define QTB_KEYBOARDLOG_FILENAME	"keyboard.log"

// date format for log
#define QTB_LOG_DATE_FORMAT "yyyy.MM.dd HH:mm:ss.zzz"

// for desktop capture
#define QTB_DESKTOP_CAPTURE_FILENAME_PREFIX			"Desktopshot-"
#define QTB_DESKTOP_CAPTURE_FILENAME_DATE_FORMAT	"yyyy-MM-dd-HH-mm-ss"

// for DEBUG
#define QTB_GRAPHICS_OUTPUT_FILENAME_PREFIX	"graphics_output"
#define QTB_SOUND_OUTPUT_FILENAME			"sound_output.pcm"
#define QTB_SOUND_OUTPUT_WAV_FILENAME		"sound_output.wav"

#endif // SETTINGS_H
