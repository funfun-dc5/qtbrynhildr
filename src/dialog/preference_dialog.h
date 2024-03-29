// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef PREFERENCE_DIALOG_H
#define PREFERENCE_DIALOG_H
// Qt Header
#include <QDialog>
#include <QFont>
#include <QResizeEvent>
#include <QStringList>

// Local Header
#include "settings.h"
#include "ui_preference_dialog.h"

namespace qtbrynhildr {

// PreferenceDialog
class PreferenceDialog : public QDialog, public Ui::PreferenceDialog
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // settings
  Settings *settings;

  // apply button
  QPushButton *applyButton;

  // changed flag
  bool changed;

  // check result
  bool resultOfSetToSettings;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  PreferenceDialog(Settings *settings, QWidget *parent = nullptr);

  // resize event
  void resizeEvent(QResizeEvent *event);

  // set keylayout file list
  void setKeylayoutList(const QStringList keylayoutList);

  // set decoder name list
  void setDecoderNameList(QStringList list);

protected:
  // show Event
  void showEvent(QShowEvent *event);

private:
  // get from settings
  void getFromSettings();

  // set to settings
  bool setToSettings();

  // resettings for dialog
  void resetting();

  // changed settings
  void changedSettings()
  {
	// enable apply button
	applyButton->setEnabled(true);

	changed = true;
  }

private slots:
  // accept button
  void accept();

  // clicked button
  void clicked(QAbstractButton *button);

  void publicModeVersionChanged(int version);

  void on_comboBox_publicModeVersion_currentIndexChanged(int index);

  void on_checkBox_onOpenConnectToServerDialogAtBootup_stateChanged(int state);

  void on_checkBox_onConfirmAtExit_stateChanged(int state);

  void on_checkBox_onCheckUpdateAtBootup_stateChanged(int state);

  void on_checkBox_desktopScalingType_stateChanged(int state);

  void on_comboBox_SIMDOperationTypeName_currentIndexChanged(int index);

  void on_spinBox_serverNameListSize_valueChanged(int i);

  void on_comboBox_convertThreadCount_currentIndexChanged(int index);

  void on_lineEdit_keylayoutPath_textChanged();

  void on_checkBox_onHoldMouseControl_stateChanged(int state);

  void on_checkBox_onShowMouseCursorMarker_stateChanged(int state);

  void on_spinBox_doubleClickThreshold_valueChanged(int i);

  void on_checkBox_onTransferFileSupport_stateChanged(int state);

#if QTB_DRAG_AND_DROP_SUPPORT
  void on_checkBox_onTransferFileSupportByDragAndDrop_stateChanged(int state);
#endif // QTB_DRAG_AND_DROP_SUPPORT

#if QTB_DRAG_AND_DROP_SUPPORT
  void on_checkBox_onShowTotalProgressForTransferFile_stateChanged(int state);
#endif // QTB_DRAG_AND_DROP_SUPPORT

  void on_checkBox_onTransferClipboardSupport_stateChanged(int state);

  void on_spinBox_graphicsBufferSize_valueChanged(int i);

  void on_spinBox_soundBufferSize_valueChanged(int i);

  void on_lineEdit_outputPath_textChanged();

  void on_lineEdit_logFile_textChanged();

  void on_lineEdit_keyboardLogFile_textChanged();

  void on_checkBox_onGamePadSupport_stateChanged(int state);

  void on_pushButton_keylayoutPath_clicked();

  void on_pushButton_outputPath_clicked();

  void on_pushButton_logFile_clicked();

  void on_pushButton_keyboardLogFile_clicked();
};

} // end of namespace qtbrynhildr


#endif // PREFERENCE_DIALOG_H
