// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QFileInfo>
#include <QtNetwork>

// Local Header
#include "httpgetter.h"


namespace qtbrynhildr {

// constructor
HttpGetter::HttpGetter(QObject *parent)
  :
  QObject(parent),
  reply(Q_NULLPTR),
  file(Q_NULLPTR),
  httpRequestAborted(false),
  // for DEBUG
  outputLog(true)
{
  connect(&networkAccessManager, &QNetworkAccessManager::authenticationRequired,
		  this, &HttpGetter::slotAuthenticationRequired);
  connect(&networkAccessManager, &QNetworkAccessManager::sslErrors,
		  this, &HttpGetter::sslErrors);
}

// destructor
HttpGetter::~HttpGetter()
{
}

// start download
bool HttpGetter::startDownload(const QString &urlSpec, const QString &fileName)
{
  if (outputLog){
	cout << "enter startDownload()" << endl << flush;
  }

  // check SSL support
  if (!QSslSocket::supportsSsl())
	return false;

  if (urlSpec.isEmpty() || fileName.isEmpty())
	return false;

  const QUrl newUrl = QUrl::fromUserInput(urlSpec);
  if (!newUrl.isValid()){
	return false;
  }

  file = openFileForWrite(fileName);
  if (file == Q_NULLPTR)
	return false;

  startRequest(newUrl);

  if (outputLog){
	cout << "leave startDownload()" << endl << flush;
  }

  return true;
}

// cancel download
void HttpGetter::cancelDownload()
{
  if (outputLog){
	cout << "enter cancelDownload()" << endl << flush;
  }

  httpRequestAborted = true;
  reply->abort();

  if (outputLog){
	cout << "leave cancelDownload()" << endl << flush;
  }
}

// check support OpenSSL
bool HttpGetter::supportsSsl()
{
  return QSslSocket::supportsSsl();
}

// open file for download
QFile *HttpGetter::openFileForWrite(const QString &fileName)
{
  if (outputLog){
	cout << "enter openFileForWrite()" << endl << flush;
  }

  QScopedPointer<QFile> file(new QFile(fileName));
  if (!file->open(QIODevice::WriteOnly)){
	return Q_NULLPTR;
  }

  if (outputLog){
	cout << "leave openFileForWrite()" << endl << flush;
  }

  return file.take();
}

// start request
void HttpGetter::startRequest(const QUrl &url)
{
  if (outputLog){
	cout << "enter startRequest()" << endl << flush;
  }

  this->url = url;
  httpRequestAborted = false;

  reply = networkAccessManager.get(QNetworkRequest(this->url));
  connect(reply, &QNetworkReply::finished, this, &HttpGetter::httpFinished);
  connect(reply, &QIODevice::readyRead, this, &HttpGetter::httpReadyRead);

  if (outputLog){
	cout << "leave startRequest()" << endl << flush;
  }
}

void HttpGetter::httpFinished()
{
  if (outputLog){
	cout << "enter httpFinished()" << endl << flush;
  }

  QFileInfo fi;
  if (file) {
	fi.setFile(file->fileName());
	file->close();
	delete file;
	file = Q_NULLPTR;
  }

  if (httpRequestAborted) {
	reply->deleteLater();
	reply = Q_NULLPTR;
	return;
  }

  if (reply->error()) {
	QFile::remove(fi.absoluteFilePath());
	reply->deleteLater();
	reply = Q_NULLPTR;
	return;
  }

  //
  emit finished();

  if (outputLog){
	cout << "leave httpFinished()" << endl << flush;
  }
}

void HttpGetter::httpReadyRead()
{
  if (outputLog){
	cout << "enter httpReadyRead()" << endl << flush;
  }

  if (file != 0)
	file->write(reply->readAll());

  if (outputLog){
	cout << "leave httpReadyRead()" << endl << flush;
  }
}

void HttpGetter::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenthicator)
{
  if (outputLog){
	cout << "enter slotAuthenticationRequired()!" << endl << flush;
  }
  if (outputLog){
	cout << "leave slotAuthenticationRequired()!" << endl << flush;
  }
}

// ssl error
void HttpGetter::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
  if (outputLog){
	cout << "enter sslErrors()" << endl << flush;
  }

  QString errorString;
  foreach (const QSslError &error, errors) {
	if (!errorString.isEmpty())
	  errorString += '\n';
	errorString += error.errorString();
  }
#if 0 // NOT Widgets
  if (QMessageBox::warning(this, tr("SSL Errors"),
						   tr("One or more SSL errors has occurred:\n%1").arg(errorString),
						   QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
	reply->ignoreSslErrors();
  }
#endif
  if (outputLog){
	cout << "leave sslErrors()" << endl << flush;
  }
}

} // end of namespace qtbrynhildr
