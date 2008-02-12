/* mpeg2enc.c, main() and parameter file reading                            */

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
#include <sstream>

void 
MpegEncoder::init()
{
	int i, size;
	static int block_count_tab[3] = {6,8,12};

  	readparmfile();
	readquantmat();


	initbits();
	init_fdct();
	init_idct();

	/* round picture dimensions to nearest multiple of 16 or 32 */
	m_mb_width = (m_horizontal_size + 15) / 16;
	m_mb_height = m_prog_seq ? (m_vertical_size + 15) / 16 : 2 * ((m_vertical_size + 31) / 32);
	m_mb_height2 = m_fieldpic ? m_mb_height>>1 : m_mb_height; /* for field pictures */
	m_width = 16 * m_mb_width;
	m_height = 16 * m_mb_height;

    m_u444_base = new unsigned char[m_width * m_height];
    m_u444 = m_u444_base;

    m_v444_base = new unsigned char[m_width * m_height];
    m_v444 = m_v444_base;

    m_u422_base = new unsigned char[(m_width >> 1 ) * m_height];
    m_u422 = m_u422_base;

    m_v422_base = new unsigned char[(m_width >> 1 ) * m_height];
    m_v422 = m_v422_base;

	m_chrom_width = (m_chroma_format == CHROMA444) ? m_width : m_width >> 1;
	m_chrom_height = (m_chroma_format != CHROMA420) ? m_height : m_height >> 1;

	m_height2 = m_fieldpic ? m_height >> 1 : m_height;
	m_width2 = m_fieldpic ? m_width << 1 : m_width;
	m_chrom_width2 = m_fieldpic ? m_chrom_width << 1 : m_chrom_width;
  
	m_block_count = block_count_tab[m_chroma_format-1];

	/* clip table */
	m_clp_base = new unsigned char[1024];
    m_clp = m_clp_base;
	m_clp += 384;

	for (i = -384; i < 640; i++)
    {
		m_clp[i] = (i < 0) ? 0 : ((i > 255) ? 255 : i);
    }

	for (i = 0; i < 3; i++)
	{
		size = (i == 0) ? m_width * m_height : m_chrom_width * m_chrom_height;

		m_newrefframe_base[i] = new unsigned char[size];
		m_newrefframe[i] = m_newrefframe_base[i];

        m_oldrefframe_base[i] = new unsigned char[size];
        m_oldrefframe[i] = m_oldrefframe_base[i];

		m_auxframe_base[i] = new unsigned char[size];
        m_auxframe[i] = m_auxframe_base[i];

		m_neworgframe_base[i] = new unsigned char[size];
        m_neworgframe[i] = m_neworgframe_base[i];

		m_oldorgframe_base[i] = new unsigned char[size];
        m_oldorgframe[i] = m_oldorgframe_base[i];

		m_auxorgframe_base[i] = new unsigned char[size];
        m_auxorgframe[i] = m_auxorgframe_base[i];

		m_predframe_base[i] = new unsigned char[size];
        m_predframe[i] = m_predframe_base[i];

	}

	m_mbinfo_base = new struct mbinfo[m_mb_width * m_mb_height2];
    m_mbinfo = m_mbinfo_base;

	m_blocks_base = (short (*)[64])(new short[m_mb_width * m_mb_height2 * m_block_count * 64]);
    m_blocks = m_blocks_base;

	/* open output file */
	if (!(m_outfile = fopen(m_output_filename.c_str(), "wb")))
	{
		sprintf(m_errortext, "Couldn't create output file %s", m_output_filename.c_str());
		error(m_errortext);
	}

	if (!(m_statfile = fopen( m_stats_filename.c_str(), "w" )) )
    {
		sprintf(m_errortext, "Couldn't create statistics file %s", m_stats_filename.c_str());
		error(m_errortext);
    }

}
 
void 
MpegEncoder::error(char* text)
{
	fprintf(stderr,text);
	putc('\n',stderr);
	//exit(1);
}


void 
MpegEncoder::readparmfile()
{
	int i;
	int h = 0;
    int m = 0;
    int s = 0;
    int f = 0;
	static double ratetab[8]=
		{24000.0/1001.0,24.0,25.0,30000.0/1001.0,30.0,50.0,60000.0/1001.0,60.0};

#if 0
	FILE *fd;
	char line[256];

    if (!(fd = fopen(fname,"r")))
	{
		sprintf(errortext,"Couldn't open parameter file %s",fname);
		error(errortext);
	}

	fgets(id_string,254,fd);
	fgets(line,254,fd); sscanf(line,"%s",tplorg);
	fgets(line,254,fd); sscanf(line,"%s",tplref);
	fgets(line,254,fd); sscanf(line,"%s",iqname);
	fgets(line,254,fd); sscanf(line,"%s",niqname);
	fgets(line,254,fd); sscanf(line,"%s",statname);
	fgets(line,254,fd); sscanf(line,"%d",&inputtype);
	fgets(line,254,fd); sscanf(line,"%d",&nframes);
	fgets(line,254,fd); sscanf(line,"%d",&frame0);
	fgets(line,254,fd); sscanf(line,"%d:%d:%d:%d",&h,&m,&s,&f);
	fgets(line,254,fd); sscanf(line,"%d",&N);
	fgets(line,254,fd); sscanf(line,"%d",&M);
	fgets(line,254,fd); sscanf(line,"%d",&mpeg1);
	fgets(line,254,fd); sscanf(line,"%d",&fieldpic);
	fgets(line,254,fd); sscanf(line,"%d",&horizontal_size);
	fgets(line,254,fd); sscanf(line,"%d",&vertical_size);
	fgets(line,254,fd); sscanf(line,"%d",&aspectratio);
	fgets(line,254,fd); sscanf(line,"%d",&frame_rate_code);
	fgets(line,254,fd); sscanf(line,"%lf",&bit_rate);
	fgets(line,254,fd); sscanf(line,"%d",&vbv_buffer_size);   
	fgets(line,254,fd); sscanf(line,"%d",&low_delay);     
	fgets(line,254,fd); sscanf(line,"%d",&constrparms);
	fgets(line,254,fd); sscanf(line,"%d",&profile);
	fgets(line,254,fd); sscanf(line,"%d",&level);
	fgets(line,254,fd); sscanf(line,"%d",&prog_seq);
	fgets(line,254,fd); sscanf(line,"%d",&chroma_format);
	fgets(line,254,fd); sscanf(line,"%d",&video_format);
	fgets(line,254,fd); sscanf(line,"%d",&color_primaries);
	fgets(line,254,fd); sscanf(line,"%d",&transfer_characteristics);
	fgets(line,254,fd); sscanf(line,"%d",&matrix_coefficients);
	fgets(line,254,fd); sscanf(line,"%d",&display_horizontal_size);
	fgets(line,254,fd); sscanf(line,"%d",&display_vertical_size);
	fgets(line,254,fd); sscanf(line,"%d",&dc_prec);
	fgets(line,254,fd); sscanf(line,"%d",&topfirst);
	fgets(line,254,fd); sscanf(line,"%d %d %d",
		frame_pred_dct_tab,frame_pred_dct_tab+1,frame_pred_dct_tab+2);
  
	fgets(line,254,fd); sscanf(line,"%d %d %d",
		conceal_tab,conceal_tab+1,conceal_tab+2);
  
	fgets(line,254,fd); sscanf(line,"%d %d %d",
		qscale_tab,qscale_tab+1,qscale_tab+2);

	fgets(line,254,fd); sscanf(line,"%d %d %d",
		intravlc_tab,intravlc_tab+1,intravlc_tab+2);
	fgets(line,254,fd); sscanf(line,"%d %d %d",
		altscan_tab,altscan_tab+1,altscan_tab+2);
	fgets(line,254,fd); sscanf(line,"%d",&repeatfirst);
	fgets(line,254,fd); sscanf(line,"%d",&prog_frame);
/* intra slice interval refresh period */  
	fgets(line,254,fd); sscanf(line,"%d",&P);
	fgets(line,254,fd); sscanf(line,"%d",&r);
	fgets(line,254,fd); sscanf(line,"%lf",&avg_act);
	fgets(line,254,fd); sscanf(line,"%d",&Xi);
	fgets(line,254,fd); sscanf(line,"%d",&Xp);
	fgets(line,254,fd); sscanf(line,"%d",&Xb);
	fgets(line,254,fd); sscanf(line,"%d",&d0i);
	fgets(line,254,fd); sscanf(line,"%d",&d0p);
	fgets(line,254,fd); sscanf(line,"%d",&d0b);

	if (N<1)
		error("N must be positive");
	if (M<1)
		error("M must be positive");
	if (N%M != 0)
		error("N must be an integer multiple of M");

	for (i=0; i<M; i++)
	{
		fgets(line,254,fd);
		sscanf(line,"%d %d %d %d",
			&motion_data[i].forw_hor_f_code, &motion_data[i].forw_vert_f_code,
			&motion_data[i].sxf, &motion_data[i].syf);

		if (i!=0)
		{
			fgets(line,254,fd);
			sscanf(line,"%d %d %d %d",
				&motion_data[i].back_hor_f_code, &motion_data[i].back_vert_f_code,
				&motion_data[i].sxb, &motion_data[i].syb);
		}
	}

	fclose(fd);
#endif

	/* make flags boolean (x!=0 -> x=1) */
	m_mpeg1 = !!m_mpeg1;
	m_fieldpic = !!m_fieldpic;
	m_low_delay = !!m_low_delay;
	m_constrparms = !!m_constrparms;
	m_prog_seq = !!m_prog_seq;
	m_topfirst = !!m_topfirst;

	for (i = 0; i < 3; i++)
	{
		m_frame_pred_dct_tab[i] = !!m_frame_pred_dct_tab[i];
		m_conceal_tab[i] = !!m_conceal_tab[i];
		m_qscale_tab[i] = !!m_qscale_tab[i];
		m_intravlc_tab[i] = !!m_intravlc_tab[i];
		m_altscan_tab[i] = !!m_altscan_tab[i];
	}

	m_repeatfirst = !!m_repeatfirst;
	m_prog_frame = !!m_prog_frame;

	/* make sure MPEG specific parameters are valid */
	range_checks();

	m_frame_rate = ratetab[m_frame_rate_code - 1];

	/* timecode -> frame number */
	m_tc0 = h;
	m_tc0 = (60 * m_tc0) + m;
	m_tc0 = (60 * m_tc0) + s;
	m_tc0 = ( (int)(m_frame_rate + 0.5) * m_tc0 ) + f;

	if (!m_mpeg1)
	{
		profile_and_level_checks();
	}
	else
	{
		/* MPEG-1 */
		if (m_constrparms)
		{
			if (m_horizontal_size > 768
				|| m_vertical_size > 576
				|| (((m_horizontal_size + 15) / 16) * ((m_vertical_size + 15) / 16)) > 396
				|| (((m_horizontal_size + 15) / 16) * 
					((m_vertical_size + 15) / 16) * m_frame_rate) > (396 * 25.0)
				|| m_frame_rate > 30.0)
			{
				if (!m_quiet)
                {
					fprintf(stderr,"Warning: setting constrained_parameters_flag = 0\n");
                }

				m_constrparms = 0;
			}
		}

		if (m_constrparms)
		{
			for (i = 0; i < m_M; i++)
			{
				if (m_motion_data[i].forw_hor_f_code > 4)
				{
					if (!m_quiet)
                    {
						fprintf(stderr,"Warning: setting constrained_parameters_flag = 0\n");
                    }

					m_constrparms = 0;
					break;
				}

				if (m_motion_data[i].forw_vert_f_code > 4)
				{
					if (!m_quiet)
                    {
						fprintf(stderr,"Warning: setting constrained_parameters_flag = 0\n");
                    }

					m_constrparms = 0;
					break;
				}

				if (i != 0)
				{
					if (m_motion_data[i].back_hor_f_code > 4)
					{
						if (!m_quiet)
                        {
							fprintf(stderr,"Warning: setting constrained_parameters_flag = 0\n");
                        }

						m_constrparms = 0;
						break;
					}

					if (m_motion_data[i].back_vert_f_code>4)
					{
						if (!m_quiet)
                        {
							fprintf(stderr,"Warning: setting constrained_parameters_flag = 0\n");
                        }

						m_constrparms = 0;
						break;
					}
				}
			}
		}
	}

	/* relational checks */

	if (m_mpeg1)
	{
		if (!m_prog_seq)
		{
			if (!m_quiet)
            {
				fprintf(stderr,"Warning: setting progressive_sequence = 1\n");
            }

			m_prog_seq = 1;
		}

		if (m_chroma_format != CHROMA420)
		{
			if (!m_quiet)
            {
				fprintf(stderr,"Warning: setting chroma_format = 1 (4:2:0)\n");
            }

			m_chroma_format = CHROMA420;
		}

		if (m_dc_prec != 0)
		{
			if (!m_quiet)
            {
				fprintf(stderr,"Warning: setting intra_dc_precision = 0\n");
            }

			m_dc_prec = 0;
		}

		for (i = 0; i < 3; i++)
        {
			if (m_qscale_tab[i])
			{
				if (!m_quiet)
                {
					fprintf(stderr,"Warning: setting qscale_tab[%d] = 0\n",i);
                }

				m_qscale_tab[i] = 0;
			}
        }

		for (i = 0; i < 3; i++)
        {
			if (m_intravlc_tab[i])
			{
				if (!m_quiet)
                {
					fprintf(stderr,"Warning: setting intravlc_tab[%d] = 0\n",i);
                }

				m_intravlc_tab[i] = 0;
			}
        }

		for (i = 0; i < 3; i++)
        {
			if (m_altscan_tab[i])
			{
				if (!m_quiet)
                {
					fprintf(stderr,"Warning: setting altscan_tab[%d] = 0\n",i);
                }

				m_altscan_tab[i] = 0;
			}
        }
	}

	if (!m_mpeg1 && m_constrparms)
	{
		if (!m_quiet)
        {
			fprintf(stderr,"Warning: setting constrained_parameters_flag = 0\n");
        }

		m_constrparms = 0;
	}

	if (m_prog_seq && !m_prog_frame)
	{
		if (!m_quiet)
        {
			fprintf(stderr,"Warning: setting progressive_frame = 1\n");
        }

		m_prog_frame = 1;
	}

	if (m_prog_frame && m_fieldpic)
	{
		if (!m_quiet)
        {
			fprintf(stderr,"Warning: setting field_pictures = 0\n");
        }

		m_fieldpic = 0;
	}

	if (!m_prog_frame && m_repeatfirst)
	{
		if (!m_quiet)
        {
			fprintf(stderr,"Warning: setting repeat_first_field = 0\n");
        }

		m_repeatfirst = 0;
	}

	if (m_prog_frame)
	{
		for (i = 0; i < 3; i++)
        {
			if (!m_frame_pred_dct_tab[i])
			{
				if (!m_quiet)
                {
					fprintf(stderr,"Warning: setting frame_pred_frame_dct[%d] = 1\n",i);
                }

				m_frame_pred_dct_tab[i] = 1;
			}
        }
	}

	if (m_prog_seq && !m_repeatfirst && m_topfirst)
	{
		if (!m_quiet)
        {
			fprintf(stderr,"Warning: setting top_field_first = 0\n");
        }

		m_topfirst = 0;
	}

	/* search windows */
	for (i = 0; i < m_M; i++)
	{
		if (m_motion_data[i].sxf > ( (4 << m_motion_data[i].forw_hor_f_code) - 1) )
		{
			if (!m_quiet)
            {
				fprintf(stderr,
					"Warning: reducing forward horizontal search width to %d\n",
					(4 << m_motion_data[i].forw_hor_f_code)-1);
            }

			m_motion_data[i].sxf = (4 << m_motion_data[i].forw_hor_f_code)-1;
		}

		if (m_motion_data[i].syf > (4 << m_motion_data[i].forw_vert_f_code)-1)
		{
			if (!m_quiet)
            {
				fprintf(stderr,
					"Warning: reducing forward vertical search width to %d\n",
					(4 << m_motion_data[i].forw_vert_f_code)-1);
            }

			m_motion_data[i].syf = (4 << m_motion_data[i].forw_vert_f_code) - 1;
		}

		if (i!=0)
		{
			if (m_motion_data[i].sxb > ( (4 << m_motion_data[i].back_hor_f_code) - 1 ) )
			{
				if (!m_quiet)
                {
					fprintf(stderr,
						"Warning: reducing backward horizontal search width to %d\n",
						(4 << m_motion_data[i].back_hor_f_code)-1);
                }

				m_motion_data[i].sxb = (4 << m_motion_data[i].back_hor_f_code)-1;
			}

			if (m_motion_data[i].syb > (4 << m_motion_data[i].back_vert_f_code)-1)
			{
				if (!m_quiet)
                {
					fprintf(stderr,
						"Warning: reducing backward vertical search width to %d\n",
						(4 << m_motion_data[i].back_vert_f_code)-1);
                }

				m_motion_data[i].syb = (4 << m_motion_data[i].back_vert_f_code)-1;
			}
		}
	}
}


void 
MpegEncoder::readquantmat()
{
	int i,v;
	FILE *fd;

	if (m_iqname[0]=='-')
	{
		/* use default intra matrix */
		m_load_iquant = 0;

		for (i = 0; i < 64; i++)
        {
			m_intra_q[i] = m_default_intra_quantizer_matrix[i];
        }
	}
	else
	{
		/* read customized intra matrix */
		m_load_iquant = 1;

		if (!(fd = fopen(m_iqname, "r")))
		{
			sprintf(m_errortext, "Couldn't open quant matrix file %s", m_iqname);
			error(m_errortext);
		}

		for (i = 0; i < 64; i++)
		{
			fscanf(fd,"%d",&v);

			if (v < 1 || v > 255)
            {
				error("invalid value in quant matrix");
            }

			m_intra_q[i] = v;
		}

		fclose(fd);
	}

	if (m_niqname[0] == '-')
	{
		/* use default non-intra matrix */
		m_load_niquant = 0;

		for (i=0; i<64; i++)
        {
			m_inter_q[i] = 16;
        }
	}
	else
	{
		/* read customized non-intra matrix */
		m_load_niquant = 1;

		if (!(fd = fopen(m_niqname, "r")))
		{
			sprintf(m_errortext, "Couldn't open quant matrix file %s", m_niqname);
			error(m_errortext);
		}

		for (i = 0; i < 64; i++)
		{
			fscanf(fd,"%d",&v);

			if (v < 1 || v > 255)
            {
				error("invalid value in quant matrix");
            }

			m_inter_q[i] = v;
		}

		fclose(fd);
	}
}



const char
MpegEncoder::m_version[] = "mpeg2encode V1.2, 96/07/19";

const char
MpegEncoder::m_author[] = "(C) 1996, MPEG Software Simulation Group";

const unsigned char
MpegEncoder::m_zig_zag_scan[64] =
{
	0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,
	12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,
	35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,
	58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63
};

const unsigned char
MpegEncoder::m_alternate_scan[64] =
{
	0,8,16,24,1,9,2,10,17,25,32,40,48,56,57,49,
	41,33,26,18,3,11,4,12,19,27,34,42,50,58,35,43,
	51,59,20,28,5,13,6,14,21,29,36,44,52,60,37,45,
	53,61,22,30,7,15,23,31,38,46,54,62,39,47,55,63
};

const unsigned char
MpegEncoder::m_default_intra_quantizer_matrix[64] =
{
	8, 16, 19, 22, 26, 27, 29, 34,
	16, 16, 22, 24, 27, 29, 34, 37,
	19, 22, 26, 27, 29, 34, 34, 38,
	22, 22, 26, 27, 29, 34, 37, 40,
	22, 26, 27, 29, 32, 35, 40, 48,
	26, 27, 29, 32, 35, 40, 48, 58,
	26, 27, 29, 34, 38, 46, 56, 69,
	27, 29, 35, 38, 46, 56, 69, 83
};

const unsigned char
MpegEncoder::m_non_linear_mquant_table[32] =
{
	0, 1, 2, 3, 4, 5, 6, 7,
	8,10,12,14,16,18,20,22,
	24,28,32,36,40,44,48,52,
	56,64,72,80,88,96,104,112
};

const unsigned char
MpegEncoder::m_map_non_linear_mquant[113] =
{
	0,1,2,3,4,5,6,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,16,16,
	16,17,17,17,18,18,18,18,19,19,19,19,20,20,20,20,21,21,21,21,22,22,
	22,22,23,23,23,23,24,24,24,24,24,24,24,25,25,25,25,25,25,25,26,26,
	26,26,26,26,26,26,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,29,
	29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,31,31,31,31,31
};

MpegEncoder::MpegEncoder()
{
	std::string input_file_sequence_name = "..\\..\\mpeg2\\src\\EffectsClip21\\EffectsClip21_%%s%%d";
	sprintf(m_tplorg, input_file_sequence_name.c_str() );
	sprintf(m_tplref, "-");
	sprintf(m_iqname, "-");
	sprintf(m_niqname, "-");
	sprintf(m_statname, "stat.out");
	m_inputtype = T_BMP;
	m_nframes = 100;
	m_frame0 = 1;
	m_tc0 = 0;
	m_N = 15;
	m_M = 3;
	m_mpeg1 = 0;
	m_fieldpic = 0;
	m_horizontal_size = 720;
	m_vertical_size = 512;
	m_aspectratio = ASPECT_SQUARE;
	m_frame_rate_code = FRAME_RATE_30;
	m_bit_rate = 10000000.0f;
	m_vbv_buffer_size = 112;
	m_low_delay = 0;
	m_constrparms = 0;
	m_profile = PROFILE_HIGH;
	m_level = LEVEL_ID_HIGH_1400;
	m_prog_seq = true;
	m_chroma_format = CHROMA420;
	m_video_format = FORMAT_NTSC;
	m_color_primaries = 5;
	m_transfer_characteristics = 5;
	m_matrix_coefficients = 4;
	m_display_horizontal_size = 720;
	m_display_vertical_size = 486;
	m_dc_prec = INTRA_DC_PRECISION_8;
	m_topfirst = 1;
	m_frame_pred_dct_tab[0] = 0;
	m_frame_pred_dct_tab[1] = 0;
	m_frame_pred_dct_tab[2] = 0;
	m_conceal_tab[0] = 0;
	m_conceal_tab[1] = 0;
	m_conceal_tab[2] = 0;
	m_qscale_tab[0] = 1;
	m_qscale_tab[1] = 1;
	m_qscale_tab[2] = 1;
	m_intravlc_tab[0] = 1;
	m_intravlc_tab[1] = 0;
	m_intravlc_tab[2] = 0;
	m_altscan_tab[0] = 0;
	m_altscan_tab[1] = 0;
	m_altscan_tab[2] = 0;
	m_repeatfirst = 0;
	m_prog_frame = 0;
	m_P = 0;
	m_r = 0;
	m_avg_act = 0;
	m_Xi = 0;
	m_Xp = 0;
	m_Xb = 0;
	m_d0i = 0;
	m_d0p = 0;
	m_d0b = 0;

	m_motion_data = new struct motion_data[m_M];

	m_motion_data[0].forw_hor_f_code = 2;
	m_motion_data[0].forw_vert_f_code = 2;
	m_motion_data[0].sxf = 11;
	m_motion_data[0].syf = 11;

	m_motion_data[1].forw_hor_f_code = 1;
	m_motion_data[1].forw_vert_f_code = 1;
	m_motion_data[1].sxf = 3;
	m_motion_data[1].syf = 3;

	m_motion_data[2].forw_hor_f_code = 1;
	m_motion_data[2].forw_vert_f_code = 1;
	m_motion_data[2].sxf = 7;
	m_motion_data[2].syf = 7;

	m_motion_data[1].back_hor_f_code = 1;
	m_motion_data[1].back_vert_f_code = 1;
	m_motion_data[1].sxb = 7;
	m_motion_data[1].syb = 7;

	m_motion_data[2].back_hor_f_code = 1;
	m_motion_data[2].back_vert_f_code = 1;
	m_motion_data[2].sxb = 3;
	m_motion_data[2].syb = 3;

    m_output_filename = "clip.mpg";
    m_stats_filename = "stats.txt";

    m_use_pixel_averaging = false;
    m_frame_number_padding = "0001";
}

MpegEncoder::~MpegEncoder()
{
	fclose(m_outfile);
	fclose(m_statfile);
    delete [] m_clp_base;

    for (int i = 0; i < 3; i++)
    {
        delete [] m_newrefframe_base[i];
        delete [] m_oldrefframe_base[i];
        delete [] m_auxframe_base[i];
        delete [] m_neworgframe_base[i];
        delete [] m_oldorgframe_base[i];
        delete [] m_auxorgframe_base[i];
        delete [] m_predframe_base[i];
    }

    delete [] m_mbinfo_base;
    delete [] m_blocks_base;
    delete [] m_motion_data;

    delete [] m_u444_base;
    delete [] m_v444_base;
    delete [] m_u422_base;
    delete [] m_v422_base;
}


void
MpegEncoder::run()
{
    //wglShareLists( wglGetCurrentContext(), m_ogl_context);
    //wglMakeCurrent(m_display_context, m_ogl_context);
    init();
    putseq();
    delete this;
}

void
MpegEncoder::setVideoHeight(int height)
{
    m_vertical_size = m_prog_seq ? ( (height + 15) / 16 ) * 16 
                                 : ( (height + 31) / 32 ) * 32;
}

void
MpegEncoder::setVideoWidth(int width)
{
    m_horizontal_size = ( (width + 15) / 16 ) * 16;
}

void
MpegEncoder::setInputFilenameString(const char* name)
{
    sprintf(m_tplorg, "%s%%s%%d", name);
}

void
MpegEncoder::setFeedbackString()
{
    *m_feedback = m_feedback_string;
}
