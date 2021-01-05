// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef VERSION_H
#define VERSION_H

// version
#define QTB_VERSION				"2.2.7"

#define QTB_RCNAME				""
#define QTB_VERSION_NUMBER		2207

#if __x86_64__ || __LP64__ || _WIN64
#define QTB_ARCHNAME			" (64bit)"
#else // __x86_64__ || __LP64__ || _WIN64
#define QTB_ARCHNAME			" (32bit)"
#endif // __x86_64__ || __LP64__ || _WIN64

// year
#define QTB_YEAR		"2015-2021"

// URL of blog
#define QTB_BLOG		"<a href=\"https://mcz-xoxo.cocolog-nifty.com/blog/\">https://mcz-xoxo.cocolog-nifty.com/blog/</a>"

// URL of manual
#define QTB_MANUAL		"<a href=\"https://mcz-xoxo.net/qtbrynhildr/docs/manual/html/\">https://mcz-xoxo.net/qtbrynhildr/docs/manual/html/</a>"

// URL of release page in GitHub
#define QTB_URL_FOR_RELEASE		"https://github.com/funfun-dc5/qtbrynhildr/releases"

// STRINGS for tag search
#define QTB_STRING_FOR_TAGSEARCH	"/funfun-dc5/qtbrynhildr/releases/tag/v"

#endif // VERSION_H
