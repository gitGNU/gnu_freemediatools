
#ifndef GLOWBUTTON_H
#define GLOWBUTTON_H

#include <qpushbutton.h>
#include <qpoint.h>
#include <qpainter.h>
#include <qstyle.h>

//! DESCR. 
/*!
	TODO: step sizes relative to range etc?
*/
class SliderButton : public QPushButton 
{
Q_OBJECT

public:
    SliderButton( QWidget* parent = 0, const char* name = 0, const char* tag = 0, double min = 0.0, double max = 100.0 ,
		double value = 50.0, bool integer = false, bool nolimits = false, bool drawbar = false, bool scalebar = false );

	//used to preset the value
	void setValue (int val);
	void setType(QString val) { theType = val; }
	void setTag(QString val) { theTag = val; }
	void setScaleBar(bool val) { scaleBar = val; };
	void setCentered(bool val)        { centered = val; }
	void setStepSensativity(bool val);

public slots:


	//! We do some extra stuff if the button is clicked to off
	virtual void clickedSlot();

	void setDrawBar(bool val) { drawBar = val; };
	void setMinValue(double val) { min = val; }
	void setMaxValue(double val) { max = val; }
	//void setStepHigh(void)       { step = 10; }
	//void setStepLow(void)        { step = 1; }


signals:
    void	sliderMoved( int value );

protected:

	virtual void paintEvent ( QPaintEvent * );
    void	drawButtonLabel( QPainter * );
	//! We overide the enterEvent and leaveEvent to do some mouse over trickery.
	virtual void enterEvent ( QEvent * );
	virtual void leaveEvent ( QEvent * );

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent ( QMouseEvent*);

	//builds the tag need to add the display option for % etc...
	void buildTag(double val);

	// We store floats, but can act as an integer slider if this is set.
	bool isIntegerSlider_;

	// set this is there are no min/max limits
	bool noLimits_;

	//controls weather we draw everything or we use the underlying button
	bool drawFlat;

	double min;
	double max;
	double value;

	int step;

	double dragvalue;

	bool centered;
	bool dragging;
	bool drawBar;
	bool scaleBar;

	QPoint dragpoint;
	QString theTag;

	QString theType;

	bool sensative;
	
};

#endif
