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
    FUNZIONALITA' DI SUPPORTO USB A BASSO LIVELLO  (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>




/*
    ---------------------------------------------------------------------------
    GESTIONE ICONA DRIVER USB
    ---------------------------------------------------------------------------
*/

void INTERNAL_DrawUsbIcon (TypeColor Colore, char Page)
/*
    Questa routine si occupa di disegnare sullo schermo l'icona USB
*/
{
    register unsigned short int UsbIconX, UsbIconY;
    register unsigned short int CounterX, CounterY;
    
    UsbIconX = MinWindowsBar_x2 + 38;
    UsbIconY = MinWindowsBar_y1 + 1;
    
    for (CounterY=0; CounterY<=10; CounterY++)
    {
        for (CounterX=0; CounterX<40; CounterX++)
        {
            if (TheUsbIcon [CounterY][CounterX]=='X') 
                 ndHAL_PutPixelToPage (UsbIconX+CounterX, UsbIconY+CounterY, Colore, Page);
        }
    }
}




void INTERNAL_ManagerUsbIcon (char Page)
/*
    Questa routine viene richiamata periodicamente da WindowsRender. 
    
    Essa si occupa di disegnare, ove necessario, l'icona USB nella 
    MinWindowsBar, indicando lo stato del collegamento USB.
*/
{
    if (!ND_HWMODE_DisableUsbService)     // Disegna qualcosa solo se l'USB non è stato disattivato
    {                                         
              
              switch (ndUsb_CommArea.UsbState)
              {
                     case USBBUS_FAILURE:
                     {
                         INTERNAL_DrawUsbIcon (COLOR_DRED, Page);
                         break;  
                     }
                     
                     case USBSTATE_FAILURE:
                     {
                         INTERNAL_DrawUsbIcon (COLOR_RED, Page);
                         break; 
                     }
                     
                     case USBSTATE_ATTACHED:
                     {
                         INTERNAL_DrawUsbIcon (COLOR_LBLUE, Page);
                         break; 
                     }
                     
                     case USBSTATE_DETACHED:
                     {
                         INTERNAL_DrawUsbIcon (COLOR_GRAY04, Page);
                         break; 
                     }        
              }
              
              ndUsb_CommArea.UsbStateIsChanged = 0;
              // Farà sì che il prossimo WindowsRender non debba ridisegnare l'icona se questa non è cambiata                            
    } 
}


/*
    ---------------------------------------------------------------------------
    GESTIONE DEI MESSAGGI CON IL DRIVER PRX USB
    ---------------------------------------------------------------------------
*/



void INTERNAL_PrepareUsbCommArea (void)
/*
    Questa routine provvede a preparare i dati relativi alla CommArea:
    questa operazione di reinizializzazione viene eseguita una sola
    volta all'avvio del sistema
*/

{
    ndUsb_CommArea.GraphSysIsLocked = 1;
    // Poni l'inibitore al valore 1, perchè in questa fase il sistema grafico non
    // è stato ancora attivato: questo significa che il renderer dell'icona USB
    // deve evitare di disegnare l'icona sullo schermo. Un'apposita routine si
    // occuperà di riportare a zero questa variabile subito dopo l'avvio di
    // ndInitSystem, consentendo la visualizzazione dell'icona. 
     
    ndUsb_CommArea.PntWindowsRender        = &INTERNAL_UsbWindowsRender;
    ndUsb_CommArea.PntVxPrintf             = &vxprintf;
    ndUsb_CommArea.PntSout                 = &_INTERNAL_WriteFncToString_;
    
    ndUsb_CommArea.MonitorEnabled          = 0;
    ndUsb_CommArea.MonitorWndIsOpen        = 0;
    
    // All'inizio il sistema pone l'USB nello stato detached: sarà il driver PRX
    // a fare passare lo stato ad un valore differente
    
    ndUsb_CommArea.UsbState                 = USBSTATE_DETACHED;
    ndUsb_CommArea.UsbStateIsChanged        = 1;
    
    return; 
}



void INTERNAL_Init_UsbSystem (void)
/*
     Questa routine si occupa di reinizializzare il sistema USB della PSP. 
     
     La reinizializzazione viene eseguita dal main thread della modalità KSU:
     tale main thread gira in kernel mode e grazie all'azione del loader
     definito in ndHAL_PlatformHeaders.h, può caricare moduli del firmware
     della PSP ed inizializzarli. 
     
     L'operazione di loading viene eseguita una sola volta all'avvio 
     dell'homebrew. 
     
     Se l'operazione riesce, la variabile ndUsb_IsFailure viene posta a 0. 
     
     Se invece l'operazione fallisce, il sistema pone la variabile ndUsb_IsFailure
     ad 1, e deposita il codice di errore dentro le variabili di sistema 
     ndUsb_Failure_MainErrCode e ndUsb_Failure_SecErrCode. 
     
     La variabile ndUsb_Failure_MainErrCode assume valori che sono stabiliti da
     una serie di costanti simboliche, stabilite in ndHAL_Variables.h. 
     
     Il significato di ndUsb_Failure_SecErrCode dipende dal codice di errore
     presente in ndUsb_Failure_MainErrCode.
     
     Questi sono i codici di errore principali riconosciuti:
     
     ERR_USB_FAILED_LOADING_PRX_DRIVER:
         Il sistema non riesce a caricare il driver PRX dalla memory stick. 
         Nella variabile ndUsb_Failure_SecErrCode viene registrato il codice 
         UID restituito dalla funzione del kernel pspSdkLoadStartModule
         
     ERR_USB_FAILED_LOADING_USB_BUS_DRIVER:
         Il sistema non riesce ad inizializzare il driver del kernel                                   
         PSP_USBBUS_DRIVERNAME (nel firmware 1.5 il vero nome è
         flash0:/kd/usb.prx, la costante simbolica è definita dentro
         usb.h). Nella variabile ndUsb_Failure_SecErrCode, viene registrato
         il valore di ritorno della funzione del firmware sceUsbStart
         
     ERR_USB_FAILED_INIT_HOSTFS_DRIVER:
         Il sistema non riesce ad inizializzare il driver hostfs che era
         stato caricato precedentemente dalla memory stick. Nella variabile 
         ndUsb_Failure_SecErrCode, viene registrato il valore di ritorno 
         della funzione del firmware sceUsbStart
         
     ERR_USB_FAILED_ACTIVATE_USB_PID:
         Il sistema non riesce ad attivare il driver PID. Normalmente questa
         operazione attiva il canale tra la PSP ed il PC. Nella variabile 
         ndUsb_Failure_SecErrCode, viene registrato il valore di ritorno 
         della funzione del firmware sceUsbActivate
*/
{
    int retVal;
	SceUID uid;
	
	ndUsb_IsFailure           = 0;
	ndUsb_Failure_MainErrCode = 0;
	ndUsb_Failure_SecErrCode  = 0;
	
	INTERNAL_PrepareUsbCommArea ();
	// Prepara la CommArea da passare al driver PRX USB
    
	uid = pspSdkLoadStartModule ("fatms0:/ndUSBDriver.prx", 0);
	
    if(uid < 0)
	{
		ndUsb_IsFailure                  = 1;
		ndUsb_Failure_MainErrCode        = ERR_USB_FAILED_LOADING_PRX_DRIVER;
		ndUsb_Failure_SecErrCode         = uid;
		
		ndUsb_CommArea.UsbState          = USBBUS_FAILURE;
        ndUsb_CommArea.UsbStateIsChanged = 1;
         
		return;
	}
	    
    pspSdkFixupImports (uid);
    // E' necessario per consentire l'accesso alle funzioni del modulo
    // che costituisce il driver USB.
    
    USB_PRX_PassCommAreaAdr (&ndUsb_CommArea);
	// Il driver PRX riceve l'indirizzo di una struttura che viene
	// opportunamente preparata, inserendovi alcuni parametri di
	// gestione e gli indirizzi di alcune routines di Nanodesktop.
	// In tal modo, il driver PRX diventa in grado di 
    // richiamare procedure di Nanodesktop, come il gestore dei
    // messaggi, senza dovere importare esplicitamente i simboli. 
    
    // Il driver PRX USB può utilizzare le funzioni grafiche di
    // Nanodesktop in modo da segnalare all'utente operazioni
    // come l'attivazione o la disattivazione dell'USB, o
    // l'inserimento del connettore USB.
    

	retVal = sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
	if (retVal != 0) 
    {
	    ndUsb_IsFailure                  = 1;
	    ndUsb_Failure_MainErrCode        = ERR_USB_FAILED_LOADING_USB_BUS_DRIVER;
	    ndUsb_Failure_SecErrCode         = retVal;
        
        ndUsb_CommArea.UsbState          = USBBUS_FAILURE;
        ndUsb_CommArea.UsbStateIsChanged = 1;
        	
	    return;
	}


	retVal = sceUsbStart(HOSTFSDRIVER_NAME, 1, 0);
	if (retVal != 0) 
    {
        ndUsb_IsFailure                  = 1;
	    ndUsb_Failure_MainErrCode        = ERR_USB_FAILED_INIT_HOSTFS_DRIVER;
        ndUsb_Failure_SecErrCode         = retVal;
        
        ndUsb_CommArea.UsbState          = USBSTATE_FAILURE;
        ndUsb_CommArea.UsbStateIsChanged = 1;
        
        return;	       
	}
	
	retVal = sceUsbActivate(HOSTFSDRIVER_PID);
	if(retVal != 0)
	{
        ndUsb_IsFailure                  = 1;
        ndUsb_Failure_MainErrCode        = ERR_USB_FAILED_ACTIVATE_USB_PID;
        ndUsb_Failure_SecErrCode         = retVal;     
        
        ndUsb_CommArea.UsbState          = USBSTATE_FAILURE;
        ndUsb_CommArea.UsbStateIsChanged = 1;
              
		return;
	}

	// Se il sistema arriva in questo punto vuol dire che non ci sono
	// stati errori nella gestione dell'USB
	
	sceKernelDelayThread (3000000);    // Ritardo di due secondi per consentire al sistema di stabilizzarsi
	
	return;
}







void INTERNAL_UnlockGraphicalSystemForUSB (void)
/*
    Segnala al driver USB PRX in memoria che il sistema grafico è stato sbloccato.
    Usata una sola volta all'avvio del sistema
*/
{
    ndUsb_CommArea.GraphSysIsLocked=0; 
}


void INTERNAL_UsbWindowsRender (void)
/*
    Questa routine viene introdotta allo scopo di risolvere un problema, senza
    che sia necessario modificare il driver ndUsbDriver.Prx. 
    
    Il WindowRender da parte del driver Usb deve essere richiesto in modo
    esplicito solo quando il sistema Phoenix è attivato
*/
{
     if (!ND_SWMODE_DisablePhoenixSubsystem) WindowsRender ();
}


void INTERNAL_TerminateUsbSystem (void)
{
     // Nessuna operazione per ora
}


