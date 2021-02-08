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
    ROUTINES HAL PER DELAY (PSPE)
    ----------------------------------------------------------------------------
*/


#include <HAL_PSPE\$_HAL_NDforND.h>


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
    Produce un ritardo sfruttando la funzione hardware della PSP. 
    PSPE non possiede la routine sceKernelDelayThread tipica del
    kernel della PSP, pertanto il suo compito viene eseguito
    da un ritardo software
*/
{
    volatile int BorderUp;
     
    BorderUp = (int)(__DELAY_SCALE*DelaySec);
    
    while (BorderUp>0)
    {
           BorderUp--;
    } 
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
   ndHAL_SoftDelay (DelaySec);
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
    switch (DelayType)                            // Sotto PSPE non sono necessari ritardi
    {
           case ND_REQDELAY_FOR_VKB0:             
                break;
           case ND_REQDELAY_MOUSECTRL:
                break;  
           case ND_REQDELAY_WINMENUGRAPH:
                break; 
           case ND_REQDELAY_WMI1PRESSED:
                break;
           case ND_REQDELAY_WMI2PRESSED:
                break;
           case ND_REQDELAY_CLOSE_VKB:
                break; 
    }
}




