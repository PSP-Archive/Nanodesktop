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
    ROUTINE PHOENIX GRAPH THREAD (PSP KSU MODE)
    ----------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>


void INTERNAL_ManageDrawMousePointer_NewFrame (void)
/*
    Questa routine si occupa di richiamare una serie di funzionalità
    per la visualizzazione del puntatore del mouse sullo schermo. 
    
    La routine viene richiamata dalla routine DoWindowsRender ogni volta
    che sullo schermo viene disegnato un nuovo frame.
*/
{
   register unsigned short int NotActiveFBPage;
  
   if (HAL_MouseEmuSystemEnabled)
   {
         sceKernelSuspendThread (phnMouse_ThreadID);
         // Sospende l'esecuzione del thread del mouse: questo impedisce che le coordinate
         // del puntatore del mouse possano cambiare durante l'esecuzione del renderer
                           
         NotActiveFBPage=!ActiveFBPage;      // Tutte le operazioni di scrittura vengono eseguite nella pagina che
                                             // in quel momento non è attiva

         ndHAL_GrRegisterMouseBackGr (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, NotActiveFBPage);

         HAL_MouseEmu_OldPosX = HAL_MouseEmu_PosX;
         HAL_MouseEmu_OldPosY = HAL_MouseEmu_PosY;

         ndHAL_GrDrawMousePointer (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, NotActiveFBPage);                         
    
         sceKernelResumeThread (phnMouse_ThreadID);
         // Riattiva il thread Phoenix Mouse
   }
        
}




void INTERNAL_ManageDrawMousePointer_NoNewFrame (void)
/*
    Questa routine si occupa di richiamare una serie di funzionalità
    per la visualizzazione del puntatore del mouse sullo schermo. 
    
    La routine viene richiamata dal Phoenix Graph Thread ogni volta
    che sullo schermo viene disegnato un nuovo frame.
*/
{
  
   if (HAL_MouseEmuSystemEnabled)
   {
         sceKernelSuspendThread (phnMouse_ThreadID);
         // Sospende l'esecuzione del thread del mouse: questo impedisce che le coordinate
         // del puntatore del mouse possano cambiare durante l'esecuzione del renderer
        
         if ( (HAL_MouseEmu_OldPosX!=HAL_MouseEmu_PosX) || (HAL_MouseEmu_OldPosY!=HAL_MouseEmu_PosY) )
         {
             if (!HAL_IsTheFirstTimeForMouse)      // Non è la prima volta per il mouse
             {       
                     if (BackgroundMouseFrame==ndNrVideoFrame)    // La pagina video nel frattempo non è cambiata
                     {
                          ndHAL_GrHideMousePointer (HAL_MouseEmu_OldPosX, HAL_MouseEmu_OldPosY, ActiveFBPage);    
                          // Nascondi il vecchio puntatore del mouse   
                     }          
             }
             
             ndHAL_GrRegisterMouseBackGr (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);
             
             HAL_MouseEmu_OldPosX = HAL_MouseEmu_PosX;
             HAL_MouseEmu_OldPosY = HAL_MouseEmu_PosY;
    
             ndHAL_GrDrawMousePointer (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);    
         }
             
         sceKernelResumeThread (phnMouse_ThreadID);
         // Riattiva il thread Phoenix Mouse
   }                                         
}


static inline void INTERNAL_SpecialKernelResumeThread (int ThreadID)
{
    
    if ((!ND_HWMODE_DisableUsbService) && (ThreadID == ndUsb_CommArea.UsbThread))
    {   
        sceKernelResumeThread (ThreadID);
        sceKernelDelayThread (100000);
    }
    else
    {
        sceKernelWakeupThread (ThreadID);
    }
}



void PhoenixGraph_SERVER (SceSize args, void *argp)
/*
     Questa routine è il server Phoenix Graph. 
     
     Questa routine fa parte di un thread che viene avviato da ndInitSystem 
     all'inizio dell'esecuzione del programma. 
     
     Il thread provvede a verificare istante per istante se vengono fatte 
     richieste da parte del client WindowsRender per l'esecuzione di
     un render delle finestre. 
     
     Il server provvede alle seguenti operazioni:
       a) serializza le richieste di rendering tra i diversi threads;
       b) provvede ad eseguire i vari ndHAL_DoWindowsRender in modo da eseguire
          i vari rendering richiesti
       c) riattiva uno ad uno i vari thread che avevano eseguito la richiesta di
          rendering: si deve ricordare che il client WindowsRender è progettato
          in modo da mettere in sleep temporaneamente il thread in attesa che
          la richiesta di rendering sia stata processata

*/
{
   int Counter;
   int EvResult;
   
   while (!ndInitSystemExecuted)
   {
         sceDisplayWaitVblankStart ();
   }
   
  
   
   /*
       Questo loop viene eseguito all'inizio per assicurarsi che, quando il 
       Phoenix Graph Thread verrà avviato, esso non farà nulla fin quando
       ndCODE_Init non avrà posto ad 1 la variabile ndInitSystemExecuted. 
       
       E' necessario perchè ndInitSystem avvia il Phoenix Graph Thread 
       prima di porre ad 1 la variabile ndInitSystemExecuted: in teoria,
       il sistema potrebbe eseguire un ndHAL_DoWindowsRender prima che
       la variabile ndInitSystemExecuted sia posta ad 1 dal thread 
       principale: la conseguenza sarebbe che il Phoenix Graph Thread
       eseguirebbe un "ndInitSystem parallelo" a quello del thread
       principale, provocando il blocco di sistema
   */
   
   while (!phnGraph_SystemStop)
   {                     
      if ( (phnGraph_WindowsRender_ReqOn) || (phnGraph_UpdWindowsRenderRegs_ReqOn) )                                
      {    // C'è qualche threads che ha richiesto un windowsrender
                              
                              sceKernelWaitEventFlag  (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, &EvResult, 0); 
                              // Attende che il buffer sia libero, dopodichè azzera il flag
  
                              // Ponendo ad 0 questo flag, ci si assicura che nessun altro
                              // thread entri in stato di attesa durante la fase di esecuzione
                              // del server. Questo è necessario perchè altrimenti, qualche
                              // thread potrebbe registrare la propria richiesta durante 
                              // la fase di rendering grafico 
                  
                              if (phnGraph_UpdWindowsRenderRegs_ReqOn)
                              {
                                  ndHAL_UpdateWindowsRenderRegisters_Core ();
                                  phnGraph_UpdWindowsRenderRegs_ReqOn=0;                                   
                              }
                              
                              if (phnGraph_WindowsRender_ReqOn)
                              {
                                  ndHAL_DoWindowsRenderNoSwap ();
                                  // Esegue l'operazione di rendering grafico. Non esegue la commutazione
                                  // del video perchè è necessario gestire prima l'operazione di disegno
                                  // del puntatore del mouse
                                  
                                  INTERNAL_ManageDrawMousePointer_NewFrame ();         
                                  // Provvedi a scrivere il puntatore del mouse nella pagina video in backscreen
                              
                                  ndHAL_ChangeFrameBufferPage();               
                                  // Al termine del processo mostra quanto costruito in off-screen. 
                              
                                  phnGraph_WindowsRender_ReqOn=0;
                                  // Le chiamate grafiche sono state eseguite: ponendo a 0 questo flag si
                                  // reinizializza il server ad accettare nuove chiamate grafiche 
                              }
                              
                              for (Counter=0; Counter<__NR_THREADS_AVAILABLE; Counter++)
                              {
                                  if (phnGraph_Callers [Counter].IsEnabled)
                                  {
                                      INTERNAL_SpecialKernelResumeThread ( phnGraph_Callers [Counter].ThreadID );
                                      
                                      // Risveglia uno alla volta i vari thread che erano stati sospesi perchè
                                      // in attesa dell'esecuzione da parte del server grafico: si noti che anche 
                                      // in caso di thread veloci, essi dovranno comunque attendere che gli altri 
                                      // thread siano tutti risvegliati prima di potere eseguire una nuova
                                      // chiamata al server grafico 
                                  
                                      phnGraph_Callers [Counter].IsEnabled=0; 
                                  }
                              }
                              
                              sceKernelSetEventFlag (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY);
                              // Adesso il server grafico viene posto di nuovo in uno stato in grado
                              // di accettare chiamate: da questo momento, i thread che erano in loop
                              // di attesa interna, escono dal loop, e tentano nuovamente di accedere
                              // al buffer richieste
                                           
      }  
      else   // Non c'è nessun thread che ha richiesto un WindowsRender
      {
             INTERNAL_ManageDrawMousePointer_NoNewFrame(); 
             // Occorre occuparsi solo del puntatore del mouse   
             
             sceDisplayWaitVblankStart ();
             // Necessario per impedire il blocco dello scheduler           
      } 
      
      
   }  
   
}



void ndHAL_LockPhoenixGraphThread (void)
/*
    Esistono casi in cui è necessario sospendere l'attività del Phoenix Graph
    Thread per impedire possibilità di blocco del sistema.
    
    Questa routine provvede a questo.
*/
{
    int EvResult;
    
    if (!ND_SWMODE_DisablePhoenixSubsystem)
    {
        sceKernelWaitEventFlag  (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY, PSP_EVENT_WAITOR, &EvResult, 0); 
        // Attende che il Phoenix Graph thread termini l'eventuale lavoro che è in corso
        
        sceKernelClearEventFlag (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY);
        // Permette di eseguire il lock del thread
    }
}
                                
void ndHAL_UnlockPhoenixGraphThread (void)
/*
    Questa routine provvede a sbloccare il Phoenix Graph Thread
*/
{
    if (!ND_SWMODE_DisablePhoenixSubsystem)
    {
        sceKernelSetEventFlag (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY);
        // Esegue l'unlock del thread                                   
    }
}
 

void ndHAL_StartPhoenixGraphThread (void)
/*
     Provvede ad avviare il thread Phoenix Graph all'avvio del sistema.
     
     Questa routine non ha effetto se l'utente ha disattivato il Phoenix
     SubSystem per mezzo della direttiva #define ND_DISABLE_PHOENIX_SUBSYSTEM
     nel main source del programma
*/
{
   int Counter;
   
   if (!ND_SWMODE_DisablePhoenixSubsystem)
   { 
       phnGraph_WindowsRender_ReqOn  = 0;
       phnGraph_SystemStop           = 0;
       
       for (Counter=0; Counter<__NR_THREADS_AVAILABLE; Counter++)
       {
           phnGraph_Callers [Counter].IsEnabled=0;
       }
       
       phnGraph_Ev0_ReqRegister = sceKernelCreateEventFlag ("Ev0_PhoenixGraph", PSP_EVENT_WAITMULTIPLE, PHNGRAPH_SVR_IS_NOT_BUSY, 0);  
       
       phnGraph_ThreadID = sceKernelCreateThread ("PhoenixGraph", PhoenixGraph_SERVER, 0x08, 32768, THREAD_ATTR_USER, 0);
       sceKernelStartThread (phnGraph_ThreadID, 0, 0);
   }
   else                     
   {
       // Se il Phoenix SubSystem è disattivato, la routine non ha effetto. Viene però posto ad 1
       // il flag che segnala che il sistema di emulazione del mouse è abilitato, in modo da
       // consentire il funzionamento delle routines grafiche che si occupano del disegno/cancellazione
       // del puntatore del mouse. Inoltre, provvediamo ad inizializzare qui alcune variabili: quando
       // il Phoenix Subsystem è operativo, queste inizializzazioni erano fatte da ndHAL_EnableMousePointer
       
       HAL_MouseEmuSystemEnabled = 1;
       
       HAL_MouseEmu_PosX          = Windows_MaxScreenX/2;
       HAL_MouseEmu_PosY          = Windows_MaxScreenY/2;
       HAL_IsTheFirstTimeForMouse = 1;
   }
}



















































































































































