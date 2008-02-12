/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

// EXR - A OpenEXR plugin to QImage.

#ifndef EXR_INC_
#define EXR_INC_

class QImageIO;

#ifdef __cplusplus
extern "C" {
#endif

void vmfx_exr_read( QImageIO* );
void vmfx_exr_write( QImageIO* );

#ifdef __cplusplus
}
#endif

#endif
