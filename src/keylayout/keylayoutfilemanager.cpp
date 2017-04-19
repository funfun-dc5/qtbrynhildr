// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

// Local Header
#include "keylayout/keylayoutfilemanager.h"
#include "keylayout/klfcompiler.h"
#include "settings.h"

namespace qtbrynhildr {

// constructor
KeyLayoutFileManager::KeyLayoutFileManager(const char *layoutfilepath)
  :
  path(layoutfilepath),
  // for DEBUG
  outputLog(false)
{
  if (outputLog)
	cout << "Created KeyLayoutFileManager! layoutfilepath = " << layoutfilepath << endl << flush;

  QDir dir(layoutfilepath);
  QFileInfoList list = dir.entryInfoList();

  for (int i = 0; i < list.size(); i++){
	QFileInfo fileInfo = list.at(i);
	QString filename = fileInfo.fileName();
	if (filename.endsWith(QTB_KEYLAYOUT_FILE_SUFFIX)){
	  if (outputLog){
		cout << "Found : " << qPrintable(filename) << endl << flush;
		if (dir.exists(filename + "x")){
		  cout << "Found : " << qPrintable(filename+"x") << endl << flush;
		}
		else {
		  cout << "NOT Found : " << qPrintable(filename) << endl << flush;
		}
	  }
	  if (!dir.exists(filename + "x")){
		QString inputFile = fileInfo.absoluteFilePath();
		QString outputFile = fileInfo.absoluteFilePath() + "x";
		const char* infile = strdup(qPrintable(QDir::toNativeSeparators(inputFile)));
		const char* outfile = strdup(qPrintable(QDir::toNativeSeparators(outputFile)));
		if (outputLog){
		  cout << "infile  : " << infile << endl;
		  cout << "outfile : " << outfile << endl << flush;
		}
		int result = make_KLX(infile, outfile);
		if (result){
		  cout << "Failed to build .klx (" << infile << ")" << endl << flush;
		}
	  }
	}
  }
}

// destructor
KeyLayoutFileManager::~KeyLayoutFileManager()
{
  if (outputLog)
	cout << "Deleted KeyLayoutFileManager!" << endl << flush;
}

} // end of namespace qtbrynhildr
