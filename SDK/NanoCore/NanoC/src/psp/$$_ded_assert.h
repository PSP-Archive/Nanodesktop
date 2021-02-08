/*
   --------------------------------------------------------------------------
   $$_ded_assert.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Istruzione assert (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_ASSERT_H_
       #define _DED_ASSERT_H_
       
       
       /*
         -----------------------------------------------------------------------
         DEFINIZIONI DI MACRO NECESSARIE PER IL CORRETTO FUNZIONAMENTO DI ASSERT
         -----------------------------------------------------------------------
       */
       
       #include "_ansi.h"
       #undef assert
   
       #ifdef NDEBUG           /* required by ANSI standard */  
            #define assert(p)  	((void)0)
       #else     
                #ifdef __STDC__
                       #define assert(e)       ((e) ? (void)0 : __assert(__FILE__, __LINE__, #e))
                #else   /* PCC */
                       #define assert(e)       ((e) ? (void)0 : __assert(__FILE__, __LINE__, "e"))
                #endif
       #endif                  /* NDEBUG */
       
       /*
         -----------------------------------------------------------------------
         PROTOTIPI
         -----------------------------------------------------------------------
       */
       
       #ifdef __cplusplus
       extern "C" {
       #endif
            
            void __assert (const char *NameFile, int NrLine, const char *DebugString);
       
       #ifdef __cplusplus
       }
       #endif
       
       
           
#endif // _DED_ASSERT_H_
