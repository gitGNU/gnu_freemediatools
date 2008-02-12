/* mpeg2dec.h, MPEG specific defines                                        */

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
#ifndef MPEGDEC_H
#define MPEGDEC_H

#ifdef __cplusplus
extern "C" {
#endif


#define MPEGDEC_ERROR (-1)

#define DECODER_PICTURE_START_CODE      0x100
#define DECODER_SLICE_START_CODE_MIN    0x101
#define DECODER_SLICE_START_CODE_MAX    0x1AF
#define DECODER_USER_DATA_START_CODE    0x1B2
#define DECODER_SEQUENCE_HEADER_CODE    0x1B3
#define DECODER_MDSEQUENCE_ERROR_CODE     0x1B4
#define DECODER_EXTENSION_START_CODE    0x1B5
#define DECODER_SEQUENCE_END_CODE       0x1B7
#define DECODER_GROUP_START_CODE        0x1B8
#define DECODER_SYSTEM_START_CODE_MIN   0x1B9
#define DECODER_SYSTEM_START_CODE_MAX   0x1FF

#define DECODER_ISO_END_CODE            0x1B9
#define DECODER_PACK_START_CODE         0x1BA
#define DECODER_SYSTEM_START_CODE       0x1BB

#define DECODER_VIDEO_ELEMENTARY_STREAM 0x1e0

/* scalable_mode */
#define DECODER_SC_NONE 0
#define DECODER_SC_DP   1
#define DECODER_SC_SPAT 2
#define DECODER_SC_SNR  3
#define DECODER_SC_TEMP 4

/* picture coding type */
#define DECODER_I_TYPE 1
#define DECODER_P_TYPE 2
#define DECODER_B_TYPE 3
#define DECODER_D_TYPE 4

/* picture structure */
#define DECODER_TOP_FIELD     1
#define DECODER_BOTTOM_FIELD  2
#define DECODER_FRAME_PICTURE 3

/* macroblock type */
#define DECODER_MACROBLOCK_INTRA                        1
#define DECODER_MACROBLOCK_PATTERN                      2
#define DECODER_MACROBLOCK_MOTION_BACKWARD              4
#define DECODER_MACROBLOCK_MOTION_FORWARD               8
#define DECODER_MACROBLOCK_QUANT                        16
#define DECODER_SPATIAL_TEMPORAL_WEIGHT_CODE_FLAG       32
#define DECODER_PERMITTED_SPATIAL_TEMPORAL_WEIGHT_CLASS 64


/* motion_type */
#define DECODER_MC_FIELD 1
#define DECODER_MC_FRAME 2
#define DECODER_MC_16X8  2
#define DECODER_MC_DMV   3

/* mv_format */
#define DECODER_MV_FIELD 0
#define DECODER_MV_FRAME 1

/* chroma_format */
#define DECODER_CHROMA420 1
#define DECODER_CHROMA422 2
#define DECODER_CHROMA444 3

/* extension start code IDs */

#define DECODER_SEQUENCE_EXTENSION_ID                    1
#define DECODER_SEQUENCE_DISPLAY_EXTENSION_ID            2
#define DECODER_QUANT_MATRIX_EXTENSION_ID                3
#define DECODER_COPYRIGHT_EXTENSION_ID                   4
#define DECODER_SEQUENCE_SCALABLE_EXTENSION_ID           5
#define DECODER_PICTURE_DISPLAY_EXTENSION_ID             7
#define DECODER_PICTURE_CODING_EXTENSION_ID              8
#define DECODER_PICTURE_SPATIAL_SCALABLE_EXTENSION_ID    9
#define DECODER_PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID  10

#define DECODER_ZIG_ZAG                                  0

#define DECODER_PROFILE_422                             (128+5)
#define DECODER_MAIN_LEVEL                              8

/* Layers: used by Verbose_Flag, Verifier_Flag, Stats_Flag, and Trace_Flag */
#define DECODER_NO_LAYER                                0
#define DECODER_SEQUENCE_LAYER                          1
#define DECODER_PICTURE_LAYER                           2
#define DECODER_SLICE_LAYER                             3    
#define DECODER_MACROBLOCK_LAYER                        4    
#define DECODER_BLOCK_LAYER                             5
#define DECODER_EVENT_LAYER                             6
#define DECODER_ALL_LAYERS                              7



#define DECODER_FILENAME_LENGTH                       256

//#include "qimagefilter.cpp"



#define DECODER_MB_WEIGHT                  32
#define DECODER_MB_CLASS4                  64

#define DECODER_MAX_IMAGE_DIMENSION		   2400

enum MPEGDEC_PURPOSE
{
	MPEGDEC_OPEN,
	MPEGDEC_GET_HEADERS,
	MPEGDEC_DECODE,
	MPEGDEC_DECODE_ONE_FRAME,
	MPEGDEC_STREAM_ONE_FRAME,
	MPEGDEC_COUNT_FRAMES,
	MPEGDEC_CLOSE
};

struct mpeg_image_data
{
	int							horizontal_size;
	int							vertical_size;
	unsigned char*				pixel_buffer;
	int							frame_number;
	int							number_of_frames;
	int							last_streaming_frame_number;
	char						filename[500];
	int							dont_write;
	enum MPEGDEC_PURPOSE		purpose;
};

extern struct mpeg_image_data	    mpeg_data;
extern enum MPEGDEC_PURPOSE         mpeg_purpose;
extern int                          mpeg_frame_number;  

//EXTERN_C void store_qimage(char* outname, unsigned char* src[], int offset, int incr, int height);
//EXTERN_C void conv422to444(unsigned char* src, unsigned char* dst);
//EXTERN_C void conv420to422(unsigned char* src, unsigned char* dst);
//EXTERN_C void mpegdec_Read_Headers();

int mpegdecmain();



                        
#ifdef __cplusplus
};
#endif

#endif
