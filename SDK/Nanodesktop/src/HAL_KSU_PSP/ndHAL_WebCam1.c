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
    -----------------------------------------------------------------------------
    DRIVER PER LA GESTIONE DELLA GOCAM/CHOTTO CAM (PSP KSU mode)
    -----------------------------------------------------------------------------
    
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>




/*
   -----------------------------------------------------------------------------
   WEBCAM API per la GoCam
   -----------------------------------------------------------------------------
*/

char INTERNAL_GOCAM_ActivateCamera (void)
/*
     Routine specifica per l'Eyeserver. Provvede ad attivarlo usando la sezione
     specifica di inizializzazione USB
*/
{
     static struct usb_device devCamera;
     char UsbHandle;
     
     devCamera.descriptor.idVendor  = GOCAM_VENDOR_ID;
     devCamera.descriptor.idProduct = GOCAM_PRODUCT_ID;
            
     UsbHandle = ndHAL_USB_OpenDevice (&devCamera);   // Tenta la connessione alla GoCam
     if (UsbHandle>=0) return UsbHandle;              // Se è riuscito ritorna l'USB handle       
}



char INTERNAL_GOCAM_GrabNewImage (int UsbHandleCamera, char Target, int AuxValue, ndint64 Options, char RenderNow)
/*
    Questa routine provvede a chiedere alla GoCam l'invio di un'immagine.
    Questa routine è alla base di tutto il sistema di gestione webcam di
    Nanodesktop. 
    
    Il parametro UsbHandleCamera indica l'handle USB della videocamera
    da utilizzare (l'handle USB è quello assegnato da ndHAL_CAM_ActivateCamera
    che deve essere richiamata prima di chiamare questa routine).

    Il significato dei parametri di questa routine, che è esclusivamente per
    uso interno, sono indicati nella documentazione della routine pubblica
    ndHAL_CAM_GrabNewImage nel file ndHAL_WebCam.c
     
    La routine restituisce i seguenti codici di errore:
     
       ERR_WRONG_PARAMETERS             Parametri errati (ad esempio, il parametro Target non
                                        è riconosciuto)           
       ERR_IO_ERROR                     L'handle della webcam è errato, o si riferisce ad un
                                        dispositivo USB che non è una webcam
       
   
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;    
} 
    


char INTERNAL_GOCAM_ChangeResolution (int UsbHandleCam, int NewSizeX, int NewSizeY)
/*
   Questa routine informa il sistema che è necessario cambiare la risoluzione
   con cui le immagini vengono inviate al client. 
   
   NewSizeX e NewSizeY rappresentano le nuove dimensioni. In uscita viene 
   normalmente restituito 0, se viene restituito un valore negativo siamo
   in presenza di un errore. Sono previsti i seguenti codici di errore:
   
           
      ERR_WRONG_PARAMETERS             Parametri NewSizeX o NewSizeY non corretti  
      
      ERR_IO_ERROR                     Errore di I/O          
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;  
}
 
 
char INTERNAL_GOCAM_ChangeTransferMode (int UsbHandleCam, int NewMode)
/*
    Questa routine permette di cambiare il transfer mode utilizzato
    dalla GoCam. 
    
    Il parametro Mode può assumere esclusivamente 3 valori: 8, 16, 24.
    
    I valori 24 e 16 consentono di avere dalla GoCam immagini a 
    colori, mentre il parametro 8 consente di avere immagini in
    toni di grigio
    
    L'utente può usare le costanti manifeste ND_RGB_TRASMISSION_MODE,
    ND_RGB555_TRASMISSION_MODE e ND_GRAY_TRASMISSION_MODE in luogo
    delle semplici costanti numeriche. 

    Sono previsti i seguenti codici di errore:
         
      ERR_WRONG_PARAMETERS             Parametri NewMode non corretto 
      
      ERR_IO_ERROR                     Errore di I/O      
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;    
}


char INTERNAL_GOCAM_GetInfo (int UsbHandleCam, int *DimImgX, int *DimImgY, int *FrameCounter, int *TransferMode)
/*
    Questa routine si occupa di prelevare le informazioni sulle immagini attualmente
    visualizzate dalla GoCam. 
    
    Sono previsti i seguenti codici di errore:
         
      ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB passato non è attivo
      
      ERR_DEVICE_NOT_SUPPORTED         L'handle USB utilizzato non si riferisce ad una webcam
           
      ERR_IO_ERROR                     Errore di I/O      
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;   
}

char INTERNAL_GOCAM_ChangeSensorMode (int UsbHandleCamera, int NewMode)
/*
     Questa routine provvede a cambiare la modalità di video del
     sensore della videocamera.
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}

char INTERNAL_GOCAM_ChangeHwResolution (int UsbHandleCamera, int NewMode)
/*
     Questa routine provvede a cambiare la risoluzione del
     sensore della videocamera. 
     
     La routine non deve essere confusa con INTERNAL_GOCAM_ChangeResolution.
     
     Infatti, la routine INTERNAL_GOCAM_ChangeResolution accetta in
     input qualsiasi risoluzione: Nanodesktop provvederà ad uno scaling
     automatico per ottenere la risoluzione richiesta a partire dalla
     risoluzione del sensore. 
     
     Al contrario, INTERNAL_GOCAM_ChangeHwResolution accetta in ingresso
     solo un numero finito di codici di risoluzione, e setta direttamente
     la risoluzione del sensore
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}


char INTERNAL_GOCAM_ChangeHwBrightness (int UsbHandleCamera, int NewBrightness)
/*
    Consente di settare la luminosità della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}

char INTERNAL_GOCAM_ChangeHwContrast (int UsbHandleCamera, int NewContrast)
/*
    Consente di settare il contrasto della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}

char INTERNAL_GOCAM_ChangeHwSharpness (int UsbHandleCamera, int NewSharpness)
/*
    Consente di settare la nitidezza della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}

char INTERNAL_GOCAM_ChangeHwEvLevel (int UsbHandleCamera, int NewEvLevel)
/*
    Consente di settare il livello di esposizione (da -128 a +128) 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}


int INTERNAL_GOCAM_GetSaturationLevel (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}


int INTERNAL_GOCAM_GetBrightness (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}


int INTERNAL_GOCAM_GetContrast (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}



int INTERNAL_GOCAM_GetSharpness (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}



int INTERNAL_GOCAM_GetHwSensorMode (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}


int INTERNAL_GOCAM_GetEvLevel (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}


char INTERNAL_GOCAM_ActivateCamMicrophone (int WorkAreaAudioSize, int Frequency, int Gain)
/*
    Questa routine è esclusivamente per uso interno. Essa provvede ad attivare il 
    microfono associato alla webcam. 
    
    Il primo parametro è la dimensione dell'area di lavoro associata al driver
    audio. Tale area di lavoro verrà allocata sul momento a richiesta
    
    Il secondo ed il terzo parametro sono la frequenza di lavoro ed il guadagno del
    microfono. 
    
    Viene restituito il valore 0 se non vi sono errori ed un valore negativo in 
    caso di errore. 
    
    Vengono restituiti i seguenti codici di errore:
        ERR_FUNCTION_NOT_SUPPORTED       Il microfono non è supportato dalla webcam
        ERR_DEVICE_IS_NOT_OPERATIVE      Problema nell'inizializzazione del microfone
        ERR_IO_ERROR                     Errore interno   
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}
    
    
int INTERNAL_GOCAM_ReadAudioBlock (void *Buffer, int NrBytesToRead)
/*
    Questa routine legge un blocco di dati audio dal microfono
    e lo copia nel buffer *Buffer. 
    
    L'utente deve avere allocato in precedenza una area di memoria 
    opportuna, e ne deve passarne l'indirizzo alla routine 

    La routine restituisce in uscita il numero di bytes letti 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}    
    
void INTERNAL_GOCAM_DeActivateCamMicrophone (void)
/*
    Provvede a disattivare il microfono associato alla webcam    
*/  
{
    /* Nothing to do */
}


void INTERNAL_GOCAM_DisableCamera (int UsbHandleCamera)
/*
    Usa la funzione del sottosistema Usb associata per liberare l'handle
    e per scaricare i drivers associati
*/
{
    /* Nothing to do */
}

/*
   ----------------------------------------------------------------------------
   ROUTINES DI REGISTRAZIONE VOCALE
   ----------------------------------------------------------------------------
*/


char INTERNAL_GOCAM_StartAudioRecording (char *NameFile, int Frequency, int Gain)
/*
     Questa routine è esclusivamente per uso interno.
     
     Questa routine inizia a registrare il flusso audio proveniente dalla webcam su un
     file di nome NameFile in formato WAV. 
     
     Viene restituito il valore 0 se il processo di avvio del thread audio
     è andato a buon fine, altrimenti viene restituito un codice di errore. 
     
     E' possibile avviare un solo thread di registrazione alla volta. 
     Quando l'utente vuole arrestare il processo di registrazione, deve
     usare la routine ndHAL_CAM_StopAudioRecording che chiude il file
     WAV e dealloca il thread di registrazione dopo averlo terminato.
     
     Sono previsti i seguenti codici di errore:
          ERR_DEVICE_IS_NOT_OPERATIVE   Problemi per l'inizializzazione del
                                        microfono
          ERR_SYSTEM_IS_BUSY            C'è già un thread di registrazione attivo
          ERR_FILE_NOT_FOUND            Non riesco ad aprire questo file per la
                                        registrazione
          ERR_MEMORY_OVERFLOW           Overflow della memoria
          ERR_INTERNAL_ERROR            Problemi ad avviare il thread di registrazione
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}


void INTERNAL_GOCAM_StopAudioRecording (void)
/*
    Arresta il processo di registrazione
*/
{
    /* Nothing to do */
}



















