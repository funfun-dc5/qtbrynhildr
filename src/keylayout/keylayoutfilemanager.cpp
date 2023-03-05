// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QDateTime>
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
  //path(layoutfilepath)
  // for DEBUG
  outputLog(false)
{
  if (outputLog)
	std::cout << "Created KeyLayoutFileManager! layoutfilepath = " << layoutfilepath << std::endl << std::flush;

  QDir dir(layoutfilepath);
  QFileInfoList list = dir.entryInfoList();

  for (int i = 0, size = (int)list.size(); i < size; i++){
	QFileInfo fileInfo = list.at(i);
	QString filename = fileInfo.fileName();
	if (filename.endsWith(QTB_KEYLAYOUT_FILE_SUFFIX)){
	  if (outputLog){
		std::cout << "Found : " << qPrintable(filename) << std::endl << std::flush;
		if (dir.exists(filename + "x")){
		  std::cout << "Found : " << qPrintable(filename+"x") << std::endl << std::flush;
		}
		else {
		  std::cout << "NOT Found : " << qPrintable(filename) << std::endl << std::flush;
		}
	  }

	  bool needBuild = false;
	  if (!dir.exists(filename + "x")){
		needBuild = true;
	  }
	  else {
		// check newer .kl than .klx
		QFileInfo xFileInfo(fileInfo.absoluteFilePath() + "x");
		QDateTime lastModifiedTime = fileInfo.lastModified();
		QDateTime xLastModifiedTime = xFileInfo.lastModified();
		needBuild = lastModifiedTime > xLastModifiedTime;
		if (outputLog)
		  std::cout << "Found : newer .kl file!" << qPrintable(filename) << std::endl << std::flush;
	  }
	  if (needBuild){
		QString inputFile = fileInfo.absoluteFilePath();
		QString outputFile = fileInfo.absoluteFilePath() + "x";
		char* infile = strdup(qPrintable(QDir::toNativeSeparators(inputFile)));
		char* outfile = strdup(qPrintable(QDir::toNativeSeparators(outputFile)));

		if (outputLog){
		  std::cout << "infile  : " << infile << std::endl;
		  std::cout << "outfile : " << outfile << std::endl << std::flush;
		}
		int result = make_KLX(infile, outfile);
		if (result < 0){
		  std::cout << "Failed to build .klx (" << infile << ")" << std::endl << std::flush;
		}
		else if (result > 0){
		  std::cout << "Found : " << result << " errors to build .klx (" << infile << ")" << std::endl << std::flush;
		}

		free(infile);
		free(outfile);
	  }
	}
  }
}

// destructor
KeyLayoutFileManager::~KeyLayoutFileManager()
{
  if (outputLog)
	std::cout << "Deleted KeyLayoutFileManager!" << std::endl << std::flush;
}

} // end of namespace qtbrynhildr
