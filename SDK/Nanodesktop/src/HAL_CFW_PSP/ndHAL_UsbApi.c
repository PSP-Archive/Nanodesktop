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
    API AD ALTO LIVELLO PER IL SUPPORTO USB  (PSP CFW mode)
    ---------------------------------------------------------------------------
    Questo modulo contiene l'API per la gestione USB ad alto livello: queste
    routines sono quelle che verranno usate direttamente dall'utente. 
    
    Esse astraggono al programma applicativo le caratteristiche della porta
    USB, nascondendo la differenza tra la gestione PC e la gestione PSP.
    
    Per esempio, queste routines fanno sì che l'utente non debba accedere
    alle funzionalità PRX  che vengono esportate dal driver USB della PSP
    ---------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>


   


/*
    ---------------------------------------------------------------------------
    INIZIALIZZAZIONE DELL'API (high level)
    ---------------------------------------------------------------------------
*/



void ndHAL_InitUSBApi (void)
{
         int Counter;
         
         for (Counter=0; Counter<ND_DEVICES_DESCRIPTORS; Counter++)
         {
             ndUsb_Dev [Counter].IsActive = 0;
         }   
     
         INTERNAL_Init_UsbSystem ();     // Esegui la reinizializzazione di tutto il sistema USB
}









/*
    ---------------------------------------------------------------------------
    API PER LA GESTIONE USB (high level)
    ---------------------------------------------------------------------------
*/




int ndHAL_USB_OpenDevice (struct usb_device *dev)
/*
    Questa routine si occupa di aprire il dispositivo USB descritto dalla struttura
    struct usb_device. 
    
    Si è mantenuta la struttura struct usb_device per compatibilità con 
    la libreria libusb che viene usata in ambiente Unix. 
    
    Le informazioni che seguono riguardano esclusivamente la PSP.
    
    Attualmente, l'unico tipo di dispositivo supportato è l'eyeserver PSP. 
    
    Pertanto, l'utente deve passare alla routine una struct di tipo
    usb_device dove vengono ignorati tutti i campi tranne il campo
    descriptor.idVendor e descriptor.idProduct. 
    
    Questi due campi definiscono il dispositivo USB utilizzato. Tutti
    gli altri campi della struct usb_device *dev vengono ignorati.
    
    Se l'utente cerca di aprire il dispositivo usando 
    descriptor.idVendor  = EYESERVER_VENDOR_ID
    descriptor.idProduct = EYESERVER_PRODUCT_ID
    
    il sistema restituirà in uscita un handle che descriverà in seguito
    il dispositivo. In seguito verranno aggiunti altri codici per il
    supporto di altri servizi come i canali asincroni del driver
    HostFS di Tyranid
    
    Sono previsti i seguenti codici di errore:
       ERR_NO_USB_HANDLE_IS_AVAILABLE         Non ci sono più handle USB disponibili      
       ERR_ERROR_IN_OPEN_DEVICE               Errore durante l'apertura del dispositivo
       ERR_DEVICE_NOT_SUPPORTED               Dispositivo non supportato
       ERR_I_CANNOT_CONNECT_TO_EYESERVER      Non sono riuscito a connettermi con l'EyeServer
       ERR_FUNCTION_HAS_BEEN_DISABLED         Le funzionalità USB sono state disabilitate dall'utente
       ERR_SYSTEM_IS_BUSY                     Il bus USB è già impegnato da un'altra periferica
*/

{
    int  HandleFound;
    char IHaveFound;
    
    IHaveFound=0;
    
    if ((!ND_HWMODE_DisableUsbService) && (!ndUsb_IsFailure))
    {         
            if (!ndUsb_BusIsBusy)
            {
                    // Iniziamo a verificare che tipo di apertura di dispositivo è stata richiesta:
                               
                    // Caso EYESERVER 
                    
                    if ((dev->descriptor.idVendor==EYESERVER_VENDOR_ID) && (dev->descriptor.idProduct==EYESERVER_PRODUCT_ID))
                    {
                        HandleFound = USBDEV0_EYESVR_OpenDevice ();
                        if (HandleFound>=0) return HandleFound;
                        
                        // Se non si sono verificati errori, restituisci l'handle alla routine
                        // chiamante
                    }
                    
                    // Caso GOCAM/CHOTTO CAM
                    
                    if ((dev->descriptor.idVendor==GOCAM_VENDOR_ID) && (dev->descriptor.idProduct==GOCAM_PRODUCT_ID))
                    {
                        HandleFound = USBDEV1_GOCAM_OpenDevice ();
                        if (HandleFound>=0) return HandleFound;
                        
                        // Se non si sono verificati errori, restituisci l'handle alla routine
                        // chiamante
                    }
                    
                    return ERR_DEVICE_NOT_SUPPORTED;
            } 
            else 
               return ERR_SYSTEM_IS_BUSY;      
    }
    else
        return ERR_FUNCTION_HAS_BEEN_DISABLED;
}




void ndHAL_USB_CloseDevice (int UsbHandle)
/*
    Questa routine si occupa di chiudere un dispositivo e di 
    disallocare l'handle USB ad esso associato.
*/
{
    if ((!ND_HWMODE_DisableUsbService) && (!ndUsb_IsFailure))
    {         
       if (ndUsb_Dev [UsbHandle].IsActive)
       {
            // Iniziamo a verificare che tipo di chiusura di dispositivo è stata richiesta...
                       
            switch (ndUsb_Dev [UsbHandle].TypeDevice)
            {
                 // Caso EYESERVER 
                 
                 case NDDEVTYPE_EYESERVER_CAM:
                 {
                      USBDEV0_EYESVR_CloseDevice ();
                      break;
                 }  
                 
                 // Caso GOCAM
                 
                 case NDDEVTYPE_GOCAM:
                 {
                      USBDEV1_GOCAM_CloseDevice ();
                      break;
                 }
            }
            
            // Disimpegniamo l'handle
            
            ndUsb_Dev [UsbHandle].IsActive=0;  
       }    
    }     
}














/*
    ---------------------------------------------------------------------------
    MONITORAGGIO DEL BUS
    ---------------------------------------------------------------------------
*/



void INTERNAL_UsbMonitorCallback (void)
{
     while (!ndUsb_CommArea.ManagerLoopExit)
     {
           
           if (ndUsb_CommArea.MemoryLockState==1)    
           // Ci sono messaggi debug USB in attesa
           
           {
                     // Indichiamo il tipo di evento USB
                     
                     switch (ndUsb_CommArea.MessageCode)
                     {
                            case USBMSG_DEBUG:
                            {
                                 ndWS_PrintLn (ndUsb_CommArea.WndHandle, COLOR_RED, NORENDER, "USB Message: Debug");
                                 break;
                            }
                            
                            case USBMSG_SERVICE:
                            {
                                 ndWS_PrintLn (ndUsb_CommArea.WndHandle, COLOR_RED, NORENDER, "USB Message: Service");
                                 break;
                            }
                            
                            default:
                            {
                                 ndWS_PrintLn (ndUsb_CommArea.WndHandle, COLOR_RED, NORENDER, "USB Message: Code %d", ndUsb_CommArea.MessageCode);
                                 break;
                            }
                     }       
                                    
                     ndWS_PrintLn (ndUsb_CommArea.WndHandle, COLOR_WHITE,  NORENDER, "%s ", ndUsb_CommArea.StrMessage);
                     ndWS_CarriageReturn (ndUsb_CommArea.WndHandle);
                     
                     XWindowRender (ndUsb_CommArea.WndHandle);     
                
                     ndUsb_CommArea.MemoryLockState=0;
                     // Libera il sistema, consentendo ad ulteriori chiamate debug USB di essere eseguite
           }
           
           sceDisplayWaitVblankStart ();
     }
}


void ndHAL_USB_EnableDriverMonitor ()
{
    int _UsbMonThread;
    
    if (ndUsb_CommArea.MonitorEnabled==0)
    {
    
            ndUsb_CommArea.WndHandle = ndLP_AllocateWindow (80, 30, Windows_MaxScreenX-30, Windows_MaxScreenY-88,
                                                                          "Psp Usb System Monitor", COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_WHITE); 
                                                                          
            if (ndUsb_CommArea.WndHandle>=0)              // Non ci sono stati errori nell'apertura della finestra
            {
                ndLP_MaximizeWindow (ndUsb_CommArea.WndHandle);
                ndUsb_CommArea.MonitorWndIsOpen = 1;
            
                _UsbMonThread = sceKernelCreateThread ("UsbMonitor", INTERNAL_UsbMonitorCallback, 0x10, 0x8000, PSP_THREAD_ATTR_USER | THREAD_ATTR_VFPU, 0);
            
                if (_UsbMonThread>=0)
                {
                     ndUsb_CommArea.MonitorThread    = _UsbMonThread;
                     ndUsb_CommArea.MemoryLockState  = 0;
                     ndUsb_CommArea.ManagerLoopExit  = 0;
                     
                     sceKernelStartThread (_UsbMonThread, 0, 0);          
                     ndUsb_CommArea.MonitorEnabled = 1;    
                }
                else
                {
                    ndLP_DestroyWindow (ndUsb_CommArea.WndHandle);
                    ndUsb_CommArea.MonitorWndIsOpen = 0;
                    
                    ndUsb_CommArea.MonitorEnabled = 0;
                }  
            }
            
    }
}



void ndHAL_USB_DisableDriverMonitor ()
{
    if (ndUsb_CommArea.MonitorEnabled)
    {
        if (ndUsb_CommArea.MonitorWndIsOpen)
        {
                ndUsb_CommArea.ManagerLoopExit=1;     // Questo provocherà l'uscita del monitor thread                                                                            
            
                sceKernelDelayThread (1000000);
                // Attende la fine del monitor thread: questo darà anche il tempo al sistema
                // di processare eventuali chiamate che erano in attesa
                
                sceKernelTerminateDeleteThread (ndUsb_CommArea.MonitorThread);
                // Il thread del monitor viene terminato e poi cancellato dal sistema
                
                ndLP_DestroyWindow (ndUsb_CommArea.WndHandle);
                
                ndUsb_CommArea.MonitorWndIsOpen = 0;
                ndUsb_CommArea.MonitorEnabled   = 0;  
        }                                                                          
    }
    
}



