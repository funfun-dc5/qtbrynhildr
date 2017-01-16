// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// for threads(control/graphics/sound)
//---------------------------------------------------------------------------
// sleep time (milli seconds)
const unsigned long QTB_THREAD_SLEEP_TIME = 5;

//---------------------------------------------------------------------------
// for control
//---------------------------------------------------------------------------
// keyboard buffer size (entry)
const int QTB_KEYBOARD_BUFFER_SIZE = 256;

// mouse buffer size (entry)
const int QTB_MOUSE_BUFFER_SIZE	= 256;

#if QTB_PUBLIC_MODE6_SUPPORT
// local data buffer size for control (byte)
const int QTB_CONTROL_LOCAL_BUFFER_SIZE = 128 * 1024;
#endif // QTB_PUBLIC_MODE6_SUPPORT

//---------------------------------------------------------------------------
// for graphics
//---------------------------------------------------------------------------
// local data buffer size for graphics (byte)
const int QTB_GRAPHICS_LOCAL_BUFFER_SIZE = 1024 * 1024;

//---------------------------------------------------------------------------
// for sound
//---------------------------------------------------------------------------
// local data buffer size for sound (byte)
const int QTB_SOUND_LOCAL_BUFFER_SIZE = 1024 * 1024;

// sound buffer size for QAudioOutput (byte)
const int QTB_AUDIOOUTPUT_SOUND_BUFFER_SIZE = 1024 * 64;

//---------------------------------------------------------------------------
// for others
//---------------------------------------------------------------------------
// max path length
const int QTB_MAXPATHLEN = 2048;

// status refresh count
const int QTB_STATUS_REFRESH_COUNT = 10;

} // end of namespace qtbrynhildr

#endif // PARAMETERS_H
