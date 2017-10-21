// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QDialog>
#include <QDir>
#include <QFileDialog>
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
  changed(false),
  resultOfSetToSettings(false),
  // for DEBUG
  outputLog(false)
{
  setupUi(this);

  // resetting
  resetting();

  // fix size
  setFixedSize(size());

  // publicModeVersion
#if QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicModeVersion->insertItem(MODE_PUBLIC5 - MODE_PUBLIC5, "MODE 5");
  comboBox_publicModeVersion->insertItem(MODE_PUBLIC6 - MODE_PUBLIC5, "MODE 6");
#if QTB_PUBLIC_MODE7_SUPPORT
  comboBox_publicModeVersion->insertItem(MODE_PUBLIC7 - MODE_PUBLIC5, "MODE 7");
#endif // QTB_PUBLIC_MODE7_SUPPORT
#else // QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicModeVersion->insertItem(0, "MODE 5");
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // serverNameListSize
  spinBox_serverNameListSize->setRange(5, 100);	// for TEST

  // convertThreadCount
  comboBox_convertThreadCount->insertItem(0, tr("none"));
  comboBox_convertThreadCount->insertItem(1, tr("2 threads"));
  comboBox_convertThreadCount->insertItem(2, tr("4 threads"));
  //  comboBox_convertThreadCount->insertItem(3, tr("8 threads"));

  // doubleClickThreshold
  spinBox_doubleClickThreshold->setRange(100, 1000); // for TEST

  // graphicsBufferSize
  spinBox_graphicsBufferSize->setRange(256, 10240); // for TEST

  // soundBufferSize
  spinBox_soundBufferSize->setRange(256, 1024); // for TEST

  // set Read Only
  lineEdit_keylayoutPath->setReadOnly(true);
  lineEdit_outputPath->setReadOnly(true);
  lineEdit_logFile->setReadOnly(true);
  lineEdit_keyboardLogFile->setReadOnly(true);
#if QTB_PORTABLE_VERSION
#if 0
  lineEdit_keylayoutPath->setEnabled(false);
  lineEdit_outputPath->setEnabled(false);
  lineEdit_logFile->setEnabled(false);
  lineEdit_keyboardLogFile->setEnabled(false);
#endif
  pushButton_keylayoutPath->setEnabled(false);
  pushButton_outputPath->setEnabled(false);
  pushButton_logFile->setEnabled(false);
  pushButton_keyboardLogFile->setEnabled(false);
#endif // QTB_PORTABLE_VERSION

  connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(clicked(QAbstractButton *)));
}

// resize event
void PreferenceDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
}

// set keylayout file list
void PreferenceDialog::setKeylayoutList(const QStringList keylayoutList)
{
  listWidget_keylayoutList->addItems(keylayoutList);
}

// get from settings
void PreferenceDialog::getFromSettings()
{
  // publicModeVersion
#if QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicModeVersion->setCurrentIndex(settings->getPublicModeVersion() - MODE_PUBLIC5);
  publicModeVersionChanged(settings->getPublicModeVersion());
#else // QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicModeVersion->setCurrentIndex(0);
  publicModeVersionChanged(MODE_PUBLIC5);
#endif // QTB_PUBLIC_MODE6_SUPPORT

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
  spinBox_serverNameListSize->setValue(settings->getServerNameListSize());

  // convertThreadCount
  comboBox_convertThreadCount->setCurrentIndex(settings->getConvertThreadCount()/2);

  // keylayoutPath
  lineEdit_keylayoutPath->setText(settings->getKeylayoutPath());

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

  // graphicsBufferSize
  spinBox_graphicsBufferSize->setValue(settings->getGraphicsBufferSize()/1024);

  // soundBufferSize
  spinBox_soundBufferSize->setValue(settings->getSoundBufferSize()/1024);

  // outputPath
  lineEdit_outputPath->setText(settings->getOutputPath());

  // logFile
  lineEdit_logFile->setText(settings->getLogFile());

  // keyboardLogFile
  lineEdit_keyboardLogFile->setText(settings->getKeyboardLogFile());

  // onGamePadSupport
  checkBox_onGamePadSupport->
	setCheckState(settings->getOnGamePadSupport() ? Qt::Checked : Qt::Unchecked);
#if !defined(Q_OS_WIN)
  checkBox_onGamePadSupport->setEnabled(false);
#endif // defined(Q_OS_WIN)
}

// set to settings
bool PreferenceDialog::setToSettings()
{
  bool result = true;

  // publicModeVersion
#if QTB_PUBLIC_MODE6_SUPPORT
  settings->
	setPublicModeVersion(comboBox_publicModeVersion->currentIndex() + MODE_PUBLIC5);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // onOpenConnectToServerDialogAtBootup
  settings->
	setOnOpenConnectToServerDialogAtBootup(checkBox_onOpenConnectToServerDialogAtBootup->checkState() == Qt::Checked);

  // onConfrimAtExit
  settings->
	setOnConfirmAtExit(checkBox_onConfirmAtExit->checkState() == Qt::Checked);

  // onCheckUpdateAtBootup
  settings->
	setOnCheckUpdateAtBootup(checkBox_onCheckUpdateAtBootup->checkState() == Qt::Checked);

  // serverNameListSize
  settings->
	setServerNameListSize(spinBox_serverNameListSize->value());

  // convertThreadCount
  settings->
	setConvertThreadCount(comboBox_convertThreadCount->currentIndex()*2);

  // keylayoutPath
  settings->
	setKeylayoutPath(lineEdit_keylayoutPath->text());

  // onHoldMouseControl
  settings->
	setOnHoldMouseControl(checkBox_onHoldMouseControl->checkState() == Qt::Checked);

  // onExtraButtonSupport
  settings->
	setOnExtraButtonSupport(checkBox_onExtraButtonSupport->checkState() == Qt::Checked);

  // onDisplayMouseCursor
  settings->
	setOnDisplayMouseCursor(checkBox_onDisplayMouseCursor->checkState() == Qt::Checked);

  // onShowMouseCursorMarker
  settings->
	setOnShowMouseCursorMarker(checkBox_onShowMouseCursorMarker->checkState() == Qt::Checked);

  // doubleClickThreshold
  settings->
	setDoubleClickThreshold(spinBox_doubleClickThreshold->value());

  // onTransferFileSupport
#if QTB_PUBLIC_MODE6_SUPPORT
  settings->
	setOnTransferFileSupport(checkBox_onTransferFileSupport->checkState() == Qt::Checked);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // onTransferFileSupportByDragAndDrop
#if QTB_DRAG_AND_DROP_SUPPORT
  settings->
	setOnTransferFileSupportByDragAndDrop(checkBox_onTransferFileSupportByDragAndDrop->checkState() == Qt::Checked);
#endif // QTB_DRAG_AND_DROP_SUPPORT

  // onShowTotalProgressForTransferFile
#if QTB_DRAG_AND_DROP_SUPPORT
  settings->
	setOnShowTotalProgressForTransferFile(checkBox_onShowTotalProgressForTransferFile->checkState() == Qt::Checked);
#endif // QTB_DRAG_AND_DROP_SUPPORT

  // onTransferClipboardSupport
#if QTB_PUBLIC_MODE6_SUPPORT
  settings->
	setOnTransferClipboardSupport(checkBox_onTransferClipboardSupport->checkState() == Qt::Checked);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // graphicsBufferSize
  settings->
	setGraphicsBufferSize(spinBox_graphicsBufferSize->value()*1024);

  // soundBufferSize
  settings->
	setSoundBufferSize(spinBox_soundBufferSize->value()*1024);

  // outputPath
  settings->
	setOutputPath(lineEdit_outputPath->text());

  // logFile
  settings->
	setLogFile(lineEdit_logFile->text());

  // keyboardLogFile
  settings->
	setKeyboardLogFile(lineEdit_keyboardLogFile->text());

  // onGamePadSupport
  settings->
	setOnGamePadSupport(checkBox_onGamePadSupport->checkState() == Qt::Checked);

  return result;
}

// settings for Tablet
void PreferenceDialog::resetting()
{
#if defined(QTB_DEV_TOUCHPANEL)
  QRect currentScreen = settings->getDesktop()->getCurrentScreen();
  int desktopWidth = currentScreen.width();
  int desktopHeight = currentScreen.height();

  int dialogWidth = desktopWidth * 0.5;
  int dialogHeight = desktopHeight * 0.5;
  int fontPointSize = 14;

  // set minimum width

  // resetting dialog window size and font size
  resize(dialogWidth, dialogHeight);
  layoutWidget_1->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
  QFont currentFont = font();
  currentFont.setPointSize(fontPointSize);
  setFont(currentFont);
#else // defined(QTB_DEV_TOUCHPANEL)
  // set minimum width

#endif // defined(QTB_DEV_TOUCHPANEL)
}

// show Event
void PreferenceDialog::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);

  getFromSettings();

  resultOfSetToSettings = true;
}

//---------------------------------------------------------------------------
// private slot
//---------------------------------------------------------------------------
// accept button
void PreferenceDialog::accept()
{
  if (outputLog)
	cout << "accept()." << endl << flush; // for DEBUG
  if (resultOfSetToSettings)
	hide();
}

#if 0
// reject button
void PreferenceDialog::reject()
{
  if (outputLog)
	cout << "reject()." << endl << flush; // for DEBUG
  hide();
}
#endif

// clicked button
void PreferenceDialog::clicked(QAbstractButton *button)
{
  if (outputLog)
	cout << "clicked()." << endl << flush; // for DEBUG

  if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole ||
	  buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole){
	if (outputLog){
	  if (buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
		cout << "clicked(Accept)." << endl << flush; // for DEBUG
	  else
		cout << "clicked(Apply)." << endl << flush; // for DEBUG
	}

	if (changed){
	  // set values to settings
	  resultOfSetToSettings = setToSettings();
	}
  }
  else if (buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole){
	if (outputLog)
	  cout << "clicked(Reject)." << endl << flush; // for DEBUG
	// ignore change
  }

  // reset changed flag
  changed = false;
}

void PreferenceDialog::publicModeVersionChanged(int version)
{
  // change enabled state
  if (version < MODE_PUBLIC6){
	// MODE5
	checkBox_onTransferFileSupport->setEnabled(false);

	checkBox_onTransferFileSupportByDragAndDrop->setEnabled(false);

	checkBox_onShowTotalProgressForTransferFile->setEnabled(false);

	checkBox_onTransferClipboardSupport->setEnabled(false);
  }
  else {
	// MODE6/7
	checkBox_onTransferFileSupport->setEnabled(true);

	checkBox_onTransferFileSupportByDragAndDrop->setEnabled(true);

	checkBox_onShowTotalProgressForTransferFile->setEnabled(true);

	checkBox_onTransferClipboardSupport->setEnabled(true);
  }
}

void PreferenceDialog::on_comboBox_publicModeVersion_currentIndexChanged(int index)
{
  Q_UNUSED(index);

  publicModeVersionChanged(index + MODE_PUBLIC5);

  changed = true;
}

void PreferenceDialog::on_checkBox_onOpenConnectToServerDialogAtBootup_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}

void PreferenceDialog::on_checkBox_onConfirmAtExit_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}

void PreferenceDialog::on_checkBox_onCheckUpdateAtBootup_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}

void PreferenceDialog::on_spinBox_serverNameListSize_valueChanged(int i)
{
  Q_UNUSED(i);

  changed = true;
}

void PreferenceDialog::on_comboBox_convertThreadCount_currentIndexChanged(int index)
{
  Q_UNUSED(index);

  changed = true;
}

void PreferenceDialog::on_lineEdit_keylayoutPath_textChanged()
{
  changed = true;
}

void PreferenceDialog::on_checkBox_onHoldMouseControl_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}

void PreferenceDialog::on_checkBox_onDisplayMouseCursor_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}

void PreferenceDialog::on_checkBox_onShowMouseCursorMarker_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}

void PreferenceDialog::on_spinBox_doubleClickThreshold_valueChanged(int i)
{
  Q_UNUSED(i);

  changed = true;
}

#if QTB_PUBLIC_MODE6_SUPPORT
void PreferenceDialog::on_checkBox_onTransferFileSupport_stateChanged(int state)
{

  checkBox_onTransferFileSupportByDragAndDrop->setEnabled(state == Qt::Checked);
  checkBox_onShowTotalProgressForTransferFile->setEnabled(state == Qt::Checked);

  changed = true;
}
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_DRAG_AND_DROP_SUPPORT
void PreferenceDialog::on_checkBox_onTransferFileSupportByDragAndDrop_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}
#endif // QTB_DRAG_AND_DROP_SUPPORT

#if QTB_DRAG_AND_DROP_SUPPORT
void PreferenceDialog::on_checkBox_onShowTotalProgressForTransferFile_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}
#endif // QTB_DRAG_AND_DROP_SUPPORT

#if QTB_PUBLIC_MODE6_SUPPORT
void PreferenceDialog::on_checkBox_onTransferClipboardSupport_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}
#endif // QTB_PUBLIC_MODE6_SUPPORT

void PreferenceDialog::on_spinBox_graphicsBufferSize_valueChanged(int i)
{
  Q_UNUSED(i);

  changed = true;
}

void PreferenceDialog::on_spinBox_soundBufferSize_valueChanged(int i)
{
  Q_UNUSED(i);

  changed = true;
}

void PreferenceDialog::on_lineEdit_outputPath_textChanged()
{
  changed = true;
}

void PreferenceDialog::on_lineEdit_logFile_textChanged()
{
  changed = true;
}

void PreferenceDialog::on_lineEdit_keyboardLogFile_textChanged()
{
  changed = true;
}

void PreferenceDialog::on_checkBox_onGamePadSupport_stateChanged(int state)
{
  Q_UNUSED(state);

  changed = true;
}


// select button of key layout path
void PreferenceDialog::on_pushButton_keylayoutPath_clicked()
{
  if (outputLog)
	cout << "on_pushButton_keylayoutPath_clicked()." << endl << flush; // for DEBUG

  QString dir = QFileDialog::getExistingDirectory(this,
												  tr("Select path"),
												  ".",
												  QFileDialog::ShowDirsOnly |
												  QFileDialog::DontResolveSymlinks);
  if (dir == ""){
	// Nothing to do
	return;
  }

  // keylayoutPath
  dir = QDir::toNativeSeparators(dir);
  lineEdit_keylayoutPath->setText(dir);
  changed = true;

  if (outputLog)
	cout << "Open Directory : " << qPrintable(dir) << endl << flush;
}

// select button of output path
void PreferenceDialog::on_pushButton_outputPath_clicked()
{
  if (outputLog)
	cout << "on_pushButton_outputPath_clicked()." << endl << flush; // for DEBUG

  QString dir = QFileDialog::getExistingDirectory(this,
												  tr("Select path"),
												  ".",
												  QFileDialog::ShowDirsOnly |
												  QFileDialog::DontResolveSymlinks);
  if (dir == ""){
	// Nothing to do
	return;
  }

  // outputPath
  dir = QDir::toNativeSeparators(dir);
  lineEdit_outputPath->setText(dir);
  changed = true;

  if (outputLog)
	cout << "Open Directory : " << qPrintable(dir) << endl << flush;
}

// select button of logfile
void PreferenceDialog::on_pushButton_logFile_clicked()
{
  if (outputLog)
	cout << "on_pushButton_logFile_clicked()." << endl << flush; // for DEBUG

  // prepare for log file
  QString fileName =
	QFileDialog::getSaveFileName(this,
								 tr("Select file"),
								 ".");
  if (fileName == ""){
	// Nothing to do
	return;
  }

  // logFile
  fileName = QDir::toNativeSeparators(fileName);
  lineEdit_logFile->setText(fileName);
  changed = true;

  if (outputLog)
	cout << "Open File : " << qPrintable(fileName) << endl << flush;
}

// select button of keyboard logfile
void PreferenceDialog::on_pushButton_keyboardLogFile_clicked()
{
  if (outputLog)
	cout << "on_pushButton_keyboardLogFile_clicked()." << endl << flush; // for DEBUG

  // prepare for keyboard log file
  QString fileName =
	QFileDialog::getSaveFileName(this,
								 tr("Select file"),
								 ".");
  if (fileName == ""){
	// Nothing to do
	return;
  }

  // keyboardLogFile
  fileName = QDir::toNativeSeparators(fileName);
  lineEdit_keyboardLogFile->setText(fileName);
  changed = true;

  if (outputLog)
	cout << "Open File : " << qPrintable(fileName) << endl << flush;
}

} // end of namespace qtbrynhildr
