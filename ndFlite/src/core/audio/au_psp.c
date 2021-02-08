/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2000                             */
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
/*               Date:  August 2000                                      */
/*************************************************************************/
/*                                                                       */
/*  Porting to Sony PSP Audio                                            */
/*  Author: Filippo Battaglia                                            */
/*  Date:  August 2007                                                   */
/*                                                                       */
/*************************************************************************/


#include <nanodesktop.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pspaudio.h>
#include <pspaudiolib.h>

#include "cst_string.h"
#include "cst_wave.h"
#include "cst_audio.h"


#define ND_AUDIO_8BIT   8
#define ND_AUDIO_16BIT  16
                                
#define ND_AUDIO_MONO   1
#define ND_AUDIO_STEREO 2
                                    

                                    

cst_audiodev *audio_open_psp(int sps, int channels, cst_audiofmt fmt)
{
    cst_audiodev *ad;
    int channelhdr;
    int nd_nrbits;
    
    switch (fmt)
    {
           case CST_AUDIO_LINEAR8:
           {
                nd_nrbits=ND_AUDIO_8BIT;
                break;
           }
           
           case CST_AUDIO_LINEAR16:
           {
                nd_nrbits=ND_AUDIO_16BIT;
                break;
           }
           
           default:
           {
                cst_errmsg("psp audio: audio format non recognized \n");
	            return NULL;   
           }
    }
    
    channelhdr = ndHAL_SND_OpenChannel (PSP_AUDIO_NEXT_CHANNEL, sps, nd_nrbits, ND_AUDIO_MONO, 100, 100, 0);
    
    if (channelhdr<0)
    {
	   cst_errmsg("psp audio: failed to open audio device. Error %d \n", channelhdr);
	   return NULL;
    }
    
    ad = cst_alloc(cst_audiodev, 1);
    ad->sps = sps;
    ad->channels = channels;
    ad->fmt = fmt;
    ad->platform_data = channelhdr;

    if (ad->channels == 0) ad->channels = 1;

    ad->real_sps = ad->sps;             
    ad->real_channels = 1;              
    ad->real_fmt = ad->fmt;             
    
    return ad;
}

int audio_close_psp(cst_audiodev *ad)
{
    return ndHAL_SND_CloseChannel (ad->platform_data);
}

int audio_write_psp(cst_audiodev *ad, void *samples, int num_bytes)
{
    int conv_factor;
    int sample_written;
    
    // Esegui la riproduzione: Nanodesktop ha bisogno del numero di campioni,
    // mentre FLite ha bisogno del numero di bytes trasferiti. Di conseguenza,
    // è necessario determinare un fattore di conversione, a seconda che la
    // voce sia a 8 o 16 bit. Si tenga presente che l'avere posto di 
    // default a 2 il parametro ad->real_channels, fa sì che il segnale
    // generato da FLite sia sempre stereo, anche se la voce è mono. 
    
    
    
    switch (ad->fmt)
    {
           case CST_AUDIO_LINEAR8:
           {
                conv_factor = 1;
                break;
           }
           
           case CST_AUDIO_LINEAR16:
           {
                conv_factor = 2;
                break;
           }
    }
       
    sample_written = ndHAL_SND_WriteToChannel (ad->platform_data, samples, num_bytes/conv_factor, 1.2);
     
    return sample_written*conv_factor;
}

int audio_flush_psp(cst_audiodev *ad)
{
    //printf ("Audio flush \n");
    return 0;
}

int audio_drain_psp(cst_audiodev *ad)
{
    //printf ("Audio drain \n");
    return 0;
}
