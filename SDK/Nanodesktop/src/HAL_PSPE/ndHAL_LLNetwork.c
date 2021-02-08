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
    FUNZIONI DI GESTIONE DI RETE A BASSO LIVELLO (PSPE)
    ---------------------------------------------------------------------------
 */
 
#include <HAL_PSPE\$_HAL_NDforND.h>




/* 
   --------------------------------------------------------------------------
   FUNZIONI DI INIZIALIZZAZIONE A LIVELLO INTERMEDIO
   --------------------------------------------------------------------------
*/



void ndHAL_InitLowLevelNetwork (void)
/*
     Questa routine si occuperebbe di reinizializzare il sistema WLAN della PSP. 
  
     In realtà, sotto PSPE essa non esegue alcuna funzione, posto
     che in emulazione le funzioni di rete non sono disponibili  
*/
{
     return;    
}

void ndHAL_TerminateLowLevelNetwork (void)
/*
     Questa routine provvede ad arrestare tutti i drivers di rete che sono
     in quel momento in funzione.

     E' usata, ad esempio, in fase di arresto del sistema
*/
{
     return;  
}
