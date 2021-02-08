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
    GESTORI MOUSECONTROL, EVENT MANAGER, WMI EVENT MANAGER (PSP KSU mode)
    VERSIONE PHOENIX COMPATIBILE
    
    Questo file contiene le routines MouseControl, Event Manager, WMI Event
    Manager compatibili con il sistem Phoenix Thread
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

/*
    *************************************************************************** 
    ***************************************************************************
                          DISPATCHER DEI MESSAGGI (Phoenix compatible)
    ***************************************************************************
    ***************************************************************************
    Questa sezione provvede alla consegna dei messaggi ai processi in attesa
    ***************************************************************************
*/

void INTERNAL_PHOENIX_DispatchMessage (struct ndEvent_Type *MyEvent)
{
    int Counter;
    int EvResult;
    
    sceKernelWaitEventFlag  (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE, PSP_EVENT_WAITOR, &EvResult, 0); 
    
    sceKernelClearEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
    // Impedisce che altri thread possano scrivere nel buffer eventi
                                     
    for (Counter=0; Counter<__NR_MOUSECTRL_RQ_ALLOWED; Counter++)
    {
        if (  (phnMouse_ReqBufferStdArray [Counter].IsEnabled) && 
             !(phnMouse_ReqBufferStdArray [Counter].IsWaiting)  )
        {
                *(phnMouse_ReqBufferStdArray [Counter].AddressData) = *MyEvent;
                // Copia il contenuto del buffer eventi dentro la locazione appositamente creata
                // dal thread chiamante
                
                phnMouse_ReqBufferStdArray [Counter].IsEnabled=0;
                  
                sceKernelWakeupThread (phnMouse_ReqBufferStdArray [Counter].ThreadID_AuthorOfRequest); 
                // Riattiva il thread chiamante
        }
    }
        
    sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
    // Riattiva la possibilità di accettare altre richieste per il buffer eventi   
}


/*
    *************************************************************************** 
    ***************************************************************************
                             REGISTRO CBT (Phoenix compatible)
    ***************************************************************************
    ***************************************************************************
    Questa sezione contiene le routines relative al registro CBT
    ***************************************************************************
*/


#define CBE_BUTTON    0
#define CBE_CLOSEWND  1
#define CBE_WMI1      2
#define CBE_WMI2      3



void INTERNAL_PHOENIX_RegisterNewCBE (int CBEThread, char TypeThread)
{
    int   Counter, CBEFound;
    char  IHaveFound;
    
    int   EvResult;
    
     
    TryToRegister:
                  
    sceKernelWaitEventFlag  (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE, PSP_EVENT_WAITOR, &EvResult, 0); 
    
    sceKernelClearEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
    // Impedisce che altri thread possano scrivere nel buffer eventi
                
    IHaveFound=0; 
    Counter   =0;
      
    while (Counter<__NR_MOUSECTRL_CBE_ALLOWED)
    {
           if (!phnMouse_CbeRegister [Counter].IsEnabled)
           {
                // Trovato l'handle
                
                phnMouse_CbeRegister [Counter].ThreadID  = CBEThread;
                phnMouse_CbeRegister [Counter].IsEnabled = 1;
           
                // Registra
                CBEFound=Counter;
                IHaveFound=1;
                
                break;                           
           }
           
           Counter++;
    } 
    
    sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
    // Riattiva la possibilità di accettare altre richieste per il buffer eventi   
    
    if (!IHaveFound)         // Non è stato possibile registrare il nuovo CBE
    {
        sceKernelDelayThread (250000);      // Un ritardo di 0.25 sec. permette al sistema di smistare l'eccesso di CBE
        goto TryToRegister;
    }
        
}

void INTERNAL_PHOENIX_UnRegisterCBE (int CBEThread)
{
    int  Counter = 0;
    int  EvResult;
     
    sceKernelWaitEventFlag  (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE, PSP_EVENT_WAITOR, &EvResult, 0); 
    
    sceKernelClearEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
    // Impedisce che altri thread possano scrivere nel buffer eventi
     
    while (Counter<__NR_MOUSECTRL_CBE_ALLOWED)
    {
           if ( (phnMouse_CbeRegister [Counter].IsEnabled) && (phnMouse_CbeRegister [Counter].ThreadID==CBEThread) )
           {
                // Trovato l'handle. Disallochiamolo
                
                phnMouse_CbeRegister [Counter].IsEnabled = 0;
                break;                          
           }
           
           Counter++;
    }  
    
    sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
    // Riattiva la possibilità di accettare altre richieste per il buffer eventi   
}


void INTERNAL_PHOENIX_LockForNewCBE (int CBEThread)
{
   int Counter;
   int EvResult;
   
   sceKernelWaitEventFlag  (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE, PSP_EVENT_WAITOR, &EvResult, 0); 
    
   sceKernelClearEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
   // Impedisce che altri thread possano scrivere nel buffer eventi
    
   for (Counter=0; Counter<__NR_MOUSECTRL_RQ_ALLOWED; Counter++)
   {
          if (  (phnMouse_ReqBufferStdArray [Counter].IsEnabled) &&
               !(phnMouse_ReqBufferStdArray [Counter].IsWaiting) )
          {
               // Esegue l'inibizione del thread. Finchè non viene completato
               // il CBE i thread che stanno attendendo una risposta da parte
               // di MouseControl devono essere inibiti, in modo che non 
               // rispondano alle richieste di MouseControl
          
               phnMouse_ReqBufferStdArray [Counter].IsWaiting      = 1;
               phnMouse_ReqBufferStdArray [Counter].WaitForCBEEnd  = CBEThread;
          }
   } 
   
   sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
    // Riattiva la possibilità di accettare altre richieste per il buffer eventi  
   
}

void INTERNAL_PHOENIX_UnLockForCBE (int CBEThread)
{
   int Counter;
   int EvResult;
   
   sceKernelWaitEventFlag  (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE, PSP_EVENT_WAITOR, &EvResult, 0); 
    
   sceKernelClearEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
   // Impedisce che altri thread possano scrivere nel buffer eventi
   
   for (Counter=0; Counter<__NR_MOUSECTRL_RQ_ALLOWED; Counter++)
   {
          if (  (phnMouse_ReqBufferStdArray [Counter].IsEnabled) &&
                (phnMouse_ReqBufferStdArray [Counter].IsWaiting) &&
                (phnMouse_ReqBufferStdArray [Counter].WaitForCBEEnd == CBEThread)  )
          {
               // Esegue la deinibizione del thread. In tal modo, i dispatch
               // successivi dei messaggi riguarderanno anche i thread che
               // precedentemente erano stati inibiti
                
               phnMouse_ReqBufferStdArray [Counter].IsWaiting      = 0;
          }
   } 
   
   sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
    // Riattiva la possibilità di accettare altre richieste per il buffer eventi  
         
}

void INTERNAL_PHOENIX_SuspendAllCBEThreads (void)
/*
   Provvede a sospendere tutti i threads CBE
*/
{
   int Counter;
   int EvResult;
   
   sceKernelWaitEventFlag  (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE, PSP_EVENT_WAITOR, &EvResult, 0); 
    
   sceKernelClearEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
   // Impedisce che altri thread possano scrivere nel buffer eventi
   
   for (Counter=0; Counter<__NR_MOUSECTRL_CBE_ALLOWED; Counter++)
   {
          if   (phnMouse_ReqBufferStdArray [Counter].IsEnabled) 
          {
               sceKernelSuspendThread ( phnMouse_CbeRegister [Counter].ThreadID );    
          }
   } 
   
   sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
   // Riattiva la possibilità di accettare altre richieste per il buffer eventi  
}

void INTERNAL_PHOENIX_ResumeAllCBEThreads (void)
/*
   Provvede a ripristinare tutti i threads CBE
*/
{
   int Counter;
   int EvResult;
   
   sceKernelWaitEventFlag  (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE, PSP_EVENT_WAITOR, &EvResult, 0); 
    
   sceKernelClearEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
   // Impedisce che altri thread possano scrivere nel buffer eventi
   
   for (Counter=0; Counter<__NR_MOUSECTRL_CBE_ALLOWED; Counter++)
   {
          if   (phnMouse_ReqBufferStdArray [Counter].IsEnabled) 
          {
               sceKernelResumeThread ( phnMouse_CbeRegister [Counter].ThreadID );    
          }
   } 
   
   sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
   // Riattiva la possibilità di accettare altre richieste per il buffer eventi           
} 



/*
    *************************************************************************** 
    ***************************************************************************
                          CALLBACK EXECUTOR (Phoenix compatible)
    ***************************************************************************
    ***************************************************************************
    Il Phoenix Mouse Thread non può eseguire direttamente le callbacks, perchè
    il codice di ogni callback in teoria potrebbe eseguire un MouseControl_CLIENT.
    
    Siccome il client MouseControl pone in suspend il thread che in quel momento
    è in esecuzione, questo significherebbe che il Phoenix Mouse Thread sospenderebbe
    sè stesso (bloccando il sistema). 
    
    Per ovviare a questo, il Phoenix Mouse Thread si limita a lanciare un thread
    a parte che esegue tutto il codice della callback: questo thread può rilanciare
    una nuova sessione del client MouseControl, senza influenzare con il proprio
    comportamento il Phoenix Mouse Thread che procede indisturbato.
    ***************************************************************************
*/

static int TCBCounter = 0;



// -------------------- Sezione relativa alle callbacks di pulsante  --------------- 

struct phnMouse_PacketForCBEBTN_Type
{
    void     *ButtonCallback;
    char     *StringID;
    ndint64  InfoField;
    char     WindowFoundID; 
    char     ButtonFoundID;
    char     TraspCallback;   
    int      CBE_ThreadID;       
};                     

static void INTERNAL_PHOENIX_ExecuteButtonCallback_L2 (SceSize args, void *argp)
{
    struct ndEvent_Type MyEvent;
    ndint64 __RetValue;
    
    // Recupera l'indirizzo del pacchetto
    struct phnMouse_PacketForCBEBTN_Type *NewPacketForCBE = argp;
    
    // Puntatore alla callback
    ndint64 (*PntToButtonCallback)(char *StringID, ndint64 InfoField, char WndHandle);
    
    // Inizio del codice 
   
    PntToButtonCallback = NewPacketForCBE->ButtonCallback;
    
    __RetValue = (*PntToButtonCallback)(   NewPacketForCBE->StringID, NewPacketForCBE->InfoField, 
                                           NewPacketForCBE->WindowFoundID    );
                                           
    // Dopo l'esecuzione della callback, provvedi ad unlockare i thread in attesa, in modo
    // che i messaggi vengano recapitati anche ai thread che attendevano la fine del CBE 
    
    INTERNAL_PHOENIX_UnLockForCBE (NewPacketForCBE->CBE_ThreadID);
    
    // Dopo l'esecuzione della callback, deve verificare se è necessario usare il dispatcher
    // per fornire un messaggio ai threads in attesa
        
    if (!NewPacketForCBE->TraspCallback)
    {
         MyEvent.__TypeEvent = CUSTOM_BUTTON_PRESSED;
         MyEvent.__WindowID = NewPacketForCBE->WindowFoundID;
         MyEvent.__ButtonID = NewPacketForCBE->ButtonFoundID;
         strcpy ( &(MyEvent.__StringID), NewPacketForCBE->StringID );
         MyEvent.__CallbackExecuted  =1;
         MyEvent.__CallbackAnswer    =__RetValue;
         strcpy ( &(MyEvent.__ZString), "");
         
         INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                         
    }
    
    INTERNAL_PHOENIX_UnRegisterCBE (NewPacketForCBE->CBE_ThreadID);   // Deregistra il thread CBE
    free (NewPacketForCBE);                                     // Disalloca il pacchetto
    
    sceKernelExitDeleteThread  (0);                                   // Esce dal thread e lo cancella
    
    return;
}

static inline void INTERNAL_PHOENIX_ExecuteButtonCallback_L1 (void *ButtonCallback, char *StringID, ndint64 InfoField, 
                                                       char WindowFoundID, char ButtonFoundID, char TraspCallback)
{
    struct phnMouse_PacketForCBEBTN_Type *NewPacketForCBE;
    char   nameofTCBThread [200];
    char   nameofTCBThread0 [32];
    int    ThreadID;
    int    SizePacket;
    
    // Iniziamo eseguendo una copia del request buffer
    
    SizePacket      = sizeof ( struct phnMouse_PacketForCBEBTN_Type );
    NewPacketForCBE = malloc  ( SizePacket );
    
    if (NewPacketForCBE)     // L'allocazione dinamica è riuscita
    {
         // Prepara il pacchetto
         
         NewPacketForCBE->ButtonCallback = ButtonCallback;
         NewPacketForCBE->StringID       = StringID;
         NewPacketForCBE->InfoField      = InfoField;
         NewPacketForCBE->WindowFoundID  = WindowFoundID;
         NewPacketForCBE->ButtonFoundID  = ButtonFoundID;
         NewPacketForCBE->TraspCallback  = TraspCallback;
         
         // Prepara il nuovo thread CBE
         
         TCBCounter++;
         if (TCBCounter==2000) TCBCounter=0;
         
         strcpy (&nameofTCBThread, "CBEBTN");
         _itoa (TCBCounter, &nameofTCBThread0, 10);
         strcat (&nameofTCBThread, &nameofTCBThread0);
         
         ThreadID=sceKernelCreateThread (&nameofTCBThread, &INTERNAL_PHOENIX_ExecuteButtonCallback_L2,
                                         33, 16384, THREAD_ATTR_USER | THREAD_ATTR_VFPU, 0);
                                         
         if (ThreadID>=0)
         {
            INTERNAL_PHOENIX_RegisterNewCBE (ThreadID, CBE_BUTTON);  // Registra il thread CBE
            INTERNAL_PHOENIX_LockForNewCBE  (ThreadID);              // Locka gli altri thread in attesa del CBE
            NewPacketForCBE->CBE_ThreadID = ThreadID;
            
            sceKernelStartThread (ThreadID, SizePacket, NewPacketForCBE); 
            sceKernelDelayThread (250000);                           // Tenta di impedire saturazione del buffer CBE per pressioni multiple
            
            return;             
         }
         else                   // Non è riuscito a creare il thread: disalloca il pacchetto
         {
            free (NewPacketForCBE);
            return;
         }
    }                        
} 

static inline void INTERNAL_PHOENIX_ExecuteButtonCallback_L1_FAST (void *ButtonCallback, char *StringID, ndint64 InfoField, 
                                                            char WindowFoundID, char ButtonFoundID, char TraspCallback)
{     
    struct ndEvent_Type MyEvent;
    
    // Puntatore alla callback
    static ndint64 (*PntToButtonCallback)(char *StringID, ndint64 InfoField, char WndHandle);
    static ndint64 __RetValue;
    
    PntToButtonCallback = ButtonCallback;
    __RetValue = (*PntToButtonCallback)(   StringID, InfoField, WindowFoundID    );
    
    if (!TraspCallback)
    {         
         MyEvent.__TypeEvent = CUSTOM_BUTTON_PRESSED;
         MyEvent.__WindowID = WindowFoundID;
         MyEvent.__ButtonID = ButtonFoundID;
         strcpy ( &(MyEvent.__StringID), StringID );
         MyEvent.__CallbackExecuted =1;
         MyEvent.__CallbackAnswer   =__RetValue;
         strcpy ( &(MyEvent.__ZString), "");
         
         INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                  
    }
    
    return;   
}



static inline void INTERNAL_PHOENIX_ExecuteButtonCallback_L0 (void *ButtonCallback, char *StringID, ndint64 InfoField, 
                                                       char WindowFoundID, char ButtonFoundID, 
                                                       char TraspCallback, char NewContext)
{
    struct ndEvent_Type MyEvent;
    
    if (ButtonCallback!=0)      // E' necessario eseguire una callback
    {
         if (!NewContext)       // E' richiesta una fast callback
         {
              INTERNAL_PHOENIX_ExecuteButtonCallback_L1_FAST (ButtonCallback, StringID, InfoField, WindowFoundID, 
                                                         ButtonFoundID, TraspCallback);           
         }
         else                   // E' richiesta l'esecuzione di una callback di pulsante in un altro contesto
         {
              INTERNAL_PHOENIX_ExecuteButtonCallback_L1 (ButtonCallback, StringID, InfoField, WindowFoundID, 
                                                         ButtonFoundID, TraspCallback);
                                                         
         } 
    }
    else                        // Non è necessario eseguire alcuna callback: invia un messaggio
    {
         MyEvent.__TypeEvent = CUSTOM_BUTTON_PRESSED;
         MyEvent.__WindowID = WindowFoundID;
         MyEvent.__ButtonID = ButtonFoundID;
         strcpy ( &(MyEvent.__StringID), StringID );
         MyEvent.__CallbackExecuted=0;
         MyEvent.__CallbackAnswer=0;
         strcpy ( &(MyEvent.__ZString), "");
         
         INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                                
    } 
    
    return;
} 

// --------------Fine della sezione relativa alle callbacks di pulsante  --------------- 

// -------------- Inizio della sezione relativa alle close window callbacks ------------


struct phnMouse_PacketForCBECLWND_Type
{
    void     *CloseWndCallback;
    ndint64  WndInfoField;
    char     WindowFoundID; 
    char     NoNotifyToMouseControl;   
    int      CBE_ThreadID;       
};                     



static void INTERNAL_PHOENIX_ExecuteCloseWndCallback_L2 (SceSize args, void *argp)
{
    struct ndEvent_Type MyEvent;
    ndint64 __RetValue;
    
    // Recupera l'indirizzo del pacchetto
    struct phnMouse_PacketForCBECLWND_Type *NewPacketForCBE = argp;
    
    // Puntatore alla callback
    ndint64 (*PntToCloseWndCallback)(char WndHandle, ndint64 WndInfoField);
    
    // Inizio del codice 
    
    PntToCloseWndCallback = NewPacketForCBE->CloseWndCallback;
    
    __RetValue = (*PntToCloseWndCallback)( NewPacketForCBE->WindowFoundID,  NewPacketForCBE->WndInfoField );
                                           
    // Dopo l'esecuzione della callback, provvedi ad unlockare i thread in attesa, in modo
    // che i messaggi vengano recapitati anche ai thread che attendevano la fine del CBE 
    
    INTERNAL_PHOENIX_UnLockForCBE (NewPacketForCBE->CBE_ThreadID);
    
    // Dopo l'esecuzione della callback, deve verificare se è necessario usare il dispatcher
    // per fornire un messaggio ai threads in attesa
        
    if (!NewPacketForCBE->NoNotifyToMouseControl)
    {
         MyEvent.__TypeEvent = CLOSEWNDCALLBACK_HAS_BEEN_EXEC;
         MyEvent.__WindowID = NewPacketForCBE->WindowFoundID;
         MyEvent.__ButtonID = -1;
         strcpy ( &(MyEvent.__StringID), "" );
         MyEvent.__CallbackExecuted  =1;
         MyEvent.__CallbackAnswer    =__RetValue;
         strcpy ( &(MyEvent.__ZString), "");
         
         INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                         
    }
    
    INTERNAL_PHOENIX_UnRegisterCBE (NewPacketForCBE->CBE_ThreadID);   // Deregistra il thread CBE
    
    free (NewPacketForCBE);                                     // Disalloca il pacchetto
    sceKernelExitDeleteThread  (0);                                   // Esce dal thread e lo cancella
    
    return;
}




static inline void INTERNAL_PHOENIX_ExecuteCloseWndCallback_L1 (void *CloseWndCallback, ndint64 WndInfoField, 
                                                         char WindowFoundID, char NoNotifyToMouseControl)
{
    struct phnMouse_PacketForCBECLWND_Type *NewPacketForCBE;
    char   nameofTCBThread [200];
    char   nameofTCBThread0 [32];
    int    ThreadID;
    int    SizePacket;
    
    // Iniziamo eseguendo una copia del request buffer
    
    SizePacket      = sizeof ( struct phnMouse_PacketForCBECLWND_Type );
    NewPacketForCBE = malloc  ( SizePacket );
    
    if (NewPacketForCBE)     // L'allocazione dinamica è riuscita
    {
         // Prepara il pacchetto
         
         NewPacketForCBE->CloseWndCallback        = CloseWndCallback;
         NewPacketForCBE->WndInfoField            = WndInfoField;
         NewPacketForCBE->WindowFoundID           = WindowFoundID;
         NewPacketForCBE->NoNotifyToMouseControl  = NoNotifyToMouseControl;
         
         // Prepara il nuovo thread CBE
         
         TCBCounter++;
         if (TCBCounter==2000) TCBCounter=0;
         
         strcpy (&nameofTCBThread, "CBECLW");
         _itoa (TCBCounter, &nameofTCBThread0, 10);
         strcat (&nameofTCBThread, &nameofTCBThread0);
         
         ThreadID=sceKernelCreateThread (&nameofTCBThread, &INTERNAL_PHOENIX_ExecuteCloseWndCallback_L2,
                                         33, 16384, THREAD_ATTR_USER | THREAD_ATTR_VFPU, 0);
                                         
         if (ThreadID>=0)
         {
            INTERNAL_PHOENIX_RegisterNewCBE (ThreadID, CBE_CLOSEWND);  // Registra il thread CBE
            INTERNAL_PHOENIX_LockForNewCBE  (ThreadID);               // Locka gli altri thread in attesa del CBE
            NewPacketForCBE->CBE_ThreadID = ThreadID;
            
            sceKernelStartThread (ThreadID, SizePacket, NewPacketForCBE); 
            return;             
         }
         else                   // Non è riuscito a creare il thread: disalloca il pacchetto
         {
            free (NewPacketForCBE);
            return;
         }
    }
}



static inline void INTERNAL_PHOENIX_ExecuteCloseWndCallback_L0 (void *CloseWndCallback, ndint64 WndInfoField, 
                                                         char WindowFoundID, char NoNotifyToMouseControl)
{
    struct ndEvent_Type MyEvent;
    
    if (CloseWndCallback!=0)    // E' necessario eseguire una CloseWndCallback
    {
         INTERNAL_PHOENIX_ExecuteCloseWndCallback_L1 (CloseWndCallback, WndInfoField, WindowFoundID, NoNotifyToMouseControl);
         
         return;
    }
    else                        // Non è necessario eseguire una CloseWndCallback: segnaliamo alla routine chiamante
    {
         MyEvent.__TypeEvent = A_WINDOW_WANTS_TO_BE_CLOSED;
         MyEvent.__WindowID = WindowFoundID;
         MyEvent.__ButtonID = 0;
         strcpy ( &(MyEvent.__StringID), "" );
         MyEvent.__CallbackExecuted=0;
         MyEvent.__CallbackAnswer=0;
         strcpy ( &(MyEvent.__ZString), "");
         
         INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                  
         return;  
    }
}



// --------------- Fine della sezione relativa alle close window callbacks  ------------



// -------------- Inizio della sezione relativa alle WMI1 callbacks ------------


struct phnMouse_PacketForCBEWMI1_Type
{
    struct ELEMENT_L1WMI_Type *MyL1WMI;
    char   WindowFoundID; 
    int    CBE_ThreadID;       
};                     



static void INTERNAL_PHOENIX_ExecuteWMI1Callback_L2 (SceSize args, void *argp)
{   
    struct ndEvent_Type MyEvent;
    
    ndint64 __RetValue;
    char     __WindowFoundID;
    
    // Recupera l'indirizzo del pacchetto
    struct phnMouse_PacketForCBEWMI1_Type *NewPacketForCBE = argp;
    
    // Puntatore alla callback
    ndint64 (*PointerToCallback)(char WndHandle);      // Puntatore alla Callback
    
    
    
    // Inizio del codice 
    
    PointerToCallback = NewPacketForCBE->MyL1WMI->CallbackFunction;
    
    __WindowFoundID   = NewPacketForCBE->WindowFoundID;
    __RetValue        = (*PointerToCallback)( __WindowFoundID );
                                           
    // Dopo l'esecuzione della callback, provvedi ad unlockare i thread in attesa, in modo
    // che i messaggi vengano recapitati anche ai thread che attendevano la fine del CBE 
    
    INTERNAL_PHOENIX_UnLockForCBE (NewPacketForCBE->CBE_ThreadID);
    
    // Dopo l'esecuzione della callback, deve verificare se è necessario usare il dispatcher
    // per fornire un messaggio ai threads in attesa
        
    MyEvent.__TypeEvent = L1WMI_BUTTON_PRESSED;
    MyEvent.__WindowID  = __WindowFoundID;
    MyEvent.__ButtonID  = 0;
         
    strcpy ( &(MyEvent.__StringID), "" );
         
    if (strcmp (WindowData [__WindowFoundID].ZString, "")==0) 
         strcpy ( &(MyEvent.__ZString), &(NewPacketForCBE->MyL1WMI->StringID) );
    else
    {
         strcpy ( &(MyEvent.__ZString), &(WindowData [__WindowFoundID].ZString) );
         strcat ( &(MyEvent.__ZString), "/");
         strcat ( &(MyEvent.__ZString), &(NewPacketForCBE->MyL1WMI->StringID) );
    }
     
    MyEvent.__CallbackExecuted   = 1;
    MyEvent.__CallbackAnswer     = __RetValue;
         
    INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                         
    
    
    INTERNAL_PHOENIX_UnRegisterCBE (NewPacketForCBE->CBE_ThreadID);   // Deregistra il thread CBE
    free (NewPacketForCBE);                                     // Disalloca il pacchetto
    
    sceKernelExitDeleteThread  (0);                                   // Esce dal thread e lo cancella
    
    return;
}




static inline void INTERNAL_PHOENIX_ExecuteWMI1Callback_L1 (struct ELEMENT_L1WMI_Type *MyL1WMI, char WindowFoundID)
{
    struct phnMouse_PacketForCBEWMI1_Type *NewPacketForCBE;
    char   nameofTCBThread [200];
    char   nameofTCBThread0 [32];
    int    ThreadID;
    int    SizePacket;
    
    // Iniziamo eseguendo una copia del request buffer
    
    SizePacket      = sizeof ( struct phnMouse_PacketForCBEWMI1_Type );
    NewPacketForCBE = malloc  ( SizePacket );
    
    if (NewPacketForCBE)     // L'allocazione dinamica è riuscita
    {
         // Prepara il pacchetto
         
         NewPacketForCBE->MyL1WMI                 = MyL1WMI;
         NewPacketForCBE->WindowFoundID           = WindowFoundID;
         
         // Prepara il nuovo thread CBE
         
         TCBCounter++;
         if (TCBCounter==2000) TCBCounter=0;
         
         strcpy (&nameofTCBThread, "CBEWM1X");
         _itoa (TCBCounter, &nameofTCBThread0, 10);
         strcat (&nameofTCBThread, &nameofTCBThread0);
         
         ThreadID=sceKernelCreateThread (&nameofTCBThread, &INTERNAL_PHOENIX_ExecuteWMI1Callback_L2,
                                         33, 16384, THREAD_ATTR_USER | THREAD_ATTR_VFPU, 0);
                                         
         if (ThreadID>=0)
         {
            INTERNAL_PHOENIX_RegisterNewCBE (ThreadID, CBE_WMI1);  // Registra il thread CBE
            INTERNAL_PHOENIX_LockForNewCBE  (ThreadID);           // Locka gli altri thread in attesa del CBE
            NewPacketForCBE->CBE_ThreadID = ThreadID;
            
            sceKernelStartThread (ThreadID, SizePacket, NewPacketForCBE); 
            return;             
         }
         else                   // Non è riuscito a creare il thread: disalloca il pacchetto
         {
            free (NewPacketForCBE);
            return;
         }
    }
}

static inline void INTERNAL_PHOENIX_ExecuteWMI1Callback_FAST (struct ELEMENT_L1WMI_Type *MyL1WMI, char WindowFoundID)
{
    struct ndEvent_Type MyEvent; 
    ndint64 __RetValue;
    
    // Puntatore alla callback
    ndint64 (*PointerToCallback)(char WndHandle);      // Puntatore alla Callback
    
    // Inizio del codice 
    
    PointerToCallback = MyL1WMI->CallbackFunction;
    __RetValue        = (*PointerToCallback)( WindowFoundID );
                                           
    // Dopo l'esecuzione della callback, deve verificare se è necessario usare il dispatcher
    // per fornire un messaggio ai threads in attesa
        
    MyEvent.__TypeEvent = L1WMI_BUTTON_PRESSED;
    MyEvent.__WindowID  = WindowFoundID;
    MyEvent.__ButtonID  = 0;
         
    strcpy ( &(MyEvent.__StringID), "" );
         
    if (strcmp (WindowData [WindowFoundID].ZString, "")==0) 
         strcpy ( &(MyEvent.__ZString), &(MyL1WMI->StringID) );
    else
    {
         strcpy ( &(MyEvent.__ZString), &(WindowData [WindowFoundID].ZString) );
         strcat ( &(MyEvent.__ZString), "/");
         strcat ( &(MyEvent.__ZString), &(MyL1WMI->StringID) );
    }
     
    MyEvent.__CallbackExecuted   = 1;
    MyEvent.__CallbackAnswer     = __RetValue;
         
    INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                         
    
    return;   
}



static inline void INTERNAL_PHOENIX_ExecuteWMI1Callback_L0 (struct ELEMENT_L1WMI_Type *MyL1WMI, char WindowFoundID)
{                                                  
   struct ndEvent_Type MyEvent;
     
   if (MyL1WMI->CallbackFunction!=0)    // E' necessario eseguire una WMI1 Callback
   {
         if (MyL1WMI->ExecInNewContext)
            INTERNAL_PHOENIX_ExecuteWMI1Callback_L1 (MyL1WMI, WindowFoundID);
         else
            INTERNAL_PHOENIX_ExecuteWMI1Callback_FAST (MyL1WMI, WindowFoundID); 
         
         return;
   }
   else                        // Non è necessario eseguire una WMI1 Callback: segnaliamo alla routine chiamante
   {
         MyEvent.__TypeEvent = L1WMI_BUTTON_PRESSED;
         MyEvent.__WindowID  = WindowFoundID;
         MyEvent.__ButtonID  = 0;
         strcpy ( &(MyEvent.__StringID), "" );
         
         if (strcmp (WindowData [WindowFoundID].ZString, "")==0) 
             strcpy ( &(MyEvent.__ZString), &(MyL1WMI->StringID) );
         else
         {
             strcpy ( &(MyEvent.__ZString), &(WindowData [WindowFoundID].ZString) );
             strcat ( &(MyEvent.__ZString), "/");
             strcat ( &(MyEvent.__ZString), &(MyL1WMI->StringID) );
         }
         
         MyEvent.__CallbackExecuted=0;
         MyEvent.__CallbackAnswer=0;
         
         INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                  
         return;  
   }     
}



// --------------- Fine della sezione relativa alle WMIItems callbacks  ------------




// -------------- Inizio della sezione relativa alle WMI2 callbacks ------------


struct phnMouse_PacketForCBEWMI2_Type
{
    struct ELEMENT_L2WMI_Type *MyL1WMI;
    struct ELEMENT_L2WMI_Type *MyL2WMI;
    char   WindowFoundID; 
    int    CBE_ThreadID;       
};                     



static void INTERNAL_PHOENIX_ExecuteWMI2Callback_L2 (SceSize args, void *argp)
{   
    struct ndEvent_Type MyEvent;
    
    ndint64 __RetValue;
    char     __WindowFoundID;
    
    // Recupera l'indirizzo del pacchetto
    struct phnMouse_PacketForCBEWMI2_Type *NewPacketForCBE = argp;
    
    // Puntatore alla callback
    ndint64 (*PointerToCallback)(char WndHandle);      // Puntatore alla Callback
    
    
    
    // Inizio del codice 
    
    PointerToCallback = NewPacketForCBE->MyL2WMI->CallbackFunction;
    
    __WindowFoundID   = NewPacketForCBE->WindowFoundID;
    __RetValue        = (*PointerToCallback)( __WindowFoundID );
                                           
    // Dopo l'esecuzione della callback, provvedi ad unlockare i thread in attesa, in modo
    // che i messaggi vengano recapitati anche ai thread che attendevano la fine del CBE 
    
    INTERNAL_PHOENIX_UnLockForCBE (NewPacketForCBE->CBE_ThreadID);
    
    // Dopo l'esecuzione della callback, deve verificare se è necessario usare il dispatcher
    // per fornire un messaggio ai threads in attesa
        
    MyEvent.__TypeEvent = L2WMI_BUTTON_PRESSED;
    MyEvent.__WindowID  = __WindowFoundID;
    MyEvent.__ButtonID  = 0;
         
    strcpy ( &(MyEvent.__StringID), "" );
         
    if (strcmp (WindowData [__WindowFoundID].ZString, "")==0)
    {
         strcpy ( &(MyEvent.__ZString), &(NewPacketForCBE->MyL1WMI->StringID) );
    }
    else
    {
         strcpy ( &(MyEvent.__ZString), &(WindowData [__WindowFoundID].ZString) );
         strcat ( &(MyEvent.__ZString), "/");
         strcat ( &(MyEvent.__ZString), &(NewPacketForCBE->MyL1WMI->StringID) );
    }
    
    strcat ( &(MyEvent.__ZString), "." );
    strcat ( &(MyEvent.__ZString), &(NewPacketForCBE->MyL2WMI->StringID) );
 
    MyEvent.__CallbackExecuted   = 1;
    MyEvent.__CallbackAnswer     = __RetValue;
         
    INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                         
    
    INTERNAL_PHOENIX_UnRegisterCBE (NewPacketForCBE->CBE_ThreadID);   // Deregistra il thread CBE
    
    free (NewPacketForCBE);                                     // Disalloca il pacchetto
    sceKernelExitDeleteThread  (0);                                   // Esce dal thread e lo cancella
    
    return;
}




static inline void INTERNAL_PHOENIX_ExecuteWMI2Callback_L1 (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ELEMENT_L2WMI_Type *MyL2WMI, char WindowFoundID)
{
    struct phnMouse_PacketForCBEWMI2_Type *NewPacketForCBE;
    char   nameofTCBThread [200];
    char   nameofTCBThread0 [32];
    int    ThreadID;
    int    SizePacket;
    
    // Iniziamo eseguendo una copia del request buffer
    
    SizePacket      = sizeof ( struct phnMouse_PacketForCBEWMI2_Type );
    NewPacketForCBE = malloc  ( SizePacket );
    
    if (NewPacketForCBE)     // L'allocazione dinamica è riuscita
    {
         // Prepara il pacchetto
         
         NewPacketForCBE->MyL1WMI                 = MyL1WMI;
         NewPacketForCBE->MyL2WMI                 = MyL2WMI;
         NewPacketForCBE->WindowFoundID           = WindowFoundID;
         
         // Prepara il nuovo thread CBE
         
         TCBCounter++;
         if (TCBCounter==2000) TCBCounter=0;
         
         strcpy (&nameofTCBThread, "CBEWM2X");
         _itoa (TCBCounter, &nameofTCBThread0, 10);
         strcat (&nameofTCBThread, &nameofTCBThread0);
         
         ThreadID=sceKernelCreateThread (&nameofTCBThread, &INTERNAL_PHOENIX_ExecuteWMI2Callback_L2,
                                         33, 16384, THREAD_ATTR_USER | THREAD_ATTR_VFPU, 0);
                                         
         if (ThreadID>=0)
         {
            INTERNAL_PHOENIX_RegisterNewCBE (ThreadID, CBE_WMI2);  // Registra il thread CBE
            INTERNAL_PHOENIX_LockForNewCBE  (ThreadID);           // Locka gli altri thread in attesa del CBE
            NewPacketForCBE->CBE_ThreadID = ThreadID;
            
            sceKernelStartThread (ThreadID, SizePacket, NewPacketForCBE); 
            return;             
         }
         else                   // Non è riuscito a creare il thread: disalloca il pacchetto
         {
            free (NewPacketForCBE);
            return;
         }
    }
}

static void INTERNAL_PHOENIX_ExecuteWMI2Callback_FAST (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ELEMENT_L2WMI_Type *MyL2WMI, char WindowFoundID)
{   
    struct ndEvent_Type MyEvent;
    ndint64 __RetValue;
    
    // Puntatore alla callback
    ndint64 (*PointerToCallback)(char WndHandle);      // Puntatore alla Callback
    
    
    
    // Inizio del codice 
    
    PointerToCallback = MyL2WMI->CallbackFunction;
    __RetValue        = (*PointerToCallback)( WindowFoundID );
                                           
    // Dopo l'esecuzione della callback, deve verificare se è necessario usare il dispatcher
    // per fornire un messaggio ai threads in attesa
        
    MyEvent.__TypeEvent = L2WMI_BUTTON_PRESSED;
    MyEvent.__WindowID  = WindowFoundID;
    MyEvent.__ButtonID  = 0;
         
    strcpy ( &(MyEvent.__StringID), "" );
         
    if (strcmp (WindowData [WindowFoundID].ZString, "")==0)
    {
         strcpy ( &(MyEvent.__ZString), &(MyL1WMI->StringID) );
    }
    else
    {
         strcpy ( &(MyEvent.__ZString), &(WindowData [WindowFoundID].ZString) );
         strcat ( &(MyEvent.__ZString), "/");
         strcat ( &(MyEvent.__ZString), &(MyL1WMI->StringID) );
    }
    
    strcat ( &(MyEvent.__ZString), "." );
    strcat ( &(MyEvent.__ZString), &(MyL2WMI->StringID) );
 
    MyEvent.__CallbackExecuted   = 1;
    MyEvent.__CallbackAnswer     = __RetValue;
         
    INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                         
    
    return;
}




static inline void INTERNAL_PHOENIX_ExecuteWMI2Callback_L0 (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ELEMENT_L2WMI_Type *MyL2WMI, char WindowFoundID)
{                                                  
   struct ndEvent_Type MyEvent;
    
   if (MyL2WMI->CallbackFunction!=0)    // E' necessario eseguire una WMI2 Callback
   {
         if (MyL2WMI->ExecInNewContext)
            INTERNAL_PHOENIX_ExecuteWMI2Callback_L1 (MyL1WMI, MyL2WMI, WindowFoundID);
         else
            INTERNAL_PHOENIX_ExecuteWMI2Callback_FAST (MyL1WMI, MyL2WMI, WindowFoundID); 
         
         return;
   }
   else                        // Non è necessario eseguire una WMI2 Callback: segnaliamo alla routine chiamante
   {
         MyEvent.__TypeEvent = L2WMI_BUTTON_PRESSED;
         MyEvent.__WindowID  = WindowFoundID;
         MyEvent.__ButtonID  = 0;
         
         strcpy ( &(MyEvent.__StringID), "" );
         
         if (strcmp (WindowData [WindowFoundID].ZString, "")==0)
         {
             strcpy ( &(MyEvent.__ZString), &(MyL1WMI->StringID) );
         }
         else
         {
             strcpy ( &(MyEvent.__ZString), &(WindowData [WindowFoundID].ZString) );
             strcat ( &(MyEvent.__ZString), "/");
             strcat ( &(MyEvent.__ZString), &(MyL1WMI->StringID) );
         }
   
         strcat ( &(MyEvent.__ZString), "." );
         strcat ( &(MyEvent.__ZString), &(MyL2WMI->StringID) );
   
         MyEvent.__CallbackExecuted=0;
         MyEvent.__CallbackAnswer  =0;
         
         INTERNAL_PHOENIX_DispatchMessage (&MyEvent);       // Invia i messaggi                                  
         return;  
   }     
   
}



// --------------- Fine della sezione relativa alle WMI2Items callbacks  ------------



/* 
    ***************************************************************************
    ***************************************************************************
                         WMI EVENT MANAGER   (Phoenix compatible)
    ***************************************************************************
    ***************************************************************************
*/

void INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (unsigned char WndHandle, char Value)
{
     WindowData [WndHandle].PrevOpenL2WMIMenu = WindowData [WndHandle].OpenL2WMIMenu;
     WindowData [WndHandle].OpenL2WMIMenu     = Value;
}

void INTERNAL_PHOENIX_Scroll_WMI1_To_Right (char WndHandle)
/* 
    Esegue lo scrolling a destra dei menù di tipo 1
*/

{
    if (WindowData [WndHandle].OpenL2WMIMenu) INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
    // L'operazione di scrolling a destra o a sinistra chiude sempre eventuali menù
    // di secondo livello aperti.
       
    WindowData [WndHandle].NrFirstL1WMIButton++;
    WindowData [WndHandle].MenuBarIsChanged=1;

    XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
}

void INTERNAL_PHOENIX_Scroll_WMI1_To_Left (char WndHandle)
/* 
    Esegue lo scrolling a sinistra dei menù di tipo 1
*/

{
    if (WindowData [WndHandle].OpenL2WMIMenu) INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
    // L'operazione di scrolling a destra o a sinistra chiude sempre eventuali menù
    // di secondo livello aperti.
    
    WindowData [WndHandle].NrFirstL1WMIButton--;
    WindowData [WndHandle].MenuBarIsChanged=1;
    
    XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
}


void INTERNAL_PHOENIX_Scroll_WMI2_To_Up (char WndHandle)
{
    WindowData [WndHandle].L2WMIScrollElement--;
    WindowData [WndHandle].MenuBarIsChanged=1;
    
    XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
}


void INTERNAL_PHOENIX_Scroll_WMI2_To_Down (char WndHandle)
{
    WindowData [WndHandle].L2WMIScrollElement++;
    WindowData [WndHandle].MenuBarIsChanged=1;
    
    XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
}


void INTERNAL_PHOENIX_GoToPreviousWMIMenu (char WndHandle)
{
   char TempBuffer [512];
   short int Counter, LenString;
   char SlashFound, SlashPos;
   
   SlashFound=0;
   LenString = strlen (WindowData [WndHandle].ZString);
   
   for (Counter=LenString; Counter>=0; Counter--)
   {
       if (WindowData [WndHandle].ZString [Counter] == '/')
       {
          SlashFound=1;
          SlashPos=Counter;
       }
   }
   
   if (!SlashFound)        // Non c'era nessuno slash
   {
      strcpy (WindowData [WndHandle].ZString, "");
   }
   else
   {
      ndStrCpy (TempBuffer, &(WindowData [WndHandle].ZString), SlashPos-1, 0);
      // Copia tutto ciò che c'era prima dello slash
      
      strcpy ( &(WindowData [WndHandle].ZString), TempBuffer);
   }
   
   WindowData [WndHandle].NrFirstL1WMIButton=0;
   WindowData [WndHandle].NrL1WMIButtonOpened=0;
   WindowData [WndHandle].L2WMIMenuScrollActive=0;
   WindowData [WndHandle].L2WMIScrollElement=0;
   
   INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
   
   WindowData [WndHandle].MenuBarIsChanged=1;
   
   XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
}


void INTERNAL_PHOENIX_EnterToNew_WMI1  (struct ELEMENT_L1WMI_Type *MyL1WMI, char WndHandle)
{ 
   WindowData [WndHandle].MenuBarIsChanged=1;
    
   if (strcmp (  &(WindowData[WndHandle].ZString), "" )==0)
   {
      strcpy (  &(WindowData[WndHandle].ZString), MyL1WMI->StringID );
   }
   else
   {
      strcat (  &(WindowData [WndHandle].ZString), "/" );
      strcat (  &(WindowData [WndHandle].ZString), MyL1WMI->StringID );
   }   
   
   
   WindowData [WndHandle].NrFirstL1WMIButton=0;
   WindowData [WndHandle].NrL1WMIButtonOpened=0;
   WindowData [WndHandle].L2WMIMenuScrollActive=0;
   WindowData [WndHandle].L2WMIScrollElement=0;
   
   INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
   
   XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
}


void INTERNAL_PHOENIX_CloseWMI2MenuOpened_Resolutive (char WndHandle)
/* 
   Chiude il menù di livello 2 aperto in modo risolutivo, nel senso che la routine
   chiamante dopo restituirà semplicemente 0 alla routine di partenza e verrà 
   eseguito un nuovo ciclo di gestione del mouse
*/
{                     
   WindowData [WndHandle].MenuBarIsChanged=1;
   INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);         // Chiude il vecchio menù
   WindowData [WndHandle].NrL1WMIButtonOpened = 0;
               
   XWindowRender (WndHandle);                         // Disegnerà la nuova configurazione del menù               
}


void INTERNAL_PHOENIX_CloseWMI2MenuOpened_Not_Resolutive (char WndHandle)
/* 
   Chiude il menù di livello 2 aperto in modo non risolutivo, nel senso che la routine
   chiamante dovrà eseguire delle altre operazioni successivamente
*/
{
   WindowData [WndHandle].MenuBarIsChanged=1;       // Per prima cosa disattiviamo 
                                                                // il vecchio menù
   INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
   WindowData [WndHandle].NrL1WMIButtonOpened = 0;  // Cambia configurazione del menù          
}



void INTERNAL_PHOENIX_OpenWMI2Menu (char NrWMI1, char WndHandle)
{
   WindowData [WndHandle].MenuBarIsChanged=1;
           
   INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 1);
   WindowData [WndHandle].NrL1WMIButtonOpened = NrWMI1;
           
   XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù          
}







void INTERNAL_PHOENIX_WMI1_Pressed (char ButtonFoundID, char WndHandle)
/*
     Questa routine viene eseguita quando viene premuto un tasto corrispondente ad un WMI
     di tipo 1.
*/
{
     struct ELEMENT_L1WMI_Type *MyL1WMI;
     char NrWMI1;
     char ErrRep;
     
     NrWMI1 = ButtonFoundID + WindowData [WndHandle].NrFirstL1WMIButton;
     // Determina a quale WMI1 ci si riferisce
     
     ErrRep=ndWMENU_EnumL1WMI (WndHandle, &(WindowData [WndHandle].ZString), NrWMI1, &MyL1WMI);
     // Consente di avere tutte le informazioni necessarie sull'elemento in questione
     
     if (!ErrRep)
     {
        // Per prima cosa, verifichiamo che non ci siano menù aperti
        
        if (WindowData [WndHandle].OpenL2WMIMenu)        // C'era già un menu aperto
        {
           if (WindowData [WndHandle].NrL1WMIButtonOpened == NrWMI1)    // Il menu aperto è quello su cui si è ricliccato
           {
              INTERNAL_PHOENIX_CloseWMI2MenuOpened_Resolutive (WndHandle); 
              return; 
           }
           else        // Il menù aperto è diverso da quello in cui si è cliccato
           {
              INTERNAL_PHOENIX_CloseWMI2MenuOpened_Not_Resolutive (WndHandle);  
           }
        }
        
        // A questo punto, se c'era un vecchio menù, di sicuro è stato disattivato. Attenzione
        // però perchè è necessario un XWindowRender per la visualizzazione dei cambiamenti
        
        
        if ((MyL1WMI->L2SubMenu)!=0)       // C'è un nuovo submenu di tipo 2 da visualizzare
        {   
           INTERNAL_PHOENIX_OpenWMI2Menu (NrWMI1, WndHandle);
        }   
        else
        {
              if ((MyL1WMI->L1SubMenu)!=0)             // C'è un nuovo submenu di tipo 1 da visualizzare
              {
                 INTERNAL_PHOENIX_EnterToNew_WMI1 (MyL1WMI, WndHandle); 
              }  
              else    // Non c'è nessun submenu, provvedi ad eseguire un eventuale callback WMI1    
              {
                 INTERNAL_PHOENIX_ExecuteWMI1Callback_L0 (MyL1WMI, WndHandle); 
              }
                 
        } 
     }
}


void INTERNAL_PHOENIX_WMI2_Pressed (char ButtonFoundID, char WndHandle)
/*
     Questa routine viene eseguita quando viene premuto un tasto corrispondente ad un WMI
     di tipo 2.
*/

{
     struct ELEMENT_L1WMI_Type *MyL1WMI;
     struct ELEMENT_L2WMI_Type *MyL2WMI;
     
     char NrL1WMI, NrL2WMI;
     char ErrRep;
     
     char TempBuffer [512];
     char SBuffer [512];
     
     unsigned short int SSPosX1, SSPosY1, SSPosX2, SSPosY2;
     unsigned short int rX1, rY1;
   
     unsigned short int LenButton;
     unsigned short int NrCharsInButton;
     unsigned short int Margin;
        
     NrL1WMI = WindowData [WndHandle].NrL1WMIButtonOpened;
     // Determina a quale WMI1 ci si riferisce
     
     ErrRep = ndWMENU_EnumL1WMI (WndHandle, &(WindowData [WndHandle].ZString), NrL1WMI, &MyL1WMI);
     // Consente di avere tutte le informazioni necessarie sull'elemento in questione
     
     if (!ErrRep)
     {
         if (strcmp ( &(WindowData [WndHandle].ZString), "" ) == 0)
         {
            strcpy (TempBuffer, MyL1WMI->StringID);
         }       
         else
         {
            strcpy (TempBuffer, &(WindowData [WndHandle].ZString) );
            strcat (TempBuffer, "/");
            strcat (TempBuffer, MyL1WMI->StringID); 
         }
         
         if (!WindowData[WndHandle].L2WMIMenuScrollActive)
            NrL2WMI = ButtonFoundID;
         else
            NrL2WMI = ButtonFoundID + WindowData[WndHandle].L2WMIScrollElement;
         
         // Ottieni le informazioni necessarie sull'elemento selezionato
     
         ErrRep = ndWMENU_EnumL2WMI (WndHandle, TempBuffer, NrL2WMI, &MyL2WMI);
         
         if (!ErrRep)
         {
             // Quello che segue è un semplice effetto grafico: il tasto che è stato
             // premuto deve venire evidenziato per un attimo prima che il sistema
             // dei menù venga chiuso.
             
             SSPosX1 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosX1;
             SSPosY1 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosY1;
             SSPosX2 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosX2;
             SSPosY2 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosY2;
             
             LenButton       = SSPosX2 - SSPosX1 - 4;
            
             NrCharsInButton = LenButton / 8;
             Margin          = LenButton - (NrCharsInButton * 8) ;
        
             rX1 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosX1 + 2 + Margin/2;
             rY1 = SSPosY1 + 4;
        
             ndStrCpy (SBuffer, MyL2WMI->NameItem, NrCharsInButton, 0);
            
             ndSS_DrawRectangle    (SSPosX1, SSPosY1, SSPosX2, SSPosY2, COLOR_BLUE, COLOR_BLUE, WndHandle, NORENDER);
             ndSS_WriteStringInSS  (rX1,     rY1,     SBuffer, COLOR_WHITE, COLOR_BLUE, WndHandle, NORENDER);
             
             XWindowRender (WndHandle);   // Mostra il pulsante evidenziato
             
             INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
             WindowData [WndHandle].MenuBarIsChanged = 1;
             
             XWindowRender (WndHandle);   // Mostra che il menù L2WMI è scomparso
             
             // Prepariamoci a fornire un messaggio alla routine chiamante
             
             INTERNAL_PHOENIX_ExecuteWMI2Callback_L0 (MyL1WMI, MyL2WMI, WndHandle);
         }    
     }
}




char INTERNAL_PHOENIX_WMIEventManager (unsigned short int MousePointer_RPosX, unsigned short int MousePointer_RPosY, char WindowFoundID, 
                                          struct ndMouseState_Type *MouseState)
{
   unsigned short int MBPosX1, MBPosY1;
   unsigned short int SSPosX1, SSPosY1;
   char ButtonFoundID;
   char ButtonFound;
   char CounterA;
   
   
   
   // Controlliamo innanzitutto se è stato premuto un tasto WMI1Serv
   
   MBPosX1 = MousePointer_RPosX - WindowData [WindowFoundID].MenuBar_PosX1;
   MBPosY1 = MousePointer_RPosY - WindowData [WindowFoundID].MenuBar_PosY1;
   
   ButtonFound=0;
     
   for (CounterA=0; CounterA<3; CounterA++)
   {          
     if ( (WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].ButtonIsActivated) &&
          (PixelInRect (MBPosX1, MBPosY1, WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].MBPosX1,
                        WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].MBPosY1,
                        WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].MBPosX2,
                        WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].MBPosY2) ) )       
     {
          ButtonFound=1;
          ButtonFoundID=CounterA;
          
          break;
     }
   }
   
   
   
   if (ButtonFound)              // E' stato premuto un pulsante WMI1Serv
   {
      switch (ButtonFoundID)
      {
             case WMI_GO_BACK:
                  {
                     INTERNAL_PHOENIX_GoToPreviousWMIMenu (WindowFoundID);
                     return 1;    // Informa EventManager che è necessario iniziare un nuovo ciclo mouse
                  }
             case SCROLL_WMI1_TO_RIGHT:
                  {
                     INTERNAL_PHOENIX_Scroll_WMI1_To_Right (WindowFoundID);
                     return 1;    // Informa EventManager che è necessario iniziare un nuovo ciclo mouse  
                  }
             case SCROLL_WMI1_TO_LEFT:
                  {
                     INTERNAL_PHOENIX_Scroll_WMI1_To_Left (WindowFoundID);
                     return 1;    // Informa EventManager che è necessario iniziare un nuovo ciclo mouse
                  }
      }   
   }
   
   // Ora vediamo se è stato premuto un tasto WMI1
   
   ButtonFound=0;
     
   for (CounterA=0; CounterA<WindowData [WindowFoundID].NrWMI1ButtonsAllocated; CounterA++)
   {          
     if ( (WindowData [WindowFoundID].ButtonWMI1 [CounterA].ButtonIsActivated) &&
          (PixelInRect (MBPosX1, MBPosY1, WindowData [WindowFoundID].ButtonWMI1 [CounterA].MBPosX1,
                        WindowData [WindowFoundID].ButtonWMI1 [CounterA].MBPosY1,
                        WindowData [WindowFoundID].ButtonWMI1 [CounterA].MBPosX2,
                        WindowData [WindowFoundID].ButtonWMI1 [CounterA].MBPosY2) ) )       
     {
          ButtonFound=1;
          ButtonFoundID=CounterA;
          
          break;
     }
   }
   
   if (ButtonFound)  // E' stato trovato un tasto WMI1
   {
       INTERNAL_PHOENIX_WMI1_Pressed (ButtonFoundID, WindowFoundID);
       ndHAL_RequestDelay (ND_REQDELAY_WMI1PRESSED);
       
       return 1;             // Informa EventManager che è necessario iniziare un nuovo ciclo mouse  
   }
   
   
   
   // A questo punto controlliamo se sono stati premuti tasti WMI2Serv
   
   SSPosX1 = MousePointer_RPosX - WindowData [WindowFoundID].AA3_X1;
   SSPosY1 = MousePointer_RPosY - WindowData [WindowFoundID].AA3_Y1;
   
   ButtonFound=0;
   for (CounterA=0; CounterA<2; CounterA++)
   {          
         if ( (WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].ButtonIsActivated) &&
              (PixelInRect (SSPosX1, SSPosY1, WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].SSPosX1,
                            WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].SSPosY1,
                            WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].SSPosX2,
                            WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].SSPosY2) ) )       
         {
              ButtonFound=1;
              ButtonFoundID=CounterA;
              
              break;
         }
   }
   
   
   
   if (ButtonFound)              // E' stato premuto un pulsante WMI2Serv
   {
      switch (ButtonFoundID)
      {
             case SCROLL_WMI2_TO_UP:
                  {
                     INTERNAL_PHOENIX_Scroll_WMI2_To_Up (WindowFoundID);
                     return 1;     // Informa EventManager che è necessario iniziare un nuovo ciclo mouse             
                  }
             case SCROLL_WMI2_TO_DOWN:
                  {
                     INTERNAL_PHOENIX_Scroll_WMI2_To_Down (WindowFoundID);
                     return 1;     // Informa EventManager che è necessario iniziare un nuovo ciclo mouse
                  }
      }   
   }
   
   // Non ci resta che controllare se è stato premuto un tasto WMI2
   
   ButtonFound=0;
   for (CounterA=0; CounterA<WindowData [WindowFoundID].NrWMI2ButtonsAllocated; CounterA++)
   {          
         if ( (WindowData [WindowFoundID].ButtonWMI2 [CounterA].ButtonIsActivated) &&
              (PixelInRect (SSPosX1, SSPosY1, WindowData [WindowFoundID].ButtonWMI2 [CounterA].SSPosX1,
                            WindowData [WindowFoundID].ButtonWMI2 [CounterA].SSPosY1,
                            WindowData [WindowFoundID].ButtonWMI2 [CounterA].SSPosX2,
                            WindowData [WindowFoundID].ButtonWMI2 [CounterA].SSPosY2) ) )       
         {
              ButtonFound=1;
              ButtonFoundID=CounterA;
              
              break;
         }
   }
   
   if (ButtonFound)  // E' stato trovato un tasto WMI2
   {
       INTERNAL_PHOENIX_WMI2_Pressed (ButtonFoundID, WindowFoundID);
       ndHAL_RequestDelay (ND_REQDELAY_WMI2PRESSED);
       
       return 1;     // Informa EventManager che è necessario iniziare un nuovo ciclo mouse        
   }
   
     
   return 0; 
   // Informa EventManager che è necessario continuare l'analisi con i pulsanti classe 4                 
}




/* 
    ***************************************************************************
    ***************************************************************************
                            EVENT MANAGER   (Phoenix compatibile)
    ***************************************************************************
    ***************************************************************************
*/

inline void INTERNAL_PHOENIX_EventManager_For_Button1 (struct ndMouseState_Type *MouseState)
/*
    Questa routine è esclusivamente per uso interno. Si occupa di gestire gli eventi
    quando viene premuto il pulsante nr.1 della piattaforma su cui gira 
    Nanodesktop
*/
{
   /* 
      L'algoritmo che si occupa di gestire gli eventi è alquanto complesso. 
      Per prima cosa, è necessario controllare che il mouse, al momento in
      cui viene premuto il pulsante, non fosse su un pulsante di classe 1.
   */
   
   unsigned short int CounterA;
   
   unsigned char ButtonFound, ButtonFoundID;
   unsigned char WindowFoundID;   
   unsigned char WindowInFirstPlaneID;   
   
   unsigned short int WndLenX, WndLenY;
   unsigned short int MousePointer_RPosX, MousePointer_RPosY;
   unsigned short int MousePointer_RRPosX, MousePointer_RRPosY;
   
   static int OLDWindowX1,  OLDWindowY1, OLDWindowX2, OLDWindowY2;
   static int THWindowX1,   THWindowY1,  THWindowX2,  THWindowY2; 
   static int NEWWindowX1,  NEWWindowY1, NEWWindowX2, NEWWindowY2;
   static int NEWWndLength, NEWWndHeight;
   
   static int ChangeX, ChangeY, VectorX, VectorY;
   
   int OLDROIPosX, OLDROIPosY;
   int IsWindowInFirstPlane, PseudoSecondaryWindow, CounterK, CounterM;
   int PosWindowInWV, WindowSpecific;   
   int NewSizeX, NewSizeY;
      
   int SuggestedX, SuggestedY;
   
         
   ButtonFound=0;
   
   
   
   for (CounterA=0; CounterA<__NRWINDOWSALLOCABLE; CounterA++)
   {
       if (ButtonClass1_Data [CounterA].ButtonIsActivated)
       {
           if (  PixelInRect (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, 
                              ButtonClass1_Data [CounterA].x1, ButtonClass1_Data [CounterA].y1,
                              ButtonClass1_Data [CounterA].x2, ButtonClass1_Data [CounterA].y2)  )
           {
                 ButtonFound=1;
                 ButtonFoundID=CounterA;
                 
                 break;
           }
       }
   }
   
   if (ButtonFound)                     // E' stato premuto un pulsante di classe 1
   {
      ndLP_PutElementAtTheTopOfWV (ButtonClass1_Data [ButtonFoundID].ButtonID);
      ndLP_MaximizeWindow (ButtonClass1_Data [ButtonFoundID].ButtonID);
      
      ndHAL_RequestDelay (ND_REQDELAY_MOUSECTRL);     // Sotto PSP è necessario un piccolo ritardo
                                                      // per evitare la massimizzazione multipla
      
      return;
   } 

   /* 
      Se il processo precedente fallisce, allora è necessario verificare se è stato
      premuto un pulsante di classe 2. 
   */

   ButtonFound=0;
   
   for (CounterA=0; CounterA<4; CounterA++)
   {
       if (ButtonClass2_Data [CounterA].ButtonIsActivated)
       {                   
           if (  PixelInRect (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, 
                             ButtonClass2_Data [CounterA].x1, ButtonClass2_Data [CounterA].y1,
                             ButtonClass2_Data [CounterA].x2, ButtonClass2_Data [CounterA].y2)    )                         
                 {      
                   ButtonFound=1;
                   ButtonFoundID=CounterA;
                   
                   break;
                 }
       }
   }
   
   if (ButtonFound)
   {
        WindowInFirstPlaneID = WindowsVector [0];
        
        if (!WindowData [WindowInFirstPlaneID].NoResizeable)
        {               
            switch (ButtonClass2_Data [ButtonFoundID].ButtonID)
            {
                   case ENLARGE_X:
                   {
                            WndLenX = WindowData [WindowInFirstPlaneID].WndLength;
                            WndLenY = WindowData [WindowInFirstPlaneID].WndHeight;
                            
                            NewSizeX=WndLenX+20;
                            NewSizeY=WndLenY;
                            
                            if (!(ndLP_ValidateNewSizeX (NewSizeX, WindowData [WindowInFirstPlaneID].WSWndLength, 
                                          WindowData [WindowInFirstPlaneID].MenuSupported, &SuggestedX)))
                                NewSizeX=SuggestedX;
                            
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, WindowInFirstPlaneID); 
                            break;
                   }
                   
                   case SMALLER_X:
                   {
                            WndLenX = WindowData [WindowInFirstPlaneID].WndLength;
                            WndLenY = WindowData [WindowInFirstPlaneID].WndHeight;
                            
                            NewSizeX=WndLenX-20;
                            NewSizeY=WndLenY;
                            
                            if (!(ndLP_ValidateNewSizeX (NewSizeX, WindowData [WindowInFirstPlaneID].WSWndLength,
                                      WindowData [WindowInFirstPlaneID].MenuSupported, &SuggestedX)))
                             NewSizeX=SuggestedX;
                            
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, WindowInFirstPlaneID);
                            break;      
                   }
                   
                   case ENLARGE_Y:
                   {
                            WndLenX = WindowData [WindowInFirstPlaneID].WndLength;
                            WndLenY = WindowData [WindowInFirstPlaneID].WndHeight;
                            
                            NewSizeX=WndLenX;
                            NewSizeY=WndLenY+20;
                            
                            if (!(ndLP_ValidateNewSizeY (NewSizeY, WindowData [WindowInFirstPlaneID].WSWndHeight, 
                                     WindowData [WindowInFirstPlaneID].MenuSupported, &SuggestedY)))
                             NewSizeY=SuggestedY;
                            
                            
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, WindowInFirstPlaneID);
                            break;
                   }
                   
                   case SMALLER_Y:
                   {
                            WndLenX = WindowData [WindowInFirstPlaneID].WndLength;
                            WndLenY = WindowData [WindowInFirstPlaneID].WndHeight;
                            
                            NewSizeX=WndLenX;
                            NewSizeY=WndLenY-20;
                            
                            if (!(ndLP_ValidateNewSizeY (NewSizeY, WindowData [WindowInFirstPlaneID].WSWndHeight, 
                                     WindowData [WindowInFirstPlaneID].MenuSupported, &SuggestedY)))
                             NewSizeY=SuggestedY;
                            
                            
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, WindowInFirstPlaneID);
                            break;    
                   }
          
            }
       
            return;
            
        }   // End of "if (!WindowData [WindowInFirstPlaneID].NoResizeable)"
        
        
   }
   
   /*
   A questo punto si passa a verificare se il puntatore del mouse ha toccato qualche
   finestra che non è in primo piano. Infatti, se fosse così, il sistema provvederà
   a portare la finestra che è stata toccata in primo piano.
   */
   
   INTERNAL_FindOwnerOfPixel (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, &WindowFoundID);
   
   if (WindowFoundID!=255)                    // Il pixel appartiene ad una finestra
   {
      
      WindowInFirstPlaneID = WindowsVector [0];
      IsWindowInFirstPlane = 0;
     
      if (WindowInFirstPlaneID == WindowFoundID)
         IsWindowInFirstPlane=1;
      else
         {
           // Se la finestra non è in primo piano è necessario verificare se siamo in presenza di una finestra pseudo-secondaria.
           // Le finestre pseudo-secondarie, sono quelle finestre che non sono in primo piano ma,
           // poichè non sono coperte da alcuna finestra di livello superiore, appaiono al
           // sistema come finestre in primo piano. 
     
           PseudoSecondaryWindow=1;          // Partiamo con un'ipotesi ottimistica. Essa dovrà essere confermata da tutti 
                                             // i cicli di analisi successiva
           
           for (CounterK=0; CounterK<__NRWINDOWSALLOCABLE; CounterK++)
           {
               if (WindowsVector [CounterK] == WindowFoundID) PosWindowInWV=CounterK;
           }
                                
           for (CounterM=0; CounterM<PosWindowInWV; CounterM++)
           {
              WindowSpecific = WindowsVector [CounterM]; 
               
              if (WindowData [WindowSpecific].WindowIsActive)
              {
                 if (! (INTERNAL_RettangoliDisgiunti (WindowData [WindowSpecific].WndLimitX1, WindowData [WindowSpecific].WndLimitY1,
                                              WindowData [WindowSpecific].WndLimitX2, WindowData [WindowSpecific].WndLimitY2,
                                              WindowData [WindowFoundID].WndLimitX1, WindowData [WindowFoundID].WndLimitY1,
                                              WindowData [WindowFoundID].WndLimitX2, WindowData [WindowFoundID].WndLimitY2) ) )
              
                 {
                     PseudoSecondaryWindow = 0;
                 }
              }                  
           }
         }
              
      if ( (!(IsWindowInFirstPlane)) && (!(PseudoSecondaryWindow)) )   // Il pixel appartiene ad una finestra non in primo piano
      {
         ndLP_PutWindowInFirstPlane (WindowFoundID);
         
         return;
      }
      else                            
      {
         /* Se il pixel appartiene ad una finestra in primo piano, o a una finestra pseudo-secondaria, 
            è necessario verificare se è stato toccato un pulsante di classe 3 */
            
         MousePointer_RPosX = HAL_MouseEmu_PosX - WindowData [WindowFoundID].WndLimitX1;
         MousePointer_RPosY = HAL_MouseEmu_PosY - WindowData [WindowFoundID].WndLimitY1;
         
         ButtonFound=0;
         
         for (CounterA=0; CounterA<7; CounterA++)
         {
             if ( (WindowData [WindowFoundID].ButtonClass3_Data [CounterA].ButtonIsActivated) &&
                  (PixelInRect (MousePointer_RPosX, MousePointer_RPosY, 
                               WindowData [WindowFoundID].ButtonClass3_Data [CounterA].x1,
                               WindowData [WindowFoundID].ButtonClass3_Data [CounterA].y1,
                               WindowData [WindowFoundID].ButtonClass3_Data [CounterA].x2,
                               WindowData [WindowFoundID].ButtonClass3_Data [CounterA].y2) ) )       
             {
                  ButtonFound=1;
                  ButtonFoundID=CounterA;
                  
                  break;
             }
         }
         
         if (ButtonFound)                // E' stato trovato un pulsante di classe 3
         {
            switch (WindowData [WindowFoundID].ButtonClass3_Data [ButtonFoundID].ButtonID) 
            {
                case MINIMIZEWINDOW_BUTTON:
                {
                    ndLP_MinimizeWindow (WindowFoundID);
                    
                    return;
                }
                
                case TITLEBAR_BUTTON:
                {    
                     if ((MouseState->VectorX!=0) || (MouseState->VectorY!=0))
                     {
                             // Salva VectorX e VectorY
                             VectorX = MouseState->VectorX;
                             VectorY = MouseState->VectorY;
                             
                             // Esegui il lock del Phoenix Graph Thread
                             ndHAL_LockPhoenixGraphThread ();
                             
                             // Imposta le coordinate della vecchia finestra
                             OLDWindowX1 = WindowData [WindowFoundID].WndLimitX1;
                             OLDWindowY1 = WindowData [WindowFoundID].WndLimitY1;
                             OLDWindowX2 = WindowData [WindowFoundID].WndLimitX2;
                             OLDWindowY2 = WindowData [WindowFoundID].WndLimitY2;
                             
                             // Salva le impostazioni della vecchia finestra
                             NEWWndLength = WindowData [WindowFoundID].WndLength;
                             NEWWndHeight = WindowData [WindowFoundID].WndHeight;
                             
                             // Esegui l'unlock del Phoenix Graph Thread
                             ndHAL_UnlockPhoenixGraphThread ();
                             
                             // Imposta di default le coordinate della nuova finestra
                             // alle vecchie: queste impostazioni saranno cambiate in
                             // un secondo momento dal programma, se necessario
                             
                             NEWWindowX1  = OLDWindowX1;
                             NEWWindowY1  = OLDWindowY1;
                             NEWWindowX2  = OLDWindowX2;
                             NEWWindowY2  = OLDWindowY2;
                             
                             // Inizia l'elaborazione
                             
                             if (VectorX<0)            // E' stato richiesto uno spostamento a destra
                             {
                                 THWindowX1 = OLDWindowX1 + VectorX;
                                 THWindowX2 = THWindowX1  + NEWWndLength;
                                 
                                 if (THWindowX1>=0)
                                 {
                                    NEWWindowX1 = THWindowX1;
                                    NEWWindowX2 = THWindowX2;
                                 }
                                 else           // La finestra andrebbe fuori schermo
                                 {
                                    NEWWindowX1 = 0;
                                    NEWWindowX2 = NEWWndLength;
                                 }
                                 
                             }
                             
                             if (VectorX>0)      // E' stato richiesto uno spostamento a sinistra
                             {
                                 THWindowX1 = OLDWindowX1 + VectorX;
                                 THWindowX2 = THWindowX1  + NEWWndLength;
                                 
                                 if (THWindowX2<Windows_MaxScreenX)
                                 {
                                    NEWWindowX1 = THWindowX1;
                                    NEWWindowX2 = THWindowX2;
                                 }
                                 else           // La finestra andrebbe fuori schermo
                                 {
                                    NEWWindowX1 = Windows_MaxScreenX - 1 - NEWWndLength;
                                    NEWWindowX2 = Windows_MaxScreenX - 1;
                                 }
                             }
                             
                             if (VectorY<0)            // E' stato richiesto uno spostamento in alto
                             {
                                 THWindowY1 = OLDWindowY1 + VectorY;
                                 THWindowY2 = THWindowY1  + NEWWndHeight;
                                 
                                 if (THWindowY1>=0)
                                 {
                                    NEWWindowY1 = THWindowY1;
                                    NEWWindowY2 = THWindowY2;
                                 }
                                 else           // La finestra andrebbe fuori schermo
                                 {
                                    NEWWindowY1 = 0;
                                    NEWWindowY2 = NEWWndHeight;
                                 }
                             }
                             
                             if (VectorY>0)      // E' stato richiesto uno spostamento in basso
                             {
                                 THWindowY1 = OLDWindowY1 + VectorY;
                                 THWindowY2 = THWindowY1  + NEWWndHeight;
                                 
                                 if (THWindowY2<Windows_MaxScreenY)
                                 {
                                    NEWWindowY1 = THWindowY1;
                                    NEWWindowY2 = THWindowY2;
                                 }
                                 else           // La finestra andrebbe fuori schermo
                                 {
                                    NEWWindowY1 = Windows_MaxScreenY - 1 - NEWWndHeight;
                                    NEWWindowY2 = Windows_MaxScreenY - 1;
                                 }
                             }
                             
                             ChangeX = NEWWindowX1 - OLDWindowX1;
                             ChangeY = NEWWindowY1 - OLDWindowY1;
                             
                             ndLP_MoveWindow (NEWWindowX1, NEWWindowY1, WindowFoundID);
                             
                             HAL_MouseEmu_PosX = HAL_MouseEmu_PosX + ChangeX;
                             HAL_MouseEmu_PosY = HAL_MouseEmu_PosY + ChangeY; 
                     }
                                                     
                     return;
                }  

                case SCROLL_LEFT_BUTTON:
                {
                     OLDROIPosX = WindowData [WindowFoundID].ROI_PosX;
                     OLDROIPosY = WindowData [WindowFoundID].ROI_PosY;
                
                     ndLP_ChangeWindowROI (OLDROIPosX - 20, OLDROIPosY, ROI_SMART, WindowFoundID);
                     
                     return;
                }
             
                case SCROLL_RIGHT_BUTTON:
                {
                     OLDROIPosX = WindowData [WindowFoundID].ROI_PosX;
                     OLDROIPosY = WindowData [WindowFoundID].ROI_PosY;
                     
                     ndLP_ChangeWindowROI (OLDROIPosX + 20, OLDROIPosY, ROI_SMART, WindowFoundID);
                     
                     return;
                }
    
                case SCROLL_UP_BUTTON:
                {
                     OLDROIPosX = WindowData [WindowFoundID].ROI_PosX;
                     OLDROIPosY = WindowData [WindowFoundID].ROI_PosY;
                     
                     ndLP_ChangeWindowROI (OLDROIPosX, OLDROIPosY - 20, ROI_SMART, WindowFoundID);
                     
                     return;
                }
             
                case SCROLL_DOWN_BUTTON:
                {
                     OLDROIPosX = WindowData [WindowFoundID].ROI_PosX;
                     OLDROIPosY = WindowData [WindowFoundID].ROI_PosY;
                     
                     ndLP_ChangeWindowROI (OLDROIPosX, OLDROIPosY + 20, ROI_SMART, WindowFoundID);
                     
                     return;
                }

			    case CLOSEWINDOW_BUTTON:
                {
                     INTERNAL_PHOENIX_ExecuteCloseWndCallback_L0 (WindowData [WindowFoundID].PntToCloseWndCallback, 
                                                                  WindowData [WindowFoundID].WndInfoField, 
                                                                  WindowFoundID, 
                                                                  WindowData [WindowFoundID].NoNotifyToMouseControl);
                     
                     return;
                }
                
            }              // Chiude lo switch
          }                // Chiude if ButtonFound
      
          // Se il pixel non appartiene ad alcun pulsante di classe 3 si può verificare se
          // esso appartiene ad un pulsante virtuale WMI, ossia ad un pulsante dei menù.
          // A tale scopo, si richiama la routine specializzata WMIMouseControl.
          
          
          if (WindowData [WindowFoundID].MenuSupported)
          { 
             
             if (INTERNAL_PHOENIX_WMIEventManager (MousePointer_RPosX, MousePointer_RPosY, WindowFoundID, MouseState)) return;
             // Se la routine restituisce 1, allora ci sono due possibilità:
             //    a) l'utente ha premuto un pulsante di tipo WMIServ1, WMIServ2, WMI1, WMI2 che
             //       ha effettuato l'esecuzione delle callback. In questo caso si deve uscire
             //       da EventManager. Si noti che in modalità Phoenix al momento dell'uscita
             //       sarà già avvenuto il dispatch dei messaggi
             //    b) l'utente ha premuto un pulsante di tipo WMI1 o WMI2 e questo ha provocato
             //       l'esecuzione di una callback, o comunque l'esecuzione di un messaggio. 
             //       Anche in questo caso si deve uscire da EventManager
             //
             //    Se la routine restituisce 0, significa semplicemente che non è stato trovato
             //    nessun pulsante WMI1, WMI2, WMI1Serv o WMI2Serv e quindi bisogna proseguire
             //    la fase di ricerca con i pulsanti di classe 4 (ossia i pulsanti custom
             //    definiti dall'utente. 
          }
          
          
          MousePointer_RRPosX = MousePointer_RPosX - WindowData [WindowFoundID].AA3_X1 + WindowData [WindowFoundID].ROI_PosX;
          MousePointer_RRPosY = MousePointer_RPosY - WindowData [WindowFoundID].AA3_Y1 + WindowData [WindowFoundID].ROI_PosY;
          // Adesso in MousePointer_RRPosX e MousePointer_RRPosY c'è la posizione nel windows space della finestra che
          // corrisponde al punto dove si trova il puntatore del mouse.
          
          ButtonFound=0;
          
          for (CounterA=0; CounterA<__NRBUTTONSALLOCABLE; CounterA++)
          {
             if ( WindowData [WindowFoundID].ButtonClass4_Data [CounterA].ButtonIsActivated )
             {
                  if (!WindowData [WindowFoundID].ButtonClass4_Data [CounterA].ButtonIsInhibited)
                  {
                      if (  PixelInRect (MousePointer_RRPosX, MousePointer_RRPosY, 
                                         WindowData [WindowFoundID].ButtonClass4_Data [CounterA].RRPosX1,
                                         WindowData [WindowFoundID].ButtonClass4_Data [CounterA].RRPosY1,
                                         WindowData [WindowFoundID].ButtonClass4_Data [CounterA].RRPosX2,
                                         WindowData [WindowFoundID].ButtonClass4_Data [CounterA].RRPosY2)  )
                      {
                           ButtonFound=1;
                           ButtonFoundID=CounterA;
                           
                           break;
                      }
                  }
             }
          }
          
          
          if (ButtonFound)
          {
             // Richiama la routine che gestisce la pressione di un tasto
             
             char __TraspCallback = ((WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].ButtonStyle & ND_TRASPARENT_CALLBACK) == ND_TRASPARENT_CALLBACK);
             char __NewContext    = ((WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].ButtonStyle & ND_CALLBACK_IN_NEW_CONTEXT) == ND_CALLBACK_IN_NEW_CONTEXT);
              
             INTERNAL_PHOENIX_ExecuteButtonCallback_L0 (WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].Callback, 
                                                         &(WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].StringID), 
                                                           WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].InfoField, 
                                                           WindowFoundID, ButtonFoundID, __TraspCallback, __NewContext);
                                                           
             return;             
          }
          
          
          
          // Se siamo arrivati a questo punto del controllo, vuol dire che il pixel appartiene ad
          // una finestra pseudo-secondaria o in primo piano, ma non appartiene a nessun pulsante.
          // Per le finestre pseudo-secondarie, si esegue un ulteriore operazione: la finestra viene
          // portata al top del window vector (sullo schermo non si vedrà nessun cambiamento, ma
          // sarà possibile per l'utente selezionare le opzioni di ingrandimento finestra).
          
          if (PseudoSecondaryWindow)
          {
             ndLP_PutWindowInFirstPlane (WindowFoundID);
      
             return;                         
          }
                    
                
      }                    // Chiude l'else per la finestra non in primo piano
   }                       // Chiude if foundID !=255
      
}



inline void INTERNAL_PHOENIX_EventManager_For_Button4 (struct ndMouseState_Type *MouseState)
/*
    Questa routine è esclusivamente per uso interno. Si occupa di gestire gli eventi
    quando viene premuto il pulsante nr.4 della piattaforma su cui gira 
    Nanodesktop
*/

{
   unsigned char WindowFoundID; 
   char Command; 
   
   unsigned short int NewSizeX, NewSizeY;
   unsigned short int WndLenX, WndLenY;
   
   int SuggestedX, SuggestedY;
   
    
   // Il pulsante nr. 4 della piattaforma viene usato normalmente per eseguire operazioni
   // di resize della finestra. Tutto deve avvenire in modo trasparente alla routine
   // chiamante, quindi la struct EventAddress deve essere posta a NO_EVENT.
   
   if (!ResizerFocusActive)               // Il resizer non ha agganciato alcuna finestra
   {
       INTERNAL_FindOwnerOfPixel (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, &WindowFoundID);
   
       if (WindowFoundID!=255)                    // Il pixel appartiene ad una finestra
       {
           if (!WindowData [WindowFoundID].NoResizeable)
           {
              ResizerFocusActive=1;
              ResizerFocusWindow=WindowFoundID;
           }
       }
   }
   
   if (ResizerFocusActive)                        // Qui il sistema ha effettivamente agganciato
   {                                              // una finestra
       
       // Ora cerca di determinare il comando di resize
       
       Command=0;
       
       if (MouseState->VectorX==-1) Command=SMALLER_X;
       else
       {
           if (MouseState->VectorX==+1) Command=ENLARGE_X;
           else
           {
               if (MouseState->VectorY==-1) Command=SMALLER_Y;
               else
               {
                   if (MouseState->VectorY==+1) Command=ENLARGE_Y;
               }
           }
       }
       
       
       if (Command!=0)     // E' stato trovato un codice di comando corretto
       {
          
            switch (Command)
            {
                   case ENLARGE_X:
                   {
                            WndLenX = WindowData [ResizerFocusWindow].WndLength;
                            WndLenY = WindowData [ResizerFocusWindow].WndHeight;
                            
                            NewSizeX=WndLenX+20;
                            NewSizeY=WndLenY;
                            
                            if (!(ndLP_ValidateNewSizeX (NewSizeX, WindowData [ResizerFocusWindow].WSWndLength, 
                                          WindowData [ResizerFocusWindow].MenuSupported, &SuggestedX)))
                                                     NewSizeX=SuggestedX;
                            
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, ResizerFocusWindow);
                            
                            break;
                   }
                   
                   case SMALLER_X:
                   {
                            WndLenX = WindowData [ResizerFocusWindow].WndLength;
                            WndLenY = WindowData [ResizerFocusWindow].WndHeight;
                            
                            NewSizeX=WndLenX-20;
                            NewSizeY=WndLenY;
                            
                            if (!(ndLP_ValidateNewSizeX (NewSizeX, WindowData [ResizerFocusWindow].WSWndLength,
                                      WindowData [ResizerFocusWindow].MenuSupported, &SuggestedX)))
                                                 NewSizeX=SuggestedX;
                            
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, ResizerFocusWindow);
                            
                            break;      
                   }
                   
                   case ENLARGE_Y:
                   {
                            WndLenX = WindowData [ResizerFocusWindow].WndLength;
                            WndLenY = WindowData [ResizerFocusWindow].WndHeight;
                            
                            NewSizeX=WndLenX;
                            NewSizeY=WndLenY+20;
                            
                            if (!(ndLP_ValidateNewSizeY (NewSizeY, WindowData [ResizerFocusWindow].WSWndHeight, 
                                     WindowData [ResizerFocusWindow].MenuSupported, &SuggestedY)))
                                                NewSizeY=SuggestedY;
                            
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, ResizerFocusWindow);
                            
                            break;
                   }
                   
                   case SMALLER_Y:
                   {
                            WndLenX = WindowData [ResizerFocusWindow].WndLength;
                            WndLenY = WindowData [ResizerFocusWindow].WndHeight;
                            
                            NewSizeX=WndLenX;
                            NewSizeY=WndLenY-20;
                            
                            if (!(ndLP_ValidateNewSizeY (NewSizeY, WindowData [ResizerFocusWindow].WSWndHeight, 
                                     WindowData [ResizerFocusWindow].MenuSupported, &SuggestedY)))
                                                NewSizeY=SuggestedY;
                            
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, ResizerFocusWindow);
                            
                            break;    
                   }
          
            }  // End of switch
                        
       }  // End of command!=0
   
       return;
       
       
   }  // End of "if ResizerFocusActive"
   
}



inline void INTERNAL_PHOENIX_EventManager_For_Button5 (void)
/*
    Questa routine è esclusivamente per uso interno. Si occupa di gestire gli eventi
    quando viene premuto il pulsante nr.5 della piattaforma su cui gira 
    Nanodesktop
*/
{
   static struct ndEvent_Type EventAddress;
   
   EventAddress.__TypeEvent=SPECIAL_KEY_PRESSED;
   EventAddress.__WindowID=0;
   EventAddress.__ButtonID=ND_BUTTON5;
   EventAddress.__CallbackExecuted=0;
   EventAddress.__CallbackAnswer=0;
   
   INTERNAL_PHOENIX_DispatchMessage (&EventAddress);        // Consegna il messaggio
   return;
}


inline void INTERNAL_PHOENIX_NoEventManager (void)
/*
    Esegue delle operazioni di reset qualora non vi siano eventi
*/
{
    ResizerFocusActive=0;
    // Pone a 0 la variabile ResizerFocusActive. Questo farà sì che l'eventuale finestra
    // agganciata dal resizer venga rilasciata.
}













/* 
    ***************************************************************************
    ***************************************************************************
                            MOUSE CONTROL   (Phoenix compatibile)
    ***************************************************************************
    ***************************************************************************
*/


void ndHAL_InitPhoenixMouseControl ()
/*  
     Questa routine provvede a reinizializzare alcune variabili che sono
     necessarie per il funzionamento di MouseControl
*/
{
     int Counter;
     
     for (Counter=0; Counter<__NR_MOUSECTRL_RQ_ALLOWED; Counter++)
     {
         phnMouse_ReqBufferStdArray [Counter].IsEnabled=0;
         phnMouse_ReqBufferStdArray [Counter].IsWaiting=0;
     }
     
     for (Counter=0; Counter<__NR_MOUSECTRL_CBE_ALLOWED; Counter++)
     {
         phnMouse_CbeRegister [Counter].IsEnabled=0;
     }
}


void INTERNAL_PHOENIX_MouseControl_SERVER (struct ndMouseState_Type *MyMouseState)
/*
    Questa routine è il componente server di MouseControl versione Phoenix
    compatibile.
    
    La routine viene richiamata periodicamente dal server PhoenixMouse, e si 
    occupa di gestire gli eventi relativi al mouse in un thread separato
    dagli altri thread definiti dall'utente. 
    
    Per funzionare, la routine ha bisogno delle informazioni sullo stato del pad
    del mouse: tali informazioni vengono passate tramite il puntatore MyMouseState. 
    
    La routine si occupa anche di diramare le informazioni sui messaggi relativi
    ai diversi eventi ai vari threads che ne abbiano fatto richiesta
*/

{
   // VERSIONE PER PSP/PSP KSU: si usa il metodo della consegna parallela
   
   char Button1Pressed, Button4Pressed, Button5Pressed;
   
   Button1Pressed = ((MyMouseState->Buttons & ND_BUTTON1)==ND_BUTTON1);
   Button4Pressed = ((MyMouseState->Buttons & ND_BUTTON4)==ND_BUTTON4);
   Button5Pressed = ((MyMouseState->Buttons & ND_BUTTON5)==ND_BUTTON5);
   
   if (Button1Pressed)
   {
       INTERNAL_PHOENIX_EventManager_For_Button1 (MyMouseState);
   }
   else
   {
       if (Button4Pressed)
       {
           INTERNAL_PHOENIX_EventManager_For_Button4 (MyMouseState); 
       }
       else
       {
           if (Button5Pressed)
           {
                INTERNAL_PHOENIX_EventManager_For_Button5 ();               
           }
           else     // Nessuno dei tasti è stato premuto
           {
                INTERNAL_PHOENIX_NoEventManager ();      
           }
       }
   }
   
   
}



void INTERNAL_PHOENIX_MouseControl_CLIENT (struct ndEvent_Type *EventAddress)
/*
    Questa routine costituisce il componente client del sistema client/server
    alla base della versione di MouseControl, compatibile Phoenix.
    
    Il thread chiamante sarà arrestato fino a quando non verrà diramato un
    messaggio dal parte del client.
    
    Per la descrizione dei messaggi, si veda la documentazione di MouseControl
    in ndCODE_MouseCtrl.c
*/
{
     char IHaveFound;
     int  Counter, HandleFound, EvResult;
     
     // Inizializza il contenuto della struttura che dovrà accogliere il messaggio
     
     EventAddress->__TypeEvent=NO_EVENT;
     EventAddress->__WindowID=0;
     EventAddress->__ButtonID=0;
     EventAddress->__CallbackExecuted=0;
     EventAddress->__CallbackAnswer=0;
   
     strcpy ( &(EventAddress->__StringID), "");
     strcpy ( &(EventAddress->__ZString),  "");
   
     // E' necessario memorizzare la richiesta dentro il buffer richieste            
                                    
     ndHAL_EnableMousePointer ();                  // Riattiva il puntatore del mouse 
     
     IHaveFound=0;
     
     while (!IHaveFound)
     {
           sceKernelWaitEventFlag  (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE, PSP_EVENT_WAITOR, &EvResult, 0); 
           // Attendi che il buffer si liberi 
    
           sceKernelClearEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
           // Impedisce che altri thread possano scrivere nel buffer eventi
      
           sceKernelSuspendThread (phnMouse_ThreadID);    
           // Sospendi per sicurezza il componente server
   
           // Cerca un handle libero nel buffer dei messaggi.
           Counter    =0;
     
           while (Counter<__NR_MOUSECTRL_RQ_ALLOWED)
           {
                   if (!phnMouse_ReqBufferStdArray [Counter].IsEnabled)
                   {
                        HandleFound = Counter;
                        IHaveFound  = 1;
                        break;                           
                   }
                   
                   Counter++;
           }
           
           // Arrivati a questo punto ci sono due possibilità: o a trovato un handle
           // libero o non lo ha trovato. Ipotizziamo che non sia stato trovato un
           // handle libero
           
           if (!IHaveFound)
           {          
               sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
               // Impedisce che altri thread possano scrivere nel buffer eventi
      
               sceKernelResumeThread (phnMouse_ThreadID);     // Riavvia il server di MouseControl  
               sceKernelDelayThread (1000000);                // Ritardo di un secondo per consentire al 
                                                              // sistema di superare l'overflow del buffer
               continue;
           }
     }      
           
     // Se il sistema esce dal loop precedente vuol dire che è stato trovato un handle libero      
             
     if (IHaveFound)            // Ho trovato un handle libero
     {
         phnMouse_ReqBufferStdArray [HandleFound].IsEnabled = 1;
         phnMouse_ReqBufferStdArray [HandleFound].IsWaiting = 0;  
     
         // Registra l'indirizzo della struct dove sarà necessario memorizzare il messaggio e 
         // l'id del thread 
     
         phnMouse_ReqBufferStdArray [HandleFound].AddressData                     = (int)(EventAddress);
         phnMouse_ReqBufferStdArray [HandleFound].ThreadID_AuthorOfRequest        = sceKernelGetThreadId ();

         // Registrazione effettuata
     
         sceKernelSetEventFlag (phnMouse_Ev0_CbeRegister, PHNMOUSE_CBEREG_IS_FREE);
         // Impedisce che altri thread possano scrivere nel buffer eventi
      
         sceKernelResumeThread (phnMouse_ThreadID);      
         // Riavvia il server di MouseControl                                                                                
     
         sceKernelSleepThread ();
         // Sospende il thread che ha eseguito la chiamata: durante questa fase il controllo sarà
         // gestito esclusivamente dal phoenix mouse thread e da eventuali altri thread.
         // Sarà compito del Phoenix Mouse Thread riattivare questo thread quando ci sarà
         // un messaggio da elaborare
     }
      
}
