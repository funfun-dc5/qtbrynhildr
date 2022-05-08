// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPVIEW_H
#define DESKTOPVIEW_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QScrollArea>

// Local Header


namespace qtbrynhildr {

class DesktopView : public QScrollArea
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Type
  //-------------------------------------------------------------------------------
private:
  // Touch Operation Type
  typedef enum {
	TOP_TYPE_UNKNOWN,
	TOP_TYPE_1POINT,
	TOP_TYPE_2POINT,
	TOP_TYPE_3POINT
  } TOP_TYPE;

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DesktopView();
  // destructor
  virtual ~DesktopView();
};

} // end of namespace qtbrynhildr

#endif // DESKTOPVIEW_H
