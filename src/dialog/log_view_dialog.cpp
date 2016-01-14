// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// log_view_dialog.cpp

// Common Header
#include "common/common.h"

// System Header

// Qt header
#include <QDialog>
#include <QFile>
#include <QRect>
#include <QTextStream>

// Local Header
#include "log_view_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
LogViewDialog::LogViewDialog(Settings *settings,
							 QWidget *parent)
  :
  //  QDialog(parent),
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  settings(settings)
{
  setupUi(this);

  // Read Only
  textEdit->setReadOnly(true);

  // resetting
  resetting();
}

// resize event
void LogViewDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event)
}

// settings for Tablet
void LogViewDialog::resetting()
{
}

// set plain text
void LogViewDialog::setPlainText(const QString &text)
{
  textEdit->setPlainText(text);
}

// clear
void LogViewDialog::clear()
{
  textEdit->clear();
}

// show
void LogViewDialog::show()
{
  QString logText;
  QFile *logFile = new QFile(settings->getLogFile());
  if (logFile->open(QFile::ReadOnly)){
	  QTextStream *logFileStream = new QTextStream(logFile);
	  logFileStream->setCodec("UTF-8");
	  logText = logFileStream->readAll();
	  logFile->close();
  }
  clear();
  setPlainText(logText);
  QDialog::show();
}

} // end of namespace qtbrynhildr
