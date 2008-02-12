/* m_ratectl.c, bitrate control routines (linear quantization only currently) */

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


void 
MpegEncoder::rc_init_seq()
{
	/* reaction parameter (constant) */
    if (m_r == 0)  { m_r = (int)floor(2.0 * m_bit_rate / m_frame_rate + 0.5); }

	/* average activity */
    if (m_avg_act == 0.0)  { m_avg_act = 400.0; }

	/* remaining # of bits in GOP */
	m_ratectl_R = 0;

	/* global complexity measure */
    if (m_Xi == 0) { m_Xi = (int)floor(160.0*m_bit_rate/115.0 + 0.5); }
    if (m_Xp == 0) { m_Xp = (int)floor( 60.0*m_bit_rate/115.0 + 0.5); }
    if (m_Xb == 0) { m_Xb = (int)floor( 42.0*m_bit_rate/115.0 + 0.5); }

	/* virtual buffer fullness */
    if (m_d0i == 0) { m_d0i = (int)floor(10.0 * m_r/31.0 + 0.5); }
    if (m_d0p == 0) { m_d0p = (int)floor(10.0 * m_r/31.0 + 0.5); }
    if (m_d0b == 0) { m_d0b = (int)floor(1.4 * 10.0 * m_r / 31.0 + 0.5); }
/*
  if (d0i==0) d0i = (int)floor(10.0*r/(qscale_tab[0] ? 56.0 : 31.0) + 0.5);
  if (d0p==0) d0p = (int)floor(10.0*r/(qscale_tab[1] ? 56.0 : 31.0) + 0.5);
  if (d0b==0) d0b = (int)floor(1.4*10.0*r/(qscale_tab[2] ? 56.0 : 31.0) + 0.5);
*/

	fprintf(m_statfile,"\nrate control: sequence initialization\n");
	fprintf(m_statfile,
		" initial global complexity measures (I,P,B): Xi=%d, Xp=%d, Xb=%d\n",
		m_Xi, m_Xp, m_Xb);
	fprintf(m_statfile," reaction parameter: m_r = %d\n", m_r);
	fprintf(m_statfile,
		" initial virtual buffer fullness (I,P,B): m_d0i=%d, m_d0p=%d, m_d0b=%d\n",
		m_d0i, m_d0p, m_d0b);
	fprintf(m_statfile," initial average activity: m_avg_act=%.1f\n", m_avg_act);
}

void 
MpegEncoder::rc_init_GOP(int np, int nb)
{
	m_ratectl_R += (int) floor((1 + np + nb) * m_bit_rate / m_frame_rate + 0.5);
	m_ratectl_Np = m_fieldpic ? 2*np+1 : np;
	m_ratectl_Nb = m_fieldpic ? 2*nb : nb;

	fprintf(m_statfile,"\nrate control: new group of pictures (GOP)\n");
	fprintf(m_statfile," target number of bits for GOP: m_ratectl_R = %d\n", m_ratectl_R);
	fprintf(m_statfile," number of P pictures in GOP: m_ratectl_Np=%d\n", m_ratectl_Np);
	fprintf(m_statfile," number of B pictures in GOP: m_ratectl_Nb=%d\n", m_ratectl_Nb);
}

/* Note: we need to substitute K for the 1.4 and 1.0 constants -- this can
   be modified to fit image content */

/* Step 1: compute target bits for current picture being coded */

void 
MpegEncoder::rc_init_pict(unsigned char* frame)
{
	double Tmin;

	switch (m_pict_type)
	{
		case I_TYPE:
			m_ratectl_T = (int) floor(m_ratectl_R/(1.0+m_ratectl_Np * m_Xp/(m_Xi*1.0)+m_ratectl_Nb*m_Xb/(m_Xi*1.4)) + 0.5);
			m_ratectl_d = m_d0i;
			break;
		case P_TYPE:
			m_ratectl_T = (int) floor(m_ratectl_R/(m_ratectl_Np+m_ratectl_Nb*1.0*m_Xb/(1.4*m_Xp)) + 0.5);
			m_ratectl_d = m_d0p;
			break;
		case B_TYPE:
			m_ratectl_T = (int) floor(m_ratectl_R/(m_ratectl_Nb+m_ratectl_Np*1.4*m_Xp/(1.0*m_Xb)) + 0.5);
			m_ratectl_d = m_d0b;
			break;
	}

	Tmin = (int) floor(m_bit_rate/(8.0*m_frame_rate) + 0.5);

	if (m_ratectl_T<Tmin)
		m_ratectl_T = int(Tmin);

	m_ratectl_S = bitcount();
	m_ratectl_Q = 0;

	calc_actj(frame);
	m_ratectl_actsum = 0.0;

	fprintf(m_statfile,"\nrate control: start of picture\n");
	fprintf(m_statfile," target number of bits: m_ratectl_T=%d\n",m_ratectl_T);
}


void 
MpegEncoder::calc_actj(unsigned char* frame)
{
	int i,j,k;
	unsigned char *p;
	double actj,var;

	k = 0;

	for (j = 0; j < m_height2; j += 16)
    {
		for (i = 0; i < m_width; i += 16)
		{
			p = frame + ((m_pict_struct == BOTTOM_FIELD)? m_width : 0) + i + m_width2 * j;

			/* take minimum spatial activity measure of luminance blocks */

			actj = var_sblk(p, m_width2);
			var = var_sblk(p+8, m_width2);

            if (var<actj) { actj = var; }
			var = var_sblk(p + (8 * m_width2), m_width2);
            if (var<actj) { actj = var; }
			var = var_sblk(p + (8 * m_width2) + 8, m_width2);
            if (var<actj) { actj = var; }

			if (!m_fieldpic && !m_prog_seq)
			{
				/* field */
				var = var_sblk(p,m_width<<1);
				if (var<actj) actj = var;
				var = var_sblk(p+8, m_width<<1);
				if (var<actj) actj = var;
				var = var_sblk(p+m_width, m_width<<1);
				if (var<actj) actj = var;
				var = var_sblk(p+m_width+8, m_width<<1);
				if (var<actj) actj = var;
			}

			actj+= 1.0;

			m_mbinfo[k++].act = actj;
		}
    }
}


void 
MpegEncoder::rc_update_pict()
{
	double X;

	m_ratectl_S = bitcount() - m_ratectl_S; /* total # of bits in picture */
	m_ratectl_R-= m_ratectl_S; /* remaining # of bits in GOP */
	X = (int) floor(m_ratectl_S*((0.5*(double)m_ratectl_Q)/(m_mb_width * m_mb_height2)) + 0.5);
	m_ratectl_d+= m_ratectl_S - m_ratectl_T;
	m_avg_act = m_ratectl_actsum/(m_mb_width * m_mb_height2);

	switch (m_pict_type)
	{
		case I_TYPE:
			m_Xi = int(X);
			m_d0i = m_ratectl_d;
			break;
		case P_TYPE:
			m_Xp = int(X);
			m_d0p = m_ratectl_d;
			m_ratectl_Np--;
			break;
		case B_TYPE:
			m_Xb = int(X);
			m_d0b = m_ratectl_d;
			m_ratectl_Nb--;
			break;
	}

	fprintf(m_statfile,"\nrate control: end of picture\n");
	fprintf(m_statfile," actual number of bits: m_ratectl_S=%d\n",m_ratectl_S);
	fprintf(m_statfile," average quantization parameter m_ratectl_Q=%.1f\n",
		(double)m_ratectl_Q/(m_mb_width * m_mb_height2));
	fprintf(m_statfile," remaining number of bits in GOP: m_ratectl_R=%d\n",m_ratectl_R);
	fprintf(m_statfile,
		" global complexity measures (I,P,B): m_Xi=%d, m_Xp=%d, m_Xb=%d\n",
		m_Xi, m_Xp, m_Xb);
	fprintf(m_statfile,
		" virtual buffer fullness (I,P,B): m_d0i=%d, m_d0p=%d, m_d0b=%d\n",
		m_d0i, m_d0p, m_d0b);
	fprintf(m_statfile," remaining number of P pictures in GOP: m_ratectl_Np=%d\n",m_ratectl_Np);
	fprintf(m_statfile," remaining number of B pictures in GOP: m_ratectl_Nb=%d\n",m_ratectl_Nb);
	fprintf(m_statfile," average activity: m_avg_act=%.1f\n", m_avg_act);
}

/* compute initial quantization stepsize (at the beginning of picture) */

int 
MpegEncoder::rc_start_mb()
{
	int mquant;

	if (m_q_scale_type)
	{
		mquant = (int) floor(2.0 * m_ratectl_d * 31.0 / m_r + 0.5);

		/* clip mquant to legal (linear) range */
		if (mquant<1)
        {
			mquant = 1;
        }
		if (mquant>112)
        {
			mquant = 112;
        }

		/* map to legal quantization level */
		mquant = m_non_linear_mquant_table[m_map_non_linear_mquant[mquant]];
	}
	else
	{
		mquant = (int) floor(m_ratectl_d * 31.0 / m_r + 0.5);
		mquant <<= 1;

		/* clip mquant to legal (linear) range */
		if (mquant<2)
        {
			mquant = 2;
        }
		if (mquant>62)
        {
        }
			mquant = 62;

		m_ratectl_prev_mquant = mquant;
	}

/*
  fprintf(m_statfile,"rc_start_mb:\n");
  fprintf(m_statfile,"mquant=%d\n",mquant);
*/

	return mquant;
}

/* Step 2: measure virtual buffer - estimated buffer discrepancy */

int 
MpegEncoder::rc_calc_mquant(int j)
{
	int mquant;
	double dj, Qj, actj, N_actj;

	/* measure virtual buffer discrepancy from uniform distribution model */
	dj = m_ratectl_d + (bitcount()-m_ratectl_S) - j*(m_ratectl_T/(m_mb_width * m_mb_height2));

	/* scale against dynamic range of mquant and the bits/picture count */
	Qj = dj * 31.0 / m_r;
/*Qj = dj*(q_scale_type ? 56.0 : 31.0)/r;  */

	actj = m_mbinfo[j].act;
	m_ratectl_actsum+= actj;

	/* compute normalized activity */
	N_actj = (2.0 * actj + m_avg_act) / ( actj + 2.0 * m_avg_act);

	if (m_q_scale_type)
	{
		/* modulate mquant with combined buffer and local activity measures */
		mquant = (int) floor(2.0*Qj*N_actj + 0.5);

		/* clip mquant to legal (linear) range */
		if (mquant<1)
			mquant = 1;
		if (mquant>112)
			mquant = 112;

		/* map to legal quantization level */
		mquant = m_non_linear_mquant_table[m_map_non_linear_mquant[mquant]];
	}
	else
	{
		/* modulate mquant with combined buffer and local activity measures */
		mquant = (int) floor(Qj*N_actj + 0.5);
		mquant <<= 1;

		/* clip mquant to legal (linear) range */
		if (mquant<2)
			mquant = 2;
		if (mquant>62)
			mquant = 62;

		/* ignore small changes in mquant */
		if (mquant>=8 && (mquant-m_ratectl_prev_mquant)>=-4 && (mquant-m_ratectl_prev_mquant)<=4)
			mquant = m_ratectl_prev_mquant;

		m_ratectl_prev_mquant = mquant;
	}

	m_ratectl_Q += mquant; /* for calculation of average mquant */

/*
  fprintf(m_statfile,"rc_calc_mquant(%d): ",j);
  fprintf(m_statfile,"bitcount=%d, dj=%f, Qj=%f, actj=%f, N_actj=%f, mquant=%d\n",
  bitcount(),dj,Qj,actj,N_actj,mquant);
*/

	return mquant;
}

/* compute variance of 8x8 block */

double 
MpegEncoder::var_sblk(unsigned char* p, int lx)
{
	int i, j;
	unsigned int v, s, s2;

	s = s2 = 0;

	for (j=0; j<8; j++)
	{
		for (i=0; i<8; i++)
		{
			v = *p++;
			s+= v;
			s2+= v*v;
		}
		p+= lx - 8;
	}

	return s2/64.0 - (s/64.0)*(s/64.0);
}

/* VBV calculations
 *
 * generates warnings if underflow or overflow occurs
 */

/* vbv_end_of_picture
 *
 * - has to be called directly after writing picture_data()
 * - needed for accurate VBV buffer overflow calculation
 * - assumes there is no byte stuffing prior to the next start code
 */

void 
MpegEncoder::vbv_end_of_picture()
{
	m_bitcnt_EOP = bitcount();
	m_bitcnt_EOP = (m_bitcnt_EOP + 7) & ~7; /* account for bit stuffing */
}

/* calc_vbv_delay
 *
 * has to be called directly after writing the picture start code, the
 * reference point for vbv_delay
 */


void 
MpegEncoder::calc_vbv_delay()
{
	double picture_delay;
	static double next_ip_delay; /* due to frame reordering delay */
	static double decoding_time;

	/* number of 1/90000 s ticks until next picture is to be decoded */
	if (m_pict_type == B_TYPE)
	{
		if (m_prog_seq)
		{
			if (!m_repeatfirst)
            {
				picture_delay = 90000.0/m_frame_rate; /* 1 frame */
            }
			else
			{
				if (!m_topfirst)
                {
					picture_delay = 90000.0*2.0/m_frame_rate; /* 2 frames */
                }
				else
                {
					picture_delay = 90000.0*3.0/m_frame_rate; /* 3 frames */
                }
			}
		}
		else
		{
			/* interlaced */
			if (m_fieldpic)
            {
				picture_delay = 90000.0/(2.0 * m_frame_rate); /* 1 field */
            }
			else
			{
				if (!m_repeatfirst)
                {
					picture_delay = 90000.0*2.0/(2.0 * m_frame_rate); /* 2 flds */
                }
				else
                {
					picture_delay = 90000.0*3.0/(2.0 * m_frame_rate); /* 3 flds */
                }
			}
		}
	}
	else
	{
		/* I or P picture */
		if (m_fieldpic)
		{
			if(m_topfirst == (m_pict_struct == TOP_FIELD))
			{
				/* first field */
				picture_delay = 90000.0/(2.0*m_frame_rate);
			}
			else
			{
				/* second field */
				/* take frame reordering delay into account */
				picture_delay = next_ip_delay - 90000.0/(2.0*m_frame_rate);
			}
		}
		else
		{
			/* frame picture */
			/* take frame reordering delay into account*/
			picture_delay = next_ip_delay;
		}

		if (!m_fieldpic || m_topfirst != (m_pict_struct == TOP_FIELD))
		{
			/* frame picture or second field */
			if (m_prog_seq)
			{
				if (!m_repeatfirst)
                {
					next_ip_delay = 90000.0/m_frame_rate;
                }
				else
				{
					if (!m_topfirst)
                    {
						next_ip_delay = 90000.0*2.0 / m_frame_rate;
                    }
					else
                    {
						next_ip_delay = 90000.0*3.0 / m_frame_rate;
                    }
				}
			}
			else
			{
				if (m_fieldpic)
                {
					next_ip_delay = 90000.0/(2.0*m_frame_rate);
                }
				else
				{
					if (!m_repeatfirst)
                    {
						next_ip_delay = 90000.0*2.0/(2.0*m_frame_rate);
                    }
					else
                    {
						next_ip_delay = 90000.0*3.0/(2.0*m_frame_rate);
                    }
				}
			}
		}
	}

	if (decoding_time==0.0)
	{
		/* first call of calc_vbv_delay */
		/* we start with a 7/8 filled VBV buffer (12.5% back-off) */
		picture_delay = ((m_vbv_buffer_size*16384*7)/8)*90000.0/m_bit_rate;

		if (m_fieldpic)
        {
			next_ip_delay = (int)(90000.0/m_frame_rate+0.5);
        }
	}

	/* VBV checks */

	/* check for underflow (previous picture) */
	if (!m_low_delay && (decoding_time < m_bitcnt_EOP * 90000.0 /m_bit_rate))
	{
		/* picture not completely in buffer at intended decoding time */
		if (!m_quiet)
		{
			fprintf(stderr,"vbv_delay underflow! (decoding_time=%.1f, t_EOP=%.1f\n)",
				decoding_time, m_bitcnt_EOP * 90000.0/m_bit_rate);
		}
	}

	/* when to decode current frame */
	decoding_time += picture_delay;

	/* warning: bitcount() may overflow (e.g. after 9 min. at 8 Mbit/s */
	m_vbv_delay = (int)(decoding_time - bitcount()*90000.0/m_bit_rate);

	/* check for overflow (current picture) */
	if ((decoding_time - (m_bitcnt_EOP * 90000.0) / m_bit_rate)
		> (m_vbv_buffer_size * 16384) * 90000.0 / m_bit_rate)
	{
		if (!m_quiet)
		{
			fprintf(stderr,"vbv_delay overflow!\n");
		}
	}

	fprintf(m_statfile,
		"\nvbv_delay=%d (bitcount=%d, decoding_time=%.2f, bitcnt_EOP=%d)\n",
		m_vbv_delay, bitcount(), decoding_time, m_bitcnt_EOP);

	if (m_vbv_delay < 0)
	{
		if (!m_quiet)
		{
			fprintf(stderr,"m_vbv_delay underflow: %d\n", m_vbv_delay);
		}

		m_vbv_delay = 0;
	}

	if (m_vbv_delay > 65535)
	{
		if (!m_quiet)
		{
			fprintf(stderr, "m_vbv_delay overflow: %d\n", m_vbv_delay);
		}

		m_vbv_delay = 65535;
	}
}
