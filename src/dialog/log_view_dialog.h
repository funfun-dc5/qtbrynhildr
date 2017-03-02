// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef LOG_VIEW_DIALOG_H
#define LOG_VIEW_DIALOG_H
// Qt Header
#include <QAbstractButton>
#include <QDialog>
#include <QResizeEvent>

// Local Header
#include "settings.h"
#include "ui_log_view_dialog.h"

namespace qtbrynhildr {

// LogViewDialog
class LogViewDialog : public QDialog, public Ui::LogViewDialog
{
  Q_OBJECT

public:
  // constructor
  LogViewDialog(Settings *settings, QWidget *parent = 0);

  // resize event
  void resizeEvent(QResizeEvent *event);

  // set plain text
  void setPlainText(const QString &text);

  // clear
  void clear();

  // show
  void show();

private:
  // resettings for dialog
  void resetting();

private slots:
  // reset
  void on_buttonBox_clicked(QAbstractButton *button);

private:
  // top position of log
  qint64 topPos;

  // last position of log
  qint64 lastPos;

  // settings
  Settings *settings;
};

} // end of namespace qtbrynhildr

#endif // LOG_VIEW_DIALOG_H
