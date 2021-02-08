/*
   --------------------------------------------------------------------------
   $$_ded_fsstrfunc.h               NanoC File Headers (PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di manipolazione stringa per il supporto fs nanodesktop (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_FSSTRFUNC_H_
       #define _DED_FSSTRFUNC_H_
       
       /*
          ---------------------------------------------------------------------
          INCLUSIONE DI ALTRI HEADERS
          ---------------------------------------------------------------------
       */
       
          #include <string.h>          
       
       /*
          ---------------------------------------------------------------------
          PROTOTIPI
          ---------------------------------------------------------------------
       */
       
       #ifdef __cplusplus
       extern "C" 
       {
       #endif
            
            void fssAppendExtension (char *NameFile, char *Ext);
            void fssPutSlashAtEnd (char *NameFile);
            void fssStringDate (int day, int month, int year, char *Stringa);
            void fssSplitNameFile (char *NomeCompleto, char *Percorso, char *Nome, char *Ext);
            int  fssIsAbsoluteName (char *LongName);
            int  fssGenerateLongName (char *Path, char *Name, char *Dest);
          
       #ifdef __cplusplus
       }
       #endif
       

#endif
