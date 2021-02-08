/* -----------------------------------------------------------------------
   NanoCore Libraries Collection (PSP/PSPE)
   -----------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ------------------------------------------------------------------------
*/   

#ifndef _NANOC_INIT_H_
        #define _NANOC_INIT_H_

         #ifdef PSPE_PLATFORM
                #include <pspe/$$_ded_init.h>
         #endif
         
         #ifdef PSP_PLATFORM
                #include <psp/$$_ded_init.h>
         #endif

#endif

