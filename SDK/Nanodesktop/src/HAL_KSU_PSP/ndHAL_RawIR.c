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
    FUNZIONI PER GENERAZIONE SEGNALI RAWIR (PSP KSU mode)
    ---------------------------------------------------------------------------
    Queste funzionalità sono state create per supportare il servizio RawIR
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

typedef enum { false, true } bool;



/*
    ----------------------------------------------------------------------------
    Simboli per gestione RAWIR
    ----------------------------------------------------------------------------
*/

extern int sceKernelWaitEventFlag(int evid, u32 bits, u32 wait, u32 *outBits, SceUInt *timeout);
extern int sceKernelClearEventFlag(SceUID evid, u32 bits);

// Routines del kernel o simboli del kernel. Gli indirizzi delle routines saranno determinati da FindProc
// dinamicamente
static u32 (*sceKernelSuspendIntr)(u32 r4);
static u32 (*sceKernelResumeIntr)(u32 r4);
static void (*sceSysregSircsIoEnable)();
static void (*sceSysregSircsIoDisable)();
static void (*sceGpioPortClear)(int r4);
static void (*sceGpioPortSet)(int r4);
static void (*sceSysreg_driver_0x6417cdd6)();
static void (*sceSysreg_driver_0x20388c9e)();
static u8 *pDriver;


typedef struct _RAW_IR_DATA
{
    int timing_val;
        // used for carrier frequency (freq = timing_val * 2 kHz)
        //  and for PWM sample ticks (1 tick = timing_val * 1.25 us)
    const u32* raw_buffer;
    int raw_count;
} RAW_IR_DATA;


// SIRCS Interrupt Handler glue

// The built-in SIRCS interrupt handler is very close to what we need,
//  so we use it as-is
//  We directly access the system globals used by the handler

struct SIRCS_HANDLER_DATA // DO NOT CHANGE
{
    // global data used by the system (at address 0x880d6328 in firmware 1.00)
    u32 data0; // not used
    u32 clToSend;
    u32 clDataSent;
    const u32* plDataNext;
    u32 semaId;
    u32 eventFlag;
    u32 active;     // 0 normally, set to 1 when handler needed
}; // DO NOT CHANGE

static struct SIRCS_HANDLER_DATA volatile* g_sircsHandlerData;

static RAW_IR_DATA* IRServerData_Pointer;
static int IRServerData_NrSequences;

static int IRServerData_Waiting = 0;
static int IRServer_Busy = 0;



/*
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                   funzioni ndFindSemaphore e ndFindEventFlag
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    Questa funzione si occupa di trovare l'indirizzo di un semaforo nella ram
    del kernel 
    
    La funzione è derivata dal codice dell'utility PSARDumper di PSP-Pet
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/

#define MAX_UIDS    200     /* overkill max number expected */

static SceUID ndFindEventFlag(const char* szFind)
{
    SceUID uids[MAX_UIDS];
	int count = 0;
	sceKernelGetThreadmanIdList(3/*event*/, uids, sizeof(uids), &count);
    int i;
    for (i = 0; i < count; i++)
    {
        SceUID uid = uids[i];
		SceKernelEventFlagInfo info;
	    info.size = sizeof(info);
	    int err = sceKernelReferEventFlagStatus(uid, &info);
        if (err == 0 && strcmp(info.name, szFind) == 0)
            return uid;
    }
    return 0;
}

static SceUID ndFindSemaphore(const char* szFind)
{
    SceUID uids[MAX_UIDS];
	int count = 0;
	sceKernelGetThreadmanIdList(2/*sema*/, uids, sizeof(uids), &count);
    int i;
    for (i = 0; i < count; i++)
    {
        SceUID uid = uids[i];
		SceKernelSemaInfo info;
	    info.size = sizeof(info);
	    int err = sceKernelReferSemaStatus(uid, &info);
        if (err == 0 && strcmp(info.name, szFind) == 0)
            return uid;
    }
    return 0;
}




/*
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                               funzioni ndFindProc
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    Questa funzione si occupa di determinare l'indirizzo di una routine del
    kernel a partire dal NID e senza usare il metodo statico (stub.s) 
    
    La funzione è derivata dal codice dell'utility PSARDumper di PSP-Pet
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/

static bool ndFindProcErrorStatus = false;

static void* ndFindProc(const char* szMod, const char* szLib, u32 nid)
{         
    SceModule* modP = sceKernelFindModuleByName (szMod);
    
    /*
         Si faccia attenzione a quanto viene fatto qui: la funzione richiama 
         sceKernelFindModuleByName che fa parte della libreria LoadCoreForKernel.
         
         Questa libreria può essere linkata esclusivamente in applicazioni PSP
         che hanno accesso alla memoria del kernel, diversamente si verifica
         un crash.
    
         Per questa ragione, il manuale utente di Nanodesktop specifica che se
         si vuole usare la modalità KSU di Nanodesktop è necessario
         specificare nel codice il tag ND_KERNELSERVICES_TO_USER_MAIN,
         che richiama la corretta macro di piattaforma, oltre a definire
         una serie di variabili HWMODE.
    */
         
      
    if (modP == NULL)
    {
        //printf("Failed to find mod '%s'\n", szMod);
		ndFindProcErrorStatus = true;
        return 0;
    }
    
    SceLibraryEntryTable* entP = (SceLibraryEntryTable*)modP->ent_top;
    while ((u32)entP < ((u32)modP->ent_top + modP->ent_size))
    {
        if (entP->libname != NULL && strcmp(entP->libname, szLib) == 0)
        {
            // found lib
            int i;
            int count = entP->stubcount; // NOT: + entP->vstubcount;
            u32* nidtable = (u32*)entP->entrytable;
            for (i = 0; i < count; i++)
            {
                if (nidtable[i] == nid)
                {
                    u32 procAddr = nidtable[count+i];
                    // printf("entry found: '%s' '%s' = $%x\n", szMod, szLib, (int)procAddr);
                    return (void*)procAddr;
                }
            }
            
            //printf("Found mod '%s' and lib '%s' but not nid=$%x\n", szMod, szLib, nid);
			ndFindProcErrorStatus = true;
            return 0;
        }
        entP++;
    }
 
}







/*
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                          funzione di reinitializzazione RAWIR                                                     
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/




void INTERNAL_Init_RawIrdaSystem ()
/* 
    Questa routine si occupa di reinizializzare il sottosistema IR RAW della PSP. 
    La routine viene eseguita esclusivamente dal thread principale in kernel mode. 
    Vengono aggiornati i puntatori a funzione ai rispettivi indirizzi nel firmware,
    tenendo conto del modulo e del NID. La conversione viene eseguita per mezzo
    della routine ndFindProc.
    
    Il risultato verrà posto nella variabile di ambiente RawIRFailure
*/
{
   if (!ND_HWMODE_DisableRawIRService)
   {     
                sceKernelSuspendIntr = ndFindProc("sceInterruptManager", "InterruptManagerForKernel", 0x750e2507);
              	sceKernelResumeIntr = ndFindProc("sceInterruptManager", "InterruptManagerForKernel", 0x494d6d2b);
                    // also in ThreadManForUser
            
            	sceSysregSircsIoEnable = ndFindProc("sceSYSREG_Driver", "sceSysreg_driver", 0x4c49a8bc);
            	sceSysregSircsIoDisable = ndFindProc("sceSYSREG_Driver", "sceSysreg_driver", 0x26fa0928);
            	sceGpioPortSet = ndFindProc("sceGPIO_Driver", "sceGpio_driver", 0x310f0ccf);
            	sceGpioPortClear = ndFindProc("sceGPIO_Driver", "sceGpio_driver", 0x103c3eb2);
            	sceSysreg_driver_0x6417cdd6 = ndFindProc("sceSYSREG_Driver", "sceSysreg_driver", 0x6417cdd6);
            	sceSysreg_driver_0x20388c9e = ndFindProc("sceSYSREG_Driver", "sceSysreg_driver", 0x20388c9e);
                	 
                pDriver = ndFindProc("sceSIRCS_IrDA_Driver", "sceSircs_driver", 0x19155a2f);
                
                if (!ndFindProcErrorStatus)         // Non ci sono stati errori nel findproc sui moduli precedenti
                {
                    g_sircsHandlerData = (struct SIRCS_HANDLER_DATA*)(pDriver+ 0x19B8);
                    // 0x19B8 offset same for 1.0 and 1.50 firmware
            	
                    // sanity check
            	    if ( (g_sircsHandlerData->eventFlag == ndFindEventFlag("SceSIRCS")) && (g_sircsHandlerData->semaId == ndFindSemaphore("SceSIRCS")) )
            	        RawIRFailure=0;        // Nessun errore
                    else
                        RawIRFailure=-2;       // Binding dinamico sul driver SIRCS non riuscito
                }
                else
                  RawIRFailure=-1;             // Errore in uno dei findproc precedenti
   }
   else  
        RawIRFailure=0;     // In realtà verrà ignorato perchè il sistema Raw Irda qui è disattivato            
}


/* 
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                              Funzioni monitor per Raw-IR
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/

char ndHAL_IR_EnableIRRawMonitor ()
/*
    Questa routine provvede ad attivare il monitor per i segnali raw-ir. 
    
    Sono previsti i seguenti codici di errore:
         ERR_INTERNAL_ERROR         Errore interno
*/
{
    if (!RawIRMonitorEnabled)
    {
        RawIRMonitorHandle = ndLP_CreateWindow (130, 0, 450, 250, "RawIR Monitor", COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_WHITE, 0);
                            
        if (RawIRMonitorHandle>=0)       // Non ci sono errori nell'allocazione della nuova finestra
        {
            ndLP_MaximizeWindow (RawIRMonitorHandle);
            RawIRMonitorEnabled=1;
            
            return 0;
        }
        else // Problema nell'allocazione della nuova finestra
          return ERR_INTERNAL_ERROR;
    } 
    
    // Il monitor è già attivato. Simula successo
    
    return 0;
}


void ndHAL_IR_DisableIRRawMonitor ()
/*
    Provvede a disattivare il monitor per i segnali raw-ir
*/
{
     if (RawIRMonitorEnabled) 
     {
        ndLP_DestroyWindow (RawIRMonitorHandle);
        RawIRMonitorEnabled=0;
     }
}


/*
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                     funzioni client/server per l'invio dei dati                                                  
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/

static inline void INTERNAL_ndHAL_RawIrDisableSystemThreads ()
/*
    Questa routine provvede a disabilitare i vari thread di sistema che sono
    attivi in quel momento: questo è necessario perchè durante l'invio del
    segnale Raw-Ir non devono avvenire interruzioni da parte dello scheduler
*/
{
    INTERNAL_PHOENIX_SuspendAllCBEThreads ();
    sceKernelSuspendThread (ndUserMainThreadID);
    sceKernelSuspendThread (ExitController_ThreadID);
    
    if (!ND_SWMODE_DisablePhoenixSubsystem)
    {
        sceKernelSuspendThread (phnGraph_ThreadID);
        
        if (HAL_MouseThreadStatus==2)              // Il mouse thread era abilitato ed attivo
        {
             sceKernelSuspendThread (phnMouse_ThreadID);
             HAL_MouseThreadStatus=3;              // Condizione speciale che indicherà in seguito al sistema
                                                   // che è necessario ripristinare il server Phoenix Mouse                                                                                        
        }                                                                                                                                                                    
    }
    
    if ((!ND_HWMODE_DisableUsbService) && (!ndUsb_IsFailure))
    {
        sceKernelSuspendThread (ndUsb_CommArea.UsbThread);                                                                                                   
    }                                                                                                                                                        
}              

static inline void INTERNAL_ndHAL_ResumeSystemThreads ()
/* 
    Provvede a riavviare i thread sospesi dalla routine precedente.
*/
{
   if ((!ND_HWMODE_DisableUsbService) && (!ndUsb_IsFailure))
   {
        sceKernelResumeThread (ndUsb_CommArea.UsbThread);                                 
   } 
   
   if (!ND_SWMODE_DisablePhoenixSubsystem)
   {
        if (HAL_MouseThreadStatus==3)              // Il mouse thread era abilitato ed attivo ed è stato sospeso
        {
             sceKernelResumeThread (phnMouse_ThreadID);
             HAL_MouseThreadStatus=2;              // Stato precedenti ripristinato                                                                                            
        } 
        
        sceKernelResumeThread (phnGraph_ThreadID);                                                                                 
   } 
   
   sceKernelResumeThread (ExitController_ThreadID);
   sceKernelResumeThread (ndUserMainThreadID); 
   INTERNAL_PHOENIX_ResumeAllCBEThreads ();   
}       




void INTERNAL_ndHAL_SendRawIR_SERVER ()
/*
    Questa routine è il server per l'invio dei dati in modalità raw-ir. 
    
    Esso provvede ad inviare i dati che vengono inviati dal client, che normalmente
    gira in un thread di tipo user-mode. 
    
    Per sapere quando sono pronti i dati, il sistema usa due variabili. 
    Se IR_ServerData_Waiting viene posta ad 1, allora il server provvede all'invio.
    La variabile IRServer_Busy viene posta ad 1 mentre il server è impegnato nella
    trasmissione, in modo che il client possa accorgersene e possa ritardare
    l'invio di ulteriori dati.
*/
{
    if ((IRServerData_Waiting))
    {                 
        
        INTERNAL_ndHAL_RawIrDisableSystemThreads ();
        IRServer_Busy=1;
        
        // take control of hardware & driver
    	g_sircsHandlerData->active = 1;
        int err=sceKernelWaitSema (g_sircsHandlerData->semaId, 1, 0);
        
        if (err != 0)
        {
    		g_sircsHandlerData->active = 0;
            // printf("Error: SIRCS device busy\n");
            return;
        }
        
    	sceSysreg_driver_0x6417cdd6();
    	sceSysregSircsIoEnable();
    	sceGpioPortClear(4);
    	
        // party on the hardware registers!
        volatile u32* RawIR_HwRegisters = (u32*)0xBE340000;
    
        // REVIEW: explore more variations here
        RawIR_HwRegisters[0] = 0xA0;
        RawIR_HwRegisters[1] = IRServerData_Pointer->timing_val;
        RawIR_HwRegisters[3] = 0x7;
        
    	sceKernelSuspendIntr(0xE);
    
        // send loop
        while (IRServerData_NrSequences-- > 0)
        {
    		sceKernelClearEventFlag(g_sircsHandlerData->eventFlag, -1);
    		g_sircsHandlerData->plDataNext = IRServerData_Pointer->raw_buffer;
    		g_sircsHandlerData->clToSend = IRServerData_Pointer->raw_count;
            g_sircsHandlerData->clDataSent = 0;
            
            // pre-fill hardware fifo - usually fills at 16, interrupts after 8
    		while ((RawIR_HwRegisters[4] & 0x20) == 0)
            {
    		    RawIR_HwRegisters[0x14/4] = *(g_sircsHandlerData->plDataNext)++;
    	        g_sircsHandlerData->clDataSent++;
            }
            
            RawIR_HwRegisters[0] |= 0x1200;
                // start the hardware to do its thing,
                //  and the interrupt handler will do the rest
    
            // wait for completion
    	    u32 result = 0;
    		sceKernelWaitEventFlag(g_sircsHandlerData->eventFlag, 1, 0x11, &result, 0);
        }
    
    	// clean up
    	sceKernelResumeIntr(0xE);
    	sceGpioPortSet(4);
    	sceSysreg_driver_0x20388c9e();
    	sceSysregSircsIoDisable();
    
        // give up control of hardware & driver
    	g_sircsHandlerData->active = 0;
    	sceKernelSignalSema(g_sircsHandlerData->semaId, 1);
    	
        ndHAL_HardDelay (0.02);
     
        IRServer_Busy=0;	
        IRServerData_Waiting=0;
     
        INTERNAL_ndHAL_ResumeSystemThreads ();
        
        // Riattiva il thread che ha richiesto l'emissione di un segnale Raw-IR
        sceKernelResumeThread (ndRawIRThreadRq);
    }
}


static inline u32 INTERNAL_IR_SIGNAL (int Value, int NegLogic, int ticks)
{
   int RealValue;
   
   if (!NegLogic) RealValue=Value;
   else RealValue=1-Value;
   
   if (RealValue==1)
      return ticks | 0x10000;
   else
      return ticks;   
}

char INTERNAL_ndHAL_SendRawIR_CLIENT (int Value, char NegLogic, char BitNumber, char MoreSignif, char Frequency,
                             char SeqBitStart, int Start0, int Start0Len, int Start1, int Start1Len,
                             int Dbit0_A, int Dbit0_A_Len, int Dbit0_B, int Dbit0_B_Len,
                             int Dbit1_A, int Dbit1_A_Len, int Dbit1_B, int Dbit1_B_Len,
                             char SeqBitStop, int Stop0, int Stop0Len, int Stop1, int Stop1Len,
                             char MonitorEnabled)
/*
    Questa routine è il client per l'invio di dati in modalità raw. 
    
    La routine accetta diversi parametri: vediamoli uno alla volta.
    
    Value:    è il valore che dovrà essere inviato dal sensore IR. 
    NegLogic: se posto a 0 non ha effetto; se posto a 1, il sensore IR userà 
              una logica negativa, ed il bit 0 verrà trasmesso con un segnale
              di livello alto.
              
    BitNumber: è il numero di bit che compongono il valore da inviare
    
    MoreSignif: avverte il sistema che devono essere inviati prima i bit più
                significativi
                
    Frequency: è la frequenza del segnale IR, espressa in Khz
    
    SeqBitStart, Start0, Start0Len, Start1, Start1Len:
              Questa routine prevede che sia possibile, settando a 1 il
              parametro SeqBitStart, settare due bit di start che precederanno
              nel flusso rawIR infrarosso la codifica del Value.
              Start0 e Start1 contengono il valore dei due bit (si tenga conto
              della logica utilizzata), mentre Start0Len e Start1Len esprimono
              la durata dei bit (33 rappresenta 1/1200 sec.)
    
    Dbit0_A, Dbit0_A_Len, Dbit0_B, Dbit0_B_Len:
    Dbit1_B, Dbit1_B_Len, Dbit1_B, Dbit1_B_Len:           
    
    La codifica di ogni bit di Value avviene nel seguente modo: per ogni bit del
    campo value, viene in realtà inviato un dibit, composto da due segnali 
    successivi, denotati con le lettere A e B. 
    
    Settando opportunamente i valori è possibile stabilire stato e lunghezza di
    ciascuno dei quattro segnali. Si ricordi che anche questi bit risentono
    dell'eventuale parametro NegLogic impostato a 1.
    
    SeqBitStop, Stop0, Stop0Len, Stop1, Stop1Len:
              Analogo ai bit di start, solo che qui viene consentito di settare
              due bit di stop dopo il flusso rawIR infrarosso successivo alla
              codifica del segnale  
              
    Il parametro MonitorEnabled viene posto normalmente a 0. 
    
    Se MonitorEnabled viene posto a 1, il sistema invierà il segnale infrarosso
    ed inoltre mostrerà un monitor con i segnali inviati.
    
    Se MonitorEnabled viene posto a 2, il sistema utilizzerà semplicemente il
    monitor di sistema senza inviare i dati via infrarosso. 
              
    La funzione riporta 0 se non vi sono errori in fase di invio. 
    Sono previsti i seguenti codici di errore:
         
    ERR_RAWIR_NOT_AVAILABLE  La funzione RawIR non è disponibile 
    ERR_TIMEOUT              Timeout del server
*/

{
    RAW_IR_DATA IRClientData;
    
    static u32 RawBuffer [128]; 
    // Buffer raw per RawBufferPointer segnale
    
    u32 Mask;
    
    int RawBufferPointer = 0;
    int Counter;
    
    char ErrMonitorRep;
    
    char _UseMonitor;
    char _Trasmission;
    
    if ( (!ND_HWMODE_DisableRawIRService) && (!RawIRFailure) )
    {          
          _UseMonitor=0;
          _Trasmission=1;
          
          if (MonitorEnabled==1)
          {
             _UseMonitor=1;
             _Trasmission=1;
          }
          
          if (MonitorEnabled==2)
          {
             _UseMonitor=1;
             _Trasmission=0;
          }   
          
          if (_UseMonitor)
          {
                    ErrMonitorRep = ndHAL_IR_EnableIRRawMonitor ();
                    
                    if (!ErrMonitorRep)
                    {
                                ndWS_PrintLn (RawIRMonitorHandle, COLOR_WHITE, RENDER, "RawIR Signal Send");
                                
                                ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "Value HEX: %X  ", Value);
                                
                                if (SeqBitStart)
                                {
                                       if (Start0)              // Questo bit è a 1
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "1");      
                                       else                     // Questo bit è a 0
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "0");    
                                       
                                       if (Start1)              // Questo bit è a 1
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "1");      
                                       else                     // Questo bit è a 0
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "0");    
                                }
                                else  // Nessuna sequenza start prevista
                                {
                                      ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "_");
                                      ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "_");
                                }
                                
                                ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, " ");
                                
                                for (Counter=0; Counter<BitNumber; Counter++)
                                {
                                        Mask = 1;
                                        
                                        if (!MoreSignif)
                                           Mask = (Mask << Counter);
                                        else
                                           Mask = (Mask << (BitNumber - 1 - Counter));
                                           
                                        if (Value & Mask)        // Questo bit è a 1
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "1");      
                                        else                     // Questo bit è a 0
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "0");               
                                }
                                
                                ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, " ");
                                
                                if (SeqBitStop)
                                {
                                       if (Stop0)               // Questo bit è a 1
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "1");      
                                       else                     // Questo bit è a 0
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "0");    
                                       
                                       if (Stop1)               // Questo bit è a 1
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "1");      
                                       else                     // Questo bit è a 0
                                            ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "0");    
                                }
                                else  // Nessuna sequenza stop prevista
                                {
                                      ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "_");
                                      ndWS_Print (RawIRMonitorHandle, COLOR_WHITE, RENDER, "_");
                                }
                                
                                ndWS_CarriageReturn (RawIRMonitorHandle);   
                                
                                ndWS_PrintLn (RawIRMonitorHandle, COLOR_WHITE, RENDER, "Frequency %d Khz NegativeLogic %d", Frequency, NegLogic);
                                
                                ndWS_CarriageReturn (RawIRMonitorHandle);
                                // Uno spazio tra un elemento e l'altro 
                                
                    }    
                    
                    // Se c'erano problemi a visualizzare il monitor, si limita a saltare l'intera
                    // procedura di monitoraggio             
          }
                        
          if (_Trasmission)
          {     
                                     
                        if (SeqBitStart)
                        {
                           RawBuffer [RawBufferPointer++] = INTERNAL_IR_SIGNAL (Start0, NegLogic, Start0Len); // start bit
                           RawBuffer [RawBufferPointer++] = INTERNAL_IR_SIGNAL (Start1, NegLogic, Start1Len);
                        }
                        
                        for (Counter=0; Counter<BitNumber; Counter++)
                        {
                            Mask = 1;
                            
                            if (!MoreSignif)
                               Mask = (Mask << Counter);
                            else
                               Mask = (Mask << (BitNumber - 1 - Counter));
                               
                            if (Value & Mask)        // Questo bit è a 1
                            {
                              RawBuffer[RawBufferPointer++] = INTERNAL_IR_SIGNAL (Dbit1_A, NegLogic, Dbit1_A_Len);      // RawBufferPointer segnale va alto per 4/1200 sec.
                              RawBuffer[RawBufferPointer++] = INTERNAL_IR_SIGNAL (Dbit1_B, NegLogic, Dbit1_B_Len);      // RawBufferPointer segnale va basso per 1/1200 sec.     
                            }
                            else               // Questo bit è a 0
                            {
                              RawBuffer[RawBufferPointer++] = INTERNAL_IR_SIGNAL (Dbit0_A, NegLogic, Dbit0_A_Len);      // RawBufferPointer segnale va alto per 1/1200 sec.
                              RawBuffer[RawBufferPointer++] = INTERNAL_IR_SIGNAL (Dbit0_B, NegLogic, Dbit0_B_Len);      // RawBufferPointer segnale va basso per 1/1200 sec.     
                            }
                        }
                        
                        if (SeqBitStop)
                        {
                           RawBuffer [RawBufferPointer++] = INTERNAL_IR_SIGNAL (Stop0, NegLogic, Stop0Len); // start bit
                           RawBuffer [RawBufferPointer++] = INTERNAL_IR_SIGNAL (Stop1, NegLogic, Stop1Len);
                        }
                        
                        IRClientData.timing_val = Frequency/2; // per Robosapiens 2 40kHz (actually 39.2kHz)
                        IRClientData.raw_buffer = RawBuffer;
                        IRClientData.raw_count  = RawBufferPointer;
                        
                        // Arrivati a questo punto, i dati sono nel buffer interno IRClientData e devono
                        // essere mandati al componente server per la gestione del raw-ir. 
                        
                        // Ricordiamoci che tale componente server fa parte del loop dei servizi del
                        // main server. 
                        
                        for (Counter=0; Counter<RAWIR_TIMEOUT; Counter++)
                        {
                             // Questa sezione della routine si pone in attesa che non vi siano dati
                             // in attesa di trasmissione da parte dell'IR Server e che l'IRServer
                             // non stia elaborando in quel momento. 
                             
                             // Solo quando non vi sono più sequenze in attesa e l'IRServer è 
                             // libero verrà inviata la sequenza elaborata da questa routine.
                             
                             // Si noti che se questa condizione non si verificasse mai il thread
                             // si bloccherebbe. Per evitare questo, esiste un numero massimo di
                             // tentativi che il client esegue sul server, determinato da
                             // RAWIR_TIMEOUT. Oltrepassato questo limite, viene restituito un
                             // errore di timeout
                              
                             sceDisplayWaitVblankStart (); // Sembra essere necessario per evitare il blocco dello scheduler...
                                 
                             if ((!IRServerData_Waiting) && (!IRServer_Busy))
                             {                    
                                IRServerData_Pointer=&IRClientData;    // Inserisci i dati che saranno ricevuti dal componente server
                                IRServerData_NrSequences=1;            // Una sola sequenza di bit da inviare
                             
                                IRServerData_Waiting=1;
                                // Viene posto a 1 IRServerData_Waiting. Questo indurrà il server a 
                                // processare la nuova sequenza. 
                                
                                ndRawIRThreadRq = sceKernelGetThreadId ();
                                // Ricava l'ID del thread che sta eseguendo la richiesta di emissione di
                                // un segnale Raw-IR. 
                                
                                ndHAL_KSU_DeInhibitKSULoop ();
                                // Riattiva il thread KSU in modo che sia possibile processare la richiesta
                                // di emissione del segnale Raw-IR
                                
                                sceKernelSuspendThread (ndRawIRThreadRq);
                                // Sospende il thread corrente in modo che il sistema non sprechi cicli
                                // macchina mentre attende che la richiesta di emissione del segnale
                                // Raw-IR sia processata. Sarà compito del server Raw-IR riattivare
                                // il presente thread una volta che il segnale Raw-IR è stato emesso
                                
                                
                                /*
                                   ---------------------------
                                   L'esecuzione è ricominciata: se la modalità monitor è attivata, 
                                   devi solo eseguire un ciclo di ritardo di 2 secondi per dare 
                                   modo all'utente di leggere
                                */
                                
                                
                                if  ((_UseMonitor) && (!ErrMonitorRep))
                                {
                                    ndWS_PrintLn (RawIRMonitorHandle, COLOR_WHITE, RENDER, "SUCCESS");
                                
                                    ndWS_CarriageReturn (RawIRMonitorHandle);
                                    // Uno spazio tra un elemento e l'altro 
                                    
                                    ndHAL_HardDelay (2);
                                } 
                                
                                return 0;
                             }
                        }
                        
                        // Se siamo arrivati in questo punto, vuol dire che c'è stato un timeout 
                        
                        if ((_UseMonitor) && (!ErrMonitorRep))
                        {
                           ndWS_PrintLn (RawIRMonitorHandle, COLOR_WHITE, RENDER, "TIMEOUT");
                                
                           ndWS_CarriageReturn (RawIRMonitorHandle);
                           // Uno spazio tra un elemento e l'altro 
                           
                           ndHAL_HardDelay (2);
                        }     
                        
                        return ERR_TIMEOUT;           // Timeout del server
                        
          } 
          
          return 0;
          // Il programma può arrivare a questo punto se, ad esempio, il software
          // è in modalità monitor ma non è abilitata la trasmissione  
              
    }
    else return ERR_RAWIR_NOT_AVAILABLE;

}














