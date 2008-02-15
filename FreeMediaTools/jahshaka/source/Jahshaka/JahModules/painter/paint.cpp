/*******************************************************************************
 **
 ** The source file for the Jahshaka animation module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/


#include "paint.h"
#include <glworldlists.h>
#include <glcore.h>
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <timer.h>
#include <qpushbutton.h>

static const QColor default_background_color = QColor(0, 0, 0);

void GLPaint::start( )
{
  setMainworldQframe(m_mainworld);
  buildInterface(m_controls);
  buildInterfaceMenus(m_leftcontroller,m_rightcontroller);
  initializeObjects();

  x1 = 0;
  y1 = 0;
  x2 = 0;
  y2 = 0;
  setBrushSize(1);
}

//first thing to do is make buffer a array so we can store multiple clips
//allowing us to paint on video or sequences
//alpha support as well would be cool
//then we need to add layer support
//i think a new paint engine is necessary really...

void GLPaint::initializeObjects()
{
  m_pJTrace->info( "Initializing GLPaint Class");

  makeCurrent();
  resizePaintGL();

  //the default canvas
  JahLayer* jah_layer = configureLayer ( "World", false, true, false, true );
  buildLayer(jah_layer, "Image Layer", LayerCategory::LAYER, ObjectCategory::BACKDROP );
  m_pImageLayer = getLayerList()->at(0)->getJahLayer();

  //setup/customize listviewitem for layer 0
  initializeListview(0,TRUE);

  //old junk from here down
  m_current_history_frame = 1;

  m_pen_color        = new QColor(0,58,140);
  m_fill_color       = new QColor(0,97,140);
  m_background_color = new QColor(0,0,0);

  m_brush_size = 1;
  m_gradient_degree = 76;
  m_fade_degree = 240;
  m_active_tool = none;

  glPointSize(1);

  //initialize the buffer
  m_pCore->switchAutoBufferSwap( true );

  glClearColor( m_pCore->backgroundRed, m_pCore->backgroundGreen, m_pCore->backgroundBlue, 0.0 );
  glClear(GL_COLOR_BUFFER_BIT);

  m_frame_width = 720;
  m_frame_height = 486;
  setImageWidthDiv2( int( float( getFrameWidth() ) / 2.0f ) );
  setImageHeightDiv2( int( float( getFrameHeight() ) / 2.0f ) );

  m_history_list_ptr = new QPtrList<PaintHistory>;
  m_history_list_ptr->setAutoDelete(true);
  m_history_list_ptr->append( new PaintHistory );

  m_dirty_frames_list = new QPtrList<PaintHistory>;

  m_copy_list_ptr = new QPtrList<PaintHistory>;
  m_copy_list_ptr->setAutoDelete(true);


  resetAll();

  m_bForcePlay       = true;
  changedFrame    = false;
  frameEdited     = false;
  openPic = false;

  setGradientDegree(0);
  setNeedsClear(true);

  m_nCurrentFrame = 1;
  clearVideoFrameDirtyList();
  setFillBucketRange(0);
  m_line_brush_size = 1;

  setSrcBlendFactor(JAH_GL_SRC_ALPHA);
  setDstBlendFactor(JAH_GL_ONE_MINUS_SRC_ALPHA);

  m_eraser_buffer = NULL;

  m_nMaxFrame = 1;
  m_nMinFrame = 1;
  updatesliderEndframe(m_nMaxFrame);
  updatesliderStartframe(m_nMinFrame);
  m_pStartFrameControl->setValue(m_nMinFrame);
  m_pEndFrameControl->setValue(m_nMaxFrame);

  m_paint_effects_dispatch_table[INVERT_EFFECT] = &GLPaint::invertEffect;
  m_paint_effects_dispatch_table[FADE_EFFECT] = &GLPaint::fadeEffect;
  m_paint_effects_dispatch_table[INTENSIFY_EFFECT] = &GLPaint::intensifyEffect;
  m_paint_effects_dispatch_table[EMBOSS_EFFECT] = &GLPaint::embossEffect;
  m_paint_effects_dispatch_table[BLUR_EFFECT] = &GLPaint::blurEffect;
  m_paint_effects_dispatch_table[SHARPEN_EFFECT] = &GLPaint::sharpenEffect;
  m_paint_effects_dispatch_table[EDGE_DETECT_X_EFFECT] = &GLPaint::edgeDetectXEffect;
  m_paint_effects_dispatch_table[EDGE_DETECT_Y_EFFECT] = &GLPaint::edgeDetectYEffect;
  m_paint_effects_dispatch_table[A_OVER_B_COMPOSITE_EFFECT] = &GLPaint::aOverBCompositeEffect;
  m_paint_effects_dispatch_table[B_OVER_A_COMPOSITE_EFFECT] = &GLPaint::bOverACompositeEffect;
  m_paint_effects_dispatch_table[A_IN_B_COMPOSITE_EFFECT] = &GLPaint::aInBCompositeEffect;
  m_paint_effects_dispatch_table[B_IN_A_COMPOSITE_EFFECT] = &GLPaint::bInACompositeEffect;
  m_paint_effects_dispatch_table[A_OUT_B_COMPOSITE_EFFECT] = &GLPaint::aOutBCompositeEffect;
  m_paint_effects_dispatch_table[B_OUT_A_COMPOSITE_EFFECT] = &GLPaint::bOutACompositeEffect;
  m_paint_effects_dispatch_table[A_ATOP_B_COMPOSITE_EFFECT] = &GLPaint::aAtopBCompositeEffect;
  m_paint_effects_dispatch_table[B_ATOP_A_COMPOSITE_EFFECT] = &GLPaint::bAtopACompositeEffect;
  m_paint_effects_dispatch_table[A_XOR_B_COMPOSITE_EFFECT] = &GLPaint::aXorBCompositeEffect;
  m_paint_effects_dispatch_table[SOBEL_EFFECT] = &GLPaint::sobelEffect;
  m_paint_effects_dispatch_table[LAPLACIAN_EFFECT] = &GLPaint::laplacianEffect;
  m_paint_effects_dispatch_table[LAPLACIAN2_EFFECT] = &GLPaint::laplacian2Effect;
  m_paint_effects_dispatch_table[LAP_OF_GAUSS_EFFECT] = &GLPaint::lapOfGaussEffect;

  setCurrentCompositeType(A_OVER_B_COMPOSITE_EFFECT);
  m_eraser_circle_lookup_table = NULL;
  setUseGpuEraser(true);

  if ( getUseGpuEraser() )
    {
      glslDrawEraser(true);
    }

}

void GLPaint::clear(bool increment_history)
{
  m_needs_clear = false;

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0f, 1.0f, 1.0f);

  if ( !getClipIsStillImage() )
    {
      QPtrList<PaintHistory>* history_list = getHistoryListPtr();
      getImageLayer()->updateVidTex( history_list->at( getCurrentHistoryFrame() )->getOriginalVideoFrameNumber() );
    }

  if (getImageLayer()->layerStatus)
    {
      drawTexturedQuad( getImageLayerTextureId() );
    }
  else
    {
      clearColorsAndAlpha();
    }

  if ( getClipIsStillImage() && increment_history )
    {
      incrementHistoryCount();
    }

  if ( getDirtyFramesList()->count() < MAX_RESIDENT_TEXTURES )
    {
      captureDrawingArea();
      setVideoFrameDirty(getCurrentHistoryFrame(), true);
      getCurrentPaintHistory()->insertSorted( getDirtyFramesList() );
    }
  else
    {
      getCurrentPaintHistory()->setTextureId( getImageLayerTextureId() );
    }

  updateTimeSlider( getCurrentHistoryFrame() );
  updateGL();
}

void
GLPaint::updateImageTexture()
{
  JahLayer* image_layer = getLayerList()->at(0)->thelayer;

  int image_width = image_layer->getImageWidth();
  int image_height = image_layer->getImageHeight();

  m_nX_RESOLUTION = image_width;
  m_nY_RESOLUTION = image_height;

  if (m_nX_RESOLUTION % 2)
    {
      m_nX_RESOLUTION -= 1;
    }

  if (m_nY_RESOLUTION % 2)
    {
      m_nY_RESOLUTION -= 1;
    }

  m_frame_width  = m_nX_RESOLUTION;
  m_frame_height = m_nY_RESOLUTION;

  refreshTexture( getImageLayer()->getTextureData().getTexIdPtr() );

  setImageWidthDiv2( int( float( getFrameWidth() ) / 2.0f ) );
  setImageHeightDiv2( int( float( getFrameHeight() ) / 2.0f ) );

  QImage* qimage = &image_layer->getTextureData().qimage;
  unsigned char* buffer = new unsigned char[getFrameWidth() * getFrameHeight() * 4];
  getRectangleSubImage(buffer, qimage->bits(), 0, 0, getFrameWidth(), getFrameHeight(), image_width, image_height, getFrameWidth( ) );
  glBindTexture(GL_TEXTURE_2D, getImageLayerTextureId() );
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getFrameWidth(), getFrameHeight(), GL_BGRA, GL_UNSIGNED_BYTE, buffer );
  delete [] buffer;
}

void GLPaint::addClip(assetData clip)
{
  m_nMaxFrame = clip.endframe;
  m_bAnimation = false;

  JahLayer* image_layer = getLayerList()->at(0)->thelayer;

  resetHistory();

  image_layer->assignAsset(clip, VideoCategory::IMAGE);
  image_layer->LoadLayerAsset(VideoCategory::IMAGE);
  image_layer->makeTexture( );
  updateImageTexture();

  if (image_layer->asset.theType == VideoType::STILL)
    {
      setClipIsStillImage(true);
      m_pRedoButton->show();
      m_pUndoButton->show();
    }
  else
    {
      setClipIsStillImage(false);
      m_pRedoButton->hide();
      m_pUndoButton->hide();
      setVideoFrameDirty(m_nMinFrame, false);
    }

  for (int i = 1; i <= clip.endframe; i++)
    {
      if ( !getHistoryListPtr()->at(i) )
	{
	  getHistoryListPtr()->append( new PaintHistory(i) );
	}
    }

  firstframeanimation();

  if (m_nCurrentFrame == 0)
    {
      m_nCurrentFrame = 1;
    }

  //update start and end points to the start and end points of the source clip
  updatesliderEndframe(clip.endframe);
  m_pEndFrameControl->setValue(clip.endframe);
  int start_frame =  jahstd::Max<int>(clip.startframe, 1);
  updatesliderStartframe(start_frame);
  m_pStartFrameControl->setValue(start_frame);

  //setNeedsClear(true);
  getCopyListPtr()->clear();
  clear(false);
  updateGL( );
}

void GLPaint::startanimation()
{
  if ( getAnimating() )
    {
      setAnimating(false);
    }
  else
    {
      setAnimating(true);
      startSingleShotTimer();
    }
}

void GLPaint::updatePosition( )
{
  if ( getAnimating() )
    {
      if ( getCurrentFrame() > m_nMaxFrame )
	{
	  m_nCurrentFrame = m_nMinFrame;
	}

      updateAnimation( getCurrentFrame() );
      incrementCurrentFrame(1);
    }
}

void GLPaint::paintGL( )
{
  GLuint* texture_id_ptr = getCurrentTextureIdPtr();
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  if (texture_id_ptr)
    {
      drawTexturedQuad(*texture_id_ptr);
    }

  drawWithActiveTool();
}

void GLPaint::schedulePosition( )
{
  if ( getAnimating() )
    {
      startSingleShotTimer();
    }
}

GLuint *GLPaint::getCurrentTextureIdPtr()
{
  GLuint* texture_id_ptr = getTextureIdPtr( getCurrentHistoryFrame() );
  return texture_id_ptr;
}

GLuint
GLPaint::getCurrentTextureId()
{
  GLuint texture_id = getTextureId( getCurrentHistoryFrame() );
  return texture_id;
}

GLuint*
GLPaint::getTextureIdPtr(int history_step)
{
  PaintHistory* paint_history = getHistoryListPtr()->at(history_step);
  GLuint* texture_id_ptr = paint_history->getTextureIdPtr();
  return texture_id_ptr;
}

GLuint
GLPaint::getTextureId(int history_step)
{
  PaintHistory* paint_history = getHistoryListPtr()->at(history_step);
  return paint_history->getTextureId();
}

PaintHistory*
GLPaint::getCurrentPaintHistory()
{
  PaintHistory* paint_history = getHistoryListPtr()->at( getCurrentHistoryFrame() );
  return paint_history;
}

void
GLPaint::drawTexturedQuad(GLuint texture_id)
{
  glDisable( GL_DEPTH_TEST );
  glDepthMask( 0 );
  glClear( GL_COLOR_BUFFER_BIT );
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glEnable(GL_TEXTURE_2D);

#ifndef PAINT_ZOOM_SUPPORT
  m_pCore->zoomVal = 1.0f;  // Hardcode this for now  FIXME
#endif

  // Terrible place to do this, but changes for videohead put the glClear(0 here
  // and needs to be checked after clear
  if ( getImageLayer()->layerIsEnabled() )
    {
      glBegin(GL_QUADS);
      {
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( getScreenCenterX() - ( getImageWidthDiv2() * m_pCore->zoomVal ),
		    getScreenCenterY() -  ( getImageHeightDiv2() * m_pCore->zoomVal ),
		    0.0f );

	glTexCoord2f(m_texture_ratio.x, 0.0f);
	glVertex3f( getScreenCenterX() + ( getImageWidthDiv2() * m_pCore->zoomVal ),
		    getScreenCenterY() - ( getImageHeightDiv2() * m_pCore->zoomVal ),
		    0.0f);

	glTexCoord2f(m_texture_ratio.x, m_texture_ratio.y);
	glVertex3f( getScreenCenterX() + ( getImageWidthDiv2() * m_pCore->zoomVal ),
		    getScreenCenterY() + ( getImageHeightDiv2() * m_pCore->zoomVal ),
		    0.0f);

	glTexCoord2f(0.0f, m_texture_ratio.y);
	glVertex3f( getScreenCenterX() - ( getImageWidthDiv2() * m_pCore->zoomVal ),
		    getScreenCenterY() + ( getImageHeightDiv2() * m_pCore->zoomVal ),
		    0.0f);
      }
      glEnd();
    }
  else
    {
      clearColorsAndAlpha();
    }

  glDisable(GL_TEXTURE_2D);
  glFlush();
}


// Resize Canvas needs to communicate with the Core
void GLPaint::resizePaintGL( void )
{
  setScreenWidthDiv2( m_pCore->getRenderWidth() / 2 );
  setScreenHeightDiv2( m_pCore->getRenderHeight() / 2 );

  setScreenCenterX( getScreenWidthDiv2() );
  setScreenCenterY( getScreenHeightDiv2() );

#if PAINT_ZOOM_SUPPORT
  m_frame_width *= m_pCore->zoomVal;
  m_frame_height *= m_pCore->zoomVal;
  GLuint* texture_id_ptr = getCurrentTextureIdPtr();
  refreshTexture(texture_id_ptr);
  setNeedsClear(true);
#endif

  glClear(GL_COLOR_BUFFER_BIT);
  updateGL();
}


void GLPaint::updateDisplayZoom()
{
  CalcZoomValues();

  float delta_x = float(m_pCore->getRenderWidth() - getFrameWidth() );
  float delta_y = float(m_pCore->getRenderHeight() - getFrameHeight() );

  m_lower_left_x = (int)jahstd::Roundf(delta_x / 2.0f);
  m_lower_left_y = (int)jahstd::Roundf(delta_y / 2.0f);

  m_upper_right_x = getLowerLeftX() + getFrameWidth();
  m_upper_right_y = getLowerLeftY() + getFrameHeight();
}

bool
GLPaint::pointIsInsideDrawingArea(int x, int y)
{
  if ( x < getLowerLeftX() || x > getUpperRightX() || y < getLowerLeftY() || y > getUpperRightY() )
    {
      return false;
    }

  return true;
}

void GLPaint::CalcZoomValues()
{
  //this is to autoscale to the window size
  //but it aint working

  int abs_w = m_pCore->getRenderWidth(); //getFrameWidth()( );
  int abs_h = m_pCore->getRenderHeight(); //( );

  //code to factor in aspect ratio as well as
  //to position the draw on screen

  // This is the physcial WxH of the image
  int phy_w = getFrameWidth();
  int phy_h = getFrameHeight();

  //aspect ratio data
  //double ar = 1.333;
  double ar = (double)getFrameWidth() / (double) getFrameHeight();

  int req_h = abs_h;
  int req_w = int( req_h * ar );

  // Scale up or down to fit
  if ( req_w > abs_w )
    {
      req_w = abs_w;
      req_h = int( req_w / ar );
    }

  zoomPos_x = ( -req_w / 2);
  zoomPos_y = ( -req_h / 2);

  zoomScale_x = (( double )req_w / ( double )phy_w );
  zoomScale_y = (( double )req_h / ( double )phy_h );

}

void GLPaint::updateAnimation(int frame_number, bool )
{
  if (m_nMinFrame < 1)
    {
      m_nMinFrame = 1;
    }

  if (frame_number < m_nMinFrame)
    {
      m_nCurrentFrame = m_nMinFrame;
      frame_number = m_nMinFrame;
    }

  if (frame_number > m_nMaxFrame)
    {
      frame_number = m_nMaxFrame;
      m_nCurrentFrame = m_nMaxFrame;
    }

  m_current_history_frame = frame_number;

  if ( getHistoryListPtr() && m_current_history_frame >= (int)getHistoryListPtr()->count() )
    {
      PaintHistory* new_paint_history = new PaintHistory(frame_number);
      new_paint_history->insertSorted( getHistoryListPtr() );
    }

  //printf("GLPaint::updateAnimation frame_number = %d  Dirty = %s\n",
  //    frame_number, getVideoFrameDirty(frame_number) ? "true" : "false");

  // Need a better algorithm here
  while ( getDirtyFramesList() && getDirtyFramesList()->count() > MAX_RESIDENT_TEXTURES )
    {
      PaintHistory* history_to_clear_dirty = getDirtyFramesList()->last();
      GLuint* texture_id_ptr = history_to_clear_dirty->getTextureIdPtr();
      glDeleteTextures(1, texture_id_ptr);
      history_to_clear_dirty->clearDirty();

      getDirtyFramesList()->remove(history_to_clear_dirty);
      //printf("Clearing video frame %d dirty\n", frame_number);
    }

  if ( getVideoFrameDirty(frame_number) || getClipIsStillImage() )
    {
    }
  else if ( getImageLayer() && getImageLayer()->layerStatus)
    {
      clear(false);
    }

  updateTimeSlider(frame_number);
}

void
GLPaint::dumpNonBlackPixels()
{
  int buffer_size = getFrameWidth() * getFrameHeight();
  unsigned int* buffer = new unsigned int[buffer_size];

  glReadPixels(getLowerLeftX(), getLowerLeftY(), getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, buffer);

  for (int row = 0; row < getFrameHeight(); row++)
    {
      for (int column = 0; column < getFrameWidth(); column++)
	{
	  int offset = ( row * getFrameWidth() ) + column;

	  if ( (buffer[offset] & 0x00ffffff) != 0x00000000)
	    {
	      printf("%d, %d : %08x\n", row, column, buffer[offset]);
	    }
	}
    }

  delete [] buffer;
}

void
GLPaint::captureDrawingArea()
{
  updateDisplayZoom();
  GLuint* texture_id_ptr = getCurrentTextureIdPtr();

  if (!texture_id_ptr)
    {
      return;
    }

  refreshTexture(texture_id_ptr);
  glBindTexture(GL_TEXTURE_2D, *texture_id_ptr);
  glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getLowerLeftX(), getLowerLeftY(), getFrameWidth(), getFrameHeight());
}

void
GLPaint::refreshTexture(GLuint* texture_id_ptr)
{
  static int previous_frame_width = 0;
  static int previous_frame_height = 0;

  if (texture_id_ptr && *texture_id_ptr != INVALID_TEXTURE_ID &&
      (getFrameWidth() != previous_frame_width || getFrameHeight() != previous_frame_height) )
    {
      glDeleteTextures(1, texture_id_ptr);
      glGenTextures(1, texture_id_ptr);
    }

  previous_frame_width = getFrameWidth();
  previous_frame_height = getFrameHeight();

  glBindTexture(GL_TEXTURE_2D, *texture_id_ptr);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  getPowerOfTwoTextureSize(m_texture_width, m_texture_height, getFrameWidth(), getFrameHeight());
  m_texture_ratio.x = float( getFrameWidth() ) / float( getTextureWidth() );
  m_texture_ratio.y = float( getFrameHeight() ) / float( getTextureHeight() );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_texture_width, m_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void
GLPaint::setBrushSize(int newSize)
{
  float min_max_brush_size[2];
  glGetFloatv(GL_LINE_WIDTH_RANGE, &min_max_brush_size[0]);
  float min_brush_size = 0;
  float max_brush_size = min_max_brush_size[1];
  m_line_brush_size = jahstd::Max<float>(float(newSize), min_brush_size);
  m_line_brush_size = jahstd::Min<float>(m_line_brush_size, max_brush_size);

  glGetFloatv(GL_POINT_SIZE_RANGE, &min_max_brush_size[0]);
  min_brush_size = min_max_brush_size[0];
  max_brush_size = min_max_brush_size[1];
  m_point_brush_size = jahstd::Max<float>(float(newSize), min_brush_size);
  m_point_brush_size = jahstd::Min<float>(m_point_brush_size, max_brush_size);
  m_point_brush_size_int = int( jahstd::Roundf(m_point_brush_size) );

  m_brush_size = newSize;
}

void
GLPaint::setClearState()
{
  if ( getNeedsClear() )
    {
      clear(false);
      setNeedsClear(false);
    }
}

void
GLPaint::activateTool(CanvasTool toolNum)
{
  setClearState();
  m_active_tool = toolNum;
}

void
GLPaint::resetHistory()
{
  delete( getHistoryListPtr() );
  m_history_list_ptr = new QPtrList<PaintHistory>;
  m_history_list_ptr->setAutoDelete(true);
  m_history_list_ptr->append( new PaintHistory );

  delete ( getDirtyFramesList() );
  m_dirty_frames_list = new QPtrList<PaintHistory>;
}

void
GLPaint::resetAll()
{
  resizePaintGL();
  m_frame_width = 720;
  m_frame_height = 486;
  m_nX_RESOLUTION = m_frame_width;
  m_nY_RESOLUTION = m_frame_height;
  setImageWidthDiv2( int( float( getFrameWidth() ) / 2.0f ) );
  setImageHeightDiv2( int( float( getFrameHeight() ) / 2.0f ) );

  refreshTexture( getImageLayerTextureIdPtr() );

  setClipIsStillImage(true);
  m_pRedoButton->show();
  m_pUndoButton->show();

  resetHistory();

  m_nMinFrame = 1;
  m_nMaxFrame = 1;
  updatesliderEndframe(m_nMaxFrame);
  updatesliderStartframe(m_nMinFrame);
  m_pStartFrameControl->setValue(m_nMinFrame);
  m_pEndFrameControl->setValue(m_nMaxFrame);

  setBackgroundColor(default_background_color);
  setAnimating(false);

  m_pixel_coords.clear( );

  m_active_tool = none;

  clear(false);
}

void
GLPaint::SceneLoad()
{
}

void
GLPaint::sceneSave()
{
}

void
GLPaint::slotSetPenAlpha(int value)
{
  float alpha = float(value) / 100.0f;
  setPenAlpha(alpha);
}

void
GLPaint::slotSetFillAlpha(int value)
{
  float alpha = float(value) / 100.0f;
  setFillAlpha(alpha);
}

void
GLPaint::slotSetBackgroundAlpha(int value)
{
  float alpha = float(value) / 100.0f;
  setBackgroundAlpha(alpha);
}

void
GLPaint::clearColorsAndAlpha()
{
  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

  glBegin(GL_QUADS);
  {
    glVertex3f(getScreenCenterX() - getImageWidthDiv2(), getScreenCenterY() - getImageHeightDiv2(), 0.0f);
    glVertex3f(getScreenCenterX() + getImageWidthDiv2(), getScreenCenterY() - getImageHeightDiv2(), 0.0f);
    glVertex3f(getScreenCenterX() + getImageWidthDiv2(), getScreenCenterY() + getImageHeightDiv2(), 0.0f);
    glVertex3f(getScreenCenterX() - getImageWidthDiv2(), getScreenCenterY() + getImageHeightDiv2(), 0.0f);
  }
  glEnd();

}


void
GLPaint::refreshCurrentTexture()
{
  refreshTexture( getCurrentTextureIdPtr() );
}


bool
GLPaint::getUseGpuEraser()
{
  return( m_gpu_select_qcheckbox->isOn() &&
	  jahstd::glslSupport() &&
	  Globals::getGlVendor().find("ATI") == std::string::npos);
}

void
GLPaint::setUseGpuEraser(bool flag)
{
  m_gpu_select_qcheckbox->setChecked(flag);
}


GLPaint::GLPaint( GLCore* core,
		  const char* name,
		  QFrame* mainworld,
		  QHBox* controls,
		  QHBox* controller,
		  QHBox* leftcontroller,
		  QHBox* rightcontroller,
		  int* globalclipnumber,
		  const QGLWidget* )
  :  GLWorld(core,name,controller,globalclipnumber )
  , m_mainworld( mainworld )
  , m_controls( controls )
  , m_leftcontroller( leftcontroller )
  , m_rightcontroller( rightcontroller )
  , mousePressed(false)
  , m_history_list_ptr( NULL )
  , m_copy_list_ptr( NULL )
  , m_paint_effects_dispatch_table(NOT_A_PAINT_EFFECT)
  , m_dirty_frames_list( NULL )
{
}

GLPaint::~GLPaint()
{
}

//PaintPixelCoords

PaintPixelCoords::PaintPixelCoords( int x1_, int y1_, int x2_, int y2_ )
  : x1( x1_ )
  , y1( y1_ )
  , x2( x2_ )
  , y2( y2_ )
{ }

PaintPixelCoords::PaintPixelCoords( int x1_, int y1_ )
  : x1( x1_ )
  , y1( y1_ )
  , x2( 0 )
  , y2( 0 )
{ }

bool PaintPixelCoords::operator !=( PaintPixelCoords &other )
{
  return x1 != other.x1 || y1 != other.y1 || x2 != other.x2 || y2 != other.y2;
}

QFrame*         GLPaint::getMainworldQframe() 
{
  return m_mainworld_qframe;
}

GLPaint::PaintEffectFunction     GLPaint::getPaintEffectFunction(int i)
{
  return m_paint_effects_dispatch_table[i];
}


    QPtrList<PaintHistory>* GLPaint::getHistoryListPtr() 
{
 return m_history_list_ptr; 
}
    QPtrList<PaintHistory>* GLPaint::getCopyListPtr() 
{
 return m_copy_list_ptr; 
}

    QPtrList<PaintHistory>* GLPaint::getDirtyFramesList() 
{
 return m_dirty_frames_list; 
}

    bool*           GLPaint::getEraserCircleLookupTable() 
{
 return m_eraser_circle_lookup_table; 
}
    unsigned int*   GLPaint::getEraserBuffer() 
{
 return m_eraser_buffer; 
}
    color4*         GLPaint::getEraserBufferColor4() 
{
 return m_eraser_buffer_color4; 
}
    GLuint          GLPaint::getImageLayerTextureId() 
{
 return getImageLayer()->getTextureData().getTexId(); 
}
    GLuint*         GLPaint::getImageLayerTextureIdPtr() 
{
 return &getImageLayer()->getTextureData().texID; 
}
    void            GLPaint::setMainworldQframe(QFrame* qframe) 
{
 m_mainworld_qframe = qframe; 
}
    float           GLPaint::getPenAlpha() 
{
 return m_pen_alpha; 
}
    void            GLPaint::setPenAlpha(float value) 
{
 m_pen_alpha = value; 
}
    float           GLPaint::getFillAlpha() 
{
 return m_fill_alpha; 
}
    void            GLPaint::setFillAlpha(float value) 
{
 m_fill_alpha = value; 
}
    float           GLPaint::getBackgroundAlpha() 
{
 return m_background_alpha; 
}
    void            GLPaint::setBackgroundAlpha(float value) 
{
 m_background_alpha = value; 
}

    int             GLPaint::getImageWidthDiv2() 
{
 return m_image_width_div_2; 
}
    void            GLPaint::setImageWidthDiv2(int value) 
{
 m_image_width_div_2 = value; 
}
    int             GLPaint::getImageHeightDiv2() 
{
 return m_image_height_div_2; 
}
    void            GLPaint::setImageHeightDiv2(int value) 
{
 m_image_height_div_2 = value; 
}
    int             GLPaint::getScreenWidthDiv2() 
{
 return m_screen_width_div_2; 
}
    void            GLPaint::setScreenWidthDiv2(int value) 
{
 m_screen_width_div_2 = value; 
}
    int             GLPaint::getScreenHeightDiv2() 
{
 return m_screen_height_div_2; 
}
    void            GLPaint::setScreenHeightDiv2(int value) 
{
 m_screen_height_div_2 = value; 
}
    int             GLPaint::getScreenCenterX() 
{
 return m_screen_center_x; 
}
    void            GLPaint::setScreenCenterX(int value) 
{
 m_screen_center_x = value; 
}
    int             GLPaint::getScreenCenterY() 
{
 return m_screen_center_y; 
}
    void            GLPaint::setScreenCenterY(int value) 
{
 m_screen_center_y = value; 
}

    int             GLPaint::getLowerLeftX() 
{
 return m_lower_left_x; 
}
    int             GLPaint::getLowerLeftY() 
{
 return m_lower_left_y; 
}
    void            GLPaint::setLowerLeftX(int value) 
{
 m_lower_left_x = value; 
}
    void            GLPaint::setLowerLeftY(int value) 
{
 m_lower_left_y = value; 
}
    int             GLPaint::getUpperRightX() 
{
 return m_upper_right_x; 
}
    int             GLPaint::getUpperRightY() 
{
 return m_upper_right_y; 
}
    void            GLPaint::setUpperRightX(int value) 
{
 m_upper_right_x = value; 
}
    void            GLPaint::setUpperRightY(int value) 
{
 m_upper_right_y = value; 
}

    float2&         GLPaint::getTextureRatio() 
{
 return m_texture_ratio; 
}
	int				GLPaint::getTextureWidth() 
{
 return m_texture_width; 
}
	int				GLPaint::getTextureHeight() 
{
 return m_texture_height; 
}
	bool			GLPaint::getNeedsClear() 
{
 return m_needs_clear; 
}
	void			GLPaint::setNeedsClear(bool flag) 
{
 m_needs_clear = flag; 
}
    GLuint          GLPaint::getWorkingAreaTextureId() 
{
 return m_working_area_texture_id; 
}
    GLuint*         GLPaint::getWorkingAreaTextureIdPtr() 
{
 return &m_working_area_texture_id; 
}

    int             GLPaint::getFrameWidth() 
{
 return m_frame_width; 
}
    int             GLPaint::getFrameHeight() 
{
 return m_frame_height; 
}

    bool            GLPaint::getClipIsStillImage() 
{
 return m_clip_is_still_image; 
}
    void            GLPaint::setClipIsStillImage(bool flag) 
{
 m_clip_is_still_image = flag; 
}
QLabel*         GLPaint::getCurrentColorLabel() 
{
 return m_current_color_label; 
}

bool GLPaint::usesKeyFrames( ) 
{
 return false; 
}

int GLPaint::limit0_255(const int & val) 
{
 return val < 0 ? 0 : val > 255 ? 255 : val; 
}

QColor GLPaint::penColor()
{
  return *m_pen_color; 
}

QColor GLPaint::fillColor()       
{
  return *m_fill_color; 
}

QColor GLPaint::backgroundColor() 
{

  return *m_background_color; 
}

int   GLPaint::brushSize()       
{
  return m_brush_size; 
}

int    GLPaint::gradientDegree()  {
  
  return m_gradient_degree; 
}

// Modifier functions.
void GLPaint::setPenColor (QColor newColor)      
{
  *m_pen_color        = newColor; 
}

void GLPaint::setFillColor(QColor newColor)
{ 
  *m_fill_color       = newColor; 
}

void GLPaint::setGradientDegree(int newVal)       
{
  m_gradient_degree  = newVal; 
}

int  GLPaint::getFillBucketRange() 
{ 
  return m_fill_bucket_range; 
}

void GLPaint::setFillBucketRange(int value)
{
  m_fill_bucket_range = value; 
}

GLPaint::PAINT_EFFECT_TYPE GLPaint::getCurrentCompositeType() 
{ 
  return m_current_composite_type; 
}

void GLPaint::setCurrentCompositeType(GLPaint::PAINT_EFFECT_TYPE type)
{

  m_current_composite_type = type; 
}

bool GLPaint::usesVideoHead( ) 
{
 
  return false; 
}
