// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// ntfs.h

#ifndef NTFS_H
#define NTFS_H

// Qt Header
#include <QDateTime>

// Local Header


namespace qtbrynhildr {

// NTFS
class NTFS
{
public:
  // constructor
  NTFS();

  // get FILETIME (64bit:UTC) from QDateTime (LocalTime)
  qint64 toFILETIME(QDateTime time);

  // get QDateTime (LocalTime) from FILETIME (64bit:UTC)
  QDateTime toQDateTime(qint64 filetime);

private:
  // NTFS Epoch (1601.1.1 01:00:00)
  QDateTime ntfsEpoch;

  // UNIX Epoch (1970.1.1 00:00:00)
  QDateTime unixEpoch;

  // diff NTFS epoch to UNIX epoch
  qint64 epochDiff;

  // diff from UTC
  int diffUTC;
};

} // end of namespace qtbrynhildr

#endif // NTFS_H
