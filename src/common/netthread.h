// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef NETTHREAD_H
#define NETTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QThread>

// Windows Header
#if defined(QTB_NET_WIN)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif // defined(QTB_NET_WIN)

// UNIX Header
#if defined(QTB_NET_UNIX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#if defined(Q_OS_FREEBSD)
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif // defined(Q_OS_FREEBSD)
#endif // defined(QTB_NET_UNIX)

// Local Header
#include "logmessage.h"
#include "protocols.h"
#include "settings.h"

namespace qtbrynhildr {

// for connect_int()
#define SOCKET_OK		0
#define SOCKET_TIMEOUT	-2
#define TIMEOUT_SOCKET	(SOCKET)-2

#if defined(QTB_NET_UNIX)
typedef int SOCKET;
typedef struct addrinfo ADDRINFO;
// constant
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SD_BOTH SHUT_RDWR
// function name
#define closesocket ::close
#endif // defined(QTB_NET_UNIX)

// result of connectToServer()
enum CONNECT_RESULT {
  CONNECT_SUCCEEDED = 0,
  CONNECT_FAILED,
  CONNECT_FAILED_TIMEOUT,
  CONNECT_WAITED_COUNT,
  CONNECT_FAILED_RETRY
};

// result of processForHeader()
enum PROCESS_RESULT {
  PROCESS_SUCCEEDED = 0,
  PROCESS_RESTART,
  PROCESS_NETWORK_ERROR,
  PROCESS_PASSWORD_ERROR,
  PROCESS_CONNECT_ERROR,
  PROCESS_VIDEO_MODE_ERROR,
  PROCESS_UNKNOWN_ERROR
};

// result of transmitBuffer()
enum TRANSMIT_RESULT {
  TRANSMIT_SUCCEEDED = 0,
  TRANSMIT_RESTART,
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

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
protected:
  // next connect count
  static const int NEXT_CONNECT_COUNT = 5;

protected:
  // thread name
  const char *name;

  // settings
  Settings *settings;

  // protocol header
  COM_DATA *com_data;

  // thread sleep time
  unsigned long threadSleepTime;

  // run thread flag
  bool runThread;

  // received data counter
  long receivedDataCounter;

  // total received data counter
  qint64 totalReceivedDataCounter;

  // previous get data rate time
  qint64 previousGetDataRateTime;

  // start time of main thread loop
  qint64 startTime;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  NetThread(const char *name, Settings *settings);
  // destructor
  virtual ~NetThread();

  // exit thread
  void exitThread();

  // get data rate (bytes/second)
  long getDataRate();

  // get total received data counter
  qint64 getTotalReceivedDataCounter();

protected:
  // thread body
  void run();

  // connect to server
  virtual CONNECT_RESULT connectToServer() = 0;

  // process for header
  virtual PROCESS_RESULT processForHeader() = 0;

  // transmit local buffer to global buffer
  virtual TRANSMIT_RESULT transmitBuffer() = 0;

  // connected
  virtual void connectedToServer();

  // shutdown connection
  virtual void shutdownConnection();

  // socket to server
  SOCKET socketToServer();

  // send eader
  long sendHeader(SOCKET sock, const char *buf, long size);

  // send data
  long sendData(SOCKET sock, const char *buf, long size);

  // receive data
  long receiveData(SOCKET sock, char *buf, long size);

  // print protocol header
  void printHeader(COM_DATA *com_data);

  // save protocol header
  void saveHeader(COM_DATA *com_data, const char* filename);

  // dump protocol header
  void dumpHeader(COM_DATA *com_data);

  // start information
  void startTimeInfo();

  // print information
  void printTimeInfo(const char *str);

private:
  // set socket option
  void setSocketOption(SOCKET sock);
  // check socket option
  void checkSocketOption(SOCKET sock);

  // setup interruptable
  void setupInterruptable(SOCKET sockfd, bool enable);

  // interruptable version send
  long send_int(SOCKET sock, const char *buf, long size, int flags);

  // interruptable version receive
  long recv_int(SOCKET sock, char *buf, long size, int flags);

  // interruptable version connect
  int connect_int(int sockfd, const struct sockaddr *addr, socklen_t addrlen, int timeoutsec);

  // connect with retry
#if !defined(Q_OS_WIN) // Portable Vresion (for MacOSX, FreeBSD...)
  int connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t addrlen);
#else // defined(Q_OS_WIN)
  int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
#endif // defined(Q_OS_WIN)

signals:
  // output Log Message
  void outputLogMessage(int msgID);

  // output Log Message
  void outputLogMessage(int id, const QString text);

  // network error
  void networkError(bool doRetry);
};

} // end of namespace qtbrynhildr

#endif // NETTHREAD_H
