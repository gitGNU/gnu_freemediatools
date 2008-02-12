/*******************************************************************************
**
** The header file for the blurlib library
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <stdio.h>
#include <math.h>
//#include <malloc.h>
//#include <stdio.h>
//#include <math.h>
//#include <malloc.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define	MAX_PBM_KEY_LENGTH			24
#define MAX_BLUR					256
#define MAX_IMAGE_SIZE				4096
#define SQRT2 						1.414213562
#define BLACK 						0
#define WHITE 						1

#define TRUE                        1
#define FALSE                       0

#define MAX(a, b)                   ((a > b) ? a : b)
#define MIN(a, b)                   ((a < b) ? a : b)

//typedef unsigned int                boolean;
typedef unsigned char               uchar;




//******************************************************************************//
class Gaussian_blur_values_c
//******************************************************************************//
{
    public:
        double                         radius;
        bool                        horizontal;
        bool                        vertical;
};


//******************************************************************************//
class Gaussian_blur_2_values_c
//******************************************************************************//
{
    public:
        double                         horizontal;
        double                         vertical;
};


//******************************************************************************//
class Gaussian_blur_c
//******************************************************************************//
{
     public:
	    Gaussian_blur_values_c         blur_values;
        Gaussian_blur_2_values_c       blur_2_values;


	    // Methods
	    int*        makeCurve(double sigma, int* length);
	    void        runLengthEncode(uchar* src, int* dest, int bytes, int width);
        void        multiplyAlpha(uchar* buf, int width, int bytes);
        void        separateAlpha(uchar* buf, int width, int bytes);

        // Constructor
        Gaussian_blur_c()
		{
            blur_values.radius = 5.0;
            blur_values.horizontal = TRUE;
            blur_values.vertical = TRUE;

            blur_2_values.horizontal = 5.0;
            blur_2_values.vertical = 5.0;
		}
};


// The image header data structure      
//******************************************************************************//
class Image_Header_c 
//******************************************************************************//
{
	public:
		int 						num_rows, num_columns;	// Rows and columns in the image 
		int 						origin_x, origin_y;		// Origin 
};


//      The IMAGE data structure        
//******************************************************************************//
class Image_Data_c 
//******************************************************************************//
{
	public:
		Image_Header_c*				info;			// Pointer to header 
		unsigned char**				pixel_data;		// Pixel values 

		// Methods
		Image_Data_c*				allocateImage(int num_rows, int nc);
		void 						copyVarImage(Image_Data_c** dest, Image_Data_c** src);
		void 						freeImage(Image_Data_c* image);
		void 						copy(Image_Data_c** a, Image_Data_c* b);
};


//******************************************************************************//
class Image_c: public Image_Data_c
//******************************************************************************//
{
	public:
		Image_Data_c*				image_data;

		// Methods
		//void 						inputPBM(char *fn);
		//Image_Data_c*				outputPBM(char *filename);
		void						createBlurMap(int num_rows, int num_columns, int x_blur, int y_blur);
		void 						dumpImageMap(Image_Data_c* p);
		void 						getNumberFromPBMfile(FILE *f, char *b, int *bi, int *res);
		void 						getNextPBMdataLine(FILE *f, char *b);
		void 						lookForPBMKeyword(char* s);
};

