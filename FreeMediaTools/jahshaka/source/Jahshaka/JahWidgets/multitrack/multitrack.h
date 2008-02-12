#ifndef MULTITRACK_H
#define MULTITRACK_H

#include <qscrollview.h>

class MultiTrack;
class MultiTrackItem;
class QPainter;

typedef enum 
{
	show_enabled = 0x1,
	show_silence = 0x2,
	show_locked = 0x4
}
track_controls;

class MultiTrackTrack
{
	struct MultiTrackTrackPrivate;
	MultiTrackTrackPrivate *d;
	
	friend class MultiTrackItem;
	
public:
	MultiTrackTrack(MultiTrack *mt);
	MultiTrackTrack(const QString &text, MultiTrack *mt);
	virtual ~MultiTrackTrack();
	
	/**
	 * Or'd flags for dictating which buttons to activate.
	 */
	void setControls( int );

	QString text() const;
	
	/**
	 * @short sets the caption text of this track
	 *
	 * you have to call update() on the widget this is
	 * contained in for the changes to take effect
	 *
	 * @sa text()
	 * @sa setPixmap()
	 **/
	void setText(const QString &text);
	
	/**
	 * @short sets the icon
	 *
	 * Sets the little icon on this item as
	 * drawn by @ref paintLabel
	 *
	 * Can be a null item
	 *
	 * @sa pixmap()
	 * @sa setText()
	 **/
	void setPixmap(const QPixmap &pm);
	
	/**
	 * @returns the item set by @ref setPixmap
	 **/
	QPixmap pixmap() const;
	
	/**
	 * @returns the Widget this track is
	 * associated with
	 **/
	MultiTrack *multiTrack();
	
	MultiTrackItem *firstItem();
	
	int length() const;
	int height() const;
	
	virtual void paintLabel(QPainter *pt, QColorGroup cg, int width);
	virtual void paintContents(QPainter *pt, const QColorGroup &cg, int width, bool event );
	
	/**
	 * removes all items from this track, thus
	 * calling "delete" on them
	 **/
	void clear();

	/** set position of this track; used for handling mouse events **/
	void setPosition( int x, int y );

	/** is this track locked for editing */
	bool locked() const;
	void setLocked( bool );
	void setEnabled( bool );
	void setSilence( bool );

	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	QValueList<MultiTrackItem*>::Iterator addItem(MultiTrackItem *i);
	void removeItem(QValueList<MultiTrackItem*>::Iterator i);
};

class MultiTrackItem
{
	struct MultiTrackItemPrivate;
	MultiTrackItemPrivate *d;
	
public:
	MultiTrackItem(MultiTrackTrack *t);
	MultiTrackItem(const QString &text, MultiTrackTrack *t);
	virtual ~MultiTrackItem();
	
	QString text() const;
	
	/**
	 * @short sets the caption text of this item
	 *
	 * you have to call update() on the widget this is
	 * contained in for the changes to take effect
	 *
	 * @sa text()
	 * @sa setPixmap()
	 **/
	void setText(const QString &text);
	
	/**
	 * @short sets the icon
	 *
	 * Sets the little icon on this item as
	 * drawn by @ref paintContents
	 *
	 * Can be a null item
	 *
	 * @sa pixmap()
	 * @sa setText()
	 **/
	void setPixmap(const QPixmap &pm);
	
	/**
	 * @returns the item set by @ref setPixmap
	 **/
	QPixmap pixmap() const;
	
	void setLength(int len);
	int length() const;
	void setTime(int x);
	
	int time() const;
	
	MultiTrackItem *nextItem();
	
	/**
	 * this function is called to check if this
	 * item can be moved by DnD.
	 *
	 * @returns true by default (to indicate that the
	 * item can be moved)
	 **/
	virtual bool isMovable() const;

public:
	virtual void paint(QPainter *pt, const QColorGroup &cg, int width, int height);
	virtual void paintContents(QPainter *pt, const QColorGroup &cg, int width, int height);
};


class MultiTrack : public QWidget
{
Q_OBJECT
	friend class MultiTrackTrack;
	struct MultiTrackPrivate;
	MultiTrackPrivate *d;
	
public:
	MultiTrack(QWidget *parent, const char *name=0);
	~MultiTrack();
	
	void setLength(int len);
	
	int length() const;
	
	/**
	 * @returns the track at the given point or 0
	 * if there isn't a track there
	 *
	 * (I don't know if this reflects scrolling!)
	 **/
	MultiTrackTrack *trackAt(const QPoint &pt);
	
	/**
	 * @returns the item at the given point or 0
	 * if there isn't an item there
	 *
	 * (I don't know if this reflects scrolling!)
	 **/
	MultiTrackItem *itemAt(const QPoint &pt);
	
	/**
	 * @returns the width of the track labels
	 * (which is the label on the left)
	 *
	 * This is the maximum size of all the
	 * track labels
	 **/
	int trackLabelWidth() const;
	
	/**
	 * @returns the width of the tracks in pixels,
	 * which is the width of the widget without
	 * the width of the track label
	 *
	 * same as QWidget::width() - @ref contentsBegin()
	 **/
	int contentsWidth() const;
	
	/**
	 * make @p item selected or not selected
	 * depending on @p sel
	 *
	 * updates the view if needed
	 **/
	void setSelected(MultiTrackItem *item, bool sel);
	
	/**
	 * make the track @p track selected or not selected
	 * depending on @p sel
	 *
	 * updates the view if needed
	 **/
	MultiTrackTrack *selectedTrack( );
	void setSelected(MultiTrackTrack *track, bool sel);
	
	/**
	 * @returns true if @p item is selected
	 * and false otherwise
	 **/
	bool isSelected(const MultiTrackItem *item) const;
	
	/**
	 * @returns true if the track @p track is selected
	 * and false otherwise
	 **/
	bool isSelected(const MultiTrackTrack *track) const;
	
	/**
	 * Sets whether the current position indicator is drawn
	 *
	 * This is a little triangle thing in a "gutter zone"
	 * on the top of the widget, and a line extending
	 * down from it
	 **/
	void setShowPosition(bool s);
	
	/**
	 * @returns whether the position indicator
	 * will be shown as set by @ref setShowPosition
	 **/
	bool showPosition() const;

	/**
	 * @returns the "x" coordinate in pixels
	 * for the time position @p t
	 *
	 * This function accounts for scrolling.
	 *
	 * Pixels that are not visible will
	 * return a position that's out of
	 * visible range
	 **/
	int timeToPixel(int t) const;
	
	/**
	 * @returns the time the "x" coordinate
	 * @p t
	 *
	 * This function accounts for scrolling.
	 *
	 * Pixels that are not visible will
	 * return a position that's out of
	 * visible range
	 *
	 * n == pixelToTime( @r timeToPixel(n) )
	 **/
	int pixelToTime(int px) const;
	
	/**
	 * @returns the width of an item in pixels.
	 *
	 * @param t is the length of the item in
	 * time units
	 **/
	int pixelWidth(int t) const;
	
	/**
	 * @returns the width of an item in time units
	 *
	 * @param p is the length of the item in
	 * pixel units
	 **/
	int timeWidth(int p) const;
	
	/**
	 * @returns the current position as set by
	 * @ref setPosition
	 **/
	int position() const;
	
	/**
	 * @returns the value set by @ref setScaling
	 **/
	double scaling() const;
	
	/**
	 * @returns true tells you if time @p t is visible,
	 * or false if some horizontal scrolling is necessary
	 * to expose it.
	 *
	 * @sa ensureTimeVisible
	 **/
	bool isTimeVisible(int t) const;

	/**
	 * Sets the offset.
	 **/
	void setOffset( const int t );

	/**
	 * @returns the offset.
	 */
	int getOffset( ) const;
    
        virtual void setFont( const QFont& fn );

        /**
         * set the FPS; required for timecode information
         **/
        void setFPS( int fps, bool drop = true );

public slots:
	/**
	 * Marks all items as not-selected
	 **/
	void deselectAll();
	
	/**
	 * removes all tracks and items, thus
	 * calls "delete" on them
	 **/
	void clear();
	
	/**
	 * sets the current position.  Only
	 * visible if @ref setShowPosition
	 * was set to true
	 **/
	void setPosition(int pos);
	
	/**
	 * the same as @ref setPosition, but does not
	 * scroll horizontally to ensure that the
	 * current position is visible
	 **/
	void seek(int pos);
	
	/**
	 * sets the scaling
	 *
	 * when @p sc == 1.0, the entire length of the tracks
	 * fit in window, at 2.0, they take twice as much
	 * space
	 **/
	void setScaling(double sc);
	
	/**
	 * scrolls the least possible horizontally such
	 * that @p t becomes visible
	 *
	 * @sa isTimeVisible
	 **/
	void ensureTimeVisible(int t);
	
	/**
	 * tries to scroll horizontally such that time @p t
	 * is at the center
	 *
	 * It may not become the center if scrolling
	 * past the end would be necessary to ensure that
	 **/
	void centerOnTime(int t);
	
signals:
	/**
	 * this signal is emitted when the @p item
	 * is dragged within its track.  The new position
	 * is @p to
	 *
	 * Once this signal is emitted, you should
	 * call @ref MultiTrackItem::setTime on
	 * the item for the changes to take effect
	 **/
	void itemMoved(MultiTrackItem *item, int to);
	void itemMoved(Qt::ButtonState state, MultiTrackItem *item, int to);
	
	/**
	 * these signals are almost the same as the above, except emmited
	 * only once per move (not for each item)
	 *
	 * @param delta is the distance (int time) they have been moved
	 **/
	void itemsMoved(const QValueList<MultiTrackItem*>&, int delta);
	void itemsMoved(Qt::ButtonState state, const QValueList<MultiTrackItem*>&, int delta);
	void itemsMoved(int delta);
	void itemsMoved(Qt::ButtonState state, int delta);
	
	/**
	 * emitted when the user right clicks on
	 * an Item
	 *
	 * @param pt is the position in widget coordinates
	 * to show a context menu (relative to this widget)
	 **/
	void rightClicked(MultiTrackItem *item, const QPoint &pt);
	
	/**
	 * emitted when the position changes
	 **/
	void positionChanged(int p);

	/**
	 * emitted when the selected track is changed.
	 */
	void trackSelected( Qt::ButtonState state, MultiTrackTrack *track );
	
	/** 
	 * emitted when the user clicks on an item
	 *
	 * @param item is the item clicked on (which will always be set)
	 * @param time is the position in time units clicked at (relative
	 * to the beginning of the track, <i>not</i> the item)
	 **/
	void itemClicked(MultiTrackItem *item, int time);

	/**
	 * emitted when a track or item is selected.
	 */
	void trackSelectEvent( QMouseEvent * );
	void itemSelectEvent( QMouseEvent * );

	/** emitted when the silence button on a multitracktrack is clicked */
	void silenceClicked( MultiTrackTrack* );

	/** emitted when the enabled button on a multitracktrack is clicked */
	void enableClicked( MultiTrackTrack* );

	/** emitted when the locked button on a multitracktrack is clicked */
	void lockedClicked( MultiTrackTrack* );

	void multitrackDragEnterEvent( QDragEnterEvent *event );
	void multitrackDropEvent( QDropEvent *event );

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	
	void wheelEvent(QWheelEvent *);

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
	
private:
	void addTrack(MultiTrackTrack *t);
	void removeTrack(MultiTrackTrack *t);
	
	/**
	 * @returns the number of units the selected
	 * items should appear to have moved from
	 * their actual location (used for DnD)
	 **/
	int selectedDelta() const;
	
	/**
	 * @returns the pixel location where the
	 * contents begin being painted
	 **/
	int contentsBegin() const;
	
	/**
	 * @returns the time that the user has horizontally 
	 * scrolled to
	 **/
	int scrolledTime() const;
	
	/**
	 * @returns the number of pixels of height
	 * everything in this has, including the tracks
	 * and the slider gutter (if visible)
	 **/
	int contentsHeight() const;
	
	/**
	 * updates the range of the vertical scroll bars
	 **/
	void setupVScrollbar();

	/**
	 * draw a timeline 
	 **/
	void drawTimeLine( QPainter* p );

	void emitSilenceClicked( MultiTrackTrack* );
	void emitEnableClicked( MultiTrackTrack* );
	void emitLockedClicked( MultiTrackTrack* );

private slots:
	void scrolled();
};


#endif
