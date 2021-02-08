/* 
    ----------------------------------------------------------------------------
    NanoDesktop HighGUI (ndHighGUI) Library - Version 0.2
    Copyright 2007-2012 Filippo Battaglia 
    ----------------------------------------------------------------------------
    This is the porting of HighGUI (the component of Intel (R) OpenCV Libraries
    that allows use of windows environment), for Nanodesktop windows manager.
    
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
    ROUTINES FOR WINDOWS MANAGER
    ------------------------------------------------------------------------------
*/

#include <$_NDHGUI_for_NDHGUI.h>


char INTERNAL_FindWndByName (char* NameWindowToSearch)
/* ----------------------------------------------------------------------------
   Routine INTERNAL_FindWndByName                                 Privato
   ----------------------------------------------------------------------------
   Questa routine ritorna alla routine chiamante l'handler della finestra che ha
   un nome pari a NameWindowToSearch.
   
   Occorre fare attenzione al fatto che INTERNAL_FindWndByName restituisce solo
   l'handler di finestre che sono state create con ndHighGUI. Le altre
   finestre sono ignorate.
   
   Questa routine è progettata esclusivamente per uso interno: per tale ragione
   essa non esegue notifiche dell'eventuale errore di finestra non trovata e
   non deposita l'errore in HGUI_SystemError. 
   
   Al contrario, questa routine restituisce in uscita l'handle della finestra e,
   ove la finestra richiesta non fosse trovata, restituisce al programma 
   chiamante un codice messaggio NDHGUI_MSG_WINDOW_NOT_FOUND
   ----------------------------------------------------------------------------
*/

{
    unsigned char Counter;
    int UserAdr, UserSize;
        
    for (Counter=0; Counter<=(__NRWINDOWSALLOCABLE-1); Counter++)
    {
        if (WindowData [Counter].WindowIsActive)
        {
           if ( !strcmp (WindowData [Counter].WindowTitle, NameWindowToSearch) ) 
           {
                // Il nome corrisponde, però ora bisogna vedere se c'è l'identificatore CVWND
                
                ndLP_GetUserAreaInfo (Counter, &UserAdr, &UserSize);
                if (!strcmp (UserAdr, "CVWND")) return Counter;
                
                // Se le due stringhe sono uguali ritorna l'handle trovato
           }              
        }                                                                           
    }
   
    // Non è stata trovata la finestra. Segnala l'errore:
    
    return NDHGUI_MSG_WINDOW_NOT_FOUND;    
}




char cvFindWindowByName (char* NameWindowToSearch)
/* ----------------------------------------------------------------------------
   Routine cvFindWindowByName                                 Pubblica
   ----------------------------------------------------------------------------
   Questa routine ritorna alla routine chiamante l'handler della finestra che ha
   un nome pari a NameWindowToSearch.
   
   Occorre fare attenzione al fatto che INTERNAL_FindWndByName restituisce solo
   l'handler di finestre che sono state create con ndHighGUI. Le altre
   finestre sono ignorate.
   
   Questa routine è progettata per essere richiamata da altre routines 
   OpenCV: se la finestra non viene trovata, viene restituito il valore -1
   in uscita, non prima però di avere notificato l'errore all'utente. 
   
   L'eventuale codice di errore viene memorizzato nella variabile di ambiente
   HGUI_SystemError. E' previsto solo il codice di errore:
   
   NDHGUI_WINDOW_NOT_FOUND       La finestra non è stata trovata
   ----------------------------------------------------------------------------
*/

{
   char Result;
   
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
   Result=INTERNAL_FindWndByName (NameWindowToSearch);
   // Usa una routine interna di HighGUI. Se non trova la finestra restituirà
   // un codice messaggio opportuno.
   
   if (Result!=NDHGUI_MSG_WINDOW_NOT_FOUND)
   {
       HGUI_SystemError=0;       
       return Result;            // Restituisci l'handle trovato     
   }
   else
   {
       HGUI_NotifyError (-2, 0, "cvFindWindow", "Window not found");
           
       HGUI_SystemError=NDHGUI_WINDOW_NOT_FOUND;
       return -1; 
   }
}





int cvNamedWindow (char *NameWindow, ndint64 flags)
/* -------------------------------------------------------------------------------------------------
    ROUTINE cvNamedWindow                                  Pubblica
   -------------------------------------------------------------------------------------------------
   Questa routine apre una finestra sullo schermo, analogamente alle routine cvNameWindow delle
   OpenCV.
   
   La routine cvNameWindow delle OpenCV ha questa definizione (da manuale):
              
              int cvNamedWindow( const char* name, unsigned long flags );
              name     Name of the window which is used as window identifier and appears in the window caption. 
              flags    Defines window properties. Currently the only supported property is ability 
                       to automatically change the window size to fit the image being hold by the window. 
                       Use CV_WINDOW_AUTOSIZE for enabling the automatical resizing or 0 otherwise. 
                       The function cvNamedWindow creates a window which can be used as a placeholder 
                       for images and trackbars. Created windows are reffered by their names.

    Il comportamento della routine viene emulato nel seguente modo.
    
    Nei sistemi Windows la finestra appare in un punto che viene determinato da una routine
    apposita. Inoltre, le dimensioni sono inizialmente settate ad un valore fisso. 
    
    Nel caso della cvNamedWindow, la posizione del punto in alto a sinistra della finestra 
    viene determinata dal generatore PosGen di Nanodesktop e la finestra ha dimensione fissa. 
    
    La lunghezza e la larghezza della finestra sono determinate dai parametri di ambiente 
    HGUI_DefaultLenX e HGUI_DefaultLenY. Ove la finestra finisse fuori schermo, essa viene ridimensionata
    automaticamente in modo da avere la massima dimensione possibile. Per cambiare il valore dei
    parametri HGUI_DefaultLenX e HGUI_DefaultLenY, si veda la funzione cvChangeDefaultWindowDim.
    
    Il parametro flag è mantenuto solo per compatibilità con il normale codice OpenCV: le
    finestre ignorano sempre il parametro ed eseguono sempre l'Autosize.
    
    La routine cvNamedWindow di Intel non prevede che il programmatore possa settare le 
    dimensioni o la posizione della finestra. In realtà, la nostra versione delle cvNamedWindow
    consente di settare ugualmente questi valori, senza che sia necessario modificare il
    prototipo della funzione. Questo assicura la retrocompatibilità necessaria per il normale
    codice di Intel. 
    
    La chiave sta nel parametro flags, che nelle ndOpenCV è un parametro a 64 bit (a differenza
    delle Intel OpenCV dove questo è un parametro a 32 bit). 
    
    Questi 64 bit sono mappati nel seguente modo:
           
      Bit        Valore
    ---------    -----------------------------------------------------------------------------------
       63        Impostato a 1 se è impostato il force della posizione
       62-52     Codice ad 11 bit che contiene la PosX (0-2047) della PosX1 della finestra
       51-41     Codice ad 11 bit che contiene la PosY (0-2047) della PosY1 della finestra
       40        Impostato a 1 se è impostato il force della width o height della finestra
       39-29     Codice ad 11 bit che contiene la Length (0-2047) della finestra
       28-18     Codice ad 11 bit che contiene la Height (0-2047) della finestra     
       17        MenuSupported: crea una finestra Nanodesktop in grado di supportare i menù
       16        UseNanodesktopWSWnd: obbliga il sistema ad usare la capacità nanodesktop wswnd
       15        Impostato a 1 se è abilitata l'opzione FORCE
       
    Utilizzando questi codici, l'utente può settare le coordinate della finestra che deve
    essere creata sullo schermo. Si noti che se l'utente eseguisse una chiamata 
    tradizionale secondo le specifiche di Intel, il sistema potrebbe ugualmente gestire
    la situazione, perchè i bit 63 e 40 sarebbero impostati a 0. 
    
    L'utente non deve calcolare manualmente i valori corretti, ma si può limitare ad
    usare le funzioni CVKEY_SETPOS (PosX, PosY) e CVKEY_SETSIZE (LenX, LenY) che
    si occupano di creare i codici a 64 bit necessari, nonchè le costanti manifeste
    CV_MENUSUPPORTED, CV_USE_NDWSWND e CV_FORCE.
    
    L'opzione CV_USE_NDWSWND permette a ndHighGUI di usare la capacità di Nanodesktop di
    usare un window space più ampio delle dimensioni dell'immagine. Per ragioni di
    carattere tecnico, se si usa questa opzione, non sarà possibile associare delle
    trackbar all'immagine.
    
    Per esempio:
        
    cvNamedWindow ("Example", CVKEY_SETPOS (0, 0) | CVKEY_SETSIZE (100, 100) | CV_MENUSUPPORTED )
    
    crea la finestra nella posizione desiderata.
    
    L'opzione CV_FORCE è molto importante. Quando si usa la normale routine cvShowImage, il
    sistema provvede ad un resize della finestra in modo da adattarne le dimensioni alle
    necessità in base alle dimensioni dello schermo, alle dimensioni dell'immagine, ed
    al numero di trackbars. Se invece la finestra è stata allocata con una cvNamedWindow
    con l'opzione CV_FORCE, il sistema manterrà le dimensioni della finestra e la 
    posizione che erano state stabilite in partenza. In questo modo, l'utente può
    stabilire con precisione in quale punto dovrà essere visualizzata un'immagine,
    anche se questo non era previsto dalle routine originarie di Intel.
    
    La routine provvede anche ad allocare la memoria necessaria per il buffer cvtrackbars. 
    Questa memoria verrà disallocata quando si utilizzerà cvDestroyWindow.
    
    A differenza di quanto accade in ambiente Windows, cvNamedWindow crea delle finestre
    minimizzate. Tuttavia, routine come cvShowImage provvedono a massimizzare automaticamente
    la finestra.
    
    Se il processo va a buon fine, in uscita viene restituito il valore 1.
    
    Se si verifica un errore, in uscita viene restituito il valore 0 e la variabile di
    ambiente HGUI_SystemError viene settata al codice di errore previsto. 
    
    Per questa routine sono previsti i seguenti codici di errore:
        NDHGUI_WRONG_POS_COORDINATES            Le coordinate della finestra sono errate
        NDHGUI_ERROR_IN_POSITION_GENERATOR      Errore nel generatore di posizione di Nanodesktop
        NDHGUI_MEMORY_ALLOCATION_ERROR          Errore nell'allocazione di memoria
        NDHGUI_UNKNOWN_ERROR                    Errore sconosciuto
        NDHGUI_NO_WINDOW_IS_AVAILABLE           Nanodesktop non ha altre finestre disponibili
    -------------------------------------------------------------------------------------------------
*/
{
     char WndHandle, ErrRep;
     char Counter;
                                 
     unsigned short int PosX1, PosY1, PosX2, PosY2;
     unsigned short int LenX, LenY;
     char MenuSupported, UseNdWSWnd, UseForce;
     unsigned short int ndAttribute;
     
     unsigned short int _ColorTitle, _ColorBGTitle, _ColorBorder, _ColorBGWS;
     
     int  UserAreaAddr; 
     int  UserAreaSize;
     struct cvWindowData_Type *New_cvWindowData;
     struct cvUserArea_Type   *cvUserArea;
     
     
     if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
     
     if ((flags & CV_FORCE)==CV_FORCE)
        UseForce=1;
     else
        UseForce=0;
         
     if ((flags & BIT40) == BIT40)    // Stai usando il force delle dimensioni della finestra?
     {
        flags = flags - BIT40;
        
        LenX = ((unsigned short int) (flags >> (40-11) )    & 0x7FF);
        LenY = ((unsigned short int) (flags >> (40-11-11) ) & 0x7FF);
     }
     else
     {
        LenX = HGUI_DefaultLenX;
        LenY = HGUI_DefaultLenY; 
     }
     
     if ((flags & BIT63) == BIT63)    // Stai usando il force della posizione?
     {
        flags = flags - BIT63;
        
        PosX1 = ((unsigned short int) (flags >> (63-11) )    & 0x7FF);
        PosY1 = ((unsigned short int) (flags >> (63-11-11) ) & 0x7FF);
        
        PosX2 = PosX1 + LenX;
        PosY2 = PosY1 + LenY;
        
        if ((PosX2>=Windows_MaxScreenX) || (PosY2>=Windows_MaxScreenY))
        {
           HGUI_NotifyError (-2, 0, "cvNamedWindow", "Wrong position coordinates");
           
           HGUI_SystemError = NDHGUI_WRONG_POS_COORDINATES;
           return 0;                             
        }
     }
     else   // No, usa il position generator interno di ndHighGUI che a sua volta richiamerà quello di 
     {
        ErrRep = ndPOSGEN_GenPosition (LenX, LenY, &PosX1, &PosY1, &PosX2, &PosY2);   
     
        if (ErrRep<0)  // Errore del position generator
        {
           HGUI_NotifyError (-2, 0, "cvNamedWindow", "Error in position generator");
           
           HGUI_SystemError = NDHGUI_ERROR_IN_POSITION_GENERATOR;
           return 0;      
        }              
     }
     
     if ((flags & CV_MENUSUPPORTED) == CV_MENUSUPPORTED)
        MenuSupported=1;
     else
        MenuSupported=0;
        
     if ((flags & CV_USE_NDWSWND) == CV_USE_NDWSWND)
        UseNdWSWnd=1;
     else
        UseNdWSWnd=0;
        
     
     if (NrWindowsAllocated<__NRWINDOWSALLOCABLE-1)         // Il sistema riserva sempre una finestra per la finestra di
     {                                                      // errore HighGUI
         ErrRep=INTERNAL_FindWndByName (NameWindow);
          
         // Poichè non è consentito usare due finestre con lo stesso identico nome, il sistema
         // prima deve verificare che non sia già stata allocata una finestra con lo stesso nome
          
         if (ErrRep==NDHGUI_MSG_WINDOW_NOT_FOUND)  // La finestra non è stata trovata. Bene.
         {
              New_cvWindowData = malloc ( sizeof (struct cvWindowData_Type) );
              // Alloca lo spazio per la nuova cvWindowData
              
              if ( (int)(New_cvWindowData)!=0 )     // L'allocazione è andata a buon fine
              {
                    INTERNAL_ProvideColor ( &(_ColorTitle), &(_ColorBGTitle), &(_ColorBGWS), &(_ColorBorder) );
                    // Ottiene i colori necessari
                    
                    ndAttribute=AUTOFIX;
                    if (MenuSupported) ndAttribute=(ndAttribute | MENUSUPPORTED);
                    
                    WndHandle=ndLP_CreateWindow (PosX1, PosY1, PosX2, PosY2, NameWindow, _ColorTitle, _ColorBGTitle, _ColorBGWS, _ColorBorder, ndAttribute);
                    // Ignora l'impostazione di WSLength e WSHeight perchè tanto usa l'opzione AutoFix 
             
                    if (WndHandle>=0) 
                    {
                          ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);
                          
                          // ndHighGUI usa la UserArea per memorizzare delle informazioni: i primi
                          // 6 byte sono un identificatore CVWND che consente ad ndHighGUI di
                          // distinguere le finestre che sono state create da ndHighGUI e quelle
                          // che sono state create da altri software.
                          
                          strcpy ( &(cvUserArea->Label), "CVWND" );
                          cvUserArea->cvWindowData = New_cvWindowData;
                          
                          // Adesso impostiamo i valori di default 
                          
                          cvUserArea->cvWindowData->WndStateIsChanged = 1;
                          // Questo farà sì che al prossimo cvShowImage si attivi il trackbar renderer
                          
                          cvUserArea->cvWindowData->CanUseNanodesktopWSWnd = UseNdWSWnd;
                          cvUserArea->cvWindowData->UseForce               = UseForce;
                          cvUserArea->cvWindowData->NrcvTrackBarsAllocated = 0;
                          cvUserArea->cvWindowData->LastIplImage           = 0;
                          
                          for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
                          {
                              cvUserArea->cvWindowData->cvTrackBarData [Counter].IsActive=0;
                              strcpy (&(cvUserArea->cvWindowData->cvTrackBarData [Counter].TBName), "");
                          }
                          
                          HGUI_SystemError=0;         // Nessun errore 
                          return 1;                   // Ritorna successo
                    }
                    else
                    {
                          HGUI_NotifyError (-2, 0, "cvNamedWindow", "Unknown error in window creation");
                     
                          HGUI_SystemError=NDHGUI_UNKNOWN_ERROR; 
                          return 0;
                    }
             }
             else
             {
                  HGUI_NotifyError (-2, 0, "cvNamedWindow", "Error in memory allocation");
                          
                  HGUI_SystemError=NDHGUI_MEMORY_ALLOCATION_ERROR;       
                  return 0;
             }             
         }
         else   // C'era già una finestra con lo stesso identico nome. Simula successo.
         {
                HGUI_SystemError=0;
                return 1;
         }
     }
     else
     {
         HGUI_NotifyError (-2, ERR_NO_WINDOW_IS_AVAILABLE, "cvNamedWindow", "Frame window can not be created");
         
         HGUI_SystemError=NDHGUI_NO_WINDOW_IS_AVAILABLE;
         return 0;  
     }
     
}



void cvDestroyWindow (char *NameWindow)
/*  -----------------------------------------------------------------------------
    Routine cvDestroyWindow                                    Pubblica
    ----------------------------------------------------------------------------- 
    Questa routine distrugge la finestra con un dato nome. Il comportamento
    è identico a quello della cvDestroyWindow delle OpenCV per Windows.
    
    La routine ignora qualsiasi finestra che non sia stata creata con 
    cvNamedWindow.
    
    Sono previsti i seguenti codici di errore:
        NDHGUI_WINDOW_NOT_FOUND     La finestra con un certo nome non è stata 
                                    trovata 
    -----------------------------------------------------------------------------
*/

{
    struct cvUserArea_Type *cvUserArea;
    int UserAreaSize;
   
    char WndHandle;
    
    if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
    WndHandle=INTERNAL_FindWndByName (NameWindow);
     
    // La routine deve innanzitutto individuare l'handler della finestra 
     
    if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)    // la finestra con un identico nome è stata trovata
    {    
        ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);
        free (cvUserArea->cvWindowData);     // Disalloca l'area di memoria che era stata usata per 
                                             // memorizzare le informazioni HighGUI.
         
        // Non è necessario ripulire la UserArea perchè tanto provvederà Nanodesktop alla
        // prossima allocazione dell'handler.  
         
        ndLP_DestroyWindow (WndHandle);  // La finestra viene distrutta
        
        HGUI_SystemError=0;
        return;
    }
    else
    {
        HGUI_SystemError=NDHGUI_WINDOW_NOT_FOUND;
        return;
    }
}



void cvDestroyAllWindows (void)
/*  -----------------------------------------------------------------------------
    Routine cvDestroyWindow                                    Pubblica
    ----------------------------------------------------------------------------- 
    Questa routine distrugge tutte le finestre allocate fino al momento. Il
    comportamento è identico a quello della cvDestroyWindow delle OpenCV per Windows.
    
    Attenzione perchè la routine distrugge esclusivamente le finestre che sono
    state allocate dalla libreria ndHighGUI: le altre finestre non vengono
    distrutte.
    
    Non sono previsti codici di errore per questa routine, ma la variabile
    HGUI_SystemError viene sempre reinizializzata a zero.
    -----------------------------------------------------------------------------
*/

{
    if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
    char Counter;
    int UserAdr, UserSize;
    
    if (NrWindowsAllocated>0)
    {
        for (Counter=NrWindowsAllocated-1; Counter>=0; Counter--)
        {
            if (WindowData [Counter].WindowIsActive)
            {
                  ndLP_GetUserAreaInfo (Counter, &UserAdr, &UserSize);
                        
                  if (!strcmp (UserAdr, "CVWND")) cvDestroyWindow (WindowData [Counter].WindowTitle);         
                  // L'identificatore CVWND è stato trovato 
            }
        }
    }           
    
    HGUI_SystemError=0;            
    return;
}


void cvResizeWindow (char *NameWindow, int width, int height )
/* ------------------------------------------------------------------------------
   Routine cvResizeWindow                                         Pubblica
   ------------------------------------------------------------------------------
   Esegue il resize della finestra
   
   Per questa routine è previsto il codice di errore:
       NDHGUI_I_CANNOT_RESIZE_WINDOW  Resize della finestra impossibile
   ------------------------------------------------------------------------------
*/

{
    if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
    char WndHandle, ErrRep;
    
    WndHandle=INTERNAL_FindWndByName (NameWindow);
    // La routine deve innanzitutto individuare l'handle della finestra
    
    if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)     // la finestra con identico nome è stata trovata
    {
        ErrRep=ndLP_ResizeWindow (width, height, WndHandle);
    
        if (ErrRep<0)
        {
           HGUI_NotifyError (-2, -ErrRep, "cvResizeWindow", "Nanodesktop error");
           
           HGUI_SystemError = NDHGUI_I_CANNOT_RESIZE_WINDOW;
           return;
        }
        
        HGUI_SystemError=0;
        return;
    }
    else       // Finestra non trovata
    {
        HGUI_NotifyError (-2, 0, "cvResizeWindow", "Window not found");
           
        HGUI_SystemError = NDHGUI_WINDOW_NOT_FOUND;
        return;
    }
}       


void cvMoveWindow( char *NameWindow, int NewPosX, int NewPosY )
/* ------------------------------------------------------------------------------
   Routine cvMoveWindow                                         Pubblica
   ------------------------------------------------------------------------------
   Sposta la finestra in una nuova posizione.
   
   Per questa routine è previsto il codice di errore:
       NDHGUI_I_CANNOT_MOVE_WINDOW    Impossibile muovere la finestra
   ------------------------------------------------------------------------------
*/

{
    if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
    char WndHandle, ErrRep;
    
    WndHandle=INTERNAL_FindWndByName (NameWindow);
    // La routine deve innanzitutto individuare l'handle della finestra
    
    if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)     // la finestra con identico nome è stata trovata
    {
        ErrRep=ndLP_MoveWindowWithAutoResize (NewPosX, NewPosY, WndHandle);
    
        if (ErrRep<0)
        {
           HGUI_NotifyError (-2, -ErrRep, "cvMoveWindow", "NanoDesktop error");
           
           HGUI_SystemError = NDHGUI_I_CANNOT_MOVE_WINDOW;
           return;
        }
        
        HGUI_SystemError = 0;
        return;
    }
    else       // Finestra non trovata
    {
        HGUI_NotifyError (-2, 0, "cvMoveWindow", "Window not found");
           
        HGUI_SystemError = NDHGUI_WINDOW_NOT_FOUND;
        return;
    }
}       



char cvGetNdWindowHandler (char *name)
/* ---------------------------------------------------------------------------
   Routine cvGetNdWindowHandler                                   Pubblica
   ---------------------------------------------------------------------------
   Questa routine ricava l'ID di una finestra con un determinato nome.
   
   Il lavoro svolto è identico a INTERNAL_FindWndByName.
   ---------------------------------------------------------------------------
*/

{
   return (INTERNAL_FindWndByName (name));
}

char *cvGetNdWindowTitle (char WndHandle)
/* ---------------------------------------------------------------------------
   Routine cvGetNdWindowTitle                                       Pubblica
   ---------------------------------------------------------------------------
   Questa routine fornisce l'indirizzo del titolo della finestra identificata
   da WndHandle. Se la finestra non è stata ancora inizializzata, allora
   la routine fornisce 0 in uscita.
   
   Non sono previsti codici di errore in uscita.
   ---------------------------------------------------------------------------
*/

{
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
   if (WindowData [WndHandle].WindowIsActive)
      return ( &(WindowData [WndHandle].WindowTitle) );
   else
      return 0;
}





void cvMaximizeWindow (char *Name)
/* ----------------------------------------------------------------------------
   Routine cvMaximizeWindow                                           Pubblica
   ----------------------------------------------------------------------------
   Questa routine massimizza la finestra identificata da Name.
   
   Sono previsti i seguenti codici di errore:
       NDHGUI_WINDOW_NOT_FOUND          La finestra non è stata trovata
       NDHGUI_I_CANNOT_MAXIMIZE_WINDOW  Non è possibile massimizzare la finestra
   ----------------------------------------------------------------------------
*/

{
  char WndHandle;
  char ErrRep;
  
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  WndHandle = INTERNAL_FindWndByName (Name);
  
  if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)
  {
     ErrRep=ndLP_MaximizeWindow (WndHandle);
     
     if (!ErrRep)
     {
         HGUI_SystemError=0;              // Tutto a posto
         return;
     }
     else
     {
         HGUI_NotifyError (-2, -ErrRep, "cvMaximizeWindow", "I cannot maximize window");
         
         HGUI_SystemError=NDHGUI_I_CANNOT_MAXIMIZE_WINDOW;
         return;
     }
  }
  else   // La finestra non è stata trovata
  {
     HGUI_NotifyError (-2, 0, "cvMaximizeWindow", "Window not found");
           
     HGUI_SystemError = NDHGUI_WINDOW_NOT_FOUND;
     return;
  } 
}



void cvMinimizeWindow (char *Name)
/* ----------------------------------------------------------------------------
   Routine cvMinimizeWindow                                           Pubblica
   ----------------------------------------------------------------------------
   Questa routine minimizza la finestra identificata da Name. Sono previsti i
   seguenti codici di errore:
            
            NDHGUI_WINDOW_NOT_FOUND          La finestra non è stata trovata         
            NDHGUI_I_CANNOT_MINIMIZE_WINDOW  Non posso minimizzare la finestra
   ----------------------------------------------------------------------------
*/

{
  char WndHandle;
  char ErrRep;
  
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  WndHandle = INTERNAL_FindWndByName (Name);
  
  if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)
  {
     ErrRep=ndLP_MinimizeWindow (WndHandle);
     
     if (!ErrRep)
     {
         HGUI_SystemError=0;              // Tutto a posto
         return;
     }
     else
     {
         HGUI_NotifyError (-2, -ErrRep, "cvMinimizeWindow", "I cannot minimize window");
         
         HGUI_SystemError=NDHGUI_I_CANNOT_MINIMIZE_WINDOW;
         return;
     }
  }
  else   // La finestra non è stata trovata
  {
     HGUI_NotifyError (-2, 0, "cvMinimizeWindow", "Window not found");
           
     HGUI_SystemError = NDHGUI_WINDOW_NOT_FOUND;
     return;
  } 
  
}







void INTERNAL_ndHGUI_TrackBar_Manager (float fValue, int TB_InfoField, unsigned char WndHandle)
/*
   -------------------------------------------------------------------------------
   Routine INTERNAL_ndHGUI_TrackBar_Manager
   -------------------------------------------------------------------------------
   Questa routine è il gestore interno di trackbar di HighGUI. La routine riceve
   il valore della trackbar aggiornato, e si occupa di:
             
   - Aggiornare il valore anche nel processo utente, dopo averlo trasformato in
     int in modo da mantenere la coerenza interna
   - Eseguire l'eventuale HGUI_Callback settata da cvCreateTrackBar.
   -------------------------------------------------------------------------------
*/
{
   void (*UserCallBack)(int arg);
                                                                                  
   struct cvUserArea_Type *cvUserArea;
   int UserAreaSize;
     
   char HGUITBHandle;
   
   HGUITBHandle = (char) (TB_InfoField);
   // Ricava dal campo InfoField il numero di HGUITrackbar handle.
   
   ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);                                                                              
   // Adesso posso accedere alle informazioni necessarie
   
   cvUserArea->cvWindowData->cvTrackBarData [HGUITBHandle].Value = (int)(fValue);
   // Aggiorna il valore nell'HGUITrackbar record
   
   if (cvUserArea->cvWindowData->cvTrackBarData [HGUITBHandle].PointerToValue!=0)
   {
      *(cvUserArea->cvWindowData->cvTrackBarData [HGUITBHandle].PointerToValue)=(int)(fValue);
      // Aggiorna il valore nel programma chiamante                                          
   }
   
   if (cvUserArea->cvWindowData->cvTrackBarData [HGUITBHandle].UserCallBack!=0)
   {
      UserCallBack = cvUserArea->cvWindowData->cvTrackBarData [HGUITBHandle].UserCallBack;
      (*UserCallBack)(cvUserArea->cvWindowData->cvTrackBarData [HGUITBHandle].Value);
      
      // Esegue la callback impostata dall'utente per mezzo di cvCreateTrackBar
   }
   
}                               



CVAPI(int) cvCreateTrackbar( const char* trackbar_name, const char* window_name,
                             int* value, int count, CvTrackbarCallback on_change )
/*
   -------------------------------------------------------------------------------
   Routine cvCreateTrackbar                                        Pubblica
   -------------------------------------------------------------------------------
   Questa routine crea una trackbar con un certo nome, associandola alla 
   finestra con un certo nome. *Value è un puntatore ad un int che conterrà il
   valore della trackbar. On_change è l'indirizzo a 32 bit della callback che
   il sistema dovrà eseguire. 
   
   La routine viene emulata in modo quasi identico a quanto viene fatto nei sistemi
   x86 di Intel, salvo che per le seguenti limitazioni:
       
       la trackbar non potrà essere associata, se la finestra è stata creata 
       usando cvNamedWindow con l'opzione CV_USE_NDWSWND
       
       la nuova trackbar sarà visualizzata solo quando si mostrerà una nuova
       finestra con una nuova chiamata a cvShowImage: l'utente può forzare
       questo meccanismo tramite una chiamata a cvForceTrackbarRender, ma
       a suo rischio. Infatti, cvForceTrackBarRender provvede a visualizzare
       l'ultima immagine visualizzata, ma se questa non è più allocata,
       il sistema avrà un crash. 
       
       il nome della trackbar non può essere superiore a 31 caratteri: i
       caratteri in più vengono troncati.
       
   La routine restituisce 1 in caso di successo, e 0 in caso di insuccesso. 
   
   Se l'operazione non ha successo, verrà restituito 0 in uscita e nella
   variabili HGUI_SystemError verrà memorizzato il codice di errore. 
   
   Sono previsti i seguenti codici di errore.
        NDHGUI_WINDOW_NOT_FOUND                   La finestra non è stata trovata
        NDHGUI_TRACKBAR_IN_NDWSWND                Non è consentito creare tb 
                                                  in finestre NDWSWND
        NDHGUI_NO_TRACKBAR_HANDLE_AVAILABLE       Non ci sono altri trackbar handle
        NDHGUI_SAME_TRACKBAR_NAME_IS_PRESENT      E' già presente un trackbar name
                                                  identico 
   ---------------------------------------------------------------------------------
*/
{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  struct cvUserArea_Type *cvUserArea;
  int UserAreaSize;
  
  char WndHandle, Counter, IHaveFound, xA;
  
  char NameBuffer [32];
  
  WndHandle = INTERNAL_FindWndByName (window_name);
  
  if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)
  { 
     ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);
   
     if (!(cvUserArea->cvWindowData->CanUseNanodesktopWSWnd))
     {
        if (cvUserArea->cvWindowData->NrcvTrackBarsAllocated<__NRTRACKBARSALLOCABLE)
        {
           // Adesso è necessario controllare che non ci sia una trackbar con lo stesso
           // nome già allocata
           
           ndStrCpy (NameBuffer, trackbar_name, 31, 0);
           
           IHaveFound=0;
           for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
           {
               if ((!strcmp (NameBuffer, cvUserArea->cvWindowData->cvTrackBarData[Counter].TBName)) &&
                       (cvUserArea->cvWindowData->cvTrackBarData[Counter].IsActive))
               {
                       IHaveFound=1;
                       break;
               }
           }
           
           if (!IHaveFound)         // Non c'è una trackbar con lo stesso nome. Bene
           {
              // Ora ricerchiamo un handle libero
              
               IHaveFound=0;
               for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
               {
                      if (!cvUserArea->cvWindowData->cvTrackBarData[Counter].IsActive)
                      {
                               IHaveFound=1;
                               break;
                      }
               }  
               
               if (IHaveFound)   // Ha trovato un handle libero
               {
                      // Registriamo le informazioni necessarie
                      
                      xA=Counter;
                      
                      strcpy (cvUserArea->cvWindowData->cvTrackBarData[xA].TBName, NameBuffer);
                      cvUserArea->cvWindowData->cvTrackBarData[xA].PointerToValue = value;
                      cvUserArea->cvWindowData->cvTrackBarData[xA].Value          = 0;
                      cvUserArea->cvWindowData->cvTrackBarData[xA].MaxValue       = count;
                      cvUserArea->cvWindowData->cvTrackBarData[xA].UserCallBack   = on_change;
                      cvUserArea->cvWindowData->cvTrackBarData[xA].IsActive       = 1;
                      
                      cvUserArea->cvWindowData->WndStateIsChanged = 1;
                      // Segnala al sistema che la configurazione delle trackbar è cambiata
                      
                      cvUserArea->cvWindowData->NrcvTrackBarsAllocated++;
                      // Aumenta di 1 il numero delle trackbar allocate
                   
                      HGUI_SystemError = 0;     // Segnala successo
                      return 1;
                      
               }
               else
               {
                  HGUI_NotifyError (-2, 0, "cvCreateTrackbar", "No more trackbar handlers available");  
                  
                  HGUI_SystemError = NDHGUI_NO_TRACKBAR_HANDLE_AVAILABLE;
                  return 0; 
               }
           }
           else   
           {
               HGUI_NotifyError (-2, 0, "cvCreateTrackbar", "Same trackbar name is already present");  
               
               HGUI_SystemError = NDHGUI_SAME_TRACKBAR_NAME_IS_PRESENT;
               return 0;    
           }
           
        }
        else
        {
           HGUI_NotifyError (-2, 0, "cvCreateTrackbar", "No more trackbar handlers available");  
           
           HGUI_SystemError = NDHGUI_NO_TRACKBAR_HANDLE_AVAILABLE;
           return 0; 
        }
     }
     else
     {
        HGUI_NotifyError (-2, 0, "cvCreateTrackbar", "It is not allowed to create trackbar in NDWSWND window type");  
        
        HGUI_SystemError = NDHGUI_TRACKBAR_IN_NDWSWND;
        return 0;  
     }     
  }
  else
  {
     HGUI_NotifyError (-2, 0, "cvCreateTrackbar", "Window not found");  
     
     HGUI_SystemError = NDHGUI_WINDOW_NOT_FOUND;
     return 0; 
  }
}



int cvGetTrackbarPos( const char* trackbar_name, const char* window_name )
/*  ---------------------------------------------------------------------------
    Routine cvGetTrackbarPos                                       Pubblica            
    ---------------------------------------------------------------------------
    Questa routine restituisce al programma chiamante il valore della trackbar
    di nome trackbar_name, che sia stata allocata nella finestra di nome
    window_name.

    Se si verifica un errore, in uscita verrà restituito il valore -1 e verrà
    settata la variabile HGUI_SystemError al codice di errore corretto. 
    
    Sono previsti i seguenti codici di errore:
       NDHGUI_WINDOW_NOT_FOUND      La finestra non è stata trovata  
       NDHGUI_TRACKBAR_NOT_FOUND    La trackbar con il nome trackbar_name non è
                                    stata trovata 
    ---------------------------------------------------------------------------
*/
{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  struct cvUserArea_Type *cvUserArea;
  int UserAreaSize;
  char Counter, WndHandle;
  
  WndHandle = INTERNAL_FindWndByName (window_name);
  
  if (HGUI_SystemError!=NDHGUI_MSG_WINDOW_NOT_FOUND)
  { 
     ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);
  
     for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
     {
         if (cvUserArea->cvWindowData->cvTrackBarData [Counter].IsActive)
         {
             if (!strcmp (cvUserArea->cvWindowData->cvTrackBarData [Counter].TBName, trackbar_name))
             {
                 HGUI_SystemError = 0;
                 return  (cvUserArea->cvWindowData->cvTrackBarData [Counter].Value);      
             }
         }
     }
     
     // Se il sistema arriva in questo punto, significa che la trackbar non è stata trovata.
     // Segnaliamo errore.
     
     HGUI_NotifyError (-2, 0, "cvGetTrackbarPos", "Trackbar name not found");  
     
     HGUI_SystemError=NDHGUI_TRACKBAR_NOT_FOUND; 
     return -1;
  } 
  else   // La finestra non è stata trovata
  {
     HGUI_NotifyError (-2, 0, "cvGetTrackbarPos", "Window not found");  
     
     HGUI_SystemError=NDHGUI_WINDOW_NOT_FOUND;
     return -1;    
  }  
   
} 




void cvSetTrackbarPos( const char* trackbar_name, const char* window_name, int Pos )
/*  ---------------------------------------------------------------------------
    Routine cvSetTrackbarPos                                       Pubblica            
    ---------------------------------------------------------------------------
    Questa routine imposta la trackbar di nome trackbar_name nella finestra
    window_name ad un determinato valore Pos. 
    
    Se si verifica un errore, verrà settata la variabile HGUI_SystemError 
    al codice di errore corretto. 
    
    Sono previsti i seguenti codici di errore:
       NDHGUI_WINDOW_NOT_FOUND      La finestra non è stata trovata  
       NDHGUI_TRACKBAR_NOT_FOUND    La trackbar con il nome trackbar_name non è
                                    stata trovata 
       NDHGUI_OUT_OF_BOUND          Il valore Pos è al di fuori dei limiti consentiti
       NDHGUI_ERR_ND_TBAR_SETVALUE  Errore nd nel settare il valore della tbar
    ---------------------------------------------------------------------------
*/
{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  struct cvUserArea_Type *cvUserArea;
  int    UserAreaSize;
  
  char Counter, WndHandle, TBHandler, ErrRep;
  
  WndHandle = INTERNAL_FindWndByName (window_name);
  
  if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)
  { 
     ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);
  
     for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
     {
         if (cvUserArea->cvWindowData->cvTrackBarData [Counter].IsActive)
         {
             if (!strcmp (cvUserArea->cvWindowData->cvTrackBarData [Counter].TBName, trackbar_name))
             {
                 // Cambiamo il valore della trackbar. Qui non si usa l'apposita routine Nanodesktop
                 // perchè le trackbar nanodesktop vengono create on-demand quando viene eseguito
                 // un cvShowImage. 
                 
                 if ((Pos>=0) && (Pos<=cvUserArea->cvWindowData->cvTrackBarData [Counter].MaxValue))
                 {
                    cvUserArea->cvWindowData->cvTrackBarData [Counter].Value=Pos;
                    
                    // Sincronizziamo con il valore gestito dalla routine utente.
                    *(cvUserArea->cvWindowData->cvTrackBarData [Counter].PointerToValue)=Pos;
                    
                    // Per consentire la visualizzazione della nuova trackbar, devi eseguire l'apposita
                    // routine Nanodesktop. ndTBAR_SetValue si occuperà anche di eseguire l'eventuale
                    // callback.
                    
                    TBHandler = cvUserArea->cvWindowData->cvTrackBarData [Counter].NDHandle;
                    ErrRep=ndTBAR_SetValue ( (float) (Pos), TBHandler, WndHandle, RENDER); 
                    
                    if (!ErrRep)
                    {
                       HGUI_SystemError=0;           // Segnala successo
                       return;         
                    }
                    else
                    {
                       HGUI_NotifyError (-2, -ErrRep, "cvSetTrackbarPos", "Error in Nanodesktop trackbar ndTBAR_SetValue ");  
                       
                       HGUI_SystemError=NDHGUI_ERR_ND_TBAR_SETVALUE;
                       return;  
                    }
                    
                 } 
                 else // Il valore richiesto non è corretto
                 {
                     HGUI_NotifyError (-2, 0, "cvSetTrackbarPos", "Value out of bounds");  
                     
                     HGUI_SystemError=NDHGUI_OUT_OF_BOUND;
                     return;  
                 }
                       
             }
         }
     }
     
     // Se il sistema arriva in questo punto, significa che la trackbar non è stata trovata.
     // Segnaliamo errore.
     
     HGUI_NotifyError (-2, 0, "cvSetTrackbarPos", "Trackbar name not found");  
     
     HGUI_SystemError=NDHGUI_TRACKBAR_NOT_FOUND; 
     return; 
  } 
  else   // La finestra non è stata trovata
  {
     HGUI_NotifyError (-2, 0, "cvSetTrackbarPos", "Window not found");  
     
     HGUI_SystemError=NDHGUI_WINDOW_NOT_FOUND;
     return;    
  }   
} 



char cvFindTrackbarByName (const char* trackbar_name, const char* window_name)
/*  ---------------------------------------------------------------------------
    Routine cvFindTrackbarByName                                       Pubblica            
    ---------------------------------------------------------------------------
    Questa routine restituisce il nanodesktop TBHandler della trackbar 
    il cui nome è dato da trackbar_name e che sia stata associata alla
    finestra window_name.
    
    Sono riconosciute solo le trackbar che sono state create da ndHighGUI.
    
    Se si verifica un errore, in uscita viene restituito il valore -1 
    ed il codice di errore viene memorizzato nella variabili HGUI_SystemError.
    
    Sono previsti i seguenti codici di errore:
       NDHGUI_WINDOW_NOT_FOUND      La finestra non è stata trovata  
       NDHGUI_TRACKBAR_NOT_FOUND    Trackbar non trovata
    ---------------------------------------------------------------------------
*/
{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  struct cvUserArea_Type *cvUserArea;
  int UserAreaSize;
  
  char Counter, WndHandle;
  
  WndHandle = INTERNAL_FindWndByName (window_name);
  
  if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)
  { 
     ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);
  
     for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
     {
         if (cvUserArea->cvWindowData->cvTrackBarData [Counter].IsActive)
         {
             if (!strcmp (cvUserArea->cvWindowData->cvTrackBarData [Counter].TBName, trackbar_name))
             {   
                    HGUI_SystemError=0;   // Abbiamo trovato l'handle che ci interessa. Segnala successo
                    
                    return (cvUserArea->cvWindowData->cvTrackBarData [Counter].NDHandle);               
             } 
         }
     }
     
     // Se il sistema arriva in questo punto, significa che la trackbar non è stata trovata.
     // Segnaliamo errore.
     
     HGUI_NotifyError (-2, 0, "cvFindTrackbarByName", "Trackbar name not found");  
     
     HGUI_SystemError=NDHGUI_TRACKBAR_NOT_FOUND;
     return -1;
  } 
  else   // La finestra non è stata trovata
  {
     HGUI_NotifyError (-2, 0, "cvFindTrackbarByName", "Window not found");  
     
     HGUI_SystemError=NDHGUI_WINDOW_NOT_FOUND;
     return -1;    
  }   
} 


void cvDestroyTrackbar (const char* trackbar_name, const char* window_name)
/*  ---------------------------------------------------------------------------
    Routine cvDestryTrackbar                                       Pubblica            
    ---------------------------------------------------------------------------
    Questa routine elimina una trackbar il cui nome è dato da trackbar_name
    e che sia stata associata alla finestra window_name.
    
    La nuova configurazione delle trackbars sarà aggiornata solo quando
    l'utente effettuerà una nuova chiamata a cvShowImage: l'utente può forzare
    questo meccanismo tramite una chiamata a cvForceTrackbarRender, ma
    a suo rischio. Infatti, cvForceTrackBarRender provvede a visualizzare
    l'ultima immagine visualizzata, ma se questa non è più allocata,
    il sistema avrà un crash. 
    
    Sono previsti i seguenti codici di errore:
        NDHGUI_WINDOW_NOT_FOUND     Finestra non trovata
        NDHGUI_TRACKBAR_NOT_FOUND   Trackbar non trovata 
    ---------------------------------------------------------------------------
*/
{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  struct cvUserArea_Type *cvUserArea;
  int UserAreaSize;
  
  char Counter, WndHandle;
  
  WndHandle = INTERNAL_FindWndByName (window_name);
  
  if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)
  { 
     ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);
  
     for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
     {
         if (cvUserArea->cvWindowData->cvTrackBarData [Counter].IsActive)
         {
             if (!strcmp (cvUserArea->cvWindowData->cvTrackBarData [Counter].TBName, trackbar_name))
             {
                cvUserArea->cvWindowData->cvTrackBarData [Counter].IsActive=0;   
                
                cvUserArea->cvWindowData->NrcvTrackBarsAllocated--;
                cvUserArea->cvWindowData->WndStateIsChanged=1;
                // In questo modo forza il renderer a rieseguire il render delle trackbars.
                
                // Non è necessario distruggere le trackbars a livello Nanodesktop con ndDestroyTrackbar
                // perchè tanto ad ogni riesecuzione del renderer di cvShowImage, le trackbar a livello
                // Nanodesktop vengono distrutte e ricostruite se c'è stato un cambiamento di stato.              
             
                HGUI_SystemError=0;    // Segnala successo
                return;
             } 
         }
     }
     
     // Se il sistema arriva in questo punto, significa che la trackbar non è stata trovata.
     // Segnaliamo errore.
     
     HGUI_NotifyError (-2, 0, "cvDestroyTrackbar", "Trackbar name not found");  
     
     HGUI_SystemError=NDHGUI_TRACKBAR_NOT_FOUND;
     return;
  } 
  else   // La finestra non è stata trovata
  {
     HGUI_NotifyError (-2, 0, "cvDestroyTrackbar", "Window not found");  
     
     HGUI_SystemError=NDHGUI_WINDOW_NOT_FOUND;
     return;    
  }   
} 



