// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef CONNECT_TO_SERVER_DIALOG_H
#define CONNECT_TO_SERVER_DIALOG_H
// Common Header
#include "common/common.h"

// Qt Header
#if QTB_AUTO_COMPLETE
#include <QCompleter>
#endif // QTB_AUTO_COMPLETE
#include <QDialog>
#include <QResizeEvent>
#if QTB_AUTO_COMPLETE
#include <QStringList>
#endif // QTB_AUTO_COMPLETE

// Local Header
#include "settings.h"
#if defined(QTB_DEV_TOUCHPANEL)
#include "ui_connect_to_server_dialog_touchpanel.h"
#else // defined(QTB_DEV_TOUCHPANEL)
#include "ui_connect_to_server_dialog.h"
#endif // defined(QTB_DEV_TOUCHPANEL)

namespace qtbrynhildr {

// ConnectToServerDialog
class ConnectToServerDialog : public QDialog, public Ui::ConnectToServerDialog
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // settings
  Settings *settings;

#if QTB_AUTO_COMPLETE
  // server name list
  QStringList *serverNameList;

  // completer
  QCompleter *completer;
#endif // QTB_AUTO_COMPLETE

#if defined(QTB_DEV_TOUCHPANEL)
  // current server name list index
  int serverNameListIndex;
#endif // defined(QTB_DEV_TOUCHPANEL)

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  ConnectToServerDialog(Settings *settings, QWidget *parent = 0);
  // destructor
  ~ConnectToServerDialog();

  // resize event
  void resizeEvent(QResizeEvent *event);

  // add keyboard type list by key layout file
  void addKeyboardTypeList(QStringList list);

  // set keyboard type
  void setKeyboardType(KEYBOARD_TYPE keyboardType);

#if defined(QTB_DEV_TOUCHPANEL)
protected:
  // event
  bool event(QEvent *event);
#endif // defined(QTB_DEV_TOUCHPANEL)

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

private slots:
  // hostname field
#if defined(QTB_DEV_TOUCHPANEL)
  void on_lineEdit_hostname_textChanged();
#else // defined(QTB_DEV_TOUCHPANEL)
  void on_comboBox_hostname_currentIndexChanged(int index);
#endif // defined(QTB_DEV_TOUCHPANEL)
  // hosttype field
  void on_comboBox_hosttype_currentIndexChanged(int index);
  // keyboard type field
  void on_comboBox_keyboardtype_currentIndexChanged(int index);
  // port no field
  void on_spinBox_portno_valueChanged(int i);
  // password field
  void on_lineEdit_password_textChanged();
  // public mode field
  void on_comboBox_publicmode_currentIndexChanged(int index);
  // show password field
  void on_checkBox_showPassword_stateChanged(int state);
  // show password field
  void on_checkBox_fullScreen_stateChanged(int state);

  // accept button
  void accept();
  // reject button
  void reject();

signals:
  // connect to server
  void connectToServer();
};

} // end of namespace qtbrynhildr

#endif // CONNECT_TO_SERVER_DIALOG_H
