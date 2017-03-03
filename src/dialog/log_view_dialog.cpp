// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
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
  topPos(0),
  lastPos(0),
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
#if defined(QTB_DEV_TABLET)
  QRect currentScreen = settings->getDesktop()->getCurrentScreen();
  int desktopWidth = currentScreen.width();
  int desktopHeight = currentScreen.height();
  int dialogWidth = desktopWidth-200;
  int dialogHeight = desktopHeight-200;
  int fontPointSize = 14;

  // resetting dialog window size and font size
  resize(dialogWidth, dialogHeight);
  layoutWidget->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
  QFont currentFont = font();
  currentFont.setPointSize(fontPointSize);
  setFont(currentFont);
#endif // defined(QTB_DEV_TABLET)
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
	  logFileStream->seek(topPos);
	  logFileStream->setCodec("UTF-8");
	  logText = logFileStream->readAll();
	  // save last position for Reset
	  lastPos = logFileStream->pos();
	  logFile->close();
  }
  clear();
  setPlainText(logText);
  QDialog::show();
}

// reset
void LogViewDialog::on_buttonBox_clicked(QAbstractButton *button)
{
  if (button->text() == "Reset"){
	// clear log
	//	cout << "Clear log!" << endl <<flush;
	topPos = lastPos;
	show();
  }
#if 0 // for DEBUG
  else {
	cout << "Other!" << endl <<flush;
  }
#endif // for DEBUG
}

} // end of namespace qtbrynhildr
