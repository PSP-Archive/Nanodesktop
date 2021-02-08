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
    HAL: FUNZIONI DECODIFICA FORMATI AUDIO (PSP)
    ---------------------------------------------------------------------------
    Rispetto alla versione per PSP KSU, non è consentita la decodifica MP3.
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

static int MessageToThread;


/*
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                                   DECODIFICATORE WAV
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/




// --------------------------------------------------------------------------
// WAV:SIMBOLI 
// --------------------------------------------------------------------------


int WAV_DecoderThreadsActive = 0;


// --------------------------------------------------------------------------
// WAV:CODICE DI DECODIFICA A BASSO LIVELLO 
// --------------------------------------------------------------------------

static inline void INTERNAL_WAV_ChangeDecoderState (struct ndMediaObj_Type *MediaObj, int NewState)
{
    int IsNecessaryWakeUp = 0;
    
    if (MediaObj->DecoderState==WAV_STATE_IDLE) IsNecessaryWakeUp=1;
    // Preparati a risvegliare il thread che era stato posto in sleep
    
    MediaObj->old_DecoderState =  MediaObj->DecoderState;
    MediaObj->DecoderState     =  NewState;  
    
    if (IsNecessaryWakeUp) sceKernelWakeupThread (MediaObj->DecThreadID);
    // Il thread viene risvegliato dopo il cambiamento di stato
}


int INTERNAL_WAV_DeterminateWAVFeatures (struct ndMediaObj_Type *ndMediaObj)
/*
     Questa routine è in grado di determinare alcune caratteristiche 
     dell'WAV. Se il processo va a buon fine, la routine restituisce
     il valore 0, altrimenti restituisce un codice di errore

     Il bitrate viene memorizzato in ndMediaObj->SampleRate
     
     Dentro il campo ndMediaObj->WAV_HeaderError viene memorizzato un codice
     speciale che indica il tipo di errore che è stato trovato nell'header
*/
{
   unsigned char buf [256];
   int BytesRead;
   int FileLength;
   
   // Determina le dimensioni del file
   FileLength=lseek (ndMediaObj->MediaHandle, 0, SEEK_END);
   
   // Riporta all'inizio il puntatore del file
   lseek (ndMediaObj->MediaHandle, 0, SEEK_SET);
   
   BytesRead = read (ndMediaObj->MediaHandle, &buf, 0x30); 
   if (BytesRead==0x30)  
   {
       if (memcmp (buf,"RIFF",4)==0) 
       {                           
               if (memcmp(buf+8,"WAVEfmt \x10\x00\x00\x00\x01\x00",14)==0) 
               {  
                       ndMediaObj->WAV_Channels=*(short *)(buf+0x16); 
                       ndMediaObj->WAV_SampleRate=*(long *)(buf+0x18); 
                       ndMediaObj->WAV_BlockSize=*(short *)(buf+0x20); 
                       ndMediaObj->WAV_BitsForSample=*(short *)(buf+0x22); 
                        
                       if (memcmp(buf+0x24,"data",4)==0) 
                       { 
                                   ndMediaObj->WAV_DataLength=*(unsigned long *)(buf+0x28);
                                   
                                   if (ndMediaObj->WAV_DataLength+0x2c<=FileLength)
                                   {       
                                           switch (ndMediaObj->WAV_Channels)
                                           {
                                                  case 1:      // mono
                                                  {
                                                      ndMediaObj->WAV_SamplesInTotal=ndMediaObj->WAV_DataLength/2; 
                                                      
                                                      ndMediaObj->WAV_HwSampleRate = ndMediaObj->WAV_SampleRate;
                                                      ndMediaObj->WAV_HwOutputMode = ND_AUDIO_MONO;
                                                      ndMediaObj->WAV_HwSampleSize = 2;    // Partendo dal presupposto che siano campioni a 16 bit
                                                                                           // nel passaggio successivo si dividerà event. per 2
                                                      break; 
                                                  }
                                                  
                                                  case 2:     // stereo
                                                  {
                                                      ndMediaObj->WAV_SamplesInTotal=ndMediaObj->WAV_DataLength/4; 
                                                      
                                                      ndMediaObj->WAV_HwSampleRate = ndMediaObj->WAV_SampleRate;
                                                      ndMediaObj->WAV_HwOutputMode = ND_AUDIO_STEREO;
                                                      ndMediaObj->WAV_HwSampleSize = 4;    // Partendo dal presupposto che siano campioni a 16 bit           
                                                                                           // nel passaggio successivo si dividerà event. per 2
                                                      break; 
                                                  }
                                           }
                                           
                                           switch (ndMediaObj->WAV_BitsForSample)
                                           {
                                                  case 8:
                                                  {
                                                      ndMediaObj->WAV_HwOutputBitRate = ND_AUDIO_8BIT;
                                                      ndMediaObj->WAV_HwSampleSize = ndMediaObj->WAV_HwSampleSize / 2;
                                                      
                                                      break; 
                                                  }
                                                  
                                                  case 16:
                                                  {
                                                      ndMediaObj->WAV_HwOutputBitRate = ND_AUDIO_16BIT;
                                                      break;  
                                                  }
                                           }
                                           
                                           ndMediaObj->WAV_PosStartData=0x2C; 
                                           
                                           return 0;
                                   }
                                   else  // Il file ha dimensioni più piccole di quanto indicato
                                   {
                                       ndMediaObj->WAV_HeaderError = ERR_WAV_FILE_TOO_SHORT;
                                       return -1; 
                                   }
                                    
                       }
                       else   // Problema con la sequenza data
                       {
                           ndMediaObj->WAV_HeaderError = ERR_WAV_TROUBLE_WITH_DATA_CODE_SEQUENCE;
                           return -1; 
                       }  
                                    
                }             
                else   // Ci sono problemi con la sequenza codici
                {
                      ndMediaObj->WAV_HeaderError = ERR_WAV_TROUBLE_WITH_CODE_SEQUENCE;
                      return -1;  
                }                
       }
       else   // Ci sono problemi: l'header non inizia per RIFF
       {
          ndMediaObj->WAV_HeaderError = ERR_WAV_IT_DOES_NOT_BEGIN_WITH_RIFF;
          return -1;    
       }
   }     
   else     // L'header che è stato letto è troppo corto. E' di sicuro danneggiato.
   {
       ndMediaObj->WAV_HeaderError = ERR_WAV_WRONG_HEADER;
       return -1;  
   }    
   
}


char INTERNAL_WAV_FeaturesAreAllowed (struct ndMediaObj_Type *ndMediaObj)
/*
    Verifica che le caratteristiche del file .WAV siano accettabili. Se
    è così riporta in uscita 0, altrimenti ritorna -1. 
    
    Nel campo ndMediaObj->WAV_HeaderError viene riportato un codice
    di errore che indica il tipo di problema
*/
{
    if (ndMediaObj->WAV_Channels!=2 && ndMediaObj->WAV_Channels!=1) 
    { 
        ndMediaObj->WAV_HeaderError = ERR_WAV_ONLY_MONO_OR_STEREO_SUPPORTED; 
        return 0; 
    } 
    
    switch (ndMediaObj->WAV_SampleRate)
    {
            case 8000:
            case 11025:
            case 12000:
            case 16000:
            case 22050:
            case 24000:
            case 32000:
            case 44100:
            case 48000:
                 break;
            default:
                 ndMediaObj->WAV_HeaderError = ERR_WAV_WRONG_SAMPLE_RATE;     
                 return 0;
    }
    
    if ( (ndMediaObj->WAV_BitsForSample!=8) && (ndMediaObj->WAV_BitsForSample!=16) ) 
    { 
       ndMediaObj->WAV_HeaderError = ERR_WAV_ONLY_16BIT_OR_8BIT_FOR_SAMPLE_ALLOWED;
       return 0; 
    } 
    
    if (ndMediaObj->WAV_SamplesInTotal<=0) 
    { 
       ndMediaObj->WAV_HeaderError = ERR_WAV_INVALID_SAMPLE_COUNT;
       return 0; 
    }
    
    ndMediaObj->WAV_HeaderError = 0;
    return 1;
}

int INTERNAL_WAV_RetrieveNextFrame (struct ndMediaObj_Type *ndMediaObj)
/*
    Provvede a caricare il prossimo frame dal file WAV. 
    
    Restituisce 0 in caso di assenza di errori e -1 in caso di errore
*/
{
    int BytesRead;
    
    BytesRead = read (ndMediaObj->MediaHandle, ndMediaObj->WAV_L1Buffer, WAV_L1_BUFFER_SIZE); 
    
    if (BytesRead>0)
    {
        if (BytesRead!=WAV_L1_BUFFER_SIZE)
        {
            memset ( ndMediaObj->WAV_L1Buffer + BytesRead, 0, WAV_L1_BUFFER_SIZE-BytesRead );
            // Riempie con degli zero l'area dati che non è stata sovrascritta già nella
            // fase di lettura dal disco                                
        }
        
        ndMediaObj->WAV_SamplesRead = BytesRead / (ndMediaObj->WAV_HwSampleSize);
        return 0;            
    }
    else    // Errore disco o file terminato
    {
        ndMediaObj->WAV_SamplesRead = 0;
        return -1;    
    }
}


void INTERNAL_WAV_DecoderThread (SceSize args, void *argp)
{
   void (*PntToMediaCallback)(ndint64 InfoField, struct ndMediaObj_Type *ndMediaObj, 
                             int NrSample, int HH, int MM, int SS);
   
   struct ndMediaObj_Type *ndMediaObj;
   int    HH, MM, SS, SecPassed;
   float  InvSamplesForSec;
   int    ErrRep, Counter;
   int    SndChannel;
   
   int    A;
   
   // E' necessario innanzitutto riprendere l'indirizzo della struttura 
   // ndMediaObj che contengono le informazioni che ci servono
   
   int PntToAddress = *(int *)(argp);
   ndMediaObj = PntToAddress;
   
   // Indirizzo recuperato: inizia il ciclo di decodifica
                                                        
   ndMediaObj->DecoderState  = -1;
   // Viene posto a -1, in modo che al successivo cambio di stato, il vecchio ed
   // il nuovo valore siano diversi
   
   INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_IDLE);
   ndMediaObj->DecoderError  = 0;
   ndMediaObj->DecoderExitRq = 0;
   
   ndMediaObj->NrTimesPlay   = 0;
   ndMediaObj->WAV_SamplesDecoded = 0;
   
   // Pone a 0 i flags evento
   sceKernelSetEventFlag (ndMediaObj->DecEventID, 0);
   
   // Setup della callback
   PntToMediaCallback = ndMediaObj->CallBack;

   // Calcola il reciproco del SampleRate
   if (ndMediaObj->WAV_SampleRate!=0)
   {
      InvSamplesForSec = 1.0/(float)(ndMediaObj->WAV_SampleRate);
   }
   
   // Porta il puntatore del file nella posizione corretta per iniziare a prendere i dati
   lseek (ndMediaObj->MediaHandle, ndMediaObj->WAV_PosStartData, SEEK_SET); 
   
   //Decoding loop:
   while ( !ndMediaObj->DecoderExitRq )
   {
       sceDisplayWaitVblankStart ();
       // Necessario per impedire il blocco del sistema
       
       switch (ndMediaObj->DecoderState)
       {
              case WAV_STATE_IDLE:
                   {
                              
                              sceKernelClearEventFlag (ndMediaObj->DecEventID, 0xFFFFFFFF);
                              sceKernelSetEventFlag (ndMediaObj->DecEventID, WAV_STATE_IDLE);
                              
                              sceKernelSleepThread ();
                              // Sospende il thread di gestione perchè non stiamo eseguendo
                              // decodifica. Questo permette di risparmiare cicli di clock
                              // Nanodesktop risveglierà il thread al prossimo cambiamento
                              // di stato
                              
                              break;
                   }
                   
              case WAV_STATE_RESET:
                   {
                              
                              lseek (ndMediaObj->MediaHandle, ndMediaObj->WAV_PosStartData, SEEK_SET); 
                               
                              INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_PLAYING);
                              ndMediaObj->DecoderError       = 0; 
                              ndMediaObj->WAV_SamplesDecoded = 0;
                              
                              sceKernelClearEventFlag (ndMediaObj->DecEventID, 0xFFFFFFFF);
                              sceKernelSetEventFlag (ndMediaObj->DecEventID, WAV_STATE_RESET);
                              break;
                   }
              
              case WAV_STATE_PLAYING:
                   {
                              // Setta il flag eventi a Playing. Il segnale Playing viene inviato
                              // esclusivamente quando il sistema rileva un cambiamento rispetto
                              // allo stato precedente: questo approccio diminuisce il carico
                              // sullo scheduler
                              
                              if (ndMediaObj->DecoderState != ndMediaObj->old_DecoderState)
                              {  
                                 sceKernelClearEventFlag (ndMediaObj->DecEventID, 0xFFFFFFFF);
                                 sceKernelSetEventFlag (ndMediaObj->DecEventID, WAV_STATE_PLAYING);
                              }
                              
                              // Adesso determina il SampleRate ed il BitRate per il prossimo frame
                              // se c'è un errore ricomincia il loop
                              
                              ErrRep=INTERNAL_WAV_RetrieveNextFrame (ndMediaObj);
                              
                              if (ErrRep) 
                              {
                                  // Quando un file termina, è qui che si verifica un errore
                                  
                                  INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_EOF);
                                  break;
                              }
                              
                              // Provvede alla correzione sign format. I campioni wave ad 8 bit sono
                              // memorizzati come unsigned char a 8 bit (per esempio, il valore 0 è
                              // rappresentato da 0x80, -1 è rappresentato da 0x7F ecc.
                              // Nanodesktop si aspetta che i dati ad 8 bit che vengono passati
                              // siano signed char. Per tale ragione, viene eseguita una conversione
                              // al volo
                              
                              if (ndMediaObj->WAV_HwOutputBitRate==8)
                              {
                                  for (Counter=0; Counter<WAV_L1_BUFFER_SIZE; Counter++)
                                  {
                                      A = (int) (*(unsigned char *)(ndMediaObj->WAV_L1Buffer + Counter));
                                      *(char *)(ndMediaObj->WAV_L1Buffer + Counter) = A - 0x80;
                                  }                                       
                              }
                              
                              // Provvede a suonare i campioni che sono stati decodificati
                              ndHAL_SND_WriteToChannel (ndMediaObj->ndSndChannel, ndMediaObj->WAV_L1Buffer, ndMediaObj->WAV_SamplesRead, 1.0);
                              
                              // Aggiorna il numero di samples decodificati
                              ndMediaObj->WAV_SamplesDecoded = ndMediaObj->WAV_SamplesDecoded + ndMediaObj->WAV_SamplesRead;                   
                              
                              
                              if (ndMediaObj->CallBack!=0)
                              {
                                  // Calcola il numero di secondi che sono trascorsi dall'inizio della
                                  // riproduzione: SecPassed = SamplesDecoded / SampleRate
                                                            
                                  SecPassed = ndMediaObj->WAV_SamplesDecoded * InvSamplesForSec;
                                  
                                  HH = SecPassed / 3600;
                                  SecPassed = SecPassed % 3600;
                                  MM = SecPassed / 60;
                                  SecPassed = SecPassed % 60;
                                  SS = SecPassed;
                                  
                                  (*PntToMediaCallback)(ndMediaObj->InfoField, ndMediaObj, ndMediaObj->WAV_SamplesDecoded, HH, MM, SS);   
                              }
                              
                              // Nuovo controllo: il numero di samples decodificati può superare o uguagliare
                              // il numero di samples complessivi del wave. In quel caso occorre uscire
                              
                              if (ndMediaObj->WAV_SamplesDecoded>=ndMediaObj->WAV_SamplesInTotal)
                              {
                                  INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_EOF);
                                  break;
                              }
                              
                              break;     
                   }
                   
            case WAV_STATE_EOF:
                   {
                              SndChannel = ndMediaObj->ndSndChannel;
                              
                              // Esegue il flush dei dati sul canale
                              
                              ndHAL_SND_FlushChannel (SndChannel); 
                              
                              // Aumenta di 1 il numero di volte in cui è stato suonato il file
          
                              ndMediaObj->NrTimesPlay++;
                              
                              if (ndMediaObj->Flags & FLAG_AUTOREWIND_ON)
                              {
                                  INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_RESET);
                                  ndMediaObj->DecoderError = 0; 
                                  // Farà sì che il sistema ricominci la riproduzione                  
                              }
                              else
                              {
                                   INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_STOP);
                                   ndMediaObj->DecoderError = 0; 
                                   // Esegui la segnalazione di interruzione e poi porta
                                   // il sistema in idle
                              }
                              
                              break; 
                   }
                   
            case WAV_STATE_STOP:
                   {
                              SndChannel = ndMediaObj->ndSndChannel;
                              
                              ndHAL_SND_CoercitiveStop (SndChannel);
                              // Ferma la riproduzione da parte del buffer audio
                              
                              INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_INT);
                              // Vai nello stato di interruzione: questo segnalerà l'avvenuta 
                              // interruzione della riproduzione e successivamente riporterà il 
                              // sistema in stato idle
                              
                              ndMediaObj->NrTimesPlay++;
                              break; 
                   }
            
            case WAV_STATE_INT:
                   {
                              sceKernelClearEventFlag (ndMediaObj->DecEventID, 0xFFFFFFFF);
                              sceKernelSetEventFlag (ndMediaObj->DecEventID, WAV_STATE_INT);
                              
                              INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_IDLE);
                              // In decoder error qui già c'è il codice di errore
                              
                              break;
                   }
       }
       
   }    
          
}


/*
    ---------------------------------------------------------------------------
    WAV: API A LIVELLO SUPERIORE
    ---------------------------------------------------------------------------
*/

int INTERNAL_WAV_LoadFile (char *NameFile, int NrChannel, struct ndMediaObj_Type *ndMediaObj, void *CallBack, ndint64 InfoField)
{
    char InternalBuffer0 [255];
    char InternalBuffer1 [255];
    
    int MediaHandle;
    int ErrRep;
    
    MediaHandle = open (NameFile, O_RDONLY, 0777);
                    
    if (MediaHandle>=0)
    {
           ndMediaObj->MediaHandle = MediaHandle; 
           
           // Alloca il buffer di primo livello per i WAVE: il codec hardware provvederà 
           // a decodificare i dati in questo buffer
           
           ndMediaObj->WAV_L1Buffer = malloc (WAV_L1_BUFFER_SIZE); 
           memset (ndMediaObj->WAV_L1Buffer, 0, WAV_L1_BUFFER_SIZE);
           
           // Determina le caratteristiche dei wave
                                    
           ErrRep = INTERNAL_WAV_DeterminateWAVFeatures (ndMediaObj);
           
           if (!ErrRep)
           {
                  //  Verifica che il SampleRate sia consentito
                               
                  if ( INTERNAL_WAV_FeaturesAreAllowed (ndMediaObj) )
                  {                                         
                          ndMediaObj->ndSndChannel = ndHAL_SND_OpenChannel (NrChannel, ndMediaObj->WAV_HwSampleRate, ndMediaObj->WAV_HwOutputBitRate, ndMediaObj->WAV_HwOutputMode, 100, 100, NO_CHECK_FREQUENCY_CORRECTNESS);
                        
                          // I processi di analisi preliminare del file sono completati. Avvia il
                          // decoder thread
                           
                          WAV_DecoderThreadsActive++;
                           
                          // Costruisce il nome evento (WavEvent0, WavEvent1 ecc. ecc.)
                          
                          strcpy (InternalBuffer0, "WavEvent");
                          _itoa (WAV_DecoderThreadsActive, &InternalBuffer1, 10);
                          strcat (InternalBuffer0, InternalBuffer1);
                          
                          // Crea un nuovo flag event che si occuperà di questo canale
                          ndMediaObj->DecEventID = sceKernelCreateEventFlag  (InternalBuffer0, PSP_EVENT_WAITMULTIPLE, 0, 0);   
                            
                          // Costruisce il nome thread (WavThread0, WavThread1 ecc. ecc.)

                          strcpy (InternalBuffer0, "WavThread");
                          _itoa (WAV_DecoderThreadsActive, &InternalBuffer1, 10);
                          strcat (InternalBuffer0, InternalBuffer1);
                          
                          // Crea un nuovo thread che si occuperà di questo canale
                          ndMediaObj->DecThreadID = sceKernelCreateThread (InternalBuffer0, INTERNAL_WAV_DecoderThread, 0x09, 0x8000, PSP_THREAD_ATTR_USER, NULL);
                          
                          // E' necessario lanciare un messaggio al thread: l'indirizzo dell'object
                          MessageToThread = (int)(ndMediaObj);
                          
                          // Registra la callback e l'InfoField
                          ndMediaObj->CallBack  = CallBack;
                          ndMediaObj->InfoField = InfoField;
                          
                          // Avvia una nuova istanza del thread di decodifica degli WAV
                          sceKernelStartThread (ndMediaObj->DecThreadID, 4, &MessageToThread);
                          
                          return 0;            
                  }
                  else    // Il sample rate non rientra tra i valori consentiti
                  {
                      free (ndMediaObj->WAV_L1Buffer);
                      // Disalloca il buffer L1
                      
                      close (MediaHandle);
                      // Chiudi il file multimediale
           
                      return ERR_MEDIA_WRONG_HEADER; 
                  }
           }
           else    // C'è stato un problema nel leggere l'header file
           {
                 free (ndMediaObj->WAV_L1Buffer);
                 // Disalloca il buffer L1
                 
                 close (MediaHandle);
                 // Chiudi il file multimediale
       
                 return ERR_MEDIA_WRONG_HEADER;        
           }
                                              
    }   
    else  // Errore nell'apertura del file
       return ERR_MEDIA_I_CANNOT_OPEN_FILE; 
}

int INTERNAL_WAV_Play (struct ndMediaObj_Type *ndMediaObj, ndint64 Options)
{
    
    INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_PLAYING);
    // Avvierà la riproduzione del file WAV
    
    if (Options & ND_PLAY_AUTOREWIND)  ndMediaObj->Flags = ndMediaObj->Flags | FLAG_AUTOREWIND_ON;
    // Abilita l'autorewind se è stato richiesto
    
    sceKernelWaitEventFlag (ndMediaObj->DecEventID, WAV_STATE_PLAYING, PSP_EVENT_WAITAND, 0, 0);
    
    if (Options & ND_LOCK_UNTIL_SONG_FINISH)
    {
        // Attendi che il thread di decodifica abbia decodificato tutto il brano
        
        sceKernelWaitEventFlag (ndMediaObj->DecEventID, WAV_STATE_INT, PSP_EVENT_WAITAND, 0, 0);         
    }
    
    return 0;
}

int INTERNAL_WAV_Stop (struct ndMediaObj_Type *ndMediaObj)
{
    if (ndMediaObj->DecoderState==WAV_STATE_PLAYING)
    {
        INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_STOP);
        // Avvia il processo di terminazione della riproduzione dello stream
        
        sceKernelWaitEventFlag (ndMediaObj->DecEventID, WAV_STATE_INT, PSP_EVENT_WAITAND, 0, 0); 
        // Attendi finchè il sistema non ha effettivamente fermato la riproduzione
    }
    
    return 0;
}

int INTERNAL_WAV_Pause (struct ndMediaObj_Type *ndMediaObj)
{
    if (ndMediaObj->DecoderState==WAV_STATE_PLAYING)
    {
        INTERNAL_WAV_ChangeDecoderState (ndMediaObj, WAV_STATE_STOP);
        // Avvia il processo di terminazione della riproduzione dello stream
        
        sceKernelWaitEventFlag (ndMediaObj->DecEventID, WAV_STATE_INT, PSP_EVENT_WAITAND, 0, 0); 
        // Attendi finchè il sistema non ha effettivamente fermato la riproduzione
    }
    
    return 0;
}

int INTERNAL_WAV_GoToSample (struct ndMediaObj_Type *ndMediaObj, int NrSample)
{
    int OldSeekPos,  OldSamplesDecoded; 
    int NewPosToGo,  NewPosAfterSeek;
    
    if (NrSample<=ndMediaObj->WAV_SamplesInTotal)
    {
        INTERNAL_WAV_Stop (ndMediaObj);   // Arresta la riproduzione
        
        // Salva i vecchi parametri qualora non riuscisse la fase di seek
        
        OldSeekPos = lseek (ndMediaObj->MediaHandle, 0, SEEK_CUR);
        // Determina la vecchia posizione del puntatore
    
        OldSamplesDecoded = ndMediaObj->WAV_SamplesDecoded;
        // Salva il vecchio numero numero di campioni che già era stato decodificato
    
        NewPosToGo = ndMediaObj->WAV_PosStartData + ndMediaObj->WAV_HwSampleSize*NrSample;
        // Calcola la posizione dove deve spostarsi il nuovo puntatore sul disco...
        
        NewPosAfterSeek = lseek (ndMediaObj->MediaHandle, NewPosToGo, SEEK_SET);
        // e cerca di spostare effettivamente il puntatore
        
        if (NewPosToGo==NewPosAfterSeek)      // Il seek è andato a buon fine
        {
            ndMediaObj->WAV_SamplesDecoded = NrSample;
            return 0;                                  
        }
        else
        {
            // Prima di riportare un codice di errore alla routine chiamante, devi ripristinare
            // la vecchia posizione del puntatore nel file e del numero di samples decodificati
           
            lseek (ndMediaObj->MediaHandle, OldSeekPos, SEEK_SET);
            ndMediaObj->WAV_SamplesDecoded = OldSamplesDecoded;
           
            // Segnala all'utente l'errore 
            return ERR_MEDIA_SAMPLE_IS_OUT_OF_FILE;   
        }
    }
    else   // Si richiede l'accesso ad un campione che non esiste
    {
        return ERR_MEDIA_SAMPLE_IS_OUT_OF_FILE; 
    }
}

int INTERNAL_WAV_GetNrSamples (struct ndMediaObj_Type *ndMediaObj)
{
    return (ndMediaObj->WAV_SamplesInTotal);
}


int INTERNAL_WAV_GoToAnIstant (struct ndMediaObj_Type *ndMediaObj, int HH, int MM, float SS)
{
    float NrSeconds;
    int   NrSample;
    
    NrSeconds = HH*3600 + MM*60 + SS;
    NrSample    = (int)(ndMediaObj->WAV_SampleRate*NrSeconds);
    
    return INTERNAL_WAV_GoToSample (ndMediaObj, NrSample);
}

int INTERNAL_WAV_UnLoad (struct ndMediaObj_Type *ndMediaObj)
{
    
    if (ndMediaObj->DecoderState==WAV_STATE_PLAYING) INTERNAL_WAV_Pause (ndMediaObj);
    // Arresta il processo di riproduzione se è già avviato
    
    
    // DISATTIVAZIONE DEL DECODER THREAD
    
    ndMediaObj->DecoderExitRq=1;
    if (ndMediaObj->DecoderState==WAV_STATE_IDLE) sceKernelWakeupThread (ndMediaObj->DecThreadID);
    
    sceKernelDelayThread (100000);      // Pausa per dare il tempo al thread decoder di stabilizzarsi
    
    sceKernelTerminateDeleteThread (ndMediaObj->DecThreadID);
    WAV_DecoderThreadsActive--;
    
    // DISATTIVAZIONE DEL FLAG EVENTI
    
    sceKernelDeleteEventFlag (ndMediaObj->DecEventID); 
    
    // RILASCIO DEL CANALE AUDIO
    
    ndHAL_SND_CloseChannel (ndMediaObj->ndSndChannel);        // Rilascia il canale usato per la riproduzione audio
    
    // LIBERARE LE AREE DI MEMORIA
    
    free (ndMediaObj->WAV_L1Buffer);                          // Disalloca il buffer L1
                                        
    // CHIUDERE FILE MULTIMEDIALE
    
    close (ndMediaObj->MediaHandle);                          // Chiudi il file multimediale
    
    return 0;     
}


/*  ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    API PUBBLICA
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/

void ndHAL_InitMediaService (void)
{
     /* Niente da fare */
}



int ndHAL_MEDIA_LoadFile (char *NameFile, int NrChannel, struct ndMediaObj_Type *ndMediaObj, void *CallBack, ndint64 InfoField)
/*
     Questa funzione carica un file dal disco. Alcune informazioni vengono 
     memorizzate nella struct ndMediaObj (Nanodesktop media object).
     Le successive chiamate dovranno essere fatte richiamando funzioni come
     play, stop ecc., a cui viene passato l'indirizzo dell'oggetto
     in questione (ndMediaObj). 
     
     Il valore Channel indica il numero di canale che dovrà essere usato
     per la riproduzione
     
     Il parametro CallBack è un parametro che indica una callback che
     viene richiamata ogni volta che viene decodificato un frame audio. 
     Se si pone questo parametro a 0, si disattiva la callback. 
     
     Il prototipo di questa MediaCallback è il seguente:
     
     int MediaCallback (ndint64 InfoField, struct ndMediaObj_Type *ndMediaObj, 
                        int NrSample, int HH, int MM, int SS)
     
     Alla callback viene passato il numero di frame, convertito in hh/mm/sec. 
     l'indirizzo della struct ndMediaObj, dal quale la callback può 
     ottenere tutte le informazioni necessarie, il numero di frame 
     attualmente decodificati, ed il corrispondente numero di ore
     minuti e secondi trascorsi 
     
     Se tutto và a buon fine, la routine restituisce 0. Altrimenti, la 
     routine restituisce un codice di errore.
     
     Sono previsti i seguenti codici di errore:
     ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED          Il supporto per i file multimediali è stato
                                                  disattivato
     ERR_MEDIA_CHANNEL_ALREADY_BUSY               Il canale è stato già impegnato
     ERR_MEDIA_FILE_NOT_FOUND                     Il file non esiste    
     ERR_MEDIA_I_CANNOT_OPEN_FILE                 Il file non può essere aperto
     ERR_MEDIA_CODEC_HAS_NOT_MEMORY               Il codec non ha memoria sufficiente
     ERR_MEDIA_TROUBLE_IN_EDRAM                   Problema nella gestione EDRAM da parte del
                                                  firmware
     ERR_MEDIA_TROUBLE_IN_MPEG_CODEC_INIT         Problema nella reinizializzazione del codec MP3
     ERR_MEDIA_WRONG_HEADER                       L'header Mp3 è errato
     ERR_MEDIA_UNSUPPORTED_TYPE                   Il file non è supportato
*/
{
     char Path    [255];
     char Name    [255];
     char Ext     [25];
     char ExtMagn [25];
     
     if (!ND_HWMODE_DisableMediaService)
     {
         if (!ndAudioDescriptor [NrChannel].IsActive)
         {
               if (fexist (NameFile))
               {
                    strcpy ( &(ndMediaObj->NameFile), NameFile );
                    // Ricopia il nome del file dentro l'oggetto
                    
                    fssSplitNameFile (NameFile, &Path, &Name, &Ext);
                    ndStrCpy (&ExtMagn, &Ext, 25, 1);
                    
                    // Setta il campo Type al valore corretto
                    
                    ndMediaObj->Type=-1;
                    
                    if (!strcmp (ExtMagn, "WAV")) ndMediaObj->Type = MEDIA_TYPE_WAV;
                    if (!strcmp (ExtMagn, "MP3")) ndMediaObj->Type = MEDIA_TYPE_MP3;
                    
                    // Ciclo switch
                    
                    switch (ndMediaObj->Type)
                    {
                           case MEDIA_TYPE_WAV:
                           {
                                return INTERNAL_WAV_LoadFile (NameFile, NrChannel, ndMediaObj, CallBack, InfoField);  
                           }
                           
                           default:
                           {
                                return ERR_MEDIA_UNSUPPORTED_TYPE;    
                           }
                    }
            
               }
               else return ERR_MEDIA_FILE_NOT_FOUND;                
         }
         else return ERR_MEDIA_CHANNEL_ALREADY_BUSY;                  
     }
     else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;  
} 



int ndHAL_MEDIA_Play (struct ndMediaObj_Type *ndMediaObj, ndint64 Options)
/*
     Questa routine provvede ad avviare la riproduzione di un file. 
     La riproduzione inizia dal punto in cui era stata interrotta.
     
     In condizioni normali, il sistema non blocca il thread chiamante fin 
     quando  la riproduzione non viene terminata. Pertanto, l'utente
     avvia la riproduzione ed il thread che contiene la chiamata
     a questa routine nel frattempo prosegue. 
      
     Il parametro Options è un parametro a 64 che serve ad impostare
     alcune opzioni. I suoi bit sono mappati nel seguente modo:
            
     0    Settato ad 1 quando si chiede che il decoder, giunto alla fine,
          avvii il processo di riproduzione. Usare la costante manifesta
          ND_PLAY_AUTOREWIND  
          
     1    Settato ad 1 quando si chiede che il sistema blocchi il thread
          chiamante fin quando non viene terminata la riproduzione del
          file. Usare la costante manifesta ND_LOCK_UNTIL_SONG_FINISH
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
*/
{
     if (!ND_HWMODE_DisableMediaService)
     {
          switch (ndMediaObj->Type)
          {
                case MEDIA_TYPE_WAV:
                {
                    return INTERNAL_WAV_Play (ndMediaObj, Options);  
                }  
          }
     }
     else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;  
}



int ndHAL_MEDIA_Stop (struct ndMediaObj_Type *ndMediaObj)
/*
     Questa funzione blocca istantamente la riproduzione di un file. 
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
*/
{
     if (!ND_HWMODE_DisableMediaService)
     {
          switch (ndMediaObj->Type)
          {
                case MEDIA_TYPE_WAV:
                {
                    return INTERNAL_WAV_Stop (ndMediaObj);  
                } 
          }
     }
     else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;  
}



int ndHAL_MEDIA_Pause (struct ndMediaObj_Type *ndMediaObj)
/*
     Questa funzione blocca istantamente la riproduzione di un file. 
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
*/
{
     if (!ND_HWMODE_DisableMediaService)
     {
          switch (ndMediaObj->Type)
          {
                case MEDIA_TYPE_WAV:
                {
                    return INTERNAL_WAV_Pause (ndMediaObj);  
                }  
          }
     }
     else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;  
}

int ndHAL_MEDIA_GetNrSamples (struct ndMediaObj_Type *ndMediaObj)
/*
     Questa funzione restituisce il numero di samples del file media. Se la
     funzione restituisce un valore negativo, allora ciò significa che 
     si è verificato un errore
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
*/
{
     if (!ND_HWMODE_DisableMediaService)
     {
          switch (ndMediaObj->Type)
          {
                case MEDIA_TYPE_WAV:
                {
                    return INTERNAL_WAV_GetNrSamples (ndMediaObj);  
                } 
                  
          }
     }
     else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;                                         
}


int ndHAL_MEDIA_GoToSample (struct ndMediaObj_Type *ndMediaObj, int NrSample)
/*
     Questa funzione arresta la riproduzione del file media e porta il
     relativo puntatore al sample NrSample. 
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
       ERR_MEDIA_SAMPLE_IS_OUT_OF_FILE      Il campione è fuori dal file
*/
{
     if (!ND_HWMODE_DisableMediaService)
     {
          switch (ndMediaObj->Type)
          {
                case MEDIA_TYPE_WAV:
                {
                    return INTERNAL_WAV_GoToSample (ndMediaObj, NrSample);  
                }   
          }
     }
     else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;  
}


int ndHAL_MEDIA_GoToAnIstant (struct ndMediaObj_Type *ndMediaObj, int HH, int MM, float SS)
/*
     Questa funzione arresta la riproduzione del file media e porta il
     relativo puntatore all'istante determinato da HH:MM:SS 
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
       ERR_MEDIA_SAMPLE_IS_OUT_OF_FILE      Il campione è fuori dal file
*/
{
     if (!ND_HWMODE_DisableMediaService)
     {
          switch (ndMediaObj->Type)
          {
                case MEDIA_TYPE_WAV:
                {
                    return INTERNAL_WAV_GoToAnIstant (ndMediaObj, HH, MM, SS);  
                }  
          }
     }
     else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;  
}



int ndHAL_MEDIA_UnLoad (struct ndMediaObj_Type *ndMediaObj)
/*
     Questa funzione scarica dalla memoria un file multimediale. Le
     aree di memoria che erano state allocate vengono scaricate dalla
     memoria ed il decoder thread viene arrestato e cancellato 
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
*/
{
     if (!ND_HWMODE_DisableMediaService)
     {
          switch (ndMediaObj->Type)
          {
                case MEDIA_TYPE_WAV:
                {
                    return INTERNAL_WAV_UnLoad (ndMediaObj);  
                }  
          }
     }
     else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;  
}


char ndHAL_MEDIA_ThisFileIsSupported (char *NameFile)
{
     char Buffer [256];
     char Path [256];
     char Name [256];
     char Ext  [16];
     
     ndStrCpy (Buffer, NameFile, strlen (NameFile), 1);
     fssSplitNameFile (&Buffer, &Path, &Name, &Ext);
     
     if (!strcmp (Ext, "WAV")) return 1;
     
     return 0; 
}



