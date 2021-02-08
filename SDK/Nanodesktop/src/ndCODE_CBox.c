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
    ROUTINES PER LA GESTIONE DEI C-BOX (Control box)
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>




void INTERNAL_CBoxRenderer (unsigned short int PosX, unsigned short int PosY, char IsOn, char IsInhibited, TypeColor MainColor, char WndHandle, char RenderNow)
{
    int CounterX, CounterY;
    
    if (IsInhibited) 
      MainColor = INTERNAL_ConvertToDarkGray (MainColor);
    
    ndWS_DrawRectangle (PosX, PosY, PosX+12, PosY+12, MainColor, COLOR_BLACK, WndHandle, NORENDER);
    
    if (IsOn)
    {
            for (CounterY=0; CounterY<12; CounterY++)
            {
                for (CounterX=0; CounterX<12; CounterX++)
                {
                      if (CBoxCross [CounterY][CounterX]=='x')
                      {
                           ndWS_PutPixel (PosX+CounterX, PosY+CounterY, COLOR_BLACK, WndHandle, NORENDER);
                      }
                      
                }
            }
    }
    
    if (RenderNow) XWindowRender (WndHandle);
}


ndint64 INTERNAL_CBoxEngine (char *StringID, ndint64 InfoField, char WndHandle)
{
     void (*PntToCBoxCallback)(int ActualValue, char *StringID, int CBoxHandle, char WndHandle);
     
     struct ndCBoxAuxInfo_Type *ndCBoxAuxInfo;
     char CBoxHandle;  
     
     // Recupera il CBoxHandle del pulsante che è stato premuto
     CBoxHandle = ndCTRL_FindButtonHandleByStringID (StringID, WndHandle); 
     
     if (CBoxHandle>=0)
     {
         ndCBoxAuxInfo = &(WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].AuxDataArea [0]); 
         ndCBoxAuxInfo->IsOn = !(ndCBoxAuxInfo->IsOn);
         
         INTERNAL_CBoxRenderer (WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosX1, 
                            WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosY1, 
                            ndCBoxAuxInfo->IsOn, ndCBoxAuxInfo->IsInhibited, ndCBoxAuxInfo->MainColor, 
                            WndHandle, RENDER);                  
                 
         // Aggiorna il valore della variabile
         
         if ((ndCBoxAuxInfo->AdrVariable)!=0) 
         {
            *(ndCBoxAuxInfo->AdrVariable) = ndCBoxAuxInfo->IsOn;        
         }
         
         // Esegui la cbox callback
         
         if ((ndCBoxAuxInfo->CBoxCallback)!=0)
         {
               PntToCBoxCallback   = ndCBoxAuxInfo->CBoxCallback;
               (*PntToCBoxCallback)  (ndCBoxAuxInfo->IsOn, StringID, CBoxHandle, WndHandle);                             
         }
         
         ndHAL_Delay (0.5);
     }
     
     return 0;
}



int ndCBOX_CreateCBox (unsigned short int PosX, unsigned short int PosY, char *StringID, char IsOn, ndint64 Features, 
                        int *AdrVariable, TypeColor MainColor, void *CBoxCallback, char WndHandle, char RenderNow)
/*
    Questa routine crea una casella di spunta (control box) sullo schermo. 
    
    La cbox viene creata nella posizione PosX/PosY. 
    La stringa StringID è un'etichetta che viene associata alla cbox.
    
    Il parametro IsOn specifica se la cbox deve essere inizialmente settata o meno. 
    
    Il parametro Features è un parametro a 64 bit che permette di specificare le
    caratteristiche della C-Box: sono supportate le seguenti caratteristiche
    
    ND_CBOX_INHIBITED       La CBOX è inibita
    ND_EXEC_IN_NEW_CONTEXT  La callback associata alla casella di spunta viene eseguita
                            in un nuovo contesto
    
    L'indirizzo AdrVariable è un puntatore ad una variabile di controllo: essa viene
    settata ad 1 o a 0 prima di richiamare la cbox-callback. Le routines possono
    verificare facilmente lo stato di una c-box, semplicemente controllando
    il valore della variabile di controllo.
    
    Il parametro CBoxCallback è l'indirizzo della routine callback. Una cbox callback
    ha prototipo
    
    void CBoxCallback (int ActualValue, char *StringID, int CBoxHandle, char WndHandle)
    
    Il parametro ActualValue passato nella chiamata alla callback è il valore aggiornato
    della variabile associata alla cbox (in altre parole, la callback viene richiamata
    dopo che il valore è stato aggiornato e non prima)
    
    La routine restituisce in uscita il c-box handle; se viene restituito un valore
    negativo, siamo in presenza di un errore. I codici di errore previsti sono:
              
    ERR_WINDOW_IS_NOT_ALLOCATED           La finestra non è allocata
    ERR_NO_BUTTON_RECORD_IS_AVAILABLE     Ogni finestra può contenere al massimo 64 cboxes
    ERR_BUTTON_OUT_OF_WS                  La cbox andrebbe fuori dal window space
    ERR_STRINGID_TOO_LONG                 La StringID non può superare 32 caratteri
    ERR_SAME_STRINGID_IS_PRESENT          E' già presente la medesima stringID
    ERR_INTERNAL_ERROR                    Errore interno
*/
{
    struct ndCBoxAuxInfo_Type *ndCBoxAuxInfo;
    
    int     ButtonHandle;
    ndint64 ButtonFeatures = 0;
    
    char    IsInhibited;
    
    // Prepariamo la creazione del pulsante
    
    ButtonFeatures = ButtonFeatures | ND_BUTTON_LOGIC_MODE | ND_TRASPARENT_CALLBACK;
    
    if ((Features & ND_CBOX_INHIBITED) == ND_CBOX_INHIBITED)
    {
       ButtonFeatures = ButtonFeatures | ND_BUTTON_INHIBITED;
       IsInhibited    = 1;
    }
    else
    {
       IsInhibited    = 0; 
    }
       
    if ((Features & ND_CALLBACK_IN_NEW_CONTEXT) == ND_CALLBACK_IN_NEW_CONTEXT)
       ButtonFeatures = ButtonFeatures | ND_CALLBACK_IN_NEW_CONTEXT;
       
    ButtonHandle = ndCTRL_CreateButton      (PosX, PosY, PosX+12, PosY+12, WndHandle,
                                             StringID, "", "", 0, 0, 0,
                                             ButtonFeatures, &INTERNAL_CBoxEngine, 0, NORENDER);
                                             
    if (ButtonHandle>=0)     // Nessun problema nella creazione del pulsante
    {
        // Usa l'AuxInfo area della callback per memorizzare delle informazioni sulla c-box
        ndCBoxAuxInfo = &(WindowData [WndHandle].ButtonClass4_Data [ButtonHandle].AuxDataArea [0]);
        ndCBoxAuxInfo->AdrVariable  = AdrVariable;
        ndCBoxAuxInfo->IsOn         = IsOn;
        ndCBoxAuxInfo->IsInhibited  = IsInhibited;
        ndCBoxAuxInfo->MainColor    = MainColor;
        ndCBoxAuxInfo->CBoxCallback = CBoxCallback;
        
        // Setta ad una valore coerente *AdrVariable
        if (AdrVariable!=0) *AdrVariable=IsOn;                 
           
        // Disegna la cbox
        INTERNAL_CBoxRenderer (PosX, PosY, IsOn, IsInhibited, MainColor, WndHandle, RenderNow); 
    } 
    
    return ButtonHandle;
}
      
                           

char ndCBOX_DestroyCBox (int CBoxHandle, char WndHandle, char OnlyLogical, TypeColor Color, char RenderNow)
/*
    Si occupa di deallocare una c-box. Il significato dei parametri è identico a quello di
    ndCTRL_DestroyButton
*/
{
    return ndCTRL_DestroyButton (CBoxHandle, WndHandle, OnlyLogical, Color, RenderNow); 
}


char ndCBOX_InhibitCBox (int CBoxHandle, char WndHandle, char RenderNow)
/*
   Inibisce una cbox. Sono previsti i seguenti codici di errore
   
   ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
   ERR_BUTTON_IS_NOT_ACTIVE        Il pulsante non è attivo 
*/
{
   struct ndCBoxAuxInfo_Type *ndCBoxAuxInfo;
   char ErrRep;
   
   ErrRep = ndCTRL_InhibitButton (CBoxHandle, WndHandle, RenderNow);
   
   if (!ErrRep)
   {
       ndCBoxAuxInfo = &(WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].AuxDataArea [0]);
       ndCBoxAuxInfo->IsInhibited  = 1;
        
       INTERNAL_CBoxRenderer (WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosX1, 
                              WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosY1, 
                              ndCBoxAuxInfo->IsOn, 
                              ndCBoxAuxInfo->IsInhibited, 
                              ndCBoxAuxInfo->MainColor, WndHandle, RenderNow);         
   }
   
   return ErrRep;                                
}


char ndCBOX_DeInhibitCBox (int CBoxHandle, char WndHandle, char RenderNow)
/*
   Deinibisce una cbox. Sono previsti i seguenti codici di errore
   
   ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
   ERR_BUTTON_IS_NOT_ACTIVE        Il pulsante non è attivo 
*/
{
   struct ndCBoxAuxInfo_Type *ndCBoxAuxInfo;
   char ErrRep;
   
   ErrRep = ndCTRL_DeInhibitButton (CBoxHandle, WndHandle, RenderNow);
   
   if (!ErrRep)
   {
       ndCBoxAuxInfo = &(WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].AuxDataArea [0]);
       ndCBoxAuxInfo->IsInhibited  = 0;
        
       INTERNAL_CBoxRenderer (WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosX1, 
                              WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosY1, 
                              ndCBoxAuxInfo->IsOn, 
                              ndCBoxAuxInfo->IsInhibited, 
                              ndCBoxAuxInfo->MainColor, WndHandle, RenderNow);         
   }
   
   return ErrRep;                                
}


char ndCBOX_SetCboxOn (int CBoxHandle, char WndHandle, char ExecCallBack, char RenderNow)
/*
   Setta ad 1 il valore associato ad una cbox. Se l'utente pone a 1 il parametro
   ExecCallBack, dopo avere modificato il valore associato alla c-box, il sistema
   richiama la relativa CboxCallback per processare il valore modificato
   
   Sono previsti i seguenti codici di errore
   
   ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
   ERR_BUTTON_IS_NOT_ACTIVE        Il pulsante non è attivo 
*/
{
   void (*PntToCBoxCallback)(int ActualValue, char *StringID, int CBoxHandle, char WndHandle);
   
   struct ndCBoxAuxInfo_Type *ndCBoxAuxInfo;
   char   *StringID;
   
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].ButtonIsActivated)
      {  
           ndCBoxAuxInfo = &(WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].AuxDataArea [0]);
           ndCBoxAuxInfo->IsOn  = 1;
       
           // Aggiorna dal punto di vista grafico
       
           INTERNAL_CBoxRenderer (WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosX1, 
                            WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosY1, 
                            ndCBoxAuxInfo->IsOn, ndCBoxAuxInfo->IsInhibited, ndCBoxAuxInfo->MainColor, 
                            WndHandle, RenderNow);  
           
           // Aggiorna il valore della variabile
         
           if ((ndCBoxAuxInfo->AdrVariable)!=0) 
           {
              *(ndCBoxAuxInfo->AdrVariable) = ndCBoxAuxInfo->IsOn;        
           }  
           
           // Esegui la cbox callback
         
           if (  ((ndCBoxAuxInfo->CBoxCallback)!=0)   &&   (ExecCallBack)   )
           {
              StringID = &(WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].StringID);
              
              PntToCBoxCallback    = ndCBoxAuxInfo->CBoxCallback;
              (*PntToCBoxCallback)  (ndCBoxAuxInfo->IsOn, StringID, CBoxHandle, WndHandle);                             
           }
           
           return 0;
                            
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;                              
}


char ndCBOX_SetCboxOff (int CBoxHandle, char WndHandle, char ExecCallBack, char RenderNow)
/*
   Setta ad 0 il valore associato ad una cbox. Se l'utente pone a 1 il parametro
   ExecCallBack, dopo avere modificato il valore associato alla c-box, il sistema
   richiama la relativa CboxCallback per processare il valore modificato
   
   Sono previsti i seguenti codici di errore
   
   ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
   ERR_BUTTON_IS_NOT_ACTIVE        Il pulsante non è attivo 
*/
{
   void (*PntToCBoxCallback)(int ActualValue, char *StringID, int CBoxHandle, char WndHandle);
   
   struct ndCBoxAuxInfo_Type *ndCBoxAuxInfo;
   char   *StringID;
   
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].ButtonIsActivated)
      {  
           ndCBoxAuxInfo = &(WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].AuxDataArea [0]);
           ndCBoxAuxInfo->IsOn  = 0;
       
           // Aggiorna dal punto di vista grafico
       
           INTERNAL_CBoxRenderer (WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosX1, 
                            WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].RRPosY1, 
                            ndCBoxAuxInfo->IsOn, ndCBoxAuxInfo->IsInhibited, ndCBoxAuxInfo->MainColor, 
                            WndHandle, RenderNow);  
           
           // Aggiorna il valore della variabile
         
           if ((ndCBoxAuxInfo->AdrVariable)!=0) 
           {
              *(ndCBoxAuxInfo->AdrVariable) = ndCBoxAuxInfo->IsOn;        
           }  
           
           // Esegui la cbox callback
         
           if (  ((ndCBoxAuxInfo->CBoxCallback)!=0)   &&   (ExecCallBack)   )
           {
              StringID = &(WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].StringID);
              
              PntToCBoxCallback    = ndCBoxAuxInfo->CBoxCallback;
              (*PntToCBoxCallback)  (ndCBoxAuxInfo->IsOn, StringID, CBoxHandle, WndHandle);                             
           }
           
           return 0;
                            
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;                              
}



char ndCBOX_ChangeCallback (int CBoxHandle, char WndHandle, void* NewCallBack)
/*
    Questa routine consente di cambiare la callback associata ad una c-box
    
    Sono previsti i seguenti codici di errore:
     
     ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata
     ERR_BUTTON_IS_NOT_ACTIVE        La cbox non è attiva      
*/
{
   struct ndCBoxAuxInfo_Type *ndCBoxAuxInfo;
   
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].ButtonClass4_Data [CBoxHandle].ButtonIsActivated)
      {
           ndCBoxAuxInfo->CBoxCallback = NewCallBack;
           
           return 0;   
      }
      else return ERR_BUTTON_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;          
}





