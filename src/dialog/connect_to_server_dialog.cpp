// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QDateTime>
#include <QDialog>
#include <QFont>
#include <QRect>

//#include <QDebug>

// Local Header
#include "connect_to_server_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
ConnectToServerDialog::ConnectToServerDialog(Settings *settings,
											 QWidget *parent)
  :QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
  ,settings(settings)
#if QTB_AUTO_COMPLETE
  ,serverNameList(0)
  ,completer(0)
#endif // QTB_AUTO_COMPLETE
#if defined(QTB_DEV_TOUCHPANEL)
  ,serverNameListIndex(0)
#endif // defined(QTB_DEV_TOUCHPANEL)
  // for DEBUG
  ,outputLog(false)
{
  setupUi(this);

#if defined(QTB_DEV_TOUCHPANEL)
  setAttribute(Qt::WA_AcceptTouchEvents, true);
#endif // defined(QTB_DEV_TOUCHPANEL)

  // server name field
  serverNameList = settings->getServerNameList();
#if defined(Q_OS_ANDROID) // for DEBUG
  if (serverNameList->size() == 0){
	serverNameList->append(settings->getServerName());
	serverNameListIndex = 0;
  }
#endif // for DEBUG

#if QTB_AUTO_COMPLETE
  completer = new QCompleter(*serverNameList, this);
#endif // QTB_AUTO_COMPLETE

#if defined(QTB_DEV_TOUCHPANEL) // for TEST
  lineEdit_hostname->insert(settings->getServerName());
  serverNameListIndex = serverNameList->indexOf(settings->getServerName());
#if QTB_AUTO_COMPLETE
  lineEdit_hostname->setCompleter(completer);
#endif // QTB_AUTO_COMPLETE
#else // defined(QTB_DEV_TOUCHPANEL)
#if QTB_AUTO_COMPLETE
  comboBox_hostname->setCompleter(completer);
#endif // QTB_AUTO_COMPLETE
  comboBox_hostname->addItems(*serverNameList);
  comboBox_hostname->setCurrentIndex(serverNameList->indexOf(QRegExp(settings->getServerName())));
  comboBox_hostname->setEditable(true);
#endif // defined(QTB_DEV_TOUCHPANEL)

  // server type field
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_XP,		STRING_SERVER_TYPE_WINDOWS_XP);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_VISTA,	STRING_SERVER_TYPE_WINDOWS_VISTA);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_7,		STRING_SERVER_TYPE_WINDOWS_7);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_8,		STRING_SERVER_TYPE_WINDOWS_8);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_8_1,	STRING_SERVER_TYPE_WINDOWS_8_1);
  comboBox_hosttype->insertItem(SERVER_TYPE_WINDOWS_10,		STRING_SERVER_TYPE_WINDOWS_10);

  // keyboard type field
  comboBox_keyboardtype->insertItem(KEYBOARD_TYPE_JP, STRING_KEYBOARD_TYPE_JP);
  comboBox_keyboardtype->insertItem(KEYBOARD_TYPE_US, STRING_KEYBOARD_TYPE_US);
#if defined(Q_OS_WIN)
  comboBox_keyboardtype->insertItem(KEYBOARD_TYPE_NATIVE, STRING_KEYBOARD_TYPE_NATIVE);
#endif // defined(Q_OS_WIN)

  // public mode field
  comboBox_publicmode->insertItem(PUBLICMODE_VERSION5 - PUBLICMODE_VERSION5, tr("MODE 5"));
  comboBox_publicmode->insertItem(PUBLICMODE_VERSION6 - PUBLICMODE_VERSION5, tr("MODE 6"));
  comboBox_publicmode->insertItem(PUBLICMODE_VERSION7 - PUBLICMODE_VERSION5, tr("MODE 7"));

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

// show Event
void ConnectToServerDialog::showEvent(QShowEvent *event)
{
  getFromSettings();

  QDialog::showEvent(event);
}

#if defined(QTB_DEV_TOUCHPANEL)
// event
bool ConnectToServerDialog::event(QEvent *event)
{
  switch(event->type()){
  case QEvent::TouchBegin:
  case QEvent::TouchEnd:
	{
	  static QDateTime prevPressedTime;
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 tapTime = prevPressedTime.msecsTo(currentTime);
	  if (prevPressedTime.isValid() && tapTime < QTB_TOUCHPANEL_TAP_TIME_THRESHOLD){
		if (serverNameList->size() > 1){
		  if (serverNameListIndex < serverNameList->size()-1){
			serverNameListIndex++;
			//qDebug() << "length : " << serverNameList->at(serverNameListIndex).length();
			if (serverNameList->at(serverNameListIndex).length() == 0)
			  serverNameListIndex = 0;
		  }
		  else {
			serverNameListIndex = 0;
		  }
		  //qDebug() << "srverNameList->size() : " << serverNameList->size();
		  //qDebug() << "serverNameListIndex : " << serverNameListIndex;
		  lineEdit_hostname->clear();
		  lineEdit_hostname->insert(serverNameList->at(serverNameListIndex));
		}
	  }
	  prevPressedTime = currentTime;
	}
	break;
  default:
	break;
  }
  return QDialog::event(event);
}
#endif // defined(QTB_DEV_TOUCHPANEL)

// get from settings
void ConnectToServerDialog::getFromSettings()
{
  // server type field
  comboBox_hosttype->setCurrentIndex(settings->getServerType());

  // keyboard type field
  comboBox_keyboardtype->setCurrentIndex(settings->getKeyboardType());

  // port no field
  spinBox_portno->setValue(settings->getPortNo());

  // password field
  QString password = settings->getPassword();
  if (password != ""){
	lineEdit_password->setText(password);
	checkBox_showPassword->setEnabled(false);
  }
  else {
	lineEdit_password->setText("");
  }

  // public mode field
  comboBox_publicmode->setCurrentIndex(settings->getPublicModeVersion() - PUBLICMODE_VERSION5);

  // show password field
  if (password != ""){
	checkBox_showPassword->setCheckState(Qt::Unchecked);
  }
  else {
	checkBox_showPassword->setCheckState(settings->getOnShowPassword() ? Qt::Checked : Qt::Unchecked);
  }

  // full screen field
  if (QTB_DESKTOP_FULL_SCREEN){
	checkBox_fullScreen->setCheckState(settings->getOnFullScreenAtConnected() ? Qt::Checked : Qt::Unchecked);
  }
  else {
	checkBox_fullScreen->setVisible(false);
  }
#if defined(QTB_DEV_TOUCHPANEL)
  checkBox_fullScreen->setVisible(false);
#endif // defined(QTB_DEV_TOUCHPANEL)
}

// set to settings
bool ConnectToServerDialog::setToSettings()
{
  // server name
#if defined(QTB_DEV_TOUCHPANEL)
  if (lineEdit_hostname->text().size() > 0){
	QString serverName = lineEdit_hostname->text();
	settings->setServerName(serverName);
#if QTB_AUTO_COMPLETE
	if (!(serverNameList->contains(serverName))){
	  serverNameList->insert(0, serverName);
	}
#endif // QTB_AUTO_COMPLETE
  }
  else {
	// Yet: error
	return false;
  }
#else // defined(QTB_DEV_TOUCHPANEL)
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
	return false;
  }
#endif // defined(QTB_DEV_TOUCHPANEL)

  // host type
  settings->setServerType(comboBox_hosttype->currentIndex());

  // keyboard type
  settings->setKeyboardType(comboBox_keyboardtype->currentIndex());

  // port no
  settings->setPortNo(spinBox_portno->value());

  // password
  settings->setPassword(lineEdit_password->text());

  // public mode
  settings->setPublicModeVersion(comboBox_publicmode->currentIndex() + PUBLICMODE_VERSION5);

  // show password field
  settings->setOnShowPassword(checkBox_showPassword->checkState() == Qt::Checked);

  // full screen field
  settings->setOnFullScreenAtConnected(checkBox_fullScreen->checkState() == Qt::Checked);

  return true;
}

// settings for Tablet
void ConnectToServerDialog::resetting()
{
#if defined(QTB_DEV_TOUCHPANEL)
  QRect currentScreen = settings->getDesktop()->getCurrentScreen();
  int screenWidth = currentScreen.width();
  int screenHeight = currentScreen.height();
  int dialogWidth = screenWidth * 0.5;
  int dialogHeight = screenHeight * 0.8;
  int fontPointSize = 14;

  // set minimum width
#if defined(QTB_DEV_TOUCHPANEL)
  lineEdit_hostname->setMinimumWidth(300);
#else // defined(QTB_DEV_TOUCHPANEL)
  comboBox_hostname->setMinimumWidth(300);
#endif // defined(QTB_DEV_TOUCHPANEL)
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
#if defined(QTB_DEV_TOUCHPANEL)
  lineEdit_hostname->setMinimumWidth(180);
#else // defined(QTB_DEV_TOUCHPANEL)
  comboBox_hostname->setMinimumWidth(180);
#endif // defined(QTB_DEV_TOUCHPANEL)
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
#if defined(QTB_DEV_TOUCHPANEL)
void ConnectToServerDialog::on_lineEdit_hostname_textChanged()
{
  if (outputLog)
	cout << "text Changed : hostname : " << endl << flush; // for DEBUG
}
#else // defined(QTB_DEV_TOUCHPANEL)
void ConnectToServerDialog::on_comboBox_hostname_currentIndexChanged(int index)
{
  if (outputLog)
	cout << "index Changed : hostname : index = " << index << endl << flush; // for DEBUG
}
#endif // defined(QTB_DEV_TOUCHPANEL)

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
  if (!checkBox_showPassword->isEnabled() &&  password.size() == 0){
	checkBox_showPassword->setEnabled(true);
  }
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

  // set values to settings
  setToSettings();

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
