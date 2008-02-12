/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "anime.h"
#include <openobjectlib.h>
#include <jahstats.h>


////////////////////////////////////////////////////////////////
//this is only used if a jahplayer build and should be a new file
//as we need to initalize glew and the gpu options
//and then load in the plugins
//see jahglsupport.cpp in JahSource/jahmain

//move into a separate file to unclutter animation module
//as this is only relevant to the player

void 
GLAnime::initializePlayer( void )
{
	QString glewinitstring;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		//problem: glewInit failed, something is seriously wrong
		m_pJTrace->debug( "Glew Error");
		glewinitstring.sprintf("Error: %s\n", glewGetErrorString(err));
		m_pJTrace->debug(glewinitstring);
	}
	else
	{
		m_pJTrace->debug( "Glew Initialized");
		glewinitstring.sprintf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		m_pJTrace->debug(glewinitstring);
	}

	//first we need to get all necessary vendor data
	QString *vendor, *renderer, *version, *extensions;

	//can we do all of this with GLEW ?

  if ( glGetString(GL_VENDOR) != NULL )
  	vendor     = new QString ((const char *) glGetString(GL_VENDOR));
  else
    vendor     = new QString( "" );

  if ( glGetString(GL_RENDERER) != NULL )
    renderer   = new QString ((const char *) glGetString(GL_RENDERER));
  else
    renderer   = new QString( "" );

  if ( glGetString(GL_VERSION) != NULL )
    version    = new QString ((const char *) glGetString(GL_VERSION));
  else
    version    = new QString( "" );

  if ( glGetString(GL_EXTENSIONS) != NULL )
    extensions = new QString ((const char *) glGetString(GL_EXTENSIONS));
  else
    extensions = new QString( "" );


    std::string vendor_string( vendor->ascii() );
    std::string renderer_string( renderer->ascii() );
    std::string version_string( version->ascii() );

	//need access to Globals
    Globals::setGlVendor(vendor_string);
    Globals::setGlRenderer(renderer_string);
    Globals::setGlVersion(version_string);

    m_pJTrace->debug( "Vendor    :"+*vendor);
    m_pJTrace->debug( "Renderer  :"+*renderer);
    m_pJTrace->debug( "Version   :"+*version);

	////////////////////////////////////////////////////////////////////
    JahStats& jstats = JahStats::getInstance();

	//check the prefs for compatability
	//#ifndef JAHPLAYER
    jstats.setVendor ( vendor_string );
    jstats.setVersion ( version_string );
    jstats.setRenderer ( renderer_string );


	////////////////////////////////////////////////////////////////////
    m_pJTrace->info( "Enabling OpenGL Extensions");

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    //check GLEW version

    if (GLEW_VERSION_1_3)
    {
      m_pJTrace->debug("OpenGL 1.3 is supported!");
    }


    //In general, you can check if GLEW_{extension_name} or GLEW_VERSION_{version} is set or not.
    //For extensions glewGetExtension provides a slower alternative: 

    if (GL_TRUE == glewGetExtension("GL_ARB_fragment_program"))
    {
      m_pJTrace->info("ARB_fragment_program is supported.");
    }

    //////////////////////////////////////////////////////////
    // other extensions

    if (GL_TRUE == glewGetExtension("GL_ARB_imaging"))
    {
      m_pJTrace->info("GL_ARB_imaging is supported.");
    }

    if (GL_TRUE == glewGetExtension("GL_ARB_multisample"))
    {
      m_pJTrace->info("GL_ARB_multisample is supported.");
      jprefs.setHwAliasing(true);
    }

    if (GL_TRUE == glewGetExtension("GL_EXT_fog_coord"))
    {
      m_pJTrace->info("GL_EXT_fog_coord is supported.");
      jprefs.setHwFog(true);
    }

    //////////////////////////////////////////////////////////
    //check for simple shader support
    bool simpleshaders = true;

    if (GL_TRUE == glewGetExtension("GL_NV_fragment_program"))  {}   else simpleshaders = false;
    if (GL_TRUE == glewGetExtension("GL_NV_vertex_program"))    {}   else simpleshaders = false;

    if (simpleshaders)
    {
        m_pJTrace->info("We have simple shader support");
        jprefs.setVShading(true);
    }

    //////////////////////////////////////////////////////////
    //check for opengl shader support
    bool glslsupport = true;

    if (GL_TRUE == glewGetExtension( "GL_ARB_fragment_shader"))			{} else glslsupport = false;
    if (GL_TRUE == glewGetExtension( "GL_ARB_vertex_shader"))			{} else glslsupport = false;
    if (GL_TRUE == glewGetExtension( "GL_ARB_shader_objects"))			{} else glslsupport = false;
    if (GL_TRUE == glewGetExtension( "GL_ARB_shading_language_100"))    {} else glslsupport = false;

    if (glslsupport)
    {
        m_pJTrace->info("We have opengl shader support");
        //jprefs.setVShading(true);
    }

    //////////////////////////////////////////////////////////
    //check for gpu and gpgpu support
    bool m_bGPUSupport = true;

    if (GL_TRUE == glewGetExtension("GL_ARB_fragment_program"))   {} else m_bGPUSupport = false;
    if (GL_TRUE == glewGetExtension("GL_ARB_vertex_program"))     {} else m_bGPUSupport = false;

    if (m_bGPUSupport)
    {
        m_pJTrace->info("We have ARB gpu support");

        jprefs.setGpusupport(true);

		GLint max_fragment_instructions;
		GLint max_vertex_instructions;

        glGetProgramivARB(  GL_FRAGMENT_PROGRAM_ARB,
            GL_MAX_PROGRAM_INSTRUCTIONS_ARB,
            &max_fragment_instructions);

        glGetProgramivARB(  GL_VERTEX_PROGRAM_ARB,
            GL_MAX_PROGRAM_INSTRUCTIONS_ARB,
            &max_vertex_instructions);

        jprefs.setMaxVertexInstructionsARB(max_vertex_instructions);
        jprefs.setMaxFragmentInstructionsARB(max_fragment_instructions);
    }

    bool nv_m_bGPUSupport = true;

    if (GL_TRUE == glewGetExtension("GL_NV_fragment_program"))    {} else nv_m_bGPUSupport = false;
    if (GL_TRUE == glewGetExtension("GL_NV_vertex_program"))      {} else nv_m_bGPUSupport = false;

    if (nv_m_bGPUSupport)
    {
        m_pJTrace->info("We have Nvidia gpu support");

        jprefs.setNVGpusupport(true);  //change back to true
    }

	//check the prefs for compatability
    jstats.setNVsupport ( nv_m_bGPUSupport );
    jstats.setARBsupport ( m_bGPUSupport );

}

void 
GLAnime::initalizePlayerLibraries()
{
	/////////////////////////////////////////////////////////////////
    //ok lets load the plugins in now...
    m_pJTrace->info( ">Initializing Plugins");
    JahPluginLib* m_pJPlugIn = JahPluginLib::getInstance();
    m_pJPlugIn->initializePlugins();
    m_pJTrace->info( ">Initialized Plugins");

	/////////////////////////////////////////////////////////////////
    //initialize the openobjectlib 
    m_pJTrace->info( ">Initializing OpenObjectLib");
	OpenObjectLib* oolib = OpenObjectLib::getInstance();
	oolib->initializePlugins();
	oolib->listPlugins();
    m_pJTrace->info( ">Initialized OOlib Plugins");
}
