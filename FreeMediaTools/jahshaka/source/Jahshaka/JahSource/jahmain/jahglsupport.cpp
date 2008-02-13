/*******************************************************************************
**
** The source file for the Jahshaka Jah module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahshaka.h"

///////////////////////////////////////////////
// messy hack here uses startupscreen to read extensions
//maybe startup could return them instead?

void Jahshaka::checkJahExtensions(void)
{
	//////////////////////////////////////////////////
	// if we use GLEW to do this then we dont need
	// to pull it from the startup screen...
	// and it would work wiht jahplayer as well

    jtrace->info( "Checking the OpenGL Extensions");

    //print out the gl extensions for debugging purposes
    //need to clean this up and put it into a routine
    
	//i beleive we can get all this from GLEW when its initalized right?
	vendor     = new QString (*thestartupscreen->vendor);
    renderer   = new QString (*thestartupscreen->renderer);
    version    = new QString (*thestartupscreen->version);
    extensions = new QString (*thestartupscreen->extensions);

    std::string vendor_string( vendor->ascii() );
    std::string renderer_string( renderer->ascii() != NULL ? renderer->ascii() : "" );
    std::string version_string( version->ascii() );

    Globals::setGlVendor(vendor_string);
    Globals::setGlRenderer(renderer_string);
    Globals::setGlVersion(version_string);

    jtrace->debug( "Vendor    :"+*vendor);
    jtrace->debug( "Renderer  :"+*renderer);
    jtrace->debug( "Version   :"+*version);

    GLint max_texture_size; 
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    Globals::setGlMaxtextureSize(max_texture_size);

    QString glewinitstring;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        //problem: glewInit failed, something is seriously wrong
		jtrace->debug( "Glew Error");
        glewinitstring.sprintf("Error: %s\n", glewGetErrorString(err));
		jtrace->debug(glewinitstring);
    }
    else
    {
		jtrace->debug( "Glew Initialized");
        glewinitstring.sprintf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		jtrace->debug(glewinitstring);
    }

    //does a complete listing of extensions
    jtrace->debug( "Extensions:"+*extensions);
	
    //added to record stats data
    //added this to record the stats data
    JahStats& jstats = JahStats::getInstance();

	//check the prefs for compatability
    jstats.setVendor ( vendor_string );
    jstats.setVersion ( version_string );
    jstats.setRenderer ( renderer_string );

    //now we can move on to more advanced tests and configuration
	enableJahExtensions();

}

void Jahshaka::enableJahExtensions(void)
{
    jtrace->info( "Enabling OpenGL Extensions");

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    //check GLEW version

    if (GLEW_VERSION_1_3)
    {
      jtrace->debug("OpenGL 1.3 is supported!");
    }


    //In general, you can check if GLEW_{extension_name} or GLEW_VERSION_{version} is set or not.
    //For extensions glewGetExtension provides a slower alternative: 

    if (GL_TRUE == glewGetExtension("GL_ARB_fragment_program"))
    {
      jtrace->info("ARB_fragment_program is supported.");
    }

    //////////////////////////////////////////////////////////
    // other extensions

    if (GL_TRUE == glewGetExtension("GL_ARB_imaging"))
    {
      jtrace->info("GL_ARB_imaging is supported.");
    }

    if (GL_TRUE == glewGetExtension("GL_ARB_multisample"))
    {
      jtrace->info("GL_ARB_multisample is supported.");
      jprefs.setHwAliasing(true);
    }

    if (GL_TRUE == glewGetExtension("GL_EXT_fog_coord"))
    {
      jtrace->info("GL_EXT_fog_coord is supported.");
      jprefs.setHwFog(true);
    }

    //////////////////////////////////////////////////////////
    //check for simple shader support
    bool simpleshaders = true;

    if (GL_TRUE == glewGetExtension("GL_NV_fragment_program"))  {}   else simpleshaders = false;
    if (GL_TRUE == glewGetExtension("GL_NV_vertex_program"))    {}   else simpleshaders = false;

    if (simpleshaders)
    {
        jtrace->info("We have simple shader support");
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
        jtrace->info("We have opengl shader support");
        //jprefs.setVShading(true);
    }

    //////////////////////////////////////////////////////////
    //check for gpu and gpgpu support
    bool gpusupport = true;

    if (GL_TRUE == glewGetExtension("GL_ARB_fragment_program"))   {} else gpusupport = false;
    if (GL_TRUE == glewGetExtension("GL_ARB_vertex_program"))     {} else gpusupport = false;

    if (gpusupport)
    {
        jtrace->info("We have ARB gpu support");

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

    bool nv_gpusupport = true;

    if (GL_TRUE == glewGetExtension("GL_NV_fragment_program"))    {} else nv_gpusupport = false;
    if (GL_TRUE == glewGetExtension("GL_NV_vertex_program"))      {} else nv_gpusupport = false;

    if (nv_gpusupport)
    {
        jtrace->info("We have Nvidia gpu support");

        jprefs.setNVGpusupport(true);  //change back to true
    }

    //added this to record the stats data
    JahStats& jstats = JahStats::getInstance();

	//check the prefs for compatability
    jstats.setNVsupport ( nv_gpusupport );
    jstats.setARBsupport ( gpusupport );


}


