// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QDateTime>

// Local Header
#include "util/logmessage.h"

namespace qtbrynhildr {

// phase name
const QString phaseName[PHASE_NUM] = {
  "[main]     ",
  "[control]  ",
  "[graphics] ",
  "[sound]    ",
  "[debug]    ",
};

// date format
const QString dateFormat = QTB_LOG_DATE_FORMAT;

// string for kind of message
#define KIND_OF_CRITICAL    ": <critical>    : "
#define KIND_OF_WARNING     ": <warning>     : "
#define KIND_OF_INFORMATION ": <information> : "
#define KIND_OF_QUESTION    ": <question>    : "
#define KIND_OF_NORMAL      ": <normal>      : "


// constructor
LogMessage::LogMessage(QWidget *parent)
  :QMessageBox(parent)
  ,settings(nullptr)
  ,logFile(nullptr)
  ,logFileStream(nullptr)
{
}

// destructor
LogMessage::~LogMessage()
{
  // delete objects
  if (logFile != nullptr){
	delete logFile;
	logFile = nullptr;
  }
  if (logFileStream != nullptr){
	delete logFileStream;
	logFileStream = nullptr;
  }
}

// open log file
bool LogMessage::openLogFile()
{
  if (logFile != nullptr)
	return false;

  logFile = new QFile(settings->getLogFile());

  // check
  if (!logFile->open(QFile::WriteOnly | QFile::Append)) {
	return false;
  }

  // create QTextStrem
  logFileStream = new QTextStream(logFile);
#if QT_VERSION < 0x060000
  logFileStream->setCodec("UTF-8");
#endif // QT_VERSION < 0x060000

  return true;
}

// close log file
bool LogMessage::closeLogFile()
{
  if (logFile == nullptr)
	return false;

  if (logFileStream == nullptr)
	return false;

  // flush
  logFileStream->flush();

  // close
  if (logFile->isOpen()){
	logFile->close();
  }

  // delete object
  delete logFileStream;
  logFileStream = nullptr;

  delete logFile;
  logFile = nullptr;

  return true;
}

// critical message
int LogMessage::criticalMessage(PHASE_ID id,
								const QString text,
								StandardButton buttons,
								StandardButton defaultButton)
{
  int ret;

  // output dialog
  ret = critical(parentWidget(),
				 QTB_APPLICATION,
				 phaseName[id] + text,
				 buttons,
				 defaultButton);

  // open log file
  if (!openLogFile()){
	return ret;
  }

  // output log file
  if (hasValidLogFileStream()){
	QDateTime dateTime = QDateTime::currentDateTime();
	(*logFileStream) << phaseName[id] << KIND_OF_CRITICAL;
	(*logFileStream) << dateTime.toString(dateFormat) << " : " << text << QTextStream_endl << QTextStream_flush;
  }

  // close log file
  closeLogFile();

  return ret;
}

// warning message
int LogMessage::warningMessage(PHASE_ID id,
							   const QString text,
							   StandardButton buttons,
							   StandardButton defaultButton)
{
  int ret;

  // output dialog
  ret = warning(parentWidget(),
				QTB_APPLICATION,
				phaseName[id] + text,
				buttons,
				defaultButton);

  // open log file
  if (!openLogFile()){
	return ret;
  }

  // output log file
  if (hasValidLogFileStream()){
	QDateTime dateTime = QDateTime::currentDateTime();
	(*logFileStream) << phaseName[id] << KIND_OF_WARNING;
	(*logFileStream) << dateTime.toString(dateFormat)   << " : " << text << QTextStream_endl << QTextStream_flush;
  }

  // close log file
  closeLogFile();

  return ret;
}

// information message
int LogMessage::informationMessage(PHASE_ID id,
								   const QString text,
								   StandardButton buttons,
								   StandardButton defaultButton)
{
  int ret;

  // output dialog
  ret = information(parentWidget(),
					QTB_APPLICATION,
					phaseName[id] + text,
					buttons,
					defaultButton);

  // open log file
  if (!openLogFile()){
	return ret;
  }

  // output log file
  if (hasValidLogFileStream()){
	QDateTime dateTime = QDateTime::currentDateTime();
	(*logFileStream) << phaseName[id] << KIND_OF_INFORMATION;
	(*logFileStream) << dateTime.toString(dateFormat) << " : " << text << QTextStream_endl << QTextStream_flush;
  }

  // close log file
  closeLogFile();

  return ret;
}

// question message
int LogMessage::questionMessage(PHASE_ID id,
								const QString text,
								StandardButton buttons,
								StandardButton defaultButton)
{
  int ret;

  // output dialog
  ret = question(parentWidget(),
				 QTB_APPLICATION,
				 phaseName[id] + text,
				 buttons,
				 defaultButton);

  // open log file
  if (!openLogFile()){
	return ret;
  }

  // output log file
  if (hasValidLogFileStream()){
	QDateTime dateTime = QDateTime::currentDateTime();
	(*logFileStream) << phaseName[id] << KIND_OF_QUESTION;
	(*logFileStream) << dateTime.toString(dateFormat) << " : " << text << QTextStream_endl << QTextStream_flush;
  }

  // close log file
  closeLogFile();

  return ret;
}

// normal log message
void LogMessage::outputLogMessage(PHASE_ID id, const QString text)
{
  // open log file
  if (!openLogFile()){
	return;
  }

  QDateTime dateTime = QDateTime::currentDateTime();
  if (hasValidLogFileStream()){
	(*logFileStream) << phaseName[id] << KIND_OF_NORMAL;
	(*logFileStream) << dateTime.toString(dateFormat) << " : " << text << QTextStream_endl << QTextStream_flush;
  }

  // close log file
  closeLogFile();
}

// output message
void LogMessage::outputMessage(int msgID)
{
  switch(msgID){
  case QTB_MSG_CONNECT_ERROR:
	criticalMessage(PHASE_QTBRYNHILDR,
					tr("Failed to connect server!"),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	break;
  case QTB_MSG_CONNECT_TIMEOUT:
	criticalMessage(PHASE_QTBRYNHILDR,
					tr("Timeout to connect server!"),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	break;
  case QTB_MSG_PASSWORD_ERROR:
	criticalMessage(PHASE_QTBRYNHILDR,
					tr("Password Error!"),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	break;
  case QTB_MSG_NOTSUPPORT_SAMPLE_RATE:
	criticalMessage(PHASE_QTBRYNHILDR,
					tr("Please check server sound device!"),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	break;
  case QTB_MSG_NOT_FOUND_SOUND_DEVICE:
	criticalMessage(PHASE_QTBRYNHILDR,
					tr("Not found sound device!"),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	break;
  case QTB_MSG_NOTSUPPORT_VIDEO_MODE:
	criticalMessage(PHASE_QTBRYNHILDR,
					tr("Not support Video Codec!"),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	break;
  case QTB_MSG_ALREADY_CONNECT_ANOTHER_CLIENT:
	criticalMessage(PHASE_QTBRYNHILDR,
					tr("Already connected another client!"),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	break;
  case QTB_MSG_UNKNOWN_ERROR:
	criticalMessage(PHASE_QTBRYNHILDR,
					tr("Unknown Error!"),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	break;
  default:
	ABORT();
	break;
  }
}

// check log file stream
bool LogMessage::hasValidLogFileStream() const
{
  // checks
  // 1) check logFile open
  if (logFile == nullptr){
	// already close
	return false;
  }
  if (!logFile->isOpen()){
	return false;
  }

  // 2) check logFileStream open
  if (logFileStream == nullptr){
	return false;
  }

  return true;
}

} // end of namespace qtbrynhildr
