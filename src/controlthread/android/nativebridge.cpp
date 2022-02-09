// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QDebug>

// Local Header
#include "nativebridge.h"

namespace qtbrynhildr {

// native method keyDownFromJava
static void keyDownFromJava(JNIEnv *env, jobject thiz, jint value)
{
  Q_UNUSED(env);
  Q_UNUSED(thiz);

  qDebug() << "keyDownFromJava : " << value;
}

// native method keyUpFromJava
static void keyUpFromJava(JNIEnv *env, jobject thiz, jint value)
{
  Q_UNUSED(env);
  Q_UNUSED(thiz);

  qDebug() << "keyUpFromJava : " << value;
}

// register native methods
void registerNativeMethods()
{
  JNINativeMethod methods[] = {
	{"nativeKeyDown", "(I)V", reinterpret_cast<void *>(keyDownFromJava)},
	{"nativeKeyUp", "(I)V", reinterpret_cast<void *>(keyUpFromJava)}
  };

  QAndroidJniObject javaClass("org/qtproject/mczxoxo/QtBrynhildr/QTBActivity");
  QAndroidJniEnvironment env;
  jclass objectClass = env->GetObjectClass(javaClass.object<jobject>());
  env->RegisterNatives(objectClass,
					   methods,
					   sizeof(methods) / sizeof(methods[0]));
  env->DeleteLocalRef(objectClass);
}

} // end of namespace qtbrynhildr
