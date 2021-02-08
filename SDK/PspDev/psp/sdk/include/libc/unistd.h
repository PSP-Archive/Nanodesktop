/*
   --------------------------------------------------------------------------
   unistd.h               SDK File Headers
   --------------------------------------------------------------------------
   This file is part of PSPSDK. If the user is compiling using Nanodesktop,
   this file will be replaced by compiler with nanoc_unistd.h
   --------------------------------------------------------------------------
*/

#ifndef __UNISTD_H__
        #define __UNISTD_H__

        #if (defined PSP_PLATFORM || defined PSPE_PLATFORM)     
             #include <nanoc_unistd.h>
        #else

                /*
                 * PSP Software Development Kit - http://www.pspdev.org
                 * -----------------------------------------------------------------------
                 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
                 *
                 * unistd.h
                 *
                 * Copyright (c) 2002-2004 PS2DEV
                 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
                 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
                 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
                 *
                 * $Id: unistd.h 1095 2005-09-27 21:02:16Z jim $
                 */
                
                #include <stdio.h>
                
        #endif         

#endif
