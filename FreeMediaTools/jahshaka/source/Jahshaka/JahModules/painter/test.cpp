/*******************************************************************************
  **
  ** The source file for the Jahshaka paint interface module
  ** The Jahshaka Project
  ** Copyright (C) 2000-2006 VM Inc.
  ** Released under the GNU General Public License
  **
  *******************************************************************************/
 
 #include "paint.h"
 #include <jahformatter.h>
 #include <InputLCD.h>
 #include <supergrangecontrol.h>
 #include <jahtranslate.h>
 #include <valueFormatters.h>
 
 #include <qframe.h>
 #include <qhbox.h>
 #include <qlistview.h>
 #include <qpushbutton.h>
 #include <qlayout.h>
 #include <widget.h>
 
 void GLPaint::buildInterfaceMenus( QHBox*, QHBox* rightcontroller) 
 {
 	// Container widget
 	QWidget *container = new QWidget( rightcontroller, "page" );
 	QVBoxLayout *container_layout = new QVBoxLayout( container, 0, 0, "container_layout");
 
 	container_layout->addStretch( );
 
 	// Wow - confusing variable names here....
     tooltranslate = new JahToolButton( container, "pen" );
     JahFormatter::addJahPlayerButton( tooltranslate, m_qsJahBasePath+"Pixmaps/paint/buttonPen.png", m_qsJahBasePath+"Pixmaps/paint/buttonPen.png" );
 	container_layout->addWidget( tooltranslate );
 
     toolrotate = new JahToolButton( container, "line" );
     JahFormatter::addJahPlayerButton( toolrotate, m_qsJahBasePath+"Pixmaps/paint/buttonLine.png", m_qsJahBasePath+"Pixmaps/paint/buttonLine.png" );
 	container_layout->addWidget( toolrotate );
 
     toolscale = new JahToolButton( container, "erase" );
     JahFormatter::addJahPlayerButton( toolscale, m_qsJahBasePath+"Pixmaps/paint/buttonEraser.png", m_qsJahBasePath+"Pixmaps/paint/buttonEraser.png" );
 	container_layout->addWidget( toolscale );
 
 	container_layout->addSpacing( 10 );
 
 	// Photo and Render
     m_pScrubRender = new JahToolButton( container, "controllerrewindbutton" );
     JahFormatter::addJahPlayerButton( m_pScrubRender, m_qsJahBasePath+"Pixmaps/desktop/phototool.png", m_qsJahBasePath+"Pixmaps/desktop/phototool.png" );
     connect( m_pScrubRender,  SIGNAL(clicked()), SLOT(Render())  );
 	container_layout->addWidget( m_pScrubRender );
    
     m_pScrubRenderAll = new JahToolButton( container, "controllerpreviousbutton" );
     JahFormatter::addJahPlayerButton( m_pScrubRenderAll, m_qsJahBasePath+"Pixmaps/desktop/rendertool.png", m_qsJahBasePath+"Pixmaps/desktop/rendertool.png" );
     connect( m_pScrubRenderAll,  SIGNAL(clicked()), SLOT( RenderScene() )  );
 	container_layout->addWidget( m_pScrubRenderAll );
 
     //connect tools to object and add object to display widget
     connect( toolscale,      SIGNAL(clicked()), this, SLOT(slotEraser() )  );
 
 	//connect tools to object and add object to display widget
     connect( tooltranslate,  SIGNAL(clicked()), this, SLOT(slotPen() )  );
     connect( toolrotate,      SIGNAL(clicked()), this, SLOT(slotLine() )  );
 
 	container_layout->addStretch( );
 }
 
 void GLPaint::buildInterface( QHBox* f ) 
 {
     
     QWidget* placeholder = new QWidget( f );
     QHBoxLayout* mainLayout = new QHBoxLayout( placeholder );
     JahFormatter::setMarginAndSpacingSmall( mainLayout );
 
     ///////////////////////////////////////////////////////////
     //The layers interface
 
     QBoxLayout* layersLayout = new QVBoxLayout();
     mainLayout->addLayout( layersLayout );
 
     ///////////////////////////////////////////////////////////
     //The layer options
     //these are defined in the world object
     m_pAddButton = new QPushButton( placeholder, "ClearButton" );
     m_pAddButton->setText( m_pJt->tr("Clear") );
     layersLayout->addWidget( m_pAddButton );
     connect  ( m_pAddButton,  SIGNAL(clicked()), this, SLOT( slotClear())  );
     
     m_pRefreshButton = new QPushButton( placeholder, "RefreshButton" );
     m_pRefreshButton->setText( m_pJt->tr("Refresh") );
     layersLayout->addWidget( m_pRefreshButton );
     connect  ( m_pRefreshButton,  SIGNAL(clicked()), this, SLOT( slotRefresh() )  );
     
     m_pRedoButton = new QPushButton( placeholder, "RedoButton" );
     m_pRedoButton->setText( m_pJt->tr("Redo") );
     layersLayout->addWidget( m_pRedoButton );
     connect  ( m_pRedoButton,  SIGNAL(clicked()), this, SLOT( slotHistoryNext() )  );
     
     m_pUndoButton = new QPushButton( placeholder, "UndoButton" );
     m_pUndoButton->setText( m_pJt->tr("Undo") );
     layersLayout->addWidget( m_pUndoButton );
     connect  ( m_pUndoButton,  SIGNAL(clicked()), this, SLOT( slotHistoryPrevious() )  );
     
     m_pDeleteButton = new QPushButton( placeholder, "DeleteButton" );
     m_pDeleteButton->setText( m_pJt->tr("Delete") );
     layersLayout->addWidget( m_pDeleteButton );
     connect  ( m_pDeleteButton,  SIGNAL(clicked()), this, SLOT( slotDelete() )  );
     
     m_pCopyOneButton = new QPushButton( placeholder, "CopyOneButton" );
     m_pCopyOneButton->setText( m_pJt->tr("Copy") );
     layersLayout->addWidget( m_pCopyOneButton );
     connect  ( m_pCopyOneButton,  SIGNAL(clicked()), this, SLOT( slotCopyOne() )  );
     
     m_pPasteButton = new QPushButton( placeholder, "pasteButton" );
     m_pPasteButton->setText( m_pJt->tr("Paste") );
     layersLayout->addWidget( m_pPasteButton );
     connect  ( m_pPasteButton,  SIGNAL(clicked()), this, SLOT( slotPaste() )  );
 
     layersLayout->addStretch();
     
     //////////////////////////////////////////////////////////////
     // set up the scene options, or prefernces
     
     EffectsFrame = new QFrame( placeholder );
     QBoxLayout* EffectsFrameLayout = new QHBoxLayout( EffectsFrame );
     JahFormatter::setSpacing( EffectsFrameLayout );
     mainLayout->addWidget( EffectsFrame );
     
     JahFormatter::addSpacingSmall( EffectsFrameLayout );
 
     /////////////////////////////////////////////////////////
     //this is the list-view
     m_pLayerListView = new QListView( EffectsFrame );
     m_pLayerListView->setRootIsDecorated(true);
     m_pLayerListView->addColumn( (m_pJt->tr("LAYERS")), -1 );
     m_pLayerListView->setSorting( -1,1 );   // disables the autosort
 
     JahFormatter::setListViewAsSingleColumn( m_pLayerListView );
     EffectsFrameLayout->addWidget( m_pLayerListView );
 
     connect( m_pLayerListView, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( clicked( QListViewItem* ) ) );
     
     //allow for multiple m_bSelections
     m_pLayerListView->setSelectionMode (QListView::Extended );
     
     /////////////////////////////////////////////////////////
     //Set up tabbed interface
     
     tabframe = new QTabWidget( EffectsFrame, "axiscontrols" );	
     EffectsFrameLayout->addWidget( tabframe );
     
     tabframe->setTabShape( QTabWidget::Triangular );
     tabframe->setAutoMask( FALSE );
     
     /////////////////////////////////////////////////////////
     //Set up tabbed interface
     //this has become the settings panel need to update variable names
     moduleOptionsUI[0] = new QHBox( EffectsFrame, "encoderUI0" ); //scenecontrols
     tabframe->insertTab( moduleOptionsUI[0], tr("  "+m_pJt->tr("SCENE")+"  ") );
     
     moduleOptionsUI[1] = new QHBox( EffectsFrame, "objectcontrols" ); //objectcontrols
     tabframe->insertTab( moduleOptionsUI[1], tr( "  "+m_pJt->tr("TOOLS")+"  " ) );
     
     moduleOptionsUI[2] = new QHBox( EffectsFrame, "axiscontrols" ); //axiscontrols
     tabframe->insertTab( moduleOptionsUI[2], tr( "  "+m_pJt->tr("BLEND")+"  " ) );
     
     moduleOptionsUI[3] = new QHBox( EffectsFrame, "texturecontrols" ); //texturecontrols
     tabframe->insertTab( moduleOptionsUI[3], tr( "    "+m_pJt->tr("EFFECTS")+"    " ) );
     
     //moduleOptionsUI[4] = new QWidget( EffectsFrame, "effectscontrols" ); //effectscontrols
     //tabframe->insertTab( moduleOptionsUI[4], tr( "  "+m_pJt->tr("CONTROLS")+"  " ) );
     
     //this has become the settings panel need to update variable names
     //moduleOptionsUI[5] = new QWidget( EffectsFrame, "keysettings" ); //keycontrols
     //tabframe->insertTab( moduleOptionsUI[5], tr( "  "+m_pJt->tr("KEYFRAMES")+"  " ) );
         
     ///////////////////////////////////////////////////////////////////
     // the scene control tab bar
     
     //mover this into the layers...
     {
         ControlFrameUI[0][0] = new QFrame( moduleOptionsUI[0], "ControlFrame3" ); //scenecontrols
         
         QBoxLayout* outer = new QVBoxLayout( ControlFrameUI[0][0] );
         JahFormatter::setMarginAndSpacing( outer );
         QBoxLayout* mainLayout = new QHBoxLayout();
         outer->addLayout( mainLayout );
         outer->addStretch();
         
         QBoxLayout* buttons = new QVBoxLayout();
         mainLayout->addLayout( buttons );
         
 #if 0
         LoadButton = new QPushButton( ControlFrameUI[0][0], "LoadButton" );
         LoadButton->setText( m_pJt->tr("Load") );
         buttons->addWidget( LoadButton );
         connect  ( LoadButton,  SIGNAL(clicked()), this, SLOT( SceneLoad())  );
         
         SaveButton = new QPushButton( ControlFrameUI[0][0], "SaveButton" );
         SaveButton->setText( m_pJt->tr("Save") );
         buttons->addWidget( SaveButton );
         connect  ( SaveButton,  SIGNAL(clicked()), this, SLOT( sceneSave())  );
 #endif
         
         ResetButton = new QPushButton( ControlFrameUI[0][0], "ResetButton" );
         ResetButton->setText( m_pJt->tr("Reset All") );
         buttons->addWidget( ResetButton );
         connect  ( ResetButton,  SIGNAL(clicked()), this, SLOT( resetAll())  );
         
         ClearButton = new QPushButton( ControlFrameUI[0][0], "ClearButton" );
         ClearButton->setText( m_pJt->tr("Clear Scene") );
         buttons->addWidget( ClearButton );
         connect( ClearButton,  SIGNAL(clicked()), this, SLOT( slotClear())  ); 
         
         mainLayout->addStretch();
         
         // 
         QBoxLayout* controls = new QVBoxLayout();
         mainLayout->addLayout( controls );
         
         m_pSelectFPSLabel = new QLabel( ControlFrameUI[0][0], "FPSPushButtonLabel" );
         controls->addWidget( m_pSelectFPSLabel );
         m_pSelectFPSLabel->setAlignment( int( QLabel::AlignCenter ) );
         m_pSelectFPSLabel->setText( m_pJt->tr( "Playback Speed" ) );
         
         m_pSelectFPSPushbutton = new QPushButton( ControlFrameUI[0][0], "FPSPushButton" );
         controls->addWidget( m_pSelectFPSPushbutton );
         QString text_string = getFramesPerSecondName(FRAMES_PER_SECOND_30).data();
         m_pSelectFPSPushbutton->setText(text_string);
         connect(m_pSelectFPSPushbutton, SIGNAL( pressed() ), SLOT( slotChooseFramesPerSecond() ) );
         
         m_gpu_select_qcheckbox = new QCheckBox( ControlFrameUI[0][0], "GpuSelect" );
         m_gpu_select_qcheckbox->setText( m_pJt->tr("Use GPU") );
         controls->addWidget( m_gpu_select_qcheckbox );
         //connect( m_gpu_select_qcheckbox,  SIGNAL(clicked()), this, SLOT( gpuSelect() )  );
         
         // 
         
         //need to move this into the module
         //this is mandatory for all modules...
         QGridLayout* lcdLayout = new QGridLayout( 3, 2 );
         lcdLayout->setColStretch( 0, 0 );
         lcdLayout->setColStretch( 1, 1 );
         lcdLayout->setColStretch( 2, 1 );
         mainLayout->addLayout( lcdLayout );
         
         QLabel* sframetext = new QLabel( ControlFrameUI[0][0], "sframetext" );
         lcdLayout->addWidget( sframetext, 0, 0, Qt::AlignVCenter );
         sframetext->setText( tr( m_pJt->tr("STARTFRAME") ) );
         sframetext->setAlignment( int( QLabel::AlignRight ) );
         
         m_pStartFrameControl = new SupergRangeControl( ControlFrameUI[0][0], "startframe" );
         m_pStartFrameControl->setFormatter( TimecodeValueFormatter() );
         lcdLayout->addWidget( m_pStartFrameControl, 0, 1 );
         JahFormatter::configure( m_pStartFrameControl, m_nAStartFrame, m_nAStartFrame );
         connect( m_pStartFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderStartframe(int)) );
         
         QLabel* eframetext = new QLabel( ControlFrameUI[0][0], "eframetext" );
         lcdLayout->addWidget( eframetext, 1, 0, Qt::AlignVCenter );
         eframetext->setText( tr( m_pJt->tr("ENDFRAME")  ) );
         eframetext->setAlignment( int( QLabel::AlignRight ) );
         
         m_pEndFrameControl = new SupergRangeControl( ControlFrameUI[0][0], "endframe" );
         m_pEndFrameControl->setFormatter( TimecodeValueFormatter() );
         lcdLayout->addWidget( m_pEndFrameControl, 1, 1 );
         JahFormatter::configure( m_pEndFrameControl, m_nAEndFrame, m_nAStartFrame );
         connect( m_pEndFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderEndframe(int)) );
         
 #if 0
         m_pJahResLabel = new QLabel( ControlFrameUI[0][0], "languagelabel" );
         m_pJahResLabel->setGeometry( QRect( 450, 75, 120, 21 ) );
         m_pJahResLabel->setAlignment( int( QLabel::AlignCenter ) );
         m_pJahResLabel->setText( m_pJt->tr( "Resolution" ) );
         
         m_pJahResCombo = new QComboBox( FALSE, ControlFrameUI[0][0], "m_pJahResCombo" );
         m_pJahResCombo->setGeometry( QRect( 450, 100, 170, 21 ) );
         m_bHasResMenu = true;
         
         projectData thedata;
         thedata.buildComboBox(m_pJahResCombo);
         int defaultRes = jprefs.getJahResolution();
         m_pJahResCombo->setCurrentItem(defaultRes); //should be set to appdefault
         connect( m_pJahResCombo,  SIGNAL(activated(int)), this, SLOT(setResolution(int))  );
 #endif
     }
 
     ///////////////////////////////////////////////////////////
     //Tool control panel
     
     ControlFrameUI[1][0] = new QFrame( moduleOptionsUI[1], "ControlFrame1" );
     ControlFrameUI[1][0]->setGeometry( QRect( 0, 0, 660, 190 ) );    // was 510
     
     ////standard tools are here
     //ControlFrame1L1 = new QLabel( ControlFrameUI[1][0], "brushslider" );
     //JahFormatter::addLabel( ControlFrame1L1, 10, 1, 150, 10, "DRAW LayerCategory::OBJECT", 9);
     
     //these were toolbuttons not pushbuttons so we could see what was m_bSelected
     m_pen_button	= new QPushButton(ControlFrameUI[1][0],"brush");
     JahFormatter::addIconButton(m_pen_button, 10, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonPen.png");
     connect( m_pen_button,  SIGNAL(clicked()), this, SLOT(slotPen())  );
     
     m_line_button	= new QPushButton(ControlFrameUI[1][0],"line");
     JahFormatter::addIconButton(m_line_button, 35, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonLine.png");
     connect( m_line_button,  SIGNAL(clicked()), this, SLOT(slotLine())  );
     
     m_rectangle_button	= new QPushButton(ControlFrameUI[1][0],"square");
     JahFormatter::addIconButton(m_rectangle_button, 70, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonRectangle.png");
     connect( m_rectangle_button,  SIGNAL(clicked()), this, SLOT(slotRectangle())  );
     
     m_rectangle_filled_button	= new QPushButton(ControlFrameUI[1][0],"square");
     JahFormatter::addIconButton(m_rectangle_filled_button, 95, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonRectangleFilled.png");
     connect( m_rectangle_filled_button,  SIGNAL(clicked()), this, SLOT(slotRectangleFilled())  );
     
     m_triangle_button	= new QPushButton(ControlFrameUI[1][0],"triangle");
     JahFormatter::addIconButton(m_triangle_button, 130, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonTriangle.png");
     connect( m_triangle_button,  SIGNAL(clicked()), this, SLOT(slotTriangle())  );
     
     m_triangle_filled_button	= new QPushButton(ControlFrameUI[1][0],"triangle");
     JahFormatter::addIconButton(m_triangle_filled_button, 155, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonTriangleFilled.png");
     connect( m_triangle_filled_button,  SIGNAL(clicked()), this, SLOT(slotTriangleFilled())  );
     
     m_circle_button	= new QPushButton(ControlFrameUI[1][0],"circle");
     JahFormatter::addIconButton(m_circle_button, 190, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonCircle.png");
     connect( m_circle_button,  SIGNAL(clicked()), this, SLOT(slotCircle())  );
     
     m_circle_filled_button	= new QPushButton(ControlFrameUI[1][0],"circle");
     JahFormatter::addIconButton(m_circle_filled_button, 215, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonCircleFilled.png");
     connect( m_circle_filled_button,  SIGNAL(clicked()), this, SLOT(slotCircleFilled())  );
     
     m_paintbucket_button = new QPushButton(ControlFrameUI[1][0],"paintbucket");
     JahFormatter::addIconButton(m_paintbucket_button, 275, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonBucket.png");
     connect( m_paintbucket_button,  SIGNAL(clicked()), this, SLOT(slotPaintBucket())  );
     
     m_paintbucket_fill_all_button = new QPushButton(ControlFrameUI[1][0],"paintbucketfillall");
     JahFormatter::addIconButton(m_paintbucket_fill_all_button, 300, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonFillAllBucket.png");
     connect( m_paintbucket_fill_all_button,  SIGNAL(clicked()), this, SLOT(slotPaintBucketFillAll())  );
     
     m_eraser_button = new QPushButton(ControlFrameUI[1][0],"eraser");
     JahFormatter::addIconButton(m_eraser_button, 335, 20, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonEraser.png");
     connect( m_eraser_button,  SIGNAL(clicked()), this, SLOT( slotEraser() )  );
     
     // Brushes
     
     //m_brushes_label = new QLabel( ControlFrameUI[1][0], "brushslider" );
     //JahFormatter::addLabel( m_brushes_label, 10, 55, 150, 10, "BRUSHES", 9);
     
     int left_x = 70;
     
     m_pen_alpha_slider = new QSlider( ControlFrameUI[1][0] );   m_pen_alpha_lcd = new InputLCD( ControlFrameUI[1][0] );
     JahFormatter::addSliderAndLcd(  m_pen_alpha_slider, left_x, 55, 150, 20,  0, 100, m_pen_alpha_lcd );
     m_pen_alpha_slider->setTickInterval(2);
     m_pen_alpha_label = new QLabel( ControlFrameUI[1][0], "PenAlpha" );
     JahFormatter::addLabel( m_pen_alpha_label, left_x, 75, 150, 15, "Pen Alpha", 9);
     
     connect( m_pen_alpha_slider, SIGNAL(valueChanged(int)), this, SLOT(slotSetPenAlpha(int)) );
     connect( m_pen_alpha_slider, SIGNAL(valueChanged(int)), m_pen_alpha_lcd, SLOT(setValue(int)) );
     connect(m_pen_alpha_lcd, SIGNAL(valueChanged(int)), this, SLOT(slotSetPenAlpha(int)) );
     connect( m_pen_alpha_lcd, SIGNAL(valueChanged(int)), m_pen_alpha_slider, SLOT(setValue(int)) );
     m_pen_alpha_slider->setValue(100);
     
     m_fill_alpha_slider = new QSlider( ControlFrameUI[1][0] );   m_fill_alpha_lcd = new InputLCD( ControlFrameUI[1][0] );
     JahFormatter::addSliderAndLcd(  m_fill_alpha_slider, left_x, 90, 150, 20,  0, 100, m_fill_alpha_lcd );
     m_fill_alpha_slider->setTickInterval(2);
     m_fill_alpha_label = new QLabel( ControlFrameUI[1][0], "FillAlpha" );
     JahFormatter::addLabel( m_fill_alpha_label, left_x, 110, 150, 15, "Fill Alpha", 9);
     
     connect( m_fill_alpha_slider, SIGNAL(valueChanged(int)), this, SLOT(slotSetFillAlpha(int)) );
     connect( m_fill_alpha_slider, SIGNAL(valueChanged(int)), m_fill_alpha_lcd, SLOT(setValue(int)) );
     connect(m_fill_alpha_lcd, SIGNAL(valueChanged(int)), this, SLOT(slotSetFillAlpha(int)) );
     connect( m_fill_alpha_lcd, SIGNAL(valueChanged(int)), m_fill_alpha_slider, SLOT(setValue(int)) );
     m_fill_alpha_slider->setValue(100);
     
     m_background_alpha_slider = new QSlider( ControlFrameUI[1][0] );   m_background_alpha_lcd = new InputLCD( ControlFrameUI[1][0] );
     JahFormatter::addSliderAndLcd(  m_background_alpha_slider, left_x, 125, 150, 20,  0, 100, m_background_alpha_lcd );
     m_background_alpha_slider->setTickInterval(2);
     m_background_alpha_label = new QLabel( ControlFrameUI[1][0], "BackgroundAlpha" );
     JahFormatter::addLabel( m_background_alpha_label, left_x, 145, 150, 15, "Background Alpha", 9);
     
     connect( m_background_alpha_slider, SIGNAL(valueChanged(int)), this, SLOT(slotSetBackgroundAlpha(int)) );
     connect( m_background_alpha_slider, SIGNAL(valueChanged(int)), m_background_alpha_lcd, SLOT(setValue(int)) );
     connect(m_background_alpha_lcd, SIGNAL(valueChanged(int)), this, SLOT(slotSetBackgroundAlpha(int)) );
     connect( m_background_alpha_lcd, SIGNAL(valueChanged(int)), m_background_alpha_slider, SLOT(setValue(int)) );
     m_background_alpha_slider->setValue(100);
     m_background_alpha_slider->hide();
     m_background_alpha_lcd->hide();
     m_background_alpha_label->hide();
     
     
     left_x = 270;
     
     m_brush_size_slider = new QSlider( ControlFrameUI[1][0] );   m_brush_size_lcd = new InputLCD( ControlFrameUI[1][0] );
     JahFormatter::addSliderAndLcd(  m_brush_size_slider, left_x, 55, 150, 20,  0, 50, m_brush_size_lcd );
     m_brush_size_slider->setTickInterval(2);
     //m_brush_size_slider->setTickmarks(QSlider::Below);
     m_brush_size_label = new QLabel( ControlFrameUI[1][0], "brushslider" );
     JahFormatter::addLabel( m_brush_size_label, left_x, 75, 150, 15, "Brush Size", 9);
     
     m_brush_size_slider->setValue(1);
     connect( m_brush_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slotBrushSize(int)) );
     connect( m_brush_size_slider, SIGNAL(valueChanged(int)), m_brush_size_lcd, SLOT(setValue(int)) );
     connect(m_brush_size_lcd, SIGNAL(valueChanged(int)), this, SLOT(slotBrushSize(int)) );
     connect( m_brush_size_lcd, SIGNAL(valueChanged(int)), m_brush_size_slider, SLOT(setValue(int)) );
     
     m_gradient_slider = new QSlider( ControlFrameUI[1][0] );   m_gradient_lcd = new InputLCD( ControlFrameUI[1][0] );
     JahFormatter::addSliderAndLcd(  m_gradient_slider, left_x, 90, 150, 20,  -255, 255, m_gradient_lcd );
     m_gradient_slider->setTickInterval(25);
     //m_gradient_slider->setTickmarks(QSlider::Below);
     m_gradient_label = new QLabel( ControlFrameUI[1][0], "gradientslider" );
     JahFormatter::addLabel( m_gradient_label, left_x, 110, 150, 15, "Gradient Angle", 9);
     
     connect( m_gradient_slider, SIGNAL(valueChanged(int)), this, SLOT(slotGradientDegree(int)) );
     connect( m_gradient_slider, SIGNAL(valueChanged(int)), m_gradient_lcd, SLOT(setValue(int)) );
     connect(m_gradient_lcd, SIGNAL(valueChanged(int)), this, SLOT(slotGradientDegree(int)) );
     
     m_bucket_range_slider = new QSlider( ControlFrameUI[1][0] );   m_bucket_range_lcd = new InputLCD( ControlFrameUI[1][0] );
     JahFormatter::addSliderAndLcd(  m_bucket_range_slider, left_x, 90, 150, 20,  0, 255, m_bucket_range_lcd );
     m_bucket_range_slider->setTickInterval(25);
     //m_bucket_range_slider->setTickmarks(QSlider::Below);
     m_bucket_range_label = new QLabel( ControlFrameUI[1][0], "bucketrangeslider" );
     JahFormatter::addLabel( m_bucket_range_label, left_x, 110, 150, 15, "Fill Bucket Range", 9);
     
     connect( m_bucket_range_slider, SIGNAL(valueChanged(int)), this, SLOT(slotFillBucketRange(int)) );
     connect( m_bucket_range_slider, SIGNAL(valueChanged(int)), m_bucket_range_lcd, SLOT(setValue(int)) );
     connect(m_bucket_range_lcd, SIGNAL(valueChanged(int)), this, SLOT(slotFillBucketRange(int)) );
     m_bucket_range_lcd->hide();
     m_bucket_range_slider->hide();
     m_bucket_range_label->hide();
     
     m_overwrite_still_frames_qcheckbox = new QCheckBox( ControlFrameUI[1][0], "overwrite" );
     m_overwrite_still_frames_qcheckbox->setGeometry(10, 140, 130, 20);
     m_overwrite_still_frames_qcheckbox->setText("Overwrite Frames");
     
     left_x = 460;
     
     m_current_color_label = new QLabel( ControlFrameUI[1][0], "Colorlabel" );
     JahFormatter::addLabel( m_current_color_label, left_x + 25, 20, 110, 22, "", 9);
     //m_background_color_label->setAlignment( QLabel::AlignRight );
     
     m_background_color_label = new QLabel( ControlFrameUI[1][0], "Background" );
     JahFormatter::addLabel( m_background_color_label, left_x, 55, 100, 22, "Background", 9);
     //m_background_color_label->setAlignment( QLabel::AlignRight );
     
     m_background_color_button	= new QPushButton(ControlFrameUI[1][0],"backgroundcolor");
     JahFormatter::addIconButton(m_background_color_button, left_x + 100, 55, 40, 22 );
     m_background_color_button->setPaletteBackgroundColor( QColor(5,38,10) );
     connect( m_background_color_button,  SIGNAL(clicked()), this, SLOT(slotBackgroundColor())  );
     
     m_foreground_color_picker_button = new QPushButton(ControlFrameUI[1][0],"foregroundcolorpicker");
     JahFormatter::addIconButton(m_foreground_color_picker_button, left_x + 75, 90, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonForegroundDropper.png");
     connect( m_foreground_color_picker_button,  SIGNAL(clicked()), this, SLOT(slotForegroundColorPicker())  );
     
     m_pen_color_label = new QLabel( ControlFrameUI[1][0], "Pen" );
     JahFormatter::addLabel( m_pen_color_label, left_x + 20, 90, 55, 22, "Pen", 9);
     //m_gradient_angle_label->setAlignment( QLabel::AlignRight );
     
     m_pen_color_button = new QPushButton(ControlFrameUI[1][0],"pencolor");
     JahFormatter::addIconButton(m_pen_color_button, left_x + 100, 90, 40, 22 );
     m_pen_color_button->setPaletteBackgroundColor( QColor(0,58,140) );
     connect( m_pen_color_button,  SIGNAL(clicked()), this, SLOT(slotPenColor())  );
     
     m_fill_color_label = new QLabel( ControlFrameUI[1][0], "Fill" );
     JahFormatter::addLabel( m_fill_color_label, left_x + 20, 125, 55, 22, "Fill", 9);
     //m_fill_color_label->setAlignment( QLabel::AlignRight );
     
     m_background_color_picker_button = new QPushButton(ControlFrameUI[1][0],"backgroundcolorpicker");
     JahFormatter::addIconButton(m_background_color_picker_button, left_x + 75, 125, 22, 22, m_qsJahBasePath+"Pixmaps/paint/buttonDropper.png");
     connect( m_background_color_picker_button,  SIGNAL(clicked()), this, SLOT(slotBackgroundColorPicker())  );
     
     m_fill_color_button	= new QPushButton(ControlFrameUI[1][0],"fillcolor");
     JahFormatter::addIconButton(m_fill_color_button, left_x + 100, 125, 40, 22 );
     m_fill_color_button->setPaletteBackgroundColor( QColor(0,97,140) );
     connect( m_fill_color_button,  SIGNAL(clicked()), this, SLOT(slotFillColor())  );
     
     ///////////////////////////////////////////////////////////
     //color control panel
     {
         
         ControlFrameUI[2][0] = new QFrame( moduleOptionsUI[2], "ControlFrame4" );
         QGridLayout* top = new QGridLayout( ControlFrameUI[2][0], 2, 2 );
         QGridLayout* g = new QGridLayout( 4, 2 );
         top->addLayout( g, 0, 0 );
         JahFormatter::setMarginAndSpacing( g );
 
         m_pSrcBlendModeSelectLabel = new QLabel( ControlFrameUI[2][0] );
         m_pSrcBlendModeSelectLabel->setText("Src Blend Mode");
         m_pSrcBlendModeSelectLabel->setAlignment( QLabel::AlignCenter );
         g->addWidget( m_pSrcBlendModeSelectLabel, 0, 0 );
     
         m_pSrcBlendModeSelectButton = new QToolButton(ControlFrameUI[2][0], "SourceBlend" );
         m_pSrcBlendModeSelectButton->setText("GL_SRC_ALPHA");
         connect( m_pSrcBlendModeSelectButton,  SIGNAL(clicked()), this, SLOT(slotSetSrcBlendFactor())  );
         g->addWidget( m_pSrcBlendModeSelectButton, 1, 0 );
 
         m_pDstBlendModeSelectLabel = new QLabel( ControlFrameUI[2][0] );
         m_pDstBlendModeSelectLabel->setText("Dest Blend Mode");
         m_pDstBlendModeSelectLabel->setAlignment( QLabel::AlignCenter );
         g->addWidget( m_pDstBlendModeSelectLabel, 0, 1 );
 
         m_pDstBlendModeSelectButton = new QToolButton(ControlFrameUI[2][0], "DstBlend" );
         m_pDstBlendModeSelectButton->setText("GL_ONE_MINUS_SRC_ALPHA");
         connect( m_pDstBlendModeSelectButton,  SIGNAL(clicked()), this, SLOT(slotSetDstBlendFactor())  );
         g->addWidget( m_pDstBlendModeSelectButton, 1, 1 );
 
         g->setRowSpacing( 2, 20 );
 
         m_pResetBlendModeButton = new QToolButton(ControlFrameUI[2][0], "ResetBlendMode" );
         m_pResetBlendModeButton->setText("Reset Blend Mode");
         connect( m_pResetBlendModeButton,  SIGNAL(clicked()), this, SLOT( slotResetBlendFactor() )  );
         g->addWidget( m_pResetBlendModeButton, 3, 0 );

 #if 0
         // N.B. if this is going to be used, it needs to be placed into the 
         // above layout scheme
         ControlFrame4L1 = new QLabel( ControlFrameUI[2][0], "gradientslider" );
         JahFormatter::addLabel( ControlFrame4L1, 10, 10, 110, 22, "BACKGROUND", 9);
         
         m_background_color_button	= new QPushButton(ControlFrameUI[2][0],"backgroundcolor");
         JahFormatter::addIconButton(m_background_color_button, 120, 10, 40, 22 );
         m_background_color_button->setPaletteBackgroundColor( QColor(5,38,10) );
         connect( m_background_color_button,  SIGNAL(clicked()), this, SLOT(slotBackgroundColor())  );
         
         m_gradient_angle_label = new QLabel( ControlFrameUI[2][0], "gradientslider" );
         JahFormatter::addLabel( m_gradient_angle_label, 10, 40, 90, 22, "PEN", 9);
         
         m_pen_color_button	= new QPushButton(ControlFrameUI[2][0],"pencolor");
         JahFormatter::addIconButton(m_pen_color_button, 120, 40, 40, 22 );
         m_pen_color_button->setPaletteBackgroundColor( QColor(0,58,140) );
         connect( m_pen_color_button,  SIGNAL(clicked()), this, SLOT(slotPenColor())  );
         
         ControlFrame4L3 = new QLabel( ControlFrameUI[2][0], "gradientslider" );
         JahFormatter::addLabel( ControlFrame4L3, 10, 70, 90, 22, "FILL", 9);
         
         m_fill_color_button	= new QPushButton(ControlFrameUI[2][0],"fillcolor");
         JahFormatter::addIconButton(m_fill_color_button, 120, 70, 40, 22 );
         m_fill_color_button->setPaletteBackgroundColor( QColor(0,97,140) );
         connect( m_fill_color_button,  SIGNAL(clicked()), this, SLOT(slotFillColor())  );
 #endif
     }
     
     /////////////////////////////////////////////////////////////////////////
     //
     
     //add the layer reset button here as well
     ControlFrameUI[3][0] = new QFrame( moduleOptionsUI[3], "ControlFrame4" );
     ControlFrameUI[3][0]->setGeometry( QRect( 0, 0, 660, 190 ) );
     
     //column one
     effectInvert = new QPushButton( ControlFrameUI[3][0], "invert" );
     JahFormatter::addButton( effectInvert, 10, 10, 120, 21, "Invert" );
     connect( effectInvert,  SIGNAL(clicked()), this, SLOT(slotInvert())  );
     
     effectBlur = new QPushButton( ControlFrameUI[3][0], "blur" );
     JahFormatter::addButton( effectBlur, 10, 40, 120, 21, "Blur" );
     connect( effectBlur,  SIGNAL(clicked()), this, SLOT(slotBlur())  );
     
     effectSharpen = new QPushButton( ControlFrameUI[3][0], "sharpen" );
     JahFormatter::addButton( effectSharpen, 10, 70, 120, 21, "Sharpen" );
     connect( effectSharpen,  SIGNAL(clicked()), this, SLOT(slotSharpen())  );
     
     m_advance_frame_qcheckbox = new QCheckBox( ControlFrameUI[3][0], "advanceframe" );
     m_advance_frame_qcheckbox->setGeometry(10, 130, 130, 20);
     m_advance_frame_qcheckbox->setText("Advance Frame");
     connect( m_advance_frame_qcheckbox,  SIGNAL(clicked()), this, SLOT( slotTurnOffApplyEffectsToAllFrames() )  );
     
     m_apply_effect_to_all_frames_qcheckbox = new QCheckBox( ControlFrameUI[3][0], "applyeffecttoallframes" );
     m_apply_effect_to_all_frames_qcheckbox->setGeometry(150, 130, 220, 20);
     m_apply_effect_to_all_frames_qcheckbox->setText("Apply Effect To All Frames");
     connect( m_apply_effect_to_all_frames_qcheckbox,  SIGNAL(clicked()), this, SLOT( slotTurnOffAdvanceFrame() )  );
     
     //column 2
     effectFade = new QPushButton( ControlFrameUI[3][0], "fade" );         //these need a slider as well
     JahFormatter::addButton( effectFade, 150, 10, 120, 21, "Fade" );
     connect( effectFade,  SIGNAL(clicked()), this, SLOT( slotFade() )  );
     
     effectIntensify = new QPushButton( ControlFrameUI[3][0], "intensify" );     //these need a slider as well
     JahFormatter::addButton( effectIntensify, 150, 40, 120, 21, "Intensify" );
     connect( effectIntensify,  SIGNAL(clicked()), this, SLOT( slotIntensify() )  );
     
     m_emboss_button = new QPushButton( ControlFrameUI[3][0], "emboss" );     //these need a slider as well
     JahFormatter::addButton( m_emboss_button, 150, 70, 120, 21, "Emboss" );
     connect( m_emboss_button,  SIGNAL(clicked()), this, SLOT( slotEmboss() )  );
     
     effectEdgeX = new QPushButton( ControlFrameUI[3][0], "edgex" );
     JahFormatter::addButton( effectEdgeX, 290, 10, 120, 21, "Edge detectX" );
     connect( effectEdgeX,  SIGNAL(clicked()), this, SLOT(slotEdgeDetectX())  );
     
     effectEdgeY = new QPushButton( ControlFrameUI[3][0], "edgey" );
     JahFormatter::addButton( effectEdgeY, 290, 40, 120, 21, "Edge detectY" );
     connect( effectEdgeY,  SIGNAL(clicked()), this, SLOT(slotEdgeDetectY())  );
     
     m_select_composite_button = new QPushButton( ControlFrameUI[3][0], "m_bSelect_composite" );
     JahFormatter::addButton( m_select_composite_button, 10, 100, 120, 21, "Composite Select" );
     connect( m_select_composite_button,  SIGNAL(clicked()), this, SLOT( slotSetCompositeType() )  );
     //m_select_composite_button->setPaletteBackgroundColor( QColor(30, 30, 30) );
     
     m_composite_button = new QPushButton( ControlFrameUI[3][0], "execute_composite" );
     JahFormatter::addButton( m_composite_button, 150, 100, 120, 21, "A over B" );
     connect( m_composite_button,  SIGNAL(clicked()), this, SLOT( slotComposite() )  );
     
     //column four
     effectSobel = new QPushButton( ControlFrameUI[3][0], "sobel" );
     JahFormatter::addButton( effectSobel, 430, 10, 120, 21, "Sobel" );
     connect( effectSobel,  SIGNAL(clicked()), this, SLOT(slotSobel())  );
     
     effectLapacian = new QPushButton( ControlFrameUI[3][0], "lapacian" );
     JahFormatter::addButton( effectLapacian, 430, 40, 120, 21, "Lapacian" );
     connect( effectLapacian,  SIGNAL(clicked()), this, SLOT(slotLaplacian())  );
     
     effectLapacian2 = new QPushButton( ControlFrameUI[3][0], "lapacian2" );
     JahFormatter::addButton( effectLapacian2, 430, 70, 120, 21, "Lapacian2" );
     connect( effectLapacian2,  SIGNAL(clicked()), this, SLOT(slotLaplacian2())  );
     
     effectGauss = new QPushButton( ControlFrameUI[3][0], "gaussian" );
     JahFormatter::addButton( effectGauss, 430, 100, 120, 21, "Gaussian" );
     connect( effectGauss,  SIGNAL(clicked()), this, SLOT(slotLapOfGauss())  );
     
 }
 
 
 void GLPaint::createMenuItem( QPopupMenu * themenu )
 {
     Q_CHECK_PTR(themenu);
     //themenu->insertItem("&"+m_pJt->tr("Add Object"),this, SLOT(addLayer()) );
     //   themenu->insertItem("&"+m_pJt->tr("Name Object"),this,SLOT(nameLayer()) );
     //   themenu->insertItem("&"+m_pJt->tr("Delete Object"),this,SLOT(delLayer()) );
     // themenu->insertSeparator();
     //make this a popup 'Move'
 }
