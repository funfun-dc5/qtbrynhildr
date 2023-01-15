// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2021- FunFun <fu.aba.dc5@gmail.com>

#ifndef URL_H
#define URL_H

// URL of blog
#define QTB_BLOG		"<a href=\"https://mcz-xoxo.cocolog-nifty.com/blog/\">https://mcz-xoxo.cocolog-nifty.com/blog/</a>"

// URL of manual
#if defined(QTB_DEV_DESKTOP)
#define QTB_MANUAL		"<a href=\"https://mcz-xoxo.net/qtbrynhildr/docs/manual/html/\">Ja</a>"
#else // defined(QTB_DEV_DESKTOP)
#define QTB_MANUAL		"<a href=\"https://mcz-xoxo.net/qtbrynhildr/docs/manual/html/\">https://mcz-xoxo.net/qtbrynhildr/docs/manual/html/</a>"
#endif // defined(QTB_DEV_DESKTOP)

// URL of privacy pollicy
#if defined(QTB_DEV_DESKTOP)
#define QTB_PRIVACY_POLICY		"<a href=\"https://mcz-xoxo.net/qtbrynhildr/docs/privacy/English.html\">En</a>"
#else // defined(QTB_DEV_DESKTOP)
#define QTB_PRIVACY_POLICY		"<a href=\"https://mcz-xoxo.net/qtbrynhildr/docs/privacy/English.html\">https://mcz-xoxo.net/qtbrynhildr/docs/privacy/English.html</a>"
#endif // defined(QTB_DEV_DESKTOP)

// URL of release page in GitHub
#define QTB_URL_FOR_RELEASE		"https://github.com/funfun-dc5/qtbrynhildr/releases"

// STRINGS for tag search
#define QTB_STRING_FOR_TAGSEARCH	"/funfun-dc5/qtbrynhildr/releases/tag/v"

#endif // URL_H
