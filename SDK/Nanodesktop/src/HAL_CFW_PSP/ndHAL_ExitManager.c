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
    EXIT MANAGER (PSP CFW mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>




/*
   -------------------------------------------------------------------------
   EXIT CUSTOM THREAD MODE (ECT Mode)
   -------------------------------------------------------------------------
*/

static SceCtrlData MyPSPPadRecord;

int ndHAL_ECT_Thread (SceSize args, void *argp) 
{ 
    while (!ExitController_Exit)
    {      
          sceKernelDelayThread (15000000);
          sceDisplayWaitVblankStart ();
          
          // Se viene premuto il tasto HOME, arresta il sistema. Bisogna passare
          // obbligatoriamente per una funzione del kernel extender, perchè
          // altrimenti il tasto HOME non viene rilevato
          
          KRNEXT_PRX_sceCtrlReadBufferPositiveDetectHome (ndPspFwEmuVersion, &MyPSPPadRecord, 1);
          
          if ((MyPSPPadRecord.Buttons & PSP_CTRL_HOME) == PSP_CTRL_HOME)
          {
              ndHAL_SystemHalt (0);
          }
    } 
    
    return 0; 
}


void ndHAL_ECT_InitExitController (void)
{
    ExitController_Exit     = 0;
    ExitController_ThreadID = sceKernelCreateThread ("ECTThread", ndHAL_ECT_Thread, 0x08, 0x4000, 0, 0);
          
    if (ExitController_ThreadID >= 0) 
    {
       sceKernelStartThread (ExitController_ThreadID , 0, 0);
    }
}


/*
   -------------------------------------------------------------------------
   INIZIALIZZAZIONE EXIT MANAGER
   -------------------------------------------------------------------------
*/

void ndHAL_InitExitController (void)
{
    /*
        Si è verificato che anche in modalità custom firmware, purtroppo, non
        è possibile usare la normale ExitCallback, ma deve essere *SEMPRE* usata una
        tecnica differente, basata su un thread custom di uscita.
        
        Il thread gestito dall'exit manager usa una funzione del kernel
        extender, pertanto non può essere usata se il kernel extender
        è in errore
    */
 
    if (!ndKernelExtenderIsInError) ndHAL_ECT_InitExitController ();
}
