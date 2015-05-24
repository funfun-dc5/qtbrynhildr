// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// connect_to_server_dialog.cpp

// Common Header
#include "common.h"

// System Header
#include <iostream>

// Qt header
#include <QDialog>

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
  QDialog(parent),
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

  // server type field
  comboBox_hosttype->setCurrentIndex(settings->getServerType());

  // keyboard type field
  comboBox_keyboardtype->insertItem(KEYBOARD_TYPE_JP, STRING_KEYBOARD_TYPE_JP);
  comboBox_keyboardtype->insertItem(KEYBOARD_TYPE_US, STRING_KEYBOARD_TYPE_US);

  // keyboard type field
  comboBox_keyboardtype->setCurrentIndex(settings->getKeyboardType());

  // port no field
  spinBox_portno->setValue(settings->getPortNo());
}

//---------------------------------------------------------------------------
// private slot
//---------------------------------------------------------------------------
// server name field
void ConnectToServerDialog::on_lineEdit_hostname_textChanged()
{
  if (outputLog)
	qDebug() << "text Changed : hostname"; // for DEBUG
}

// server type field
void ConnectToServerDialog::on_comboBox_hosttype_currentIndexChanged(int index)
{
  if (outputLog)
	qDebug() << "index Changed : hosttype : index = " << index; // for DEBUG
}

// keyboard type field
void ConnectToServerDialog::on_comboBox_keyboardtype_currentIndexChanged(int index)
{
  if (outputLog)
	qDebug() << "index Changed : keyboardtype : index = " << index; // for DEBUG
}

// port no field
void ConnectToServerDialog::on_spinBox_portno_valueChanged(int i)
{
  if (outputLog)
	qDebug() << "value Changed : portno : value = " << i; // for DEBUG
}

// password field
void ConnectToServerDialog::on_lineEdit_password_textChanged()
{
  if (outputLog)
	qDebug() << "text Changed : password"; // for DEBUG

  // check
  QString password = lineEdit_password->text();
  if (password.size() > ENCRYPTION_KEY_LENGTH){
	// error
	qDebug() << "password is too long : " << password.size();
  }
}

// accept button
void ConnectToServerDialog::accept()
{
  if (outputLog)
	qDebug() << "accept()."; // for DEBUG

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

  hide();
}

// reject button
void ConnectToServerDialog::reject()
{
  if (outputLog)
	qDebug() << "reject()."; // for DEBUG
  hide();
}

} // end of namespace qtbrynhildr
