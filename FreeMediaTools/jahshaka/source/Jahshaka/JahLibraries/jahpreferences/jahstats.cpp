/*******************************************************************************
**
** The source file for the Jahshaka jahstats object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahstats.h"

/////////////////////////////////////////////////////////////////////
//we should make this routine save the prefs and load the prefs in 
//to make things a bit cleane rmon

JahStats JahStats::_instance ;

JahStats::JahStats() 
    throw()
{   
    ARBsupport = false;
    NVsupport = false;
}

JahStats::~JahStats() 
    throw()
{

}

JahStats& JahStats::getInstance()
    throw()
{
    return (_instance);
}

void JahStats::initalizeStats(void)
    throw()
{
    /////////////////////////////////////////////////
    //build stats objects


    //first vendor and gl data
    std::string glven   = "OpenGL Vendor:";
    std::string glver   = "OpenGL Version:";
    statsdata1 = glven+stats_glvendor + " " + glver + stats_glversion;

    //now the gpu processing support
    std::string nvsupport;
    if (NVsupport)
    {
        nvsupport = "NVidia GPU: ON";
    }
    else
    {
        nvsupport = "NVidia GPU: OFF";
    }
        
    std::string arbsupport;
    if (ARBsupport)
    {
        arbsupport = "ARB GPU: ON";
    }
    else
    {
        arbsupport = "ARB GPU: OFF";
    }

    statsdata2 = "                      "+nvsupport + "  " + arbsupport;

    //here the renderer data

    std::string glren   = "OpenGL Renderer:";
    statsdata3 = glren+stats_glrender;

}

//set stats
void JahStats::setVendor(const std::string &s)
{
    stats_glvendor = s;
}

void JahStats::setVersion(const std::string &s)
{
    stats_glversion = s;
}

void JahStats::setRenderer(const std::string &s)
{
    stats_glrender = s;
}

void JahStats::setNVsupport(bool val)
{
    NVsupport=val;
}

void JahStats::setARBsupport(bool val)
{
    ARBsupport=val;
}

//get stats
std::string JahStats::getstatsdata1() 
{ 
    return statsdata1; 
}

std::string JahStats::getstatsdata2() 
{ 
    return statsdata2; 
}

std::string JahStats::getstatsdata3() 
{ 
    return statsdata3; 
}



