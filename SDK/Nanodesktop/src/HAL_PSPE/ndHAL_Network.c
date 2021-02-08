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
    FUNZIONI DI RETE (PSPE)
    ---------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>





/*
   ---------------------------------------------------------------------------
    API PUBBLICA
   ---------------------------------------------------------------------------
*/


int ndHAL_WLAN_InitNetworkLayer ()
/*
     Questa routine provvede ad inizializzare il layer di rete in user-space. 
     Viene richiamata automaticamente dalle routines di gestione della rete
     in user-space, in modo tale che l'utente non debba preoccuparsi di
     richiamare esplicitamente la routine. 
     
     Se non vi sono errori, la routine restituisce il valore 0.
      
     Altrimenti viene restituito un codice di errore. I codici di errori 
     previsti sono i seguenti:
              
     ERRNET_PSP__NETWORK_HAS_BEEN_DISABLED_BY_SOFT
         L'utente ha disattivato il supporto di rete per mezzo della costante
         manifesta ND_DISABLE_NETWORK_SERVICES
         
     ERRNET_PSP__NETWORK_IS_FAILURE 
         La rete ha un qualche errore (in genere, è nel caricamento dei 
         moduli all'avvio)    
         
     ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER
         Il processo di inizializzazione è fallito. In questo caso viene inserito
         un codice di errore in ndNetInit_Failure_MainErrCode ed un 
         codice di errore secondario in ndNetInit_Failure_SecErrCode                                        
*/
{
     return ERR_FUNCTION_NOT_SUPPORTED;      
}

int ndHAL_WLAN_GetAccessPointInfo (int NrAccessPoint, struct ndNetworkObject_Type *NetObj)
/*
     Questa routine consente di ottenere le informazioni di configurazione relative
     ad un determinato profilo per gli access point, memorizzato nel firmware della
     PSP. I dati vengono memorizzati in formato stringa dentro la struttura puntata
     dal puntatore NetObj
     
     Se tutto è corretto, viene restituito in uscita il valore 0, mentre se si
     verifica un errore, viene restituito in uscita un codice a 32 bit. 
     
     Sono previsti i seguenti codici di errore:
     
     ERRNET_PSP__NETWORK_HAS_BEEN_DISABLED_BY_SOFT
         L'utente ha disattivato il supporto di rete per mezzo della costante
         manifesta ND_DISABLE_NETWORK_SERVICES
         
     ERRNET_PSP__NETWORK_IS_FAILURE 
         La rete ha un qualche errore (in genere, è nel caricamento dei 
         moduli all'avvio)  
         
     ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER
         Il processo di inizializzazione è fallito. In questo caso viene inserito
         un codice di errore in ndNetInit_Failure_MainErrCode ed un 
         codice di errore secondario in ndNetInit_Failure_SecErrCode    
    
    ERRNET_PSP__APCTL_CONFIG_NOT_EXIST
         Il profilo di configurazione APCTL indicato da NrAccessPoint non esiste
         nella memoria del firmware della PSP                             
*/
{
     return ERR_FUNCTION_NOT_SUPPORTED;      
}





int ndHAL_WLAN_PSPLanSwitchIsOpen ()
/*
    Questa funzione restituisce 1 se l'interruttore LAN della PSP è
    aperto, altrimenti restituisce 0
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;  
}


int ndHAL_WLAN_OpenPSPAccessPoint (char NrAccessPoint, struct ndNetworkObject_Type *NetObj, ndint64 Options, int TimeOut)
/*
    Questa routine provvede ad attivare un access point che sia già stato definito 
    nel firmware della PSP. Se l'operazione va a buon fine, il risultato sarà un
    oggetto di tipo ndNetworkObject che potrà essere passato alla routine
    apposita per la creazione del socket di comunicazione.
    
    L'utente deve solo passare il numero di configurazione dell'access point nel
    profilo della PSP: il primo profilo ha codice 1. 
    
    Il parametro a 64 bit Options serve a passare alcune opzioni alla routine:
    sono previste le seguenti costanti manifeste 

    ND_SILENT_AP_ACCESS       Il sistema non fornisce all'utente un feedback visivo
                              del processo di connessione. 
                              
    Il parametro TimeOut specifica il tempo massimo consentito per la connessione
    in sec: se si imposta a 0 non viene eseguito alcun controllo. 
    
    Se tutto va bene, la routine prepara la struttura NetObj, che deve essere
    stata allocata in memoria prima di invocare la routine (il secondo parametro
    è appunto un puntatore a tale struttura), ed in uscita viene restituito
    il valore 0. 
    
    Se invece si verifica un errore, il sistema in uscita riporterà un codice
    di errore ERRNET a 32 bit che indicherà il tipo di condizione di errore
    che si è verificata. 
    
    Sono previsti i seguenti codici di errore:
    
    ERRNET_PSP__NETWORK_HAS_BEEN_DISABLED_BY_SOFT
         L'utente ha disattivato il supporto di rete per mezzo della costante
         manifesta ND_DISABLE_NETWORK_SERVICES
         
    ERRNET_PSP__NETWORK_IS_FAILURE 
         La rete ha un qualche errore (in genere, è nel caricamento dei 
         moduli all'avvio)  
         
    ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER
         Il processo di inizializzazione è fallito. In questo caso viene inserito
         un codice di errore in ndNetInit_Failure_MainErrCode ed un 
         codice di errore secondario in ndNetInit_Failure_SecErrCode    
         
    ERRNET_PSP__FAILURE_IN_APCTL_CONNECT
         Si è verificato un errore in sceNetApctlConnect. In questo caso viene inserito
         questo codice di errore in ndNetInit_Failure_MainErrCode ed un 
         codice di errore secondario in ndNetInit_Failure_SecErrCode 
         
    ERRNET_PSP__TIMEOUT
         Timeout per la connessione
         
    ERRNET_PSP__LAN_SWITCH_IS_OFF   
         Lo switch di rete della PSP è chiuso                                                     
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;  
}

int ndHAL_WLAN_ClosePSPAccessPoint (char NrAccessPoint)
/*
    Questa routine consente di disattivare un access point
    
    Se non vi sono errori, la routine restituisce 0, altrimenti si ottiene
    un codice di errore ErrNet a 32 bit. Sono previsti i seguenti codici
    di errore:
              
    ERRNET_PSP__FAILURE_IN_APCTL_DISCONNECT 
    Errore nella disconnessione dall'access point. In ndNetInit_Failure_SecErrCode 
    è presente il codice di errore secondario fornito dal firmware. 
*/

{
    return ERR_FUNCTION_NOT_SUPPORTED;  
}






