// p_vector.h - yet another vector class.
//
// Copyright 1997 by Jonathan P. Leech
// Modifications Copyright 1997-1999 by David K. McAllister
//
// A simple 3D float vector class for internal use by the particle systems.

#ifndef particle_vector_h
#define particle_vector_h

#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433f
#endif

#ifdef WIN32
#define drand48() (((float) rand())/((float) RAND_MAX))
#define srand48(x) srand(x)

// This is because their stupid compiler thinks it's smart.
//#define inline __forceinline
#endif

/* can we put this in macosx_math.h ? */
#ifdef __APPLE__
#define drand48() (((float) rand())/((float) RAND_MAX))
#define srand48(x) srand(x)
#endif

class pVector
{
public:
	float x, y, z;
	pVector(float ax, float ay, float az);
	pVector();
	float length() const;
	float length2() const;
	float normalize();
	float operator*(const pVector &a) const;
	pVector operator*(const float s) const;
	pVector operator/(const float s) const;
	pVector operator+(const pVector& a) const;
	pVector operator-(const pVector& a) const;
	pVector operator-();
	pVector& operator+=(const pVector& a);
	pVector& operator-=(const pVector& a);
	pVector& operator*=(const float a);
	pVector& operator/=(const float a);
	pVector& operator=(const pVector& a);
	pVector operator^(const pVector& b) const;
};

#endif
