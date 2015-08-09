// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// option.cpp

// Common Header
#include "common.h"

// Syetem Header
#include <iostream>
#include <cstdlib>
#include <cstring>

// Local Header
#include "option.h"
#include "parameters.h"
#include "settings.h"
#include "version.h"

namespace qtbrynhildr {

// constructor
Option::Option(int argc, char *argv[])
  :
  serverName(0),
  portNo(0),
  password(0),
  iniFileName(0),
  debug(0),
#if QTB_RECORDER
  recordingFlag(false),
  recordingFileName(0),
  replayingFlag(false),
  replayingFileName(0),
#endif // QTB_RECORDER
  bootupFlag(false),
  shutdownFlag(false)
{
  // analysis options
  if (analyzeOptions(argc, argv)){
	// error
  }
}

// destructor
Option::~Option()
{
}

// analyze command line options
bool Option::analyzeOptions(int argc, char *argv[])
{
  // analyze options
  while(argc > 1){
	if (argv[1][0] == '-'){
	  const char *optionName = (char*)(&argv[1][1]);
	  if (strncmp("server", optionName, sizeof("server")) == 0){
		// check argument
		if (argc < 3){
		  // error : no argument
		  cout << "-server option need an argument." << endl << flush;
		  printHelp();
		  shutdownFlag = true;
		}
		else {
		  char *buf= new char[QTB_MAXPATHLEN+1];
		  const char *arg = (char*)(&argv[2][0]);
		  strncpy(buf, arg, QTB_MAXPATHLEN);
		  // analyze option argument
		  // exp) (*) no white space character
		  // -server 192.168.10.1:mcz-xoxo
		  // -server 192.168.10.1:mcz-xoxo:55500
		  // -server urd-PC:mcz-xoxo
		  // -server urd-PC:mcz-xoxo:30000
		  const char* server =  strtok(buf, ":");
		  const char* passwd = strtok(NULL, ":");
		  if (passwd != NULL){
			if (strlen(passwd) > ENCRYPTION_KEY_LENGTH){
			  // error
			  cout << "-server option : password is too long." << endl << flush;
			  printHelp();
			  shutdownFlag = true;
			  break;
			}
		  }
		  else {
			// error
			cout << "-server option need password field." << endl << flush;
			printHelp();
			shutdownFlag = true;
			break;
		  }
		  const char* port = strtok(NULL, ":");
		  // set information
		  serverName = server;
		  password = passwd;
		  if (port != NULL){
			int portNo = atoi(port);
			if (portNo > 0 && portNo < 65536){
			  this->portNo = portNo;
			}
			else {
			  // error
			  cout << "illegal port no. : " << port << " (ignored)" << endl << flush;
			}
		  }

		  // boot up ON
		  bootupFlag = true;

		  // next argument
		  argc--;
		  argv++;
		}
	  }
	  else if (strncmp("debug", optionName, sizeof("debug")) == 0){
		// check argument (on/off)
		if (argc < 3){
		  // error : no argument
		  cout << "-debug option need an argument(on or off)." << endl << flush;
		  printHelp();
		  shutdownFlag = true;
		}
		else {
		  const char *arg = (char*)(&argv[2][0]);
		  // analyze option argument
		  if (strncmp("on", arg, sizeof("on")) == 0){
			debug = "on";
		  }
		  else if (strncmp("off", arg, sizeof("off")) == 0){
			debug = "off";
		  }
		  else {
			// unknown argument for -debug
			cout << "-debug option need an argument(on or off)." << endl << flush;
			printHelp();
			shutdownFlag = true;
		  }

		  // next argument
		  argc--;
		  argv++;
		}
	  }
#if QTB_RECORDER
	  else if (strncmp("record", optionName, sizeof("record")) == 0){
		// check argument
		if (argc < 3){
		  // error : no argument
		  cout << "-record option need an argument." << endl << flush;
		  printHelp();
		  shutdownFlag = true;
		}
		else {
		  const char *arg = (char*)(&argv[2][0]);

		  // check other option
		  if (arg[0] == '-'){
			cout << "-record option need an argument." << endl << flush;
			printHelp();
			shutdownFlag = true;
		  }
		  else {
			recordingFlag = true;
			recordingFileName = arg;
		  }

		  // next argument
		  argc--;
		  argv++;
		}
	  }
	  else if (strncmp("replay", optionName, sizeof("replay")) == 0){
		// check argument
		if (argc < 3){
		  // error : no argument
		  cout << "-replay option need an argument." << endl << flush;
		  printHelp();
		  shutdownFlag = true;
		}
		else {
		  const char *arg = (char*)(&argv[2][0]);

		  // check other option
		  if (arg[0] == '-'){
			cout << "-replay option need an argument." << endl << flush;
			printHelp();
			shutdownFlag = true;
		  }
		  else {
			replayingFlag = true;
			replayingFileName = arg;
		  }

		  // next argument
		  argc--;
		  argv++;
		}
	  }
#endif // QTB_RECORDER
	  else if ((strncmp("version", optionName, sizeof("version")) == 0)||
			   (strncmp("v", optionName, sizeof("v")) == 0)){
		printVersion();
		shutdownFlag = true;
	  }
	  else if (strncmp("vv", optionName, sizeof("vv")) == 0){
		printVersionString();
		shutdownFlag = true;
	  }
	  else if ((strncmp("help", optionName, sizeof("help")) == 0)||
			   (strncmp("h", optionName, sizeof("h")) == 0)){
		printHelp();
		shutdownFlag = true;
	  }
	  else {
		cout << "unknown option : " << argv[1] << endl << flush;
		shutdownFlag = true;
	  }
	}
	else {
	  // ini filename
	  iniFileName = strdup(argv[1]);
	}

	// next argument
	argc--;
	argv++;
  }

  return true;
}

// print version
void Option::printVersion() const
{
  cout << QTB_APPLICATION <<
	" Ver." << QTB_VERSION <<
	" Copyright (c) " << QTB_YEAR << " " << QTB_AUTHOR << " @ " << QTB_ORGANIZATION << endl << flush;
}

// print version string
void Option::printVersionString() const
{
  string str = QTB_VERSION;
  for (string::size_type pos = str.find(".") ; pos != string::npos; pos = str.find(".")){
	str.erase(pos,1);
  }
  cout << str << flush;
}

// print help messages
void Option::printHelp() const
{
  // print header
  cout << "-------------------------------------------------------" << endl;
  cout << QTB_APPLICATION << " [options] <.ini file>" << endl;
  cout << "-------------------------------------------------------" << endl;

  // -server <server name|IP address>:<password>[:<port no>]
  cout << "-server <server name|IP address>:<password>[:<port no>]" << endl;
  cout << "        " << "bootup for server." << endl;

#if QTB_RECORDER
  // -record <file>
  cout << "-record <file>" << endl;

  // -replay <file>
  cout << "-replay <file>" << endl;
#endif // QTB_RECORDER

  // -debug <on|off>
  cout << "-debug <on|off>" << endl;
  cout << "        " << "set debug mode to on/off." << endl;

  // -version (-v)
  cout << "-version (-v)" << endl;
  cout << "        " << "display version." << endl;

  // -help (-h)
  cout << "-help (-h)" << endl;
  cout << "        " << "display this help message." << endl << flush;

  // .ini file
  cout << "<.ini file>" << endl;
  cout << "        " << ".ini filename." << endl << flush;
}

} // end of namespace qtbrynhildr
