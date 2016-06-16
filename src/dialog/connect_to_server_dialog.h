// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// connect_to_server_dialog.h

#ifndef CONNECT_TO_SERVER_DIALOG_H
#define CONNECT_TO_SERVER_DIALOG_H
// Qt Header
#include <QDialog>
#include <QResizeEvent>

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

  // resize event
  void resizeEvent(QResizeEvent *event);

private:
  // resettings for dialog
  void resetting();

private slots:
  // hostname field
  void on_lineEdit_hostname_textChanged();
  // hosttype field
  void on_comboBox_hosttype_currentIndexChanged(int index);
  // keyboard type field
  void on_comboBox_keyboardtype_currentIndexChanged(int index);
  // port no field
  void on_spinBox_portno_valueChanged(int i);
  // password field
  void on_lineEdit_password_textChanged();
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

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // CONNECT_TO_SERVER_DIALOG_H
