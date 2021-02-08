/*
   --------------------------------------------------------------------------
   $$_ded_misc0.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines varie (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_MISC0_H_
       #define _DED_MISC0_H_
       
       #ifdef __cplusplus
       extern "C" 
       {
       #endif
       
           extern void _pspsdk_libc_init (void); 
           extern void _pspsdk_libc_deinit (void); 
           extern void abort (void); 
           extern void _Exit (int retval); 
           extern void __assert (const char *NameFile, int NrLine, const char *DebugString); 
          
       #ifdef __cplusplus
       }
       #endif
       
#endif
