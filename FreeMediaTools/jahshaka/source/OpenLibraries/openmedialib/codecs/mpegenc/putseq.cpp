/* putseq.c, sequence level routines                                        */

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
MpegEncoder::putseq()
{
	/* this routine assumes (N % M) == 0 */
	int i, j, k, f, f0, n, np, nb;
	int sxf = 0;
	int syf = 0;
	int sxb = 0;
	int syb = 0;
	int ipflag;
	char name[256];
	unsigned char *neworg[3], *newref[3];
	static char ipb[5] = {' ','I','P','B','D'};

	rc_init_seq(); /* initialize rate control */

	/* sequence header, sequence extension and sequence display extension */
	putseqhdr();

	if (!m_mpeg1)
	{
		putseqext();
		putseqdispext();
	}

	/* optionally output some text data (description, copyright or whatever) */
	if (strlen(m_id_string) > 1)
    {
		putuserdata(m_id_string);
    }

	/* loop through all frames in encoding/decoding order */
	for (i = 0; i < m_nframes; i++)
	{
        if (*m_stop_signal)
        {
            fclose(m_outfile);
         	fclose(m_statfile);

#ifdef WIN32
            std::string delete_command = "del ";
            delete_command += "\"";
            delete_command += m_output_filename;
            delete_command += "\"";

            int next_slash = 0;

            while (next_slash != std::string::npos )
            {
                next_slash = (int)delete_command.find("/", next_slash + 1);

                if (next_slash != std::string::npos)
                {
                    delete_command.replace(next_slash, 1, "\\");
                }
            }
#else
            std::string delete_command = "rm -f ";
            delete_command += "\"";
            delete_command += m_output_filename;
            delete_command += "\"";
#endif

            system( delete_command.c_str() ); 
            break;
        }

		if (!m_quiet)
		{
			fprintf(stderr,"Encoding frame %d ",i);
			fflush(stderr);
		}

		/* f0: lowest frame number in current GOP
		 *
		 * first GOP contains N-(M-1) frames,
		 * all other GOPs contain N frames
		 */
		f0 = m_N *( (i + (m_M - 1) ) / m_N) - (m_M -1);

		if (f0 < 0)
        {
			f0 = 0;
        }

		if ( (i == 0) || ( (i - 1) % m_M == 0 ) )
		{
			/* I or P frame */
			for (j = 0; j < 3; j++)
			{
				/* shuffle reference frames */
				neworg[j] = m_oldorgframe[j];
				newref[j] = m_oldrefframe[j];
				m_oldorgframe[j] = m_neworgframe[j];
				m_oldrefframe[j] = m_newrefframe[j];
				m_neworgframe[j] = neworg[j];
				m_newrefframe[j] = newref[j];
			}

			/* f: frame number in display order */
			f = (i == 0) ? 0 : i + m_M - 1;

			if (f >= m_nframes)
            {
				f = m_nframes - 1;
            }

			if (i == f0) /* first displayed frame in GOP is I */
			{
				/* I frame */
				m_pict_type = I_TYPE;
				m_forw_hor_f_code = m_forw_vert_f_code = 15;
				m_back_hor_f_code = m_back_vert_f_code = 15;

				/* n: number of frames in current GOP
				 *
				 * first GOP contains (M-1) less (B) frames
				 */
				n = (i==0) ? m_N -( m_M - 1) : m_N;

				/* last GOP may contain less frames */
				if (n > m_nframes - f0 )
					n = m_nframes - f0;

				/* number of P frames */
				if (i == 0)
                {
					np = (n + 2 * (m_M - 1)) / m_M - 1; /* first GOP */
                }
				else
                {
					np = (n + (m_M - 1)) / m_M - 1;
                }

				/* number of B frames */
				nb = n - np - 1;

				rc_init_GOP(np,nb);

				putgophdr(f0,i==0); /* set closed_GOP in first GOP only */
			}
			else
			{
				/* P frame */
				m_pict_type = P_TYPE;
				m_forw_hor_f_code = m_motion_data[0].forw_hor_f_code;
				m_forw_vert_f_code = m_motion_data[0].forw_vert_f_code;
				m_back_hor_f_code = m_back_vert_f_code = 15;
				sxf = m_motion_data[0].sxf;
				syf = m_motion_data[0].syf;
			}
		}
		else
		{
			/* B frame */
			for (j=0; j<3; j++)
			{
				neworg[j] = m_auxorgframe[j];
				newref[j] = m_auxframe[j];
			}

			/* f: frame number in display order */
			f = i - 1;
			m_pict_type = B_TYPE;
			n =  ( (i - 2) % m_M ) + 1; /* first B: n=1, second B: n=2, ... */
			m_forw_hor_f_code = m_motion_data[n].forw_hor_f_code;
			m_forw_vert_f_code = m_motion_data[n].forw_vert_f_code;
			m_back_hor_f_code = m_motion_data[n].back_hor_f_code;
			m_back_vert_f_code = m_motion_data[n].back_vert_f_code;
			sxf = m_motion_data[n].sxf;
			syf = m_motion_data[n].syf;
			sxb = m_motion_data[n].sxb;
			syb = m_motion_data[n].syb;
		}

		m_temp_ref = f - f0;
		m_frame_pred_dct = m_frame_pred_dct_tab[m_pict_type - 1];
		m_q_scale_type = m_qscale_tab[m_pict_type - 1];
		m_intravlc = m_intravlc_tab[m_pict_type - 1];
		m_altscan = m_altscan_tab[m_pict_type - 1];

		fprintf(m_statfile,"\nFrame %d (#%d in display order):\n",i,f);
		fprintf(m_statfile," picture_type=%c\n", ipb[m_pict_type]);
		fprintf(m_statfile," temporal_reference=%d\n", m_temp_ref);
		fprintf(m_statfile," m_frame_pred_frame_dct=%d\n", m_frame_pred_dct);
		fprintf(m_statfile," m_q_scale_type=%d\n", m_q_scale_type);
		fprintf(m_statfile," m_intra_vlc_format=%d\n", m_intravlc);
		fprintf(m_statfile," m_alternate_scan=%d\n", m_altscan);

		if (m_pict_type != I_TYPE)
		{
			fprintf(m_statfile," forward search window: %d...%d / %d...%d\n",
				-sxf,sxf,-syf,syf);
			fprintf(m_statfile," forward vector range: %d...%d.5 / %d...%d.5\n",
				-(4 << m_forw_hor_f_code),(4 << m_forw_hor_f_code)-1,
				-(4 << m_forw_vert_f_code),(4 << m_forw_vert_f_code)-1);
		}

		if (m_pict_type == B_TYPE)
		{
			fprintf(m_statfile," backward search window: %d...%d / %d...%d\n",
				-sxb,sxb,-syb,syb);
			fprintf(m_statfile," backward vector range: %d...%d.5 / %d...%d.5\n",
				-(4 << m_back_hor_f_code),(4 << m_back_hor_f_code)-1,
				-(4 << m_back_vert_f_code),(4 << m_back_vert_f_code)-1);
		}

        std::ostringstream current_frame_stringstream;
        int current_frame_number = f + m_frame0;
        current_frame_stringstream << current_frame_number;
        int current_frame_number_of_digits = (int)current_frame_stringstream.str().length();
        int padding_length = (int)m_frame_number_padding.length() - current_frame_number_of_digits;

        std::string padding = "";

        for (int padding_index = 0; padding_index < padding_length; padding_index++)
        {
            padding += "0";
        }
        
        sprintf(name, m_tplorg, padding.c_str(), current_frame_number);

        sprintf(m_feedback_string, "PROCESSING frame number :%d", i );
        setFeedbackString();

		readframe(f, name, neworg);

		if (m_fieldpic)
		{
			if (!m_quiet)
			{
				fprintf(stderr,"\nfirst field  (%s) ", m_topfirst ? "top" : "bot");
				fflush(stderr);
			}

			m_pict_struct = m_topfirst ? TOP_FIELD : BOTTOM_FIELD;

			//motion_estimation(m_oldorgframe[0],m_neworgframe[0],
			//	m_oldrefframe[0],m_newrefframe[0],
			//	neworg[0],newref[0],
			//	sxf, syf, sxb, syb, m_mbinfo,0,0);

			//predict(m_oldrefframe, m_newrefframe, m_predframe, 0, m_mbinfo);
			//dct_type_estimation(m_predframe[0], neworg[0], m_mbinfo);
			//transform(m_predframe, neworg, m_mbinfo, m_blocks);

			putpict(neworg[0]);

			for (k = 0; k < m_mb_height2 * m_mb_width; k++)
			{
				if (m_mbinfo[k].mb_type & MB_INTRA)
                {
					for (j = 0; j < m_block_count; j++)
                    {
						iquant_intra(m_blocks[k * m_block_count+j], m_blocks[k * m_block_count+j],
							m_dc_prec, m_intra_q, m_mbinfo[k].mquant);
                    }
                }
				else
                {
					for (j = 0; j < m_block_count; j++)
                    {
						iquant_non_intra(m_blocks[k * m_block_count+j], 
                            m_blocks[k * m_block_count + j],
							m_inter_q, m_mbinfo[k].mquant);
                    }
                }
			}

			itransform(m_predframe, newref, m_mbinfo, m_blocks);
			calcSNR(neworg,newref);
			stats();

			if (!m_quiet)
			{
				fprintf(stderr,"second field (%s) ", m_topfirst ? "bot" : "top");
				fflush(stderr);
			}

			m_pict_struct = m_topfirst ? BOTTOM_FIELD : TOP_FIELD;

			ipflag = (m_pict_type == I_TYPE);

			if (ipflag)
			{
				/* first field = I, second field = P */
				m_pict_type = P_TYPE;
				m_forw_hor_f_code = m_motion_data[0].forw_hor_f_code;
				m_forw_vert_f_code = m_motion_data[0].forw_vert_f_code;
				m_back_hor_f_code = m_back_vert_f_code = 15;
				sxf = m_motion_data[0].sxf;
				syf = m_motion_data[0].syf;
			}

			motion_estimation(m_oldorgframe[0], m_neworgframe[0],
				m_oldrefframe[0], m_newrefframe[0],
				neworg[0], newref[0],
				sxf, syf, sxb, syb,	m_mbinfo, 1, ipflag);

			predict(m_oldrefframe, m_newrefframe, m_predframe, 1, m_mbinfo);
			dct_type_estimation(m_predframe[0], neworg[0], m_mbinfo);
			transform(m_predframe, neworg, m_mbinfo, m_blocks);

			putpict(neworg[0]);

			for (k = 0; k < m_mb_height2 * m_mb_width; k++)
			{
				if (m_mbinfo[k].mb_type & MB_INTRA)
                {
					for (j = 0; j < m_block_count; j++)
                    {
						iquant_intra(m_blocks[k * m_block_count + j], m_blocks[k * m_block_count + j],
							m_dc_prec, m_intra_q, m_mbinfo[k].mquant);
                    }
                }
				else
                {
					for (j = 0; j < m_block_count; j++)
                    {
						iquant_non_intra(m_blocks[k * m_block_count + j], m_blocks[k * m_block_count + j],
							m_inter_q, m_mbinfo[k].mquant);
                    }
                }
			}

			itransform(m_predframe, newref, m_mbinfo, m_blocks);
			calcSNR(neworg,newref);
			stats();
		}
		else
		{
			m_pict_struct = FRAME_PICTURE;

			/* do motion_estimation
			 *
			 * uses source frames (...orgframe) for full pel search
			 * and reconstructed frames (...refframe) for half pel search
			 */

			motion_estimation(m_oldorgframe[0], m_neworgframe[0],
				m_oldrefframe[0], m_newrefframe[0],
				neworg[0], newref[0],
				sxf, syf, sxb, syb, m_mbinfo, 0, 0);

			predict(m_oldrefframe, m_newrefframe, m_predframe, 0, m_mbinfo);
			dct_type_estimation(m_predframe[0], neworg[0], m_mbinfo);
			transform(m_predframe, neworg, m_mbinfo, m_blocks);

			putpict(neworg[0]);

			for (k = 0; k < (m_mb_height * m_mb_width); k++)
			{
				if (m_mbinfo[k].mb_type & MB_INTRA)
                {
					for (j = 0; j < m_block_count; j++)
                    {
						iquant_intra(m_blocks[(k * m_block_count) + j], 
                            m_blocks[(k * m_block_count) + j],
							m_dc_prec, m_intra_q, m_mbinfo[k].mquant);
                    }
                }
				else
                {
					for (j = 0; j < m_block_count; j++)
                    {
						iquant_non_intra(m_blocks[(k * m_block_count) + j], 
							m_blocks[(k * m_block_count) + j],
							m_inter_q, m_mbinfo[k]. mquant);
                    }
                }
			}

			itransform(m_predframe, newref, m_mbinfo, m_blocks);
			calcSNR(neworg,newref);
			stats();
		}

		sprintf(name, m_tplref, f + m_frame0);
		writeframe(name, newref);

	}

	putseqend();
}
