/*******************************************************************************
**
** The source file for the Jahshaka core 3dobject file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahrender.h"

#include <p_vector.h>


void JahRender::makeParticle() {

        //glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); //first we correct the orientation

        glTranslatef( 0.0, 0.0, -5.0 );
        glRotatef( -90, 1.0, 0.0, 0.0 );
        glScalef ( 40, 40, 40 );

        glDisable(GL_TEXTURE_2D);
        bool ConstColor = thelayer->ParticleColors;  //otherwise it roatates the colors

        switch(thelayer->ParticleDraw)
        {
            case ParticleDrawstyle::PARLINE:      pDrawGroupp(GL_LINES, true, ConstColor);          break;
            case ParticleDrawstyle::PARPOINT:     pDrawGroupp(GL_POINTS, true, ConstColor);         break;

            case ParticleDrawstyle::PARGAUSS:		{	glEnable(GL_TEXTURE_2D); 
									thelayer->MakeGaussianSpotTexture();
                                    pVector view = pVector(0, 0, 0) - pVector(0, -23.0, 10);
                                    view.normalize();
                                    pVector up(0, 0, 1);
                                    DrawGroupQuadSplat(view, up, 0.16f, true, true, ConstColor); break; }

            case ParticleDrawstyle::PARTEXTURE:	{	glEnable(GL_TEXTURE_2D);
                                    thelayer->makeLayerTexture();
                                    pVector view = pVector(0, 0, 0) - pVector(0, -23.0, 10);
                                    view.normalize();
                                    pVector up(0, 0, 1);
                                    DrawGroupQuadSplat(view, up, 0.16f, true, true, ConstColor); break; }
			case ParticleDrawstyle::NOT_A_TYPE: break;
        }

        switch(thelayer->ParticleStyle)
        {
            case ParticleCategory::ATOMIC:       Atom(thelayer->ParticleFirsttime);          break;
            case ParticleCategory::JETSPRAY:     JetSpray(thelayer->ParticleFirsttime);      break;
            case ParticleCategory::WATERFALL:    Waterfall(thelayer->ParticleFirsttime);     break;
            case ParticleCategory::ROCKET:       { thelayer->SteerShape = PDPlane; 
								 Rocket(thelayer->ParticleFirsttime);      break; }
            case ParticleCategory::EXPLOSION:    Explosion(thelayer->ParticleFirsttime);     break;
            case ParticleCategory::SWIRL:        Swirl(thelayer->ParticleFirsttime);         break;
            case ParticleCategory::BALLOONS:     Balloons(thelayer->ParticleFirsttime);      break;
            case ParticleCategory::SHAPE:        Shape(thelayer->ParticleFirsttime);         break;
            case ParticleCategory::FIREFLIES:    FireFlies(thelayer->ParticleFirsttime) ;    break;
            case ParticleCategory::SHOWER:       { thelayer->SteerShape = PDPlane; 
								 Shower(thelayer->ParticleFirsttime);      break; }
            case ParticleCategory::FOUNTAIN:     Fountain(thelayer->ParticleFirsttime);      break;
			case ParticleCategory::NOT_A_TYPE: break;
        }

        glEnable(GL_TEXTURE_2D);

	thelayer->ParticleFirsttime=FALSE;

	//pDrawGroupp(GL_LINES, true);

	glPopMatrix();

}


void JahRender::addParticleObj() 
{
    jtrace->info( "JahLayer::addParticleObj","Creating particle object");

    // we need to make sure this is done only once...
    //particle_handle;
    thelayer->action_handle   =-1;
    thelayer->maxParticles    = 20000;
    thelayer->demoNum         = 10;
    thelayer->numSteps        = 1;
    thelayer->prim            = GL_LINES;
    thelayer->listID          = -1;
    thelayer->SpotTexID       = -1;
    thelayer->BlurRate        = 0.09f;
    thelayer->SteerShape      = PDSphere;

    //do we need this? or just in the routines when called between changes
    thelayer->ParticleFirsttime=TRUE;

    //is this 1000 particles?
    thelayer->particle_handle = pGenParticleGroups(1, 1000);
    pCurrentGroup(thelayer->particle_handle);

}


void JahRender::Atom(bool first_time = true)
{
    if(first_time && thelayer->action_handle<0) 
    { 
        thelayer->action_handle = pGenActionLists(1);
    //	pGenParticleGroups(1, 100);
    }

    pVelocityD(PDBlob, 0.02f, -0.2f, 0, 0.015f);
    partSize(1.0);

    if(first_time)
        pNewActionList(thelayer->action_handle);

    static float jetx = -4.0f, jety = 0.0f, jetz = -2.4f;

    static float djx = drand48() * 0.05f;
    static float djy = drand48() * 0.05f;
    static float djz = drand48() * 0.05f;

    jetx += djx;
    jety += djy;
    jetz += djz;

    if(jetx > 10 || jetx < -10) djx = -djx;
    if(jety > 10 || jety < -10) djy = -djy;
    if(jetz > 10 || jetz < -10) djz = -djz;

    pCopyVertexB(false, true);

    int LifeTime = 350;

    pKillOld(LifeTime);

    pColorD(1, PDSphere, 0.4f + fabs(jetx*0.1f), 0.4f + fabs(jety*0.1f), 0.4+fabs(jetz*0.1f), 0.1f);
    pSource(thelayer->maxParticles / LifeTime, PDPoint, jetx, jety, jetz);

    pOrbitPoint(2, 0, 3, 0.1, 0.1);

    pOrbitPoint(-2, 0, -3, 0.1, 0.1);

    pDamping(0.994, 0.994, 0.994, 0.2);

    //pSink(false, PDSphere, 0, 0, 0, 15);

    pMove();

    if(first_time)
        pEndActionList();
}

void JahRender::JetSpray(bool)
{
    static pVector Jet;
    static pVector dJet;
    static bool FirstTime = true;

    if(FirstTime)
    {
        FirstTime = false;
        double Ang = drand48() * 2.0 * M_PI;
        dJet = pVector(cos(Ang), sin(Ang), 0);
        dJet *= 0.1;
    }
    Jet += dJet;
    if(Jet.x > 10 || Jet.x < -10) {dJet.x = -dJet.x; dJet.y += drand48() * 0.005;}
    if(Jet.y > 10 || Jet.y < -10) {dJet.y = -dJet.y; dJet.x += drand48() * 0.005;}

    pColorD(1.0, PDLine, 1,0,0, 1,1,0.3);

    pVelocityD(PDBlob, 0,0,0.1, 0.01);
    pStartingAge(0);
    partSize(1);

    int LifeTime = 100;

    pKillOld(LifeTime);

    pSource(thelayer->maxParticles / LifeTime, PDDisc, Jet.x, Jet.y, Jet.z, 0, 0, 1, 0.5);

    pGravity(.000, .01, .000);

    pDamping(0.9, 0.97, 0.9);

    float BOX = .017;

    pRandomAccel(PDBox, -BOX, -BOX, -BOX, BOX, BOX, BOX);

    pMove();
}


void JahRender::Waterfall(bool first_time = true)
{
    pVelocityD(PDBlob, 0.1, 0, 0.1, 0.004);
    pColorD(1.0, PDLine, 0.8, 0.9, 1.0, 1.0, 1.0, 1.0);
    partSize(1.5);
    pStartingAge(0);

    if(first_time)
    {
        thelayer->action_handle = pGenActionLists(1);
        pNewActionList(thelayer->action_handle);
    }

    pCopyVertexB(false, true);

    pSource(50, PDPoint, -4, 0, 6);

    pGravity(0.0, 0.0, -0.01);

    pKillOld(250);

    pBounce(0, 0.01, 0, PDSphere, -1, 0, 4, 1);

    pBounce(0, 0.01, 0, PDSphere, -2.5, 0, 2, 1);

    pBounce(0, 0.01, 0, PDSphere, 0.7, -0.5, 2, 1);

    pBounce(-0.01, 0.35, 0, PDPlane, 0,0,0, 0,0,1);

    pMove();

    if(first_time)
        pEndActionList();
}

void JahRender::Rocket(bool )
{
#define MAX_ROCKETS 15
    static int RocketSystem = -1;

    if(RocketSystem < 0)
        RocketSystem = pGenParticleGroups(1, MAX_ROCKETS);

    // Move the rockets.
    pCurrentGroup(RocketSystem);
    pVelocityD(PDCylinder, 0,0,0.15, 0,0,0.2, 0.11, 0.07);
    pColorD(1, PDBox, 0,0.5,0, 1,1,1);

    pSource(0.02, PDDisc, 0,0,0, 0,0,1, 6);
    pSink(false, PDPlane, 0,0,-1, 0,0,1);
    pGravity(0,0,-0.003);
    pMove();

    // Read back the position of the rockets.
    float rocketp[MAX_ROCKETS][3], rocketc[MAX_ROCKETS][3], rocketv[MAX_ROCKETS][3];
    int rcount = pGetParticles(0, MAX_ROCKETS, (float *)rocketp, (float *)rocketc, (float *)rocketv);

    pCurrentGroup(thelayer->particle_handle);

    partSize(1.0);
    pStartingAge(0, 6);

    for(int i=0; i<rcount; i++)
    {
        pVector rv(rocketv[i][0], rocketv[i][1], rocketv[i][2]);
        rv.normalize();
        rv *= -0.026;
        //cerr << i << " " <<rocketp[i][0]<<" "<<rocketp[i][1]<<" "<<rocketp[i][2]<<"\n";
        //cerr << "c " <<rocketc[i][0]<<" "<<rocketc[i][1]<<" "<<rocketc[i][2]<<"\n";

        pColorD(1.0, PDLine, rocketc[i][0], rocketc[i][1], rocketc[i][2], 1,.5,.5);
        pVelocityD(PDBlob, rv.x, rv.y, rv.z, 0.006);
        pSource(30, PDPoint, rocketp[i][0], rocketp[i][1], rocketp[i][2]);
    }

    pGravity(0,0,-0.001);
    //pDamping(0.999, 0.999, 0.999);
    pTargetColor(0,1,0,0, 0.02);
    pKillOld(90);

    pMove();
}

void JahRender::Balloons(bool)
{
    float x=0, y=0, z=-1;
    float qty = thelayer->maxParticles / 6.0;	// Because of 6 colors

    float BBOX = 0.5;

    pStartingAge(0, 5);
    pVelocity(0,0,0);
    partColor(1,0,0);
    pSource(qty, PDBox, x-BBOX, y-BBOX, z-BBOX, x+BBOX, y+BBOX, z+BBOX);

    partColor(1,1,0);
    pSource(qty, PDBox, x-BBOX, y-BBOX, z-BBOX, x+BBOX, y+BBOX, z+BBOX);

    partColor(0,1,0);
    pSource(qty, PDBox, x-BBOX, y-BBOX, z-BBOX, x+BBOX, y+BBOX, z+BBOX);

    partColor(0,1,1);
    pSource(qty, PDBox, x-BBOX, y-BBOX, z-BBOX, x+BBOX, y+BBOX, z+BBOX);

    partColor(0,0,1);
    pSource(qty, PDBox, x-BBOX, y-BBOX, z-BBOX, x+BBOX, y+BBOX, z+BBOX);

    partColor(1,0,1);
    pSource(qty, PDBox, x-BBOX, y-BBOX, z-BBOX, x+BBOX, y+BBOX, z+BBOX);

    pGravity(.0005, .005, .0005);

    pDamping(0.9, 0.67, 0.9);

    float BOX = .005;

    pRandomAccel(PDBox, -BOX, -BOX, -BOX, BOX, BOX, BOX);
    pKillOld(400);

    pMove();
    pStartingAge(0);
}


void JahRender::Shape(bool first_time = true)
{
    if(first_time)
    {
        pKillOld(0);
        int dim = int(pow(float(thelayer->maxParticles), (float)0.33333333));
#define XX 8
#define YY 12
#define ZZ 8

        float dx = 2*XX / float(dim);
        float dy = 2*YY / float(dim);
        float dz = 2*ZZ / float(dim);

        float z = -ZZ;
        for(int k=0; k<dim; k++, z += dz)
        {
            float y = -YY;
            for(int l=0; l<dim; l++, y += dy)
            {
                float x = -XX;
                for(int j=0; j<dim; j++, x += dx)
                {
                    // Make the particles.
                    pVelocityD(PDBlob, 0, 0, 0, 0.01);
                    partColor(0.5 + z*0.05, 0.5 + x*0.05, 0.5 + x*0.05);
                    pVertex(x, y, z);

                }
            }
        }

        thelayer->action_handle = pGenActionLists(1);
        pNewActionList(thelayer->action_handle);
        pEndActionList();
    }
}



void JahRender::Explosion(bool first_time = true)
{
    pVelocityD(PDSphere, 0,0,0,0.01,0.01);
    pColorD(1.0, PDSphere, 0.5, 0.7, 0.5, .3);
    partSize(1.0);
    pStartingAge(0);

    pCopyVertexB(false, true);

    pDamping(0.999, 0.999, 0.999, 0.2);

    static float i=0;

    if(first_time) i=0;

    pOrbitPoint(0, 0, 0, .02, 0.1);

    pExplosion(0, 0, 0, 1, 8, 3, 0.1, i += (1.0f / float(thelayer->numSteps)));

    pSink(false, PDSphere, 0, 0, 0, 30);

    pMove();
}


void JahRender::Swirl(bool)
{
    pVelocityD(PDBlob, 0.02, -0.2, 0, 0.015);
    partSize(1.0);
    pStartingAge(0);

    static float jetx=-4, jety=0, jetz=-2.4;

    static float djx = drand48() * 0.05;
    static float djy = drand48() * 0.05;
    static float djz = drand48() * 0.05;

    jetx += djx;
    jety += djy;
    jetz += djz;

    if(jetx > 10 || jetx < -10) djx = -djx;
    if(jety > 10 || jety < -10) djy = -djy;
    if(jetz > 10 || jetz < -10) djz = -djz;

    pCopyVertexB(false, true);

    int LifeTime = 300;

    pKillOld(LifeTime);

    pColorD(1.0, PDSphere, 0.4+fabs(jetx*0.1), 0.4+fabs(jety*0.1), 0.4+fabs(jetz*0.1), 0.1);
    pSource(thelayer->maxParticles / LifeTime, PDPoint, jetx, jety, jetz);

    pOrbitLine(0, 0, 1, 1, 0, 0, 0.1, 0.1);

    pDamping(1, 0.994, 0.994);

    pSink(false, PDSphere, 0, 0, 0, 15);

    pMove();
}

void JahRender::Shower(bool first_time = true)
{
    static float jetx=0, jety=0;
    static float djx, djy;

    if(first_time)
    {
        jetx = 1;
        jety = 1;
        djx = drand48() * 0.1;
        djy = drand48() * 0.1;
    }

    jetx += djx;
    jety += djy;

    if(jetx > 1 || jetx < 0) {djx = -djx; djy += drand48() * 0.0005;}
    if(jety > 2 || jety < -2) {djy = -djy; djx += drand48() * 0.0005;}

    pVelocity(0, 0, 0);
    partSize(1.5);
    pStartingAge(0);

    partColor(jetx, jety, 1);

    pSource(120, PDLine, -5,jety,8, 5,jety,8);

    pGravity(0.0, 0.0, -0.004);

    glColor3f(1,1,0);
    if(thelayer->SteerShape == PDSphere)
    {
        pAvoid(0.2, 1.0, 20, PDSphere, 0,0,0, 1.1);

	glPushAttrib( GL_POLYGON_BIT );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	GLUquadric* sphere = gluNewQuadric( );
	gluSphere( sphere, 1, 16, 8 );
	gluDeleteQuadric( sphere );
	glPopAttrib( );
    }
    else if(thelayer->SteerShape == PDTriangle)
    {
        pAvoid(2, 1.0, 20, PDTriangle, 0,-1,0, 2,0,0, 0,2,0);

        glBegin(GL_TRIANGLES);
        glVertex3f(0,-1,0);
        glVertex3f(2,0,0);
        glVertex3f(0,2,0);
        glEnd();
    }
    else if(thelayer->SteerShape == PDRectangle)
    {
        pAvoid(2, 1.0, 20, PDRectangle, 0,-1,0, 2,1,0, 0,2,0);

        glBegin(GL_QUADS);
        glVertex3f(0,-1,0);
        glVertex3f(2,0,0);
        glVertex3f(2,2,0);
        glVertex3f(0,1,0);
        glEnd();
    }
    else if(thelayer->SteerShape == PDPlane)
    {
        pAvoid(2, 1.0, 20, PDPlane, 0,0,0, 0,0,1);

        glBegin(GL_QUADS);
        glVertex3f(-2,-2,0);
        glVertex3f(2,-2,0);
        glVertex3f(2,2,0);
        glVertex3f(-2,2,0);
        glEnd();
    }

    pKillOld(130);

    pMove();
}

// A fountain spraying up in the middle of the screen
void JahRender::FireFlies(bool)
{
    partSize(1.0);
    pVelocityD(PDPoint, 0,0,0);
    pColorD(1.0, PDSphere, .5, .4, .1, .1);
    pStartingAge(0);

    pCopyVertexB(false, true);

    pSource(1, PDBlob, 0, 0, 2, 2);

    pRandomAccel(PDSphere, 0, 0, 0, 0.02);

    pKillOld(20);

    pMove();
}



void JahRender::Fountain(bool)
{

// Set up the state.
    pVelocityD(PDCylinder, 0.01, 0.0, 0.35, 0.01, 0.0, 0.37, 0.021, 0.019);
    pColorD(1.0, PDLine, 0.8, 0.9, 1.0, 1.0, 1.0, 1.0);
    partSize(1.5);

    // Generate particles along a very small line in the nozzel.
    pSource(100, PDLine, 0.0, 0.0, 0.401, 0.0, 0.0, 0.405);

    // Gravity.
    pGravity(0.0, 0.0, -0.01);

    // Bounce particles off a disc of radius 5.
    pBounce(-0.05, 0.35, 0, PDDisc, 0, 0, 0,  0, 0, 1,  5);

    // Kill particles below Z=-3.
    pSink(false, PDPlane, 0,0,-3, 0,0,1);

    // Move particles to their new positions.
    pMove();
}




// Draw each particle as a screen-aligned triangle with texture.
// Doesn't make the texture current. Just emits texcoords, if specified.
// If size_scale is 1 and const_size is true then the textured square
// will be 2x2 in world space (making the triangle sides be 4x4).
// view and up must be normalized and unequal.

// Draw as a splat texture on a quad.
void JahRender::DrawGroupTriSplat(const pVector &view, const pVector &up,
                    float size_scale = 1.0f, bool draw_tex=false,
                    bool const_size=false, bool const_color=false)
{
    int cnt = pGetGroupCount();

    if(cnt < 1)
        return;

    pVector *ppos = new pVector[cnt];
    float *color = const_color ? NULL : new float[cnt * 4];
    pVector *size = const_size ? NULL : new pVector[cnt];

    pGetParticles(0, cnt, (float *)ppos, color, NULL, (float *)size);

    // Compute the vectors from the particle to the corners of its tri.
    // 2
    // |\ The particle is at the center of the x.
    // |-\ V0, V1, and V2 go from there to the vertices.
    // |x|\ The texcoords are (0,0), (2,0), and (0,2) respectively.
    // 0-+-1 We clamp the texture so the rest is transparent.

    pVector right = view ^ up;
    right.normalize();
    pVector nup = right ^ view;
    right *= size_scale;
    nup *= size_scale;

    pVector V0 = -(right + nup);
    pVector V1 = V0 + right * 4;
    pVector V2 = V0 + nup * 4;

    //cerr << "x " << view.x << " " << view.y << " " << view.z << endl;
    //cerr << "x " << nup.x << " " << nup.y << " " << nup.z << endl;
    //cerr << "x " << right.x << " " << right.y << " " << right.z << endl;
    //cerr << "x " << V0.x << " " << V0.y << " " << V0.z << endl;

    glBegin(GL_TRIANGLES);

    for(int i = 0; i < cnt; i++)
    {
        pVector &p = ppos[i];
        //cerr << p.x << " " << p.y << " " << p.z << endl;
        // cerr << color[i*4+3] << endl;

        if(!const_color)
            glColor4fv((GLfloat *)&color[i*4]);

        pVector sV0 = V0;
        pVector sV1 = V1;
        pVector sV2 = V2;

        if(!const_size)
        {
        sV0 *= size[i].x;
        sV1 *= size[i].x;
        sV2 *= size[i].x;
        }

        if(draw_tex) glTexCoord2f(0,0);
        pVector ver = p + sV0;
        glVertex3fv((GLfloat *)&ver);

        if(draw_tex) glTexCoord2f(2,0);
        ver = p + sV1;
        glVertex3fv((GLfloat *)&ver);

        if(draw_tex) glTexCoord2f(0,2);
        ver = p + sV2;
        glVertex3fv((GLfloat *)&ver);
    }

    glEnd();

    delete [] ppos;
    if(color) delete [] color;
    if(size) delete [] size;
}

// Draw as a splat texture on a quad.
void JahRender::DrawGroupQuadSplat(const pVector &view, const pVector &up,
                    float size_scale = 1.0f, bool draw_tex=false,
                    bool const_size=false, bool const_color=false)
{
    int cnt = pGetGroupCount();

    if(cnt < 1)
        return;

    pVector *ppos = new pVector[cnt];
    float *color = const_color ? NULL : new float[cnt * 4];
    pVector *size = const_size ? NULL : new pVector[cnt];

    pGetParticles(0, cnt, (float *)ppos, color, NULL, (float *)size);

    // Compute the vectors from the particle to the corners of its quad.
    // The particle is at the center of the x.
    // 3-2 V0, V1, V2 and V3 go from there to the vertices.
    // |x| The texcoords are (0,0), (1,0), (1,1), and (0,1) respectively.
    // 0-1 We clamp the texture so the rest is transparent.

    pVector right = view ^ up;
    right.normalize();
    pVector nup = right ^ view;
    right *= size_scale;
    nup *= size_scale;

    pVector V0 = -(right + nup);
    pVector V1 = right - nup;
    pVector V2 = right + nup;
    pVector V3 = nup - right;

    //cerr << "x " << view.x << " " << view.y << " " << view.z << endl;
    //cerr << "x " << nup.x << " " << nup.y << " " << nup.z << endl;
    //cerr << "x " << right.x << " " << right.y << " " << right.z << endl;
    //cerr << "x " << V0.x << " " << V0.y << " " << V0.z << endl;

    glBegin(GL_QUADS);

    for(int i = 0; i < cnt; i++)
    {
        pVector &p = ppos[i];
        //cerr << p.x << " " << p.y << " " << p.z << endl;
        // cerr << color[i*4+3] << endl;

        if(!const_color)
            glColor4fv((GLfloat *)&color[i*4]);

        pVector sV0 = V0;
        pVector sV1 = V1;
        pVector sV2 = V2;
        pVector sV3 = V3;

        if(!const_size)
        {
        sV0 *= size[i].x;
        sV1 *= size[i].x;
        sV2 *= size[i].x;
        sV3 *= size[i].x;
        }

        if(draw_tex) glTexCoord2f(0,0);
        pVector ver = p + sV0;
        glVertex3fv((GLfloat *)&ver);

        if(draw_tex) glTexCoord2f(1,0);
        ver = p + sV1;
        glVertex3fv((GLfloat *)&ver);

        if(draw_tex) glTexCoord2f(1,1);
        ver = p + sV2;
        glVertex3fv((GLfloat *)&ver);

        if(draw_tex) glTexCoord2f(0,1);
        ver = p + sV3;
        glVertex3fv((GLfloat *)&ver);
    }

    glEnd();

    delete [] ppos;
    if(color) delete [] color;
    if(size) delete [] size;
}


