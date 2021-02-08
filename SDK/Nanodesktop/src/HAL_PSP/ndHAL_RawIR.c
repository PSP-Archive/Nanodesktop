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
    FUNZIONI PER GENERAZIONE SEGNALI RAWIR - MELORA MODULE (PSP)
    ---------------------------------------------------------------------------
    Queste funzionalità sono state create per supportare il servizio RawIR.
    
    Si tenga ben presente che questa versione della libreria Nanodesktop per PSP
    ha disattivato la modalità KSU. In questo caso, non è possibile fornire i servizi
    Raw-IR, quindi l'API si limita ad eseguire l'emulazione del
    comportamento del sistema Melora, ma senza inviare segnali raw. 
    
    Per inviare veri segnali RawIR, è necessario usare la versione KSU_PSP di
    Nanodesktop.
    ---------------------------------------------------------------------------
*/

#include <HAL_PSP\$_HAL_NDforND.h>

typedef enum { false, true } bool;





/*
    ----------------------------------------------------------------------------
    Simboli per gestione RAWIR (PSP User mode)
    ----------------------------------------------------------------------------
*/


typedef struct _RAW_IR_DATA
{
    int timing_val;
        // used for carrier frequency (freq = timing_val * 2 kHz)
        //  and for PWM sample ticks (1 tick = timing_val * 1.25 us)
    const u32* raw_buffer;
    int raw_count;
} RAW_IR_DATA;





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
                          funzioni client per l'invio dei dati                                                  
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/

/*
    Si noti che, a differenza di quanto avviene nella versione PSP, non
    c'è nessun server per l'invio
*/

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
    
    Sotto PSP, sarebbero previsti i seguenti codici di errore:
         
    ERR_YOU_MUST_USE_KSU     Non è stata abilitata la funzione KernelServicesToUser
    ERR_RAWIR_NOT_AVAILABLE  La funzione RawIR non è disponibile 
    ERR_TIMEOUT              Timeout del server
    
    ma di fatto la routine non riporta errori in emulazione
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
                    
                    // In questo punto, sotto PSPE o sotto PSP con modalità KSU disattivata, 
                    // inseriamo una semplice routine di emulazione che simula il successo.
                    
                    if  ((_UseMonitor) && (!ErrMonitorRep))
                    {
                          ndWS_PrintLn (RawIRMonitorHandle, COLOR_WHITE, RENDER, "SUCCESS");
                            
                          ndWS_CarriageReturn (RawIRMonitorHandle);
                          // Uno spazio tra un elemento e l'altro 
                                
                          ndHAL_HardDelay (2);
                    } 
                            
                    return 0;
                                 
      } 
      
      return 0;
      // Il programma può arrivare a questo punto se, ad esempio, il software
      // è in modalità monitor ma non è abilitata la trasmissione  
               
}



























