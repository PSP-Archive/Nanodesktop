/* 
   -----------------------------------------------------------------------
   NanoCore Libraries Collection (PSP/PSPE)
   -----------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   -----------------------------------------------------------------------
   Routines di manipolazione stringa per il supporto fs nanodesktop
   -----------------------------------------------------------------------
*/

#ifndef _NANOC_FSSTRINGFUNC_H
#define _NANOC_FSSTRINGFUNC_H

         #ifdef PSPE_PLATFORM
                #include <pspe/$$_ded_fsstrfunc.h>
         #endif
         
         #ifdef PSP_PLATFORM
                #include <psp/$$_ded_fsstrfunc.h>
         #endif


#endif // _NANOC_FSSTRINGFUNC_H
