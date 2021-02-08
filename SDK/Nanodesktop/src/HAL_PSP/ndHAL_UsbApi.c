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
    API AD ALTO LIVELLO PER IL SUPPORTO USB  (PSP)
    ---------------------------------------------------------------------------
    Questa API viene mantenuta solo per garantire il funzionamento del linker,
    perchè sotto PSP senza KSU non è supportato USB
    ---------------------------------------------------------------------------
*/

#include <HAL_PSP\$_HAL_NDforND.h>
   


/*
    ---------------------------------------------------------------------------
    INIZIALIZZAZIONE DELL'API (high level)
    ---------------------------------------------------------------------------
*/



void ndHAL_InitHighLevelUSBApi (void)
{
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
*/

{
        return ERR_FUNCTION_HAS_BEEN_DISABLED;
}


int ndHAL_USB_ReadData (int UsbHandle, void *Data, int Size, int TimeOut, int Options)
/*
    Questa routine legge dei dati dalla periferica individuata dall'UsbHandle. 
    
    I dati sono inseriti nel buffer *Data ed il numero di byte che il 
    sistema tenta di leggere è dato da Size.
    
    Il parametro TimeOut permette di settare la condizione di timeout del
    driver di lettura. Il parametro non ha effetto ove il driver USB non
    supporti la funzione (ad esempio, sotto PSP)
    
    Il parametro a 32 bit Options serve per settare le funzioni di lettura
    speciali: non è attualmente utilizzato e può essere impostato a 0.

    Se tutto va a buon fine, il sistema restituisce un valore che rappresenta
    il numero di bytes letti

    Se invece il sistema restituisce un valore negativo, si è in presenza 
    di un errore. 
    
    Sono previsti i seguenti codici di errore:
     ERR_USBHANDLE_IS_NOT_ACTIVE    L'handle che è stato fornito non è attivo  
     ERR_IO_ERROR                   Errore di I/O  
     ERR_TIMEOUT                    Timeout in lettura
     ERR_FUNCTION_HAS_BEEN_DISABLED Le funzionalità USB sono state disabilitate dall'utente
*/
{
        return ERR_FUNCTION_HAS_BEEN_DISABLED; 
}




int ndHAL_USB_WriteData (int UsbHandle, void *Data, int Size, int TimeOut, int Options)
/*
    Questa routine trasferisce dei dati nella periferica individuata 
    dall'UsbHandle. 
    
    I dati sono prelevati dal buffer *Data ed il numero di byte che il 
    sistema tenta di leggere è dato da Size.
    
    Il parametro TimeOut permette di settare la condizione di timeout del
    driver in scrittura. Il parametro non ha effetto ove il driver USB non
    supporti la funzione (ad esempio, sotto PSP)
    
    Il parametro a 32 bit Options serve per settare le funzioni di scrittura
    speciali: non è attualmente utilizzato e può essere impostato a 0.

    Se tutto va a buon fine, il sistema restituisce un valore che rappresenta
    il numero di bytes trasferiti con successo nel dispositivo 

    Se invece il sistema restituisce un valore negativo, si è in presenza 
    di un errore. 
    
    Sono previsti i seguenti codici di errore:
     ERR_USBHANDLE_IS_NOT_ACTIVE    L'handle che è stato fornito non è attivo  
     ERR_IO_ERROR                   Errore di I/O  
     ERR_TIMEOUT                    Timeout in lettura
     ERR_FUNCTION_HAS_BEEN_DISABLED Le funzionalità USB sono state disabilitate dall'utente
*/
{
       return ERR_FUNCTION_HAS_BEEN_DISABLED;  
}



void ndHAL_USB_CloseDevice (int UsbHandle)
/*
    Questa routine si occupa di chiudere un dispositivo e di 
    disallocare l'handle USB ad esso associato.
*/
{      
}


int ndHAL_USB_CommXChg (int UsbHandle, void *OutCmd, int OutCmdLen, void *InCmd, int InCmdLen, 
                        const void *OutData, int OutLen, void *InData, int InLen, int TimeOut, int Options)
/*
    Questa routine esegue la sequenza di scambio comandi con la periferica individuata da UsbHandle.
    
    Per prima cosa viene trasferito verso la periferica il contenuto del buffer OutCmd, seguito 
    dal contenuto del buffer OutData. 
    
    Poi viene letto dalla periferica il flusso di dati InCmd, seguito dal flusso di dati InData. 
    
    Ciascuna delle 4 fasi può essere skippata, ponendo a 0 il relativo parametro OutCmdLen, InCmdLen,
    OutLen, InLen. 
    
    I parametri TimeOut ed Options sono attualmente inutilizzati. 
    
    Se l'intera operazione di trasferimento va a buon fine, la routine restituisce il valore 1.
    In caso contrario, verrà restituito un valore negativo.
    
    Sono previsti i seguenti codici di errore:
      ERR_USBHANDLE_IS_NOT_ACTIVE    L'handle che è stato fornito non è attivo  
      ERR_IO_ERROR                   Errore di I/O  
      ERR_TIMEOUT                    Timeout in lettura

*/
{
    return ERR_FUNCTION_HAS_BEEN_DISABLED;    
}



int ndHAL_USB_FastCommXChg (int UsbHandle, void *OutCmd, int OutCmdLen, void *InCmd, int InCmdLen, 
                             const void *OutData, int OutLen, void *InData, int InLen, int TimeOut, int Options)
/*
    Questa routine è analoga alla precedente, ma è stata progettata per essere estremamente
    veloce. La routine non controlla che l'handle Usb sia già stato attivato.
    
    Se l'intera operazione di trasferimento va a buon fine, la routine restituisce il valore 1.
    In caso contrario, verrà restituito un valore negativo. L'esatto valore negativo restituito
    può cambiare tra piattaforma e piattaforma, essendo dipendente dal sistema operativo
    su cui sta girando Nanodesktop
*/
{
    return -1;     
}







/*
    ---------------------------------------------------------------------------
    MONITORAGGIO DEL BUS
    ---------------------------------------------------------------------------
*/


void ndHAL_USB_EnableDriverMonitor ()
{   
}

void ndHAL_USB_DisableDriverMonitor ()
{ 
}



