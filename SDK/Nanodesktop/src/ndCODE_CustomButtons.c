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
    ----------------------------------------------------------------------------
    ROUTINES PER LA GESTIONE DEI PULSANTI CUSTOM
    ----------------------------------------------------------------------------
    Queste routine si occupano della gestione dei pulsanti custom, ossia dei
    pulsanti che vengono definiti dall'utente.
    
    Tutte le routines che si occupano di gestione dei pulsanti custom hanno
    un prefisso ndCTRL_ 
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

/*  
    ----------------------------------------------------------------------------
    FUNZIONI AUSILIARIE DI GESTIONE PULSANTI
    ----------------------------------------------------------------------------
*/

char ndCTRL_FindButtonHandleByStringID (char *StringID, char WndHandle)
/* 
    Questa routine restituisce il ButtonHandle di un pulsante caratterizzato da
    una certa StringID
    
    La routine restituisce i seguenti codici di errore:
       ERR_WINDOW_IS_NOT_ALLOCATED           La finestra non è allocata
       ERR_STRINGID_NOT_FOUND                StringID non trovata
*/        
{
   char Counter;       
          
   if (WindowData [WndHandle].WindowIsActive)
   {
      for (Counter=0; Counter<__NRBUTTONSALLOCABLE; Counter++)
      {
         if ( (WindowData [WndHandle].ButtonClass4_Data [Counter].ButtonIsActivated) 
         && (strcmp (StringID, WindowData [WndHandle].ButtonClass4_Data [Counter].StringID)==0) )
         return Counter;       
      }   
      
      // Se si arriva qui, vuol dire che la StringID non è stata trovata
      return ERR_STRINGID_NOT_FOUND;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;       
}



/*
    ----------------------------------------------------------------------------
    GESTORE BUTTON RENDERER
    ----------------------------------------------------------------------------
    Per la visualizzazione dei pulsanti, Nanodesktop fa uso di una speciale 
    routine grafica che è in grado di visualizzare pulsanti ombreggiati oppure
    pulsanti in modalità normale. Alcune differenze in questa routine impediscono
    di usare le routine grafiche standard e costringono alla creazione di
    routine speciali (che sono comunque tutte interne).
    ----------------------------------------------------------------------------
*/

TypeColor INTERNAL_ConvertToDarkGray (TypeColor Colore)
/*
    Questa routine trasforma un colore espresso per mezzo del relativo MagicNumber
    nel corrispondente colore a tono di grigio. Viene usato dal ButtonRenderer
    per visualizzare i pulsanti che sono stati inibiti
*/
{
    unsigned char Red, Green, Blue;
    unsigned short int Average;
    
    ndHAL_MagicNumberToRGB (Colore, &Red, &Green, &Blue);
    Average = ((Red+Green+Blue)/3) >> 2;
    
    return ndHAL_RGBToMagicNumber (Average, Average, Average);
}


void INTERNAL_ShowImageBNDark (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, 
                               unsigned char WndHandle)

{
   unsigned short int CounterX, CounterY;
   TypeColor ColorPixel;
   
   for (CounterY=0; CounterY<MyImage->LenY; CounterY++)
   {
       for (CounterX=0; CounterX<MyImage->LenX; CounterX++)
       {
           ndIMG_GetPixelFromImage (MyImage, CounterX, CounterY, &ColorPixel);
           ColorPixel = INTERNAL_ConvertToDarkGray (ColorPixel);
           
           ndWS_PutPixel (RRPosX+CounterX, RRPosY+CounterY, ColorPixel, WndHandle, NORENDER);
       }
   }     
}



void INTERNAL_ButtonRenderer (char ButtonHandle, char WndHandle,
                              char IsInhibited, char Rounded, char RenderNow)
{
    unsigned short int Q1LenX, Q1LenY;
    unsigned short int NrCharsVisualizable;
    unsigned short int MarginX, MarginY;
    char NrStringsNotVoid;
    
    unsigned short int LenString1, LenString2;
    char TempBuffer [512];
    char FntHandle, FntSize;
    char YouCanExit;
    
    int  SizeRequestedX, SizeRequestedY;
    
    ndint64 TextCode;
    
    TypeColor __ColorButton, __ColorBorderButton, __ColorText;
    
    // Per prima cosa dobbiamo visualizzare il rettangolo che contiene il pulsante
    
    if (!IsInhibited) __ColorText = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorText;
       else __ColorText =  INTERNAL_ConvertToDarkGray (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorText);
   
    if (!IsInhibited) __ColorButton = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorButton;
       else __ColorButton =  INTERNAL_ConvertToDarkGray (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorButton);
   
    if (!IsInhibited) __ColorBorderButton = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorBorderButton;
       else __ColorBorderButton =  INTERNAL_ConvertToDarkGray (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorBorderButton);
   
    if (!Rounded)
    {
       ndWS_DrawRectangle ( WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1,
                            WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1,
                            WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX2,
                            WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY2,
                            __ColorButton, __ColorBorderButton, WndHandle, NORENDER);
    }
    else
    {
       ndWS_DrawSpRectangle ( WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1,
                            WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1,
                            WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX2,
                            WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY2,
                            __ColorButton, __ColorBorderButton, ND_ROUNDED, WndHandle, NORENDER);
    }
    
    // Mostriamo l'icona
    
    if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data!=0)
    {
       if (!IsInhibited)
       { 
         ndIMG_ShowImage (&(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage),
                         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1 +
                         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosX1,
                         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1 +
                         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosY1,
                         WndHandle, NORENDER);
       }
       else
       {
          INTERNAL_ShowImageBNDark (&(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage),
                         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1 +
                         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosX1,
                         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1 +
                         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosY1,
                         WndHandle);
       }    
                         
    }
    
    // Adesso visualizziamo le label del pulsante
    
    Q1LenX = abs (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX2 - WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX1);
    Q1LenY = abs (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY2 - WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY1);
    
    
    
    NrStringsNotVoid=0;
    LenString1 = strlen (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton1);
    if (LenString1>0) NrStringsNotVoid++;
    
    LenString2 = strlen (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton2);
    if (LenString2>0) NrStringsNotVoid++;
    
    FntHandle = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].FntHandle;
    
    if (!FontHandlesArray [FntHandle].IsTrueType)
    {    
            NrCharsVisualizable = Q1LenX/8;         
         
            if (NrStringsNotVoid>0)
            {
               MarginY = (Q1LenY - (8*NrStringsNotVoid))/2;
               
               ndStrCpy (TempBuffer, WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton1, NrCharsVisualizable, 0);
               
               MarginX = (Q1LenX - 8*strlen(TempBuffer))/2;
               
               TextCode = NDKEY_FONT (FntHandle);
               
               ndWS_GrWriteLn (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1 +
                                     WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX1 + MarginX,
                                     WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1 +
                                     WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY1 + MarginY,
                                     &TempBuffer, __ColorText, __ColorButton, WndHandle, TextCode | NORENDER);
                                     
               if (NrStringsNotVoid>1)
               {
                  ndStrCpy (TempBuffer, WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton2, NrCharsVisualizable, 0);
                  
                  MarginX = (Q1LenX - 8*strlen(TempBuffer))/2;
                  
                  ndWS_GrWriteLn (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1 +
                                        WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX1 + MarginX,
                                        WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1 +
                                        WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY1 + 8 + MarginY,
                                        &TempBuffer, __ColorText, __ColorButton, WndHandle, TextCode | NORENDER);                   
               }
            }
    }
    else     // Il font è di tipo True Type
    {
        #ifdef ND_ENABLE_TTF_SUPPORT    
            
            // Recupera la size del font
            
            FntSize = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].FntSize;
            if (FntSize==0) FntSize=16;
            
            // Verifica quanti caratteri è possibile inserire
            
            YouCanExit          = 0;
            NrCharsVisualizable = strlen (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton1);
            
            while (!YouCanExit)
            {
                 ndStrCpy (TempBuffer, WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton1, NrCharsVisualizable, 0);
               
                 ndFLIB_StrLength (TempBuffer, FntHandle, FntSize, &SizeRequestedX, &SizeRequestedY);
                 
                 if (SizeRequestedY<Q1LenX) break;        // Ok, abbiamo trovato la configurazione corretta
                 
                 // Non è sufficiente lo spazio: prova ad accorciare la stringa
                 
                 NrCharsVisualizable--; 
                 if (NrCharsVisualizable==0) break;      // Non si può rappresentare nemmeno un carattere
            }
            
            
            if ((NrStringsNotVoid>0) && (NrCharsVisualizable>0))
            {
               MarginX = (Q1LenX - SizeRequestedX)/2;
               MarginY = (Q1LenY - SizeRequestedY)/2;
               
               TextCode = NDKEY_FONT (FntHandle) | NDKEY_SETFNTSIZE (FntSize) | TRASP | NORENDER;
               
               ndWS_GrWriteLn (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1 +
                                     WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX1 + MarginX,
                                     WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1 +
                                     WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY2 - MarginY,
                                     &TempBuffer, __ColorText, __ColorButton, WndHandle, TextCode);
            
            }
            
            // La seconda stringa viene sempre ignorata per i fonts True Type
            
         #endif  
    }
            
    if (RenderNow) XWindowRender (WndHandle);  
}

/*
   -----------------------------------------------------------------------------
   API PER LA GESTIONE DEI PULSANTI
   -----------------------------------------------------------------------------
*/

char ndCTRL_CreateButtonComplex      (unsigned short int RRPosX1, unsigned short int RRPosY1, 
                                      unsigned short int RRPosX2, unsigned short int RRPosY2,
                                      char WndHandle,
                                      char *StringID, char *LabelButton1, char *LabelButton2,
                                      TypeColor ColorText, TypeColor ColorButton, 
                                      TypeColor ColorBorderButton,
                                      unsigned short int Q1PosX1, unsigned short int Q1PosY1,
                                      unsigned short int Q1PosX2, unsigned short int Q1PosY2,
                                      struct ndImage_Type *IconImage,
                                      unsigned short int Q2PosX1, unsigned short int Q2PosY1,
                                      unsigned short int Q2PosX2, unsigned short int Q2PosY2,
                                      ndint64 ButtonStyle, void* Callback, ndint64 InfoField, char RenderNow)
/* -------------------------------------------------------------------------------------------
   Questa routine crea sullo schermo un pulsante provvisto di icona. 
   
   Il pulsante è creato in un'area rettangolare di coordinate RRPosX1, RRPosY1, RRPosX2, 
   RRPosY2. Tali coordinate devono essere fornite in funzione del primo punto in alto a 
   sinistra del WindowSpace della finestra, il cui identificatore è dato dal parametro
   WndHandle. 
   
   Tale area rettangolare sarà detta, da qui in poi, Button Area. 
   
   Nella ButtonArea vengono sempre definite due ulteriori aree rettangolari. 
   
   La prima, di coordinate Q1PosX1, Q1PosY1, Q2PosX2 e Q2PosY2, è l'area che conterrà la
   stringa che deve apparire sul pulsante. 
   
   La seconda area rettangolare, di coordinate Q2PosX1, Q2PosY1, Q2PosX2 e Q2PosY2, è l'area
   che conterrà un'icona associata al pulsante. L'icona deve essere di tipo ndImageType e
   la routine chiamante deve passare a CreateButtonComplex l'indirizzo dell'immagine
   per mezzo del puntatore IconImage. Qualora venga passato un puntatore nullo, non
   verrà visualizzata alcuna icona.
   
   Sia le coordinate Q1x che Q2x devono essere fornite rispetto al punto in alto a 
   sinistra del pulsante. 
   
   La stringa StringID è un identificatore stringa di 32 caratteri: questo identificatore
   viene restituito alla routine chiamante, che può identificare quale pulsante è stato
   premuto proprio da tale stringa. 
   
   Per una data finestra, l'identificatore StringID è unico: se si cerca di immettere
   una richiesta di creazione di un pulsante con una stringa StringID identica a quella 
   di un pulsante che era già stato creato nella medesima finestra, si otterrà un errore.  
   
   I puntatori a costante stringa LabelButton1 e LabelButton2, indicano al sistema quali 
   etichette dovranno essere poste sui pulsanti. Se si vuole che l'etichetta sia una sola, 
   è sufficiente passare una stringa vuota 
   
   E' ammesso l'uso di una stringa o di due. Nel caso si usi una sola stringa, la seconda
   deve essere fornita passando il parametro stringa vuota "" alla routine. 
   
   La routine eseguirà automaticamente i calcoli per fare in modo che le etichette desiderata
   compaiano al centro dell'area rettangolare Q1. Qualora lo spazio per contenere la stringa
   fosse insufficiente, essa verrà troncata dal sistema grafico. 
   
   Il parametro IconImage è un puntatore ad una immagine NdImage che deve essere già stata
   preventivamente caricata dal sistema. Nanodesktop esegue una miniatura dell'immagine e
   ne salva una copia in memoria: questo significa che dopo avere richiamato la routine
   per la gestione delle immagini, il programmatore può anche disallocare l'immagine 
   usata come template per l'icona. Se si passa 0 come parametro IconImage, non
   sarà visualizzata alcuna icona. 
   
   La memoria usata da Nanodesktop per memorizzare la copia dell'icona, verrà rilasciata
   automaticamente alla chiusura della finestra, o quando verrà usata la routine 
   DestroyButton. 
   
   Il parametro ButtonStyle è un parametro a 64 bit, i cui vari bit forniscono delle
   informazioni sulle caratteristiche del pulsante. Nella attuale versione, hanno
   significato esclusivamente i bit:
               
       0       Il bit meno significativo è posto a 0 se il pulsante è attivato ed a
               1 se il pulsante è inibito. Se il pulsante è inibito, esso verrà
               visualizzato ombreggiato e la sua pressione non avrà effetto.
               Usare la costante ND_BUTTON_INHIBITED per inibire il pulsante
               
       1       Viene posto a 0 se la cornice del pulsante deve avere forma rettangolare,
               mentre viene posto ad 1 se la cornice del pulsante deve avere forma
               ovale.
               Usare la costante ND_BUTTON_RECTANGLE o ND_BUTTON_ROUNDED.
               
       2       Viene posto normalmente a 0. Se viene posto ad 1, allora il pulsante
               viene visualizzato in "logical mode": una modalità in cui il pulsante
               viene allocato ma non viene mostrato nulla sullo schermo.
               Usare la costante ND_BUTTON_LOGIC_MODE
               
       3       Se posto ad 1, abilita l'esecuzione trasparente delle callback. 
               MouseControl normalmente ritorna il controllo alla routine chiamante
               dopo avere eseguito una callback, in modo che il valore che è stato
               ottenuto dalla callback possa essere processato. Se è attivata la
               modalità di esecuzione trasparente delle callback, MouseControl 
               eseguirà la callback ma non ritornerà alla routine chiamante.
               Usare la costante ND_TRASPARENT_CALLBACK
               
       4       Se posto ad 1, abilita l'esecuzione l'esecuzione di una callback di
               pulsante in un contesto specifico. Questo parametro viene ignorato
               quando il sistema Phoenix non è attivo. Il codice delle callback, normalmente
               non dovrebbe contenere chiamate a MouseControl, perchè questo significherebbe,
               nei sistemi che eseguono Phoenix, dovere eseguire MouseControl da parte del
               Phoenix Mouse Thread. Questa evenienza è detta anche "MouseControl in
               MouseControl" e conduce sempre al blocco di Nanodesktop.
               
               Per tale ragione, di default le callback di pulsante non dovrebbero contenere
               chiamate a MouseControl. Tuttavia, qualora fosse proprio necessario eseguire 
               delle callback che a loro volta richiamano MouseControl (o routines derivate),
               è possibile usare l'opzione ND_CALLBACK_IN_NEW_CONTEXT. Tale opzione fa sì
               che ogni volta che si vuole premere un pulsante, venga creato un nuovo
               thread specifico e che questo esegua la propria callback in modo 
               autonomo.
               
       31-23   Questi bit contengono il font handle da utilizzare nella visualizzazione del
               pulsante. L'utente può impostare tale font handle usando la key NDKEY_FONT.
               Se non si usa questa key, o si imposta a 0 questo campo, Nanodesktop userà
               il font handle di default impostato nel window space della finestra 
               considerata (ossia, l'ultimo font impostato da ndWS_SetFont).
               
       54-47   Indicano l'ampiezza del font. Usare la key NDKEY_SETFNTSIZE (x). 
               Se non viene specificato una dimensione, Nanodesktop userà una dimensione
               di 16x16 pixel. Questo parametro viene ignorato quando si usa un
               font non TrueType
               
   Si presti attenzione al fatto che quando si usano fonts di tipo TrueType, la seconda
   label viene sempre ignorata e viene usata solo la prima. 
               
   Il parametro Callback è un indirizzo a 32 bit di una routine che viene richiamata
   automaticamente da MouseControl alla pressione del tasto. Se si pone Callback pari
   a 0, MouseControl non userà la routine ma si limiterà a segnalare alla routine
   chiamante che è stato premuto un tasto. 
   
   Il parametro InfoField è un numero a 64 bit che viene passato alla callback 
   quando il pulsante viene premuto. Questo parametro può essere usato in vari
   modi: può essere un indirizzo che deve essere passato alla callback, oppure
   può contenere varie informazioni, ottenute mappando i vari bit del campo.
   
   Il parametro RenderNow specifica se il rendering del pulsante deve essere 
   essere eseguito immediatamente o meno.
   
   Se l'operazione va a buon fine, la routine restituisce un char che specifica il
   numero di ordine del pulsante all'interno della finestra.
   
   Se qualcosa non funziona, la funzione restituisce in uscita un valore negativo 
   che indica un codice di errore. 
   
   Il pulsante verrà visualizzato immediatamente solo quando il parametro RenderNow sarà
   posto pari a 1.
   
   I codici di errore previsti sono:
            
      ERR_WINDOW_IS_NOT_ALLOCATED           La finestra non è allocata
      ERR_NO_BUTTON_RECORD_IS_AVAILABLE     Ogni finestra può contenere al massimo 64 pulsanti
      ERR_BUTTON_OUT_OF_WS                  Il pulsante andrebbe fuori dal WindowSpace
      ERR_WRONG_BUTTON_PARAMETERS           I parametri Q1 o Q2 vanno fuori dalla ButtonArea
      ERR_STRINGID_TOO_LONG                 La StringID non può superare 32 caratteri
      ERR_SAME_STRINGID_IS_PRESENT          E' già presente la medesima stringID
      ERR_INTERNAL_ERROR                    Non c'è lo spazio in memoria per memorizzare l'icona
      ERR_FONTHDL_IS_NOT_ACTIVE             Il fonthandle scelto non è attivo
  -------------------------------------------------------------------------------------------------
*/ 
{
   char ButtonHandle;
   char IHaveFound, ErrRep;
   char Counter, CounterX, CounterY;
   char LogicMode, IsInhibited, Rounded;
   unsigned short int ButtonLength, ButtonHeight; 
   unsigned short int Q2LenX, Q2LenY;
   
   float RatioX, RatioY;
   unsigned short int XPosX, XPosY;
   
   TypeColor Colore;
   unsigned char FntHandle;
   
   if ( !ndInitSystemExecuted ) ndInitSystem();
   
   LogicMode   = ((ButtonStyle & ND_BUTTON_LOGIC_MODE) == ND_BUTTON_LOGIC_MODE);
   Rounded     = ((ButtonStyle & ND_BUTTON_ROUNDED)    == ND_BUTTON_ROUNDED);
   IsInhibited = ((ButtonStyle & ND_BUTTON_INHIBITED)  == ND_BUTTON_INHIBITED);
     
   if (WindowData [WndHandle].WindowIsActive)
   {
       FntHandle = (unsigned char) (ButtonStyle >> (31-8));
       // Ricava il font handle 
   
       if (FntHandle==0) FntHandle=WindowData [WndHandle].ndWS_CurrentFntHandle;
                        
       if (FontHandlesArray [FntHandle].IsActive)
       {
             // Adesso eseguiamo dei controlli preliminari sui parametri passati 
             // alla funzione
                       
             if ( ( RRPosX1 <= WindowData [WndHandle].WSWndLength ) && ( RRPosX2 <= WindowData [WndHandle].WSWndLength ) &&
                  ( RRPosY1 <= WindowData [WndHandle].WSWndHeight ) && ( RRPosY2 <= WindowData [WndHandle].WSWndHeight ) )
             {
                  ButtonLength = RRPosX2 - RRPosX1;
                  ButtonHeight = RRPosY2 - RRPosY1;
                  
                  if ( ( Q1PosX1 <= ButtonLength ) && ( Q1PosX2 <= ButtonLength ) &&
                       ( Q1PosY1 <= ButtonHeight ) && ( Q1PosY2 <= ButtonHeight ) )
                  {
                       
                       if ( ( Q2PosX1 <= ButtonLength ) && ( Q2PosX2 <= ButtonLength ) &&
                            ( Q2PosY1 <= ButtonHeight ) && ( Q2PosY2 <= ButtonHeight ) )
                       {
                            
                            if (WindowData [WndHandle].NrButtonsAllocated<__NRBUTTONSALLOCABLE)
                            {
                                           
                                 if (strlen (StringID)<32)
                                 {
                                       for (Counter=0; Counter<__NRBUTTONSALLOCABLE; Counter++)
                                       {
                                           if ( (WindowData [WndHandle].ButtonClass4_Data [Counter].ButtonIsActivated) 
                                           && (strcmp (StringID, WindowData [WndHandle].ButtonClass4_Data [Counter].StringID)==0) )
                                           return ERR_SAME_STRINGID_IS_PRESENT;       
                                       }                                    
                                       
                                       IHaveFound=0;
                                            
                                       for (Counter=0; Counter<__NRBUTTONSALLOCABLE; Counter++)
                                       {
                                           if (WindowData [WndHandle].ButtonClass4_Data [Counter].ButtonIsActivated == 0)
                                           {
                                               IHaveFound=1;
                                               break;
                                           }
                                       }  
                                       
                                       if (!IHaveFound) return ERR_NO_BUTTON_RECORD_IS_AVAILABLE;  
                                       // Non dovrebbe verificarsi mai, se le routines funzionano correttamente.....
                                       
                                       ButtonHandle=Counter;
                                       // Abbiamo trovato un handle libero per il pulsante.
                                       
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsInhibited=IsInhibited;
                                       
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1          =RRPosX1;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1          =RRPosY1;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX2          =RRPosX2;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY2          =RRPosY2;
                                       
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX1          =Q1PosX1;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY1          =Q1PosY1;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX2          =Q1PosX2;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY2          =Q1PosY2;
                                       
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosX1          =Q2PosX1;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosY1          =Q2PosY1;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosX2          =Q2PosX2;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosY2          =Q2PosY2;
                                       
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorText         = ColorText;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorButton       = ColorButton;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorBorderButton = ColorBorderButton;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].FntHandle         = FntHandle;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].FntSize           = (unsigned char)  ( (ButtonStyle >> 63-9-8) & 0x00FF );
                                       
                                       strcpy (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].StringID, StringID);
                                       
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton1 = LabelButton1;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton2 = LabelButton2;
                                       
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle  = ButtonStyle;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Callback     = Callback;
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].InfoField    = InfoField;
                                       
                                       
                                       // Tutti i dati relativi al pulsante adesso sono stati memorizzati, ma l'handle non è
                                       // stato ancora bloccato perchè occorre vedere se è possibile memorizzare l'icona
                                       
                                       if ((LogicMode) || (IconImage==0))
                                       {
                                          WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data=0;
                                       }
                                       else
                                       {
                                           Q2LenX = abs (Q2PosX2 - Q2PosX1);
                                           Q2LenY = abs (Q2PosY2 - Q2PosY1);
                                           
                                           ndIMG_InitImage ( &(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage) );
                                           
                                           ErrRep = ndIMG_CreateImage ( &(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage), Q2LenX, Q2LenY, NDMGKNB);
                                           
                                           if (ErrRep==0)             // Il processo di allocazione dell'icona è riuscito
                                           {
                                              // E' necessario copiare i dati dall'immagine sorgente all'immagine icona
                                              
                                              RatioX = (float) (IconImage->LenX) / (float) (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.LenX);
                                              RatioY = (float) (IconImage->LenY) / (float) (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.LenY);
                                       
                                              for (CounterY=0; CounterY<Q2LenY; CounterY++)
                                              {
                                                  for (CounterX=0; CounterX<Q2LenX; CounterX++)
                                                  {
                                                      XPosX = (unsigned short int) ( (float)(CounterX)*RatioX + 0.5 );
                                                      XPosY = (unsigned short int) ( (float)(CounterY)*RatioY + 0.5 );
                                                      
                                                      ndIMG_GetPixelFromImage (IconImage, XPosX, XPosY, &Colore);
                                                      ndIMG_PutPixelToImage ( &(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage), CounterX, CounterY, Colore);                                              
                                                  }
                                              }
                                               
                                           }
                                           else   // Non è riuscito ad allocare lo spazio in memoria per memorizzare l'icona
                                             return ERR_INTERNAL_ERROR;             
                                           
                                       }
                                       
                                       if (!LogicMode) INTERNAL_ButtonRenderer (ButtonHandle, WndHandle, IsInhibited, Rounded, RenderNow);
                                       // Provvede alla visualizzazione grafica del pulsante richiamando eventualmente
                                       // XWindowRender
                                       
                                       WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated = 1;
                                       // Registra che il pulsante è attivato
                                       
                                       return ButtonHandle;
                                           
                                 }
                                 else return ERR_STRINGID_TOO_LONG;
                            }
                            else return ERR_NO_BUTTON_RECORD_IS_AVAILABLE;    
                       }
                       else return ERR_WRONG_BUTTON_PARAMETERS;
                  }
                  else return ERR_WRONG_BUTTON_PARAMETERS;
             }
             else return ERR_BUTTON_OUT_OF_WS;
        }
        else return ERR_FONTHDL_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndCTRL_CreateButton      (unsigned short int RRPosX1, unsigned short int RRPosY1, 
                               unsigned short int RRPosX2, unsigned short int RRPosY2,
                               char WndHandle,
                               char *StringID, char *LabelButton1, char *LabelButton2,
                               TypeColor ColorText, TypeColor ColorButton, 
                               TypeColor ColorBorderButton,
                               ndint64 ButtonStyle, void* Callback, ndint64 InfoField, 
                               char RenderNow)
/* 
   Routine analoga a ndCTRL_CreateButton, ma non permette di visualizzare delle icone.
   Il vantaggio è questa routine richiede un numero minore di parametri per 
   funzionare.
*/
{
   return  ndCTRL_CreateButtonComplex (RRPosX1, RRPosY1, RRPosX2, RRPosY2, WndHandle,
                                        StringID, LabelButton1, LabelButton2, 
                                        ColorText, ColorButton, ColorBorderButton,
                                        0, 0, RRPosX2 - RRPosX1, RRPosY2 - RRPosY1, 
                                        0, 
                                        0, 0, 0, 0,
                                        ButtonStyle, Callback, InfoField, RenderNow);                                           
}



char ndCTRL_CreateButtonSmart     (unsigned short int RRPosX1, unsigned short RRPosY1,
                                    char WndHandle, 
                                    char *StringID, char *LabelButton1, char *LabelButton2,
                                    TypeColor ColorText, TypeColor ColorButton, 
                                    TypeColor ColorBorderButton,
                                    struct ndImage_Type *ImageIcon,
                                    ndint64 ButtonStyle, void* Callback, ndint64 InfoField, 
                                    char RenderNow)
/* ---------------------------------------------------------------------------------
   La routine CreateButtonComplex richiede un numero molto elevato di parametri per potere
   essere usata correttamente. Per facilitare il lavoro degli sviluppatori, è stata
   implementata la funzione ndCTRL_CreateButtonSmart, che richiede un minore
   numero di parametri, poichè altri vengono calcolati internamente dalla funzione.
   
   L'utente deve solo indicare il punto dove deve essere posto il pulsante (indicato
   da RRPosX1 e RRPosX2). Le dimensioni del pulsante vengono calcolate automaticamente
   in base al numero di caratteri presente nelle stringhe. 
   
   Inoltre, l'immagine dell'icona deve obbligatoriamente avere dimensioni di 24*24
   pixel. Non sono ammesse dimensioni diverse. 
   
   Qualora si passi alla routine un valore di ImageIcon pari a 0, non verrà 
   visualizzata alcuna bitmap. 
   
   I codici di errore sono gli stessi rilasciati da CreateButtonComplex   
  ------------------------------------------------------------------------------------------
*/ 
{
   unsigned short sX1, sX2, sY1, sY2;
   unsigned short q1x1, q1x2, q1y1, q1y2;
   unsigned short q2x1, q2x2, q2y1, q2y2;
   
   unsigned short BitMapMarginX;
   
   unsigned short LenStringa1, LenStringa2, MaxLen;
   
   LenStringa1=strlen (LabelButton1);
   LenStringa2=strlen (LabelButton2);
   
   if (LenStringa1>=LenStringa2)
      MaxLen = LenStringa1;
   else
      MaxLen = LenStringa2;
      
   if (ImageIcon == 0)
      BitMapMarginX = 0;
   else
      BitMapMarginX = 24;
   
   sX1 = RRPosX1;
   sY1 = RRPosY1;
   sX2 = RRPosX1 + BitMapMarginX + MaxLen*8 + 12;
   sY2 = RRPosY1 + 32;
   
   q1x1 = 4 + BitMapMarginX + 4;
   q1y1 = 4;
   q1x2 = 4 + BitMapMarginX + 4 + MaxLen*8;
   q1y2 = 28;
   
   q2x1 = 4;
   q2y1 = 4;
   q2x2 = 28;
   q2y2 = 28;
   
   return ndCTRL_CreateButtonComplex (sX1, sY1, sX2, sY2, WndHandle, 
                         StringID, LabelButton1, LabelButton2, 
                         ColorText, ColorButton, ColorBorderButton, 
                         q1x1, q1y1, q1x2, q1y2, 
                         ImageIcon,
                         q2x1, q2y1, q2x2, q2y2, 
                         ButtonStyle, Callback, InfoField, RenderNow);                                                                                                         
}



char ndCTRL_DestroyButton (char ButtonHandle, char WndHandle, char OnlyLogical, TypeColor Color, char RenderNow)
/* -------------------------------------------------------------------------------
   Questa routine si occupa di disallocare un pulsante in una finestra. 
   
   Il pulsante viene identificato da ButtonHandle e la finestra viene identificata
   da WndHandle. Se il pulsante contiene un icona, verrà liberata la memoria 
   che era stata allocata per contenere i pixel dell'icona.
   
   Se si pone ad 1 il parametro OnlyLogical, il pulsante verrà disallocato solo dal
   punto di vista logico: il disegno del pulsante rimarrà sullo schermo, anche se
   cliccandoci sopra MouseControl non farà nulla. 
   
   Se invece si pone a 0 il parametro OnlyLogical, il pulsante verrà cancellato
   anche graficamente: questo viene fatto riempiendo lo spazio dove si trovava il
   pulsante con il colore Color (che è in genere il colore di sfondo della finestra.
   
   La routine prevede i seguenti codici di errore:
      ERR_WINDOW_IS_NOT_ALLOCATED           La finestra non è allocata
      ERR_BUTTON_IS_NOT_ACTIVE              Il pulsante richiesto non è attivo           
   --------------------------------------------------------------------------------- 
*/
{
  char Rounded;
                                                                                     
  if ( !ndInitSystemExecuted ) ndInitSystem();
   
  if (WindowData [WndHandle].WindowIsActive)
  {
      if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated)
      {
         Rounded = ((WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & ND_BUTTON_ROUNDED) == ND_BUTTON_ROUNDED);
         
         if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data!=0)
         {
            free (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data);
            WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data=0;
         }
         
         if (!OnlyLogical)
         {
            if (!Rounded)
            {
               ndWS_DrawRectangle ( WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1,
                                    WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1,
                                    WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX2,
                                    WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY2,
                                    Color, Color, WndHandle, NORENDER);
            }
            else
            {
               ndWS_DrawSpRectangle ( WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1,
                                           WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1,
                                           WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX2,
                                           WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY2,
                                           Color, Color, ND_ROUNDED, WndHandle, NORENDER);
            } 
         
            if (RenderNow) XWindowRender (WndHandle);
         }
      
         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated = 0;
      
         return 0;
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;    
}


char ndCTRL_DestroyButtonUsingStringID (char *StringID, char WndHandle, char OnlyLogical, TypeColor Color, char RenderNow)
/* -----------------------------------------------------------------------------
   E' analoga a DestroyButton, ma l'individuazione del pulsante avviene per mezzo 
   della sua StringID.
   
   Oltre ai codici di errore previsti da DestroyButton, questa routine può anche
   restituire il codice di errore:
       ERR_STRINGID_NOT_FOUND 
   -----------------------------------------------------------------------------
*/

{
   char ButtonHandleOrError;
                                                                                
   ButtonHandleOrError = ndCTRL_FindButtonHandleByStringID (StringID, WndHandle);
   // Esegue la ricerca del ButtonHandle
   
   if (ButtonHandleOrError>=0)
      return ndCTRL_DestroyButton (ButtonHandleOrError, WndHandle, OnlyLogical, Color, RenderNow);
   else
      return ButtonHandleOrError;
}


char ndCTRL_ChangeButtonParameters_Complex   (char ButtonHandle, char WndHandle,
                                      char *NewStringID, char *NewLabelButton1, char *NewLabelButton2,
                                      TypeColor ColorText, TypeColor ColorButton, 
                                      TypeColor ColorBorderButton,
                                      unsigned short int Q1PosX1, unsigned short int Q1PosY1,
                                      unsigned short int Q1PosX2, unsigned short int Q1PosY2,
                                      struct ndImage_Type *IconImage,
                                      unsigned short int Q2PosX1, unsigned short int Q2PosY1,
                                      unsigned short int Q2PosX2, unsigned short int Q2PosY2,
                                      ndint64 ButtonStyle, char RenderNow)
/* ------------------------------------------------------------------------------------------
   Provvede a cambiare i parametri relativi ad un pulsante. Il pulsante viene identificato
   dal parametro ButtonHandle e la finestra è identificata da WndHandle.
   
   Questa routine viene usata per scopi grafici. Essa non consente di cambiare i parametri di
   inibizione/deinibizione, la forma del pulsante o la posizione.
   
   Sono previsti i seguenti codici di errore:
      ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
      ERR_BUTTON_IS_NOT_ACTIVE        Il pulsante non è attivo 
      ERR_STRINGID_TOO_LONG           L'identificatore stringa è troppo lungo (+ di 32 caratteri) 
      ERR_INTERNAL_ERROR              Errore interno
   -----------------------------------------------------------------------------------------
*/
{
   int Flag;
   char LogicMode, IsInhibited, Rounded, NewContext;
   unsigned short int Q2LenX, Q2LenY;
   char ErrRep;
   
   float RatioX, RatioY;
   char  CounterX, CounterY;
   unsigned short int XPosX, XPosY;
   TypeColor Colore;
   
   unsigned char FntHandle;
   
   
   if ( !ndInitSystemExecuted ) ndInitSystem();
   
   LogicMode   = ((ButtonStyle & ND_BUTTON_LOGIC_MODE) == ND_BUTTON_LOGIC_MODE);
   NewContext  = ((ButtonStyle & ND_CALLBACK_IN_NEW_CONTEXT) == ND_CALLBACK_IN_NEW_CONTEXT);
   
   if (strlen (NewStringID)>=32) return ERR_STRINGID_TOO_LONG; 
   
   if (WindowData [WndHandle].WindowIsActive)
   {            
      if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated)
      {
          FntHandle = (unsigned char) (ButtonStyle >> (31-8));
           // Ricava il font handle 
       
          if (FntHandle==0) FntHandle=WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].FntHandle;
          // Se il font handle non è stato precisato, allora mantieni quello precedente 
                            
          if (FontHandlesArray [FntHandle].IsActive)
          {
             if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data!=0)
             {
                free (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data);
                WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data=0;
             }
             
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated=0;
             // Poniamo per sicurezza a 0 il flag ButtonIsActivated: così, se dovesse fallire il
             // processo di allocazione della nuova icona, il pulsante risulterà disattivato
                                       
             Rounded     = ((WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & ND_BUTTON_ROUNDED)    == ND_BUTTON_ROUNDED);
             IsInhibited = ((WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & ND_BUTTON_INHIBITED)  == ND_BUTTON_INHIBITED);             
                                       
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX1          =Q1PosX1;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY1          =Q1PosY1;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosX2          =Q1PosX2;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q1PosY2          =Q1PosY2;
                                       
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosX1          =Q2PosX1;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosY1          =Q2PosY1;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosX2          =Q2PosX2;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Q2PosY2          =Q2PosY2;
                                       
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorText         = ColorText;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorButton       = ColorButton;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ColorBorderButton = ColorBorderButton;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].FntHandle         = FntHandle;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].FntSize           = (unsigned char)  ( (ButtonStyle >> 63-9-8) & 0x00FF );
                                                                 
             strcpy (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].StringID, NewStringID);
                                       
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton1 = NewLabelButton1;
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].LabelButton2 = NewLabelButton2;
                                       
             // L'unica cosa nel parametro ButtonStyle che è possibile cambiare è logic mode e new context
             
             if (LogicMode)
             {
               WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle  = 
                  (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle | ND_BUTTON_LOGIC_MODE);
             }
             else
             { 
               Flag = ~ND_BUTTON_LOGIC_MODE; 
                        
               WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle  = 
                  (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & Flag);          
             }     
             
             if (NewContext)
             {
               WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle  = 
                  (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle | ND_CALLBACK_IN_NEW_CONTEXT);
             }
             else
             { 
               Flag = ~ND_CALLBACK_IN_NEW_CONTEXT; 
                        
               WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle  = 
                  (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & Flag);          
             }  
                      
                           
             // Tutti i dati relativi al pulsante adesso sono stati memorizzati, ma l'handle non è
             // stato ancora bloccato perchè occorre vedere se è possibile memorizzare l'icona
                                       
             if ((LogicMode) || (IconImage==0))
             {
                WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.Data=0;
             }
             else
             {
                Q2LenX = abs (Q2PosX2 - Q2PosX1);
                Q2LenY = abs (Q2PosY2 - Q2PosY1);
                                           
                ndIMG_InitImage ( &(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage) );
                                           
                ErrRep = ndIMG_CreateImage ( &(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage), Q2LenX, Q2LenY, NDMGKNB);
                                           
                if (ErrRep==0)             // Il processo di allocazione dell'icona è riuscito
                {
                   // E' necessario copiare i dati dall'immagine sorgente all'immagine icona
                                              
                   RatioX = IconImage->LenX / WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.LenX;
                   RatioY = IconImage->LenY / WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage.LenY;
                                       
                   for (CounterY=0; CounterY<Q2LenY; CounterY++)
                   {
                       for (CounterX=0; CounterX<Q2LenX; CounterX++)
                      {
                          XPosX = (unsigned short int) ( (float)(CounterX)*RatioX );
                          XPosY = (unsigned short int) ( (float)(CounterY)*RatioY );
                          
                          ndIMG_GetPixelFromImage (IconImage, XPosX, XPosY, &Colore);
                          ndIMG_PutPixelToImage ( &(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].IconImage), CounterX, CounterY, Colore);                                              
                      }
                  }
                           
               }
               else   // Non è riuscito ad allocare lo spazio in memoria per memorizzare l'icona
                 return ERR_INTERNAL_ERROR;             
               
             }
                                       
             if (!LogicMode) INTERNAL_ButtonRenderer (ButtonHandle, WndHandle, IsInhibited, Rounded, RenderNow);
             // Provvede alla visualizzazione grafica del pulsante richiamando eventualmente
             // XWindowRender
                                       
             WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated = 1;
             // Registra che il pulsante è attivato
                                       
             return 0;
             // Segnala alla routine chiamante che non ci sono stati errori                        
          }
          else return ERR_FONTHDL_IS_NOT_ACTIVE;                    
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;                                                                                             
}


char ndCTRL_ChangeButtonParametersUsingStringID_Complex  (char *StringID, char WndHandle,
                                      char *NewStringID, char *NewLabelButton1, char *NewLabelButton2,
                                      TypeColor ColorText, TypeColor ColorButton, 
                                      TypeColor ColorBorderButton,
                                      unsigned short int Q1PosX1, unsigned short int Q1PosY1,
                                      unsigned short int Q1PosX2, unsigned short int Q1PosY2,
                                      struct ndImage_Type *IconImage,
                                      unsigned short int Q2PosX1, unsigned short int Q2PosY1,
                                      unsigned short int Q2PosX2, unsigned short int Q2PosY2,
                                      ndint64 ButtonStyle, char RenderNow)
/* 
   ----------------------------------------------------------------------------
   Analoga a ndCTRL_ChangeButtonParameters_Complex, ma l'identificazione del pulsante
   avviene per mezzo della StringID.
   ----------------------------------------------------------------------------
*/

{
   char ButtonHandleOrError;
                                                                                
   ButtonHandleOrError = ndCTRL_FindButtonHandleByStringID (StringID, WndHandle);
   // Esegue la ricerca del ButtonHandle
   
   if (ButtonHandleOrError>=0)
   {
      return ndCTRL_ChangeButtonParameters_Complex (ButtonHandleOrError, WndHandle,
                                            NewStringID, NewLabelButton1, NewLabelButton2,
                                            ColorText, ColorButton, ColorBorderButton,
                                            Q1PosX1, Q1PosY1, Q1PosX2, Q1PosY2,
                                            IconImage,
                                            Q2PosX1, Q2PosY1, Q2PosX2, Q2PosY2,
                                            ButtonStyle, RenderNow);
   }
   else
      return ButtonHandleOrError;
                                    
}


char ndCTRL_ChangeButtonParameters   (char ButtonHandle, char WndHandle,
                                      char *NewStringID, char *NewLabelButton1, char *NewLabelButton2,
                                      TypeColor ColorText, TypeColor ColorButton, 
                                      TypeColor ColorBorderButton,
                                      ndint64 ButtonStyle, char RenderNow)
/* 
   ------------------------------------------------------------------------------------------------
   E' una versione semplificata di ChangeButtonParameters_Complex, che calcola automaticamente
   i valori dei parametri Q1 e Q2.
   ------------------------------------------------------------------------------------------------
*/  
{
   unsigned short int RRPosX1, RRPosY1, RRPosX2, RRPosY2; 
    
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated)
      {                   
          RRPosX1 = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX1;
          RRPosY1 = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY1;
          RRPosX2 = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosX2;
          RRPosY2 = WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].RRPosY2;
          
          return ndCTRL_ChangeButtonParameters_Complex   (ButtonHandle, WndHandle,
                                      NewStringID, NewLabelButton1, NewLabelButton2,
                                      ColorText, ColorButton, ColorBorderButton,
                                      0, 0, RRPosX2 - RRPosX1, RRPosY2 - RRPosY1,
                                      0,
                                      0, 0, 0, 0,
                                      ButtonStyle, RenderNow);   
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;                                      
}


char ndCTRL_ChangeButtonParametersUsingStringID (char *StringID, char WndHandle,
                                      char *NewStringID, char *NewLabelButton1, char *NewLabelButton2,
                                      TypeColor ColorText, TypeColor ColorButton, 
                                      TypeColor ColorBorderButton,
                                      ndint64 ButtonStyle, char RenderNow)
/* 
   ----------------------------------------------------------------------------
   Analoga a ndCTRL_ChangeButtonParameters, ma l'identificazione del pulsante
   avviene per mezzo della StringID.
   ----------------------------------------------------------------------------
*/
{
   char ButtonHandleOrError;
                                                                                
   ButtonHandleOrError = ndCTRL_FindButtonHandleByStringID (StringID, WndHandle);
   // Esegue la ricerca del ButtonHandle
   
   if (ButtonHandleOrError>=0)
   {
      return ndCTRL_ChangeButtonParameters (ButtonHandleOrError, WndHandle,
                                            NewStringID, NewLabelButton1, NewLabelButton2,
                                            ColorText, ColorButton, ColorBorderButton,
                                            ButtonStyle, RenderNow);
   }
   else
      return ButtonHandleOrError;
                                    
}





char ndCTRL_InhibitButton (char ButtonHandle, char WndHandle, char RenderNow)
/*
   Inibisce un pulsante. Sono previsti i seguenti codici di errore
   
   ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
   ERR_BUTTON_IS_NOT_ACTIVE        Il pulsante non è attivo 
*/
{
   char IsInhibited, Rounded, LogicMode;
   
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated)
      {                   
         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsInhibited =
           WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsInhibited | ND_BUTTON_INHIBITED;
       
         // Ricalcolo dei parametri per il renderer
         Rounded     = ((WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & ND_BUTTON_ROUNDED)    == ND_BUTTON_ROUNDED);
         LogicMode   = ((WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & ND_BUTTON_LOGIC_MODE) == ND_BUTTON_LOGIC_MODE);
         IsInhibited = 1;
                                   
         if (!LogicMode) INTERNAL_ButtonRenderer (ButtonHandle, WndHandle, IsInhibited, Rounded, RenderNow);
         // Provvede alla visualizzazione grafica del pulsante richiamando eventualmente
         // XWindowRender
                                                             
         return 0;
         // Segnala alla routine chiamante che non ci sono stati errori                          
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;                                                                                                      
}





char ndCTRL_DeInhibitButton (char ButtonHandle, char WndHandle, char RenderNow)
/*
   Toglie l'inibizione di un pulsante. Sono previsti i seguenti codici di errore
   
   ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
   ERR_BUTTON_IS_NOT_ACTIVE        Il pulsante non è attivo 
*/
{
   int Flag;
   char IsInhibited, Rounded, LogicMode;
   
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated)
      {
         Flag = ~ND_BUTTON_INHIBITED;
         
         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsInhibited =
           WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsInhibited & Flag ;
         
         // Ricalcolo dei parametri per il renderer
         Rounded     = ((WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & ND_BUTTON_ROUNDED)    == ND_BUTTON_ROUNDED);
         LogicMode   = ((WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle & ND_BUTTON_LOGIC_MODE) == ND_BUTTON_LOGIC_MODE);
         IsInhibited = 0;
                                   
         if (!LogicMode) INTERNAL_ButtonRenderer (ButtonHandle, WndHandle, IsInhibited, Rounded, RenderNow);
         // Provvede alla visualizzazione grafica del pulsante richiamando eventualmente
         // XWindowRender
                                                             
         return 0;
         // Segnala alla routine chiamante che non ci sono stati errori                          
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;                                                                                                      
}





char ndCTRL_InhibitButtonUsingStringID (char *StringID, char WndHandle, char RenderNow)
/*
   Analoga a InhibitButton ma usa StringID per identificare il pulsante
*/
{
   char ButtonHandleOrError;
                                                                                
   ButtonHandleOrError = ndCTRL_FindButtonHandleByStringID (StringID, WndHandle);
   // Esegue la ricerca del ButtonHandle
   
   if (ButtonHandleOrError>=0)
   {
      return ndCTRL_InhibitButton (ButtonHandleOrError, WndHandle, RenderNow);
   }
   else
      return ButtonHandleOrError;
}                                    




char ndCTRL_DeInhibitButtonUsingStringID (char *StringID, char WndHandle, char RenderNow)
/*
   Analoga a DeInhibitButton ma usa StringID per identificare il pulsante
*/
{
   char ButtonHandleOrError;
                                                                                
   ButtonHandleOrError = ndCTRL_FindButtonHandleByStringID (StringID, WndHandle);
   // Esegue la ricerca del ButtonHandle
   
   if (ButtonHandleOrError>=0)
   {
      return ndCTRL_DeInhibitButton (ButtonHandleOrError, WndHandle, RenderNow);
   }
   else
      return ButtonHandleOrError;
}                                    


char ndCTRL_ChangeCallBack (int Callback, ndint64 InfoField, char ButtonHandle, char WndHandle, char cbOptions)
/*
   Setta una nuova CallBack per un pulsante (o, qualora il parametro Callback sia
   settato a 0, disattiva la gestione delle CallBack). 
   
   Inoltre, setta un nuovo valore del parametro InfoField.
   
   Il parametro CbOptions ha la funzione di stabilire le caratteristiche della
   nuova callback. Esso può avere due valori (eventualmente or-abili insieme):
         
   ND_TRASPARENT_CALLBACK      La nuova callback sarà eseguita in modalità trasparente
   ND_CALLBACK_IN_NEW_CONTEXT  La nuova callback dovrà essere eseguita extra-contesto 
   
   Sono previsti i seguenti codici di errore
   
   ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
   ERR_BUTTON_IS_NOT_ACTIVE        Il pulsante non è attivo 
*/
{
   ndint64 TempValue;

   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonIsActivated)
      {                   
         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].Callback=Callback;
         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].InfoField=InfoField;
         
         // Copia il valore di ButtonStyle in una variabile a 64 bit temporanea
         TempValue=WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle;
         
         if (cbOptions & ND_TRASPARENT_CALLBACK)
             TempValue = TempValue | ND_TRASPARENT_CALLBACK;
         else
             TempValue = TempValue & (~(ndint64)(ND_TRASPARENT_CALLBACK));
             
         if (cbOptions & ND_CALLBACK_IN_NEW_CONTEXT)
             TempValue = TempValue | ND_CALLBACK_IN_NEW_CONTEXT;
         else
             TempValue = TempValue & (~(ndint64)(ND_CALLBACK_IN_NEW_CONTEXT));
         
         // Ricopia la variabile modificata nelle strutture di sistema
         WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].ButtonStyle = TempValue;
                                   
         return 0;
         // Segnala alla routine chiamante che non ci sono stati errori                          
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;                                                                                                      
}


char ndCTRL_ChangeCallBackUsingStringID (int Callback, ndint64 InfoField, char *StringID, char WndHandle, char cbOptions)
{
   char ButtonHandleOrError;
                                                                                
   ButtonHandleOrError = ndCTRL_FindButtonHandleByStringID (StringID, WndHandle);
   // Esegue la ricerca del ButtonHandle
   
   if (ButtonHandleOrError>=0)
   {
      return ndCTRL_ChangeCallBack (Callback, InfoField, ButtonHandleOrError, WndHandle, cbOptions);
   }
   else
      return ButtonHandleOrError;
}                                    





/*
    ----------------------------------------------------------------------------
    ROUTINES MAKE ONE BUTTON, MAKE TWO BUTTON .....
    ----------------------------------------------------------------------------
*/

char ndCTRL_MakeOneButton (char *Message, unsigned short int PosY1, unsigned short int PosY2, 
                           TypeColor ColorText, TypeColor ColorButton,
                           TypeColor ColorBorderButton, unsigned short int WndHandle,
                           char AutoClose)
/*
   --------------------------------------------------------------------------------------------------
   La routine MakeOneButton crea nella finestra WndHandle un singolo pulsante, con la stringa
   Message. 
   
   La routine serve agli sviluppatori che hanno bisogno di scrivere velocemente il codice per
   disegnare un singolo pulsante in una dialog box. 
   
   La posizione orizzontale del pulsante viene determinata automaticamente (il pulsante viene
   posto al centro dello schermo), mentre le coordinate verticali sono date da PosY1 e PosY2. 
   
   Dopo avere visualizzato il pulsante, la routine richiama MouseControl per consentire 
   all'utente di premere il pulsante. 
   
   La routine ritorna 0 qualora l'utente abbia premuto il pulsante, mentre ritorna la
   costante DIALOG_BOX_EXIT qualora l'utente abbia chiuso la dialog_box.
   
   Il parametro AutoClose, specifica se quando l'utente tenta di chiudere la finestra, 
   MakeOneButton deve eseguire internamente la DestroyWindow.
   
   Se AutoClose è posto a 0 (o NOAUTOCLOSE), la routine MakeOneButton provvederà semplicemente a
   riportare in uscita la costante DIALOG_BOX_EXIT, e sarà compito della routine chiamante
   chiudere la DialogBox.
   
   Al contrario, se AutoClose è posto a 1 (o AUTOCLOSE), la routine MakeOneButton eseguirà
   internamente una DestroyWindow prima di passare il controllo alla routine chiamante.
   
   La routine riporta i seguenti codici di errore:
   
   ERR_WINDOW_WAS_NOT_ACTIVE     La finestra non era allocata
   ERR_MESSAGE_IS_TOO_LONG       Il messaggio sul pulsante è troppo lungo
   -------------------------------------------------------------------------------------------------
*/

{
  struct ndEvent_Type Event;
  
  unsigned short int PixelLenStringX;                                                                                                  
  unsigned short int CenterX;
  
  unsigned short int aX1, aY1, aX2, aY2;
  
  char ButtonHandle;
  char YouCanExit;
                                                                                                    
  if (WindowData [WndHandle].WindowIsActive)
  {
    PixelLenStringX = 30 + strlen (Message)*8;
    
    if (PixelLenStringX < WindowData [WndHandle].WSWndLength)
    {
       CenterX = WindowData [WndHandle].WSWndLength / 2;
       
       aX1 = CenterX - PixelLenStringX/2;
       aY1 = PosY1;
       aX2 = CenterX + PixelLenStringX/2;
       aY2 = PosY2;
       
       ButtonHandle = ndCTRL_CreateButton (aX1, aY1, aX2, aY2, WndHandle, "Button1", Message, "", 
                    ColorText, ColorButton, ColorBorderButton, ND_BUTTON_ROUNDED, 0, 0, NORENDER);
       
       XWindowRender (WndHandle);
       
       YouCanExit=0;

       while (!YouCanExit)
       {
         MouseControl (&Event);

         if ( (Event.__TypeEvent==CUSTOM_BUTTON_PRESSED) && (Event.__WindowID==WndHandle) )
         {
             if ( Event.__ButtonID==ButtonHandle )
             {
               YouCanExit=1;
               return 0;
             }
         }
         
         if (( Event.__TypeEvent == A_WINDOW_WANTS_TO_BE_CLOSED ) && (Event.__WindowID==WndHandle))
         {
            YouCanExit=1;
            if (AutoClose) ndLP_DestroyWindow (WndHandle);
               
            return DIALOG_BOX_EXIT;  
         }
         
       }

    }
    else
     return   ERR_MESSAGE_IS_TOO_LONG;       
  }
  else
   return ERR_WINDOW_WAS_NOT_ACTIVE;
                                                                                                    
}
   

char ndCTRL_MakeTwoButton (char *Message1, char *Message2, unsigned short int PosY1, unsigned short int PosY2, 
                           TypeColor ColorText, TypeColor ColorButton,
                           TypeColor ColorBorderButton, unsigned short int WndHandle,
                           char AutoClose)
/*
   --------------------------------------------------------------------------------------------------
   La routine MakeOneButton crea nella finestra WndHandle due pulsanti, uno con la stringa 
   Message1 ed uno con la stringa Message2. 
    
   La routine serve agli sviluppatori che hanno bisogno di scrivere velocemente il codice per
   disegnare un doppio pulsante in una dialog box. Per esempio, può essere usato per
   una finestra dove i due pulsanti sono confirm e cancel. 
   
   La posizione orizzontale del pulsante viene determinata automaticamente, 
   mentre le coordinate verticali sono date da PosY1 e PosY2. 
   
   Dopo avere visualizzato i pulsanti, la routine richiama MouseControl per consentire 
   all'utente di premerne uno o di minimizzare la finestra. 
   
   Qualora l'utente prema il pulsante di sinistra, la routine riporta 0. Qualora
   l'utente prema il pulsante di destra, la routine riporta 1.
   
   Se l'utente tenti di chiudere la DialogBox, la routine riporta in uscita la
   costante DIALOG_BOX_EXIT.
   
   Il parametro AutoClose, specifica se quando l'utente tenta di chiudere la finestra, 
   MakeTwoButton deve eseguire internamente la DestroyWindow.
   
   Se AutoClose è posto a 0 (o NOAUTOCLOSE), la routine MakeTwoButton provvederà semplicemente a
   riportare in uscita la costante DIALOG_BOX_EXIT, e sarà compito della routine chiamante
   chiudere la DialogBox.
   
   Al contrario, se AutoClose è posto a 1 (o AUTOCLOSE), la routine MakeTwoButton eseguirà
   internamente una DestroyWindow prima di passare il controllo alla routine chiamante.
               
   La routine riporta i seguenti codici di errore:
   
   ERR_WINDOW_WAS_NOT_ACTIVE     La finestra non era allocata
   ERR_MESSAGE_IS_TOO_LONG       La lunghezza del più lungo dei messaggi è troppo grande
   -------------------------------------------------------------------------------------------------
*/

{
  struct ndEvent_Type Event;
  
  unsigned short int PixelLenStringX;                                                                                                  
  unsigned short int CenterX1, CenterX2;
  
  unsigned short int aX1, aY1, aX2, aY2;
  
  char Button1Handle, Button2Handle;
  char YouCanExit;
  
  char LenStrMax;
                                                                                                    
  if (WindowData [WndHandle].WindowIsActive)
  {
    if (strlen (Message1) > strlen (Message2))
      LenStrMax = strlen (Message1);
    else
      LenStrMax = strlen (Message2);
                              
    PixelLenStringX = 30 + LenStrMax*8;
    
    if (PixelLenStringX < WindowData [WndHandle].WSWndLength/2)
    {
       CenterX1 = (0.5 * (float) (WindowData [WndHandle].WSWndLength / 2));
       CenterX2 = (1.5 * (float) (WindowData [WndHandle].WSWndLength / 2));
       
       aX1 = CenterX1 - PixelLenStringX/2;
       aY1 = PosY1;
       aX2 = CenterX1 + PixelLenStringX/2;
       aY2 = PosY2;
       
       Button1Handle = ndCTRL_CreateButton (aX1, aY1, aX2, aY2, WndHandle, "Button1", Message1, "", 
                    ColorText, ColorButton, ColorBorderButton, ND_BUTTON_ROUNDED, 0, 0, NORENDER);
       
       aX1 = CenterX2 - PixelLenStringX/2;
       aY1 = PosY1;
       aX2 = CenterX2 + PixelLenStringX/2;
       aY2 = PosY2;
       
       Button2Handle = ndCTRL_CreateButton (aX1, aY1, aX2, aY2, WndHandle, "Button2", Message2, "", 
                    ColorText, ColorButton, ColorBorderButton, ND_BUTTON_ROUNDED, 0, 0, NORENDER);
       
       XWindowRender (WndHandle);

       YouCanExit=0;

       while (!YouCanExit)
       {
         MouseControl (&Event);

         if ( (Event.__TypeEvent==CUSTOM_BUTTON_PRESSED) && (Event.__WindowID==WndHandle) )
         {
             if ( Event.__ButtonID==Button1Handle )
             {
               YouCanExit=1;
               return 0;
             }
             
             if ( Event.__ButtonID==Button2Handle )
             {
               YouCanExit=1;
               return 1;
             }
         }
         
         if (( Event.__TypeEvent == A_WINDOW_WANTS_TO_BE_CLOSED ) && (Event.__WindowID==WndHandle))
         {
            YouCanExit=1;
            if (AutoClose) ndLP_DestroyWindow (WndHandle);
               
            return DIALOG_BOX_EXIT;  
         }

       }

    }
    else
     return   ERR_MESSAGE_IS_TOO_LONG;       
  }
  else
   return ERR_WINDOW_WAS_NOT_ACTIVE;
                                                                                                    
}
