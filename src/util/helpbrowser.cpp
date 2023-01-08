// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2021- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QtGlobal>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QUrl>

// Local Header
#include "helpbrowser.h"

namespace qtbrynhildr {


// constructor
HelpBrowser::HelpBrowser(const QString &path, const QString &page, QWidget *parent)
  :QWidget(parent)
  // for DEBUG
  ,outputLog(true)
{
  setAttribute(Qt::WA_DeleteOnClose);
#if QT_VERSION < 0x060000
  setAttribute(Qt::WA_GroupLeader);
#endif // QT_VERSION < 0x060000

  textBrowser = new QTextBrowser;

  homeButton = new QPushButton(tr("&Home"));
  backButton = new QPushButton(tr("&Back"));
  closeButton = new QPushButton(tr("&Close"));
  closeButton->setShortcut(tr("Esc"));

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(homeButton);
  buttonLayout->addWidget(backButton);
  buttonLayout->addStretch();
  buttonLayout->addWidget(closeButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(buttonLayout);
  mainLayout->addWidget(textBrowser);
  setLayout(mainLayout);

  connect(homeButton, SIGNAL(clicked()), textBrowser, SLOT(home()));
  connect(backButton, SIGNAL(clicked()), textBrowser, SLOT(backward()));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(textBrowser, SIGNAL(sourceChanged(const QUrl &)), this, SLOT(updateWindowTitle()));

  textBrowser->setSearchPaths(QStringList() << path << ":/images");
  textBrowser->setSource(page);
}

// destructor
HelpBrowser::~HelpBrowser()
{
}

 // show page
void HelpBrowser::showPage(const QString &page)
{
  QString path = QApplication::applicationDirPath() + HTML_PATH;
  HelpBrowser *browser = new HelpBrowser(path, page);
  browser->resize(1280, 800);
  browser->show();
}

void HelpBrowser::updateWindowTitle()
{
  setWindowTitle(tr("Help: %1").arg(textBrowser->documentTitle()));
}

} // end of namespace qtbrynhildr
