// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QDialog>
#include <QFile>
#include <QPushButton>
#include <QRect>
#include <QTextStream>

#if defined(QTB_DEBUG)
#include <QDebug>
#endif // defined(QTB_DEBUG)

// Local Header
#include "log_view_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
LogViewDialog::LogViewDialog(Settings *settings,
							 QWidget *parent)
  :QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
  ,settings(settings)
  ,topPos(0)
  ,lastPos(0)
{
  setupUi(this);

  // Read Only
  textEdit->setReadOnly(true);

  // select All button
  QPushButton *selectAllButton = buttonBox->addButton(tr("Select All"), QDialogButtonBox::ActionRole);
  connect(selectAllButton, SIGNAL(pressed()), textEdit, SLOT(selectAll()));

  // Copy button
  QPushButton *copyButton = buttonBox->addButton(tr("Copy"), QDialogButtonBox::ActionRole);
  connect(copyButton, SIGNAL(pressed()), textEdit, SLOT(copy()));

  // resetting
  resetting();

  // fix size
  setFixedSize(size());
}

// resize event
void LogViewDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
}

// settings for Tablet
void LogViewDialog::resetting()
{
#if defined(QTB_DEV_TOUCHPANEL)
  int screenWidth = settings->getCurrentScreenWidth();
  int screenHeight = settings->getCurrentScreenHeight();
  int dialogWidth = screenWidth-200;
  int dialogHeight = screenHeight-400;
  int fontPointSize = 14;

  // resetting dialog window size and font size
  resize(dialogWidth, dialogHeight);
  layoutWidget->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
  QFont currentFont = font();
  currentFont.setPointSize(fontPointSize);
  setFont(currentFont);
#endif // defined(QTB_DEV_TOUCHPANEL)
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
	  if (logFileStream != 0){
		delete logFileStream;
	  }
  }
  clear();
  setPlainText(logText);
  //qDebug() << "topPos : " << topPos;
  //qDebug() << "lastPos: " << lastPos;
  if (logFile != 0){
	delete logFile;
	logFile = 0;
  }
  QDialog::show();
}

// reset
void LogViewDialog::on_buttonBox_clicked(QAbstractButton *button)
{
  if (button->text() == tr("Reset") || button->text() == "Reset"){ // for Android(Bug?)
	// clear log
	//	cout << "Clear log!" << endl <<flush;
	topPos = lastPos;
	//qDebug() << "Reset: topPos: " << lastPos;
	show();
  }
#if 0 // for DEBUG
  else {
	qDebug()<< "Reset: unknown text! : " << button->text();
  }
#endif // 0 // for DEBUG
}

} // end of namespace qtbrynhildr
