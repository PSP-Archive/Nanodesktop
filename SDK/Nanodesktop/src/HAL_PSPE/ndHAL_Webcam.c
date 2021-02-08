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
    DRIVER PER LA GESTIONE DELLA WEBCAM (PSPE)
    -----------------------------------------------------------------------------
    
*/

#include <HAL_PSPE\$_HAL_NDforND.h>


/*
   -----------------------------------------------------------------------------
   WEBCAM API
   -----------------------------------------------------------------------------
*/

    
int ndHAL_CAM_ActivateCamera (int NrCamera)
/*
    Questa routine provvede ad attivare il driver per la webcam. 
    
    Il parametro NrCamera è ignorato sotto PSP. 
    
    La routine restituisce l'handle assegnato alla camera USB se 
    non vi sono errori, mentre restituisce un valore negativo 
    qualora non sia stato possibile attivare la videocamera
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;
}



char ndHAL_CAM_GrabNewImage (int UsbHandleCamera, char Target, int AuxValue, ndint64 Options, char RenderNow)
/*
    -------------------------------------------------------------------------------
    
    Questa routine provvede a chiedere all'EyeServer l'invio di un'immagine.
    Questa routine è alla base di tutto il sistema di gestione webcam di
    Nanodesktop. 
    
    Il parametro UsbHandleCamera indica l'handle USB della videocamera
    da utilizzare (l'handle USB è quello assegnato da ndHAL_CAM_ActivateCamera
    che deve essere richiamata prima di chiamare questa routine).

    Nanodesktop ha bisogno di sapere che cosa fare con l'immagine una volta che
    essa è stata acquisita. Questo può essere settato per mezzo del parametro
    Target che può assumere esclusivamente i seguenti valori:
           
    ND_CAM_TO_NULL             Usato solo per scopi interni: viene richiesta una
                               immagine, ma non viene sottoposta ad alcuna
                               elaborazione
    
    ND_CAM_TO_WINDOW           L'immagine ottenuta dalla webcam deve essere inviata
                               ad una finestra di Nanodesktop che è aperta
                            
    ND_CAM_TO_BASESCREEN       L'immagine ottenuta deve essere inviata al 
                               BaseScreen di Nanodesktop
                            
    ND_CAM_TO_NDIMAGE_STRUCT   L'immagine ottenuta deve essere inviata ad una
                               struct di tipo ndImage_Type che è stata 
                               precedentemente allocata
                               
    ND_CAM_TO_CVIMAGE          L'immagine ottenuta dalla webcam deve essere inviata
                               ad una immagine IplImage           
    -------------------------------------------------------------------------------
    
    I parametri AuxValue e Options hanno un significato diverso a seconda
    del Target che è stato richiesto

    - ND_CAM_TO_NULL
    
          In questo caso i parametri AuxValue e Options sono ignorati.
    
    - ND_CAM_TO_WINDOW
    
          Se si è scelta l'opzione ND_CAM_TO_WINDOW, l'int AuxValue deve contenere
          l'handle della finestra dove dovrà essere memorizzata l'immagine. 
    
          Il parametro Options è un parametro a 64 bit che permette di passare le
          informazioni sulla posizione che l'immagine dovrà avere all'interno
          della finestra. L'utente può settare tali posizioni per mezzo delle keys
          NDKEY_POSX e NDKEY_POSY. 
    
    - ND_CAM_TO_BASESCREEN
    
          Analogo alla precedente, ma stavolta l'immagine viene inviata al BaseScreen
          di Nanodesktop. E' anche qui possibile passare le informazioni di posizione
          attraverso le keys NDKEY_POSX e NDKEY_POSY
    
    - ND_CAM_TO_NDIMAGE_STRUCT
    
          In questo caso l'immagine grabbata viene inviata ad una struct ndImage_Type.
          L'utente deve avere già allocato tale immagine per mezzo di ndIMG_CreateImage.
    
          Il parametro AuxValue è l'indirizzo della struct ndImage_Type che descrive
          l'immagine Nanodesktop. Nanodesktop provvederà a convertire automaticamente
          l'immagine grabbata dal suo formato originale ad un formato compatibile
          con quello della struttura ndImage_Type (NDMGKNB o NDRGB). 
    
          Il sistema in questo caso ignorerà eventuali opzioni di posizione che vengano
          passate attraverso il codice a 64 bit Options, compresa l'eventuale opzione
          ND_FORCECAM che sarà sempre attivata
          
    - ND_CAM_TO_CVIMAGE 
    
          In questo caso l'immagine grabbata verrà inviata ad una struct di tipo 
          IPLImage, il formato usato dalle OpenCV di Intel. 
          
          Il parametro AuxValue è l'indirizzo dell'immagine IplImage che deve
          obbligatoriamente essere un'immagine con profondità di colore 8 bit,
          3 canali, nessuna ROI o COI settata. 
          
          Il sistema ignora eventuali opzioni di posizione che vengano passate 
          attraverso il parametro a 64 bit Options, ma non ignora l'opzione
          ND_FORCECAM              
     -------------------------------------------------------------------------------------
     
     IL PARAMETRO A 64 BIT OPTIONS
     
     Il parametro a 64 bit Options ha i bit mappati nel seguente modo:
                  
     23           Settato ad 1 se si vuole che la videocamera usi l'opzione FORCECAM. 
                  L'utente può ottenere un valore che abbia questo bit settato per mezzo
                  della costante manifesta ND_FORCECAM. Usando questa opzione, 
                  si impedirà che l'EyeClient per PSP scarti le aree dell'immagine che
                  non hanno subito cambiamenti rispetto ai frame precedenti. Questo
                  aumenta la nitidezza dell'immagine acquisita ed elimina la possibilità
                  che vi siano artefatti, ma peggiora la velocità di acquisizione.
                  
                  L'opzione non ha alcun significato per quelle piattaforme che non usano
                  le architetture EyeServer/EyeClient, ma che possono controllare
                  direttamente la webcam attraverso l'interfaccia USB di tipo host           
     
     20-11        Posizione PosX 
     10-0         Posizione PosY
     
    --------------------------------------------------------------------------------------
    
    IL PARAMETRO AD 8 BIT RenderNow 
     
    Questo parametro deve essere posto ad 1 qualora si voglia che la routine esegua 
    un WindowsRender o BaseScreenRender dopo l'acquisizione di ogni frame (scelta
    consigliata). 
     
    Il parametro viene ignorato quando il target della webcam è ND_CAM_TO_NDIMAGE_STRUCT.
    
    --------------------------------------------------------------------------------------- 
    
    CODICI DI ERRORE

    La routine restituisce i seguenti codici di errore:
               
       ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB passato non è attivo        
    
       ERR_DEVICE_NOT_SUPPORTED         L'handle USB utilizzato non si riferisce ad una webcam
    
       ERR_WRONG_PARAMETERS             Parametri errati (ad esempio, il parametro Target non
                                        è riconosciuto) 
               
       ERR_IO_ERROR                     L'handle della webcam è errato, o si riferisce ad un
                                        dispositivo USB che non è una webcam
                       
       ERR_LOGICAL_ERROR_IN_EYESERVER   Non è stato possibile capire il significato
                                        delle informazioni ottenute dell'Eyeserver
                                       
       ERR_DATA_INTERRUPTED             Il flusso dei dati è stato interrotto
       
   ---------------------------------------------------------------------------------------------    
*/
{
   return ERR_FUNCTION_HAS_BEEN_DISABLED; 
} 
    


char ndHAL_CAM_ChangeResolution (int UsbHandleCam, int NewSizeX, int NewSizeY)
/*
   Questa routine informa l'Eyeserver che è necessario cambiare la risoluzione
   con cui le immagini vengono inviate al client. 
   
   NewSizeX e NewSizeY rappresentano le nuove dimensioni. In uscita viene 
   normalmente restituito 0, se viene restituito un valore negativo siamo
   in presenza di un errore. Sono previsti i seguenti codici di errore:
   
      ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB passato non è attivo
      
      ERR_DEVICE_NOT_SUPPORTED         L'handle USB utilizzato non si riferisce ad una webcam
           
      ERR_WRONG_PARAMETERS             Parametri NewSizeX o NewSizeY non corretti  
      
      ERR_IO_ERROR                     Errore di I/O          
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}
 
 
char ndHAL_CAM_ChangeTransferMode (int UsbHandleCam, int NewMode)
/*
    Questa routine permette di cambiare il transfer mode utilizzato
    dall'EyeServer. 
    
    Il parametro Mode può assumere esclusivamente 3 valori: 8, 16, 24.
    
    Il valore 8 permette di ottenere dall'Eyeserver immagini in toni di
    grigio a 256 livelli, il valore 16 consente di ottenere immagini a
    colori in formato RGB555 (il formato nativo della PSP), mentre il
    valore 24 consente di ottenere immagini RGB standard
    
    L'utente può usare le costanti manifeste ND_RGB_TRASMISSION_MODE,
    ND_RGB555_TRASMISSION_MODE e ND_GRAY_TRASMISSION_MODE in luogo
    delle semplici costanti numeriche. 

    Sono previsti i seguenti codici di errore:
    
      ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB passato non è attivo
      
      ERR_DEVICE_NOT_SUPPORTED         L'handle USB utilizzato non si riferisce ad una webcam
           
      ERR_WRONG_PARAMETERS             Parametri NewMode non corretto 
      
      ERR_IO_ERROR                     Errore di I/O      
*/
{
   return ERR_FUNCTION_HAS_BEEN_DISABLED;       
}


char ndHAL_CAM_GetInfo (int UsbHandleCam, int *DimImgX, int *DimImgY, int *FrameCounter, int *TransferMode)
/*
    Questa routine si occupa di prelevare le informazioni sulle immagini attualmente
    visualizzate dall'EyeServer. Attenzione perchè le infomazioni di dimensione
    hanno un significato solo dopo che l'utente ha impostato la dimensione
    dell'immagine o dopo che l'Eyeserver lo ha fatto automaticamente attraverso
    una chiamata a ndHAL_CAM_GrabNewImage.
    
    Se si richiama ndHAL_CAM_GetInfo prima di avere impostato una dimensione
    immagine, il sistema restituirà le dimensioni entrambe a -1
    
    Sono previsti i seguenti codici di errore:
         
      ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB passato non è attivo
      
      ERR_DEVICE_NOT_SUPPORTED         L'handle USB utilizzato non si riferisce ad una webcam
           
      ERR_IO_ERROR                     Errore di I/O      
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;         
}

char ndHAL_CAM_ChangeSensorMode (int UsbHandleCamera, int NewMode)
/*
     Sulle piattaforme supportate, questa funzione provvede a cambiare la modalità
     di funzionamento del sensore CCD della videocamera. Il parametro NewMode
     è un codice a 32 bit che indica la nuova modalità da adottare. I codici a
     32 bit possono cambiare da piattaforma a piattaforma: per la GoCam i codici
     supportati sono associati ai seguenti simboli:
                
     ND_USBCAM_EFFECTMODE_NORMAL      0
     ND_USBCAM_EFFECTMODE_NEGATIVE    1
     ND_USBCAM_EFFECTMODE_BLACKWHITE  2
     ND_USBCAM_EFFECTMODE_SEPIA       3
     ND_USBCAM_EFFECTMODE_BLUE        4
     ND_USBCAM_EFFECTMODE_RED         5
     ND_USBCAM_EFFECTMODE_GREEN       6
                                    
     La routine restituisce 0 se non ci sono errori ed un valore negativo in
     caso di errore. Sono previsti i seguenti codici di errore:
     
     ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB passato non è attivo
      
     ERR_DEVICE_NOT_SUPPORTED         L'handle USB utilizzato non si riferisce ad una webcam
           
     ERR_WRONG_PARAMETERS             Parametri NewMode non corretto 
           
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;          
}

char ndHAL_CAM_ChangeHwResolution (int UsbHandleCamera, int NewMode)
/*
     Questa routine provvede a cambiare la risoluzione del
     sensore della videocamera. 
     
     La routine non deve essere confusa con ndHAL_CAM_ChangeResolution.
     
     Infatti, la routine ndHAL_CAM_ChangeResolution accetta in
     input qualsiasi risoluzione: Nanodesktop provvederà ad uno scaling
     automatico per ottenere la risoluzione richiesta a partire dalla
     risoluzione del sensore. 
     
     Al contrario, ndHAL_CAM_ChangeHwResolution accetta in ingresso
     solo un numero finito di codici di risoluzione, e setta direttamente
     la risoluzione del sensore
     
     I codici di risoluzione riconosciuti sono i seguenti
       ND_RESOLUTION_CODE_176_144
       ND_RESOLUTION_CODE_320_240
       ND_RESOLUTION_CODE_352_288
       ND_RESOLUTION_CODE_640_480
       ND_RESOLUTION_CODE_480_272
       
     La funzione restituisce 0 se non ci sono errori.
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}


char ndHAL_CAM_ChangeHwBrightness (int UsbHandleCamera, int NewBrightness)
/*
    Consente di settare la luminosità della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}

char ndHAL_CAM_ChangeHwContrast (int UsbHandleCamera, int NewContrast)
/*
    Consente di settare il contrasto della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}

char ndHAL_CAM_ChangeHwSharpness (int UsbHandleCamera, int NewSharpness)
/*
    Consente di settare la nitidezza della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;    
}

char ndHAL_CAM_ChangeHwEvLevel (int UsbHandleCamera, int NewEvLevel)
/*
    Consente di settare il livello di esposizione (da -128 a +128) 
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}

int ndHAL_CAM_GetSaturationLevel (int UsbHandleCamera)
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}

int ndHAL_CAM_GetBrightness (int UsbHandleCamera)
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}

int ndHAL_CAM_GetContrast (int UsbHandleCamera)
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}

int ndHAL_CAM_GetSharpness (int UsbHandleCamera)
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}

int ndHAL_CAM_GetHwSensorMode (int UsbHandleCamera)
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;   
}


int ndHAL_CAM_GetEvLevel (int UsbHandleCamera)
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;     
}

char ndHAL_CAM_ActivateCamMicrophone (int UsbHandleCamera, int WorkAreaAudioSize, int Frequency, int Gain)
/*
    Questa provvede ad attivare il microfono associato alla webcam. 
    
    Il primo parametro è l'handle della webcam
    
    Il secondo parametro è la dimensione dell'area di lavoro associata al driver
    audio. Tale area di lavoro verrà allocata sul momento a richiesta
    
    Il terzo ed il quarto parametro sono la frequenza di lavoro ed il guadagno del
    microfono. 
    
    Viene restituito il valore 0 se non vi sono errori ed un valore negativo in 
    caso di errore. 
    
    Vengono restituiti i seguenti codici di errore:
        ERR_FUNCTION_NOT_SUPPORTED       Il microfono non è supportato dalla webcam
        ERR_DEVICE_IS_NOT_OPERATIVE      Problema nell'inizializzazione del microfone
        ERR_IO_ERROR                     Errore interno 
        ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB non è operativo
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;     
}
    
    
int ndHAL_CAM_ReadAudioBlock (int UsbHandleCamera, void *Buffer, int NrBytesToRead)
/*
    Questa routine legge un blocco di dati audio dal microfono
    e lo copia nel buffer *Buffer. 
    
    L'utente deve avere allocato in precedenza una area di memoria 
    opportuna, e ne deve passarne l'indirizzo alla routine 

    La routine restituisce in uscita il numero di bytes letti 
    
    Se viene riportato in uscita un valore negativo, vuol dire
    che c'è stato un errore. Sono previsti i seguenti codici
    di errore:
       
       ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB non è operativo          
       ERR_DEVICE_NOT_SUPPORTED         Dispositivo non supportato
    
*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;     
}    
    
void ndHAL_CAM_DeActivateCamMicrophone (int UsbHandleCamera)
/*
    Provvede a disattivare il microfono associato alla webcam    
*/  
{
}


void ndHAL_CAM_DisableCamera (int UsbHandleCam)
/*
   Questa funzione si occupa semplicemente di rilasciare l'handle
   USB per la webcam che era stato impegnato da ActivateCamera
*/
{ 
}


/*
   ----------------------------------------------------------------------------
   ROUTINES DI REGISTRAZIONE VOCALE (Api ad alto livello)
   ----------------------------------------------------------------------------
*/



char ndHAL_CAM_StartAudioRecording (int UsbHandleCamera, char *NameFile, int Frequency, int Gain)
/*
     Questa routine inizia a registrare il flusso audio proveniente 
     dalla webcam identificata da UsbHandleCamera su un file di nome 
     NameFile in formato WAV. 
     
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
    return ERR_FUNCTION_HAS_BEEN_DISABLED;      
}


void ndHAL_CAM_StopAudioRecording (int UsbHandleCamera)
/*
    Arresta il processo di registrazione avviato sulla webcam 
    identificata da UsbHandleCamera
*/
{
}






