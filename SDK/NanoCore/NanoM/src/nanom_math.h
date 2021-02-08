/*
   --------------------------------------------------------------------------
   nanom_math.h               NanoM File Headers
   --------------------------------------------------------------------------
   This file is part of NanoM headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically
   --------------------------------------------------------------------------
*/

#ifndef _NANOM_MATH_H
        #define _NANOM_MATH_H

        #ifdef PSPE_PLATFORM
               #include <pspe/$$_ded_math.h>
        #endif
        
        #ifdef PSP_PLATFORM
               #include <psp/$$_ded_math.h>
        #endif
        
#endif

