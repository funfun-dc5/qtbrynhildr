// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <stdio.h>
#include <string.h>

// Local Header
#include "klfcompiler.h"

// string for platform
char *platform[ID_PLATFORM_NUM] = {
  "Windows",
  "Linux",
  "macOS"
};

// version
const char *version = "0.0.3";

// for TEST
int main(int argc, char *argv[])
{
  int ret = 0;
  char *infile = NULL;
  char outfile[4096];

  /* openning message */
  printf("Key Layout File Compiler (KLFC) Ver.%s\n", version);
  printf("Platform: %s\n", platform[THIS_PLATFORM]);

  if (argc == 2){
	int result;

	infile = argv[1];
	strncpy(outfile, infile, 4096);
	strcat(outfile,"x");

	result = make_KLX(infile, outfile);
	if (result < 0){
	  // file error
	  printf("error : file error!\n");
	  ret = 1;
	}
	else if (result > 0){
	  // found error
	  printf("error : found %d error!\n", result);
	  ret = 1;
	}
	else { // result == 0
	  // Not found error
	  printf("output : %s\n", outfile);
	  printf("found %d error.\n", result);
	}
  }
  else if (argc > 2){
	// error
	printf("error : too many arguments!\n");
	ret = 1;
  }
  else {
#ifdef DEBUG_YACC
	int result = make_KLX(NULL, NULL); // stdin
	if (result < 0){
	  // file error
	  printf("error : file error!\n");
	  ret = 1;
	}
	else if (result > 0){
	  // found error
	  printf("error : found %d error!\n", result);
	  ret = 1;
	}
#else /* DEBUG_YACC */
	// error
	printf("error : need a argument(.kl)\n");
	ret = 1;
#endif /* DEBUG_YACC */
  }

  printf("Done.\n");

  return ret;
}
