// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPPANELOBJECT_H
#define DESKTOPPANELOBJECT_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QGraphicsItem>
#include <QGraphicsObject>

// Local Header
#include "mainwindow/desktoppanel.h"
#include "settings.h"

namespace qtbrynhildr {

class QtBrynhildr;

#if defined(Q_OS_WIN)
class DesktopPanelObject : public QGraphicsObject, public DesktopPanel, public QAbstractNativeEventFilter
#else // defined(Q_OS_WIN)
class DesktopPanelObject : public QGraphicsObject, public DesktopPanel
#endif // defined(Q_OS_WIN)
{
  Q_OBJECT

public:
  // constructor
  DesktopPanelObject(Settings *settings, QtBrynhildr *qtbrynhildr, QGraphicsItem *parent = Q_NULLPTR);
  // destructor
  ~DesktopPanelObject();

  // resize desktop
  void resizeDesktop(int width, int height);

  // update desktop
  void updateDesktop();

private:
#if defined(Q_OS_WIN)
  // native event filter
  bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif // defined(Q_OS_WIN)
};

} // end of namespace qtbrynhildr

#endif // DESKTOPPANELOBJECT_H
