
#ifndef SLICKSLIDER_H
#define SLICKSLIDER_H

#include <qslider.h>
#include <qpixmap.h>

/* A new slider with progress for movie playing etc.
*/

class SlickSlider : public QFrame
{
Q_OBJECT

public:
    SlickSlider( QWidget* parent = 0, const char* name = 0, int value = 0  );

    void setDrawSolidBlock( bool set ) { drawsolidblock = set; };
    void setDrawSunken( bool set ) { drawsunken = set; };
    void setDrawDarkStyle( bool set ) { drawdarkstyle = set ;};
    void setDrawFancyBg( bool set ) { drawfancybg = set ;};
    void setRange ( int minval, int maxval );
    void setMinValue ( int val );
    void setMaxValue ( int val );
    int minValue();
    int maxValue();
	int value( );

	virtual QSize sizeHint() const { return QSize( 150, 26 ); }

public slots:
    void setValue ( int val );

signals:
	void sliderPressed( );
	void sliderReleased( );
	void sliderMoved( int );
	void valueChanged( int );

protected:
	virtual void mousePressEvent( QMouseEvent *e );
	virtual void mouseReleaseEvent( QMouseEvent *e );
	virtual void mouseDoubleClickEvent( QMouseEvent *e );
	virtual void mouseMoveEvent( QMouseEvent *e );
	virtual void paintEvent ( QPaintEvent * );
    virtual void resizeEvent( QResizeEvent *);

private:
	bool dragging;
    QColor uppercol;
    QColor lowercol;
    QColor customBarColor;
    bool drawsolidblock;
    bool drawsunken;
    bool drawsunken2;
    bool drawdarkstyle;
    bool drawfancybg;
    bool drawtriangles;
    bool usecustombarcolor;
    QColor basestore;
	QRangeControl m_range;
    void genBackground();
    bool mustGenerateBackground;
    QPixmap bg;
};

#endif
