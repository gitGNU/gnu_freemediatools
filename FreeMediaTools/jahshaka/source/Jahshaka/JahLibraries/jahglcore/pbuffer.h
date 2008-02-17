/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef PBUFFER_H
#define PBUFFER_H

#ifdef WIN32
#	include <GL/glew.h>
#	include <GL/wglew.h>
#else
#	include <GL/glew.h>
#endif

class PBuffer
{
   public:
      PBuffer( int width, int height, int bpp, bool isFloat, 
		bool dBuffer, bool hasDepth, bool hasStencil, bool texture, bool share);
      ~PBuffer();

      //is this buffer usable
      bool IsValid();
      
      //make it available for rendering
      void Bind();
      void Unbind();

      //make it available as a texture if applicable 
      void BindAsTexture( int buffer);
      void ReleaseTexture( int buffer);

      //swap if applicable
      void Swap();

      int               getPreviousViewportWidth() ;
      void              setPreviousViewportWidth(int width);
      int               getPreviousViewportHeight();
      void              setPreviousViewportHeight(int height);
      int               getWidth();
      void              setWidth(int width);
      int               getHeight() ;
      void              setHeight(int height);


protected:
      int               m_width;
      int               m_height;
      int               m_previous_viewport_width;
      int               m_previous_viewport_height;

   private:

#ifdef WIN32
      HPBUFFERARB       m_gl_pbuffer;
      HGLRC             m_rendering_context;
      HDC               m_display_context;

      //used to restore the previous RC when unbinding
      HGLRC             m_previous_rendering_context;
      HDC               m_previous_display_context;
#else
	GLuint fb_id_, rb_id_, tex_id_;
	GLenum target_;
	int width_, height_;
	bool valid_;
#endif
};

#endif //  PBUFFER_H
