// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// Qt Header
#include <QTimeZone>

// Local Header
#include "ntfs.h"

namespace qtbrynhildr {

// constructor
NTFS::NTFS()
{
  // init NTFS epoch (1601.1.1 01:00:00)
  ntfsEpoch.setDate(QDate(1601,1,1));
  ntfsEpoch.setTime(QTime(1,0));
  // init UNIX epoch (1970.1.1 00:00:00)
  unixEpoch.setDate(QDate(1970,1,1));
  unixEpoch.setTime(QTime(0,0));

  epochDiff = ntfsEpoch.msecsTo(unixEpoch); // msecs

  diffUTC = ntfsEpoch.offsetFromUtc() * (qint64)1000; // msecs
}

// get FILETIME (64bit:UTC) from QDateTime (LocalTime)
qint64 NTFS::toFILETIME(QDateTime time)
{
  return (ntfsEpoch.msecsTo(time) - diffUTC) * 10000; // from epoch (1 = 100 nsecs)
}

// get QDateTime (LocalTime) from FILETIME (64bit:UTC)
QDateTime NTFS::toQDateTime(qint64 filetime)
{
  qint64 msecs = filetime / 10000 - epochDiff;

  return QDateTime::fromMSecsSinceEpoch(msecs);
}

} // end of namespace qtbrynhildr
