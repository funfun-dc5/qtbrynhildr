/* -*- mode: c; coding: utf-8-unix -*- */
/*
// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>
//
// for c   : flex -o keylayoutfile.c keylayoutfile.l ; gcc -o keylayoutfile keylayoutfile.c
// for c++ : flex -+ -o keylayoutfile.cpp keylayoutfile.l ; g++ -o keylayoutfile -I. keylayoutfile.cpp
*/

%{
#ifdef __FLEX_LEXER_H
#include <cstdio>
#else // __FLEX_LEXER_H
#include <stdio.h>
#endif // __FLEX_LEXER_H

#include "keylayoutfile.tab.h"

%}

ws			[ \t]+
comment		#.*
nl			\n
section		\[[a-zA-Z]+\]
key_id		Key_[a-zA-Z0-9_]+
vk_id		VK_[a-zA-Z0-9_]+
shiftkey	SHIFTKEY_NEED|SHIFTKEY_NONEED|SHIFTKEY_THROUGH
platform	Windows|macOS|Linux
string		[a-zA-Z_]+
number		[0-9]+
comma		,
equal		=
%x qstring

%%

  char string_buf[1024];
  char *string_buf_ptr;

\"	{
  string_buf_ptr = string_buf;
  BEGIN(qstring);
}

<qstring>\"	{
  BEGIN(INITIAL);
  *string_buf_ptr = '\0';
  printf("qstring  = \"%s\"\n", string_buf);
  return QSTRING;
}

<qstring>\\(.|\n) {
  *string_buf_ptr++ = yytext[1];
}

<qstring>[^\\\n\"]+ {
  char *yptr = yytext;

  while (*yptr){
	*string_buf_ptr++ = *yptr++;
  }
}

{ws}		;
{comment}	{
  printf("comment  = %s\n", yytext);
}
{nl}		;
{comma}		;
{equal}		;
{section}	{
  printf("section  = %s\n", yytext);
  return SECTION;
}
{key_id}	{
  printf("key_id   = %s\n", yytext);
  return KEY_ID;
}
{vk_id}		{
  printf("vk_id    = %s\n", yytext);
  return VK_ID;
}
{shiftkey}	{
  printf("shiftkey = %s\n", yytext);
  return SHIFTKEY;
}
{platform}	{
  printf("platform = %s\n", yytext);
  return PLATFORM;
}
{string}	{
  printf("string   = %s\n", yytext);
  return STRING;
}
{number}	{
  printf("number   = %s\n", yytext);
  return NUMBER;
}

%%

/* for TEST */
#ifdef __FLEX_LEXER_H
/* for C++ */
int yyFlexLexer::yywrap()
{
  return 1;
}

int main(int argc, char *argv[])
{
  yyFlexLexer lexer;
  while(true){
	int id = lexer.yylex();
	  printf("id = %d\n", id);
	if (id == 0){
	  break;
	}
  }
}
#else // __FLEX_LEXER_H
/* for C */
int yywrap()
{
  return 1;
}

int main(int argc, char *argv[])
{
  while(1){
	int id = yylex();
	printf("id = %d\n", id);
	if (id == 0){
	  break;
	}
  }
}
#endif // __FLEX_LEXER_H
