/*
   ---------------------------------------------------------------------------------
   nanoc_locale.h               NanoC File Headers (PSP/PSPE)
   ---------------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ---------------------------------------------------------------------------------
   ROUTINES DI GESTIONE IMPOSTAZIONI LOCALI
   ---------------------------------------------------------------------------------
*/ 

#ifndef _NANOC_LOCALE_H
        #define _NANOC_LOCALE_H
               
        #ifdef PSPE_PLATFORM
             #include <nanoc_3rdparty/pspe/utf8/include/utf8/nd_locale.h>             
        #endif
         
        #ifdef PSP_PLATFORM
             #include <nanoc_3rdparty/psp/utf8/include/utf8/nd_locale.h>         
        #endif


#endif	// _NANOC_LOCALE_H



















