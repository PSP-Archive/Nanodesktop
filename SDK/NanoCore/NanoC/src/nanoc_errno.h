/*
   --------------------------------------------------------------------------
   nanoc_errno.h               NanoC File Headers
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically
   --------------------------------------------------------------------------
*/


#ifndef _NANOC_ERRNO_H
#define _NANOC_ERRNO_H

         #ifdef PSPE_PLATFORM
                #include <pspe/$$_ded_errno.h>
         #endif
         
         #ifdef PSP_PLATFORM
                #include <psp/$$_ded_errno.h>
         #endif

#endif
                    
