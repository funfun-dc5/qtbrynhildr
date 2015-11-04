// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// desktop_scaling_dialog.h

#ifndef DESKTOP_SCALING_DIALOG_H
#define DESKTOP_SCALING_DIALOG_H
// Qt Header
#include <QDialog>
#include <QFont>
#include <QResizeEvent>

// Local Header
#include "settings.h"
#include "ui_desktop_scaling_dialog.h"

namespace qtbrynhildr {

// DesktopScalingDialog
class DesktopScalingDialog : public QDialog, public Ui::DesktopScalingDialog
{
  Q_OBJECT

public:
  // constructor
  DesktopScalingDialog(Settings *settings, QWidget *parent = 0);

  // resize event
  void resizeEvent(QResizeEvent *event);

  // set scale slider position
  void setSliderPositionFromSetting();

protected:
  // show Event
  void showEvent(QShowEvent *event);

private:
  // set setting from scale slider position
  void setSettingFromSliderPosition();

  // resettings for dialog
  void resetting();

private slots:
  // scale slider
  void on_horizontalSlider_valueChanged();

  // reset scale
  void on_resetButton_clicked();

  // accept button
  void accept();
  // reject button
  void reject();

private:
  // settings
  Settings *settings;

  // original scaling factor
  qreal originalFactor;

  // output log flag
  bool outputLog;

public:
  // FACTOR for slider
  constexpr static const qreal SLIDER_FACTOR = 10.0;

private:
  // MINIMUM for slider
  static const int SLIDER_MINIMUM = 1;
  // MAXIMUM for slider
  static const int SLIDER_MAXIMUM = 20;
};

} // end of namespace qtbrynhildr

#endif // DESKTOP_SCALING_DIALOG_H
