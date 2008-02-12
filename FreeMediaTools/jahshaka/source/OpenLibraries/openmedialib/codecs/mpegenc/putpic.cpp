/* putpic.c, block and motion vector encoding routines                      */

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


/* quantization / variable length encoding of a complete picture */

void 
MpegEncoder::putpict(unsigned char* frame)
{
	int i, j, k, comp, cc;
	int mb_type;
	int PMV[2][2][2];
	int prev_mquant;
	int cbp; 
	int MBAinc = 0;

	rc_init_pict(frame); /* set up rate control */

	/* picture header and picture coding extension */
	putpicthdr();

	if (!m_mpeg1)
	{
		putpictcodext();
	}

	prev_mquant = rc_start_mb(); /* initialize quantization parameter */

	k = 0;

	for (j = 0; j < m_mb_height2; j++)
	{
		/* macroblock row loop */

		for (i = 0; i < m_mb_width; i++)
		{
			/* macroblock loop */
			if (i == 0)
			{
				/* slice header (6.2.4) */
				alignbits();

				if (m_mpeg1 || m_vertical_size <= 2800)
				{
					putbits(SLICE_MIN_START+j,32); /* slice_start_code */
				}
				else
				{
					putbits(SLICE_MIN_START+(j&127),32); /* slice_start_code */
					putbits(j>>7,3); /* slice_vertical_position_extension */
				}
  
				/* quantiser_scale_code */
				putbits(m_q_scale_type ? m_map_non_linear_mquant[prev_mquant]
					: prev_mquant >> 1, 5);
  
				putbits(0,1); /* extra_bit_slice */
  
				/* reset predictors */

				for (cc=0; cc<3; cc++)
				{
					m_dc_dct_pred[cc] = 0;
				}

				PMV[0][0][0]=PMV[0][0][1]=PMV[1][0][0]=PMV[1][0][1]=0;
				PMV[0][1][0]=PMV[0][1][1]=PMV[1][1][0]=PMV[1][1][1]=0;
  
				MBAinc = i + 1; /* first MBAinc denotes absolute position */
			}

			mb_type = m_mbinfo[k].mb_type;

			/* determine mquant (rate control) */
			m_mbinfo[k].mquant = rc_calc_mquant(k);

			/* quantize macroblock */
			if (mb_type & MB_INTRA)
			{
				for (comp = 0; comp < m_block_count; comp++)
				{
					quant_intra(m_blocks[(k * m_block_count) + comp], m_blocks[(k * m_block_count) + comp],
						m_dc_prec, m_intra_q, m_mbinfo[k].mquant);
				}

				m_mbinfo[k].cbp = cbp = (1 << m_block_count) - 1;
			}
			else
			{
				cbp = 0;

				for (comp = 0;comp < m_block_count; comp++)
				{
					cbp = (cbp<<1) | quant_non_intra(m_blocks[(k * m_block_count) + comp],
						m_blocks[(k * m_block_count) + comp],
						m_inter_q, m_mbinfo[k].mquant);
				}

				m_mbinfo[k].cbp = cbp;

				if (cbp)
				{
					mb_type|= MB_PATTERN;
				}
			}

			/* output mquant if it has changed */
			if (cbp && prev_mquant != m_mbinfo[k].mquant)
				mb_type|= MB_QUANT;

			/* check if macroblock can be skipped */
			if ((i != 0) && (i != m_mb_width - 1) && !cbp)
			{
				/* no DCT coefficients and neither first nor last macroblock of slice */

				if (m_pict_type == P_TYPE && !( mb_type & MB_FORWARD))
				{
					/* P picture, no motion vectors -> skip */

					/* reset predictors */

					for (cc=0; cc<3; cc++)
					{
						m_dc_dct_pred[cc] = 0;
					}

					PMV[0][0][0]=PMV[0][0][1]=PMV[1][0][0]=PMV[1][0][1]=0;
					PMV[0][1][0]=PMV[0][1][1]=PMV[1][1][0]=PMV[1][1][1]=0;

					m_mbinfo[k].mb_type = mb_type;
					m_mbinfo[k].skipped = 1;
					MBAinc++;
					k++;
					continue;
				}

				if (m_pict_type == B_TYPE && m_pict_struct == FRAME_PICTURE
					&& m_mbinfo[k].motion_type == MC_FRAME
					&& ((m_mbinfo[k-1].mb_type ^ mb_type) & (MB_FORWARD | MB_BACKWARD)) == 0
					&& (!(mb_type&MB_FORWARD) ||
						(PMV[0][0][0] == m_mbinfo[k].MV[0][0][0] &&
							PMV[0][0][1] == m_mbinfo[k].MV[0][0][1]))
					&& (!(mb_type&MB_BACKWARD) ||
						(PMV[0][1][0] == m_mbinfo[k].MV[0][1][0] &&
							PMV[0][1][1] == m_mbinfo[k].MV[0][1][1])))
				{
					/* conditions for skipping in B frame pictures:
					 * - must be frame predicted
					 * - must be the same prediction type (forward/backward/interp.)
					 *   as previous macroblock
					 * - relevant vectors (forward/backward/both) have to be the same
					 *   as in previous macroblock
					 */

					m_mbinfo[k].mb_type = mb_type;
					m_mbinfo[k].skipped = 1;
					MBAinc++;
					k++;
					continue;
				}

				if (m_pict_type == B_TYPE && m_pict_struct != FRAME_PICTURE
					&& m_mbinfo[k].motion_type == MC_FIELD
					&& ((m_mbinfo[k-1].mb_type ^ mb_type) & (MB_FORWARD | MB_BACKWARD)) == 0
					&& (!(mb_type & MB_FORWARD) ||
						(PMV[0][0][0] == m_mbinfo[k].MV[0][0][0] &&
							PMV[0][0][1] == m_mbinfo[k].MV[0][0][1] &&
							m_mbinfo[k].mv_field_sel[0][0] == (m_pict_struct == BOTTOM_FIELD)))
					&& (!(mb_type & MB_BACKWARD) ||
						(PMV[0][1][0] == m_mbinfo[k].MV[0][1][0] &&
							PMV[0][1][1] == m_mbinfo[k].MV[0][1][1] &&
							m_mbinfo[k].mv_field_sel[0][1] == (m_pict_struct == BOTTOM_FIELD))))
				{
					/* conditions for skipping in B field pictures:
					 * - must be field predicted
					 * - must be the same prediction type (forward/backward/interp.)
					 *   as previous macroblock
					 * - relevant vectors (forward/backward/both) have to be the same
					 *   as in previous macroblock
					 * - relevant motion_vertical_field_selects have to be of same
					 *   parity as current field
					 */

					m_mbinfo[k].mb_type = mb_type;
					m_mbinfo[k].skipped = 1;
					MBAinc++;
					k++;
					continue;
				}
			}

			/* macroblock cannot be skipped */
			m_mbinfo[k].skipped = 0;

			/* there's no VLC for 'No MC, Not Coded':
			 * we have to transmit (0,0) motion vectors
			 */
			if (m_pict_type == P_TYPE && !cbp && !(mb_type & MB_FORWARD))
				mb_type |= MB_FORWARD;

			putaddrinc(MBAinc); /* macroblock_address_increment */
			MBAinc = 1;

			putmbtype(m_pict_type, mb_type); /* macroblock type */

			if (mb_type & (MB_FORWARD|MB_BACKWARD) && !m_frame_pred_dct)
			{
				putbits(m_mbinfo[k].motion_type,2);
			}

			if (m_pict_struct == FRAME_PICTURE && cbp && !m_frame_pred_dct)
			{
				putbits(m_mbinfo[k].dct_type,1);
			}

			if (mb_type & MB_QUANT)
			{
				putbits(m_q_scale_type ? m_map_non_linear_mquant[m_mbinfo[k].mquant]
					: m_mbinfo[k].mquant>>1,5);
				prev_mquant = m_mbinfo[k].mquant;
			}

			if (mb_type & MB_FORWARD)
			{
				/* forward motion vectors, update predictors */
				putmvs(m_mbinfo[k].MV, PMV, m_mbinfo[k].mv_field_sel, m_mbinfo[k].dmvector,0,
					m_mbinfo[k].motion_type, m_forw_hor_f_code, m_forw_vert_f_code);
			}

			if (mb_type & MB_BACKWARD)
			{
				/* backward motion vectors, update predictors */
				putmvs(m_mbinfo[k].MV, PMV, m_mbinfo[k].mv_field_sel, m_mbinfo[k].dmvector,1,
					m_mbinfo[k].motion_type, m_back_hor_f_code, m_back_vert_f_code);
			}

			if (mb_type & MB_PATTERN)
			{
				putcbp((cbp >> (m_block_count - 6)) & 63);

				if (m_chroma_format != CHROMA420)
				{
					putbits(cbp, m_block_count - 6);
				}
			}

			for (comp = 0; comp < m_block_count; comp++)
			{
				/* block loop */
				if (cbp & (1 << (m_block_count - 1 - comp) ))
				{
					if (mb_type & MB_INTRA)
					{
						cc = (comp<4) ? 0 : (comp&1)+1;
						putintrablk(m_blocks[(k * m_block_count) + comp], cc);
					}
					else
					{
						putnonintrablk(m_blocks[(k * m_block_count) + comp] );
					}
				}
			}

			/* reset predictors */
			if (!(mb_type & MB_INTRA))
			{
				for (cc=0; cc<3; cc++)
				{
					m_dc_dct_pred[cc] = 0;
				}
			}

			if (mb_type & MB_INTRA || (m_pict_type == P_TYPE && !(mb_type & MB_FORWARD)))
			{
				PMV[0][0][0]=PMV[0][0][1]=PMV[1][0][0]=PMV[1][0][1]=0;
				PMV[0][1][0]=PMV[0][1][1]=PMV[1][1][0]=PMV[1][1][1]=0;
			}

			m_mbinfo[k].mb_type = mb_type;
			k++;
		}
	}

	rc_update_pict();
	vbv_end_of_picture();
}


/* output motion vectors (6.2.5.2, 6.3.16.2)
 *
 * this routine also updates the predictions for motion vectors (PMV)
 */
 

void MpegEncoder::putmvs(
	int MV[2][2][2],
	int PMV[2][2][2],
	int mv_field_sel[2][2],
	int dmvector[2],
	int s,
	int motion_type,
	int hor_f_code,
	int vert_f_code)
{
	if (m_pict_struct == FRAME_PICTURE)
	{
		if (motion_type==MC_FRAME)
		{
			/* frame prediction */
			putmv(MV[0][s][0]-PMV[0][s][0],hor_f_code);
			putmv(MV[0][s][1]-PMV[0][s][1],vert_f_code);
			PMV[0][s][0]=PMV[1][s][0]=MV[0][s][0];
			PMV[0][s][1]=PMV[1][s][1]=MV[0][s][1];
		}
		else if (motion_type==MC_FIELD)
		{
			/* field prediction */
			putbits(mv_field_sel[0][s],1);
			putmv(MV[0][s][0]-PMV[0][s][0],hor_f_code);
			putmv((MV[0][s][1]>>1)-(PMV[0][s][1]>>1),vert_f_code);
			putbits(mv_field_sel[1][s],1);
			putmv(MV[1][s][0]-PMV[1][s][0],hor_f_code);
			putmv((MV[1][s][1]>>1)-(PMV[1][s][1]>>1),vert_f_code);
			PMV[0][s][0]=MV[0][s][0];
			PMV[0][s][1]=MV[0][s][1];
			PMV[1][s][0]=MV[1][s][0];
			PMV[1][s][1]=MV[1][s][1];
		}
		else
		{
			/* dual prime prediction */
			putmv(MV[0][s][0]-PMV[0][s][0],hor_f_code);
			putdmv(dmvector[0]);
			putmv((MV[0][s][1]>>1)-(PMV[0][s][1]>>1),vert_f_code);
			putdmv(dmvector[1]);
			PMV[0][s][0]=PMV[1][s][0]=MV[0][s][0];
			PMV[0][s][1]=PMV[1][s][1]=MV[0][s][1];
		}
	}
	else
	{
		/* field picture */
		if (motion_type==MC_FIELD)
		{
			/* field prediction */
			putbits(mv_field_sel[0][s],1);
			putmv(MV[0][s][0]-PMV[0][s][0],hor_f_code);
			putmv(MV[0][s][1]-PMV[0][s][1],vert_f_code);
			PMV[0][s][0]=PMV[1][s][0]=MV[0][s][0];
			PMV[0][s][1]=PMV[1][s][1]=MV[0][s][1];
		}
		else if (motion_type==MC_16X8)
		{
			/* 16x8 prediction */
			putbits(mv_field_sel[0][s],1);
			putmv(MV[0][s][0]-PMV[0][s][0],hor_f_code);
			putmv(MV[0][s][1]-PMV[0][s][1],vert_f_code);
			putbits(mv_field_sel[1][s],1);
			putmv(MV[1][s][0]-PMV[1][s][0],hor_f_code);
			putmv(MV[1][s][1]-PMV[1][s][1],vert_f_code);
			PMV[0][s][0]=MV[0][s][0];
			PMV[0][s][1]=MV[0][s][1];
			PMV[1][s][0]=MV[1][s][0];
			PMV[1][s][1]=MV[1][s][1];
		}
		else
		{
			/* dual prime prediction */
			putmv(MV[0][s][0]-PMV[0][s][0],hor_f_code);
			putdmv(dmvector[0]);
			putmv(MV[0][s][1]-PMV[0][s][1],vert_f_code);
			putdmv(dmvector[1]);
			PMV[0][s][0]=PMV[1][s][0]=MV[0][s][0];
			PMV[0][s][1]=PMV[1][s][1]=MV[0][s][1];
		}
	}
}
