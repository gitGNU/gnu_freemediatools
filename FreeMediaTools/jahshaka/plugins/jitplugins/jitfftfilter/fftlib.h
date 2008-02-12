/*----------------------------------------------------------------------
					Based on code from
					J. R. Parker
					Laboratory for Computer Vision
					University of Calgary
					Calgary, Alberta, Canada

					Updated by
					The Jahshaka Project
  ----------------------------------------------------------------------*/
#ifndef FFT_MAX_H
#define FFT_MAX_H

#include "blurlib.h"

#define FFT_MAX

typedef float** COMPLEX_IMAGE;

//********************************************************************************************//
class  Complex_c  
//********************************************************************************************//
{	// Complex variable type 
  public:
	float 					real;
	float 					imag;
};


//#if defined(FFT_MAX)
/*
#else
*/
extern double*				sintabpt;
extern int*					bittabpt;
extern int*					powers;
extern int					numpts;						// Size of FFT 
extern int  				nn;							// 2^nn = numpts 
extern float 				direction;					// Direction of FFT 
extern double  				scalef;
extern int 					FFTN;
extern int 					NORMALIZE;
//#endif
//
//  Header File for Fast Fourier Transform library routine 
#define     FORWARD     -1.0		// Perform forward FFT 
#define     REVERSE      1.0		// Perform reverse FFT 

//********************************************************************************************//
class FFT_c
//********************************************************************************************//
{
  public:
	COMPLEX_IMAGE 					dupcomplex(COMPLEX_IMAGE im);
	void 							freecomplex(COMPLEX_IMAGE x);
	COMPLEX_IMAGE 					newcomplex(Image_Data_c* im);
	void 							image_fft(Image_Data_c* image, COMPLEX_IMAGE *output);
	void 							image_fftoc(Image_Data_c* image, COMPLEX_IMAGE *output);
	void 							fftx(Image_Data_c* image, int oc, COMPLEX_IMAGE *output);
	void 							image_fftinv(COMPLEX_IMAGE image, COMPLEX_IMAGE * output);
	void 							normalize_set();
	void 							normalize_clear();
	void 							image_fftinvoc(COMPLEX_IMAGE image, COMPLEX_IMAGE * output);
	void 							fftinvx(COMPLEX_IMAGE image, int oc, COMPLEX_IMAGE *output);
	void 							cprod(float c1r, float c1i, float *c2r, float *c2i);
	float 							pix_cnorm(float cr, float ci);
	void 							cdiv(float c1r, float c1i, float *c2r, float *c2i);
	void 							fft2d(COMPLEX_IMAGE image, float direction );
	void 							filt_orig( COMPLEX_IMAGE array );
	void 							filt_toint( COMPLEX_IMAGE image , Image_Data_c* output, float *h);
	int 							vlog2(int x);
	void 							fft( float data[], float dir );
	static int 						_fft( float tseries[], int level, int chunk );
	void 							fftinit( int nopts );
	static int 						bitrev( int bits );
	void 							realtoint(COMPLEX_IMAGE fim, long *H);
	void 							pairsort(float *arr, unsigned int *iarr, int n);
	void 							fqsort(float *arr, unsigned int *iarr, int l, int r);
	void 							hack_reorder_quadrants( Image_Data_c* array );
};

#endif
