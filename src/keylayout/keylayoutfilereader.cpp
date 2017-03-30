// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>
#include <fstream>
#include <dirent.h>

// Qt Header

// Local Header
#include "keylayout/keylayoutfilereader.h"
#include "parameters.h"

namespace qtbrynhildr {

// constructor
KeyLayoutFileReader::KeyLayoutFileReader(const char *layoutfilepath)
  :
  path(layoutfilepath)
{
  cout << "Created KeyLayoutFileReader! layoutfilepath = " << layoutfilepath << endl << flush;

  DIR *dir;
  dir = opendir(layoutfilepath);
  if(dir != NULL){
	while(true){
	  struct dirent *dp;
	  dp = readdir(dir);
	  if (dp == NULL)
		break;
	  if (strncmp(dp->d_name, ".", 1) == 0 ||
		  strncmp(dp->d_name, "..", 2) == 0 ){
		continue;
	  }
	  if (!strncmp(&dp->d_name[strlen(dp->d_name)-4], ".klx", 4) == 0){
		cout << "NOT supported File : " << dp->d_name << endl << flush;
		continue;
	  }

	  // read "*.klx"
	  cout << "    supported File : " << dp->d_name << endl << flush;
	  readKeyLayoutFile((const char*)dp->d_name);
	}
	closedir(dir);
  }
  else {
	cout << "Failed to opendir()" << endl << flush;
  }
}

// destructor
KeyLayoutFileReader::~KeyLayoutFileReader()
{
  cout << "Deleted KeyLayoutFileReader!" << endl << flush;
}

// read a key layout file
void KeyLayoutFileReader::readKeyLayoutFile(const char *filename)
{
  fstream file;
  char fullname[QTB_MAXPATHLEN+1];
  snprintf(fullname, QTB_MAXPATHLEN, "%s/%s", path, filename);

  file.open(fullname, ios::in | ios::binary);
  if (file.is_open()){
	cout << "Opened file : " << fullname << endl << flush;

	KLFHeader header;
	file.read((char*)&header, sizeof(KLFHeader));
	cout << "[File Header]" << endl;
	cout << "magic      = " << header.magic << endl;
	cout << "spec       = " << header.spec << endl;
	cout << "size       = " << header.size << endl;
	cout << "keynum     = " << header.keynum << endl;
	cout << "softkeynum = " << header.softkeynum << endl;
	cout << "[General]" << endl;
	cout << "Name       = " << header.name << endl;
	cout << "Author     = " << header.author << endl << flush;
	file.close();
	cout << "Closed file : " << fullname << endl << flush;
  }
  else {
	cout << "Error: Failed to open file" << endl << flush;
  }
}

} // end of namespace qtbrynhildr
