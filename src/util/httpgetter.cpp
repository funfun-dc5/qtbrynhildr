// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#if 0 // for TEST
#include <QFileInfo>
#endif
#include <QtNetwork>

// Local Header
#include "httpgetter.h"


namespace qtbrynhildr {

// constructor
HttpGetter::HttpGetter(QObject *parent)
  :
  QObject(parent),
  reply(Q_NULLPTR),
#if 0 // for TEST
  file(Q_NULLPTR),
#endif
  httpRequestAborted(false),
  // for DEBUG
  outputLog(false)
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
#if 0 // for TEST
bool HttpGetter::startDownload(const QString &urlSpec, const QString &fileName)
#else
bool HttpGetter::startDownload(const QString &urlSpec)
#endif
{
  if (outputLog){
	cout << "enter startDownload()" << endl << flush;
  }

  // check SSL support
  if (!QSslSocket::supportsSsl())
	return false;

#if 0 // for TEST
  if (urlSpec.isEmpty() || fileName.isEmpty())
	return false;
#else
  if (urlSpec.isEmpty())
	return false;
#endif

  const QUrl newUrl = QUrl::fromUserInput(urlSpec);
  if (!newUrl.isValid()){
	return false;
  }

#if 0 // for TEST
  file = openFileForWrite(fileName);
  if (file == Q_NULLPTR)
	return false;
#else
  byteArray.clear();
#endif

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

#if 1 // for TEST
  byteArray.clear();
#endif

  if (outputLog){
	cout << "leave cancelDownload()" << endl << flush;
  }
}

// check support OpenSSL
bool HttpGetter::supportsSsl()
{
  return QSslSocket::supportsSsl();
}

#if 1 // for TEST
// get download page
QByteArray &HttpGetter::getPage()
{
  return byteArray;
}

// clear download page
void HttpGetter::clear()
{
  byteArray.clear();
}
#endif

#if 0 // for TEST
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
#endif

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

#if 0 // for TEST
  QFileInfo fi;
  if (file) {
	fi.setFile(file->fileName());
	file->close();
	delete file;
	file = Q_NULLPTR;
  }
#endif

  if (httpRequestAborted) {
	reply->deleteLater();
	reply = Q_NULLPTR;
	return;
  }

  if (reply->error()) {
#if 0 // for TEST
	QFile::remove(fi.absoluteFilePath());
#else
	byteArray.clear();
#endif
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

#if 0 // for TEST
  if (file != 0)
	file->write(reply->readAll());
#else
  byteArray.append(reply->readAll());
#endif

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
