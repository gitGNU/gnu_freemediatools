/*******************************************************************************
**
** The header file for the Jahshaka keyframer jahkeyframes file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef KEYFRAMER_H
#define KEYFRAMER_H

#include <qcanvas.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <widget.h>
#include <qwmatrix.h>

#include "keyframeritem.h"
#include "keyframeeditor.h"

//libraries
#include "jahkeyframes.h"
#include "jahtracer.h"

// std
#include <memory>

class QCanvasLine;
class QListBox;

///////////////////////////////////////////
// main class

class JahKeyframer : public QWidget 
{
    Q_OBJECT

public:
    //JahKeyframer(QCanvas&, QWidget* parent=0, const char* name=0, WFlags f=0);
    JahKeyframer(QWidget* theparent=0, QWidget* parent=0, const char* name=0, WFlags f=0);
    ~JahKeyframer();
    
    void updateKeyframes();
    void updateKeyframes(int max_frame, int current_frame, QPtrList<motionNode>* nodelist);

    int                     getMaxFrame() { return m_max_frame; }
    void                    setMaxFrame(int value) { m_max_frame = value; }
    int                     getCurrentFrame() { return m_current_frame; }
    void                    setCurrentFrame(int value) { m_current_frame = value; }
    
public slots:
    //should all be private except for calling routine above
    void clear();
    void buildInterface();

    void buildKeyframes();
    void buildKeyframes(int max_frame, int current_frame, QPtrList<motionNode>* nodelist);

    void movePlayLine( int keyframe );

private slots:

    void drawLine(int x1, int y1, int x2, int y2);

    void enlarge();
    void shrink();
    void zoomIn();
    void zoomOut();
    void addPlayLine();

    void emitAddKeyframe(void);
    void emitRemoveKeyframe(void);
    void emitPrevKeyframe(void);
    void emitNextKeyframe(void);

    void canvasResized();
    void slotZoomInY();
    void slotZoomOutY();
    void slotShiftYUp();
    void slotShiftYDown();

signals:

    void AddKeyframe(void);
    void RemoveKeyframe(void);
    void PrevKeyframe(void);
    void NextKeyframe(void);
    void signalKeyframeChanged(void);

protected slots:
    virtual void languageChange();
    void slotKeyframeChanged();
    void traceSelectionChanged();

private:
    QHBoxLayout* keyframeruiLayout;
    QVBoxLayout* keycontrolsLayout;

    // Creates keyframe display for one property (e.g. "tx", "ty" and so on).
    void buildKeyframeSequence(int max_frame, int current_frame, QString property, QPtrList<motionNode>* nodelist);

    // Returns the value of a keyframe for the given property (e.g. "tx", "ty" and so on).
    float* propertyPtr(std::string property, motionNode* keyframe);
    QColor pointColor(std::string property);


    int                         m_max_frame;
    int                         m_current_frame;
    QPtrList<motionNode>*       m_current_nodelist;

    ////////////////////////////////////////
    //pointer to tracer singleton
    JahTrace			* jtrace;

    QCanvas* canvas;
    KeyframeEditor *keyeditor;
    QListBox* traces;

    QPopupMenu* options;

    int dbf_id;

    double zoomlevel;
    
    std::auto_ptr< SliderItem > playline;  // should be in editor perhaps?
    std::auto_ptr< QCanvasLine > m_interfaceV;
    std::auto_ptr< QCanvasLine > m_interfaceH;
    std::auto_ptr< QCanvasLine > m_center_line;

    QPushButton* m_zoom_in_y_button;
    QPushButton* m_zoom_out_y_button;
    QPushButton* m_shift_y_up_button;
    QPushButton* m_shift_y_down_button;

};

#endif
