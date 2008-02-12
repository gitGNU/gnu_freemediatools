
// ml - A media library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.
// my changes are under THE GPL. Not LGPL.

#include <qvaluelist.h>
#include <qfile.h>
#include <cmath>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/GL_utility.hpp>

#include <openmedialib/ml/ml.hpp>

#include <qgl.h>

//TODO: dont have this #include <../../JahModules/editing_mlt/audio.h>
#include <jahheadable.h>

#include <qtimer.h>
#include <qdragobject.h>
#include <qmime.h>
#include <qstrlist.h>
#include <qevent.h>
#include <qcursor.h>
#include <qmessagebox.h>
#include <cmath>
#include <iostream>
#include <deque>

namespace ml  = olib::openmedialib::ml;
namespace il  = olib::openimagelib::il;
namespace opl = olib::openpluginlib;

class QtPlayer : public QGLWidget, public JahHeadable
{
  Q_OBJECT
    public:
  QtPlayer( const QGLWidget *, const std::string &input, int speed = 0, QWidget *parent=0, char *name=0 ) ;
  virtual ~QtPlayer( );
  void headRender( int w = 0, int h = 0 );
  bool play( const QString &input, int speed = 1 );
  bool play( const std::string &input, int speed = 1 );
  bool play( ml::input_type_ptr input, int speed = 1 );
  void stop();
  /*
    ever heard of  not blowing up header files with huge inline functions?
  */
  void set_speed( int speed, bool save = true );
  void restore_speed( );
  int speed( );
  void set_fps( float fps );
  int position( );
  void seek( int position );
  int length( );
  void toggle_full_screen( );
  void toggle_mute( );
  float volume( );
  void set_volume( float volume );
  void refresh( );
  void ensureWritableImages( bool on );
  void zoom( double value );
  typedef enum { pause, ramplayer, loop, next } eof_condition;
  void set_eof( eof_condition eof );
  eof_condition get_eof( );
  void switch_deinterlace_mode( );
 protected:
  void dragEnterEvent( QDragEnterEvent *event );
  void dropEvent( QDropEvent *event );;
  void mousePressEvent( QMouseEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void initializeGL();
  void resizeGL( int w, int h );

  // Very silly
  void visualise( ml::frame_type_ptr frame );
  ml::frame_type_ptr levels( ml::frame_type_ptr frame );
  bool proxy_texture_target_test( GLuint id, GLenum target, GLenum internal_format, GLenum format, GLenum type, int width, int height );
  void paintGL();
  int get_bpp( ml::image_type_ptr image );

  protected slots: // what syntax is this?
    void contextMenuEvent( QContextMenuEvent * );
  
  virtual void timeOutSlot();
  virtual void mouseSlot();;

 private:
  int deinterlace_mode( );
  inline void calculate_dimensions( ml::frame_type_ptr frame, int &phy_w, int &phy_h, int &req_w, int &req_h );
  void audio_play( ml::frame_type_ptr frame );;
  void audio_flush( );

 signals: // this is not c++!
  void showImage( int position, QImage *image );
  void draggedAImage( );
  void requestNextItem( );
  void showInHead( JahHeadable * );

 private:
  ml::input_type_ptr input_;
  //TODO :DONT HAVE	jahnle::MltAudio *audio_;
  std::deque < ml::frame_type_ptr > preroll_;
  ml::frame_type_ptr current_frame_;
  ml::frame_type_ptr field_;
  GLuint texture_id_;
  QTimer *mouse_;
  int mouse_moved_;
  QTimer *timer_;
  int speed_;
  bool accept_move_;
  bool accept_jog_;
  int jog_x_;
  GLenum old_target_;
  int old_phy_w_;
  int old_phy_h_;
  int old_speed_;
  int last_frame_;
  float volume_;
  int deinterlace_mode_;
  bool writable_images_;
  double zoom_;
  eof_condition eof_;
  float sequence_fps_;
  int current_width_;
  int current_height_;
  bool ask_audio_;
  bool mute_;
  GLuint buffer_id_;
  bool gpu_can_handle_;
};
