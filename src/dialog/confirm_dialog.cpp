// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// confirm_dialog.cpp

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt header
#include <QDialog>

// Local Header
#include "confirm_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
ConfirmDialog::ConfirmDialog(QString msg,
							 bool confirmFlag,
							 QWidget *parent)
  :
  //  QDialog(parent),
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  // for DEBUG
  outputLog(false)
{
  setupUi(this);

  // set confirm message
  message->setText(msg);

  // confirm field
  this->confirmFlag = confirmFlag;
  checkBox_confirm->setCheckState(confirmFlag ? Qt::Checked : Qt::Unchecked);

  // resetting
  resetting();
}

// resize event
void ConfirmDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event)
}

// get confirm flag
bool ConfirmDialog::getConfirmFlag() const
{
  if (outputLog)
	cout << "getConfirmFlag() : confirmFlag : " << confirmFlag << endl << flush; // for DEBUG
  return confirmFlag;
}

#if defined(QTB_DEV_TABLET)
  // settings for Tablet
void ConfirmDialog::resetting()
{
  // resetting dialog window size and font size
  resize(600, 300);
  layoutWidget->setGeometry(QRect(20, 20, 540, 260));
  QFont currentFont = font();
  currentFont.setPointSize(14);
  setFont(currentFont);
}
#else // defined(QTB_DEV_TABLET)
void ConfirmDialog::resetting()
{
  // Nothing to do
}
#endif // defined(QTB_DEV_TABLET)

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
