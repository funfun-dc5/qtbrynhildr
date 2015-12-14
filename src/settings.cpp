// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// settings.cpp

// Common Header
#include "common/common.h"

// Syetem Header

// Qt Header
#include <QDir>
#include <QtCore>
#include <QString>

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
#if QTB_CRYPTOGRAM
  cipher(cipher),
#endif // QTB_CRYPTGRAM
  bootupFlag(false),
  shutdownFlag(false),
  connected(false),
#if defined(QTB_BRYNHILDR2_SUPPORT)
  onDisplayCursor(false),
#endif // defined(QTB_BRYNHILDR2_SUPPORT)
  monitorCount(0),
  onShowSoftwareKeyboard(false),
  onShowSoftwareButton(false),
#if defined(QTB_RECORDER)
  onRecordingControl(false),
  onReplayingControl(false),
#endif // defined(QTB_RECORDER)
  onScrollMode(false)
{
  if (iniFileName != 0){
	settings = new QSettings(iniFileName, QSettings::IniFormat);
  }
  else {
	settings =
	  new QSettings(QSettings::IniFormat, QSettings::UserScope, QTB_ORGANIZATION, QTB_APPLICATION);
  }

  // set default
  setServerName(QTB_SERVERNAME_DEFAULT);
  setServerType(QTB_SERVERTYPE_DEFAULT);
  setKeyboardType(getDefaultKeyboardType());
  setPortNo(QTB_PORTNO_DEFAULT);
  setPassword(QTB_PASSWORD_DEFAULT);
  setOnControl(QTB_ONCONTROL_DEFAULT);
  setOnHoldMouseControl(QTB_ONHOLDMOUSECONTROL_DEFAULT);
  setOnControlOffWithGraphicsOff(QTB_ONCONTROLOFFWITHGRAPHICSOFF_DEFAULT);
  setOnGraphics(QTB_ONGRAPHICS_DEFAULT);
  setVideoQuality(QTB_VIDEOQUALITY_DEFAULT);
#if defined(QTB_BRYNHILDR2_SUPPORT)
  setOnDisplayCursor(QTB_ONDISPLAYCURSOR_DEFAULT);
#endif // defined(QTB_BRYNHILDR2_SUPPORT)
  setFrameRate(QTB_FRAMERATE_DEFAULT);
  setOnSound(QTB_ONSOUND_DEFAULT);
  setSoundQuality(QTB_SOUNDQUALITY_DEFAULT);

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

  setOnConfirmAtExit(QTB_ONCONFIRMATEXIT_DEFAULT);

  setOnSaveSettingsAtExit(QTB_ONSAVESETTINGSATEXIT_DEFAULT);

  setOnExitAfterReplay(QTB_ONEXITAFTERREPLAY_DEFAULT);

  setOnStaysOnTop(QTB_ONSTAYSONTOP_DEFAULT);

  setOnFrameLessWindow(QTB_ONFRAMELESSWINDOW_DEFAULT);

  setOnShowMenuBar(QTB_ONSHOWMENUBAR_DEFAULT);
  setOnShowStatusBar(QTB_ONSHOWSTATUSBAR_DEFAULT);

  setOnShowFrameRate(QTB_ONSHOWFRAMERATE_DEFAULT);

  setOnShowPassword(QTB_ONSHOWPASSWORD_DEFAULT);

  setGraphicsBufferSize(QTB_GRAPHICSBUFFERSIZE_DEFAULT);
  setSoundBufferSize(QTB_SOUNDBUFFERSIZE_DEFAULT);

  setDoubleClickThreshold(QTB_DOUBLECLICKTHRESHOLD_DEFAULT);

  setOutputGraphicsDataToFile(QTB_OUTPUTGRAPHICSDATATOFILE_DEFAULT);
  setOutputSoundDataToFile(QTB_OUTPUTSOUNDDATATOFILE_DEFAULT);
  setOutputLog(QTB_OUTPUTLOG_DEFAULT);

  setLogFile(getDefaultLogFile());
}

// destructor
Settings::~Settings()
{
}

// get settings
QSettings *Settings::getSettings() const
{
  return settings;
}

// load settings from setting files or registry
void Settings::readSettings()
{
  // load serverName
  setServerName(settings->value(QTB_SERVERNAME,
								QTB_SERVERNAME_DEFAULT).toString());

  // load serverType
  setServerType(settings->value(QTB_SERVERTYPE,
								(qint32)QTB_SERVERTYPE_DEFAULT).toInt());

  // load keyboardType
  setKeyboardType(settings->value(QTB_KEYBOARDTYPE,
								  (qint32)getDefaultKeyboardType()).toInt());

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

  // load onControlOffWithGraphicsOff
  setOnControlOffWithGraphicsOff(settings->value(QTB_ONCONTROLOFFWITHGRAPHICSOFF,
												 QTB_ONCONTROLOFFWITHGRAPHICSOFF_DEFAULT).toBool());

  // load onGraphics
  setOnGraphics(settings->value(QTB_ONGRAPHICS,
								QTB_ONGRAPHICS_DEFAULT).toBool());

  // load videoQualilty
  setVideoQuality(settings->value(QTB_VIDEOQUALITY,
								  (qint32)QTB_VIDEOQUALITY_DEFAULT).toInt());

  // load frameRate
  setFrameRate(settings->value(QTB_FRAMERATE,
							   (qint32)QTB_FRAMERATE_DEFAULT).toInt());

  // load onSound
  setOnSound(settings->value(QTB_ONSOUND,
							 QTB_ONSOUND_DEFAULT).toBool());

  // load soundQualilty
  setSoundQuality(settings->value(QTB_SOUNDQUALITY,
								 (qint32)QTB_SOUNDQUALITY_DEFAULT).toInt());

  // load soundCapture
  //  setSoundCapture(settings->value(QTB_SOUNDCAPTURE,
  //								 (qint32)QTB_SOUNDCAPTURE_DEFAULT).toInt());

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

  // load onFrameLessWindow
  setOnFrameLessWindow(settings->value(QTB_ONFRAMELESSWINDOW,
									   QTB_ONFRAMELESSWINDOW_DEFAULT).toBool());

  // load onShowMenuBar
  setOnShowMenuBar(settings->value(QTB_ONSHOWMENUBAR,
								   QTB_ONSHOWMENUBAR_DEFAULT).toBool());

  // load onShowStatusBar
  setOnShowStatusBar(settings->value(QTB_ONSHOWSTATUSBAR,
									 QTB_ONSHOWSTATUSBAR_DEFAULT).toBool());

  // load onShowFrameRate
  setOnShowFrameRate(settings->value(QTB_ONSHOWFRAMERATE,
									 QTB_ONSHOWFRAMERATE_DEFAULT).toBool());

  // load onShowPassword
  setOnShowPassword(settings->value(QTB_ONSHOWPASSWORD,
									QTB_ONSHOWPASSWORD_DEFAULT).toBool());

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

  // load outputLog
  setOutputLog(settings->value(QTB_OUTPUTLOG,
							   QTB_OUTPUTLOG_DEFAULT).toBool());

  // load log file
  setLogFile(settings->value(QTB_LOGFILE,
							 getDefaultLogFile()).toString());
}

// save settings to setting file or registry
void Settings::writeSettings()
{
  // save serverName
  settings->setValue(QTB_SERVERNAME, serverName);

  // save serverType
  settings->setValue(QTB_SERVERTYPE, (qint32)serverType);

  // save keyboardType
  settings->setValue(QTB_KEYBOARDTYPE, (qint32)keyboardType);

  // save portNo
  settings->setValue(QTB_PORTNO, portNo);

  // save password
  //  settings->setValue(QTB_PASSWORD, password);

  // save onControl
  settings->setValue(QTB_ONCONTROL, onControl);

  // save onHoldMouseControl
  settings->setValue(QTB_ONHOLDMOUSECONTROL, onHoldMouseControl);

  // save onControlOffWithGraphicsOff
  settings->setValue(QTB_ONCONTROLOFFWITHGRAPHICSOFF, onControlOffWithGraphicsOff);

  // save onGraphics
  settings->setValue(QTB_ONGRAPHICS, onGraphics);

  // save videoQuality
  settings->setValue(QTB_VIDEOQUALITY, (qint32)videoQuality);

  // save frameRate
  settings->setValue(QTB_FRAMERATE, (qint32)frameRate);

  // save onSound
  settings->setValue(QTB_ONSOUND, onSound);

  // save soundQuality
  settings->setValue(QTB_SOUNDQUALITY, (qint32)soundQuality);

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

  // save confirmAtExit
  settings->setValue(QTB_ONCONFIRMATEXIT, onConfirmAtExit);

  // save saveSettingsAtExit
  settings->setValue(QTB_ONSAVESETTINGSATEXIT, onSaveSettingsAtExit);

  // save exit after replay
  settings->setValue(QTB_ONEXITAFTERREPLAY, onExitAfterReplay);

  // save onStaysOnTop
  settings->setValue(QTB_ONSTAYSONTOP, onStaysOnTop);

  // save onFrameLessWindow
  settings->setValue(QTB_ONFRAMELESSWINDOW, onFrameLessWindow);

  // save onShowMenuBar
  settings->setValue(QTB_ONSHOWMENUBAR, onShowMenuBar);

  // save onShowMenuBar
  settings->setValue(QTB_ONSHOWMENUBAR, onShowMenuBar);

  // save onShowFrameRate
  settings->setValue(QTB_ONSHOWFRAMERATE, onShowFrameRate);

  // save onShowPassword
  settings->setValue(QTB_ONSHOWPASSWORD, onShowPassword);

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

  // save outputLog
  settings->setValue(QTB_OUTPUTLOG, outputLog);

  // save logFile
  settings->setValue(QTB_LOGFILE, logFile);

  // sync
  settings->sync();
}

// for log
void Settings::printSettings() const
{
  qDebug() << "---------------- Settings ----------------";
  qDebug() << "Server Name   : " << serverName;
  qDebug() << "Server Type   : " << getServerTypeByString();
  qDebug() << "Keyboard Type : " << getKeyboardTypeByString();
  qDebug() << "Port No       : " << portNo;
  qDebug() << "Password      : " << password;
  qDebug() << "Control       : " << onControl;
#if defined(QTB_BRYNHILDR2_SUPPORT)
  qDebug() << "DisplayCursor : " << onDisplayCursor;
#endif // defined(QTB_BRYNHILDR2_SUPPORT)
  qDebug() << "  HoldMouseControl : " << onHoldMouseControl;
  qDebug() << "  ControlOffWithGraphicsOff : " << onControlOffWithGraphicsOff;
  qDebug() << "Graphics      : " << onGraphics;
  qDebug() << "    QUALITY   : " << getVideoQualityByString();
  qDebug() << "    FrameRate : " << frameRate;
  qDebug() << "Sound         : " << onSound;
  qDebug() << "    QUALITY   : " << getSoundQualityByString();
  qDebug() << "    CAPTURE   : " << getSoundCaptureByString();

  qDebug() << "KeepOriginalDesktopSize : " << onKeepOriginalDesktopSize;
  qDebug() << "DesktopScalingFactor    : " << desktopScalingFactor;
  qDebug() << "DesktopScalingQuality   : " << desktopScalingQuality;
  qDebug() << "DesktopScalingType      : " << desktopScalingType;

  qDebug() << "CutDesktopBlankArea     : " << onCutDesktopBlankArea;

  qDebug() << "DesktopOffsetX          : " << desktopOffsetX;
  qDebug() << "DesktopOffsetY          : " << desktopOffsetY;

  qDebug() << "MonitorNo               : " << monitorNo;
  qDebug() << "ConfirmAtExit           : " << onConfirmAtExit;
  qDebug() << "SaveSettingsAtExit      : " << onSaveSettingsAtExit;
  qDebug() << "ExitAfterReplay         : " << onExitAfterReplay;
  qDebug() << "StaysOnTop              : " << onStaysOnTop;
  qDebug() << "FrameLessWindow         : " << onFrameLessWindow;
  qDebug() << "ShowMenuBar             : " << onShowMenuBar;
  qDebug() << "ShowStatusBar           : " << onShowStatusBar;
  qDebug() << "ShowFrameRate           : " << onShowFrameRate;
  qDebug() << "ShowPassword            : " << onShowPassword;

  qDebug() << "------------------------------------------";
  qDebug() << "Graphics Buffer Size (bytes) : " << graphicsBufferSize;
  qDebug() << "Sound    Buffer Size (bytes) : " << soundBufferSize;

  qDebug() << "Double Click Threshold(msecond) : " << doubleClickThreshold;

  qDebug() << "------------------------------------------";
  qDebug() << "DBG: output Graphics Data To File : " << outputGraphicsDataToFile;
  qDebug() << "DBG: output Sound    Data To File : " << outputSoundDataToFile;
  qDebug() << "DBG: output Log : " << outputLog;
  qDebug() << "------------------------------------------";
  qDebug() << "Log File : " << logFile;
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

// get Default Log File
QString Settings::getDefaultLogFile() const
{
  return QDir::toNativeSeparators(QDir::tempPath() + QDir::separator() + QTB_LOG_FILENAME);
}

} // end of namespace qtbrynhildr
