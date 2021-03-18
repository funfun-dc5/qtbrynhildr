// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstdio>
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
  //  cout << "[" << name << "]" << " exitThread()" << endl << flush;
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
	cout << "[" << name << "]" << " start thread...start run()" << endl << flush;

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
		//cout << "[" << name << "]" << " connect Error: connectToServer()" << endl << flush; // error
		shutdownConnection();
		emit networkError(true);
		//emit outputLogMessage(QTB_MSG_CONNECT_ERROR);
		continue;
	  }
	  if (result_connect == CONNECT_FAILED_RETRY){
		if (settings->getOutputLog())
		  cout << "[" << name << "]" << " connect Error: connectToServer()" << endl << flush; // error
		runThread = false;
		break;
	  }
	  if (result_connect == CONNECT_FAILED_TIMEOUT){
		if (settings->getOutputLog())
		  cout << "[" << name << "]" << " connect Timeout: connectToServer()" << endl << flush; // error
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
		  cout << "[" << name << "]" << " Network Error: processForHeader()" << endl << flush; // error
		shutdownConnection();
		emit networkError(true);
		continue;
	  }
	  if (result_process == PROCESS_PASSWORD_ERROR){
		// error
		if (settings->getOutputLog())
		  cout << "[" << name << "]" << " Password Error: processForHeader()" << endl << flush; // error
		shutdownConnection();
		runThread = false;
		emit networkError(false);
		emit outputLogMessage(QTB_MSG_PASSWORD_ERROR);
		break;
	  }
	  if (result_process == PROCESS_CONNECT_ERROR){
		// error
		if (settings->getOutputLog())
		  cout << "[" << name << "]" << " Connect Error: processForHeader()" << endl << flush; // error
		shutdownConnection();
		runThread = false;
		emit networkError(false);
		emit outputLogMessage(QTB_MSG_ALREADY_CONNECT_ANOTHER_CLIENT);
		break;
	  }
	  if (result_process == PROCESS_VIDEO_MODE_ERROR){
		// error
		if (settings->getOutputLog())
		  cout << "[" << name << "]" << " Connect Error: processForHeader()" << endl << flush; // error
		shutdownConnection();
		runThread = false;
		emit networkError(false);
		emit outputLogMessage(QTB_MSG_NOTSUPPORT_VIDEO_MODE);
		break;
	  }
	  if (result_process == PROCESS_UNKNOWN_ERROR){
		// error
		if (settings->getOutputLog())
		  cout << "[" << name << "]" << " Unknown Error: processForHeader()" << endl << flush; // error
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
		cout << "[" << name << "]" << " Failed: transmitBuffer(): network error" << endl << flush; // error
	  shutdownConnection();
	  emit networkError(true);
	  continue;
	  break;
	case TRANSMIT_DATASIZE_ERROR:
	  if (settings->getOutputLog())
		cout << "[" << name << "]" << " Failed: transmitBuffer(): data size" << endl << flush; // error
	  break;
	case TRANSMIT_FAILED_PUT_BUFFER:
	  if (settings->getOutputLog())
		cout << "[" << name << "]" << " Failed: transmitBuffer(): put buffer." << endl << flush; // error
	  break;
	case TRANSMIT_FAILED_TRANSMIT_DEVICE_BUFFER:
	  if (settings->getOutputLog())
		cout << "[" << name << "]" << " Failed: transmitBuffer(): transmit device buffer." << endl << flush; // error
	  break;
	case TRANSMIT_FAILED_IMAGE_DRAW:
	  if (settings->getOutputLog())
		cout << "[" << name << "]" << " Failed: transmitBuffer(): image draw." << endl << flush; // error
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
	cout << "[" << name << "]" << " stop thread...exit run()" << endl << flush;
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
	  //cout << "TimeOut!" << endl << flush;
	  return TIMEOUT_SOCKET;
	}
	break;
  }
  // free all addrinfo
  while(topAddrinfo != NULL){
	addrinfo = topAddrinfo;
	topAddrinfo = addrinfo->ai_next;
	freeaddrinfo(addrinfo);
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
	const char *encryption_key = qPrintable(settings->getPassword());
	for (int i = 0;i < ENCRYPTION_KEY_LENGTH; i++){
	  if (encryption_key[i] == '\0')
		break;

	  key[i] = encryption_key[i];
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
	  cout << "errno = " << errno << endl << flush;
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
  cout << "============================== HEADER ==============================" << endl;
  cout << "com_data->data_type      :" << (int)com_data->data_type << endl;
  cout << "com_data->thread         :" << (int)com_data->thread << endl;
  cout << "com_data->sound_type     :" << (int)com_data->sound_type << endl;
  cout << "com_data->encryption     :" << (int)com_data->encryption << endl;
  cout << "com_data->data_size      :" << com_data->data_size << endl;

  ios::fmtflags flags = cout.flags();

  cout << "com_data->check_digit_enc:" << hex << uppercase << setfill('0');
  for (int i = 0; i < ENCRYPTION_KEY_LENGTH; i++){
	cout << setw(2) << (int)(com_data->check_digit_enc[i] & 0xff);
  }
  cout << endl;
  cout << "com_data->check_digit    :"	<< (int)(com_data->check_digit & 0xffff) << endl;

  cout.setf(flags);

  cout << "com_data->ver            :\"" << com_data->ver[0]
										 << com_data->ver[1]
										 << com_data->ver[2]
										 << com_data->ver[3] << "\"" << endl;
  cout << "com_data->samplerate     :"  << com_data->samplerate << endl;
  cout << "com_data->image_cx       :"  << com_data->image_cx << endl;
  cout << "com_data->image_cy       :"  << com_data->image_cy << endl;
  cout << "com_data->server_cx      :"  << com_data->server_cx << endl;
  cout << "com_data->server_cy      :"  << com_data->server_cy << endl;
  cout << "com_data->control        :"	<< (int)com_data->control << endl;
#if QTB_PLUGINS_DISABLE_SUPPORT
  cout << "com_data->plugins_disable:"	<< (int)com_data->plugins_disable << endl;
#endif // QTB_PLUGINS_DISABLE_SUPPORT
  cout << "com_data->filedrop       :"	<< (int)com_data->filedrop << endl;
  cout << "com_data->mouse_move     :"	<< (int)com_data->mouse_move << endl;
#if QTB_EXTRA_BUTTON_SUPPORT
  cout << "com_data->mouse_x1       :"  << (int)com_data->mouse_x1 << endl;
  cout << "com_data->mouse_x2       :"  << (int)com_data->mouse_x2 << endl;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  cout << "com_data->mouse_x        :"  << com_data->mouse_x << endl;
  cout << "com_data->mouse_y        :"  << com_data->mouse_y << endl;
  cout << "com_data->mouse_left     :"  << (int)com_data->mouse_left << endl;
  cout << "com_data->mouse_right    :"  << (int)com_data->mouse_right << endl;
#if QTB_EXTRA_BUTTON_SUPPORT
  cout << "com_data->mouse_middle   :"  << (int)com_data->mouse_middle << endl;
#endif // QTB_EXTRA_BUTTON_SUPPORT
  cout << "com_data->mouse_wheel    :"  << (int)com_data->mouse_wheel << endl;
  cout << "com_data->keycode        :"  << (int)com_data->keycode << endl;
  cout << "com_data->keycode_flg    :"  << (int)com_data->keycode_flg << endl;
  cout << "com_data->cursor_hotspot_x :"  << (int)com_data->cursor_hotspot_x << endl;
  cout << "com_data->cursor_hotspot_y :"  << (int)com_data->cursor_hotspot_y << endl;
  cout << "com_data->monitor_no     :"	<< (int)com_data->monitor_no << endl;
  cout << "com_data->monitor_count  :"	<< (int)com_data->monitor_count << endl;
  cout << "com_data->max_fps        :"	<< (int)com_data->max_fps << endl;
  cout << "com_data->sound_capture  :"	<< (int)com_data->sound_capture << endl;
  cout << "com_data->keydown        :"	<< com_data->keydown << endl;
  cout << "com_data->video_quality  :"	<< (int)com_data->video_quality << endl;
  cout << "com_data->mouse_cursor   :"	<< (int)com_data->mouse_cursor << endl;
  cout << "com_data->gamepad1       :"	<< com_data->gamepad1 << endl;
  cout << "com_data->gamepad2       :"	<< com_data->gamepad2 << endl;
  cout << "com_data->gamepad3       :"	<< com_data->gamepad3 << endl;
  cout << "com_data->gamepad4       :"	<< com_data->gamepad4 << endl;
  cout << "com_data->client_scroll_x:"  << com_data->client_scroll_x << endl;
  cout << "com_data->client_scroll_y:"  << com_data->client_scroll_y << endl;
  cout << "com_data->video_mode     :"  << (int)com_data->video_mode << endl;
  cout << "com_data->server_version :"  << com_data->server_version << endl;
  cout << "com_data->zoom           :"  << com_data->zoom << endl;
  cout << "com_data->mode           :"	<< (int)com_data->mode << endl;
  cout << "com_data->sound_quality  :"	<< (int)com_data->sound_quality << endl;
  cout << "com_data->frame_no       :"	<< (int)com_data->frame_no << endl;
  cout << "com_data->gamepad5       :"	<< com_data->gamepad5 << endl;
  cout << "com_data->gamepad6       :"	<< com_data->gamepad6 << endl;
  cout << "com_data->gamepad7       :"	<< com_data->gamepad7 << endl;
  cout << "com_data->gamepad8       :"	<< com_data->gamepad8 << endl;

  cout << endl << flush;
}

// save protocol header
void NetThread::saveHeader(COM_DATA *com_data, const char* filename)
{
  fstream file;
  file.open(filename, ios::out | ios::binary | ios::trunc);
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

  cout << "========================== HEADER DUMP ==========================" << endl;

  ios::fmtflags flags = cout.flags();

  cout << hex << uppercase << setfill('0');

  unsigned char *ptr = (unsigned char *)com_data;
  for (unsigned int i = 0; i < sizeof(COM_DATA); i++){
	if (i % 16 == 0){
	  cout << endl;
	  cout << setw(4) << i << ": ";
	}
	else {
	  cout << " ";
	}
	cout << setw(2) << (unsigned int)*ptr;
	ptr++;
  }
  cout << endl << endl << flush;

  cout.setf(flags);
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
  cout << "================================   " << duration << endl;
}

// print time information
void NetThread::printTimeInfo(const char *str)
{
  qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
  qint64 pastTime = currentTime - startTime;

  ios::fmtflags flags = cout.flags();

  cout << "[" << name << "] " << left << setw(20) << str << ": " << pastTime << endl;

  cout.setf(flags);
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
	cout << "[" << name << "] sockopt: TCP_NODELAY : setsockopt() error" << endl;
	cout << "errno = " << errno << endl << flush;
  }
  else {
	// Succeeded to set TCP_NODELAY
	if (outputLog)
	  cout << "[" << name << "] sockopt: TCP_NODELAY : setsockopt("<< val << ") O.K." << endl;
  }
#endif // 0 // for TEST

  // SO_RCVBUF
  val = 512*1024; // BDP(512KB) for TEST
  if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const VAL_TYPE*)&val, len) == -1){
	cout << "[" << name << "] sockopt: SO_RCVBUF : setsockopt() error" << endl;
	cout << "errno = " << errno << endl << flush;
  }
  else {
	// Succeeded to set SO_RCVBUF
	if (outputLog)
	  cout << "[" << name << "] sockopt: SO_RCVBUF : setsockopt("<< val << ") O.K." << endl;
  }

  // SO_SNDBUF
  val = 512*1024; // BDP(512KB) for TEST
  if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const VAL_TYPE*)&val, len) == -1){
	cout << "[" << name << "] sockopt: SO_SNDBUF : setsockopt() error" << endl;
	cout << "errno = " << errno << endl << flush;
  }
  else {
	// Succeeded to set SO_SNDBUF
	if (outputLog)
	  cout << "[" << name << "] sockopt: SO_SNDBUF : setsockopt("<< val << ") O.K." << endl;
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
	cout << "[" << name << "] sockopt: TCP_NODELAY : getsockopt() error";
  }
  else {
	cout << "[" << name << "] sockopt: TCP_NODELAY : " << val.i_val << " : ";
	if (val.i_val == 0){
	  cout << "off";
	}
	else {
	  cout << "on";
	}
  }

  cout << endl;

  // SO_RCVBUF
  len = sizeof(val);
  if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (VAL_TYPE*)&val, &len) == -1){
	cout << "[" << name << "] sockopt: SO_RCVBUF : getsockopt() error";
  }
  else {
	cout << "[" << name << "] sockopt: SO_RCVBUF : " << val.i_val << " bytes";
  }

  cout << endl;

  // SO_SNDBUF
  len = sizeof(val);
  if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (VAL_TYPE*)&val, &len) == -1){
	cout << "[" << name << "] sockopt: SO_SNDBUF : getsockopt() error";
  }
  else {
	cout << "[" << name << "] sockopt: SO_SNDBUF : " << val.i_val << " bytes";
  }

  cout << endl;

  // flush
  cout << flush;
}

// set socket attribute
void NetThread::setSocketAttribute(SOCKET sockfd, int socket_type, bool enable)
{
#if defined(Q_OS_WIN)
  u_long val = enable ? 1 : 0;

  ioctlsocket(sockfd, socket_type, &val);
#else // defined(Q_OS_WIN)
  int val = enable ? 1 : 0;

  ioctl(sockfd, socket_type, &val);
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
  int result = SOCKET_OK;

  // set attribute(Non Blocking)
  setSocketAttribute(sockfd, FIONBIO, true);

  // start connect
  if (timeoutsec < 0){
	// no timeout
	result = ::connect(sockfd, addr, addrlen);
	//cout << "result = " << result << endl << flush;
	//cout << "errno = " << errno << endl << flush;
	return SOCKET_OK;
  }
  else {
	// timeout
	result = ::connect(sockfd, addr, addrlen);

	if (result == -1){
	  if (errno != EINPROGRESS && errno != 0){
		// error
		//cout << "errno != EINPROGRESS" << endl << flush;
		//cout << "errno = " << errno << endl << flush;
		return SOCKET_ERROR;
	  }
	}
	else if (result == 0){
	  // connect
	  //cout << "result == 0" << endl << flush;
	  //cout << "errno = " << errno << endl << flush;
	  return SOCKET_OK;
	}
	else {
	  // unknown return value
	  //cout << "unknown return value" << endl << flush;
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
		//cout << "return from select()" << endl << flush;
		//cout << "timecounter = " << timecounter << endl << flush;
		return SOCKET_OK;
	  }
	  if (select_result == -1){
		if (errno != EINTR){
		  // error
		  //cout << "select_result == -1 : errno != EINTR" << endl << flush;
		  return SOCKET_ERROR;
		}
	  }
	  else if (select_result == 0){
		// time out
		//cout << "timeout : select_result == 0" << endl << flush;
		timecounter++;
		//cout << "timecounter = " << timecounter << endl << flush;
		if (timecounter > timeoutsec){
		  //cout << "timeout!!" << endl << flush;
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
			  //cout << "connect : val.i_val == 0" << endl << flush;
			  return SOCKET_OK;
			}
			else {
			  // connect error
			  //cout << "connect error : val.i_val != 0" << endl << flush;
			  return SOCKET_ERROR;
			}
		  }
		  else {
			// getsockopt error
			//cout << "getsockopt error : getsockopt_result == -1" << endl << flush;
			return SOCKET_ERROR;
		  }
		}
	  }
	}
  }

  //cout << "last result = " << result << endl << flush;
  return SOCKET_OK;
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
  if (connect_int(fd, addr, addrlen, timeout) == SOCKET_OK){
	return fd;
  }
  closesocket(fd);

  return TIMEOUT_SOCKET;
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
