/*
    ----------------------------------------------------------------------------
    Nanodesktop Library 
    Copyright 2007-2012 Filippo Battaglia 
    ----------------------------------------------------------------------------
    Version for PSP/PSPE
    ----------------------------------------------------------------------------
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    NDISpell Header File
    ----------------------------------------------------------------------------
    Nanodesktop is distribuited with ndISpell library. This library contains 
    the code of all ISpell utilities. The nd applications can access to ISpell
    services throught pseudoExec-interface. 
    
    In order to do this, the programmer must include this header file in his
    source and after use pseudoExec to call the wanted program
    ----------------------------------------------------------------------------
*/

#ifndef _NDISPELL_LIB_H_
        #define _NDISPELL_LIB_H_

        extern int ispell_main(int argn, char *argv[]);
        extern int buildhash_main (int argc, char *argv[]);
        extern int icombine_main (int argc, char *argv[]);
        extern int ijoin_main (int argc, char *argv[]);
        extern int sq_main (int argc, char *argv[]);
        extern int unsq_main (int argc, char *argv[]);
        
        void ndISpell_DisableTerminal (void);
        void ndISpell_EnableTerminal (void);
        
        
#endif
