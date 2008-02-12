#ifndef CUSTOMLISTVIEWITEM_H
#define CUSTOMLISTVIEWITEM_H

#include <qlistview.h>
#include <qstring.h>
#include <qcolor.h>
#include <qpainter.h> 
#include <qpixmap.h>

// Declare in advance some Qt classes
class QWidget;
class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;

class QListView;
class QCheckListItem;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//! Custom checklistitem with alternative graphic. Behaves as a CheckBoxController, defaults to On. 
class CustomCheckListItem : public QCheckListItem
{
public:
    //! (TODO may need to add constr overloads, see QCheckListItem).
    CustomCheckListItem ( QCheckListItem * parent, const QString & text );//, QCheckListItem::Type tt = CheckBoxController );
    CustomCheckListItem ( QListView * parent, const QString & text, QCheckListItem::Type tt = CheckBoxController );
    CustomCheckListItem ( QCheckListItem * parent, QListViewItem *after, const QString & text, QCheckListItem::Type tt = CheckBoxController );
    CustomCheckListItem ( QListView * parent, QListViewItem *after, const QString & text, QCheckListItem::Type tt = CheckBoxController );
      
    
    //! Reimpl.
    void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );
    // Reimpl. (switched off, actuall)
    void paintFocus ( QPainter * p, const QColorGroup & cg, const QRect & r );

protected:
    void init();
    QPixmap eyeicon_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//! A custom listview item, so we can paint ourselves + customise behaviour. See SuperG mockups.
class CustomListViewItem : public QListViewItem
{
public:

	//! Constr with listview parent.
	CustomListViewItem( QListView *parentv );

	//! Constr with listviewitem parent.
	CustomListViewItem( CustomListViewItem *parenti );

	//! The function gives the option to grey-out an item. Default is false.
	void setGreyedOut( bool enable );

	bool isGreyedOut ();

	//! The function gives the option to highlight an item. Default is false.
	void setHighlighted( bool enable );

	bool isHighlighted ();

	//! Reimplementned from base class so we can tweak the painting (colours etc).
	void paintCell( QPainter *, const QColorGroup & cg, int column, int width, int alignment );

protected :

	//! Initialises our static icons, if they arent initialised already. Called from constr.
	void initIcons();

	//! Initialises the  colors (for alternating rows).
	void initColors();

    //! Returns a background color. Used for alternating bg colors for each row.
    QColor bgColor();

	//! Switched bg color per row
	void bgColorSwitcher();

    bool greyout_;
    bool highlight_;

	//! Keep parent item, if we had one (do not to assume we did).
	CustomListViewItem *p_;

	QColor mybgColor_;

private:

	//! Icons for this type of widget should always be sharded.
	static const QPixmap *ouricon_;	

	QColor bgColor1_;
	QColor bgColor2_;

};


#endif
