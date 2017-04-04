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
  printf("Found Section (Section No. = %d)\n", $1);
#endif /* DEBUG_YACC */
}
| STRING '=' QSTRING {
#ifdef DEBUG_YACC
  printf("ENVVAR = QSTRING (%s = %s)\n", $1, $3);
#endif /* DEBUG_YACC */
}
| STRING '=' NUMBER {
#ifdef DEBUG_YACC
  printf("ENVVAR = NUMBER (%s = %d)\n", $1, $3);
#endif /* DEBUG_YACC */
}
| KEY_ID ',' VK_ID ',' SHIFTKEY {
#ifdef DEBUG_YACC
  printf("KEY_ID , VK_ID, SHIFTKEY           : %-25s , %-25s, SHIFTKEY(%d)\n", $1, $3, $5);
#endif /* DEBUG_YACC */
}
| KEY_ID ',' VK_ID ',' SHIFTKEY ',' PLATFORM {
#ifdef DEBUG_YACC
  printf("KEY_ID , VK_ID, SHIFTKEY, PLATFORM : %-25s , %-25s, SHIFTKEY(%d), PLATFORM(%d)\n", $1, $3, $5, $7);
#endif /* DEBUG_YACC */
}
| NUMBER ',' QSTRING ',' QSTRING ',' VK_ID ',' QSTRING ',' VK_ID {
#ifdef DEBUG_YACC
  printf("NUMBER, QSTRING, QSTRING, VK_ID, QSTRING, VK_ID : ");
  printf("%2d, %-10s, %-10s, %-25s, %-10s, %s\n", $1, $3, $5, $7, $9, $11);
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
