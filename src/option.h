// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// option.h

#ifndef OPTION_H
#define OPTION_H
// Common Header
#include "common.h"

namespace qtbrynhildr {

// Option
class Option
{
public:
  // constructor
  Option(int argc, char *argv[]);
  // destructor
  ~Option();

  // get server name
  const char *getServerName() const
  {
	return serverName;
  }

  // get port no
  int getPortNo() const
  {
	return portNo;
  }

  // get password
  const char *getPassword() const
  {
	return password;
  }

  // get ini file name
  const char *getIniFileName() const
  {
	return iniFileName;
  }

  // get bootup flag
  bool getBootupFlag() const
  {
	return bootupFlag;
  }

  // set bootup flag
  void setBootupFlag(bool bootupFlag)
  {
	this->bootupFlag = bootupFlag;
  }

  // get shutdown flag
  bool getShutdownFlag() const
  {
	return shutdownFlag;
  }

  // get debug
  const char* getDebug() const
  {
	return debug;
  }

#if QTB_RECORDER
  // recording flag
  bool getRecordingFlag() const
  {
	return recordingFlag;
  }

  // recording filename
  const char *getRecordingFileName() const
  {
	return recordingFileName;
  }

  // replaying flag
  bool getReplayingFlag() const
  {
	return replayingFlag;
  }

  // replaying filename
  const char *getReplayingFileName() const
  {
	return replayingFileName;
  }
#endif // QTB_RECORDER

private:
  // analyze command line options
  bool analyzeOptions(int argc, char *argv[]);

  // print version
  void printVersion() const;

  // print version string
  void printVersionString() const;

  // print help messages
  void printHelp() const;

private:
  // server name
  const char *serverName;

  // port no
  int portNo;

  // password
  const char *password;

  // ini filename
  const char *iniFileName;

  // debug
  const char *debug;

#if QTB_RECORDER
  // recording flag
  bool recordingFlag;

  // recording filename
  const char *recordingFileName;

  // replaying flag
  bool replayingFlag;

  // replaying filename
  const char *replayingFileName;
#endif // QTB_RECORDER

  // bootup flag
  bool bootupFlag;

  // shutdown flag
  bool shutdownFlag;
};

}

#endif // OPTION_H
