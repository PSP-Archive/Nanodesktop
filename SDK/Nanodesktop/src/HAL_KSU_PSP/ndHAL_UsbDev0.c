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
    ROUTINES DI GESTIONE USB SPECIFICHE PER PERIFERICA
    EYESERVER  (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>


/*
   ----------------------------------------------------------------------------
   Routines di supporto per l'inizializzazione della CAM via Eyeserver
   ----------------------------------------------------------------------------
*/

static struct ndImage_Type _CameraVideo_Image;              // Usato internamente al modulo per notifica Eyeserver connection
static char EyeSvrConn_WndNotifyHnd;
static char EyeSvrConn_WndCreated = 0;   


static void INTERNAL_EYESVR_WaitForConnectNotify (int Attempt)
{
    
    // Crea la finestra di notifica della finestra
    
    if (!EyeSvrConn_WndCreated)
    {
            EyeSvrConn_WndNotifyHnd = ndLP_CreateWindow (70, Windows_MaxScreenY/2 - 40, Windows_MaxScreenX - 70, Windows_MaxScreenY/2 + 40,
                           "Waiting", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY, COLOR_GRAY, 0); 
            
            if (EyeSvrConn_WndNotifyHnd>=0)
            {
                   EyeSvrConn_WndCreated=1;
                                
                   ndLP_MaximizeWindow (EyeSvrConn_WndNotifyHnd);
                
                   ndIMG_LoadImageFromNDFArray (&_CameraVideo_Image, CameraVideoWidth, CameraVideoHeight, &CameraVideo, NDMGKNB);
                
                   ndIMG_ShowImage (&_CameraVideo_Image, 30, 10, EyeSvrConn_WndNotifyHnd, NORENDER);
                   ndWS_GrWriteLn (100, 25, "USB: wait for connection", COLOR_BLACK, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, RENDER);
                   ndWS_GrPrintLn (100, 33, COLOR_BLACK, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, RENDER, "Attempt nr. %d", Attempt);
            
                   ndIMG_ReleaseImage (&_CameraVideo_Image);
            } 
    }
    else    // La finestra è già creata
    {
                  ndWS_DrawRectangle (95, 15, WindowData [EyeSvrConn_WndNotifyHnd].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, NORENDER);
                  ndWS_GrWriteLn (100, 25, "USB: wait for connection", COLOR_BLACK, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, NORENDER);
                  ndWS_GrPrintLn (100, 33, COLOR_BLACK, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, RENDER, "Attempt nr. %d", Attempt);
    
                  ndDelay (1);
    }   
}




static void INTERNAL_EYESVR_ConnectionNotify (void)
{
    if (EyeSvrConn_WndCreated)
    {
        ndWS_DrawRectangle (95, 15, WindowData [EyeSvrConn_WndNotifyHnd].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, NORENDER);
        ndWS_GrWriteLn (100, 25, "Connecting to Eyeserver", COLOR_BLACK, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, NORENDER);
        
        ndDelay (1);
    }   
}

static void INTERNAL_EYESVR_SuccessConnectionNotify (void)
{
     if (EyeSvrConn_WndCreated)
     {
        EyeSvrConn_WndCreated = 0;
      
        ndWS_DrawRectangle (95, 15, WindowData [EyeSvrConn_WndNotifyHnd].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, NORENDER);
        ndWS_GrWriteLn (100, 25, "Connection established", COLOR_BLACK, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, RENDER);
    
        ndDelay (1);
        ndLP_DestroyWindow (EyeSvrConn_WndNotifyHnd);
     }
}

static void INTERNAL_EYESVR_ErrorConnectionNotify (void)
{
     if (EyeSvrConn_WndCreated)
     {
        EyeSvrConn_WndCreated = 0;
      
        ndWS_DrawRectangle (95, 15, WindowData [EyeSvrConn_WndNotifyHnd].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, NORENDER);
        ndWS_GrWriteLn (100, 25, "Connection failed", COLOR_RED, COLOR_GRAY, EyeSvrConn_WndNotifyHnd, RENDER);
    
        ndDelay (1);
        ndLP_DestroyWindow (EyeSvrConn_WndNotifyHnd);
     }
}



/*
   -----------------------------------------------------------------------------
   Gestione dei drivers USB per l'Eyeserver
   -----------------------------------------------------------------------------
*/

int INTERNAL_EYESVR_LoadDeviceDrivers (void)
/*
     Questa routine provvede a caricare i drivers necessari per la gestione
     dell'Eyeserver in memoria. Restituisce 0 se tutto ok, ed un codice
     negativo in caso di errore. 
     
     Nel caso di errore, la routine deposita dentro la variabile di ambiente
     ndEyesvrFailure_MainErrCode il codice di errore principale e dentro
     ndEyesvrFailure_SecErrCode il codice di errore secondario
*/
{
    // Nulla da fare, perchè nella modalità KSU l'abilitazione dell'USB 
    // avviene nella fase di boot del sistema
    
    return 0;    // Simula successo
}

void INTERNAL_EYESVR_UnLoadDeviceDrivers (void)
{
    // Nulla da fare, perchè nella modalità KSU la disabilitazione dell'USB 
    // avviene nella fase di hang del sistema
}








/*
   ---------------------------------------------------------------------------
   OPEN/CLOSE
   ---------------------------------------------------------------------------
*/

int USBDEV0_EYESVR_OpenDevice (void)
/*
    Questa routine provvede ad aprire una periferica virtuale Eyeserver.
    In realtà, questa routine provvede a gestire tutto il processo
    di connessione USB con l'Eyeserver.
    
    Se il sistema riesce a connettersi con l'Eyeserver, impegna un handle USB
    e lo restituisce in uscita.
    
    Altrimenti viene restituito un codice di errore negativo.
    
    Sono previsti i seguenti codici di errore:
       ERR_I_CANNOT_CONNECT_TO_EYESERVER      Non sono riuscito a connettermi con l'EyeServer
       ERR_FUNCTION_HAS_BEEN_DISABLED         Le funzionalità USB sono state disabilitate dall'utente
       ERR_NO_USB_HANDLE_IS_AVAILABLE         Nessun handle USB disponibile
       ERR_CANNOT_LOAD_DRIVERS                Non posso caricare i drivers
*/         
{
    int  DriverErr; 
    int  HandleFound;
    char IHaveFound;
    int  Attempt=0;
    int  Counter;
                                
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
    
    // Proviamo a caricare i drivers dell'Eyeserver
    
    DriverErr = INTERNAL_EYESVR_LoadDeviceDrivers ();
    
    if (!DriverErr) 
    {        
            while (Attempt<4)
            { 
                  Attempt++;
                            
                  INTERNAL_EYESVR_WaitForConnectNotify (Attempt);
            
                  if ( USB_PRX_WaitForConnect (3000000) )                  // Attendi la connessione USB con l'Eyeserver
                  {  
                                    INTERNAL_EYESVR_ConnectionNotify ();
                                     
                                    memset (&UsbWCPacketRq,   0, sizeof (UsbWCPacketRq));
                                    memset (&UsbWCPacketResp, 0, sizeof (UsbWCPacketResp));
                                    
                                    UsbWCPacketRq.cmd.magic     = HOSTFS_MAGIC;
                                    UsbWCPacketRq.cmd.command   = CMD_HELLO_IMAGESERVER;
                                    UsbWCPacketRq.cmd.extralen  = 0;
                     
                                    // In questo punto, la scrittura del comando e la lettura della
                                    // risposta da parte dell'Eyeserver devono avvenire in due
                                    // momenti separati. Per una qualche ragione, si è verificato
                                    // che usando USB_PRX_CommXChg qui si verificano
                                    // blocchi di sistema saltuari. E' per questa ragione che
                                    // si è fatto ricorso a questo hack.
                                    
                                    sceKernelDelayThread (2000000);
                                    
                                    if ( USB_PRX_WriteData (&UsbWCPacketRq, sizeof(UsbWCPacketRq), 2000000 ) )
                                    {
                                       sceKernelDelayThread (1000000);
                                       
                                       if ( USB_PRX_ReadData (&UsbWCPacketResp, sizeof (UsbWCPacketResp), 2000000 ) )
                                       {
                            
                                            if ((UsbWCPacketResp.cmd.magic == HOSTFS_MAGIC) && (UsbWCPacketResp.cmd.command == CMD_RESPONSE_TO_HELLO))
                                            {
                                                 ndUsb_Dev [HandleFound].TypeDevice = NDDEVTYPE_EYESERVER_CAM;
                                                 ndUsb_Dev [HandleFound].IsActive   = 1;
                                                 
                                                 // Impegna il bus
                                                 ndUsb_BusIsBusy=1;
                                    
                                                 // Notifica successo
                                                 INTERNAL_EYESVR_SuccessConnectionNotify (); 
                                                 return HandleFound;       
                                            }
                                            else    // Errore nel pacchetto ricevuto
                                            {
                                                 continue;
                                            }
                                       }
                                       else   // Errore in lettura
                                       {
                                           continue;
                                       }
                                    }     
                                    else   // Errore in scrittura
                                    {
                                        continue;
                                    }
                                 
                  }
                  
                  sceKernelDelayThread (2000000);
                  // Aspetta un secondo prima di riprovare a connetterti
            
            }
            
            // Se il sistema arriva a questo punto, significa che il
            // processo di connessione è fallito. Innanzitutto provvedi
            // a disattivare il sistema Usb
            
            INTERNAL_EYESVR_UnLoadDeviceDrivers ();  
                   
            // Ed ora notifica il problema all'utente       
            INTERNAL_EYESVR_ErrorConnectionNotify (); 
            return ERR_I_CANNOT_CONNECT_TO_EYESERVER; 
    }        
    
    // Se il sistema arriva a questo punto, vuol dire che ci sono dei problemi
    // con i drivers connessi al sistema Eyeserver. Scaricali dalla memoria
    
    INTERNAL_EYESVR_UnLoadDeviceDrivers ();     
}    


void USBDEV0_EYESVR_CloseDevice (void)
{
    INTERNAL_EYESVR_UnLoadDeviceDrivers (); 
    ndUsb_BusIsBusy=0;                  // Disimpegna il bus
}
