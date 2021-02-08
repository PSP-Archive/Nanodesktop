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
    FUNZIONI DI INIZIALIZZAZIONE DELLA PIATTAFORMA (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#define HAL_I_NEED_TO_DECLARE_VARIABLES
        #include <HAL_KSU_PSP\$_HAL_NDforND.h>
#undef  HAL_I_NEED_TO_DECLARE_VARIABLES



void ndHAL_InitPlatform_StepA (void)
{
   // Provvede ad impostare la frequenza di clock del processore della PSP a 333 Mhz
   scePowerSetClockFrequency (333, 333, 166);  
   
   ndHAL_InitComponents ();      // Reinizializza la libc ed il gestore di memoria  
   ndHAL_InitFileSystem();       // Inizializza il file system
   ndHAL_InitFirmwareCodes();    // Inizializza i codici firmware 
   ndHAL_InitLowLevelNetwork (); // Carica i drivers a basso livello per la gestione della rete
      
   // Setta i parametri del gamepad
   sceCtrlSetSamplingCycle (0);
   sceCtrlSetSamplingMode (1);
   
   // Disattiva le eccezioni della FPU della PSP
   pspSdkDisableFPUExceptions();
     
   // Reinizializza l'acceleratore video della PSP
   sceGuInit();
   
   ndHAL_InitGraph  ();         // Esegue le routine HAL per l'inizializzazione del sottosistema grafico
   ndHAL_InitIrSystem();        // Reinizializza il sistema ir 
   ndHAL_InitUSBApi();          // Reinizializza l'API USB 
   ndHAL_InitMediaService();    // Inizializza servizi multimediali
   ndHAL_InitAudioApi ();       // Reinizializza l'API per l'audio 
   
   ndHAL_InitExitController (); // Inizializza il controllore di uscita  
}


void ndHAL_InitPlatform_StepB (void)
{
   ndHAL_InitPhoenixMouseControl(); // Reinizializza delle variabili del Phoenix Mouse Thread
   ndHAL_StartPhoenixGraphThread(); // Avvia il thread Phoenix Graph, se non disattivato     
}

void ndHAL_InitPlatform_StepC (void)
{
     // Avvia i servizi specifici per la modalità KSU
     
   ndHAL_KSU_InitLoopServices();                 // Provvede a reinizializzare i servizi del kernel loop service: RawIR 
   INTERNAL_UnlockGraphicalSystemForUSB ();      // Sistema grafico reinizializzato: comunicalo per il driver USB PRX      

   ndHAL_InitPosixEngine ();                     // Inizializza i moduli per compatibilità POSIX
   ndPspKSUSeed=1;
}


