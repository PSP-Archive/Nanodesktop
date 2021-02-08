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
    ndMiniZip frontend 
    ----------------------------------------------------------------------------
    This is a simple example for ndMiniZip library. This library allows to
    Nanodesktop to manage the ZIP files. 
    
    In this example, Nanodesktop uses pseudoExec to call the program MiniZip,
    but you also use the library routines directly (see zip.h or unzip.h)
    for further details
    ----------------------------------------------------------------------------
*/



#include <nanodesktop.h>
#include <zip.h>
#include <unzip.h>
   
int minizip_main(int argc, char *argv[]);
int ErrorLevel;



int ndMain (void)
{
   ndInitSystem ();
   
   pseudoExec (&minizip_main, "minizip", "", &ErrorLevel); 
}

 
