// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// Syetem Header

// Qt Header
#include <QDir>
#include <QString>

#if defined(QTB_DEBUG)
#include <QDebug>
#endif // defined(QTB_DEBUG)

// Local Header
#include "settings.h"

namespace qtbrynhildr {

// constructor
#if QTB_CRYPTOGRAM
Settings::Settings(const char *iniFileName, Cipher *cipher)
#else // !QTB_CRYPTGRAM
Settings::Settings(const char *iniFileName)
#endif // !QTB_CRYPTGRAM
  :desktop(new Desktop())
  ,hSpace(0)
  ,vSpace(0)
  ,bootupFlag(false)
  ,shutdownFlag(false)
#if QTB_CRYPTOGRAM
  ,cipher(cipher)
#endif // QTB_CRYPTGRAM
  ,serverNameListSize(QTB_SERVERNAMELISTSIZE_DEFAULT)
  ,serverNameList(new QStringList)
  ,connected(false)
  ,onSendControlKeyState(false)
  ,onSendClipboard(false)
  ,sendFileCount(0)
  ,onGamePadSupport(false)
  ,onBrynhildr2Support(true)
  ,monitorNo(QTB_MONITOR_NO_DEFAULT)
  ,monitorCount(0)
  ,onShowSoftwareKeyboard(false)
  ,onShowSoftwareButton(false)
#if QTB_RECORDER
  ,onRecordingControl(false)
  ,recordingControlFileName(nullptr)
  ,onReplayingControl(false)
  ,replayingControlFileName(nullptr)
#endif // QTB_RECORDER
  ,onDesktopCapture(false)
  ,onScrollMode(false)
  ,onViewerMode(false)
  ,onFulFulMode(false)
{
  if (iniFileName != nullptr){
	settings = new QSettings(iniFileName, QSettings::IniFormat);
  }
  else {
#if !QTB_PORTABLE_VERSION
	settings =
	  new QSettings(QSettings::IniFormat, QSettings::UserScope, QTB_ORGANIZATION, QTB_APPLICATION);
#else // QTB_PORTABLE_VERSION
	QString portableIniFileName = QDir::toNativeSeparators(qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator() + QTB_APPLICATION + ".ini");
	settings = new QSettings(portableIniFileName, QSettings::IniFormat);
#endif // QTB_PORTABLE_VERSION
  }

  // set default values
  setDefaultValues();

  // set version information
  setGeneratedVersion(QTB_GENERATEDVERSION_DEFAULT);
  setCurrentVersion(QTB_CURRENTVERSION_DEFAULT);

  // set default
  setPublicModeVersion(QTB_PUBLICMODEVERSION_DEFAULT);
  setServerName(QTB_SERVERNAME_DEFAULT);
  setServerType(QTB_SERVERTYPE_DEFAULT);
  setKeyboardType(getDefaultKeyboardType());
  setKeyboardTypeName(QTB_KEYBOARDTYPENAME_DEFAULT);
  setPortNo(QTB_PORTNO_DEFAULT);
  setPassword(QTB_PASSWORD_DEFAULT);
  setOnControl(QTB_ONCONTROL_DEFAULT);
  setOnTaskbarAssist(QTB_ONTASKBARASSIST_DEFAULT);
  setTaskbarAssistAreaWidth(QTB_TASKBARASSISTAREAWIDTH_DEFAULT);
  setTaskbarAssistAreaHeight(QTB_TASKBARASSISTAREAHEIGHT_DEFAULT);
#if QTB_PLUGINS_DISABLE_SUPPORT
  setOnPluginsDisable(QTB_ONPLUGINSDISABLE_DEFAULT);
#endif // QTB_PLUGINS_DISABLE_SUPPORT
  setOnNativeMenuBarDisable(QTB_ONNATIVEMENUBARDISABLE_DEFAULT);
  setOnSoundCriticalMessageDisable(QTB_ONSOUNDCRITICALMESSAGEDISABLE_DEFAULT);
  setOnHoldMouseControl(QTB_ONHOLDMOUSECONTROL_DEFAULT);
#if QTB_EXTRA_BUTTON_SUPPORT
  setOnExtraButtonSupport(QTB_ONEXTRABUTTONSUPPORT_DEFAULT);
#endif // QTB_EXTRA_BUTTON_SUPPORT
  setOnControlOffWithGraphicsOff(QTB_ONCONTROLOFFWITHGRAPHICSOFF_DEFAULT);
  setOnSendControlKeyState(QTB_ONSENDCONTROLKEYSTATE_DEFAULT);
  setOnGraphics(QTB_ONGRAPHICS_DEFAULT);
  setVideoQuality(QTB_VIDEOQUALITY_DEFAULT);
  setOnGamePadSupport(QTB_ONGAMEPADSUPPORT_DEFAULT);
  setOnBrynhildr2Support(QTB_ONBRYNHILDR2SUPPORT_DEFAULT);
  setFrameRate(QTB_FRAMERATE_DEFAULT);
  setOnSound(QTB_ONSOUND_DEFAULT);
#if QTB_CELT_SUPPORT
  setSoundType(QTB_SOUNDTYPE_DEFAULT);
#endif // QTB_CELT_SUPPORT
  setSoundQuality(QTB_SOUNDQUALITY_DEFAULT);
  setSoundCacheTime(QTB_SOUNDCACHETIME_DEFAULT);

  setDesktopScalingFactorLimit(QTB_DESKTOPSCALINGFACTORLIMIT_DEFAULT);
  setDesktopScalingFactor(QTB_DESKTOPSCALINGFACTOR_DEFAULT);
  setDesktopScalingQuality(QTB_DESKTOPSCALINGQUALITY_DEFAULT);
  setDesktopScalingType(QTB_DESKTOPSCALINGTYPE_DEFAULT);
  setAutoresizeDesktopScalingFactor(QTB_AUTORESIZEDESKTOPSCALINGFACTOR_DEFAULT);

#if QTB_DESKTOP_COMPRESS_MODE
  setDesktopCompressMode(QTB_DESKTOPCOMPRESSMODE_DEFAULT);
#endif // QTB_DESKTOP_COMPRESS_MODE

  setOnCutDesktopBlankArea(QTB_ONCUTDESKTOPBLANKAREA_DEFAULT);

  setOnMonochromeMode(QTB_ONMONOCHROMEMODE_DEFAULT);

  setOnMouseTrackingMode(QTB_ONMOUSETRACKINGMODE_DEFAULT);

  setDesktopOffsetX(QTB_DESKTOPOFFSETX_DEFAULT);
  setDesktopOffsetY(QTB_DESKTOPOFFSETY_DEFAULT);

  setDesktopWidth(QTB_MAX_SERVER_DESKTOP_WIDTH);
  setDesktopHeight(QTB_MAX_SERVER_DESKTOP_HEIGHT);

  setDesktopImageWidth(QTB_MAX_SERVER_DESKTOP_WIDTH);
  setDesktopImageHeight(QTB_MAX_SERVER_DESKTOP_HEIGHT);

  setMonitorNo(QTB_MONITOR_NO_DEFAULT);

  setOnOpenConnectToServerDialogAtBootup(QTB_ONOPENCONNECTTOSERVERDIALOGATBOOTUP_DEFAULT);

  setOnCheckUpdateAtBootup(QTB_ONCHECKUPDATEATBOOTUP_DEFAULT);

  setOnConfirmAtExit(QTB_ONCONFIRMATEXIT_DEFAULT);

  setOnSaveSettingsAtExit(QTB_ONSAVESETTINGSATEXIT_DEFAULT);

  setOnExitAfterReplay(QTB_ONEXITAFTERREPLAY_DEFAULT);

  setOnStaysOnTop(QTB_ONSTAYSONTOP_DEFAULT);

  setOnDesktopScaleFixed(QTB_ONDESKTOPSCALEFIXED_DEFAULT);

  setOnWindowSizeFixed(QTB_ONWINDOWSIZEFIXED_DEFAULT);

  setOnDesktopAutoresize(QTB_ONDESKTOPAUTORESIZE_DEFAULT);

  setOnFrameLessWindow(QTB_ONFRAMELESSWINDOW_DEFAULT);

  setOnShowMenuBar(QTB_ONSHOWMENUBAR_DEFAULT);

#if QTB_TOOLBAR
  setOnShowToolBar(QTB_ONSHOWTOOLBAR_DEFAULT);
#endif // QTB_TOOLBAR

  setOnShowStatusBar(QTB_ONSHOWSTATUSBAR_DEFAULT);

  setDisplayDataCounterType(QTB_DISPLAYDATACOUNTERTYPE_DEFAULT);

  setOnFullScreenAtConnected(QTB_ONFULLSCREENATCONNECTED_DEFAULT);

  setOnViewerAtConnected(QTB_ONVIEWERATCONNECTED_DEFAULT);

  setOnHideMenuAndStatusBarAtFullScreen(QTB_ONHIDEMENUANDSTATUSBARATFULLSCREEN_DEFAULT);

  setOnShowFrameRate(QTB_ONSHOWFRAMERATE_DEFAULT);

  setOnShowPassword(QTB_ONSHOWPASSWORD_DEFAULT);

  setOnClipCursor(QTB_ONCLIPCURSOR_DEFAULT);

  setOnShowMouseCursorMarker(QTB_ONSHOWMOUSECURSORMARKER_DEFAULT);

  setOnTransferFileSupport(QTB_ONTRANSFERFILESUPPORT_DEFAULT);
  setOnShowTotalProgressForTransferFile(QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE_DEFAULT);
#if QTB_DRAG_AND_DROP_SUPPORT
  setOnTransferFileSupportByDragAndDrop(QTB_ONTRANSFERFILESUPPORTBYDRAGANDDROP_DEFAULT);
#endif // QTB_DRAG_AND_DROP_SUPPORT
  setOnTransferClipboardSupport(QTB_ONTRANSFERCLIPBOARDSUPPORT_DEFAULT);

  setSIMDOperationTypeName(QTB_SIMDOPERATIONTYPENAME_DEFAULT);

  setGraphicsBufferSize(QTB_GRAPHICSBUFFERSIZE_DEFAULT);
  setSoundBufferSize(QTB_SOUNDBUFFERSIZE_DEFAULT);

  setDoubleClickThreshold(QTB_DOUBLECLICKTHRESHOLD_DEFAULT);

  setConvertThreadCount(QTB_CONVERTTHREADCOUNT_DEFAULT);

  setTimeoutTime(QTB_TIMEOUTTIME_DEFAULT);

#if defined(QTB_DEV_TOUCHPANEL)
  setTouchpanelOperationType(QTB_TOUCHPANELOPERATIONTYPE_DEFAULT);
  setTouchpanelInterfaceType(QTB_TOUCHPANELINTERFACETYPE_DEFAULT);
#endif // defined(QTB_DEV_TOUCHPANEL)

  setOutputGraphicsDataToFile(QTB_OUTPUTGRAPHICSDATATOFILE_DEFAULT);
  setOutputSoundDataToFile(QTB_OUTPUTSOUNDDATATOFILE_DEFAULT);
  setOutputSoundDataToWavFile(QTB_OUTPUTSOUNDDATATOWAVFILE_DEFAULT);
  setOutputLog(QTB_OUTPUTLOG_DEFAULT);
  setOutputKeyboardLog(QTB_OUTPUTKEYBOARDLOG_DEFAULT);

  setOutputPath(getDefaultOutputPath());
  setKeylayoutPath(getDefaultKeylayoutPath());
  setLogFile(getDefaultLogFile());
  setKeyboardLogFile(getDefaultKeyboardLogFile());

  setDesktopCaptureFormat(QTB_DESKTOPCAPTUREFORMAT_DEFAULT);
}

// destructor
Settings::~Settings()
{
  // delete objects
  if (settings != nullptr){
	delete settings;
	settings = nullptr;
  }
  if (serverNameList != nullptr){
	delete serverNameList;
	serverNameList = nullptr;
  }
  if (desktop != nullptr){
	delete desktop;
	desktop = nullptr;
  }
}

// get settings
QSettings *Settings::getSettings() const
{
  return settings;
}

// read server name list
void Settings::readServerNameList()
{
  // load serverNameListSize
  setServerNameListSize(settings->value(QTB_SERVERNAMELISTSIZE,
										(qint32)QTB_SERVERNAMELISTSIZE_DEFAULT).toInt());
  // load all entries
  for (int i = 0; i < serverNameListSize; i++){
	QString entryName = QTB_SERVERNAME + QString::number(i);
	// load serverName
	(*serverNameList) << settings->value(entryName, "").toString();
  }
}

// write server name list
void Settings::writeServerNameList()
{
  // save serverNameListSize
  settings->setValue(QTB_SERVERNAMELISTSIZE, (qint32)serverNameListSize);
  // save all entries
  int serverNameListNum = (int)serverNameList->size();
  for (int i = 0; i < serverNameListSize; i++){
	QString entryName = QTB_SERVERNAME + QString::number(i);
	// save serverName
	if (i < serverNameListNum){
	  settings->setValue(entryName, serverNameList->at(i));
	}
	else {
	  settings->setValue(entryName, "");
	}
  }
}

// set default values
void Settings::setDefaultValues()
{
  // set margin (hspace, vspace)
  QString kernelVersion = QSysInfo::kernelVersion();
  int hspace = 0;
  int vspace = 0;

#if !defined(QTB_DEV_TOUCHPANEL)
#if defined(Q_OS_WIN)
  if (kernelVersion.startsWith("10.")){			// Windows 10
	hspace = 2;
	vspace = 3;
  }
  else if (kernelVersion.startsWith("6.3")){	// Windows 8.1
	hspace = 2;
	vspace = 3;
  }
  else if (kernelVersion.startsWith("6.2")){	// Windows 8
	hspace = 2;
	vspace = 3;
  }
  else if (kernelVersion.startsWith("6.1")){	// Windows 7
	hspace = 2;
	vspace = 4;
  }
  else {
	// NOT supported Version
	hspace = 2;
	vspace = 3;
  }
#elif defined(Q_OS_LINUX)
  // Linux Desktop
  hspace = 2;
  vspace = 8;
#elif defined(Q_OS_CYGWIN)
  // Cygwin
  hspace = 2;
  vspace = 2;
#elif defined(Q_OS_FREEBSD)
  // FreeBSD
  hspace = 0;
  vspace = 0;
#elif defined(Q_OS_MACOS)
  // Darwin
  hspace = 1;
  vspace = 0;
#endif // defined(Q_OS_MACOS)
#endif // !defined(QTB_DEV_TOUCHPANEL)

  // set hSpace, vSpace
  setHSpace(hspace);
  setVSpace(vspace);
}

// load settings from setting files or registry
void Settings::readSettings()
{
  // load hSpace
  setHSpace(settings->value(QTB_HSPACE, 0).toInt());

  // load vSpace
  setVSpace(settings->value(QTB_VSPACE, 0).toInt());

  // load version information
  setGeneratedVersion(settings->value(QTB_GENERATEDVERSION,
									  (qint32)QTB_GENERATEDVERSION_DEFAULT).toInt());

  // load publicModeVersion
  setPublicModeVersion(settings->value(QTB_PUBLICMODEVERSION,
									   (qint32)QTB_PUBLICMODEVERSION_DEFAULT).toInt());

  // load serverName
  setServerName(settings->value(QTB_SERVERNAME,
								QTB_SERVERNAME_DEFAULT).toString());

  // load serverType
  setServerType(settings->value(QTB_SERVERTYPE,
								(qint32)QTB_SERVERTYPE_DEFAULT).toInt());

  // load keyboardType
  setKeyboardType(settings->value(QTB_KEYBOARDTYPE,
								  (qint32)getDefaultKeyboardType()).toInt());

  // load keyboardTypeName
  setKeyboardTypeName(settings->value(QTB_KEYBOARDTYPENAME,
									  QTB_KEYBOARDTYPENAME_DEFAULT).toString());
  // load portNo
  setPortNo(settings->value(QTB_PORTNO,
							QTB_PORTNO_DEFAULT).toInt());

  // load password
  password = settings->value(QTB_PASSWORD,
							 QTB_PASSWORD_DEFAULT).toString();

  // load onControl
  setOnControl(settings->value(QTB_ONCONTROL,
							   QTB_ONCONTROL_DEFAULT).toBool());

  // load onTaskbarAssist
  setOnTaskbarAssist(settings->value(QTB_ONTASKBARASSIST,
									 QTB_ONTASKBARASSIST_DEFAULT).toBool());

  // load taskbarAssistAreaWidth
  setTaskbarAssistAreaWidth(settings->value(QTB_TASKBARASSISTAREAWIDTH,
											QTB_TASKBARASSISTAREAWIDTH_DEFAULT).toInt());

  // load taskbarAssistAreaHeight
  setTaskbarAssistAreaHeight(settings->value(QTB_TASKBARASSISTAREAHEIGHT,
											 QTB_TASKBARASSISTAREAHEIGHT_DEFAULT).toInt());

  // load onPluginsDisable
  setOnPluginsDisable(settings->value(QTB_ONPLUGINSDISABLE,
									  QTB_ONPLUGINSDISABLE_DEFAULT).toBool());

  // load onNativeMenuBarDisable
  setOnNativeMenuBarDisable(settings->value(QTB_ONNATIVEMENUBARDISABLE,
											QTB_ONNATIVEMENUBARDISABLE_DEFAULT).toBool());

  // load onSoundCriticalMessageDisable
  setOnSoundCriticalMessageDisable(settings->value(QTB_ONSOUNDCRITICALMESSAGEDISABLE,
												   QTB_ONSOUNDCRITICALMESSAGEDISABLE_DEFAULT).toBool());

  // load onHoldMouseControl
  setOnHoldMouseControl(settings->value(QTB_ONHOLDMOUSECONTROL,
										QTB_ONHOLDMOUSECONTROL_DEFAULT).toBool());

#if QTB_EXTRA_BUTTON_SUPPORT
  setOnExtraButtonSupport(settings->value(QTB_ONEXTRABUTTONSUPPORT,
										  QTB_ONEXTRABUTTONSUPPORT_DEFAULT).toBool());
#endif // QTB_EXTRA_BUTTON_SUPPORT

  // load onControlOffWithGraphicsOff
  setOnControlOffWithGraphicsOff(settings->value(QTB_ONCONTROLOFFWITHGRAPHICSOFF,
												 QTB_ONCONTROLOFFWITHGRAPHICSOFF_DEFAULT).toBool());

  // load onSendControlKeyState
  setOnSendControlKeyState(settings->value(QTB_ONSENDCONTROLKEYSTATE,
										   QTB_ONSENDCONTROLKEYSTATE_DEFAULT).toBool());

  // load onGraphics
  setOnGraphics(settings->value(QTB_ONGRAPHICS,
								QTB_ONGRAPHICS_DEFAULT).toBool());

  // load videoQualilty
  setVideoQuality(settings->value(QTB_VIDEOQUALITY,
								  (qint32)QTB_VIDEOQUALITY_DEFAULT).toInt());

  // load onGamePadSupport
  setOnGamePadSupport(settings->value(QTB_ONGAMEPADSUPPORT,
									  QTB_ONGAMEPADSUPPORT_DEFAULT).toBool());

  // load onBrynhildr2Support
  setOnBrynhildr2Support(settings->value(QTB_ONBRYNHILDR2SUPPORT,
										 QTB_ONBRYNHILDR2SUPPORT_DEFAULT).toBool());

  // load frameRate
  setFrameRate(settings->value(QTB_FRAMERATE,
							   (qint32)QTB_FRAMERATE_DEFAULT).toInt());

  // load onSound
  setOnSound(settings->value(QTB_ONSOUND,
							 QTB_ONSOUND_DEFAULT).toBool());

#if QTB_CELT_SUPPORT
  // load soundType
  setSoundType(settings->value(QTB_SOUNDTYPE,
							   (qint32)QTB_SOUNDTYPE_DEFAULT).toInt());
#endif // QTB_CELT_SUPPORT

  // load soundQuality
  setSoundQuality(settings->value(QTB_SOUNDQUALITY,
								 (qint32)QTB_SOUNDQUALITY_DEFAULT).toInt());

  // load soundCapture
  //  setSoundCapture(settings->value(QTB_SOUNDCAPTURE,
  //								 (qint32)QTB_SOUNDCAPTURE_DEFAULT).toInt());

  // load soundCacheTime
  setSoundCacheTime(settings->value(QTB_SOUNDCACHETIME,
									(qint32)QTB_SOUNDCACHETIME_DEFAULT).toInt());

  // load desctopScalingFactor
  setDesktopScalingFactor(settings->value(QTB_DESKTOPSCALINGFACTOR,
										  (qreal)QTB_DESKTOPSCALINGFACTOR_DEFAULT).toFloat());

  // load desctopScalingQuality
  setDesktopScalingQuality((Qt::TransformationMode)settings->value(QTB_DESKTOPSCALINGQUALITY,
																   QTB_DESKTOPSCALINGQUALITY_DEFAULT).toInt());

  // load desktopScalingType
  setDesktopScalingType(settings->value(QTB_DESKTOPSCALINGTYPE,
								 (qint32)QTB_DESKTOPSCALINGTYPE_DEFAULT).toInt());

#if QTB_DESKTOP_COMPRESS_MODE
  // load desktopCompressMode
  setDesktopCompressMode(settings->value(QTB_DESKTOPCOMPRESSMODE,
								 (qint32)QTB_DESKTOPCOMPRESSMODE_DEFAULT).toInt());
#endif // QTB_DESKTOP_COMPRESS_MODE

  // load onCutDesktopBlankArea
  setOnCutDesktopBlankArea(settings->value(QTB_ONCUTDESKTOPBLANKAREA,
										   QTB_ONCUTDESKTOPBLANKAREA_DEFAULT).toBool());

  // load onMonochromeMode
  setOnMonochromeMode(settings->value(QTB_ONMONOCHROMEMODE,
									  QTB_ONMONOCHROMEMODE_DEFAULT).toBool());

  // load onMouseTrackingMode
  setOnMouseTrackingMode(settings->value(QTB_ONMOUSETRACKINGMODE,
										 QTB_ONMOUSETRACKINGMODE_DEFAULT).toBool());

#if 0 // for TEST
  // load desktopOffsetX
  setDesktopOffsetX(settings->value(QTB_DESKTOPOFFSETX,
							   (qint32)QTB_DESKTOPOFFSETX_DEFAULT).toInt());

  // load desktopOffsetY
  setDesktopOffsetY(settings->value(QTB_DESKTOPOFFSETY,
							   (qint32)QTB_DESKTOPOFFSETY_DEFAULT).toInt());
#endif // 0 // for TEST

  // load monitorNo
  setMonitorNo(settings->value(QTB_MONITOR_NO,
							   (qint32)QTB_MONITOR_NO_DEFAULT).toInt());

  // load onOpenConnectToServerDialogAtBootup
  setOnOpenConnectToServerDialogAtBootup(settings->value(QTB_ONOPENCONNECTTOSERVERDIALOGATBOOTUP,
														 QTB_ONOPENCONNECTTOSERVERDIALOGATBOOTUP_DEFAULT).toBool());

  // load onCheckUpdateAtBootup
  setOnCheckUpdateAtBootup(settings->value(QTB_ONCHECKUPDATEATBOOTUP,
										   QTB_ONCHECKUPDATEATBOOTUP_DEFAULT).toBool());

  // load onConfirmAtExit
  setOnConfirmAtExit(settings->value(QTB_ONCONFIRMATEXIT,
									 QTB_ONCONFIRMATEXIT_DEFAULT).toBool());

  // load onSaveSettingsAtExit
  setOnSaveSettingsAtExit(settings->value(QTB_ONSAVESETTINGSATEXIT,
										  QTB_ONSAVESETTINGSATEXIT_DEFAULT).toBool());

  // load onExitAfterReplay
  setOnExitAfterReplay(settings->value(QTB_ONEXITAFTERREPLAY,
									   QTB_ONEXITAFTERREPLAY_DEFAULT).toBool());

  // load onStaysOnTop
  setOnStaysOnTop(settings->value(QTB_ONSTAYSONTOP,
								  QTB_ONSTAYSONTOP_DEFAULT).toBool());

  // load onDesktopScaleFixed
  setOnDesktopScaleFixed(settings->value(QTB_ONDESKTOPSCALEFIXED,
										 QTB_ONDESKTOPSCALEFIXED_DEFAULT).toBool());

  // load onWindowSizeFixed
  setOnWindowSizeFixed(settings->value(QTB_ONWINDOWSIZEFIXED,
									   QTB_ONWINDOWSIZEFIXED_DEFAULT).toBool());

  // load onDesktopAutoresize
  setOnDesktopAutoresize(settings->value(QTB_ONDESKTOPAUTORESIZE,
										 QTB_ONDESKTOPAUTORESIZE_DEFAULT).toBool());

  // load onFrameLessWindow
  setOnFrameLessWindow(settings->value(QTB_ONFRAMELESSWINDOW,
									   QTB_ONFRAMELESSWINDOW_DEFAULT).toBool());

  // load onShowMenuBar
  setOnShowMenuBar(settings->value(QTB_ONSHOWMENUBAR,
								   QTB_ONSHOWMENUBAR_DEFAULT).toBool());

#if QTB_TOOLBAR
  // load onShowToolBar
  setOnShowToolBar(settings->value(QTB_ONSHOWTOOLBAR,
								   QTB_ONSHOWTOOLBAR_DEFAULT).toBool());
#endif // QTB_TOOLBAR

  // load onShowStatusBar
  setOnShowStatusBar(settings->value(QTB_ONSHOWSTATUSBAR,
									 QTB_ONSHOWSTATUSBAR_DEFAULT).toBool());

  // load displayDataCounterType
  setDisplayDataCounterType(settings->value(QTB_DISPLAYDATACOUNTERTYPE,
											(qint32)QTB_DISPLAYDATACOUNTERTYPE_DEFAULT).toInt());

  // load onFullScreenAtConnected
  setOnFullScreenAtConnected(settings->value(QTB_ONFULLSCREENATCONNECTED,
											 QTB_ONFULLSCREENATCONNECTED_DEFAULT).toBool());

  // load onViewerAtConnected
  setOnViewerAtConnected(settings->value(QTB_ONVIEWERATCONNECTED,
										 QTB_ONVIEWERATCONNECTED_DEFAULT).toBool());

  // load onHideMenuAndStatusBarAtFullScreen
  setOnHideMenuAndStatusBarAtFullScreen(settings->value(QTB_ONHIDEMENUANDSTATUSBARATFULLSCREEN,
														QTB_ONHIDEMENUANDSTATUSBARATFULLSCREEN_DEFAULT).toBool());

  // load onShowFrameRate
  setOnShowFrameRate(settings->value(QTB_ONSHOWFRAMERATE,
									 QTB_ONSHOWFRAMERATE_DEFAULT).toBool());

  // load onShowPassword
  setOnShowPassword(settings->value(QTB_ONSHOWPASSWORD,
									QTB_ONSHOWPASSWORD_DEFAULT).toBool());

  // load onClipCursor
  setOnClipCursor(settings->value(QTB_ONCLIPCURSOR,
								  QTB_ONCLIPCURSOR_DEFAULT).toBool());

  // load onShowMouseCursorMarker
  setOnShowMouseCursorMarker(settings->value(QTB_ONSHOWMOUSECURSORMARKER,
											 QTB_ONSHOWMOUSECURSORMARKER_DEFAULT).toBool());

  // load onTansferFile
  setOnTransferFileSupport(settings->value(QTB_ONTRANSFERFILESUPPORT,
									QTB_ONTRANSFERFILESUPPORT_DEFAULT).toBool());
  // load onShowTotalProgressForTransferFile
  setOnShowTotalProgressForTransferFile(settings->value(QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE,
														QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE_DEFAULT).toBool());
#if QTB_DRAG_AND_DROP_SUPPORT
  // load onTansferFileByDragAndDrop
  setOnTransferFileSupportByDragAndDrop(settings->value(QTB_ONTRANSFERFILESUPPORTBYDRAGANDDROP,
												 QTB_ONTRANSFERFILESUPPORTBYDRAGANDDROP_DEFAULT).toBool());
#endif // QTB_DRAG_AND_DROP_SUPPORT

  // load onTansferClipboardSupport
  setOnTransferClipboardSupport(settings->value(QTB_ONTRANSFERCLIPBOARDSUPPORT,
												QTB_ONTRANSFERCLIPBOARDSUPPORT_DEFAULT).toBool());

  // load SIMDOperationTypeName
  setSIMDOperationTypeName(settings->value(QTB_SIMDOPERATIONTYPENAME,
										   QTB_SIMDOPERATIONTYPENAME_DEFAULT).toString());

  // load graphicsBufferSize
  setGraphicsBufferSize(settings->value(QTB_GRAPHICSBUFFERSIZE,
										QTB_GRAPHICSBUFFERSIZE_DEFAULT).toInt());

  // load soundBufferSize
  setSoundBufferSize(settings->value(QTB_SOUNDBUFFERSIZE,
									 QTB_SOUNDBUFFERSIZE_DEFAULT).toInt());

  // load doubleClickThreshold
  setDoubleClickThreshold(settings->value(QTB_DOUBLECLICKTHRESHOLD,
										  QTB_DOUBLECLICKTHRESHOLD_DEFAULT).toInt());

  // load convertThreadCount
  setConvertThreadCount(settings->value(QTB_CONVERTTHREADCOUNT,
										QTB_CONVERTTHREADCOUNT_DEFAULT).toInt());

  // load timeoutTime
  setTimeoutTime(settings->value(QTB_TIMEOUTTIME,
								 QTB_TIMEOUTTIME_DEFAULT).toInt());

#if defined(QTB_DEV_TOUCHPANEL)
  // load touchpanelOperationType
  setTouchpanelOperationType(settings->value(QTB_TOUCHPANELOPERATIONTYPE,
											 QTB_TOUCHPANELOPERATIONTYPE_DEFAULT).toInt());
  // load touchpanelInterfaceType
  setTouchpanelInterfaceType(settings->value(QTB_TOUCHPANELINTERFACETYPE,
											 QTB_TOUCHPANELINTERFACETYPE_DEFAULT).toInt());
#endif // defined(QTB_DEV_TOUCHPANEL)

  // load outputGraphicsDataToFile
  setOutputGraphicsDataToFile(settings->value(QTB_OUTPUTGRAPHICSDATATOFILE,
											  QTB_OUTPUTGRAPHICSDATATOFILE_DEFAULT).toBool());

  // load outputSoundDataToFile
  setOutputSoundDataToFile(settings->value(QTB_OUTPUTSOUNDDATATOFILE,
										   QTB_OUTPUTSOUNDDATATOFILE_DEFAULT).toBool());

  // load outputSoundDataToWavFile
  setOutputSoundDataToWavFile(settings->value(QTB_OUTPUTSOUNDDATATOWAVFILE,
											  QTB_OUTPUTSOUNDDATATOWAVFILE_DEFAULT).toBool());

  // load outputLog
  setOutputLog(settings->value(QTB_OUTPUTLOG,
							   QTB_OUTPUTLOG_DEFAULT).toBool());

  // load outputKeyboardLog
  setOutputKeyboardLog(settings->value(QTB_OUTPUTKEYBOARDLOG,
									   QTB_OUTPUTKEYBOARDLOG_DEFAULT).toBool());

#if !QTB_PORTABLE_VERSION
  // load output path
  setOutputPath(settings->value(QTB_OUTPUTPATH,
								getDefaultOutputPath()).toString());

  // load keylayout path
  setKeylayoutPath(settings->value(QTB_KEYLAYOUTPATH,
								getDefaultKeylayoutPath()).toString());

  // load log file
  setLogFile(settings->value(QTB_LOGFILE,
							 getDefaultLogFile()).toString());

  // load keyboard log file
  setKeyboardLogFile(settings->value(QTB_KEYBOARDLOGFILE,
									 getDefaultKeyboardLogFile()).toString());
#endif // !QTB_PORTABLE_VERSION

  // load desktop capture format
  setDesktopCaptureFormat(settings->value(QTB_DESKTOPCAPTUREFORMAT,
										  QTB_DESKTOPCAPTUREFORMAT_DEFAULT).toString());

  // read server name list
  readServerNameList();
}

// save settings to setting file or registry
void Settings::writeSettings()
{
  // save hSpace
  settings->setValue(QTB_HSPACE, (qint32)hSpace);

  // save vSpace
  settings->setValue(QTB_VSPACE, (qint32)vSpace);

  // save vSpace
  setVSpace(settings->value(QTB_VSPACE, 0).toInt());

  // save informations (write only)
  settings->setValue(QTB_INFOKERNELTYPE,		QSysInfo::kernelType());
  settings->setValue(QTB_INFOKERNELVERSION,		QSysInfo::kernelVersion());
  settings->setValue(QTB_INFOPRODUCTTYPE,		QSysInfo::productType());
  settings->setValue(QTB_INFOPRODUCTVERSION,	QSysInfo::productVersion());
  settings->setValue(QTB_INFOBUILDABI,			QSysInfo::buildAbi());

  // save version information
  settings->setValue(QTB_GENERATEDVERSION, (qint32)generatedVersion);
  settings->setValue(QTB_CURRENTVERSION, (qint32)currentVersion);

  // save publicModeVersion
  settings->setValue(QTB_PUBLICMODEVERSION, (qint32)publicModeVersion);

  // save serverName
  settings->setValue(QTB_SERVERNAME, serverName);

  // save serverType
  settings->setValue(QTB_SERVERTYPE, (qint32)serverType);

  // save keyboardType
  keyboardType = keyboardType < KEYBOARD_TYPE_KLF ? keyboardType : KEYBOARD_TYPE_KLF;
  settings->setValue(QTB_KEYBOARDTYPE, (qint32)keyboardType);
  settings->setValue(QTB_KEYBOARDTYPENAME, getKeyboardTypeName());

  // save portNo
  settings->setValue(QTB_PORTNO, portNo);

  // save password
  //  settings->setValue(QTB_PASSWORD, password);

  // save onControl
  settings->setValue(QTB_ONCONTROL, onControl);

  // save onTaskbarAssist
  settings->setValue(QTB_ONTASKBARASSIST, onTaskbarAssist);

  // save taskbarAssistAreaWidth
  settings->setValue(QTB_TASKBARASSISTAREAWIDTH, taskbarAssistAreaWidth);

  // save taskbarAssistAreaHeight
  settings->setValue(QTB_TASKBARASSISTAREAHEIGHT, taskbarAssistAreaHeight);

  // save onPluginsDisable
  settings->setValue(QTB_ONPLUGINSDISABLE, onPluginsDisable);

  // save onNativeMenuBarDisable
  settings->setValue(QTB_ONNATIVEMENUBARDISABLE, onNativeMenuBarDisable);

  // save onSoundCriticalMessageDisable
  settings->setValue(QTB_ONSOUNDCRITICALMESSAGEDISABLE, onSoundCriticalMessageDisable);

  // save onHoldMouseControl
  settings->setValue(QTB_ONHOLDMOUSECONTROL, onHoldMouseControl);

#if QTB_EXTRA_BUTTON_SUPPORT
  // save onExtraButtonSupport
  settings->setValue(QTB_ONEXTRABUTTONSUPPORT, onExtraButtonSupport);
#endif // QTB_EXTRA_BUTTON_SUPPORT

  // save onControlOffWithGraphicsOff
  settings->setValue(QTB_ONCONTROLOFFWITHGRAPHICSOFF, onControlOffWithGraphicsOff);

  // save onSendControlKeyState
  settings->setValue(QTB_ONSENDCONTROLKEYSTATE, onSendControlKeyState);

  // save onGraphics
  settings->setValue(QTB_ONGRAPHICS, onGraphics);

  // save videoQuality
  settings->setValue(QTB_VIDEOQUALITY, (qint32)videoQuality);

  // save onGamePadSupport
  settings->setValue(QTB_ONGAMEPADSUPPORT, onGamePadSupport);

  // save onBrynhildr2Support
  settings->setValue(QTB_ONBRYNHILDR2SUPPORT, onBrynhildr2Support);

  // save frameRate
  settings->setValue(QTB_FRAMERATE, (qint32)frameRate);

  // save onSound
  settings->setValue(QTB_ONSOUND, onSound);

#if QTB_CELT_SUPPORT
  // save soundType
  settings->setValue(QTB_SOUNDTYPE, (qint32)soundType);
#endif // QTB_CELT_SUPPORT

  // save soundQuality
  settings->setValue(QTB_SOUNDQUALITY, (qint32)soundQuality);

  // save soundCacheTime
  settings->setValue(QTB_SOUNDCACHETIME, (qint32)soundCacheTime);

  // save soundCapture
  //  settings->setValue(QTB_SOUNDCAPTURE, (qint32)soundCapture);

  //  save desktopScalingFactor
  settings->setValue(QTB_DESKTOPSCALINGFACTOR, (qreal)desktopScalingFactor);

  //  save desktopScalingQuality
  settings->setValue(QTB_DESKTOPSCALINGQUALITY, desktopScalingQuality);

  // save desktopScalingType
  settings->setValue(QTB_DESKTOPSCALINGTYPE, (qint32)desktopScalingType);

#if QTB_DESKTOP_COMPRESS_MODE
  // save desktopCompressMode
  settings->setValue(QTB_DESKTOPCOMPRESSMODE, (qint32)desktopCompressMode);
#endif // QTB_DESKTOP_COMPRESS_MODE

  // save onCutDesktopBlankArea
  settings->setValue(QTB_ONCUTDESKTOPBLANKAREA, onCutDesktopBlankArea);

  // save onMonochromeMode
  settings->setValue(QTB_ONMONOCHROMEMODE, onMonochromeMode);

  // save onMouseTrackingMode
  settings->setValue(QTB_ONMOUSETRACKINGMODE, onMouseTrackingMode);

#if 0 // for TEST
  // save desktop offset X
  settings->setValue(QTB_DESKTOPOFFSETX, (qint32)desktopOffsetX);

  // save desktop offset Y
  settings->setValue(QTB_DESKTOPOFFSETY, (qint32)desktopOffsetY);
#endif // 0 // for TEST

  // save monitor no
  settings->setValue(QTB_MONITOR_NO, (qint32)monitorNo);

  // save onOpenConnectToServerDialogAtBootup
  settings->setValue(QTB_ONOPENCONNECTTOSERVERDIALOGATBOOTUP, onOpenConnectToServerDialogAtBootup);

  // save onCheckUpdateAtBootup
  settings->setValue(QTB_ONCHECKUPDATEATBOOTUP, onCheckUpdateAtBootup);

  // save confirmAtExit
  settings->setValue(QTB_ONCONFIRMATEXIT, onConfirmAtExit);

  // save saveSettingsAtExit
  settings->setValue(QTB_ONSAVESETTINGSATEXIT, onSaveSettingsAtExit);

  // save exit after replay
  settings->setValue(QTB_ONEXITAFTERREPLAY, onExitAfterReplay);

  // save onStaysOnTop
  settings->setValue(QTB_ONSTAYSONTOP, onStaysOnTop);

  // save onDesktopScaleFixed
  settings->setValue(QTB_ONDESKTOPSCALEFIXED, onDesktopScaleFixed);

  // save onWindowSizeFixed
  settings->setValue(QTB_ONWINDOWSIZEFIXED, onWindowSizeFixed);

  // save onDesktopAutoresize
  settings->setValue(QTB_ONDESKTOPAUTORESIZE, onDesktopAutoresize);

  // save onFrameLessWindow
  settings->setValue(QTB_ONFRAMELESSWINDOW, onFrameLessWindow);

  // save onShowMenuBar
  settings->setValue(QTB_ONSHOWMENUBAR, onShowMenuBar);

#if QTB_TOOLBAR
  // save onShowToolBar
  settings->setValue(QTB_ONSHOWTOOLBAR, onShowToolBar);
#endif // QTB_TOOLBAR

  // save onShowStatusBar
  settings->setValue(QTB_ONSHOWSTATUSBAR, onShowStatusBar);

  // save displayDataCounterType
  settings->setValue(QTB_DISPLAYDATACOUNTERTYPE, (qint32)displayDataCounterType);

  // save onFullScreenAtConnected
  settings->setValue(QTB_ONFULLSCREENATCONNECTED, onFullScreenAtConnected);

  // save onViewerAtConnected
  settings->setValue(QTB_ONVIEWERATCONNECTED, onViewerAtConnected);

  // save onHideMenuAndStatusBarAtFullScreen
  settings->setValue(QTB_ONHIDEMENUANDSTATUSBARATFULLSCREEN, onHideMenuAndStatusBarAtFullScreen);

  // save onShowFrameRate
  settings->setValue(QTB_ONSHOWFRAMERATE, onShowFrameRate);

  // save onShowPassword
  settings->setValue(QTB_ONSHOWPASSWORD, onShowPassword);

  // save onClipCursor
  settings->setValue(QTB_ONCLIPCURSOR, onClipCursor);

  // save onShowMouseCursorMarker
  settings->setValue(QTB_ONSHOWMOUSECURSORMARKER, onShowMouseCursorMarker);

  // save onTransferFileSupport
  settings->setValue(QTB_ONTRANSFERFILESUPPORT, onTransferFileSupport);
  // save onShowTotalProgressForTransferFile
  settings->setValue(QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE, onShowTotalProgressForTransferFile);
#if QTB_DRAG_AND_DROP_SUPPORT
  // save onTransferFileSupportByDragAndDrop
  settings->setValue(QTB_ONTRANSFERFILESUPPORTBYDRAGANDDROP, onTransferFileSupportByDragAndDrop);
#endif // QTB_DRAG_AND_DROP_SUPPORT
  // save onTransferClipboardSupport
  settings->setValue(QTB_ONTRANSFERCLIPBOARDSUPPORT, onTransferClipboardSupport);

  // save SIMDOperationTypeName
  settings->setValue(QTB_SIMDOPERATIONTYPENAME, SIMDOperationTypeName);

  // save graphicsBufferSize
  settings->setValue(QTB_GRAPHICSBUFFERSIZE, graphicsBufferSize);

  // save soundBufferSize
  settings->setValue(QTB_SOUNDBUFFERSIZE, soundBufferSize);

  // save doubleClickThreshold
  settings->setValue(QTB_DOUBLECLICKTHRESHOLD, doubleClickThreshold);

  // save convertThreadCount
  settings->setValue(QTB_CONVERTTHREADCOUNT, convertThreadCount);

  // save timeoutTime
  settings->setValue(QTB_TIMEOUTTIME, timeoutTime);

#if defined(QTB_DEV_TOUCHPANEL)
  // save touchpanelOperationType
  settings->setValue(QTB_TOUCHPANELOPERATIONTYPE, touchpanelOperationType);
  // save touchpanelInterfaceType
  settings->setValue(QTB_TOUCHPANELINTERFACETYPE, touchpanelInterfaceType);
#endif // defined(QTB_DEV_TOUCHPANEL)

  // save outputGraphicsDataToFile
  settings->setValue(QTB_OUTPUTGRAPHICSDATATOFILE, outputGraphicsDataToFile);

  // save outputSoundDataToFile
  settings->setValue(QTB_OUTPUTSOUNDDATATOFILE, outputSoundDataToFile);

  // save outputSoundDataToWavFile
  settings->setValue(QTB_OUTPUTSOUNDDATATOWAVFILE, outputSoundDataToWavFile);

  // save outputLog
  settings->setValue(QTB_OUTPUTLOG, outputLog);

  // save outputKeyboardLog
  settings->setValue(QTB_OUTPUTKEYBOARDLOG, outputKeyboardLog);

#if !QTB_PORTABLE_VERSION
  // save outputPath
  settings->setValue(QTB_OUTPUTPATH, outputPath);

  // save keylayoutPath
  settings->setValue(QTB_KEYLAYOUTPATH, keylayoutPath);

  // save logFile
  settings->setValue(QTB_LOGFILE, logFile);

  // save keyboardLogFile
  settings->setValue(QTB_KEYBOARDLOGFILE, keyboardLogFile);
#endif // !QTB_PORTABLE_VERSION

  // save desktopCaptureFormat
  settings->setValue(QTB_DESKTOPCAPTUREFORMAT, desktopCaptureFormat);

  // write server name list
  writeServerNameList();

  // sync
  settings->sync();
}

#if defined(QTB_DEBUG)
// for log
void Settings::printSettings() const
{
  qDebug() << "---------------- Settings ----------------";
  qDebug() << "hSpace        : " << hSpace;
  qDebug() << "vSpace        : " << vSpace;
  qDebug() << "------------------------------------------";
  qDebug() << "Generated Version : " << generatedVersion;
  qDebug() << "Current Version   : " << currentVersion;
  qDebug() << "Public Mode Version: " << getPublicModeVersionByString();
  qDebug() << "------------------------------------------";
  qDebug() << "Server Name   : " << serverName;
  qDebug() << "Server Type   : " << getServerTypeByString();
  qDebug() << "Keyboard Type : " << getKeyboardTypeByString();
  qDebug() << "Port No       : " << portNo;
  qDebug() << "Password      : " << password;
  qDebug() << "Control       : " << onControl;
  qDebug() << "Taskbar Assist: " << onTaskbarAssist;
  qDebug() << "Taskbar Assist Area Width : " << taskbarAssistAreaWidth;
  qDebug() << "Taskbar Assist Area Height: " << taskbarAssistAreaHeight;
#if QTB_PLUGINS_DISABLE_SUPPORT
  qDebug() << "PluginsDisable: " << onPluginsDisable;
  qDebug() << "NativeMenuBarDisable: " << onNativeMenuBarDisable;
  qDebug() << "SoundCriticalMessageDisable: " << onSoundCriticalMessageDisable;
#endif // QTB_PLUGINS_DISABLE_SUPPORT
  qDebug() << "GamePadSupport: " << onGamePadSupport;
  qDebug() << "  HoldMouseControl : " << onHoldMouseControl;
#if QTB_EXTRA_BUTTON_SUPPORT
  qDebug() << "  ExtraButtonSupport : " << onExtraButtonSupport;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  qDebug() << "  ControlOffWithGraphicsOff : " << onControlOffWithGraphicsOff;
  qDebug() << "  SendControlKeyState       : " << onSendControlKeyState;
  qDebug() << "  SendClipboard : " << onSendClipboard;
  qDebug() << "  SendFileCount : " << sendFileCount;
  qDebug() << "Graphics      : " << onGraphics;
  qDebug() << "    QUALITY   : " << getVideoQualityByString();
  qDebug() << "    FrameRate : " << frameRate;
  qDebug() << "Sound         : " << onSound;
#if QTB_CELT_SUPPORT
  qDebug() << "    TYPE      : " << getSoundTypeByString();
#endif // QTB_CELT_SUPPORT
  qDebug() << "    QUALITY   : " << getSoundQualityByString();
  qDebug() << "    CAPTURE   : " << getSoundCaptureByString();
  qDebug() << "    CacheTime : " << getSoundCacheTime();

  qDebug() << "DesktopScalingFactor    : " << desktopScalingFactor;
  qDebug() << "DesktopScalingQuality   : " << desktopScalingQuality;
  qDebug() << "DesktopScalingType      : " << desktopScalingType;
#if QTB_DESKTOP_COMPRESS_MODE
  qDebug() << "DesktopCompressMode     : " << desktopCompressMode;
#endif // QTB_DESKTOP_COMPRESS_MODE

  qDebug() << "CutDesktopBlankArea     : " << onCutDesktopBlankArea;

  qDebug() << "MonochromeMode          : " << onMonochromeMode;

  qDebug() << "MouseTrackingMode       : " << onMouseTrackingMode;

  qDebug() << "DesktopOffsetX          : " << desktopOffsetX;
  qDebug() << "DesktopOffsetY          : " << desktopOffsetY;

  qDebug() << "MonitorNo               : " << monitorNo;
  qDebug() << "OpenConnectToServerDialogAtBootup           : " << onOpenConnectToServerDialogAtBootup;
  qDebug() << "CheckUpdateAtBootup     : " << onCheckUpdateAtBootup;
  qDebug() << "ConfirmAtExit           : " << onConfirmAtExit;
  qDebug() << "SaveSettingsAtExit      : " << onSaveSettingsAtExit;
  qDebug() << "ExitAfterReplay         : " << onExitAfterReplay;
  qDebug() << "StaysOnTop              : " << onStaysOnTop;
  qDebug() << "DesktopScaleFixed       : " << onDesktopScaleFixed;
  qDebug() << "WindowSizeFixed         : " << onWindowSizeFixed;
  qDebug() << "DesktopAutoresize       : " << onDesktopAutoresize;
  qDebug() << "FrameLessWindow         : " << onFrameLessWindow;
  qDebug() << "ShowMenuBar             : " << onShowMenuBar;
#if QTB_TOOLBAR
  qDebug() << "ShowToolBar             : " << onShowToolBar;
#endif // QTB_TOOLBAR
  qDebug() << "ShowStatusBar           : " << onShowStatusBar;
  qDebug() << "DisplayDataCounterType  : " << displayDataCounterType;
  qDebug() << "FullScreenAtConnected   : " << onFullScreenAtConnected;
  qDebug() << "ViewerAtConnected       : " << onViewerAtConnected;
  qDebug() << "HideMenuAndStatusBarAtFullScreen : " << onHideMenuAndStatusBarAtFullScreen;
  qDebug() << "ShowFrameRate           : " << onShowFrameRate;
  qDebug() << "ShowPassword            : " << onShowPassword;
  qDebug() << "ClipCursor              : " << onClipCursor;
  qDebug() << "ShowMouseCursorMarker   : " << onShowMouseCursorMarker;
  qDebug() << "TransferFileSupport     : " << onTransferFileSupport;
  qDebug() << "ShowTotalProgressForTransferFile : " << onShowTotalProgressForTransferFile;
#if QTB_DRAG_AND_DROP_SUPPORT
  qDebug() << "TransferFileSupportByDragAndDrop : " << onTransferFileSupportByDragAndDrop;
#endif // QTB_DRAG_AND_DROP_SUPPORT
  qDebug() << "TransferClipboardSupport: " << onTransferClipboardSupport;

  qDebug() << "SIMDOperationTypeName   : " << SIMDOperationTypeName;

  qDebug() << "Brynhildr2Support       : " << onBrynhildr2Support;

  qDebug() << "------------------------------------------";
  qDebug() << "Graphics Buffer Size (bytes) : " << graphicsBufferSize;
  qDebug() << "Sound    Buffer Size (bytes) : " << soundBufferSize;

  qDebug() << "Double Click Threshold(msecond) : " << doubleClickThreshold;
  qDebug() << "Convert Thread Count         : " << convertThreadCount;
  qDebug() << "Timeout Time                 : " << timeoutTime;
#if defined(QTB_DEV_TOUCHPANEL)
  qDebug() << "TouchpanelOperationType      : " << touchpanelOperationType;
  qDebug() << "TouchpanelInterfaceType      : " << touchpanelInterfaceType;
#endif // defined(QTB_DEV_TOUCHPANEL)

  qDebug() << "------------------------------------------";
  qDebug() << "DBG: output Graphics Data To File    : " << outputGraphicsDataToFile;
  qDebug() << "DBG: output Sound    Data To File    : " << outputSoundDataToFile;
  qDebug() << "DBG: output Sound    Data To WavFile : " << outputSoundDataToWavFile;
  qDebug() << "DBG: output Log                      : " << outputLog;
  qDebug() << "DBG: output Keyboard Log             : " << outputKeyboardLog;
  qDebug() << "------------------------------------------";
  qDebug() << "Output Path       : " << outputPath;
  qDebug() << "Keylayout Path    : " << keylayoutPath;
  qDebug() << "Log File          : " << logFile;
  qDebug() << "Keyboard Log File : " << keyboardLogFile;
  qDebug() << "Desktop Capture Format : " << desktopCaptureFormat;
}

// get default keyboard type
KEYBOARD_TYPE Settings::getDefaultKeyboardType() const
{
  if (QLocale::system().name() == "ja_JP"){
	return KEYBOARD_TYPE_JP;
  }
  else {
	return KEYBOARD_TYPE_US;
  }
}
#endif // defined(QTB_DEBUG)

// get default output path
QString Settings::getDefaultOutputPath() const
{
#if !QTB_PORTABLE_VERSION
  return QDir::toNativeSeparators(QDir::homePath() + QDir::separator());
#else // QTB_PORTABLE_VERSION
  return QDir::toNativeSeparators(qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator());
#endif // QTB_PORTABLE_VERSION
}

// get default keylayout path
QString Settings::getDefaultKeylayoutPath() const
{
  return QDir::toNativeSeparators(qApp->applicationDirPath() + QTB_KEYLAYOUT_FILE_PATH  + QDir::separator());
}

// get Default Log File
QString Settings::getDefaultLogFile() const
{
#if !QTB_PORTABLE_VERSION
  return QDir::toNativeSeparators(QDir::tempPath() + QDir::separator() + QTB_LOG_FILENAME);
#else // QTB_PORTABLE_VERSION
  return QDir::toNativeSeparators(qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator()
								  + QTB_LOG_FILENAME);
#endif // QTB_PORTABLE_VERSION
}

// get Default Keyboard Log File
QString Settings::getDefaultKeyboardLogFile() const
{
#if !QTB_PORTABLE_VERSION
  return QDir::toNativeSeparators(QDir::tempPath() + QDir::separator() + QTB_KEYBOARDLOG_FILENAME);
#else // QTB_PORTABLE_VERSION
  return QDir::toNativeSeparators(qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator()
								  + QTB_KEYBOARDLOG_FILENAME);
#endif // QTB_PORTABLE_VERSION
}

} // end of namespace qtbrynhildr
