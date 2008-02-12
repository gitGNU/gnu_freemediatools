/* mpg2enc.h, defines and types                                             */

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

#ifndef MPEG2ENC_H
#define MPEG2ENC_H

#include "vlc.h"
#include <string>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <qthread.h>
#include <sstream>

class Globals;

static const int PICTURE_START_CODE = 0x100L;
static const int SLICE_MIN_START    = 0x101L;
static const int SLICE_MAX_START    = 0x1AFL;
static const int USER_START_CODE    = 0x1B2L;
static const int SEQ_START_CODE     = 0x1B3L;
static const int EXT_START_CODE     = 0x1B5L;
static const int SEQ_END_CODE       = 0x1B7L;
static const int GOP_START_CODE     = 0x1B8L;
static const int ISO_END_CODE       = 0x1B9L;
static const int PACK_START_CODE    = 0x1BAL;
static const int SYSTEM_START_CODE  = 0x1BBL;

/* picture coding type */
static const int I_TYPE = 1;
static const int P_TYPE = 2;
static const int B_TYPE = 3;
static const int D_TYPE = 4;

/* picture structure */
static const int TOP_FIELD     = 1;
static const int BOTTOM_FIELD  = 2;
static const int FRAME_PICTURE = 3;

/* macroblock type */
static const int MB_INTRA    = 1;
static const int MB_PATTERN  = 2;
static const int MB_BACKWARD = 4;
static const int MB_FORWARD  = 8;
static const int MB_QUANT    = 16;

/* motion_type */
static const int MC_FIELD = 1;
static const int MC_FRAME = 2;
static const int MC_16X8  = 2;
static const int MC_DMV   = 3;

/* mv_format */
static const int MV_FIELD = 0;
static const int MV_FRAME = 1;

/* chroma_format */
static const int CHROMA420 = 1;
static const int CHROMA422 = 2;
static const int CHROMA444 = 3;

/* extension start code IDs */

static const int SEQ_ID       = 1;
static const int DISP_ID      = 2;
static const int QUANT_ID     = 3;
static const int SEQSCAL_ID   = 5;
static const int PANSCAN_ID   = 7;
static const int CODING_ID    = 8;
static const int SPATSCAL_ID  = 9;
static const int TEMPSCAL_ID = 10;

static const int  LEVEL_ID_LOW                 = 10;
static const int  LEVEL_ID_MAIN                = 8;
static const int  LEVEL_ID_HIGH_1400           = 6;
static const int  LEVEL_ID_HIGH                = 4;


class MpegEncoder : public QThread
{
  public:
    virtual void        run();

    struct mbinfo 
    {
        int mb_type; /* intra/forward/backward/interpolated */
        int motion_type; /* frame/field/16x8/dual_prime */
        int dct_type; /* field/frame DCT */
        int mquant; /* quantization parameter */
        int cbp; /* coded block pattern */
        int skipped; /* skipped macroblock */
        int MV[2][2][2]; /* motion vectors */
        int mv_field_sel[2][2]; /* motion vertical field select */
        int dmvector[2]; /* dual prime vectors */
        double act; /* activity measure */
        int var; /* for debugging */
    };

    /* motion data */
    struct motion_data {
        int forw_hor_f_code,forw_vert_f_code; /* vector range */
        int sxf,syf; /* search range */
        int back_hor_f_code,back_vert_f_code;
        int sxb,syb;
    };


    enum MpegInputFiletype
    {
        T_Y_U_V,
        T_YUV,
        T_PPM,
        T_BMP,
        T_RGB,
        T_RGBA,
        T_QIMAGE,
        NOT_A_VALID_FILETYPE
    };

    enum AspectRatio
    {
        ASPECT_UNDEFINED,
        ASPECT_SQUARE,
        ASPECT_4_3,
        ASPECT_16_9,
        ASPECT_2_11_1,
        ASPECT_NOT_VALID
    };

    enum FrameRate
    {
        FRAME_RATE_INVALID,
        FRAME_RATE_23_976,
        FRAME_RATE_24,
        FRAME_RATE_25,
        FRAME_RATE_29_97,
        FRAME_RATE_30,
        NOT_A_FRAME_RATE
    };

    enum ProfileID
    {
        PROFILE_NONE,
        PROFILE_HIGH,
        PROFILE_SPATIAL,
        PROFILE_SNR,
        PROFILE_MAIN,
        PROFILE_SIMPLE,
        NOT_A_PROFILE
    };

   enum VideoFormat
    {
        FORMAT_COMP,
        FORMAT_PAL,
        FORMAT_NTSC,
        FORMAT_SECAM,
        FORMAT_MAC,
        NOT_A_FORMAT
    };

    enum IntraDCPrecision
    {
        INTRA_DC_PRECISION_8,
        INTRA_DC_PRECISION_9,
        INTRA_DC_PRECISION_10,
        INTRA_DC_PRECISION_11,
        NOT_AN_INTRA_DC_PRECISION
    };

    void        setVideoWidth(int width);
    void        setVideoHeight(int height);
    void        setSrcFileName(char* name) { strcpy(m_tplorg, name); }
    void        setFrameRate(FrameRate rate) { m_frame_rate_code = rate; }
    void        setBitRate(float rate) { m_bit_rate = rate; }
    void        setProfileID(ProfileID ID) { m_profile = ID; }
    void        setInputFileFormat(MpegInputFiletype type) { m_inputtype = type; }
    void        setFirstFrame(int first) { m_frame0 = first; }
    void        setNumberOfFrames(int count) { m_nframes = count; }
    void        setLevelID(int ID) { m_level = ID; }
    void        setChromaFormat(int format) { m_chroma_format = format; }
    void        setAspectRatio(AspectRatio ratio) { m_aspectratio = ratio; }
    void        setVideoFormat(VideoFormat format) { m_video_format = format; }
    void        setOutputFilename(std::string name) { m_output_filename = name; }
    void        setN (int value) { m_N = value; }
    void        setM (int value) { m_M = value; }
    void        setIntraDCPrecision( IntraDCPrecision precision ) { m_dc_prec = precision; }
    void        setProgressiveSequence(bool flag) { m_prog_seq = int(flag); }
    void        setUsePixelAveraging(bool flag) { m_use_pixel_averaging = flag; }
    void        setInputFilenameString(const char* name);
    void        setInputFilenameExtension(char* extension) { m_input_filename_extension = extension; }
    
    void        setFeedback(char** feedback) { m_feedback = feedback; }
    void        setStopSignal(bool* signal) { m_stop_signal = signal; }
    void        setFrameNumberPadding(std::string padding) { m_frame_number_padding = padding; }
 

                MpegEncoder();
                ~MpegEncoder();
    void        putseq(void);
    void        init();



 protected:
    
    void        setFeedbackString();

    void        range_checks(void);
    void        profile_and_level_checks();
    void        init_fdct(void);
    void        fdct(short *block);
    void        idct(short *block);
    void        init_idct(void);
    void        motion_estimation(unsigned char *oldorg, unsigned char *neworg,
                    unsigned char *oldref, unsigned char *newref, unsigned char *cur,
                    unsigned char *curref, int sxf, int syf, int sxb, int syb,
                    struct mbinfo *mbi, int secondfield, int ipflag);
    void        error(char *text);

    void        predict(unsigned char *reff[], unsigned char *refb[],
                    unsigned char *cur[3], int secondfield, struct mbinfo *mbi);
    void            initbits(void);
    void        putbits(int val, int n);
    void        alignbits(void);
    int         bitcount(void);
    void        putseqhdr(void);
    void        putseqext(void);
    void        putseqdispext(void);
    void        putuserdata(char *userdata);
    void        putgophdr(int frame, int closed_gop);
    void        putpicthdr(void);
    void        putpictcodext(void);
    void        putseqend(void);
    void        putintrablk(short *blk, int cc);
    void        putnonintrablk(short *blk);
    void        putmv(int dmv, int f_code);
    void        putpict(unsigned char *frame);
    void        putDClum(int val);
    void        putDCchrom(int val);
    void        putACfirst(int run, int val);
    void        putAC(int run, int signed_level, int vlcformat);
    void        putaddrinc(int addrinc);
    void        putmbtype(int pict_type, int mb_type);
    void        putmotioncode(int motion_code);
    void        putdmv(int dmv);
    void        putcbp(int cbp);
    int         quant_intra(short *src, short *dst, int dc_prec,
                    unsigned char *quant_mat, int mquant);
    int         quant_non_intra(short *src, short *dst,
                    unsigned char *quant_mat, int mquant);
    void        iquant_intra(short *src, short *dst, int dc_prec,
                    unsigned char *quant_mat, int mquant);
    void        iquant_non_intra(short *src, short *dst,
                    unsigned char *quant_mat, int mquant);
    void        rc_init_seq(void);
    void        rc_init_GOP(int np, int nb);
    void        rc_init_pict(unsigned char *frame);
    void        rc_update_pict(void);
    int         rc_start_mb(void);
    int         rc_calc_mquant(int j);
    void        vbv_end_of_picture(void);
    void        calc_vbv_delay(void);
    void        readframe(int frame_number, char *fname, unsigned char *frame[]);
    void        calcSNR(unsigned char *org[3], unsigned char *rec[3]);
    void        stats(void);
    void        transform(unsigned char *pred[], unsigned char *cur[],
                    struct mbinfo *mbi, short blocks[][64]);
    void        itransform(unsigned char *pred[], unsigned char *cur[],
                    struct mbinfo *mbi, short blocks[][64]);
    void        dct_type_estimation(unsigned char *pred, unsigned char *cur,
                    struct mbinfo *mbi);
    void        writeframe(char *fname, unsigned char *frame[]);

    void frame_ME(
                unsigned char* oldorg,
                unsigned char* neworg,
                unsigned char* oldref,
                unsigned char* newref,
                unsigned char* cur,
                int i,
                int j,
                int sxf,
                int syf,
                int sxb,
                int syb,
                struct mbinfo* mbi);

    void field_ME(
            unsigned char* oldorg,
            unsigned char* neworg,
            unsigned char* oldref,
            unsigned char* newref,
            unsigned char* cur,
            unsigned char* curref,
            int i,
            int j,
            int sxf,
            int syf,
            int sxb,
            int syb,
            struct mbinfo* mbi,
            int secondfield,
            int ipflag);

    void frame_estimate(
            unsigned char* org,
            unsigned char* ref,
            unsigned char* mb,
            int i,
            int j,
            int sx,
            int sy,
            int* iminp,
            int* jminp,
            int* imintp,
            int* jmintp,
            int* iminbp,
            int* jminbp,
            int* dframep,
            int* dfieldp,
            int* tselp,
            int* bselp,
            int imins[2][2],
            int jmins[2][2]);

    void field_estimate(
            unsigned char* toporg,
            unsigned char* topref,
            unsigned char* botorg,
            unsigned char* botref,
            unsigned char* mb,
            int i,
            int j,
            int sx,
            int sy,
            int ipflag,
            int* iminp,
            int* jminp,
            int* imin8up,
            int* jmin8up,
            int* imin8lp,
            int* jmin8lp,
            int* dfieldp,
            int* d8p,
            int* selp,
            int* sel8up,
            int* sel8lp,
            int* iminsp,
            int* jminsp,
            int* dsp);

    void dpframe_estimate(
        unsigned char* ref,
        unsigned char* mb,
        int i,
        int j,
        int iminf[2][2],
        int jminf[2][2],
        int* iminp,
        int* jminp,
        int* imindmvp, 
        int* jmindmvp, 
        int* dmcp, 
        int* vmcp);

void dpfield_estimate(
        unsigned char* topref,
        unsigned char* botref,
        unsigned char* mb,
        int i,
        int j,
        int imins,
        int jmins,
        int* imindmvp, 
        int* jmindmvp, 
        int* dmcp, 
        int* vmcp);

    int fullsearch(
            unsigned char* org,
            unsigned char* ref,
            unsigned char* blk,
            int lx,
            int i0,
            int j0,
            int sx,
            int sy,
            int h,
            int xmax,
            int ymax,
            int* iminp,
            int* jminp);

    int dist1(
        unsigned char* blk1,
        unsigned char* blk2,
        int lx,
        int hx,
        int hy,
        int h,
        int distlim);

    int dist2(
        unsigned char* blk1,
        unsigned char* blk2,
        int lx,
        int hx,
        int hy,
        int h);

    int bdist1(
        unsigned char* pf,
        unsigned char* pb,
        unsigned char* p2,
        int lx,
        int hxf,
        int hyf,
        int hxb,
        int hyb,
        int h);

    int bdist2(
        unsigned char* pf,
        unsigned char* pb,
        unsigned char* p2,
        int lx,
        int hxf,
        int hyf,
        int hxb,
        int hyb,
        int h);

    int variance(unsigned char* p, int lx);

    void readparmfile();
    void readquantmat();

    void predict_mb(
        		unsigned char* 		oldref[],
        		unsigned char* 		newref[],
        		unsigned char* 		cur[],
        		int 				lx,
        		int 				bx,
        		int 				by,
        		int 				pict_type,
        		int 				pict_struct,
        		int 				mb_type,
        		int 				motion_type,
        		int 				secondfield,
        		int 				PMV[2][2][2],
        		int 				mv_field_sel[2][2],
        		int 				dmvector[2]);

    void pred(
        		unsigned char* 		src[],
        		int 				sfield,
        		unsigned char* 		dst[],
        		int 				dfield,
        		int 				lx,
        		int 				w,
        		int 				h,
        		int 				x,
        		int 				y,
        		int 				dx,
        		int 				dy,
        		int 				addflag);

    void pred_comp(
        		unsigned char* 		src,
        		unsigned char* 		dst,
        		int 				lx,
        		int 				w,
        		int 				h,
        		int 				x,
        		int 				y,
        		int 				dx,
        		int 				dy,
        		int 				addflag);

    void calc_DMV(int DMV[][2], int* dmvector, int mvx, int mvy);
    void clearblock(unsigned char* cur[], int i0, int j0);

    void putmvs(
                int 				MV[2][2][2],
                int 				PMV[2][2][2],
                int 				mv_field_sel[2][2],
                int 				dmvector[2],
                int	 				s,
                int 				motion_type,
                int 				hor_f_code,
                int 				vert_f_code);

    void iquant1_intra(
        		short* 				src, 
        		short* 				dst, 
        		int 				dc_prec, 
        		unsigned char* 		quant_mat,
        		int 				mquant);

    void iquant1_non_intra(short* src, short* dst, unsigned char* quant_mat, int mquant);
    void read_y_u_v(char* fname, unsigned char* frame[]);
    void read_yuv(char* fname, unsigned char* frame[]);
    void read_ppm(char* fname, unsigned char* frame[]);
    void read_bmp(char* fname, unsigned char* frame[]);
    void read_qimage(char* fname, unsigned char* frame[]);
    void border_extend(unsigned char* frame, int w1, int h1, int w2, int h2);
    void conv444to422(unsigned char* src, unsigned char* dst);
    void conv422to420(unsigned char* src, unsigned char* dst);

    void calcSNR1(
        		unsigned char* 		org, 
        		unsigned char* 		rec, 
        		int 				lx, 
        		int 				w, 
        		int 				h,
        		double* 			pv,
        		double* 			pe);

    void        add_pred(unsigned char* pred, unsigned char* cur, int lx, short* blk);
    void        sub_pred(unsigned char* pred, unsigned char* cur, int lx, short* blk);
    int         frametotc(int frame);
    void        putDC(const sVLCtable* tab, int val);
    void        calc_actj(unsigned char* frame);
    double      var_sblk(unsigned char* p, int lx);
    char        pbm_getc(FILE* file);
    int         pbm_getint(FILE* file);

    struct level_limits 
    {
        int hor_f_code;
        int vert_f_code;
        int hor_size;
        int vert_size;
        int sample_rate;
        int bit_rate; /* Mbit/s */
        int vbv_buffer_size; /* 16384 bit steps */
    };

    bool                                m_use_pixel_averaging;

    static const char                   m_version[];
    static const char                   m_author[];
    static const unsigned char          m_zig_zag_scan[64];
    static const unsigned char          m_alternate_scan[64];
    static const unsigned char          m_default_intra_quantizer_matrix[64];
    static const unsigned char          m_non_linear_mquant_table[32];
    static const unsigned char          m_map_non_linear_mquant[113]; 

    std::string             m_output_filename;
    std::string             m_stats_filename;

    unsigned char*          m_newrefframe[3];
    unsigned char*          m_oldrefframe[3]; 
    unsigned char*          m_auxframe[3];
    unsigned char*          m_neworgframe[3]; 
    unsigned char*          m_oldorgframe[3];
    unsigned char*          m_auxorgframe[3];
    unsigned char*          m_predframe[3];

    unsigned char*          m_newrefframe_base[3];
    unsigned char*          m_oldrefframe_base[3]; 
    unsigned char*          m_auxframe_base[3];
    unsigned char*          m_neworgframe_base[3]; 
    unsigned char*          m_oldorgframe_base[3];
    unsigned char*          m_auxorgframe_base[3];
    unsigned char*          m_predframe_base[3];

    short                   (*m_blocks)[64];
    short                   (*m_blocks_base)[64];

    unsigned char           m_intra_q[64]; 
    unsigned char           m_inter_q[64];
    unsigned char           m_chrom_intra_q[64];
    unsigned char           m_chrom_inter_q[64];
    int                     m_dc_dct_pred[3];
    struct mbinfo*          m_mbinfo;
    struct mbinfo*          m_mbinfo_base;
    struct motion_data*     m_motion_data;
    struct motion_data*     m_motion_data_base;
    unsigned char*          m_clp;
    unsigned char*          m_clp_base;
    char                    m_id_string[256];
    char                    m_tplorg[256];
    char                    m_tplref[256];
    char                    m_iqname[256];
    char                    m_niqname[256];
    char                    m_statname[256];
    char                    m_errortext[256];
    FILE*                   m_outfile;
    FILE*                   m_statfile;
    int                     m_inputtype; /* format of input frames */
    int                     m_quiet; /* suppress warnings */
    int                     m_N; /* number of frames in Group of Pictures */
    int                     m_M; /* distance between I/P frames */
    int                     m_P; /* intra slice refresh interval */
    int                     m_nframes; /* total number of frames to encode */
    int                     m_frame0; 
    int                     m_tc0; /* number and timecode of first frame */
    int                     m_mpeg1; /* ISO/IEC IS 11172-2 sequence */
    int                     m_fieldpic; /* use field pictures */
    int                     m_horizontal_size;
    int                     m_vertical_size; /* frame size (pels) */
    int                     m_width;
    int                     m_height; /* encoded frame size (pels) multiples of 16 or 32 */
    int                     m_chrom_width;
    int                     m_chrom_height;
    int                     m_block_count;
    int                     m_mb_width; 
    int                     m_mb_height; /* frame size (macroblocks) */
    int                     m_width2; 
    int                     m_height2; 
    int                     m_mb_height2;
    int                     m_chrom_width2; /* picture size */
    int                     m_aspectratio; /* aspect ratio information (pel or display) */
    int                     m_frame_rate_code; /* coded value of frame rate */
    double                  m_frame_rate; /* frames per second */
    double                  m_bit_rate; /* bits per second */
    int                     m_vbv_buffer_size; /* size of VBV buffer (* 16 kbit) */
    int                     m_constrparms; /* constrained parameters flag (MPEG-1 only) */
    int                     m_load_iquant;
    int                     m_load_niquant; /* use non-default quant. matrices */
    int                     m_load_ciquant;
    int                     m_load_cniquant;
    int                     m_profile;
    int                     m_level; /* syntax / parameter constraints */
    int                     m_prog_seq; /* progressive sequence */
    int                     m_chroma_format;
    int                     m_low_delay; /* no B pictures, skipped pictures */
    int                     m_video_format; /* component, PAL, NTSC, SECAM or MAC */
    int                     m_color_primaries; /* source primary chromaticity coordinates */
    int                     m_transfer_characteristics; /* opto-electronic transfer char. (gamma) */
    int                     m_matrix_coefficients; /* Eg,Eb,Er / Y,Cb,Cr matrix coefficients */
    int                     m_display_horizontal_size;
    int                     m_display_vertical_size; /* display size */
    int                     m_temp_ref; /* temporal reference */
    int                     m_pict_type; /* picture coding type (I, P or B) */
    int                     m_vbv_delay; /* video buffering verifier delay (1/90000 seconds) */
    int                     m_forw_hor_f_code;
    int                     m_forw_vert_f_code;
    int                     m_back_hor_f_code;
    int                     m_back_vert_f_code; /* motion vector ranges */
    int                     m_dc_prec; /* DC coefficient precision for intra coded blocks */
    int                     m_pict_struct; /* picture structure (frame, top / bottom field) */
    int                     m_topfirst; /* display top field first */
    int                     m_frame_pred_dct_tab[3]; 
    int                     m_frame_pred_dct;
    int                     m_conceal_tab[3]; /* use concealment motion vectors (I,P,B) */
    int                     m_qscale_tab[3]; 
    int                     m_q_scale_type; /* linear/non-linear quantizaton table */
    int                     m_intravlc_tab[3];
    int                     m_intravlc; /* intra vlc format (I,P,B,current) */
    int                     m_altscan_tab[3]; 
    int                     m_altscan; /* alternate scan (I,P,B,current) */
    int                     m_repeatfirst; /* repeat first field after second field */
    int                     m_prog_frame; /* progressive frame */

    int                     m_bitcnt_EOP;

    static const char       m_profile_level_defined[5][4];

    static level_limits     m_maxval_tab[4];
    double                  m_transform_coefficients[8][8];
    short                   m_clipping_table[1024];
    short*                  m_clipping_table_ptr;
    unsigned char           m_out_buffer;
    int                     m_out_count;
    int                     m_byte_count;

    int                     m_Xi;
    int                     m_Xp;
    int                     m_Xb;
    int                     m_r; 
    int                     m_d0i; 
    int                     m_d0p; 
    int                     m_d0b;
    double                  m_avg_act;

    int                     m_ratectl_R;
    int                     m_ratectl_T;
    int                     m_ratectl_d;
    double                  m_ratectl_actsum;
    int                     m_ratectl_Np;
    int                     m_ratectl_Nb;
    int                     m_ratectl_S;
    int                     m_ratectl_Q;
    int                     m_ratectl_prev_mquant;

    void                    idctrow(short* blk);
    void                    idctcol(short* blk);

    unsigned char*          m_u444;
    unsigned char*          m_v444;
    unsigned char*          m_u422;
    unsigned char*          m_v422;

    unsigned char*          m_u444_base;
    unsigned char*          m_v444_base;
    unsigned char*          m_u422_base;
    unsigned char*          m_v422_base;

    std::string             m_input_filename_extension;
    char**                  m_feedback;
    char                    m_feedback_string[250];
    bool*                   m_stop_signal;

    std::string             m_frame_number_padding;
};



#endif // MPEG2ENC_H
