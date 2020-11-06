// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// for threads(control/graphics/sound)
//---------------------------------------------------------------------------
// sleep time (milli seconds)
const unsigned long QTB_THREAD_SLEEP_TIME = 1;

//---------------------------------------------------------------------------
// for control
//---------------------------------------------------------------------------
// keyboard buffer size (entry)
const int QTB_KEYBOARD_BUFFER_SIZE = 256;

// mouse buffer size (entry)
const int QTB_MOUSE_BUFFER_SIZE	= 256;

// local data buffer size for control (byte)
const int QTB_CONTROL_LOCAL_BUFFER_SIZE = 128 * 1024;

//---------------------------------------------------------------------------
// for graphics
//---------------------------------------------------------------------------
// local data buffer size for graphics (byte)
const int QTB_GRAPHICS_LOCAL_BUFFER_SIZE = 1024 * 1024;

// minimum width of area mode
const int QTB_AREA_MODE_MINIMUM_WIDTH = 64;

// minimum height of area mode
const int QTB_AREA_MODE_MINIMUM_HEIGHT = 64;

//---------------------------------------------------------------------------
// for sound
//---------------------------------------------------------------------------
// local data buffer size for sound (byte)
const int QTB_SOUND_LOCAL_BUFFER_SIZE = 512 * 1024;

//---------------------------------------------------------------------------
// for GUI
//---------------------------------------------------------------------------
// window opacity in floating
const double QTB_WINDOW_OPACITY = 0.95;

// window update duration (ms)
const double QTB_WINDOW_UPDATE_DURATION = 1000;

//---------------------------------------------------------------------------
// for GUI (TOUCHPANEL)
//---------------------------------------------------------------------------
// tap time thresold for touchpanel (ms)
const int QTB_TOUCHPANEL_TAP_TIME_THRESHOLD = 500;

// move distance thresold for touchpanel
const int QTB_TOUCHPANEL_MOVE_DIST_THRESHOLD = 40;

// touchpanel max screen width
const int QTB_TOUCHPANEL_WIDTH_SUPPORT_MAX = 3840;

// touchpanel max screen height
const int QTB_TOUCHPANEL_HEIGHT_SUPPORT_MAX = 2160;

//---------------------------------------------------------------------------
// for others
//---------------------------------------------------------------------------
// max path length
const int QTB_MAXPATHLEN = 2048;

} // end of namespace qtbrynhildr

#endif // PARAMETERS_H
