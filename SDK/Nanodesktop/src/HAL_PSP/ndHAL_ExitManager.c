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
    EXIT MANAGER (PSP)
    ---------------------------------------------------------------------------
*/

#include <HAL_PSP\$_HAL_NDforND.h>

/*
   -------------------------------------------------------------------------
   STANDARD CALLBACK EXIT METHOD
   -------------------------------------------------------------------------
*/

// Exit callback 
int ndHAL_SCB_ExitCallback (int arg1, int arg2, void *common) 
{
          sceKernelExitGame();
          return 0;
}

// Callback thread 
int ndHAL_SCB_Thread (SceSize args, void *argp) 
{
          int cbid;

          cbid = sceKernelCreateCallback("Exit Callback", ndHAL_SCB_ExitCallback, NULL);
          sceKernelRegisterExitCallback(cbid);

          sceKernelSleepThreadCB();

          return 0;
}

// Sets up the callback thread and returns its thread id 
int ndHAL_SCB_InitExitController (void) 
{
          int thid = 0;

          thid = sceKernelCreateThread("update_thread", ndHAL_SCB_Thread, 0x10, 0xFA0, 0, 0);
          
          if(thid >= 0) 
          {
                    sceKernelStartThread(thid, 0, 0);
          }

          return thid;
} 




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
          sceKernelDelayThread (24000000);
          sceDisplayWaitVblankStart ();
          
          // Se è stato premuto il tasto HOME avvia immediatamente la procedura
          // di arresto
          
          sceCtrlPeekBufferPositive (&MyPSPPadRecord, 1);
          
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
    ExitController_ThreadID = sceKernelCreateThread ("ExitController", ndHAL_ECT_Thread, 36, 8000, 0, 0);
          
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
    ndHAL_SCB_InitExitController ();
}
