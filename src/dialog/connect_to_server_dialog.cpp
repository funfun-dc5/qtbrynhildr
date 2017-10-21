// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
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
#if QTB_AUTO_COMPLETE
  serverNameList(0),
  completer(0),
#endif // QTB_AUTO_COMPLETE
  // for DEBUG
  outputLog(false)
{
  setupUi(this);

  // server name field
  serverNameList = settings->getServerNameList();
#if defined(Q_OS_ANDROID) // for DEBUG
  if (serverNameList->size() == 0){
	serverNameList->append(settings->getServerName());
  }
#endif // for DEBUG
#if QTB_AUTO_COMPLETE
  completer = new QCompleter(*serverNameList, this);
  comboBox_hostname->setCompleter(completer);
  comboBox_hostname->addItems(*serverNameList);
  comboBox_hostname->setCurrentIndex(serverNameList->indexOf(QRegExp(settings->getServerName())));
  comboBox_hostname->setEditable(true);
#endif // QTB_AUTO_COMPLETE

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
#if defined(Q_OS_WIN)
  comboBox_keyboardtype->insertItem(KEYBOARD_TYPE_NATIVE, STRING_KEYBOARD_TYPE_NATIVE);
#endif // defined(Q_OS_WIN)

  // keyboard type field
  comboBox_keyboardtype->setCurrentIndex(settings->getKeyboardType());

  // port no field
  spinBox_portno->setValue(settings->getPortNo());

  // public mode field
#if QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicmode->insertItem(PUBLICMODE_VERSION5 - PUBLICMODE_VERSION5, tr("MODE 5"));
  comboBox_publicmode->insertItem(PUBLICMODE_VERSION6 - PUBLICMODE_VERSION5, tr("MODE 6"));
#if QTB_PUBLIC_MODE7_SUPPORT
  comboBox_publicmode->insertItem(PUBLICMODE_VERSION7 - PUBLICMODE_VERSION5, tr("MODE 7"));
#endif // QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicmode->setCurrentIndex(settings->getPublicModeVersion() - PUBLICMODE_VERSION5);
#else // QTB_PUBLIC_MODE6_SUPPORT
  comboBox_publicmode->insertItem(0, tr("MODE 5"));
  comboBox_publicmode->setCurrentIndex(0);
  comboBox_publicmode->setEnabled(false);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // show password field
  checkBox_showPassword->setCheckState(settings->getOnShowPassword() ? Qt::Checked : Qt::Unchecked);

  // full screen field
  if (QTB_DESKTOP_FULL_SCREEN){
	checkBox_fullScreen->setCheckState(settings->getOnFullScreenAtConnected() ? Qt::Checked : Qt::Unchecked);
  }
  else {
	checkBox_fullScreen->setVisible(false);
  }

  // resetting
  resetting();

  // fix size
  setFixedSize(size());
}

// destructor
ConnectToServerDialog::~ConnectToServerDialog()
{
  // delete objects
#if QTB_AUTO_COMPLETE
  if (completer != 0){
	delete completer;
	completer = 0;
  }
#endif // QTB_AUTO_COMPLETE
}

// add keyboard type list by key layout file
void ConnectToServerDialog::addKeyboardTypeList(QStringList keyboardTypeList)
{
  comboBox_keyboardtype->addItems(keyboardTypeList);
}

// set keyboard type
void ConnectToServerDialog::setKeyboardType(KEYBOARD_TYPE keyboardType)
{
  comboBox_keyboardtype->setCurrentIndex(keyboardType);
}

// resize event
void ConnectToServerDialog::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
}

// settings for Tablet
void ConnectToServerDialog::resetting()
{
#if defined(QTB_DEV_TOUCHPANEL)
  QRect currentScreen = settings->getDesktop()->getCurrentScreen();
  int desktopWidth = currentScreen.width();
  int desktopHeight = currentScreen.height();
  int dialogWidth = desktopWidth * 0.5;
  int dialogHeight = desktopHeight * 0.8;
  int fontPointSize = 14;

  // set minimum width
  comboBox_hostname->setMinimumWidth(300);
  comboBox_hosttype->setMinimumWidth(300);
  comboBox_keyboardtype->setMinimumWidth(300);
  spinBox_portno->setMinimumWidth(300);
  lineEdit_password->setMinimumWidth(300);
  comboBox_publicmode->setMinimumWidth(300);

  // resetting dialog window size and font size
  resize(dialogWidth, dialogHeight);
  layoutWidget->setGeometry(QRect(20, 20, dialogWidth-40, dialogHeight-40));
  QFont currentFont = font();
  currentFont.setPointSize(fontPointSize);
  setFont(currentFont);
#else // defined(QTB_DEV_TOUCHPANEL)
  // set minimum width
  comboBox_hostname->setMinimumWidth(180);
  comboBox_hosttype->setMinimumWidth(180);
  comboBox_keyboardtype->setMinimumWidth(180);
  spinBox_portno->setMinimumWidth(180);
  lineEdit_password->setMinimumWidth(180);
  comboBox_publicmode->setMinimumWidth(180);
#endif // defined(QTB_DEV_TOUCHPANEL)
}

//---------------------------------------------------------------------------
// private slot
//---------------------------------------------------------------------------
// server name field
void ConnectToServerDialog::on_comboBox_hostname_currentIndexChanged(int index)
{
  if (outputLog)
	cout << "index Changed : hostname : index = " << index << endl << flush; // for DEBUG
}

// server type field
void ConnectToServerDialog::on_comboBox_hosttype_currentIndexChanged(int index)
{
  if (outputLog)
	cout << "index Changed : hosttype : index = " << index << endl << flush; // for DEBUG
}

// keyboard type field
void ConnectToServerDialog::on_comboBox_keyboardtype_currentIndexChanged(int index)
{
  if (outputLog)
	cout << "index Changed : keyboardtype : index = " << index << endl << flush; // for DEBUG
}

// port no field
void ConnectToServerDialog::on_spinBox_portno_valueChanged(int i)
{
  if (outputLog)
	cout << "value Changed : portno : value = " << i << endl << flush; // for DEBUG
}

// password field
void ConnectToServerDialog::on_lineEdit_password_textChanged()
{
  if (outputLog)
	cout << "text Changed : password" << endl << flush; // for DEBUG

  // check
  QString password = lineEdit_password->text();
  if (password.size() > ENCRYPTION_KEY_LENGTH){
	// error
	cout << "password is too long : " << password.size() << endl << flush;
  }
}

// public mode field
void ConnectToServerDialog::on_comboBox_publicmode_currentIndexChanged(int index)
{
  if (outputLog)
	cout << "index Changed : publicmode : index = " << index << endl << flush; // for DEBUG
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

// full screen field
void ConnectToServerDialog::on_checkBox_fullScreen_stateChanged(int state)
{
  if (outputLog){
	cout << "state Changed : full Screen" << endl << flush; // for DEBUG
	if (state == Qt::Checked){
	  cout << "full Screen: checked" << endl << flush; // for DEBUG
	}
	else {
	  cout << "full Screen: unchecked" << endl << flush; // for DEBUG
	}
  }
}

// accept button
void ConnectToServerDialog::accept()
{
  if (outputLog)
	cout << "accept()." << endl << flush; // for DEBUG

  // server name
  if (comboBox_hostname->currentText().size() > 0){
	QString serverName = comboBox_hostname->currentText();
	settings->setServerName(serverName);
#if QTB_AUTO_COMPLETE
	if (!(serverNameList->contains(serverName))){
	  serverNameList->insert(0, serverName);
	}
#endif // QTB_AUTO_COMPLETE
  }
  else {
	// Yet: error
	return;
  }

  // host type
  settings->setServerType(comboBox_hosttype->currentIndex());

  // keyboard type
  settings->setKeyboardType(comboBox_keyboardtype->currentIndex());

  // port no
  settings->setPortNo(spinBox_portno->value());

  // password
  settings->setPassword(lineEdit_password->text());

#if QTB_PUBLIC_MODE6_SUPPORT
  // public mode
  settings->setPublicModeVersion(comboBox_publicmode->currentIndex() + PUBLICMODE_VERSION5);
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // show password field
  settings->setOnShowPassword(checkBox_showPassword->checkState() == Qt::Checked);

  // full screen field
  settings->setOnFullScreenAtConnected(checkBox_fullScreen->checkState() == Qt::Checked);

  // connect to server
  emit connectToServer();

  hide();
}

// reject button
void ConnectToServerDialog::reject()
{
  if (outputLog)
	cout << "reject()." << endl << flush; // for DEBUG
  hide();
}

} // end of namespace qtbrynhildr
