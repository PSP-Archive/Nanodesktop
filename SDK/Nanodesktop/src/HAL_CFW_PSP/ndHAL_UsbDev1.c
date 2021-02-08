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
    ROUTINES DI GESTIONE USB SPECIFICHE PER PERIFERICA GOCAM  (PSP CFW mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>


/*
   -----------------------------------------------------------------------------
   Routines di supporto per la GoCam
   -----------------------------------------------------------------------------
*/

static void INTERNAL_GOCAM_SuccessConnectionNotify (void)
{
     
}




/*
   -----------------------------------------------------------------------------
   Gestione dei drivers USB per la GoCam
   -----------------------------------------------------------------------------
*/


int INTERNAL_GOCAM_LoadDeviceDrivers ()
/*
     Questa routine provvede a caricare i drivers necessari per la gestione
     della GoCam in memoria. Restituisce 0 se tutto ok, ed un codice
     negativo in caso di errore. 
     
     Nel caso di errore, la routine deposita dentro la variabile di ambiente
     ndGoCamFailure_MainErrCode il codice di errore principale e dentro
     ndGoCamFailure_SecErrCode il codice di errore secondario
*/
{
    int result;
    
    ndGoCamLoadDepth = 0;  
     
    result = sceUsbStart(PSP_USBACC_DRIVERNAME, 0, 0);
	if (result < 0)
	{
	   ndGoCamFailureState        = ND_FAILURE_TO_INIT_USBACC;
	   ndGoCamSecondaryErrorCode  = result;
       ndGoCamIsFailure           = 1;
       return -1;		
	}
	
	ndGoCamLoadDepth = 1;
	
	result = sceUsbStart(PSP_USBCAM_DRIVERNAME, 0, 0);
	if (result < 0)
	{
	   ndGoCamFailureState        = ND_FAILURE_TO_INIT_USBCAMDRV;
	   ndGoCamSecondaryErrorCode  = result;
       ndGoCamIsFailure           = 1;
       return -1;		
	}

    ndGoCamLoadDepth = 2;

	result = sceUsbStart(PSP_USBCAMMIC_DRIVERNAME, 0, 0);
	if (result < 0)
	{
	   ndGoCamFailureState        = ND_FAILURE_TO_INIT_USBCAMMICDRV;
	   ndGoCamSecondaryErrorCode  = result;
       ndGoCamIsFailure           = 1; 
       return -1;			
	} 
	
	ndGoCamLoadDepth = 3;
	
	result = sceUsbActivate(PSP_USBCAM_PID);
    if (result < 0)
    {
       ndGoCamFailureState        = ND_FAILURE_TO_ACTIVATE_NEW_CAMERA;
	   ndGoCamSecondaryErrorCode  = result;
       ndGoCamIsFailure           = 1;
       return -1;
    }
    
    ndGoCamLoadDepth = 4;
    
    // Se il sistema arriva a questo punto, vuol dire che i drivers
    // sono tutti caricati ed in memoria
    
    ndGoCamIsFailure           = 0;
    ndGoCamFailureState        = 0;
    ndGoCamSecondaryErrorCode  = 0;
    
    return 0;
}

void INTERNAL_GOCAM_UnLoadDeviceDrivers (void)
{
    if (ndGoCamLoadDepth>=4)  sceUsbDeactivate (PSP_USBCAM_PID);
    if (ndGoCamLoadDepth>=3)  sceUsbStop (PSP_USBCAMMIC_DRIVERNAME, 0, 0); 
    if (ndGoCamLoadDepth>=2)  sceUsbStop (PSP_USBCAM_DRIVERNAME, 0, 0); 
    if (ndGoCamLoadDepth>=1)  sceUsbStop (PSP_USBACC_DRIVERNAME, 0, 0); 
    
    ndGoCamLoadDepth=0;
}


/*
   -----------------------------------------------------------------------------
   Configurazione del dispositivo
   -----------------------------------------------------------------------------
*/

int INTERNAL_GOCAM_TryToActivateCam (void)
/*
    Questa routine tenta di attivare la webcam. Restituisce 0 se non ci sono
    errori e -1 in caso di errore
*/
{
    int Counter, ErrRep;
    int ReserveFlags;
    
    for (Counter=0; Counter<100; Counter++)
    {
        if ((sceUsbGetState() & 0xF) == PSP_USB_CONNECTION_ESTABLISHED)
        {
            ErrRep=sceUsbCamGetReverseMode (&ReserveFlags);
            /* 
               In questo punto si adotta un trucco. Il sistema sembra non essere
               in grado di distinguere tra il caso in cui viene collegata una
               GoCam al bus USB, ed il caso in cui viene collegato un cavo USB
               
               Per risolvere, viene chiesta una informazioni alla webcam. Se
               viene tornato un codice di errore 0, allora vuol dire che è
               davvero collegata una webcam e non l'Eyeserver
            */
            
            if (!ErrRep) return 0; 
               else return -1;         // E' collegato il cavo USB         
        }
 
		sceKernelDelayThread(50000);
    }
    
    // Se il sistema è arrivato qui, vuol dire che la connessione è fallita.
    return -1;
}



int INTERNAL_GOCAM_ConfigureStartupDevice (void)
/*
    Questa routine si occupa della configurazione iniziale della webcam.

    Viene restituito il valore 0 se tutto si risolve in modo corretto,
    mentre viene restituito un valore negativo in caso di errore
*/
{
    PspUsbCamSetupVideoParam GoCam_Parameters;    
    // Struttura destinata ad accogliere i dati di configurazione della
    // webcam 
	
    int result;

    // Per prima cosa provvedi a configurare il sistema di decompressione
    // video ad una risoluzione standard di 480x272
    
    result = ndHAL_HWJPG_EnableJpegDecoder (ND_JPEGDEC_GOCAM_SUPPORT, ND_RESOLUTION_CODE_480_272);

    if (result==0)
    { 
    	memset(&GoCam_Parameters, 0, sizeof(GoCam_Parameters));
    	GoCam_Parameters.size = sizeof(GoCam_Parameters);
    	GoCam_Parameters.resolution = PSP_USBCAM_RESOLUTION_480_272;
    	GoCam_Parameters.framerate = PSP_USBCAM_FRAMERATE_15_FPS;
    	GoCam_Parameters.wb = PSP_USBCAM_WB_AUTO;
    	GoCam_Parameters.saturation = 125;
    	GoCam_Parameters.brightness = 128;
    	GoCam_Parameters.contrast = 64;
    	GoCam_Parameters.sharpness = 0;
    	GoCam_Parameters.effectmode = PSP_USBCAM_EFFECTMODE_NORMAL;
    	GoCam_Parameters.framesize = MAX_VIDEO_FRAME_JPG_SIZE;
    	GoCam_Parameters.evlevel = PSP_USBCAM_EVLEVEL_0_0;	
    	
    	result = sceUsbCamSetupVideo(&GoCam_Parameters, CAM_WorkAreaDriver, WORK_AREA_DRIVER_SIZE);	
        if (result < 0) return -1;
    	
    	sceUsbCamAutoImageReverseSW(1);
    	
    	result = sceUsbCamStartVideo();	
    	if (result < 0) return -2;
    	
    	// HACK: E' necessario che al momento della configurazione iniziale della videoamera,
    	// dopo l'inizializzazione venga letto almeno un frame (che viene perso). E' per questa 
        // ragione che a partire da Nanodesktop 0.3.4 il buffer JPEG viene allocato prima
        // di richiamare ConfigureStartupDevice. 
        // Infatti, l'utente può richiamare, subito dopo l'attivazione della webcam, la
        // funzione ChangeHwResolution. Quest'ultima esegue uno sceUsbCamStopVideo e questa
        // ultima funzione blocca la PSP se al momento in cui viene richiamata non era stato
        // letto almeno un frame
        
    	sceUsbCamReadVideoFrameBlocking (CAM_JpegBuffer, MAX_VIDEO_FRAME_JPG_SIZE);
    	
    	// Se l'elaborazione è giunta qui, vuol dire che non ci sono errori. Usa una
    	// routine apposita per eseguire l'impostazione iniziale dei dati
    	// immagine (questa routine si trova in ndHAL_WebCam1.c)
    	
        INTERNAL_GOCAM_RegisterNewInfoAboutCameraImages (480, 272, 480, 272, 24);
        return 0;
    }
    else               // Problema nel settare il decoder JPEG
      return -3;
}




/*
   ---------------------------------------------------------------------------
   OPEN/CLOSE
   ---------------------------------------------------------------------------
*/



int USBDEV1_GOCAM_OpenDevice (void)
/*

    Questa routine provvede ad aprire la periferica GoCam di Sony.
    
    Se il sistema riesce a collegarsi, impegna un handle USB e lo 
    restituisce in uscita.
    
    Altrimenti viene restituito un codice di errore negativo.
    
    Sono previsti i seguenti codici di errore:
       ERR_GOCAM_SUPPORT_IS_DISABLED            Il supporto GoCam è stato disattivato  
       ERR_FUNCTION_HAS_BEEN_DISABLED           Le funzionalità USB sono state disabilitate dall'utente
       ERR_DISABLED_FOR_TROUBLES_IN_MODULE_LOAD Problemi nel caricamento dei moduli del kernel
       ERR_DISABLED_FOR_TROUBLES_IN_JPEGDEC     Disabilitato per problemi nel supporto JPEG
       ERR_NO_USB_HANDLE_IS_AVAILABLE           Nessun handle USB disponibile
       ERR_IMPOSSIBLE_TO_CHANGE_STATE           Il sistema non riesce a cambiare lo stato del bus USB
       ERR_DEVICE_IS_NOT_OPERATIVE              La GoCam non è collegata
       ERR_IO_ERROR                             Fallita la configurazione iniziale della videocamera
       ERR_SYSTEM_IS_BUSY                       Il bus USB è impegnato da un'altra periferica
*/
{
    int  Counter, ErrRep, HandleFound;
    char IHaveFound;
    int  DriverErr;  
    int  result;
    
    if (!ND_HWMODE_DisableGoCamService)
    {     
        if (!ndGoCamIsFailure)
        {       
               if (!ndJpegDecoderIsFailure)  
               {
                            if (!ndUsb_BusIsBusy)
                            {
                                    for (Counter=0; Counter<ND_DEVICES_DESCRIPTORS; Counter++)
                                    {
                                        if (ndUsb_Dev [Counter].IsActive==0)
                                        {
                                             IHaveFound  = 1;
                                             HandleFound = Counter;
                                             
                                             break;
                                        }     
                                    }
                                    
                                    if (!IHaveFound) return ERR_NO_USB_HANDLE_IS_AVAILABLE;
                                    
                                    // Provvedi al caricamento dei drivers
                                    
                                    DriverErr = INTERNAL_GOCAM_LoadDeviceDrivers ();
                                    
                                    if (!DriverErr)
                                    {
                                         CAM_WorkAreaDriver = memalign (64, WORK_AREA_DRIVER_SIZE);               
                                         
                                         if (CAM_WorkAreaDriver!=0)
                                         {
                                                 ErrRep = INTERNAL_GOCAM_TryToActivateCam ();  
                                              
                                                 if (!ErrRep)    // Siamo riusciti ad attivare la videocamera
                                                 {
                                                      // Provvediamo alla configurazione iniziale della videocamera
                                                      
                                                      CAM_JpegBuffer = memalign (64, MAX_VIDEO_FRAME_DECOMP_SIZE);
                                                      
                                                      if (CAM_JpegBuffer!=0)
                                                      {            
                                                          ErrRep = INTERNAL_GOCAM_ConfigureStartupDevice ();
                                                                
                                                          if (!ErrRep)
                                                          {       
                                                               // Ora creiamo in memoria i buffer successivi a quello JPEG
                                                                                                         
                                                               CAM_FrameBuffer = memalign (64, FRAMEBUFFER_SIZE*sizeof(int));
                                                               
                                                               if (CAM_FrameBuffer!=0)
                                                               {
                                                                         ndUsb_Dev [HandleFound].TypeDevice = NDDEVTYPE_GOCAM;
                                                                         ndUsb_Dev [HandleFound].IsActive   = 1;
                                                            
                                                                         INTERNAL_GOCAM_SuccessConnectionNotify (); 
                                                                         
                                                                         // Impedirà al sistema di attivare due volte la webcam
                                                                         ndUsb_BusIsBusy=1;
                                                                         
                                                                         return HandleFound; 
                                                                         
                                                               }
                                                               else    // Non riesco ad allocare CAM_FrameBuffer
                                                               {
                                                                   free (CAM_JpegBuffer);
                                                                   // Libera l'area di memoria dell'area JPEG
                                                                   
                                                                   free (CAM_WorkAreaDriver);
                                                                   // Libera l'area di memoria del driver
                                                                   
                                                                   INTERNAL_GOCAM_UnLoadDeviceDrivers ();
                                                                   return ERR_MEMORY_OVERFLOW;
                                                               }
                                                            
                                                          }
                                                          else    // C'è stato un errore  nella configurazione della videocamera
                                                          {
                                                               free (CAM_JpegBuffer);
                                                               // Libera l'area di memoria dell'area JPEG
                                                               
                                                               free (CAM_WorkAreaDriver);
                                                               // Libera l'area di lavoro del driver
                                                               
                                                               INTERNAL_GOCAM_UnLoadDeviceDrivers ();
                                                               return ERR_IO_ERROR;       
                                                          }           
                                                 
                                                       }
                                                       else          // Errore nell'allocazione del buffer JPEG
                                                       {
                                                            free (CAM_WorkAreaDriver);
                                                      
                                                            INTERNAL_GOCAM_UnLoadDeviceDrivers ();
                                                            return ERR_MEMORY_OVERFLOW;   
                                                       }
                                                 }
                                                 else            // La videocamera non è presente o è guasta
                                                 {
                                                      free (CAM_WorkAreaDriver);
                                                      
                                                      INTERNAL_GOCAM_UnLoadDeviceDrivers ();
                                                      return ERR_DEVICE_IS_NOT_OPERATIVE;           
                                                 }
                                        
                                         }
                                         else     // Problemi nell'allocazione dell'area di lavoro del driver
                                         {   
                                             INTERNAL_GOCAM_UnLoadDeviceDrivers ();
                                             return ERR_MEMORY_OVERFLOW;
                                         }                        
                                                                        
                                    }
                                    else     // Problema nel caricamento dei drivers
                                    {
                                        INTERNAL_GOCAM_UnLoadDeviceDrivers ();
                                        return ERR_CANNOT_LOAD_DRIVERS;
                                    }
                                                                              
                            }
                            else     // Il bus è già impegnato
                              return ERR_SYSTEM_IS_BUSY;
               }              
               else    // Disabilitato per problemi nel decoder JPEG
                 return ERR_DISABLED_FOR_TROUBLES_IN_JPEGDEC;  
                 
        }
        else  // Problemi nel caricamento dei moduli del kernel
          return ERR_DISABLED_FOR_TROUBLES_IN_MODULE_LOAD;                                                                
    }
    else    // Supporto GoCam disabilitato
      return ERR_GOCAM_SUPPORT_IS_DISABLED;
}


void USBDEV1_GOCAM_CloseDevice (void)
{
    INTERNAL_GOCAM_StopAudioRecording ();
    // Disattiva il processo di record audio eventualmente attivo
    
    INTERNAL_GOCAM_DeActivateCamMicrophone ();
    // Disattiva il microfono della videocamera
    
    free (CAM_JpegBuffer);
    free (CAM_FrameBuffer);
    free (CAM_WorkAreaDriver);
                 
    ndHAL_HWJPG_DisableJpegDecoder ();
    // Disimpegna il decoder JPEG hardware liberandolo per altre applicazioni          
                 
    INTERNAL_GOCAM_UnLoadDeviceDrivers (); 
    // Scarica i drivers della videocamera dalla memoria
    
    ndUsb_BusIsBusy=0;                            // Disimpegna il bus
}


