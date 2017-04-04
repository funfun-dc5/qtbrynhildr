/* -*- mode: c; coding: utf-8-unix -*- */
/*
// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>
//
*/
%{
#define DEBUG_YACC 1
#ifdef DEBUG_YACC
#include <stdio.h>
#endif /* DEBUG_YACC */

int yylex();
int yyerror(char *s);

%}

%union {
  char *strp;
  int intval;
}

%token <strp>	STRING
%token <strp>	QSTRING
%token <intval>	NUMBER
%token <intval>	SECTION
%token <strp>	KEY_ID
%token <strp>	VK_ID
%token <intval>	SHIFTKEY
%token <intval>	PLATFORM

%%

input:	/* */
		| input line
		;

line:	'\n'
| SECTION {
#ifdef DEBUG_YACC
  printf("Found Section\n");
#endif /* DEBUG_YACC */
}
| STRING '=' QSTRING {
#ifdef DEBUG_YACC
  printf("VAR = QSTRING\n");
#endif /* DEBUG_YACC */
}
| STRING '=' NUMBER {
#ifdef DEBUG_YACC
  printf("ENVVAR = NUMBER\n");
#endif /* DEBUG_YACC */
}
| KEY_ID ',' VK_ID ',' SHIFTKEY {
#ifdef DEBUG_YACC
  printf("KEY_ID , VK_ID, SHIFTKEY\n");
#endif /* DEBUG_YACC */
}
| KEY_ID ',' VK_ID ',' SHIFTKEY ',' PLATFORM {
#ifdef DEBUG_YACC
  printf("KEY_ID , VK_ID, SHIFTKEY, PLATFORM\n");
#endif /* DEBUG_YACC */
}
| NUMBER ',' QSTRING ',' QSTRING ',' VK_ID ',' QSTRING ',' VK_ID {
#ifdef DEBUG_YACC
  printf("NUMBER, QSTRING, QSTRING, VK_ID, QSTRING, VK_ID\n");
#endif /* DEBUG_YACC */
}
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
