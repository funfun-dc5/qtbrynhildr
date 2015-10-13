// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// sofware_button.h

#ifndef SOFTWARE_BUTTON_H
#define SOFTWARE_BUTTON_H
// Qt Header
#include <QDateTime>
#include <QResizeEvent>
#include <QSize>
#include <QWidget>

// Local Header
#include "control/mousebuffer.h"
#include "settings.h"
#include "ui_software_button.h"

namespace qtbrynhildr {

// SoftwareButton
class SoftwareButton : public QWidget, public Ui::SoftwareButton
{
  Q_OBJECT

public:
  // constructor
  SoftwareButton(Settings *settings, MouseBuffer *mouseBuffer, QWidget *parent = 0);

  // size hint
  QSize sizeHint() const;

  // resize event
  void resizeEvent(QResizeEvent *event);

private:
  // button id
  typedef enum {
	ID_BUTTON_1 = 1,
	ID_BUTTON_2,
	ID_BUTTON_3,
	ID_BUTTON_4,
	ID_BUTTON_5,
	ID_BUTTON_6,
	ID_BUTTON_7,
	ID_BUTTON_8,
	ID_BUTTON_9,
	ID_BUTTON_10,
	ID_BUTTON_11,
	ID_BUTTON_12,
	ID_BUTTON_13,
	ID_BUTTON_14,
	ID_BUTTON_15,
	ID_BUTTON_16,
	ID_BUTTON_17,
	ID_BUTTON_18,
	ID_BUTTON_19,
	ID_BUTTON_20,
	ID_BUTTON_21,
	ID_BUTTON_22,
	ID_BUTTON_23,
	ID_BUTTON_24,
	ID_BUTTON_25,
	ID_BUTTON_26,
	ID_BUTTON_27,
	ID_BUTTON_28,
	ID_BUTTON_29,
	ID_BUTTON_30,
	ID_BUTTON_31,
	ID_BUTTON_NUM = ID_BUTTON_31
  } ID_BUTTON;
	
  // toggle option button
  void toggleOptionButton();

  // toggle monitor button
  void toggleShowMonitorButton();

  // toggle sound quality button
  void toggleShowSoundQualityButton();

  // toggle video quality button
  void toggleShowVideoQualityButton();

  // toggle sound button
  void toggleShowSoundButton();

  // update size
  void updateSize();

  // pressed key
  void pressedButton(ID_BUTTON id);

  // released key
  void releasedButton(ID_BUTTON id);

private slots:
  // pressed slots
  void pressedButton_1();
  void pressedButton_2();
  void pressedButton_3();
  void pressedButton_4();
  void pressedButton_5();
  void pressedButton_6();
  void pressedButton_7();
  void pressedButton_8();
  void pressedButton_9();
  void pressedButton_10();
  void pressedButton_11();
  void pressedButton_12();
  void pressedButton_13();
  void pressedButton_14();
  void pressedButton_15();
  void pressedButton_16();
  void pressedButton_17();
  void pressedButton_18();
  void pressedButton_19();
  void pressedButton_20();
  void pressedButton_21();
  void pressedButton_22();
  void pressedButton_23();
  void pressedButton_24();
  void pressedButton_25();
  void pressedButton_26();
  void pressedButton_27();
  void pressedButton_28();
  void pressedButton_29();
  void pressedButton_30();
  void pressedButton_31();

  // released slots
  void releasedButton_1();
  void releasedButton_2();
  void releasedButton_3();
  void releasedButton_4();
  void releasedButton_5();
  void releasedButton_6();
  void releasedButton_7();
  void releasedButton_8();
  void releasedButton_9();
  void releasedButton_10();
  void releasedButton_11();
  void releasedButton_12();
  void releasedButton_13();
  void releasedButton_14();
  void releasedButton_15();
  void releasedButton_16();
  void releasedButton_17();
  void releasedButton_18();
  void releasedButton_19();
  void releasedButton_20();
  void releasedButton_21();
  void releasedButton_22();
  void releasedButton_23();
  void releasedButton_24();
  void releasedButton_25();
  void releasedButton_26();
  void releasedButton_27();
  void releasedButton_28();
  void releasedButton_29();
  void releasedButton_30();
  void releasedButton_31();

private:
  // settings
  Settings *settings;

  // mouse buffer
  MouseBuffer *mouseBuffer;

  // option button status flag
  bool onOptionButton;

  // show monitor button flag
  bool onShowMonitorButton;

  // show sound quality button flag
  bool onShowSoundQualityButton;

  // show video quality button flag
  bool onShowVideoQualityButton;

  // show sound button flag
  bool onShowSoundButton;

  // previous click button
  MouseInfoType previousClick;

  // previous click time
  QDateTime previousClickTime;

  // output log flag
  bool outputLog;

signals:
  // refresh menu
  void refreshMenu();
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_BUTTON_H
