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
    GESTORE AUDIO (PSP KSU mode)
    ----------------------------------------------------------------------------
*/          

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

/*
   ----------------------------------------------------------------------------
   PROTOTIPI DI FUNZIONI GESTITE MEDIANTE STUBS
   ----------------------------------------------------------------------------
*/

int sceAudioSetFrequency (int frequency); 

/*
   ----------------------------------------------------------------------------
   SIMBOLI INTERNI
   ----------------------------------------------------------------------------
*/

#define PSP_AUDIO_VOLUME_MAX 0x8000	
#define PSP_POSTAMP_LEVEL    7


/*
   ----------------------------------------------------------------------------
   STRUTTURE INTERNE
   ----------------------------------------------------------------------------
*/


char Audio_YouCanExit [NR_AUDIO_DESCRIPTORS];
static int MessageToThread;



/*
   ----------------------------------------------------------------------------
   THREAD PER LA GESTIONE DELL'AUDIO
   ----------------------------------------------------------------------------
*/

void INTERNAL_DecodeThread (SceSize args, void *argp)
{
     register int *PntToNrHandle = argp;
     register int SndHandle = *PntToNrHandle;      // Recupera l'handle audio a cui appartieni
     
     register int Counter, RD_Buffer;
     register int IdleStateCounter;
     register int Count0, Count1, Count2;
     register int VolumeDx, VolumeSx;
     
     Audio_YouCanExit  [SndHandle]=0;
     ndAudioDescriptor [SndHandle].CoercitiveStop=0;
     
     // Pone a 0 i flag evento
     sceKernelSetEventFlag (ndAudioDescriptor [SndHandle].EventID, 0);
     ndAudioDescriptor [SndHandle].GeneralState = SND_STATE_PLAYING;
      
     while (!Audio_YouCanExit [SndHandle])
     {
           __CycleRestart:
           
           switch (ndAudioDescriptor [SndHandle].GeneralState)
           {
                  case SND_STATE_IDLE:
                  {
                         sceKernelClearEventFlag (ndAudioDescriptor [SndHandle].EventID, 0xFFFFFFFF);
                         sceKernelSetEventFlag (ndAudioDescriptor [SndHandle].EventID, SND_STATE_IDLE);
                         // Segnala ai thread che sono fermi in attesa che lo stato del sistema è
                         // in idle
                         
                         sceKernelSleepThread ();
                         // Metti in sleep il thread. La routine di scrittura provvederà a riattivarlo
                         // quando necessario
                         
                         break;
                  }
                  
                  case SND_STATE_PLAYING:
                  {
                         sceKernelClearEventFlag (ndAudioDescriptor [SndHandle].EventID, 0xFFFFFFFF);
                         sceKernelSetEventFlag (ndAudioDescriptor [SndHandle].EventID, SND_STATE_PLAYING);
                         
                         RD_Buffer = ndAudioDescriptor [SndHandle].RD_Buffer;
         
                         IdleStateCounter=0;
                         
                         while (ndAudioDescriptor [SndHandle].SoundData->State [RD_Buffer]!=BUFFER_WRITTEN_AND_READY_FOR_PLAY)
                         {
                               IdleStateCounter++;

                               if (IdleStateCounter>10)
                               {
                                  ndAudioDescriptor [SndHandle].GeneralState = SND_STATE_IDLE;
                                  
                                  // Necessario per consentire il corretto funzionamento di flush
                                  sceKernelDelayThread (500000);
                                  
                                  // Esegue la commutazione nello stato IDLE
                                  goto __CycleRestart; 
                               }  
                               
                               sceKernelDelayThread (100000);
                         }
                         
                         ndAudioDescriptor [SndHandle].SoundData->State [RD_Buffer]=BUFFER_IS_BUSY_FOR_READ;
                         
                         for (Counter=0; (Counter<=ndAudioDescriptor [SndHandle].SoundData->SubBuffersReady [RD_Buffer]); Counter++)
                         {
                             VolumeDx = ndAudioDescriptor [SndHandle].HW_VolumeDX;
                             VolumeSx = ndAudioDescriptor [SndHandle].HW_VolumeSX;
                         
                             sceAudioOutputPannedBlocking (ndAudioDescriptor [SndHandle].ChannelHdr, VolumeSx, VolumeDx, &(ndAudioDescriptor [SndHandle].SoundData->Buffer[RD_Buffer][Counter][0]) );    
                         
                             if (ndAudioDescriptor [SndHandle].CoercitiveStop)
                             {
                                ndAudioDescriptor [SndHandle].GeneralState = SND_STATE_COERCITIVE;
                                goto __CycleRestart;
                             }                   
                         }
                         
                         ndAudioDescriptor [SndHandle].SoundData->State [RD_Buffer]=BUFFER_FREE_FOR_WRITE; 
                         ndAudioDescriptor [SndHandle].RD_Buffer = (RD_Buffer+1) % NR_BUFFER;      
                         break;    
                  }
                  
                  case SND_STATE_COERCITIVE:
                  {
                         sceKernelClearEventFlag (ndAudioDescriptor [SndHandle].EventID, 0xFFFFFFFF);
                         sceKernelSetEventFlag (ndAudioDescriptor [SndHandle].EventID, SND_STATE_COERCITIVE);
                         
                         ndAudioDescriptor [SndHandle].RD_Buffer=0;
                         ndAudioDescriptor [SndHandle].WR_Buffer=0;
    
                         ndAudioDescriptor [SndHandle].RD_SubBuffer=0;
                         ndAudioDescriptor [SndHandle].WR_SubBuffer=0;
    
                         ndAudioDescriptor [SndHandle].RD_Offset=0;
                         ndAudioDescriptor [SndHandle].WR_Offset=0;
                         
                         for (Count0=0; Count0<NR_BUFFER; Count0++)
                         {
                             ndAudioDescriptor [SndHandle].SoundData->State [Count0] = BUFFER_FREE_FOR_WRITE;
                         
                             for (Count1=0; Count1<NR_SUBBUFFER_FOR_BUFFER; Count1++)
                             {
                                 for (Count2=0; Count2<SAMPLES_FOR_SUBBUFFER; Count2++)
                                 {
                                     ndAudioDescriptor [SndHandle].SoundData->Buffer [Count0][Count1][Count2].l=0;
                                     ndAudioDescriptor [SndHandle].SoundData->Buffer [Count0][Count1][Count2].r=0;
                                 }
                             }
                         }
                  
                         ndAudioDescriptor [SndHandle].CoercitiveStop=0;
                         // Azzera il flag di coercizione in modo che al prossimo cambiamento
                         // di stato sia necessario reimpostarlo
                  
                         ndAudioDescriptor [SndHandle].GeneralState = SND_STATE_IDLE;
                         break;
                  }
           }
     }
          
     sceKernelExitDeleteThread (0);
}


void INTERNAL_ChangeBufferInWriting (int SndHandle)
{
    char ClosedBuffer, NewBuffer;
    
    ClosedBuffer = ndAudioDescriptor [SndHandle].WR_Buffer;
    
    ndAudioDescriptor [SndHandle].SoundData->SubBuffersReady [ClosedBuffer]=NR_SUBBUFFER_FOR_BUFFER-1;
    ndAudioDescriptor [SndHandle].SoundData->State           [ClosedBuffer]=BUFFER_WRITTEN_AND_READY_FOR_PLAY;
    
    NewBuffer=(ndAudioDescriptor [SndHandle].WR_Buffer+1) % NR_BUFFER;
    
    // Riattiva il thread audio se è in stato idle
    
    if (ndAudioDescriptor [SndHandle].GeneralState==SND_STATE_IDLE)
    {
        ndAudioDescriptor [SndHandle].GeneralState=SND_STATE_PLAYING;
        sceKernelWakeupThread (ndAudioDescriptor [SndHandle].ThreadID);                   
    } 
    
    // Mettiti in attesa che il buffer successivo si liberi
    
    while (ndAudioDescriptor [SndHandle].SoundData->State [NewBuffer] != BUFFER_FREE_FOR_WRITE)
    {
          sceDisplayWaitVblankStart();
    }
    
    ndAudioDescriptor [SndHandle].WR_Buffer   =NewBuffer;
    ndAudioDescriptor [SndHandle].WR_SubBuffer=0;
    ndAudioDescriptor [SndHandle].WR_Offset   =0;
    
    ndAudioDescriptor [SndHandle].SoundData->State [NewBuffer]=BUFFER_IS_BUSY_FOR_WRITE;
}


void INTERNAL_ChangeSubBufferInWriting (int SndHandle)
{
   ndAudioDescriptor [SndHandle].WR_SubBuffer++;
   ndAudioDescriptor [SndHandle].WR_Offset=0;
   
   if (ndAudioDescriptor [SndHandle].WR_SubBuffer==NR_SUBBUFFER_FOR_BUFFER)
   {
       INTERNAL_ChangeBufferInWriting (SndHandle);                                      
   }
}



/*
   ----------------------------------------------------------------------------
   API PUBBLICA
   ----------------------------------------------------------------------------
*/


void ndHAL_InitAudioApi ()
/*
    Provvede alla reinizializzazione del sistema audio
*/
{
     int Counter;
     
     for (Counter=0; Counter<NR_AUDIO_DESCRIPTORS; Counter++)
     {
         ndAudioDescriptor [Counter].IsActive = 0;
     }
     
     pspAudioInit (); 
     sceAudioSetFrequency (48000); 
}

int ndHAL_SND_OpenChannel (int NrDevice, int Frequency, int NrBits, int NrChannels, int VolumeSx, int VolumeDx, ndint64 Options)
/*
    Questa funzione fa sì che Nanodesktop assegni un handle per canale audio. 
    
    Sotto PSP, NrDevice rappresenta il numero di canale, compreso tra 0 ed 8. 
    Se si specifica -1, il sistema assegnerà il primo canale disponibile. 
    
    Il parametro NrBits può assumere valore pari a 8 o a 16. 
    
    Il parametro Options contiene dei bit che indica particolari opzioni di lavoro.
    
    Se si setta a 1 il primo bit, usando la costante manifesta
    NO_CHECK_FREQUENCY_CORRECTNESS, Nanodesktop non uscirà (segnalando un 
    codice di errore) qualora la frequenza di funzionamento non corrispondesse ad
    una delle frequenze standard usate dal sistema. Questa opzione viene
    generalmente usata per scopi interni.
    
    La frequenza viene ottenuta per interpolazione. Se tutto va a buon fine, 
    la routine restituisce un valore pari a 0 o positivo.
    
    I parametri VolumeDx e VolumeSx sono due parametri float il cui valore
    deve essere compreso tra 0 e 100. Se l'audio è mono, Nanodesktop
    considererà solo il parametro VolumeSx, mentre VolumeSx sarà
    ignorato.  
    
    Se si verifica qualche problema, viene restituito un codice di errore.
    Sono previsti i seguenti codici di errore:
         
         ERR_NO_HANDLE_AVAILABLE    Non ci sono handle disponibili
         ERR_WRONG_PARAMETERS       I parametri di frequenza o numero canali sono errati 
         ERR_MEMORY_ERROR           Memoria insufficiente per allocare il sound buffer
         ERR_IO_ERROR               Errore nell'assegnazione hardware del canale  
*/
{
    char InternalBuffer0 [20];
    char InternalBuffer1 [20];
    
    char Counter, IHaveFound, HandleFound;
    int  ChannelHdr;
    int  PSPStereoCode;
    
    void *BufferAlloc;
         
    IHaveFound=0;     
         
    for (Counter=0; Counter<NR_AUDIO_DESCRIPTORS; Counter++)
    {
        if (!ndAudioDescriptor [Counter].IsActive)
        {
            HandleFound = Counter;
            IHaveFound=1;                   
            
            break;                  
        }
    }  
    
    // Analizza la validità dei parametri
    
    if (IHaveFound)
    {
        switch (Frequency)
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
                    ndAudioDescriptor [HandleFound].VFreq = Frequency;
                    break;
               default:
                    {
                          if (Options & NO_CHECK_FREQUENCY_CORRECTNESS)
                              ndAudioDescriptor [HandleFound].VFreq = Frequency;                                
                          else
                              return ERR_WRONG_PARAMETERS;                                            
                    }
                    
        }           
        
        switch (NrChannels)
        {
               case ND_AUDIO_MONO:
               case ND_AUDIO_STEREO:
                    ndAudioDescriptor [HandleFound].NrChannels = NrChannels;
                    break;
               default:
                    return ERR_WRONG_PARAMETERS;                 
        }
        
        switch (NrBits)
        {
               case 8:
               case 16:
                    ndAudioDescriptor [HandleFound].NrBits = NrBits;
                    break;
               default:
                    return ERR_WRONG_PARAMETERS;    
        }
        
        if ( (VolumeDx<0) || (VolumeDx>100) ) return ERR_WRONG_PARAMETERS;    
        if ( (VolumeSx<0) || (VolumeSx>100) ) return ERR_WRONG_PARAMETERS;    
                
        // Fatti fornire l'identificatore di canale dal firmware della PSP
        
        switch (NrChannels)
        {
               case ND_AUDIO_MONO:
                    PSPStereoCode =  PSP_AUDIO_FORMAT_STEREO;
                    break;
               case ND_AUDIO_STEREO:
                    PSPStereoCode =  PSP_AUDIO_FORMAT_STEREO;
                    break;
               default:
                    return ERR_WRONG_PARAMETERS;                 
        }
        
        ChannelHdr = sceAudioChReserve (NrDevice, SAMPLES_FOR_SUBBUFFER, PSPStereoCode);
        
        if (ChannelHdr>=0)
        {
              ndAudioDescriptor [HandleFound].ChannelHdr=ChannelHdr;          
        
              // Esegui l'allocazione del SoundBuffer di canale
              
              BufferAlloc = malloc (sizeof (struct AudioBuffer_Type));
              
              if (BufferAlloc!=0)
              {
                  ndAudioDescriptor [HandleFound].SoundData = BufferAlloc; 
              
                  // Resetta l'area di memoria del buffer
                  
                  for (Counter=0; Counter<NR_BUFFER; Counter++)
                  {
                     ndAudioDescriptor [HandleFound].SoundData->State [Counter] = BUFFER_FREE_FOR_WRITE;
                  }         
                
                  // Setup dei puntatori buffer
                  
                  ndAudioDescriptor [HandleFound].RD_Buffer=0;
                  ndAudioDescriptor [HandleFound].WR_Buffer=0;
    
                  ndAudioDescriptor [HandleFound].RD_SubBuffer=0;
                  ndAudioDescriptor [HandleFound].WR_SubBuffer=0;
    
                  ndAudioDescriptor [HandleFound].RD_Offset=0;
                  ndAudioDescriptor [HandleFound].WR_Offset=0;
                  
                  // Setup del volume
                  
                  switch (NrChannels)
                  {
                       case ND_AUDIO_MONO:
                            ndAudioDescriptor [HandleFound].HW_VolumeDX = (VolumeSx*PSP_AUDIO_VOLUME_MAX)/100;
                            ndAudioDescriptor [HandleFound].HW_VolumeSX = (VolumeSx*PSP_AUDIO_VOLUME_MAX)/100;
                            break;
                       case ND_AUDIO_STEREO:
                            ndAudioDescriptor [HandleFound].HW_VolumeSX = (VolumeSx*PSP_AUDIO_VOLUME_MAX)/100;
                            ndAudioDescriptor [HandleFound].HW_VolumeDX = (VolumeDx*PSP_AUDIO_VOLUME_MAX)/100;
                            break;
                       default:
                            return ERR_WRONG_PARAMETERS;                 
                  }
                  
                  // Costruisce il nome per il nuovo event field
                  
                  strcpy (InternalBuffer0, "SndEvent");
                  _itoa (NrDevice, &InternalBuffer1, 10);
                  strcat (InternalBuffer0, InternalBuffer1);
                   
                  // Crea un nuovo event field
                  
                  ndAudioDescriptor [HandleFound].EventID = sceKernelCreateEventFlag  (InternalBuffer0, PSP_EVENT_WAITMULTIPLE, 0, 0);
                  
                  // Costruisce il nome thread (SndThread0, SndThread1 ecc. ecc.)
              
                  strcpy (InternalBuffer0, "SndThread");
                  _itoa (NrDevice, &InternalBuffer1, 10);
                  strcat (InternalBuffer0, InternalBuffer1);
                  
                  // Crea un nuovo thread che si occuperà di questo canale
                  ndAudioDescriptor [HandleFound].ThreadID = sceKernelCreateThread (InternalBuffer0, INTERNAL_DecodeThread, 0x09, 0x10000, 0, NULL);
                  
                  // E' necessario lanciare un messaggio al thread: il numero di handle a cui esso appartiene
                  MessageToThread=HandleFound;
                  
                  // Avvia una nuova istanza del thread di gestione del buffer audio
                  sceKernelStartThread (ndAudioDescriptor [HandleFound].ThreadID, 4, &MessageToThread);
                  
                  // Blocca l'handle audio e restituisce l'handle
            
                  ndAudioDescriptor [HandleFound].IsActive=1;
                  return HandleFound;
              }
              else
                return ERR_MEMORY_ERROR;
        }
        else
          return ERR_IO_ERROR; 
    }
    else           // Non è stato trovato alcun handle
      return ERR_NO_HANDLE_AVAILABLE;   
}




int ndHAL_SND_WriteToChannel (int NrChannel, void *samples, int NrSamples, float AmpFactor)
{
    register float OverSamples, InvOverSamples, PositionSample;
    register int   RealNrSamples;
    register int   Counter;
    register int   WR_Buffer, WR_SubBuffer, WR_Offset;
    register short int L1, L2;
    
    if (ndAudioDescriptor [NrChannel].IsActive)
    {
            // Calcola il numero di oversample necessario per ogni campione
            
            OverSamples = 48000.0/(float)(ndAudioDescriptor [NrChannel].VFreq);
            InvOverSamples = 1.0/OverSamples;
            
            // Esegui la riproduzione (modalità mono)
            
            if (ndAudioDescriptor [NrChannel].NrChannels==ND_AUDIO_MONO)
            {          
                    switch (ndAudioDescriptor [NrChannel].NrBits)
                    {            
                           case ND_AUDIO_16BIT:
                           {
                                RealNrSamples = OverSamples * NrSamples;        
                                        
                                for (Counter=0; Counter<RealNrSamples; Counter++)
                                {
                                    PositionSample = (float)(Counter)*(float)(InvOverSamples);
                                    
                                    // Ricava dal buffer il valore che serve
                                    
                                    L1 = *(short int *)(samples+(int)(PositionSample)*2);
                                    
                                    WR_Buffer    = ndAudioDescriptor [NrChannel].WR_Buffer;
                                    WR_SubBuffer = ndAudioDescriptor [NrChannel].WR_SubBuffer;
                                    WR_Offset    = ndAudioDescriptor [NrChannel].WR_Offset;
                                    
                                    ndAudioDescriptor [NrChannel].SoundData->Buffer[WR_Buffer][WR_SubBuffer][WR_Offset].l = L1*AmpFactor;
                                    ndAudioDescriptor [NrChannel].SoundData->Buffer[WR_Buffer][WR_SubBuffer][WR_Offset].r = L1*AmpFactor;
                                    
                                    ndAudioDescriptor [NrChannel].WR_Offset++;
                                    
                                    if (ndAudioDescriptor [NrChannel].WR_Offset==SAMPLES_FOR_SUBBUFFER)
                                    {
                                        INTERNAL_ChangeSubBufferInWriting (NrChannel);                                               
                                    }
                                }        
                                        
                                return NrSamples;           
                           }
                           
                           case ND_AUDIO_8BIT:
                           {
                                RealNrSamples = OverSamples * NrSamples;        
                                        
                                for (Counter=0; Counter<RealNrSamples; Counter++)
                                {
                                    PositionSample = (float)(Counter)*(float)(InvOverSamples);
                                    
                                    // Ricava dal buffer il valore che serve
                                    
                                    L1 = *(char *)(samples+(int)(PositionSample));
                                    
                                    WR_Buffer    = ndAudioDescriptor [NrChannel].WR_Buffer;
                                    WR_SubBuffer = ndAudioDescriptor [NrChannel].WR_SubBuffer;
                                    WR_Offset    = ndAudioDescriptor [NrChannel].WR_Offset;
                                    
                                    ndAudioDescriptor [NrChannel].SoundData->Buffer[WR_Buffer][WR_SubBuffer][WR_Offset].l = (L1*256)*AmpFactor;
                                    ndAudioDescriptor [NrChannel].SoundData->Buffer[WR_Buffer][WR_SubBuffer][WR_Offset].r = (L1*256)*AmpFactor;
                                    
                                    ndAudioDescriptor [NrChannel].WR_Offset++;
                                    
                                    if (ndAudioDescriptor [NrChannel].WR_Offset==SAMPLES_FOR_SUBBUFFER)
                                    {
                                        INTERNAL_ChangeSubBufferInWriting (NrChannel);                                               
                                    }
                                }        
                                        
                                return NrSamples;           
                           }
                    }
            }        
            
            // Esegui la riproduzione (modalità stereo)
            
            if (ndAudioDescriptor [NrChannel].NrChannels==ND_AUDIO_STEREO)
            {          
                    switch (ndAudioDescriptor [NrChannel].NrBits)
                    {            
                           case ND_AUDIO_16BIT:
                           {
                                RealNrSamples = OverSamples * NrSamples;
                                
                                for (Counter=0; Counter<RealNrSamples; Counter++)
                                {
                                    PositionSample = (float)(Counter)*(float)(InvOverSamples);
                                    
                                    // Ricava dal buffer il valore che serve
                                    
                                    L1 = *(short int *)(samples+(int)(PositionSample)*4);
                                    L2 = *(short int *)(samples+(int)(PositionSample)*4+2);
                                    
                                    WR_Buffer    = ndAudioDescriptor [NrChannel].WR_Buffer;
                                    WR_SubBuffer = ndAudioDescriptor [NrChannel].WR_SubBuffer;
                                    WR_Offset    = ndAudioDescriptor [NrChannel].WR_Offset;
                                    
                                    ndAudioDescriptor [NrChannel].SoundData->Buffer[WR_Buffer][WR_SubBuffer][WR_Offset].l = L2*AmpFactor;
                                    ndAudioDescriptor [NrChannel].SoundData->Buffer[WR_Buffer][WR_SubBuffer][WR_Offset].r = L1*AmpFactor;
                                    
                                    ndAudioDescriptor [NrChannel].WR_Offset++;
                                    
                                    if (ndAudioDescriptor [NrChannel].WR_Offset==SAMPLES_FOR_SUBBUFFER)
                                    {
                                        INTERNAL_ChangeSubBufferInWriting (NrChannel);                                               
                                    }
                                }        
                                        
                                return NrSamples;           
                           }
                           
                           case ND_AUDIO_8BIT:
                           {
                                RealNrSamples = OverSamples * NrSamples;
                                        
                                for (Counter=0; Counter<RealNrSamples; Counter++)
                                {
                                    PositionSample = (float)(Counter)*(float)(InvOverSamples);
                                    
                                    // Ricava dal buffer il valore che serve
                                    
                                    L1 = *(char *)(samples+(int)(PositionSample)*2);
                                    L2 = *(char *)(samples+(int)(PositionSample)*2+1);
                                    
                                    WR_Buffer    = ndAudioDescriptor [NrChannel].WR_Buffer;
                                    WR_SubBuffer = ndAudioDescriptor [NrChannel].WR_SubBuffer;
                                    WR_Offset    = ndAudioDescriptor [NrChannel].WR_Offset;
                                    
                                    ndAudioDescriptor [NrChannel].SoundData->Buffer[WR_Buffer][WR_SubBuffer][WR_Offset].l = (L2*256)*AmpFactor;
                                    ndAudioDescriptor [NrChannel].SoundData->Buffer[WR_Buffer][WR_SubBuffer][WR_Offset].r = (L1*256)*AmpFactor;
                                    
                                    ndAudioDescriptor [NrChannel].WR_Offset++;
                                    
                                    if (ndAudioDescriptor [NrChannel].WR_Offset==SAMPLES_FOR_SUBBUFFER)
                                    {
                                        INTERNAL_ChangeSubBufferInWriting (NrChannel);                                               
                                    }
                                }        
                                        
                                return NrSamples;           
                           }
                    }
            }        
            
            return 0;   // Se il formato è inconsistente, segnala errore
    }
    else 
       return ERR_HANDLE_IS_NOT_ACTIVE; 
}


int ndHAL_SND_CoercitiveStop (int NrChannel)
/*
     Questa funzione determina un brusco arresto della riproduzione del buffer
     che si trova in cache. E' previsto un solo codice di errore:
         
         ERR_HANDLE_IS_NOT_ACTIVE   L'handle audio non è attivo
*/
{
    if (ndAudioDescriptor [NrChannel].IsActive)
    {
         ndAudioDescriptor [NrChannel].CoercitiveStop=1; 
         
         // Mettiti in attesa fin quando il thread audio non ha terminato il suo lavoro
         sceKernelWaitEventFlag (ndAudioDescriptor [NrChannel].EventID, SND_STATE_IDLE, PSP_EVENT_WAITAND, 0, 0); 
      
         return 0;              
    }
    else 
       return ERR_HANDLE_IS_NOT_ACTIVE; 
}


int ndHAL_SND_SetVolume (int NrChannel, int VolumeSx, int VolumeDx)
/*
     Questa routine consente di impostare il volume del canale audio.
     
     I parametri VolumeDx e VolumeSx sono due parametri float il cui valore
     deve essere compreso tra 0 e 100. Se l'audio è mono, Nanodesktop
     considererà solo il parametro VolumeSx, mentre VolumeSx sarà
     ignorato.  
    
     Sono previsti i seguenti codici di errore:
        ERR_HANDLE_IS_NOT_ACTIVE   L'handle audio non è attivo
        ERR_WRONG_PARAMETERS       Il volume è errato  
*/
{
    if (ndAudioDescriptor [NrChannel].IsActive)
    {
         if ( (VolumeDx<0) || (VolumeDx>100) ) return ERR_WRONG_PARAMETERS;    
         if ( (VolumeSx<0) || (VolumeSx>100) ) return ERR_WRONG_PARAMETERS;    
         
         switch (ndAudioDescriptor [NrChannel].NrChannels)
         {
               case ND_AUDIO_MONO:
                    ndAudioDescriptor [NrChannel].HW_VolumeDX = (VolumeSx*PSP_AUDIO_VOLUME_MAX)/100;
                    ndAudioDescriptor [NrChannel].HW_VolumeSX = (VolumeSx*PSP_AUDIO_VOLUME_MAX)/100;
                    break;
               case ND_AUDIO_STEREO:
                    ndAudioDescriptor [NrChannel].HW_VolumeSX = (VolumeSx*PSP_AUDIO_VOLUME_MAX)/100;
                    ndAudioDescriptor [NrChannel].HW_VolumeDX = (VolumeDx*PSP_AUDIO_VOLUME_MAX)/100;
                    break;
               default:
                    return ERR_WRONG_PARAMETERS;                 
         }                
         
         // E' tutto a posto: segnalalo alla routine chiamante
         return 0;
    }
    else 
       return ERR_HANDLE_IS_NOT_ACTIVE; 
}



int ndHAL_SND_FlushChannel (int NrChannel)
{
    int ClosedBuffer;
    int WR_SubBuffer;
    int Counter;
    
    // Memorizza il codice del buffer che verrà chiuso
    ClosedBuffer = ndAudioDescriptor [NrChannel].WR_Buffer;
    WR_SubBuffer = ndAudioDescriptor [NrChannel].WR_SubBuffer;
    
    // Provvedi ad azzerare il resto del buffer
    for (Counter=ndAudioDescriptor [NrChannel].WR_Offset; Counter<SAMPLES_FOR_SUBBUFFER; Counter++)
    {
        ndAudioDescriptor [NrChannel].SoundData->Buffer [ClosedBuffer][WR_SubBuffer][Counter].l=0;
        ndAudioDescriptor [NrChannel].SoundData->Buffer [ClosedBuffer][WR_SubBuffer][Counter].r=0;
    }
    
    // Prepara il sistema a riprodurre gli ultimi campioni
    ndAudioDescriptor [NrChannel].SoundData->SubBuffersReady [ClosedBuffer]=WR_SubBuffer;
    ndAudioDescriptor [NrChannel].SoundData->State           [ClosedBuffer]=BUFFER_WRITTEN_AND_READY_FOR_PLAY;
    
    // Mettiti in attesa fin quando il thread audio non ha terminato il suo lavoro
    sceKernelWaitEventFlag (ndAudioDescriptor [NrChannel].EventID, SND_STATE_IDLE, PSP_EVENT_WAITAND, 0, 0); 
    
    // Ritardo necessario perchè il sistema possa stabilizzarsi
    sceKernelDelayThread (100000);
    
    return 0;
}

int ndHAL_SND_Beep (int NrChannel, int Tone)
/*
     Questa funzione emette un beep attraverso il canale NrChannel del 
     dispositivo utilizzato. Il valore Tone indica il tono (la frequenza
     è data dalla relazione f = 100 Hz * Tone).

     La funzione restituisce 0 se non ci sono errori ed un codice di errore
     in caso contrario. Sono previsti i seguenti codici di errore:
     
        ERR_ERROR_IN_OPEN_DEVICE      Errore nell'aprire il canale    
*/
{
    int      SndHandle, Counter;
    char     Buffer [1024];
    register float Factor;
    
    SndHandle = ndHAL_SND_OpenChannel (2, 8000, 8, 1, 100, 100, 0);
    
    if (SndHandle>=0)
    {
        // Riempi il buffer: la formula da applicare sarebbe stata la seguente
        // 6.28*100.0*Tone*Counter/8192.0. Per velocizzare, precalcoliamo il
        // prodotto delle costanti
        
        Factor = 6.28*100.0/8192.0;
        
        for (Counter=0; Counter<1024; Counter++)
        {
            Buffer [Counter]=128.0*ndHAL_XFPU_sinf(Factor*Tone*Counter);
        }
        
        // Esegui la riproduzione e chiudi il canale
        
        ndHAL_SND_WriteToChannel (SndHandle, &Buffer, 1024, 1.0);
        ndHAL_SND_CloseChannel (SndHandle);     
        
        return 0;        
    }
    else        // C'è un problema ad aprire il canale
      return ERR_ERROR_IN_OPEN_DEVICE; 
}



int ndHAL_SND_CloseChannel (int NrChannel)
{
    int ClosedBuffer;
    int WR_SubBuffer;
    int Counter;
    
    // Memorizza il codice del buffer che verrà chiuso
    ClosedBuffer = ndAudioDescriptor [NrChannel].WR_Buffer;
    WR_SubBuffer = ndAudioDescriptor [NrChannel].WR_SubBuffer;
    
    // Provvedi ad azzerare il resto del buffer
    for (Counter=ndAudioDescriptor [NrChannel].WR_Offset; Counter<SAMPLES_FOR_SUBBUFFER; Counter++)
    {
        ndAudioDescriptor [NrChannel].SoundData->Buffer [ClosedBuffer][WR_SubBuffer][Counter].l=0;
        ndAudioDescriptor [NrChannel].SoundData->Buffer [ClosedBuffer][WR_SubBuffer][Counter].r=0;
    }
    
     // Prepara il sistema a riprodurre gli ultimi campioni
    ndAudioDescriptor [NrChannel].SoundData->SubBuffersReady [ClosedBuffer]=WR_SubBuffer;
    ndAudioDescriptor [NrChannel].SoundData->State           [ClosedBuffer]=BUFFER_WRITTEN_AND_READY_FOR_PLAY;
    
    // Mettiti in attesa fin quando il thread audio non ha terminato il suo lavoro
    sceKernelWaitEventFlag (ndAudioDescriptor [NrChannel].EventID, SND_STATE_IDLE, PSP_EVENT_WAITAND, 0, 0); 
    
    // Ritardo necessario perchè il sistema possa stabilizzarsi
    sceKernelDelayThread (100000);
        
    // Portare a zero il volume del canale prima di rilasciarlo riduce i disturbi
    sceAudioChangeChannelVolume (ndAudioDescriptor [NrChannel].ChannelHdr, 0, 0);
    sceAudioChRelease (ndAudioDescriptor [NrChannel].ChannelHdr);

    // Libera la memoria che era stata destinata al buffer audio
    free (ndAudioDescriptor [NrChannel].SoundData);
    
    // Cancella il thread che si occupa dell'audio su questo canale
    Audio_YouCanExit [NrChannel]=1;
    sceKernelTerminateDeleteThread (ndAudioDescriptor [NrChannel].ThreadID);
    
    // Cancella il field eventi
    sceKernelDeleteEventFlag (ndAudioDescriptor [NrChannel].EventID);
    
    // Libera il descrittore
    ndAudioDescriptor [NrChannel].IsActive=0;
    return 0;
}


