// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// connect_to_server_dialog.cpp

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt header
#include <QDialog>
#include <QFont>
#include <QRect>

// Local Header
#include "connect_to_server_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
ConnectToServerDialog::ConnectToServerDialog(Settings *settings,
											 QWidget *parent)
  :
  //  QDialog(parent),
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  settings(settings),
  // for DEBUG
  outputLog(false)
{
  setupUi(this);

  // server name field
  lineEdit_hostname->insert(settings->getServerName());

  // server type field
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_XP,		STRING_SERVER_TYPE_WINDOWS_XP);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_VISTA,	STRING_SERVER_TYPE_WINDOWS_VISTA);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_7,		STRING_SERVER_TYPE_WINDOWS_7);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_8,		STRING_SERVER_TYPE_WINDOWS_8);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_8_1,	STRING_SERVER_TYPE_WINDOWS_8_1);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_10,		STRING_SERVER_TYPE_WINDOWS_10);

  // server type field
  comboBox_hosttype->setCurrentIndex(settings->getServerType());

  // keyboard type field
  comboBox_keyboardtype->insertItem(KEYBOARD_TYPE_JP, STRING_KEYBOARD_TYPE_JP);
  comboBox_keyboardtype->insertItem(KEYBOARD_TYPE_US, STRING_KEYBOARD_TYPE_US);

  // keyboard type field
  comboBox_keyboardtype->setCurrentIndex(settings->getKeyboardType());

  // port no field
  spinBox_portno->setValue(settings->getPortNo());

  // show password field
  checkBox_showPassword->setCheckState(settings->getOnShowPassword() ? Qt::Checked : Qt::Unchecked);

  // resetting
  resetting();
}

// resize event
void ConnectToServerDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event)
}

  // settings for Tablet
void ConnectToServerDialog::resetting()
{
#if defined(QTB_DEV_TABLET)
  QRect currentScreen = settings->getDesktop()->getCurrentScreen();
  int desktopWidth = currentScreen.width();
  int desktopHeight = currentScreen.height();
  int dialogWidth = desktopWidth/3;
  int dialogHeight = desktopHeight*0.8;
  int fontPointSize = 14;

  // resetting dialog window size and font size
  resize(dialogWidth, dialogHeight);
  layoutWidget->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
  QFont currentFont = font();
  currentFont.setPointSize(fontPointSize);
  setFont(currentFont);
#endif // defined(QTB_DEV_TABLET)
}

//---------------------------------------------------------------------------
// private slot
//---------------------------------------------------------------------------
// server name field
void ConnectToServerDialog::on_lineEdit_hostname_textChanged()
{
  if (outputLog)
	cout << "text Changed : hostname"; // for DEBUG
}

// server type field
void ConnectToServerDialog::on_comboBox_hosttype_currentIndexChanged(int index)
{
  if (outputLog)
	cout << "index Changed : hosttype : index = " << index; // for DEBUG
}

// keyboard type field
void ConnectToServerDialog::on_comboBox_keyboardtype_currentIndexChanged(int index)
{
  if (outputLog)
	cout << "index Changed : keyboardtype : index = " << index; // for DEBUG
}

// port no field
void ConnectToServerDialog::on_spinBox_portno_valueChanged(int i)
{
  if (outputLog)
	cout << "value Changed : portno : value = " << i; // for DEBUG
}

// password field
void ConnectToServerDialog::on_lineEdit_password_textChanged()
{
  if (outputLog)
	cout << "text Changed : password"; // for DEBUG

  // check
  QString password = lineEdit_password->text();
  if (password.size() > ENCRYPTION_KEY_LENGTH){
	// error
	cout << "password is too long : " << password.size();
  }
}

// show password field
void ConnectToServerDialog::on_checkBox_showPassword_stateChanged(int state)
{
  if (outputLog)
	cout << "state Changed : show Password" << endl << flush; // for DEBUG

  if (state == Qt::Checked){
	lineEdit_password->setEchoMode(QLineEdit::Normal);
  }
  else {
	lineEdit_password->setEchoMode(QLineEdit::Password);
  }
}

// accept button
void ConnectToServerDialog::accept()
{
  if (outputLog)
	cout << "accept()."; // for DEBUG

  // server name
  settings->setServerName(lineEdit_hostname->text());

  // host type
  settings->setServerType(comboBox_hosttype->currentIndex());

  // keyboard type
  settings->setKeyboardType(comboBox_keyboardtype->currentIndex());

  // port no
  settings->setPortNo(spinBox_portno->value());

  // password
  settings->setPassword(lineEdit_password->text());

  // show password field
  settings->setOnShowPassword(checkBox_showPassword->checkState() == Qt::Checked);

  hide();
}

// reject button
void ConnectToServerDialog::reject()
{
  if (outputLog)
	cout << "reject()."; // for DEBUG
  hide();
}

} // end of namespace qtbrynhildr
