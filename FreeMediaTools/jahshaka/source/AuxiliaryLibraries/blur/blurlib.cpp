/*******************************************************************************
**
** The source file for the blurlib library
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "blurlib.h"

long	seed = 132531;
int	PBM_SE_ORIGIN_COL=0, PBM_SE_ORIGIN_ROW=0;

char**	arg;
int	maxargs;

//******************************************************************************//
void Gaussian_blur_c::separateAlpha(uchar* buf, int width, int bytes)
//******************************************************************************//
{
    int                         i, j;
    uchar                       alpha;
    double                      recip_alpha;
    int                         new_val;

    for (i = 0; i < width * bytes; i += bytes)
    {
        alpha = buf[i + bytes - 1];

        if (alpha != 0 && alpha != 255)
	    {
	        recip_alpha = 255.0 / alpha;

	        for (j = 0; j < bytes - 1; j++)
	        {
	            new_val = buf[i + j] * (int)recip_alpha;
	            buf[i + j] = MIN(255, new_val);
	        }
	    }
    }
}


//******************************************************************************//
int* Gaussian_blur_c::makeCurve(double sigma, int* length)
//******************************************************************************//
{
    int*                        curve;
    double                      sigma2;
    double                      l;
    int                         temp;
    int                         i;
    int                         n;

    sigma2 = 2 * sigma * sigma;
    l = sqrt (-sigma2 * log(1.0 / 255.0));
    n = (int)ceil(l) * 2;

    if ((n % 2) == 0)
	{
        n += 1;
	}

    //curve = (int*)calloc(n, sizeof(int))
    curve = new int [n];

    *length = n / 2;
    curve += *length;
    curve[0] = 255;

    for (i = 1; i <= *length; i++)
    {
      temp = (int)(exp (- (i * i) / sigma2) * 255);
      curve[-i] = temp;
      curve[i] = temp;
    }

  return curve;
}


//******************************************************************************//
void Gaussian_blur_c::runLengthEncode(uchar* src, int* dest, int bytes, int width)
//******************************************************************************//
{
    int                         start;
    int                         i;
    int                         j;
    uchar                       last;

    last = *src;
    src += bytes;
    start = 0;

    for (i = 1; i < width; i++)
    {
        if (*src != last)
	    {
	        for (j = start; j < i; j++)
	        {
	            *dest++ = (i - j);
	            *dest++ = last;
	        }

	        start = i;
	        last = *src;
	    }

        src += bytes;
    }

    for (j = start; j < i; j++)
    {
         *dest++ = (i - j);
         *dest++ = last;
    }
}


//******************************************************************************//
void Gaussian_blur_c::multiplyAlpha(uchar* buf, int width, int bytes)
//******************************************************************************//
{
    int                          i; 
    int                          j;
    double                       alpha;
    double                       scaled_color;

    for (i = 0; i < width * bytes; i += bytes)
    {
        // Convert alpha to a number between 0.0 and 1.0
        alpha = (double)buf[i + bytes - 1] * (1.0 / 255.0);

        // Go through the red, green, blue bytes and scale them
        for (j = 0; j < bytes - 1; j++)
		{
            scaled_color = (double)buf[i + j] * alpha;
	        buf[i + j] = (uchar)scaled_color;  
		}
    }
}



//      DUMPIMAGEMAP - Print a structuring element to the screen    
//******************************************************************************//
void Image_c::dumpImageMap(Image_Data_c* p)
//******************************************************************************//
{
	int 					i,j;

	printf("\n=====================================================\n");

	if (p == NULL) { printf(" Structuring element is NULL.\n"); }
	else 
	{
		printf("Structuring element: %dx%d origin at (%d,%d)\n",
		p->info->num_rows, p->info->num_columns, p->info->origin_x, p->info->origin_y);

		for (i = 0; i < p->info->num_rows; i++)
		{
			printf("	");

			for (j = 0; j < p->info->num_columns; j++) 
			{ 
				printf("%4d ", p->pixel_data[i][j]); 
			}

			printf("\n");
		}
	}
	printf("\n=====================================================\n");
}


//******************************************************************************//
void Image_c::createBlurMap(int num_rows, int num_columns, int x_blur, int y_blur)
//******************************************************************************//
{
	int						row;
	int						column;
	int						x_blur_scale;
	int						y_blur_scale;

	x_blur_scale = x_blur / 3;
	y_blur_scale = y_blur / 3;
	
	image_data = allocateImage(num_rows, num_columns);
	image_data->info->origin_x = 0;
	image_data->info->origin_y = 0;

	for (row = 0; row < num_rows; row++)
	{
		for (column = 0; column < num_columns; column++)
		{
			if (column <= x_blur_scale && row <= y_blur_scale)
			{
				image_data->pixel_data[row][column] = 1;
			}
			else
			{
				image_data->pixel_data[row][column] = 0;
			}
		}
	}
}

/*
void Image_c::inputPBM(char* file_name)
{
	int 						row, column, pbm_type, num_rows;
   	int							num_columns, max_value, string_index;
	unsigned char 				ucval;
	int 						val;
	long 						here;
	char 						buf1[256];
	FILE*						file_ptr;

	strcpy(buf1, file_name);
	file_ptr = fopen(buf1, "r");

	if (file_ptr == NULL)
	{
		printf("Can't open the PBM file named '%s'\n", buf1);
		return;
	}

	getNextPBMdataLine(file_ptr, buf1);

	if (buf1[0] == 'P')
	{
		switch (buf1[1])
		{
			case '1':       { pbm_type = 1; break; }
			case '2':       { pbm_type = 2; break; }
			case '3':       { pbm_type = 3; break; }
			case '4':       { pbm_type = 4; break; }
			case '5':       { pbm_type = 5; break; }
			case '6':       { pbm_type = 6; break; }
			default:        { printf("Not a PBM/PGM/PPM file.\n"); return; }
	  	}
	}

	string_index = 2;

	getNumberFromPBMfile(file_ptr, buf1, &string_index, &num_columns);		
	getNumberFromPBMfile(file_ptr, buf1, &string_index, &num_rows);	

	if (pbm_type != 1 && pbm_type != 4) 
	{ 
		getNumberFromPBMfile(file_ptr, buf1, &string_index, &max_value); 
	}
	else 
	{ 
		max_value = 1; 
	}

	fprintf(stderr, "\nPBM file class %d size %d columns X %d rows Max=%d\n", 
										pbm_type, num_columns, num_rows, max_value);
	
	// Binary file? Re-open as 'rb'         
	if (pbm_type > 3)
	{
		here = ftell(file_ptr);
		fclose(file_ptr);
		file_ptr = fopen(file_name, "rb");       
		here++;

		if (fseek(file_ptr, here, 0) != 0) 
		{
			printf("inputPBM: Sync error, file '%s'. Use ASCII PGM.\n",file_name);
			exit(3);
		}
	}

	// Allocate the image 
	if (pbm_type == 3 || pbm_type == 6)		// Colour 
	{
		exit(0);
	}
	else 
	{
		image_data = allocateImage(num_rows, num_columns);
		image_data->info->origin_x = PBM_SE_ORIGIN_ROW;
		image_data->info->origin_y = PBM_SE_ORIGIN_COL;
		PBM_SE_ORIGIN_ROW = 0;
		PBM_SE_ORIGIN_COL = 0;

		for (row = 0; row < num_rows; row++)
		{
			for (column = 0; column < num_columns; column++)
			{
				if (pbm_type < 3)
				{
					fscanf(file_ptr, "%d", &val);
					image_data->pixel_data[row][column] =(unsigned char)val;
				} 
				else 
				{
					fscanf(file_ptr, "%c", &ucval);
					image_data->pixel_data[row][column] = ucval;
	      		}
			}
		}
	}
}
*/

/*
Image_Data_c* Image_c::outputPBM(char* filename)
{
	FILE*					f;
	int 					i, j, k, columns_per_line;
	char 					buf1[64];

	strcpy(buf1, filename);

	if (image_data->info->num_columns > 20) 
	{ 
		columns_per_line = 20; 
	}
	else 
	{ 
		columns_per_line = image_data->info->num_columns - 1;
	}

	f = fopen(buf1, "w");
	if (f == 0) 
	{
		exit(0);
	}

	fprintf(f, "P2\n#origin %d %d\n", image_data->info->origin_y, image_data->info->origin_x);
	fprintf(f, "%d %d %d\n", image_data->info->num_columns, image_data->info->num_rows, 255);
	k = 0;

	for (i=0; i < image_data->info->num_rows; i++)
	{
		for (j=0; j < image_data->info->num_columns; j++)
		{
			fprintf(f, "%d ", image_data->pixel_data[i][j]);
			k++;

			if (k > columns_per_line)
			{
				fprintf(f, "\n");
				k = 0;
			}
		}
	}

	fprintf(f, "\n");
	fclose(f);
	return image_data;
}
*/

//******************************************************************************//
void Image_c::getNumberFromPBMfile(FILE* file_ptr, char* buffer, 
										int* line_index_ptr, int* number_ptr)
//******************************************************************************//
{
	int 					string_index;
	char 					string[80];

	while (   buffer[*line_index_ptr] == ' ' 
		   || buffer[*line_index_ptr] == '\t' 
		   || buffer[*line_index_ptr] == '\n')
	{
		if (buffer[*line_index_ptr] == '\n') 
		{
			getNextPBMdataLine(file_ptr, buffer);
			*line_index_ptr = 0;
		} 
		else 
		{ 
			*line_index_ptr += 1; 
		}
	}

	string_index = 0;

	while (buffer[*line_index_ptr] >= '0' && buffer[*line_index_ptr] <= '9') 
	{ 
		string[string_index++] = buffer[(*line_index_ptr)++]; 
	}

	string[string_index] = '\0';
	sscanf(string, "%d", number_ptr);
}

/* Get the next non-comment line from the PBM file f into the
   buffer b. Look for 'pragmas' - commands hidden in the comments */
//******************************************************************************//
void Image_c::getNextPBMdataLine(FILE* file_ptr, char* buffer)
//******************************************************************************//
{
	int 					index;
	char 					character;

	// Read the next significant line (non-comment) from file_ptr into buffer 
	do
	{
		// Read the next line 
		index = 0;

		do
		{
	    	fscanf(file_ptr, "%c", &character);
	    	buffer[index++] = character;

	    	if (character == '\n') 
			{ 
				buffer[index] = '\0'; 
			}

		} while(character != '\n');

		// If a comment, look for a special parameter 
		if (buffer[0] == '#') 
		{
			lookForPBMKeyword(&buffer[1]);
		}

	} while(buffer[0]=='\n' || buffer[0] == '#');
}


//      Look for a parameter hidden in a comment        
//******************************************************************************//
void Image_c::lookForPBMKeyword(char* string)
//******************************************************************************//
{
	int 					index, key_index;
	char 					key[MAX_PBM_KEY_LENGTH];
	int						upper_to_lower_conversion;

	upper_to_lower_conversion = (int)'A' - (int)'a';

	for (index = 0; index < MAX_PBM_KEY_LENGTH; index++)
	{
		if (string[index] == ' ' || string[index] == '\0' || string[index] == '\n') 
		{ 
			break; 
		}

		key[index] = string[index];
	}

	key_index = index;
	key[key_index] = '\0';

	// Convert to lower case 
	for(index = 0; index < key_index; index++)
	{
		if( (key[index] >= 'A') && (key[index] <= 'Z') )
		{
			key[index] = (char)((int)key[index] - upper_to_lower_conversion);
		}
	}

	// Which key word is it? 
	if(!strcmp(key, "origin"))		// ORIGIN key word 
	{
		sscanf(&(string[key_index + 1]), "%d %d", &PBM_SE_ORIGIN_COL, &PBM_SE_ORIGIN_ROW);
		return;
	}
}


//******************************************************************************//
Image_Data_c* Image_Data_c::allocateImage(int num_rows, int num_columns)
//******************************************************************************//
{
	//unsigned char*			ptr;		// new pixel array 
	int 					row;
	Image_Data_c*			new_image_ptr;

	if (num_rows < 0 || num_columns < 0) 
	{
		printf("Error: Bad image size (%d, %d)\n", num_rows, num_columns);
		return NULL;
	}

	// Allocate the image descriptor    
	//new_image_ptr = (Image_Data_c*)malloc(sizeof(Image_Data_c));
    new_image_ptr = new Image_Data_c;

	if (new_image_ptr == NULL) 
	{
		printf("Out of storage in NEWIMAGE.\n");
		return NULL;
	}

	// Allocate and initialize the header      
	//new_image_ptr->info = (Image_Header_c*)malloc( sizeof(Image_Header_c) );
	new_image_ptr->info = new Image_Header_c;
    

	if (new_image_ptr->info == NULL) 
	{
		printf("Out of storage in NEWIMAGE.\n");
		return NULL;
	}

	new_image_ptr->info->num_rows = num_rows;       
	new_image_ptr->info->num_columns = num_columns;
	new_image_ptr->info->origin_x = new_image_ptr->info->origin_y = 0;

	// Allocate the pixel array        
	//new_image_ptr->pixel_data = (unsigned char**)malloc(sizeof(unsigned char*) * num_rows); 
	new_image_ptr->pixel_data = new unsigned char* [num_rows]; 

	// Pointers to rows 
	if (new_image_ptr->pixel_data == NULL) 
	{
		printf("Out of storage in NEWIMAGE.\n");
		return NULL;
	}
	
	for (row = 0; row < num_rows; row++) 
	{
		new_image_ptr->pixel_data[row] =  new unsigned char [num_columns];

		if (new_image_ptr->pixel_data[row] == NULL)
		{
			printf("Out of storage. Newimage - row %d\n", row);
			exit(1);
		}
	}

	return new_image_ptr;
}


//******************************************************************************//
void Image_Data_c::freeImage(Image_Data_c* image)
//******************************************************************************//
{
	// Free the storage associated with the image IMAGE_DATA    
	int 					row;

	if (image != NULL)
	{
		for (row = 0; row < image->info->num_rows; row++) 
		{ 
			delete image->pixel_data[row]; 
		}
	}

	delete image->info;
	delete image->pixel_data;
}


//******************************************************************************//
void Image_Data_c::copy(Image_Data_c** a, Image_Data_c* b)
//******************************************************************************//
{
	copyVarImage(a, &b);
}


//******************************************************************************//
void Image_Data_c::copyVarImage(Image_Data_c** dest, Image_Data_c** src)
//******************************************************************************//
{
	int 					i,j;

	if (dest == src) 
	{ 
		return; 
	}

	if (*dest != NULL) 
	{ 
		freeImage(*dest); 
	}

	*dest = allocateImage((*src)->info->num_rows, (*src)->info->num_columns);

	if(*dest == NULL) 
	{ 
		exit(0); 
	}

	for(i=0; i < (*src)->info->num_rows; i++)
	{
		for(j = 0; j < (*src)->info->num_columns; j++)
		{
			(*dest)->pixel_data[i][j] = (*src)->pixel_data[i][j];
		}
	}

	(*dest)->info->origin_x =(*src)->info->origin_x;
	(*dest)->info->origin_y =(*src)->info->origin_y;
}



