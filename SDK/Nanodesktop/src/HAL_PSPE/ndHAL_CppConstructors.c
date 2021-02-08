/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2008-2013 Filippo Battaglia 
    ----------------------------------------------------------------------------
    Version for PSP/PSPE
    ----------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

    By downloading, copying, installing or using the software you agree to 
    Nanodesktop license, a modified version of BSD license. Read the file
    license.doc in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    ---------------------------------------------------------------------------
    MANAGER STATICO DEI COSTRUTTORI E DISTRUTTORI GLOBALI PER IL C++
    ---------------------------------------------------------------------------
    Si veda il file <ndenv path>\PSP\SDK\Tools\ndGenCtor\ndGENCTor.c per ulteriori
    informazioni
    ----------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>

extern int ndNoGlobalCtorDtorInit;

// Riferimenti a routines definite nel seguito

void InitializeConstructorMatrix (void);
volatile void __attribute__((used)) CTORMGR_PSPE_do_global_ctors_aux (void);

// Riferimenti extern alla matrice definite nel file ndpspe_ctor_matrix.c
// generato dall'utility CTOR al momento della compilazione

NDEXTERN int RELADR_ndInitSystem; 
NDEXTERN int RELADR_Cpp_Constructors [256]; 
NDEXTERN int RELADR_Cpp_Destructors [256]; 


// Esegui i construttori globali CPP dei vari moduli

void (*PointerToFunc)(void);

void ndHAL_ExecuteCPPGlobalConstructors ()
{
   int Counter;
   
   if (!ndNoGlobalCtorDtorInit)
   {
           InitializeConstructorMatrix ();
           
           for (Counter=0; Counter<256; Counter++)
           {
               if (RELADR_Cpp_Constructors [Counter])
               {
                  PointerToFunc = &ndInitSystem - RELADR_ndInitSystem + RELADR_Cpp_Constructors [Counter];
                  (*PointerToFunc)();                          
               }
           }
   }
}

void ndHAL_ExecuteCPPGlobalDestructors ()
{
   int Counter;
   
   if (!ndNoGlobalCtorDtorInit)
   {   
           for (Counter=0; Counter<256; Counter++)
           {
               if (RELADR_Cpp_Destructors [Counter])
               {
                  PointerToFunc = &ndInitSystem - RELADR_ndInitSystem + RELADR_Cpp_Destructors [Counter];
                  (*PointerToFunc)();                          
               }
           }       
   }
}


