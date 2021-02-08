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
    FUNZIONI DI INTERFACCIAMENTO CON L'OS (PSP CFW mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>

int  ExitThreadID;
int  InhibitRedondantExit = 0;


int ExitThread(SceSize args, void *argp) 
{
  ndLL_ClrScr ();
  ndLL_ClrScr ();
  
  
  ndLL_WriteLn ("Nanodesktop: beginning system halt", COLOR_LBLUE);
  ndLL_CarriageReturn (); 
  
  if (!ND_SWMODE_DisablePhoenixSubsystem)
  {
     // Terminiamo il Phoenix Mouse Thread
     
             ndLL_Write ("Terminating Phoenix Mouse thread ....", COLOR_YELLOW);
                
                phnMouse_SystemStop = 1;
                sceKernelWaitThreadEnd  (phnMouse_ThreadID, 0);
             
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);
             
     
     // Terminiamo il Phoenix Graph Thread
     
             ndLL_Write ("Terminating Phoenix Graph thread ....", COLOR_YELLOW);
             
                phnGraph_SystemStop = 1;
                sceKernelWaitThreadEnd  (phnGraph_ThreadID, 0);
             
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);
  }
  
  // Terminiamo i drivers USB (Eyeserver, GoCam...)
  
  if ((!ND_HWMODE_DisableUsbService) && (!ndUsb_IsFailure))
  {     
             ndLL_Write ("Terminating USB driver thread ....", COLOR_YELLOW);
             
                INTERNAL_TerminateUsbSystem ();
                sceKernelDelayThread (1000000);
             
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);
  }
  
  // Terminiamo i drivers di rete
  
  if (!ND_HWMODE_DisableNetworkService)
  {
             ndLL_Write ("Terminating network drivers ....", COLOR_YELLOW);   
             
                ndHAL_TerminateLowLevelNetwork();
                sceKernelDelayThread (2000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);                     
  }
  
  // Terminiamo il decoder JPEG hardware
  
  if (!ND_HWMODE_DisableMediaService)
  {
             ndLL_Write ("Terminating MJPEG hardware decompressor ..", COLOR_YELLOW);   
             
                INTERNAL_TerminateMJPEGDecoder ();
                sceKernelDelayThread (3000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);                         
  }
  
  // Scarichiamo i codec multimediali
  
  if (!ND_HWMODE_DisableMediaService)
  {
             ndLL_Write ("Terminating A/V modules ....", COLOR_YELLOW);   
             
                INTERNAL_TerminateMedia ();
                sceKernelDelayThread (3000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);                         
  }
  
  // Terminiamo il driver audio
  
  {
             ndLL_Write ("Terminating audio driver ....", COLOR_YELLOW);   
             
                INTERNAL_TerminateAudioDriver ();
                sceKernelDelayThread (2000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);        
  }
                                                               
  // Ritorna al valore standard
  
              ndLL_Write ("System halted ....", COLOR_YELLOW); 
              sceKernelExitGame ();
              
          
  return 0;  
}






void ndHAL_SystemHalt (int ExitCode)
/*
   Questa routine provoca la terminazione immediata dell'homebrew. Sotto PSP
   l'exitcode viene sempre ignorato. 
*/

{
  ndDelay (1);   // Ritardo di 2 secondi: è utile per impedire che il sistema chiuda
                 // troppo velocemente l'interfaccia quando viene eseguito un 
                 // Hello world
  
  /*
      Creiamo un thread in kernel mode apposito per gestire tutta la fase di arresto.
      In questo modo le procedure necessarie non saranno eseguite nel contesto di
      un thread che gira in user mode
  */
  
  if (!InhibitRedondantExit)
  {
          InhibitRedondantExit = 1;
          // Impedisce che altri thread possano eseguire altri system halt in concorrenza
          // con la richiesta del primo thread
  
          ExitThreadID = sceKernelCreateThread ("SYSHLTThread", ExitThread, 0x08, 0x8000, 0, 0);
          sceKernelStartThread  (ExitThreadID, 0, 0);
  }
          
  
  // Fondamentale: fa in modo che il thread che ha richiesto l'arresto non prosegua
  
  sceKernelExitDeleteThread (0);
                
}


/*
   ----------------------------------------------------------------------------
   FUNZIONI DI ARRESTO DEL SISTEMA: EXECEXIT THREAD
   ----------------------------------------------------------------------------
*/

int sctrlKernelLoadExecVSHMs2(const char *file, struct SceKernelLoadExecVSHParam *param);

int ExecExitThread(SceSize args, void *argp) 
{
  ndLL_ClrScr ();
  ndLL_ClrScr ();
  
  ndLL_WriteLn ("Nanodesktop: preparing to load another", COLOR_LBLUE);
  ndLL_WriteLn ("application", COLOR_LBLUE);
  ndLL_CarriageReturn (); 
  
  if (!ND_SWMODE_DisablePhoenixSubsystem)
  {
     // Terminiamo il Phoenix Mouse Thread
     
             ndLL_Write ("Terminating Phoenix Mouse thread ....", COLOR_GRAY);
                
                phnMouse_SystemStop = 1;
                sceKernelWaitThreadEnd  (phnMouse_ThreadID, 0);
             
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);
             
     
     // Terminiamo il Phoenix Graph Thread
     
             ndLL_Write ("Terminating Phoenix Graph thread ....", COLOR_GRAY);
             
                phnGraph_SystemStop = 1;
                sceKernelWaitThreadEnd  (phnGraph_ThreadID, 0);
             
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);
  }
  
  // Terminiamo i drivers USB (Eyeserver, GoCam...)
  
  if ((!ND_HWMODE_DisableUsbService) && (!ndUsb_IsFailure))
  {     
             ndLL_Write ("Terminating USB driver thread ....", COLOR_GRAY);
             
                INTERNAL_TerminateUsbSystem ();
                sceKernelDelayThread (1000000);
             
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);
  }
  
  // Terminiamo i drivers di rete
  
  if (!ND_HWMODE_DisableNetworkService)
  {
             ndLL_Write ("Terminating network drivers ....", COLOR_GRAY);   
             
                ndHAL_TerminateLowLevelNetwork();
                sceKernelDelayThread (2000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);                     
  }
  
  // Terminiamo il decoder JPEG hardware
  
  if (!ND_HWMODE_DisableMediaService)
  {
             ndLL_Write ("Terminating MJPEG hardware decompressor ..", COLOR_GRAY);   
             
                INTERNAL_TerminateMJPEGDecoder ();
                sceKernelDelayThread (3000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);                         
  }
  
  // Scarichiamo i codec multimediali
  
  if (!ND_HWMODE_DisableMediaService)
  {
             ndLL_Write ("Terminating A/V modules ....", COLOR_GRAY);   
             
                INTERNAL_TerminateMedia ();
                sceKernelDelayThread (3000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);                         
  }
  
  // Terminiamo il driver audio
  
  {
             ndLL_Write ("Terminating audio driver ....", COLOR_GRAY);   
             
                INTERNAL_TerminateAudioDriver ();
                sceKernelDelayThread (2000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);        
  }
  
    
    // Esecuzione del nuovo programma                                          
    
                ndLL_CarriageReturn ();
                ndLL_CarriageReturn ();
                
                ndLL_WriteLn ("Waiting: now I'll load the new program:", COLOR_GREEN);
                ndLL_PrintLn (COLOR_WHITE, "%s ", ndTransExecutionFileName);
                
                // Copia i dati nella struttura in modo da preparla all'esecuzione del nuovo homebrew
                memset(&ndLoadExecStdStruct, 0, sizeof(ndLoadExecStdStruct));
                ndLoadExecStdStruct.size = sizeof(ndLoadExecStdStruct)+strlen(ndTransExecutionFileName)+1;
                ndLoadExecStdStruct.args = strlen(ndTransExecutionFileName)+1;
                ndLoadExecStdStruct.argp = ndTransExecutionFileName;
                ndLoadExecStdStruct.key = "game";
                
                // Carica il nuovo file
                sctrlKernelLoadExecVSHMs2 (&ndTransExecutionFileName, &ndLoadExecStdStruct);  
                sceKernelDelayThread (2000000);
                
                return 0;
}

int ndHAL_SystemExec (char *Name)
/*
    Questa routine provvede ad arrestare il sistema ed a caricare un nuovo
    homebrew. Se la routine ritorna un errore in uscita, vuol dire che il
    processo di caricamento è fallito. 
    
    Nella modalità CFW il file da caricare deve essere un PBP
    
    Sono previsti i seguenti codici di errore:
        ERR_FILE_NOT_FOUND             Il file non è stato trovato
        ERR_FUNCTION_NOT_SUPPORTED     La funzione non è supportata   
*/

{
    if (fexist (Name))
    {
        // Copia in un buffer interno il nome del file da eseguire
        strcpy (&ndTransExecutionFileName, Name);
         
        if (!InhibitRedondantExit)
        {
            InhibitRedondantExit = 1;
            // Impedisce che altri thread possano eseguire altri system halt in concorrenza
            // con la richiesta del primo thread
  
            ExitThreadID = sceKernelCreateThread ("SYSHLTThread", ExecExitThread, 0x08, 0x8000, 0, 0);
            sceKernelStartThread  (ExitThreadID, 0, 0);
        }
        
        return 0;
    }
    else 
      return ERR_FILE_NOT_FOUND;  
}
