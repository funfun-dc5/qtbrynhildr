// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

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
  :serverName(0)
  ,portNo(0)
  ,password(0)
  ,hostType(0)
  ,publicModeVersion(0)
  ,iniFileName(0)
  ,initFlag(false)
  ,debug(0)
  ,fullScreenFlag(false)
#if QTB_CELT_SUPPORT
  ,celtFlag(false)
#endif // QTB_CELT_SUPPORT
#if QTB_RECORDER
  ,recordingFlag(false)
  ,recordingFileName(0)
  ,replayingFlag(false)
  ,replayingFileName(0)
#endif // QTB_RECORDER
  ,noTransFlag(false)
  ,bootupFlag(false)
  ,shutdownFlag(false)
  ,testModeFlag(false)
{
  // analysis options
  if (analyzeOptions(argc, argv)){
	// error
  }
}

// destructor
Option::~Option()
{
  // delete objects
  if (iniFileName != 0){
	free(iniFileName);
	iniFileName = 0;
  }
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
		  static char buf[QTB_MAXPATHLEN+1];
		  const char *arg = (char*)(&argv[2][0]);
		  strncpy(buf, arg, QTB_MAXPATHLEN);
		  // analyze option argument
		  // exp) (*) no white space character
		  // -server 192.168.10.1:mcz-xoxo
		  // -server 192.168.10.1:mcz-xoxo:55500
		  // -server urd-PC:mcz-xoxo
		  // -server urd-PC:mcz-xoxo:30000
		  // -server urd-PC:mcz-xoxo:30000:xp
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
		  const char* hostType = strtok(NULL, ":");
		  // set information
		  serverName = server;
		  password = passwd;
		  // port no
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
		  // host type
		  if (hostType != NULL){
			this->hostType = hostType;
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
	  else if (strncmp("init", optionName, sizeof("init")) == 0){
		initFlag = true;
	  }
	  else if (strncmp("notrans", optionName, sizeof("notrans")) == 0){
		noTransFlag = true;
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
	  else if (strncmp("mode5", optionName, sizeof("mode5")) == 0){
		publicModeVersion = 5;
	  }
	  else if (strncmp("mode6", optionName, sizeof("mode6")) == 0 ||
			   strncmp("mjpeg", optionName, sizeof("mjpeg")) == 0){
		publicModeVersion = 6;
	  }
	  else if (strncmp("mode7", optionName, sizeof("mode7")) == 0 ||
			   strncmp("compress", optionName, sizeof("compress")) == 0){
		publicModeVersion = 7;
	  }
	  else if (strncmp("fullscreen", optionName, sizeof("fullscreen")) == 0){
		fullScreenFlag = true;
	  }
#if QTB_CELT_SUPPORT
	  else if (strncmp("celt", optionName, sizeof("celt")) == 0){
		celtFlag = true;
	  }
#endif // QTB_CELT_SUPPORT
	  else if ((strncmp("version", optionName, sizeof("version")) == 0)||
			   (strncmp("v", optionName, sizeof("v")) == 0)){
		printVersion();
		shutdownFlag = true;
	  }
	  else if (strncmp("vv", optionName, sizeof("vv")) == 0){
		printVersionString();
		shutdownFlag = true;
	  }
	  else if (strncmp("spec", optionName, sizeof("spec")) == 0){
		printSpec();
		shutdownFlag = true;
	  }
	  else if ((strncmp("help", optionName, sizeof("help")) == 0)||
			   (strncmp("h", optionName, sizeof("h")) == 0)){
		printHelp();
		shutdownFlag = true;
	  }
	  else if (strncmp("testmode", optionName, sizeof("testmode")) == 0){
		testModeFlag = true;
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

// get version string
const char *Option::getVersionString() const
{
  static string str = QTB_VERSION QTB_RCNAME;
  for (string::size_type pos = str.find(".") ; pos != string::npos; pos = str.find(".")){
	str.erase(pos,1);
  }
  return str.c_str();
}

// print version
void Option::printVersion() const
{
  cout << QTB_APPLICATION <<
	" Ver." << QTB_VERSION QTB_RCNAME <<
	" Copyright (c) " << QTB_YEAR << " " << QTB_AUTHOR << " @ " << QTB_ORGANIZATION << endl << flush;
}

// print version string
void Option::printVersionString() const
{
  string str = QTB_VERSION QTB_RCNAME;
  for (string::size_type pos = str.find(".") ; pos != string::npos; pos = str.find(".")){
	str.erase(pos,1);
  }
  cout << str << flush;
}

// print spec
void Option::printSpec() const
{
  cout << "spec :";

#if QTB_RECORDER
  cout << " RECORDER";
#endif // QTB_RECORDER

  cout << " MODE6";

  cout << " MODE7";

#if QTB_CELT_SUPPORT
  cout << " CELT";
#endif // QTB_CELT_SUPPORT

#ifdef USE_KEYLAYOUTFILE
  cout << " KEYLAYOUTFILE";
#endif // USE_KEYLAYOUTFILE

#if QTB_PORTABLE_VERSION
  cout << " PORTABLE";
#endif // QTB_PORTABLE_VERSION

  cout << endl << flush;
}

// print help messages
void Option::printHelp() const
{
  // print header
  cout << "-------------------------------------------------------" << endl;
  cout << QTB_APPLICATION << " [options] <.ini file>" << endl;
  cout << "-------------------------------------------------------" << endl;

  cout << "-mode5/mode6(mjpeg)/mode7(compress)" << endl;
  cout << "        " << "support public mode 5/6/7." << endl;
  cout << endl;

  // -server <server name|IP address>:<password>[:<port no>[:<host type>]]
  cout << "-server <server name|IP address>:<password>[:<port no>[:<host type>]]" << endl;
  cout << "        " << "bootup for server." << endl;
  cout << "        " << "host type : xp/vista/7/8/8.1/10" << endl;
  cout << endl;

#if QTB_CELT_SUPPORT
  // -celt
  cout << "-celt" << endl;
  cout << "        " << "support CELT for sound." << endl;
  cout << endl;
#endif // QTB_CELT_SUPPORT

#if QTB_RECORDER
  // -record <file>
  cout << "-record <file>" << endl;
  cout << "        " << "record operations to <file>." << endl;
  cout << endl;

  // -replay <file>
  cout << "-replay <file>" << endl;
  cout << "        " << "replay operations from <file>." << endl;
  cout << endl;
#endif // QTB_RECORDER

  // -fullscreen
  cout << "-fullscreen" << endl;
  cout << "        " << "full screen mode." << endl;
  cout << endl;

  // -debug <on|off>
  cout << "-debug <on|off>" << endl;
  cout << "        " << "set debug mode to on/off." << endl;
  cout << endl;

  // -version (-v)
  cout << "-version (-v)" << endl;
  cout << "        " << "display version." << endl;
  cout << endl;

  // -spec
  cout << "-spec" << endl;
  cout << "        " << "display spec." << endl;
  cout << endl;

  // -help (-h)
  cout << "-help (-h)" << endl;
  cout << "        " << "display this help message." << endl;
  cout << endl;

  // -init
  cout << "-init" << endl;
  cout << "        " << "initialize settings." << endl;
  cout << endl;

  // -notrans
  cout << "-notrans" << endl;
  cout << "        " << "no translation." << endl;
  cout << endl;

  // .ini file
  cout << "<.ini file>" << endl;
  cout << "        " << ".ini filename." << endl;


  cout << flush;
}

} // end of namespace qtbrynhildr
