// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// confirm_dialog.h

#ifndef CONFIRM_DIALOG_H
#define CONFIRM_DIALOG_H
// Qt Header
#include <QDialog>
#include <QResizeEvent>
#include <QString>

// Local Header
#include "settings.h"
#include "ui_confirm_dialog.h"

namespace qtbrynhildr {

// ConfirmDialog
class ConfirmDialog : public QDialog, public Ui::ConfirmDialog
{
  Q_OBJECT

public:
  // constructor
  ConfirmDialog(QString msg, bool confirmFlag, Settings *settings, QWidget *parent = 0);

  // resize event
  void resizeEvent(QResizeEvent *event);

  // get confirm flag
  bool getConfirmFlag() const;

private:
  // resettings for dialog
  void resetting();

private slots:
  // confirm field
  void on_checkBox_confirm_stateChanged(int state);

private:
  // confirm flag
  bool confirmFlag;

  // settings
  Settings *settings;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // CONFIRM_DIALOG_H
