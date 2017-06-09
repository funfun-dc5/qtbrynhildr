// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// Syetem Header

// Qt Header
#include <QDir>
#include <QString>
#if 1 // for qDebug()
#include <QtCore>
#endif // for TEST

// Local Header
#include "settings.h"

namespace qtbrynhildr {

// constructor
#if QTB_CRYPTOGRAM
Settings::Settings(const char *iniFileName, Cipher *cipher)
#else // QTB_CRYPTGRAM
Settings::Settings(const char *iniFileName)
#endif // QTB_CRYPTGRAM
  :
  desktop(new Desktop()),
  bootupFlag(false),
  shutdownFlag(false),
#if QTB_CRYPTOGRAM
  cipher(cipher),
#endif // QTB_CRYPTGRAM
#if QTB_AUTO_COMPLETE
  serverNameListSize(QTB_SERVERNAMELISTSIZE_DEFAULT),
  serverNameList(new QStringList),
#endif // QTB_AUTO_COMPLETE
  connected(false),
#if QTB_PUBLIC_MODE6_SUPPORT
  onSendClipboard(false),
  sendFileCount(0),
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if QTB_BRYNHILDR2_SUPPORT
  onDisplayCursor(false),
  onSupportGamePad(false),
  onDisableBrynhildr2Support(false),
#endif // QTB_BRYNHILDR2_SUPPORT
  frameDrawTime(0),
  monitorCount(0),
  onShowSoftwareKeyboard(false),
  onShowSoftwareButton(false),
#if QTB_RECORDER
  onRecordingControl(false),
  recordingControlFileName(0),
  onReplayingControl(false),
  replayingControlFileName(0),
#endif // QTB_RECORDER
  onDesktopCapture(false),
  onScrollMode(false)
{
  if (iniFileName != 0){
	settings = new QSettings(iniFileName, QSettings::IniFormat);
  }
  else {
	settings =
	  new QSettings(QSettings::IniFormat, QSettings::UserScope, QTB_ORGANIZATION, QTB_APPLICATION);
  }

  // set version information
  setGeneratedVersion(QTB_GENERATEDVERSION_DEFAULT);
  setCurrentVersion(QTB_CURRENTVERSION_DEFAULT);

  // set default
#if QTB_PUBLIC_MODE6_SUPPORT
  setPublicModeVersion(QTB_PUBLICMODEVERSION_DEFAULT);
#endif // QTB_PUBLIC_MODE6_SUPPORT
  setServerName(QTB_SERVERNAME_DEFAULT);
  setServerType(QTB_SERVERTYPE_DEFAULT);
  setKeyboardType(getDefaultKeyboardType());
  setKeyboardTypeName(QTB_KEYBOARDTYPENAME_DEFAULT);
  setPortNo(QTB_PORTNO_DEFAULT);
  setPassword(QTB_PASSWORD_DEFAULT);
  setOnControl(QTB_ONCONTROL_DEFAULT);
#if QTB_PLUGINS_DISABLE_SUPPORT
  setOnPluginsDisable(QTB_ONPLUGINSDISABLE_DEFAULT);
#endif // QTB_PLUGINS_DISABLE_SUPPORT
  setOnHoldMouseControl(QTB_ONHOLDMOUSECONTROL_DEFAULT);
#if QTB_EXTRA_BUTTON_SUPPORT
  setOnExtraButtonSupport(QTB_ONEXTRABUTTONSUPPORT_DEFAULT);
#endif // QTB_EXTRA_BUTTON_SUPPORT
  setOnControlOffWithGraphicsOff(QTB_ONCONTROLOFFWITHGRAPHICSOFF_DEFAULT);
  setOnGraphics(QTB_ONGRAPHICS_DEFAULT);
  setVideoQuality(QTB_VIDEOQUALITY_DEFAULT);
#if QTB_BRYNHILDR2_SUPPORT
  setOnDisplayCursor(QTB_ONDISPLAYCURSOR_DEFAULT);
  setOnSupportGamePad(QTB_ONSUPPORTGAMEPAD_DEFAULT);
  setOnDisableBrynhildr2Support(QTB_ONDISABLEBRYNHILDR2SUPPORT_DEFAULT);
#endif // QTB_BRYNHILDR2_SUPPORT
  setFrameRate(QTB_FRAMERATE_DEFAULT);
  setOnSound(QTB_ONSOUND_DEFAULT);
#if QTB_CELT_SUPPORT
  setSoundType(QTB_SOUNDTYPE_DEFAULT);
#endif // QTB_CELT_SUPPORT
  setSoundQuality(QTB_SOUNDQUALITY_DEFAULT);
  setSoundCacheTime(QTB_SOUNDCACHETIME_DEFAULT);

  setOnKeepOriginalDesktopSize(QTB_ONKEEPORIGINALDESKTOPSIZE_DEFAULT);
  setDesktopScalingFactor(QTB_DESKTOPSCALINGFACTOR_DEFAULT);
  setDesktopScalingQuality(QTB_DESKTOPSCALINGQUALITY_DEFAULT);
  setDesktopScalingType(QTB_DESKTOPSCALINGTYPE_DEFAULT);

  setOnCutDesktopBlankArea(QTB_ONCUTDESKTOPBLANKAREA_DEFAULT);

  setDesktopOffsetX(QTB_DESKTOPOFFSETX_DEFAULT);
  setDesktopOffsetY(QTB_DESKTOPOFFSETY_DEFAULT);

  setDesktopWidth(QTB_MAX_SERVER_DESKTOP_WIDTH);
  setDesktopHeight(QTB_MAX_SERVER_DESKTOP_WIDTH);

  setMonitorNo(QTB_MONITOR_NO_DEFAULT);

  setOnOpenConnectToServerDialogAtBootup(QTB_ONOPENCONNECTTOSERVERDIALOGATBOOTUP_DEFAULT);

  setOnCheckUpdateAtBootup(QTB_ONCHECKUPDATEATBOOTUP_DEFAULT);

  setOnConfirmAtExit(QTB_ONCONFIRMATEXIT_DEFAULT);

  setOnSaveSettingsAtExit(QTB_ONSAVESETTINGSATEXIT_DEFAULT);

  setOnExitAfterReplay(QTB_ONEXITAFTERREPLAY_DEFAULT);

  setOnStaysOnTop(QTB_ONSTAYSONTOP_DEFAULT);

  setOnDesktopScaleFixed(QTB_ONDESKTOPSCALEFIXED_DEFAULT);

  setOnWindowSizeFixed(QTB_ONWINDOWSIZEFIXED_DEFAULT);

  setOnFrameLessWindow(QTB_ONFRAMELESSWINDOW_DEFAULT);

  setOnShowMenuBar(QTB_ONSHOWMENUBAR_DEFAULT);
  setOnShowStatusBar(QTB_ONSHOWSTATUSBAR_DEFAULT);

  setOnFullScreenAtConnected(QTB_ONFULLSCREENATCONNECTED_DEFAULT);

  setOnHideMenuAndStatusBarAtFullScreen(QTB_ONHIDEMENUANDSTATUSBARATFULLSCREEN_DEFAULT);

  setOnShowFrameRate(QTB_ONSHOWFRAMERATE_DEFAULT);

  setOnShowPassword(QTB_ONSHOWPASSWORD_DEFAULT);

  setOnClipCursor(QTB_ONCLIPCURSOR_DEFAULT);

  setOnShowMarker(QTB_ONSHOWMARKER_DEFAULT);

#if QTB_PUBLIC_MODE6_SUPPORT
  setOnDisableTransferFile(QTB_ONDISABLETRANSFERFILE_DEFAULT);
  setOnShowTotalProgressForTransferFile(QTB_ONDISABLETRANSFERFILE_DEFAULT);
#if QTB_DRAG_AND_DROP_SUPPORT
  setOnDisableTransferFileByDragAndDrop(QTB_ONDISABLETRANSFERFILEBYDRAGANDDROP_DEFAULT);
#endif // QTB_DRAG_AND_DROP_SUPPORT
  setOnDisableTransferClipboard(QTB_ONDISABLETRANSFERCLIPBOARD_DEFAULT);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  setGraphicsBufferSize(QTB_GRAPHICSBUFFERSIZE_DEFAULT);
  setSoundBufferSize(QTB_SOUNDBUFFERSIZE_DEFAULT);

  setDoubleClickThreshold(QTB_DOUBLECLICKTHRESHOLD_DEFAULT);

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
  if (desktop != 0){
	delete desktop;
	desktop = 0;
  }
#if QTB_AUTO_COMPLETE
  if (serverNameList != 0){
	delete serverNameList;
	serverNameList = 0;
  }
#endif // QTB_AUTO_COMPLETE
}

// get settings
QSettings *Settings::getSettings() const
{
  return settings;
}

#if QTB_AUTO_COMPLETE
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
  int serverNameListNum = serverNameList->size();
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
#endif // QTB_AUTO_COMPLETE

// load settings from setting files or registry
void Settings::readSettings()
{
  // load version information
  setGeneratedVersion(settings->value(QTB_GENERATEDVERSION,
									  (qint32)QTB_GENERATEDVERSION_DEFAULT).toInt());

#if QTB_PUBLIC_MODE6_SUPPORT
  // load publicModeVersion
  setPublicModeVersion(settings->value(QTB_PUBLICMODEVERSION,
									   (qint32)QTB_PUBLICMODEVERSION_DEFAULT).toInt());
#endif //QTB_PUBLIC_MODE6_SUPPORT

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
  //  password = settings->value(QTB_PASSWORD,
  //							 QTB_PASSWORD_DEFAULT).toString();

  // load onControl
  setOnControl(settings->value(QTB_ONCONTROL,
							   QTB_ONCONTROL_DEFAULT).toBool());

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

  // load onGraphics
  setOnGraphics(settings->value(QTB_ONGRAPHICS,
								QTB_ONGRAPHICS_DEFAULT).toBool());

  // load videoQualilty
  setVideoQuality(settings->value(QTB_VIDEOQUALITY,
								  (qint32)QTB_VIDEOQUALITY_DEFAULT).toInt());

#if QTB_BRYNHILDR2_SUPPORT
  // load onDisplayCursor
  setOnDisplayCursor(settings->value(QTB_ONDISPLAYCURSOR,
									 QTB_ONDISPLAYCURSOR_DEFAULT).toBool());

  // load onSupportGamePad
  setOnSupportGamePad(settings->value(QTB_ONSUPPORTGAMEPAD,
									  QTB_ONSUPPORTGAMEPAD_DEFAULT).toBool());

  // load onDisableBrynhildr2Support
  setOnDisableBrynhildr2Support(settings->value(QTB_ONDISABLEBRYNHILDR2SUPPORT,
												QTB_ONDISABLEBRYNHILDR2SUPPORT_DEFAULT).toBool());
#endif // QTB_BRYNHILDR2_SUPPORT

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

  // load onKeepOriginalDesktopSize
  setOnKeepOriginalDesktopSize(settings->value(QTB_ONKEEPORIGINALDESKTOPSIZE,
											   QTB_ONKEEPORIGINALDESKTOPSIZE_DEFAULT).toBool());
  // load desctopScalingFactor
  setDesktopScalingFactor(settings->value(QTB_DESKTOPSCALINGFACTOR,
										  (qreal)QTB_DESKTOPSCALINGFACTOR_DEFAULT).toFloat());

  // load desctopScalingQuality
  setDesktopScalingQuality((Qt::TransformationMode)settings->value(QTB_DESKTOPSCALINGQUALITY,
																   QTB_DESKTOPSCALINGQUALITY_DEFAULT).toInt());

  // load desktopScalingType
  setDesktopScalingType(settings->value(QTB_DESKTOPSCALINGTYPE,
								 (qint32)QTB_DESKTOPSCALINGTYPE_DEFAULT).toInt());

  // load onCutDesktopBlankArea
  setOnCutDesktopBlankArea(settings->value(QTB_ONCUTDESKTOPBLANKAREA,
										   QTB_ONCUTDESKTOPBLANKAREA_DEFAULT).toBool());

  // load desktopOffsetX
  setDesktopOffsetX(settings->value(QTB_DESKTOPOFFSETX,
							   (qint32)QTB_DESKTOPOFFSETX_DEFAULT).toInt());

  // load desktopOffsetY
  setDesktopOffsetY(settings->value(QTB_DESKTOPOFFSETY,
							   (qint32)QTB_DESKTOPOFFSETY_DEFAULT).toInt());

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

  // load onFrameLessWindow
  setOnFrameLessWindow(settings->value(QTB_ONFRAMELESSWINDOW,
									   QTB_ONFRAMELESSWINDOW_DEFAULT).toBool());

  // load onShowMenuBar
  setOnShowMenuBar(settings->value(QTB_ONSHOWMENUBAR,
								   QTB_ONSHOWMENUBAR_DEFAULT).toBool());

  // load onShowStatusBar
  setOnShowStatusBar(settings->value(QTB_ONSHOWSTATUSBAR,
									 QTB_ONSHOWSTATUSBAR_DEFAULT).toBool());

  // load onFullScreenAtConnected
  setOnFullScreenAtConnected(settings->value(QTB_ONFULLSCREENATCONNECTED,
											 QTB_ONFULLSCREENATCONNECTED_DEFAULT).toBool());

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

  // load onShowMarker
  setOnShowMarker(settings->value(QTB_ONSHOWMARKER,
								  QTB_ONSHOWMARKER_DEFAULT).toBool());

#if QTB_PUBLIC_MODE6_SUPPORT
  // load onDisableTansferFile
  setOnDisableTransferFile(settings->value(QTB_ONDISABLETRANSFERFILE,
										   QTB_ONDISABLETRANSFERFILE_DEFAULT).toBool());
  // load onShowTotalProgressForTransferFile
  setOnShowTotalProgressForTransferFile(settings->value(QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE,
														QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE_DEFAULT).toBool());
#if QTB_DRAG_AND_DROP_SUPPORT
  // load onDisableTansferFileByDragAndDrop
  setOnDisableTransferFileByDragAndDrop(settings->value(QTB_ONDISABLETRANSFERFILEBYDRAGANDDROP,
														QTB_ONDISABLETRANSFERFILEBYDRAGANDDROP_DEFAULT).toBool());
#endif // QTB_DRAG_AND_DROP_SUPPORT

  // load onDisableTansferClipboard
  setOnDisableTransferClipboard(settings->value(QTB_ONDISABLETRANSFERCLIPBOARD,
												QTB_ONDISABLETRANSFERCLIPBOARD_DEFAULT).toBool());
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // load graphicsBufferSize
  setGraphicsBufferSize(settings->value(QTB_GRAPHICSBUFFERSIZE,
										QTB_GRAPHICSBUFFERSIZE_DEFAULT).toInt());

  // load soundBufferSize
  setSoundBufferSize(settings->value(QTB_SOUNDBUFFERSIZE,
									 QTB_SOUNDBUFFERSIZE_DEFAULT).toInt());

  // load doubleClickThreshold
  setDoubleClickThreshold(settings->value(QTB_DOUBLECLICKTHRESHOLD,
										  QTB_DOUBLECLICKTHRESHOLD_DEFAULT).toInt());

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
  //  setOutputKeyboardLog(settings->value(QTB_OUTPUTKEYBOARDLOG,
  //									   QTB_OUTPUTKEYBOARDLOG_DEFAULT).toBool());

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

  // load desktop capture format
  setDesktopCaptureFormat(settings->value(QTB_DESKTOPCAPTUREFORMAT,
										  QTB_DESKTOPCAPTUREFORMAT_DEFAULT).toString());

#if QTB_AUTO_COMPLETE
  // read server name list
  readServerNameList();
#endif // QTB_AUTO_COMPLETE
}

// save settings to setting file or registry
void Settings::writeSettings()
{
  // save version information
  settings->setValue(QTB_GENERATEDVERSION, (qint32)generatedVersion);
  settings->setValue(QTB_CURRENTVERSION, (qint32)currentVersion);

#if QTB_PUBLIC_MODE6_SUPPORT
  // save publicModeVersion
  settings->setValue(QTB_PUBLICMODEVERSION, (qint32)publicModeVersion);
#endif // QTB_PUBLIC_MODE6_SUPPORT

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

  // save onHoldMouseControl
  settings->setValue(QTB_ONHOLDMOUSECONTROL, onHoldMouseControl);

#if QTB_EXTRA_BUTTON_SUPPORT
  // save onExtraButtonSupport
  settings->setValue(QTB_ONEXTRABUTTONSUPPORT, onExtraButtonSupport);
#endif // QTB_EXTRA_BUTTON_SUPPORT

  // save onControlOffWithGraphicsOff
  settings->setValue(QTB_ONCONTROLOFFWITHGRAPHICSOFF, onControlOffWithGraphicsOff);

  // save onGraphics
  settings->setValue(QTB_ONGRAPHICS, onGraphics);

  // save videoQuality
  settings->setValue(QTB_VIDEOQUALITY, (qint32)videoQuality);

#if QTB_BRYNHILDR2_SUPPORT
  // save onDisplayCursor
  settings->setValue(QTB_ONDISPLAYCURSOR, onDisplayCursor);

  // save onSupportGamePad
  settings->setValue(QTB_ONSUPPORTGAMEPAD, onSupportGamePad);

  // save onDisableBrynhildr2Support
  settings->setValue(QTB_ONDISABLEBRYNHILDR2SUPPORT, onDisableBrynhildr2Support);
#endif // QTB_BRYNHILDR2_SUPPORT

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

  // save onKeepOriginalDesktopSize
  settings->setValue(QTB_ONKEEPORIGINALDESKTOPSIZE, onKeepOriginalDesktopSize);

  //  save desktopScalingFactor
  settings->setValue(QTB_DESKTOPSCALINGFACTOR, (qreal)desktopScalingFactor);

  //  save desktopScalingQuality
  settings->setValue(QTB_DESKTOPSCALINGQUALITY, desktopScalingQuality);

  // save desktopScalingType
  settings->setValue(QTB_DESKTOPSCALINGTYPE, (qint32)desktopScalingType);

  // save onCutDesktopBlankArea
  settings->setValue(QTB_ONCUTDESKTOPBLANKAREA, onCutDesktopBlankArea);

  // save desktop offset X
  settings->setValue(QTB_DESKTOPOFFSETX, (qint32)desktopOffsetX);

  // save desktop offset Y
  settings->setValue(QTB_DESKTOPOFFSETY, (qint32)desktopOffsetY);

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

  // save onFrameLessWindow
  settings->setValue(QTB_ONFRAMELESSWINDOW, onFrameLessWindow);

  // save onShowMenuBar
  settings->setValue(QTB_ONSHOWMENUBAR, onShowMenuBar);

  // save onShowStatusBar
  settings->setValue(QTB_ONSHOWSTATUSBAR, onShowStatusBar);

  // save onFullScreenAtConnected
  settings->setValue(QTB_ONFULLSCREENATCONNECTED, onFullScreenAtConnected);

  // save onHideMenuAndStatusBarAtFullScreen
  settings->setValue(QTB_ONHIDEMENUANDSTATUSBARATFULLSCREEN, onHideMenuAndStatusBarAtFullScreen);

  // save onShowFrameRate
  settings->setValue(QTB_ONSHOWFRAMERATE, onShowFrameRate);

  // save onShowPassword
  settings->setValue(QTB_ONSHOWPASSWORD, onShowPassword);

  // save onClipCursor
  settings->setValue(QTB_ONCLIPCURSOR, onClipCursor);

  // save onShowMarker
  settings->setValue(QTB_ONSHOWMARKER, onShowMarker);

#if QTB_PUBLIC_MODE6_SUPPORT
  // save onDisableTransferFile
  settings->setValue(QTB_ONDISABLETRANSFERFILE, onDisableTransferFile);
  // save onShowTotalProgressForTransferFile
  settings->setValue(QTB_ONSHOWTOTALPROGRESSFORTRANSFERFILE, onShowTotalProgressForTransferFile);
#if QTB_DRAG_AND_DROP_SUPPORT
  // save onDisableTransferFileByDragAndDrop
  settings->setValue(QTB_ONDISABLETRANSFERFILEBYDRAGANDDROP, onDisableTransferFileByDragAndDrop);
#endif // QTB_DRAG_AND_DROP_SUPPORT
  // save onDisableTransferClipboard
  settings->setValue(QTB_ONDISABLETRANSFERCLIPBOARD, onDisableTransferClipboard);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // save graphicsBufferSize
  settings->setValue(QTB_GRAPHICSBUFFERSIZE, graphicsBufferSize);

  // save soundBufferSize
  settings->setValue(QTB_SOUNDBUFFERSIZE, soundBufferSize);

  // save doubleClickThreshold
  settings->setValue(QTB_DOUBLECLICKTHRESHOLD, doubleClickThreshold);

  // save outputGraphicsDataToFile
  settings->setValue(QTB_OUTPUTGRAPHICSDATATOFILE, outputGraphicsDataToFile);

  // save outputSoundDataToFile
  settings->setValue(QTB_OUTPUTSOUNDDATATOFILE, outputSoundDataToFile);

  // save outputSoundDataToWavFile
  settings->setValue(QTB_OUTPUTSOUNDDATATOWAVFILE, outputSoundDataToWavFile);

  // save outputLog
  settings->setValue(QTB_OUTPUTLOG, outputLog);

  // save outputKeyboardLog
  //  settings->setValue(QTB_OUTPUTKEYBOARDLOG, outputKeyboardLog);

  // save outputPath
  settings->setValue(QTB_OUTPUTPATH, outputPath);

  // save keylayoutPath
  settings->setValue(QTB_KEYLAYOUTPATH, keylayoutPath);

  // save logFile
  settings->setValue(QTB_LOGFILE, logFile);

  // save keyboardLogFile
  settings->setValue(QTB_KEYBOARDLOGFILE, keyboardLogFile);

  // save desktopCaptureFormat
  settings->setValue(QTB_DESKTOPCAPTUREFORMAT, desktopCaptureFormat);

#if QTB_AUTO_COMPLETE
  // write server name list
  writeServerNameList();
#endif // QTB_AUTO_COMPLETE

  // sync
  settings->sync();
}

// for log
void Settings::printSettings() const
{
  qDebug() << "---------------- Settings ----------------";
  qDebug() << "Generated Version : " << generatedVersion;
  qDebug() << "Current Version   : " << currentVersion;
#if QTB_PUBLIC_MODE6_SUPPORT
  qDebug() << "Public Mode Version: " << getPublicModeVersionByString();
#endif // QTB_PUBLIC_MODE6_SUPPORT
  qDebug() << "------------------------------------------";
  qDebug() << "Server Name   : " << serverName;
  qDebug() << "Server Type   : " << getServerTypeByString();
  qDebug() << "Keyboard Type : " << getKeyboardTypeByString();
  qDebug() << "Port No       : " << portNo;
  qDebug() << "Password      : " << password;
  qDebug() << "Control       : " << onControl;
#if QTB_PLUGINS_DISABLE_SUPPORT
  qDebug() << "PluginsDisable: " << onPluginsDisable;
#endif // QTB_PLUGINS_DISABLE_SUPPORT
#if QTB_BRYNHILDR2_SUPPORT
  qDebug() << "DisplayCursor : " << onDisplayCursor;
  qDebug() << "SupportGamePad: " << onSupportGamePad;
#endif // QTB_BRYNHILDR2_SUPPORT
  qDebug() << "  HoldMouseControl : " << onHoldMouseControl;
#if QTB_EXTRA_BUTTON_SUPPORT
  qDebug() << "  ExtraButtonSupport : " << onExtraButtonSupport;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  qDebug() << "  ControlOffWithGraphicsOff : " << onControlOffWithGraphicsOff;
#if QTB_PUBLIC_MODE6_SUPPORT
  qDebug() << "  SendClipboard : " << onSendClipboard;
  qDebug() << "  SendFileCount : " << sendFileCount;
#endif // QTB_PUBLIC_MODE6_SUPPORT
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

  qDebug() << "KeepOriginalDesktopSize : " << onKeepOriginalDesktopSize;
  qDebug() << "DesktopScalingFactor    : " << desktopScalingFactor;
  qDebug() << "DesktopScalingQuality   : " << desktopScalingQuality;
  qDebug() << "DesktopScalingType      : " << desktopScalingType;

  qDebug() << "CutDesktopBlankArea     : " << onCutDesktopBlankArea;

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
  qDebug() << "FrameLessWindow         : " << onFrameLessWindow;
  qDebug() << "ShowMenuBar             : " << onShowMenuBar;
  qDebug() << "ShowStatusBar           : " << onShowStatusBar;
  qDebug() << "FullScreenAtConnected   : " << onFullScreenAtConnected;
  qDebug() << "HideMenuAndStatusBarAtFullScreen : " << onHideMenuAndStatusBarAtFullScreen;
  qDebug() << "ShowFrameRate           : " << onShowFrameRate;
  qDebug() << "ShowPassword            : " << onShowPassword;
  qDebug() << "ClipCursor              : " << onClipCursor;
  qDebug() << "ShowMarker              : " << onShowMarker;
#if QTB_PUBLIC_MODE6_SUPPORT
  qDebug() << "DisableTransferFile     : " << onDisableTransferFile;
  qDebug() << "ShowTotalProgressForTransferFile     : " << onShowTotalProgressForTransferFile;
#if QTB_DRAG_AND_DROP_SUPPORT
  qDebug() << "DisableTransferFileByDragAndDrop     : " << onDisableTransferFileByDragAndDrop;
#endif // QTB_DRAG_AND_DROP_SUPPORT
  qDebug() << "DisableTransferClipboard: " << onDisableTransferClipboard;
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_BRYNHILDR2_SUPPORT
  qDebug() << "DisableBrynhildr2Support: " << onDisableBrynhildr2Support;
#endif // QTB_BRYNHILDR2_SUPPORT

  qDebug() << "------------------------------------------";
  qDebug() << "Graphics Buffer Size (bytes) : " << graphicsBufferSize;
  qDebug() << "Sound    Buffer Size (bytes) : " << soundBufferSize;

  qDebug() << "Double Click Threshold(msecond) : " << doubleClickThreshold;

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

// get default output path
QString Settings::getDefaultOutputPath() const
{
  return QDir::toNativeSeparators(QDir::homePath() + QDir::separator());
}

// get default keylayout path
QString Settings::getDefaultKeylayoutPath() const
{
#if 0 // for TEST
  QFileInfo fileInfo(settings->fileName());
  return QDir::toNativeSeparators(fileInfo.absolutePath() + QTB_KEYLAYOUT_FILE_PATH);
#else
  return QDir::toNativeSeparators(qApp->applicationDirPath() + QTB_KEYLAYOUT_FILE_PATH);
#endif
}

// get Default Log File
QString Settings::getDefaultLogFile() const
{
  return QDir::toNativeSeparators(QDir::tempPath() + QDir::separator() + QTB_LOG_FILENAME);
}

// get Default Keyboard Log File
QString Settings::getDefaultKeyboardLogFile() const
{
  return QDir::toNativeSeparators(QDir::tempPath() + QDir::separator() + QTB_KEYBOARDLOG_FILENAME);
}

} // end of namespace qtbrynhildr
