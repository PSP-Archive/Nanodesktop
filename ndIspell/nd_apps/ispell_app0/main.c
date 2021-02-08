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
    NDISpell Application 0 
    ----------------------------------------------------------------------------
    Nanodesktop is distribuited with ndISpell library. This library contains 
    the code of all ISpell utilities. The nd applications can access to ISpell
    services throught pseudoExec-interface. 
    
    In this example, we show ISpell in interactive mode. The user has to choose
    the file using FileManager and after the system starts ISpell. 
    
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

char NameFile [256];
char CLILine  [512];
int  ErrorLevel;
   
extern int ispell_systemhaltonexit;

int ndMain (void)
{
   int  ErrRep;
   
   ndInitSystem ();
   ispell_systemhaltonexit=1;      // Enables normal exit when ISpell quit
     
   ErrRep = FileManager ("ndISpell: choose a file", 0, 0, &NameFile);
   
   if (!ErrRep)
   {
       strcpy (CLILine, "-d francais ");
       strcat (CLILine, NameFile);
       
       pseudoExec (&ispell_main, "ispell", CLILine, &ErrorLevel); 
   }  
   else
   {
       printf ("Operation interrupted by the user \n");
       return -1;
   }
}

 
