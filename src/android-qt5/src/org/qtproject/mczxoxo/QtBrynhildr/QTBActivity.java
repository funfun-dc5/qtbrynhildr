// -*- mode: java; coding: utf-8-unix -*-
// Copyright (c) 2021- FunFun <fu.aba.dc5@gmail.com>

package org.qtproject.mczxoxo.QtBrynhildr;

import android.util.Log;
import android.view.KeyEvent;

import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.bindings.QtApplication;

public class QTBActivity extends QtActivity
{
  public QTBActivity()
  {
	Log.d("QTB", "Activity has created.");
  }

  private static native void nativeKeyDown(int value);
  private static native void nativeKeyUp(int value);

  @Override
  public boolean onKeyDown(int keyCode, KeyEvent event)
  {
	// remapping keyCode
	Log.v("QTB keyCode :", Integer.toString(keyCode));
	if (keyCode == 211){
	  nativeKeyDown(keyCode);
	}
	return super.onKeyDown(keyCode, event);
  }

  @Override
  public boolean onKeyUp(int keyCode, KeyEvent event)
  {
	// remapping keyCode
	Log.v("QTB keyCode :", Integer.toString(keyCode));
	if (keyCode == 211) {
	  nativeKeyUp(keyCode);
	}
	return super.onKeyUp(keyCode, event);
  }
}
