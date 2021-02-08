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
    FUNZIONI DI INTERFACCIAMENTO CON L'OS (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>


int  ExitThreadID;
int  InhibitRedondantExit = 0;


/*
   ----------------------------------------------------------------------------
   FUNZIONI DI ARRESTO DEL SISTEMA: EXIT THREAD
   ----------------------------------------------------------------------------
*/



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
  
  // Terminiamo il thread relativo al driver USB
  
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
     
  // Terminiamo il driver audio
  {
             ndLL_Write ("Terminating audio driver ....", COLOR_YELLOW);   
             
                pspAudioEnd ();
                sceKernelDelayThread (2000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);        
  }
       
  // Provvediamo ad interrompere il loop KSU
                                            
              ndLL_Write ("Waiting for KSU loop natural end ....", COLOR_YELLOW);
              
                  ndServicesLoopExit=1;
                  ndHAL_KSU_DeInhibitKSULoop ();
                  sceKernelDelayThread (2000000);
                  
              ndLL_GoCursor (50, LL_PosCursY); 
              ndLL_WriteLn ("Done", COLOR_GREEN); 
              
              ndLL_Write ("Terminating KSU loop thread ....", COLOR_YELLOW);
              
                  sceKernelTerminateDeleteThread  (ndKSUThreadID);
                  
              ndLL_GoCursor (50, LL_PosCursY); 
              ndLL_WriteLn ("Done", COLOR_GREEN); 
              
  // Provvediamo ad interrompere il thread UserMain
  
              ndLL_Write ("Terminating user main thread ....", COLOR_YELLOW);
              
                  sceKernelTerminateDeleteThread  (ndUserMainThreadID);
                  sceKernelDelayThread (1000000);
                  
              ndLL_GoCursor (50, LL_PosCursY);  
              ndLL_WriteLn ("Done", COLOR_GREEN); 
  
  // Arresta il sistema

             ndLL_Write ("System halted ....", COLOR_YELLOW); 
             sceKernelExitGame ();        

  return 0;  
}

/*
   ----------------------------------------------------------------------------
   FUNZIONI DI ARRESTO DEL SISTEMA: EXECEXIT THREAD
   ----------------------------------------------------------------------------
*/

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
  
  // Terminiamo il thread relativo al driver USB
  
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
  
  // Terminiamo il driver audio
  {
             ndLL_Write ("Terminating audio driver ....", COLOR_GRAY);   
             
                pspAudioEnd ();
                sceKernelDelayThread (2000000);  
               
             ndLL_GoCursor (50, LL_PosCursY); 
             ndLL_WriteLn ("Done", COLOR_GREEN);        
  }
  
       
  // Provvediamo ad interrompere il thread UserMain
  
              ndLL_Write ("Terminating user main thread ....", COLOR_GRAY);
              
                  sceKernelTerminateDeleteThread  (ndUserMainThreadID);
                  sceKernelDelayThread (1000000);
                  
              ndLL_GoCursor (50, LL_PosCursY);  
              ndLL_WriteLn ("Done", COLOR_GREEN); 
    
    // Esecuzione del nuovo programma                                          
    
                ndLL_CarriageReturn ();
                ndLL_CarriageReturn ();
                
                ndLL_WriteLn ("Waiting: now I'll load the new program:", COLOR_GREEN);
                ndLL_PrintLn (COLOR_WHITE, "%s ", ndTransExecutionFileName);
                
                ndTransExecutionRequested=1;           // Richiede la transesecuzione del codice per il loop KSU 
                
                // Siccome ndTransExecutionRequested è attivato, l'uscita dal loop
                // KSU farà sì che il sistema esegua il nuovo homebrew    
                
                ndServicesLoopExit=1;
                ndHAL_KSU_DeInhibitKSULoop ();
                
                return 0;  
}


/*
   ----------------------------------------------------------------------------
   API AD ALTO LIVELLO
   ----------------------------------------------------------------------------
*/


void ndHAL_SystemHalt (int ExitCode)
/*
   Questa routine provoca la terminazione immediata dell'homebrew. Sotto PSP
   l'exitcode viene sempre ignorato. 
*/

{
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


int ndHAL_SystemExec (char *Name)
/*
    Questa routine provvede ad arrestare il sistema ed a caricare un nuovo
    homebrew. Se la routine ritorna un errore in uscita, vuol dire che il
    processo di caricamento è fallito. 
    
    Nella modalità KSU il file da caricare deve essere un ELF (firmware 1.5)
    
    Sono previsti i seguenti codici di errore:
        ERR_FILE_NOT_FOUND             Il file non è stato trovato
        ERR_FUNCTION_NOT_SUPPORTED     La funzione non è supportata   
*/

{
    if (fexist (Name))
    {
        // Copia in un buffer interno il nome del file da eseguire
        strcpy (&ndTransExecutionFileName, Name);
        
        // Copia i dati nella struttura in modo da preparla all'esecuzione del nuovo homebrew
        memset(&ndLoadExecStdStruct, 0, sizeof(ndLoadExecStdStruct));
        ndLoadExecStdStruct.size = sizeof(ndLoadExecStdStruct)+strlen(ndTransExecutionFileName)+1;
        ndLoadExecStdStruct.args = strlen(ndTransExecutionFileName)+1;
        ndLoadExecStdStruct.argp = ndTransExecutionFileName;
        ndLoadExecStdStruct.key = "game";
        
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

void ndHAL_SignalTroublesInKernelExtender (void)
{
     /* Nothing to do on this platform */
}
