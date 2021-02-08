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
    GESTORE AUDIO (PSPE)
    ----------------------------------------------------------------------------
    L'emulatore PSPE non gestisce audio. Tuttavia, il comportamento dell'API
    viene comunque emulato in modo da consentire il test delle applicazioni
    ----------------------------------------------------------------------------
*/          

#include <HAL_PSPE\$_HAL_NDforND.h>


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




/*
   ----------------------------------------------------------------------------
   THREAD PER LA GESTIONE DELL'AUDIO
   ----------------------------------------------------------------------------
*/




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
        
        ChannelHdr=0;      // Emulato
        
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
                  
                  // (...)
                  
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
    
    if (ndAudioDescriptor [NrChannel].IsActive)
    {
            // Simula il processo di scrittura nel canale
            
            return NrSamples;
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
         // Simula operazione di arresto
         
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
    
    // Memorizza il codice del buffer che verrà chiuso
    ClosedBuffer = ndAudioDescriptor [NrChannel].WR_Buffer;
    WR_SubBuffer = ndAudioDescriptor [NrChannel].WR_SubBuffer;
    
    // Prepara il sistema a riprodurre gli ultimi campioni
    ndAudioDescriptor [NrChannel].SoundData->SubBuffersReady [ClosedBuffer]=WR_SubBuffer;
    ndAudioDescriptor [NrChannel].SoundData->State           [ClosedBuffer]=BUFFER_WRITTEN_AND_READY_FOR_PLAY;
    
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
    
    // Memorizza il codice del buffer che verrà chiuso
    ClosedBuffer = ndAudioDescriptor [NrChannel].WR_Buffer;
    WR_SubBuffer = ndAudioDescriptor [NrChannel].WR_SubBuffer;
    
    // Prepara il sistema a riprodurre gli ultimi campioni
    ndAudioDescriptor [NrChannel].SoundData->SubBuffersReady [ClosedBuffer]=WR_SubBuffer;
    ndAudioDescriptor [NrChannel].SoundData->State           [ClosedBuffer]=BUFFER_WRITTEN_AND_READY_FOR_PLAY;
        
    // Libera la memoria che era stata destinata al buffer audio
    free (ndAudioDescriptor [NrChannel].SoundData);
    
    // Cancella il thread che si occupa dell'audio su questo canale
    Audio_YouCanExit [NrChannel]=1;
    
    // Libera il descrittore
    ndAudioDescriptor [NrChannel].IsActive=0;
    return 0;
}


