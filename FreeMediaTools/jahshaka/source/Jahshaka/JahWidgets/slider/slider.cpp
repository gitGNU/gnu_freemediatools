#include "slider.h"

#include <qpixmap.h>
#include <qpainter.h>
#include <qdrawutil.h>
#include <stdlib.h>

#include <string>

static const int inOutMarkerW = 4;
static const int sliderMarkerW = 11;

static const int sliderMarkerH = 7;
static const int sliderStrut = 8;


struct TimeSlider::TimeSliderPrivate
{
	int in, out, length, position;
	
	enum
	{
		Nothing, In, Out, Position
	} clickedOn;
	
	
	inline TimeSliderPrivate()
	{
		clickedOn = Nothing;
	}
};


TimeSlider::TimeSlider(QWidget *parent, const char *name)
	: QFrame(parent, name, WRepaintNoErase)
{
	d = new TimeSliderPrivate;
}


TimeSlider::~TimeSlider()
{
	delete d;
}

int TimeSlider::in() const
{
	return d->in;
}

int TimeSlider::out() const
{
	return d->out;
}

int TimeSlider::length() const
{
	return d->length;
}

int TimeSlider::position() const
{
	return d->position;
}

void TimeSlider::realSetIn(int frames)
{

#if ( defined _MSC_VER ) && ( _MSC_VER <= 1200 ) // devstudio6
	frames = std::_cpp_max(0, frames);
	frames = std::_cpp_min(length(), frames);
#else
	frames = std::max(0, frames);
	frames = std::min(length(), frames);
#endif

	if (d->in == frames) return;
	d->in = frames;
	emit inChanged(frames);
	update();
}

void TimeSlider::realSetOut(int frames)
{
#if ( defined _MSC_VER ) && ( _MSC_VER <= 1200 ) // devstudio6
	frames = std::_cpp_max(0, frames);
	frames = std::_cpp_min(length(), frames);
#else
	frames = std::max(0, frames);
	frames = std::min(length(), frames);
#endif

	if (d->out == frames) return;
	d->out = frames;
	emit outChanged(frames);
	update();
}

void TimeSlider::realSetPosition(int position)
{
#if ( defined _MSC_VER ) && ( _MSC_VER <= 1200 ) // devstudio6
	position = std::_cpp_max(0, position);
	position = std::_cpp_min(length(), position);
#else
	position = std::max(0, position);
	position = std::min(length(), position);
#endif

	if (d->position == position) return;
	d->position = position;
	emit positionChanged(position);
	update();
}

void TimeSlider::setIn(int frames)
{
	if (d->clickedOn == TimeSliderPrivate::In)
		return;
	realSetIn(frames);
}

void TimeSlider::setOut(int frames)
{
	if (d->clickedOn == TimeSliderPrivate::Out)
		return;
	realSetOut(frames);
}

void TimeSlider::setLength(int frames)
{
	if (d->length == frames) return;
	d->length = frames;
	emit lengthChanged(frames);
	update();
}

void TimeSlider::setPosition(int position)
{
	if (d->clickedOn == TimeSliderPrivate::Position)
		return;

	realSetPosition(position);
}

void TimeSlider::setPositionVisible(bool)
{

}

int TimeSlider::valueToPosition(int v) const
{
	int edge, offset;
	if (inOutMarkerW*2 > sliderMarkerW)
	{
		edge = inOutMarkerW*2;
		offset = inOutMarkerW;
	}
	else
	{
		edge = sliderMarkerW;
		offset = sliderMarkerW/2;
	}
	
	const int w = width() - edge;
	
	return v*w / length() + offset;
}

int TimeSlider::positionToValue(int p) const
{
	int edge, offset;
	if (inOutMarkerW*2 > sliderMarkerW)
	{
		edge = inOutMarkerW*2;
		offset = inOutMarkerW;
	}
	else
	{
		edge = sliderMarkerW;
		offset = sliderMarkerW/2;
	}
	
	const int w = width() - edge;
	
	return (p-offset) *length() / w;
}

void TimeSlider::paintEvent(QPaintEvent *)
{
	QPixmap pixmap(size());
	QPainter p(&pixmap, this);
	
	p.fillRect(rect(), colorGroup().base());

	qDrawShadeLine(&p,
			inOutMarkerW, height()/2,
			width()-inOutMarkerW, height()/2,
			colorGroup(), true, 1, 1
		);

	QPointArray array(3);
	
	
	{ // position
		if (d->clickedOn == TimeSliderPrivate::Position)
		{
			QColor hl = colorGroup().highlight();
			p.setPen(QPen(hl, 0, SolidLine));
			p.setBrush(hl);
		}
		else
		{
			p.setBrush(colorGroup().foreground());
		}
	
		const int at = valueToPosition(position());
		array.putPoints(
				0, 3,
				at - sliderMarkerW/2, 0,
				at + sliderMarkerW/2, 0,
				at, sliderMarkerH
			);
		
		p.drawPolygon(array);
		p.drawLine(at, sliderMarkerH, at, height());
	}
	
	
	{ // in
		if (d->clickedOn == TimeSliderPrivate::In)
		{
			QColor hl = colorGroup().highlight();
			p.setPen(QPen(hl));
			p.setBrush(hl);
		}
		else
		{
			p.setPen(QPen(colorGroup().foreground()));
			p.setBrush(colorGroup().foreground());
		}
		
		const int at = valueToPosition(in());
		array.putPoints(
				0, 3,
				at, 0,
				at + inOutMarkerW, 0,
				at, sliderMarkerH
			);
		
		p.drawPolygon(array);
		QPen pen = p.pen();
		pen.setStyle(DotLine);
		p.setPen(pen);
		p.drawLine(at, sliderMarkerH, at, height());
		pen.setStyle(SolidLine);
		p.setPen(pen);
	}
	
	{ // out
		if (d->clickedOn == TimeSliderPrivate::Out)
		{
			QColor hl = colorGroup().highlight();
			p.setPen(hl);
			p.setBrush(hl);
		}
		else
		{
			p.setPen(QPen(colorGroup().foreground()));
			p.setBrush(colorGroup().foreground());
		}
		const int at = valueToPosition(out());
		array.putPoints(
				0, 3,
				at-inOutMarkerW, 0,
				at, 0,
				at, sliderMarkerH
			);
		
		p.drawPolygon(array);
		QPen pen = p.pen();
		pen.setStyle(DotLine);
		p.setPen(pen);
		p.drawLine(at, sliderMarkerH, at, height());
		pen.setStyle(SolidLine);
		p.setPen(pen);
	}
	
	p.end();
	
	bitBlt(this, 0, 0, &pixmap);
}

void TimeSlider::mouseReleaseEvent(QMouseEvent *)
{
	d->clickedOn = TimeSliderPrivate::Nothing;
	update();
}

void TimeSlider::mousePressEvent(QMouseEvent *e)
{
	int pos = e->pos().x();
	
	int din = abs(valueToPosition(in()) - pos);
	int dout = abs(valueToPosition(out()) - pos);
	int dpos = abs(valueToPosition(position()) - pos);
	
	d->clickedOn = TimeSliderPrivate::Nothing;
	
	if (dpos <= dout && dpos <= din)
	{
		if (dpos < sliderStrut)
			d->clickedOn = TimeSliderPrivate::Position;
	}
	else if (din <= dout && din <= dpos)
	{
		if (din < sliderStrut)
			d->clickedOn = TimeSliderPrivate::In;
	}
	else if (dout <= din && dout <= dpos)
	{
		if (dout < sliderStrut)
			d->clickedOn = TimeSliderPrivate::Out;
	}
	update();
}

void TimeSlider::mouseMoveEvent(QMouseEvent *e)
{
	int pos = e->pos().x();
	int v = positionToValue(pos);
	
	switch (d->clickedOn)
	{
	case TimeSliderPrivate::In:
#if ( defined _MSC_VER ) && ( _MSC_VER <= 1200 ) // devstudio6
		v = std::_cpp_min(v, out()-1);
#else
		v = std::min(v, out()-1);
#endif
		realSetIn(v);
		break;
	case TimeSliderPrivate::Out:
#if ( defined _MSC_VER ) && ( _MSC_VER <= 1200 ) // devstudio6
		v = std::_cpp_max(v, in()+1);
#else
		v = std::max(v, in()+1);
#endif
		realSetOut(v);
		break;
	case TimeSliderPrivate::Position:
		realSetPosition(v);
		break;
	default:
		;
	}
	
}

