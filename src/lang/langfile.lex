/* -*- mode: c; coding: utf-8-unix -*-
// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
for c   : flex -o langfile.c langfile.lex ; gcc -o langfile langfile.c
for c++ : flex -+ -o langfile.cpp langfile.lex ; g++ -o langfile -I. langfile.cpp
*/

%{
#ifdef __FLEX_LEXER_H
#include <cstdio>
#else // __FLEX_LEXER_H
#include <stdio.h>
#endif // __FLEX_LEXER_H
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
}
{key_id}	{
  printf("key_id   = %s\n", yytext);
}
{vk_id}		{
  printf("vk_id    = %s\n", yytext);
}
{shiftkey}	{
  printf("shiftkey = %s\n", yytext);
}
{platform}	{
  printf("platform = %s\n", yytext);
}
{string}	{
  printf("string   = %s\n", yytext);
}
{number}	{
  printf("number   = %s\n", yytext);
}

%%

#ifdef __FLEX_LEXER_H
/* for C++ */
int yyFlexLexer::yywrap()
{
  return 1;
}

int main(int argc, char *argv[])
{
  yyFlexLexer lexer;
  lexer.yylex();
}
#else // __FLEX_LEXER_H
/* for C */
int yywrap()
{
  return 1;
}

int main(int argc, char *argv[])
{
  yylex();
}
#endif // __FLEX_LEXER_H
