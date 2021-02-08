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
    ROUTINES BASESCREEN (BS)
    ----------------------------------------------------------------------------
    Nanodesktop mette a disposizione delle applicazioni una serie di routine che
    consentono la creazione di elementi grafici al di fuori del contesto di
    qualsiasi finestra. Tali routines prendono il nome di BaseScreen ed iniziano
    con il prefisso ndBS_
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

#define BIT10 0x0200

void BaseScreenRender ()
{
   if (!BaseScreenIsActive) ndLP_EnableBaseScreen ();
   WindowsRender ();    
}


inline char INTERNAL_ndBS_IsPixelInBS (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY)
{            
     return ((RRPosPixelX<Windows_MaxScreenX) && (RRPosPixelY<Windows_MaxScreenY));
}


inline int ndBS_GetPixel (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY)
/* 
   Questa funzione restituisce il colore del pixel nel base screen 

   La funzione restituisce i seguenti codici di errore:
               
      ERR_PIXEL_NOT_IN_BS           Il pixel non è nel BS
*/

{
    if ( !ndInitSystemExecuted ) ndInitSystem();

    if (INTERNAL_ndBS_IsPixelInBS (RRPosPixelX, RRPosPixelY))              
       return BaseScreen [RRPosPixelY][RRPosPixelX];
    else
       return ERR_PIXEL_NOT_IN_BS;
}



inline void ndBS_PutPixel_NORENDER_FAST (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, TypeColor Color)
{
   BaseScreen [RRPosPixelY] [RRPosPixelX]=Color;               
   // Inserisce il colore nel base screen    
}







char ndBS_PutPixel (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, TypeColor Color, unsigned char RenderNow)
/*
   Questa funzione scrive un pixel nel base screen. 
   
   La routine restituisce i seguenti codici di errore:
     ERR_PIXEL_NOT_IN_BS           Il pixel non è nel service space
*/

{
  if ( !ndInitSystemExecuted ) ndInitSystem();   
     
  if (INTERNAL_ndBS_IsPixelInBS (RRPosPixelX, RRPosPixelY))
  {
      ndBS_PutPixel_NORENDER_FAST (RRPosPixelX, RRPosPixelY, Color);
      
      if (RenderNow) BaseScreenRender ();  
      return 0;
  }
  else
    return ERR_PIXEL_NOT_IN_BS;
}


/*
   ----------------------------------------------------------------------------
   ROUTINES PER IL DISEGNO DI LINEE
   ----------------------------------------------------------------------------
*/

char ndBS_DrawLine      (int RRPosX1, int RRPosY1, int RRPosX2, int RRPosY2, TypeColor Color, unsigned char RenderNow)
/*
     Questa funzione scrive una linea sullo schermo, tra i due punti le cui coordinate sono
     date da RRPosX1,RRPosY1 e RRPosX2,RRPosY2. 
     
     La funzione accetta anche coordinate negative: in tal caso viene disegnato solo ciò
     che è rappresentabile sullo schermo.
     
     Non sono previsti codici di errore per questa routine
*/

{
    float CounterR;
    float SignX, SignY;
    float ScaleX, ScaleY;
    float m1, m2;
    int KPosX, KPosY;
    char BoundCheckOk;
       
    if ( !ndInitSystemExecuted ) ndInitSystem();
       
   CounterR=0;
   
   ScaleX = (float) (RRPosX2 - RRPosX1);
   ScaleY = (float) (RRPosY2 - RRPosY1);
   
   if ( (INTERNAL_ndBS_IsPixelInBS (RRPosX1, RRPosY1)) && (INTERNAL_ndBS_IsPixelInBS (RRPosX2, RRPosY2))  )
        BoundCheckOk=1;
   else
        BoundCheckOk=0;
   
   // Se il controllo precedente ha successo, vuol dire che tutti i pixel sono nel window space della
   // finestra. Allora è possibile utilizzare la PutPixel Fast piuttosto che la normale PutPixel:
   // questo accorgimento aumenta molto la velocità di drawline
   
   if (ScaleX<0) SignX=-1;
     else SignX=1;
     
   if (ScaleY<0) SignY=-1;
     else SignY=1;
 
   if ((ScaleX!=0) && (ScaleY!=0))          // Non si è chiesta nè una linea verticale, nè una orizzontale
   {
       m1=ScaleY/ScaleX;                  // Si noti che se ScaleY!=ScaleX, uno dei due deve essere minore di 1
       m2=ScaleX/ScaleY;                  // in valore assoluto
       
       if (fabs(m1)<=1)
       {
           for (CounterR=0; CounterR<=fabs(ScaleX); CounterR++)
           {
               KPosX = RRPosX1 + SignX*CounterR;
               KPosY = RRPosY1 + m1*SignX*CounterR;
               
               if (BoundCheckOk) ndBS_PutPixel_NORENDER_FAST (KPosX, KPosY, Color);
               else ndBS_PutPixel (KPosX, KPosY, Color, NORENDER);
           }
           
           if (RenderNow) BaseScreenRender ();
           return 0;
       }
       else
       {
           for (CounterR=0; CounterR<=fabs(ScaleY); CounterR++)
           {
               KPosX = RRPosX1 + m2*SignY*CounterR;
               KPosY = RRPosY1 + SignY*CounterR;
               
               if (BoundCheckOk) ndBS_PutPixel_NORENDER_FAST (KPosX, KPosY, Color);
               else ndBS_PutPixel (KPosX, KPosY, Color, NORENDER);
               
           }
           
           if (RenderNow) BaseScreenRender ();
           return 0;
       }
           
   }
   else    // Si è chiesta una linea verticale o orizzontale
   {
       if ((ScaleX==0) && (ScaleY!=0))      // Linea verticale
       {       
               for (CounterR=0; CounterR<=fabs(ScaleY); CounterR++)
               {
                   KPosX = RRPosX1;
                   KPosY = RRPosY1 + SignY*CounterR;  
           
                   if (BoundCheckOk) ndBS_PutPixel_NORENDER_FAST (KPosX, KPosY, Color);
                   else ndBS_PutPixel (KPosX, KPosY, Color, NORENDER);    
               }
               
               if (RenderNow) BaseScreenRender ();
               return 0;
       }
       
       if ((ScaleX!=0) && (ScaleY==0))      // Linea orizzontale
       {       
               for (CounterR=0; CounterR<=fabs(ScaleX); CounterR++)
               {
                   KPosX = RRPosX1 + SignX*CounterR;
                   KPosY = RRPosY1;  
           
                   if (BoundCheckOk) ndBS_PutPixel_NORENDER_FAST (KPosX, KPosY, Color);
                   else ndBS_PutPixel (KPosX, KPosY, Color, NORENDER);
           
               }
               
               if (RenderNow) BaseScreenRender ();
               return 0;
       }    
       
       if ((ScaleX==0) && (ScaleY==0))     // La linea si riduce ad un punto
       {
          if (BoundCheckOk) ndBS_PutPixel_NORENDER_FAST (RRPosX1, RRPosY1, Color);
            else ndBS_PutPixel (RRPosX1, RRPosY1, Color, NORENDER);
          
          if (RenderNow) BaseScreenRender ();
          return 0;
       }
   }
}


char ndBS_DrawPoly (TypeColor Color, char RenderNow, int NrPixels, ...)
/*
     Questa routine consente di disegnare una linea spezzata o un poligono nel
     BaseScreen.
     
     Il funzionamento di questa routine è leggermente differente rispetto alle altre
     fornite dalla libreria. 
     
     L'utente deve indicare l'handle della finestra, il colore, l'eventuale parametro
     RenderNow, ed il numero di pixels che comporranno la spezzata. 
     
     Subito dopo, devono essere inserite le coordinate nel formato:
            X, Y, X, Y, X, Y .....
            
     Si faccia attenzione al fatto che Nanodesktop non ha alcun modo per verificare quanti
     sono effettivamente le coordinate passate: l'unica indicazione gli viene data dal 
     parametro NrPixels. E' dunque fondamentale che l'utente setti questo parametro al
     valore corretto: se si immette il valore errato, gli effetti saranno imprevedibili.
     
     Questa routine non restituisce alcun codice di errore.
*/
{
    va_list args;    // Puntatore agli argomenti   
    int Counter;
  
    int CoordX, CoordY;
    int FirstPixelIsSaved;
  
    int FirstPixelX, FirstPixelY;
    int SecondPixelX, SecondPixelY;
  
    if ( !ndInitSystemExecuted ) ndInitSystem();
    
    va_start(args,NrPixels);   // Inizializza il puntatore argomenti all'ultimo parametro fisso (nr. pixel)
    
    FirstPixelIsSaved=0;
    
    for (Counter=0; Counter<NrPixels; Counter++)
    {
     // Iniziamo a prendere le coordinate per i vari pixels
     
     CoordX = va_arg(args,int);           // Preleva il primo int dallo stack
     CoordY = va_arg(args,int);           // Preleva il secondo int dallo stack
     
     if (!FirstPixelIsSaved)
     {
        FirstPixelX = CoordX;
        FirstPixelY = CoordY;
        
        FirstPixelIsSaved=1;              // Segnala che il primo pixel è stato memorizzato
     
        if (NrPixels==1) return ndBS_PutPixel (CoordX, CoordY, Color, RenderNow);                
        // Caso speciale: c'è un solo pixel nella catena
     }
     else
     {
        SecondPixelX = CoordX;
        SecondPixelY = CoordY;
        
        ndBS_DrawLine (FirstPixelX, FirstPixelY, SecondPixelX, SecondPixelY, Color, NORENDER);
        // Disegna la linea
        
        FirstPixelX = SecondPixelX;
        FirstPixelY = SecondPixelY;
        // Scambio di coordinate: il penultimo punto della spezzata diventa l'ultimo  
     }
    }
    
    va_end(args);     // Rilascia il puntatore argomenti
    
    if (RenderNow) BaseScreenRender ();
    return 0;  

} 


/*
  ----------------------------------------------------------------------------------------------
  ROUTINE BS CHE SI OCCUPANO DELLA CREAZIONE DI ELEMENTI GRAFICI SULLO SCHERMO: ROUTINE DI FILL
  ----------------------------------------------------------------------------------------------
*/




char INTERNAL_ndBS_FillArea (int PosX, int PosY, TypeColor Color, TypeColor BorderColor)
/*
    Questa routine è il motore di FillArea. E' ottimizzata per eseguire il minimo numero di
    controlli possibili, in modo da ridurre l'overhead di gestione, in modo che sia possibile
    utilizzarla in un algoritmo ricorsivo.
*/
{
    int SPosX, SPosY;
    TypeColor ScrColor;
    
    ndBS_PutPixel_NORENDER_FAST (PosX, PosY, Color);
        
    SPosX = PosX;
    SPosY = PosY - 1;
    
    if ( (SPosX>=0) && (SPosX<Windows_MaxScreenX) && (SPosY>=0) && (SPosY<Windows_MaxScreenY) )
    {
        ScrColor = BaseScreen [SPosY][SPosX];
        
        if ( (ScrColor!=BorderColor) && (ScrColor!=Color) )
           INTERNAL_ndBS_FillArea (SPosX, SPosY, Color, BorderColor); 
    }
    
    SPosX = PosX - 1;
    SPosY = PosY;
    
    if ( (SPosX>=0) && (SPosX<Windows_MaxScreenX) && (SPosY>=0) && (SPosY<Windows_MaxScreenY) )
    {
        ScrColor = BaseScreen [SPosY][SPosX];
         
        if ( (ScrColor!=BorderColor) && (ScrColor!=Color) )
           INTERNAL_ndBS_FillArea (SPosX, SPosY, Color, BorderColor); 
    }
    
    SPosX = PosX + 1;
    SPosY = PosY;
    
    if ( (SPosX>=0) && (SPosX<Windows_MaxScreenX) && (SPosY>=0) && (SPosY<Windows_MaxScreenY) )
    {
        ScrColor = BaseScreen [SPosY][SPosX];
         
        if ( (ScrColor!=BorderColor) && (ScrColor!=Color) )
           INTERNAL_ndBS_FillArea (SPosX, SPosY, Color, BorderColor); 
    }
    
    
    SPosX = PosX;
    SPosY = PosY + 1;
    
    if ( (SPosX>=0) && (SPosX<Windows_MaxScreenX) && (SPosY>=0) && (SPosY<Windows_MaxScreenY) )
    {
        ScrColor = BaseScreen [SPosY][SPosX];
        
        if ( (ScrColor!=BorderColor) && (ScrColor!=Color) )
           INTERNAL_ndBS_FillArea (SPosX, SPosY, Color, BorderColor); 
    }
    
    return 0;
}

char ndBS_FillArea (int PosX, int PosY, TypeColor Color, TypeColor BorderColor, char RenderNow)
/*
    Questa routine provvede a riempire una certa zona dello schermo con il colore Color. 
    L'algoritmo di filler si ferma solo quando viene trovato un colore pari a BorderColor.
    
    Sono previsti i seguenti codici di errore:
          ERR_PIXEL_NOT_IN_BS         Il pixel è fuori dalla finestra
*/
{
    if ( !ndInitSystemExecuted ) ndInitSystem();
          
    if ( (PosX>=0) && (PosX<Windows_MaxScreenX) && (PosY>=0) && (PosY<Windows_MaxScreenY) )
    {
      INTERNAL_ndBS_FillArea (PosX, PosY, Color, BorderColor);
      if (RenderNow) BaseScreenRender ();
      
      return 0;
    }
    else
    return ERR_PIXEL_NOT_IN_BS;
}

/*
  ----------------------------------------------------------------------------------------------
  ROUTINE BS CHE SI OCCUPANO DELLA CREAZIONE DI ELEMENTI GRAFICI SULLO SCHERMO: RETTANGOLI
  ----------------------------------------------------------------------------------------------
*/


char ndBS_DrawSpRectangle (unsigned short int RRPosX1, unsigned short int RRPosY1, 
                           unsigned short int RRPosX2, unsigned short int RRPosY2, 
                           TypeColor Color, TypeColor BorderColor, ndint64 CodeStyle,
                           unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo nel base screen.
     
     Il significato dei parametri è chiaro, salvo che per il parametro CodeStyle. 
     
     Questo parametro a 64 bit contiene delle informazioni che consentono di personalizzare
     il comportamento della routine. I bit di questo parametro sono mappati nel seguente
     modo:
          
     Bit:
                        
       53-50          Consente di ottenere lo spessore del bordo dell'elemento grafico:
                      è consentito uno spessore compreso tra 1 e 15 pixel. L'utente
                      può ottenere il corretto codice tramite la funzione 
                      NDKEY_BORDER_SIZE ()
                      
       1              Il bit 1 segnala se l'elemento grafico deve avere gli spigoli
                      arrotondati. L'utente può ottenere un elemento grafico con
                      gli spigoli arrotondati, usando la costante ND_ROUNDED
                      
       0              Il bit 0 segnala se l'elemento grafico deve essere vuoto o pieno.
                      L'utente può ottenere un elemento grafico "vuoto", ossia in cui
                      viene disegnato solo il bordo, utilizzando la costante ND_VOID.
                      Quando l'elemento grafico che si vuole utilizzare è di tipo
                      VOID, il parametro Color viene ignorato, e viene usato solo
                      il parametro BorderColor per identificare il colore del bordo
     
     La routine ignora completamente eventuali codici di rotazione.
     
     Sono previsti due codici di errore:
          ERR_RECTANGLE_OUT_OF_BS             Il rettangolo andrebbe fuori dai limiti del BS
*/
    
{      
   if ( !ndInitSystemExecuted ) ndInitSystem();
    
   int IntX, IntY;
   unsigned short int KDiffX, KDiffY;
    
   char Offset   [10] = {2, 1, 0, 0, 0, 0, 0, 0, 0};      
   char NrPixels [10] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    
   unsigned short int PosOffset, NrPixelsChoose;
    
   ndint64 Temp1;
    
   char Border, Void, Rounded;
    
   if (RRPosX1>RRPosX2) UShort_Swap (&RRPosX1, &RRPosX2);
   if (RRPosY1>RRPosY2) UShort_Swap (&RRPosY1, &RRPosY2);

   KDiffX=RRPosX2-RRPosX1;
   KDiffY=RRPosY2-RRPosY1;
             
   // Determiniamo lo spessore del bordo
  
   Temp1 = CodeStyle;
   Temp1 = Temp1 >> (63-1-9-4);     // Qui ora c'è il codice per lo spessore del bordo
  
   Temp1 = Temp1 & 0x0F;            // Isolato lo spessore del bordo
   Border = Temp1;

   if (Border==0) Border=1;    // La mancata indicazione del bordo indica bordo 1. 
  
   // Verifichiamo se è stata richiesta l'opzione void

   if ((CodeStyle & ND_VOID) == ND_VOID)
     Void=1;
   else
     Void=0; 

   // Verifichiamo se è stata richiesta l'opzione rounded
  
   if ((CodeStyle & ND_ROUNDED) == ND_ROUNDED)
     Rounded=1;
   else
     Rounded=0; 
               
               
   if ( (KDiffX<Windows_MaxScreenX) && (KDiffY<Windows_MaxScreenY) )
   {
        for (IntY=0; IntY<=KDiffY; IntY++)
        {
            if ( (IntY<Border) || ((KDiffY-IntY)<Border) )   // Siamo sulle prime o sulle ultime righe      
            {
                 // Determiniamo l'offset
                 
                 if (!Rounded) 
                 {
                    PosOffset=0;
                 }
                 else           // E' stato richiesto un bordo arrotondato
                 {
                    if (IntY<8)
                    {
                        PosOffset=Offset [IntY];
                    }
                    else
                    {
                        if ((KDiffY-IntY)<8)
                        {
                            PosOffset=Offset [KDiffY-IntY];
                        }
                        else
                            PosOffset=0;
                    }
                 }
                 
                 
                 // Disegniamo le righe del bordo superiore o inferiore
                 
                 for (IntX=PosOffset; IntX<=KDiffX-PosOffset; IntX++)
                 {
                     ndBS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, BorderColor);
                 }
            }
            else  // Non siamo tra le prime o le ultime righe
            {
                 // Determiniamo offset e numero pixel da scrivere lateralmente
                 
                 if (!Rounded) 
                 {
                    PosOffset=0;
                    NrPixelsChoose=Border;
                 }
                 else
                 {
                    if (IntY<8)
                    {
                        PosOffset=Offset [IntY];
                        NrPixelsChoose = Border + NrPixels [IntY] - 1;
                    }
                    else
                    {
                        if ((KDiffY-IntY)<8)
                        {
                            PosOffset=Offset [KDiffY-IntY];
                            NrPixelsChoose = Border + NrPixels [KDiffY-IntY] - 1;
                        }
                        else
                        {
                            PosOffset=0;
                            NrPixelsChoose = Border;
                        }
                    } 
                 }
                 
                 for (IntX=0; IntX<NrPixelsChoose; IntX++)
                   ndBS_PutPixel_NORENDER_FAST (RRPosX1+PosOffset+IntX, RRPosY1+IntY, BorderColor);
                 
                 for (IntX=0; IntX<NrPixelsChoose; IntX++)
                   ndBS_PutPixel_NORENDER_FAST (RRPosX2-PosOffset-IntX, RRPosY1+IntY, BorderColor);
                 
                 // Ora se necessario disegniamo l'interno: l'ultimo pixel che è stato visualizzato
                 // dalla riga precedente ha coordinate:
                 //
                 // RRPosX1+PosOffset+NrPixelsChoose-1 
                 // 
                 // mentre il primo pixel che è stato visualizzato dalla seconda delle righe precedenti
                 // ha coordinate
                 //
                 // RRPosX2-PosOffset-(NrPixelsChoose-1) = RRPosX2-PosOffset-NrPixelChoose+1
                 //
                 // Quindi l'interno del rettangolo può essere visualizzato scrivendo il pixels
                 // da:
                 //
                 // RRPosX1+PosOffset+NrPixelsChoose a RRPosX2-PosOffset-NrPixelChoose
                 
                 if (!Void)
                 {
                    for (IntX=RRPosX1+PosOffset+NrPixelsChoose; IntX<=RRPosX2-PosOffset-NrPixelsChoose; IntX++)
                        ndBS_PutPixel_NORENDER_FAST (IntX, RRPosY1+IntY, Color);
                 }
                 
            }
        }
            
            
        // Ora, eseguiamo il rendering della finestra in modo che possa apparire il
        // rettangolo

        if (RenderNow) BaseScreenRender();
        return 0;
   }
   else
     return ERR_RECTANGLE_OUT_OF_BS;
            
}


char ndBS_DrawRectangle (unsigned short int RRPosX1, unsigned short int RRPosY1, 
                         unsigned short int RRPosX2, unsigned short int RRPosY2, 
                         TypeColor Color, TypeColor BorderColor, unsigned char RenderNow)
/*
   Funzione analoga a SpDrawRectangle, ma disegna un semplice rettangolo pieno
   con bordo di spessore 1.
   
   Sono previsti i seguenti codici di errore:
      ERR_RECTANGLE_OUT_OF_BS             Il rettangolo andrebbe fuori dai limiti del BS 
*/

{
   return  ndBS_DrawSpRectangle (RRPosX1, RRPosY1, RRPosX2, RRPosY2, Color, BorderColor, 0, RenderNow);
}



char ndBS_DrawRtRectangle (int CenterRectX, int CenterRectY, int UserDimX, int UserDimY,
                            TypeColor Color, TypeColor BorderColor, ndint64 CodeStyle,
                            unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo nel base screen, ma, a differenza delle precedenti, 
     è in grado di ruotare il rettangolo intorno al proprio centro di un certo numero di gradi. 
     
     L'utente non fornisce le coordinate dei vertici, come nel caso delle precedenti routine
     DrawRectangle, ma indica semplicemente le coordinate del centro del rettangolo e le
     dimensioni. 
     
     Si faccia attenzione al fatto che DrawRtRectangle esegue un controllo per assicurarsi
     che le dimensioni non siano maggiori delle dimensioni del base screen e che siano
     positive, ma non esegue alcun controllo per assicurarsi che tutto il rettangolo, dopo
     la rotazione, sia ancora contenuto nel base screen. Se una parte del
     rettangolo andasse fuori dalle coordinate del base screen, essa sarà semplicemente
     non visualizzata. 
     
     Veniamo al significato del parametro CodeStyle a 64 bit:
     
     Questo parametro a 64 bit contiene delle informazioni che consentono di personalizzare
     il comportamento della routine. I bit di questo parametro sono mappati nel seguente
     modo:
          
     Bit:
       63             0 se la rotazione ha segno positivo, 1 se ha segno negativo
       62-54          9 bit che contengono un codice di rotazione in gradi (0-512)

                      Usando questi bit si può ottenere una rotazione del rettangolo
                      rispetto al proprio centro con una precisione di 0,35 gradi, da +180
                      a -180 gradi. L'utente può ottenere il corretto codice tramite
                      la funzione NDKEY_ROTATE () che va sottoposta ad or con gli altri
                      parametri per ottenere il corretto codice CodeStyle a 64 bit
                     
       53-50          Consente di ottenere lo spessore del bordo dell'elemento grafico:
                      è consentito uno spessore compreso tra 1 e 15 pixel. L'utente
                      può ottenere il corretto codice tramite la funzione 
                      NDKEY_BORDER_SIZE ()
                      
       1              Il bit 1 segnala se l'elemento grafico deve avere gli spigoli
                      arrotondati. L'utente può ottenere un elemento grafico con
                      gli spigoli arrotondati, usando la costante ND_ROUNDED
                      
       0              Il bit 0 segnala se l'elemento grafico deve essere vuoto o pieno.
                      L'utente può ottenere un elemento grafico "vuoto", ossia in cui
                      viene disegnato solo il bordo, utilizzando la costante ND_VOID.
                      Quando l'elemento grafico che si vuole utilizzare è di tipo
                      VOID, il parametro Color viene ignorato, e viene usato solo
                      il parametro BorderColor per identificare il colore del bordo
     
     Sono previsti due codici di errore:
          ERR_RECTANGLE_OUT_OF_BS             Le dimensioni del rettangolo sono troppo grandi
          ERR_WRONG_PARAMETERS                Una delle dimensioni è negativa
*/
    
{      
    if ( !ndInitSystemExecuted ) ndInitSystem();

    float FlCounterX, FlCounterY;

    char Offset   [10] = {2, 1, 0, 0, 0, 0, 0, 0, 0};      
    char NrPixels [10] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    unsigned short int PosOffset, NrPixelsChoose;
    
    ndint64 Temp1;
    char Border, Void, Rounded;
    
    unsigned short int RotateCode;
    
    char SignRotate;
    float RotateDegree, RotateRadiant; 
    
    float HalfDimX, HalfDimY; 
    float UpCornerPixelX, UpCornerPixelY;
    float CenterPixelX, CenterPixelY; 
    
    int TargetX, TargetY;
            
   // Determiniamo il codice di rotazione

   Temp1 = CodeStyle;
   Temp1 = Temp1 >> (63-1-9);     // Qui ora c'è il codice di rotazione
  
   if ( (Temp1 & BIT10) == BIT10) SignRotate=-1;
     else SignRotate=1;
        
   Temp1 = Temp1 & 0x01FF;        // Isolato il codice di rotazione
   RotateCode = (unsigned short) Temp1;            
  
   if (RotateCode!=255) 
     RotateDegree = SignRotate * RotateCode * (180.0/511.0);                  
   else            
   {           
     // RotateCode di 255 indica sempre rotazione di 90 gradi
     // Questo tipo di rotazione non è gestita correttamente, a causa degli errori
     // di approssimazione dei float. Qui è previsto un workaround
     
     Int_Swap (&UserDimX, &UserDimY);
     RotateDegree = 0;
   }
  
   RotateRadiant = RotateDegree * 6.28/360.0;
           
   // Determiniamo lo spessore del bordo
  
   Temp1 = CodeStyle;
   Temp1 = Temp1 >> (63-1-9-4);     // Qui ora c'è il codice per lo spessore del bordo
  
   Temp1 = Temp1 & 0x0F;            // Isolato lo spessore del bordo
   Border = Temp1;

   if (Border==0) Border=1;         // La mancata indicazione del bordo indica bordo 1. 
  
   // Verifichiamo se è stata richiesta l'opzione void

   if ((CodeStyle & ND_VOID) == ND_VOID)
     Void=1;
   else
     Void=0; 

   // Verifichiamo se è stata richiesta l'opzione rounded
  
   if ((CodeStyle & ND_ROUNDED) == ND_ROUNDED)
     Rounded=1;
   else
     Rounded=0; 
               
   // Iniziamo il render del rettangolo
   
   HalfDimX = UserDimX/2;
   HalfDimY = UserDimY/2;
   
   if ( (UserDimX>0) && (UserDimY>0) )
   {
       if ( (UserDimX<Windows_MaxScreenX) && (UserDimY<Windows_MaxScreenY) )
       {
            for (FlCounterY=0; FlCounterY<=UserDimY; FlCounterY=FlCounterY+0.5)
            {
                if ( (FlCounterY<Border) || ((UserDimY-FlCounterY)<Border) )   // Siamo sulle prime o sulle ultime righe      
                {
                     // Determiniamo l'offset
                     
                     if (!Rounded) PosOffset=0;
                     else           // E' stato richiesto un bordo arrotondato
                     {
                        if (FlCounterY<8) PosOffset=Offset [(int)FlCounterY];
                        else
                        {
                            if ((UserDimY-FlCounterY)<8) PosOffset=Offset [(int)(UserDimY-FlCounterY)];
                            else PosOffset=0;
                        }
                     }
                     
                     
                     // Disegniamo le righe del bordo superiore o inferiore
                     
                     for (FlCounterX=PosOffset; FlCounterX<=UserDimX-PosOffset; FlCounterX=FlCounterX+0.5)
                     {
                         UpCornerPixelX = FlCounterX;         // Coordinate rispetto al punto in alto a sinistra
                         UpCornerPixelY = FlCounterY;
                         
                         CenterPixelX = UpCornerPixelX-HalfDimX;     // Coordinate rispetto al centro del rettangolo
                         CenterPixelY = UpCornerPixelY-HalfDimY;
                         
                         INTERNAL_Rotate (CenterRectX, CenterRectY, CenterPixelX, CenterPixelY, RotateRadiant, &TargetX, &TargetY);
                         
                         ndBS_PutPixel (TargetX, TargetY, BorderColor, NORENDER);
                     }
                }
                else  // Non siamo tra le prime o le ultime righe
                {    
                     // Determiniamo offset e numero pixel da scrivere lateralmente
                     
                     if (!Rounded) 
                     {
                        PosOffset=0;
                        NrPixelsChoose=Border;
                     }
                     else
                     {
                        if (FlCounterY<8)
                        {
                            PosOffset=Offset [(int)(FlCounterY)];
                            NrPixelsChoose = Border + NrPixels [(int)(FlCounterY)] - 1;
                        }
                        else
                        {
                            if ((UserDimY-FlCounterY)<8)
                            {
                                PosOffset=Offset [(int)(UserDimY-FlCounterY)];
                                NrPixelsChoose = Border + NrPixels [(int)(UserDimY-FlCounterY)] - 1;
                            }
                            else
                            {
                                PosOffset=0;
                                NrPixelsChoose = Border;
                            }
                        } 
                     }
                     
                     for (FlCounterX=0; FlCounterX<NrPixelsChoose; FlCounterX=FlCounterX+0.5)
                     {
                       UpCornerPixelX = PosOffset+FlCounterX;
                       UpCornerPixelY = FlCounterY;
                       
                       CenterPixelX = UpCornerPixelX-HalfDimX;     // Coordinate rispetto al centro del rettangolo
                       CenterPixelY = UpCornerPixelY-HalfDimY;
                         
                       INTERNAL_Rotate (CenterRectX, CenterRectY, CenterPixelX, CenterPixelY, RotateRadiant, &TargetX, &TargetY);
                             
                       ndBS_PutPixel (TargetX, TargetY, BorderColor, NORENDER);
                     }
                     
                     for (FlCounterX=0; FlCounterX<NrPixelsChoose; FlCounterX=FlCounterX+0.5)
                     {
                       UpCornerPixelX = UserDimX-PosOffset-FlCounterX;
                       UpCornerPixelY = FlCounterY;
                       
                       CenterPixelX = UpCornerPixelX-HalfDimX;     // Coordinate rispetto al centro del rettangolo
                       CenterPixelY = UpCornerPixelY-HalfDimY;
                         
                       INTERNAL_Rotate (CenterRectX, CenterRectY, CenterPixelX, CenterPixelY, RotateRadiant, &TargetX, &TargetY);
                         
                       ndBS_PutPixel (TargetX, TargetY, BorderColor, NORENDER);
                     }
                     
                     if (!Void)
                     {
                        for (FlCounterX=PosOffset+NrPixelsChoose; FlCounterX<=UserDimX-PosOffset-NrPixelsChoose; FlCounterX=FlCounterX+0.5)
                        {
                            UpCornerPixelX = FlCounterX;
                            UpCornerPixelY = FlCounterY;
                            
                            CenterPixelX = UpCornerPixelX-HalfDimX;     // Coordinate rispetto al centro del rettangolo
                            CenterPixelY = UpCornerPixelY-HalfDimY;
                       
                            INTERNAL_Rotate (CenterRectX, CenterRectY, CenterPixelX, CenterPixelY, RotateRadiant, &TargetX, &TargetY);
                       
                            ndBS_PutPixel (TargetX, TargetY, Color, NORENDER);
                        }
                     
                     }
                                      
                }
            }
                    
            // Ora, eseguiamo il rendering della finestra in modo che possa apparire il
            // rettangolo

            if (RenderNow) BaseScreenRender();
            return 0;
       }
       else
         return ERR_RECTANGLE_OUT_OF_BS;
    }
    else
      return ERR_WRONG_PARAMETERS;
            
}


/*
  -----------------------------------------------------------------------------------------------
  ROUTINE BS CHE SI OCCUPANO DELLA CREAZIONE DI ELEMENTI GRAFICI SULLO SCHERMO: CERCHI ED ELLISSI
  -----------------------------------------------------------------------------------------------
*/

char INTERNAL_ndBS_EllipseEngine (unsigned short int CenterX, unsigned short int CenterY, 
                                  float RayA, float RayB, float RayX, float DegreeA, float DegreeB, float Rot,
                                  TypeColor Colore, char DrawRay, char RenderNow)
{
  float t;
  int X, Y;
  
  int X1, Y1, X2, Y2;
  
  int PrevX, PrevY;
  char First;
  
  float DegreeA_Radiant;
  float DegreeB_Radiant;
  float Rot_Radiant;
  
  float Swap;
  float Increase;
  char ErrRep;
  
  if (DegreeA>DegreeB)
  {
     Swap=DegreeA;
     DegreeA=DegreeB;
     DegreeB=Swap;
  }
  
  DegreeA_Radiant = (6.28/360.0)*DegreeA;
  DegreeB_Radiant = (6.28/360.0)*DegreeB;
  Rot_Radiant = (6.28/360.0)*Rot;
  
  First=1;
  Increase=0.01;
  
  for (t=DegreeA_Radiant; t<=DegreeB_Radiant; t=t+Increase)
  {  
    INTERNAL_Rotate (CenterX, CenterY, RayA*ndHAL_XFPU_cosf (t), - RayB*ndHAL_XFPU_sinf (t), Rot_Radiant, &X, &Y);
    
    if (First)
    {
       ErrRep=ndBS_PutPixel (X, Y, Colore, NORENDER);  
    
       if (!ErrRep)
       {
          PrevX = X;
          PrevY = Y;
          First = 0;
       }
    }
    else
    {
       ndBS_DrawLine (X, Y, PrevX, PrevY, Colore, NORENDER);  
  
       PrevX = X;
       PrevY = Y;
    }
    
    if (RayX!=0)                   // E' stata richiesta la produzione di uno spessore
    {
        if ( (RayX<RayA) && (RayX<RayB) )
        {
            INTERNAL_Rotate (CenterX, CenterY, RayA*ndHAL_XFPU_cosf (t), - RayB*ndHAL_XFPU_sinf (t), Rot_Radiant, &X1, &Y1);
            INTERNAL_Rotate (CenterX, CenterY, (RayA-RayX)*ndHAL_XFPU_cosf (t), - (RayB-RayX)*ndHAL_XFPU_sinf (t), Rot_Radiant, &X2, &Y2);
        
            ndBS_DrawLine (X1,   Y1, X2,   Y2, Colore, NORENDER);
            ndBS_DrawLine (X1-1, Y1, X2-1, Y2, Colore, NORENDER);
            ndBS_DrawLine (X1+1, Y1, X2+1, Y2, Colore, NORENDER);
            ndBS_DrawLine (X1, Y1-1, X2, Y2-1, Colore, NORENDER);
            ndBS_DrawLine (X1, Y1+1, X2, Y2+1, Colore, NORENDER);
             
        }
    }
  }
  
  if ((DrawRay) && (RayX<RayA) && (RayX<RayB) && (fabs (DegreeA-DegreeB)<360))
  {                   
       INTERNAL_Rotate (CenterX, CenterY, (RayA-RayX)*ndHAL_XFPU_cosf (DegreeA_Radiant), - (RayB-RayX)*ndHAL_XFPU_sinf (DegreeA_Radiant), Rot_Radiant, &X, &Y);     
       ndBS_DrawLine (CenterX, CenterY, X, Y, Colore, NORENDER);
       
       INTERNAL_Rotate (CenterX, CenterY, (RayA-RayX)*ndHAL_XFPU_cosf (DegreeB_Radiant), - (RayB-RayX)*ndHAL_XFPU_sinf (DegreeB_Radiant), Rot_Radiant, &X, &Y);       
       ndBS_DrawLine (CenterX, CenterY, X, Y, Colore, NORENDER);
  }
             
  if (RenderNow) BaseScreenRender ();
   
  return 0;
}



char ndBS_DrawArcOfEllipse (unsigned short int CenterX, unsigned short int CenterY, 
                            float RayA, float RayB, float DegreeA, float DegreeB, 
                            TypeColor Color, TypeColor ColorBorder, 
                            ndint64 CodeStyle, char RenderNow)
/*
     Questa routine permette di disegnare un arco di ellisse dentro il base screen. 
     
     Il significato dei parametri è chiaro, salvo che per il parametro CodeStyle. 
     
     Questo parametro a 64 bit contiene delle informazioni che consentono di personalizzare
     il comportamento della routine. I bit di questo parametro sono mappati nel seguente
     modo:
          
     Bit
       63             0 se la rotazione ha segno positivo, 1 se ha segno negativo
       62-54          9 bit che contengono un codice di rotazione in gradi (0-512)

                      Usando questi bit si può ottenere una rotazione dell'ellisse
                      rispetto al proprio centro con una precisione di 0,35 gradi, da +180
                      a -180 gradi. L'utente può ottenere il corretto codice tramite
                      la funzione NDKEY_ROTATE () che va sottoposta ad or con gli altri
                      parametri per ottenere il corretto codice CodeStyle a 64 bit
                      
       53-50          Consente di ottenere lo spessore del bordo dell'elemento grafico:
                      è consentito uno spessore compreso tra 1 e 15 pixel. L'utente
                      può ottenere il corretto codice tramite la funzione 
                      NDKEY_BORDER_SIZE ()
                      
       0              Il bit 0 segnala se l'elemento grafico deve essere vuoto o pieno.
                      L'utente può ottenere un elemento grafico "vuoto", ossia in cui
                      viene disegnato solo il bordo, utilizzando la costante ND_VOID.
                      Quando l'elemento grafico che si vuole utilizzare è di tipo
                      VOID, il colore Color viene ignorato.
                      
   Non sono previsti codici di errore per questa routine.
*/

{
    unsigned short int RotateCode;
    unsigned short int BorderCode;       
    char SignRotate; 
   
    float RotateDegree, RotateRadiant;  
      
    ndint64 Temp1;  
    
    char ErrRep;
    char Void;
   
    float FillPoint_X, FillPoint_Y;
    int FillPoint__X, FillPoint__Y;
   
    float InterDegree, InterRadiant;
          
    if ( !ndInitSystemExecuted ) ndInitSystem();
          
    // Determiniamo il codice di rotazione
    
    Temp1 = CodeStyle;
    Temp1 = Temp1 >> (63-1-9);     // Qui ora c'è il codice di rotazione
    
    if ( (Temp1 & BIT10) == BIT10) SignRotate=-1;
     else SignRotate=1;
        
    Temp1 = Temp1 & 0x01FF;        // Isolato il codice di rotazione
    RotateCode = (unsigned short) Temp1;            
    
    RotateDegree = SignRotate * RotateCode * (180.0/511.0);
    RotateRadiant = RotateDegree * 6.28/360.0;
    
    // Determiniamo lo spessore del bordo
    
    Temp1 = CodeStyle;
    Temp1 = Temp1 >> (63-1-9-4);     // Qui ora c'è il codice per lo spessore del bordo
    
    Temp1 = Temp1 & 0x0F;            // Isolato lo spessore del bordo
    BorderCode = Temp1;
    
    if (BorderCode==1) BorderCode=0;
    // Un bordercode pari a 0 passato come RayX a INTERNAL_ndWS_EllipseEngine impedirà che
    // venga disegnato un bordo molto grande....
    
    // Verifichiamo se è stata richiesta l'opzione void
    
    if ((CodeStyle & ND_VOID) == ND_VOID)
     Void=1;
    else
     Void=0; 
     
    if (fabs (DegreeB-DegreeA)>=360)      // Ha chiesto di visualizzare una circonferenza
    {   
      ErrRep = INTERNAL_ndBS_EllipseEngine (CenterX, CenterY, RayA, RayB, BorderCode, DegreeA, DegreeB, RotateDegree, ColorBorder, 0, NORENDER);
      if (ErrRep) return ErrRep;
      
      if (!Void) ErrRep = ndBS_FillArea (CenterX, CenterY, Color, ColorBorder, NORENDER);
      if (ErrRep) return ErrRep;  
    }
    else    // Ha chiesto di visualizzare un arco di circonferenza
    {   
      ErrRep = INTERNAL_ndBS_EllipseEngine (CenterX, CenterY, RayA, RayB, BorderCode, DegreeA, DegreeB, RotateDegree, ColorBorder, 1, NORENDER);
      if (ErrRep) return ErrRep; 
      
      // Qui è necessario calcolare il punto di filler
      
      if ((!Void) && (RayA-BorderCode>3) && (RayB-BorderCode>3))
      {
          InterDegree = (DegreeA + DegreeB)/2;
          InterRadiant = InterDegree*6.28/360.0;
          
          FillPoint_X =   (RayA-BorderCode-2)*ndHAL_XFPU_cosf (InterRadiant);
          FillPoint_Y = - (RayB-BorderCode-2)*ndHAL_XFPU_sinf (InterRadiant);
    
          INTERNAL_Rotate (CenterX, CenterY, FillPoint_X, FillPoint_Y, RotateRadiant, &FillPoint__X, &FillPoint__Y);     
    
          ErrRep = ndBS_FillArea (FillPoint__X, FillPoint__Y, Color, ColorBorder, NORENDER);
          if (ErrRep) return ErrRep; 
      }
    } 
    
    if (RenderNow) BaseScreenRender ();
    return 0;

}

char ndBS_DrawEllipse      (unsigned short int CenterX, unsigned short int CenterY, 
                            float RayA, float RayB, TypeColor Color, TypeColor ColorBorder, 
                            ndint64 CodeStyle, char RenderNow)
/*
     Questa routine permette di disegnare un'ellisse dentro il base screen.
     
     Il significato del parametro StyleCode è lo stesso usato nelle altre routine
     di disegno.
                      
     Non sono previsti codici di errore per questa routine.
*/

{
    return ndBS_DrawArcOfEllipse (CenterX, CenterY, RayA, RayB, 0, 360, 
           Color, ColorBorder, CodeStyle, RenderNow);
}


char ndBS_DrawArcOfCircle  (unsigned short int CenterX, unsigned short int CenterY, 
                            float Ray, float DegreeA, float DegreeB,
                            TypeColor Color, TypeColor ColorBorder, 
                            ndint64 CodeStyle, char RenderNow)
/*
     Questa routine permette di disegnare un arco di cerchio dentro il base
     screen.
     
     Il significato del parametro StyleCode è lo stesso usato nelle altre routine
     di disegno.
                      
     Non sono previsti codici di errore per questa routine
*/

{
    return ndBS_DrawArcOfEllipse (CenterX, CenterY, Ray, Ray, DegreeA, DegreeB, 
           Color, ColorBorder, CodeStyle, RenderNow);
}



char ndBS_DrawCircle       (unsigned short int CenterX, unsigned short int CenterY, 
                            float Ray, TypeColor Color, TypeColor ColorBorder, 
                            ndint64 CodeStyle, char RenderNow)
/*
     Questa routine permette di disegnare un cerchio o una circonferenza dentro 
     la finestra caratterizzata da un certo WndHandle. 
     
     Il significato del parametro StyleCode è lo stesso usato nelle altre routine
     di disegno.
                      
     Non sono previsti codici di errore per questa routine
*/

{
    return ndBS_DrawArcOfEllipse (CenterX, CenterY, Ray, Ray, 0, 360, 
           Color, ColorBorder, CodeStyle, RenderNow);
}

/* 
   -------------------------------------------------------------------------------
   ROUTINES DI PULIZIA
   -------------------------------------------------------------------------------
*/

char ndBS_ClrBaseScreen (void)
/*
     Questa funzione pulisce il base screen.
     
     Non sono previsti codici di errore per questa routine
*/

{
    if ( !ndInitSystemExecuted ) ndInitSystem();
          
    unsigned short int CounterX, CounterY;

    for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
    {
        for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
        {
            ndBS_PutPixel_NORENDER_FAST (CounterX, CounterY, COLOR_BLACK);
        }
    }

    BaseScreenRender ();
    return 0;
}

/* 
   -----------------------------------------------------------------------------
   ROUTINES DI SCRITTURA SPECIALI
   -----------------------------------------------------------------------------
*/

char ndBS_GrWriteChar (unsigned short RRPosPixelX, unsigned short RRPosPixelY, unsigned char Carattere, TypeColor Color, TypeColor BGColor, ndint64 TextCode)
/*
     Questa funzione permette di scrivere un carattere nel base screen.
     
     Le coordinate RPosPixelX e RPosPixelY rappresentano le coordinate del pixel del carattere 
     più in alto a sinistra. Tali coordinate devono essere fornite rispetto al punto in alto
     a sinistra del base screen. 
      
     Questa funzione è stata progettata per la scrittura di caratteri singoli. 
     
     Le caratteristiche del carattere vengono passate per mezzo del parametro a 64 bit TextCode.
   
     I bit sono mappati nel seguente modo:
       
     63      Segno del codice di rotazione (1 indica un codice di rotazione negativo)          
     62-55   9 bit che contengono un codice di rotazione in gradi per il font (0-180)
             Non ha significato per i font che non sono di Tipo TrueType. Usate la 
             chiave NDKEY_SETFNTROTATE (x) dove x è compreso tra 0 e 180
             
     54-47   Indicano l'ampiezza del font. Non ha significato per i font che 
             non sono di Tipo TrueType
                
     31-23   Indicano l'handle del font da utilizzare. L'utente può ottenere la chiave necessaria
             attraverso la funzione NDKEY_FONT (FontHandle). Se si omette questa chiave, ovvero se
             si stabilisce un handle pari a 0, il sistema imposterà di default il font CONSOLE
             o l'ultimo font settato con ndBS_SetFont
             
     1       Se si imposta questo bit a 1, il sistema visualizzerà il carattere in modalità 
             trasparent. Quando viene attivata questa modalità, il parametro BGColor viene
             ignorato.
             
     0       Se si imposta questo bit a 1, il sistema richiamerà BaseScreenRender dopo la visualizzazione
             del carattere. Usare la costante manifesta RENDER
     
   
     
     Sono previsti i seguenti codici di errore:

     ERR_FONTHDL_IS_NOT_ACTIVE          L'handle del font non è attivo
*/
{
    unsigned char *FontData;		//puntatore al font
    
    unsigned char BitFilter, BitExtract;
    unsigned char ByteInChar;
    unsigned char FntHandle;
    int  IntX, IntY;
    
    char RenderNow, Trasp;
    ndint64 Temp;
    
    static char   ErrRep;
     
    int             NewTTFSize, TTFAngle;
	unsigned char   XStr [2];
    
    
    // Inizio del codice
     
    if ( !ndInitSystemExecuted ) ndInitSystem();
           
    Temp = TextCode;
    Temp = Temp >> (31-8);   
       
    FntHandle = (unsigned char) Temp;
    
    if (FntHandle==0) FntHandle=ndBS_CurrentFntHandle;
    
    // Parametri Render e Trasp
    
    if ((TextCode & RENDER) == RENDER) RenderNow=1;
       else RenderNow=0;
       
    if ((TextCode & TRASP) == TRASP) Trasp=1;
       else Trasp=0;
 
    // Inizio analisi
 
    if (FontHandlesArray [FntHandle].IsActive)
    {  
        if (!FontHandlesArray [FntHandle].IsTrueType)
        { 
                        if (Carattere!=0)
                        {
                           FontData=FontHandlesArray [FntHandle].Data+Carattere*8;	//Inizializza il puntatore alla corretta lettera nel buffer dei font
                    
                    	   ByteInChar=0;
                    
                    	   for (IntY=0; IntY<8; IntY++)
                    	   {
                    	   	   BitFilter=0x80;
                    
                    		   for (IntX=0; IntX<8; IntX++)
                    		   {
                    			   BitExtract=(*(FontData+ByteInChar)&BitFilter);
                    
                    			   if (BitExtract!=0)
                    			   {
                                       ErrRep=ndBS_PutPixel (RRPosPixelX+IntX, RRPosPixelY+IntY, Color, NORENDER);
                                   }
                                   else
                                   {
                                       if (!Trasp) 
                                         ErrRep=ndBS_PutPixel (RRPosPixelX+IntX, RRPosPixelY+IntY, BGColor, NORENDER);
                                       else
                                         ErrRep=0;  
                                   }
                                   
                                   if (ErrRep!=0) return ErrRep;
                    
                    			   BitFilter=BitFilter>>1;		//La maschera BitFilter è stata spostata di un bit a dx
                    		    }
                    
                    		    ByteInChar++;
                    
                           }
                    
                           if (RenderNow) BaseScreenRender ();           
                        }
                        
                        return 0;
        }
        else    // Font TrueType
        {
               #ifdef ND_ENABLE_TTF_SUPPORT         
                        
                        TTFAngle   = (unsigned short) ( (TextCode >> 63-9) & 0x00FF );      // Isolato il codice di rotazione (abs)          
                        if ( ( TextCode >> 63-9  ) & 0x0100 ) TTFAngle=-TTFAngle;
                        
                        NewTTFSize = (unsigned char)  ( (TextCode >> 63-9-8) & 0x00FF );    // Isolato il codice di ampiezza TTF
                    
                        // Inserisci il carattere nel buffer
                        XStr [0]=Carattere;
                        XStr [1]=0;
                        
                        // Esegui il renderer
                        ndFLIB_BS_TTFEngine0 (RRPosPixelX, RRPosPixelY, &XStr, Color, BGColor, FntHandle, NewTTFSize, TTFAngle, Trasp);
                    
                        if (RenderNow) BaseScreenRender ();          // Al termine del processo di copia il rendering fa apparire la stringa
                        return 0; 
                        
               #else
               
                        return ERR_GRAPHFONT_NOT_SUPPORTED;
               
               #endif 
        }
    }
    else
      return ERR_FONTHDL_IS_NOT_ACTIVE;
}

char ndBS_GrWriteLn (unsigned short RRPosPixelX, unsigned short RRPosPixelY, char *str, TypeColor Color, TypeColor BGColor, ndint64 TextCode)
/*
   Questa funzione serve per scrivere una stringa nel base screen.
   
   La stringa comparirà nel base screen a partire dal pixel determinato dalle coordinate RPosPixelX
   e RPosPixelY. Tali coordinate devono essere fornite rispetto al punto in alto a sinistra del
   base screen.
  
   Le caratteristiche del carattere vengono passate per mezzo del parametro a 64 bit TextCode.
   
     I bit sono mappati nel seguente modo:
       
     63      Segno del codice di rotazione (1 indica un codice di rotazione negativo)          
     62-55   9 bit che contengono un codice di rotazione in gradi per il font (0-180)
             Non ha significato per i font che non sono di Tipo TrueType. Usate la 
             chiave NDKEY_SETFNTROTATE (x) dove x è compreso tra 0 e 180
             
     54-47   Indicano l'ampiezza del font. Non ha significato per i font che 
             non sono di Tipo TrueType
                
     31-23   Indicano l'handle del font da utilizzare. L'utente può ottenere la chiave necessaria
             attraverso la funzione NDKEY_FONT (FontHandle). Se si omette questa chiave, ovvero se
             si stabilisce un handle pari a 0, il sistema imposterà di default il font CONSOLE
             o l'ultimo font settato con ndBS_SetFont
             
     1       Se si imposta questo bit a 1, il sistema visualizzerà il carattere in modalità 
             trasparent. Quando viene attivata questa modalità, il parametro BGColor viene
             ignorato.
             
     0       Se si imposta questo bit a 1, il sistema richiamerà BaseScreenRender dopo la visualizzazione
             del carattere. Usare la costante manifesta RENDER
     
   
   
   Sono previsti i seguenti codici di errore:
        ERR_FONTHDL_IS_NOT_ACTIVE          L'handle del font non è attivo       
*/

{
    int InternalPosX, InternalPosY;
         
    int MaxLenStr=255;
	int t=0;
	char *strpointer;			//Puntatore alla stringa
	
	char RenderNow, Trasp;
	ndint64 NewCode, Mask, Temp;
	
	unsigned char FntHandle;
	int           NewTTFSize, TTFAngle;
	
	static char ErrRep;

    // Inizio del codice

    if ( !ndInitSystemExecuted ) ndInitSystem();

	Temp = TextCode;
    Temp = Temp >> (31-8);   
       
    FntHandle = (unsigned char) Temp;
    
    if (FntHandle==0) FntHandle=ndBS_CurrentFntHandle;
    
    // Parametri Render e Trasp
    
    if ((TextCode & RENDER) == RENDER) RenderNow=1;
    else RenderNow=0;
                        
    if ((TextCode & TRASP) == TRASP) Trasp=1;
       else Trasp=0;
                     
    // Inizio analisi                 
                                            
    if (FontHandlesArray [FntHandle].IsActive)
    { 
        if (!FontHandlesArray [FntHandle].IsTrueType)
        {     
                        strpointer=str;
                    	   
                        Mask = 0xFFFFFFFFFFFFFFFE;
                        NewCode = TextCode & Mask;   // Cancella il bit meno significativo: serve a fare il NORENDER sui caratteri
                        
                        while (*strpointer!=0 && t<MaxLenStr)
                        {
                              InternalPosX=RRPosPixelX + t*8;
                              InternalPosY=RRPosPixelY;
                              
                    	      ErrRep=ndBS_GrWriteChar (InternalPosX, InternalPosY, *strpointer, Color, BGColor, NewCode);
                    	      // GrWriteChar erediterà attraverso NewCode tutte le caratteristiche (font, eventuale attributi di
                    	      // trasparenza) passate alla routine salvo l'attributo di rendering che sarà sempre fissato a
                    	      // NORENDER
                    	      
                    	      if (ErrRep!=0) return ErrRep;
                              
                              strpointer++;
                              t++;
                        }
                    
                        
                        if (RenderNow) BaseScreenRender ();          // Al termine del processo di copia il rendering fa apparire la stringa
                        return 0;
        }
        else    // Il font è un True Type
        {
                  #ifdef ND_ENABLE_TTF_SUPPORT         
                        
                        TTFAngle   = (unsigned short) ( (TextCode >> 63-9) & 0x00FF );      // Isolato il codice di rotazione (abs)          
                        if ( ( TextCode >> 63-9  ) & 0x0100 ) TTFAngle=-TTFAngle;
                        
                        NewTTFSize = (unsigned char)  ( (TextCode >> 63-9-8) & 0x00FF );    // Isolato il codice di ampiezza TTF
                        
                        // Esegui il renderer
                        ndFLIB_BS_TTFEngine0 (RRPosPixelX, RRPosPixelY, str, Color, BGColor, FntHandle, NewTTFSize, TTFAngle, Trasp);
                    
                        if (RenderNow) BaseScreenRender ();          // Al termine del processo di copia il rendering fa apparire la stringa
                        return 0;
                  
                  #else
                  
                        return ERR_GRAPHFONT_NOT_SUPPORTED;
                  
                  #endif
        }
    }
    else return ERR_FONTHDL_IS_NOT_ACTIVE;
}


void ndBS_GrPrintLn (unsigned short RRPosPixelX, unsigned short int RRPosPixelY, 
                    TypeColor Color, TypeColor BGColor, ndint64 TextCode, char *FirstParam, ...)
/*
    Questa routine si occupa di visualizzare una stringa complessa nel base
    screen.  
    
    Il funzionamento è analogo a quello previsto per la printf della libc, salvo
    per il fatto che l'utente non deve inserire un \n alla fine per andare a
    capo.
    
    La stringa comparirà nel base screen a partire dal pixel determinato dalle 
    coordinate RPosPixelX e RPosPixelY. Tali coordinate devono essere fornite 
    rispetto al punto in alto a sinistra del base screen. 
    
    Sono previsti tutti i codici di errore previsti per ndBS_GrWriteLn, ed 
    in più un codice di errore:
       ERR_INTERNAL_ERROR        Un problema con snprintf
*/
{
      va_list args;  // Lista degli argomenti da passare. Il tipo va_list indica una
                     // lista di parametri passati alla funzione per mezzo di  
                     // ellissi. La funzione che vuole accedere ai singoli parametri
                     // deve usare la macro va_arg (puntatore alla lista, tipo parametro)
                     // per ottenere i vari parametri uno alla volta
      
      struct s_strargument xarg;    // Struttura argomenti: contiene i parametri necessari
                                    // per la corretta programmazione del funzionamento di
                                    // vxprintf (ad esempio, l'indirizzo della stringa dove
                                    // è necessario scrivere e l'eventuale limite dei
                                    // caratteri da scrivere.
                                    
      int ErrRep1;
      
      xarg.next = &IntBufPrintF;          // Preparati a programmare la snprintf in modo che scriva
      xarg.last = &(IntBufPrintF[511]);   // in IntBufPrintf e setta l'indirizzo dell'ultimo elemento
                                          // del buffer in modo da evitare buffer overflow.       
                                      
      va_start(args, FirstParam);   // Copia in args la lista di parametri a passati a ndLL_PrintF
                                    // a partire da Stringa.
                                    
      ErrRep1 = vxprintf(_INTERNAL_WriteFncToString_ , &xarg, FirstParam, args);   // Il fatto che venga richiamato vxprintf usando
                                                     // _INTERNAL_WriteFncToString_  come routine di scrittura, significa che
                                                     // stiamo eseguendo una snprintf. xarg fornirà alla
                                                     // snprintf le informazioni necessarie (locazione in
                                                     // memoria della stringa da scrivere e numero di 
                                                     // caratteri necessari. Stringa fornirà alla routine
                                                     // l'indirizzo del primo argomento e args fornirà la
                                                     // lista degli argomenti successivi.
    
      va_end(args);     // Disalloca la lista degli argomenti 
      
      if (ErrRep1>=0)       // Non ci sono stati problemi nella scrittura
      {
         ndBS_GrWriteLn (RRPosPixelX, RRPosPixelY, &IntBufPrintF, Color, BGColor, TextCode);
      } 
  
}



char ndBS_SetFont (unsigned char FntHandle)
/*
   Questa routine provvede a settare il font per le routines di tipo BS.
   
   Le routines di tipo BS supportano esclusivamente i font di tipo TEXT e non
   i font di tipo GRAPHICS. Questa limitazione sarà rimossa nella prossima
   versione di Nanodesktop. 
   
   Sono previsti i seguenti codici di errore:
        ERR_FONTHDL_IS_NOT_ACTIVE     Il font handle non è attivo
        ERR_GRAPHFONT_NOT_SUPPORTED   I graphfont non sono supportati
*/
{
   if ( !ndInitSystemExecuted ) ndInitSystem();
           
   if (FontHandlesArray [FntHandle].IsActive)
   {                   
      if ( (FontHandlesArray [FntHandle].features & NDTEXTFONT) == NDTEXTFONT )
      {
         ndBS_CurrentFntHandle = FntHandle;
         // Registra il nuovo text-font corrente
                  
         return 0;
         // e ritorna il codice di controllo 0
      }
      else return ERR_GRAPHFONT_NOT_SUPPORTED;
   }
   else
       return ERR_FONTHDL_IS_NOT_ACTIVE;   

}


