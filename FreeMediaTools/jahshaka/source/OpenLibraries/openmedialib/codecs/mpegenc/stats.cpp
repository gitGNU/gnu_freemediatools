/* stats.c, coding statistics                                               */

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

/* private prototypes */


void 
MpegEncoder::calcSNR(unsigned char* org[3], unsigned char* rec[3])
{
	int w,h,offs;
	double v,e;

	w = m_horizontal_size;

	h = (m_pict_struct == FRAME_PICTURE) ? m_vertical_size : (m_vertical_size >> 1);

	offs = (m_pict_struct == BOTTOM_FIELD) ? m_width : 0;

	calcSNR1(org[0]+offs, rec[0]+offs, m_width2, w, h, &v, &e);
	fprintf(m_statfile,"Y: variance=%4.4g, MSE=%3.3g (%3.3g dB), SNR=%3.3g dB\n",
		v, e, 10.0*log10(255.0*255.0/e), 10.0*log10(v/e));

	if (m_chroma_format != CHROMA444)
	{
		w >>= 1;
		offs >>= 1;
	}

	if (m_chroma_format == CHROMA420)
    {
		h >>= 1;
    }

	calcSNR1(org[1]+offs, rec[1]+offs, m_chrom_width2, w, h, &v, &e);
	fprintf(m_statfile,"U: variance=%4.4g, MSE=%3.3g (%3.3g dB), SNR=%3.3g dB\n",
		v, e, 10.0*log10(255.0*255.0/e), 10.0*log10(v/e));

	calcSNR1(org[2]+offs,rec[2]+offs, m_chrom_width2, w, h, &v, &e);
	fprintf(m_statfile,"V: variance=%4.4g, MSE=%3.3g (%3.3g dB), SNR=%3.3g dB\n",
		v, e, 10.0*log10(255.0*255.0/e), 10.0*log10(v/e));
}


void 
MpegEncoder::calcSNR1(
	unsigned char* org, 
	unsigned char* rec, 
	int lx, 
	int w, 
	int h,
	double* pv,
	double* pe)
{
	int i, j;
	double v1, s1, s2, e2;

	s1 = s2 = e2 = 0.0;

	for (j=0; j<h; j++)
	{
		for (i=0; i<w; i++)
		{
			v1 = org[i];
			s1+= v1;
			s2+= v1*v1;
			v1-= rec[i];
			e2+= v1*v1;
		}
		org += lx;
		rec += lx;
	}

	s1 /= w*h;
	s2 /= w*h;
	e2 /= w*h;

	/* prevent division by zero in calcSNR() */
	if(e2==0.0)
		e2 = 0.00001;

	*pv = s2 - s1*s1; /* variance */
	*pe = e2;         /* MSE */
}


void 
MpegEncoder::stats()
{
	int i, j, k, nmb, mb_type;
	int n_skipped, n_intra, n_ncoded, n_blocks, n_interp, n_forward, n_backward;
	struct mbinfo *mbi;

	nmb = m_mb_width * m_mb_height2;

	n_skipped=n_intra=n_ncoded=n_blocks=n_interp=n_forward=n_backward=0;

	for (k=0; k<nmb; k++)
	{
		mbi = m_mbinfo+k;

		if (mbi->skipped)
			n_skipped++;
		else if (mbi->mb_type & MB_INTRA)
			n_intra++;
		else if (!(mbi->mb_type & MB_PATTERN))
			n_ncoded++;

		for (i = 0; i < m_block_count; i++)
        {
			if (mbi->cbp & (1 << i) )
            {
				n_blocks++;
            }
        }

		if (mbi->mb_type & MB_FORWARD)
		{
			if (mbi->mb_type & MB_BACKWARD)
				n_interp++;
			else
				n_forward++;
		}
		else if (mbi->mb_type & MB_BACKWARD)
			n_backward++;
	}

	fprintf(m_statfile,"\npicture statistics:\n");
	fprintf(m_statfile," # of intra coded macroblocks:  %4d (%.1f%%)\n",
		n_intra,100.0*(double)n_intra/nmb);
	fprintf(m_statfile," # of coded blocks:             %4d (%.1f%%)\n",
		n_blocks,100.0*(double)n_blocks/(m_block_count*nmb));
	fprintf(m_statfile," # of not coded macroblocks:    %4d (%.1f%%)\n",
		n_ncoded,100.0*(double)n_ncoded/nmb);
	fprintf(m_statfile," # of skipped macroblocks:      %4d (%.1f%%)\n",
		n_skipped,100.0*(double)n_skipped/nmb);
	fprintf(m_statfile," # of forw. pred. macroblocks:  %4d (%.1f%%)\n",
		n_forward,100.0*(double)n_forward/nmb);
	fprintf(m_statfile," # of backw. pred. macroblocks: %4d (%.1f%%)\n",
		n_backward,100.0*(double)n_backward/nmb);
	fprintf(m_statfile," # of interpolated macroblocks: %4d (%.1f%%)\n",
		n_interp,100.0*(double)n_interp/nmb);

	fprintf(m_statfile,"\nmacroblock_type map:\n");

	k = 0;

	for (j = 0; j < m_mb_height2; j++)
	{
		for (i = 0; i < m_mb_width; i++)
		{
			mbi = m_mbinfo + k;
			mb_type = mbi->mb_type;

			if (mbi->skipped)
			{
				putc('S',m_statfile);
			}
			else if (mb_type & MB_INTRA)
			{
				putc('I',m_statfile);
			}
			else switch (mb_type & (MB_FORWARD|MB_BACKWARD))
			{
				case MB_FORWARD:
					putc(mbi->motion_type==MC_FIELD ? 'f' :
						mbi->motion_type==MC_DMV   ? 'p' :
						'F',m_statfile); break;
				case MB_BACKWARD:
					putc(mbi->motion_type==MC_FIELD ? 'b' :
						'B',m_statfile); break;
				case MB_FORWARD|MB_BACKWARD:
					putc(mbi->motion_type==MC_FIELD ? 'd' :
						'D',m_statfile); break;
				default:
					putc('0',m_statfile); break;
			}

			if (mb_type & MB_QUANT)
				putc('Q',m_statfile);
			else if (mb_type & (MB_PATTERN|MB_INTRA))
				putc(' ',m_statfile);
			else
				putc('N',m_statfile);

			putc(' ',m_statfile);

			k++;
		}
		putc('\n',m_statfile);
	}

	fprintf(m_statfile,"\nmquant map:\n");

	k=0;

	for (j = 0; j < m_mb_height2; j++)
	{
		for (i = 0; i < m_mb_width; i++)
		{
			if (i==0 || m_mbinfo[k].mquant != m_mbinfo[k-1].mquant)
			{
				fprintf(m_statfile,"%3d", m_mbinfo[k].mquant);
			}
			else
			{
				fprintf(m_statfile,"   ");
			}

			k++;
		}
		putc('\n',m_statfile);
	}

#if 0
	fprintf(m_statfile,"\ncbp map:\n");

	k=0;
	for (j=0; j<mb_height2; j++)
	{
		for (i=0; i<mb_width; i++)
		{
			fprintf(m_statfile,"%02x ",mbinfo[k].cbp);

			k++;
		}
		putc('\n',m_statfile);
	}

	if (pict_struct==FRAME_PICTURE && !frame_pred_dct)
	{
		fprintf(m_statfile,"\ndct_type map:\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & (MB_PATTERN|MB_INTRA))
					fprintf(m_statfile,"%d  ",mbinfo[k].dct_type);
				else
					fprintf(m_statfile,"   ");
  
				k++;
			}
			putc('\n',m_statfile);
		}
	}

	if (pict_type!=I_TYPE)
	{
		fprintf(m_statfile,"\nforward motion vectors (first vector, horizontal):\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & MB_FORWARD)
					fprintf(m_statfile,"%4d",mbinfo[k].MV[0][0][0]);
				else
					fprintf(m_statfile,"   .");
  
				k++;
			}
			putc('\n',m_statfile);
		}

		fprintf(m_statfile,"\nforward motion vectors (first vector, vertical):\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & MB_FORWARD)
					fprintf(m_statfile,"%4d",mbinfo[k].MV[0][0][1]);
				else
					fprintf(m_statfile,"   .");
  
				k++;
			}
			putc('\n',m_statfile);
		}

		fprintf(m_statfile,"\nforward motion vectors (second vector, horizontal):\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & MB_FORWARD
					&& ((pict_struct==FRAME_PICTURE && mbinfo[k].motion_type==MC_FIELD) ||
						(pict_struct!=FRAME_PICTURE && mbinfo[k].motion_type==MC_16X8)))
					fprintf(m_statfile,"%4d",mbinfo[k].MV[1][0][0]);
				else
					fprintf(m_statfile,"   .");
  
				k++;
			}
			putc('\n',m_statfile);
		}

		fprintf(m_statfile,"\nforward motion vectors (second vector, vertical):\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & MB_FORWARD
					&& ((pict_struct==FRAME_PICTURE && mbinfo[k].motion_type==MC_FIELD) ||
						(pict_struct!=FRAME_PICTURE && mbinfo[k].motion_type==MC_16X8)))
					fprintf(m_statfile,"%4d",mbinfo[k].MV[1][0][1]);
				else
					fprintf(m_statfile,"   .");
  
				k++;
			}
			putc('\n',m_statfile);
		}


	}
    
	if (pict_type==B_TYPE)
	{
		fprintf(m_statfile,"\nbackward motion vectors (first vector, horizontal):\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & MB_BACKWARD)
					fprintf(m_statfile,"%4d",mbinfo[k].MV[0][1][0]);
				else
					fprintf(m_statfile,"   .");
  
				k++;
			}
			putc('\n',m_statfile);
		}

		fprintf(m_statfile,"\nbackward motion vectors (first vector, vertical):\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & MB_BACKWARD)
					fprintf(m_statfile,"%4d",mbinfo[k].MV[0][1][1]);
				else
					fprintf(m_statfile,"   .");
  
				k++;
			}
			putc('\n',m_statfile);
		}

		fprintf(m_statfile,"\nbackward motion vectors (second vector, horizontal):\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & MB_BACKWARD
					&& ((pict_struct==FRAME_PICTURE && mbinfo[k].motion_type==MC_FIELD) ||
						(pict_struct!=FRAME_PICTURE && mbinfo[k].motion_type==MC_16X8)))
					fprintf(m_statfile,"%4d",mbinfo[k].MV[1][1][0]);
				else
					fprintf(m_statfile,"   .");
  
				k++;
			}
			putc('\n',m_statfile);
		}

		fprintf(m_statfile,"\nbackward motion vectors (second vector, vertical):\n");

		k=0;
		for (j=0; j<mb_height2; j++)
		{
			for (i=0; i<mb_width; i++)
			{
				if (mbinfo[k].mb_type & MB_BACKWARD
					&& ((pict_struct==FRAME_PICTURE && mbinfo[k].motion_type==MC_FIELD) ||
						(pict_struct!=FRAME_PICTURE && mbinfo[k].motion_type==MC_16X8)))
					fprintf(m_statfile,"%4d",mbinfo[k].MV[1][1][1]);
				else
					fprintf(m_statfile,"   .");
  
				k++;
			}
			putc('\n',m_statfile);
		}


	}
#endif
    
#if 0
	/* useful for debugging */
	fprintf(m_statfile,"\nmacroblock info dump:\n");

	k=0;
	for (j=0; j<mb_height2; j++)
	{
		for (i=0; i<mb_width; i++)
		{
			fprintf(m_statfile,"%d: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
				k,
				mbinfo[k].mb_type,
				mbinfo[k].motion_type,
				mbinfo[k].dct_type,
				mbinfo[k].mquant,
				mbinfo[k].cbp,
				mbinfo[k].skipped,
				mbinfo[k].MV[0][0][0],
				mbinfo[k].MV[0][0][1],
				mbinfo[k].MV[0][1][0],
				mbinfo[k].MV[0][1][1],
				mbinfo[k].MV[1][0][0],
				mbinfo[k].MV[1][0][1],
				mbinfo[k].MV[1][1][0],
				mbinfo[k].MV[1][1][1],
				mbinfo[k].mv_field_sel[0][0],
				mbinfo[k].mv_field_sel[0][1],
				mbinfo[k].mv_field_sel[1][0],
				mbinfo[k].mv_field_sel[1][1]);

			k++;
		}
	}
#endif
}
