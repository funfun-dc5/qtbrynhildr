// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

// Qt Header
#include <QDateTime>

// Local Header
#include "netthread.h"
#include "parameters.h"


namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
NetThread::NetThread(const char *name, Settings *settings)
  :name(name)
  ,settings(settings)
  ,com_data(new COM_DATA)
  ,threadSleepTime(QTB_THREAD_SLEEP_TIME)
  ,runThread(true)
  ,receivedDataCounter(0)
  ,totalReceivedDataCounter(0)
  ,previousGetDataRateTime(0)
  ,startTime(0)
  // for DEBUG
  ,outputLog(false)
{
}

// destructor
NetThread::~NetThread()
{
  // delete objects
  // com_data
  if (com_data != 0){
	delete com_data;
	com_data = 0;
  }
}

// exit thread
void NetThread::exitThread()
{
  //  std::cout << "[" << name << "]" << " exitThread()" << std::endl << std::flush;
  runThread = false;
}

// get data rate
long NetThread::getDataRate()
{
  qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
  long bps = 0;

  if (previousGetDataRateTime != 0){
	qint64 diffMSeconds = currentTime - previousGetDataRateTime;
	if (diffMSeconds != 0){
	  bps = (long)(receivedDataCounter / ((double)diffMSeconds)*1000);
	}
  }
  previousGetDataRateTime = currentTime;
  totalReceivedDataCounter += receivedDataCounter;
  receivedDataCounter = 0;
  return bps;
}

// get total received data counter
qint64 NetThread::getTotalReceivedDataCounter()
{
  return totalReceivedDataCounter;
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// thread body
void NetThread::run()
{
  if (settings->getOutputLog())
	std::cout << "[" << name << "]" << " start thread...start run()" << std::endl << std::flush;

  // start thread
  runThread = true;

  // main loop
  while(runThread){
	// set start time of main loop
	startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

	//QThread::msleep(5); // 5 (ms) sleep
	QThread::msleep(threadSleepTime); // threadSleepTime (ms) sleep

	CONNECT_RESULT result_connect = connectToServer();
	if (result_connect != CONNECT_SUCCEEDED){
	  if (result_connect == CONNECT_WAITED_COUNT){
		continue;
	  }
	  if (result_connect == CONNECT_FAILED){
		//std::cout << "[" << name << "]" << " connect Error: connectToServer()" << std::endl << std::flush; // error
		shutdownConnection();
		emit networkError(true);
		//emit outputLogMessage(QTB_MSG_CONNECT_ERROR);
		continue;
	  }
	  if (result_connect == CONNECT_FAILED_RETRY){
		if (settings->getOutputLog())
		  std::cout << "[" << name << "]" << " connect Error: connectToServer()" << std::endl << std::flush; // error
		runThread = false;
		break;
	  }
	  if (result_connect == CONNECT_FAILED_TIMEOUT){
		if (settings->getOutputLog())
		  std::cout << "[" << name << "]" << " connect Timeout: connectToServer()" << std::endl << std::flush; // error
		shutdownConnection();
		emit networkError(false);
		emit outputLogMessage(QTB_MSG_CONNECT_TIMEOUT);
		runThread = false;
		break;
	  }
	}

	// process header (send and receive)
	PROCESS_RESULT result_process = processForHeader();
	if (result_process != PROCESS_SUCCEEDED){
	  if (result_process == PROCESS_RESTART){
		// restart
		shutdownConnection();
		QThread::sleep(2);
		emit networkError(true);
		continue;
	  }

	  if (result_process == PROCESS_NETWORK_ERROR){
		// error
		if (settings->getOutputLog())
		  std::cout << "[" << name << "]" << " Network Error: processForHeader()" << std::endl << std::flush; // error
		shutdownConnection();
		emit networkError(true);
		continue;
	  }
	  if (result_process == PROCESS_PASSWORD_ERROR){
		// error
		if (settings->getOutputLog())
		  std::cout << "[" << name << "]" << " Password Error: processForHeader()" << std::endl << std::flush; // error
		shutdownConnection();
		runThread = false;
		emit networkError(false);
		emit outputLogMessage(QTB_MSG_PASSWORD_ERROR);
		break;
	  }
	  if (result_process == PROCESS_CONNECT_ERROR){
		// error
		if (settings->getOutputLog())
		  std::cout << "[" << name << "]" << " Connect Error: processForHeader()" << std::endl << std::flush; // error
		shutdownConnection();
		runThread = false;
		emit networkError(false);
		emit outputLogMessage(QTB_MSG_ALREADY_CONNECT_ANOTHER_CLIENT);
		break;
	  }
	  if (result_process == PROCESS_VIDEO_MODE_ERROR){
		// error
		if (settings->getOutputLog())
		  std::cout << "[" << name << "]" << " Connect Error: processForHeader()" << std::endl << std::flush; // error
		shutdownConnection();
		runThread = false;
		emit networkError(false);
		emit outputLogMessage(QTB_MSG_NOTSUPPORT_VIDEO_MODE);
		break;
	  }
	  if (result_process == PROCESS_UNKNOWN_ERROR){
		// error
		if (settings->getOutputLog())
		  std::cout << "[" << name << "]" << " Unknown Error: processForHeader()" << std::endl << std::flush; // error
		shutdownConnection();
		runThread = false;
		emit networkError(false);
		emit outputLogMessage(QTB_MSG_UNKNOWN_ERROR);
		break;
	  }
	}

	// transmit local buffer to device buffer
	TRANSMIT_RESULT result_transmit = transmitBuffer();
	switch(result_transmit){
	case TRANSMIT_SUCCEEDED:
	  // Nothing to do
	  break;
	case TRANSMIT_RESTART:
	  // restart
	  shutdownConnection();
	  QThread::sleep(2);
	  emit networkError(true);
	  continue;
	  break;
	case TRANSMIT_NETWORK_ERROR:
	  if (settings->getOutputLog())
		std::cout << "[" << name << "]" << " Failed: transmitBuffer(): network error" << std::endl << std::flush; // error
	  shutdownConnection();
	  emit networkError(true);
	  continue;
	  break;
	case TRANSMIT_DATASIZE_ERROR:
	  if (settings->getOutputLog())
		std::cout << "[" << name << "]" << " Failed: transmitBuffer(): data size" << std::endl << std::flush; // error
	  break;
	case TRANSMIT_FAILED_PUT_BUFFER:
	  if (settings->getOutputLog())
		std::cout << "[" << name << "]" << " Failed: transmitBuffer(): put buffer." << std::endl << std::flush; // error
	  break;
	case TRANSMIT_FAILED_TRANSMIT_DEVICE_BUFFER:
	  if (settings->getOutputLog())
		std::cout << "[" << name << "]" << " Failed: transmitBuffer(): transmit device buffer." << std::endl << std::flush; // error
	  break;
	case TRANSMIT_FAILED_IMAGE_DRAW:
	  if (settings->getOutputLog())
		std::cout << "[" << name << "]" << " Failed: transmitBuffer(): image draw." << std::endl << std::flush; // error
	  break;
	default:
	  // error
	  ABORT();
	  break;
	}
  }

  // disconnect
  shutdownConnection();

  if (settings->getOutputLog())
	std::cout << "[" << name << "]" << " stop thread...exit run()" << std::endl << std::flush;
}

// connected
void NetThread::connectedToServer()
{
  // reset counter
  receivedDataCounter = 0;
  totalReceivedDataCounter = 0;

  // reset previous get data rate time
  previousGetDataRateTime = 0;
}

// shutdown connection
#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
void NetThread::shutdownConnection()
{
  // reset previous get data rate time
  previousGetDataRateTime = 0;
}
#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
#if !QTB_NET_WINSOCK1
// socket to server
SOCKET NetThread::socketToServer()
{
  SOCKET sock = INVALID_SOCKET;
  ADDRINFO *addrinfo;
  ADDRINFO addrinfo_hints;

  memset(&addrinfo_hints, 0, sizeof(ADDRINFO));
#if 0 // for TEST
  addrinfo_hints.ai_family = AF_INET; // for IP v4
#else // for TEST
  addrinfo_hints.ai_family = AF_UNSPEC;
#endif // for TEST
  addrinfo_hints.ai_socktype = SOCK_STREAM;

  char server[512];
  char port[16];
  int error;
  int result;
  result = snprintf(server, 256, "%s", qPrintable(settings->getServerName()));
  if (result <= 0 || result > 255){
	if (settings->getOutputLog()){
	  const QString text = QString("socketToServer() : snprintf() error! for server");
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
	return INVALID_SOCKET;
  }
  result = snprintf(port, 10, "%d", settings->getPortNo());
  if (result <= 0 || result > 9){
	if (settings->getOutputLog()){
	  const QString text = QString("socketToServer() : snprintf() error! for port");
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
	return INVALID_SOCKET;
  }
  error = getaddrinfo(server, port, &addrinfo_hints, &addrinfo);
  if (error != 0){
#if defined(QTB_NET_WIN)
	if (settings->getOutputLog()){
	  const QString text = QString("socketToServer() : getaddrinfo(): error = ") + QString::number(error);
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
#elif defined(QTB_NET_UNIX)
	if (settings->getOutputLog()){
	  const QString text = QString("socketToServer() : getaddrinfo(): strerror = ") + gai_strerror(error);
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
#endif // defined(QTB_NET_UNIX)
	return INVALID_SOCKET;
  }

  ADDRINFO *topAddrinfo = addrinfo;
  for (; addrinfo != NULL; addrinfo = addrinfo->ai_next){
#if !defined(Q_OS_WIN) // Portable Vresion (for MacOSX, FreeBSD...)
	sock = connect_retry(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol,
						 addrinfo->ai_addr, (int)addrinfo->ai_addrlen);
	if (sock == INVALID_SOCKET){
	  continue;
	}
#else
	sock = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
	if (sock == INVALID_SOCKET){
	  continue;
	}
	int result = connect_retry(sock, addrinfo->ai_addr, (int)addrinfo->ai_addrlen);
	if (result == SOCKET_ERROR){
	  closesocket(sock);
	  sock = INVALID_SOCKET;
	  continue;
	}
#endif
	if (result == SOCKET_TIMEOUT || sock == TIMEOUT_SOCKET){
	  //std::cout << "TimeOut!" << std::endl << std::flush;
	  closesocket(sock);
	  sock = TIMEOUT_SOCKET;
	  break;
	}
	break;
  }

  if (sock == INVALID_SOCKET){
	// INVALID_SOCKET
	if (settings->getOutputLog()){
	  const QString text = "socketToServer() : sock = INVALID_SOCKET";
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
  }
  else if (sock == TIMEOUT_SOCKET){
	// TIMEOUT_SOCKET
	if (settings->getOutputLog()){
	  const QString text = "socketToServer() : sock = TIMEOUT_SOCKET";
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
  }
  else {
	// set socket option
	setSocketOption(sock);
	// check socket option
	if (outputLog)
	  checkSocketOption(sock);
  }

  // free all addrinfo
  while(topAddrinfo != NULL){
	addrinfo = topAddrinfo;
	topAddrinfo = addrinfo->ai_next;
	freeaddrinfo(addrinfo);
  }

  return sock;
}
#else // !QTB_NET_WINSOCK1
// socket to server
// for WinSock 1
SOCKET NetThread::socketToServer()
{
  SOCKET sock = INVALID_SOCKET;
  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;

  char server[512];
  HOSTENT *host;
  unsigned int address;
  int result;
  // server name
  result = snprintf(server, 256, "%s", qPrintable(settings->getServerName()));
  if (result <= 0 || result > 255){
	if (settings->getOutputLog()){
	  const QString text = QString("socketToServer() : snprintf() error! for server");
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
	return INVALID_SOCKET;
  }
  host = gethostbyname(server);
  if (host == 0){
	address = inet_addr(server);
	host = gethostbyaddr((char*)&address, 4, AF_INET);
  }
  if (host == 0){
	return INVALID_SOCKET;
  }

  addr.sin_port = htons(settings->getPortNo());
  addr.sin_addr.s_addr = *((u_long*)host->h_addr);

  sock = socket(AF_INET, SOCK_STREAM, 0);

  result = connect_int(sock, (struct sockaddr *)&addr, sizeof(addr), -1);
  if (result == SOCKET_ERROR){
	sock = INVALID_SOCKET;
  }

  // for socket option
  if (sock != INVALID_SOCKET){
	// set socket option
	setSocketOption(sock);
	// check socket option
	if (outputLog)
	  checkSocketOption(sock);
  }
  else {
	// INVALID_SOCKET
	if (settings->getOutputLog()){
	  const QString text = "socketToServer() : sock = INVALID_SOCKET";
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
  }

  return sock;
}
#endif // !QTB_NET_WINSOCK1

// send header
long NetThread::sendHeader(SOCKET sock, const char *buf, long size)
{
  //                                      0123456789ABCDEF
  char key[ENCRYPTION_KEY_LENGTH + 1] = {"@@@@@@@@@@@@@@@@"};

  // copy encryption key
  if (!settings->getPassword().isEmpty()){
	char *encryption_key = strdup(qPrintable(settings->getPassword()));
	for (int i = 0;i < ENCRYPTION_KEY_LENGTH; i++){
	  if (encryption_key[i] == '\0')
		break;

	  key[i] = encryption_key[i];
	}
	if (encryption_key != 0){
	  free(encryption_key);
	}
  }

  COM_DATA *com_data = (COM_DATA*)buf;
  long data_long = com_data->data_type + (com_data->data_size & 0x0000ffff);

  com_data->encryption = ENCRYPTION_OFF;
  com_data->check_digit = ~data_long;

  for (int i = 0; i < ENCRYPTION_KEY_LENGTH; i++){
	char key_char = ~key[i];
	key_char += (char)((long)i * ~com_data->check_digit);
	com_data->check_digit_enc[i] = key_char;
  }

  //saveHeader("sentHeader.dat");

  // send
  return send_int(sock, buf, size, 0);
}

// send data
long NetThread::sendData(SOCKET sock, const char *buf, long size)
{
  long sent_size = 0;

  while(sent_size < size){
	long ret = send_int(sock, buf + sent_size, size - sent_size, 0);
#if 0 // for TEST
	if (ret < 0)
	  std::cout << "errno = " << errno << std::endl << std::flush;
#endif // for TEST
	if (ret > 0){
	  sent_size += ret;
	}
	else {
	  return -1;
	}
  }
  return sent_size;
}

// receive data
long NetThread::receiveData(SOCKET sock, char *buf, long size)
{
  long received_size = 0;

  while(received_size < size){
	long ret = recv_int(sock, buf + received_size, size - received_size, 0);
	if (ret > 0){
	  received_size += ret;
	}
	else {
	  return -1;
	}
  }

  receivedDataCounter += received_size;

  return received_size;
}

// print protocol header
void NetThread::printHeader(COM_DATA *com_data)
{
  std::cout << "============================== HEADER ==============================" << std::endl;
  std::cout << "com_data->data_type      :" << (int)com_data->data_type << std::endl;
  std::cout << "com_data->thread         :" << (int)com_data->thread << std::endl;
  std::cout << "com_data->sound_type     :" << (int)com_data->sound_type << std::endl;
  std::cout << "com_data->encryption     :" << (int)com_data->encryption << std::endl;
  std::cout << "com_data->data_size      :" << com_data->data_size << std::endl;

  std::ios::fmtflags flags = std::cout.flags();

  std::cout << "com_data->check_digit_enc:" << std::hex << std::uppercase << std::setfill('0');
  for (int i = 0; i < ENCRYPTION_KEY_LENGTH; i++){
	std::cout << std::setw(2) << (int)(com_data->check_digit_enc[i] & 0xff);
  }
  std::cout << std::endl;
  std::cout << "com_data->check_digit    :"	<< (int)(com_data->check_digit & 0xffff) << std::endl;

  std::cout.setf(flags);

  std::cout << "com_data->ver            :\"" << com_data->ver[0]
										 << com_data->ver[1]
										 << com_data->ver[2]
										 << com_data->ver[3] << "\"" << std::endl;
  std::cout << "com_data->samplerate     :"  << com_data->samplerate << std::endl;
  std::cout << "com_data->image_cx       :"  << com_data->image_cx << std::endl;
  std::cout << "com_data->image_cy       :"  << com_data->image_cy << std::endl;
  std::cout << "com_data->server_cx      :"  << com_data->server_cx << std::endl;
  std::cout << "com_data->server_cy      :"  << com_data->server_cy << std::endl;
  std::cout << "com_data->control        :"	<< (int)com_data->control << std::endl;
#if QTB_PLUGINS_DISABLE_SUPPORT
  std::cout << "com_data->plugins_disable:"	<< (int)com_data->plugins_disable << std::endl;
#endif // QTB_PLUGINS_DISABLE_SUPPORT
  std::cout << "com_data->filedrop       :"	<< (int)com_data->filedrop << std::endl;
  std::cout << "com_data->mouse_move     :"	<< (int)com_data->mouse_move << std::endl;
#if QTB_EXTRA_BUTTON_SUPPORT
  std::cout << "com_data->mouse_x1       :"  << (int)com_data->mouse_x1 << std::endl;
  std::cout << "com_data->mouse_x2       :"  << (int)com_data->mouse_x2 << std::endl;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  std::cout << "com_data->mouse_x        :"  << com_data->mouse_x << std::endl;
  std::cout << "com_data->mouse_y        :"  << com_data->mouse_y << std::endl;
  std::cout << "com_data->mouse_left     :"  << (int)com_data->mouse_left << std::endl;
  std::cout << "com_data->mouse_right    :"  << (int)com_data->mouse_right << std::endl;
#if QTB_EXTRA_BUTTON_SUPPORT
  std::cout << "com_data->mouse_middle   :"  << (int)com_data->mouse_middle << std::endl;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  std::cout << "com_data->mouse_wheel    :"  << (int)com_data->mouse_wheel << std::endl;
  std::cout << "com_data->keycode        :"  << (int)com_data->keycode << std::endl;
  std::cout << "com_data->keycode_flg    :"  << (int)com_data->keycode_flg << std::endl;
  std::cout << "com_data->cursor_hotspot_x :"  << (int)com_data->cursor_hotspot_x << std::endl;
  std::cout << "com_data->cursor_hotspot_y :"  << (int)com_data->cursor_hotspot_y << std::endl;
  std::cout << "com_data->monitor_no     :"	<< (int)com_data->monitor_no << std::endl;
  std::cout << "com_data->monitor_count  :"	<< (int)com_data->monitor_count << std::endl;
  std::cout << "com_data->max_fps        :"	<< (int)com_data->max_fps << std::endl;
  std::cout << "com_data->sound_capture  :"	<< (int)com_data->sound_capture << std::endl;
  std::cout << "com_data->keydown        :"	<< com_data->keydown << std::endl;
  std::cout << "com_data->video_quality  :"	<< (int)com_data->video_quality << std::endl;
  std::cout << "com_data->mouse_cursor   :"	<< (int)com_data->mouse_cursor << std::endl;
  std::cout << "com_data->gamepad1       :"	<< com_data->gamepad1 << std::endl;
  std::cout << "com_data->gamepad2       :"	<< com_data->gamepad2 << std::endl;
  std::cout << "com_data->gamepad3       :"	<< com_data->gamepad3 << std::endl;
  std::cout << "com_data->gamepad4       :"	<< com_data->gamepad4 << std::endl;
  std::cout << "com_data->client_scroll_x:"  << com_data->client_scroll_x << std::endl;
  std::cout << "com_data->client_scroll_y:"  << com_data->client_scroll_y << std::endl;
  std::cout << "com_data->video_mode     :"  << (int)com_data->video_mode << std::endl;
  std::cout << "com_data->server_version :"  << com_data->server_version << std::endl;
  std::cout << "com_data->zoom           :"  << com_data->zoom << std::endl;
  std::cout << "com_data->mode           :"	<< (int)com_data->mode << std::endl;
  std::cout << "com_data->sound_quality  :"	<< (int)com_data->sound_quality << std::endl;
  std::cout << "com_data->frame_no       :"	<< (int)com_data->frame_no << std::endl;
  std::cout << "com_data->gamepad5       :"	<< com_data->gamepad5 << std::endl;
  std::cout << "com_data->gamepad6       :"	<< com_data->gamepad6 << std::endl;
  std::cout << "com_data->gamepad7       :"	<< com_data->gamepad7 << std::endl;
  std::cout << "com_data->gamepad8       :"	<< com_data->gamepad8 << std::endl;

  std::cout << std::endl << std::flush;
}

// save protocol header
void NetThread::saveHeader(COM_DATA *com_data, const char* filename)
{
  std::fstream file;
  file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
  if (file.is_open()){
	file.write((char *)com_data, sizeof(COM_DATA));
	file.close();
  }
  else {
	// Yet: open error
  }
}

// dump protocol header
void NetThread::dumpHeader(COM_DATA *com_data)
{
  // check
  if (com_data == 0)
	return;

  std::cout << "========================== HEADER DUMP ==========================" << std::endl;

  std::ios::fmtflags flags = std::cout.flags();

  std::cout << std::hex << std::uppercase << std::setfill('0');

  unsigned char *ptr = (unsigned char *)com_data;
  for (unsigned int i = 0; i < sizeof(COM_DATA); i++){
	if (i % 16 == 0){
	  std::cout << std::endl;
	  std::cout << std::setw(4) << i << ": ";
	}
	else {
	  std::cout << " ";
	}
	std::cout << std::setw(2) << (unsigned int)*ptr;
	ptr++;
  }
  std::cout << std::endl << std::endl << std::flush;

  std::cout.setf(flags);
}

// start information
void NetThread::startTimeInfo()
{
  static qint64 previousTime = 0;
  qint64 duration = 0;
  if (previousTime != 0){
	duration = startTime - previousTime;
  }
  previousTime = startTime;
  std::cout << "================================   " << duration << std::endl;
}

// print time information
void NetThread::printTimeInfo(const char *str)
{
  qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
  qint64 pastTime = currentTime - startTime;

  std::ios::fmtflags flags = std::cout.flags();

  std::cout << "[" << name << "] " << std::left << std::setw(20) << str << ": " << pastTime << std::endl;

  std::cout.setf(flags);
}

// set socket option
#include <cerrno>
#if defined(QTB_NET_UNIX)
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#if defined(Q_OS_ANDROID)
#include <netinet/in.h>
#endif // defined(Q_OS_ANDROID)
#endif // defined(QTB_NET_UNIX)
#if defined(QTB_NET_WIN)
#define VAL_TYPE char
#elif defined(QTB_NET_UNIX)
#define VAL_TYPE void
#endif
void NetThread::setSocketOption(SOCKET sock)
{
  int val;
  socklen_t len = sizeof(val);

#if 0 // for TEST
  // TCP_NODELAY
  val = 1;
  if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const VAL_TYPE*)&val, len) == -1){
	std::cout << "[" << name << "] sockopt: TCP_NODELAY : setsockopt() error" << std::endl;
	std::cout << "errno = " << errno << std::endl << std::flush;
  }
  else {
	// Succeeded to set TCP_NODELAY
	if (outputLog)
	  std::cout << "[" << name << "] sockopt: TCP_NODELAY : setsockopt("<< val << ") O.K." << std::endl;
  }
#endif // 0 // for TEST

  // SO_RCVBUF
  val = 512*1024; // BDP(512KB) for TEST
  if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const VAL_TYPE*)&val, len) == -1){
	std::cout << "[" << name << "] sockopt: SO_RCVBUF : setsockopt() error" << std::endl;
	std::cout << "errno = " << errno << std::endl << std::flush;
  }
  else {
	// Succeeded to set SO_RCVBUF
	if (outputLog)
	  std::cout << "[" << name << "] sockopt: SO_RCVBUF : setsockopt("<< val << ") O.K." << std::endl;
  }

  // SO_SNDBUF
  val = 512*1024; // BDP(512KB) for TEST
  if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const VAL_TYPE*)&val, len) == -1){
	std::cout << "[" << name << "] sockopt: SO_SNDBUF : setsockopt() error" << std::endl;
	std::cout << "errno = " << errno << std::endl << std::flush;
  }
  else {
	// Succeeded to set SO_SNDBUF
	if (outputLog)
	  std::cout << "[" << name << "] sockopt: SO_SNDBUF : setsockopt("<< val << ") O.K." << std::endl;
  }
}

// check socket option
void NetThread::checkSocketOption(SOCKET sock)
{
  // return value
  union {
	int		i_val;
	long	l_val;
	char	c_val[10];
	struct linger linger_val;
	struct timeval timeval_val;
  } val;
  socklen_t len;

  // TCP_NODELAY
  len = sizeof(val);
  if (getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (VAL_TYPE*)&val, &len) == -1){
	std::cout << "[" << name << "] sockopt: TCP_NODELAY : getsockopt() error";
  }
  else {
	std::cout << "[" << name << "] sockopt: TCP_NODELAY : " << val.i_val << " : ";
	if (val.i_val == 0){
	  std::cout << "off";
	}
	else {
	  std::cout << "on";
	}
  }

  std::cout << std::endl;

  // SO_RCVBUF
  len = sizeof(val);
  if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (VAL_TYPE*)&val, &len) == -1){
	std::cout << "[" << name << "] sockopt: SO_RCVBUF : getsockopt() error";
  }
  else {
	std::cout << "[" << name << "] sockopt: SO_RCVBUF : " << val.i_val << " bytes";
  }

  std::cout << std::endl;

  // SO_SNDBUF
  len = sizeof(val);
  if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (VAL_TYPE*)&val, &len) == -1){
	std::cout << "[" << name << "] sockopt: SO_SNDBUF : getsockopt() error";
  }
  else {
	std::cout << "[" << name << "] sockopt: SO_SNDBUF : " << val.i_val << " bytes";
  }

  std::cout << std::endl;

  // std::flush
  std::cout << std::flush;
}

// setup interruptable
void NetThread::setupInterruptable(SOCKET sockfd, bool enable)
{
  // non blocking mode
#if defined(Q_OS_WIN)
  u_long val = enable ? 1L : 0L;
  ioctlsocket(sockfd, FIONBIO, &val);
#else // defined(Q_OS_WIN)
  int val = enable ? 1 : 0;
  ioctl(sockfd, FIONBIO, &val);
#endif // defined(Q_OS_WIN)
}

// interruptable version send
long NetThread::send_int(SOCKET sockfd, const char *buf, long size, int flags)
{
  long ret = 0;
  struct timeval timeout;
  fd_set writefds;

  while(true){ // polling
	// setup bitmap
	FD_ZERO(&writefds);
	FD_SET(sockfd, &writefds);

	// set timeout
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	ret = ::select(sockfd+1, 0, &writefds, 0, &timeout);

	if (!runThread){
	  ret = 0;
	  break;
	}

	if (ret != 0){
	  if (FD_ISSET(sockfd, &writefds)){
		ret = ::send(sockfd, buf, size, flags);
		break;
	  }
	}
  }

  return ret;
}

// interruptable version recv
long NetThread::recv_int(SOCKET sockfd, char *buf, long size, int flags)
{
  long ret = 0;
  struct timeval timeout;
  fd_set readfds;

  while(true){ // polling

	// setup bitmap
	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);

	// set timeout
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	ret = ::select(sockfd+1, &readfds, 0, 0, &timeout);

	if (!runThread){
	  ret = 0;
	  break;
	}

	if (ret != 0){
	  if (FD_ISSET(sockfd, &readfds)){
		ret = ::recv(sockfd, buf, size, flags);
		break;
	  }
	}
  }

  return ret;
}

// interruptable version connect
int NetThread::connect_int(int sockfd, const struct sockaddr *addr, socklen_t addrlen, int timeoutsec)
{
#if 0 // for TEST (new)
  int result = SOCKET_OK;

  // set attribute
  setupInterruptable(sockfd, true);

  // start connect
  if (timeoutsec < 0){
	// no timeout
	result = ::connect(sockfd, addr, addrlen);
	//std::cout << "result = " << result << std::endl << std::flush;
	//std::cout << "errno = " << errno << std::endl << std::flush;

	if (result < 0 && errno != EINPROGRESS && errno != 0){
	  return SOCKET_ERROR;
	}
	else {
	  return SOCKET_OK;
	}
  }
  else {
	// timeout
	result = ::connect(sockfd, addr, addrlen);

	if (result == -1){
	  if (errno != EINPROGRESS && errno != 0){
		// error
		//std::cout << "errno != EINPROGRESS" << std::endl << std::flush;
		//std::cout << "errno = " << errno << std::endl << std::flush;
		// set attribute
		setupInterruptable(sockfd, false);
		return SOCKET_ERROR;
	  }
	}
	else if (result == 0){
	  // connect
	  //std::cout << "result == 0" << std::endl << std::flush;
	  //std::cout << "errno = " << errno << std::endl << std::flush;
	  return SOCKET_OK;
	}
	else {
	  // unknown return value
	  //std::cout << "unknown return value" << std::endl << std::flush;
	  setupInterruptable(sockfd, false);
	  return SOCKET_ERROR;
	}

	// select
	struct timeval timeout;
	fd_set mask;
	fd_set write_mask, read_mask;
	int width = sockfd + 1;
	long timecounter = 0;

	// setup bitmap
	FD_ZERO(&mask);
	FD_SET(sockfd, &mask);

	// set timeout
	timeout.tv_sec = 1; // 1 sec.
	timeout.tv_usec = 0;

	while(true){
	  read_mask = mask;
	  write_mask = mask;
	  int select_result = ::select(width, &read_mask, &write_mask, 0, &timeout);
	  if (!runThread){
		//std::cout << "return from select()" << std::endl << std::flush;
		//std::cout << "timecounter = " << timecounter << std::endl << std::flush;
		return SOCKET_OK;
	  }
	  if (select_result == -1){
		if (errno != EINTR){
		  // error
		  //std::cout << "select_result == -1 : errno != EINTR" << std::endl << std::flush;
		  setupInterruptable(sockfd, false);
		  return SOCKET_ERROR;
		}
	  }
	  else if (select_result == 0){
		// time out
		//std::cout << "timeout : select_result == 0" << std::endl << std::flush;
		timecounter++;
		//std::cout << "timecounter = " << timecounter << std::endl << std::flush;
		if (timecounter > timeoutsec){
		  //std::cout << "timeout!!" << std::endl << std::flush;
		  setupInterruptable(sockfd, false);
		  return SOCKET_TIMEOUT;
		}
	  }
	  else {
		if (FD_ISSET(sockfd, &read_mask) || FD_ISSET(sockfd, &write_mask)){
		  // return value
		  union {
			int		i_val;
			long	l_val;
			char	c_val[10];
			struct linger linger_val;
			struct timeval timeval_val;
		  } val;
		  socklen_t len;

		  len = sizeof(len);
		  int getsockopt_result = ::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (VAL_TYPE*)&val, &len);
		  if (getsockopt_result != -1){
			if (val.i_val == 0){
			  // connect
			  //std::cout << "connect : val.i_val == 0" << std::endl << std::flush;
			  return SOCKET_OK;
			}
			else {
			  // connect error
			  //std::cout << "connect error : val.i_val != 0" << std::endl << std::flush;
			  setupInterruptable(sockfd, false);
			  return SOCKET_ERROR;
			}
		  }
		  else {
			// getsockopt error
			//std::cout << "getsockopt error : getsockopt_result == -1" << std::endl << std::flush;
			setupInterruptable(sockfd, false);
			return SOCKET_ERROR;
		  }
		}
	  }
	}
  }

  //std::cout << "last result = " << result << std::endl << std::flush;
  return SOCKET_OK;
#else // 0 // for TEST
  // set attribute
  setupInterruptable(sockfd, true);

  // start connect
  int result_connect = ::connect(sockfd, addr, addrlen);

  // check result of connect()
  if (result_connect == -1){
#if 0 // for TEST
	std::cout << "errno   = " << errno << std::endl << std::flush;
	std::cout << "sockfd  = " << sockfd << std::endl << std::flush;
	std::cout << "addrlen = " << addrlen << std::endl << std::flush;
	if (errno != EINPROGRESS && errno != 0){
		// error
		//std::cout << "errno != EINPROGRESS" << std::endl << std::flush;
		//std::cout << "errno = " << errno << std::endl << std::flush;
		// set attribute
		setupInterruptable(sockfd, false);
		return SOCKET_ERROR;
	}
#endif // 0 // for TEST
	// inprogress
  }
  else if (result_connect == 0){
	// connect OK
	//std::cout << "result_connect == 0" << std::endl << std::flush;
	//std::cout << "errno = " << errno << std::endl << std::flush;
	return SOCKET_OK;
  }
  else {
	// unknown return value
	std::cout << "unknown return value of connect() = " << result_connect << std::endl << std::flush;
	setupInterruptable(sockfd, false);
	return SOCKET_ERROR;
  }

  // check socket file descriptor
  struct timeval timeout;
  fd_set mask;
  fd_set write_mask, read_mask;
  int width = sockfd + 1;
  long timecounter = 0;

  // setup bitmap
  FD_ZERO(&mask);
  FD_SET(sockfd, &mask);

  // set timeout
  timeout.tv_sec = 1; // 1 sec.
  timeout.tv_usec = 0;

  while(runThread){
	// set initial masks
	read_mask = mask;
	write_mask = mask;
	// select
	int select_result = ::select(width, &read_mask, &write_mask, 0, &timeout);

	// check result of select()
	if (select_result == -1){
	  if (errno != EINTR){
		// error
		//std::cout << "select_result == -1 : errno != EINTR" << std::endl << std::flush;
		setupInterruptable(sockfd, false);
		return SOCKET_ERROR;
	  }
	}
	else if (select_result == 0){
	  //std::cout << "timeout : select_result == 0" << std::endl << std::flush;
	  // timeout
	  if (timeoutsec < 0){
		// no timeout
		continue;
	  }
	  else {
		// check timeout
		timecounter++;
		//std::cout << "timecounter = " << timecounter << std::endl << std::flush;
		if (timecounter > timeoutsec){
		  //std::cout << "timeout!!" << std::endl << std::flush;
		  setupInterruptable(sockfd, false);
		  return SOCKET_TIMEOUT;
		}
	  }
	}
	else {
	  // check socket
	  if (FD_ISSET(sockfd, &read_mask) || FD_ISSET(sockfd, &write_mask)){
		// return value
		union {
		  int		i_val;
		  long	l_val;
		  char	c_val[10];
		  struct linger linger_val;
		  struct timeval timeval_val;
		} val;
		socklen_t len;

		len = sizeof(len);
		int getsockopt_result = ::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (VAL_TYPE*)&val, &len);
		if (getsockopt_result != -1){
		  if (val.i_val == 0){
			// connect
			//std::cout << "connect : val.i_val == 0" << std::endl << std::flush;
			return SOCKET_OK;
		  }
		  else {
			// connect error
			//std::cout << "connect error : val.i_val != 0" << std::endl << std::flush;
			setupInterruptable(sockfd, false);
			return SOCKET_ERROR;
		  }
		}
		else {
		  // getsockopt error
		  //std::cout << "getsockopt error : getsockopt_result == -1" << std::endl << std::flush;
		  setupInterruptable(sockfd, false);
		  return SOCKET_ERROR;
		}
	  }
	}
  } // while-loop

  //std::cout << "last result = " << result << std::endl << std::flush;
  return SOCKET_OK;
#endif // 0 // for TEST
}

// connect with retry
#if !defined(Q_OS_WIN) // Portable Vresion (for MacOSX, FreeBSD...)
int NetThread::connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t addrlen)
{
  int timeout = settings->getTimeoutTime();
  int fd = socket(domain, type, protocol);
  if (fd < 0){
	return SOCKET_ERROR;
  }
  int result = connect_int(fd, addr, addrlen, timeout);
  if (result == SOCKET_OK){
	return fd;
  }
  closesocket(fd);

  if (result == SOCKET_TIMEOUT){
	return TIMEOUT_SOCKET;
  }
  else {
	return INVALID_SOCKET;
  }
}
#else // !defined(Q_OS_WIN)
int NetThread::connect_retry(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int timeout = settings->getTimeoutTime();
  int result = connect_int(sockfd, addr, addrlen, timeout);

  return result;
}
#endif // !defined(Q_OS_WIN)

#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

} // end of namespace qtbrynhildr
