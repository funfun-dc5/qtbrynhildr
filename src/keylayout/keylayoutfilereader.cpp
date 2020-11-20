// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>
#include <fstream>

// Qt Header
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

// Local Header
#include "keylayout/keylayoutfilereader.h"
#include "parameters.h"
#include "settings.h"

namespace qtbrynhildr {

// constructor
KeyLayoutFileReader::KeyLayoutFileReader(const char *layoutfilepath)
  :path(layoutfilepath)
  // for DEBUG
  ,outputLog(false)
{
  if (outputLog)
	cout << "Created KeyLayoutFileReader! layoutfilepath = " << layoutfilepath << endl << flush;

  QDir dir(layoutfilepath);
  QFileInfoList list = dir.entryInfoList();

  for (int i = 0, size = list.size(); i < size; i++){
	QFileInfo fileInfo = list.at(i);
	QString filename = fileInfo.fileName();
	if (filename.endsWith(QTB_KEYLAYOUT_FILE_SUFFIXX)){
	  if (outputLog)
		cout << "Found : " << qPrintable(filename) << endl << flush;
	  readKeyLayoutFile(qPrintable(filename));
	}
  }
}

// destructor
KeyLayoutFileReader::~KeyLayoutFileReader()
{
  // delete objects
  // all key layout file
  QList<KeyLayoutFile*>::iterator i;
  for (i = list.begin(); i != list.end(); i++){
	delete *i;
  }

  if (outputLog)
	cout << "Deleted KeyLayoutFileReader!" << endl << flush;
}

// get key layout file
KeyLayoutFile* KeyLayoutFileReader::getKeyLayoutFile(int index)
{
  return (KeyLayoutFile*)list.at(index);
}

// get keyboard type list
QStringList KeyLayoutFileReader::getKeyboardTypeList()
{
  return keyboardTypeList;
}

// get index of keyboard type
int KeyLayoutFileReader::getIndexOfKeyboardType(QString keyboardTypeName)
{
  return keyboardTypeList.indexOf(keyboardTypeName);
}

// read a key layout file
void KeyLayoutFileReader::readKeyLayoutFile(const char *filename)
{
  fstream file;
  char fullname[QTB_MAXPATHLEN+1];
  snprintf(fullname, QTB_MAXPATHLEN, "%s/%s", path, filename);

  file.open(fullname, ios::in | ios::binary);
  if (file.is_open()){
	if (outputLog)
	  cout << "Opened file : " << fullname << endl << flush;

	KLFHeader header;
	file.read((char*)&header, sizeof(KLFHeader));
	if (outputLog) {
	  cout << "[File Header]" << endl;
	  cout << "magic      = \"" << header.magic << '\"' << endl;
	  cout << "spec       = " << header.spec << endl;
	  cout << "size       = " << header.size << endl;
	  cout << "keynum     = " << header.keynum << endl;
	  cout << "softkeynum = " << header.softkeynum << endl;
	  cout << "[General]" << endl;
	  cout << "Name       = " << header.name << endl;
	  cout << "Author     = " << header.author << endl << flush;
	}

	// rewind and read all KLF image
	const char *klfImage = new char[header.size];
	file.seekg(0);
	file.read((char*)klfImage, header.size);
	KeyLayoutFile *klf = new KeyLayoutFile(klfImage);

	// append
	list.append(klf);

	file.close();
	if (outputLog)
	  cout << "Closed file : " << fullname << endl << flush;

	// append to keyboard type list
	keyboardTypeList << QString(header.name);
  }
  else {
	if (outputLog)
	  cout << "Error: Failed to open file" << endl << flush;
  }
}

} // end of namespace qtbrynhildr
