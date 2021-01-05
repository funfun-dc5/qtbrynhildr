// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>

// Local Header

using namespace std;

namespace qtbrynhildr {

// phase ID
typedef enum {
  PHASE_QTBRYNHILDR = 0,
  PHASE_CONTROL,
  PHASE_GRAPHICS,
  PHASE_SOUND,
  PHASE_DEBUG,
  PHASE_NUM
} PHASE_ID;

// log message
typedef enum {
  QTB_MSG_CONNECT_ERROR,
  QTB_MSG_PASSWORD_ERROR,
  QTB_MSG_NOTSUPPORT_SAMPLE_RATE,
  QTB_MSG_NOT_FOUND_SOUND_DEVICE,
  QTB_MSG_NOTSUPPORT_VIDEO_MODE,
  QTB_MSG_ALREADY_CONNECT_ANOTHER_CLIENT,
  QTB_MSG_UNKNOWN_ERROR
} MSG_ID;

// LogMessage
class LogMessage : public QMessageBox
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // log file
  QFile	*logFile;

  // log file stream
  QTextStream *logFileStream;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  LogMessage(QWidget *parent);
  // destructor
  ~LogMessage();

  // open log file
  bool openLogFile(QString filename);

  // close log file
  bool closeLogFile();

  // critical message dialog and log message
  int criticalMessage(PHASE_ID id,
					  const QString text,
					  StandardButton buttons,
					  StandardButton defaultButton);

  // warning message dialog and log message
  int warningMessage(PHASE_ID id,
					 const QString text,
					 StandardButton buttons,
					 StandardButton defaultButton);

  // information message dialog and log message
  int informationMessage(PHASE_ID id,
						 const QString text,
						 StandardButton buttons,
						 StandardButton defaultButton);

  // question message dialog and log message
  int questionMessage(PHASE_ID id,
					 const QString text,
					 StandardButton buttons,
					 StandardButton defaultButton);

  // normal log message
  void outputLogMessage(PHASE_ID id, const QString text);

  // output message
  void outputMessage(int msgID);

private:
  // check log file stream
  bool hasValidLogFileStream() const;
};

} // end of namespace qtbrynhildr

#endif // LOGMESSAGE_H
