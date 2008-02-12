#include <stdlib.h>
#include "fftlib.h"

double*						sintabpt = (double *)0;		// Points to sine table 
int*						bittabpt = (int *)0;		// Points to bit table 
int*						powers = (int *)0;			// Points to table of powers of 2 
int							numpts;            			// Size of FFT 
int							nn;							// 2^nn = numpts 
float 						direction;					// Direction of FFT 
double  					scalef = 0.0;				// Scale factor, will = 1/sqrt(numpts) 
int 						FFTN = 0;
int 						NORMALIZE = 0;

//****************************************************************************//
void FFT_c::image_fft(Image_Data_c* image, COMPLEX_IMAGE *output)
//****************************************************************************//
{
	fftx (image, 0, output);
}

//****************************************************************************//
void FFT_c::image_fftoc (Image_Data_c* image, COMPLEX_IMAGE *output)
//****************************************************************************//
{
	fftx(image, 1, output);
}

//      Do a 2D FFT on a set of BYTE-type pixels        

//****************************************************************************//
void FFT_c::fftx(Image_Data_c* image, int oc, COMPLEX_IMAGE *output)
//****************************************************************************//
{
	int  					n;
	COMPLEX_IMAGE 			cim;

	if (image->info->num_rows != image->info->num_columns) 
	{
		printf ("** Restriction: Image must be square.\n");
		return;
	}

	n = image->info->num_columns;

	// Initialize the FFT if needed 
	if (FFTN != n) { fftinit (n); }

	FFTN = n;

	// Convert to COMPLEX type 
	cim = newcomplex(image);
	n = image->info->num_rows;

	// Origin center the transform if OC=1 
	if (oc) filt_orig (cim);

	// Perform the FFT in two dimensions 
	fft2d (cim, FORWARD);

	// Return a pointer to the FFT image 
	*output = cim;
	return;
}

//******************************************************************************//
void FFT_c::image_fftinv (COMPLEX_IMAGE image, COMPLEX_IMAGE * output)
//******************************************************************************//
{
	fftinvx (image, 0, output);
}


//******************************************************************************//
void FFT_c::image_fftinvoc (COMPLEX_IMAGE image, COMPLEX_IMAGE * output)
//******************************************************************************//
{
	fftinvx (image, 1, output);
}


//******************************************************************************//
void FFT_c::fftinvx (COMPLEX_IMAGE image, int oc, COMPLEX_IMAGE* output)
//******************************************************************************//
{
	int  					n;
	COMPLEX_IMAGE 			cim;

	n = FFTN;
	cim = dupcomplex(image);

	if (FFTN != n) 
	{
		fftinit(n);
		FFTN = n;
	}

	if (oc) { filt_orig(cim); }

	fft2d(cim, REVERSE);
	*output = cim;
	return;
}


//******************************************************************************//
void FFT_c::cprod(float c1r, float c1i, float* c2r, float* c2i)
//******************************************************************************//
{                               // Complex product: res = c1*c2 
	float 					real, imag;

	real = (c1r * *c2r) - (c1i * *c2i);
	imag = (c1r * *c2i) + (*c2r * c1i);
	*c2r = real;    
	*c2i = imag;
}


//******************************************************************************//
float FFT_c::pix_cnorm (float cr, float ci)
//******************************************************************************//
{                               // abs value squared: res = |c1|**2 
	return((cr *cr) + (ci * ci));
}


//******************************************************************************//
void FFT_c::cdiv(float c1r, float c1i, float* c2r, float* c2i)
//******************************************************************************//
{                               // res = c1/c2 ... Complex 
	float 						z,real, imag;

	z = (*c2r * *c2r) + (*c2i * *c2i);

	if (z != 0.0) 
	{
		real = ((c1r * *c2r) + (c1i * *c2i)) / z;
		imag = ((*c2r * c1i) - (c1r * *c2i)) / z;
		*c2r = real;
		*c2i = imag;
		return;
	}

	*c2r = 0.0;     *c2i = 0.0;
}

//************************************************************************
//  fft2d -- Calls `fft' to perform a 2 dimensional Fast Fourier Trans-   
//           form on an N x N array of complex data. (N must be defined). 
//                                                                        
//                Calls:         fft().                                   
//                Called by:     user's program.                          
//                Parameters:    Array of complex (see fft.h) data,       
//                               direction of Fourier Transform (+/-1.0). 
//                Returns:       Nothing, but array contains transformed  
//                               values.                                  
//                Caveats:       fftinit() must be called previously.     
//                                                                        
//                     February 21, 1985    Terry Ingoldsby               
//************************************************************************

void FFT_c::fft2d( COMPLEX_IMAGE image, float direction )
{
	float 					temp[MAX_IMAGE_SIZE];   // Need to put rows into consecutive storage 
	int  					i, j;         			// Iteration counters 

	// Transform Columns 
	for( i = 0; i < FFTN; i++ ) { fft (image[i], direction); }

	printf ("The transform is half finished.\n");

// Transform Rows 
	for(i = 0; i < FFTN; i++ )  
	{
		// First put rows in consecutive storage 
		for( j = 0; j < FFTN; j++ )
		{
			temp[j] = image[j][i];
			temp[j+FFTN] =image[j][i+FFTN];
		}

		fft (temp, direction);

		// Copy back to image 
		for( j = 0; j < FFTN; j++ ) 
		{
			image[j][i] = temp[j];
			image[j][i+FFTN] = temp[j+FFTN];
		}
	}
}


//******************************************************************************//
void FFT_c::hack_reorder_quadrants( Image_Data_c* array )
//******************************************************************************//
{
	int  				i, j;
	int					FFTN_DIV_2;
	char				temp;

	FFTN_DIV_2 = FFTN / 2;

	for( i = 0; i < FFTN_DIV_2; i++ )
	{
		for( j = 0; j < FFTN; j++ )
		{
			if (j >= FFTN_DIV_2)
			{
				temp = array->pixel_data[i + FFTN_DIV_2][j - FFTN_DIV_2];
				array->pixel_data[i + FFTN_DIV_2][j - FFTN_DIV_2] = array->pixel_data[i][j];
				array->pixel_data[i][j] = temp;
			}
			else
			{
				temp = array->pixel_data[i + FFTN_DIV_2][j + FFTN_DIV_2];
				array->pixel_data[i + FFTN_DIV_2][j + FFTN_DIV_2] = array->pixel_data[i][j];
				array->pixel_data[i][j] = temp;
			}
		}
	}
}


//***********************************************************************
//  Fixorig -- Modifies the input data so that after being Fourier trans-
//             formed the origin (f = 0) lies at the center of the array.
//                                                                       
//                    Called by:        User's program.                  
//                    Calls:            Nothing.                         
//                    Parameters:       An array of type `complex'.      
//                    Returns:          Nothing, but on return the array 
//                                      fixed s.t. after F.T. origin will
//                                      be at the center.                
//***********************************************************************

//******************************************************************************//
void FFT_c::filt_orig( COMPLEX_IMAGE array )
//******************************************************************************//
{
	int  				i, j;

	for( i = 0; i < FFTN; i++ )
	{
		for( j =0; j < FFTN; j++ )
		{
			if( (i + j) % 2 )  
			{
				array[i][j] = -array[i][j];
				array[i][j + FFTN] = -array[i][j + FFTN];
			}
		}
	}
}


// Convert a REAL/COMPLEX image into an integer one for display 
//******************************************************************************//
void FFT_c::filt_toint ( COMPLEX_IMAGE image , Image_Data_c* output, float *h)
//******************************************************************************//
{
	float 				xmax, xmin, x, xdif;
	int 				i,j,n;
	long 				H[256];
	Image_Data_c* 				cim;

/* OUTPUT must be an allocated byte image, at least as big as Image_Data_c* in
   each dimension. Image_Data_c* will be converted into real from complex.      */
	n = FFTN;
	cim = output;

	xmax = -1.0e20f; xmin = -xmax; 

	for (i = 0; i < n; i++) 
	{
		for (j = 0; j < n; j++) 
		{
			x = pix_cnorm(image[i][j], image[i][j + n]);
			x = (float)sqrt((double)x);

			if (x > 0.0) 
			{ 
				x = (float) log(sqrt ( (double) x )); 
			}
			else 
			{ 
				x = 0.0; 
			}

			// Find the max norm of all pixels 
		    if (x > xmax) 
			{ 
				xmax = x; 
			}

		    if (x < xmin) 
			{ 
				xmin = x; 
			}

			// Store norm in (i,j) 
		    image[i][j] = x;
		    image[i][j + FFTN] = 0.0;
		}
	}

	if ( (xmin <= 0.00001) && (xmin >= -0.00001) ) { xmin = 0.000000; }

	printf ("Xmax is %12.5f   Xmin is %12.6f\n", xmax, xmin);

	// Compute the histogram H or accept it as a parameter 
	if (h == 0) 
	{ 
		for (i = 0; i < 256; i++) { H[i] = (FFTN * FFTN) / 256; }
	}
	else 
	{
		// If h is passed, it contains % of total number of pixels. 
		for (i = 0; i < 256; i++) { H[i] = (int)((h[i] / 100.0) *( FFTN * FFTN)); }
	}

	xdif = xmax - xmin;

	if (xdif <= 0.0) { printf ("******* ZERO RANGE!!\n"); }

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++) 
		{
			x = image[i][j];
			x = (x-xmin)/xdif * 255.0;

			if (x < 0.0) { x = 0.0; }
			else if (x > 255.0) { x = 255.0; }

			cim->pixel_data[i][j] = (unsigned char)x;
		}
	}
}


//           Scale a floating point image to the range 0-255              
//                                                                        
//     After a restoration, an image can have an enormous level range.    
//     This program will convert that range to grey levels that can be    
//     displayed. It uses a grey level histogram of the image before      
//     processing, and fits this to the floating point 'after' image.     
//     This resembles the histogram fitting/equalization algorithm.       
//     In a 512x512 image there can be at most 200K different floating    
//     point levels, so it is tractable.                                  
//                                                                        

//******************************************************************************//
void FFT_c::realtoint (COMPLEX_IMAGE fim, long* H)
//******************************************************************************//
{
	float*						data;                             // Floating point values 
	unsigned int*				dij;                       // i,j for the pixels    
	int 						i, j, k;
	unsigned int 				ii, jj;
	float 						flast = 0.0, xmax, xmin, xd;      // Last float value given a bin 
	int 						num_rows, num_columns;
	long 						N;

	num_rows = FFTN; num_columns = FFTN;
	N = (long)num_rows * (long)num_columns;

	// No histogram given 
	if (H == 0)
	{
		xmax = xmin = fim[0][0];

		for (i=0; i<num_rows; i++)
		{
			for (j=0; j<num_columns; j++)
			{
				if (xmax < fim[i][j]) { xmax = fim[i][j]; }
				if (xmin > fim[i][j]) { xmin = fim[i][j]; }
			}
		}

		xd = xmax-xmin;

		for (i = 0; i < num_rows; i++)
		{
			for (j=0; j<num_columns; j++) { fim[i][j] = (fim[i][j]-xmin)/xd * 255; }
		}

		return;
	}

// Copy pixels to DATA array 
	k = 0;
	data = (float *)malloc(N*sizeof(float));
	dij  = (unsigned int *)malloc(N*sizeof(int));

	for (i=0; i<num_rows; i++)
	{
		for (j=0; j<num_columns; j++)
		{
			data[k] = fim[i][j];            // Save the pixel values 
			dij[k++]= i<<10 | j;                // Save the (i,j) coords 
		}
	}

	printf ("Data pixels extracted.\n");

	// Sort the data array 
	pairsort (data, dij, N);
	printf ("Real data is sorted.\n");

	// Assign BYTE values to pixels according to their histogram cell 
	j = 0;  k = 0;

	for (i=0; i<256; i++)                 // For each histogram entry 
	{
		if ((i!=0) && (i%10==0)) { printf ("Starting row %d\n", i); }

		if (k <= 0) { k = 0; }

		while ((k < H[i]) && (j < (num_rows * num_columns))) // K is the #floats assigned to this bin 
		{
			ii = dij[j];                           // Extract coordinates 
			jj = ii & 01777;
			ii = (ii >> 10) & 01777;

			flast = data[j];
			fim[ii][jj] = (float)i;  // Assign the value of this bin 
			k++;  j++;
		}

		k = 0;

		while (data[j] == flast) // Assign same bin to same float value 
		{
			ii = dij[j];  jj = ii%1024; ii = ii/1024;
			fim[ii][jj] = (float)i;
			j++; k++;
		}
	}

	free(data);
	free(dij);
}


//******************************************************************************//
void FFT_c::pairsort (float* arr, unsigned int* iarr, int n)
//******************************************************************************//
{
	fqsort (arr, iarr, 0, n-1);
}

//******************************************************************************//
void FFT_c::fqsort (float* arr, unsigned int* iarr, int l, int r)
//******************************************************************************//
{
	int 					i, j;
	unsigned int 			k;
	float 					x, w;

	i = l; j = r;
	x = arr[(l + r) / 2];

	do
	{
		while (arr[i] < x) { i++; }
		while (x < arr[j]) { j--; }

		if (i <= j)
		{
			w = arr[i]; arr[i] = arr[j]; arr[j] = w;
			k = iarr[i]; iarr[i] = iarr[j]; iarr[j] = k;
			i++; j--;
		}

	} while (i <= j);

	if (l < j) fqsort(arr, iarr, l, j);
	if (i < r) fqsort(arr, iarr, i, r);
}


//******************************************************************************//
int FFT_c::vlog2(int x)
//******************************************************************************//
{
	int 					k,j;

	j = 0; k=1;

	while (k<x) 
	{
		k *= 2;
		j += 1;
	}

	return j;
}


//******************************************************************************//
void FFT_c::fft( float data[], float dir )
//******************************************************************************//
{
    Complex_c  					tempval;   // Use for unscrambling array 
    int  						i;

    direction = dir;

    if ( direction != REVERSE )  // Make sure valid direction 
	{
		direction = FORWARD;
	}

    _fft( data, 1, 0 );     // Call recursive FFT routine to transform data 
			    // Start at recursion level 1, begin at node 0 

    // Unscramble final values 
    for ( i = 0; i < numpts; i++ )
	{
		if( *(bittabpt + i) <= i )  
		{   // If not yet de-scrambled 
			tempval.real = data[i];
			tempval.imag = data[i+FFTN];
			data[i] = data[ *(bittabpt + i) ];
			data[i + FFTN] = data[ *(bittabpt + i)  + FFTN];
			data[ *(bittabpt + i) ] = tempval.real;
			data[ *(bittabpt + i)  + FFTN] = tempval.imag;
		}
	}

    // Multiply by scale factor 
    for( i = 0; i < numpts; i++ )  
	{
		data[i] *= scalef;
		data[i + FFTN] *= scalef;
    }
}


//---------------------------------------------------------------------------
//  _fft -- A recursive FFT routine.  Contributions of Bart Hicks are acknow-
//          edged!  Implements the FFT algorithm as described in Brigham.    
//                                                                           
//                  Calls:      _fft()                                       
//                  Called by:  fft()                                        
//                  Parameters: tseries - Pntr to array of data to be FFTed  
//                              level - indicates how recursively deep the   
//                                      algorithm currently is.  This allows 
//                                      us to know when to stop (if level >= 
//                                      nn) and how many nodes are in the    
//                                      sub-FFT we are currently performing  
//                              chunk - tells us which is the starting node  
//                                      of the sub-FFT.                      
//                  Returns:    Nothing, but on return tseries[] contains    
//                              the scrambled transformed values.            
//---------------------------------------------------------------------------

//******************************************************************************//
int FFT_c::_fft( float tseries[], int level, int chunk )
//******************************************************************************//
{
    int  					nodes, i;      
							/* `nodes' indicates how many nodes are in the sub-FFT
			   				under consideration.  `i' is a counter.  */
    int  					sinindx, cosindx;   // Indices into sin table built by fftinit() 
    int  					dual1, dual2;       // Represent current dual node pair indices 
    Complex_c 				dual1val, dual2val, dualprod, wp;
							/* Partial products of the dual node pair and the `wp'
							exponential.  Notice that in the FFT algorithm the term
							exp(-j*2*PI*n*k/N) is often abbreviated as W**nk, where
							W=exp(-j*2*PI/N).  Each level of recursion (ie. each 
							sub-FFT at a given level) requires calculation of W to
							some power, say `p'.  Since `C' does not support complex


							arithmetic the sin and cos functions serve in place of
							the complex exponential.  The variable here chosen to
							represent this is `wp'.
							When calculationing a given node, two nodes from the
							previous level are used in this calculation.  These
							same two nodes are used in calculating the so called
							`dual node pair'.  The indices of this dual node pair
							are the variables `dual1' and `dual2'.  The values of
							these nodes, `dual1val' and `dual2val' are calculated by
							adding (->dual1val) or subtracting (->dual2val) the pre-
							vious value (the value at the previous level) of the
							node to the product of the previous value of the node of
							the other pair member and `wp', the exponential value
							for the current level.  Since this product is used for
							several calculations it is stored in `dualprod'.    */

    nodes = *(powers + (nn - level) );

    sinindx = *(bittabpt + chunk/nodes);    // Get index into trig table for 
    cosindx = (sinindx + numpts/4) % numpts; // sin & cos values needed now 
    wp.real = *(sintabpt + cosindx);
    wp.imag = direction * *(sintabpt + sinindx);

    for( i = 0; i < nodes; i++)  
	{  // Calculate new node values 
		dual1 = chunk + i;
		dual2 = dual1 + nodes;
		dual1val.real = tseries[dual1];
		dual1val.imag = tseries[dual1 + FFTN];
		dual2val.real = tseries[dual2];
		dual2val.imag = tseries[dual2 + FFTN];

		dualprod.real = dual2val.real * wp.real - dual2val.imag * wp.imag;
		dualprod.imag = dual2val.real * wp.imag + dual2val.imag * wp.real;

		tseries[dual1] = dual1val.real + dualprod.real;
		tseries[dual1 + FFTN] = dual1val.imag + dualprod.imag;
		tseries[dual2] = dual1val.real - dualprod.real;
		tseries[dual2 + FFTN] = dual1val.imag - dualprod.imag;
    }

    if( level < nn )  
	{  // Are we all done?  If not do another sub-FFT 
		_fft( tseries, level + 1, chunk );  // Do top dual node pair sub-FFT 
		_fft( tseries, level + 1, chunk + nodes);  // Do bottom dual node pair sub-FFT 
    }

    return 0;
}


//===========================================================================

//---------------------------------------------------------------------------
//  fftinit -- This routine initializes internal trig. tables with approp-   
//             riate values for a particular size Fast Fourier Transform.    
//             It also creates an array which is used for de-scrambling the  
//             FFT.  It MUST be called prior to using `fft' but need not be  
//             called subsequently unless `size' changes.                    
//                To ensure that memory is not wasted while still allowing   
//             for dynamic array size, the table arrays are acquired using   
//             `calloc'.  The arrays are then initialized with the sin()     
//             values needed and then `fftinit' stores the pointers to these 
//             arrays in variables global to the FFT routines, `sintabpt'    
//             and `bittabpt'.                                               
//                                                                           
//                  Calls:      calloc(), sin(), log(), sqrt(), bitrev().    
//                  Called by:  user's program before FFT's performed.       
//                  Parameters: Number of points in FFT.                     
//                  Returns:    Nothing.                                     
//---------------------------------------------------------------------------
//******************************************************************************//
void FFT_c::fftinit( int nopts )
//******************************************************************************//
{
    int  						i;

    numpts = nopts; // Store in global variable 
    nn = int((log( (double) numpts )/log( 2.0 )) + 0.5);   /* Compute nn s.t. 
							 2^nn = numpts */
    scalef = 1.0 / sqrt( (double) numpts );    // Compute scale factor 

    // If tables were previously allocated then release storage 
    if( sintabpt != (double *)0 ) 
	{ 
		free( sintabpt ); 
	}

    if( bittabpt != (int *)0 ) 
	{ 
		free( bittabpt ); 
	}

    if( powers != (int *)0) 
	{ 
		free( powers ); 
	}

    if ( (sintabpt = (double *) calloc( nopts, sizeof( double ) )) == 0 )  
	{
		printf("\n\nInsufficient memory available for sin table, aborting!\n");
		exit( 0 );
    }
    
    if ( (bittabpt = (int *) calloc( nopts, sizeof( int ) )) == 0 )  
	{
		printf("\n\nInsufficient memory available for bit table, aborting!\n");
		exit( 0 );
    }
    
    if ( (powers = (int *) calloc( nn + 1, sizeof( int ) )) == 0 )  
	{
		printf("\n\nInsufficient memory available for powers table, aborting!\n");
		exit( 0 );
    }

    for( i = 0; i <= nn; i++ )  
	{  // Build table of powers of 2 
		*(powers + i) = (int)(pow( 2.0, (double) i ) + .5);
    }

    for( i = 0; i < numpts ; i++ )  
	{  // Build sine and bit reverse tables 
		*(sintabpt + i) = sin( 6.283185307179587 * i / numpts );
		*(bittabpt + i) = bitrev( i );    // Build table for unscrambling 
    }


}


//******************************************************************************//
int FFT_c::bitrev( int bits )   // Static for privacy 
//******************************************************************************//
{
    int  				i, lookmask, setmask, tempbit;   /* `i' is a counter, `setmask' allows
					     examination of specific bits of
					     `bits'.  `setmask' allows specific
					     bits of `tempbit' to be set. */

    lookmask = 1;   // Mask to look at bits in input index, start with bit 0 
    setmask = numpts;    // Look at MSBit of significance 
    setmask >>= 1;       // But recall that indices from 0 to numpts - 1 
    tempbit = 0;    // Initialize reversed value to 0 

    for( i = 0; i <= nn - 1; i++ )  
	{  // Examine up to `nn' its 
		if( (bits & lookmask) == lookmask )  // Is bit `i' set? 
		{
	    	tempbit = tempbit | setmask;
		}

		lookmask <<= 1;     // Look at next bit left 
		setmask >>= 1;      // Move set bit one bit right 
    }

    return( tempbit );      // `bits' is now reversed 
}


//******************************************************************************//
COMPLEX_IMAGE FFT_c::newcomplex(Image_Data_c* im)
//******************************************************************************//
{
	int 					i,j;
	COMPLEX_IMAGE 			x;
	float*					y;
	float					xmax;

	x = (COMPLEX_IMAGE)malloc(im->info->num_rows * sizeof(float *) );

	if (x == NULL)
	{
		printf ("Out of storage in NEWCOMPLEX!\n");
		exit (1);
	}
	
	y = (float*)malloc(sizeof(float) * im->info->num_rows * im->info->num_columns * 2);

	if (y == NULL)
	{
		printf ("Out of storage in NEWCOMPLEX!\n");
		exit (1);
	}

	for (i = 0; i < im->info->num_rows; i++) 
	{ 
		x[i] = &(y[i * FFTN*2]); 
	}

	// Copy data 
	xmax = 0.0;

	for (i = 0; i < im->info->num_rows; i++)
	{
		for (j = 0; j < im->info->num_columns; j++)
		{
			x[i][j] = (float)(im->pixel_data[i][j]);
			x[i][j + im->info->num_columns] = 0.0;

			if (x[i][j] > xmax) 
			{ 
				xmax = x[i][j]; 
			}
		}
	}

	// Normalize 
	if (NORMALIZE)
	{
		for (i = 0; i < im->info->num_rows; i++)
		{
			for (j=0; j<im->info->num_columns; j++) 
			{
				x[i][j] /= xmax; 
			}
		}
	}

	return x;
}


//******************************************************************************//
void FFT_c::normalize_set ()
//******************************************************************************//
{
	NORMALIZE = 1;
}


//******************************************************************************//
void FFT_c::normalize_clear ()
//******************************************************************************//
{
	NORMALIZE = 0;
}


//******************************************************************************//
COMPLEX_IMAGE FFT_c::dupcomplex(COMPLEX_IMAGE im)
//******************************************************************************//
{
	int 					i,j;
	COMPLEX_IMAGE 			x;
	float*					y;

	x = (COMPLEX_IMAGE)malloc(FFTN * sizeof(float *) );

	if (x == NULL)
	{
		printf ("Out of storage in DUPCOMPLEX!\n");
		exit (1);
	}
	
	y = (float*)malloc(sizeof(float) * FFTN * FFTN * 2);

	if (y == NULL)
	{
	  printf ("Out of storage in DUPCOMPLEX!\n");
	  exit (1);
	}

	x[0] = y;

	for (i = 1; i < FFTN; i++)
	{
		x[i] = &(y[i * 2 * FFTN]);
	}

	// Copy data 
	for (i = 0; i < FFTN; i++)
	{
		for (j = 0; j < FFTN; j++)
		{
			x[i][j] = im[i][j];
			x[i][j + FFTN] = im[i][j + FFTN];
		}
	}

	return x;
}


//******************************************************************************//
void FFT_c::freecomplex(COMPLEX_IMAGE x)
//******************************************************************************//
{
	if (x)
	{
	  free(x[0]);
	  free(x);
	}
}
