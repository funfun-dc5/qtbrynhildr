// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

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
#include "ui_connect_to_server_dialog.h"

namespace qtbrynhildr {

// ConnectToServerDialog
class ConnectToServerDialog : public QDialog, public Ui::ConnectToServerDialog
{
  Q_OBJECT

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

private:
  // resettings for dialog
  void resetting();

private slots:
  // hostname field
  void on_comboBox_hostname_currentIndexChanged(int index);
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

private:
  // settings
  Settings *settings;

#if QTB_AUTO_COMPLETE
  // server name list
  QStringList *serverNameList;

  // completer
  QCompleter *completer;
#endif // QTB_AUTO_COMPLETE

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // CONNECT_TO_SERVER_DIALOG_H
