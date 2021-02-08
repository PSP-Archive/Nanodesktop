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
   ---------------------------------------------------------------------------
   CONTROLLO KSU-LOOP
   ---------------------------------------------------------------------------
*/

void ndHAL_KSU_InhibitKSULoop (void)
{
    sceKernelChangeThreadPriority (ndKSUThreadID, KSU_SLEEP_PRIORITY);
    sceKernelSuspendThread (ndKSUThreadID);      
}

void ndHAL_KSU_DeInhibitKSULoop (void)
{
    sceKernelResumeThread (ndKSUThreadID);  
    sceKernelChangeThreadPriority (ndKSUThreadID, ndKSUStdPriority);
}

/*
   ---------------------------------------------------------------------------
   KSU-LOOP
   ---------------------------------------------------------------------------
*/

void ndHAL_KSU_InitLoopServices (void)
/*
    Questa routine provvede a reinizializzare i servizi che compongono il
    LoopService.
*/
{
   // Registra l'ID e la priorità standard del thread KSU
   ndKSUThreadID    = sceKernelGetThreadId ();
   ndKSUStdPriority = sceKernelGetThreadCurrentPriority ();    
    
   // Reinizializza il sistema RawIR. Se c'è un errore, RawIRFailure sarà posto ad un valore
   // diverso da zero
   INTERNAL_Init_RawIrdaSystem();    
} 

   


void ndHAL_KSU_ExecuteLoopServices ()
/*
    Il main kernel thread si pone in un loop infinito, ed inizia a richiamare
    le varie routine server che forniscono i vari servizi. 

    Per interrompere il loop, è necessario porre a 1, per mezzo di un thread,
    la variabile ndServicesLoopExit

    Non è possibile richiamare esplicitamente questa routine: essa viene
    richiamata automaticamente dal sistema operativo quando l'applicazione
    viene compilata con le costanti manifeste ND_PSP_KERNEL_MODE_APPLICATION
    e ND_KERNELSERVICES_TO_USER_MAIN. 
*/
{   

   ndServicesLoopExit=0;
                         
   while (!ndServicesLoopExit)
   {
        ndHAL_KSU_InhibitKSULoop ();
           //  A partire da Nanodesktop 0.4, il loop non viene mantenuto 
           //  costantemente attivo, ma viene sospeso ogni qualvolta esso
           //  non è necessario. Una semplice sospensione con 
           //  sceKernelSuspendThread sembra interferire
           //  con il funzionamento del supporto di rete, pertanto viene
           //  usata una funzione speciale, che contemporaneamente alla
           //  sospensione diminuisce anche la priorità del thread,
           //  portandola ad un valore estremamente basso (definito
           //  dalla costante manifesta KSU_SLEEP_PRIORITY
           //  (di solito è 80) 
           
           //  Poichè KSU è sospeso, è il client che deve provvedere a 
           //  riattivare il thread che contiene il loop KSU in modo
           //  che sia possibile processare la richiesta per i
           //  relativi servizi  
         
        if ((!ND_HWMODE_DisableRawIRService) && (!RawIRFailure))     
        {
           INTERNAL_ndHAL_SendRawIR_SERVER ();
        }
   }    
    
}

