/*
   ---------------------------------------------------------------------------------
   nanoc_time.h               NanoC File Headers (PSP/PSPE)
   ---------------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ---------------------------------------------------------------------------------
   ROUTINES DI GESTIONE ORA E DATA
   ---------------------------------------------------------------------------------
*/ 

#ifndef _NANOC_TIME_H
        #define _NANOC_TIME_H
               
        #ifdef PSPE_PLATFORM
                #include <pspe/$$_ded_time.h>        
        #endif
         
        #ifdef PSP_PLATFORM
                #include <psp/$$_ded_time.h>
        #endif


#endif	// _NANOC_TIME_H



















