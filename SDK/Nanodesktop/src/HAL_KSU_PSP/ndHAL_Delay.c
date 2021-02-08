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
    ROUTINES HAL PER DELAY (PSP KSU mode)
    ----------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

inline void ndHAL_SoftDelay (float DelaySec)
/*
    Produce un ritardo semplicemente per mezzo di un conteggio software
*/
{
    volatile int BorderUp;
     
    BorderUp = (int)(__DELAY_SCALE*DelaySec);
    
    while (BorderUp>0)
    {
           BorderUp--;
    }
}


inline void ndHAL_HardDelay (float DelaySec)
/*
    Produce un ritardo sfruttando la funzione hardware della PSP
*/
{
     sceKernelDelayThread ( (int)(DelaySec*1000000) );
}


inline void ndHAL_ClockDelay (float DelaySec)
/*
    Produce un ritardo, sincronizzandosi con l'orologio interno della PSP
*/
{
   int InitialClock;
   
   InitialClock   = clock ();
   
   while (1)
   {
         if ((clock () - InitialClock) > (int)( DelaySec*(float)(_CLOCKS_PER_SEC_) )) 
         {
            break;
         }
   }
   
   return;
}


void ndHAL_Delay (float DelaySec)
/*
    Produce un ritardo, scegliendo il metodo di default per la piattaforma usata
*/
{
   ndHAL_HardDelay (DelaySec);
}







// Routines di delay necessarie per il funzionamento della piattaforma

void ndHAL_RequestDelay (unsigned char DelayType)
/*
    Questa routine esegue dei ritardi a seconda della situazione, che viene segnalata
    dei vari componenti del sistema per mezzo del char DelayType che contiene un
    codice NDREQDELAY_FOR xxxx.
    
    Questo ritardo è specifico per la piattaforma utilizzata
*/
{
    switch (DelayType)                            // Ritardi per PSP
    {
           case ND_REQDELAY_FOR_VKB0:             
                ndHAL_HardDelay (0.1);
                break;
           case ND_REQDELAY_MOUSECTRL:
                ndHAL_HardDelay (0.1);
                break;
           case ND_REQDELAY_WINMENUGRAPH:
                ndHAL_SoftDelay (0.1);            // Necessario che sia SoftDelay
                break;
           case ND_REQDELAY_WMI1PRESSED:
                ndHAL_SoftDelay (0.01);
                break;
           case ND_REQDELAY_WMI2PRESSED:
                ndHAL_SoftDelay (0.01);
                break;
           case ND_REQDELAY_CLOSE_VKB:
                ndHAL_HardDelay (0.1);
                break; 
    }
}



