// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef PREFERENCE_DIALOG_H
#define PREFERENCE_DIALOG_H
// Qt Header
#include <QDialog>
#include <QFont>
#include <QResizeEvent>

// Local Header
#include "settings.h"
#include "ui_preference_dialog.h"

namespace qtbrynhildr {

// PreferenceDialog
class PreferenceDialog : public QDialog, public Ui::PreferenceDialog
{
  Q_OBJECT

public:
  // constructor
  PreferenceDialog(Settings *settings, QWidget *parent = 0);

  // resize event
  void resizeEvent(QResizeEvent *event);

protected:
  // show Event
  void showEvent(QShowEvent *event);

private:
  // resettings for dialog
  void resetting();

private slots:

  // accept button
  void accept();
  // reject button
  void reject();

private:
  // settings
  Settings *settings;

  // output log flag
  bool outputLog;

public:

private:
};

} // end of namespace qtbrynhildr


#endif // PREFERENCE_DIALOG_H
