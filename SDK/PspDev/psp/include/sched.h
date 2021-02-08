/*
 *  Written by Joel Sherrill <joel@OARcorp.com>.
 *
 *  COPYRIGHT (c) 1989-2000.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Permission to use, copy, modify, and distribute this software for any
 *  purpose without fee is hereby granted, provided that this entire notice
 *  is included in all copies of any software which is or includes a copy
 *  or modification of this software.
 *
 *  THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 *  WARRANTY.  IN PARTICULAR,  THE AUTHOR MAKES NO REPRESENTATION
 *  OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY OF THIS
 *  SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 *  $Id: sched.h,v 1.2 2002/06/20 19:51:24 fitzsim Exp $
 */


#ifndef __POSIX_SCHEDULING_H
#define __POSIX_SCHEDULING_H

#if (defined PSPE_PLATFORM || defined PSP_PLATFORM)     
            #include <nanoc_sched.h>
#else 
            /* Niente da fare: questo file non era previsto nella versione standard
               dell'SDK */
#endif






#endif  // __POSIX_SCHEDULING_H
