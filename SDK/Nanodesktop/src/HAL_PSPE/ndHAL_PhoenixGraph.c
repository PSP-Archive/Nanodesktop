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
    ----------------------------------------------------------------------------
    ROUTINE PHOENIX GRAPH THREAD (PSPE)
    ----------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>

void ndHAL_LockPhoenixGraphThread (void)
{
   // Poichè PSPE non supporta il Phoenix Graph Thread, la routine non ha effetto.  
}
                                
void ndHAL_UnlockPhoenixGraphThread (void)
{
   // Poichè PSPE non supporta il Phoenix Graph Thread, la routine non ha effetto. 
}


void ndHAL_StartPhoenixGraphThread (void)
{
       // Poichè PSPE non supporta il Phoenix Graph Thread, la routine non ha effetto.
       
       // Tuttavia viene però posto ad 1 il flag che segnala che il sistema di emulazione del 
       // mouse è abilitato, in modo da consentire il funzionamento delle routines grafiche 
       // che si occupano del disegno/cancellazione del puntatore del mouse. 
       // Inoltre, provvediamo ad inizializzare qui alcune variabili
       
       HAL_MouseEmuSystemEnabled = 1;
       
       HAL_MouseEmu_PosX          = Windows_MaxScreenX/2;
       HAL_MouseEmu_PosY          = Windows_MaxScreenY/2;
       HAL_IsTheFirstTimeForMouse = 1;
}
