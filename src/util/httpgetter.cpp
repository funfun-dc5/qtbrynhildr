// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

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
  :QObject(parent)
  ,reply(nullptr)
  ,file(nullptr)
  ,httpRequestAborted(false)
  // for DEBUG
  ,outputLog(false)
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

// start download to file
bool HttpGetter::startDownload(const QString &urlSpec, const QString &fileName)
{
  if (outputLog){
	std::cout << "enter startDownload()" << std::endl << std::flush;
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
  if (file == nullptr)
	return false;

  startRequest(newUrl);

  if (outputLog){
	std::cout << "leave startDownload()" << std::endl << std::flush;
  }

  return true;
}

// start download to memory
bool HttpGetter::startDownload(const QString &urlSpec)
{
  if (outputLog){
	std::cout << "enter startDownload()" << std::endl << std::flush;
  }

  // check SSL support
  if (!QSslSocket::supportsSsl())
	return false;

  if (urlSpec.isEmpty())
	return false;

  const QUrl newUrl = QUrl::fromUserInput(urlSpec);
  if (!newUrl.isValid()){
	return false;
  }

  byteArray.clear();

  startRequest(newUrl);

  if (outputLog){
	std::cout << "leave startDownload()" << std::endl << std::flush;
  }

  return true;
}

// cancel download
void HttpGetter::cancelDownload()
{
  if (outputLog){
	std::cout << "enter cancelDownload()" << std::endl << std::flush;
  }

  httpRequestAborted = true;
  reply->abort();

  if (file == nullptr)
	byteArray.clear();

  if (outputLog){
	std::cout << "leave cancelDownload()" << std::endl << std::flush;
  }
}

// check support OpenSSL
bool HttpGetter::supportsSsl()
{
  return QSslSocket::supportsSsl();
}

// get download page image
QByteArray &HttpGetter::getPage()
{
  return byteArray;
}

// clear download page
void HttpGetter::clear()
{
  byteArray.clear();
}

// open file for download
#if QT_VERSION < 0x060000
QFile *HttpGetter::openFileForWrite(const QString &fileName)
{
  if (outputLog){
	std::cout << "enter openFileForWrite()" << std::endl << std::flush;
  }

  QScopedPointer<QFile> file(new QFile(fileName));
  if (!file->open(QIODevice::WriteOnly)){
	return nullptr;
  }

  if (outputLog){
	std::cout << "leave openFileForWrite()" << std::endl << std::flush;
  }

  return file.take();
}
#else // QT_VERSION >= 0x060000
QFile *HttpGetter::openFileForWrite(const QString &fileName)
{
  if (outputLog){
	std::cout << "enter openFileForWrite()" << std::endl << std::flush;
  }

  std::unique_ptr<QFile> file(new QFile(fileName));
  if (!file->open(QIODevice::WriteOnly)){
	return nullptr;
  }

  if (outputLog){
	std::cout << "leave openFileForWrite()" << std::endl << std::flush;
  }

  return file.release();
}
#endif // QT_VERSION >= 0x060000
// start request
void HttpGetter::startRequest(const QUrl &url)
{
  if (outputLog){
	std::cout << "enter startRequest()" << std::endl << std::flush;
  }

  this->url = url;
  httpRequestAborted = false;

  reply = networkAccessManager.get(QNetworkRequest(this->url));
  connect(reply, &QNetworkReply::finished, this, &HttpGetter::httpFinished);
  connect(reply, &QIODevice::readyRead, this, &HttpGetter::httpReadyRead);

  if (outputLog){
	std::cout << "leave startRequest()" << std::endl << std::flush;
  }
}

void HttpGetter::httpFinished()
{
  if (outputLog){
	std::cout << "enter httpFinished()" << std::endl << std::flush;
  }

  QFileInfo fi;
  if (file != nullptr) {
	fi.setFile(file->fileName());
	file->close();
	delete file;
	file = nullptr;
  }

  if (httpRequestAborted) {
	reply->deleteLater();
	reply = nullptr;
	return;
  }

  if (reply->error()) {
	if (file != nullptr) {
	  QFile::remove(fi.absoluteFilePath());
	}
	byteArray.clear();
	reply->deleteLater();
	reply = nullptr;
	return;
  }

  // emit finished() signal
  emit finished();

  if (outputLog){
	std::cout << "leave httpFinished()" << std::endl << std::flush;
  }
}

void HttpGetter::httpReadyRead()
{
  if (outputLog){
	std::cout << "enter httpReadyRead()" << std::endl << std::flush;
  }

  if (file != nullptr){
	file->write(reply->readAll());
  }
  else {
	byteArray.append(reply->readAll());
  }

  if (outputLog){
	std::cout << "leave httpReadyRead()" << std::endl << std::flush;
  }
}

void HttpGetter::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenthicator)
{
  Q_UNUSED(reply);
  Q_UNUSED(authenthicator);

  if (outputLog){
	std::cout << "enter slotAuthenticationRequired()!" << std::endl << std::flush;
  }
  if (outputLog){
	std::cout << "leave slotAuthenticationRequired()!" << std::endl << std::flush;
  }
}

// ssl error
void HttpGetter::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
  if (outputLog){
	std::cout << "enter sslErrors()" << std::endl << std::flush;
  }

  QString errorString;
  foreach (const QSslError &error, errors) {
	if (!errorString.isEmpty())
	  errorString += '\n';
	errorString += error.errorString();
  }

  // Yet: error message

  // ignore errors
  reply->ignoreSslErrors();

  if (outputLog){
	std::cout << "leave sslErrors()" << std::endl << std::flush;
  }
}

} // end of namespace qtbrynhildr
