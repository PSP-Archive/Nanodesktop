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
    FUNZIONI DI GESTIONE DI RETE A BASSO LIVELLO (PSP KSU MODE)
    ---------------------------------------------------------------------------
 */
 
#include <HAL_KSU_PSP\$_HAL_NDforND.h>

/* 
   --------------------------------------------------------------------------
   FUNZIONI DI MANIPOLAZIONE KERNEL
   --------------------------------------------------------------------------
*/

// Riferimenti necessari per il corretto funzionamento

#include <pspmoduleinfo.h>
extern SceModuleInfo module_info;     



static u32 INTERNAL_FindProcEntryByUID(u32 oid, u32 nid)
{
    SceModule* modP = sceKernelFindModuleByUID(oid);
    if (modP == NULL)
    {
        return 0;
    }
    SceLibraryEntryTable* entP = (SceLibraryEntryTable*)modP->ent_top;
    while ((u32)entP < ((u32)modP->ent_top + modP->ent_size))
    {
        int count = entP->stubcount + entP->vstubcount;
        u32* nidtable = (u32*)entP->entrytable;
        int i;
        for (i = 0; i < count; i++)
        {
            if (nidtable[i] == nid)
            {
                u32 procAddr = nidtable[count+i];
                return procAddr;
            }
        }
        entP++;
    }
    return 0;
}

static int INTERNAL_PatchLibEntries(SceModuleInfo* modInfoPtr, SceUID oidLib)
{
    //dumb version
    int nPatched = 0;

    int* stubPtr; // 20 byte structure
    for (stubPtr = modInfoPtr->stub_top;
        stubPtr + 5 <= (int*)modInfoPtr->stub_end;
        stubPtr += 5)
    {
        int count = (stubPtr[2] >> 16) & 0xFFFF;
        int* idPtr = (int*)stubPtr[3];
        int* procPtr = (int*)stubPtr[4];

        if (stubPtr[1] != 0x90000)
            continue;   // skip non-lazy loaded modules
        while (count--)
        {
            if (procPtr[0] == 0x54C && procPtr[1] == 0)
            {
                // SWI - scan for NID
                u32 proc = INTERNAL_FindProcEntryByUID(oidLib, *idPtr);
                if (proc != 0)
                {
                    if (((u32)procPtr & 0xF0000000) != (proc & 0xF0000000))
                    {
                        // if not in user space we can't use it
                        // printf("!! NOT PATCH\n");
                    }
                    else
                    {
                        u32 val = (proc & 0x03FFFFFF) >> 2;
                        procPtr[0] = 0x0A000000 | val;
                        procPtr[1] = 0;
                        
                        nPatched++;
                    }
                }
            }
            idPtr++;
            procPtr += 2; // 2 opcodes
        }
    }
    return nPatched;
}

SceUID INTERNAL_LoadStartPatchMod(SceModuleInfo* modInfoPtr, const char* szFile)
    // return oid or error code
{
    SceUID oid;
    int err;
    int startStatus; // ignored

    oid = sceKernelLoadModule(szFile, 0, NULL);
    if (oid & 0x80000000)
        return oid; // error code

    err = sceKernelStartModule(oid, 0, 0, &startStatus, 0);
    if (err != oid)
        return err;

    INTERNAL_PatchLibEntries(modInfoPtr, oid);
    return oid;
}


extern void sceKernelIcacheInvalidateAll();

void INTERNAL_FlushCache (void)
{
    // not sure if these are necessary, but to be extra safe
    sceKernelDcacheWritebackAll();
    sceKernelIcacheInvalidateAll();
}




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

void INTERNAL_FW150_InitLowLevelNetwork (void)
/*
     Questa routine provvede a caricare i drivers a basso livello per la gestione
     della rete da parte della PSP, per il fimware 1.5
    
     Provvede inoltre alla reinizializzazione del sistema WLAN 
*/
{
     int FirmErrRep;
     
     FirmErrRep = INTERNAL_LoadStartPatchMod(&module_info, "flash0:/kd/ifhandle.prx"); // kernel
     
     if (FirmErrRep<0)
     {
          ndNetInit_IsFailure = 1;
          ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_LOAD_IFHANDLE_MODULE;
          ndNetInit_Failure_SecErrCode  = FirmErrRep;
          
          return;                      
     }
     
     FirmErrRep = INTERNAL_LoadStartPatchMod(&module_info, "flash0:/kd/pspnet.prx");
     
     if (FirmErrRep<0)
     {
          ndNetInit_IsFailure = 1;
          ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_LOAD_NET_MODULE;
          ndNetInit_Failure_SecErrCode  = FirmErrRep;
          
          return;                      
     }
     
     FirmErrRep = INTERNAL_LoadStartPatchMod(&module_info, "flash0:/kd/pspnet_inet.prx");
     
     if (FirmErrRep<0)
     {
          ndNetInit_IsFailure = 1;
          ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_LOAD_NETINET_MODULE;
          ndNetInit_Failure_SecErrCode  = FirmErrRep;
          
          return;                      
     }
     
     FirmErrRep = INTERNAL_LoadStartPatchMod(&module_info, "flash0:/kd/pspnet_apctl.prx");
     
     if (FirmErrRep<0)
     {
          ndNetInit_IsFailure = 1;
          ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_LOAD_APCTL_MODULE;
          ndNetInit_Failure_SecErrCode  = FirmErrRep;
          
          return;                      
     }
     
     FirmErrRep = INTERNAL_LoadStartPatchMod(&module_info, "flash0:/kd/pspnet_resolver.prx");
     
     if (FirmErrRep<0)
     {
          ndNetInit_IsFailure = 1;
          ndNetInit_Failure_MainErrCode = ERRNET_PSP__FAILED_TO_LOAD_RESOLVER_MODULE;
          ndNetInit_Failure_SecErrCode  = FirmErrRep;
          
          return;                      
     }
     
     INTERNAL_FlushCache ();

     // Se il sistema è arrivato qui significa che non ci sono errori

     ndNetInit_IsFailure = 0;
     ndNetInit_Failure_MainErrCode = 0;
     ndNetInit_Failure_SecErrCode  = 0;
     
     // Monitor di rete inizialmente disattivato
     
     ndNetMonitorEnabled = 0;     
     return;                
}

void INTERNAL_FW150_TerminateLowLevelNetwork (void)
/*
     Questa routine provvede ad arrestare tutti i drivers di rete che sono
     in quel momento in funzione. E' specifica per il firmware è 1.50
*/
{
    sceNetApctlTerm();
    sceNetResolverTerm();
    sceNetInetTerm();
    sceNetTerm();

    return; 
}

void INTERNAL_CFFW150_InitLowLevelNetwork (void)
/*
     Questa routine provvede a caricare i drivers a basso livello per la gestione
     della rete da parte della PSP. E' usata per i firmware in versione "custom"
     quando essi operano in emulazione firmware 1.50
    
     Provvede inoltre alla reinizializzazione del sistema WLAN 
*/
{
     int FirmErrRep;
     FirmErrRep = pspSdkLoadInetModules();
     
     if (FirmErrRep<0)
     {
          ndNetInit_IsFailure = 1;
          ndNetInit_Failure_MainErrCode = ERRNET_PSP__GENERIC_FAILURE_IN_LOAD;
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

void INTERNAL_CFFW150_TerminateLowLevelNetwork (void)
/*
     Questa routine provvede ad arrestare tutti i drivers di rete che sono
     in quel momento in funzione. E' specifica per il firmware è 1.50
*/
{
    sceNetApctlTerm();
    sceNetResolverTerm();
    sceNetInetTerm();
    sceNetTerm();

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
     
     La reinizializzazione viene eseguita dal main thread della modalità KSU:
     tale main thread gira in kernel mode e grazie all'azione del loader
     definito in ndHAL_PlatformHeaders.h, può caricare moduli del firmware
     della PSP ed inizializzarli. 
     
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
     
     ERRNET_PSP__FAILED_TO_LOAD_IFHANDLE_MODULES
     ERRNET_PSP__FAILED_TO_LOAD_NET_MODULE
     ERRNET_PSP__FAILED_TO_LOAD_NETINET_MODULE
     ERRNET_PSP__FAILED_TO_LOAD_APCTL_MODULE
     ERRNET_PSP__FAILED_TO_LOAD_RESOLVER_MODULE
        Il sistema non è riuscito a caricare i moduli del firmware che permettono
        di reinizializzare la rete. Il codice di errore secondario è il codice
        di errore restituito dal firmware PSP.
        
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
*/
{
     int ClassFirmCode;
     
     if (!ND_HWMODE_DisableNetworkService)
     {
         ClassFirmCode = ndPspFwVersion & 0xFFFFFF00;    // Codice di classe del firmware
         
         switch (ClassFirmCode)
         {
                case (NDPSPFW_SONY + 0x00000100):
                {
                                  switch (ndPspFwEmuVersion)
                                  {
                                         case NDPSPEMUFW150:
                                         {
                                              INTERNAL_FW150_InitLowLevelNetwork ();
                                              return;
                                         }
                                  }
                }
                
                case (NDPSPFW_CF + 0x00000300):      // CF versione 3
                {
                                  switch (ndPspFwEmuVersion)
                                  {
                                         case NDPSPEMUFW150:
                                         {
                                              INTERNAL_CFFW150_InitLowLevelNetwork ();
                                              return;
                                         }
                                  }
                }
         }
                          
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
         ClassFirmCode = ndPspFwVersion & 0xFFFFFF00;    // Codice di classe del firmware
         
         switch (ClassFirmCode)
         {
                case (NDPSPFW_SONY + 0x00000100):
                {
                                  switch (ndPspFwEmuVersion)
                                  {
                                         case NDPSPEMUFW150:
                                         {
                                              INTERNAL_FW150_TerminateLowLevelNetwork ();
                                              return;
                                         }
                                  }
                }
                
                case (NDPSPFW_CF + 0x00000300):      // CF versione 3
                {
                                  switch (ndPspFwEmuVersion)
                                  {
                                         case NDPSPEMUFW150:
                                         {
                                              INTERNAL_CFFW150_TerminateLowLevelNetwork ();
                                              return;
                                         }
                                  }
                }
         }
          
     }  
}
