/*******************************************************************************
**
** The header file for the Jahshaka projectdata file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include "qstring.h"
#include "qcombobox.h"
#include "qfont.h"
#include <string>


//////////////////////////////////////////////////////////
//to keep track of project resolution and stuff like that

struct jahResolution
{
    QString     family;
    QString     title;
    double      frameratio;
    QString     frameratiotitle;
    double      pixelratio;
	double    	framerate;
	QString     fieldorder;

	int         xres;
	int         yres;

    int         m_field_number;

    int         getFieldNumber() { return m_field_number; }
    void        setFieldNumber(int value) { m_field_number = value; }

};

class projectData
{
public:
    projectData(void);
    ~projectData(void) {};

	int      getNumformats(void);
	int      getDefaultformat(void);
	int      getXres(int data);
	int      getYres(int data);
	QString  getFamily(int data);
	QString  getTitle(int data);
	double   getPixelRatio(int data);
	int      getFPS( int data );
	bool     getDropFrame( int data );

	void     buildComboBox(QComboBox * box);
    int      getFieldNumber(std::string name_string);

private:
	static const int numberformats = 17;
	void configureProjectData(void);
	jahResolution jahProjects[ numberformats ];
	int defaultformat;
};

#endif






