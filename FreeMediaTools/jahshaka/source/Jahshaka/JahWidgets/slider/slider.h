#ifndef TIMESLIDER_H
#define TIMESLIDER_H

#include <qframe.h>


class TimeSlider : public QFrame
{
Q_OBJECT
	struct TimeSliderPrivate;
	TimeSliderPrivate *d;
	
public:
	TimeSlider(QWidget *parent, const char *name=0);
	
	~TimeSlider();

	int in() const;
	int out() const;
	
	int length() const;
	
	int position() const;

public slots:
	void setIn(int frames);
	void setOut(int frames);
	void setLength(int frames);
	void setPosition(int position);
	
	void setPositionVisible(bool visible);

signals:
	void inChanged(int v);
	void outChanged(int v);
	
	void lengthChanged(int l);
	void positionChanged(int pos);

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	
	int valueToPosition(int v) const;
	int positionToValue(int p) const;
	
private:
	void realSetIn(int frames);
	void realSetOut(int frames);
	void realSetPosition(int position);

};


#endif
