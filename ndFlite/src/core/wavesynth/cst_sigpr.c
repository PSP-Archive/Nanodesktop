/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2001                            */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  January 2001                                     */
/*************************************************************************/
/*                                                                       */
/*  Signal processing functions                                          */
/*                                                                       */  
/*  Nanodesktop EMI accelerated                                          */
/*                                                                       */
/*************************************************************************/

#include <nanodesktop.h>

#include "cst_math.h"
#include "cst_hrg.h"
#include "cst_wave.h"
#include "cst_sigpr.h"
#include "cst_sts.h"

static float __attribute__((aligned(16))) FASTBUF_FOR_FRAMES [16];
static float __attribute__((aligned(16))) FASTBUF_FOR_LPC_RANGE [16];
static float __attribute__((aligned(16))) FASTBUF_FOR_LPC_MIN [16];

static float __attribute__((aligned(16))) FASTBUF_FOR_INV_65535 [16] =
{      
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0,
       1.0/65535.0
};

static inline float FAST_cst_ulaw_to_float ( unsigned char ulawbyte )
{
    static const int exp_lut[8] = { 0, 132, 396, 924, 1980, 4092, 8316, 16764 };
    register int sign, exponent, mantissa;
    register float sample;

    ulawbyte = ~ ulawbyte;
    sign = ( ulawbyte & 0x80 );
    exponent = ( ulawbyte >> 4 ) & 0x07;
    mantissa = ulawbyte & 0x0F;
    sample = exp_lut[exponent] + ( mantissa << ( exponent + 3 ) );
    if ( sign != 0 ) sample = -sample;

    return sample;
}



cst_wave *lpc_resynth(cst_lpcres *lpcres)
{
    cst_wave *w;
    register int i,j,r,o,k;
    register int ci,cr;
    float *outbuf, *lpccoefs;
    int pm_size_samps;
    float pp = 0;
    
    int NrBlocks, BlockOffset, Counter, IntCounter;
    register float _outbuf_o;
    register float fastreg;

    /* Get a new wave to build the signal into */
    w = new_wave();
    cst_wave_resize(w,lpcres->num_samples * lpcres->residual_fold,1);
    w->sample_rate = lpcres->sample_rate;
    /* outbuf is a circular buffer with past relevant samples in it */
    outbuf = cst_alloc(float,1+lpcres->num_channels);
    /* unpacked lpc coefficients */
    lpccoefs = cst_alloc(float,lpcres->num_channels);
  
  
    
    // PREPARE FOR VFPU PROCESSING
    
    NrBlocks    = lpcres->num_channels / 16;
    BlockOffset = lpcres->num_channels % 16;
        	
   	fastreg = lpcres->lpc_range;   
    for (IntCounter=0; IntCounter<16; IntCounter++) FASTBUF_FOR_LPC_RANGE [IntCounter] = fastreg;
    
    fastreg = lpcres->lpc_min;            
    for (IntCounter=0; IntCounter<16; IntCounter++) FASTBUF_FOR_LPC_MIN [IntCounter] = fastreg;
    
    // Preload of floats in VFPU registers
    
    ndHAL_XFPU_Load16FloatsToMatrix(1,&FASTBUF_FOR_INV_65535);
    ndHAL_XFPU_Load16FloatsToMatrix(2,&FASTBUF_FOR_LPC_RANGE);
    ndHAL_XFPU_Load16FloatsToMatrix(3,&FASTBUF_FOR_LPC_MIN);
                
    // END PREPARE FOR VFPU PROCESSING



    // BEGIN COMPUTATIONS

    for (r=0,o=lpcres->num_channels,i=0; i < lpcres->num_frames; i++)
    {
        	pm_size_samps = lpcres->sizes[i] * lpcres->residual_fold;
        
        /*  ---- REPLACED BY NANODESKTOP VFPU CODE ----------------------------
        	// Unpack the LPC coefficients 
        	for (k=0; k<lpcres->num_channels; k++)
        	{
        	    //lpccoefs[k] = (float)((((double)lpcres->frames[i][k])/65535.0)*
        		//	   lpcres->lpc_range) + lpcres->lpc_min;
            }
            ---- REPLACED BY NANODESKTOP VFPU CODE ----------------------------
        */
        
         //  ---- NANODESKTOP VFPU CODE --------------------------- 
            
        	for (Counter=0; Counter<NrBlocks; Counter++)
        	{
                k=16*Counter;
                
                for (IntCounter=0; IntCounter<16; IntCounter++)
                {
                    FASTBUF_FOR_FRAMES [IntCounter] = (float)lpcres->frames[i][k+IntCounter];
                }
                
                // Caricamento dei dati nelle matrici hardware della VFPU
                
                ndHAL_XFPU_Load16FloatsToMatrix(0,&FASTBUF_FOR_FRAMES);
                
                // Iniziamo i calcoli
                
                ndHAL_XFPU_MatrixTrvMul(0,1,4);
                // (float)lpcres->frames[i][k])/65535.0)
                
                ndHAL_XFPU_MatrixTrvMul(2,4,4);
                // * lpcres->lpc_range
                
                ndHAL_XFPU_MatrixSum(3,4,4);
                // + lpcres->lpc_min
                
                // Calcoli finiti. Adesso bisogna copiare tutto nei registri di destinazione
                ndHAL_XFPU_Store16FloatsFromMatrix(4,&(lpccoefs[k])); 
            }
        	
        	// Standard computations for managing offsets
        	
        	for (IntCounter=0; IntCounter<BlockOffset; IntCounter++)
        	{
                k=16*NrBlocks+IntCounter;
                
                lpccoefs[k] = (float)((((float)lpcres->frames[i][k])/65535.0)*
            			   lpcres->lpc_range) + lpcres->lpc_min;  
            }
            
        // ---- END NANODESKTOP VFPU CODE ---------------------------  
        	
        	/* Note we don't zero the lead in from the previous part */
        	/* seems like you should but it makes it worse if you do */
        /*	memset(outbuf,0,sizeof(float)*(1+lpcres->num_channels)); */
        
        	/* resynthesis the signal */
        	for (j=0; j < pm_size_samps; j++,r++)
        	{
        	    _outbuf_o = FAST_cst_ulaw_to_float (lpcres->residual[r/lpcres->residual_fold]);
        	    cr = (o == 0 ? lpcres->num_channels : o-1);
        	     
                for (ci=0; ci < lpcres->num_channels; ci++)
        	    {
        		    _outbuf_o += lpccoefs[ci] * outbuf[cr];
        		    cr = (cr == 0 ? lpcres->num_channels : cr-1);
        	    }
        	    
        	    w->samples[r] = (short)(_outbuf_o + pp*lpcres->post_emphasis); 
        	    pp = _outbuf_o;
        	    
        	    outbuf [o] = _outbuf_o;
        	    o = (o == lpcres->num_channels ? 0 : o+1);
        	}
    }

    cst_free(outbuf);
    cst_free(lpccoefs);

    return w;

}

cst_wave *lpc_resynth_windows(cst_lpcres *lpcres)
{
    cst_wave *w;
    int i,j,r,o,k;
    int ci,cr;
    float *outbuf, *lpccoefs;
    int pm_size_samps;
    float pp = 0;

    /* Get a new wave to build the signal into */
    w = new_wave();
    cst_wave_resize(w,lpcres->num_samples * lpcres->residual_fold,1);
    w->sample_rate = lpcres->sample_rate;
    /* outbuf is a circular buffer with past relevant samples in it */
    outbuf = cst_alloc(float,1+lpcres->num_channels);
    /* unpacked lpc coefficients */
    lpccoefs = cst_alloc(float,lpcres->num_channels);

    for (r=0,o=lpcres->num_channels,i=0; i < lpcres->num_frames; i++)
    {
	pm_size_samps = lpcres->sizes[i] * lpcres->residual_fold;

	/* Unpack the LPC coefficients */
	for (k=0; k<lpcres->num_channels; k++)
	{
	    lpccoefs[k] = ((float)(((double)lpcres->frames[i][k])/65535.0)*
			   lpcres->lpc_range) + lpcres->lpc_min;
	}
	memset(outbuf,0,sizeof(float)*(1+lpcres->num_channels)); 

	/* resynthesis the signal */
	for (j=0; j < pm_size_samps; j++,r++)
	{
	    outbuf[o] = (float)cst_ulaw_to_short(lpcres->residual[r / lpcres->residual_fold]);
	    cr = (o == 0 ? lpcres->num_channels : o-1);
	    for (ci=0; ci < lpcres->num_channels; ci++)
	    {
		outbuf[o] += lpccoefs[ci] * outbuf[cr];
		cr = (cr == 0 ? lpcres->num_channels : cr-1);
	    }
	    w->samples[r] = (short)(outbuf[o] + pp*lpcres->post_emphasis); 
/*	    w->samples[r] = (short)outbuf[o] - pp*0.95; */
	    pp = outbuf[o];
	    o = (o == lpcres->num_channels ? 0 : o+1);
	}
    }

    cst_free(outbuf);
    cst_free(lpccoefs);

    return w;

}

