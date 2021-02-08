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
    FUNZIONI DI GESTIONE DI RETE A BASSO LIVELLO (PSP CFW MODE)
    ---------------------------------------------------------------------------
 */
 
#include <HAL_CFW_PSP\$_HAL_NDforND.h>




/* 
   --------------------------------------------------------------------------
   FUNZIONI DI GESTIONE DELL'ICONA NETWORK
   --------------------------------------------------------------------------
*/

void INTERNAL_DrawNetworkIcon (TypeColor Colore, char Page)
/*
    Questa routine si occupa di disegnare sullo schermo l'icona USB
*/
{
    register unsigned short int NetworkIconX, NetworkIconY;
    register unsigned short int CounterX, CounterY;
    
    NetworkIconX = MinWindowsBar_x2 + 6;
    NetworkIconY = MinWindowsBar_y1 + 1;
    
    for (CounterY=0; CounterY<=10; CounterY++)
    {
        for (CounterX=0; CounterX<40; CounterX++)
        {
            if (TheNetworkIcon [CounterY][CounterX]=='X') 
                 ndHAL_PutPixelToPage (NetworkIconX+CounterX, NetworkIconY+CounterY, Colore, Page);
        }
    }
}




void INTERNAL_ManagerNetworkIcon (char Page)
/*
    Questa routine viene richiamata periodicamente da WindowsRender. 
    
    Essa si occupa di disegnare, ove necessario, l'icona di rete nella 
    MinWindowsBar, indicando lo stato del collegamento wireless
*/
{
    TypeColor Colore;
    
    if (!ND_HWMODE_DisableNetworkService)     // Disegna qualcosa solo se il wireless non è stato disattivato
    {                                         
              if (ndNetInit_IsFailure)
              {
                   Colore=COLOR_RED;
              }
              else
              {
                   if (ndNrNetworkConnectionsActive>0)
                   {
                       Colore=COLOR_GREEN;
                   }
                   else
                   {
                       Colore=COLOR_GRAY04;
                   }
              }
              
              INTERNAL_DrawNetworkIcon (Colore, Page);
              
              ndNetworkStateIsChanged = 0;
              // Farà sì che il prossimo WindowsRender non debba ridisegnare l'icona se questa non è cambiata                            
    } 
}


void INTERNAL_ChangeStateNetIcon_AddConnection ()
{
    ndNrNetworkConnectionsActive++;
    ndNetworkStateIsChanged=1;
    
    WindowsRender(); 
}

void INTERNAL_ChangeStateNetIcon_SubConnection ()
{
    ndNrNetworkConnectionsActive--;
    ndNetworkStateIsChanged=1;
    
    WindowsRender(); 
}



/* 
   --------------------------------------------------------------------------
   FUNZIONI DI INIZIALIZZAZIONE/DEINIZIALIZZAZIONE SPECIFICHE PER
   IL FIRMWARE
   --------------------------------------------------------------------------
*/



void INTERNAL_CFFW3XX_InitLowLevelNetwork (void)
/*
     Questa routine provvede a caricare i drivers a basso livello per la gestione
     della rete da parte della PSP. 
     
     E' usata dai custom firmware a partire dalla versione 3.00.
    
     Provvede inoltre alla reinizializzazione del sistema WLAN 
*/
{
     int FirmErrRep;
     
     FirmErrRep = sceUtilityLoadNetModule (PSP_NET_MODULE_COMMON);
     
     if (FirmErrRep<0)
     {
          ndNetInit_IsFailure = 1;
          ndNetInit_Failure_MainErrCode = ERRNET_PSP__GENERIC_FAILURE_IN_LOAD_CFMOD0;
          ndNetInit_Failure_SecErrCode  = FirmErrRep;
          
          return;                      
     }
     
     FirmErrRep = sceUtilityLoadNetModule (PSP_NET_MODULE_INET);
     
     if (FirmErrRep<0)
     {
          ndNetInit_IsFailure = 1;
          ndNetInit_Failure_MainErrCode = ERRNET_PSP__GENERIC_FAILURE_IN_LOAD_CFMOD1;
          ndNetInit_Failure_SecErrCode  = FirmErrRep;
          
          return;                      
     }                
     
     FirmErrRep = sceNetInit(0x20000, 0x20, 0x1000, 0x20, 0x1000);
     if (FirmErrRep != 0)
     {
         ndNetInit_IsFailure = 2;
         ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_INIT_SDK_NET;
         ndNetInit_Failure_SecErrCode  = FirmErrRep;     
     
         return;
     }
     
     FirmErrRep = sceNetInetInit();
     
     if (FirmErrRep != 0)
     {
         ndNetInit_IsFailure = 2;
         ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_INIT_SDK_NETINET;
         ndNetInit_Failure_SecErrCode  = FirmErrRep;     
     
         return;
     }
     
     FirmErrRep = sceNetResolverInit();
     if (FirmErrRep != 0)
     {
         ndNetInit_IsFailure = 2;
         ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_INIT_SDK_RESOLVER;
         ndNetInit_Failure_SecErrCode  = FirmErrRep;     
     
         return;
     }
     
     
     FirmErrRep = sceNetApctlInit(0x1400, 0x42);
     if (FirmErrRep != 0)
     {
         ndNetInit_IsFailure = 2;
         ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_INIT_SDK_APCTL;
         ndNetInit_Failure_SecErrCode  = FirmErrRep;     
     
         return;
     }

     
     // Se il sistema è arrivato qui significa che non ci sono errori

     ndNetInit_IsFailure = 0;
     ndNetInit_Failure_MainErrCode = 0;
     ndNetInit_Failure_SecErrCode  = 0;
     
     // Monitor di rete inizialmente disattivato
     
     ndNetMonitorEnabled = 0;  
     
     return;                
}

void INTERNAL_CFFW3XX_TerminateLowLevelNetwork (void)
/*
     Questa routine provvede ad arrestare tutti i drivers di rete che sono
     in quel momento in funzione. E' specifica per i custom firmware
     versione 3.XX
*/
{
    sceNetApctlTerm();
    sceNetResolverTerm();
    sceNetInetTerm();
    sceNetTerm();
    
    sceUtilityUnloadNetModule (PSP_NET_MODULE_INET);
    sceUtilityUnloadNetModule (PSP_NET_MODULE_COMMON);

    return; 
}



/* 
   --------------------------------------------------------------------------
   FUNZIONI DI INIZIALIZZAZIONE A LIVELLO INTERMEDIO
   --------------------------------------------------------------------------
*/



void ndHAL_InitLowLevelNetwork (void)
/*
     Questa routine provvede a caricare i drivers a basso livello per la gestione
     della rete da parte della PSP
    
     Inoltre, essa provvede a reinizializzare il sistema WLAN della PSP. 
     
     L'operazione di loading viene eseguita una sola volta all'avvio 
     dell'homebrew: Nanodesktop verifica la versione del firmware presente
     e provvede a richiamare la corretta routine di inizializzazione 
     
     Se l'operazione riesce, la variabile ndNetInit_IsFailure viene posta a 0. 
     
     Se invece l'operazione fallisce, il sistema pone la variabile ndNetInit_IsFailure
     ad 1, e deposita il codice di errore dentro le variabili di sistema 
     ndNetInit_Failure_MainErrCode e ndNetInit_Failure_SecErrCode. 
     
     La variabile ndNetInit_Failure_MainErrCode assume valori che sono stabiliti da
     una serie di costanti simboliche, stabilite in $$_Errors.h. 
     
     Il significato di ndNetInit_Failure_SecErrCode dipende dal codice di errore
     principale.
     
     Questi sono i codici di errore principali riconosciuti:
     
     ERRNET_PSP__GENERIC_FAILURE_IN_LOAD_CFMOD0 
     ERRNET_PSP__GENERIC_FAILURE_IN_LOAD_CFMOD1
        Il sistema non è riuscito a caricare i moduli del firmware che permettono
        di reinizializzare la rete. Il codice di errore secondario è il codice
        di errore restituito dal firmware PSP. Si noti che questi codici di
        errore sono diversi da quelli restituiti nel caso del firmware 1.50
        
     ERRNET_PSP__FIRMWARE_IS_NOT_SUPPORTED_FOR_NET
        La routine di inizializzazione di Nanodesktop, su alcuni firmware può
        dare problemi così gravi da determinare una eccezione del sistema
        (con uscita del programma e ritorno alla dashboard all'avvio). 
        
        Questa situazione non si verificherebbe solo per i programmi di rete:
        poichè l'inizializzazione del sistema con ndInitSystem() implica  
        anche l'inizializzazione della rete (salvo che il supporto di rete
        non sia stato preventivamente disattivato), questa condizione 
        determinerebbe l'impossibilità di usare applicazioni Nanodesktop 
        su svariati firmware. Per ovviare a tale inconveniente, Nanodesktop
        controlla all'avvio quale firmware è attualmente installato sulla 
        PSP: se esso non è la versione 1.5 o una versione supportata, 
        Nanodesktop disattiva automaticamente il supporto di rete  
      
     
     Sui custom firmware, Nanodesktop esegue anche subito l'operazione di
     reinizializzazione dei layer di rete. Nell'HAL KSU, questo invece veniva
     fatto in user-mode, da ndHAL_WLAN_InitNetworkLayer. La ragione è 
     tecnica: per una qualche ragione che non è stato possibile verificare, è
     necessario che la rete sia reinizializzata prima del caricamento del
     kernel extender, altrimenti, il sistema restituirà un codice 80020190
     (out of memory)
     
     Se il processo di inizializzazione di questo layer di rete fallisce, allora
     Nanodesktop pone la variabile ndNetInit_IsFailure ad 2, e 
     registra il codice di errore dentro le variabili di sistema 
     ndNetInit_Failure_MainErrCode e ndNetInit_Failure_SecErrCode. 
        
     Questi sono i codici di errore riconosciuti   
        
     ERRNET_PSP__FAILED_TO_INIT_SDK_NET
     ERRNET_PSP__FAILED_TO_INIT_SDK_NETINET
     ERRNET_PSP__FAILED_TO_INIT_SDK_RESOLVER
     ERRNET_PSP__FAILED_TO_INIT_SDK_APCTL
        Indicano errori nella fase di inizializzazione dei layer di rete che
        normalmente si trovano in user mode
*/
{
     int ClassFirmCode;
     
     if (!ND_HWMODE_DisableNetworkService)
     {
         INTERNAL_CFFW3XX_InitLowLevelNetwork ();
         return;          
     }
     else  // Il supporto di rete è stato disattivato
     {
         ndNetInit_IsFailure = 0;   // In realtà ignorato perchè la rete qui è disattivata  
     }    
}

void ndHAL_TerminateLowLevelNetwork (void)
/*
     Questa routine provvede ad arrestare tutti i drivers di rete che sono
     in quel momento in funzione.

     E' usata, ad esempio, in fase di arresto del sistema
*/
{
     int ClassFirmCode;
     
     if (!ND_HWMODE_DisableNetworkService)
     {       
         INTERNAL_CFFW3XX_TerminateLowLevelNetwork ();
         return;
     }  
}
