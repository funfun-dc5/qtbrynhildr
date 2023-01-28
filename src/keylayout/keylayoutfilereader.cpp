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
	std::cout << "Created KeyLayoutFileReader! layoutfilepath = " << layoutfilepath << std::endl << std::flush;

  QDir dir(layoutfilepath);
  QFileInfoList list = dir.entryInfoList();

  for (int i = 0, size = list.size(); i < size; i++){
	QFileInfo fileInfo = list.at(i);
	QString filename = fileInfo.fileName();
	if (filename.endsWith(QTB_KEYLAYOUT_FILE_SUFFIXX)){
	  if (outputLog)
		std::cout << "Found : " << qPrintable(filename) << std::endl << std::flush;
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
	std::cout << "Deleted KeyLayoutFileReader!" << std::endl << std::flush;
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
  std::fstream file;
  char fullname[QTB_MAXPATHLEN+1];
  snprintf(fullname, QTB_MAXPATHLEN, "%s/%s", path, filename);

  file.open(fullname, std::ios::in | std::ios::binary);
  if (file.is_open()){
	if (outputLog)
	  std::cout << "Opened file : " << fullname << std::endl << std::flush;

	KLFHeader header;
	file.read((char*)&header, sizeof(KLFHeader));
	if (outputLog) {
	  std::cout << "[File Header]" << std::endl;
	  std::cout << "magic      = \"" << header.magic << '\"' << std::endl;
	  std::cout << "spec       = " << header.spec << std::endl;
	  std::cout << "size       = " << header.size << std::endl;
	  std::cout << "keynum     = " << header.keynum << std::endl;
	  std::cout << "softkeynum = " << header.softkeynum << std::endl;
	  std::cout << "[General]" << std::endl;
	  std::cout << "Name       = " << header.name << std::endl;
	  std::cout << "Author     = " << header.author << std::endl << std::flush;
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
	  std::cout << "Closed file : " << fullname << std::endl << std::flush;

	// append to keyboard type list
	keyboardTypeList << QString(header.name);

	// delete KLF image
	if (klfImage != nullptr){
	  delete [] klfImage;
	  klfImage = nullptr;
	}
  }
  else {
	if (outputLog)
	  std::cout << "Error: Failed to open file" << std::endl << std::flush;
  }
}

} // end of namespace qtbrynhildr
