// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

#ifndef NATIVEBRIDGE_H
#define NATIVEBRIDGE_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>

// Local Header


namespace qtbrynhildr {

// native method keyDownFromJava
static void keyDownFromJava(JNIEnv *env, jobject thiz, jint value);

// native method keyUpFromJava
static void keyUpFromJava(JNIEnv *env, jobject thiz, jint value);

// register native methods
void registerNativeMethods();

} // end of namespace qtbrynhildr

#endif // NATIVEBRIDGE_H
