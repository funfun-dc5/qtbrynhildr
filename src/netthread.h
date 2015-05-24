// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// netthread.h

#ifndef NETTHREAD_H
#define NETTHREAD_H
// Common Header
#include "common.h"

// Qt Header
#include <QThread>

// Windows Header
#if defined(Q_OS_WIN)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif // defined(Q_OS_WIN)

// Linux Header
#if defined(Q_OS_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#endif // defined(Q_OS_LINUX)

// Local Header
#include "logmessage.h"
#include "mainwindow.h"
#include "settings.h"

namespace qtbrynhildr {

#if defined(Q_OS_LINUX)
typedef int SOCKET;
typedef struct addrinfo ADDRINFO;
// constant
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SD_BOTH SHUT_RDWR
// function name
#define closesocket ::close
#endif // defined(Q_OS_LINUX)

// result of connectToServer()
enum CONNECT_RESULT {
  CONNECT_SUCCEEDED = 0,
  CONNECT_FAILED,
  CONNECT_WAITED_COUNT,
  CONNECT_FAILED_RETRY
};

// result of processForHeader()
enum PROCESS_RESULT {
  PROCESS_SUCCEEDED = 0,
  PROCESS_NETWORK_ERROR,
  PROCESS_PASSWORD_ERROR,
  PROCESS_CONNECT_ERROR,
  PROCESS_UNKNOWN_ERROR
};

// result of transmitBuffer()
enum TRANSMIT_RESULT {
  TRANSMIT_SUCCEEDED = 0,
  TRANSMIT_NETWORK_ERROR,
  TRANSMIT_DATASIZE_ERROR,
  TRANSMIT_FAILED_PUT_BUFFER,
  TRANSMIT_FAILED_TRANSMIT_DEVICE_BUFFER,
  TRANSMIT_FAILED_IMAGE_DRAW,
};

// NetThread
class NetThread : public QThread
{
  Q_OBJECT

public:
  // constructor
  NetThread(const char *name, Settings *settings, MainWindow *mainWindow);
  // destructor
  ~NetThread();

  // exit thread
  void exitThread();

protected:
  // thread body
  void run();

  // connect to server
  virtual CONNECT_RESULT connectToServer() = 0;

  // process for header
  virtual PROCESS_RESULT processForHeader() = 0;

  // transmit local buffer to global buffer
  virtual TRANSMIT_RESULT transmitBuffer() = 0;

  // shutdown connection
  virtual void shutdownConnection() = 0;

  // connected
  void connectedToServer();

  // socket to server
  SOCKET socketToServer();

  // print protocol header
  void printHeader();

protected:
  // send data
  long sendData(SOCKET sock, const char *buf, long size);

  // receive data
  long receiveData(SOCKET sock, char *buf, long size);

protected:
  // MainWindow
  MainWindow *mainWindow;

  // thread name
  const char *name;

  // settings
  Settings *settings;

  // protocol header
  COM_DATA *com_data;

  // run thread flag
  bool runThread;

  // output log flag
  bool outputLog;

private:
  // set socket option
  void setSocketOption(SOCKET sock);
#if defined(DEBUG)
  // check socket option
  void checkSocketOption(SOCKET sock);
#endif // defined(DEBUG)

  // connect with retry
  int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

signals:
  // output Log Message
  void outputLogMessage(int msgID);

  // output Log Message
  void outputLogMessage(int id, const QString text);

  // network error
  void networkError();
};

} // end of namespace qtbrynhildr

#endif // NETTHREAD_H
