/*
   --------------------------------------------------------------------------
   misc0.c                                      NanoC Files (PSP)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines varie
   ----------------------------------------------------------------------------
*/   

#include <stdio.h>

/* 
   ----------------------------------------------------------------------------
   RIFERIMENTI A ROUTINES NANODESKTOP
   ----------------------------------------------------------------------------
*/

    typedef unsigned short int TypeColor;
    
    extern void ndLL_DrawRectangle (unsigned short int RRPosX1, unsigned short int RRPosY1, unsigned short int RRPosX2, unsigned short int RRPosY2, TypeColor Color);
    extern void ndLL_WriteChar (short int ChPosX, short int ChPosY, char Carattere, TypeColor Color); 
    extern void ndLL_GoCursor (short unsigned int PosX, short unsigned int PosY); 
    extern void ndLL_CarriageReturn (void); 
    extern void ndLL_WriteLetter (char Lettera, TypeColor Color); 
    extern void ndLL_Write (char *str, TypeColor color); 
    extern void ndLL_WriteLn (char *str, TypeColor color); 
    extern void ndLL_ClrScr (void); 
    extern void ndLL_WriteCharSpecial (short int PosPixelCharX, short int PosPixelCharY, char Carattere, TypeColor Color, short unsigned int Page); 
    extern void ndLL_PrintLn (TypeColor Color, char *FirstParam, ...); 
    extern void ndLL_Print (TypeColor Color, char *FirstParam, ...); 
    extern char ndLL_SetFont (unsigned char FntHandle); 
     
    extern void ndHAL_SoftDelay  (float DelaySec);
    extern void ndHAL_HardDelay  (float DelaySec); 
    extern void ndHAL_ClockDelay (float DelaySec); 
    extern void ndHAL_Delay (float DelaySec); 
                                 
    void ndSystemHalt (int ExitCode);
    

/*
   ----------------------------------------------------------------------------
   ROUTINES DI INIZIALIZZAZIONE
   ----------------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * init.c
 *
 * Copyright (c) 2002-2004 PS2DEV
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: init.c 1095 2005-09-27 21:02:16Z jim $
   ----------------------------------------------------------------------
*/

void _pspsdk_libc_init()
{
    _pspsdk_alloc_init();
    _pspsdk_stdio_init();
    _pspsdk_stdlib_init();
}


void _pspsdk_libc_deinit()
{
    _pspsdk_stdlib_deinit();
    _pspsdk_stdio_deinit();
    _pspsdk_alloc_deinit();
}


/*
   ----------------------------------------------------------------------------
   ROUTINES DI ARRESTO DEL SISTEMA
   ----------------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * init.c
 *
 * Copyright (c) 2002-2004 PS2DEV
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: init.c 1095 2005-09-27 21:02:16Z jim $
   ----------------------------------------------------------------------
*/


void abort()
{
	printf("Program aborted. \n");
    ndSystemHalt (1);
}

void _Exit (int retval)
{
    ndSystemHalt (retval);
}


/* -----------------------------------------------------------------------------
   Origin: Filippo Battaglia                                        ASSERT
   -----------------------------------------------------------------------------
   Copyright 2006-2011 Filippo Battaglia
   
   IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

   By downloading, copying, installing or using the software you agree to this license.
   If you do not agree to this license, do not download, install,
   copy or use the software.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

     * Redistribution's of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
 
     * Redistribution's in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
  
     * The name of the author may not be used to endorse or promote products
       derived from this software without specific prior written permission.
  
   This software is provided by the copyright holders and contributors "as is" and
   any express or implied warranties, including, but not limited to, the implied
   warranties of merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the author or contributors be liable for any direct,
   indirect, incidental, special, exemplary, or consequential damages
   (including, but not limited to, procurement of substitute goods or services;
   loss of use, data, or profits; or business interruption) however caused
   and on any theory of liability, whether in contract, strict liability,
   or tort (including negligence or otherwise) arising in any way out of
   the use of this software, even if advised of the possibility of such damage.
   -----------------------------------------------------------------------------
*/

#define COLOR_WHITE 0xFFFF


void __assert (const char *NameFile, int NrLine, const char *DebugString)
{
   ndLL_ClrScr ();
   ndLL_CarriageReturn ();
   ndLL_PrintLn (COLOR_WHITE, "CRITICAL ERROR (assert breakpoint)");
   ndLL_PrintLn (COLOR_WHITE, "----------------------------------");
   ndLL_CarriageReturn ();
   ndLL_PrintLn (COLOR_WHITE, "Current application will be terminated.");
   ndLL_CarriageReturn ();  
   ndLL_PrintLn (COLOR_WHITE, "Assert condition in %s line %d", NameFile, NrLine);
   ndLL_CarriageReturn ();
   ndLL_PrintLn (COLOR_WHITE, "Debug string %s ", DebugString);
   ndLL_CarriageReturn ();
   ndLL_PrintLn (COLOR_WHITE, "Reboot will be executed in 20 seconds.");
   ndHAL_SoftDelay (20);
   
   ndSystemHalt (0);    // Arresta il sistema per l'errore critico
}


/*
   ----------------------------------------------------------------------------
   CTYPE POINTER
   ----------------------------------------------------------------------------
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)ctype_.c	5.6 (Berkeley) 6/1/90";
#endif /* LIBC_SCCS and not lint */



#define _CTYPE_DATA_0_127 \
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C, \
	_C,	_C|_S,	_C|_S,	_C|_S,	_C|_S,	_C|_S,	_C,	_C, \
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C, \
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C, \
	_S|_B,	_P,	_P,	_P,	_P,	_P,	_P,	_P, \
	_P,	_P,	_P,	_P,	_P,	_P,	_P,	_P, \
	_N,	_N,	_N,	_N,	_N,	_N,	_N,	_N, \
	_N,	_N,	_P,	_P,	_P,	_P,	_P,	_P, \
	_P,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U, \
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U, \
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U, \
	_U,	_U,	_U,	_P,	_P,	_P,	_P,	_P, \
	_P,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L, \
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L, \
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L, \
	_L,	_L,	_L,	_P,	_P,	_P,	_P,	_C

#define _CTYPE_DATA_128_256 \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0

const char _ctype_[1 + 256] = 
{
	0,
	_CTYPE_DATA_0_127,
	_CTYPE_DATA_128_256
};

const char *__ctype_ptr = _ctype_ + 1;

