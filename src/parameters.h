// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// parameters.h

#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// for MainWindow
//---------------------------------------------------------------------------
#if defined(Q_OS_WIN)
const int QTB_MAINWINDOW_WIDTH_CORRECT = 2;
const int QTB_MAINWINDOW_HEIGHT_CORRECT = 5;
#elif defined(Q_OS_LINUX)
const int QTB_MAINWINDOW_WIDTH_CORRECT = 2;
const int QTB_MAINWINDOW_HEIGHT_CORRECT = 2;
#endif // defined(Q_OS_LINUX)

//---------------------------------------------------------------------------
// for keyboard & mouse
//---------------------------------------------------------------------------
// keyboard buffer size (entry)
const int QTB_KEYBOARD_BUFFER_SIZE = 256;

// mouse buffer size (entry)
const int QTB_MOUSE_BUFFER_SIZE	= 256;

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

} // end of namespace qtbrynhildr

#endif // PARAMETERS_H
