// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOFTWARE_BUTTON_H
#define SOFTWARE_BUTTON_H
// System Header

// Qt Header
#include <QDateTime>
#include <QPaintEvent>
#include <QRect>
#include <QResizeEvent>
#include <QWidget>

// Local Header

namespace qtbrynhildr {

// SoftwareButton
class SoftwareButton : public QWidget
{
public:
  // constructor
  SoftwareButton(QWidget *parent = 0);
  // destructor
  //  ~SoftwareButton();

protected:
  // paint event
  void paintEvent(QPaintEvent *event) override;

  // resize event
  void resizeEvent(QResizeEvent *event) override;

#if 0
  // minimum size hint
  QSize minimumSizeHint() const override;
#endif

  // size hint
  QSize sizeHint() const override;

  // mouse event
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

protected:
  // button id
  typedef enum {
	ID_BUTTON_0,
	ID_BUTTON_1,
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
	ID_BUTTON_NUM
  } ID_BUTTON;

protected:
  // pressed button
  virtual void pressedButton(ID_BUTTON id);

  // released button
  virtual void releasedButton(ID_BUTTON id);

private:
  // calclate layout
  void calclateLayout(qreal xFactor, qreal yFactor);

  // get ID
  ID_BUTTON getID(QPoint pos);

  // toggle option button
  void toggleOptionButton();

  // toggle monitor button
  void toggleShowMonitorButton();

  // toggle sound button
  void toggleShowSoundButton();

  // toggle sound quality button
  void toggleShowSoundQualityButton();

  // toggle video quality button
  void toggleShowVideoQualityButton();

protected:
  // button top information
  typedef struct {
	QString buttonTop;
	bool visible;
  } ButtonTopInfo;

  // key top information table
  ButtonTopInfo buttonTopInfo[ID_BUTTON_NUM] = {
	{"", 				false},  	// DUMMY

	{"Option",			true},  	// ID_BUTTON_1
	{"Monitor",			false},		// ID_BUTTON_2
	{"1",				false},		// ID_BUTTON_3
	{"2",				false},		// ID_BUTTON_4
	{"3",				false},	 	// ID_BUTTON_5
	{"4",				false},		// ID_BUTTON_6
	{"5",				false},		// ID_BUTTON_7
	{"6",				false},		// ID_BUTTON_8
	{"7",				false},		// ID_BUTTON_9
	{"8",				false},		// ID_BUTTON_10
	{"9",				false},		// ID_BUTTON_11

	{"Info",			true},	 	// ID_BUTTON_12
	{"Sound",			false},	 	// ID_BUTTON_13
	{"OFF",				false},	 	// ID_BUTTON_14
	{"ON",				false},	 	// ID_BUTTON_15

	{"Wheel+",			true},	 	// ID_BUTTON_16
	{"Sound Quality",	false},		// ID_BUTTON_17
	{"Lowest",			false},		// ID_BUTTON_18
	{"Low",				false},		// ID_BUTTON_19
	{"Standard",		false},		// ID_BUTTON_20
	{"High",			false},		// ID_BUTTON_21
	{"Highest",			false}, 	// ID_BUTTON_22

	{"Wheel-",			true},		// ID_BUTTON_23
	{"Video Quality",	false},		// ID_BUTTON_24
	{"Lowest",			false},	 	// ID_BUTTON_25
	{"Low",				false},	 	// ID_BUTTON_26
	{"Standard",		false},		// ID_BUTTON_27
	{"High",			false},	 	// ID_BUTTON_28
	{"Highest",			false}, 	// ID_BUTTON_29

	{"Right",			true},	 	// ID_BUTTON_30
	{"Left",			true}	 	// ID_BUTTON_31
  };

private:
  // original size for layout
  static const int WIDTH = 198;
  static const int HEIGHT = 75;

  // button size
  QSize buttonSize;

  // layout table for button
  const QRect buttonLayout[ID_BUTTON_NUM] = {
	//       x,  y,  w,  h 
	QRect(  0,  0,  0,  0), // dummy entry

	// 1st row
	QRect(180,  0, 18, 15),
	QRect(162,  0, 18, 15),
	QRect(144,  0, 18, 15),
	QRect(126,  0, 18, 15),
	QRect(108,  0, 18, 15),
	QRect( 90,  0, 18, 15),
	QRect( 72,  0, 18, 15),
	QRect( 54,  0, 18, 15),
	QRect( 36,  0, 18, 15),
	QRect( 18,  0, 18, 15),
	QRect(  0,  0, 18, 15),

	// 2nd row
	QRect(180, 15, 18, 15),
	QRect(162, 15, 18, 15),
	QRect(144, 15, 18, 15),
	QRect(126, 15, 18, 15),

	// 3rd row
	QRect(180, 30, 18, 15),
	QRect(162, 30, 18, 15),
	QRect(144, 30, 18, 15),
	QRect(126, 30, 18, 15),
	QRect(108, 30, 18, 15),
	QRect( 90, 30, 18, 15),
	QRect( 72, 30, 18, 15),

	// 4th row
	QRect(180, 45, 18, 15),
	QRect(162, 45, 18, 15),
	QRect(144, 45, 18, 15),
	QRect(126, 45, 18, 15),
	QRect(108, 45, 18, 15),
	QRect( 90, 45, 18, 15),
	QRect( 72, 45, 18, 15),

	// 5th row
	QRect(180, 60, 18, 15),
	QRect(162, 60, 18, 15),
  };

  // layout table for button
  typedef struct {
	QRect rect;
	bool pushed;
  } Layout;
  Layout layout[ID_BUTTON_NUM];

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

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // BUTTON_H
