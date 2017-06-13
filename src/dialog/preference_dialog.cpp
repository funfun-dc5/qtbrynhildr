// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QDialog>
#include <QRect>

// Local Header
#include "preference_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
PreferenceDialog::PreferenceDialog(Settings *settings,
								   QWidget *parent)
  :
  //  QDialog(parent),
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  settings(settings),
  // for DEBUG
  outputLog(false)
{
  setupUi(this);

  // resetting
  resetting();

  // fix size
  setFixedSize(size());
}

// resize event
void PreferenceDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
}

// settings for Tablet
void PreferenceDialog::resetting()
{
#if defined(QTB_DEV_TABLET)
  QRect currentScreen = settings->getDesktop()->getCurrentScreen();
  int desktopWidth = currentScreen.width();
  int desktopHeight = currentScreen.height();

  int dialogWidth = desktopWidth * 0.5;
  int dialogHeight = desktopHeight * 0.5;
  int fontPointSize = 14;

  // resetting dialog window size and font size
  resize(dialogWidth, dialogHeight);
  layoutWidget->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
  QFont currentFont = font();
  currentFont.setPointSize(fontPointSize);
  setFont(currentFont);
#endif // defined(QTB_DEV_TABLET)
}

// show Event
void PreferenceDialog::showEvent(QShowEvent *event)
{
  Q_UNUSED(event)
}

//---------------------------------------------------------------------------
// private slot
//---------------------------------------------------------------------------
// accept button
void PreferenceDialog::accept()
{
  if (outputLog)
	cout << "accept()."; // for DEBUG
  hide();
}

// reject button
void PreferenceDialog::reject()
{
  if (outputLog)
	cout << "reject()."; // for DEBUG
  hide();
}

} // end of namespace qtbrynhildr
