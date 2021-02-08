/* 
    ----------------------------------------------------------------------------
    Libreria ndGCURL - Versione 1.0
    Copyright 2007-2012 Filippo Battaglia 
    ----------------------------------------------------------------------------
    The ndGCURL (Graphical LibCurl for Nanodesktop) is a library that provides
    some graphical methods, called "jobs", for proceeding with file downloads and
    other curl operations on the network. The jobs can be made using a simple 
    programming interface: they manage also visual feedback and Nanodesktop
    dialog box. 
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
        
    By downloading, copying, installing or using the software you agree to this license.
    If you do not agree to this license, do not download, install,
    copy or use the software.
    
    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    
      * Redistribution's of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
    
      * Redistribution's in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
    
      * The name of the Visilab research center may not be used to endorse or promote products
        derived from this software without specific prior written permission.
    
    This software is provided by the copyright holders and contributors "as is" and
    any express or implied warranties, including, but not limited to, the implied
    warranties of merchantability and fitness for a particular purpose are disclaimed.
    In no event shall the author of the software, the Visilab Research Center or 
    contributors be liable for any direct, indirect, incidental, special, exemplary, 
    or consequential damages (including, but not limited to, procurement of substitute 
    goods or services; loss of use, data, or profits; or business interruption) 
    however caused and on any theory of liability, whether in contract, strict liability,
    or tort (including negligence or otherwise) arising in any way out of
    the use of this software, even if advised of the possibility of such damage.
    -----------------------------------------------------------------------------
    GCURL JOB: TRANSFER FILE
    -----------------------------------------------------------------------------
*/


#include <$$GCurl_Internal.h>
// Include i simboli interni della libreria (richiamerà anche definizioni di
// simboli pubblici

/*
    -----------------------------------------------------------------------------
    CALLBACKS DI SCRITTURA E DI MONITORAGGIO
    -----------------------------------------------------------------------------
*/


static int INTERNAL_FileTransfer_cbWrite (void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct gCurl_FileTransfer_cbContainer *MyCbContainer; 
  
    // Recupera le informazioni relative al cbContainer
    
    MyCbContainer = (struct gCurl_FileTransfer_cbContainer *)(stream);
  
    if (MyCbContainer)
    {            
          if (!MyCbContainer->AbortRequested)    // Non è stata richiesta l'interruzione utente
          {           
                // Se il file non è stato ancora aperto, fallo
                if ( !MyCbContainer->Stream )
                {
                    MyCbContainer->Stream = fopen (MyCbContainer->FileDest, "wb");
                    
                    if ( !MyCbContainer->Stream )     // Il processo di scrittura è fallito
                    {
                        return -1;   // Segnala l'errore alla libreria curl che terminerà il trasferimento
                    }
                }
                
                // Esegui il trasferimento
                return fwrite(buffer, size, nmemb, MyCbContainer->Stream); 
          }
          else  //  E' stato richiesto di abortire il processo di trasferimento
          {
                return -1;
                // Simula una condizione di errore: indurrà la libreria curl ad
                // interrompere immediatamente il trasferimento
          }                                            
    }
    else
      return 0;    // Curl continue
}

static int INTERNAL_FileTransfer_cbProgress (void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
   struct gCurl_FileTransfer_cbContainer *MyCbContainer = clientp;
   float  Value;
   
   if (dltotal!=0)
   {           
       Value = (dlnow/dltotal)*100;
       INTERNAL_FileTransfer_DrawNetProgressBar (Value, MyCbContainer, NORENDER);
       INTERNAL_FileTransfer_Message_PROGRESS (MyCbContainer, (int)(dlnow), RENDER);
   }  
       
   return 0;
}

/*
    -----------------------------------------------------------------------------
    CALLBACKS DI GESTIONE PULSANTI
    -----------------------------------------------------------------------------
*/

static ndint64 INTERNAL_FileTransfer_cbOKPressed (char *StringID, ndint64 InfoField, char WndHandle)
/*
    E' questa la callback che viene richiamata quando viene premuto il tasto OK
*/
{
    struct gCurl_FileTransfer_cbContainer *MyCbContainer = (void *)(InfoField);
    MyCbContainer->SemaphoreForExit=1;    // Provocherà l'uscita di ProvideMouseUntil
    
    return 0;
}

static ndint64 INTERNAL_FileTransfer_cbCloseWndDuringTransfer (char WndHandle, ndint64 WndInfoField)
/*
    Questa callback è quella che viene eseguita qualora si provi a chiudere la finestra
    di dialogo durante la fase di trasferimento del file 
*/
{
    struct gCurl_FileTransfer_cbContainer *MyCbContainer = (void *)(WndInfoField);
    MyCbContainer->AbortRequested=1;      // Questa farà sì che tra pochi ns il processo si interrompa
    
    return 0;   
}

static ndint64 INTERNAL_FileTransfer_cbCloseWndWorkComplete (char WndHandle, ndint64 WndInfoField)
/*
    Questa callback è quella che viene eseguita quando si prova a chiudere la dialog box, 
    mentre il sistema è in attesa che si prema il tasto OK 
*/
{
    struct gCurl_FileTransfer_cbContainer *MyCbContainer = (void *)(WndInfoField);
    MyCbContainer->SemaphoreForExit=1;    // Provocherà l'uscita di ProvideMouseUntil
    
    return 0;   
}

static ndint64 INTERNAL_FileTransfer_cbAbort (char *StringID, ndint64 InfoField, char WndHandle)
/*
    E' questa la callback che viene richiamata quando viene premuto il tasto Abort
*/
{
    struct gCurl_FileTransfer_cbContainer *MyCbContainer = (void *)(InfoField);
    MyCbContainer->AbortRequested=1;      // Questa farà sì che tra pochi ns il processo si interrompa
    
    return 0;
}


/*
    -----------------------------------------------------------------------------
    ROUTINES DI SETTAGGIO FINESTRE E POSIZIONE
    -----------------------------------------------------------------------------
*/

void INTERNAL_FileTransfer_SetupColors (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions)
/*
     Provvede a settare i colori ai valori standard
*/    
{
     TypeColor StdTitleColor        = COLOR_BLUE;
     TypeColor StdMainColor         = COLOR_GRAY18;
     TypeColor StdSecondaryColor    = COLOR_GRAY14;
     TypeColor StdBarColor          = COLOR_BLUE; 
     TypeColor StdSecondaryBarColor = COLOR_GRAY07; 
     TypeColor StdTextColor         = COLOR_BLACK;   
     TypeColor StdTextBarColor      = COLOR_WHITE;    
      
     if (!MyOptions)        // Non ci sono opzioni esplicite. Valori standard
     {
         MyCbContainer->SetTitleColor        = StdTitleColor;
         MyCbContainer->SetMainColor         = StdMainColor;
         MyCbContainer->SetSecondaryColor    = StdSecondaryColor;
         MyCbContainer->SetBarColor          = StdBarColor; 
         MyCbContainer->SetSecondaryBarColor = StdSecondaryBarColor; 
         MyCbContainer->SetTextColor         = StdTextColor;   
         MyCbContainer->SetTextBarColor      = StdTextBarColor;                   
     }
     else
     {
         if (!MyOptions->NoStdTitleColor) MyCbContainer->SetTitleColor = StdTitleColor;
            else MyCbContainer->SetTitleColor = MyOptions->OptTitleColor;
            
         if (!MyOptions->NoStdMainColor) MyCbContainer->SetMainColor = StdMainColor;
            else MyCbContainer->SetMainColor = MyOptions->OptMainColor;
            
         if (!MyOptions->NoStdSecondaryColor) MyCbContainer->SetSecondaryColor = StdSecondaryColor;
            else MyCbContainer->SetSecondaryColor = MyOptions->OptSecondaryColor;
         
         if (!MyOptions->NoStdBarColor) MyCbContainer->SetBarColor = StdBarColor;
            else MyCbContainer->SetBarColor = MyOptions->OptBarColor;
         
         if (!MyOptions->NoStdSecondaryBarColor) MyCbContainer->SetSecondaryBarColor = StdSecondaryBarColor;
            else MyCbContainer->SetSecondaryBarColor = MyOptions->OptSecondaryBarColor;
         
         if (!MyOptions->NoStdTextColor) MyCbContainer->SetTextColor = StdTextColor;
            else MyCbContainer->SetTextColor = MyOptions->OptTextColor;
            
         if (!MyOptions->NoStdTextBarColor) MyCbContainer->SetTextBarColor = StdTextBarColor;
            else MyCbContainer->SetTextBarColor = MyOptions->OptTextBarColor;
     } 
}


void INTERNAL_FileTransfer_SetupPosition (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions)
/*
    Setta la posizione della finestra di dialogo.
*/
{
    if (!MyOptions)          // Valori standard (chiedi al position generator)
    {
        ndPOSGEN_GenPosition ( 300, 170, &(MyCbContainer->PosX1), &(MyCbContainer->PosY1), &(MyCbContainer->PosX2), &(MyCbContainer->PosY2) );            
    }
    else
    {
        if (!MyOptions->NoStdPosition)
        {
           ndPOSGEN_GenPosition ( 300, 170, &(MyCbContainer->PosX1), &(MyCbContainer->PosY1), &(MyCbContainer->PosX2), &(MyCbContainer->PosY2) );                                       
        }
        else
        {
           MyCbContainer->PosX1 = MyOptions->OptPosX1;
           MyCbContainer->PosY1 = MyOptions->OptPosY1;
           MyCbContainer->PosX2 = MyOptions->OptPosX1 + 300;
           MyCbContainer->PosY2 = MyOptions->OptPosY1 + 170;
        }
    }
}


/*
    -----------------------------------------------------------------------------
    ROUTINES PER IL MONITORAGGIO DELL'AVANZAMENTO DEL DOWNLOAD
    -----------------------------------------------------------------------------
*/


void INTERNAL_FileTransfer_DrawNetProgressBar (float Value, struct gCurl_FileTransfer_cbContainer *MyCbContainer, char RenderNow)
{
    ndTBAR_DrawProgressBar (12, 80, 278, 95, Value, 0, 100, MyCbContainer->SetTextBarColor, MyCbContainer->SetBarColor, 
                                                                MyCbContainer->SetSecondaryBarColor, MyCbContainer->SetTextColor, PBAR_PERCENT, 
                                                                MyCbContainer->WndHandle, RenderNow);
          
} 

void INTERNAL_FileTransfer_Message_PROGRESS (struct gCurl_FileTransfer_cbContainer *MyCbContainer, int BytesNow, char RenderNow)
{
    char InternalBuffer0 [256];
    char InternalBuffer1 [256];
    float  TimeElapsed;
    float  ByteDiff;
    float  Speed, SpeedKB;
    int    TheClock;
    
    sprintf (InternalBuffer0, "In progress ");
    
    // Calcola velocità di download
    TheClock = clock ();
    
    if (!(MyCbContainer->oldBytesRegistered))
    {
        MyCbContainer->oldTime  = TheClock;
        MyCbContainer->oldBytes = BytesNow;
        MyCbContainer->oldBytesRegistered = 1;                                   
    }
    else
    {
        TimeElapsed = TheClock - MyCbContainer->oldTime;
        ByteDiff    = BytesNow - MyCbContainer->oldBytes;
        
        Speed   = (int)(ByteDiff/TimeElapsed*_CLOCKS_PER_SEC_);
        SpeedKB = Speed/1024;
        
        // Aggiorna i nuovi valori
        
        MyCbContainer->oldTime  = TheClock;
        MyCbContainer->oldBytes = BytesNow;
        
        sprintf (&InternalBuffer1, "(%5.1f KB/s)", SpeedKB);
        strcat (&InternalBuffer0, &InternalBuffer1);
    }
    
    ndWS_DrawRectangle (10, 100, 280, 105+12, MyCbContainer->SetMainColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, RenderNow); 
    ndWS_GrWriteLn     (10, 102, &InternalBuffer0, MyCbContainer->SetTextColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, RenderNow);          
}

void INTERNAL_FileTransfer_Message_COMPLETE_WITHOUT_ERRORS (struct gCurl_FileTransfer_cbContainer *MyCbContainer, char RenderNow)
{
    ndWS_DrawRectangle (10, 100, 280, 105+12, MyCbContainer->SetMainColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, RenderNow); 
    ndWS_GrWriteLn     (10, 102, "Completed. No errors", MyCbContainer->SetTextColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, RenderNow);           
}

void INTERNAL_FileTransfer_Message_USERABORTED (struct gCurl_FileTransfer_cbContainer *MyCbContainer, char RenderNow)
{
    ndWS_DrawRectangle (10, 100, 280, 105+12, MyCbContainer->SetMainColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, RenderNow); 
    ndWS_GrWriteLn     (10, 102, "Aborted by user", MyCbContainer->SetTextColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, RenderNow);           
}

void INTERNAL_FileTransfer_Message_CURLERROR (struct gCurl_FileTransfer_cbContainer *MyCbContainer, int Error, char RenderNow)
{
    char InternalBuffer0 [256];
    
    sprintf (InternalBuffer0, "Interrupted. Curl error: %d ", Error);

    ndWS_DrawRectangle (10, 100, 280, 105+12, MyCbContainer->SetMainColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, RenderNow); 
    ndWS_GrWriteLn     (10, 102, &InternalBuffer0, MyCbContainer->SetTextColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, RenderNow);           
}


/*
    -----------------------------------------------------------------------------
    ROUTINE PER LA CREAZIONE DELLA DIALOG BOX
    -----------------------------------------------------------------------------
*/

int INTERNAL_FileTransfer_CreateWnd (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions)
/*
     Questa routine provvede a creare la finestra di file transfer sullo schermo
     Se non vi sono errori viene restituito 0
*/
{
     char InternalBuffer0 [255];
     char InternalBuffer1 [255];
     
     INTERNAL_FileTransfer_SetupColors (MyCbContainer, MyOptions);
     INTERNAL_FileTransfer_SetupPosition (MyCbContainer, MyOptions);
     
     MyCbContainer->WndHandle = ndLP_CreateWindow (MyCbContainer->PosX1, MyCbContainer->PosY1, 
                                                   MyCbContainer->PosX2, MyCbContainer->PosY2, 
                                                   "Downloading...", COLOR_WHITE, 
                                                   MyCbContainer->SetTitleColor, MyCbContainer->SetMainColor, 
                                                   MyCbContainer->SetMainColor, 0);
    
     if (MyCbContainer->WndHandle>=0)
     {
         ndLP_MaximizeWindow (MyCbContainer->WndHandle); 
         
         ndWS_GrWriteLn     (10, 5, "From", MyCbContainer->SetTextColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, NORENDER);
         ndWS_DrawRectangle (10, 15, 280, 15+22, MyCbContainer->SetSecondaryColor, MyCbContainer->SetSecondaryColor, MyCbContainer->WndHandle, NORENDER); 
         
         ndStrCpyBounded (&InternalBuffer0, MyCbContainer->FileName, 0, 32, 0);
         ndWS_GrWriteLn     (12, 15+2, &InternalBuffer0, MyCbContainer->SetTextColor, MyCbContainer->SetSecondaryColor, MyCbContainer->WndHandle, NORENDER); 
         
         if (strlen (MyCbContainer->FileName)>=32)
         {
            ndStrCpyBounded (&InternalBuffer1, MyCbContainer->FileName, 33, 64, 0);
            ndWS_GrWriteLn  (12, 15+12, &InternalBuffer1, MyCbContainer->SetTextColor, MyCbContainer->SetSecondaryColor, MyCbContainer->WndHandle, NORENDER); 
         }
         
         ndWS_GrWriteLn     (10, 40, "To", MyCbContainer->SetTextColor, MyCbContainer->SetMainColor, MyCbContainer->WndHandle, NORENDER);
         ndWS_DrawRectangle (10, 50, 280, 50+22, MyCbContainer->SetSecondaryColor, MyCbContainer->SetSecondaryColor, MyCbContainer->WndHandle, NORENDER); 
         
         ndStrCpyBounded (&InternalBuffer0, MyCbContainer->FileDest, 0, 32, 0);
         ndWS_GrWriteLn     (12, 50+2, &InternalBuffer0, MyCbContainer->SetTextColor, MyCbContainer->SetSecondaryColor, MyCbContainer->WndHandle, NORENDER); 
         
         if (strlen (MyCbContainer->FileDest)>=32)
         {
            ndStrCpyBounded (&InternalBuffer1, MyCbContainer->FileDest, 33, 64, 0);
            ndWS_GrWriteLn     (12, 50+12, &InternalBuffer1, MyCbContainer->SetTextColor, MyCbContainer->SetSecondaryColor, MyCbContainer->WndHandle, NORENDER); 
         }
         
         // Crea la progress bar
         INTERNAL_FileTransfer_DrawNetProgressBar (0, MyCbContainer, NORENDER);
         
         // Progressing
         INTERNAL_FileTransfer_Message_PROGRESS (MyCbContainer, 0, NORENDER);
         
         // Crea i pulsanti  
         MyCbContainer->BtnAbort = ndCTRL_CreateButton (138, 120, 138+64, 135, MyCbContainer->WndHandle, "BtnAbort", "Abort", "", MyCbContainer->SetTextColor, MyCbContainer->SetMainColor, 
                             MyCbContainer->SetTextColor, ND_BUTTON_ROUNDED, &(INTERNAL_FileTransfer_cbAbort), (int)(MyCbContainer), NORENDER); 
         
         MyCbContainer->BtnOK = ndCTRL_CreateButton (138+64+10, 120, 138+64+10+64, 135, MyCbContainer->WndHandle, "BtnOK", "OK", "", MyCbContainer->SetTextColor, MyCbContainer->SetMainColor, 
                             MyCbContainer->SetTextColor, ND_BUTTON_ROUNDED, 0, MyCbContainer, NORENDER); 
         
         // Il pulsante OK all'inizio deve essere disattivato
         ndCTRL_InhibitButton (MyCbContainer->BtnOK, MyCbContainer->WndHandle, NORENDER);
         
         // Setup della callback di chiusura finestra (sarà cambiata a trasferimento completato)
         ndLP_SetupCloseWndCallback (&(INTERNAL_FileTransfer_cbCloseWndDuringTransfer), (int)(MyCbContainer), 0, MyCbContainer->WndHandle);
         
         // Rendering di quanto creato finora
         XWindowRender (MyCbContainer->WndHandle);                            
         return 0;                                       // Segnala alla routine chiamante che non ci sono problemi
     }
     else return -1; 
}


/*
    -----------------------------------------------------------------------------
    ROUTINES CHE GESTISCONO IL COMPORTAMENTO AL TERMINE DEL DOWNLOAD
    -----------------------------------------------------------------------------
*/


void INTERNAL_FileTransfer_EndTransfer_SUCCESS (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions)
/*
    Questa routine si occupa di gestire il sistema nel momento in cui il trasferimento di
    un file viene completato senza errori.
*/
{
   char DoYouWantAutoClose;
   
   if (MyCbContainer->Stream) fclose (MyCbContainer->Stream);                       
   // Chiudi il file, avendo avuto successo il trasferimento
   
   INTERNAL_FileTransfer_Message_COMPLETE_WITHOUT_ERRORS (MyCbContainer, RENDER);
   // Segnala l'assenza di errori
   
   if (!MyOptions) 
      DoYouWantAutoClose=1;
   else 
      DoYouWantAutoClose = !(MyOptions->NoAutoCloseDBoxWhenFinish);
   
   if (DoYouWantAutoClose)            // Si vuole l'autoclose
   {
       ndDelay (2);        // Ritardo di 2 secondi per consentire la lettura
       
       ndLP_DestroyWindow (MyCbContainer->WndHandle);
       return;                                                  
   }
   else
   {
       ndCTRL_InhibitButton   (MyCbContainer->BtnAbort, MyCbContainer->WndHandle, NORENDER);
       ndCTRL_DeInhibitButton (MyCbContainer->BtnOK, MyCbContainer->WndHandle, NORENDER);
       XWindowRender (MyCbContainer->WndHandle);
       
       // Modifica la callback del pulsante OK: adesso quando viene premuto il pulsante il sistema uscirà dal loop
       ndCTRL_ChangeCallBack  (&INTERNAL_FileTransfer_cbOKPressed, (int)(MyCbContainer), MyCbContainer->BtnOK, MyCbContainer->WndHandle, 0); 
       
       // Modifica la callback di chiusura finestra
       ndLP_SetupCloseWndCallback (&(INTERNAL_FileTransfer_cbCloseWndWorkComplete), (int)(MyCbContainer), 0, MyCbContainer->WndHandle);
       
       // Mantieni il sistema fermo fin quando l'utente non preme il tasto OK
       ndProvideMeTheMouse_Until (&(MyCbContainer->SemaphoreForExit), ND_IS_EQUAL, 1, 0, 0);
       
       // Arrivati a questo punto, distruggi la finestra       
       ndLP_DestroyWindow (MyCbContainer->WndHandle);
       return;
   }  
                                        
}


void INTERNAL_FileTransfer_EndTransfer_USERABORTED (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions)
/*
    Questa routine si occupa di gestire il sistema nel momento in cui il trasferimento di
    un file è stato abortito dal sistema.
*/
{
   char DoYouWantAutoClose;
   
   if (MyCbContainer->Stream) 
   {
      fclose (MyCbContainer->Stream);                       
      // Chiudi il file. Il trasferimento è stato interrotto....
   
      remove (MyCbContainer->FileDest);
      // .... ed il file parziale deve essere cancellato dal disco
   }
   
   INTERNAL_FileTransfer_Message_USERABORTED (MyCbContainer, RENDER);
   // Segnala l'interruzione all'utente
   
   if (!MyOptions) 
      DoYouWantAutoClose=1;
   else 
      DoYouWantAutoClose = !(MyOptions->NoAutoCloseDBoxWhenFinish);
   
   if (DoYouWantAutoClose)            // Si vuole l'autoclose
   {
       ndDelay (2);        // Ritardo di 2 secondi per consentire la lettura
       
       ndLP_DestroyWindow (MyCbContainer->WndHandle);
       return;                                                  
   }
   else
   {
       ndCTRL_InhibitButton   (MyCbContainer->BtnAbort, MyCbContainer->WndHandle, NORENDER);
       ndCTRL_DeInhibitButton (MyCbContainer->BtnOK, MyCbContainer->WndHandle, NORENDER);
       XWindowRender (MyCbContainer->WndHandle);
       
       // Modifica la callback del pulsante OK: adesso quando viene premuto il pulsante il sistema uscirà dal loop
       ndCTRL_ChangeCallBack  (&INTERNAL_FileTransfer_cbOKPressed, (int)(MyCbContainer), MyCbContainer->BtnOK, MyCbContainer->WndHandle, 0); 
       
       // Modifica la callback di chiusura finestra
       ndLP_SetupCloseWndCallback (&(INTERNAL_FileTransfer_cbCloseWndWorkComplete), (int)(MyCbContainer), 0, MyCbContainer->WndHandle);
       
       // Mantieni il sistema fermo fin quando l'utente non preme il tasto OK
       ndProvideMeTheMouse_Until (&(MyCbContainer->SemaphoreForExit), ND_IS_EQUAL, 1, 0, 0);
       
       // Arrivati a questo punto, distruggi la finestra       
       ndLP_DestroyWindow (MyCbContainer->WndHandle);
       return;
   }                                        
}


void INTERNAL_FileTransfer_EndTransfer_CURLERROR (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions, int CurlError)
/*
    Questa routine si occupa di gestire il sistema nel momento in cui il trasferimento di
    un file è stato fermato per un errore CURL.
*/
{
   char DoYouWantAutoClose;
   
   if (MyCbContainer->Stream) 
   {
      fclose (MyCbContainer->Stream);                       
      // Chiudi il file. Il trasferimento è stato interrotto....
   
      remove (MyCbContainer->FileDest);
      // .... ed il file parziale deve essere cancellato dal disco
   }
   
   INTERNAL_FileTransfer_Message_CURLERROR (MyCbContainer, CurlError, RENDER);
   // Segnala l'interruzione all'utente
   
   if (!MyOptions) 
      DoYouWantAutoClose=1;
   else 
      DoYouWantAutoClose = !(MyOptions->NoAutoCloseDBoxWhenFinish);
   
   if (DoYouWantAutoClose)            // Si vuole l'autoclose
   {
       ndDelay (2);        // Ritardo di 2 secondi per consentire la lettura
       
       ndLP_DestroyWindow (MyCbContainer->WndHandle);
       return;                                                  
   }
   else
   {
       ndCTRL_InhibitButton   (MyCbContainer->BtnAbort, MyCbContainer->WndHandle, NORENDER);
       ndCTRL_DeInhibitButton (MyCbContainer->BtnOK, MyCbContainer->WndHandle, NORENDER);
       XWindowRender (MyCbContainer->WndHandle);
       
       // Modifica la callback del pulsante OK: adesso quando viene premuto il pulsante il sistema uscirà dal loop
       ndCTRL_ChangeCallBack  (&INTERNAL_FileTransfer_cbOKPressed, (int)(MyCbContainer), MyCbContainer->BtnOK, MyCbContainer->WndHandle, 0); 
       
       // Modifica la callback di chiusura finestra
       ndLP_SetupCloseWndCallback (&(INTERNAL_FileTransfer_cbCloseWndWorkComplete), (int)(MyCbContainer), 0, MyCbContainer->WndHandle);
       
       // Mantieni il sistema fermo fin quando l'utente non preme il tasto OK
       ndProvideMeTheMouse_Until (&(MyCbContainer->SemaphoreForExit), ND_IS_EQUAL, 1, 0, 0);
       
       // Arrivati a questo punto, distruggi la finestra       
       ndLP_DestroyWindow (MyCbContainer->WndHandle);
       return;
   }                                        
}


/*
    -----------------------------------------------------------------------------
    ROUTINES PER IL SETUP DELLA ROUTINE DI TRASFERIMENTO
    -----------------------------------------------------------------------------
*/


void INTERNAL_FileTransfer_SetVerboseMode (struct gCurl_FileTransfer_Options *MyOptions, CURL *MyCurl)
{
    if (MyOptions!=0)
    {
          if (MyOptions->BeVerbose)
          {           
              curl_easy_setopt (MyCurl, CURLOPT_VERBOSE, TRUE);
          }
    }                             
}


void INTERNAL_FileTransfer_SetProxyServer (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions, struct ndNetworkObject_Type *NetObj, CURL *MyCurl)
/*
    Si occupa di impostare il proxy server ai valori corretti
*/
{
    char InternalBuffer0 [255];
    char InternalBuffer1 [255];
    char AutoConfigProxy;
    
    if (!MyOptions)
        AutoConfigProxy=1;
    else
        AutoConfigProxy=!(MyOptions->DoNotUseProxyAutoConfig);
    
    if (AutoConfigProxy)       // Si è chiesta l'autoconfigurazione del proxy
    {
       if (NetObj->Psp.USE_PROXY)
       {
           ndHAL_NET_ConvertIPv4ToString ( &(NetObj->Psp.PROXY_SERVER), &InternalBuffer1 );
           strcpy (&InternalBuffer0, &InternalBuffer1);
           
           strcat (&InternalBuffer0, ":");
           
           _itoa (NetObj->Psp.PROXY_PORT, &InternalBuffer1, 10);
           strcat (&InternalBuffer0, &InternalBuffer1);
           
           // Adesso in InternalBuffer0 c'è una stringa che contiene l'indirizzo del
           // proxy server ricavato dal firmware.
           
           curl_easy_setopt(MyCurl, CURLOPT_PROXY, &InternalBuffer0);
           curl_easy_setopt(MyCurl, CURLOPT_HTTPPROXYTUNNEL, MyOptions->HttpTunneling);
           
           // Impostiamo user name e password
           
           if ((strlen (&(NetObj->Psp.PROXY_USER))!=0) || (strlen (&(NetObj->Psp.PROXY_PASS))!=0))
           {
              strcpy (&InternalBuffer0, &(NetObj->Psp.PROXY_USER));
              strcat (&InternalBuffer0, ":");
              strcat (&InternalBuffer0, &(NetObj->Psp.PROXY_PASS));
              
              curl_easy_setopt(MyCurl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
              curl_easy_setopt(MyCurl, CURLOPT_PROXYUSERPWD, &InternalBuffer0);        
           }
       }                 
    }
    else   // La configurazione del proxy verrà fatta manualmente
    {
       if (MyOptions->ManualProxy_Enabled)      // Si è chiesto manualmente di abilitare il proxy
       {
           strcpy ( &InternalBuffer0, &(MyOptions->ManualProxy_Address) );
           strcat ( &InternalBuffer0, ":");
           strcat ( &InternalBuffer0, &(MyOptions->ManualProxy_Port) );
                                               
           // Adesso in InternalBuffer0 c'è una stringa che contiene l'indirizzo del
           // proxy server ricavato dal firmware.
           
           curl_easy_setopt(MyCurl, CURLOPT_PROXY, &InternalBuffer0);
           curl_easy_setopt(MyCurl, CURLOPT_HTTPPROXYTUNNEL, MyOptions->HttpTunneling);
           
           // Impostiamo user name e password
           
           if ( (strlen (&(MyOptions->ManualProxy_User))!=0) || (strlen (&(MyOptions->ManualProxy_Pass))!=0) )
           {
              strcpy ( &InternalBuffer0, &(MyOptions->ManualProxy_User) );
              strcat ( &InternalBuffer0, ":" );
              strcat ( &InternalBuffer0, &(MyOptions->ManualProxy_Pass) );
              
              curl_easy_setopt(MyCurl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
              curl_easy_setopt(MyCurl, CURLOPT_PROXYUSERPWD, &InternalBuffer0);         
           }                                    
       } 
    }                             
}

/*
    -----------------------------------------------------------------------------
    ROUTINE PRINCIPALE
    -----------------------------------------------------------------------------
*/

int ndGCURL_FileTransfer (struct ndNetworkObject_Type *NetObj, char *UrlSrcFile, char *DstFile, struct gCurl_FileTransfer_Options *Options)
/*
    Questa routine provvede a trasferire un file via FTP usando l'interfaccia 
    CURL. NetObj rappresenta un network object che sia già stato associato ad
    un collegamento funzionante per mezzo di una chiamata ad una apposita
    routine di Nanodesktop, ad esempio per mezzo di una chiamata a
    ndHAL_WLAN_OpenPSPAccessPoint. 
    
    UrlSrcFile è l'indirizzo URL della sorgente del file, mentre DestFile è
    l'indirizzo del file di destinazione. 

    Il puntatore alla struct gCurl_FileTransfer_Options, punta ad una struttura
    che contiene le opzioni che dovranno essere usate da questa routine nel
    proprio lavoro. Se si pone a zero, il sistema farà uso delle opzioni
    standard
    
    OPZIONI
    -------
    
    Guardiamo le opzioni previste dalla struct gCurl_FileTransfer_Options. 
    
    struct gCurl_FileTransfer_Options
    {
        char NoStdTitleColor;
        char NoStdMainColor;
        char NoStdSecondaryColor;
        char NoStdBarColor;
        char NoStdSecondaryBarColor;
        char NoStdTextColor;
        char NoStdTextBarColor;
        
        TypeColor OptTitleColor;
        TypeColor OptMainColor;
        TypeColor OptSecondaryColor;
        TypeColor OptBarColor;
        TypeColor OptSecondaryBarColor;
        TypeColor OptTextColor;
        TypeColor OptTextBarColor;  
        
        char NoStdPosition;
        unsigned short int OptPosX1, OptPosY1;  
    };

    Se si vuole cambiare il valore del colore della barra, ad esempio, si deve 
    imporre:
            
       struct gCurl_FileTransfer_Options opt;
       
       memset (&opt, 0, sizeof (struct gCurl_FileTransfer_Options));
       opt.NoStdBarColor=1;
       opt.OptBarColor=COLOR_LBLUE;
       
    Quando i flags NoStd..Color non sono ad 1, il sistema usa le impostazioni
    standard. 
    
    Il flag NoStdPosition si pone ad 1 quando si vuole che la posizione della
    nuova finestra non sia determinata dal Position Generator automaticamente,
    ma sia impostata manualmente ai valori OptPosX1, OptPosY1 
    
    ERROR CODES:
    ------------
    
    La routine restituisce il valore 0 se non ci sono errori, ed un valore 
    a 32 bit se invece ci sono degli errori.
    
    I codici di errore possibili sono:
             
    ERR_GCURL_EASY_INIT_FAILED
    Errore nell'allocare la struttura curl
    
    ERR_GCURL_CBCONTAINER_MALLOC_FAILED     
    Il sistema ha fallito nell'allocare la memoria per il container callback
    
    ERR_GCURL_DIALOGBOX_FAILED
    Problemi nel creare la dialog box
    
    ERR_GCURL_ABORTED_BY_USER
    Il processo di trasferimento è stato abortito dall'utente

    ERR_CURL_TROUBLE
    C'è stato un problema interno alla libreria Curl. In realtà, quando si
    verifica questa condizione, i 6 nibble meno significativi del codice
    di ritorno sono riservati al codice interno di errore Curl. 
    Pertanto, questa condizione di errore si verifica con un
              Err & ERR_CURL_TROUBLE == ERR_CURL_TROUBLE
    e se il controllo riesce si esegue un 
              Err & 0x00FFFFFF 
    per trovare il codice di errore interno curl riportato dalla libreria

*/ 

{
      int       ErrRep;
      
      CURL       *MyCurl;                    // Necessario per comandi CURL
      CURLcode   MyCurlResult;               // Risultato trasferimento CURL
      
      struct gCurl_FileTransfer_cbContainer *MyCbContainer;
      struct gCurl_FileTransfer_IntOptions  *MyIntOptions;
      
      MyCurl = curl_easy_init();
      
      if(MyCurl) 
      {
                  MyCbContainer = malloc (sizeof (struct gCurl_FileTransfer_cbContainer));
                  // Alloca un'area di memoria apposita per il container delle callback
                  
                  if (MyCbContainer)   // Nessun errore
                  {       
                              memset (MyCbContainer, 0, sizeof (struct gCurl_FileTransfer_cbContainer));
                              
                              // STEP 1: Iniziamo a copiare le informazioni necessarie dentro il container
                              // delle callbacks
                              
                              // Copiamo il nome del file nella struct opportuna
                              MyCbContainer->FileName=UrlSrcFile;
                              MyCbContainer->FileDest=DstFile;
                        
                              // STEP 2: Crea la finestra di monitoraggio
                              ErrRep=INTERNAL_FileTransfer_CreateWnd (MyCbContainer, Options);
                              
                              if (!ErrRep)
                              {        
                                      // STEP 3: Programmiamo la libreria curl
                                      
                                      // Impostiamo il proxy server
                                      INTERNAL_FileTransfer_SetProxyServer (MyCbContainer, Options, NetObj, MyCurl);
                                      
                                      // Definiamo la posizione del file sorgente
                                      curl_easy_setopt(MyCurl, CURLOPT_URL, UrlSrcFile);
                                
                                      // Impostiamo la callback di scrittura
                                      curl_easy_setopt(MyCurl, CURLOPT_WRITEFUNCTION, &INTERNAL_FileTransfer_cbWrite);
                                      curl_easy_setopt(MyCurl, CURLOPT_WRITEDATA, MyCbContainer);
                                      
                                      // Impostiamo la callback di monitoraggio
                                      curl_easy_setopt(MyCurl, CURLOPT_NOPROGRESS, FALSE);
                                      curl_easy_setopt(MyCurl, CURLOPT_PROGRESSFUNCTION, INTERNAL_FileTransfer_cbProgress);
                                      curl_easy_setopt(MyCurl, CURLOPT_PROGRESSDATA, MyCbContainer);
                                      
                                      // Attiva il protocollo di debug se necessario
                                      INTERNAL_FileTransfer_SetVerboseMode (Options, MyCurl);
                                      
                                      // AVVIO DEL PROCESSO DI TRASFERIMENTO
                                      MyCurlResult = curl_easy_perform (MyCurl);
                                      
                                      if (MyCurlResult==0)     // Ok, il sistema è riuscito nel trasferimento   
                                      {
                                          INTERNAL_FileTransfer_EndTransfer_SUCCESS (MyCbContainer, Options);
                                          curl_easy_cleanup (MyCurl);
                                          free (MyCbContainer);
   
                                          return 0;            // Segnalalo alla routine chiamante                
                                      }
                                      else   // C'è stato un errore o una interruzione che il sistema deve gestire
                                      {
                                          if (MyCbContainer->AbortRequested==0)   // E' un semplice errore curl interno
                                          {
                                              INTERNAL_FileTransfer_EndTransfer_CURLERROR (MyCbContainer, Options, MyCurlResult);
                                              curl_easy_cleanup (MyCurl);
                                              free (MyCbContainer);
                                              
                                              MyCurlResult = MyCurlResult & 0x00FFFFFF;
                                              return (ERR_CURL_TROUBLE | MyCurlResult);               
                                          }                     
                                          else  // L'utente ha richiesto una interruzione del trasferimento
                                          {
                                              INTERNAL_FileTransfer_EndTransfer_USERABORTED (MyCbContainer, Options);
                                              curl_easy_cleanup (MyCurl);
                                              free (MyCbContainer);
   
                                              return ERR_GCURL_ABORTED_BY_USER;   // Segnala alla routine chiamante che l'utente ha interrotto il processo
                                          }
                                      }
                              }
                              else   // Non è riuscito ad allocare la finestra richiesta
                              {
                                  curl_easy_cleanup(MyCurl);
                                  free (MyCbContainer);
                                  
                                  return ERR_GCURL_DIALOGBOX_FAILED;          
                              }
                  }
                  else   // Problema nell'allocazione della memoria per MyCBContainer
                  {
                      curl_easy_cleanup(MyCurl);
                      return ERR_GCURL_CBCONTAINER_MALLOC_FAILED;  
                  }
      }     
      else  // Problema nella creazione della struttura curl
      {
          return ERR_GCURL_EASY_INIT_FAILED;
      }    
}


