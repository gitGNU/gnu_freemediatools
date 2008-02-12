/*********************************************************************NVMH3****

Copyright NVIDIA Corporation 2002
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
*AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


Comments:

******************************************************************************/

//
// demo.cpp
//
// Basic example of the use of the Cg runtime in a simple OpenGL
// program
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef WIN32
# include <windows.h>
#endif
#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glext.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glut.h>
# include <GL/glext.h>
#endif
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#ifndef CWD
# define CWD ""
#endif


// Global variables: hold the Cg context that we're storing our programs
// in as well as handles to the vertex and fragment program used in this
// demo.

CGcontext context;
CGprogram vertexProgram, fragmentProgram;

// Forward declarations of functions

static void keyboard(unsigned char key, int x, int y);
static void display();
static void handleCgError();
static void DrawGeometry();
static void ParametricEval(float u, float v, GLfloat *P, GLfloat *N);
static void LoadTextures();
static void LoadCgPrograms();
static void ChooseProfiles();

///////////////////////////////////////////////////////////////////////////

// Main program; do basic GLUT and Cg setup, but leave most of the work
// to the display() function.

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Jahshaka basic");

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);

    // Basic Cg setup; register a callback function for any errors
    // and create an initial context
    cgSetErrorCallback(handleCgError);
    context = cgCreateContext();

    // and all the rest happens in the display function...
    glutMainLoop();

    return 0;
}


// display callback function

static void display() 
{
    static int curTime = 0;
	GLfloat						texture_width = (float)512;
	GLfloat						texture_height = (float)512;


    static bool haveLoadedPrograms = false;
    if (!haveLoadedPrograms) {
	// Do one-time setup only once; setup Cg programs and textures
	// and set up OpenGL state.
        ChooseProfiles();
        LoadCgPrograms();
        LoadTextures();

        glEnable(GL_DEPTH_TEST);
        haveLoadedPrograms = true;
    }

    // The usual OpenGL stuff to clear the screen and set up viewing.
    glClearColor(.25, .25, .25, 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80., 1.0, .1, 3000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //gluLookAt(1, 1, 10, 0, 0, 0, 0, 0, 1);
	//gluLookAt(0.01, 0.5, 3, 0, 0, 0, 0, 1, 0);

    // Make the object rotate a bit each time the display function
    // is called
    glRotatef(curTime/10, 0, 0, 1);

	glTranslatef(0, 0, -840.0);

// 	float					   float_matrix[4][4];

// 	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&float_matrix[0][0]);

// 	printf("\nGL_MODELVIEW_MATRIX\n");

// 	for (int ii = 0; ii < 4; ii++)
// 	{
// 		printf("%f %f %f %f\n", 
// 			float_matrix[ii][0],
// 			float_matrix[ii][1],
// 			float_matrix[ii][2],
// 			float_matrix[ii][3]);
// 	}


    // Now make sure that the vertex and fragment programs, loaded
    // in LoadCgPrograms() are bound.
    cgGLBindProgram(vertexProgram);
    cgGLBindProgram(fragmentProgram);

    // Bind uniform parameters to vertex shader
    cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram, "ModelViewProj"),
                                CG_GL_MODELVIEW_PROJECTION_MATRIX,
                                CG_GL_MATRIX_IDENTITY);
    cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram, "ModelView"),
                                CG_GL_MODELVIEW_MATRIX,
                                CG_GL_MATRIX_IDENTITY);
    cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram, "ModelViewIT"),
                                CG_GL_MODELVIEW_MATRIX,
                                CG_GL_MATRIX_INVERSE_TRANSPOSE);

    cgGLSetParameter1fv(cgGetNamedParameter(vertexProgram, "texture_width"), &texture_width);
    cgGLSetParameter1fv(cgGetNamedParameter(vertexProgram, "texture_height"), &texture_height);
	
    float lightPos[3] = { 1600, 1500, 900 };
    cgGLSetParameter3fv(cgGetNamedParameter(fragmentProgram, "light_position"), lightPos);
    float lightColor[3] = { 1, 1, 1 };
    cgGLSetParameter3fv(cgGetNamedParameter(fragmentProgram, "light_color"), lightColor);



    // We can also go ahead and bind varying parameters to vertex shader
    // that we just want to have the same value for all vertices.  The
    // vertex shader could be modified so that these were uniform for
    // better efficiency, but this gives us flexibility for the future.

    // And finally, enable the approprate texture for fragment shader; the
    // texture was originally set up in LoadTextures().
    cgGLEnableTextureParameter(cgGetNamedParameter(fragmentProgram, "texture"));
    // And go ahead and draw the scene geometry
    DrawGeometry();
    // Disable the texture now that we're done with it.
    cgGLDisableTextureParameter(cgGetNamedParameter(fragmentProgram, "texture"));

    glutSwapBuffers();
    ++curTime;
    // Force another display(), so that the object keeps moving
    glutPostRedisplay();
}


// Choose the vertex and fragment profiles to use.  Try to use
// CG_PROFILE_ARBVFP1 and CG_PROFILE_ARBFP1, depending on hardware support.
// If those aren't available, fall back to CG_PROFILE_VP30 and
// CG_PROFILE_FP30, respectively.

CGprofile vertexProfile, fragmentProfile;

static void ChooseProfiles()
{
    // Make sure that the appropriate profiles are available on the
    // user's system.
    if (cgGLIsProfileSupported(CG_PROFILE_ARBVP1))
        vertexProfile = CG_PROFILE_ARBVP1;
    else {
        // try VP30
        if (cgGLIsProfileSupported(CG_PROFILE_VP30))
            vertexProfile = CG_PROFILE_VP30;
        else {
            fprintf(stderr, "Neither arbvp1 or vp30 vertex profiles supported on this system.\n");
            exit(1);
        }
    }

    if (cgGLIsProfileSupported(CG_PROFILE_ARBFP1))
        fragmentProfile = CG_PROFILE_ARBFP1;
    else {
        // try FP30
        if (cgGLIsProfileSupported(CG_PROFILE_FP30))
            fragmentProfile = CG_PROFILE_FP30;
        else {
            fprintf(stderr, "Neither arbfp1 or fp30 fragment profiles supported on this system.\n");
            exit(1);
        }
    }
}


static void LoadCgPrograms()
{
    assert(cgIsContext(context));

    // Load and compile the vertex program from jahshaka_basic_vert.cg; hold on to the
    // handle to it that is returned.
    vertexProgram = cgCreateProgramFromFile(context, CG_SOURCE, CWD "jahshaka_basic_vert.cg",
                                            vertexProfile, NULL, NULL);
    if (!cgIsProgramCompiled(vertexProgram))
        cgCompileProgram(vertexProgram);

    // Enable the appropriate vertex profile and load the vertex program.
    cgGLEnableProfile(vertexProfile);
    cgGLLoadProgram(vertexProgram);

    // And similarly set things up for the fragment program.
    fragmentProgram = cgCreateProgramFromFile(context, CG_SOURCE, CWD "jahshaka_basic_frag.cg",
                                              fragmentProfile, NULL, NULL);
    if (!cgIsProgramCompiled(fragmentProgram))
        cgCompileProgram(fragmentProgram);

    cgGLEnableProfile(fragmentProfile);
    cgGLLoadProgram(fragmentProgram);
}

static void LoadTextures()
{
    // There is only one texture needed here--we'll set up a basic
    // checkerboard--which is used to modulate the diffuse channel in the
    // fragment shader.
    GLuint handle;
    glGenTextures(1, &handle);

    // Basic OpenGL texture state setup
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Fill in the texture map.
#define RES 512
    float *data = new float[RES*RES*4];
    float *dp = data;
    for (int i = 0; i < RES; ++i) {
        for (int j = 0; j < RES; ++j) {
            if ((i/32+j/32) % 2) {
                *dp++ = .7;
                *dp++ = .7;
                *dp++ = .7;
            }
            else {
                *dp++ = .1;
                *dp++ = .1;
                *dp++ = .1;
            }
	    *dp++ = 1.;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RES, RES, 0, GL_RGBA, GL_FLOAT, data);
    delete[] data;

    // Tell Cg which texture handle should be associated with the sampler2D
    // parameter to the fragment shader.
    cgGLSetTextureParameter(cgGetNamedParameter(fragmentProgram, "texture"),
                            handle);
}

// Geometry creation and drawing function; we'll just draw a sphere.

static void DrawGeometry()
{
    // Cache the sphere positions, normals, texture coordinates, and 
    // vertex indices in a local array; we only need to fill them in the
    // first time through this function.
    static GLfloat *P, *N, *uv;
    static GLuint *indices;
    int nu  = 30, nv = 30;
    int nTris = 2*(nu-1)*(nv-1), nVerts = nu*nv;
    if (!P) {
	int u, v;

        P = new GLfloat[3*nVerts];
        N = new GLfloat[3*nVerts];
        uv = new GLfloat[2*nVerts];

	// Fill in the position, normal, and texture coordinate arrays.
	// Just loop over all of the vertices, compute their parametreic
	// (u,v) coordinates (which we use for texture coordinates as
	// well), and call the ParametricEval() function, which turns (u,v)
	// coordinates into positions and normals on the surface of the
	// object.
        GLfloat *pp = P, *np = N, *uvp = uv;
        for (v = 0; v < nv; ++v) {
            float fv = (float)v / (float)(nv-1);
            for (u = 0; u < nu; ++u) {
                float fu = (float)u / (float)(nu-1);
                uvp[0] = fu;
                uvp[1] = fv;
                ParametricEval(fu, fv, pp, np);
                pp += 3;
                np += 3;
                uvp += 2;
            }
        }

	// Now fill in the vertex index arrays
        indices = new GLuint[3*nTris];
        GLuint *ip = indices;
        for (v = 0; v < nv-1; ++v) {
            for (u = 0; u < nu-1; ++u) {
#define VERTEX(u, v) ((u) + (v) * nu)
                *ip++ = VERTEX(u, v);
                *ip++ = VERTEX(u+1, v);
                *ip++ = VERTEX(u+1, v+1);

                *ip++ = VERTEX(u, v);
                *ip++ = VERTEX(u+1, v+1);
                *ip++ = VERTEX(u, v+1);
#undef VERTEX
            }
        }


	// Tell Cg which of these data pointers are associated with which
	// parameters to the vertex shader, so that when we call
	// cgGLEnableClientState() and then glDrawElements(), the shader
	// gets the right input information.
	CGparameter param = cgGetNamedParameter(vertexProgram, "object_position");
	cgGLSetParameterPointer(param, 3, GL_FLOAT, 0, P);
	param = cgGetNamedParameter(vertexProgram, "TexUV");
	cgGLSetParameterPointer(param, 2, GL_FLOAT, 0, uv);
    }

    // And now, each time through, enable the bindings to the parameters
    // that we set up the first time through
    CGparameter param = cgGetNamedParameter(vertexProgram, "object_position");
    cgGLEnableClientState(param);
    param = cgGetNamedParameter(vertexProgram, "TexUV");
    cgGLEnableClientState(param);

    // Enable the texture parameter as well.
    param = cgGetNamedParameter(fragmentProgram, "texture");
    cgGLEnableTextureParameter(param);

    // And now, draw the geometry.
    glDrawElements(GL_TRIANGLES, 3*nTris, GL_UNSIGNED_INT, indices);

    // Be a good citizen and disable the various bindings we set up above.
    param = cgGetNamedParameter(vertexProgram, "object_position");
    cgGLDisableClientState(param);
    param = cgGetNamedParameter(vertexProgram, "TexUV");
    cgGLDisableClientState(param);

    param = cgGetNamedParameter(fragmentProgram, "texture");
    cgGLDisableTextureParameter(param);
}

#define MAG			256
static void ParametricEval(float u, float v, GLfloat *P, GLfloat *N)
{
#define PI 3.1415926535f
    float theta = PI * u, phi = 2. * PI * v;
    P[0] = sin(theta) * sin(phi);
    P[1] = sin(theta) * cos(phi);
    P[2] = cos(theta);

    N[0] = P[0];
    N[1] = P[1];
    N[2] = P[2];

	P[0] = (P[0] * (float)MAG);
	P[1] = (P[1] * (float)MAG);
	P[2] = (P[2] * (float)MAG);

	//printf("P = %8.3f %8.3f %8.3f\n", P[0], P[1], P[2]);
}


static void keyboard(unsigned char key, int x, int y)
{
    if (key == 'q' || key == 'Q' || key == 27 /* escape */) {
        cgDestroyContext(context);
        exit(0);
    }
}

static void handleCgError() 
{
    fprintf(stderr, "Cg error: %s\n", cgGetErrorString(cgGetError()));
    exit(1);
}
