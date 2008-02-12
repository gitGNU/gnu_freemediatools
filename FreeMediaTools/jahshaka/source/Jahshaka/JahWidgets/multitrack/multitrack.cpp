#include "multitrack.h"

#include <iostream>
using namespace std;
#include <qpainter.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qrect.h>
#include <qstyle.h>

#include <set>
#include <cstdlib>

// local
#include <valueFormatters.h>
#include <jahpreferences.h>

// that's actually the nominal amount at scaling()==1.0
static const int ticksInSlider = 100;

static const int sliderMarkerW = 11;
static const int sliderMarkerH = 7;
static const int sliderGutterHeight = 25; 
static const int sliderStrut = 8;
static const int PADDING = 3;

struct MultiTrackTrack::MultiTrackTrackPrivate
{
    MultiTrackTrackPrivate()
        : mt( NULL ),
          x( 0 ),
          y( 0 ), 
		  m_controls( show_enabled | show_silence | show_locked ),
          m_pixmapPath( QString( JahPrefs::getInstance().getBasePath().data() ) + "Pixmaps/widgets/" )
    {
		setLocked( false );
		setSilence( false );
		setEnabled( true );
    }

    bool locked() const 
	{ 
		return m_locked; 
	}

    void setLocked( bool l ) 
	{ 
		m_locked = l; 
        lockButtonIcon = QPixmap( m_pixmapPath + ( m_locked ? "lock.png" : "unlock.png" ) );
	}

    void setSilence( bool l ) 
	{ 
		m_silence = l; 
        silenceButtonIcon = QPixmap( m_pixmapPath + ( m_silence ? "no-silence.png" : "silence.png" ) );
	}

    void setEnabled( bool l ) 
	{ 
		m_enabled = l; 
        enableButtonIcon = QPixmap( m_pixmapPath + ( m_enabled ? "no-silence.png" : "silence.png" ) );
	}

	void setControls( int controls )
	{
		m_controls = controls;
		if ( mt ) mt->update( );
	}

	int controls( ) const
	{
		return m_controls;
	}

    MultiTrack *mt;
    QValueList<MultiTrackItem*> items;
    QString text;
    QPixmap pixmap;

    QRect lockButton;
    QPixmap lockButtonIcon;

    QRect silenceButton;
    QPixmap silenceButtonIcon;

    QRect enableButton;
    QPixmap enableButtonIcon;

    int x;
    int y;

private:
	int m_controls;
    bool m_locked;
    bool m_silence;
    bool m_enabled;
    QString m_pixmapPath;
};

MultiTrackTrack::MultiTrackTrack(MultiTrack *mt)
{
    d = new MultiTrackTrackPrivate;
    d->mt = mt;
    mt->addTrack(this);
}

MultiTrackTrack::MultiTrackTrack(const QString &text, MultiTrack *mt)
{
    d = new MultiTrackTrackPrivate;
    d->mt = mt;
    d->text = text;
    mt->addTrack(this);
}

MultiTrackTrack::~MultiTrackTrack()
{
    clear( );
    d->mt->removeTrack(this);
    delete d;
}

QString MultiTrackTrack::text() const
{
    return d->text;
}

void MultiTrackTrack::setText(const QString &text)
{
    d->text = text;
}

QPixmap MultiTrackTrack::pixmap() const
{
    return d->text;
}

void MultiTrackTrack::setPixmap(const QPixmap &pm)
{
    d->pixmap = pm;
}


MultiTrack *MultiTrackTrack::multiTrack()
{
    return d->mt;
}

MultiTrackItem *MultiTrackTrack::firstItem()
{
    if (d->items.begin() == d->items.end()) return 0;
    return *d->items.begin();
}


int MultiTrackTrack::length() const
{
    return 5 * ( PADDING + 18 );
}

int MultiTrackTrack::height() const
{
    // return a guess
    // height of text, height of a 16x16 pixmap, 3*padding
    return 12 + 18 + 3*PADDING;
}

void MultiTrackTrack::paintLabel(QPainter *p, QColorGroup cg, int width)
{
    p->save();
    if (multiTrack()->isSelected(this))
        cg.setColor( QColorGroup::Button, cg.button().dark( 150 ) );

    p->setBrush( cg.button() );
    
    p->setPen( QPen( cg.shadow(), 1 ) );
    p->drawRect(0, 0, width, height());
    p->restore();
    
    // place the text in the top left corner
    QFontMetrics fm( p->fontMetrics() );
    QRect textBounding = fm.boundingRect( d->text );
    p->drawText( PADDING, PADDING + textBounding.height(), d->text );
    
    // add three "buttons"
    const QSize buttonSize( 18, 18 );
	int controls = d->controls( );
    d->enableButton = QRect( PADDING, height() - PADDING - buttonSize.height(), buttonSize.width(), buttonSize.height() );
    d->silenceButton = QRect( 2 * PADDING + buttonSize.width(), height() - PADDING - buttonSize.height(), buttonSize.width(), buttonSize.height() );
    d->lockButton = QRect( width - PADDING - buttonSize.width(), height() - PADDING - buttonSize.height(), buttonSize.width(), buttonSize.height() );

    QStyle& style = QApplication::style();

	if ( ( controls & show_enabled ) != 0 )
	{
    	style.drawPrimitive( QStyle::PE_ButtonCommand, p, d->enableButton, cg, QStyle::Style_Raised );
    	p->drawPixmap( d->enableButton.topLeft() + QPoint( 1, 1 ), d->enableButtonIcon );
	}

	if ( ( controls & show_silence ) != 0 )
	{
    	style.drawPrimitive( QStyle::PE_ButtonCommand, p, d->silenceButton, cg, QStyle::Style_Raised );
    	p->drawPixmap( d->silenceButton.topLeft() + QPoint( 1, 1 ), d->silenceButtonIcon );
	}

	if ( ( controls & show_locked ) != 0 )
	{
    	style.drawPrimitive( QStyle::PE_ButtonCommand, p, d->lockButton, cg, QStyle::Style_Raised );
    	p->drawPixmap( d->lockButton.topLeft() + QPoint( 1, 1 ), d->lockButtonIcon );
	}
}

void MultiTrackTrack::paintContents(QPainter *pt, const QColorGroup &cg, int _width, bool even )
{
    QColor troughColour = even ? cg.button() : cg.button().light( 110 ) ;
    
    pt->fillRect(0, 0, _width, height(), troughColour );
    
    QValueList<MultiTrackItem*>::Iterator i;
    for ( i = d->items.begin(); i != d->items.end(); ++i )
    {
        MultiTrackItem *item = *i;
        const bool selected = multiTrack()->isSelected(item);
        if ( !selected )
        {
            const int time = item->time();
            const int width = multiTrack()->pixelWidth(item->length());
            pt->save();
            pt->translate(double(multiTrack()->pixelWidth(time)), 0);
            item->paint(pt, cg, width, height());
            pt->restore();
        }
    }
    
    for ( i = d->items.begin(); i != d->items.end(); ++i )
    {
        MultiTrackItem *item = *i;
        const bool selected = multiTrack()->isSelected(item);
        if ( selected )
        {
            const bool canMove = item->isMovable();
            const int time = item->time();
            const int dx = canMove ? -multiTrack()->selectedDelta() : 0;
            const int width = multiTrack()->pixelWidth(item->length());
            pt->save();
            pt->translate(double(multiTrack()->pixelWidth(time) + dx), 0);
            item->paint(pt, cg, width, height());
            pt->restore();
        }
    }
}

void MultiTrackTrack::clear()
{
    while (1)
    {
        QValueList<MultiTrackItem*>::Iterator i(d->items.begin());
        if (i == d->items.end())
            break;
        delete *i;
    }
}

void MultiTrackTrack::setPosition( int x, int y )
{
    d->x = x;
    d->y = y;
}

bool MultiTrackTrack::locked() const
{
    return d->locked();
}

void MultiTrackTrack::setControls( int controls )
{
	d->setControls( controls );
}

void MultiTrackTrack::setLocked( bool on )
{
	d->setLocked( on );
}

void MultiTrackTrack::setEnabled( bool on )
{
	d->setEnabled( on );
}

void MultiTrackTrack::setSilence( bool on )
{
	d->setSilence( on );
}

void MultiTrackTrack::mousePressEvent( QMouseEvent* e )
{
    QRect lockButton = d->lockButton;
    lockButton.moveBy( d->x, d->y );
    if ( lockButton.contains( e->pos() ) )
        d->mt->emitLockedClicked( this );

    QRect enableButton = d->enableButton;
    enableButton.moveBy( d->x, d->y );
    if ( enableButton.contains( e->pos() ) )
        d->mt->emitEnableClicked( this );

    QRect silenceButton = d->silenceButton;
    silenceButton.moveBy( d->x, d->y );
    if ( silenceButton.contains( e->pos() ) )
        d->mt->emitSilenceClicked( this );
}

void MultiTrackTrack::mouseReleaseEvent( QMouseEvent* )
{
}

QValueList<MultiTrackItem*>::Iterator MultiTrackTrack::addItem(MultiTrackItem *i)
{
    return d->items.append(i);
}

void MultiTrackTrack::removeItem(QValueList<MultiTrackItem*>::Iterator i)
{
    d->items.remove(i);
}



struct MultiTrackItem::MultiTrackItemPrivate
{
    int time;
    int length;
    MultiTrackTrack *track;
    QValueList<MultiTrackItem*>::Iterator pos;
    QString text;
    QPixmap pixmap;
    
    inline MultiTrackItemPrivate()
	{
            time = 0;
            length = 100;
	}
};

MultiTrackItem::MultiTrackItem(MultiTrackTrack *t)
{
    d = new MultiTrackItemPrivate;
    d->track = t;
    d->pos = t->addItem(this);
}

MultiTrackItem::MultiTrackItem(const QString &text, MultiTrackTrack *t)
{
    d = new MultiTrackItemPrivate;
    d->track = t;
    d->pos = t->addItem(this);
    d->text = text;
}


MultiTrackItem::~MultiTrackItem()
{
    d->track->removeItem(d->pos);
    delete d;
}

QString MultiTrackItem::text() const
{
    return d->text;
}

void MultiTrackItem::setText(const QString &text)
{
    d->text = text;
}

QPixmap MultiTrackItem::pixmap() const
{
    return d->text;
}

void MultiTrackItem::setPixmap(const QPixmap &pm)
{
    d->pixmap = pm;
}


void MultiTrackItem::setLength(int length)
{
    d->length = length;
}

int MultiTrackItem::length() const
{
    return d->length;
}

void MultiTrackItem::setTime(int x)
{
    d->time = x;
}

int MultiTrackItem::time() const
{
    return d->time;
}

MultiTrackItem *MultiTrackItem::nextItem()
{
    QValueList<MultiTrackItem*>::Iterator p = d->pos;
    ++p;
    if (p == d->track->d->items.end())
        return 0;
    return *p;
}

bool MultiTrackItem::isMovable() const
{
    return true;
}



void MultiTrackItem::paint(QPainter *p, const QColorGroup &cg, int width, int height)
{
    if ( d->text != "" )
    {
        p->save();
        QColor itemColor( 81, 100, 109 );
        const bool selected = d->track->multiTrack()->isSelected(this);
        if (selected)
            p->setBrush( itemColor.dark(150) );
        else
            p->setBrush( itemColor );
        
        p->setPen( QPen( itemColor.light(), 1 ) );
        p->drawRect(0, 0, width, height);

        // draw small handles in the top corners
        const unsigned int HANDLE_SIZE = 8;
        p->setBrush( itemColor.light() );
        p->drawRect( 0, 0, HANDLE_SIZE, HANDLE_SIZE );
        p->drawRect( width - HANDLE_SIZE, 0, HANDLE_SIZE, HANDLE_SIZE );

        paintContents(p, cg, width, height);

        p->restore();        
    }
}

void MultiTrackItem::paintContents(QPainter *p, const QColorGroup &, int width, int height )
{
    // place the text in the lower left corner
    // truncate if the text will not fit into the available space
    const QString ELLIPSIS = "...";
    QFontMetrics fm( p->fontMetrics() );
    int w = fm.width( d->text );

    const int MAX_WIDTH = width - 2*PADDING;
    if ( w > MAX_WIDTH )
    {
        QString truncated = d->text;
        for ( int i= d->text.length() - ELLIPSIS.length() - 1; i > 1; --i )
        {
            truncated = d->text.left( i ) + ELLIPSIS;
            if ( fm.width( truncated ) < MAX_WIDTH )
            {
                break;
            }
        }
        
        d->text = truncated;
    }

    p->drawText( PADDING, height - PADDING, d->text );
}





struct MultiTrack::MultiTrackPrivate
{
    QScrollBar *scrollH, *scrollV;
    QWidget *corner;
    QValueList<MultiTrackTrack*> tracks;
    
    std::set<MultiTrackItem*> selectedItems;
    
    // this may be invalid, so you can never return it
    MultiTrackTrack *selectedTrack;
    
    QPoint clickedAt;
    int selectedDelta;
    int position;
    int length;
    int offset;
    
    bool showPosition:1;
    double scaling;

    int fps;
    bool drop;
    
    enum
    {
        Nothing,
        Item,
        Slider,
        Unknown 
        
    } clickedOn;

    inline MultiTrackPrivate()
	{
            selectedTrack = 0;
            showPosition = true;
            selectedDelta = 0;
            position = 0;
            length = 1;
            offset = 0;
            scaling = 1.0;
            fps = 30;
            drop = true;
            clickedOn = Nothing;
	}
};

MultiTrack::MultiTrack(QWidget *parent, const char *name)
    : QWidget(parent, name, WNoAutoErase)
{
    d = new MultiTrackPrivate;
    
    QGridLayout *gl = new QGridLayout(this, 2, 2);
    
    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Ignored);
    gl->addItem(spacer, 0, 1);
    
    d->scrollH = new QScrollBar(Horizontal, this);
    d->scrollH->hide();
    gl->addWidget(d->scrollH, 1, 1);
    
    
    d->scrollV = new QScrollBar(Vertical, this);
    connect(d->scrollH, SIGNAL(valueChanged(int)), SLOT(scrolled()));
    connect(d->scrollV, SIGNAL(valueChanged(int)), SLOT(scrolled()));
    gl->addWidget(d->scrollV, 0, 0);
    
    // the corner widget
    d->corner = new QWidget(this);
    d->corner->hide();
    gl->addWidget(d->corner, 1, 0);
    
    setScaling(1.0);

    // setup the font explicitly
    setFont( font() );

	setAcceptDrops( true );
}

MultiTrack::~MultiTrack()
{
    clear();
    delete d;
}

void MultiTrack::dragEnterEvent( QDragEnterEvent *e )
{
	emit multitrackDragEnterEvent( e );
}

void MultiTrack::dropEvent( QDropEvent *e )
{
	emit multitrackDropEvent( e );
}

void MultiTrack::setLength(int len)
{
    d->length = len;
}

int MultiTrack::length() const
{
    return d->length;
}

void MultiTrack::setOffset( const int t )
{
    d->offset = t;
}

int MultiTrack::getOffset( ) const
{
    return d->offset;
}

void MultiTrack::paintEvent(QPaintEvent *)
{
    const int trackLabelWidth = this->trackLabelWidth();
    
    QColorGroup cg = colorGroup();
    
    QPixmap pm(size());
    pm.fill(cg.base());
    
    QPainter pt(&pm, this);
    
    const int verticalBegin = -d->scrollV->value();
    
    int ypos = verticalBegin;
    
    if (showPosition())
        ypos += sliderGutterHeight;

    const int xOffset = d->scrollV->width();
    const int xDelta = timeToPixel( 0 );
    
    // draw tracks
    unsigned int count = 0;
    for (
        QValueList<MultiTrackTrack*>::Iterator i(d->tracks.begin());
        i != d->tracks.end();
        ++i, ++count
        )
    {
        MultiTrackTrack *t = *i;
        
        const int h = t->height();
        
        pt.save();
        pt.translate(double(xDelta), double(ypos));
        pt.setClipRect(xDelta, ypos, pixelWidth(length()), h);
        t->paintContents(&pt, cg, pixelWidth(length()) - 1, count % 2 );
        pt.restore();
        
        ypos += h; // + 2;
    }
    
    ypos = verticalBegin;
    
    // draw time indicator
    if (showPosition())
    {
        drawTimeLine( &pt );
        pt.save();
        pt.translate( 0, double(ypos) );
        if (d->clickedOn == MultiTrackPrivate::Slider)
        {
            QColor hl = colorGroup().highlight();
            pt.setPen(QPen(hl, 0, SolidLine));
            pt.setBrush(hl);
        }
        else
        {
            pt.setBrush(colorGroup().foreground());
        }
	
        const int at = timeToPixel(position());
        QPointArray array(3);
        array.putPoints(
            0, 3,
            at - sliderMarkerW/2, 0,
            at + sliderMarkerW/2, 0,
            at, sliderMarkerH
            );
        
        pt.drawPolygon(array);
        pt.drawLine(at, sliderMarkerH, at, height());
	
        pt.restore();
        ypos += sliderGutterHeight;
    }
    

    // draw label
    for (
        QValueList<MultiTrackTrack*>::Iterator j(d->tracks.begin());
        j != d->tracks.end();
        ++j
        )
    {
        MultiTrackTrack *t = *j;
        
        const int h = t->height();
        
        pt.save();
        pt.translate( xOffset, double(ypos));
        pt.setClipRect( xOffset, ypos, trackLabelWidth, h);
        t->setPosition( xOffset, ypos );
        t->paintLabel(&pt, cg, trackLabelWidth);
        pt.restore();
        
        ypos += h;
    }

    // draw border
    pt.save();
    pt.setPen( QPen( cg.shadow(), 1 ) );
    pt.setBrush( QBrush::NoBrush );
    pt.drawRect( 0, 0, width(), height() );
    pt.restore();

    
    pt.end();
    
    bitBlt(this, QPoint(0,0), &pm, QRect(QPoint(0,0), pm.size()));
}

static int findNiceInterval( int s, int fps )
{
    // align to s
    while ( s % fps )
    {
        ++s;
    }
    // then, ensure 5s is the minimum gap
    while ( ( s / fps ) % 5 )
    {
        ++s;
    }

    return s;
}

void MultiTrack::drawTimeLine( QPainter* p )
{
    // split the timeline vertically, draw timecodes in the lower half
    // time (frames) ranges from:
    // - scrolledTime() to 
    // - pixelToTime( width() )
    TimecodeValueFormatter fmt( d->fps, d->drop );

    QFontMetrics fm( p->fontMetrics() );

    // find the minimum spacing: two timecodes
    QRect boundingRect = fm.boundingRect( fmt.convert( 0 ) );
    int spacing = 2 * pixelToTime( timeToPixel( 0 ) + boundingRect.width() );

    // round up to next nice value
    spacing = std::max( spacing, d->fps );
    spacing = findNiceInterval( spacing, d->fps );

    // draw
    p->save();
    p->setBrush( colorGroup().shadow() );
    p->setPen( QPen::NoPen );
    int timecodeBackgroundHeight = boundingRect.height() + 2*PADDING;
    QRect lower( 0, sliderGutterHeight - timecodeBackgroundHeight, width(), timecodeBackgroundHeight );
    p->drawRect( lower );
    
    int t = findNiceInterval( scrolledTime(), d->fps );
    int end = pixelToTime( width() );

    p->setPen( colorGroup().button() );
    while ( t < end )
    {
        p->drawText( timeToPixel( t ), sliderGutterHeight - PADDING, fmt.convert( t ) );
        t += spacing;
    }
    
    p->restore();
}

void MultiTrack::resizeEvent(QResizeEvent *)
{
    setupVScrollbar();
}


void MultiTrack::mouseMoveEvent(QMouseEvent *e)
{
    if (e->pos().x() < contentsBegin()) return;

    MultiTrackTrack* track = trackAt( e->pos() );
	if ( track && track->locked( ) ) return;

    if (d->clickedOn == MultiTrackPrivate::Item || d->clickedOn == MultiTrackPrivate::Unknown )
    {
        d->clickedOn = MultiTrackPrivate::Item;
        const int dx = d->clickedAt.x() - e->pos().x();
        d->selectedDelta = dx;
        update();
    }
    else if (d->clickedOn == MultiTrackPrivate::Slider)
    {
        int t = pixelToTime( e->pos().x() - d->scrollV->width() );
        t = t < 0 ? 0 : t >= d->length ? d->length - 1 : t;
        d->position = t;
        update();
        emit positionChanged(t);
    }
}

void MultiTrack::mousePressEvent(QMouseEvent *e)
{
	d->clickedOn = MultiTrackPrivate::Nothing;

    if (e->pos().x() < contentsBegin())
	{
    	MultiTrackTrack* track = trackAt( e->pos() );
		if ( track )
    		track->mousePressEvent( e );
		if ( trackAt( e->pos() ) )
        	emit trackSelected( e->state( ), trackAt( e->pos() ) );
		return;
	}

    MultiTrackItem* item = itemAt( e->pos() );
    d->clickedAt = e->pos();

    if (e->button() == RightButton)
    {
        if (item)
            emit rightClicked(item, e->pos());
    }
    else if (showPosition() && e->pos().y() < sliderGutterHeight)
    {
        const int at = timeToPixel( position() );
        
        // ok, the slider was clicked on for to drag
        if (abs(at - e->pos().x()) < sliderStrut)
        {
            d->clickedOn = MultiTrackPrivate::Slider;
            update();
        }
    }
    else if (item)
    {
       	const int t = pixelToTime(e->pos().x());
       	emit positionChanged( t );

       	emit itemClicked(itemAt( e->pos() ), t);
       	emit itemSelectEvent( e );

       	d->clickedOn = MultiTrackPrivate::Unknown;

        if ( trackAt( e->pos() ) != d->selectedTrack )
            emit trackSelected( e->state( ), trackAt( e->pos() ) );
    }
}

void MultiTrack::mouseReleaseEvent(QMouseEvent *e)
{
    d->selectedDelta = 0;
    const int dx = d->clickedAt.x() - e->pos().x();
    
    QValueList<MultiTrackItem*> items;
    
    for (
        std::set<MultiTrackItem*>::iterator i(d->selectedItems.begin());
        i != d->selectedItems.end();
        ++i
        )
    {
        MultiTrackItem *item = *i;
        if (!isSelected(item)) continue;
        items.append(item);
	
        const int t = pixelToTime(timeToPixel(item->time()) - dx);
        
        if ( d->clickedOn == MultiTrackPrivate::Item )
        {
            emit itemMoved(item, t);
            emit itemMoved(e->state(), item, t);
        }
    }
    
    const int delta = timeWidth(dx);
    
    if ( d->clickedOn == MultiTrackPrivate::Item )
    {
        emit itemsMoved(items, delta);
        emit itemsMoved(e->state(), items, delta);
        emit itemsMoved(delta);
        emit itemsMoved(e->state(), delta);
    }
    
    d->clickedOn = MultiTrackPrivate::Nothing;
    
    update();
}

void MultiTrack::wheelEvent(QWheelEvent *e)
{
    QWheelEvent w(QPoint(0,0), d->scrollH->mapToGlobal(QPoint(0,0)), e->delta(), e->state());
    QApplication::sendEvent(d->scrollH, &w);
}

void MultiTrack::deselectAll()
{
    d->selectedItems.clear();
    update();
}

void MultiTrack::clear()
{
    while (1)
    {
        QValueList<MultiTrackTrack*>::Iterator i(d->tracks.begin());
        if (i == d->tracks.end())
            break;
        delete *i;
    }
}

void MultiTrack::setFPS( int fps, bool drop )
{
    d->fps = fps;
    d->drop = drop;
}

void MultiTrack::setPosition(int pos)
{
    seek(pos);
    if (!isTimeVisible(pos))
        centerOnTime(pos);
}

void MultiTrack::seek(int pos)
{
    if (d->position == pos)
        return;
    if (d->clickedOn == MultiTrackPrivate::Slider)
        return;
    
    d->position = pos;
    
    update();
}

void MultiTrack::setScaling(double sc)
{
    if ( sc > 1.0 )
        d->scrollH->show();
    else
        d->scrollH->hide();
    
    // now, check if I fit vertically
    int cheight = contentsHeight();
    if (d->scrollH->isVisible())
        cheight += d->scrollH->height();
    
    if (d->scrollH->isVisible() )
        d->corner->show();
    else
        d->corner->hide();
    
    int at = pixelToTime(contentsBegin());
    d->scaling = sc;
    if (d->scrollH->isVisible()) // don't print warnings with negative nums
        d->scrollH->setRange(0, int(ticksInSlider*scaling() - ticksInSlider));
    else
        d->scrollH->setRange(0, 0);
    
    d->scrollH->setValue(int(at*ticksInSlider*scaling()/length()));
    
    update();
}

double MultiTrack::scaling() const
{
    return d->scaling;
}

bool MultiTrack::isTimeVisible(int t) const
{
    return (t >= scrolledTime() && t < pixelToTime(width()));
}

void MultiTrack::ensureTimeVisible(int t)
{
    if (scrolledTime() > t)
    { // scroll so that t is at the LHS
        double v = t*scaling()*ticksInSlider/length();
        d->scrollH->setValue(int(v-1)); // QScrollBar clamps
    }
    else if (scrolledTime() <= pixelToTime(width()))
    { // scroll so that t is at the RHS
        int pix = t - timeWidth(contentsWidth());
        
        double v = pix*scaling()*ticksInSlider/length();
        d->scrollH->setValue(int(v)+1);
    }
    // (I might also not scroll at all!)
}

void MultiTrack::centerOnTime(int t)
{
    int pix = t - timeWidth(contentsWidth()/2);
    
    double v = pix*scaling()*ticksInSlider/length();
    d->scrollH->setValue(int(v)+1);
}

void MultiTrack::setSelected(MultiTrackItem *item, bool sel)
{
    if (sel)
        d->selectedItems.insert(item);
    else
        d->selectedItems.erase(item);
    update();
}

void MultiTrack::setSelected(MultiTrackTrack *track, bool sel)
{
    if (sel)
        d->selectedTrack = track;
    else
        d->selectedTrack = 0;
    update();
}

MultiTrackTrack *MultiTrack::selectedTrack( )
{
	return d->selectedTrack;
}

bool MultiTrack::isSelected(const MultiTrackItem *item) const
{
    std::set<MultiTrackItem*>::iterator i =
        d->selectedItems.find(const_cast<MultiTrackItem*>(item));
    return i != d->selectedItems.end();
}

bool MultiTrack::isSelected(const MultiTrackTrack *track) const
{
    return d->selectedTrack == track;
}

void MultiTrack::setShowPosition(bool show)
{
    d->showPosition = show;
    update();
}

bool MultiTrack::showPosition() const
{
    return d->showPosition;
}

int MultiTrack::timeToPixel(int t) const
{
    int px = int(((t - scrolledTime()) * contentsWidth() )  * scaling() / length() ) + contentsBegin();
    return px;
}

int MultiTrack::pixelToTime(int px) const
{
    double t = (px - contentsBegin()) * length() / scaling() / contentsWidth() + scrolledTime();
    return int(t);
}

int MultiTrack::pixelWidth(int t) const
{
    return int( t * contentsWidth() * scaling() / length() );
}

int MultiTrack::timeWidth(int p) const
{
    double t = p * length() / scaling() / contentsWidth();
    return int( t );
}


int MultiTrack::position() const
{
    return d->position;
}


MultiTrackTrack *MultiTrack::trackAt(const QPoint &pt)
{
    int ypos = 0;
    if (showPosition())
        ypos += sliderGutterHeight;
    
    for (
        QValueList<MultiTrackTrack*>::Iterator i(d->tracks.begin());
        i != d->tracks.end();
        ++i
        )
    {
        MultiTrackTrack *t = *i;
        
        const int h = t->height();
        
        if (ypos <= pt.y() && ypos+h >= pt.y())
            return t;
        
        ypos += h + 2;
    }
    return 0;
}

MultiTrackItem *MultiTrack::itemAt(const QPoint &pt)
{
    MultiTrackTrack *t = trackAt(pt);
    if (!t) return 0;
    
    const int xDelta = -scrolledTime();
    
    for (MultiTrackItem *i = t->firstItem(); i; i = i->nextItem())
    {
        const int time = i->time() + xDelta;
        
        const int x = timeToPixel(time);
        const int width = pixelWidth(i->length());
        
        if (x <= pt.x() && x+width >= pt.x())
            return i;
    }
    return 0;
}

int MultiTrack::trackLabelWidth() const
{
    int w=0;
    for (
        QValueList<MultiTrackTrack*>::Iterator i(d->tracks.begin());
        i != d->tracks.end();
        ++i
        )
    {
        MultiTrackTrack *t = *i;
        w = QMAX(w, t->length());
    }
    
    return w;
}

int MultiTrack::contentsWidth() const
{
    return width() - contentsBegin();
}


void MultiTrack::addTrack(MultiTrackTrack *mt)
{
    d->tracks.append(mt);
    setupVScrollbar();
}

void MultiTrack::removeTrack(MultiTrackTrack *mt)
{
    d->tracks.remove(mt);
    setupVScrollbar();
}

int MultiTrack::selectedDelta() const
{
    return d->selectedDelta;
}

int MultiTrack::contentsBegin() const
{ 
    return d->scrollV->width() + trackLabelWidth() + 1; 
}

int MultiTrack::scrolledTime() const
{
    double t = d->scrollH->value() * length()/double(ticksInSlider) /scaling();
    return int(t);
}

int MultiTrack::contentsHeight() const
{ // probably should cache this thing's result
    
    int ypos = -2; // count the gap at the end
    
    if (showPosition())
        ypos += sliderGutterHeight;
    
    for (
        QValueList<MultiTrackTrack*>::ConstIterator i(d->tracks.begin());
        i != d->tracks.end();
        ++i
        )
    {
        MultiTrackTrack *t = *i;
        const int h = t->height();
        ypos += h + 2;
    }
    return ypos;
}

void MultiTrack::setupVScrollbar()
{
    int max = contentsHeight()-height();
    if (max < 0) max = 0;
    d->scrollV->setRange(0, max);
    setScaling(scaling());
}

void MultiTrack::scrolled()
{
    update();
}

void MultiTrack::setFont( const QFont& fn_ )
{
    // make the text slightly smaller and italic
    QFont fn( fn_ );
    fn.setPointSize( fn.pointSize() - 1 );
    fn.setItalic( true );
    QWidget::setFont( fn );
}

void MultiTrack::emitSilenceClicked( MultiTrackTrack* t )
{
    emit silenceClicked( t );
}

void MultiTrack::emitEnableClicked( MultiTrackTrack* t )
{
    emit enableClicked( t );
}

void MultiTrack::emitLockedClicked( MultiTrackTrack* t )
{
    emit lockedClicked( t );
}

#ifdef TESTING
#include "multitrack.moc"
#endif
