/*
   --------------------------------------------------------------------------
   $$_ded_stdioscanf.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines per esecuzione pseudo-main program   (header)
   ----------------------------------------------------------------------------
*/   


#ifndef _DED_STDIOEXEC_H_
       #define _DED_STDIOEXEC_H_

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

           #ifdef __cplusplus
           extern "C" 
           {
           #endif

        /*
           --------------------------------------------------------------------
           COSTANTI MANIFESTE DI COMPILAZIONE
           --------------------------------------------------------------------
        */
        
        #ifdef DED_STDIOEXEC_I_NEED_VARIABLES
                #define STDIOEXEC_EXTERN
        #else
                #define STDIOEXEC_EXTERN extern
        #endif
        
        
        /*
           --------------------------------------------------------------------
           SYMBOLS
           --------------------------------------------------------------------
        */
                
        #ifdef DED_STDIOEXEC_I_NEED_VARIABLES
               int opterr = 1;
        #else
               extern int opterr;
        #endif
        
        #ifdef DED_STDIOEXEC_I_NEED_VARIABLES
               int optind = 1;
        #else
               extern int optind;
        #endif
        
        STDIOEXEC_EXTERN int optopt;
        STDIOEXEC_EXTERN char *optarg;

       /* 
          -----------------------------------------------------------------------------------------
          CONFIGURAZIONE DEI PARAMETRI PSEUDOEXEC
          -----------------------------------------------------------------------------------------
       */ 
        
        // PseudoExec
        
        #define ND_NR_PARAMS_PASSABLE_TO_PSEUDOEXEC                  20
        #define ND_MAXLEN_OF_PARAM_PASSED_TO_PSEUDOEXEC             256
        #define ND_MAXLEN_OF_ENVSTRING_OF_PSEUDOEXEC               1024
        
        struct pseudoExec_ArrayOfTheStrings_Type
        {
           char Str [ND_NR_PARAMS_PASSABLE_TO_PSEUDOEXEC][ND_MAXLEN_OF_PARAM_PASSED_TO_PSEUDOEXEC];   
        };
        
        struct pseudoExec_ArrayOfThePointersToTheStrings_Type
        {
           void *PntToStr [ND_NR_PARAMS_PASSABLE_TO_PSEUDOEXEC];   
        };
                                    
        /* 
          -----------------------------------------------------------------------------------------
          PROTOTIPI
          -----------------------------------------------------------------------------------------
       */                              

        extern int pseudoExec (void *MainRoutine, char *NameProgram, char *EnvStr, int *ErrorLevel);                            
        extern int getopt( int argc, char *argv[], const char *opts );
        
        unsigned sleep(unsigned seconds);

        #ifdef __cplusplus
        }
        #endif
        


       
#endif
