// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef VERSION_H
#define VERSION_H

// version
#define QTB_VERSION				"2.0.25"
#define QTB_RCNAME				"-benchmark"
#define QTB_VERSION_NUMBER		2025

#if __x86_64__ || __LP64__ || _WIN64
#define QTB_ARCHNAME			" (64bit)"
#else // __x86_64__ || __LP64__ || _WIN64
#define QTB_ARCHNAME			" (32bit)"
#endif // __x86_64__ || __LP64__ || _WIN64

// year
#define QTB_YEAR		"2015-2019"

// URL of blog
#define QTB_BLOG		"<a href=\"http://mcz-xoxo.cocolog-nifty.com/blog/\">http://mcz-xoxo.cocolog-nifty.com/blog/</a>"

// URL of release page in GitHub
#define QTB_URL_FOR_RELEASE		"https://github.com/funfun-dc5/qtbrynhildr/releases"

// STRINGS for tag search
#define QTB_STRING_FOR_TAGSEARCH	"/funfun-dc5/qtbrynhildr/releases/tag/v"

#endif // VERSION_H
