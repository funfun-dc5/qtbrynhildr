// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

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
  :versionString(0)
  ,serverName(0)
  ,portNo(0)
  ,password(0)
  ,hostType(0)
  ,publicModeVersion(0)
  ,iniFileName(0)
  ,initFlag(false)
  ,debug(0)
  ,fullScreenFlag(false)
  ,viewerFlag(false)
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
  ,benchmarkFlag(false)
{
  // versionString
  static std::string str = QTB_VERSION QTB_RCNAME;
  for (std::string::size_type pos = str.find(".") ; pos != std::string::npos; pos = str.find(".")){
       str.erase(pos,1);
  }
  versionString = str.c_str();

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
		  std::cout << "-server option need an argument." << std::endl << std::flush;
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
			  std::cout << "-server option : password is too long." << std::endl << std::flush;
			  printHelp();
			  shutdownFlag = true;
			  break;
			}
		  }
		  else {
			// error
			std::cout << "-server option need password field." << std::endl << std::flush;
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
			  std::cout << "illegal port no. : " << port << " (ignored)" << std::endl << std::flush;
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
		  std::cout << "-debug option need an argument(on or off)." << std::endl << std::flush;
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
			std::cout << "-debug option need an argument(on or off)." << std::endl << std::flush;
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
		  std::cout << "-record option need an argument." << std::endl << std::flush;
		  printHelp();
		  shutdownFlag = true;
		}
		else {
		  const char *arg = (char*)(&argv[2][0]);

		  // check other option
		  if (arg[0] == '-'){
			std::cout << "-record option need an argument." << std::endl << std::flush;
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
		  std::cout << "-replay option need an argument." << std::endl << std::flush;
		  printHelp();
		  shutdownFlag = true;
		}
		else {
		  const char *arg = (char*)(&argv[2][0]);

		  // check other option
		  if (arg[0] == '-'){
			std::cout << "-replay option need an argument." << std::endl << std::flush;
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
	  else if (strncmp("viewer", optionName, sizeof("viewer")) == 0){
		viewerFlag = true;
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
#if QTB_BENCHMARK
	  else if (strncmp("benchmark", optionName, sizeof("benchmark")) == 0){
		benchmarkFlag = true;
	  }
#endif // QTB_BENCHMARK
	  else {
		std::cout << "unknown option : " << argv[1] << std::endl << std::flush;
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
  std::cout << QTB_APPLICATION <<
	" Ver." << QTB_VERSION QTB_RCNAME <<
	" Copyright (c) " << QTB_YEAR << " " << QTB_AUTHOR << " @ " << QTB_ORGANIZATION << std::endl << std::flush;
}

// print version string
void Option::printVersionString() const
{
  std::cout << getVersionString() << std::flush;
}

// print spec
void Option::printSpec() const
{
  std::cout << "spec :";

#if QTB_RECORDER
  std::cout << " RECORDER";
#endif // QTB_RECORDER

  std::cout << " MODE6";

  std::cout << " MODE7";

#if QTB_CELT_SUPPORT
  std::cout << " CELT";
#endif // QTB_CELT_SUPPORT

#ifdef USE_KEYLAYOUTFILE
  std::cout << " KEYLAYOUTFILE";
#endif // USE_KEYLAYOUTFILE

#if QTB_PORTABLE_VERSION
  std::cout << " PORTABLE";
#endif // QTB_PORTABLE_VERSION

  std::cout << std::endl << std::flush;
}

// print help messages
void Option::printHelp() const
{
  // print header
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << QTB_APPLICATION << " [options] <.ini file>" << std::endl;
  std::cout << "-------------------------------------------------------" << std::endl;

  std::cout << "-mode5/mode6(mjpeg)/mode7(compress)" << std::endl;
  std::cout << "        " << "support public mode 5/6/7." << std::endl;
  std::cout << std::endl;

  // -server <server name|IP address>:<password>[:<port no>[:<host type>]]
  std::cout << "-server <server name|IP address>:<password>[:<port no>[:<host type>]]" << std::endl;
  std::cout << "        " << "bootup for server." << std::endl;
  std::cout << "        " << "host type : xp/vista/7/8/8.1/10" << std::endl;
  std::cout << std::endl;

#if QTB_CELT_SUPPORT
  // -celt
  std::cout << "-celt" << std::endl;
  std::cout << "        " << "support CELT for sound." << std::endl;
  std::cout << std::endl;
#endif // QTB_CELT_SUPPORT

#if QTB_RECORDER
  // -record <file>
  std::cout << "-record <file>" << std::endl;
  std::cout << "        " << "record operations to <file>." << std::endl;
  std::cout << std::endl;

  // -replay <file>
  std::cout << "-replay <file>" << std::endl;
  std::cout << "        " << "replay operations from <file>." << std::endl;
  std::cout << std::endl;
#endif // QTB_RECORDER

  // -fullscreen
  std::cout << "-fullscreen" << std::endl;
  std::cout << "        " << "full screen mode." << std::endl;
  std::cout << std::endl;

  // -viewer
  std::cout << "-viewer" << std::endl;
  std::cout << "        " << "viewer mode." << std::endl;
  std::cout << std::endl;

  // -debug <on|off>
  std::cout << "-debug <on|off>" << std::endl;
  std::cout << "        " << "set debug mode to on/off." << std::endl;
  std::cout << std::endl;

  // -version (-v)
  std::cout << "-version (-v)" << std::endl;
  std::cout << "        " << "display version." << std::endl;
  std::cout << std::endl;

  // -spec
  std::cout << "-spec" << std::endl;
  std::cout << "        " << "display spec." << std::endl;
  std::cout << std::endl;

  // -help (-h)
  std::cout << "-help (-h)" << std::endl;
  std::cout << "        " << "display this help message." << std::endl;
  std::cout << std::endl;

  // -init
  std::cout << "-init" << std::endl;
  std::cout << "        " << "initialize settings." << std::endl;
  std::cout << std::endl;

  // -notrans
  std::cout << "-notrans" << std::endl;
  std::cout << "        " << "no translation." << std::endl;
  std::cout << std::endl;

  // .ini file
  std::cout << "<.ini file>" << std::endl;
  std::cout << "        " << ".ini filename." << std::endl;


  std::cout << std::flush;
}

} // end of namespace qtbrynhildr
