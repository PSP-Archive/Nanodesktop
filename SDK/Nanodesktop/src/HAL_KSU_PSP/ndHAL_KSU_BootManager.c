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
    FUNZIONI KERNEL TO USER (KSU) (PSP KSU mode)
    BOOT MANAGER
    ---------------------------------------------------------------------------
    Queste funzionalità sono state create per supportare la modalità kernel
    to user
    
    Nella modalità KSU, il sistema avvia un main kernel thread che ha la 
    capacità di accedere alla memoria del kernel. Questo consente di avere
    totale accesso all'hardware e permette di fornire servizi come 
    l'invio di dati in modalità raw-ir. 
    
    In realtà, sarebbe stato meglio un approccio più convenzionale (per esempio,
    un user thread che contiene il codice di Nanodesktop che crea un kernel
    thread per il solo tempo strettamente necessario a fornire le funzionalità
    raw-ir, ma purtroppo si è verificato che questa soluzione non funziona.
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

/*
    --------------------------------------------------------------------------
    BOOT SCREEN
    --------------------------------------------------------------------------
*/

void ndHAL_KSU_BootScreen (void)
{
   int Counter;
   int LenX1, LenX2, LenX3;
   
   // La fase di boot screen ha la funzione di inserire un ritardo all'avvio
   // che talvolta evita dei blocchi per scorrette interazioni di Phoenix
   // Mouse Thread e Phoenix Graph Thread
   
   for (Counter=0; Counter<68; Counter++)
   {
       LenX1 = 0;
       LenX2 = 2*Counter;
       LenX3 = 140-LenX2;
       
       ndLL_DrawRectangle (10 + LenX1, 5, 10 + LenX2, 9, COLOR_WHITE);
       ndLL_DrawRectangle (10 + LenX2, 5, 10 + LenX3, 9, COLOR_LBLUE);
       
       sceKernelDelayThread (400000);
   }  
   
   // Cancella lo schermo a livello Low-level
   
   ndLL_DrawRectangle (5, 1, 220, 30, COLOR_BLACK);
}



/*
   ---------------------------------------------------------------------------
   BOOT MANAGER PER LA MODALITA' KSU
   ---------------------------------------------------------------------------
*/





int ndMain (void);

int UserMainThreadMgr (SceSize args, void *argp)
/*
    Questa routine è la callback del thread UserMainThread, che dovrà contenere il
    codice utente. 
    
    La routine incapsula il codice utente, che viene fornito attraverso la
    routine UserMain. Al termine di quanto contenuto dentro il programma
    utente, viene eseguito automaticamente ndHAL_SystemHalt che esegue 
    l'operazione di arresto del sistema
*/
{  
    ndHAL_KSU_BootScreen ();         // Esegui il boot screen: servirà anche ad acquisire un ritardo di 10 secondi   
    ndMain ();                       // Esegui il programma utente
    
    
    ndHAL_SystemHalt (0);            // Arresta il sistema quando il codice utente è terminato
    return 0;                        // Valore di ritorno del thread UserMain
}



void ndHAL_KSU_BootManager ()
/*
    Questa routine si occupa di gestire l'intero processo di avvio del sistema
    quando si usa il loop KSU
*/

{
      ndInitSystem ();          // Reinizializza il sistema                               
      
      // Queste righe di codice provvedono a creare un thread in user space, ed ad avviarlo.
      // Da quel momento, il kernel thread principale ed il thread in user space proseguiranno
      // insieme l'elaborazione in multithread.
                     
      ndUserMainThreadID = sceKernelCreateThread ("UserMainThread", UserMainThreadMgr, 36, 65535, PSP_THREAD_ATTR_USER | THREAD_ATTR_VFPU, 0);
      sceKernelStartThread (ndUserMainThreadID, 0, 0);     // Da questo momento il codice utente è avviato.          
      
      // Il kernel thread prosegue richiamando i kernel service: si noti che in questo punto
      // l'esecuzione può proseguire solo se il sistema esce dal suo loop KSU. In realtà
      // questo non avviene perchè il loop KSU viene disattivato solo da ndHAL_SystemHalt.     
      
      ndHAL_KSU_ExecuteLoopServices (); 
}
