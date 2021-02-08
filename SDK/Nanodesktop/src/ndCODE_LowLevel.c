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
    -------------------------------------------------------------------------
    ROUTINES CHE LAVORANO A LIVELLO LOW
    -------------------------------------------------------------------------
*/

#include <$_NDforND.h>



void ndCODE_InitGraph (void)
/*  E' la routine di reinizializzazione logica del sistema a finestre di Nanodesktop.

    Questa routine reinizializza le variabili della libreria ai loro valori di
    default. Non contiene codice specifico per l'hardware: tale codice è 
    contenuto in ndHAL_InitGraph.

    Ha il compito di richiamare le routine del kernel che si occupano di inizializzare
    il framebuffer ed imposta ai valori iniziali tutte le variabili del sistema
    grafico */

{   
      unsigned short int Counter, CounterX, CounterY;
    
      LL_PosCursX=0;
      LL_PosCursY=0;

      // Gestore dello sfondo

      for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
        for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
              BaseScreen [CounterY][CounterX] = 0;

      BaseScreenIsActive=0;
      
      TrasparencyEnabled=0;
      ThumbnailEnabled=1;

      // Reinizializzazione pagine logiche

      for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
      {
          WindowData [Counter].WindowIsActive=0;
          
          WindowData [Counter].WindowTitle=0;
          WindowData [Counter].ColorTitle=0;
          WindowData [Counter].ColorBGTitle=0;
          WindowData [Counter].ColorBGWS=0;

          WindowData [Counter].WndLimitX1=0;
          WindowData [Counter].WndLimitY1=0;
          WindowData [Counter].WndLimitX2=0;
          WindowData [Counter].WndLimitY2=0;

          WindowData [Counter].ROI_PosX=0;
          WindowData [Counter].ROI_PosY=0;

          WindowData [Counter].ROI_LenX=0;
          WindowData [Counter].ROI_LenY=0;

          WindowData [Counter].AA1_X1=0;
          WindowData [Counter].AA1_Y1=0;
          WindowData [Counter].AA1_X2=0;
          WindowData [Counter].AA1_Y2=0;
          WindowData [Counter].AA1_LenX=0;
          WindowData [Counter].AA2_LenY=0;

          WindowData [Counter].AA2_X1=0;
          WindowData [Counter].AA2_Y1=0;
          WindowData [Counter].AA2_X2=0;
          WindowData [Counter].AA2_Y2=0;
          WindowData [Counter].AA2_LenX=0;
          WindowData [Counter].AA2_LenY=0;

          WindowData [Counter].AA3_X1=0;
          WindowData [Counter].AA3_Y1=0;
          WindowData [Counter].AA3_X2=0;
          WindowData [Counter].AA3_Y2=0;
          WindowData [Counter].AA3_LenX=0;
          WindowData [Counter].AA3_LenY=0;

          WindowData [Counter].WndLength=0;
          WindowData [Counter].WndHeight=0;

          WindowData [Counter].WSWndLength=0;
          WindowData [Counter].WSWndLength=0;
          
          WindowData [Counter].Minimize=0;

          WindowData [Counter].WS_MaxCursX=0;
          WindowData [Counter].WS_MaxCursY=0;

          WindowData [Counter].WS_PosCursX=0;
          WindowData [Counter].WS_PosCursY=0;
       }

       for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
       {
         WindowsVector [Counter]=255;                               // Un valore 255 in Windows Vector significa non allocato
       }

       NrWindowsAllocated=0;

       IsTheFirstTimeForMouse=1;

       MousePointer_PosX=MaxScreenX/2;
       MousePointer_PosY=MaxScreenY/2;

       for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)       // L'array dei pulsanti di classe 1 viene posto pari a
       {                                                              // zero.
          ButtonClass1_Data [Counter].ButtonIsActivated = 0;
       }

       for (Counter=0; Counter<4; Counter++)                          // L'array dei pulsanti di classe 2 viene posto pari a
       {                                                              // zero.
          ButtonClass2_Data [Counter].ButtonIsActivated = 0;
       }

       for (CounterX=0; CounterX<__NRWINDOWSALLOCABLE; CounterX++)
       {
         for (Counter=0; Counter<7; Counter++)
         {
             WindowData[CounterX].ButtonClass3_Data[Counter].ButtonIsActivated=0;
         }
       }

       for (CounterX=0; CounterX<__NRWINDOWSALLOCABLE; CounterX++)
       {
         WindowData [CounterX].NrButtonsAllocated=0;
         WindowData [CounterX].NrTrackBarsAllocated=0;

         for (Counter=0; Counter<__NRBUTTONSALLOCABLE; Counter++)
             WindowData [CounterX].ButtonClass4_Data[Counter].ButtonIsActivated=0;
         
         for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
             WindowData [CounterX].TrackBar [Counter].IsActive = 0;
       
         for (Counter=0; Counter<__NRTEXTBOXALLOCABLE; Counter++)
         {
             WindowData [CounterX].TextBox [Counter].IsActive       = 0;
             WindowData [CounterX].TextBox [Counter].KeybIsOpened   = 0;
             WindowData [CounterX].TextBox [Counter].TextAreaIsOn   = 0; 
         }
         
         for (Counter=0; Counter<__NRLISTBOXALLOCABLE; Counter++)
         {
             WindowData [CounterX].ListBox [Counter].IsActive              = 0;
             WindowData [CounterX].ListBox [Counter].NrSlotsVisualizable   = 0;
         }
             
       
       }

       MinWindowsBar_LenX = MinWindowsBar_x2 - MinWindowsBar_x1;
       MinWindowsBar_LenY = MinWindowsBar_y2 - MinWindowsBar_y1;

       MinWindowsBarIsChanged = 1;                                  // La minwindows bar viene rigenerata ad ogni windowrender,
                                                                    // ma solo se vi sono cambiamenti. Ponendo ad 1 il valore
                                                                    // iniziale di MinWindowsBar, al primo WindowRender ci
                                                                    // sarà la reinizializzazione dei pulsanti

       OtherFBPageIsNotUpgraded = 0;
 
       // Reinizializzazione dell'owner array
       
       for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
       {
          for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
          {
              OwnerArray [CounterX][CounterY][0]=255;
              OwnerArray [CounterX][CounterY][1]=255;
          }
       }   
       
       // Reinizializzazione del sistema dei font
       
       for (Counter=0; Counter<NRFONTS_USABLE; Counter++)
         FontHandlesArray [Counter].IsActive=0;
       
       // Necessario per il corretto funzionamento del resizer di sistema
       
       ResizerFocusActive=0;
       ResizerFocusWindow=0;
        
}


#define ndLL_PutPixel                         ndHAL_PutPixel
#define ndLL_GetPixel                         ndHAL_GetPixel
#define ndLL_PutPixelToPage                   ndHAL_PutPixelToPage
#define ndLL_GetPixelFromPage                 ndHAL_GetPixelFromPage
#define ndLL_ChangeFrameBufferPage            ndHAL_ChangeFrameBufferPage

void ndLL_DrawRectangle (unsigned short int RRPosX1, unsigned short int RRPosY1, 
                         unsigned short int RRPosX2, unsigned short int RRPosY2, 
                         TypeColor Color)
{
    // Provvede a creare un rettangolo a livello LL. Usato dal BootScreen in
    // modalità KSU
    
    int CounterX, CounterY;
    
    for (CounterY=RRPosY1; CounterY<RRPosY2; CounterY++)
    {
        for (CounterX=RRPosX1; CounterX<RRPosX2; CounterX++)
        {
            ndLL_PutPixel (CounterX, CounterY, Color);
        }
    }
}


void ndLL_WriteChar (short int ChPosX, short int ChPosY, char Carattere, TypeColor Color)
		// Questa funzione viene usata dalla routine ndLL_WriteString per scrivere dei caratteri
        // sullo schermo. Si tratta di una funzione che dovrebbe essere usata solo a scopo diagnostico.
        // La funzione scrive il carattere nel punto desiderato. Il puntatore per il
		// cursore non viene aggiornato, e non è supportato alcun tipo di scrolling.
{
	int 		  PosPixelCharX, PosPixelCharY;
	unsigned char *TextFontData;		//puntatore al font

	unsigned char BitFilter, BitExtract;
	unsigned char IntX, IntY;
	unsigned char ByteInChar;

    if (!ndInitSystemExecuted) ndInitSystem();

	if (Carattere!=0)
	{
		PosPixelCharX=ChPosX*8;
		PosPixelCharY=ChPosY*8;

		TextFontData=FontHandlesArray [ndLL_CurrentFntHandle].Data + Carattere*8;	    //Inizializza il puntatore alla corretta lettera nel buffer dei font

		ByteInChar=0;

		for (IntY=0; IntY<8; IntY++)
		{
			BitFilter=0x80;

			for (IntX=0; IntX<8; IntX++)
			{
				BitExtract=(*(TextFontData+ByteInChar)&BitFilter);

				if (BitExtract!=0) ndLL_PutPixel (PosPixelCharX+IntX, PosPixelCharY+IntY, Color);

				BitFilter=BitFilter>>1;		//La maschera BitFilter è stata spostata di un bit a dx
			}

			ByteInChar++;

		}
	}
}

void ndLL_GoCursor(unsigned short int PosX, unsigned short int PosY)
/*   Questa funzione serve a spostare il cursore a basso livello. Questo cursore si riferisce
     alle routine diagnostiche
*/
{
    if (!ndInitSystemExecuted) ndInitSystem();

	LL_PosCursX=PosX;
	LL_PosCursY=PosY;
}

void ndLL_CarriageReturn(void)
{
    if (!ndInitSystemExecuted) ndInitSystem();

    if (LL_PosCursY<MaxCursY) LL_PosCursY++;
    LL_PosCursX=0;
}

void ndLL_WriteLetter(char Lettera, TypeColor Color)
/*
		 Questa funzione provvede a scrivere un carattere sullo schermo nella
		 posizione corrente del cursore. A differenza di WriteChar, la posizione
		 viene aggiornata: quindi, se il cursore supera i limiti di schermo,
		 viene eseguito il carriage return. La funzione agisce a basso livello
		 e quindi dovrebbe essere usata solo a scopo diagnostico
         Poichè queste routine devono servire solo a scopo diagnostico, il
         carriage return e lo scrolling non sono supportati. Le parti della
         stringa che portano il testo fuori schermo vengono semplicemente
         troncate.
*/

{
    if (!ndInitSystemExecuted) ndInitSystem();

    if (LL_PosCursX<MaxCursX)
    {
        ndLL_WriteChar (LL_PosCursX, LL_PosCursY, Lettera, Color);
        LL_PosCursX++;

        if (LL_PosCursX==MaxCursX) ndLL_CarriageReturn();
    }
}


void ndLL_Write (char *str, TypeColor color)
/*
   Questa funzione, insieme a ndLL_WriteLn si occupa di stampare una stringa sullo
   schermo nelle coordinate definite dalle variabili LL_PosCursX e LL_PosCursY.
   La funzionalità dovrebbe essere usata solo a scopo diagnostico
*/

{
	int MaxLenStr=255;
	int t=0;
	char *strpointer;			//Puntatore alla stringa

    if (!ndInitSystemExecuted) ndInitSystem();

	strpointer=str;

	while (*strpointer!=0 && t<MaxLenStr)
	{
		ndLL_WriteLetter (*strpointer, color);
		strpointer++;
		t++;
	}

}

void ndLL_WriteLn (char *str, TypeColor color)
/*
   Questa funzione si occupa di stampare una stringa sullo
   schermo nelle coordinate definite dalle variabili LL_PosCursX e LL_PosCursY.
   Alla fine dell'operazione viene mandato a capo il cursore.
   La funzionalità dovrebbe essere usata solo a scopo diagnostico
*/

{
	ndLL_Write (str, color);
	ndLL_CarriageReturn();
}

void ndLL_ClrScr (void)
// Questa funzione pulisce lo schermo
{
     unsigned short int x, y;

     if (!ndInitSystemExecuted) ndInitSystem();

     for (y=0; y<MaxScreenY; y++)
     {
         for (x=0; x<MaxScreenX; x++)
         {
             ndLL_PutPixel (x,y,0);
         }
     }

     ndLL_GoCursor (0,0);
}

void ndLL_WriteCharSpecial (short int PosPixelCharX, short int PosPixelCharY, char Carattere, TypeColor Color, unsigned short int Page)
     // Questa funzione viene usata da funzioni speciali come ndLP_DrawMinWindowsBar per disegnare un carattere in
     // un pixel specifico. La funzione può scrivere anche in una pagina del framebuffer diversa da quella corrente
{
	unsigned char *TextFontData;		//puntatore al font

	unsigned char BitFilter, BitExtract;
	unsigned char IntX, IntY;
	unsigned char ByteInChar;

    if (!ndInitSystemExecuted) ndInitSystem();

	if (Carattere!=0)
    {
		TextFontData=FontHandlesArray [ndLL_CurrentFntHandle].Data+Carattere*8;	//Inizializza il puntatore alla corretta lettera nel buffer dei font

		ByteInChar=0;

		for (IntY=0; IntY<8; IntY++)
		{
			BitFilter=0x80;

			for (IntX=0; IntX<8; IntX++)
			{
				BitExtract=(*(TextFontData+ByteInChar)&BitFilter);

				if (BitExtract!=0) ndLL_PutPixelToPage (PosPixelCharX+IntX, PosPixelCharY+IntY, Color, Page);

				BitFilter=BitFilter>>1;		//La maschera BitFilter è stata spostata di un bit a dx
			}

			ByteInChar++;

		}
	}
}

void ndLL_PrintLn (TypeColor Color, char *FirstParam, ...)
/*
    Questa funzione si occupa di visualizzare sullo schermo una stringa complessa.
    Il funzionamento è analogo a quello previsto per la printf della libc, salvo
    per il fatto che l'utente non deve inserire un \n alla fine per andare a
    capo (viene fatto automaticamente)
*/
{
      va_list args;  // Lista degli argomenti da passare. Il tipo va_list indica una
                     // lista di parametri passati alla funzione per mezzo di un 
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
         ndLL_WriteLn (&IntBufPrintF, Color);
      } 
}


void ndLL_Print (TypeColor Color, char *FirstParam, ...)
/*
    Funzione analoga a ndLL_PrintLn, salvo che non esegue automaticamente un
    carriage return alla fine dell'operazione di scrittura su schermo
*/
{
      va_list args;  // Lista degli argomenti da passare. Il tipo va_list indica una
                     // lista di parametri passati alla funzione per mezzo di un 
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
         ndLL_Write (&IntBufPrintF, Color);
      } 
}



char ndLL_SetFont (unsigned char FntHandle)
/*
   Questa routine provvede a settare il font per le routines di tipo LL.
   
   Le routines di tipo LL supportano esclusivamente i font di tipo TEXT e non
   i font di tipo GRAPHICS. 
   
   Sono previsti i seguenti codici di errore:
        ERR_FONTHDL_IS_NOT_ACTIVE     Il font handle non è attivo
        ERR_GRAPHFONT_NOT_SUPPORTED   I graphfont non sono supportati
*/
{
   if (FontHandlesArray [FntHandle].IsActive)
   {                   
      if ( (FontHandlesArray [FntHandle].features & NDTEXTFONT) == NDTEXTFONT )
      {
         ndLL_CurrentFntHandle = FntHandle;
         // Registra il nuovo text-font corrente
         
         return 0;
      }
      else return ERR_GRAPHFONT_NOT_SUPPORTED;
   }
   else
       return ERR_FONTHDL_IS_NOT_ACTIVE;   
}




