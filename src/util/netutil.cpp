// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if defined(Q_OS_WIN)
#include <winsock2.h>
#include <iphlpapi.h>
#else // defined(Q_OS_WIN)
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif // defined(Q_OS_WIN)

#include <iomanip>
#include <cstdio>
#include <cstring>

// Qt Header

// Local Header
#include "netutil.h"

namespace qtbrynhildr {

// constructor
NetUtil::NetUtil()
  :
  // for DEBUG
  outputLog(true)
{
  // for TEST
  getMacAddress();
}

// destructor
NetUtil::~NetUtil()
{
}

// get mac address
#if defined(Q_OS_WIN)
bool NetUtil::getMacAddress()
{
  PIP_ADAPTER_ADDRESSES addresses;
  ULONG result;
  ULONG size;

  // get size
  result = ::GetAdaptersAddresses(AF_UNSPEC, 0, NULL, NULL, &size);
  if (result != ERROR_BUFFER_OVERFLOW){
	return false;
  }
  // alloc buffer
  addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);
  if (addresses == NULL){
	return false;
  }

  result = ::GetAdaptersAddresses(AF_UNSPEC, 0, NULL, addresses, &size);
  if (result != ERROR_SUCCESS){
	free(addresses);
	return false;
  }

  PIP_ADAPTER_ADDRESSES adr;
  adr = addresses;
  while(adr != NULL){
	// check network status
	if (adr->OperStatus == IfOperStatusUp){
	  // check network type
	  if (adr->IfType != IF_TYPE_SOFTWARE_LOOPBACK){
		// get a mac address
		if (adr->PhysicalAddressLength > 0){
		  std::cout << "MAC:" << std::hex << setfill('0');
		  for (ULONG i = 0; i < adr->PhysicalAddressLength; ++ i) {
			std::cout << std::setw(2) << (int)adr->PhysicalAddress[i];
		  }
		  std::cout << std::endl << std::flush;
		}
	  }
	}
	adr = adr->Next;
  }

  free(addresses);

  return true;
}
#else // defined(Q_OS_WIN)
char mac_address[128]={""};
bool NetUtil::getMacAddress()
{
  //http://field-notes.hatenablog.jp/entry/20101216/1292467817
  mac_address[0]=0;
  struct ifreq *ifr, *ifend;
  struct ifreq ifreq;
  struct ifconf ifc;
  struct ifreq ifs[16];
  int fd;
  char *addr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  ifc.ifc_len = sizeof(ifs);
  ifc.ifc_req = ifs;
  if(ioctl(fd, SIOCGIFCONF, &ifc)<0){close(fd);return false;}

  ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
  for(ifr = ifc.ifc_req;ifr < ifend;ifr++){
	if(ifr->ifr_addr.sa_family == AF_INET){
	  strncpy(ifreq.ifr_name, ifr->ifr_name, sizeof(ifreq.ifr_name));
	  if(ioctl(fd, SIOCGIFHWADDR, &ifreq)<0)continue;
	  addr = ifreq.ifr_hwaddr.sa_data;
	  sprintf(mac_address,"%02x:%02x:%02x:%02x:%02x:%02x",
			  (unsigned char)addr[0],  (unsigned char)addr[1],  (unsigned char)addr[2],  (unsigned char)addr[3],  (unsigned char)addr[4],  (unsigned char)addr[5]);
	  std::cout << "MAC:" << mac_address << std::endl << std::flush;
	}
  }
  close(fd);

  return true;
}
#endif // defined(Q_OS_WIN)

} // end of namespace qtbrynhildr
