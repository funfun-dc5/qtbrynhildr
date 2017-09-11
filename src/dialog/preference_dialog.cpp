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

  setup();

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

// setup
void PreferenceDialog::setup()
{
  // publicModeVersion
#if QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicModeVersion->insertItem(MODE_PUBLIC5 - MODE_PUBLIC5, "MODE 5");
  comboBox_publicModeVersion->insertItem(MODE_PUBLIC6 - MODE_PUBLIC5, "MODE 6");
#if QTB_PUBLIC_MODE7_SUPPORT
  comboBox_publicModeVersion->insertItem(MODE_PUBLIC7 - MODE_PUBLIC5, "MODE 7");
#endif // QTB_PUBLIC_MODE7_SUPPORT
  comboBox_publicModeVersion->setCurrentIndex(settings->getPublicModeVersion() - MODE_PUBLIC5);
#else // QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicModeVersion->insertItem(0, "MODE 5");
  comboBox_publicModeVersion->setCurrentIndex(0);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // onBrynhildr2Support
  checkBox_onBrynhildr2Support->
	setCheckState(settings->getOnBrynhildr2Support() ? Qt::Checked : Qt::Unchecked);

  // onOpenConnectToServerDialogAtBootup
  checkBox_onOpenConnectToServerDialogAtBootup->
	setCheckState(settings->getOnOpenConnectToServerDialogAtBootup() ? Qt::Checked : Qt::Unchecked);

  // onConfrimAtExit
  checkBox_onConfirmAtExit->
	setCheckState(settings->getOnConfirmAtExit() ? Qt::Checked : Qt::Unchecked);

  // onCheckUpdateAtBootup
  checkBox_onCheckUpdateAtBootup->
	setCheckState(settings->getOnCheckUpdateAtBootup() ? Qt::Checked : Qt::Unchecked);

  // serverNameListSize
  spinBox_serverNameListSize->setMinimum(5);	// for TEST
  spinBox_serverNameListSize->setMaximum(100);	// for TEST
  spinBox_serverNameListSize->setValue(settings->getServerNameListSize());

  // keylayoutPath
  lineEdit_keylayoutPath->insert(settings->getKeylayoutPath());

  // onHoldMouseControl
  checkBox_onHoldMouseControl->
	setCheckState(settings->getOnHoldMouseControl() ? Qt::Checked : Qt::Unchecked);

  // onExtraButtonSupport
  checkBox_onExtraButtonSupport->
	setCheckState(settings->getOnExtraButtonSupport() ? Qt::Checked : Qt::Unchecked);

  // onDisplayMouseCursor
  checkBox_onDisplayMouseCursor->
	setCheckState(settings->getOnDisplayMouseCursor() ? Qt::Checked : Qt::Unchecked);

  // onShowMouseCursorMarker
  checkBox_onShowMouseCursorMarker->
	setCheckState(settings->getOnShowMouseCursorMarker() ? Qt::Checked : Qt::Unchecked);

  // doubleClickThreshold
  spinBox_doubleClickThreshold->setMinimum(100);	// for TEST
  spinBox_doubleClickThreshold->setMaximum(1000);	// for TEST
  spinBox_doubleClickThreshold->setValue(settings->getDoubleClickThreshold());

  // onTransferFileSupport
#if QTB_PUBLIC_MODE6_SUPPORT
  checkBox_onTransferFileSupport->
	setCheckState(settings->getOnTransferFileSupport() ? Qt::Checked : Qt::Unchecked);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // onTransferFileSupportByDragAndDrop
#if QTB_DRAG_AND_DROP_SUPPORT
  checkBox_onTransferFileSupportByDragAndDrop->
	setCheckState(settings->getOnTransferFileSupportByDragAndDrop() ? Qt::Checked : Qt::Unchecked);
#endif // QTB_DRAG_AND_DROP_SUPPORT

  // onShowTotalProgressForTransferFile
#if QTB_DRAG_AND_DROP_SUPPORT
  checkBox_onShowTotalProgressForTransferFile->
	setCheckState(settings->getOnShowTotalProgressForTransferFile() ? Qt::Checked : Qt::Unchecked);
#endif // QTB_DRAG_AND_DROP_SUPPORT

  // onTransferClipboardSupport
#if QTB_PUBLIC_MODE6_SUPPORT
  checkBox_onTransferClipboardSupport->
	setCheckState(settings->getOnTransferClipboardSupport() ? Qt::Checked : Qt::Unchecked);
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if !QTB_PUBLIC_MODE6_SUPPORT
  // onTransferFileSupport
  checkBox_onTransferFileSupport->setEnabled(false);

  // onTransferFileSupportByDragAndDrop
  checkBox_onTransferFileSupportByDragAndDrop->setEnabled(false);

  // onShowTotalProgressForTransferFile
  checkBox_onShowTotalProgressForTransferFile->setEnabled(false);

  // onTransferClipboardSupport
  checkBox_onTransferClipboardSupport->setEnabled(false);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // graphicsBufferSize
  spinBox_graphicsBufferSize->setMinimum(256);		// for TEST
  spinBox_graphicsBufferSize->setMaximum(10240);	// for TEST
  spinBox_graphicsBufferSize->setValue(settings->getGraphicsBufferSize()/1024);

  // soundBufferSize
  spinBox_soundBufferSize->setMinimum(256);		// for TEST
  spinBox_soundBufferSize->setMaximum(10240);	// for TEST
  spinBox_soundBufferSize->setValue(settings->getSoundBufferSize()/1024);

  // outputPath
  lineEdit_outputPath->insert(settings->getOutputPath());

  // logFile
  lineEdit_logFile->insert(settings->getLogFile());

  // keyboardLogFile
  lineEdit_keyboardLogFile->insert(settings->getKeyboardLogFile());

  // onGamePadSupport
  checkBox_onGamePadSupport->
	setCheckState(settings->getOnGamePadSupport() ? Qt::Checked : Qt::Unchecked);

  connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(clicked(QAbstractButton *)));
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
  layoutWidget_1->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
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
	cout << "accept()." << endl << flush; // for DEBUG
  hide();
}

// reject button
void PreferenceDialog::reject()
{
  if (outputLog)
	cout << "reject()." << endl << flush; // for DEBUG
  hide();
}

// clicked button
void PreferenceDialog::clicked(QAbstractButton *button)
{
  if (outputLog)
	cout << "clicked()." << endl << flush; // for DEBUG

  if (button->text() == "Apply"){
	cout << "clicked(Apply)." << endl << flush; // for DEBUG
  }
}

} // end of namespace qtbrynhildr
