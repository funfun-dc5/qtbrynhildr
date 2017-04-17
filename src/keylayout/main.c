// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <stdio.h>

// Local Header
#include "klfcompiler.h"

// for TEST
int main(int argc, char *argv[])
{
  int ret = 0;

  if (argc == 2){
	int result = make_KLX(argv[1]);
	if (result != 0){
	  // error
	  printf("error : .kl error!\n");
	  ret = 1;
	}
  }
  else if (argc > 2){
	// error
	printf("error : too many arguments!\n");
	ret = 1;
  }
  else {
	int result = make_KLX(NULL); // stdin
	if (result != 0){
	  // error
	  printf("error : .kl error!\n");
	  ret = 1;
	}
  }

  return ret;
}
