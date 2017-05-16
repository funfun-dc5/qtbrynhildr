// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef HTTPGETTER_H
#define HTTPGETTER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QByteArray>
#include <QFile>
#include <QList>
#include <QNetworkAccessManager>
#include <QString>
#include <QUrl>

// Local Header


namespace qtbrynhildr {

// HttpGetter
class HttpGetter : public QObject
{
  Q_OBJECT

public:
  // constructor
  HttpGetter(QObject *parent = 0);
  // destructor
  ~HttpGetter();

  // start download
#if 0 // for TEST
  bool startDownload(const QString &url, const QString &fileName);
#else
  bool startDownload(const QString &url);
#endif

  // cancel download
  void cancelDownload();

#if 1 // for TEST
  // get download page
  QByteArray &getPage();

  // clear download page
  void clear();
#endif

  // check support OpenSSL
  bool supportsSsl();

signals:
  //
  void finished();

private:
#if 0 // for TEST
  // open file for download
  QFile *openFileForWrite(const QString &fileName);
#endif

  // start request
  void startRequest(const QUrl &url);

private slots:
  void httpFinished();
  void httpReadyRead();
  void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenthicator);
  void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
  QUrl url;
  QNetworkAccessManager networkAccessManager;
  QNetworkReply *reply;
#if 0 // for TEST
  QFile *file;
#else
  QByteArray byteArray;
#endif
  bool httpRequestAborted;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // HTTPGETTER_H
