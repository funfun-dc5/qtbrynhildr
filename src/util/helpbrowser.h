// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2021- FunFun <fu.aba.dc5@gmail.com>

#ifndef HELPBROWSER_H
#define HELPBROWSER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>

// Local Header

// html path
#define HTML_PATH	"/../docs/manual/html"

namespace qtbrynhildr {

class HelpBrowser : public QWidget
{
  Q_OBJECT;

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  HelpBrowser(const QString &path, const QString &page, QWidget *parent = nullptr);

  // destructor
  virtual ~HelpBrowser();

  // show page
  static void showPage(const QString &page);
private slots:
  void updateWindowTitle();

private:
  QTextBrowser	*textBrowser;
  QPushButton	*homeButton;
  QPushButton	*backButton;
  QPushButton	*closeButton;
};

} // end of namespace qtbrynhildr

#endif // HELPBROWSER_H
