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
    NDISpell Application 1 
    ----------------------------------------------------------------------------
    Nanodesktop is distribuited with ndISpell library. This library contains 
    the code of all ISpell utilities. The nd applications can access to ISpell
    services throught pseudoExec-interface. 
    
    In this example, we show ISpell in non interactive mode. 
    
    ndISpell introduces the new option -k. When you use -k, the system
    checks the input file and it reports the errors in the output
    file defined by the -f option. In other word, the syntax is the
    following:
              
    pseudoExec (&ispell_main, "ispell","-d [language] -k -f [output_file] 
    [input_file]  ", &ErrorLevel); 
    
    IMPORTANT: Create in memory stick a folder called ISpell (ms0:/ISpell) 
    before starting the program. Furthermore, create a subfolder ISpell/lib
    and copy in it the hash files that you can find in $hashfiles. 
    
    The dict and aff files are in $dictionaries: remember that the hash files
    that you can download from internet aren't compatible with this version
    of ndISpell, but you can generate the new hash files from diff/aff files
    using buildhash (see ispell app1 in this distribution) 
    ----------------------------------------------------------------------------
*/



#include <nanodesktop.h>
#include <ndispell.h>

int  ErrorLevel;
   

int ndMain (void)
{
   ndInitSystem ();
   //ndISpell_DisableTerminal ();     // Disable ISpell terminal
     
   pseudoExec (&ispell_main, "ispell","-d italian -k -f ms0:/output.txt input.txt", &ErrorLevel); 
   // Changes the parameters in a way compatible with your needs. 
}

 








