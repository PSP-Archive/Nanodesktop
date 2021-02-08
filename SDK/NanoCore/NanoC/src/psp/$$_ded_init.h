/*
   --------------------------------------------------------------------------
   $$_ded_init.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Inizializzazione (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_INIT_H_
#define _DED_INIT_H_

        #ifdef __cplusplus
        extern "C" 
        {
        #endif
        
        void _pspsdk_libc_init();        
        void _pspsdk_libc_deinit();
        
        #ifdef __cplusplus
        }
        #endif

       
#endif
