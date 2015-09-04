// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// netthread.cpp

// Common Header
#include "common/common.h"

// System Header
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>

// Qt Header

// Local Header
#include "netthread.h"
#include "parameters.h"

#if 0 // for TEST
#include <fstream>
#endif

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
NetThread::NetThread(const char *name, Settings *settings, MainWindow *mainWindow)
  :
  mainWindow(mainWindow),
  name(name),
  settings(settings),
  com_data(0),
  runThread(true)
{
}

// destructor
NetThread::~NetThread()
{
}

// exit thread
void NetThread::exitThread()
{
  //  cout << "[" << name << "]" << " exitThread()" << endl << flush;
  runThread = false;
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
	QThread::msleep(5); // 5 msecs sleep

	CONNECT_RESULT result_connect = connectToServer();
	if (result_connect != CONNECT_SUCCEEDED){
	  if (result_connect == CONNECT_WAITED_COUNT){
		continue;
	  }
	  if (result_connect == CONNECT_FAILED){
		cout << "[" << name << "]" << " connect Error: connectToServer()" << endl << flush; // error
		shutdownConnection();
		emit networkError();
		runThread = false;
		emit outputLogMessage(QTB_MSG_CONNECT_ERROR);
		break;
	  }
	  if (result_connect == CONNECT_FAILED_RETRY){
		cout << "[" << name << "]" << " connect Error: connectToServer()" << endl << flush; // error
		runThread = false;
		break;
	  }
	}

	// process header (send and receive)
	PROCESS_RESULT result_process = processForHeader();
	if (result_process != PROCESS_SUCCEEDED){
	  if (result_process == PROCESS_NETWORK_ERROR){
		// error
		cout << "[" << name << "]" << " Network Error: processForHeader()" << endl << flush; // error
		shutdownConnection();
		emit networkError();
		continue;
	  }
	  if (result_process == PROCESS_PASSWORD_ERROR){
		// error
		cout << "[" << name << "]" << " Password Error: processForHeader()" << endl << flush; // error
		shutdownConnection();
		runThread = false;
		emit outputLogMessage(QTB_MSG_PASSWORD_ERROR);
		break;
	  }
	  if (result_process == PROCESS_CONNECT_ERROR){
		// error
		cout << "[" << name << "]" << " Connect Error: processForHeader()" << endl << flush; // error
		shutdownConnection();
		runThread = false;
		emit outputLogMessage(QTB_MSG_ALREADY_CONNECT_ANOTHER_CLIENT);
		break;
	  }
	  if (result_process == PROCESS_UNKNOWN_ERROR){
		// error
		cout << "[" << name << "]" << " Unkown Error: processForHeader()" << endl << flush; // error
		shutdownConnection();
		runThread = false;
		break;
	  }
	}

	// transmit local buffer to device buffer
	TRANSMIT_RESULT result_transmit = transmitBuffer();
	switch(result_transmit){
	case TRANSMIT_SUCCEEDED:
	  // Nothing to do
	  break;
	case TRANSMIT_NETWORK_ERROR:
	  cout << "[" << name << "]" << " Failed: transmitBuffer(): network error" << endl << flush; // error
	  shutdownConnection();
	  emit networkError();
	  continue;
	  break;
	case TRANSMIT_DATASIZE_ERROR:
	  cout << "[" << name << "]" << " Failed: transmitBuffer(): data size" << endl << flush; // error
	  break;
	case TRANSMIT_FAILED_PUT_BUFFER:
	  cout << "[" << name << "]" << " Failed: transmitBuffer(): put buffer." << endl << flush; // error
	  break;
	case TRANSMIT_FAILED_TRANSMIT_DEVICE_BUFFER:
	  cout << "[" << name << "]" << " Failed: transmitBuffer(): transmit device buffer." << endl << flush; // error
	  break;
	case TRANSMIT_FAILED_IMAGE_DRAW:
	  cout << "[" << name << "]" << " Failed: transmitBuffer(): image draw." << endl << flush; // error
	  break;
	default:
	  // error
	  ABORT();
	  break;
	}
  }

  if (settings->getOutputLog())
	cout << "[" << name << "]" << " stop thread...exit run()" << endl << flush;
}

// connected
void NetThread::connectedToServer()
{
}

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_MAC)
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
#if defined(Q_OS_WIN)
	if (settings->getOutputLog()){
	  const QString text = QString("socketToServer() : getaddrinfo(): error = ") + QString::number(error);
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	if (settings->getOutputLog()){
	  const QString text = QString("socketToServer() : getaddrinfo(): strerror = ") + gai_strerror(error);
	  emit outputLogMessage(PHASE_DEBUG, text);
	}
#endif // defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	return INVALID_SOCKET;
  }

  for (; addrinfo != NULL; addrinfo = addrinfo->ai_next){
	sock = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
	if (sock == INVALID_SOCKET){
	  continue;
	}
	if (connect_retry(sock, addrinfo->ai_addr, (int)addrinfo->ai_addrlen) == SOCKET_ERROR){
	  closesocket(sock);
	  sock = INVALID_SOCKET;
	  continue;
	}
	break;
  }
  freeaddrinfo(addrinfo);

  // for socket option
  if (sock != INVALID_SOCKET){
	// set socket option
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	setSocketOption(sock);
#endif // defined(Q_OS_LINUX) || defined(Q_OS_MAC)
#if defined(DEBUG)
	// check socket option
	checkSocketOption(sock);
#endif // defined(DEBUG)
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

// send data
long NetThread::sendData(SOCKET sock, const char *buf, long size)
{
  //                                      0123456789ABCDEF
  char key[ENCRYPTION_KEY_LENGTH + 1] = {"@@@@@@@@@@@@@@@@"};

  // copy encryption key
#if 1 // simple description
  const char *encryption_key = qPrintable(settings->getPassword());
#else
  const char *encryption_key = settings->getPassword().toUtf8().constData();
#endif
  for (int i = 0;i < ENCRYPTION_KEY_LENGTH; i++){
	if (encryption_key[i] == '\0')
	  break;

	key[i] = encryption_key[i];
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

#if 0 // for TEST
  {
	fstream file;
	file.open("header.dat", ios::out | ios::binary | ios::trunc);
	if (file.is_open()){
	  file.write((char*)com_data, sizeof(COM_DATA));
	  file.close();
	}
  }
#endif

  // send
  return send(sock, buf, size, 0);
}

// receive data
long NetThread::receiveData(SOCKET sock, char *buf, long size)
{
  long received_size = 0;

  while(received_size < size){
	long ret = recv(sock, buf + received_size, size - received_size, 0);
	if (ret > 0){
	  received_size += ret;
	}
	else {
	  received_size = -1;
	  break;
	}
  }

  return received_size;
}

// set socket option
#include <cerrno>
void NetThread::setSocketOption(SOCKET sock)
{
  int val = 1;
  socklen_t len = sizeof(val);
#if defined(Q_OS_WIN)
  if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&val, len) == -1){
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
  if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const void*)&val, len) == -1){
#endif
	cout << "[" << name << "] sockopt: SO_KEEPALIVE : setsockopt() error";
	cout << "errno = " << errno << endl << flush;
  }
  else {
	// Succeeded to set SO_KEEPALIVE
  }
}

#if defined(DEBUG)
// check socket option
void NetThread::checkSocketOption(SOCKET sock)
{
  union {
	int		i_val;
	long	l_val;
	char	c_val[10];
	struct linger linger_val;
	struct timeval timeval_val;
  } val;
  socklen_t len;
  len = sizeof(val);

  // SO_KEEPALIVE
#if defined(Q_OS_WIN)
  if (getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, &len) == -1){
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
  if (getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&val, &len) == -1){
#endif
	cout << "[" << name << "] sockopt: SO_KEEPALIVE : getsockopt() error";
  }
  else {
	cout << "[" << name << "] sockopt: SO_KEEPALIVE : ";
	if (val.i_val == 0){
	  cout << "off";
	}
	else {
	  cout << "on";
	}
  }
  // flush
  cout << endl << flush;
}
#endif // defined(DEBUG)

// connect with retry
#define MAXSLEEP 128
int NetThread::connect_retry(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  for (int numsec = 1; numsec <= MAXSLEEP; numsec <<= 1){
	if (::connect(sockfd, addr, addrlen) == 0){
	  return 0;
	}

	// check exit
	if (!runThread){
	  return SOCKET_ERROR;
	}

	// sleep for next try
	if (numsec <= MAXSLEEP/2){
	  QThread::sleep(numsec);
	}
  }

  return SOCKET_ERROR;
}

#endif // defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_MAC)

// print protocol header
void NetThread::printHeader()
{
  // check
  if (com_data == 0)
	return;

  cout << "============================== HEADER ==============================" << endl;
  cout << "com_data->data_type      :" << (int)com_data->data_type << endl;
  cout << "com_data->thread         :" << (int)com_data->thread << endl;
  cout << "com_data->sound_type     :" << (int)com_data->sound_type << endl;
  cout << "com_data->encryption     :" << com_data->encryption << endl;
  cout << "com_data->data_size      :" << com_data->data_size << endl;

  ios::fmtflags flags = cout.flags();
  char fill = cout.fill();
  cout << "com_data->check_digit_enc:" << hex << uppercase << setfill('0');
  for (int i = 0; i < ENCRYPTION_KEY_LENGTH; i++){
	cout << setw(2) << (int)(com_data->check_digit_enc[i] & 0xff);
  }
  cout << endl;
  cout << "com_data->check_digit    :"	<< (int)(com_data->check_digit & 0xff) << endl;
  cout << setiosflags(flags) << setfill(fill);

  cout << "com_data->ver            :\"" << com_data->ver[0]
										 << com_data->ver[1]
										 << com_data->ver[2]
										 << com_data->ver[3] << "\"" << endl;
  cout << "com_data->samplerate     :"  << com_data->samplerate << endl;
  cout << "com_data->image_cx       :"  << com_data->image_cx << endl;
  cout << "com_data->image_cy       :"  << com_data->image_cy << endl;
  cout << "com_data->server_cx      :"  << com_data->server_cx << endl;
  cout << "com_data->server_cy      :"  << com_data->server_cy << endl;
  cout << "com_data->control        :"	<< com_data->control << endl;
  cout << "com_data->mouse_move     :"	<< com_data->mouse_move << endl;
  cout << "com_data->mouse_x        :"  << com_data->mouse_x << endl;
  cout << "com_data->mouse_y        :"  << com_data->mouse_y << endl;
  cout << "com_data->mouse_left     :"  << (int)com_data->mouse_left << endl;
  cout << "com_data->mouse_right    :"  << (int)com_data->mouse_right << endl;
  cout << "com_data->mouse_wheel    :"  << (int)com_data->mouse_wheel << endl;
  cout << "com_data->keycode        :"  << (int)com_data->keycode << endl;
  cout << "com_data->keycode_flg    :"  << (int)com_data->keycode_flg << endl;
  cout << "com_data->monitor_no     :"	<< (int)com_data->monitor_no << endl;
  cout << "com_data->monitor_count  :"	<< (int)com_data->monitor_count << endl;
  cout << "com_data->sound_capture  :"	<< com_data->sound_capture << endl;
  cout << "com_data->keydown        :"	<< com_data->keydown << endl;
  cout << "com_data->video_quality  :"	<< com_data->video_quality << endl;
  cout << "com_data->client_scroll_x:"  << com_data->client_scroll_x << endl;
  cout << "com_data->client_scroll_y:"  << com_data->client_scroll_y << endl;
  cout << "com_data->zoom           :"  << com_data->zoom << endl;
  cout << "com_data->mode           :"	<< com_data->mode << endl;
  cout << "com_data->sound_quality  :"	<< com_data->sound_quality << endl;

  cout << endl << flush;
}

} // end of namespace qtbrynhildr
