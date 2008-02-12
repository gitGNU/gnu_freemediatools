/* readpic.c, read source pictures                                          */

/* Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved. */

/*
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis.  The MPEG Software Simulation Group disclaims
 * any and all warranties, whether express, implied, or statuary, including any
 * implied warranties or merchantability or of fitness for a particular
 * purpose.  In no event shall the copyright-holder be liable for any
 * incidental, punitive, or consequential damages of any kind whatsoever
 * arising from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs and user's
 * customers, employees, agents, transferees, successors, and assigns.
 *
 * The MPEG Software Simulation Group does not represent or warrant that the
 * programs furnished hereunder are free of infringement of any third-party
 * patents.
 *
 * Commercial implementations of MPEG-1 and MPEG-2 video, including shareware,
 * are subject to royalty fees to patent holders.  Many of these patents are
 * general enough such that they are unavoidable regardless of implementation
 * design.
 *
 */

#include "mpeg2enc.h"
#include "qimage.h"


void 
MpegEncoder::readframe(int, char* fname, unsigned char* frame[])
{
    switch (m_inputtype)
    {
        case T_Y_U_V :
        {
            read_y_u_v(fname,frame);
            break;
        }
        case T_YUV :
        {
            read_yuv(fname,frame);
            break;
        }
        case T_PPM :
        {
            read_ppm(fname,frame);
            break;
        }
        case T_BMP :
        {
            read_bmp(fname,frame);
            break;
        }
        case T_QIMAGE :
        {
            read_qimage(fname, frame);
            break;
        }
        default:
        {
            break;
        }
    }
}


void 
MpegEncoder::read_y_u_v(char* fname, unsigned char* frame[])
{
    int i;
    int chrom_hsize, chrom_vsize;
    char name[128];
    FILE *fd;

    chrom_hsize = (m_chroma_format == CHROMA444) ? m_horizontal_size
        : m_horizontal_size >> 1;
    chrom_vsize = (m_chroma_format != CHROMA420) ? m_vertical_size
        : m_vertical_size>>1;

    sprintf(name,"%s.Y",fname);
    if (!(fd = fopen(name,"rb")))
    {
        sprintf(m_errortext, "Couldn't open %s\n", name);
        error(m_errortext);
    }

    for (i = 0; i < m_vertical_size; i++)
	{
        fread(frame[0]+ (i * m_width), 1, m_horizontal_size, fd);
	}

    fclose(fd);
    border_extend(frame[0], m_horizontal_size, m_vertical_size, m_width, m_height);

    sprintf(name,"%s.U",fname);
    if (!(fd = fopen(name,"rb")))
    {
        sprintf(m_errortext, "Couldn't open %s\n", name);
        error(m_errortext);
    }

    for (i=0; i<chrom_vsize; i++)
	{
        fread(frame[1] + (i * m_chrom_width), 1, chrom_hsize, fd);
	}

    fclose(fd);
    border_extend(frame[1], chrom_hsize, chrom_vsize, m_chrom_width, m_chrom_height);

    sprintf(name,"%s.V",fname);
    if (!(fd = fopen(name,"rb")))
    {
        sprintf(m_errortext, "Couldn't open %s\n", name);
        error(m_errortext);
    }

    for (i = 0; i < chrom_vsize; i++)
	{
        fread(frame[2] + (i * m_chrom_width), 1, chrom_hsize, fd);
	}

    fclose(fd);
    border_extend(frame[2], chrom_hsize, chrom_vsize, m_chrom_width, m_chrom_height);
}


void 
MpegEncoder::read_yuv(char* fname, unsigned char* frame[])
{
    int i;
    int chrom_hsize, chrom_vsize;
    char name[128];
    FILE *fd;

    chrom_hsize = (m_chroma_format == CHROMA444) ? m_horizontal_size
        : m_horizontal_size >> 1;
    chrom_vsize = (m_chroma_format != CHROMA420) ? m_vertical_size
        : m_vertical_size >> 1;

    sprintf(name,"%s.yuv",fname);
    if (!(fd = fopen(name,"rb")))
    {
        sprintf(m_errortext, "Couldn't open %s\n", name);
        error(m_errortext);
    }

    /* Y */
    for (i = 0; i < m_vertical_size; i++)
	{
        fread(frame[0] + (i * m_width), 1, m_horizontal_size, fd);
	}

    border_extend(frame[0], m_horizontal_size, m_vertical_size, m_width, m_height);

    /* Cb */
    for (i=0; i<chrom_vsize; i++)
    {
        fread(frame[1] + (i * m_chrom_width), 1, chrom_hsize, fd);
    }

    border_extend(frame[1], chrom_hsize, chrom_vsize, m_chrom_width, m_chrom_height);

    /* Cr */
    for (i = 0; i < chrom_vsize; i++)
    {
        fread(frame[2] + (i * m_chrom_width), 1, chrom_hsize, fd);
    }

    border_extend(frame[2], chrom_hsize, chrom_vsize, m_chrom_width, m_chrom_height);

    fclose(fd);
}


void 
MpegEncoder::read_ppm(char* fname, unsigned char* frame[])
{
    int i, j;
    int r, g, b;
    double y, u, v;
    double cr, cg, cb, cu, cv;
    char name[128];
    FILE *fd;
    unsigned char *yp, *up, *vp;
    static double coef[7][3] = {
        {0.2125,0.7154,0.0721}, /* ITU-R Rec. 709 (1990) */
        {0.299, 0.587, 0.114},  /* unspecified */
        {0.299, 0.587, 0.114},  /* reserved */
        {0.30,  0.59,  0.11},   /* FCC */
        {0.299, 0.587, 0.114},  /* ITU-R Rec. 624-4 System B, G */
        {0.299, 0.587, 0.114},  /* SMPTE 170M */
        {0.212, 0.701, 0.087}}; /* SMPTE 240M (1987) */

    i = m_matrix_coefficients;

    if (i > 8)
    {
        i = 3;
    }

    cr = coef[i-1][0];
    cg = coef[i-1][1];
    cb = coef[i-1][2];
    cu = 0.5/(1.0-cb);
    cv = 0.5/(1.0-cr);

    if (m_chroma_format == CHROMA444)
    {
        m_u444 = frame[1];
        m_v444 = frame[2];
    }

    sprintf(name,"%s.ppm",fname);

    if (!(fd = fopen(name,"rb")))
    {
        sprintf(m_errortext,"Couldn't open %s\n",name);
        error(m_errortext);
    }

    /* skip header */
    getc(fd); getc(fd); /* magic number (P6) */
    pbm_getint(fd); pbm_getint(fd); pbm_getint(fd); /* width height maxcolors */

    for (i = 0; i < m_vertical_size; i++)
    {
        yp = frame[0] + i*m_width;
        up = m_u444 + i*m_width;
        vp = m_v444 + i*m_width;

        for (j = 0; j < m_horizontal_size; j++)
        {
            r=getc(fd); g=getc(fd); b=getc(fd);
            /* convert to YUV */
            y = cr*r + cg*g + cb*b;
            u = cu*(b-y);
            v = cv*(r-y);
            yp[j] = (unsigned char)((219.0/256.0)*y + 16.5);  /* nominal range: 16..235 */
            up[j] = (unsigned char)((224.0/256.0)*u + 128.5); /* 16..240 */
            vp[j] = (unsigned char)((224.0/256.0)*v + 128.5); /* 16..240 */
        }
    }
    fclose(fd);

    border_extend(frame[0], m_horizontal_size, m_vertical_size, m_width, m_height);
    border_extend(m_u444, m_horizontal_size, m_vertical_size, m_width, m_height);
    border_extend(m_v444, m_horizontal_size, m_vertical_size, m_width , m_height);

    if (m_chroma_format == CHROMA422)
    {
        conv444to422(m_u444, frame[1]);
        conv444to422(m_v444, frame[2]);
    }

    if (m_chroma_format == CHROMA420)
    {
        conv444to422(m_u444, m_u422);
        conv444to422(m_v444, m_v422);
        conv422to420(m_u422, frame[1]);
        conv422to420(m_v422, frame[2]);
    }
}


void 
MpegEncoder::read_bmp(char* fname, unsigned char* frame[])
{
    int i, j;
    int r, g, b;
    double y, u, v;
    double cr, cg, cb, cu, cv;
    char name[128];
    FILE *fd;
    unsigned char *yp, *up, *vp;

    static double coef[7][3] = {
        {0.2125,0.7154,0.0721}, /* ITU-R Rec. 709 (1990) */
        {0.299, 0.587, 0.114},  /* unspecified */
        {0.299, 0.587, 0.114},  /* reserved */
        {0.30,  0.59,  0.11},   /* FCC */
        {0.299, 0.587, 0.114},  /* ITU-R Rec. 624-4 System B, G */
        {0.299, 0.587, 0.114},  /* SMPTE 170M */
        {0.212, 0.701, 0.087}}; /* SMPTE 240M (1987) */

    i = m_matrix_coefficients;

    if (i > 8)
    {
        i = 3;
    }

    cr = coef[i-1][0];
    cg = coef[i-1][1];
    cb = coef[i-1][2];
    cu = 0.5/(1.0-cb);
    cv = 0.5/(1.0-cr);

    if (m_chroma_format == CHROMA444)
    {
        m_u444 = frame[1];
        m_v444 = frame[2];
    }

    sprintf(name,"%s.bmp",fname);

    if (!(fd = fopen(name,"rb")))
    {
        sprintf(m_errortext,"Couldn't open %s\n",name);
        error(m_errortext);
    }

    unsigned char   header_data[30];
    int             header_offset;

    // Get the header information
    // http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
 
    for (header_offset = 0; header_offset < 30; header_offset++)
    {
        header_data[header_offset] = (unsigned char)getc(fd);
    }

//     int magic_number = header_data[1] << 8 | header_data[0];

//     int file_size = 
//         header_data[5] << 24 | header_data[4] << 16 | 
//         header_data[3] <<  8 | header_data[2] <<  0;

    int data_offset = 
        header_data[13] << 24 | header_data[12] << 16 | 
        header_data[11] <<  8 | header_data[10] <<  0;

    int image_width = 
        header_data[21] << 24 | header_data[20] << 16 | 
        header_data[19] <<  8 | header_data[18] <<  0;

    int image_height = 
        header_data[25] << 24 | header_data[24] << 16 | 
        header_data[23] <<  8 | header_data[22] <<  0;

    float width_ratio = float(image_width) / float(m_horizontal_size);
    float height_ratio = float(image_height) / float(m_vertical_size);

//     int bits_per_pixel = 
//         header_data[29] <<  8 | header_data[28] <<  0;

    // Go to the start of the data
    for (header_offset = 30; header_offset < data_offset; header_offset++)
    {
        getc(fd);
    }

    // bmp files are upside down
    int num_bytes = image_height * image_width * 3;
    unsigned char* image_data = new unsigned char[num_bytes];
    fread(image_data, sizeof(unsigned char), num_bytes, fd);

    int bytes_per_pixel = 3;

    for (i = 0; i < m_vertical_size; i++)
    {
        int image_row = int( float(m_vertical_size - 1 - i) * height_ratio );
        unsigned char* row_ptr = image_data + (image_row * image_width * bytes_per_pixel);
        unsigned char* row2_ptr;

        if (i < m_vertical_size - 1)
        {
            row2_ptr = row_ptr - (image_width * bytes_per_pixel);
        }
        else
        {
            row2_ptr = row_ptr;
        }
 
        yp = frame[0] + (i * m_width);
        up = m_u444 + (i * m_width);
        vp = m_v444 + (i * m_width);

        for (j = 0; j < m_horizontal_size; j++)
        {
            int j_scaled =  int( float(j) * width_ratio) * bytes_per_pixel;
            int j2_scaled;

            if (m_use_pixel_averaging)
            {
                if (j < m_horizontal_size - 1)
                {
                    j2_scaled = j_scaled + bytes_per_pixel;
                }
                else
                {
                    j2_scaled = j_scaled;
                }
                
                r = ( 
                    *(row_ptr + j_scaled + 0) + 
                    *(row_ptr + j2_scaled + 0) +
                    *(row2_ptr + j_scaled + 0) + 
                    *(row2_ptr + j2_scaled + 0) 
                    ) / 4;
                
                g = ( 
                    *(row_ptr + j_scaled + 1) + 
                    *(row_ptr + j2_scaled + 1)  +
                    *(row2_ptr + j_scaled + 1) + 
                    *(row2_ptr + j2_scaled + 1) 
                    ) / 4;
                b = ( 
                    *(row_ptr + j_scaled + 2) + 
                    *(row_ptr + j2_scaled + 2) +
                    *(row2_ptr + j_scaled + 2) + 
                    *(row2_ptr + j2_scaled + 2) 
                    ) / 4;
            }
            else
            {
                r = *(row_ptr + j_scaled + 0);
                g = *(row_ptr + j_scaled + 1);
                b = *(row_ptr + j_scaled + 2);
            }
            
            /* convert to YUV */
            y = (cr * r) + (cg * g) + (cb * b);
            u = cu * (b-y);
            v = cv * (r-y);
            yp[j] = (unsigned char)( ( (219.0 / 256.0) * y ) + 16.5);  /* nominal range: 16..235 */
            up[j] = (unsigned char)( ( (224.0 / 256.0) * u ) + 128.5); /* 16..240 */
            vp[j] = (unsigned char)( ( (224.0 / 256.0) * v ) + 128.5); /* 16..240 */
        }
    }

    fclose(fd);

    delete [] image_data;

    border_extend(frame[0], m_horizontal_size, m_vertical_size, m_width, m_height);
    border_extend(m_u444, m_horizontal_size, m_vertical_size, m_width, m_height);
    border_extend(m_v444, m_horizontal_size, m_vertical_size, m_width, m_height);

    if (m_chroma_format == CHROMA422)
    {
        conv444to422(m_u444, frame[1]);
        conv444to422(m_v444, frame[2]);
    }

    if (m_chroma_format == CHROMA420)
    {
        conv444to422(m_u444, m_u422);
        conv444to422(m_v444, m_v422);
        conv422to420(m_u422, frame[1]);
        conv422to420(m_v422, frame[2]);
    }
}



void 
MpegEncoder::border_extend(unsigned char* frame, int w1, int h1, int w2, int h2)
{
    int i, j;
    unsigned char *fp;

    /* horizontal pixel replication (right border) */

    for (j = 0; j < h1; j++)
    {
        fp = frame + (j * w2);

        for (i = w1; i < w2; i++)
        {
            fp[i] = fp[i-1];
        }
    }

    /* vertical pixel replication (bottom border) */

    for (j = h1; j < h2; j++)
    {
        fp = frame + (j * w2);

        for (i = 0; i < w2; i++)
        {
            fp[i] = fp[i-w2];
        }
    }
}

/* horizontal filter and 2:1 subsampling */

void 
MpegEncoder::conv444to422(unsigned char* src, unsigned char* dst)
{
    int i, j, im5, im4, im3, im2, im1, ip1, ip2, ip3, ip4, ip5, ip6;

    if (m_mpeg1)
    {
        for (j = 0; j < m_height; j++)
        {
            for (i = 0; i < m_width; i += 2)
            {
                im5 = (i<5) ? 0 : i-5;
                im4 = (i<4) ? 0 : i-4;
                im3 = (i<3) ? 0 : i-3;
                im2 = (i<2) ? 0 : i-2;
                im1 = (i<1) ? 0 : i-1;
                ip1 = (i<m_width-1) ? i+1 : m_width-1;
                ip2 = (i<m_width-2) ? i+2 : m_width-1;
                ip3 = (i<m_width-3) ? i+3 : m_width-1;
                ip4 = (i<m_width-4) ? i+4 : m_width-1;
                ip5 = (i<m_width-5) ? i+5 : m_width-1;
                ip6 = (i<m_width-6) ? i+6 : m_width-1;

                /* FIR filter with 0.5 sample interval phase shift */
                dst[i>>1] = m_clp[(int)(228 * (src[i] + src[ip1])
                    +70*(src[im1]+src[ip2])
                    -37*(src[im2]+src[ip3])
                    -21*(src[im3]+src[ip4])
                    +11*(src[im4]+src[ip5])
                    + 5*(src[im5]+src[ip6])+256)>>9];
            }
            src+= m_width;
            dst+= m_width>>1;
        }
    }
    else
    {
        /* MPEG-2 */
        for (j = 0; j < m_height; j++)
        {
            for (i = 0; i < m_width; i += 2)
            {
                im5 = (i<5) ? 0 : i-5;
                im3 = (i<3) ? 0 : i-3;
                im1 = (i<1) ? 0 : i-1;
                ip1 = (i<m_width-1) ? i+1 : m_width-1;
                ip3 = (i<m_width-3) ? i+3 : m_width-1;
                ip5 = (i<m_width-5) ? i+5 : m_width-1;

                /* FIR filter coefficients (*512): 22 0 -52 0 159 256 159 0 -52 0 22 */
                dst[i>>1] = m_clp[(int)(  22*(src[im5]+src[ip5])-52*(src[im3]+src[ip3])
                    +159*(src[im1]+src[ip1])+256*src[i]+256)>>9];
            }
            src += m_width;
            dst += m_width>>1;
        }
    }
}

/* vertical filter and 2:1 subsampling */

void 
MpegEncoder::conv422to420(unsigned char* src, unsigned char* dst)
{
    int w, i, j, jm6, jm5, jm4, jm3, jm2, jm1;
    int jp1, jp2, jp3, jp4, jp5, jp6;

    w = m_width>>1;

    if (m_prog_frame)
    {
        /* intra frame */
        for (i = 0; i < w; i++)
        {
            for (j = 0; j < m_height; j += 2)
            {
                jm5 = (j<5) ? 0 : j-5;
                jm4 = (j<4) ? 0 : j-4;
                jm3 = (j<3) ? 0 : j-3;
                jm2 = (j<2) ? 0 : j-2;
                jm1 = (j<1) ? 0 : j-1;
                jp1 = (j < m_height - 1) ? j + 1 : m_height - 1;
                jp2 = (j < m_height - 2) ? j + 2 : m_height - 1;
                jp3 = (j < m_height - 3) ? j + 3 : m_height - 1;
                jp4 = (j < m_height - 4) ? j + 4 : m_height - 1;
                jp5 = (j < m_height - 5) ? j + 5 : m_height - 1;
                jp6 = (j < m_height - 6) ? j + 6 : m_height - 1;

                /* FIR filter with 0.5 sample interval phase shift */
                dst[w * (j >> 1)] = m_clp[(int)(228*(src[w*j]+src[w*jp1])
                    +70*(src[w*jm1]+src[w*jp2])
                    -37*(src[w*jm2]+src[w*jp3])
                    -21*(src[w*jm3]+src[w*jp4])
                    +11*(src[w*jm4]+src[w*jp5])
                    + 5*(src[w*jm5]+src[w*jp6])+256)>>9];
            }
            src++;
            dst++;
        }
    }
    else
    {
        /* intra field */
        for (i = 0; i < w; i++)
        {
            for (j = 0; j < m_height; j += 4)
            {
                /* top field */
                jm5 = (j<10) ? 0 : j-10;
                jm4 = (j<8) ? 0 : j-8;
                jm3 = (j<6) ? 0 : j-6;
                jm2 = (j<4) ? 0 : j-4;
                jm1 = (j<2) ? 0 : j-2;
                jp1 = (j < m_height - 2) ? j + 2 : m_height - 2;
                jp2 = (j < m_height - 4) ? j + 4 : m_height - 2;
                jp3 = (j < m_height - 6) ? j + 6 : m_height - 2;
                jp4 = (j < m_height - 8) ? j + 8 : m_height - 2;
                jp5 = (j < m_height - 10) ? j + 10 : m_height - 2;
                jp6 = (j < m_height - 12) ? j + 12: m_height - 2;

                /* FIR filter with 0.25 sample interval phase shift */
                dst[w*(j>>1)] = m_clp[(int)(8*src[w*jm5]
                    +5*src[w*jm4]
                    -30*src[w*jm3]
                    -18*src[w*jm2]
                    +113*src[w*jm1]
                    +242*src[w*j]
                    +192*src[w*jp1]
                    +35*src[w*jp2]
                    -38*src[w*jp3]
                    -10*src[w*jp4]
                    +11*src[w*jp5]
                    +2*src[w*jp6]+256)>>9];

                /* bottom field */
                jm6 = (j<9) ? 1 : j-9;
                jm5 = (j<7) ? 1 : j-7;
                jm4 = (j<5) ? 1 : j-5;
                jm3 = (j<3) ? 1 : j-3;
                jm2 = (j<1) ? 1 : j-1;
                jm1 = (j < m_height - 1) ? j + 1 : m_height - 1;

                jp1 = (j < m_height - 3) ? j + 3 : m_height - 1;
                jp2 = (j < m_height - 5) ? j + 5 : m_height - 1;
                jp3 = (j < m_height - 7) ? j + 7 : m_height - 1;
                jp4 = (j < m_height - 9) ? j + 9 : m_height - 1;
                jp5 = (j < m_height - 11) ? j + 11 : m_height - 1;
                jp6 = (j < m_height - 13) ? j + 13 : m_height - 1;

                /* FIR filter with 0.25 sample interval phase shift */
                dst[w*((j>>1)+1)] = m_clp[(int)(8*src[w*jp6]
                    +5*src[w*jp5]
                    -30*src[w*jp4]
                    -18*src[w*jp3]
                    +113*src[w*jp2]
                    +242*src[w*jp1]
                    +192*src[w*jm1]
                    +35*src[w*jm2]
                    -38*src[w*jm3]
                    -10*src[w*jm4]
                    +11*src[w*jm5]
                    +2*src[w*jm6]+256)>>9];
            }
            src++;
            dst++;
        }
    }
}

/* pbm_getc() and pbm_getint() are essentially taken from
 * PBMPLUS (C) Jef Poskanzer
 * but without error/EOF checking
 */

char 
MpegEncoder::pbm_getc(FILE* file)
{
    char ch;

    ch = getc(file);

    if (ch=='#')
    {
        do
        {
            ch = getc(file);
        }
        while (ch!='\n' && ch!='\r');
    }

    return ch;
}

int 
MpegEncoder::pbm_getint(FILE* file)
{
    char ch;
    int i;

    do
    {
        ch = pbm_getc(file);
    }
    while (ch==' ' || ch=='\t' || ch=='\n' || ch=='\r');

    i = 0;
    do
    {
        i = i*10 + ch-'0';
        ch = pbm_getc(file);
    }
    while (ch>='0' && ch<='9');

    return i;
}

void 
MpegEncoder::read_qimage(char* file_name, unsigned char* frame[])
{
    unsigned int                current_pixel;
    int                         i, j;
    int                         r, g, b;
    double                      y, u, v;
    double                      cr, cg, cb, cu, cv;
    unsigned char               *yp, *up, *vp;
    static unsigned char        *u444, *v444, *u422, *v422;

    static double coef[7][3] = 
    {
        {0.2125,0.7154,0.0721}, /* ITU-R Rec. 709 (1990) */
        {0.299, 0.587, 0.114},  /* unspecified */
        {0.299, 0.587, 0.114},  /* reserved */
        {0.30,  0.59,  0.11},   /* FCC */
        {0.299, 0.587, 0.114},  /* ITU-R Rec. 624-4 System B, G */
        {0.299, 0.587, 0.114},  /* SMPTE 170M */
        {0.212, 0.701, 0.087}
    }; /* SMPTE 240M (1987) */

    i = m_matrix_coefficients;

    if (i > 8)
    {
        i = 3;
    }

    cr = coef[i-1][0];
    cg = coef[i-1][1];
    cb = coef[i-1][2];
    cu = 0.5 / (1.0 - cb);
    cv = 0.5 / (1.0 - cr);

    if (m_chroma_format == CHROMA444)
    {
        u444 = frame[1];
        v444 = frame[2];
    }
    else
    {
        if (!u444)
        {
            u444 = new unsigned char[m_width * m_height];
            v444 = new unsigned char[m_width * m_height];

            if (m_chroma_format == CHROMA420)
            {
                u422 = new unsigned char[(m_width >> 1) * m_height];	
                v422 = new unsigned char[(m_width >> 1) * m_height];
            }
        }
    }

    std::string full_name = file_name;
    full_name += m_input_filename_extension;
    
    //if we are dealing with a tga we need to let
    //the image loader know this
    //image.load(name, "TGA");
    QImage image;
    bool successfully_loaded = image.load(full_name.c_str(), 0);

    if (!successfully_loaded)
    {
        bool failed_to_load = true;
        return;
    }

    int image_width = image.width();
    int image_height = image.height();

    float width_ratio = float( (m_horizontal_size + 1) / float(image_width) );
    float height_ratio =  float( (m_vertical_size + 1) / float(image_height) );
    
    QImage scaled_image = 
        image.smoothScale( int(image_width * width_ratio), int(image_height * height_ratio) );
    
    for (i = 0; i < m_vertical_size; i++)
    {
        yp = frame[0] + (i * m_width);
        up = u444 + (i * m_width);
        vp = v444 + (i * m_width);

        for (j = 0; j < m_horizontal_size; j++)
        {
            current_pixel = (unsigned int)scaled_image.pixel(j, i);

            r = (int)((current_pixel >> 16) & 0xff);
            g = (int)((current_pixel >>  8) & 0xff);
            b = (int)((current_pixel >>  0) & 0xff);

            /* convert to YUV */
            y = (cr * r) + (cg * g) + (cb * b);
            u = cu * (b - y);
            v = cv * (r - y);

            yp[j] = ( unsigned char )( ((219.0 / 256.0) * y) + 16.5 );  /* nominal range: 16..235 */
            up[j] = ( unsigned char )( ((224.0 / 256.0) * u) + 128.5 ); /* 16..240 */
            vp[j] = ( unsigned char )( ((224.0 / 256.0) * v) + 128.5 ); /* 16..240 */
        }
    }

    border_extend(frame[0], m_horizontal_size, m_vertical_size, m_width, m_height);
    border_extend(u444, m_horizontal_size, m_vertical_size, m_width, m_height);
    border_extend(v444, m_horizontal_size, m_vertical_size, m_width, m_height);

    if (m_chroma_format == CHROMA422)
    {
        conv444to422(u444, frame[1]);
        conv444to422(v444, frame[2]);
    }

    if (m_chroma_format == CHROMA420)
    {
        conv444to422(u444,u422);
        conv444to422(v444,v422);
        conv422to420(u422,frame[1]);
        conv422to420(v422,frame[2]);
    }
}
