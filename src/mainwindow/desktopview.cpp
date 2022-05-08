// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QPainter>

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
  Q_UNUSED(event);

  if (image.isNull()){
	return;
  }

  QPainter painter(this);

  // draw desktop image
  painter.drawImage(0, 0, image);

  // draw marker for mouse cursor
  if (drawMarkerCounter > 0){
	int length = drawMarkerCounter*10;
	int x = currentMousePos.x() - length/2;
	int y = currentMousePos.y() - length/2;

	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setPen(QPen(Qt::green, 4));
	painter.drawArc(x, y, length, length, 0*360, 16*360);

	drawMarkerCounter--;
  }
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
