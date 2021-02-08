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
    FUNZIONI DI RETE (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>


/*
   ---------------------------------------------------------------------------
    ROUTINES INTERNE PER FEEDBACK-VIDEO
   ---------------------------------------------------------------------------
*/

char INTERNAL_CreateConnectionDialogBox (char VideoFeedback, struct ndNetworkObject_Type *NetObj)
{
    char WndHandle;
    char WndTitle [255];
    char StringBuffer0 [16];
    struct ndImage_Type _WifiVideo_Image;
    
    if (VideoFeedback)
    {
         strcpy (WndTitle, "Connecting using access point ");
         _itoa (NetObj->Psp.NrConfigInFirmw, StringBuffer0, 10);
         strcat (WndTitle, StringBuffer0);              
                      
         WndHandle = ndLP_CreateWindow (70, Windows_MaxScreenY/2 - 50, Windows_MaxScreenX - 70, Windows_MaxScreenY/2 + 50,
                           WndTitle, COLOR_WHITE, COLOR_BLUE, COLOR_GRAY, COLOR_GRAY, 0); 
                     
         if (WndHandle>=0) 
         {
            ndIMG_LoadImageFromNDFArray (&_WifiVideo_Image, WifiIconWidth, WifiIconHeight, &WifiIcon, NDMGKNB);
            ndIMG_ShowImage (&_WifiVideo_Image, 30, 10, WndHandle, NORENDER);
                       
            NetObj->Psp.WndDialogBox0_Handle  = WndHandle;
            NetObj->Psp.WndDialogBox0_IsOpen  = 1;
            
            NetObj->Psp.BtnHandle0 = ndCTRL_CreateButton (WindowData [WndHandle].WSWndLength - 70, WindowData [WndHandle].WSWndHeight - 20,
                                                          WindowData [WndHandle].WSWndLength - 1,  WindowData [WndHandle].WSWndHeight - 1,
                                                          WndHandle, "BtnOk", "OK", "",
                                                          COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, 
                                                          ND_BUTTON_INHIBITED | ND_BUTTON_ROUNDED, 0, 0, NORENDER);
                                                           
            NetObj->Psp.BtnHandle1 = ndCTRL_CreateButton (WindowData [WndHandle].WSWndLength - 150, WindowData [WndHandle].WSWndHeight - 20,
                                                          WindowData [WndHandle].WSWndLength - 80,  WindowData [WndHandle].WSWndHeight - 1,
                                                          WndHandle, "BtnDetail", "Details", "",
                                                          COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, 
                                                          ND_BUTTON_INHIBITED | ND_BUTTON_ROUNDED, 0, 0, NORENDER);
                                         
            ndLP_MaximizeWindow (WndHandle);
            return 0;         // Nessun errore. Ritorna 0
         } 
         else return -1;
    }
    else
      return 0;       // Nessun errore se il feedback video è disattivato
}

char INTERNAL_WaitingForAPConnect (char VideoFeedback, struct ndNetworkObject_Type *NetObj)
{
    char WndHandle;
   
    WndHandle=NetObj->Psp.WndDialogBox0_Handle;
    
    if (VideoFeedback)
    {
        ndWS_DrawRectangle (85, 15, WindowData [WndHandle].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrWriteLn (90, 25, "Connecting to access point", COLOR_BLACK, COLOR_GRAY, WndHandle, NORENDER);
        XWindowRender (WndHandle);
        
        return 0;
    }
    else
      return 0;       // Nessun errore se il feedback video è disattivato 
}


char INTERNAL_SignalConnectionStage (char Stage, char VideoFeedback, struct ndNetworkObject_Type *NetObj)
{
    char WndHandle;
    
    WndHandle=NetObj->Psp.WndDialogBox0_Handle;
    
    if (VideoFeedback)
    { 
        ndWS_DrawRectangle (85, 15, WindowData [WndHandle].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrWriteLn (90, 25, "Connection in progress", COLOR_BLACK, COLOR_GRAY, WndHandle, NORENDER);
        
        ndWS_DrawRectangle (90, 40, 90+200, 45, COLOR_BLUE, COLOR_BLACK, WndHandle, NORENDER);
        ndWS_DrawRectangle (90, 40, 90+50*Stage, 45, COLOR_YELLOW, COLOR_BLACK, WndHandle, NORENDER);
     
        XWindowRender (WndHandle);
        
        return 0;
    }
    else
      return 0;       // Nessun errore se il feedback video è disattivato 
}


static ndint64 INTERNAL_cbConnectionSuccess_OK (char *StringID, ndint64 InfoField, char WndHandle)
{
    struct ndNetworkObject_Type *NetObj = (void *)(InfoField);
    // Recupera le informazioni di struttura dal sistema
    
    NetObj->Psp.Ok_Switcher=1;
    // Determinerà l'uscita dalla dialog box
    
    return 0;
}

static ndint64 INTERNAL_cbConnectionSuccess_DETAILS_CWC (char WndHandle, ndint64 InfoField)
{
    struct ndNetworkObject_Type *NetObj = (void *)(InfoField);
    // Recupera le informazioni di struttura dal sistema
    
    if (NetObj->Psp.WndDialogBox1_IsOpen)
    {
        ndLP_DestroyWindow (WndHandle);
        NetObj->Psp.WndDialogBox1_IsOpen=0;
    }    
    
    return 0;
}

static ndint64 INTERNAL_cbConnectionSuccess_DETAILS (char *StringID, ndint64 InfoField, char WndHandle)
{
    struct ndNetworkObject_Type *NetObj = (void *)(InfoField);
    // Recupera le informazioni di struttura dal sistema
 
    char InternalBuffer [128];   
    char DialogBoxHandle;
    
    if (!NetObj->Psp.WndDialogBox1_IsOpen)
    {                                    
            DialogBoxHandle = ndLP_CreateWindow (10, Windows_MaxScreenY-80, Windows_MaxScreenX-1, Windows_MaxScreenY - 1,
                                   "Connection details", COLOR_WHITE, COLOR_RED, COLOR_BLACK, COLOR_GRAY, 
                                   NDKEY_SETWSLENGTH (240) | NDKEY_SETWSHEIGHT (200) ); 
                             
            if (DialogBoxHandle>=0) 
            {
                NetObj->Psp.WndDialogBox1_Handle  = DialogBoxHandle;
                NetObj->Psp.WndDialogBox1_IsOpen  = 1;
                
                ndLP_MaximizeWindow (DialogBoxHandle);
                ndLP_SetupCloseWndCallback (&INTERNAL_cbConnectionSuccess_DETAILS_CWC, InfoField, 1, DialogBoxHandle);
                
                ndWS_GrPrintLn (5, 8,  COLOR_WHITE, COLOR_BLACK, DialogBoxHandle, NORENDER, "Informations:");
                
                ndWS_GrPrintLn (5, 24, COLOR_LBLUE , COLOR_BLACK, DialogBoxHandle, NORENDER, "Name    : %s",  NetObj->Psp.Name);
                ndWS_GrPrintLn (5, 32, COLOR_YELLOW, COLOR_BLACK, DialogBoxHandle, NORENDER, "SSID    : %s",  NetObj->Psp.SSID);
                ndWS_GrPrintLn (5, 40, COLOR_LBLUE,  COLOR_BLACK, DialogBoxHandle, NORENDER, "SECURE  : %s",  NetObj->Psp.SECURE);
                ndWS_GrPrintLn (5, 48, COLOR_YELLOW, COLOR_BLACK, DialogBoxHandle, NORENDER, "WEPKEY  : %s",  NetObj->Psp.WEPKEY);
                
                ndWS_GrPrintLn (5, 56, COLOR_LBLUE,  COLOR_BLACK, DialogBoxHandle, NORENDER, "STATICIP: %d ", NetObj->Psp.IS_STATIC_IP);
                
                ndHAL_NET_ConvertIPv4ToString ( &(NetObj->Psp.IP), &InternalBuffer );
                ndWS_GrPrintLn (5, 64, COLOR_YELLOW, COLOR_BLACK, DialogBoxHandle, NORENDER, "IP      : %s ", InternalBuffer);
                
                ndHAL_NET_ConvertIPv4ToString ( &(NetObj->Psp.NETMASK), &InternalBuffer );
                ndWS_GrPrintLn (5, 72, COLOR_LBLUE,  COLOR_BLACK, DialogBoxHandle, NORENDER, "NETMASK : %s ", InternalBuffer);
               
                ndHAL_NET_ConvertIPv4ToString ( &(NetObj->Psp.ROUTE), &InternalBuffer );
                ndWS_GrPrintLn (5, 88, COLOR_YELLOW, COLOR_BLACK, DialogBoxHandle, NORENDER, "Router  : %s ", InternalBuffer);
               
                ndHAL_NET_ConvertIPv4ToString ( &(NetObj->Psp.PRIMARY_DNS), &InternalBuffer );
                ndWS_GrPrintLn (5, 96, COLOR_LBLUE,  COLOR_BLACK, DialogBoxHandle, NORENDER, "PrimaryDNS  : %s ", InternalBuffer);
                
                ndHAL_NET_ConvertIPv4ToString ( &(NetObj->Psp.SECONDARY_DNS), &InternalBuffer );
                ndWS_GrPrintLn (5,104, COLOR_YELLOW, COLOR_BLACK, DialogBoxHandle, NORENDER, "SecondaryDNS: %s", InternalBuffer);
               
                ndWS_GrPrintLn (5,112, COLOR_LBLUE,  COLOR_BLACK, DialogBoxHandle, NORENDER, "PROXY_USER  : %s",  NetObj->Psp.PROXY_USER);
                ndWS_GrPrintLn (5,120, COLOR_YELLOW, COLOR_BLACK, DialogBoxHandle, NORENDER, "PROXY_PASS  : %s",  NetObj->Psp.PROXY_PASS);
                
                ndWS_GrPrintLn (5,136, COLOR_LBLUE,  COLOR_BLACK, DialogBoxHandle, NORENDER, "Proxy enabled: %d",  NetObj->Psp.USE_PROXY);
                
                ndHAL_NET_ConvertIPv4ToString ( &(NetObj->Psp.PROXY_SERVER), &InternalBuffer );
                ndWS_GrPrintLn (5,144, COLOR_YELLOW, COLOR_BLACK, DialogBoxHandle, NORENDER, "Proxy server : %s", InternalBuffer);
               
                ndWS_GrPrintLn (5,152, COLOR_LBLUE,  COLOR_BLACK, DialogBoxHandle, NORENDER, "Proxy port   : %d",  NetObj->Psp.PROXY_PORT);
               
                if (NetObj->IPIsKnown)
                {
                    ndHAL_NET_ConvertIPv4ToString ( &(NetObj->IPV4), &InternalBuffer );
                    ndWS_GrPrintLn (5,168, COLOR_RED, COLOR_BLACK, DialogBoxHandle, NORENDER, "IP assigned by server: %s", InternalBuffer);                         
                }            
                else
                {
                    ndWS_GrPrintLn (5,168, COLOR_RED, COLOR_BLACK, DialogBoxHandle, NORENDER, "IP assigned is unknown");            
                }
            
                XWindowRender (DialogBoxHandle);
            }
    }  
    
    return 0;           
}

char INTERNAL_SignalConnectionSuccess (char VideoFeedback, struct ndNetworkObject_Type *NetObj)
{
    char WndHandle;
    
    WndHandle=NetObj->Psp.WndDialogBox0_Handle;
    
    if (VideoFeedback)
    { 
        ndWS_DrawRectangle (85, 15, WindowData [WndHandle].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrWriteLn (90, 25, "Connection successful", COLOR_BLACK, COLOR_GRAY, WndHandle, NORENDER);
        
        XWindowRender (WndHandle);
        
        ndCTRL_DeInhibitButton (NetObj->Psp.BtnHandle0, WndHandle, RENDER);
        ndCTRL_ChangeCallBack (&INTERNAL_cbConnectionSuccess_OK, NetObj, NetObj->Psp.BtnHandle0, WndHandle, 0);
        
        ndCTRL_DeInhibitButton (NetObj->Psp.BtnHandle1, WndHandle, RENDER);
        ndCTRL_ChangeCallBack (&INTERNAL_cbConnectionSuccess_DETAILS, NetObj, NetObj->Psp.BtnHandle1, WndHandle, 0);
        
        NetObj->Psp.Ok_Switcher=0;
        ndProvideMeTheMouse_Until (&(NetObj->Psp.Ok_Switcher), ND_IS_EQUAL, 1, 0, 0);
        // Bloccherà il processo fin quando non verrà premuto il tasto ok
       
        if (NetObj->Psp.WndDialogBox1_IsOpen) ndLP_DestroyWindow (NetObj->Psp.WndDialogBox1_Handle);
        ndLP_DestroyWindow (WndHandle);
         
        return 0;
    }
    else
      return 0;       // Nessun errore se il feedback video è disattivato 
}


// Apctl

static ndint64 INTERNAL_cbFailureApctl_OK (char *StringID, ndint64 InfoField, char WndHandle)
{
    struct ndNetworkObject_Type *NetObj = (void *)(InfoField);
    // Recupera le informazioni di struttura dal sistema
    
    NetObj->Psp.Ok_Switcher=1;
    // Determinerà l'uscita dalla dialog box
    
    return 0;
}


char INTERNAL_SignalFailureApctl (char VideoFeedback, int fwErrRep, struct ndNetworkObject_Type *NetObj)
{
    char WndHandle;
    
    WndHandle=NetObj->Psp.WndDialogBox0_Handle;
    
    if (VideoFeedback)
    { 
        ndWS_DrawRectangle (85, 15, WindowData [WndHandle].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrWriteLn (90, 20, "Error in sceNetApctlConnect", COLOR_RED, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrPrintLn (90, 30, COLOR_RED, COLOR_GRAY, WndHandle, NORENDER, "Firmware code: %X", fwErrRep);
        
        XWindowRender (WndHandle);
        
        ndCTRL_DeInhibitButton (NetObj->Psp.BtnHandle0, WndHandle, RENDER);
        ndCTRL_ChangeCallBack (&INTERNAL_cbFailureApctl_OK, NetObj, NetObj->Psp.BtnHandle0, WndHandle, 0);
        
        NetObj->Psp.Ok_Switcher=0;
        ndProvideMeTheMouse_Until (&(NetObj->Psp.Ok_Switcher), ND_IS_EQUAL, 1, 0, 0);
        // Bloccherà il processo fin quando non verrà premuto il tasto ok
       
        ndLP_DestroyWindow (WndHandle); 
        return 0;
    }
    else
      return 0;       // Nessun errore se il feedback video è disattivato 
}




// AplctlGetState

static ndint64 INTERNAL_cbFailureApctlGetState_OK (char *StringID, ndint64 InfoField, char WndHandle)
{
    struct ndNetworkObject_Type *NetObj = (void *)(InfoField);
    // Recupera le informazioni di struttura dal sistema
    
    NetObj->Psp.Ok_Switcher=1;
    // Determinerà l'uscita dalla dialog box
    
    return 0;
}


char INTERNAL_SignalFailureApctlGetState (char VideoFeedback, int fwErrRep, struct ndNetworkObject_Type *NetObj)
{
    char WndHandle;
    char StringBuffer0 [255];
    char StringBuffer1 [255];
    
    WndHandle=NetObj->Psp.WndDialogBox0_Handle;
    
    if (VideoFeedback)
    { 
        ndWS_DrawRectangle (85, 15, WindowData [WndHandle].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrWriteLn (90, 20, "Error in sceNetApctlGetState", COLOR_RED, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrPrintLn (90, 30, COLOR_RED, COLOR_GRAY, WndHandle, NORENDER, "Firmware code: %X", fwErrRep);
        
        XWindowRender (WndHandle);
        
        ndCTRL_DeInhibitButton (NetObj->Psp.BtnHandle0, WndHandle, RENDER);
        ndCTRL_ChangeCallBack (&INTERNAL_cbFailureApctlGetState_OK, NetObj, NetObj->Psp.BtnHandle0, WndHandle, 0);
        
        NetObj->Psp.Ok_Switcher=0;
        ndProvideMeTheMouse_Until (&(NetObj->Psp.Ok_Switcher), ND_IS_EQUAL, 1, 0, 0);
        // Bloccherà il processo fin quando non verrà premuto il tasto ok
       
        ndLP_DestroyWindow (WndHandle); 
        return 0;
    }
    else
      return 0;       // Nessun errore se il feedback video è disattivato 
}


// Timeout

char INTERNAL_SignalTimeOut (char VideoFeedback, struct ndNetworkObject_Type *NetObj)
{
    char WndHandle;
    
    WndHandle=NetObj->Psp.WndDialogBox0_Handle;
    
    if (VideoFeedback)
    {      
        ndWS_DrawRectangle (85, 15, WindowData [WndHandle].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrWriteLn (90, 25, "TimeOut", COLOR_RED, COLOR_GRAY, WndHandle, NORENDER);
        
        XWindowRender (WndHandle);
        
        ndCTRL_DeInhibitButton (NetObj->Psp.BtnHandle0, WndHandle, RENDER);
        ndCTRL_ChangeCallBack (&INTERNAL_cbFailureApctl_OK, NetObj, NetObj->Psp.BtnHandle0, WndHandle, 0);
        
        NetObj->Psp.Ok_Switcher=0;
        ndProvideMeTheMouse_Until (&(NetObj->Psp.Ok_Switcher), ND_IS_EQUAL, 1, 0, 0);
        // Bloccherà il processo fin quando non verrà premuto il tasto ok
       
        ndLP_DestroyWindow (WndHandle); 
        return 0;
    }
    else
      return 0;       // Nessun errore se il feedback video è disattivato 
}


// Lan Switch is off

char INTERNAL_LanSwitchIsOff (char VideoFeedback, struct ndNetworkObject_Type *NetObj)
{
    char WndHandle;
    
    WndHandle=NetObj->Psp.WndDialogBox0_Handle;
    
    if (VideoFeedback)
    {      
        ndWS_DrawRectangle (85, 15, WindowData [WndHandle].WSWndLength - 10, 45, COLOR_GRAY, COLOR_GRAY, WndHandle, NORENDER);
        ndWS_GrWriteLn (90, 25, "WI-FI switch is off", COLOR_RED, COLOR_GRAY, WndHandle, NORENDER);
        
        XWindowRender (WndHandle);
        
        ndCTRL_DeInhibitButton (NetObj->Psp.BtnHandle0, WndHandle, RENDER);
        ndCTRL_ChangeCallBack (&INTERNAL_cbFailureApctl_OK, NetObj, NetObj->Psp.BtnHandle0, WndHandle, 0);
        
        NetObj->Psp.Ok_Switcher=0;
        ndProvideMeTheMouse_Until (&(NetObj->Psp.Ok_Switcher), ND_IS_EQUAL, 1, 0, 0);
        // Bloccherà il processo fin quando non verrà premuto il tasto ok
       
        ndLP_DestroyWindow (WndHandle); 
        return 0;
    }
    else
      return 0;       // Nessun errore se il feedback video è disattivato 
}



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
    int ErrRep;
    
    if (!ND_HWMODE_DisableNetworkService)
    {
          if (!ndNetInit_IsFailure)
          {
                   if (!ndNetLayer_HasBeenInit)
                   {
                         ErrRep = sceNetInit(0x20000, 0x20, 0x1000, 0x20, 0x1000);
                         if (ErrRep != 0)
                         {
                             ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_INIT_SDK_NET;
                             ndNetInit_Failure_SecErrCode  = ErrRep;     
                         
                             return ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER;
                         }
        
                         
                         ErrRep = sceNetInetInit();
                         if (ErrRep != 0)
                         {
                             ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_INIT_SDK_NETINET;
                             ndNetInit_Failure_SecErrCode  = ErrRep;     
                         
                             return ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER;
                         }
        
        
                         ErrRep = sceNetResolverInit();
                         if (ErrRep != 0)
                         {
                             ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_INIT_SDK_RESOLVER;
                             ndNetInit_Failure_SecErrCode  = ErrRep;     
                         
                             return ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER;
                         }
        
        
                         ErrRep = sceNetApctlInit(0x1400, 0x42);
                         if (ErrRep != 0)
                         {
                             ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_INIT_SDK_APCTL;
                             ndNetInit_Failure_SecErrCode  = ErrRep;     
                         
                             return ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER;
                         }
        
                         // Se il sistema giunge a questo punto, allora vuol dire che
                         // il processo di inizializzazione è corretto
        
                         ndNetLayer_HasBeenInit = 1;
                         return 0;   
                   }
                   else  // Il sistema già era stato reinizializzato
                   {
                         return 0;     // Simula l'assenza di errori  
                   }                         
          }
          else     // C'è qualche errore di rete (nella inizializzazione hardware)
          {
               return ERRNET_PSP__NETWORK_IS_FAILURE;     
          }  
    }
    else    // Il supporto network è stato disattivato via software
    {
        return ERRNET_PSP__NETWORK_HAS_BEEN_DISABLED_BY_SOFT;    
    }
          
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
   static unsigned char InternalBuffer [128];
   
   if (!ND_HWMODE_DisableNetworkService)
   {
          if (!ndNetInit_IsFailure)
          {
               if ( !ndHAL_WLAN_InitNetworkLayer() )
               {
                    if (!sceUtilityCheckNetParam(NrAccessPoint))   // L'access point richiesto esiste
                    {
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_NAME, &InternalBuffer);
                               strcpy (NetObj->Psp.Name, InternalBuffer);
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_SSID, &InternalBuffer);
                               strcpy (NetObj->Psp.SSID, InternalBuffer);
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_SECURE, &InternalBuffer);
                               strcpy (NetObj->Psp.SECURE, InternalBuffer); 
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_WEPKEY, &InternalBuffer);
                               strcpy (NetObj->Psp.WEPKEY, InternalBuffer); 
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_IS_STATIC_IP, &InternalBuffer);
                               NetObj->Psp.IS_STATIC_IP = *(char *)(&InternalBuffer); 
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_IP, &InternalBuffer);
                               ndHAL_NET_ConvertStringToIPv4 ( &InternalBuffer, &(NetObj->Psp.IP) );
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_NETMASK, &InternalBuffer);
                               ndHAL_NET_ConvertStringToIPv4 ( &InternalBuffer, &(NetObj->Psp.NETMASK) );
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_ROUTE, &InternalBuffer);
                               ndHAL_NET_ConvertStringToIPv4 ( &InternalBuffer, &(NetObj->Psp.ROUTE) );
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_PRIMARYDNS, &InternalBuffer);
                               ndHAL_NET_ConvertStringToIPv4 ( &InternalBuffer, &(NetObj->Psp.PRIMARY_DNS) );
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_SECONDARYDNS, &InternalBuffer);
                               ndHAL_NET_ConvertStringToIPv4 ( &InternalBuffer, &(NetObj->Psp.SECONDARY_DNS) );
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_PROXY_USER, &InternalBuffer);
                               strcpy (NetObj->Psp.PROXY_USER, InternalBuffer); 
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_PROXY_PASS, &InternalBuffer);
                               strcpy (NetObj->Psp.PROXY_PASS, InternalBuffer); 
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_USE_PROXY, &InternalBuffer);
                               NetObj->Psp.USE_PROXY = *(unsigned char *)(&InternalBuffer); 
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_PROXY_SERVER, &InternalBuffer);
                               ndHAL_NET_ConvertStringToIPv4 ( &InternalBuffer, &(NetObj->Psp.PROXY_SERVER) );
                               
                               sceUtilityGetNetParam (NrAccessPoint, PSP_NETPARAM_PROXY_PORT, &InternalBuffer);
                               NetObj->Psp.PROXY_PORT = *(unsigned short int *)(&InternalBuffer);
                               
                               // Ora cerca di determinare l'indirizzo IP che è stato assegnato
                               // dalla rete wireless
                               
                               if (sceNetApctlGetInfo ( 8, &(NetObj->IPV4str) )==0)   // Ha ricavato un IP statico
                               {   
                                  ndHAL_NET_ConvertStringToIPv4 ( &(NetObj->IPV4str), &(NetObj->IPV4) );
                                  NetObj->IPIsKnown=1;
                               }
                               else
                               {
                                  NetObj->IPV4.Code32 = 0;
                                  NetObj->IPIsKnown=0;
                               }
                               
                               return 0; // Nessun errore                                                  
                    }         
                    else // La configurazione per l'access point richiesto non esiste
                    {
                         return ERRNET_PSP__APCTL_CONFIG_NOT_EXIST;
                    }
               }
               else
               {
                   return ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER;
               }                    
          }
          else     // C'è qualche errore di rete (nella inizializzazione hardware)
          {
               return ERRNET_PSP__NETWORK_IS_FAILURE;     
          }  
    }
    else    // Il supporto network è stato disattivato via software
    {
        return ERRNET_PSP__NETWORK_HAS_BEEN_DISABLED_BY_SOFT;    
    }    
}





int ndHAL_WLAN_PSPLanSwitchIsOpen ()
/*
    Questa funzione restituisce 1 se l'interruttore LAN della PSP è
    aperto, altrimenti restituisce 0
*/
{
    return ( sceWlanGetSwitchState() );
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
    int  fwErrRep, ErrRep, State;
    char VideoFeedback;
    
    int  LibcCodeBegin, LibcCodeEnd;
    char ConnectionOk;
    
    // Setta al valore corretto il flag VideoFeedback
    
    if ((Options & ND_SILENT_AP_ACCESS)==ND_SILENT_AP_ACCESS)
        VideoFeedback=0;
    else
        VideoFeedback=1;
        
    // Inizia la connessione...
    
    if (!ND_HWMODE_DisableNetworkService)
    {
          if (!ndNetInit_IsFailure)
          {
               if ( !ndHAL_WLAN_InitNetworkLayer() )
               {
                            NetObj->Psp.NrConfigInFirmw=NrAccessPoint;
                            NetObj->Psp.WndDialogBox0_IsOpen=0;
                            NetObj->Psp.WndDialogBox1_IsOpen=0;
                            
                            ErrRep=INTERNAL_CreateConnectionDialogBox (VideoFeedback, NetObj);
                            
                            if (ErrRep!=0) VideoFeedback=0;
                            // Disattiva feedback video se ci sono stati problemi nel creare la
                            // finestra
                            
                            if ( !ndHAL_WLAN_PSPLanSwitchIsOpen() )     // Lo switch LAN è chiuso
                            {
                                INTERNAL_LanSwitchIsOff (VideoFeedback, NetObj);
                                
                                ndNetInit_Failure_MainErrCode = ERRNET_PSP__LAN_SWITCH_IS_OFF;
                                ndNetInit_Failure_SecErrCode  = 0;     
                         
                                return ERRNET_PSP__LAN_SWITCH_IS_OFF; 
                            }
                            
                            INTERNAL_WaitingForAPConnect (VideoFeedback, NetObj);
                            
                            // Connect
                            
                            fwErrRep = sceNetApctlConnect(NrAccessPoint);
                            if (fwErrRep != 0)
                            {
                                INTERNAL_SignalFailureApctl (VideoFeedback, fwErrRep, NetObj);
                                sceNetApctlDisconnect();
                                
                                ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILURE_IN_APCTL_CONNECT;
                                ndNetInit_Failure_SecErrCode  = fwErrRep;     
                         
                                return ERRNET_PSP__FAILURE_IN_APCTL_CONNECT;
                            }
                        
                            // Memorizza il valore del clock prima di iniziare il ciclo
                            
                            LibcCodeBegin = sceKernelLibcClock();
                            ConnectionOk  = 0;
                            
                            // Report status while waiting for connection to access point
                            int StateLast = -1;
                            
                            while (1)
                            {
                                fwErrRep = sceNetApctlGetState(&State);
                                if (fwErrRep != 0)
                                {
                                    INTERNAL_SignalFailureApctlGetState (VideoFeedback, fwErrRep, NetObj);
                                    sceNetApctlDisconnect();
                                
                                    ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILURE_IN_APCTL_GETSTATE;
                                    ndNetInit_Failure_SecErrCode  = fwErrRep;     
                         
                                    return ERRNET_PSP__FAILURE_IN_APCTL_GETSTATE;
                                }
                                
                                if (State > StateLast)
                                {
                                    INTERNAL_SignalConnectionStage (State, VideoFeedback, NetObj);
                                    StateLast = State;
                                }
                                
                                if (State == 4) break;         // connected with static IP
                                sceKernelDelayThread(50*1000); // wait a little before polling again : 50ms
                            
                                LibcCodeEnd = sceKernelLibcClock();
                                
                                if ( (TimeOut!=0) && (LibcCodeEnd-LibcCodeBegin>TimeOut*1000000) )
                                {
                                    INTERNAL_SignalTimeOut (VideoFeedback, NetObj);
                                    sceNetApctlDisconnect();
                                
                                    ndNetInit_Failure_MainErrCode = ERRNET_PSP__TIMEOUT;
                                    ndNetInit_Failure_SecErrCode  = 0;     
                         
                                    return ERRNET_PSP__TIMEOUT;
                                }
                            }
                            
                            // Se il sistema è arrivato a questo punto, allora significa
                            // che la connessione è riuscita. Segnalalo per mezzo di
                            // un cambiamento nell'icona NET
                            
                            INTERNAL_ChangeStateNetIcon_AddConnection ();
                        
                            // Il sistema deve ricavare velocemente tutte le informazioni 
                            // disponibili sulla connessione
                        
                            ndHAL_WLAN_GetAccessPointInfo (NrAccessPoint, NetObj);
                            INTERNAL_SignalConnectionSuccess (VideoFeedback, NetObj);
                            
                            return 0; 
               }
               else
               {
                   return ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER;
               }                    
          }
          else     // C'è qualche errore di rete (nella inizializzazione hardware)
          {
               return ERRNET_PSP__NETWORK_IS_FAILURE;     
          }  
    }
    else    // Il supporto network è stato disattivato via software
    {
        return ERRNET_PSP__NETWORK_HAS_BEEN_DISABLED_BY_SOFT;    
    }
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
    if (!ND_HWMODE_DisableNetworkService)
    {
          if (!ndNetInit_IsFailure)
          {
               if ( !ndHAL_WLAN_InitNetworkLayer() )
               {
                    int fwErrRep = sceNetApctlDisconnect();

                    if (!fwErrRep)
                    {
                        // Segnala che una connessione è stata tolta
                        INTERNAL_ChangeStateNetIcon_SubConnection ();
                        
                        // Nessun errore
                        ndNetInit_Failure_MainErrCode = 0;
                        ndNetInit_Failure_SecErrCode  = 0;     
                        return 0;  
                    }
                    else
                    {
                        ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILURE_IN_APCTL_DISCONNECT;
                        ndNetInit_Failure_SecErrCode  = fwErrRep;     
                        return ERRNET_PSP__FAILURE_IN_APCTL_DISCONNECT;                 
                    }
               }
               else
               {
                   return ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER;
               }                    
          }
          else     // C'è qualche errore di rete (nella inizializzazione hardware)
          {
               return ERRNET_PSP__NETWORK_IS_FAILURE;     
          }  
    }
    else    // Il supporto network è stato disattivato via software
    {
        return ERRNET_PSP__NETWORK_HAS_BEEN_DISABLED_BY_SOFT;    
    }
}






