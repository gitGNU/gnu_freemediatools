
/* mpeg2dec.c, main(), initialization, option processing                    */

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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif


#define GLOBAL
#include "config.h"
#include "global.h"

#include "mpeg2dec.h"


/* private prototypes */
static int  video_sequence _ANSI_ARGS_((int *framenum));
static int Decode_Bitstream _ANSI_ARGS_((void));
static int  Headers _ANSI_ARGS_((void));
static void Initialize_Sequence _ANSI_ARGS_((void));
static void Initialize_Decoder _ANSI_ARGS_((void));
static void Deinitialize_Sequence _ANSI_ARGS_((void));
static void Process_Options _ANSI_ARGS_((int argc, char *argv[]));

struct mpeg_image_data					mpeg_data;
int										mpeg_input_file_is_open = 0;
int                                     mpeg_frame_number;           



#if OLD
static int  Get_Val _ANSI_ARGS_((char *argv[]));
#endif

/* #define DEBUG */

static void Clear_Options();
#ifdef DEBUG
static void Print_Options();
#endif

#define necessary
#ifdef necessary

int mpegdecmain()
{
	int 							ret, code;
	static char*					argv[5];
	static char						args[5][500];

	if (mpeg_data.frame_number < 1)
	{
		// Hack to keep a defective calling function from trying to start
		// before frame 1 FIXME
		mpeg_data.frame_number = 1;
	}
	else if (mpeg_data.frame_number > mpeg_data.number_of_frames)
	{
		// Hack to keep a defective calling function from trying to stream 
		// past the end of the sequence FIXME
		mpeg_data.frame_number = mpeg_data.number_of_frames;
	}

	if (mpeg_data.purpose == MPEGDEC_STREAM_ONE_FRAME && 
		(mpeg_data.frame_number <= (mpeg_data.number_of_frames - 1)) &&
		(mpeg_data.frame_number > mpeg_data.last_streaming_frame_number))
	{
	//	printf("mpegdecmain streaming frame # %d\n", mpeg_data.frame_number);
		Headers();
		mpeg_data.dont_write = 0;
		Decode_Picture(mpeg_data.frame_number, mpeg_data.frame_number);	
		mpeg_data.last_streaming_frame_number = mpeg_data.frame_number;

		if (mpeg_data.frame_number == (mpeg_data.number_of_frames - 1))
		{
			// We need a better way of signalling that we have arrived at the
			// last frame so that memory can be deallocated.  FIXME
			Deinitialize_Sequence();
		}

		return(0);
	}

	if (mpeg_input_file_is_open)
	{
		if (mpeg_data.purpose == MPEGDEC_CLOSE)
		{
			close(base.Infile);
			mpeg_input_file_is_open = 0;
			return(-1);
		}
	}
	else if (mpeg_data.purpose == MPEGDEC_CLOSE)
	{
		return(-1);
	}
	else if (mpeg_data.purpose == MPEGDEC_OPEN || mpeg_data.purpose == MPEGDEC_DECODE_ONE_FRAME)
	{
		// mpeg_data.pixel_buffer needs to be initialized somewhere.  FIXME
		if (!mpeg_data.pixel_buffer)
		{
			// Make this buffer large enough for any image.  Because of the way
			// mpegdec is structured, It is very
			// difficult to determine the actual image size before this buffer is needed.
			mpeg_data.pixel_buffer = 
					(unsigned char*)calloc(DECODER_MAX_IMAGE_DIMENSION * DECODER_MAX_IMAGE_DIMENSION * 4, sizeof(unsigned char));
		}

	}


  	Clear_Options();

  /* decode command line arguments */
	strcpy(args[1], "-O3");
	strcpy(args[2], "test");
	strcpy(args[3], "-b");
	strcpy(args[4], mpeg_data.filename);
	argv[1] = args[1]; argv[2] = args[2]; argv[3] = args[3]; argv[4] =args[4];
	Process_Options(5, argv);

	

#ifdef DEBUG
  Print_Options();
#endif

  ld = &base; /* select base layer context */

  /* open MPEG base layer bitstream file(s) */
  /* NOTE: this is either a base layer stream or a spatial enhancement stream */
  if (!mpeg_input_file_is_open)
  {
	  if ((base.Infile = open(Main_Bitstream_Filename, O_RDONLY | O_BINARY)) < 0)
	  {
		  fprintf(stderr, "Base layer input file %s not found\n", Main_Bitstream_Filename);
		  //exit(1);
	  }

	  mpeg_input_file_is_open = 1;
  
	  if(base.Infile != 0)
	  {
		  Initialize_Buffer(); 
  
		  if(Show_Bits(8) == 0x47)
		  {
			  sprintf(Error_Text, "Decoder currently does not parse transport streams\n");
			  Error(Error_Text);
		  }

		  next_start_code();
		  code = Show_Bits(32);

		  switch(code)
		  {
			  case DECODER_SEQUENCE_HEADER_CODE:
				  break;
			  case DECODER_PACK_START_CODE:
				  System_Stream_Flag = 1;
			  case DECODER_VIDEO_ELEMENTARY_STREAM:
				  System_Stream_Flag = 1;
				  break;
			  default:
				  sprintf(Error_Text, "Unable to recognize stream type\n");
				  Error(Error_Text);
				  break;
		  }

		  lseek(base.Infile, 0l, 0);
		  Initialize_Buffer(); 
	  }
  }

  if(base.Infile!=0)
  {
    lseek(base.Infile, 0l, 0);
  }

  Initialize_Buffer(); 

  if(Two_Streams)
  {
    ld = &enhan; /* select enhancement layer context */

    if ((enhan.Infile = open(Enhancement_Layer_Bitstream_Filename, O_RDONLY | O_BINARY)) < 0)
    {
      sprintf(Error_Text,"enhancment layer bitstream file %s not found\n",
        Enhancement_Layer_Bitstream_Filename);

      Error(Error_Text);
    }

    Initialize_Buffer();
    ld = &base;
  }

  Initialize_Decoder();

  ret = Decode_Bitstream();
  mpeg_data.horizontal_size = horizontal_size;
  mpeg_data.vertical_size = vertical_size;



  //close(base.Infile);

  if (Two_Streams)
    close(enhan.Infile);

  return 0;
}

#endif

/* IMPLEMENTAION specific rouintes */
static void Initialize_Decoder()
{
  int i;

  /* Clip table */
  if (!(Clip=(unsigned char *)malloc(1024)))
    Error("Clip[] malloc failed\n");

  Clip += 384;

  for (i=-384; i<640; i++)
    Clip[i] = (i<0) ? 0 : ((i>255) ? 255 : i);

  /* IDCT */
  if (Reference_IDCT_Flag)
    Initialize_Reference_IDCT();
  else
    Initialize_Fast_IDCT();

}

/* mostly IMPLEMENTAION specific rouintes */
static void Initialize_Sequence()
{
  int cc, size;
  static int Table_6_20[3] = {6,8,12};

  /* check scalability mode of enhancement layer */
  if (Two_Streams && (enhan.scalable_mode!=DECODER_SC_SNR) && (base.scalable_mode!=DECODER_SC_DP))
    Error("unsupported scalability mode\n");

  /* force MPEG-1 parameters for proper decoder behavior */
  /* see ISO/IEC 13818-2 section D.9.14 */
  if (!base.MPEG2_Flag)
  {
    progressive_sequence = 1;
    progressive_frame = 1;
    picture_structure = DECODER_FRAME_PICTURE;
    frame_pred_frame_dct = 1;
    chroma_format = DECODER_CHROMA420;
    matrix_coefficients = 5;
  }

  /* round to nearest multiple of coded macroblocks */
  /* ISO/IEC 13818-2 section 6.3.3 sequence_header() */
  mb_width = (horizontal_size+15)/16;
  mb_height = (base.MPEG2_Flag && !progressive_sequence) ? 2*((vertical_size+31)/32)
                                        : (vertical_size+15)/16;

  Coded_Picture_Width = 16*mb_width;
  Coded_Picture_Height = 16*mb_height;

  /* ISO/IEC 13818-2 sections 6.1.1.8, 6.1.1.9, and 6.1.1.10 */
  Chroma_Width = (chroma_format==DECODER_CHROMA444) ? Coded_Picture_Width
                                           : Coded_Picture_Width>>1;
  Chroma_Height = (chroma_format!=DECODER_CHROMA420) ? Coded_Picture_Height
                                            : Coded_Picture_Height>>1;
  
  /* derived based on Table 6-20 in ISO/IEC 13818-2 section 6.3.17 */
  block_count = Table_6_20[chroma_format-1];

  for (cc=0; cc<3; cc++)
  {
    if (cc==0)
      size = Coded_Picture_Width*Coded_Picture_Height;
    else
      size = Chroma_Width*Chroma_Height;

    if (!(backward_reference_frame[cc] = (unsigned char *)malloc(size)))
      Error("backward_reference_frame[] malloc failed\n");

    if (!(forward_reference_frame[cc] = (unsigned char *)malloc(size)))
      Error("forward_reference_frame[] malloc failed\n");

    if (!(auxframe[cc] = (unsigned char *)malloc(size)))
      Error("auxframe[] malloc failed\n");

    if(Ersatz_Flag)
      if (!(substitute_frame[cc] = (unsigned char *)malloc(size)))
        Error("substitute_frame[] malloc failed\n");


    if (base.scalable_mode==DECODER_SC_SPAT)
    {
      /* this assumes lower layer is 4:2:0 */
      if (!(llframe0[cc] = (unsigned char *)malloc((lower_layer_prediction_horizontal_size*lower_layer_prediction_vertical_size)/(cc?4:1))))
        Error("llframe0 malloc failed\n");
      if (!(llframe1[cc] = (unsigned char *)malloc((lower_layer_prediction_horizontal_size*lower_layer_prediction_vertical_size)/(cc?4:1))))
        Error("llframe1 malloc failed\n");
    }
  }

  /* SCALABILITY: Spatial */
  if (base.scalable_mode==DECODER_SC_SPAT)
  {
    if (!(lltmp = (short *)malloc(lower_layer_prediction_horizontal_size*((lower_layer_prediction_vertical_size*vertical_subsampling_factor_n)/vertical_subsampling_factor_m)*sizeof(short))))
      Error("lltmp malloc failed\n");
  }

#ifdef DISPLAY
  if (Output_Type==T_X11)
  {
    Initialize_Display_Process("");
    Initialize_Dither_Matrix();
  }
#endif /* DISPLAY */

}

void Error(text)
char *text;
{
  fprintf(stderr,text);
  exit(1);
}

/* Trace_Flag output */
void Print_Bits(code,bits,len)
int code,bits,len;
{
  int i;
  for (i=0; i<len; i++)
    printf("%d",(code>>(bits-1-i))&1);
}



/* option processing */
static void Process_Options(argc,argv)
    int argc;                  /* argument count  */
    char *argv[];              /* argument vector */
{
    int i, LastArg, NextArg;

    /* at least one argument should be present */
    if (argc<2)
    {
        printf("\n%s, %s\n",Version,Author);
        printf("Usage:  mpeg2decode {options}\n\
Options: -b  file  main bitstream (base or spatial enhancement layer)\n\
         -cn file  conformance report (n: level)\n\
         -e  file  enhancement layer bitstream (SNR or Data Partitioning)\n\
         -f        store/display interlaced video in frame format\n\
         -g        concatenated file format for substitution method (-x)\n\
         -in file  information & statistics report  (n: level)\n\
         -l  file  file name pattern for lower layer sequence\n\
                   (for spatial scalability)\n\
         -on file  output format (0:YUV 1:SIF 2:TGA 3:PPM 4:QIMAGE 5:X11 6:X11HiQ)\n\
         -q        disable warnings to stderr\n\
         -r        use double precision reference IDCT\n\
         -t        enable low level tracing to stdout\n\
         -u  file  print user_data to stdio or file\n\
         -vn       verbose output (n: level)\n\
         -x  file  filename pattern of picture substitution sequence\n\n\
File patterns:  for sequential filenames, \"printf\" style, e.g. rec%%d\n\
                 or rec%%d%%c for fieldwise storage\n\
Levels:        0:none 1:sequence 2:picture 3:slice 4:macroblock 5:block\n\n\
Example:       mpeg2decode -b bitstream.mpg -f -r -o0 rec%%d\n\
         \n");
        exit(0);
    }


    Output_Type = -1;
    i = 1;

    /* command-line options are proceeded by '-' */

    while(i < argc)
    {
        /* check if this is the last argument */
        LastArg = ((argc-i)==1);

        /* parse ahead to see if another flag immediately follows current
           argument (this is used to tell if a filename is missing) */
        if(!LastArg)
            NextArg = (argv[i+1][0]=='-');
        else
            NextArg = 0;

        /* second character, [1], after '-' is the switch */
        if(argv[i][0]=='-')
        {
            switch(toupper(argv[i][1]))
            {
                /* third character. [2], is the value */
                case 'B':
                    Main_Bitstream_Flag = 1;

                    if(NextArg || LastArg)
                    {
                        printf("ERROR: -b must be followed the main bitstream filename\n");
                    }
                    else
                        Main_Bitstream_Filename = argv[++i]; 

                    break;


                case 'C':

#ifdef VERIFY
                    Verify_Flag = atoi(&argv[i][2]); 

                    if((Verify_Flag < NO_LAYER) || (Verify_Flag > ALL_LAYERS))
                    {
                        printf("ERROR: -c level (%d) out of range [%d,%d]\n",
                            Verify_Flag, NO_LAYER, ALL_LAYERS);
                        exit(MPEGDEC_ERROR);
                    }
#else  /* VERIFY */
                    printf("This program not compiled for Verify_Flag option\n");
#endif /* VERIFY */
                    break;

                case 'E':
                    Two_Streams = 1; /* either Data Partitioning (DP) or SNR Scalability enhancment */
                       
                    if(NextArg || LastArg)
                    {
                        printf("ERROR: -e must be followed by filename\n");
                        exit(MPEGDEC_ERROR);
                    }
                    else
                        Enhancement_Layer_Bitstream_Filename = argv[++i]; 

                    break;


                case 'F':
                    Frame_Store_Flag = 1;
                    break;

                case 'G':
                    Big_Picture_Flag = 1;
                    break;


                case 'I':
#ifdef VERIFY
                    Stats_Flag = atoi(&argv[i][2]); 
#else /* VERIFY */
                    printf("WARNING: This program not compiled for -i option\n");
#endif /* VERIFY */     
                    break;
    
                case 'L':  /* spatial scalability flag */
                    Spatial_Flag = 1;

                    if(NextArg || LastArg)
                    {
                        printf("ERROR: -l must be followed by filename\n");
                        exit(MPEGDEC_ERROR);
                    }
                    else
                        Lower_Layer_Picture_Filename = argv[++i]; 

                    break;

                case 'O':
  
                    Output_Type = atoi(&argv[i][2]); 
  
                    if((Output_Type == T_QIMAGE) || (Output_Type == T_X11) || (Output_Type == T_X11HIQ))
                        Output_Picture_Filename = "test";  /* no need of filename */
                    else if(NextArg || LastArg)  
                    {
                        printf("ERROR: -o must be followed by filename\n");
                        exit(MPEGDEC_ERROR);
                    }
                    else
                        /* filename is separated by space, so it becomes the next argument */
                        Output_Picture_Filename = argv[++i]; 

#ifdef DISPLAY
                    if (Output_Type==T_X11HIQ)
                    {
                        hiQdither = 1;
                        Output_Type=T_X11;
                    }
#endif /* DISPLAY */
                    break;

                case 'Q':
                    Quiet_Flag = 1;
                    break;

                case 'R':
                    Reference_IDCT_Flag = 1;
                    break;
    
                case 'T':
#ifdef TRACE
                    Trace_Flag = 1;
#else /* TRACE */
                    printf("WARNING: This program not compiled for -t option\n");
#endif /* TRACE */
                    break;

                case 'U':
                    User_Data_Flag = 1;

                case 'V':
#ifdef VERBOSE
                    Verbose_Flag = atoi(&argv[i][2]); 
#else /* VERBOSE */
                    printf("This program not compiled for -v option\n");
#endif /* VERBOSE */
                    break;


                case 'X':
                    Ersatz_Flag = 1;

                    if(NextArg || LastArg)
                    {
                        printf("ERROR: -x must be followed by filename\n"); 
                        exit(MPEGDEC_ERROR);
                    }
                    else
                        Substitute_Picture_Filename = argv[++i]; 

                    break;



                default:
                    fprintf(stderr,"undefined option -%c ignored. Exiting program\n", 
                        argv[i][1]);

                    exit(MPEGDEC_ERROR);
    
            } /* switch() */
        } /* if argv[i][0] == '-' */
    
        i++;

        /* check for bitstream filename argument (there must always be one, at the very end
           of the command line arguments */

    } /* while() */


    /* options sense checking */

    if(Main_Bitstream_Flag!=1)
    {
        printf("There must be a main bitstream specified (-b filename)\n");
    }

    /* force display process to show frame pictures */
    if((Output_Type == T_X11 || Output_Type == T_X11HIQ) && Frame_Store_Flag)
        Display_Progressive_Flag = 1;
    else
        Display_Progressive_Flag = 0;

#ifdef VERIFY
    /* parse the bitstream, do not actually decode it completely */
  

#if 0
    if(Output_Type==-1)
    {
        Decode_Layer = Verify_Flag;
        printf("FYI: Decoding bitstream elements up to: %s\n", 
            Layer_Table[Decode_Layer]);
    }
    else
#endif
        Decode_Layer = ALL_LAYERS;

#endif /* VERIFY */

    /* no output type specified */
    if(Output_Type==-1)
    {
        Output_Type = 9; 
        Output_Picture_Filename = "";
    }


#ifdef DISPLAY
    if (Output_Type==T_X11)
    {
        if(Frame_Store_Flag)
            Display_Progressive_Flag = 1;
        else
            Display_Progressive_Flag = 0;

        Frame_Store_Flag = 1; /* to avoid calling dither() twice */
    }
#endif


}


#ifdef OLD
/* 
   this is an old routine used to convert command line arguments
   into integers 
*/
static int Get_Val(argv)
char *argv[];
{
  int val;

  if (sscanf(argv[1]+2,"%d",&val)!=1)
    return 0;

  while (isdigit(argv[1][2]))
    argv[1]++;

  return val;
}
#endif



static int Headers()
{
  int ret;

  ld = &base;
  

  /* return when end of sequence (0) or picture
     header has been parsed (1) */

  ret = Get_Hdr();


  if (Two_Streams)
  {
    ld = &enhan;
    if (Get_Hdr()!=ret && !Quiet_Flag)
      fprintf(stderr,"streams out of sync\n");
    ld = &base;
  }

  return ret;
}



static int Decode_Bitstream()
{
  int ret;
  int Bitstream_Framenum;

  Bitstream_Framenum = 30;

  for(;;)
  {

#ifdef VERIFY
    Clear_Verify_Headers();
#endif /* VERIFY */

    ret = Headers();
    
    if(ret==1)
    {
      	ret = video_sequence(&Bitstream_Framenum);

		if (ret == -5)
		{
			return(ret);
		}
    }
    else
      return(ret);
  }

}


static void Deinitialize_Sequence()
{
  int i;

  /* clear flags */
  base.MPEG2_Flag=0;

  for(i=0;i<3;i++)
  {
    free(backward_reference_frame[i]);
    free(forward_reference_frame[i]);
    free(auxframe[i]);

    if (base.scalable_mode==DECODER_SC_SPAT)
    {
     free(llframe0[i]);
     free(llframe1[i]);
    }
  }

  if (base.scalable_mode==DECODER_SC_SPAT)
    free(lltmp);

#ifdef DISPLAY
  if (Output_Type==DECODER_T_X11) 
    Terminate_Display_Process();
#endif
}


static int video_sequence(Bitstream_Framenumber)
int *Bitstream_Framenumber;
{
  int Bitstream_Framenum;
  int Sequence_Framenum;
  int Return_Value;

  Bitstream_Framenum = *Bitstream_Framenumber;
  Sequence_Framenum=0;

  Initialize_Sequence();

  /* decode picture whose header has already been parsed in 
     Decode_Bitstream() */

  //if ((mpeg_data.frame_number - Sequence_Framenum) >= 1)
  {
	  Decode_Picture(Bitstream_Framenum, Sequence_Framenum);
  }

  /* update picture numbers */
  if (!Second_Field)
  {
    Bitstream_Framenum++;
    Sequence_Framenum++;
  }


  /* loop through the rest of the pictures in the sequence */
  while ((Return_Value = Headers()))
  {
	  mpeg_data.dont_write = 0;

	  if (mpeg_data.purpose == MPEGDEC_STREAM_ONE_FRAME &&
		  mpeg_data.frame_number != Sequence_Framenum)
	  {
		  mpeg_data.dont_write = 1;
	  }
	  else if (mpeg_data.purpose == MPEGDEC_COUNT_FRAMES)
	  {
		  mpeg_data.dont_write = 1;
	  }
	  else if (mpeg_data.purpose == MPEGDEC_DECODE_ONE_FRAME &&
		  mpeg_data.frame_number != Sequence_Framenum)
	  {
		  mpeg_data.dont_write = 1;
	  }

	  //if (!mpeg_data.dont_write)
	  if ( (!mpeg_data.dont_write || mpeg_data.purpose != MPEGDEC_DECODE_ONE_FRAME) )
	  { 
		  //printf("Starting Decode_Picture,  Sequence_Framenum = %d\n", Sequence_Framenum);
		  Decode_Picture(Bitstream_Framenum, Sequence_Framenum);


		  if (mpeg_data.purpose == MPEGDEC_STREAM_ONE_FRAME && 
			  (Sequence_Framenum < mpeg_data.number_of_frames) &&
			  (mpeg_data.frame_number == Sequence_Framenum))
		  {
				mpeg_data.last_streaming_frame_number = Sequence_Framenum;
  				return(-5);
		  }
	  }

	  if (!Second_Field)
	  {
		  Bitstream_Framenum++;
		  Sequence_Framenum++;
	  }
  }

  mpeg_data.number_of_frames = Sequence_Framenum - 1;
  mpeg_data.last_streaming_frame_number = Sequence_Framenum;


  /* put last frame */
  if (Sequence_Framenum!=0)
  {
    Output_Last_Frame_of_Sequence(Bitstream_Framenum);
  }

  Deinitialize_Sequence();

#ifdef VERIFY
    Clear_Verify_Headers();
#endif /* VERIFY */

  *Bitstream_Framenumber = Bitstream_Framenum;
  return(Return_Value);
}



static void Clear_Options()
{
  Verbose_Flag = 0;
  Output_Type = 0;
  Output_Picture_Filename = " ";
  hiQdither  = 0;
  Output_Type = 0;
  Frame_Store_Flag = 0;
  Spatial_Flag = 0;
  Lower_Layer_Picture_Filename = " ";
  Reference_IDCT_Flag = 0;
  Trace_Flag = 0;
  Quiet_Flag = 0;
  Ersatz_Flag = 0;
  Substitute_Picture_Filename  = " ";
  Two_Streams = 0;
  Enhancement_Layer_Bitstream_Filename = " ";
  Big_Picture_Flag = 0;
  Main_Bitstream_Flag = 0;
  Main_Bitstream_Filename = " ";
  Verify_Flag = 0;
  Stats_Flag  = 0;
  User_Data_Flag = 0; 
}


#ifdef DEBUG
static void Print_Options()
{
  
  printf("Verbose_Flag                         = %d\n", Verbose_Flag);
  printf("Output_Type                          = %d\n", Output_Type);
  printf("Output_Picture_Filename              = %s\n", Output_Picture_Filename);
  printf("hiQdither                            = %d\n", hiQdither);
  printf("Output_Type                          = %d\n", Output_Type);
  printf("Frame_Store_Flag                     = %d\n", Frame_Store_Flag);
  printf("Spatial_Flag                         = %d\n", Spatial_Flag);
  printf("Lower_Layer_Picture_Filename         = %s\n", Lower_Layer_Picture_Filename);
  printf("Reference_IDCT_Flag                  = %d\n", Reference_IDCT_Flag);
  printf("Trace_Flag                           = %d\n", Trace_Flag);
  printf("Quiet_Flag                           = %d\n", Quiet_Flag);
  printf("Ersatz_Flag                          = %d\n", Ersatz_Flag);
  printf("Substitute_Picture_Filename          = %s\n", Substitute_Picture_Filename);
  printf("Two_Streams                          = %d\n", Two_Streams);
  printf("Enhancement_Layer_Bitstream_Filename = %s\n", Enhancement_Layer_Bitstream_Filename);
  printf("Big_Picture_Flag                     = %d\n", Big_Picture_Flag);
  printf("Main_Bitstream_Flag                  = %d\n", Main_Bitstream_Flag);
  printf("Main_Bitstream_Filename              = %s\n", Main_Bitstream_Filename);
  printf("Verify_Flag                          = %d\n", Verify_Flag);
  printf("Stats_Flag                           = %d\n", Stats_Flag);
  printf("User_Data_Flag                       = %d\n", User_Data_Flag);

}
#endif
