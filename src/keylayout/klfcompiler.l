/* -*- mode: c; coding: utf-8-unix -*- */
/*
// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>
//
// for c   : flex -o keylayoutfile.c keylayoutfile.l ; gcc -o keylayoutfile keylayoutfile.c
// for c++ : flex -+ -o keylayoutfile.cpp keylayoutfile.l ; g++ -o keylayoutfile -I. keylayoutfile.cpp
*/

%{

#ifdef _MSC_VER
#include "common/msvc.h"
#endif // _MSC_VER

#ifdef __FLEX_LEXER_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#else // __FLEX_LEXER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif // __FLEX_LEXER_H

#include "klfcompiler.h"
#include "klfcompiler.tab.h"

#include "keylayoutfile.h"


int lineno = 1;

%}

ws			[ \t]+
comment		#.*
nl			\n
section		\[[a-zA-Z]+\]
key_id		Key_[a-zA-Z0-9_]+
vk_id		VK_[a-zA-Z0-9_]+
shiftkey	SHIFTKEY_THROUGH|SHIFTKEY_NEED|SHIFTKEY_NONEED
platform	Windows|macOS|Linux
string		[a-zA-Z_]+
number		[0-9]+
comma		,
equal		=
%x qstring

%%

  char string_buf[1024];
  char *string_buf_ptr = NULL;

\"	{
  string_buf_ptr = string_buf;
  BEGIN(qstring);
}

<qstring>\"	{
  BEGIN(INITIAL);
  *string_buf_ptr = '\0';
#ifdef DEBUG_LEX
  printf("qstring  = \"%s\"\n", string_buf);
#endif /* DEBUG_LEX */
  if (strlen(string_buf) > MAX_STRING){
	printf("error : qstring > %d \n", MAX_STRING);
  }
  yylval.strp = strdup(string_buf);
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
#ifdef DEBUG_LEX
  printf("comment  = %s\n", yytext);
#endif /* DEBUG_LEX */
}
{nl}		{
  lineno++;
  return '\n';
}
{comma}		{
  return ',';
}
{equal}		{
  return '=';
}

\[General\] {
#ifdef DEBUG_LEX
  printf("section  = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = ID_SECTION_GENERAL;
  return SECTION;
}
\[Keys\] {
#ifdef DEBUG_LEX
  printf("section  = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = ID_SECTION_KEYS;
  return SECTION;
}
\[SoftKeys\] {
#ifdef DEBUG_LEX
  printf("section  = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = ID_SECTION_SOFTKEYS;
  return SECTION;
}
{key_id}	{
#ifdef DEBUG_LEX
  printf("key_id   = %s\n", yytext);
#endif /* DEBUG_LEX */
  if (strlen(yytext) > MAX_STRING){
	printf("error : Key_* > %d \n", MAX_STRING);
  }
  yylval.strp = strdup(yytext);
  return KEY_ID;
}
{vk_id}		{
#ifdef DEBUG_LEX
  printf("vk_id    = %s\n", yytext);
#endif /* DEBUG_LEX */
  if (strlen(yytext) > MAX_STRING){
	printf("error : VK_* > %d \n", MAX_STRING);
  }
  yylval.strp = strdup(yytext);
  return VK_ID;
}
SHIFTKEY_THROUGH {
  yylval.intval = ID_SHIFTKEY_THROUGH;
  return SHIFTKEY;
}
SHIFTKEY_NEED {
#ifdef DEBUG_LEX
  printf("shiftkey = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = ID_SHIFTKEY_NEED;
  return SHIFTKEY;
}
SHIFTKEY_NONEED {
#ifdef DEBUG_LEX
  printf("shiftkey = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = ID_SHIFTKEY_NONEED;
  return SHIFTKEY;
}
Windows {
#ifdef DEBUG_LEX
  printf("platform = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = ID_PLATFORM_WINDOWS;
  return PLATFORM;
}
macOS {
#ifdef DEBUG_LEX
  printf("platform = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = ID_PLATFORM_MACOS;
  return PLATFORM;
}
Linux {
#ifdef DEBUG_LEX
  printf("platform = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = ID_PLATFORM_LINUX;
  return PLATFORM;
}
{string}	{
#ifdef DEBUG_LEX
  printf("string   = %s\n", yytext);
#endif /* DEBUG_LEX */
  if (strlen(yytext) > MAX_STRING){
	printf("error : string > %d \n", MAX_STRING);
  }
  yylval.strp = strdup(yytext);
  return STRING;
}
{number}	{
#ifdef DEBUG_LEX
  printf("number   = %s\n", yytext);
#endif /* DEBUG_LEX */
  yylval.intval = atoi(yytext);
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
	//	  printf("id = %d\n", id);
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

#if 0
int main(int argc, char *argv[])
{
  while(1){
	int id = yylex();
	//	printf("id = %d\n", id);
	if (id == 0){
	  break;
	}
  }
}
#endif
#endif // __FLEX_LEXER_H
