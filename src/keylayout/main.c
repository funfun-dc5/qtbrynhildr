// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header
#include <stdio.h>
#include <string.h>

// Local Header
#include "klfcompiler.h"

char *platform[ID_PLATFORM_NUM] = {
  "Windows",
  "Linux",
  "macOS"
};

// for TEST
int main(int argc, char *argv[])
{
  int ret = 0;
  char *infile = NULL;
  char outfile[4096];

  /* openning message */
  printf("Key Layout File Compiler (KLFC) Ver.0.0.1\n");
  printf("Platform: %s\n", platform[THIS_PLATFORM]);

  if (argc == 2){
	int result = 1;

	infile = argv[1];
	strncpy(outfile, infile, 4096);
	strcat(outfile,"x");

	result = make_KLX(infile, outfile);
	if (result != 0){
	  // error
	  printf("error : .kl error!\n");
	  ret = 1;
	}
	else {
	  printf("output : %s\n", outfile);
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
	if (result != 0){
	  // error
	  printf("error : .kl error!\n");
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