/*******************************************************************************
**
** The header file for the Jahshaka widget module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef VALUE_FORMATTERS_H
#define VALUE_FORMATTERS_H

class QString;

//! A formating object that will take a value and convert it to a string
class IntValueFormatter
{
public:
  IntValueFormatter();
  virtual ~IntValueFormatter();

    virtual QString convert( int ) const = 0;
    virtual IntValueFormatter* clone() const = 0;
};

//! A formatting object to convert a frame count into a timecode
class TimecodeValueFormatter : public IntValueFormatter
{
public:
    TimecodeValueFormatter();
    TimecodeValueFormatter( int fps, bool drop = true );

    void setFPS( int fps, bool drop );

    virtual QString convert( int ) const;
    virtual IntValueFormatter* clone() const;

private:
    int correct( int ) const;

    int m_fps;
    bool m_dropFrame;
};



#endif // VALUE_FORMATTERS_H
