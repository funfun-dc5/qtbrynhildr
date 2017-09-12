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

  // reset size
  QSize resetSize();

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
	ID_BUTTON_32,
	ID_BUTTON_33,
	ID_BUTTON_34,
	ID_BUTTON_35,
	ID_BUTTON_36,
	ID_BUTTON_37,
	ID_BUTTON_38,
	ID_BUTTON_NUM,
	ID_BUTTON_0 = ID_BUTTON_NUM
  } ID_BUTTON;

protected:
  // pressed button
  virtual void pressedButton(ID_BUTTON id);

  // released button
  virtual void releasedButton(ID_BUTTON id);

private:
  // calculate layout
  void calculateLayout(qreal xFactor, qreal yFactor);

  // get ID
  ID_BUTTON getID(QPoint pos);

  // clear buttons
  void clearButtons();

  // toggle option button
  void toggleOptionButton();

  // toggle sound button
  void toggleShowSoundButton();

  // toggle sound quality button
  void toggleShowSoundQualityButton();

  // toggle video quality button
  void toggleShowVideoQualityButton();

  // toggle sound cache button
  void toggleShowSoundCacheButton();

  // toggle public mode button
  void toggleShowPublicModeButton();

  // toggle video FPS button
  void toggleShowVideoFPSButton();

protected:
  // button top
  typedef struct {
	QString buttonTop;
	bool visible;
  } ButtonTop;

  // key top table
  ButtonTop buttonTopTable[ID_BUTTON_NUM] = {
	// 1st row
	{"Fn",				true},  	// ID_BUTTON_1
	{"Monitor",			false},		// ID_BUTTON_2
	{"Sound",			false},		// ID_BUTTON_3
	{"Off",				false},		// ID_BUTTON_4
	{"On",				false},		// ID_BUTTON_5

	// 2nd row
	{"00.0 fps\n\n00.0 Mbps",true},	// ID_BUTTON_6
	{"Sound\nCache",	false},		// ID_BUTTON_7
	{"1",				false},		// ID_BUTTON_8
	{"2",				false},		// ID_BUTTON_9
	{"3",				false},		// ID_BUTTON_10
	{"4",				false},		// ID_BUTTON_11
	{"5",				false},	 	// ID_BUTTON_12

	// 3rd row
	{"Option",			true},	 	// ID_BUTTON_13
	{"Sound\nQuality",	false},	 	// ID_BUTTON_14
	{"1",				false},	 	// ID_BUTTON_15
	{"2",				false},		// ID_BUTTON_16
	{"3",				false},		// ID_BUTTON_17
	{"4",				false},		// ID_BUTTON_18
	{"5",				false},		// ID_BUTTON_19

	// 4th row
	{"Wheel +",			true},		// ID_BUTTON_20
	{"Video\nCodec",	false},		// ID_BUTTON_21
	{"MJPEG",			false},		// ID_BUTTON_22
	{"Compress",		false},	 	// ID_BUTTON_23

	// 5th row
	{"Wheel -",			true},	 	// ID_BUTTON_24
	{"Video\nQuality",	false},	 	// ID_BUTTON_25
	{"1",				false}, 	// ID_BUTTON_26
	{"2",				false},	 	// ID_BUTTON_27
	{"3",				false},	 	// ID_BUTTON_28
	{"4",				false},	 	// ID_BUTTON_29
	{"5",				false},	 	// ID_BUTTON_30

	// 6th row
	{"Left",			true},	 	// ID_BUTTON_31
	{"Right",			true},	 	// ID_BUTTON_32
	{"Video\nFPS",		false},	 	// ID_BUTTON_33
	{"Minimum",			false},	 	// ID_BUTTON_34
	{"10",				false},	 	// ID_BUTTON_35
	{"30",				false},	 	// ID_BUTTON_36
	{"60",				false},	 	// ID_BUTTON_37
	{"Maximum",			false}	 	// ID_BUTTON_38
  };

private:
  // original size for layout
  static const int WIDTH = 144;
  static const int HEIGHT = 90;

  // button size
  QSize buttonSize;

  // layout table for button
  const QRect buttonLayout[ID_BUTTON_NUM] = {
	// 1st row
	QRect(  0,  0, 18, 15),
	QRect( 18,  0, 18, 15),
	QRect( 36,  0, 18, 15),
	QRect( 54,  0, 18, 15),
	QRect( 72,  0, 18, 15),

	// 2nd row
	QRect(  0, 15, 18, 15),
	QRect( 36, 15, 18, 15),
	QRect( 54, 15, 18, 15),
	QRect( 72, 15, 18, 15),
	QRect( 90, 15, 18, 15),
	QRect(108, 15, 18, 15),
	QRect(126, 15, 18, 15),

	// 3rd row
	QRect(  0, 30, 18, 15),
	QRect( 36, 30, 18, 15),
	QRect( 54, 30, 18, 15),
	QRect( 72, 30, 18, 15),
	QRect( 90, 30, 18, 15),
	QRect(108, 30, 18, 15),
	QRect(126, 30, 18, 15),

	// 4th row
	QRect(  0, 45, 18, 15),
	QRect( 36, 45, 18, 15),
	QRect( 54, 45, 18, 15),
	QRect( 72, 45, 18, 15),

	// 5th row
	QRect(  0, 60, 18, 15),
	QRect( 36, 60, 18, 15),
	QRect( 54, 60, 18, 15),
	QRect( 72, 60, 18, 15),
	QRect( 90, 60, 18, 15),
	QRect(108, 60, 18, 15),
	QRect(126, 60, 18, 15),

	// 6th row
	QRect(  0, 75, 18, 15),
	QRect( 18, 75, 18, 15),
	QRect( 36, 75, 18, 15),
	QRect( 54, 75, 18, 15),
	QRect( 72, 75, 18, 15),
	QRect( 90, 75, 18, 15),
	QRect(108, 75, 18, 15),
	QRect(126, 75, 18, 15)
};

  // layout table for button
  typedef struct {
	QRect rect;
	bool pushed;
	bool selected;
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

  // show sound cache button flag
  bool onShowSoundCacheButton;

  // show public mode button flag
  bool onShowPublicModeButton;

  // show video FPS button flag
  bool onShowVideoFPSButton;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // BUTTON_H
