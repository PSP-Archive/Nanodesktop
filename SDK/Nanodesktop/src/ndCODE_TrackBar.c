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
    ROUTINES PER LA GESTIONE DELLE PROGRESS-BAR
    ----------------------------------------------------------------------------
    Queste routine si occupano della gestione delle progress-bars e delle 
    track-bars del sistema.
    
    Tutte le routines che si occupano della gestione delle progress-bar o delle
    trackbar hanno un prefisso ndTBAR_ 
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

/* 
   ---------------------------------------------------------------------------
                      Codice per la gestione delle progress-bar
   ---------------------------------------------------------------------------
*/


char ndTBAR_DrawProgressBar (unsigned short int RRPosX1, unsigned short int RRPosY1,
                             unsigned short int RRPosX2, unsigned short int RRPosY2,
                             float Value, float MinValue, float MaxValue, 
                             TypeColor StringColor, TypeColor BarColor, 
                             TypeColor BGBarColor, TypeColor BorderColor, 
                             ndint64 Attribute, char WndHandle, 
                             char RenderNow)
/*
    Questa routine si occupa di disegnare una barra di avanzamento sullo schermo
    nella posizione indicata e nella finestra WndHandle. 
    
    Value rappresenta il valore corrente della variabile, MinValue il valore minimo
    e MaxValue rappresenta il valore massimo. 
    
    Il campo Attribute è un campo a 64 bit i cui bit hanno il seguente significato.
    
    I quattro bit (3-0) meno significativi indicano la precisione con cui deve essere riportato
    il valore: 0 indica che il valore deve essere troncato e riportato intero, 1 che
    il valore deve essere riportato troncandolo alla prima cifra decimale, 2 che occorre
    troncarlo alla seconda cifra decimale ecc. E' chiaro che in questo modo è possibile
    settare una precisione fino alla sedicesima cifra decimale. 
    
    La routine eseguirà comunque un controllo allo scopo di assicurarsi che la stringa
    che deve sovrapporsi alla barra di avanzamento, con le cifre decimali, non vada
    fuori dal rettangolo di contenimento. Se è così, allora le cifre decimali meno
    significative saranno troncate.
    
    I quattro bit (7-4) immediatamente più a sinistra, stabiliscono la modalità con cui
    deve essere rappresentato il numero sopra alla barra. 
    
    Se il bit nr. 4 viene posto a 1 (basta usare un OR con la costante PBAR_PERCENT), il
    valore verrà rappresentato in percentuale.
    
    Se il bit nr. 6 viene posto a 1 (basta un OR con la costante PBAR_NOT_LABEL), non
    vi sarà indicazione della percentuale di avanzamento
    
    I bit 31-24 rappresentano un codice a 8 bit, che contiene l'handle del font che
    deve essere utilizzato. L'utente può settare tali bit per mezzo della funzione
    NDKEY_FONT. Se tali bit non vengono settati, Nanodesktop userà il font che è 
    attualmente attivo nel WindowSpace della finestra considerata. 
    
    I 32 bit più significativi del parametro Attribute sono riservati ad usi futuri e non
    sono attualmente utilizzati
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_VALUE_MISMATCH            Non è rispettata la disequ. MinValue < Value < MaxValue  
         ERR_FONTHDL_IS_NOT_ACTIVE     Il font handle non è attivo

*/
{
   float DiffX, Diff;
   float XValue;
   
   unsigned short int Length;
   unsigned short int KPosX, KPosY;
   
   char InternalBuffer [64];
   char ControlString1 [10];
   char ControlString2 [10];
   
   char Percent, NrFPChars, NrIntChars, Overflow;
   char YouCanExit;
   
   long int IntRegister0;
   
   char MaxCharsVisualizable;
   
   unsigned char FntHandle;
   ndint64 FontCode;
   
   DiffX = Value - MinValue;
   Diff  = MaxValue - MinValue;
   
   Length = (DiffX/Diff)*(RRPosX2 - RRPosX1 - 4);      
         
   if (WindowData [WndHandle].WindowIsActive)
   {
      FntHandle = (unsigned char)(Attribute >> (31-8));
      
      if (FntHandle==0) FntHandle = WindowData [WndHandle].ndWS_CurrentFntHandle;
      
      if (FontHandlesArray [FntHandle].IsActive)
      {           
          if ((MinValue<=Value) && (MaxValue>=Value))
          {
              ndWS_DrawRectangle (RRPosX1, RRPosY1, RRPosX2, RRPosY2, BGBarColor, BorderColor, WndHandle, NORENDER);
              ndWS_DrawRectangle (RRPosX1+2, RRPosY1+2, RRPosX1+2+Length, RRPosY2-2, BarColor, BarColor, WndHandle, NORENDER);
              
              Percent = ((Attribute & PBAR_PERCENT) == PBAR_PERCENT);
              NrFPChars = (char) (Attribute & 15);
       
              if (!Percent)
                 XValue=Value;
              else
                 XValue=( (Value - MinValue) / (MaxValue - MinValue) )*100; 
              
              MaxCharsVisualizable = (RRPosX2 - RRPosX1)/8;
       
              IntRegister0 = (long int)( fabs (XValue) );
              NrIntChars   = 0;
              
              if (IntRegister0>0)
              {
                 do
                 {
                    IntRegister0 = IntRegister0 / 10;
                    NrIntChars++;
                 }
                 while (IntRegister0>0);  
              }
              
              YouCanExit=0;       
              Overflow=0;
              
              while (!YouCanExit)
              {
                 // Creiamo un'opportuna control char da passare a sprintf
                 
                 strcpy (&ControlString1, "%");
                 _itoa (NrIntChars, &ControlString2, 10);
                 strcat (&ControlString1, ".");
                 _itoa (NrFPChars,  &ControlString2, 10);
                 strcat (&ControlString1, &ControlString2);
                 strcat (&ControlString1, "f");
                 
                 sprintf (&InternalBuffer, &ControlString1, XValue);
                 
                 if (Percent) strcat (&InternalBuffer, " %");
                 // Aggiungi il simbolo di percentuale
                 
                 if (strlen (InternalBuffer)<MaxCharsVisualizable) break;          // La stringa ora a noi va bene
                 
                 // Se la stringa non va bene, tenta di ridurre il numero di caratteri a virgola
                 // mobile di un unità
                 
                 if (NrFPChars>0)
                    NrFPChars--;      // Prova diminuendo il numero di cifre dopo la virgola
                 else
                 {
                    strcpy (&InternalBuffer, "...");     // Non è più possibile ridurre il numero di cifre
                    Overflow=1;                          // Segnala overflow ed esci 
                    break;  
                 } 
              }
              
              if (!(Attribute & PBAR_NOT_LABEL))
              {
                  KPosY = RRPosY1 + ( (RRPosY2 - RRPosY1 - 8)/2 );
                  KPosX = RRPosX1 + ( (RRPosX2 - RRPosX1 - strlen (InternalBuffer)*8)/2 );
                  
                  FontCode = NDKEY_FONT (FntHandle);
                  ndWS_GrWriteLn (KPosX, KPosY, &InternalBuffer, StringColor, 0, WndHandle, FontCode | NDKEY_SETFNTSIZE (8) | TRASP | NORENDER);
              }
              
              if (RenderNow) XWindowRender (WndHandle);
              
              return 0;
          }
          else return ERR_VALUE_MISMATCH;
       }
       else return ERR_FONTHDL_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
   
} 

/* 
   ---------------------------------------------------------------------------
                      Codice per la gestione delle track-bars
   ---------------------------------------------------------------------------
*/

/* 
   ----------------------------------------------------------------------------
   INTERNAL: Costanti manifeste non pubbliche
   ----------------------------------------------------------------------------
*/

#define ND_TBBODY_CREATED           1
#define ND_MINUS_BUTTON_CREATED     2
#define ND_PBAR_CREATED             4
#define ND_PLUS_BUTTON_CREATED      8
#define ND_INFO_BUTTON_CREATED      16
#define ND_MINUS_STBUTTON_CREATED   32
#define ND_PLUS_STBUTTON_CREATED    64

/*
   ----------------------------------------------------------------------------
   INTERNAL: Routines di servizio per la creazione delle trackbars
   ----------------------------------------------------------------------------
*/

void INTERNAL_ElabLabel (char *Stringa1, char *Stringa2, char Number)
{
     char Buffer [5];
     
     strcpy (Stringa1, Stringa2);
     strcat (Stringa1, "(");
     _itoa (Number, &Buffer, 10);
     strcat (Stringa1, Buffer);
     strcat (Stringa1, ")");
} 

float INTERNAL_GetStep (char StepCode)
{
   switch (StepCode)
   {
          case -4:
               return 0.0001;
          case -3:
               return 0.001;
          case -2:
               return 0.01;
          case -1:
               return 0.1;
          case 0:
               return 1;
          case 1:
               return 10;
          case 2:
               return 100;
          case 3:
               return 1000;
          case 4:
               return 10000;
          default:
               return 0;   
   }
}

void INTERNAL_GetStepString (char StepCode, char *Str)
{
  switch (StepCode)
  {
          case -4:
               strcpy (Str, "0.0001");
               return;
          case -3:
               strcpy (Str, "0.001");
               return;
          case -2:
               strcpy (Str, "0.01");
               return;
          case -1:
               strcpy (Str, "0.1");
               return;
          case 0:
               strcpy (Str, "1");
               return;
          case 1:
               strcpy (Str, "10");
               return;
          case 2:
               strcpy (Str, "100");
               return;
          case 3:
               strcpy (Str, "1000");
               return;
          case 4:
               strcpy (Str, "10000");
               return;
          default:
               strcpy (Str, "");
               return;   
  }   
}

char INTERNAL_GetStepCodeFromTrackAttribute (ndint64 TrackAttribute)
{
  char StepCode;

  StepCode=0;          // Controlliamo i valori dello step. Step posto a 10^0=1 di default   

  if ((TrackAttribute & TBBAR_STEP_1)       == TBBAR_STEP_1)       StepCode=0;
  if ((TrackAttribute & TBBAR_STEP_10)      == TBBAR_STEP_10)      StepCode=1;
  if ((TrackAttribute & TBBAR_STEP_100)     == TBBAR_STEP_100)     StepCode=2;
  if ((TrackAttribute & TBBAR_STEP_1000)    == TBBAR_STEP_1000)    StepCode=3;
  if ((TrackAttribute & TBBAR_STEP_10000)   == TBBAR_STEP_10000)   StepCode=4;
  
  if ((TrackAttribute & TBBAR_STEP_1_E01)   == TBBAR_STEP_1_E01)   StepCode=-1;
  if ((TrackAttribute & TBBAR_STEP_1_E02)   == TBBAR_STEP_1_E02)   StepCode=-2;
  if ((TrackAttribute & TBBAR_STEP_1_E03)   == TBBAR_STEP_1_E03)   StepCode=-3;
  if ((TrackAttribute & TBBAR_STEP_1_E04)   == TBBAR_STEP_1_E04)   StepCode=-4;

  return StepCode;
}


unsigned short int INTERNAL_CalcLengthBar (char TBHandle, char WndHandle)
{
   unsigned short int LenX;
   
   LenX = WindowData [WndHandle].TrackBar [TBHandle].TBar_X2 - WindowData [WndHandle].TrackBar [TBHandle].TBar_X1;
   
   if (WindowData [WndHandle].TrackBar [TBHandle].StepButtonOn)
      return LenX-145;
   else
      return LenX-55;
} 


unsigned short int INTERNAL_CalcLengthStepDisplay (char TBHandle, char WndHandle)
{
   if (WindowData [WndHandle].TrackBar [TBHandle].StepButtonOn)
      return 60;
   else
      return 24;   
}

unsigned short int ndTBAR_ValidateLenXTrackBar (char ChangeStepEnabled)
{
   if (ChangeStepEnabled)
      return 240;        // La minima dimensione consentita è 240 con ChangeStep attivato
   else
      return 150;        // Mentre è 150 quando ChangeStep è disattivato
}

unsigned short int ndTBAR_ValidateLenYTrackBar (char ChangeStepEnabled)
{
   if (ChangeStepEnabled)
      return 28;        // La minima dimensione consentita è 28 con ChangeStep attivato
   else
      return 20;         // Mentre è 20 quando ChangeStep è disattivato
}





/* 
  ----------------------------------------------------------------------------
  INTERNAL: Routines grafiche per la creazione degli elementi grafici delle
  trackbar
  ----------------------------------------------------------------------------
*/


void INTERNAL_DrawTrackBarBody (char TBHandle, char WndHandle)
{
      unsigned short int RXPosX1, RXPosY1, RXPosX2, RXPosY2;
      TypeColor XTBarColor, XTBarBorderColor;  
     
      RXPosX1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1;
      RXPosY1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1;
      RXPosX2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X2;
      RXPosY2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2;
      
      if (!(WindowData [WndHandle].TrackBar [TBHandle].IsInhibited))
      {
         XTBarColor       = WindowData [WndHandle].TrackBar [TBHandle].TBarColor;
         XTBarBorderColor = WindowData [WndHandle].TrackBar [TBHandle].TBarBorderColor; 
      }
      else
      {
         XTBarColor       = INTERNAL_ConvertToDarkGray (WindowData [WndHandle].TrackBar [TBHandle].TBarColor);
         XTBarBorderColor = WindowData [WndHandle].TrackBar [TBHandle].TBarBorderColor; 
      }
        
        
      ndWS_DrawRectangle (RXPosX1, RXPosY1, RXPosX2, RXPosY2, XTBarColor, XTBarBorderColor, WndHandle, NORENDER);  
      // Disegna il rettangolo che dovrà contenere la trackbar  
      
      WindowData [WndHandle].TrackBar [TBHandle].Register = WindowData [WndHandle].TrackBar [TBHandle].Register | ND_TBBODY_CREATED;
      // Registra che il TBBarBody è stato creato   
       
}


void INTERNAL_DrawMinusButton (char TBHandle, char WndHandle, int TB_CallBack, int TB_InfoField)
{
   char Label [32];
   
   static ndint64 InfoField;  
   unsigned short int RXPosX1, RXPosY1, RXPosX2, RXPosY2;
   char BtnHandle;
   struct ndImage_Type IconImage;
       
   if ( (WindowData [WndHandle].TrackBar [TBHandle].Register & ND_MINUS_BUTTON_CREATED) != ND_MINUS_BUTTON_CREATED)
   {  
      RXPosX1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+5;
      RXPosY1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1+5;
      RXPosX2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+25;
      RXPosY2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2-5;
      
      // Creiamo il pulsante minus. E' necessario programmare la InfoCallback per fornire
      // le necessarie informazioni a INTERNAL_EngineChangeValue
   
      InfoField = TBHandle;
   
                                      // Nel campo InfoField al secondo banco da 32 bit
      InfoField = InfoField << 32;    // c'è il numero di trackbar handle
   
      InfoField = InfoField | 0;      // Il bit posto a 0 nel primo banco a 32 bit indicherà 
                                      // che si vuole decrementare il valore
   
      // Infine, registra nel record la TB_CallBack ed il parametro TB_InfoField
   
      WindowData [WndHandle].TrackBar [TBHandle].TB_CallBack  = TB_CallBack;
      WindowData [WndHandle].TrackBar [TBHandle].TB_InfoField = TB_InfoField;
      
      int InhibitFlag = (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)*ND_BUTTON_INHIBITED; 
        
      // Alloca la nuova immagine per l'icona
      
      ndIMG_LoadImageFromNDFArray (&IconImage, MinusSignWidth, MinusSignHeight, &MinusSign, NDMGKNB);
      
      // Crea un'etichetta
      
      INTERNAL_ElabLabel (&Label, "Minus", TBHandle);
      
      // Alloca il nuovo pulsante
   
      BtnHandle = ndCTRL_CreateButtonComplex (RXPosX1, RXPosY1, RXPosX2, RXPosY2, WndHandle, 
                           &Label, "", "", COLOR_BLACK, COLOR_GRAY, COLOR_WHITE, 
                           0, 0, 0, 0, &IconImage, 2, 2, 18, RXPosY2-RXPosY1-2,
                           ND_TRASPARENT_CALLBACK | ND_BUTTON_ROUNDED | InhibitFlag, 
                           &INTERNAL_EngineChangeValue, InfoField, NORENDER);
                           
      
      // Rilascia lo spazio per l'icona
      ndIMG_ReleaseImage (&IconImage);
   
      // Registra l'handle del pulsante
      WindowData [WndHandle].TrackBar [TBHandle].MinusButtonHandle = BtnHandle;
      
      // Registra la creazione dell'elemento
      WindowData [WndHandle].TrackBar [TBHandle].Register = 
           WindowData [WndHandle].TrackBar [TBHandle].Register | ND_MINUS_BUTTON_CREATED;
   }
   else  
   {
      // Il pulsante è già stato creato: è necessario solo eseguire controllo di inibizione/deinibizione   
         
      if (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)
         ndCTRL_InhibitButton   (WindowData [WndHandle].TrackBar [TBHandle].MinusButtonHandle, WndHandle, NORENDER);
      else
         ndCTRL_DeInhibitButton (WindowData [WndHandle].TrackBar [TBHandle].MinusButtonHandle, WndHandle, NORENDER);
         
   }  
}




void INTERNAL_RenderProgressBar (char TBHandle, char WndHandle, char RenderNow)
{  
   unsigned short int RXPosX1, RXPosY1, RXPosX2, RXPosY2;
   TypeColor __TextColor, __PBarColor, __PBarBkgrColor, __PBarBorderColor; 
                               
   char Corrected_Attribute;
   char PB_Resolution;
   
   ndint64 FontCode;
   
   // Determina la posizione della progress-bar
   
   RXPosX1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1 + 30;
   RXPosY1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1 + 5;
   RXPosX2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1 + INTERNAL_CalcLengthBar (TBHandle, WndHandle);
   RXPosY2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2 - 5;
   
   // Quando si disegna la progress bar, se la risoluzione dello step è più alta
   // quella della progress-bar, è necessario aumentare la risoluzione della progress-bar
   // in modo da compensare il problema.    
     
   if (WindowData [WndHandle].TrackBar [TBHandle].StepCode>=0)
   {
         Corrected_Attribute = WindowData [WndHandle].TrackBar [TBHandle].Attribute;
         // Quando lo step code è un numero maggiore o uguale ad 1, non è necessario alcun
         // processo di correzione della risoluzione della progress bar
   }
   else  // StepCode negativo
   {
         PB_Resolution = - (WindowData [WndHandle].TrackBar [TBHandle].Attribute & 0x0F);
         // La risoluzione della progress-bar si ottiene dagli ultimi 4 bit con il segno
         // cambiato. Ad esempio, se la progress-bar ha gli ultimi quattro bit posti a 0010,
         // ciò significa che la risoluzione è 10^-2 (che corrisponde ad uno step code di -2)
       
         if (WindowData [WndHandle].TrackBar [TBHandle].StepCode < PB_Resolution)
         {
            Corrected_Attribute = - (WindowData [WndHandle].TrackBar [TBHandle].StepCode);
            // La risoluzione della progress-bar viene ottenuta cambiando il segno dello
            // step-code. Si noti che in questo punto del codice lo step-code è negativo,
            // quindi di certo il risultato è maggiore di 0   
         } 
         else // Non è necessaria alcuna correzione
         {
            Corrected_Attribute = WindowData [WndHandle].TrackBar [TBHandle].Attribute;
         }
   }
   
   // Gestione percentuale e label
   
   if ((WindowData [WndHandle].TrackBar [TBHandle].Attribute & TBBAR_PERCENT) == TBBAR_PERCENT)
   {
      Corrected_Attribute = Corrected_Attribute | PBAR_PERCENT;
      // Aggiungi all'attributo corretto l'eventuale bit di segnalazione percent
   } 
    
   if ((WindowData [WndHandle].TrackBar [TBHandle].Attribute & TBBAR_NOT_LABEL) == TBBAR_NOT_LABEL)
   {
      Corrected_Attribute = Corrected_Attribute | PBAR_NOT_LABEL;
      // Aggiungi all'attributo corretto l'eventuale bit di segnalazione not label
   }  

   // Eseguiamo una eventuale conversione colori
   
   if (!(WindowData [WndHandle].TrackBar [TBHandle].IsInhibited))
   {
     __TextColor       = WindowData [WndHandle].TrackBar [TBHandle].TextColor; 
     __PBarColor       = WindowData [WndHandle].TrackBar [TBHandle].PBarColor;
     __PBarBkgrColor   = WindowData [WndHandle].TrackBar [TBHandle].PBarBkgrColor;
     __PBarBorderColor = WindowData [WndHandle].TrackBar [TBHandle].PBarBorderColor; 
   }
   else
   {
     __TextColor       = INTERNAL_ConvertToDarkGray (WindowData [WndHandle].TrackBar [TBHandle].TextColor); 
     __PBarColor       = INTERNAL_ConvertToDarkGray (WindowData [WndHandle].TrackBar [TBHandle].PBarColor);
     __PBarBkgrColor   = INTERNAL_ConvertToDarkGray (WindowData [WndHandle].TrackBar [TBHandle].PBarBkgrColor);
     __PBarBorderColor = INTERNAL_ConvertToDarkGray (WindowData [WndHandle].TrackBar [TBHandle].PBarBorderColor); 
   }
   
   FontCode = NDKEY_FONT (WindowData [WndHandle].TrackBar [TBHandle].FntHandle);
    
   ndTBAR_DrawProgressBar (RXPosX1, RXPosY1, RXPosX2, RXPosY2, 
                           WindowData [WndHandle].TrackBar [TBHandle].Value, 
                           WindowData [WndHandle].TrackBar [TBHandle].MinValue, 
                           WindowData [WndHandle].TrackBar [TBHandle].MaxValue, 
                           __TextColor, __PBarColor, __PBarBkgrColor, __PBarBorderColor, 
                           Corrected_Attribute | FontCode, WndHandle, RenderNow);
                           
   // Registra la creazione dell'elemento
   WindowData [WndHandle].TrackBar [TBHandle].Register = 
       WindowData [WndHandle].TrackBar [TBHandle].Register | ND_PBAR_CREATED;

}


void INTERNAL_DrawPlusButton (char TBHandle, char WndHandle, int TB_CallBack, int TB_InfoField)
{
   char Label [32];
   
   static ndint64 InfoField;  
   unsigned short int RXPosX1, RXPosY1, RXPosX2, RXPosY2;
   char BtnHandle;
   struct ndImage_Type IconImage;
       
   if ( (WindowData [WndHandle].TrackBar [TBHandle].Register & ND_PLUS_BUTTON_CREATED) != ND_PLUS_BUTTON_CREATED)
   {
      RXPosX1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+5;
      RXPosY1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1+5;
      RXPosX2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+25;
      RXPosY2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2-5;
      
      // Creiamo il pulsante plus. E' necessario programmare la InfoCallback per fornire
      // le necessarie informazioni a INTERNAL_EngineChangeValue
   
      InfoField = TBHandle;
   
                                      // Nel campo InfoField al secondo banco da 32 bit
      InfoField = InfoField << 32;    // c'è il numero di trackbar handle
   
      InfoField = InfoField | 1;      // Il bit posto a 1 nel primo banco a 32 bit indicherà 
                                      // che si vuole incrementare il valore
   
      // Infine, registra nel record la TB_CallBack ed il parametro TB_InfoField
   
      WindowData [WndHandle].TrackBar [TBHandle].TB_CallBack  = TB_CallBack;
      WindowData [WndHandle].TrackBar [TBHandle].TB_InfoField = TB_InfoField;
   
      int InhibitFlag = (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)*ND_BUTTON_INHIBITED; 
      
      // Alloca la nuova immagine per l'icona
      
      ndIMG_LoadImageFromNDFArray (&IconImage, PlusSignWidth, PlusSignHeight, &PlusSign, NDMGKNB);
      
      // Crea un'etichetta
      
      INTERNAL_ElabLabel (&Label, "Plus", TBHandle);
      
      // Alloca il nuovo pulsante
   
      BtnHandle = ndCTRL_CreateButtonComplex (RXPosX1, RXPosY1, RXPosX2, RXPosY2, WndHandle, 
                           &Label, "", "", COLOR_BLACK, COLOR_GRAY, COLOR_WHITE, 
                           0, 0, 0, 0, &IconImage, 2, 2, 18, RXPosY2-RXPosY1-2,
                           ND_TRASPARENT_CALLBACK | ND_BUTTON_ROUNDED | InhibitFlag, 
                           &INTERNAL_EngineChangeValue, InfoField, NORENDER);
                           
      
      // Rilascia lo spazio per l'icona
      ndIMG_ReleaseImage (&IconImage);
   
      // Registra l'handle del pulsante
      WindowData [WndHandle].TrackBar [TBHandle].PlusButtonHandle = BtnHandle;
      
      // Registra la creazione dell'elemento
      WindowData [WndHandle].TrackBar [TBHandle].Register = 
           WindowData [WndHandle].TrackBar [TBHandle].Register | ND_PLUS_BUTTON_CREATED;
   }
   else
   {
      // Il pulsante è già stato creato: è necessario solo eseguire controllo di inibizione/deinibizione   
         
      if (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)
         ndCTRL_InhibitButton   (WindowData [WndHandle].TrackBar [TBHandle].PlusButtonHandle, WndHandle, NORENDER);
      else
         ndCTRL_DeInhibitButton (WindowData [WndHandle].TrackBar [TBHandle].PlusButtonHandle, WndHandle, NORENDER);
      
   }  
}


void INTERNAL_DrawInfoButton (char TBHandle, char WndHandle, int InfoCallBack)
{
   char Label [32];
   
   unsigned short int RXPosX1, RXPosY1, RXPosX2, RXPosY2;
   char BtnHandle;
   int  Reg;
   struct ndImage_Type IconImage;
       
   if ( (WindowData [WndHandle].TrackBar [TBHandle].Register & ND_INFO_BUTTON_CREATED) != ND_INFO_BUTTON_CREATED)
   {
      RXPosX1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+30;
      RXPosY1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1+5;
      RXPosX2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+50;
      RXPosY2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2-5;
   
      // Creiamo il pulsante info
   
      Reg = ND_BUTTON_ROUNDED | ND_TRASPARENT_CALLBACK; 
      
      if ( (InfoCallBack==0) || (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited) )
         Reg = Reg | ND_BUTTON_INHIBITED; 
      
      // Alloca la nuova immagine per l'icona
      
      ndIMG_LoadImageFromNDFArray (&IconImage, QuestionPointSignWidth, QuestionPointSignHeight, &QuestionPointSign, NDMGKNB);
      
      // Crea un'etichetta
      
      INTERNAL_ElabLabel (&Label, "Info", TBHandle);
      
      // Alloca il nuovo pulsante
   
      BtnHandle = ndCTRL_CreateButtonComplex (RXPosX1, RXPosY1, RXPosX2, RXPosY2, WndHandle, 
                           &Label, "", "", COLOR_BLACK, COLOR_GRAY, COLOR_WHITE, 
                           0, 0, 0, 0, &IconImage, 2, 1, 18, RXPosY2-RXPosY1,
                           Reg, InfoCallBack, 0, NORENDER);
                           
      // Rilascia lo spazio per l'icona
      ndIMG_ReleaseImage (&IconImage);
   
      // Registra l'handle del pulsante
      WindowData [WndHandle].TrackBar [TBHandle].HelpButtonHandle = BtnHandle;
      
      // Registra la creazione dell'elemento
      WindowData [WndHandle].TrackBar [TBHandle].Register = 
          WindowData [WndHandle].TrackBar [TBHandle].Register | ND_INFO_BUTTON_CREATED;  
   }
   else
   {
      // Il pulsante è già stato creato: è necessario solo eseguire controllo di inibizione/deinibizione   
         
      if (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)
         ndCTRL_InhibitButton   (WindowData [WndHandle].TrackBar [TBHandle].HelpButtonHandle, WndHandle, NORENDER);
      else
         ndCTRL_DeInhibitButton (WindowData [WndHandle].TrackBar [TBHandle].HelpButtonHandle, WndHandle, NORENDER);
       
   }                     
      
}


void INTERNAL_DrawStepDisplay (char TBHandle, char WndHandle, char RenderNow)
{
   static char Str [6];
   unsigned short int LenX, LenY, StrPxLen;
   
   unsigned short int StepDisplay_X1, StepDisplay_Y1;
   unsigned short int StepDisplay_X2, StepDisplay_Y2;
   
   TypeColor __BkgrStepDisplay, __BorderStepDisplay, __ColorText;
   ndint64 FontCode;
   
   float Step;
   
   if (WindowData [WndHandle].TrackBar [TBHandle].StepButtonOn)
   {
           StepDisplay_X1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+55;
           
           StepDisplay_Y1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1+5;
           
           StepDisplay_X2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+55
                       +INTERNAL_CalcLengthStepDisplay (TBHandle, WndHandle);
           
           StepDisplay_Y2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2-5;
           
           if (!(WindowData [WndHandle].TrackBar [TBHandle].IsInhibited))
           {
               __BkgrStepDisplay   = COLOR_GRAY08;
               __BorderStepDisplay = COLOR_GRAY14;
               __ColorText         = COLOR_WHITE;
           }
           else
           {
               __BkgrStepDisplay   = INTERNAL_ConvertToDarkGray (COLOR_GRAY08);
               __BorderStepDisplay = INTERNAL_ConvertToDarkGray (COLOR_GRAY14);
               __ColorText         = INTERNAL_ConvertToDarkGray (COLOR_WHITE);
           }
                       
           ndWS_DrawRectangle (StepDisplay_X1, StepDisplay_Y1, StepDisplay_X2, StepDisplay_Y2, 
                               __BkgrStepDisplay, __BorderStepDisplay, WndHandle, NORENDER); 
        
           Step = INTERNAL_GetStep (WindowData [WndHandle].TrackBar [TBHandle].StepCode);
           // Determina il reale valore dello step
           
           INTERNAL_GetStepString (WindowData [WndHandle].TrackBar [TBHandle].StepCode, &Str);
           // Trova la corrispondente stringa
           
           LenX = StepDisplay_X2 - StepDisplay_X1;
           LenY = StepDisplay_Y2 - StepDisplay_Y1;
           
           StrPxLen = strlen (Str)*8;
           
           FontCode = NDKEY_FONT (WindowData [WndHandle].TrackBar [TBHandle].FntHandle);
           
           ndWS_GrWriteLn (StepDisplay_X1 + (LenX - StrPxLen)/2, StepDisplay_Y1 + (LenY - 8)/2, 
                           Str, __ColorText, __BkgrStepDisplay, WndHandle, NDKEY_SETFNTSIZE (8) | FontCode | NORENDER);
           
           if (RenderNow) XWindowRender (WndHandle); 
   }   
}


void INTERNAL_DrawPlusStepButton (char TBHandle, char WndHandle)
{
   char Label [32];
   
   char BtnHandle;
   unsigned short int RXPosX1, RXPosY1, RXPosX2, RXPosY2;  
     
   ndint64 InfoField;
   struct ndImage_Type IconImage;
   
   if (WindowData [WndHandle].TrackBar [TBHandle].StepButtonOn)
   {
       if ( (WindowData [WndHandle].TrackBar [TBHandle].Register & ND_PLUS_STBUTTON_CREATED) != ND_PLUS_STBUTTON_CREATED)
       {
          // Creiamo i pulsanti per il change step. I pulsanti + e - hanno bisogno di un'indicazione
          // sull'handle della callback che si sta richiamando per specificare se si tratta di uno
          // step-up, di uno step-down, ed hanno bisogno di un'ulteriore indicazione sull'handle di
          // trackbar. 
       
          RXPosX1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+55
                           +INTERNAL_CalcLengthStepDisplay (TBHandle, WndHandle)+5;
               
          RXPosY1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1+2;
               
          RXPosX2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+55
                           +INTERNAL_CalcLengthStepDisplay (TBHandle, WndHandle)+25;
               
          RXPosY2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2-2;
                        
          
          InfoField = (TBHandle << 1) + 1;
          
          int InhibitFlag = (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)*ND_BUTTON_INHIBITED; 
       
          // Alloca la nuova immagine per l'icona
      
          ndIMG_LoadImageFromNDFArray (&IconImage, TriangleUpWidth, TriangleUpHeight, &TriangleUp, NDMGKNB);
      
          // Crea un'etichetta
      
          INTERNAL_ElabLabel (&Label, "StepUp", TBHandle);
      
          // Alloca il nuovo pulsante
   
          BtnHandle = ndCTRL_CreateButtonComplex (RXPosX1, RXPosY1+1, RXPosX2-1, ((RXPosY1 + RXPosY2)/2)-1, WndHandle, 
                           &Label, "", "", COLOR_BLACK, COLOR_GRAY, COLOR_WHITE, 
                           0, 0, 0, 0, &IconImage, 4, 2, 16, ((RXPosY2 - RXPosY1)/2)-4,
                           ND_BUTTON_ROUNDED | ND_TRASPARENT_CALLBACK | InhibitFlag, 
                           &INTERNAL_EngineChangeStep, InfoField, NORENDER);
                                            
          // Rilascia lo spazio per l'icona
          ndIMG_ReleaseImage (&IconImage);
   
          // Registra l'handle del pulsante
          WindowData [WndHandle].TrackBar [TBHandle].ChStepPlusButtonHandle = BtnHandle;
      
          // Registra la creazione dell'elemento
          WindowData [WndHandle].TrackBar [TBHandle].Register = 
          WindowData [WndHandle].TrackBar [TBHandle].Register | ND_PLUS_STBUTTON_CREATED;     
       
       }
       else
       {
          // Il pulsante è già stato creato: è necessario solo eseguire controllo di inibizione/deinibizione   
         
          if (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)
             ndCTRL_InhibitButton   (WindowData [WndHandle].TrackBar [TBHandle].ChStepPlusButtonHandle, WndHandle, NORENDER);
          else
             ndCTRL_DeInhibitButton (WindowData [WndHandle].TrackBar [TBHandle].ChStepPlusButtonHandle, WndHandle, NORENDER);
          
       }

   }
}


void INTERNAL_DrawMinusStepButton (char TBHandle, char WndHandle)
{
   char Label [32];
   
   char BtnHandle;
   unsigned short int RXPosX1, RXPosY1, RXPosX2, RXPosY2;  
     
   ndint64 InfoField;
   struct ndImage_Type IconImage;
   
   if (WindowData [WndHandle].TrackBar [TBHandle].StepButtonOn)
   {
       if ( (WindowData [WndHandle].TrackBar [TBHandle].Register & ND_MINUS_STBUTTON_CREATED) != ND_MINUS_STBUTTON_CREATED)
       {
          // Creiamo i pulsanti per il change step. I pulsanti + e - hanno bisogno di un'indicazione
          // sull'handle della callback che si sta richiamando per specificare se si tratta di uno
          // step-up, di uno step-down, ed hanno bisogno di un'ulteriore indicazione sull'handle di
          // trackbar. 
       
          RXPosX1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+55
                           +INTERNAL_CalcLengthStepDisplay (TBHandle, WndHandle)+5;
               
          RXPosY1 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1+2;
               
          RXPosX2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_X1+INTERNAL_CalcLengthBar (TBHandle, WndHandle)+55
                           +INTERNAL_CalcLengthStepDisplay (TBHandle, WndHandle)+25;
               
          RXPosY2 = WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2-2;
                        
          InfoField = (TBHandle << 1);
          
          int InhibitFlag = (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)*ND_BUTTON_INHIBITED; 
       
          // Alloca la nuova immagine per l'icona
      
          ndIMG_LoadImageFromNDFArray (&IconImage, TriangleDownWidth, TriangleDownHeight, &TriangleDown, NDMGKNB);
      
          // Crea un'etichetta
      
          INTERNAL_ElabLabel (&Label, "StepDown", TBHandle);
      
          
          // Alloca il nuovo pulsante
   
          BtnHandle = ndCTRL_CreateButtonComplex (RXPosX1, ((RXPosY1 + RXPosY2)/2)+1, RXPosX2-1, RXPosY2-1, WndHandle, 
                           &Label, "", "", COLOR_BLACK, COLOR_GRAY, COLOR_WHITE, 
                           0, 0, 0, 0, &IconImage, 4, 3, 16, ((RXPosY2 - RXPosY1)/2)-4,
                           ND_BUTTON_ROUNDED | ND_TRASPARENT_CALLBACK | InhibitFlag, 
                           &INTERNAL_EngineChangeStep, InfoField, NORENDER);
                                            
          // Rilascia lo spazio per l'icona
          ndIMG_ReleaseImage (&IconImage);
                            
          // Registra l'handle del pulsante
          WindowData [WndHandle].TrackBar [TBHandle].ChStepMinusButtonHandle = BtnHandle;
      
          // Registra la creazione dell'elemento
          WindowData [WndHandle].TrackBar [TBHandle].Register = 
          WindowData [WndHandle].TrackBar [TBHandle].Register | ND_MINUS_STBUTTON_CREATED;     
       }
       else
       {
          // Il pulsante è già stato creato: è necessario solo eseguire controllo di inibizione/deinibizione   
         
          if (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)
             ndCTRL_InhibitButton   (WindowData [WndHandle].TrackBar [TBHandle].ChStepMinusButtonHandle, WndHandle, NORENDER);
          else
             ndCTRL_DeInhibitButton (WindowData [WndHandle].TrackBar [TBHandle].ChStepMinusButtonHandle, WndHandle, NORENDER); 
       }

   }
}


/*
   ----------------------------------------------------------------------------------
   Engines di gestione trackbar
   ----------------------------------------------------------------------------------
*/


ndint64 INTERNAL_EngineChangeStep (char *StringID, ndint64 InfoField, char WndHandle)
/* 
   Gestisce l'engine per il change degli step
*/
{                    
   char IsUp;
   char TBHandle;
   char NewStepCode;
   
   IsUp      = (char)(InfoField & 1);
   TBHandle = (char)(InfoField >> 1);
   
   if (IsUp)
   {
         NewStepCode = WindowData [WndHandle].TrackBar [TBHandle].StepCode + 1;
         
         if (NewStepCode<=4)
         {
            WindowData [WndHandle].TrackBar [TBHandle].StepCode = NewStepCode;
            INTERNAL_DrawStepDisplay (TBHandle, WndHandle, NORENDER);
            
            INTERNAL_RenderProgressBar (TBHandle, WndHandle, NORENDER);
            // Ridisegna anche la progress-bar in modo da consentire l'aggiustamento della
            // precisione
       
            XWindowRender (WndHandle);
         }
         
         return 0;
   }
   else
   {
         NewStepCode = WindowData [WndHandle].TrackBar [TBHandle].StepCode - 1;
         
         if (NewStepCode>=-4)
         {
            WindowData [WndHandle].TrackBar [TBHandle].StepCode = NewStepCode;
            INTERNAL_DrawStepDisplay (TBHandle, WndHandle, NORENDER);
            
            INTERNAL_RenderProgressBar (TBHandle, WndHandle, NORENDER);
            // Ridisegna anche la progress-bar in modo da consentire l'aggiustamento della
            // precisione
            
            XWindowRender (WndHandle);
         }
         
         return 0;
   }
   
}



ndint64 INTERNAL_EngineChangeValue (char *StringID, ndint64 InfoField, char WndHandle)
{
   // Provvede a modificare il valore gestito dalla trackbar ed ad eseguire la relativa
   // TB_Callback. Provvediamo innanzitutto ad estrarre le infomazioni che ci servono
   // dal campo InfoField. 
 
   struct ndMouseState_Type MouseState;
   
   char TBHandle;
   
   int FirstBank, SecondBank;
   
   char ValueUp;
   
   float Step, NewValue;
   float Diff, Perc, NewPerc; 
         
   // Puntatore alla TBCallBack
   void (*PointerToTBCallBack) (float Value, int TB_InfoField, unsigned char WndHandle);
   
   // Estrae i due banchi
   FirstBank  = (int)(InfoField);
   SecondBank = (int)(InfoField >> 32);
    
   ValueUp = (char)(FirstBank & 1); 
   // Dal primo banco di 32 bit estrae l'eventuale bit ad 1 che indica se aumentare
   // il valore
   
   TBHandle = (char) (SecondBank);
   // Nel secondo banco a 32 bit c'è il numero di handle della trackbar.
   // Adesso posso accedere alle informazioni necessarie
   
   Step = INTERNAL_GetStep (WindowData [WndHandle].TrackBar [TBHandle].StepCode);

   RepeatChangeValue:
  
   if (ValueUp)            // E' stato richiesto un'incremento
   {
      if (!((WindowData [WndHandle].TrackBar [TBHandle].Attribute & PBAR_PERCENT) == PBAR_PERCENT))
      {
          NewValue = WindowData [WndHandle].TrackBar [TBHandle].Value + Step;
          
          if (NewValue<=WindowData [WndHandle].TrackBar [TBHandle].MaxValue)
              WindowData [WndHandle].TrackBar [TBHandle].Value = NewValue;
          else
              WindowData [WndHandle].TrackBar [TBHandle].Value = WindowData [WndHandle].TrackBar [TBHandle].MaxValue;
      }
      else
      {
          Diff = WindowData [WndHandle].TrackBar [TBHandle].MaxValue - WindowData [WndHandle].TrackBar [TBHandle].MinValue;
          Perc = ((WindowData [WndHandle].TrackBar [TBHandle].Value - WindowData [WndHandle].TrackBar [TBHandle].MinValue)/Diff)*100.0;
          
          NewPerc = Perc + Step;
          
          if (NewPerc>100.0) NewPerc=100.0;
          WindowData [WndHandle].TrackBar [TBHandle].Value = WindowData [WndHandle].TrackBar [TBHandle].MinValue + (Diff/100.0)*NewPerc;
          
          if ( (WindowData [WndHandle].TrackBar [TBHandle].Value)>(WindowData [WndHandle].TrackBar [TBHandle].MaxValue) ) 
              WindowData [WndHandle].TrackBar [TBHandle].Value = WindowData [WndHandle].TrackBar [TBHandle].MaxValue;
      }  
   }
   else                    // E' stato richiesto un decremento
   {
      if (!((WindowData [WndHandle].TrackBar [TBHandle].Attribute & PBAR_PERCENT) == PBAR_PERCENT))
      {
          NewValue = WindowData [WndHandle].TrackBar [TBHandle].Value - Step;
          
          if (NewValue>=WindowData [WndHandle].TrackBar [TBHandle].MinValue)
              WindowData [WndHandle].TrackBar [TBHandle].Value = NewValue;
          else
              WindowData [WndHandle].TrackBar [TBHandle].Value = WindowData [WndHandle].TrackBar [TBHandle].MinValue;
      }                    
      else
      {
          Diff = WindowData [WndHandle].TrackBar [TBHandle].MaxValue - WindowData [WndHandle].TrackBar [TBHandle].MinValue;
          Perc = ((WindowData [WndHandle].TrackBar [TBHandle].Value - WindowData [WndHandle].TrackBar [TBHandle].MinValue)/Diff)*100.0;
          
          NewPerc = Perc - Step;
          
          if (NewPerc<0) NewPerc=0.0;
          WindowData [WndHandle].TrackBar [TBHandle].Value = WindowData [WndHandle].TrackBar [TBHandle].MinValue + (Diff/100.0)*NewPerc;
          
          if ( (WindowData [WndHandle].TrackBar [TBHandle].Value)<(WindowData [WndHandle].TrackBar [TBHandle].MinValue) ) 
              WindowData [WndHandle].TrackBar [TBHandle].Value = WindowData [WndHandle].TrackBar [TBHandle].MinValue;
      }
      
   }
  
   INTERNAL_RenderProgressBar (TBHandle, WndHandle, RENDER);   // Esegui il render della progress bar
       
   if ( !WindowData [WndHandle].TrackBar [TBHandle].PervasiveCallback )
   { 
           // Su molte piattaforme non è conveniente eseguire la callback per tutti i valori
           // intermedi: queste righe di codice provvedono a controllare se, dopo la prima pressione
           // del tasto questo è ancora premuto e se è così, rieseguono l'incremento o il
           // decremento per i valori intermedi senza eseguire la callback. L'esecuzione
           // della callback sarà eseguita solo quando il tasto sarà rilasciato
           
           ndHAL_GetMouseState (&MouseState);     // Ricava la posizione del mouse
          
           if ( (MouseState.Buttons & ND_BUTTON1) == ND_BUTTON1 ) 
           {
              goto RepeatChangeValue;   
           }        
   }
   
   // Esegui la callback, se prevista
  
   if (WindowData [WndHandle].TrackBar [TBHandle].TB_CallBack!=0)
   {
      PointerToTBCallBack = WindowData [WndHandle].TrackBar [TBHandle].TB_CallBack;
      // Registra l'indirizzo della TB_CallBack

      (*PointerToTBCallBack) (WindowData [WndHandle].TrackBar [TBHandle].Value, WindowData [WndHandle].TrackBar [TBHandle].TB_InfoField, WndHandle);
      // Esegue l'eventuale TB_CallBack con la TB_InfoField
   }
   
   return 0;
}



/* 
  -------------------------------------------------------------------------------
  INTERNAL: Renderer centrale per la trackbar
  
  Questa routine provvede a richiamare tutte le altre routines che creano gli
  elementi grafici della trackbar e che registrano i pulsanti.
  -------------------------------------------------------------------------------
*/


char INTERNAL_TrackBarRenderer (char TBHandle, char WndHandle, int TB_CallBack, int TB_InfoField, int TB_InfoCallBack, char RenderNow)
{
   INTERNAL_DrawTrackBarBody  (TBHandle, WndHandle);  
   INTERNAL_DrawMinusButton   (TBHandle, WndHandle, TB_CallBack, TB_InfoField);
   INTERNAL_RenderProgressBar (TBHandle, WndHandle, NORENDER);  
   INTERNAL_DrawPlusButton    (TBHandle, WndHandle, TB_CallBack, TB_InfoField);
   INTERNAL_DrawInfoButton    (TBHandle, WndHandle, TB_InfoCallBack);
   INTERNAL_DrawStepDisplay   (TBHandle, WndHandle, NORENDER);              
   INTERNAL_DrawPlusStepButton (TBHandle, WndHandle);
   INTERNAL_DrawMinusStepButton (TBHandle, WndHandle);
   
   if (RenderNow) XWindowRender (WndHandle);
   
   return 0;
}


/* 
   -----------------------------------------------------------------------------
   API pubblica per la gestione delle trackbar
   -----------------------------------------------------------------------------
*/



char ndTBAR_CreateTrackBar (unsigned short int RRPosX1, unsigned short int RRPosY1,
                            unsigned short int RRPosX2, unsigned short int RRPosY2,
                            TypeColor TextColor, TypeColor PBarColor, TypeColor PBarBkgrColor,
                            TypeColor PBarBorderColor, TypeColor TBarColor, 
                            TypeColor TBarBorderColor,
                            float Value, float MinValue, float MaxValue,
                            ndint64 TrackAttribute, int TB_CallBack, int InfoCallBack, 
                            char WndHandle, char RenderNow)

/*
    Questa routine crea una trackbar sullo schermo. L'intera trackbar, compresi i 
    pulsanti di controllo, sarà contenuta all'interno del rettangolo di coordinate
    PosX1, PosY1, PosX2, PosY2.
    
    Quando viene modificato il valore, viene eseguita la callback il cui indirizzo
    è dato da TB_Callback. Queste trackbar callback sono delle funzioni del tipo
    
    void TB_Callback (float Value, int TB_InfoField, unsigned char WndHandle);
    
    La routine InfoCallback è una diversa callback che viene richiamata quando occorre
    fornire delle informazioni sulla trackbar. Se il parametro viene posto a 0, la
    InfoCallback è disabilitata.
     
    Veniamo al parametro a 64 bit TrackAttribute. Questo parametro fornisce delle
    informazioni sul comportamento della TrackBar. 
    
    Questo viene ottenuto per mezzo di alcuni bit che vengono mappati opportunamente.
    
    I quattro bit (3-0) meno significativi indicano la precisione con cui deve essere riportato
    il valore: 0 indica che il valore deve essere troncato e riportato intero, 1 che
    il valore deve essere riportato troncandolo alla prima cifra decimale, 2 che occorre
    troncarlo alla seconda cifra decimale ecc. E' chiaro che in questo modo è possibile
    settare una precisione fino alla sedicesima cifra decimale. 
    
    La routine eseguirà comunque un controllo allo scopo di assicurarsi che la stringa
    che deve sovrapporsi alla barra di avanzamento, con le cifre decimali, non vada
    fuori dal rettangolo di contenimento. Se è così, allora le cifre decimali meno
    significative saranno troncate.
    
    I quattro bit (7-4) immediatamente più a sinistra, stabiliscono la modalità con cui
    deve essere rappresentato il numero sopra alla barra.
    
    Se il bit nr. 4 viene posto a 1 (basta usare un OR con la costante PBAR_PERCENT), il
    valore verrà rappresentato in percentuale.
    
    Il bit nr. 5, determina se devono essere attivati i pulsanti ChangeStep. Se è posto
    a 0, i pulsanti ChangeStep non vengono attivati, e lo step viene posto al valore
    standard 1. Per attivare i pulsanti ChangeStep, è necessario porre ad 1 il valore
    del bit 5 oppure eseguire un OR con la costante manifesta CHANGE_STEP_ENABLED.
    
    Il bit nr. 6 determina se deve essere visualizzata la notazione percentuale.
    Occorre eseguire un OR con la costante manifesta TBBAR_NOT_LABEL
    
    I bit (11-8) stabiliscono qual è lo step iniziale nell'ipotesi in cui il 
    pulsante ChangeStep sia attivato. Per settare lo step si usano le seguenti
    costanti manifeste (eseguire un OR con gli altri parametri usati):
             
    #define TBBAR_STEP_1        0*256

    #define TBBAR_STEP_10       1*256
    #define TBBAR_STEP_100      2*256
    #define TBBAR_STEP_1000     3*256
    #define TBBAR_STEP_10000    4*256

    #define TBBAR_STEP_1_E01    5*256
    #define TBBAR_STEP_1_E02    6*256
    #define TBBAR_STEP_1_E03    7*256
    #define TBBAR_STEP_1_E04    8*256 
    
    Il bit nr. 12 viene posto a 1 quando si vuole che la TB_Callback sia
    inizialmente inibita. Normalmente ciò viene fatto per mezzo di un OR
    con la costante manifesta TBBAR_INHIBITED.
    
    Il bit nr. 13 determina se la callback deve essere eseguita sempre dopo ogni
    millisecondo di pressione di un tasto. Usare la costante manifesta
    TBBAR_PERVASIVE_CALLBACK
    
    I bit 31-23 vengono usati per stabilire il font che deve essere usato
    dagli elementi che costituiscono la trackbar. L'utente può settare i
    bit necessari per mezzo della funzione NDKEY_FONT. Se l'utente non usa
    tale funzione, o se specifica un font handle pari a 0, Nanodesktop 
    userà il font di default nel WindowSpace della finestra
    
    I 32 bit più significativi del campo TrackAttribute costituiscono il
    campo TB_InfoField: questo valore a 32 bit viene passato alla TB_Callback
    come parametro.

    La routine, in uscita restituisce un handle per la trackbar, che la
    identifica in modo univoco nell'ambito della finestra.
    
    Sono previsti i seguenti codici di errore
    ERR_WINDOW_IS_NOT_ALLOCATED              La finestra non è allocata
    ERR_NO_TRACKBAR_RECORD_IS_AVAILABLE      Non esistono record per la visualizzare 
                                             di ulteriori trackbar
    ERR_I_NEED_MORE_BUTTONS                  Per essere visualizzare la TrackBar ha
                                             bisogno di più record per i pulsanti
    ERR_TRACKBAR_SPACE_TOO_SMALL             Le dimensioni sono troppo esigue perchè
                                             si possa creare una trackbar
    ERR_VALUE_MISMATCH                       Il valore non è compreso tra il 
                                             massimo ed il minimo consentito
    ERR_FONTHDL_IS_NOT_ACTIVE                L'handle del font non è attivo
                                             
    ---------------------------- NOTA TECNICA ---------------------------------
    
    Il funzionamento delle trackbar callback è abbastanza complesso. In pratica,
    si usano callback che richiamano ulteriori callback. 
    
    Quando viene premuto un pulsante, verrà richiamata una callback di pulsante.
    Questa callback di pulsante ha un prototipo (si veda la documentazione di
    EventManager): 
                   
    ndint64 Callback (char *StringID, ndint64 InfoField, char WndHandle);
    
    si noti che questa callback non contiene informazioni sul float da inviare.
    
    In realtà, CreateTrackBar provvede a riprogrammare il pulsante con un
    opportuno campo InfoField a 64 bit:
    
       32-BIT-1       32-BIT-0        
    
       Handle        Valori di        
       TrackBar       servizio
       (da qui si
       ottiene 
       float)               

    La callback di pulsante che viene richiamata è sempre una routine interna
    di Nanodesktop. 
    
    Tale routine interna controlla i valori di servizio per vedere se deve
    aumentare o diminuire la variabile float e di quanto, esegue l'incremento
    o decremento, e richiama l'eventuale TBCallback con prototipo:
                  
    void TB_Callback (float Value, int TB_InfoField, unsigned char WndHandle);
    
    La TB_Callback è, per così dire, una callback di secondo livello. Esiste
    anche un'eventuale callback di terzo livello, ma viene usata solo da
    HighGUI.
    
    In pratica, le callback di Intel operano per definizione con valori interi.
    
    Per tale motivo, e per mantenere la compatibilità con OpenCV, è necessario
    che ci sia una routine intermedia che converte il parametro float in
    un parametro int. 
    
    Le ndHighGUI settano come TB_Callback una routine standard del sistema,
    e poi usano TB_Infofield per fornire l'indirizzo della vera callback 
    definita dall'utente OpenCV
    --------------------------------------------------------------------------
*/

{
   char ChangeStepEnabled, TBInhibited, PervasiveCallback; 
   char StepCode;
   
   unsigned short int PB_Attribute;
   
   char ButtonsNeeded;
   char Counter, xA;
   char IHaveFound;
   
   unsigned char FntHandle;

   int TB_InfoField;
    
   if (WindowData [WndHandle].WindowIsActive)
   {
       if (WindowData [WndHandle].NrTrackBarsAllocated < __NRTRACKBARSALLOCABLE)
       {                  
              if ((TrackAttribute & CHANGE_STEP_ENABLED) == CHANGE_STEP_ENABLED)
              {
                 ChangeStepEnabled=1;
                 StepCode=INTERNAL_GetStepCodeFromTrackAttribute (TrackAttribute);
              }    
              else
              {
                 ChangeStepEnabled=0;
                 StepCode=0;         // Uno stepcode uguale a 0 implica un valore di step 10^0=1
              }
                      
              if (ChangeStepEnabled)      // Controlla che ci siano i pulsanti disponibili
                 ButtonsNeeded=5;
              else
                 ButtonsNeeded=3;
                 
              if ((TrackAttribute & TBBAR_INHIBITED) == TBBAR_INHIBITED)
                 TBInhibited=1;
              else
                 TBInhibited=0;
                 
              if ((TrackAttribute & TBBAR_PERVASIVE_CALLBACK) == TBBAR_PERVASIVE_CALLBACK)   
                 PervasiveCallback=1;
              else
                 PervasiveCallback=0;
                 
              FntHandle = (unsigned char)(TrackAttribute >> 31-8);
              
              if (FntHandle==0) FntHandle=WindowData [WndHandle].ndWS_CurrentFntHandle;
              
              if (FontHandlesArray [FntHandle].IsActive)
              {
                  if ( (WindowData [WndHandle].NrButtonsAllocated + ButtonsNeeded) < __NRBUTTONSALLOCABLE )
                  {
                     if ( RRPosY2 - RRPosY1 >= ndTBAR_ValidateLenYTrackBar (ChangeStepEnabled) )
                     {
                        if ( RRPosX2 - RRPosX1 >= ndTBAR_ValidateLenXTrackBar (ChangeStepEnabled) )
                        {
                             if ((Value<=MaxValue) && (Value>=MinValue))
                             {
                                   // Adesso tutti i controlli sono completati. Iniziamo le operazioni
                                   
                                   IHaveFound=0;
                                   
                                   for (Counter=0; (Counter<__NRTRACKBARSALLOCABLE); Counter++)
                                   {
                                       if (!(WindowData [WndHandle].TrackBar [Counter].IsActive))
                                       {
                                          xA=Counter; 
                                          IHaveFound=1;
                                          break;
                                       }
                                   } 
                                   
                                   if (!IHaveFound) return ERR_INTERNAL_ERROR;
                                   // In realtà non dovrebbe succedere in condizioni normali
                                   
                                   // Iniziamo a salvare i dati nei registri di trackbar
                                   WindowData [WndHandle].TrackBar [xA].Value    = Value;
                                   WindowData [WndHandle].TrackBar [xA].MinValue = MinValue;
                                   WindowData [WndHandle].TrackBar [xA].MaxValue = MaxValue;
                                   WindowData [WndHandle].TrackBar [xA].StepCode = StepCode;
                                   WindowData [WndHandle].TrackBar [xA].IsActive = 1;
                                   
                                   WindowData [WndHandle].TrackBar [xA].IsInhibited       = TBInhibited;
                                   WindowData [WndHandle].TrackBar [xA].PervasiveCallback = PervasiveCallback;
                                   WindowData [WndHandle].TrackBar [xA].StepButtonOn      = ChangeStepEnabled;
                                   
                                   WindowData [WndHandle].TrackBar [xA].TBar_X1           = RRPosX1;
                                   WindowData [WndHandle].TrackBar [xA].TBar_Y1           = RRPosY1;
                                   WindowData [WndHandle].TrackBar [xA].TBar_X2           = RRPosX2;
                                   WindowData [WndHandle].TrackBar [xA].TBar_Y2           = RRPosY2;
                                   
                                   WindowData [WndHandle].TrackBar [xA].TextColor         = TextColor;
                                   WindowData [WndHandle].TrackBar [xA].PBarColor         = PBarColor;
                                   WindowData [WndHandle].TrackBar [xA].PBarBkgrColor     = PBarBkgrColor;
                                   WindowData [WndHandle].TrackBar [xA].PBarBorderColor   = PBarBorderColor;
                                   
                                   WindowData [WndHandle].TrackBar [xA].TBarColor         = TBarColor;
                                   WindowData [WndHandle].TrackBar [xA].TBarBorderColor   = TBarBorderColor;
                                   
                                   WindowData [WndHandle].TrackBar [xA].FntHandle   = FntHandle;
                                   
                                   // Ora costruiamo il registro degli attributi della progress bar           
                                     
                                   PB_Attribute = TrackAttribute & 0x2F;
                                   // I primi 5 bit di TrackAttribute sono proprio coincidenti con la precisione
                                   // da usarsi nella trackbar + il bit di gestione percentuale
                                   
                                   WindowData [WndHandle].TrackBar [xA].Attribute = PB_Attribute;
                                   // Salva l'attributo della progress-bar  
                                   
                                   WindowData [WndHandle].TrackBar [xA].Register = 0;
                                   // Se si pone a 0 il parametro register tutti i componenti del 
                                   // renderer della trackbar eseguiranno la creazione dei rispettivi
                                   // elementi, e la registrazione dei rispettivi handle.
                                   
                                   TB_InfoField = (int) (TrackAttribute >> 32);
                                   // Determina l'eventuale TB_InfoField. La TB_CallBack è nei parametri di funzione
                                
                                   INTERNAL_TrackBarRenderer (xA, WndHandle, TB_CallBack, TB_InfoField, InfoCallBack, RenderNow);
                                   // Richiama il renderer della trackbar
                                
                                   WindowData [WndHandle].NrTrackBarsAllocated++;
                                   // Incrementa di uno il numero di trackbars allocato
                                
                                   return xA;
                                   // Restituisce l'handle per la trackbar alla routine chiamante
                
                             }
                             else return ERR_VALUE_MISMATCH;
                        }
                        else return ERR_TRACKBAR_SPACE_TOO_SMALL;
                     }
                     else return ERR_TRACKBAR_SPACE_TOO_SMALL;
                  }
                  else return ERR_I_NEED_MORE_BUTTONS;
               }
               else return ERR_FONTHDL_IS_NOT_ACTIVE;                                  
       }
       else return ERR_NO_TRACKBAR_RECORD_IS_AVAILABLE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;     
}


char ndTBAR_CreateLabeledTrackBar      (unsigned short int RRPosX1, unsigned short int RRPosY1,
                                        unsigned short int RRPosX2, unsigned short int RRPosY2,
                                        TypeColor TextColor, TypeColor PBarColor, TypeColor PBarBkgrColor,
                                        TypeColor PBarBorderColor, TypeColor TBarColor, 
                                        TypeColor TBarBorderColor, 
                                        char *Label,
                                        TypeColor LabelColor, 
                                        TypeColor ExoRectColor, TypeColor ExoRectBorderColor,
                                        float Value, float MinValue, float MaxValue,
                                        ndint64 TrackAttribute, int TB_CallBack, int InfoCallBack, 
                                        char WndHandle, char RenderNow)
/*
    Questa routine prima crea un rettangolo di colore ExoRectColor, ExoRectBorderColor,
    ponendovi dentro la label usando il colore LabelColor, e poi pone nella parte 
    bassa di questo rettangolo una trackbar di dimensioni minime,
    
    Sono previsti i medesimi codici di errore di CreateLabeledTrackBar.
*/
{
   unsigned short int LenRectangleX, LenRectangleY;
   unsigned short int LenTextAreaX, LenTextAreaY;
   unsigned short int PosLabelX, PosLabelY;
   char ErrRep;
   char ChangeStepEnabled;
   unsigned short int MinDimX, MinDimY;
   
   unsigned char FntHandle;
   
   unsigned short tbx1, tby1, tbx2, tby2;
   
   if (WindowData [WndHandle].WindowIsActive)
   {
     if ((TrackAttribute & CHANGE_STEP_ENABLED) == CHANGE_STEP_ENABLED)
         ChangeStepEnabled=1;
     else
         ChangeStepEnabled=0;
         
     FntHandle = (unsigned char) (TrackAttribute >> 31-8);
     if (FntHandle==0) FntHandle=WindowData [WndHandle].ndWS_CurrentFntHandle;
     
     if (FontHandlesArray [FntHandle].IsActive)
     {               
         MinDimX = ndTBAR_ValidateLenXTrackBar (ChangeStepEnabled);
         MinDimY = ndTBAR_ValidateLenYTrackBar (ChangeStepEnabled);
         
         LenRectangleX = RRPosX2 - RRPosX1;
         LenRectangleY = RRPosY2 - RRPosY1;
         
         if ((LenRectangleX-4>=MinDimX) && (LenRectangleY-16>=MinDimY))
         {
             ndWS_DrawRectangle (RRPosX1, RRPosY1, RRPosX2, RRPosY2, ExoRectColor, ExoRectBorderColor, WndHandle, NORENDER);
    
             LenTextAreaX = LenRectangleX-4;
             LenTextAreaY = LenRectangleY-MinDimY-8;
             
             PosLabelX = RRPosX1 + 2 + (LenTextAreaX - strlen (Label)*8)/2; 
             PosLabelY = RRPosY1 + 2 + (LenTextAreaY - 8)/2;
             
             ndWS_GrWriteLn (PosLabelX, PosLabelY, Label, LabelColor, ExoRectColor, WndHandle, NDKEY_FONT (FntHandle) | NDKEY_SETFNTSIZE (8) | NORENDER);
       
             tbx1 = RRPosX1+2;
             tbx2 = RRPosX2-2;
             tby1 = RRPosY1 + 2 + LenTextAreaY + 2;
             tby2 = RRPosY2-2;
          
             ErrRep=ndTBAR_CreateTrackBar (tbx1, tby1, tbx2, tby2, TextColor, PBarColor, PBarBkgrColor, PBarBorderColor, 
                                            TBarColor, TBarBorderColor, Value, MinValue, MaxValue,
                                            TrackAttribute, TB_CallBack, InfoCallBack, WndHandle, NORENDER);
             
             if (RenderNow) XWindowRender (WndHandle);
             return ErrRep;
          }
          else return ERR_TRACKBAR_SPACE_TOO_SMALL;
      }
      else return ERR_FONTHDL_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}




char ndTBAR_GetValue (float *Value, char TBHandle, char WndHandle) 
/*
    Questa funzione restituisce il valore attuale della trackbar il cui handle viene
    dato da TBHandle. Il valore viene depositato nella variabile puntata da *Value.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
         ERR_TBBAR_IS_NOT_ALLOCATED           La trackbar non è allocata
*/
{
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].TrackBar [TBHandle].IsActive)
      {
          *Value = WindowData [WndHandle].TrackBar [TBHandle].Value;
          
          return 0;
      }
      else return ERR_TRACKBAR_IS_NOT_ALLOCATED;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}



char ndTBAR_SetValue (float Value, char TBHandle, char WndHandle, char RenderNow) 
/*
    Questa funzione permette di impostare il valore attuale della trackbar il cui 
    handle viene dato da TBHandle. 
    
    La routine ha effetto solo se la trackbar non è inibita. Inoltre, se è stata
    impostata una callback di trackbar questa sarà eseguita automaticamente
    con il nuovo valore.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
         ERR_TBBAR_IS_NOT_ALLOCATED           La trackbar non è allocata
         ERR_VALUE_MISMATCH                   Il valore non è compreso tra il 
                                              massimo ed il minimo consentito
*/
{
   // Puntatore alla TBCallBack
   void (*PointerToTBCallBack) (float Value, int TB_InfoField, unsigned char WndHandle);
                                                                                            
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].TrackBar [TBHandle].IsActive)
      {
          if ( (Value>=WindowData [WndHandle].TrackBar [TBHandle].MinValue) &&
                 (Value<=WindowData [WndHandle].TrackBar [TBHandle].MaxValue) )
          {
              WindowData [WndHandle].TrackBar [TBHandle].Value = Value;
              
              if (!(WindowData [WndHandle].TrackBar [TBHandle].IsInhibited))
              {
                    INTERNAL_RenderProgressBar (TBHandle, WndHandle, RenderNow);  
   
                    // Se è presente una trackbar callback, essa deve essere 
                    // eseguita
                    
                    if (WindowData [WndHandle].TrackBar [TBHandle].TB_CallBack!=0)
                    {
                       PointerToTBCallBack = WindowData [WndHandle].TrackBar [TBHandle].TB_CallBack;
                       // Registra l'indirizzo della TB_CallBack

                       (*PointerToTBCallBack) (WindowData [WndHandle].TrackBar [TBHandle].Value, 
                            WindowData [WndHandle].TrackBar [TBHandle].TB_InfoField, WndHandle);
                       // Esegue l'eventuale TB_CallBack con la TB_InfoField
                    }
              }      
   
              return 0;
          }
          else return ERR_VALUE_MISMATCH;
      }
      else return ERR_TRACKBAR_IS_NOT_ALLOCATED;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}



char ndTBAR_InhibitTrackBar (char TBHandle, char WndHandle, char RenderNow) 
/*
    Questa funzione si occupa di inibire una trackbar 
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
         ERR_TBBAR_IS_NOT_ALLOCATED           La trackbar non è allocata
         
*/
{
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].TrackBar [TBHandle].IsActive)
      {
          if (!WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)
          {
             WindowData [WndHandle].TrackBar [TBHandle].IsInhibited=1;
             // Adesso la trackbar è in stato di inibizione. Richiamando il renderer
             // della trackbar verranno eseguite le inibizioni di tutti i pulsanti
             
             INTERNAL_TrackBarRenderer (TBHandle, WndHandle, 0, 0, 0, RenderNow);
             // E' almeno la seconda volta che si richiama il TrackBarRenderer: il
             // campo register segnalerà alla routine che i pulsanti già sono stati
             // creati. In questa situazione, non vengono registrate callbacks o 
             // InfoFields e quindi i relativi parametri vengono posti a 0 perchè
             // tanto saranno ignorati
          }
          
          return 0;    
      }
      else return ERR_TRACKBAR_IS_NOT_ALLOCATED;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}



char ndTBAR_DeInhibitTrackBar (char TBHandle, char WndHandle, char RenderNow) 
/*
    Questa funzione si occupa di deinibire una trackbar 
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
         ERR_TBBAR_IS_NOT_ALLOCATED           La trackbar non è allocata
         
*/
{
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].TrackBar [TBHandle].IsActive)
      {
          if (WindowData [WndHandle].TrackBar [TBHandle].IsInhibited)
          {
             WindowData [WndHandle].TrackBar [TBHandle].IsInhibited=0;
             // Adesso la trackbar è in stato di inibizione. Richiamando il renderer
             // della trackbar verranno eseguite le inibizioni di tutti i pulsanti
             
             INTERNAL_TrackBarRenderer (TBHandle, WndHandle, 0, 0, 0, RenderNow);
             // E' almeno la seconda volta che si richiama il TrackBarRenderer: il
             // campo register segnalerà alla routine che i pulsanti già sono stati
             // creati. In questa situazione, non vengono registrate callbacks o 
             // InfoFields e quindi i relativi parametri vengono posti a 0 perchè
             // tanto saranno ignorati
          }
          
          return 0;    
      }
      else return ERR_TRACKBAR_IS_NOT_ALLOCATED;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndTBAR_DestroyTrackBar (char TBHandle, char WndHandle, char OnlyLogical, TypeColor Color, char RenderNow) 
/*
    Questa funzione si occupa di cancellare una trackbar, disallocando i
    relativi pulsanti.  
    
    Se si pone a zero il parametro OnlyLogical, la trackbar sarà cancellata
    anche fisicamente: il parametro CoveringColor è il colore con cui
    sarà riempito lo spazio che prima era occupato dalla trackbar.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
         ERR_TBBAR_IS_NOT_ALLOCATED           La trackbar non è allocata
         
*/
{
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].TrackBar [TBHandle].IsActive)
      {
          ndCTRL_DestroyButton (WindowData [WndHandle].TrackBar [TBHandle].MinusButtonHandle, WndHandle, 1, 0, NORENDER);
          ndCTRL_DestroyButton (WindowData [WndHandle].TrackBar [TBHandle].PlusButtonHandle, WndHandle, 1, 0, NORENDER);
          ndCTRL_DestroyButton (WindowData [WndHandle].TrackBar [TBHandle].HelpButtonHandle, WndHandle, 1, 0, NORENDER);
          
          if (WindowData [WndHandle].TrackBar [TBHandle].StepButtonOn)
          {
             ndCTRL_DestroyButton (WindowData [WndHandle].TrackBar [TBHandle].ChStepPlusButtonHandle, WndHandle, 1, 0, NORENDER);
             ndCTRL_DestroyButton (WindowData [WndHandle].TrackBar [TBHandle].ChStepMinusButtonHandle, WndHandle, 1, 0, NORENDER);
          }
       
          WindowData [WndHandle].TrackBar [TBHandle].IsActive=0;
          WindowData [WndHandle].NrTrackBarsAllocated--;
          
          if (!OnlyLogical)
          {
              ndWS_DrawRectangle (WindowData [WndHandle].TrackBar [TBHandle].TBar_X1, 
                                  WindowData [WndHandle].TrackBar [TBHandle].TBar_Y1,
                                  WindowData [WndHandle].TrackBar [TBHandle].TBar_X2,
                                  WindowData [WndHandle].TrackBar [TBHandle].TBar_Y2,
                                  Color, Color, WndHandle, NORENDER);
              
              if (RenderNow) XWindowRender (WndHandle);
          } 
              
          return 0;    
      }
      else return ERR_TRACKBAR_IS_NOT_ALLOCATED;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}





















