/*
   --------------------------------------------------------------------------
   nanoc_malloc.h               NanoC File Headers (PSP/PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   -----------------------------------------------------------------------
*/   

/* ----------------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * malloc.h
 *
 * Copyright (c) 2002-2004 PS2DEV
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: malloc.h 1095 2005-09-27 21:02:16Z jim $
 *
 * ----------------------------------------------------------------------------
 */



#ifndef _NANOC_MALLOC_H
#define _NANOC_MALLOC_H

#include <pspkernel.h>
#include <stddef.h>
#include <stdarg.h>
#include <errno.h>

         #ifdef PSPE_PLATFORM
                #include <pspe/$$_ded_malloc.h>
         #endif
         
         #ifdef PSP_PLATFORM
                #include <psp/$$_ded_malloc.h>
         #endif

#endif	// _NANOC_MALLOC_H

