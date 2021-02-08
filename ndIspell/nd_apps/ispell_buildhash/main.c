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
    NDISpell Application (BuildHash) 
    ----------------------------------------------------------------------------
    Nanodesktop is distribuited with ndISpell library. This library contains 
    the code of all ISpell utilities. The nd applications can access to ISpell
    services throught pseudoExec-interface. 
    
    In this example, we show ISpell that uses the code of buildhash to generate
    a new dictionary. 
    
    IMPORTANT: Create in memory stick a folder called ISpell (ms0:/ISpell) 
    before starting the program. Furthermore, create a subfolder ms0:/ISpell/lib
    in the previous folder.
    
    Modify the call to buildhash to compile the required dict/aff pair of
    files: the generated hash will be stored in ISpell/lib folder
    
    Some dict and aff files are in $dictionaries: remember that the hash files
    that you can download from internet aren't compatible with this version
    of ndISpell, but you can generate the new hash files from diff/aff files
    using buildhash (i.e. this nd application)
    ----------------------------------------------------------------------------
*/



#include <nanodesktop.h>
#include <ndispell.h>

int  ErrorLevel;
   

int ndMain (void)
{
   int  ErrRep;
   
   ndInitSystem ();
     
   pseudoExec (&buildhash_main, "buildhash","ms0:/francais.med ms0:/francais.aff francais.hash", &ErrorLevel); 
   // Compile the med/aff pair of files, in order to create hash file
}

 
