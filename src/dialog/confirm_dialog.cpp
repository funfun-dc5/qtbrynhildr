// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QDialog>
#include <QRect>

// Local Header
#include "confirm_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
ConfirmDialog::ConfirmDialog(QString msg,
							 bool confirmFlag,
							 Settings *settings,
							 QWidget *parent)
  :QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
  ,settings(settings)
  // for DEBUG
  ,outputLog(false)
{
  setupUi(this);

  // set confirm message
  message->setText(msg);

  // confirm field
  this->confirmFlag = confirmFlag;
  checkBox_confirm->setCheckState(confirmFlag ? Qt::Checked : Qt::Unchecked);

  // resetting
  resetting();

  // fix size
  setFixedSize(size());
}

// resize event
void ConfirmDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
}

// get confirm flag
bool ConfirmDialog::getConfirmFlag() const
{
  if (outputLog)
	cout << "getConfirmFlag() : confirmFlag : " << confirmFlag << endl << flush; // for DEBUG
  return confirmFlag;
}

// settings for Tablet
void ConfirmDialog::resetting()
{
#if defined(QTB_DEV_TOUCHPANEL)
  int screenWidth = settings->getCurrentScreenWidth();
  int screenHeight = settings->getCurrentScreenHeight();
  int dialogWidth = screenWidth * 0.5;
  int dialogHeight = screenHeight * 0.8;
  int fontPointSize = 14;

  // resetting dialog window size and font size
  resize(dialogWidth, dialogHeight);
  layoutWidget->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
  QFont currentFont = font();
  currentFont.setPointSize(fontPointSize);
  setFont(currentFont);
#endif // defined(QTB_DEV_TOUCHPANEL)
}

//---------------------------------------------------------------------------
// private slot
//---------------------------------------------------------------------------
// show password field
void ConfirmDialog::on_checkBox_confirm_stateChanged(int state)
{
  confirmFlag = (state == Qt::Checked);

  if (outputLog)
	cout << "stateChanged() : confirmFlag : " << confirmFlag << endl << flush; // for DEBUG
}

} // end of namespace qtbrynhildr
