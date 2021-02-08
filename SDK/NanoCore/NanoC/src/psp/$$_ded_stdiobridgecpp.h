/*
   --------------------------------------------------------------------------
   $$_ded_stdiobridgecpp.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   NanoCPP usa una particolare versione delle routines di accesso al disco:
   in pratica, prevede che routines come read, write, lseek ecc. possano
   riconoscere come fd i codici degli streams stdin, stdout, stderr. 
   
   Questo non è il comportamento standard della NanoC. Pertanto, è stato
   realizzato un particolare bridge che ha la funzione di porsi tra
   le routines di accesso al disco di NanoCPP e le routines della NanoC.
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_STDIOCPPBRIDGE_H_
       #define _DED_STDIOCPPBRIDGE_H_

/*
          ---------------------------------------------------------------------
          INCLUSIONE DI ALTRI HEADERS
          ---------------------------------------------------------------------
       */
       
           #include <ctype.h>
           #include <limits.h>
           #include <stdarg.h>
           #include <stdio.h>
           #include <stdlib.h>
           #include <string.h>
           
           // Inclusione di altri header stdio (solo headers)
           #include <psp/$$_ded_stdiocore.h>         
           #include <psp/$$_ded_stdiostream.h>
           #include <psp/$$_ded_stdioprintf.h>

       /*
          ---------------------------------------------------------------------
          PROTOTIPI DI ALTRE FUNZIONI
          ---------------------------------------------------------------------
       */

         #ifdef __cplusplus
         extern "C" 
         {
         #endif

           int brdcpp_open  (const char *NameFile, int flags, int mode);
           int brdcpp_close (int VFHNumber);
           int brdcpp_write (int VFHNumber, const void *Data, int Size);
           int brdcpp_read (int VFHNumber, const void *Data, int Size);
           int brdcpp_fcntl (int VFHNumber, int cmd, ...);
           int brdcpp_seek (int VFHNumber, int Offset, int Whence);
           int brdcpp_fileno (FILE * f);

         #ifdef __cplusplus
         }
         #endif  



#endif  // #ifndef _DED_STDIOCPPBRIDGE_H_

