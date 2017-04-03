/* -*- mode: c; coding: utf-8-unix -*- */
/*
// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>
//
*/
%{
%}

%union {
  char *strp;
  int intval;
}

%token STRING
%token QSTRING
%token NUMBER
%token SECTION
%token KEY_ID
%token VK_ID
%token SHIFTKEY
%token PLATFORM

%%

input:	/* */
		| input line
		;

line:	'\n'
		;

%%

/* for TEST */
int main(int argc, char *argv[])
{
  yyparse();
}

#include <stdio.h>

int yyerror(char *s)
{
  printf("%s\n", s);
}
