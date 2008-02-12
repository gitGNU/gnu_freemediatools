/*******************************************************************************
**
** The header file for the Jahshaka jahtimeline file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHTIMELINE_H
#define JAHTIMELINE_H

#include "widget.h"
#include <qcheckbox.h>
#include <qtabwidget.h>

#include "openmedialib.h"

#include "jahtimelineeditor.h"

class EditDesktop : public QWidget {
    Q_OBJECT

public:
    EditDesktop(QWidget* theparent=0, QWidget* parent=0, const char* name=0, WFlags f=0);

    QImage getClip();
    void setClipname(QString&);
    QString getClipname();
    int clipnumber;
    assetData getVideoData();

   int videolayer;
	int layer_height;
	int layer_pos;

public slots:
    void addRectangle(int numline);

    //we overide these here for both manual and automated
    void addEditLayer(int numline, bool status, int startframe, int endframe);
    void addEditLayer(int numline, assetData thedata);
private slots:

    void clear();
    void init();
    void addRectangle();

    void drawScale(int width);
    void addLine();
    void addLines(int width, int numline);

private:
    EditEditor *editor;
    QCanvas* canvas;
    int dbf_id;
    QImage* img;
};


#endif
