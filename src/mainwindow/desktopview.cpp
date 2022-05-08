// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "desktopview.h"

namespace qtbrynhildr {

// constructor
DesktopView::DesktopView(QtBrynhildr *qtbrynhildr, QWidget *parent)
  :QScrollArea(parent)
  ,DesktopPanel(qtbrynhildr)
  // for DEBUG
  ,outputLog(true)
{
}

// destructor
DesktopView::~DesktopView()
{
}

// paint event
void DesktopView::paintEvent(QPaintEvent *event)
{
}

// viewport event for event handling (touchpanel)
bool DesktopView::viewportEvent(QEvent *event)
{
  switch(event->type()){
  case QEvent::TouchBegin:
  case QEvent::TouchUpdate:
  case QEvent::TouchEnd:
	{
	  // touch event
	  switch(settings->getTouchpanelOperationType()){
	  case QTB_TOUCHPANELOPERATIONTYPE_QTBRYNHILDR:
		break;
	  case QTB_TOUCHPANELOPERATIONTYPE_KEROREMOTE:
		break;
	  default:
		// Illegal TouchpanelOperationType
		break;
	  }
	}
	break;
  default:
	// Nothing to do
	break;
  }

  return QScrollArea::viewportEvent(event);
}

} // end of namespace qtbrynhildr
