
#ifndef ANIMATION_EYE_CHECK_LIST_ITEM_H
#define ANIMATION_EYE_CHECK_LIST_ITEM_H

// qt
#include <qobject.h>
#include <qlistview.h>
#include <qstring.h>
#include <qpixmap.h>

class QPoint;

/// simple QCheckListItem sub-class showing a toggle-able 
/// eye in the second column.
class EyeCheckListItem : public QObject, public QCheckListItem
{
    Q_OBJECT

public:
    EyeCheckListItem( QCheckListItem* parent, const QString& text, Type tt = RadioButtonController );
    EyeCheckListItem( QCheckListItem* parent, QListViewItem* after, const QString& text, Type tt = RadioButtonController );
    EyeCheckListItem( QListView* parent, const QString& text, Type tt = RadioButtonController );
    EyeCheckListItem( QListView* parent, QListViewItem* after, const QString& text, Type tt = RadioButtonController );

    bool eyeState() const;
    void setEyeState( bool b );

private slots:
    void onPressed( QListViewItem* item, const QPoint&, int c );

private:
    void init();
    QPixmap getPixmapForState( bool open );

    QString m_pixmapPath;
    bool m_open;
};

#endif // ANIMATION_EYE_CHECK_LIST_ITEM_H
