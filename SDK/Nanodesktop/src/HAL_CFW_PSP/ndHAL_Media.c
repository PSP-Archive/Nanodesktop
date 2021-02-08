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
    HAL: FUNZIONI DECODIFICA FORMATI AUDIO (PSP CFW mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>

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






/*
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                                   DECODIFICATORE MP3
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/




// --------------------------------------------------------------------------
// MP3:SIMBOLI 
// --------------------------------------------------------------------------

#define MP3_LAYER3   1
#define MP3_LAYER2   2
#define MP3_LAYER1   3

#define MP3_VERSION_2_5     0
#define MP3_VERSION_2       2
#define MP3_VERSION_1       3




static int _Mp3SampleRateList_[4][3] = 
{
    {11025, 12000, 8000,},//mpeg 2.5
    {0, 0, 0,}, //reserved
    {22050, 24000, 16000,},//mpeg 2
    {44100, 48000, 32000}//mpeg 1
};

static int _Mp3BitRateList_ [5][16]= {{0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448}, // V1 L1
                              {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384}, // V1 L2 
                              {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 }, // V1 L3 *
                              {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256 }, // V2 L1
                              {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 } }; // V2, L2 L3

int MP3_DecoderThreadsActive = 0;


// --------------------------------------------------------------------------
// MP3:CODICE DI DECODIFICA A BASSO LIVELLO 
// --------------------------------------------------------------------------

static inline void INTERNAL_MP3_ChangeDecoderState (struct ndMediaObj_Type *MediaObj, int NewState)
{
    int IsNecessaryWakeUp = 0;
    
    if (MediaObj->DecoderState==MP3_STATE_IDLE) IsNecessaryWakeUp=1;
    // Preparati a risvegliare il thread che era stato posto in sleep
    
    MediaObj->old_DecoderState =  MediaObj->DecoderState;
    MediaObj->DecoderState     =  NewState;  
    
    if (IsNecessaryWakeUp) sceKernelWakeupThread (MediaObj->DecThreadID);
    // Il thread viene risvegliato dopo il cambiamento di stato
}


int INTERNAL_MP3_DeterminateNextFrameStart (SceUID fd)
/*
    Questa funzione provvede a determinare la posizione nel file MP3 del 
    prossimo frame.
    
    La funzione proviene dal prx Music 0.55 di joek2100 
*/
{
    int offset = 0;
    unsigned char buf[1024];
    unsigned char *pBuffer;
    int i;
    int size = 0;

    offset = lseek (fd, 0, SEEK_CUR);
    read (fd, buf, sizeof(buf));
    if (!strncmp((char*)buf, "ID3", 3) || !strncmp((char*)buf, "ea3", 3)) //skip past id3v2 header, which can cause a false sync to be found
    {
        //get the real size from the syncsafe int
        size = buf[6];
        size = (size<<7) | buf[7];
        size = (size<<7) | buf[8];
        size = (size<<7) | buf[9];

        size += 10;

        if (buf[5] & 0x10) //has footer
            size += 10;
    }

    lseek (fd, offset + size, SEEK_SET); //now seek for a sync
    while(1) 
    {
        offset = lseek (fd, 0, SEEK_CUR);
        size = read (fd, buf, sizeof(buf));

        if (size <= 2)//at end of file
            return -1;
    
        if (!strncmp((char*)buf, "EA3", 3))//oma mp3 files have non-safe ints in the EA3 header
        {
            lseek (fd, (buf[4]<<8)+buf[5], SEEK_CUR);
            continue;
        }

        pBuffer = buf;
        for( i = 0; i < size; i++)
        {
            //if this is a valid frame sync (0xe0 is for mpeg version 2.5,2+1)
            
            if ( (pBuffer[i] == 0xff) && ((pBuffer[i+1] & 0xE0) == 0xE0))
            {
                offset += i;
                lseek (fd, offset, SEEK_SET);
                return offset;
            }
        }
        
        //go back two bytes to catch any syncs that on the boundary
        lseek (fd, -2, SEEK_CUR);
    } 
}


int INTERNAL_MP3_DeterminateFeaturesOfNextFrame (struct ndMediaObj_Type *ndMediaObj)
/*
    Questa routine è in grado di determinare le caratteristiche del prossimo 
    frame Mp3. La routine viene richiamata periodicamente dall'engine di
    decodifica durante la fase di processamento del file MP3.
*/
{
     unsigned char mp3_header_buf[4];
     
     ndMediaObj->MP3_DataStart = INTERNAL_MP3_DeterminateNextFrameStart (ndMediaObj->MediaHandle);
        
     if ( read ( ndMediaObj->MediaHandle, mp3_header_buf, 4 ) != 4 ) 
     {
         return -1;          // Segnala alla routine chiamante che c'è un errore
     }
        
     int mp3_header = mp3_header_buf[0];
     mp3_header = (mp3_header<<8) | mp3_header_buf[1];
     mp3_header = (mp3_header<<8) | mp3_header_buf[2];
     mp3_header = (mp3_header<<8) | mp3_header_buf[3];
        
     // Memorizza la versione del file MP3 ed il layer MP3
     
     ndMediaObj->MP3_BitRateIndex = (mp3_header & 0xf000) >> 12;
     ndMediaObj->MP3_Version = (mp3_header & 0x180000) >> 19;
     ndMediaObj->MP3_Layer = (mp3_header & 0x60000) >> 17;
     ndMediaObj->MP3_Padding = (mp3_header & 0x200) >> 9;
     ndMediaObj->MP3_StereoMode = (mp3_header & 0xC0) >> 6;
     
     // Determina il SampleRate della registrazione
     
     ndMediaObj->MP3_SampleRate = _Mp3SampleRateList_ [ndMediaObj->MP3_Version][ (mp3_header & 0xC00) >> 10 ];
     
     // Determina il bitrate type
     
     if ( (ndMediaObj->MP3_Version==MP3_VERSION_1) && (ndMediaObj->MP3_Layer==MP3_LAYER1) )
        ndMediaObj->MP3_BitRateSeq=0;
        
     if ( (ndMediaObj->MP3_Version==MP3_VERSION_1) && (ndMediaObj->MP3_Layer==MP3_LAYER2) )
        ndMediaObj->MP3_BitRateSeq=1;
        
     if ( (ndMediaObj->MP3_Version==MP3_VERSION_1) && (ndMediaObj->MP3_Layer==MP3_LAYER3) )
        ndMediaObj->MP3_BitRateSeq=2;
        
     if ( ((ndMediaObj->MP3_Version==MP3_VERSION_2) || (ndMediaObj->MP3_Version==MP3_VERSION_2_5)) 
                 && (ndMediaObj->MP3_Layer==MP3_LAYER1) )
     {            
          ndMediaObj->MP3_BitRateSeq=3;
     } 
        
     if ( ((ndMediaObj->MP3_Version==MP3_VERSION_2) || (ndMediaObj->MP3_Version==MP3_VERSION_2_5)) 
                 && ((ndMediaObj->MP3_Layer==MP3_LAYER2) || (ndMediaObj->MP3_Layer==MP3_LAYER3))  )
     {
          ndMediaObj->MP3_BitRateSeq=4;       
     }        
     
     ndMediaObj->MP3_BitRate = _Mp3BitRateList_ [ndMediaObj->MP3_BitRateSeq][ndMediaObj->MP3_BitRateIndex];
     
     // Setta i parametri in modo compatibile con la versione MP3
     
     if (ndMediaObj->MP3_Version==MP3_VERSION_1)
     {
         ndMediaObj->MP3_FrameCostant = 144;
         ndMediaObj->MP3_SamplesForFrame = MP3_L1_BUFFER_SIZE;
     }
     else   // MPEG versione 2 o 2.5
     {
         ndMediaObj->MP3_FrameCostant = 72;
         ndMediaObj->MP3_SamplesForFrame = MP3_L1_BUFFER_SIZE/2;
     }
     
     // Esegui la correzione del numero di MP3_SamplesForFrame: per i files mono
     // tale valore deve essere raddioppiato
     
     if (ndMediaObj->MP3_StereoMode == 3)
     {
        ndMediaObj->MP3_SamplesForFrame = ndMediaObj->MP3_SamplesForFrame*2; 
     }
     
     // Determina l'ampiezza del frame
     
     ndMediaObj->MP3_FrameSize = ndMediaObj->MP3_FrameCostant*(ndMediaObj->MP3_BitRate*1000)/(ndMediaObj->MP3_SampleRate) + ndMediaObj->MP3_Padding;                                                   
     return 0;
}




int INTERNAL_MP3_DeterminateMp3Features (struct ndMediaObj_Type *ndMediaObj)
/*
     Questa routine è in grado di determinare alcune caratteristiche 
     dell'MP3. Se il processo va a buon fine, la routine restituisce
     il valore 0, altrimenti restituisce un codice di errore

     Il bitrate viene memorizzato in ndMediaObj->SampleRate
*/
{
     int ErrRep;            
     
     // Provvedi a portarti all'inizio del file e preleva le informazioni sul file
     
     lseek (ndMediaObj->MediaHandle, 0, SEEK_SET);
     ErrRep = INTERNAL_MP3_DeterminateFeaturesOfNextFrame (ndMediaObj);
     
     // Determina l'HwSampleRate e l'HwOutputMode: questi parametri saranno passati a
     // ndHAL_SND_OpenChannel che provvederà a settare la periferica audio di uscita
     // nel modo corretto
     
     if (!ErrRep)
     {
            switch (ndMediaObj->MP3_StereoMode)
            {
                case 0:                     // Stereo
                {
                     ndMediaObj->MP3_HwSampleRate = ndMediaObj->MP3_SampleRate;
                     ndMediaObj->MP3_HwOutputMode = ND_AUDIO_STEREO;
                     ndMediaObj->MP3_HwSampleSize = 4;
                     break;
                }
                
                case 1:                     // Joint stereo o stereo
                {
                     ndMediaObj->MP3_HwSampleRate = ndMediaObj->MP3_SampleRate;
                     ndMediaObj->MP3_HwOutputMode = ND_AUDIO_STEREO;
                     ndMediaObj->MP3_HwSampleSize = 4;
                     break;
                }
                
                case 2:                    // Dual channel
                {
                     ndMediaObj->MP3_HwSampleRate = ndMediaObj->MP3_SampleRate;
                     ndMediaObj->MP3_HwOutputMode = ND_AUDIO_STEREO;
                     ndMediaObj->MP3_HwSampleSize = 4;
                     break;
                }
                
                case 3:                    // Mono
                {
                     // Attenzione: il sample rate reale deve essere raddoppiato.
                     // Noi consideriamo il sample rate in campioni (mono e stereo
                     // non importa) al secondo. Gli MP3 invece mostrano i campioni
                     // scritti nel file al secondo, quindi il valore che presentano
                     // è esattamente la metà di quello reale
                     
                     ndMediaObj->MP3_HwSampleRate = ndMediaObj->MP3_SampleRate*2;
                     ndMediaObj->MP3_HwOutputMode = ND_AUDIO_MONO;
                     ndMediaObj->MP3_HwSampleSize = 2;
                     break;
                } 
            }
         
             
             // Riporta il puntatore file a 0
             
             lseek (ndMediaObj->MediaHandle, 0, SEEK_SET);
             return 0;               // Segnala alla routine chiamante che non ci sono errori
     }
     else
        return ErrRep;
}


char INTERNAL_MP3_SampleRateIsAllowed (struct ndMediaObj_Type *ndMediaObj)
{
     switch (ndMediaObj->MP3_SampleRate)
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
                 return 1;
            default:
                 return 0;
     }
}




void INTERNAL_MP3_DecoderThread (SceSize args, void *argp)
{  
     
   void (*PntToMediaCallback)(ndint64 InfoField, struct ndMediaObj_Type *ndMediaObj, 
                             int NrSample, int HH, int MM, int SS);
   
   struct ndMediaObj_Type *ndMediaObj;
   int    HH, MM, SS, SecPassed;
   float  InvSamplesForSec;
   int    ErrRep;
   int    SndChannel;
   
   // E' necessario innanzitutto riprendere l'indirizzo della struttura 
   // ndMediaObj che contengono le informazioni che ci servono
   
   int PntToAddress = *(int *)(argp);
   ndMediaObj = PntToAddress;
   
   // Indirizzo recuperato: inizia il ciclo di decodifica
                                                        
   ndMediaObj->MP3_FlexibleDataBufferCreated = 0;
   
   ndMediaObj->DecoderState  = -1;
   // Viene posto a -1, in modo che al successivo cambio di stato, il vecchio ed
   // il nuovo valore siano diversi
   
   INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_IDLE);
   ndMediaObj->DecoderError  = 0;
   ndMediaObj->DecoderExitRq = 0;
   
   ndMediaObj->NrTimesPlay   = 0;
   ndMediaObj->MP3_SamplesDecoded = 0;
   
   // Pone a 0 i flags evento
   sceKernelSetEventFlag (ndMediaObj->DecEventID, 0);
   
   // Setup della callback
   PntToMediaCallback = ndMediaObj->CallBack;

   // Calcola il reciproco del SampleRate
   if (ndMediaObj->MP3_SampleRate!=0)
   {
      InvSamplesForSec = 1.0/(float)(ndMediaObj->MP3_SampleRate);
   }
   
   //Decoding loop:
   while ( !ndMediaObj->DecoderExitRq )
   {
       sceDisplayWaitVblankStart ();
       // Necessario per impedire il blocco del sistema
       
       switch (ndMediaObj->DecoderState)
       {
              case MP3_STATE_IDLE:
                   {
                              sceKernelClearEventFlag (ndMediaObj->DecEventID, 0xFFFFFFFF);
                              sceKernelSetEventFlag   (ndMediaObj->DecEventID, MP3_STATE_IDLE);
                              
                              sceKernelSleepThread ();
                              // Sospende il thread di gestione perchè non stiamo eseguendo
                              // decodifica. Questo permette di risparmiare cicli di clock
                              // Nanodesktop risveglierà il thread al prossimo cambiamento
                              // di stato
                              
                              break;
                   }
                   
              case MP3_STATE_RESET:
                   {
                              lseek (ndMediaObj->MediaHandle, 0, SEEK_SET); 
                               
                              INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_PLAYING);
                              ndMediaObj->DecoderError       = 0; 
                              ndMediaObj->MP3_SamplesDecoded = 0;
                              
                              sceKernelClearEventFlag (ndMediaObj->DecEventID, 0xFFFFFFFF);
                              sceKernelSetEventFlag (ndMediaObj->DecEventID, MP3_STATE_RESET);
                              break;
                   }
              
              case MP3_STATE_PLAYING:
                   {
                              // Setta il flag eventi a Playing. Il segnale Playing viene inviato
                              // esclusivamente quando il sistema rileva un cambiamento rispetto
                              // allo stato precedente: questo approccio diminuisce il carico
                              // sullo scheduler
                              
                              if (ndMediaObj->DecoderState != ndMediaObj->old_DecoderState)
                              {  
                                 sceKernelClearEventFlag (ndMediaObj->DecEventID, 0xFFFFFFFF);
                                 sceKernelSetEventFlag (ndMediaObj->DecEventID, MP3_STATE_PLAYING);
                              }
                              
                              // Adesso determina il SampleRate ed il BitRate per il prossimo frame
                              // se c'è un errore ricomincia il loop
                              
                              ErrRep=INTERNAL_MP3_DeterminateFeaturesOfNextFrame (ndMediaObj);
                              
                              if (ErrRep) 
                              {
                                  // Quando un file termina, è qui che si verifica un errore
                                  
                                  INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_EOF);
                                  break;
                              }
                              
                              // Occupati di gestire l'allocazione del buffer MP3 resizeable
                              
                              if (!ndMediaObj->MP3_FlexibleDataBufferCreated)
                              {
                                   // E' la prima volta che viene allocato il DataBuffer resizeable
                                   
                                   ndMediaObj->MP3_FlexibleDataBuffer = (u8*)memalign(64, ndMediaObj->MP3_FrameSize);
                                   
                                   ndMediaObj->MP3_FlexibleDataBufferOldSize = ndMediaObj->MP3_FrameSize;
                                   ndMediaObj->MP3_FlexibleDataBufferCreated = 1;
                              }
                              else
                              {
                                   // C'è già un buffer di tipo resizeable in memoria: provvedi al free 
                                   // ed alla riallocazione solo se è necessario 
                                   
                                   if (ndMediaObj->MP3_FlexibleDataBufferOldSize!=ndMediaObj->MP3_FrameSize)
                                   {
                                       free (ndMediaObj->MP3_FlexibleDataBuffer);
                                       ndMediaObj->MP3_FlexibleDataBuffer = (u8*)memalign(64, ndMediaObj->MP3_FrameSize);                                                                    
                                   
                                       ndMediaObj->MP3_FlexibleDataBufferOldSize = ndMediaObj->MP3_FrameSize;
                                       ndMediaObj->MP3_FlexibleDataBufferCreated = 1;
                                   }
                              } 
                              
                              // Il puntatore file si era spostato allo scopo di leggere 
                              // l'header del frame. Ora riportati all'inizio del frame
                              
                              lseek (ndMediaObj->MediaHandle, ndMediaObj->MP3_DataStart, SEEK_SET); //seek back
                              
                              // Copia nel buffer flessibile i dati MP3 da decodificare
                    
                              if ( read ( ndMediaObj->MediaHandle, ndMediaObj->MP3_FlexibleDataBuffer, ndMediaObj->MP3_FrameSize ) != ndMediaObj->MP3_FrameSize ) 
                              {
                                    INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_INT);
                                    ndMediaObj->DecoderError = DECODER_ERROR;  
                                    
                                    break;
                              }
                              
                              // Aggiorna DataStart in modo che al prossimo ciclo, il
                              // sistema richiederà il frame in un punto diverso da
                              // quello attuale (cioè richiamerà il frame successivo)
                    
                              ndMediaObj->MP3_DataStart += ndMediaObj->MP3_FrameSize;
                              
                              // Prepara la CommCodecArea per passare informazioni all'Engine
                    
                              ndMediaObj->MP3_CommCodecArea[6] = (unsigned long)ndMediaObj->MP3_FlexibleDataBuffer;
                              ndMediaObj->MP3_CommCodecArea[7] = ndMediaObj->MP3_FrameSize;
                              ndMediaObj->MP3_CommCodecArea[8] = (unsigned long)ndMediaObj->MP3_L1Buffer;
                              ndMediaObj->MP3_CommCodecArea[9]  = ndMediaObj->MP3_SamplesForFrame * ndMediaObj->MP3_HwSampleSize;
                              ndMediaObj->MP3_CommCodecArea[10] = ndMediaObj->MP3_FrameSize;
                              
                              // Esegue la decodifica dei campioni MP3 che erano stati raccolti
                    
                              int res = sceAudiocodecDecode(ndMediaObj->MP3_CommCodecArea, 0x1002);
                              
                              if ( res < 0 ) 
                              {
                                    INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_INT);
                                    ndMediaObj->DecoderError = DECODER_ERROR;  
                                    
                                    break;
                              }
                              
                              // Provvede a suonare i campioni che sono stati decodificati
                              ndHAL_SND_WriteToChannel (ndMediaObj->ndSndChannel, ndMediaObj->MP3_L1Buffer, ndMediaObj->MP3_SamplesForFrame, 1.0);
                              
                              // Aggiorna il numero di samples decodificati
                              ndMediaObj->MP3_SamplesDecoded = ndMediaObj->MP3_SamplesDecoded + ndMediaObj->MP3_SamplesForFrame;                   
                              
                              
                              if ((ndMediaObj->CallBack!=0) && (ndMediaObj->MP3_SampleRate))
                              {
                                  // Calcola il numero di secondi che sono trascorsi dall'inizio della
                                  // riproduzione: SecPassed = SamplesDecoded / SampleRate
                                                            
                                  SecPassed = ndMediaObj->MP3_SamplesDecoded * InvSamplesForSec;
                                  
                                  HH = SecPassed / 3600;
                                  SecPassed = SecPassed % 3600;
                                  MM = SecPassed / 60;
                                  SecPassed = SecPassed % 60;
                                  SS = SecPassed;
                                  
                                  (*PntToMediaCallback)(ndMediaObj->InfoField, ndMediaObj, ndMediaObj->MP3_SamplesDecoded, HH, MM, SS);   
                              }
                              
                              
                              break;     
                   }
                   
            case MP3_STATE_EOF:
                   {
                              SndChannel = ndMediaObj->ndSndChannel;
                              
                              // Esegue il flush dei dati sul canale
                              
                              ndHAL_SND_FlushChannel (SndChannel); 
                              
                              // Aumenta di 1 il numero di volte in cui è stato suonato il file
                              
                              ndMediaObj->NrTimesPlay++;
                              
                              if (ndMediaObj->Flags & FLAG_AUTOREWIND_ON)
                              {
                                  INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_RESET);
                                  ndMediaObj->DecoderError = 0; 
                                  // Farà sì che il sistema ricominci la riproduzione                  
                              }
                              else
                              {
                                  INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_STOP);
                                  ndMediaObj->DecoderError = 0; 
                                  // Esegui la segnalazione di interruzione e poi porta
                                  // il sistema in idle
                              }
                              
                              break; 
                   }
                   
            case MP3_STATE_STOP:
                   {
                              SndChannel = ndMediaObj->ndSndChannel;
                              
                              ndHAL_SND_CoercitiveStop (SndChannel);
                              // Ferma la riproduzione da parte del buffer audio
                               
                              INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_INT);
                              // Vai nello stato di interruzione: questo segnalerà l'avvenuta 
                              // interruzione della riproduzione e successivamente riporterà il 
                              // sistema in stato idle
                              
                              ndMediaObj->NrTimesPlay++;
                              break; 
                   }
            
            case MP3_STATE_INT:
                   {
                              sceKernelClearEventFlag (ndMediaObj->DecEventID, 0xFFFFFFFF);
                              sceKernelSetEventFlag (ndMediaObj->DecEventID, MP3_STATE_INT);
                              
                              INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_IDLE);
                              // In decoder error qui già c'è il codice di errore
                              
                              break;
                   }
       }
       
   }  
   
   // All'uscita dal thread, cancella il buffer flessibile  
   if (ndMediaObj->MP3_FlexibleDataBufferCreated) free (ndMediaObj->MP3_FlexibleDataBuffer);                
   return 0;    
}


/*
    ---------------------------------------------------------------------------
    MP3: API A LIVELLO SUPERIORE
    ---------------------------------------------------------------------------
*/

int INTERNAL_MP3_LoadFile (char *NameFile, int NrChannel, struct ndMediaObj_Type *ndMediaObj, void *CallBack, ndint64 InfoField)
{
    char InternalBuffer0 [255];
    char InternalBuffer1 [255];
    
    int MediaHandle;
    int ErrRep;
    
    MediaHandle = open (NameFile, O_RDONLY, 0777);
                    
    if (MediaHandle>=0)
    {
           ndMediaObj->MediaHandle = MediaHandle; 
        
           // Alloca la CommArea del codec MP3 della PSP e provvedi a reinizializzarlo
           // riempiendolo di zeri (la commarea deve essere 64 alligned)
           
           ndMediaObj->MP3_CommCodecArea = memalign (64, 65*sizeof(unsigned long int)); 
           memset (ndMediaObj->MP3_CommCodecArea, 0, 65*sizeof(unsigned long int));
           
           // Alloca il buffer di primo livello per gli MP3: il codec hardware provvederà 
           // a decodificare i dati in questo buffer
           
           ndMediaObj->MP3_L1Buffer = memalign (64, MP3_L1_BUFFER_SIZE*2*sizeof(short int)); 
           memset (ndMediaObj->MP3_L1Buffer, 0, MP3_L1_BUFFER_SIZE*2*sizeof(short int));
           
           // Reinizializza il codec audio: controlla se c'è memoria sufficiente
        
           if ( sceAudiocodeCheckNeedMem(ndMediaObj->MP3_CommCodecArea, 0x1002) >= 0 )
           {   
                       // Fatti fornire l'EDRAM dal codec
 
                       if ( sceAudiocodecGetEDRAM(ndMediaObj->MP3_CommCodecArea, 0x1002) >= 0 )
                       {
                               ndMediaObj->MP3_GetEDRAM = 1;
                            
                               // Reinizializzazione del codec
                               
                               if ( sceAudiocodecInit(ndMediaObj->MP3_CommCodecArea, 0x1002) >= 0 ) 
                               {
                                       // Provvedi a ricavare le informazioni su questo MP3 dal file
                                       
                                       ErrRep = INTERNAL_MP3_DeterminateMp3Features (ndMediaObj);
                                       
                                       if (!ErrRep)
                                       {
                                              //  Verifica che il SampleRate sia consentito
                                                           
                                              if ( INTERNAL_MP3_SampleRateIsAllowed (ndMediaObj) )
                                              {             
                                                          
                                                          
                                                          ndMediaObj->ndSndChannel = ndHAL_SND_OpenChannel (NrChannel, ndMediaObj->MP3_HwSampleRate, ND_AUDIO_16BIT, ndMediaObj->MP3_HwOutputMode, 100, 100, NO_CHECK_FREQUENCY_CORRECTNESS);
                                                        
                                                          // I processi di analisi preliminare del file sono completati. Avvia il
                                                          // decoder thread
                                                           
                                                          MP3_DecoderThreadsActive++;
                                                           
                                                          // Costruisce il nome evento (Mp3Event0, Mp3Event1 ecc. ecc.)
                                                          
                                                          strcpy (InternalBuffer0, "Mp3Event");
                                                          _itoa (MP3_DecoderThreadsActive, &InternalBuffer1, 10);
                                                          strcat (InternalBuffer0, InternalBuffer1);
                                                          
                                                          // Crea un nuovo flag event che si occuperà di questo canale
                                                          ndMediaObj->DecEventID = sceKernelCreateEventFlag  (InternalBuffer0, PSP_EVENT_WAITMULTIPLE, 0, 0);   
                                                            
                                                          // Costruisce il nome thread (Mp3Thread0, Mp3Thread1 ecc. ecc.)
              
                                                          strcpy (InternalBuffer0, "Mp3Thread");
                                                          _itoa (MP3_DecoderThreadsActive, &InternalBuffer1, 10);
                                                          strcat (InternalBuffer0, InternalBuffer1);
                                                          
                                                          // Crea un nuovo thread che si occuperà di questo canale
                                                          ndMediaObj->DecThreadID = sceKernelCreateThread (InternalBuffer0, INTERNAL_MP3_DecoderThread, 0x09, 0x8000, PSP_THREAD_ATTR_USER, NULL);
                                                          
                                                          // E' necessario lanciare un messaggio al thread: l'indirizzo dell'object
                                                          MessageToThread = (int)(ndMediaObj);
                                                          
                                                          // Registra la callback e l'InfoField
                                                          ndMediaObj->CallBack  = CallBack;
                                                          ndMediaObj->InfoField = InfoField;
                                                          
                                                          // Avvia una nuova istanza del thread di decodifica degli MP3
                                                          sceKernelStartThread (ndMediaObj->DecThreadID, 4, &MessageToThread);
                                                          
                                                          return 0;
                                                           
                                              }
                                              else    // Il sample rate non rientra tra i valori consentiti
                                              {
                                                  free (ndMediaObj->MP3_L1Buffer);
                                                  // Disalloca il buffer L1
                                                  
                                                  free (ndMediaObj->MP3_CommCodecArea);
                                                  // Disalloca la Codec Comm Area
                                                  
                                                  close (MediaHandle);
                                                  // Chiudi il file multimediale
                                       
                                                  return ERR_MEDIA_WRONG_HEADER; 
                                              }
                                       }
                                       else    // C'è stato un problema nel leggere l'header file
                                       {
                                             free (ndMediaObj->MP3_L1Buffer);
                                             // Disalloca il buffer L1
                                             
                                             free (ndMediaObj->MP3_CommCodecArea);
                                             // Disalloca la Codec Comm Area
                                             
                                             close (MediaHandle);
                                             // Chiudi il file multimediale
                                   
                                             return ERR_MEDIA_WRONG_HEADER;        
                                       }
                               }
                               else    // Problema nella reinizializzazione del codec
                               {
                                     free (ndMediaObj->MP3_L1Buffer);
                                     // Disalloca il buffer L1
                                     
                                     free (ndMediaObj->MP3_CommCodecArea);
                                     // Disalloca la Codec Comm Area
                                     
                                     close (MediaHandle);
                                     // Chiudi il file multimediale
                           
                                     return ERR_MEDIA_TROUBLE_IN_MPEG_CODEC_INIT;  
                               }
                       }
                       else    // Problema nella gestione EDRAM da parte del firmware
                       {
                           free (ndMediaObj->MP3_L1Buffer);
                           // Disalloca il buffer L1
                           
                           free (ndMediaObj->MP3_CommCodecArea);
                           // Disalloca la Codec Comm Area
                           
                           close (MediaHandle);
                           // Chiudi il file multimediale
                           
                           return ERR_MEDIA_TROUBLE_IN_EDRAM;
                       }
                       
              }
              else     // Il codec audio non ha abbastanza memoria per funzionare
              {
                       free (ndMediaObj->MP3_L1Buffer);
                       // Disalloca il buffer L1
                       
                       free (ndMediaObj->MP3_CommCodecArea);
                       // Disalloca la Codec Comm Area
                           
                       close (MediaHandle);
                       // Chiudi il file multimediale
                       
                       return ERR_MEDIA_CODEC_HAS_NOT_MEMORY;
              }                               
    }   
    else  // Errore nell'apertura del file
       return ERR_MEDIA_I_CANNOT_OPEN_FILE;  
}


int INTERNAL_MP3_Play (struct ndMediaObj_Type *ndMediaObj, ndint64 Options)
{
    INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_PLAYING);
    // Avvierà la riproduzione del file MP3
    
    if (Options & ND_PLAY_AUTOREWIND)  ndMediaObj->Flags = ndMediaObj->Flags | FLAG_AUTOREWIND_ON;
    // Abilita l'autorewind se è stato richiesto
    
    sceKernelWaitEventFlag (ndMediaObj->DecEventID, MP3_STATE_PLAYING, PSP_EVENT_WAITAND, 0, 0);
    
    if (Options & ND_LOCK_UNTIL_SONG_FINISH)
    {
        // Attendi che il thread di decodifica abbia decodificato tutto il brano
        
        sceKernelWaitEventFlag (ndMediaObj->DecEventID, MP3_STATE_INT, PSP_EVENT_WAITAND, 0, 0);         
    }
    
    return 0; 
}

int INTERNAL_MP3_Stop (struct ndMediaObj_Type *ndMediaObj)
{
    if (ndMediaObj->DecoderState==MP3_STATE_PLAYING)
    {
        INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_STOP);
        // Avvia il processo di terminazione della riproduzione dello stream
        
        sceKernelWaitEventFlag (ndMediaObj->DecEventID, MP3_STATE_INT, PSP_EVENT_WAITAND, 0, 0); 
        // Attendi finchè il sistema non ha effettivamente fermato la riproduzione
    }
    
    return 0;
}

int INTERNAL_MP3_Pause (struct ndMediaObj_Type *ndMediaObj)
{
    if (ndMediaObj->DecoderState==MP3_STATE_PLAYING)
    {
        INTERNAL_MP3_ChangeDecoderState (ndMediaObj, MP3_STATE_STOP);
        // Avvia il processo di terminazione della riproduzione dello stream
        
        sceKernelWaitEventFlag (ndMediaObj->DecEventID, MP3_STATE_INT, PSP_EVENT_WAITAND, 0, 0); 
        // Attendi finchè il sistema non ha effettivamente fermato la riproduzione
    }
    
    return 0;
}

int INTERNAL_MP3_GetNrSamples (struct ndMediaObj_Type *ndMediaObj)
/*
     Gli MP3 non hanno un campo che indichi quanti sono i samples che
     sono in tutto contenuti nel file, quindi è necessario eseguire 
     tutta la scansione del file dall'inizio alla fine
*/
{
    int YouCanExit,  ErrRep;
    int OldSeekPos,  OldSamplesDecoded;
    int IntermSeekPos;
    int SamplesAlreadyCounted;
    
    INTERNAL_MP3_Stop (ndMediaObj);   // Arresta la riproduzione
    
    // Salva i vecchi parametri qualora non riuscisse la fase di seek
    
    OldSeekPos = lseek (ndMediaObj->MediaHandle, 0, SEEK_CUR);
    // Determina la vecchia posizione del puntatore

    OldSamplesDecoded = ndMediaObj->MP3_SamplesDecoded;
    // Salva il vecchio numero numero di campioni che già era stato decodificato

    // Inizia la fase di scan del file: riportati all'inizio
    
    lseek (ndMediaObj->MediaHandle, 0, SEEK_SET); 
    SamplesAlreadyCounted=0;                                          
    
    YouCanExit=0;
    
    while (!YouCanExit)
    {                
          ErrRep=INTERNAL_MP3_DeterminateFeaturesOfNextFrame (ndMediaObj);
          // Analizza il frame e provvede a determinare quanti samples contiene:
          // questo metterà il numero di samples del frame corrente in
          // ndMediaObj->MP3_SamplesForFrame. Inoltre porrà l'inizio del
          // frame in ndMediaObj->MP3_DataStart
          
          if (!ErrRep)         // Sono riuscito a leggere tutto il frame
          {
               lseek (ndMediaObj->MediaHandle, ndMediaObj->MP3_DataStart, SEEK_SET); 
               // Il puntatore del file si era spostato allo scopo di leggere l'header
               // del frame: adesso riportati nella posizione DataStart
               
               lseek (ndMediaObj->MediaHandle, ndMediaObj->MP3_FrameSize, SEEK_CUR); 
               // Porta il puntatore nella nuova posizione: significa avanzare nel file
               // di una quantità pari a FrameSize a partire dalla posizione attuale:
               // è esattamente quanto viene fatto dal sistema con read quando si
               // esegue realmente la decodifica MP3
               
               ndMediaObj->MP3_DataStart += ndMediaObj->MP3_FrameSize;
               // Simula l'aggiornamento di DataStart in modo che al prossimo ciclo, il
               // sistema richiederà il frame in un punto diverso da quello attuale
               // (cioè richiamerà il frame successivo)               
               
               SamplesAlreadyCounted += ndMediaObj->MP3_SamplesForFrame;
               // Simula l'esecuzione dei samples: le cose vanno "come se" fossero stati decodificati
               // ulteriori campioni
               
          }  
          else    // Non sono riuscito a leggere tutto il frame: vuol dire che abbiamo il numero di frame che servono
          {
               // Prima di riportare il numero di samples richiesti alla routine chiamante,
               // ripristina la vecchia posizione del puntatore
               
               lseek (ndMediaObj->MediaHandle, OldSeekPos, SEEK_SET);
               ndMediaObj->MP3_SamplesDecoded = OldSamplesDecoded;
               
               // Segnala all'utente l'errore 
               return SamplesAlreadyCounted;   
          }         
    }
}


int INTERNAL_MP3_GoToSample (struct ndMediaObj_Type *ndMediaObj, int NrSample)
{
    int YouCanExit,  ErrRep;
    int FirstSample, LastSample;
    int OldSeekPos,  OldSamplesDecoded;
    int IntermSeekPos;
    
    INTERNAL_MP3_Stop (ndMediaObj);   // Arresta la riproduzione
    
    // Salva i vecchi parametri qualora non riuscisse la fase di seek
    
    OldSeekPos = lseek (ndMediaObj->MediaHandle, 0, SEEK_CUR);
    // Determina la vecchia posizione del puntatore

    OldSamplesDecoded = ndMediaObj->MP3_SamplesDecoded;
    // Salva il vecchio numero numero di campioni che già era stato decodificato

    // Inizia la fase di scan del file
    
    if (NrSample<ndMediaObj->MP3_SamplesDecoded)
    {
        lseek (ndMediaObj->MediaHandle, 0, SEEK_SET); 
        ndMediaObj->MP3_SamplesDecoded=0;                                          
    }
    
    YouCanExit=0;
    
    while (!YouCanExit)
    {                
          IntermSeekPos = lseek (ndMediaObj->MediaHandle, 0, SEEK_CUR);
          // Provvede a salvare la posizione nel file prima di leggere tutto
          // il frame
          
          ErrRep=INTERNAL_MP3_DeterminateFeaturesOfNextFrame (ndMediaObj);
          // Analizza il frame e provvede a determinare quanti samples contiene:
          // questo metterà il numero di samples del frame corrente in
          // ndMediaObj->MP3_SamplesForFrame. Inoltre porrà l'inizio del
          // frame in ndMediaObj->MP3_DataStart
          
          if (!ErrRep)         // Sono riuscito a leggere tutto il frame
          {
               lseek (ndMediaObj->MediaHandle, ndMediaObj->MP3_DataStart, SEEK_SET); 
               // Il puntatore del file si era spostato allo scopo di leggere l'header
               // del frame: adesso riportati nella posizione DataStart
               
               FirstSample = ndMediaObj->MP3_SamplesDecoded;
               // Determina il primo sample del frame
               
               LastSample = ndMediaObj->MP3_SamplesDecoded + ndMediaObj->MP3_SamplesForFrame;
               // Determina l'ultimo sample del frame
               
               if ((FirstSample<=NrSample) && (NrSample<=LastSample))   // Siamo nel punto giusto
               { 
                   // Ripristina la posizione del puntatore nel file: il decodificatore
                   // provvederà a decodificare i frames proprio a partire da questo punto
                   lseek (ndMediaObj->MediaHandle, IntermSeekPos, SEEK_SET);
                   
                   // Torna alla routine chiamante segnalando che è tutto ok
                   return 0;                         
               }
               
               lseek (ndMediaObj->MediaHandle, ndMediaObj->MP3_FrameSize, SEEK_CUR); 
               // Porta il puntatore nella nuova posizione: significa avanzare nel file
               // di una quantità pari a FrameSize a partire dalla posizione attuale:
               // è esattamente quanto viene fatto dal sistema con read quando si
               // esegue realmente la decodifica MP3
               
               ndMediaObj->MP3_DataStart += ndMediaObj->MP3_FrameSize;
               // Simula l'aggiornamento di DataStart in modo che al prossimo ciclo, il
               // sistema richiederà il frame in un punto diverso da quello attuale
               // (cioè richiamerà il frame successivo)               
               
               ndMediaObj->MP3_SamplesDecoded += ndMediaObj->MP3_SamplesForFrame;
               // Simula l'esecuzione dei samples: le cose vanno "come se" fossero stati decodificati
               // ulteriori campioni
               
          }  
          else    // Non sono riuscito a leggere tutto il frame
          {
               // Prima di riportare un codice di errore alla routine chiamante, devi ripristinare
               // la vecchia posizione del puntatore nel file e del numero di samples decodificati
               
               lseek (ndMediaObj->MediaHandle, OldSeekPos, SEEK_SET);
               ndMediaObj->MP3_SamplesDecoded = OldSamplesDecoded;
               
               // Segnala all'utente l'errore 
               return ERR_MEDIA_SAMPLE_IS_OUT_OF_FILE;   
          }         
    }
}

int INTERNAL_MP3_GoToAnIstant (struct ndMediaObj_Type *ndMediaObj, int HH, int MM, float SS)
{
    float NrSeconds;
    int   NrSample;
    
    NrSeconds = HH*3600 + MM*60 + SS;
    NrSample    = (int)(ndMediaObj->MP3_SampleRate*NrSeconds);
    
    return INTERNAL_MP3_GoToSample (ndMediaObj, NrSample);
}



int INTERNAL_MP3_UnLoad (struct ndMediaObj_Type *ndMediaObj)
{
    if (ndMediaObj->DecoderState==MP3_STATE_PLAYING) INTERNAL_MP3_Pause (ndMediaObj);
    // Arresta il processo di riproduzione se è già avviato
    
    // DISATTIVAZIONE DEL DECODER THREAD
    
    ndMediaObj->DecoderExitRq=1;
    if (ndMediaObj->DecoderState==MP3_STATE_IDLE) sceKernelWakeupThread (ndMediaObj->DecThreadID);
    
    sceKernelDelayThread (100000);      // Pausa per dare il tempo al thread decoder di stabilizzarsi
    
    sceKernelTerminateDeleteThread (ndMediaObj->DecThreadID);
    MP3_DecoderThreadsActive--;
    
    // DISATTIVAZIONE DEL FLAG EVENTI
    
    sceKernelDeleteEventFlag (ndMediaObj->DecEventID); 
    
    // RILASCIO DEL CANALE AUDIO
    
    ndHAL_SND_CloseChannel (ndMediaObj->ndSndChannel);
    // Rilascia il canale usato per la riproduzione audio
    
    // LIBERARE LE AREE DI MEMORIA
    
    if ( ndMediaObj->MP3_GetEDRAM ) sceAudiocodecReleaseEDRAM (ndMediaObj->MP3_CommCodecArea);  
    // Rilascia area di memoria nel codec   
    
    free (ndMediaObj->MP3_L1Buffer);                          // Disalloca il buffer L1
    
    free (ndMediaObj->MP3_CommCodecArea);                     // Disalloca la Codec Comm Area
                                        
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
     if (!ND_HWMODE_DisableMediaService)
     {
            sceUtilityLoadAvModule(PSP_AV_MODULE_AVCODEC);                      
     }
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
     
     if (!ndKernelExtenderIsInError)
     {
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
                               
                               case MEDIA_TYPE_MP3:
                               {
                                    return INTERNAL_MP3_LoadFile (NameFile, NrChannel, ndMediaObj, CallBack, InfoField);  
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
     else return ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER;
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
     if (!ndKernelExtenderIsInError)
     {
         if (!ND_HWMODE_DisableMediaService)
         {
              switch (ndMediaObj->Type)
              {
                    case MEDIA_TYPE_WAV:
                    {
                        return INTERNAL_WAV_Play (ndMediaObj, Options);  
                    }
                    
                    case MEDIA_TYPE_MP3:
                    {
                        return INTERNAL_MP3_Play (ndMediaObj, Options);  
                    }  
              }
         }
         else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;      
     }
     else return ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER;
}



int ndHAL_MEDIA_Stop (struct ndMediaObj_Type *ndMediaObj)
/*
     Questa funzione blocca istantamente la riproduzione di un file. 
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
*/
{
     if (!ndKernelExtenderIsInError)
     { 
         if (!ND_HWMODE_DisableMediaService)
         {
              switch (ndMediaObj->Type)
              {
                    case MEDIA_TYPE_WAV:
                    {
                        return INTERNAL_WAV_Stop (ndMediaObj);  
                    }
                    
                    case MEDIA_TYPE_MP3:
                    {
                        return INTERNAL_MP3_Stop (ndMediaObj);  
                    }  
              }
         }
         else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;        
     }
     else return ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER;
}



int ndHAL_MEDIA_Pause (struct ndMediaObj_Type *ndMediaObj)
/*
     Questa funzione blocca istantamente la riproduzione di un file. 
     
     Sono previsti i seguenti codici di errore
       ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED  Il supporto per i files multimediali
                                            è stato disattivato
*/
{
     if (!ndKernelExtenderIsInError)
     {
         if (!ND_HWMODE_DisableMediaService)
         {
              switch (ndMediaObj->Type)
              {
                    case MEDIA_TYPE_WAV:
                    {
                        return INTERNAL_WAV_Pause (ndMediaObj);  
                    }
                    
                    case MEDIA_TYPE_MP3:
                    {
                        return INTERNAL_MP3_Pause (ndMediaObj);  
                    }  
              }
         }
         else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;    
     }
     else return ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER;  
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
     if (!ndKernelExtenderIsInError)
     {    
         if (!ND_HWMODE_DisableMediaService)
         {
              switch (ndMediaObj->Type)
              {
                    case MEDIA_TYPE_WAV:
                    {
                        return INTERNAL_WAV_GetNrSamples (ndMediaObj);  
                    } 
                    
                    case MEDIA_TYPE_MP3:
                    {
                        return INTERNAL_MP3_GetNrSamples (ndMediaObj);  
                    }  
              }
         }
         else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;      
     }
     else return ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER;                                       
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
     if (!ndKernelExtenderIsInError)
     {
         if (!ND_HWMODE_DisableMediaService)
         {
              switch (ndMediaObj->Type)
              {
                    case MEDIA_TYPE_WAV:
                    {
                        return INTERNAL_WAV_GoToSample (ndMediaObj, NrSample);  
                    } 
                    
                    case MEDIA_TYPE_MP3:
                    {
                        return INTERNAL_MP3_GoToSample (ndMediaObj, NrSample);  
                    }  
              }
         }
         else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;     
     }
     else return ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER;
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
     if (!ndKernelExtenderIsInError)
     {
         if (!ND_HWMODE_DisableMediaService)
         {
              switch (ndMediaObj->Type)
              {
                    case MEDIA_TYPE_WAV:
                    {
                        return INTERNAL_WAV_GoToAnIstant (ndMediaObj, HH, MM, SS);  
                    } 
                    
                    case MEDIA_TYPE_MP3:
                    {
                        return INTERNAL_MP3_GoToAnIstant (ndMediaObj, HH, MM, SS);  
                    }  
              }
         }
         else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED;  
     }
     else return ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER;
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
     if (!ndKernelExtenderIsInError)
     {
         if (!ND_HWMODE_DisableMediaService)
         {
              switch (ndMediaObj->Type)
              {
                    case MEDIA_TYPE_WAV:
                    {
                        return INTERNAL_WAV_UnLoad (ndMediaObj);  
                    }
                    
                    case MEDIA_TYPE_MP3:
                    {
                        return INTERNAL_MP3_UnLoad (ndMediaObj);  
                    }  
              }
         }
         else return ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED; 
     }
     else return ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER;  
}


char ndHAL_MEDIA_ThisFileIsSupported (char *NameFile)
{
     char Buffer [256];
     char Path [256];
     char Name [256];
     char Ext  [16];
     
     ndStrCpy (Buffer, NameFile, strlen (NameFile), 1);
     fssSplitNameFile (&Buffer, &Path, &Name, &Ext);
     
     if (!strcmp (Ext, "MP3")) return 1;
     if (!strcmp (Ext, "WAV")) return 1;
     
     return 0; 
}

/*
    --------------------------------------------------------------------------
    TERMINAZIONE SERVIZI MULTIMEDIALI
    --------------------------------------------------------------------------
*/


void INTERNAL_TerminateMedia ()
{
    if (!ND_HWMODE_DisableMediaService)
    {
         sceUtilityUnloadAvModule(PSP_AV_MODULE_AVCODEC);    
    }     
}



