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
    API PER LA GESTIONE DEL SISTEMA INFRAROSSO (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

void ndHAL_InitIrSystem()
{
         ndIrChannelsArray [0].IsEnabled = 0;
}


int ndHAL_IR_AssignChannel (ndint64 Code)
/*
    Le routines Nanodesktop richiedono che un canale IR, prima di potere essere 
    utilizzato, venga prenotato dall'utente. Questa routine restituisce in uscita
    un handle (>=0) che da quel momento individua il dispositivo IR in tutte le
    chiamate successive. Questa tecnica fa sì che sia possibile astrarre il
    numero di dispositivi IR effettivamente presenti nella piattaforma. 
    
    Il parametro Code è un parametro a 64 bit che ha significato diverso a 
    seconda della piattaforma: sotto PSP viene ignorato. 
    
    Se la routine restituisce un valore negativo in uscita, vuol dire che si
    è verificato un errore. Questi sono i codici di errore previsti:
         ERR_NO_CHANNEL_AVAILABLE  Non ci sono più dispositivi disponibili          
*/
{
    int Counter;
    
    for (Counter=0; Counter<__IRDACHANNELSSUPPORTED; Counter++)
    {
        if (!ndIrChannelsArray [Counter].IsEnabled)
        {
             ndIrChannelsArray [Counter].IsEnabled = 1;
             ndIrChannelsArray [Counter].Mode      = ND_IR_NULL_MODE; 
             ndIrChannelsArray [Counter].DevHandle = 0;
             
             return Counter;                // Ritorna il valore dell'handle
        }
    }
    
    // Se siamo arrivati a questo punto, vuol dire che non sono stati trovati
    // handles/canali disponibili
    
    return ERR_NO_CHANNEL_AVAILABLE;        
}


char ndHAL_IR_ConfigureChannel (int IrHandleChannel, char Mode, int AdrStruct)
/*
   Questa routine viene usata per configurare un certo canale IR che viene
   individuato dal parametro IrHandleChannel.
   
   Il parametro Mode può avere due valori: ND_IR_IRDA_MODE, ND_IR_RAW_MODE. 
   
   Nel primo caso si abilita la modalità IRDA per il canale, mentre nel
   secondo caso si abilita la modalità IRRAW.

   Il terzo parametro è un indirizzo a 32 bit. Per configurare il canale
   IRDA, il programma deve allocare una struttura di tipo ndIRDAConfigBlock_Type
   e deve passarne l'indirizzo al programma chiamante. 
   
   Per configurare il canale in modalità RAWIR, il programma deve allocare 
   una struttura di tipo ndRawIRConfigBlock_Type, porre al suo interno i dati
   di configurazione, e successivamente passare l'indirizzo alla routine
   ConfigureChannel. Questa provvederà a copiare le informazioni fornite nella
   struttura in un record riservato del sistema. 
   
   Per quanto riguarda il significato dei parametri di configurazione, la
   situazione è la seguente. 
   
   IRDA:
                    L'unico parametro che attualmente viene utilizzato per IRDA
                    è MonitorEnabled
                    
                    Il parametro MonitorEnabled viene posto normalmente a 0. 
                    
                    Se MonitorEnabled viene posto a 1, il sistema invierà il segnale infrarosso
                    ed inoltre mostrerà un monitor con i segnali inviati.
                    
                    Se MonitorEnabled viene posto a 2, il sistema utilizzerà semplicemente il
                    monitor di sistema senza inviare i dati via infrarosso. 
                     
                                
   RAWIR:
         
                    Questo è il significato dei campi della struttura di tipo
                    ndRawIRConfigBlock_Type. I valori di configurazione devono
                    essere immessi prima di richiamare la routine:
                          
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
                                      
   Sono previsti i seguenti codici di errore:
        ERR_CHANNEL_NOT_ACTIVE     Il canale non è attivo
        ERR_VALUE_MISMATCH         Il valore Mode non è corretto
        ERR_RAWIR_NOT_AVAILABLE    Raw-IR non abilitato
        ERR_ERROR_IN_OPEN_DEVICE   Errore nell'apertura del dispositivo (IRDA)
*/
{
     char HwHandle;   
        
     if (ndIrChannelsArray [IrHandleChannel].IsEnabled)
     {
          switch (Mode)
          {
                 case ND_IR_IRDA_MODE:
                 {
                      HwHandle = sceIoOpen("irda0:", PSP_O_RDWR, 0); 
                      
                      if (HwHandle>=0)      // Non ci sono stati errori nell'apertura del device
                      {
                            ndIrChannelsArray [IrHandleChannel].DevHandle = HwHandle;
                            ndIrChannelsArray [IrHandleChannel].Mode      = ND_IR_IRDA_MODE;
                            ndIrChannelsArray [IrHandleChannel].IrError   = 0;
                                         
                            memcpy ( &(ndIrChannelsArray [IrHandleChannel].IRDAcfg), AdrStruct, sizeof (struct ndIRDAConfigBlock_Type));
                            // Copia dentro RawIRcfg i dati di configurazione contenuti nella struttura
                            // allocata dentro il programma chiamante
                       
                            return 0;
                      }
                      else  // Problema hardware nell'apertura del device
                      {
                            ndIrChannelsArray [IrHandleChannel].DevHandle = 0;
                            ndIrChannelsArray [IrHandleChannel].Mode      = ND_IR_NULL_MODE;
                            ndIrChannelsArray [IrHandleChannel].IrError   = 0;
                            
                            // Il canale non sarà utilizzabile per lettura/scrittura, perchè
                            // è fallita la sua reinizializzazione
                            
                            return ERR_ERROR_IN_OPEN_DEVICE;
                      }
                      
                 }
                 
                 case ND_IR_RAW_MODE:
                 {
                      ndIrChannelsArray [IrHandleChannel].DevHandle=0;
                      ndIrChannelsArray [IrHandleChannel].Mode = ND_IR_RAW_MODE;
                      ndIrChannelsArray [IrHandleChannel].IrError   = 0;
                      
                      memcpy ( &(ndIrChannelsArray [IrHandleChannel].RawIRcfg), AdrStruct, sizeof (struct ndRawIRConfigBlock_Type));
                      // Copia dentro RawIRcfg i dati di configurazione contenuti nella struttura
                      // allocata dentro il programma chiamante
                       
                      return 0; 
                 }
                 
                 default:   // Il codice immesso non è corretto
                 {
                      return ERR_VALUE_MISMATCH;
                 }
          }
     }
     else return ERR_CHANNEL_NOT_ACTIVE;
}

int ndHAL_IR_SendData (int IrHandleChannel, int AdrData, int Number)
/*
    Questa funzione provvede ad inviare dei dati attraverso il canale IR
    identificato da IrHandleChannel. 
    
    Il modo in cui i dati saranno inviati dipende dalla modalità
    che è stata precedentemente programmata attraverso 
    ndHAL_IR_ConfigureChannel.
    
    Vediamo le due modalità.
    
    In modalità IRDA, verra inviato un numero di byte pari a Number, memorizzati
    a partire dall'indirizzo AdrData. La routine invia dati fin quando non si
    verifica un errore: in uscita viene restituito il numero di byte IRDA che
    sono stati effettivamente inviati. Se la routine trova qualche errore 
    nell'invio, memorizza la condizione di errore riscontrata nel campo
    IrError della struct ndIrChannelsArray relativa al canale in cui stiamo 
    lavorando. 
    
    In modalità RAWIR, la routine assume che all'indirizzo AdrData ci sia un
    numero di campi a 32 bit pari a Number. Dopodichè, analizza ciascuno di
    questi campi a 32 bit, uno alla volta, e li invia usando il raw-ir client. 
    I parametri di funzionamento saranno quelli che sono stati stabiliti 
    durante l'ultimo processo di configurazione del canale. Alla fine la 
    routine ritorna il numero di valori a 32 bit che sono stati effettivamente
    scritti. 
    
    Attenzione che dovranno essere usati campi a 32 bit anche se il numero di
    bit del Value inviato con il segnale raw-ir è inferiore. 
    
    Per gli errori che sono critici, in uscita possono essere restituiti i
    seguenti valori:
             
    ERR_CHANNEL_NOT_CONFIGURED       Il canale non è stato ancora configurato
    ERR_CHANNEL_NOT_ACTIVE           Il canale non è attivo
*/
{
    int Counter;
    int *PointerToWord;
    int ElementsSent;
    int IOResult;
    int Value;
    
    if (ndIrChannelsArray [IrHandleChannel].IsEnabled)
    {
        switch (ndIrChannelsArray [IrHandleChannel].Mode)
        {
                 case ND_IR_IRDA_MODE:
                 {
                      // A differenza di quello che accade nel caso di RawIR, per IRDA l'elaborazione 
                      // e la gestione degli errori avviene tutta nel client
                      
                      IOResult = INTERNAL_ndHAL_SendIRDA_CLIENT (IrHandleChannel, AdrData, Number, 
                                       ndIrChannelsArray [IrHandleChannel].IRDAcfg.MonitorEnabled);
                      
                      return IOResult;
                 }
                 
                 case ND_IR_RAW_MODE:
                 {
                     ElementsSent=0; 
                     
                     ndIrChannelsArray [IrHandleChannel].IrError=0;
                      
                     for (Counter=0; Counter<Number; Counter++)
                     {
                         PointerToWord=AdrData + Counter;
                         // Si noti che è come se Counter venisse moltiplicato per 4 perchè il
                         // puntatore è di tipo int
                         
                         Value = *PointerToWord;
                         // Preleva il valore che serve
                         
                         IOResult = INTERNAL_ndHAL_SendRawIR_CLIENT (Value,
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.NegLogic, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.BitNumber, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.MoreSignif, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Frequency,
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.SeqBitStart, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Start0, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Start0Len, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Start1, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Start1Len,
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Dbit0_A, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Dbit0_A_Len, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Dbit0_B, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Dbit0_B_Len,
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Dbit1_A, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Dbit1_A_Len, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Dbit1_B, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Dbit1_B_Len,
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.SeqBitStop, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Stop0, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Stop0Len, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Stop1, 
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.Stop1Len,
                                                             ndIrChannelsArray [IrHandleChannel].RawIRcfg.MonitorEnabled);
                                                             
                         if (IOResult==0)     // Non ci sono stati errori nell'invio
                         {
                             ElementsSent++;
                         }
                         else                // Logga l'errore
                         {
                             ndIrChannelsArray [IrHandleChannel].IrError=IOResult;
                             break;          // Termina il ciclo for di invio                
                         }
                     } 
                     
                     return (ElementsSent);     // Ritorna il numero di elementi inviati
                 }
                 
                 default:
                 {
                         return ERR_CHANNEL_NOT_CONFIGURED;
                         // Se si è arrivati a questo punto, il canale non è stato ancora configurato
                 }
        }
          
    }
    else 
      return ERR_CHANNEL_NOT_ACTIVE;  
}

int ndHAL_IR_ReceiveData (int IrHandleChannel, int AdrData, int Number)
/*
    Questa funzione provvede a ricevere dei dati attraverso il canale IR
    identificato da IrHandleChannel. 
    
    Il modo in cui i dati saranno ricevuti dipende dalla modalità
    che è stata precedentemente programmata attraverso 
    ndHAL_IR_ConfigureChannel.
    
    Vediamo le due modalità.
    
    In modalità IRDA, la routine cercherà di ricevere un numero di byte pari a 
    Number, che saranno memorizzati a partire dall'indirizzo AdrData. 
    
    La routine attende dati fin quando non si verifica un errore critico, o
    fin quando il sistema operativo non restituisce il controllo alla fine
    del processo di lettura IRDA: in uscita viene restituito il numero di byte IRDA che
    sono stati effettivamente ricevuti. 
    
    Se la routine trova qualche errore critico nella ricezione, memorizza la 
    condizione di errore riscontrata nel campo IrError della struct ndIrChannelsArray 
    relativa al canale in cui stiamo  lavorando. 
    
    Se per questo canale era stata impostata la modalità RAW-IR, si riceverà un
    errore, perchè non è supportata la ricezione RAW-IR, ma solo l'invio.
    
    Per gli errori che sono critici, in uscita possono essere restituiti i
    seguenti valori:
             
    ERR_CHANNEL_NOT_CONFIGURED       Il canale non è stato ancora configurato
    ERR_CHANNEL_NOT_ACTIVE           Il canale non è attivo
    ERR_FUNCTION_NOT_SUPPORTED       La funzione non è supportata

*/
{
    int IOResult;
    
    if (ndIrChannelsArray [IrHandleChannel].IsEnabled)
    {
        switch (ndIrChannelsArray [IrHandleChannel].Mode)
        {
                 case ND_IR_IRDA_MODE:
                 {
                      // A differenza di quello che accade nel caso di RawIR, per IRDA l'elaborazione 
                      // e la gestione degli errori avviene tutta nel client
                      
                      IOResult = INTERNAL_ndHAL_ReceiveIRDA_CLIENT (IrHandleChannel, AdrData, Number, 
                                       ndIrChannelsArray [IrHandleChannel].IRDAcfg.MonitorEnabled);
                      
                      return IOResult;
                 }
                 
                 case ND_IR_RAW_MODE:
                 {
                      // La ricezione dei dati in modalità Raw-IR non è supportata                 
    
                      return ERR_FUNCTION_NOT_SUPPORTED;
                 }
                 
                 default:
                 {
                         return ERR_CHANNEL_NOT_CONFIGURED;
                         // Se si è arrivati a questo punto, il canale non è stato ancora configurato
                 }
        }
          
    }
    else 
      return ERR_CHANNEL_NOT_ACTIVE;  
}



char ndHAL_IR_ReleaseChannel (int IrHandleChannel)
/*
     Questa funzione provvede a rilasciare un canale IRDA.
     
     Sono previsti i seguenti codici di errore:
          ERR_CHANNEL_NOT_ACTIVE     Canale non attivo
*/
{
    if (ndIrChannelsArray [IrHandleChannel].IsEnabled)
    {
        ndIrChannelsArray [IrHandleChannel].IsEnabled=0;
        return 0;
    }
    else
       return  ERR_CHANNEL_NOT_ACTIVE;
}
