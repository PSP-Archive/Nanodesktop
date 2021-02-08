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
    ROUTINE CHE LAVORANO A LIVELLO LOGICAL PAGE (LP)
    ----------------------------------------------------------------------------
    Queste routine si occupano di gestire le 8 finestre disponibili a livello
    logical page.

    Sono divise in tre categorie:

    a) Routines LP relative alla logica di gestione delle finestre
    b) Routines LP per la scrittura sullo schermo
    c) Routines LP per l'allocazione delle finestre
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

/*
    -------------------------------------------------------------------------------
    ROUTINE LP RELATIVE ALLA LOGICA DI GESTIONE DELLE FINESTRE (Windows Vector)
    -------------------------------------------------------------------------------
*/

unsigned char ndLP_IsPixelInWindow (unsigned short int PosPixelX, unsigned short int PosPixelY, unsigned char WndHandle)
/*
         Questa funzione restituisce 1 se il pixel fa parte della pagina logica della finestra
         definita dall'handle.

         Altrimenti restituisce 0.
*/

{
   return ( (WindowData [WndHandle].WndLimitX1 <= PosPixelX) &&
            (WindowData [WndHandle].WndLimitX2 >= PosPixelX) &&
            (WindowData [WndHandle].WndLimitY1 <= PosPixelY) &&
            (WindowData [WndHandle].WndLimitY2 >= PosPixelY) );
}

char ndLP_ShootElementInTheWV (unsigned char Element)
/*
   Questa funzione pone l'elemento Element al primo posto del
   windowvector.

   Per impedire che possano essere presenti due elementi
   identici nel windows vector, viene eseguito un controllo
   preliminare. E' previsto un codice di errore

   ERR_ELEMENT_ALREADY_PRESENT    L'elemento è già presente

   L'ultimo elemento viene sempre perso, ma non è un problema
   perchè la routine viene eseguita solo quando ci sono allocate
   meno di 8 finestre.
*/

{
   int Counter, FoundElement;

   if (!ndInitSystemExecuted) ndInitSystem();

   FoundElement=0;

   for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)        // Questo è il primo test per identificare la presenza di
   {                                                               // element nel vettore
      if (WindowsVector [Counter]==Element)
      {
         FoundElement=1;
         break;
      }
   }

   if (FoundElement==1) return ERR_ELEMENT_ALREADY_PRESENT;

   for (Counter=__NRWINDOWSALLOCABLE-2; Counter>=0; Counter--)
         WindowsVector [Counter+1]= WindowsVector [Counter];

   WindowsVector [0]=Element;

   return 0;
}


char ndLP_PutElementAtTheTopOfWV (unsigned char Element)
/*
  Questa funzione pone l'elemento Element al top del Window Vector.
  Viene prima controllato che l'elemento sia già presente nel
  vettore, e poi lo si porta al primo piano.

  La funzione prevede un codice di errore:

    ERR_ELEMENT_NOT_PRESENT     L'elemento non è presente
*/

{
   unsigned char TempWV [__NRWINDOWSALLOCABLE];
   unsigned char a1;
   int Counter, Index;
   int FoundElement;

   if (!ndInitSystemExecuted) ndInitSystem();

   FoundElement=0;

   for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)        // Questo è il primo test per identificare la presenza di
   {                                                               // element nel vettore
      if (WindowsVector [Counter]==Element)
      {
         FoundElement=1;
         break;
      }
   }

   if (FoundElement==0) return ERR_ELEMENT_NOT_PRESENT;

   TempWV [0]=Element;

   Index=1;

   for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
   {
       a1=WindowsVector [Counter];

       if (a1!=Element)
       {
          TempWV [Index]=a1;
          Index++;
       }
   }

   // Arrivati a questo punto, in TempWV c'è già il vettore che serve a noi
   // e si tratta solo di ricopiarlo nel vecchio WindowsVector

   for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
   {
       WindowsVector [Counter]=TempWV [Counter];
   }

   return 0;
}

char ndLP_RemoveElementFromWV (unsigned char Element)
/* Questa funzione si occupa di cancellare un elemento dal windows vector.
   Questa funzione controlla anche che l'elemento non sia già stato
   cancellato dal window vector.

   E' previsto un codice di errore
        ERR_ELEMENT_NOT_PRESENT     L'elemento non è presente
*/

{
  int FoundElement, Counter, Index;
  char TempWV [__NRWINDOWSALLOCABLE];
  char a1;

  if (!ndInitSystemExecuted) ndInitSystem();

  FoundElement=0;

   for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)        // Questo è il primo test per identificare la presenza di
   {                                                               // element nel vettore
      if (WindowsVector [Counter]==Element)
      {
         FoundElement=1;
         break;
      }
   }

   if (FoundElement==0) return ERR_ELEMENT_NOT_PRESENT;

   Index=0;                                                        // Adesso inizia a copiare i dati in un buffer temporaneo

   for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
   {
       a1=WindowsVector [Counter];

       if (a1!=Element)
       {
          TempWV [Index]=a1;
          Index++;
       }
   }

   TempWV[__NRWINDOWSALLOCABLE-1]=255;                              // Avendo eliminato un elemento, è necessario porre
                                                                    // un 255 (non allocato) alla fine del vettore
   for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
   {
       WindowsVector [Counter]=TempWV [Counter];                    // Il buffer temporaneo viene copiato su quello ordinario
   }

   return 0;
}


inline void ndLP_FindOwnerOfPixel (unsigned short int PosPixelX, unsigned short int PosPixelY, unsigned char *OwnerID)
/*
     Questa routine ha lo scopo di individuare qual è la finestra che "possiede" un certo pixel, ossia
     a quale finestra massimizzata sullo schermo appartiene quel pixel in quel dato momento.

     La routine risponde con l'handle della finestra. Se la routine risponde con un valore
     255, allora vuol dire che il pixel non appartiene a nessuna finestra in quel momento.
     
     La routine chiamante deve allocare una variabile di tipo unsigned char nel proprio
     stack, atta ad ospitare l'informazione richiesta. Di tale variabile viene passato
     l'indirizzo.

     Le coordinate del pixel sono espresse sempre in modo assoluto.
*/

{
     int Counter, IDWindow;
     
     if (!ndInitSystemExecuted) ndInitSystem();

     for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
     {
         if (WindowsVector[Counter]!=255)
         {
             IDWindow=WindowsVector[Counter];    // La prima (o seconda, terza ecc. ecc.) finestra nel Windows Vector;

             if ( !(WindowData [IDWindow].Minimize) )       // Il processo di analisi deve andare avanti solo se la finestra non
                                                            // è minimizzata
             {                        
                 if (ndLP_IsPixelInWindow(PosPixelX, PosPixelY, IDWindow))
                 {
                    *OwnerID=IDWindow;          
                    return;                        // Se il pixel appartiene alla finestra riporta in uscita l'handle
                 }                                 // ed esce
             }
         }
         else
         {
            *OwnerID=255;                             
                                         
            return;                     // Poichè le routine Windows Vector non prevedono che possa esserci un
                                        // elemento pari a 255 in mezzo al vettore (è previsto solo alla fine), se
                                        // il contatore è arrivato fino a far sì che il vettore riporti 255 significa
                                        // che fin qui non è stato trovato alcun proprietario del pixel, e neanche lo
                                        // si troverà in seguito, pertanto la funzione deve ritornare 255;
         }

         
     }
}


inline void ndLP_FindTwoOwnersOfPixel (unsigned short int PosPixelX, unsigned short int PosPixelY,
                                       unsigned char *OwnerID1, unsigned char *OwnerID2)
/*
   Questa routine provvede a trovare l'ID della finestra a cui appartiene un certo
   pixel e l'ID della finestra immediatamente sottostante.
   
   Se il pixel non appartiene ad alcuna finestra, *Owner1ID e *Owner2ID verranno posti
   a 255. 
   
   Se il pixel appartiene ad una finestra, ma non è presente alcuna finestra sottostante,
   *Owner1D riporterà l'ID della finestra, mentre *Owner2D riporterà 255.
   
   La routine chiamante deve allocare due variabili di tipo unsigned char nel proprio
   stack e ne deve passare gli indirizzi come parametri. 
   
   Non sono previsti codici di errore per questa routine.
*/

{  
  unsigned short int IHaveFound1, IHaveFound2;
  unsigned short int Counter;
  
  int IDWindow;

  Counter=0;
  IHaveFound1=0;
  IHaveFound2=0;

  *OwnerID1=255;            // Impostiamo a 255 per sicurezza
  *OwnerID2=255;

  if (!ndInitSystemExecuted) ndInitSystem();

  while (  (Counter<__NRWINDOWSALLOCABLE) && (! (IHaveFound1 && IHaveFound2) )  )
  {
     IDWindow = WindowsVector [Counter];

     if (IDWindow==255) break;          // Siamo a fine vettore. Esegui break

     if (!IHaveFound1)                // Non è stata ancora trovata la prima finestra
     {
            if  ( (!WindowData [IDWindow].Minimize) && ( ndLP_IsPixelInWindow (PosPixelX, PosPixelY, IDWindow) ) )
            {
                 IHaveFound1=1;
                 *OwnerID1=IDWindow;
            }
     }
     else        // Se la prima finestra è stata già trovata, stiamo cercando la seconda
     {
            if (!(WindowData [IDWindow].Minimize) && ( ndLP_IsPixelInWindow (PosPixelX, PosPixelY, IDWindow) ) )
            {
                 IHaveFound2=1;
                 *OwnerID2=IDWindow;
            }
     }
     
     Counter++;
  }
       
}





unsigned char ndLP_MyPixelIsInFirstPlane (unsigned int RPosPixelX, unsigned int RPosPixelY, unsigned char WndHandle)
{
    unsigned char x;

    if (!ndInitSystemExecuted) ndInitSystem();

    ndLP_FindOwnerOfPixel (WindowData [WndHandle].WndLimitX1+RPosPixelX, 
                           WindowData [WndHandle].WndLimitY1+RPosPixelY, &x);

    if (x==WndHandle)
       return 1;
    else
       return 0;
}





void WindowsRender ()
/*
     Questa routine è una delle più importanti della libreria NANODesktop.

     Essa, esegue il rendering dell'insieme di finestre: a partire dalle varie Logical Page,
     e considerati i dati nel Windows Vector, viene disegnata la pagina video.

     La pagina video viene prima disegnata in una pagina off-screen del framebuffer, e poi
     avviene la commutazione del video.

     Si noti che le routines WindowsRender sono state spostate in HAL allo scopo di supportare
     l'accelerazione hardware specifica per la piattaforma e la serializzazione per il
     supporto del multithreading (ove previsto)
*/

{
   ndHAL_WindowsRender_CLIENT ();     // Richiama il client WindowsRender
}





void ndLP_EnableTrasparency (void)
/*
   Attiva le trasparenze del desktop.
*/

{
   ndHAL_LockPhoenixGraphThread ();
   
   {
      TrasparencyEnabled=1;
   }
   
   ndHAL_UnlockPhoenixGraphThread ();
   
   ndHAL_UpdateWindowsRenderRegisters ();
   WindowsRender ();
}

void ndLP_DisableTrasparency (void)
/*
  Disattiva le trasparenze del desktop
*/

{
   ndHAL_LockPhoenixGraphThread ();
   
   {
      TrasparencyEnabled=0;
   }
   
   ndHAL_UnlockPhoenixGraphThread ();
   
   ndHAL_UpdateWindowsRenderRegisters ();
   WindowsRender ();
}



char ndLP_LoadWallPaperFromFile (char *NameFile)
/*
   Questa routine consente di selezionare uno sfondo per il desktop.
   
   Se la stringa NameFile è vuota, la routine non ha effetto. 
   
   Se invece la stringa NameFile non è vuota, carica l'immagine nel buffer
   di sfondo e la visualizza.
   
   Riporta in uscita 0 se tutto va correttamente, oppure riporta il codice
   di errore di LoadImage in caso di errore. 
   
   Lo sfondo viene sottoposto ad un processo di scaling automatico per
   adattarlo alle dimensioni dello schermo.
   
   La routine restituisce 0 in mancanza di errori. Se invece viene restituito
   un codice di errore negativo, siamo in presenza di errori. 
   
   Sono previsti i seguenti codici di errore:
        ERR_FILE_NOT_FOUND          Il file non è stato trovato
        ERR_FORMAT_NOT_SUPPORTED    Formato dell'immagine non supportato
        ERR_IO_IMAGE                Un errore non identificato nel 
                                    caricamento dell'immagine
        ERR_MEMORY_OVERFLOW         Overflow della memoria
        ERR_LOGICAL_IN_IMAGE        Errore logico nell'immagine
        ERR_VALUE_MISMATCH          Si è fornito un nome di file vuoto
*/

{
   unsigned short int CounterX, CounterY;
   unsigned short int RevX, RevY;
   
   struct ndImage_Type BufferImage;
   TypeColor MagicNumber;
   
   float ScaleX, ScaleY;
   char ErrRep;
   
   if (strcmp (NameFile, "")!=0)
   {
      ErrRep = ndIMG_LoadImage (&BufferImage, NameFile, NDMGKNB);
      // Carica in bufferimage l'immagine necessaria.
   
      if (!ErrRep)
      {
         ScaleX = ((float)(Windows_MaxScreenX)) / ((float)(BufferImage.LenX));
         ScaleY = ((float)(Windows_MaxScreenY)) / ((float)(BufferImage.LenY));
   
         for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
         {
             for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
             {
                 RevX = (int) ( CounterX / ScaleX );
                 RevY = (int) ( CounterY / ScaleY );
             
                 ndIMG_GetPixelFromImage (&BufferImage, RevX, RevY, &MagicNumber);
             
                 BaseScreen [CounterY][CounterX]=MagicNumber;
             }
         }
     
         BaseScreenIsActive=1;
         WindowsRender ();
     
         ndIMG_ReleaseImage (&BufferImage);
         return 0;    
     }
     else
         return (ErrRep);
   }
   else   // Si è fornito un nome di file vuoto
     return ERR_VALUE_MISMATCH; 
}


char ndLP_LoadWallPaperFromNDImage (struct ndImage_Type *MyImage)
/*
   Questa routine consente di selezionare uno sfondo per il desktop.
   
   L'immagine di sfondo deve essere già stata caricata con
   un ndIMG_LoadImage nella struct MyImage.
   
   Lo sfondo viene sottoposto ad un processo di scaling automatico per
   adattarlo alle dimensioni dello schermo.

   Qualora MyImage non punti ad alcuna immagine, la routine non
   ha effetto.
   
   La routine restituisce 0 se non vi sono errori. Qualora la routine
   restituisca un valore negativo, siamo in presenza di un errore. 
   
   Sono previsti i seguenti codici di errore:
        ERR_IMAGE_IS_NOT_ALLOCATED        L'immagine non è allocata
        
*/
{
   unsigned short int CounterX, CounterY;
   unsigned short int RevX, RevY;
   
   TypeColor MagicNumber;
   float ScaleX, ScaleY;
   
   if ((MyImage->Data)!=0)
   {
         ScaleX = ((float)(Windows_MaxScreenX)) / ((float)(MyImage->LenX));
         ScaleY = ((float)(Windows_MaxScreenY)) / ((float)(MyImage->LenY));
   
         for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
         {
             for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
             {
                 RevX = (int) ( CounterX / ScaleX );
                 RevY = (int) ( CounterY / ScaleY );
             
                 ndIMG_GetPixelFromImage (MyImage, RevX, RevY, &MagicNumber);
             
                 BaseScreen [CounterY][CounterX]=MagicNumber;
             }
         }
     
         BaseScreenIsActive=1;
         WindowsRender ();
     
         return 0;    
   }
   else    // L'immagine non è allocata
   {
         return ERR_IMAGE_IS_NOT_ALLOCATED;  
   }
}


void ndLP_EnableBaseScreen(void)
/*
  Attiva lo sfondo
*/

{ 
   BaseScreenIsActive=1;
   WindowsRender ();
}



void ndLP_DisableBaseScreen(void)
/*
  Disattiva lo sfondo
*/

{ 
   BaseScreenIsActive=0;
   WindowsRender ();
}

void ndLP_EnableWallPaper (void)
{
     ndLP_EnableBaseScreen ();
}

void ndLP_DisableWallPaper (void)
{
     ndLP_DisableBaseScreen ();
}





/*
  ----------------------------------------------------------------------------
  2.2 ROUTINE LP RELATIVE ALLA SCRITTURA A LIVELLO LOGICAL PAGE
  ----------------------------------------------------------------------------
*/


char ndLP_PutPixel (unsigned short int RPosPixelX, unsigned short int RPosPixelY, TypeColor Color, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione si occupa di inserire un pixel nella pagina logica di una data finestra. Le coordinate devono
     essere fornite con riferimento al pixel della finestra in alto a sinistra (contenute in WndLimitX1 e WndLimitY1),
     ossia in termini relativi e non assoluti.

     Sono previsti due errori:
            ERR_WINDOW_IS_NOT_ALLOCATED     Finestra non allocata
            ERR_PIXEL_IS_OUT_OF_WINDOW      Posizione del pixel fuori finestra

     La funzione possiede un meccanismo di accelerazione. Dopo la scrittura del pixel viene controllato se esso si trova in
     primo piano e se è così esso viene immediatamente stampato sullo schermo. Ciò avviene però solo se l'ultimo parametro è
     diverso da 0.

     Se invece l'ultimo parametro è pari a 0, allora il pixel non viene renderizzato finchè non viene eseguita una
     WindowRender

*/

{
  unsigned short int PosPixelX, PosPixelY;

  if (!ndInitSystemExecuted) ndInitSystem();

  if (WindowData [WndHandle].WindowIsActive)
  {
      if ((RPosPixelX<=WindowData [WndHandle].WndLength) && (RPosPixelY<=WindowData [WndHandle].WndHeight))
      {
          WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY)=Color;           // La scrittura in Ram avviene in coordinate relative. Ciò
                                                                             // consente di eseguire lo spostamento delle finestre senza
                                                                             // bisogno di eseguire copia di memoria
          if (RenderNow)
          {  
             PosPixelX=WindowData [WndHandle].WndLimitX1+RPosPixelX;
             PosPixelY=WindowData [WndHandle].WndLimitY1+RPosPixelY;
                                              
             if ((OwnerArray [PosPixelX][PosPixelY][0])==WndHandle)            // Verifica se il pixel è in primo piano e se così è
                ndLL_PutPixel (PosPixelX, PosPixelY, Color);                 // lo scrive direttamente se RenderNow=1
             
          }
          
          return 0;
      }
      else
          return ERR_PIXEL_IS_OUT_OF_WINDOW;
  }
  else
      return ERR_WINDOW_IS_NOT_ALLOCATED;

}


inline void ndLP_PutPixel_NORENDER_FAST (unsigned short int RPosPixelX, unsigned short int RPosPixelY, TypeColor Color, unsigned char WndHandle)
/*
     Questa funzione si occupa di inserire un pixel nella pagina logica di una data finestra. Le coordinate devono
     essere fornite con riferimento al pixel della finestra in alto a sinistra (contenute in WndLimitX1 e WndLimitY1),
     ossia in termini relativi e non assoluti.

     La funzione è progettata per essere richiamata dalla routine XWindowRender. Poichè questa routine richiama migliaia
     di volte al secondo questa routine, essa è progettata per essere la più veloce possibile.

     USANDO QUESTA ROUTINE DA UTENTE E' POSSIBILE PROVOCARE DANNI GRAVI AL SISTEMA GRAFICO, PER TALE
     RAGIONE ESSA NON VIENE ESPOSTA IN NANODESKTOP.H.

     E' stata progettata esclusivamente per uso interno.

     Non esegue controlli, e non restituisce codici di errore;

     Questa funzione si limita a scrivere nelle LogicalPage. Per visualizzare il pixel, è necessario eseguire un
     operazione di rendering completo dello schermo.
*/

{
          WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY)=Color;           // La scrittura in Ram avviene in coordinate relative. Ciò
                                                                             // consente di eseguire lo spostamento delle finestre senza
                                                                             // bisogno di eseguire copia di memoria


}



char ndLP_WriteChar (unsigned short int RPosPixelCharX, unsigned short int RPosPixelCharY, char Carattere, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, unsigned char RenderNow)
		// Questa funzione viene usata dalla routine ndLP_WriteString per scrivere dei caratteri
        // in una pagina logica relativa alla finestra WndHandle. In genere viene usata per scrivere nella
        // pagina logica il titolo. A livello logical page non è previsto un cursore (mentre sarà previsto
        // a livello WS), quindi la scrittura avviene passando al parametro la posizione della lettera
        // in termini di posizione pixel. La posizione pixel deve essere in termini relativi e non in
        // termini assoluti. A differenza di quanto avveniva per la WriteChar di tipo LOW-LEVEL,
        // qui è necessario fornire alla routine sia il colore del carattere che il colore di background

        // L'ultimo parametro dice alla routine se il carattere deve essere renderizzato immediatamente

        // La finestra riporta in uscita il codice di errore riportato da PutPixel

{
    unsigned char *TextFontData;		//puntatore al font

	unsigned char BitFilter, BitExtract;
	unsigned char ByteInChar;
	int IntX, IntY;
	
	char ErrRep;
	
	int  FntHandle;

	if (!ndInitSystemExecuted) ndInitSystem();

	if (Carattere!=0)
	{
		FntHandle = WindowData [WndHandle].ndLP_CurrentFntHandle;
        TextFontData=FontHandlesArray [FntHandle].Data+Carattere*8;	//Inizializza il puntatore alla corretta lettera nel buffer dei font

		ByteInChar=0;

		for (IntY=0; IntY<8; IntY++)
		{
			BitFilter=0x80;

			for (IntX=0; IntX<8; IntX++)
			{
				BitExtract=(*(TextFontData+ByteInChar)&BitFilter);

				if (BitExtract!=0)
                   ErrRep=ndLP_PutPixel (RPosPixelCharX+IntX, RPosPixelCharY+IntY, Color, WndHandle, RenderNow);
                else
                   ErrRep=ndLP_PutPixel (RPosPixelCharX+IntX, RPosPixelCharY+IntY, BGColor, WndHandle, RenderNow);

                if (ErrRep!=0) return ErrRep;

				BitFilter=BitFilter>>1;		//La maschera BitFilter è stata spostata di un bit a dx
			}

			ByteInChar++;

		}

    }

    return 0;
}

char ndLP_Write (unsigned short int RPosPixelX, unsigned short int RPosPixelY, char *str, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, unsigned char RenderNow)
/*
   Questa funzione permette di scrivere una stringa nella pagina logica della finestra definita da
   WndHandle. Poichè a livello LogicalPage non è previsto un cursore, la posizione della stringa deve
   essere passata per mezzo della coppia di parametri PosPixelX e PosPixelY. Anche in questo caso,
   le coordinate del pixel sono relative al primo pixel della finestra in alto a sinistra.

   L'ultimo parametro dice alla routine se il carattere deve essere renderizzato immediatamente

   La funzione riporta in uscita l'errore riportato dalla funzione ndLP_PutPixel.
*/

{
	int MaxLenStr=255;
	unsigned short int t, kx, ky;
	char *strpointer;			//Puntatore alla stringa

    char ErroreRiportato;

    if (!ndInitSystemExecuted) ndInitSystem();

	strpointer=str;
	t=0;

    ky=RPosPixelY;

	while (*(strpointer+t)!=0 && t<MaxLenStr)
	{
        kx=RPosPixelX+t*8;

		ErroreRiportato=ndLP_WriteChar (kx, ky, *(strpointer+t), Color, BGColor, WndHandle, RenderNow);
		if (ErroreRiportato!=0) return (ErroreRiportato);

       	t++;
	}
    return 0;

}





char ndLP_DrawRectangle (unsigned short int PosX1, unsigned short int PosY1, unsigned short int PosX2, unsigned short int PosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo nella pagina logica della finestra identificata da
     WndHandle. Le coordinate devono essere fornite in modo relativo, con riferimento al primo
     pixel in alto a sinistra della finestra

     L'ultimo parametro dice alla routine se il carattere deve essere renderizzato immediatamente

*/
  {
	int IntX, IntY;

	unsigned short int KDiffX, KDiffY;
    char ErroreRiportato;

    if (!ndInitSystemExecuted) ndInitSystem();
	
	if (PosX1>PosX2) UShort_Swap (&PosX1, &PosX2);		
	if (PosY1>PosY2) UShort_Swap (&PosY1, &PosY2);
		
	KDiffX=PosX2-PosX1;
    KDiffY=PosY2-PosY1;
	
	
	//Disegniamo prima l'interno del rettangolo

	for (IntY=0; IntY<=KDiffY; IntY++)
	{
			for (IntX=0; IntX<=KDiffX; IntX++)
			{
					ErroreRiportato=ndLP_PutPixel (IntX+PosX1, IntY+PosY1, Color, WndHandle, RenderNow);
				    if (ErroreRiportato!=0) return ErroreRiportato;
            }
	}

	//Ora disegniamo il border del rettangolo: lato superiore
	//ed inferiore


	for (IntX=0; IntX<=KDiffX; IntX++)
	{
			ErroreRiportato=ndLP_PutPixel (IntX+PosX1, PosY1, BorderColor, WndHandle, RenderNow);
			if (ErroreRiportato!=0) return ErroreRiportato;

            ErroreRiportato=ndLP_PutPixel (IntX+PosX1, PosY2, BorderColor, WndHandle, RenderNow);
		    if (ErroreRiportato!=0) return ErroreRiportato;
    }

    //Ora disegniamo il border del rettangolo: lato destro e
	//e sinistro


	for (IntY=0; IntY<=KDiffY; IntY++)
	{

			ErroreRiportato=ndLP_PutPixel (PosX1, IntY+PosY1, BorderColor, WndHandle, RenderNow);
			if (ErroreRiportato!=0) return ErroreRiportato;

            ErroreRiportato=ndLP_PutPixel (PosX2, IntY+PosY1, BorderColor, WndHandle, RenderNow);
            if (ErroreRiportato!=0) return ErroreRiportato;
    }

   // Se tutto è andato a buon fine, restituiamo codice di errore 0

   return 0;
}



char ndLP_DrawRectangleVoid (unsigned short int PosX1, unsigned short int PosY1, unsigned short int PosX2, unsigned short int PosY2, TypeColor BorderColor, unsigned char WndHandle, unsigned short int RenderNow)
/*
     Questa funzione scrive un rettangolo nella pagina logica della finestra identificata da
     WndHandle. Le coordinate devono essere fornite in modo relativo, con riferimento al primo
     pixel in alto a sinistra della finestra

     L'ultimo parametro dice alla routine se il carattere deve essere renderizzato immediatamente

     A differenza della normale DrawRectangle, il rettangolo che viene creato qui è vuoto.
*/
   {
	int IntX, IntY;

	unsigned short int KDiffX, KDiffY;
    char ErroreRiportato;

    if (!ndInitSystemExecuted) ndInitSystem();	
	
    if (PosX1>PosX2) UShort_Swap (&PosX1, &PosX2);	
	if (PosY1>PosY2) UShort_Swap (&PosY1, &PosY2);
		
	KDiffX=PosX2-PosX1;
    KDiffY=PosY2-PosY1;
	
	//Disegniamo il border del rettangolo: lato superiore
	//ed inferiore

	for (IntX=0; IntX<=KDiffX; IntX++)
	{
			ErroreRiportato=ndLP_PutPixel (IntX+PosX1, PosY1, BorderColor, WndHandle, RenderNow);
			if (ErroreRiportato!=0) return ErroreRiportato;

            ErroreRiportato=ndLP_PutPixel (IntX+PosX1, PosY2, BorderColor, WndHandle, RenderNow);
		    if (ErroreRiportato!=0) return ErroreRiportato;
    }

    //Ora disegniamo il border del rettangolo: lato destro e
	//e sinistro


	for (IntY=0; IntY<=KDiffY; IntY++)
	{

			ErroreRiportato=ndLP_PutPixel (PosX1, IntY+PosY1, BorderColor, WndHandle, RenderNow);
			if (ErroreRiportato!=0) return ErroreRiportato;

            ErroreRiportato=ndLP_PutPixel (PosX2, IntY+PosY1, BorderColor, WndHandle, RenderNow);
            if (ErroreRiportato!=0) return ErroreRiportato;
    }

    // Se tutto è andato a buon fine, restituiamo codice di errore 0

    return 0;
}



// --------------------------------------------------------------------------
// 2.3 ROUTINE LP DI ALLOCAZIONE DELLE FINESTRE
// --------------------------------------------------------------------------


void Draw_CloseWindow_Button (unsigned short int RPosX, unsigned short int RPosY, unsigned char WndHandle)
/*
    Questa funzione disegna nella pagina logica di una finestra, il pulsante di chiusura
    finestra. Il pulsante viene disegnato a partire dal pixel di coordinate RPosX, RPosY
    (le coordinate di tale pixel sono riferite al primo pixel della finestra in alto a
    sinistra).

    Inoltre, la routine provvede ad inserire i dati necessari nel campo ButtonClass3_Data nel
    record relativo alla finestra su cui stiamo lavorando.
*/
{
   unsigned short int CounterX, CounterY;

   for (CounterY=0; CounterY<10; CounterY++)
   {
      for (CounterX=0; CounterX<10; CounterX++)
      {
          if (Button_CloseWindow [CounterY][CounterX] == 'B')
             ndLP_PutPixel (RPosX+CounterX, RPosY+CounterY, COLOR_BLACK, WndHandle, NORENDER);

          if (Button_CloseWindow [CounterY][CounterX] == 'R')
             ndLP_PutPixel (RPosX+CounterX, RPosY+CounterY, COLOR_RED, WndHandle, NORENDER);

          if (Button_CloseWindow [CounterY][CounterX] == 'W')
             ndLP_PutPixel (RPosX+CounterX, RPosY+CounterY, COLOR_WHITE, WndHandle, NORENDER);
         }
   }

   // Adesso attiviamo il pulsante nel buffer logico per i pulsanti di classe 3

   WindowData [WndHandle].ButtonClass3_Data [CLOSEWINDOW_BUTTON].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonClass3_Data [CLOSEWINDOW_BUTTON].ButtonID=CLOSEWINDOW_BUTTON;
   WindowData [WndHandle].ButtonClass3_Data [CLOSEWINDOW_BUTTON].x1=RPosX;
   WindowData [WndHandle].ButtonClass3_Data [CLOSEWINDOW_BUTTON].y1=RPosY;
   WindowData [WndHandle].ButtonClass3_Data [CLOSEWINDOW_BUTTON].x2=RPosX+10;
   WindowData [WndHandle].ButtonClass3_Data [CLOSEWINDOW_BUTTON].y2=RPosY+10;

}


void Draw_MinimizeWindow_Button (unsigned short int RPosX, unsigned short int RPosY, unsigned char WndHandle)
/*
    Questa funzione disegna nella pagina logica di una finestra, il pulsante di minimizzazione
    finestra. Il pulsante viene disegnato a partire dal pixel di coordinate RPosX, RPosY
    (le coordinate di tale pixel sono riferite al primo pixel della finestra in alto a
    sinistra).

    Inoltre, la routine provvede ad inserire i dati necessari nel campo ButtonClass3_Data nel
    record relativo alla finestra su cui stiamo lavorando.
*/
{
   unsigned short int CounterX, CounterY;

   for (CounterY=0; CounterY<10; CounterY++)
   {
      for (CounterX=0; CounterX<10; CounterX++)
      {
          if (Button_MinimizeWindow [CounterY][CounterX] == 'B')
             ndLP_PutPixel (RPosX+CounterX, RPosY+CounterY, COLOR_BLACK, WndHandle, NORENDER);

          if (Button_MinimizeWindow [CounterY][CounterX] == 'R')
             ndLP_PutPixel (RPosX+CounterX, RPosY+CounterY, COLOR_RED, WndHandle, NORENDER);

          if (Button_MinimizeWindow [CounterY][CounterX] == 'W')
             ndLP_PutPixel (RPosX+CounterX, RPosY+CounterY, COLOR_WHITE, WndHandle, NORENDER);

          if (Button_MinimizeWindow [CounterY][CounterX] == 'L')
             ndLP_PutPixel (RPosX+CounterX, RPosY+CounterY, COLOR_LLBLUE, WndHandle, NORENDER);

         }
   }

   // Adesso attiviamo il pulsante nel buffer logico per i pulsanti di classe 3

   WindowData [WndHandle].ButtonClass3_Data [MINIMIZEWINDOW_BUTTON].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonClass3_Data [MINIMIZEWINDOW_BUTTON].ButtonID=MINIMIZEWINDOW_BUTTON;
   WindowData [WndHandle].ButtonClass3_Data [MINIMIZEWINDOW_BUTTON].x1=RPosX;
   WindowData [WndHandle].ButtonClass3_Data [MINIMIZEWINDOW_BUTTON].y1=RPosY;
   WindowData [WndHandle].ButtonClass3_Data [MINIMIZEWINDOW_BUTTON].x2=RPosX+10;
   WindowData [WndHandle].ButtonClass3_Data [MINIMIZEWINDOW_BUTTON].y2=RPosY+10;

}


void Draw_Title_Button (unsigned short int RPosX1, unsigned short int RPosY1, unsigned short int RPosX2, unsigned short int RPosY2, unsigned char WndHandle)
/*
    Ogni finestra ha un pulsante che si trova sulla barra del titolo. Viene detto pseudopulsante,
    perchè non c'è nessun segno grafico sulla finestra che lo renda visibile.

    Questa routine si limita ad inserire i dati necessari nel buffer logico per i pulsanti di classe 3.

*/
{
   WindowData [WndHandle].ButtonClass3_Data [TITLEBAR_BUTTON].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonClass3_Data [TITLEBAR_BUTTON].ButtonID=TITLEBAR_BUTTON;
   WindowData [WndHandle].ButtonClass3_Data [TITLEBAR_BUTTON].x1=RPosX1;
   WindowData [WndHandle].ButtonClass3_Data [TITLEBAR_BUTTON].y1=RPosY1;
   WindowData [WndHandle].ButtonClass3_Data [TITLEBAR_BUTTON].x2=RPosX2;
   WindowData [WndHandle].ButtonClass3_Data [TITLEBAR_BUTTON].y2=RPosY2;
}


void Draw_ScrollToLeft_Button (unsigned short int RPosX1, unsigned short int RPosY1, unsigned short int RPosX2, unsigned short int RPosY2,
       TypeColor Color, TypeColor BGColor, unsigned short WndHandle)
/*
  Questa routine si occupa di creare il pulsante che si occupa dello scrolling a sinistra delle
  finestre. La routine si occupa anche di inserire i dati necessari nel buffer logico per i pulsanti di classe 3.
*/
{
   unsigned short int CounterX, CounterY;

   ndLP_DrawRectangleVoid (RPosX1, RPosY1, RPosX2, RPosY2, Color, WndHandle, NORENDER);

   for (CounterY=0; CounterY<8; CounterY++)
   {
      for (CounterX=0; CounterX<8; CounterX++)
      {
          if (Button_ScrollLeft [CounterY][CounterX] == 'x')
             ndLP_PutPixel (RPosX1+CounterX+1, RPosY1+CounterY+1, Color, WndHandle, NORENDER);
          else
             ndLP_PutPixel (RPosX1+CounterX+1, RPosY1+CounterY+1, BGColor, WndHandle, NORENDER);

      }
   }

   WindowData [WndHandle].ButtonClass3_Data [SCROLL_LEFT_BUTTON].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_LEFT_BUTTON].ButtonID=SCROLL_LEFT_BUTTON;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_LEFT_BUTTON].x1=RPosX1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_LEFT_BUTTON].y1=RPosY1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_LEFT_BUTTON].x2=RPosX2;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_LEFT_BUTTON].y2=RPosY2;

}



void Draw_ScrollToRight_Button (unsigned short int RPosX1, unsigned short int RPosY1, unsigned short int RPosX2, unsigned short int RPosY2,
       TypeColor Color, TypeColor BGColor, unsigned short WndHandle)
/*
  Questa routine si occupa di creare il pulsante che si occupa dello scrolling a destra delle
  finestre. La routine si occupa anche di inserire i dati necessari nel buffer logico per i pulsanti di classe 3.
*/
{
   unsigned short int CounterX, CounterY;

   ndLP_DrawRectangleVoid (RPosX1, RPosY1, RPosX2, RPosY2, Color, WndHandle, NORENDER);

   for (CounterY=0; CounterY<8; CounterY++)
   {
      for (CounterX=0; CounterX<8; CounterX++)
      {
          if (Button_ScrollRight [CounterY][CounterX] == 'x')
             ndLP_PutPixel (RPosX1+CounterX+1, RPosY1+CounterY+1, Color, WndHandle, NORENDER);
          else
             ndLP_PutPixel (RPosX1+CounterX+1, RPosY1+CounterY+1, BGColor, WndHandle, NORENDER);

      }
   }

   WindowData [WndHandle].ButtonClass3_Data [SCROLL_RIGHT_BUTTON].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_RIGHT_BUTTON].ButtonID=SCROLL_RIGHT_BUTTON;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_RIGHT_BUTTON].x1=RPosX1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_RIGHT_BUTTON].y1=RPosY1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_RIGHT_BUTTON].x2=RPosX2;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_RIGHT_BUTTON].y2=RPosY2;
}



void Draw_ScrollToUp_Button (unsigned short int RPosX1, unsigned short int RPosY1, unsigned short int RPosX2, unsigned short int RPosY2,
       TypeColor Color, TypeColor BGColor, unsigned short WndHandle)
/*
  Questa routine si occupa di creare il pulsante che si occupa dello scrolling verso l'alto delle
  finestre. La routine si occupa anche di inserire i dati necessari nel buffer logico per i pulsanti di classe 3.
*/
{
   unsigned short int CounterX, CounterY;

   ndLP_DrawRectangleVoid (RPosX1, RPosY1, RPosX2, RPosY2, Color, WndHandle, NORENDER);

   for (CounterY=0; CounterY<8; CounterY++)
   {
      for (CounterX=0; CounterX<8; CounterX++)
      {
          if (Button_ScrollUp [CounterY][CounterX] == 'x')
             ndLP_PutPixel (RPosX1+CounterX+1, RPosY1+CounterY+1, Color, WndHandle, NORENDER);
          else
             ndLP_PutPixel (RPosX1+CounterX+1, RPosY1+CounterY+1, BGColor, WndHandle, NORENDER);

      }
   }

   WindowData [WndHandle].ButtonClass3_Data [SCROLL_UP_BUTTON].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_UP_BUTTON].ButtonID=SCROLL_UP_BUTTON;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_UP_BUTTON].x1=RPosX1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_UP_BUTTON].y1=RPosY1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_UP_BUTTON].x2=RPosX2;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_UP_BUTTON].y2=RPosY2;


}



void Draw_ScrollToDown_Button (unsigned short int RPosX1, unsigned short int RPosY1, unsigned short int RPosX2, unsigned short int RPosY2,
       TypeColor Color, TypeColor BGColor, unsigned short WndHandle)
/*
  Questa routine si occupa di creare il pulsante che si occupa dello scrolling verso il basso delle
  finestre. La routine si occupa anche di inserire i dati necessari nel buffer logico per i pulsanti di classe 3.
*/
{
  unsigned short int CounterX, CounterY;

  ndLP_DrawRectangleVoid (RPosX1, RPosY1, RPosX2, RPosY2, Color, WndHandle, NORENDER);

  for (CounterY=0; CounterY<8; CounterY++)
   {
      for (CounterX=0; CounterX<8; CounterX++)
      {
          if (Button_ScrollDown [CounterY][CounterX] == 'x')
             ndLP_PutPixel (RPosX1+CounterX+1, RPosY1+CounterY+1, Color, WndHandle, NORENDER);
          else
             ndLP_PutPixel (RPosX1+CounterX+1, RPosY1+CounterY+1, BGColor, WndHandle, NORENDER);

      }
   }

   WindowData [WndHandle].ButtonClass3_Data [SCROLL_DOWN_BUTTON].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_DOWN_BUTTON].ButtonID=SCROLL_DOWN_BUTTON;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_DOWN_BUTTON].x1=RPosX1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_DOWN_BUTTON].y1=RPosY1;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_DOWN_BUTTON].x2=RPosX2;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_DOWN_BUTTON].y2=RPosY2;

}


void INTERNAL_NoOrizzontalScroll (unsigned short int WndHandle)
/* Usata per usi interni */
{
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_LEFT_BUTTON].ButtonIsActivated=0;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_RIGHT_BUTTON].ButtonIsActivated=0;
}

void INTERNAL_NoVerticalScroll (unsigned short int WndHandle)
/* Usata per usi interni */
{
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_UP_BUTTON].ButtonIsActivated=0;
   WindowData [WndHandle].ButtonClass3_Data [SCROLL_DOWN_BUTTON].ButtonIsActivated=0;
}




char ndLP_DrawScrollerRectangles (unsigned int WndHandle)
/*
   Questa routine ha il compito di rappresentare i rettangoli di scrolling sullo
   schermo. Per mezzo di tali rettangoli, è possibile conoscere con precisione
   la posizione della ROI.

   La routine provvede anche a disegnare, ove fosse necessario, i pulsanti
   per la gestione della barra di scrolling ed aggiorna i relativi campi
   nella pagina logica.

   E' previsto un codice di errore:
      WINDOW_IS_NOT_ALLOCATED    se la finestra non è assegnata
*/

{
      float RatioX, RatioY, RatioXM, RatioYM;
      float WndLengthX, ROILengthX, WndLengthY, ROILengthY, ROIx1, ROIx2, ROIy1, ROIy2;

      unsigned short int rX1, rX2, rY1, rY2;

      if (!ndInitSystemExecuted) ndInitSystem();

      if (WindowData [WndHandle].WindowIsActive)
      {
                     
        if (!(WindowData [WndHandle].NoResizeable))
        {        
           ndLP_DrawRectangleVoid (WindowData[WndHandle].AA2_X1, WindowData[WndHandle].AA2_Y1, WindowData[WndHandle].AA2_X2, WindowData[WndHandle].AA2_Y2,
                                      WindowData[WndHandle].ColorBorder, WndHandle, NORENDER);
           
           WndLengthX = WindowData [WndHandle].WSWndLength;
           ROILengthX = WindowData [WndHandle].ROI_LenX;

           RatioX = ROILengthX/WndLengthX;

           if (RatioX < 1)                                // La ROI visualizzata è più piccola sull'asse x della Window Space?
           {
              // Creiamo la barra di scrolling orizzontale
              ndLP_DrawRectangle (WindowData[WndHandle].OrizzBar_PosX1+10, WindowData[WndHandle].OrizzBar_PosY1, WindowData[WndHandle].OrizzBar_PosX2-10, WindowData[WndHandle].OrizzBar_PosY2,
                                     WindowData[WndHandle].ColorBGTitle, WindowData[WndHandle].ColorBorder, WndHandle, NORENDER);
              
              ROIx1 = WindowData [WndHandle].ROI_PosX1;
              ROIx2 = WindowData [WndHandle].ROI_PosX2;

              RatioXM = (ROILengthX - 28)/ROILengthX;
              // Se le coordinate ROIx1 e ROIx2 fossero semplicemente moltiplicate per RatioX, allora quando ROIx2=WndLengthX
              // il risultato sarebbe pari a ROILengthX. Ma questo valore **non** può essere passato ad una DrawRectangle
              // perchè la barra di scrolling è più piccola di 28 pixel (quattro pixel di margine a destra, quattro pixel di
              // margine a sinistra, 10 pixel per il pulsante per lo scroll a sinistra e 10 pixel per il pulsante per lo
              // scroll a destra). Pertanto è necessario un ulteriore fattore di compressione

              rX1 = (WindowData [WndHandle].OrizzBar_PosX1) + (ROIx1*RatioX*RatioXM) + 14;
              rX2 = (WindowData [WndHandle].OrizzBar_PosX1) + (ROIx2*RatioX*RatioXM) + 14;

              rY1 = WindowData [WndHandle].OrizzBar_PosY2 - 8;
              rY2 = WindowData [WndHandle].OrizzBar_PosY2 - 2;

              ndLP_DrawRectangle ( WindowData [WndHandle].OrizzBar_PosX1 + 14, WindowData [WndHandle].OrizzBar_PosY2 - 6, WindowData [WndHandle].OrizzBar_PosX2 - 16, WindowData [WndHandle].OrizzBar_PosY2 - 4,
                 WindowData [WndHandle].ColorBorder, WindowData [WndHandle].ColorBorder, WndHandle, NORENDER);

              ndLP_DrawRectangle (rX1, rY1, rX2, rY2, WindowData[WndHandle].ColorTitle, WindowData[WndHandle].ColorBorder, WndHandle, NORENDER);

              // Adesso creiamo i pulsanti di scroll di destra e di sinistra
              Draw_ScrollToLeft_Button (WindowData [WndHandle].OrizzBar_PosX1, WindowData [WndHandle].OrizzBar_PosY1, WindowData [WndHandle].OrizzBar_PosX1 + 10,  WindowData [WndHandle].OrizzBar_PosY2,
                                        WindowData [WndHandle].ColorBorder, WindowData [WndHandle].ColorBGWS, WndHandle);

              Draw_ScrollToRight_Button (WindowData [WndHandle].OrizzBar_PosX2 - 10, WindowData [WndHandle].OrizzBar_PosY1, WindowData [WndHandle].OrizzBar_PosX2, WindowData [WndHandle].OrizzBar_PosY2,
                                         WindowData [WndHandle].ColorBorder, WindowData [WndHandle].ColorBGWS, WndHandle);
           }
           else
               INTERNAL_NoOrizzontalScroll (WndHandle);


           WndLengthY = WindowData [WndHandle].WSWndHeight;
           ROILengthY = WindowData [WndHandle].ROI_LenY;

           RatioY = ROILengthY/WndLengthY;

           if (RatioY < 1)                                // La ROI visualizzata è più piccola sull'asse y della Window Space?
           {       
              // Creiamo la barra di scrolling verticale
              ndLP_DrawRectangle (WindowData[WndHandle].VertBar_PosX1, WindowData[WndHandle].VertBar_PosY1+10, WindowData[WndHandle].VertBar_PosX2, WindowData[WndHandle].VertBar_PosY2-10,
                                     WindowData[WndHandle].ColorBGTitle, WindowData[WndHandle].ColorBorder, WndHandle, NORENDER);

              ROIy1 = WindowData [WndHandle].ROI_PosY1;
              ROIy2 = WindowData [WndHandle].ROI_PosY2;

              RatioYM = (ROILengthY - 28)/ROILengthY;
              // Se le coordinate ROIy1 e ROIy2 fossero semplicemente moltiplicate per RatioY, allora quando ROIy2=WndLengthY
              // il risultato sarebbe pari a ROILengthY. Ma questo valore **non** può essere passato ad una DrawRectangle
              // perchè la barra di scrolling è più piccola di 28 pixel (quattro pixel di margine in alto, e quattro pixel di
              // margine in basso + 20 pixel per i pulsanti). Pertanto è necessario un ulteriore fattore di compressione

              rY1 = (WindowData [WndHandle].VertBar_PosY1) + (ROIy1*RatioY*RatioYM) + 14;
              rY2 = (WindowData [WndHandle].VertBar_PosY1) + (ROIy2*RatioY*RatioYM) + 14;

              rX1 = WindowData [WndHandle].VertBar_PosX2 - 8;
              rX2 = WindowData [WndHandle].VertBar_PosX2 - 2;

              ndLP_DrawRectangle ( WindowData [WndHandle].VertBar_PosX2 - 6, WindowData [WndHandle].VertBar_PosY1 + 14, WindowData [WndHandle].VertBar_PosX2 - 4, WindowData [WndHandle].VertBar_PosY2 - 16,
                                      WindowData [WndHandle].ColorBorder, WindowData [WndHandle].ColorBorder, WndHandle, NORENDER);

              ndLP_DrawRectangle (rX1, rY1, rX2, rY2, WindowData[WndHandle].ColorTitle, WindowData[WndHandle].ColorBorder, WndHandle, NORENDER);

              Draw_ScrollToUp_Button   (WindowData [WndHandle].VertBar_PosX2 - 10, WindowData [WndHandle].VertBar_PosY1, WindowData [WndHandle].VertBar_PosX2,  WindowData [WndHandle].VertBar_PosY1 + 10,
                                          WindowData [WndHandle].ColorBorder, WindowData [WndHandle].ColorBGWS, WndHandle);

              Draw_ScrollToDown_Button (WindowData [WndHandle].VertBar_PosX2 - 10, WindowData [WndHandle].VertBar_PosY2 - 10, WindowData [WndHandle].VertBar_PosX2,  WindowData [WndHandle].VertBar_PosY2,
                                          WindowData [WndHandle].ColorBorder, WindowData [WndHandle].ColorBGWS, WndHandle);
            }
            else
                INTERNAL_NoVerticalScroll (WndHandle);

         }
         else   // Finestra in modalità no-resizeable. Occorre comunque disegnare la cornice del window space
         {
           ndLP_DrawRectangleVoid (WindowData[WndHandle].AA2_X1, WindowData[WndHandle].AA2_Y1, WindowData[WndHandle].AA2_X2, WindowData[WndHandle].AA2_Y2,
                                      WindowData[WndHandle].ColorBorder, WndHandle, NORENDER);     
         }

         return 0;      
      }
	  else
	  	return ERR_WINDOW_IS_NOT_ALLOCATED;
}

inline unsigned short int ndLP_GetMinWndSizeX (char MenuSupported)
/*
    Questa routine restituisce la dimensione minima accettabile per una certa finestra.
    
    Non sono previsti codici di errore per questa routine.
*/
{
    if (!MenuSupported)
       return __MINWNDSIZEX_WITHOUT_MENU;
    else
       return __MINWNDSIZEX_WITH_MENU; 
}

inline unsigned short int ndLP_GetMinWndSizeY (char MenuSupported)
/*
    Questa routine restituisce la dimensione minima accettabile per una certa finestra.
    
    Non sono previsti codici di errore per questa routine.
*/
{
    if (!MenuSupported)
       return __MINWNDSIZEY_WITHOUT_MENU;
    else
       return __MINWNDSIZEY_WITH_MENU; 
}


/* ----------------------------------------------------------------------------
   Routines ndLP_ValidateNewSizeX e ndLP_ValidateNewSizeY
   
   Dei test eseguiti sul sistema suggeriscono che, quando una finestra viene
   allocata con AUTOFIX, senza usare il supporto menu, si ottiene:
            
   WndLength    120   WSWndLength   104
   WndHeight    120   WSWndHeight    93

   Partendo da queste considerazioni, è possibile scrivere che:
            
      WSWndLength = WndLength - 16
      WSWndHeight = WndHeight - 27
      
   I valori di WSWndLength e di WSWndHeight sono da considerarsi valori
   minimi: è comunque possibile settare un valore maggiore per
   WSWndLength e WSWndHeight e quindi si può scrivere:
               
     WSWndLength >= WndLength - 16
     WSWndHeight >= WndHeight - 27
     
   Queste relazioni possono essere riscritte come:
          
     WndLength <= WSWndLength + 16
     WndHeight <= WSWndHeight + 27
     
   che esprimono i valori limite superiore per WndLength e WndHeight. 
   
   Si è inoltre deciso, per ragioni tecniche, di imporre una dimensione
   minima, per le finestre che non supportano i menu, di 48x48 pixel.
   
   Le routines ndLP_ValidateNewSizeX (o Y) hanno un prototipo:
               
   ndLP_ValidateNewSizeX (int NewSizeX, int WSWndLength, char MenuSupported, int *NewSizeSuggested)
   
   La routine controlla che la NewSizeX (o Y) risponda alla disequazione:
              
     48 < NewSizeX < WSWndLength + 16
     
   se NewSizeX risponde a tale disequazione, il valore di NewSizeX viene riportato
   all'int indirizzato dal puntatore *NewSizeSuggested, e la routine in uscita
   restituisce 1. 
   
   Se la routine verifica che il valore di NewSizeX è troppo basso, allora restituisce
   0 e pone in *NewSizeSuggested il valore minimo consentito. 
   
   Se la routine verifica che il valore di NewSizeX è troppo alto, allora restituisce
   0 e pone in *NewSizeSuggested il valore massimo consentito. 
   
   Veniamo alle finestre che supportano i menù. In questo caso si è verificato che:
           
    WndLength    120    WSWndLength   104
    WndHeight    120    WSWndHeight    74
    
    Quindi, rifacendo il ragionamento precedente, è richiesto che siano rispettate le
    seguenti disequazioni:
             
    110 < NewSizeX < WSWndLength + 16
    110 < NewSizeY < WSWndHeight + 46
--------------------------------------------------------------------------------------
*/

char ndLP_ValidateNewSizeX (int NewSizeX, int WSWndLength, char MenuSupported, int *NewSizeSuggested)
{      
   if (!MenuSupported)
   {
       if ((NewSizeX>=__MINWNDSIZEX_WITHOUT_MENU) && (NewSizeX<=WSWndLength+16))
       {
           *NewSizeSuggested = NewSizeX;
           return 1;
       }
       else
       {
           if (NewSizeX<__MINWNDSIZEX_WITHOUT_MENU)
           {
               *NewSizeSuggested = __MINWNDSIZEX_WITHOUT_MENU;
               return 0;
           }
           
           if (NewSizeX>WSWndLength+16)
           {
               *NewSizeSuggested = WSWndLength+16;
               return 0;
           }
       }
   }
   else
   {
       if ((NewSizeX>=__MINWNDSIZEX_WITH_MENU) && (NewSizeX<=WSWndLength+16))
       {
           *NewSizeSuggested = NewSizeX;
           return 1;
       }
       else
       {
           if (NewSizeX<__MINWNDSIZEX_WITH_MENU )
           {
              *NewSizeSuggested = __MINWNDSIZEX_WITH_MENU;
              return 0;
           }
           
           if (NewSizeX>WSWndLength+16)
           {
              *NewSizeSuggested = WSWndLength+16;
              return 0;
           }
       }
   }
}


char ndLP_ValidateNewSizeY (int NewSizeY, int WSWndHeight, char MenuSupported, int *NewSizeSuggested)
{
   if (!MenuSupported)
   {
       if ((NewSizeY>=__MINWNDSIZEY_WITHOUT_MENU) && (NewSizeY<=WSWndHeight+27))
       {
           *NewSizeSuggested = NewSizeY;
           return 1;
       }
       else
       {
           if (NewSizeY<__MINWNDSIZEY_WITHOUT_MENU)
           {
               *NewSizeSuggested = __MINWNDSIZEY_WITHOUT_MENU;
               return 0;
           }
           
           if (NewSizeY>WSWndHeight+27)
           {
               *NewSizeSuggested = WSWndHeight+27;
               return 0;
           }
       }
   }
   else
   {
       if ((NewSizeY>=__MINWNDSIZEY_WITH_MENU) && (NewSizeY<=WSWndHeight+46))
       {
           *NewSizeSuggested = NewSizeY;
           return 1;
       }
       else
       {
           if (NewSizeY<__MINWNDSIZEY_WITH_MENU)
           {
              *NewSizeSuggested = __MINWNDSIZEY_WITH_MENU;
              return 0;
           }
           
           if (NewSizeY>WSWndHeight+46)
           {
              *NewSizeSuggested = WSWndHeight+46;
              return 0;
           }
       }
   }
}



char ndLP_CreateWindow               (unsigned short int _PosWindowX1,
                                      unsigned short int _PosWindowY1,
                                      unsigned short int _PosWindowX2,
                                      unsigned short int _PosWindowY2,
                                      char *_WindowTitle,
                                      TypeColor _ColorTitle,
                                      TypeColor _ColorBGTitle,
                                      TypeColor _ColorWindow,
                                      TypeColor _ColorBorderWindow,
                                      ndint64 Attribute)
/*
  Questa routine si occupa di allocare una finestra. La routine deve prima
  verificare qual è la finestra che in quel momento è libera, poi deve
  reinizializzarla, ed infine deve fornire alla routine chiamante un
  handle per la finestra.
  
  Il parametro Attribute, è un parametro a 64 bit che fornisce informazioni
  sul comportamento della finestra. I seguenti valori rappresentano bit del
  campo che modificano il comportamento della finestra
  
  NrBit
  
  0         AUTOFIX           Le dimensioni del window space sono calcolate 
                              automaticamente
  1         NO_RESIZEABLE     La finestra non può essere ridimensionata
  2         MENUSUPPORTED     I menù di finestra sono supportati
  3         USE_POSGEN        Usa il generatore PosGen
  
  15-8                        Contiene il font che sarà usato per i menù. 
                              L'utente può inserire questo valore per mezzo
                              della key NDKEY_SETMENUFONT (x). Se 
                              l'utente non specifica nulla, sarà usato il
                              font di default
                              
  23-16                       Contiene il font che sarà usato per il titolo
                              L'utente può inserire questo valore per mezzo
                              della key NDKEY_SETTITLEFONT (x). Se
                              l'utente non specifica nulla, sarà usato il
                              font di default
                              
  31-24                       Contiene il font che sarà usato per il testo in
                              WindowSpace. L'utente può inserire questo valore
                              per mezzo della key NDKEY_SETFONT (x) oppure della
                              key NDKEY_FONT(x). Se l'utente non specifica nulla,
                              sarà usato il font di default
                              
  63-51                       Contiene il parametro _WSLength (da 0 a 2048), che
                              specifica la lunghezza del WindowSpace
                              L'utente può impostare questo valore per mezzo
                              della key NDKEY_SETWSLENGTH (x)
                              
  51-39                       Contiene il parametro _WSHeight (da 0 a 2048) che 
                              specifica la larghezza del WindowSpace.
                              L'utente può impostare questo valore per mezzo
                              della key NDKEY_SETWSHEIGHT (x)

  Se il flag AUTOFIX è settato, ovvero se almeno uno dei parametri NDKEY_SETWSLENGTH
  o NDKEY_SETWSHEIGHT non viene specificato, i parametri _WSLength e _WSHeight
  vengono ignorati e le dimensioni del window space saranno settate 
  automaticamente alle dimensioni della finestra sullo schermo. Tuttavia,
  la finestra potrà essere comunque ridimensionata tramite GUI o tramite
  una chiamata a ndLP_ResizeWindow.
  
  Se il flag NO_RESIZEABLE è settato, non solo le dimensioni del window
  space saranno determinate automaticamente, ma sarà anche impossibile
  ridimensionare in seguito la finestra. 
  
  Se il flag USE_POSGEN è attivato, la routine determinerà le quantità
  _LenX = PosWindowX2 - PosWindowX1
  _LenY = PosWindowY2 - PosWindowY1
  
  e dopo si farà fornire le coordinate necessarie dal generatore POSGEN.

  Quando la routine restituisce un valore negativo, allora siamo in presenza
  di un errore. I codici di errore sono:

        ERR_NO_WINDOW_IS_AVAILABLE         Non ci sono più finestre disponibili
        ERR_WINDOW_IS_TOO_SMALL            Finestra troppo piccola
        ERR_WS_SPACE_REQUEST_IS_TOO_LARGE  L'estensione della Window Space richiesta è
                                           troppo grande
        ERR_WINDOW_OVER_THE_FUNC_AREA      La finestra coprirebbe l'area funzionale
        ERR_MEMORY_OVERFLOW                Overflow della memoria

  Questa routine pone la finestra in primo piano, ma minimizzata. E' necessaria
  la routine ndLP_MaximizeWindow per mostrare la finestra.

  Qualora l'utente richiesta una window space "più piccola" dell'area ROI della
  finestra, la routine adatta automaticamente la richiesta per la WindowSpace
  al valore della ROI della finestra, non essendo consentito di lavorare con
  WindowSpace che siano più piccole dell'area ROI della finestra

*/

{
  ndint64 Temp;
  int Counter, CounterX, CounterY, xA;

  unsigned short int LunghezzaTitolo,
                     PosTitoloX,
                     PosTitoloY;
                     
  unsigned short int ThWndLength, ThWndHeight;
  unsigned short int _WSHeight, _WSLength;
  
  char _WSFntHandle, _LPFntHandle, _MBFntHandle;
  char NoResizeable, AutoFix, MenuSupported;
  
  
 // Inizio del codice

 if (!ndInitSystemExecuted) ndInitSystem();
 
 // Determinazione di tutti i parametri necessari estraendoli da attribute:
 // _WSLength and _WSHeight
 
 Temp = Attribute;
 Temp = (Temp >> (63-11)) & 2047;
 _WSLength = (unsigned short int)(Temp);
 
 Temp = Attribute;
 Temp = (Temp >> (63-11-11)) & 2047;
 _WSHeight = (unsigned short int)(Temp);
 
 // Determiniamo i font richiesti
 
 Temp = Attribute;
 Temp = (Temp >> (31-8)) & 0xFF;
 _WSFntHandle = (char)(Temp);
 
 Temp = Attribute;
 Temp = (Temp >> (23-8)) & 0xFF;
 _LPFntHandle = (char)(Temp);
 
 Temp = Attribute;
 Temp = (Temp >> (15-8)) & 0xFF;
 _MBFntHandle = (char)(Temp);
 
 // Setta ai valori di default i font se l'utente non li ha specificati
 
 if (_WSFntHandle==0) _WSFntHandle=_FntHandle_Console;
 if (_LPFntHandle==0) _LPFntHandle=_FntHandle_Console;
 if (_MBFntHandle==0) _MBFntHandle=_FntHandle_Console;
 
 // Estrai i bit NO_RESIZEABLE e MENU_SUPPORTED
 
 if ((Attribute & NO_RESIZEABLE) == NO_RESIZEABLE)  NoResizeable=1;
    else NoResizeable=0;
    
 if ((Attribute & MENUSUPPORTED) == MENUSUPPORTED) MenuSupported=1;
    else MenuSupported=0;
    
 // Setta al valore corretto la variabile AutoFix   
    
 AutoFix=0;
 
 if ((Attribute & AUTOFIX) == AUTOFIX) AutoFix=1;
 if ((_WSLength==0) || (_WSHeight==0)) AutoFix=1;   
    
 // Inizio del codice   
     
 if ((_WSLength >= MAXDIM_WSLENGTH) || (_WSHeight >= MAXDIM_WSHEIGHT)) 
    return ERR_WS_SPACE_REQUEST_IS_TOO_LARGE;

 if (NrWindowsAllocated==__NRWINDOWSALLOCABLE)
    return (ERR_NO_WINDOW_IS_AVAILABLE);                // Se già sono allocate 8 pagine restituisci un errore
 
 ThWndLength = abs (_PosWindowX2 - _PosWindowX1);
 ThWndHeight = abs (_PosWindowY2 - _PosWindowY1);
 
 if (   (   ThWndLength>=ndLP_GetMinWndSizeX (MenuSupported)   ) ||
        (   ThWndHeight>=ndLP_GetMinWndSizeY (MenuSupported)   )     )
 {
   if (  (_PosWindowX1<Windows_MaxScreenX) && (_PosWindowX2<Windows_MaxScreenX) &&
      (_PosWindowY1<Windows_MaxScreenY) && (_PosWindowY2<Windows_MaxScreenY)  )
   {
      Counter=0;
      NrWindowsAllocated++;

      while (WindowData [Counter].WindowIsActive != 0) Counter++;
                   
      xA=Counter;     // In xA, al termine del processo, ci sarà la prima finestra libera

      WindowData [xA].WindowTitle=_WindowTitle;          // Qui viene memorizzato l'indirizzo del titolo finestra (che è ciò che viene
      WindowData [xA].ColorTitle=_ColorTitle;            // passato alla funzione tramite stack
      WindowData [xA].ColorBGTitle=_ColorBGTitle;

      WindowData [xA].ColorBorder=_ColorBorderWindow;
      WindowData [xA].ColorBGWS=_ColorWindow;
      
      if ((Attribute & USE_POSGEN)==USE_POSGEN)
      {
          ndPOSGEN_GenPosition (ThWndLength, ThWndHeight, &_PosWindowX1, &_PosWindowY1, &_PosWindowX2, &_PosWindowY2);
      }

      WindowData [xA].WndLimitX1=_PosWindowX1;
      WindowData [xA].WndLimitY1=_PosWindowY1;
      WindowData [xA].WndLimitX2=_PosWindowX2;
      WindowData [xA].WndLimitY2=_PosWindowY2;
                   
      WindowData [xA].WndLength=ThWndLength;
      WindowData [xA].WndHeight=ThWndHeight;
      
      // Gli acceleratori grafici di alcune piattaforme hanno un'unità che pretende che i rettangoli
      // copiati dal blitter siano con dimensioni multiple di un certo valore. Tale valore viene 
      // impostato da un costante definita nel codice del HAL, e detta NDHAL_VIDEOACCELERATOR_ALLIGNMENT
      // Per usare l'accelerazione video 2d, Nanodesktop deve impostare delle dimensioni virtuali delle
      // LogicalPage. Le logical page avranno una dimensione maggiore di quanto necessario, per
      // rispettare le richieste degli acceleratori grafici.
      
      WindowData [xA].HwWndLength = INTERNAL_AllignValue (WindowData [xA].WndLength+1, 16);
      WindowData [xA].HwWndHeight = INTERNAL_AllignValue (WindowData [xA].WndHeight+1, 1);
      
      WindowData [xA]._p = malloc  ( (WindowData[xA].HwWndLength) * (WindowData[xA].HwWndHeight) * sizeof (TypeColor) );
      // Alloca lo spazio necessario per la pagina logica. Le routine considerano il pixel di coordinate
      // WndLength e WndHeight come facenti parte della finestra, quindi bisogna modificare di 1 la richiesta
      // di memoria su entrambe le direzioni
                   
      if ( WindowData [xA]._p > 0)   // Il processo di allocazione della pagina logica è andato a buon fine 
      {
         // Provvediamo a reinizializzare la pagina logica
                      
         for (CounterY=0; CounterY<=WindowData [xA].WndHeight; CounterY++)      
         {
            for (CounterX=0; CounterX<=WindowData [xA].WndLength; CounterX++)
            {
               WINDOWDATA_P (xA, CounterX, CounterY)=0;
            }
         }
         
         // Eseguiamo dei calcoli per determinare le dimensioni del Window-Space
         
         // L'area attiva di livello nr. 1 ha due dimensioni diverse a seconda che MenuSupported sia
         // attivo o meno. 
         // Quando MenuSupported non è attivato, l'area attiva di livello 1 è il rettangolo che 
         // inizia subito sotto la barra del titolo, termina due pixel prima del limite LP 
         // della finestra, mentre orizzontalmente inizia due pixel dopo il limite
         // LP sinistro della finestra e finisce due pixel prima del limite LP destro della finestra.
         // Quando MenuSupported è attivato, l'area attiva di livello 1 inizia dopo la menubar
         
         if (!MenuSupported)
         {                
            WindowData[xA].MenuSupported = 0;
            
            WindowData[xA].MenuBar_PosX1 = 0;
            WindowData[xA].MenuBar_PosY1 = 0;
            WindowData[xA].MenuBar_PosX2 = 0;
            WindowData[xA].MenuBar_PosY2 = 0;
            
            WindowData[xA].MenuBar_LenX  = 0;
            WindowData[xA].MenuBar_LenY  = 0;
            
            WindowData[xA].AA1_X1=2;                        
            WindowData[xA].AA1_Y1=13;
            WindowData[xA].AA1_X2=WindowData[xA].WndLength-2;
            WindowData[xA].AA1_Y2=WindowData[xA].WndHeight-2;

            WindowData[xA].AA1_LenX=WindowData[xA].AA1_X2-WindowData[xA].AA1_X1;  // e le relative dimensioni
            WindowData[xA].AA1_LenY=WindowData[xA].AA1_Y2-WindowData[xA].AA1_Y1;
           
         }
         else
         {      
            WindowData[xA].MenuSupported = 1;
            
            WindowData[xA].MenuBar_PosX1 = 2;
            WindowData[xA].MenuBar_PosY1 = 14;
            WindowData[xA].MenuBar_PosX2 = WindowData[xA].WndLength - 2;
            WindowData[xA].MenuBar_PosY2 = 30;
            
            WindowData[xA].MenuBar_LenX  = WindowData[xA].MenuBar_PosX2 - WindowData[xA].MenuBar_PosX1;
            WindowData[xA].MenuBar_LenY  = WindowData[xA].MenuBar_PosY2 - WindowData[xA].MenuBar_PosY1;
            
            WindowData[xA].MenuBar_LA_PosX1 = WindowData[xA].MenuBar_PosX1 + 34;
            WindowData[xA].MenuBar_LA_PosY1 = WindowData[xA].MenuBar_PosY1;
            WindowData[xA].MenuBar_LA_PosX2 = WindowData[xA].MenuBar_PosX2 - 17;
            WindowData[xA].MenuBar_LA_PosY2 = WindowData[xA].MenuBar_PosY2;
            
            WindowData[xA].MenuBar_LA_LenX  = WindowData[xA].MenuBar_LA_PosX2 - WindowData[xA].MenuBar_LA_PosX1;
            WindowData[xA].MenuBar_LA_LenY  = WindowData[xA].MenuBar_LA_PosY2 - WindowData[xA].MenuBar_LA_PosY1;
            
            // Reinizializza MenuBarIsChanged
            WindowData[xA].MenuBarIsChanged=0;
            
            // Reinizializza ZString
            strcpy (WindowData [xA].ZString, "");
            
            // Il primo elemento da visualizzare è il nr. 0
            WindowData[xA].NrFirstL1WMIButton=0;
            
            // Al momento dell'allocazione della finestra, non ci sono menù di secondo livello aperti
            WindowData[xA].OpenL2WMIMenu=0;
            WindowData[xA].NrL1WMIButtonOpened=0;
            
            // Reinizializza AA1
            WindowData[xA].AA1_X1=2;                        
            WindowData[xA].AA1_Y1=32;
            WindowData[xA].AA1_X2=WindowData[xA].WndLength-2;
            WindowData[xA].AA1_Y2=WindowData[xA].WndHeight-2;

            WindowData[xA].AA1_LenX=WindowData[xA].AA1_X2-WindowData[xA].AA1_X1;  // e le relative dimensioni
            WindowData[xA].AA1_LenY=WindowData[xA].AA1_Y2-WindowData[xA].AA1_Y1;
            
            // Reinizializza la lista di voci del menù
            WindowData[xA].MainWinMenu.FirstElementAddr=0;
            WindowData[xA].MainWinMenu.LastElementAddr=0;
            WindowData[xA].MainWinMenu.NrElements=0;
            WindowData[xA].MainWinMenu.Dynamic=0;
         }
         
         if (!NoResizeable)
         {
            WindowData[xA].NoResizeable=0;
         
            // Coordinate della barra di scrolling orizzontale
            WindowData [xA].OrizzBar_PosX1 = WindowData [xA].AA1_X1;
            WindowData [xA].OrizzBar_PosX2 = WindowData [xA].AA1_X2-10;
            WindowData [xA].OrizzBar_PosY1 = WindowData [xA].AA1_Y2-10;
            WindowData [xA].OrizzBar_PosY2 = WindowData [xA].AA1_Y2;
     
            // Coordinate della barra di scrolling verticale
            WindowData [xA].VertBar_PosX1 = WindowData [xA].AA1_X2-10;
            WindowData [xA].VertBar_PosX2 = WindowData [xA].AA1_X2;
            WindowData [xA].VertBar_PosY1 = WindowData [xA].AA1_Y1;
            WindowData [xA].VertBar_PosY2 = WindowData [xA].AA1_Y2-10;
            
            // L'area attiva di livello nr. 2 è il rettangolo compreso tra le barre di scrolling della ROI.

            WindowData[xA].AA2_X1=WindowData[xA].AA1_X1;
            WindowData[xA].AA2_Y1=WindowData[xA].AA1_Y1;
            WindowData[xA].AA2_X2=WindowData[xA].AA1_X2-10;
            WindowData[xA].AA2_Y2=WindowData[xA].AA1_Y2-10;

            WindowData[xA].AA2_LenX=WindowData[xA].AA2_X2-WindowData[xA].AA2_X1;
            WindowData[xA].AA2_LenY=WindowData[xA].AA2_Y2-WindowData[xA].AA2_Y1;
         }
         else      // Attivata la funzionalità No-Resizeable
         {
            WindowData[xA].NoResizeable=1;
         
            // In questo caso, le coordinate della barra di scrolling non hanno significato
            WindowData [xA].OrizzBar_PosX1 = 0;
            WindowData [xA].OrizzBar_PosX2 = 0;
            WindowData [xA].OrizzBar_PosY1 = 0;
            WindowData [xA].OrizzBar_PosY2 = 0;
            WindowData [xA].VertBar_PosX1  = 0;
            WindowData [xA].VertBar_PosX2  = 0;
            WindowData [xA].VertBar_PosY1  = 0;
            WindowData [xA].VertBar_PosY2  = 0;
            
            // L'area attiva di livello nr. 2 coincide con l'area attiva di livello 1.

            WindowData[xA].AA2_X1=WindowData[xA].AA1_X1;
            WindowData[xA].AA2_Y1=WindowData[xA].AA1_Y1;
            WindowData[xA].AA2_X2=WindowData[xA].AA1_X2;
            WindowData[xA].AA2_Y2=WindowData[xA].AA1_Y2;
         }   

         WindowData[xA].AA2_LenX=WindowData[xA].AA2_X2-WindowData[xA].AA2_X1;
         WindowData[xA].AA2_LenY=WindowData[xA].AA2_Y2-WindowData[xA].AA2_Y1;       
             
         // L'area attiva di livello nr. 3 è il rettangolo dove può avvenire effettivamente la visualizzazione
         // delle immagini e dei caratteri

         WindowData [xA].AA3_X1=WindowData[xA].AA2_X1+1;
         WindowData [xA].AA3_Y1=WindowData[xA].AA2_Y1+1;
         WindowData [xA].AA3_X2=WindowData[xA].AA2_X2-1;
         WindowData [xA].AA3_Y2=WindowData[xA].AA2_Y2-1;

         WindowData[xA].AA3_LenX=WindowData[xA].AA3_X2-WindowData[xA].AA3_X1;
         WindowData[xA].AA3_LenY=WindowData[xA].AA3_Y2-WindowData[xA].AA3_Y1;

         // ATTENZIONE: Le variabili di tipo AAx esprimono la posizione di pixel con riferimento al
         // primo pixel in alto a sinistra della finestra. Infatti, il primo pixel dell'area attiva di
         // livello 1 ha coordinate (2, 13) proprio perchè si trova due pixel a destra e 13 pixel in
         // basso rispetto al primo pixel della finestra.

         // La posizione della ROI all'inizio è sempre posta al massimo valore in alto a sinistra:
         
         WindowData [xA].ROI_PosX=0;
         WindowData [xA].ROI_PosY=0;

         WindowData [xA].ROI_LenX = WindowData [xA].AA3_LenX;
         WindowData [xA].ROI_LenY = WindowData [xA].AA3_LenY;
         
         // Esegui il calcolo delle coordinate della ROI
         WindowData [xA].ROI_PosX1 = WindowData [xA].ROI_PosX;
         WindowData [xA].ROI_PosY1 = WindowData [xA].ROI_PosY;
         WindowData [xA].ROI_PosX2 = WindowData [xA].ROI_PosX + WindowData [xA].ROI_LenX;
         WindowData [xA].ROI_PosY2 = WindowData [xA].ROI_PosY + WindowData [xA].ROI_LenY;

         // I pixel della Window space hanno coordinate che sono riferite al primo punto della
         // Window Space (NON, come le coordinate AAx, al primo punto della finestra).

         // Di conseguenza, se la Window Space fosse un rettangolo avrebbe coordinate
         // (0, 0, WSWndLength, WSWndHeight). Sono necessari sono due parametri per descrivere le
         // dimensioni della Window Space

         if ( (!NoResizeable) && (!AutoFix) )
         {
            if (_WSLength >= WindowData [xA].ROI_LenX)
               WindowData [xA].WSWndLength=_WSLength;
            else
               WindowData [xA].WSWndLength=WindowData [xA].ROI_LenX;

            if (_WSHeight >= WindowData [xA].ROI_LenY)
               WindowData [xA].WSWndHeight=_WSHeight;
            else
               WindowData [xA].WSWndHeight=WindowData [xA].ROI_LenY;
         }
         else     // Modalità non resizeable o autofix
         {
            WindowData [xA].WSWndLength = WindowData [xA].ROI_LenX;
            WindowData [xA].WSWndHeight = WindowData [xA].ROI_LenY;
         }

         // Provvediamo ad allocare lo spazio relativo al window space
         // N.B. Per il WindowSpace non è necessario allocare 1 elemento in più su entrambe le dimensioni
         // perchè le routine WS non considerano il pixel di coordinate WindowData [xA].WSWndLength,
         // WindowData [xA].WSWndHeight come facente parte del Window Space
         
         WindowData [xA]._ws=malloc  ( WindowData [xA].WSWndLength * WindowData [xA].WSWndHeight * sizeof (TypeColor) );
         
         if (WindowData [xA]._ws > 0)   // Il processo di allocazione del window space è andato a buon fine
         {           
             // Provvediamo a reinizializzare il Window Space
                      
             for (CounterY=0; CounterY<WindowData [xA].WSWndHeight; CounterY++)      
             {
                 for (CounterX=0; CounterX<WindowData [xA].WSWndLength; CounterX++)
                 {
                     WINDOWDATA_WS (xA, CounterX, CounterY)=_ColorWindow;
                 }
             }
             
             // Calcoliamo il numero di caratteri contenibili

             WindowData [xA].MargineCharX = ((WindowData [xA].WSWndLength) % 8)/2;
             WindowData [xA].MargineCharY = ((WindowData [xA].WSWndHeight) % 8)/2;
             
             // Ricalcoliamo le coordinate della char-area
             WindowData [xA].CharArea_PosX1 = WindowData [xA].MargineCharX;
             WindowData [xA].CharArea_PosY1 = WindowData [xA].MargineCharY;
             WindowData [xA].CharArea_PosX2 = WindowData [xA].WSWndLength - WindowData[xA].MargineCharX - 1;
             WindowData [xA].CharArea_PosY2 = WindowData [xA].WSWndHeight - WindowData[xA].MargineCharY - 1;
             // Supponiamo che il margine sia 0. CharArea_PosX2 e CharArea_PosY2 devono avere valore pari
             // a WSWndLength-1 e WSWndHeight-1.

             WindowData [xA].WS_MaxCursX = WindowData [xA].WSWndLength/8;
             WindowData [xA].WS_MaxCursY = WindowData [xA].WSWndHeight/8;

             // Adesso tentiamo di allocare la mappa caratteri
             
             WindowData [xA]._CharMap =malloc  (WindowData [xA].WS_MaxCursX * WindowData [xA].WS_MaxCursY * sizeof (unsigned char));
             
             if (WindowData [xA]._CharMap>0)       // Il processo di allocazione della char map è riuscito
             {    
                    WindowData [xA]._ColorCharMap = malloc  (WindowData [xA].WS_MaxCursX * WindowData [xA].WS_MaxCursY * sizeof (TypeColor));
                    
                    if (WindowData [xA]._ColorCharMap>0)    // Il processo di allocazione della color char map è riuscito
                    {
                           for (CounterY=0; CounterY<WindowData [xA].WS_MaxCursY; CounterY++)          // E' ora viene ripulita la memoria caratteri
                           {
                                  for (CounterX=0; CounterX<WindowData [xA].WS_MaxCursX; CounterX++)
                                  {
                                      WINDOWDATA_CHARMAP (xA, CounterX, CounterY)='\x00';
                                      WINDOWDATA_COLORCHARMAP (xA, CounterX, CounterY)=0;
                                  }
                           }
                                 
                           if (MenuSupported) 
                           {                
                              WindowData [xA].SS_Supported = 1;
                              WindowData [xA].SS_Enabled   = 1;
                              WindowData [xA].SS_LenX      = WindowData [xA].ROI_LenX;
                              WindowData [xA].SS_LenY      = WindowData [xA].ROI_LenY;
                           
                              WindowData [xA]._ss = malloc  (WindowData [xA].SS_LenX * WindowData [xA].SS_LenY * sizeof (TypeColor) );
                           
                              // Eseguiamo la reinizializzazione del service space
                              
                              if (WindowData [xA]._ss > 0)
                              {
                                 for (CounterY=0; CounterY<WindowData [xA].SS_LenY; CounterY++)
                                 {
                                     for (CounterX=0; CounterX<WindowData [xA].SS_LenX; CounterX++)
                                     {
                                         WINDOWDATA_SS (xA, CounterX, CounterY)=0;
                                     }
                                 }
                              }
                              
                              WindowData [xA].PrevOpenL2WMIMenu=0;
                              // Necessario per il funzionamento corretto del renderer WMIL2
                           }
                           else
                           {
                              WindowData [xA].SS_Supported = 0;
                              WindowData [xA].SS_Enabled   = 0;
                              WindowData [xA].SS_LenX      = 0;
                              WindowData [xA].SS_LenY      = 0;
                              
                              WindowData [xA]._ss=0; 
                           }
                           
                           if ( (!MenuSupported) || (WindowData [xA]._ss>0) )
                           {
                              WindowData [xA].NrTrackBarsAllocated = 0;
                              WindowData [xA].NrTextBoxAllocated = 0;
                              WindowData [xA].NrListBoxAllocated = 0;
                              WindowData [xA].NrButtonsAllocated = 0;
                              
                              // Reinizializziamo posizione cursore                              
                              WindowData [xA].WS_PosCursX=0;
                              WindowData [xA].WS_PosCursY=0;
                              
                              // Settiamo i parametri font a quelli richiesti dall'utente
                              WindowData [xA].ndLP_CurrentFntHandle=_LPFntHandle;
                              WindowData [xA].ndWS_CurrentFntHandle=_WSFntHandle;
                              WindowData [xA].ndMB_CurrentFntHandle=_MBFntHandle;
                              
                              // Segnala che la finestra è attivata
                              ndHAL_LockPhoenixGraphThread ();
                               
                              { 
                                 WindowData [xA].WindowIsActive=1;                         // All'inizio la finestra viene considerata attiva
                                 WindowData [xA].Minimize=1;                               // ma minimizzata (perchè non compare ancora su schermo)
                              }
                              
                              ndHAL_UnlockPhoenixGraphThread ();

                              // La nuova finestra viene posta come prima finestra nel Windows Vector. Non comparirà subito perchè è minimizzata
                              ndLP_ShootElementInTheWV (xA);

                              // Le seguenti righe di codice costruiscono la finestra nella relativa pagina logica.
                              ndLP_DrawRectangle (0, 0, WindowData[xA].WndLength, WindowData[xA].WndHeight, _ColorWindow, _ColorBorderWindow, xA, NORENDER);
                              ndLP_DrawRectangle (0, 0, WindowData[xA].WndLength, 12, _ColorBGTitle, _ColorBorderWindow, xA, NORENDER);

                              // Provvediamo al salvataggio del titolo nel buffer interno
                              LunghezzaTitolo=strlen (_WindowTitle);

                              if (LunghezzaTitolo>58) LunghezzaTitolo=58;
                              // In caso di caratteri in più essi vengono semplicemente troncati

                              for (CounterX=0; CounterX<LunghezzaTitolo; CounterX++)
                                  WindowData [xA].WindowTitleChar[CounterX]=*(_WindowTitle+CounterX);
                              // Copia nel buffer interno WindowTitleChar il titolo fornito dalla
                              // routine chiamante

                              WindowData [xA].WindowTitleChar[CounterX]=0;
                              // Assicura che la stringa termini con un carattere 0 anche il caso di
                              // troncamento

                              WindowData [xA].WindowTitle=&(WindowData [xA].WindowTitleChar[0]);

                              // Ora scriviamo il titolo della finestra: lo poniamo al centro dello schermo.

                              if ( (LunghezzaTitolo*8) <= (WindowData [xA].WndLength - 30) )
                              {
                                 PosTitoloX=30+( (WindowData [xA].WndLength - 30) - LunghezzaTitolo*8)/2;
                                 PosTitoloY=2;

                                 ndLP_Write (PosTitoloX, PosTitoloY, WindowData[xA].WindowTitle,
                                 WindowData[xA].ColorTitle, WindowData[xA].ColorBGTitle, xA, NORENDER);
                              }
                              else
                              {
                                 PosTitoloX=30+( (WindowData [xA].WndLength - 30) - 3*8)/2;
                                 PosTitoloY=2;

                                 ndLP_Write (PosTitoloX, PosTitoloY, "...",
                                 WindowData[xA].ColorTitle, WindowData[xA].ColorBGTitle, xA, NORENDER);
                              }

                              // Ora provvediamo a disegnare sullo schermo il pulsante di chiusura finestra
                              Draw_CloseWindow_Button (5, 2, xA);
                              Draw_MinimizeWindow_Button (20, 2, xA);
                              Draw_Title_Button (30, 2, WindowData [xA].WndLength, 12, xA);

                              // Inizializziamo i registri per la CloseWndCallback
                              
                              WindowData[xA].PntToCloseWndCallback = 0;
                              WindowData[xA].WndInfoField          = 0;
                              WindowData[xA].NoNotifyToMouseControl= 0;

                              // e richiamiamo la funzione che si occupa di scrivere le barre di scrolling sullo schermo:
                              ndLP_DrawScrollerRectangles (xA);
                              
                              // Infine, provvediamo a ripulire il buffer a disposizione dell'utente
                              memset (&(WindowData [xA].UserArea [0]), 0, __USERAREASIZE-1);
                           
                              // Se i menù di finestra sono supportati, provvedi a disegnare la menubar
                              if (MenuSupported) INTERNAL_CreateClearMenuBar (xA);
                   
                              // Segnaliamo che la MinWindowsBar potrebbe essere cambiata
                              MinWindowsBarIsChanged=1;
                              
                              // Eseguiamo un XWindowsRender: non rappresenta la finestra, poichè essa è
                              // minimizzata, ma farà comparire l'icona nella MinWindowsBar
                              ndHAL_UpdateWindowsRenderRegisters();                 
                   
                              XWindowRender (xA);
                   
                              // Ed alla fine si ritorna l'identificatore della finestra alla funzione chiamante
                              return (xA);
                          }
                          else  // Il processo di allocazione del service space è fallito
                          {
                              free (WindowData [xA]._p);             // Disalloca la pagina logica
                              free (WindowData [xA]._ws);            // Disalloca il window space
                              free (WindowData [xA]._CharMap);       // Disalloca la CharMap
                              free (WindowData [xA]._ColorCharMap);  // Disalloca la ColorCharMap
                
                              return ERR_MEMORY_OVERFLOW;        
                          }
                                
                    }
                    else  // Il processo di allocazione della color char map è fallito
                    {
                       free (WindowData [xA]._p);       // Disalloca la pagina logica
                       free (WindowData [xA]._ws);      // Disalloca il window space
                       free (WindowData [xA]._CharMap); // Disalloca la CharMap
                
                       return ERR_MEMORY_OVERFLOW;      
                    }
             
             }
             else  // Il processo di allocazione della char map è fallito
             {
                free (WindowData [xA]._p);       // Disalloca la pagina logica
                free (WindowData [xA]._ws);      // Disalloca il window space
                
                return ERR_MEMORY_OVERFLOW;   
             }   
                        
         }
         else    // Il processo di allocazione del window space è fallito
         {
            free (WindowData [xA]._p);  // Disalloca la pagina logica
            return ERR_MEMORY_OVERFLOW;     
         }
         
      }
      else      // Il processo di allocazione della pagina logica è fallito
         return ERR_MEMORY_OVERFLOW;        
    }
    else         // La finestra andrebbe fuori dall'area funzionale
      return  ERR_WINDOW_OVER_FUNC_AREA;
 }
 else    // La finestra richiesta sarebbe troppo piccola
   return (ERR_WINDOW_IS_TOO_SMALL);  
     
}





char ndLP_MaximizeWindow (unsigned char WndHandle)
{
     if (!ndInitSystemExecuted) ndInitSystem();

     if (WindowData [WndHandle].Minimize)
     {
        ndHAL_LockPhoenixGraphThread ();
        
        {
           WindowData [WndHandle].Minimize=0;
           MinWindowsBarIsChanged=1;
        }
        
        ndHAL_UnlockPhoenixGraphThread ();

        ndHAL_UpdateWindowsRenderRegisters ();
        WindowsRender();
     }
     
     return 0;
}

char ndLP_MinimizeWindow (unsigned char WndHandle)
{
     if (!ndInitSystemExecuted) ndInitSystem();

     if (!WindowData [WndHandle].Minimize)
     {
        ndHAL_LockPhoenixGraphThread ();
        
        {
           WindowData [WndHandle].Minimize=1;
           MinWindowsBarIsChanged=1;
        }
        
        ndHAL_UnlockPhoenixGraphThread ();

        ndHAL_UpdateWindowsRenderRegisters ();
        WindowsRender();
     }
     
     return 0;
}




char ndLP_ResizeWindow (unsigned short int NewSizeX, unsigned short int NewSizeY, unsigned char WndHandle)
/*
    Questa funzione si occupa di eseguire il resize di una finestra. La funzione di Resize non si occupa di modificare
    le dimensioni della WindowSpace (che non sono modificabili e restano quelle settate in fase di allocazione della
    finestra), ma si occupano solo di cambiare la posizione della ROI (ovviamente, se si diminuisce la grandezza della
    finestra, appariranno le barre di scrolling).

    Non viene neanche consentito di far sì che la ROI superi in dimensione la WindowSpace, perchè abbiamo
    visto che non è consentito che la ROI possa superare la Window Space.

    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
         ERR_WINDOW_IS_TOO_SMALL              La finestra diventerebbe troppo piccola
         ERR_WINDOW_WILL_BE_TOO_BIG           La finestra diventerebbe troppo grande
         ERR_MEMORY_OVERFLOW                  La memoria è insufficiente
         ERR_WINDOW_IS_NOT_RESIZEABLE         La finestra non è ridimensionabile
         ERR_MEMORY_OVERFLOW                  Overflow della memoria
*/

{  
  int CounterX, CounterY;

  unsigned short int LunghezzaTitolo,
                     PosTitoloX,
                     PosTitoloY;
                     
  struct WindowDataType ResWindowData;
  
  int  SuggestedX, SuggestedY;
  char ValidateX, ValidateY;
  
  // Controllo preliminare sulle dimensioni della WindowSpace.

  if (!ndInitSystemExecuted) ndInitSystem();
 
  if (WindowData [WndHandle].WindowIsActive)
  {
    if (!WindowData [WndHandle].NoResizeable)
    {
           ValidateX = ndLP_ValidateNewSizeX (NewSizeX, WindowData [WndHandle].WSWndLength, WindowData [WndHandle].MenuSupported, &SuggestedX);
           ValidateY = ndLP_ValidateNewSizeY (NewSizeY, WindowData [WndHandle].WSWndHeight, WindowData [WndHandle].MenuSupported, &SuggestedY);
           
           if (ValidateX && ValidateY)
           {
              // Per prima cosa copiamo tutti i parametri dentro un buffer registri
              // temporaneo: in tal modo sarà possibile operare senza alterare la
              // struttura WindowData originaria. Se il processo di creazione della
              // nuova finestra dovesse fallire, per un qualsiasi motivo, si 
              // potrà continuare ad usare la vecchia finestra senza problemi.
                  
              memcpy (&ResWindowData, &(WindowData [WndHandle]), sizeof (struct WindowDataType)); 
          
              // Cominciamo a eseguire i ricalcoli delle dimensioni della logical
              // page
                  
              ResWindowData.WndLimitX2=ResWindowData.WndLimitX1 + NewSizeX;
              ResWindowData.WndLimitY2=ResWindowData.WndLimitY1 + NewSizeY;
                             
              if ((ResWindowData.WndLimitX2<Windows_MaxScreenX) && (ResWindowData.WndLimitY2<Windows_MaxScreenY))    
              {      
                  ResWindowData.WndLength=NewSizeX;
                  ResWindowData.WndHeight=NewSizeY;
                  
                  // Gli acceleratori grafici di alcune piattaforme hanno un'unità che pretende che i rettangoli
                  // copiati dal blitter siano con dimensioni multiple di un certo valore. Tale valore viene 
                  // impostato da un costante definita nel codice del HAL, e detta NDHAL_VIDEOACCELERATOR_ALLIGNMENT
                  // Per usare l'accelerazione video 2d, Nanodesktop deve impostare delle dimensioni virtuali delle
                  // LogicalPage. Le logical page avranno una dimensione maggiore di quanto necessario, per
                  // rispettare le richieste degli acceleratori grafici.
      
                  ResWindowData.HwWndLength = INTERNAL_AllignValue (ResWindowData.WndLength+1, 16);
                  ResWindowData.HwWndHeight = INTERNAL_AllignValue (ResWindowData.WndHeight+1, 1);
      
                  ResWindowData._p = malloc  ( (ResWindowData.HwWndLength) * (ResWindowData.HwWndHeight) * sizeof (TypeColor) );
                  // Alloca lo spazio necessario per la pagina logica. Le routine considerano il pixel di coordinate
                  // WndLength e WndHeight come facenti parte della finestra, quindi bisogna modificare di 1 la richiesta
                  // di memoria su entrambe le direzioni
                               
                  if ( ResWindowData._p > 0)   // Il processo di allocazione della pagina logica è andato a buon fine 
                  {
                     // Provvediamo a reinizializzare la pagina logica
                                  
                     for (CounterY=0; CounterY<=ResWindowData.WndHeight; CounterY++)      
                     {
                        for (CounterX=0; CounterX<=ResWindowData.WndLength; CounterX++)
                        {
                           (*( ResWindowData._p + ( (ResWindowData.WndLength+1) * (CounterY) + (CounterX) ) ))=0;
                        }
                     }
                     
                     // Eseguiamo dei calcoli per determinare le dimensioni del Window-Space
                     
                     // L'area attiva di livello nr. 1 ha due dimensioni diverse a seconda che MenuSupported sia
                     // attivo o meno. 
                     // Quando MenuSupported non è attivato, l'area attiva di livello 1 è il rettangolo che 
                     // inizia subito sotto la barra del titolo, termina due pixel prima del limite LP 
                     // della finestra, mentre orizzontalmente inizia due pixel dopo il limite
                     // LP sinistro della finestra e finisce due pixel prima del limite LP destro della finestra.
                     // Quando MenuSupported è attivato, l'area attiva di livello 1 inizia dopo la menubar
                     
                     if (!(ResWindowData.MenuSupported))
                     {                
                        ResWindowData.MenuBar_PosX1 = 0;
                        ResWindowData.MenuBar_PosY1 = 0;
                        ResWindowData.MenuBar_PosX2 = 0;
                        ResWindowData.MenuBar_PosY2 = 0;
                        
                        ResWindowData.MenuBar_LenX  = 0;
                        ResWindowData.MenuBar_LenY  = 0;
                        
                        ResWindowData.AA1_X1=2;                        
                        ResWindowData.AA1_Y1=13;
                        ResWindowData.AA1_X2=ResWindowData.WndLength-2;
                        ResWindowData.AA1_Y2=ResWindowData.WndHeight-2;
            
                        ResWindowData.AA1_LenX=ResWindowData.AA1_X2-ResWindowData.AA1_X1;  // e le relative dimensioni
                        ResWindowData.AA1_LenY=ResWindowData.AA1_Y2-ResWindowData.AA1_Y1;
                     }
                     else
                     {         
                        ResWindowData.MenuBar_PosX1 = 2;
                        ResWindowData.MenuBar_PosY1 = 14;
                        ResWindowData.MenuBar_PosX2 = ResWindowData.WndLength - 2;
                        ResWindowData.MenuBar_PosY2 = 30;
                        
                        ResWindowData.MenuBar_LenX  = ResWindowData.MenuBar_PosX2 - ResWindowData.MenuBar_PosX1;
                        ResWindowData.MenuBar_LenY  = ResWindowData.MenuBar_PosY2 - ResWindowData.MenuBar_PosY1;
                        
                        ResWindowData.MenuBar_LA_PosX1 = ResWindowData.MenuBar_PosX1 + 34;
                        ResWindowData.MenuBar_LA_PosY1 = ResWindowData.MenuBar_PosY1;
                        ResWindowData.MenuBar_LA_PosX2 = ResWindowData.MenuBar_PosX2 - 17;
                        ResWindowData.MenuBar_LA_PosY2 = ResWindowData.MenuBar_PosY2;
                        
                        ResWindowData.MenuBar_LA_LenX  = ResWindowData.MenuBar_LA_PosX2 - ResWindowData.MenuBar_LA_PosX1;
                        ResWindowData.MenuBar_LA_LenY  = ResWindowData.MenuBar_LA_PosY2 - ResWindowData.MenuBar_LA_PosY1;
                        
                        // Poni MenuBarIsChanged ad 1: il prossimo XWindowRender ricreerà il menù
                        ResWindowData.MenuBarIsChanged=1;
                        
                        // Durante la fase di resize: gli eventuali menù di secondo livello vengono sempre chiusi
                        ResWindowData.OpenL2WMIMenu=0;
                        ResWindowData.NrL1WMIButtonOpened=0;
                        
                        ResWindowData.PrevOpenL2WMIMenu=0;
                        // Necessario per il funzionamento corretto del renderer WMIL2
                        
                        // Reinizializza AA1
                        ResWindowData.AA1_X1=2;                        
                        ResWindowData.AA1_Y1=32;
                        ResWindowData.AA1_X2=ResWindowData.WndLength-2;
                        ResWindowData.AA1_Y2=ResWindowData.WndHeight-2;
            
                        ResWindowData.AA1_LenX=ResWindowData.AA1_X2-ResWindowData.AA1_X1;  // e le relative dimensioni
                        ResWindowData.AA1_LenY=ResWindowData.AA1_Y2-ResWindowData.AA1_Y1;
                        
                     }
                     
                     
                     // Coordinate della barra di scrolling orizzontale
                     ResWindowData.OrizzBar_PosX1 = ResWindowData.AA1_X1;
                     ResWindowData.OrizzBar_PosX2 = ResWindowData.AA1_X2-10;
                     ResWindowData.OrizzBar_PosY1 = ResWindowData.AA1_Y2-10;
                     ResWindowData.OrizzBar_PosY2 = ResWindowData.AA1_Y2;
             
                     // Coordinate della barra di scrolling verticale
                     ResWindowData.VertBar_PosX1 = ResWindowData.AA1_X2-10;
                     ResWindowData.VertBar_PosX2 = ResWindowData.AA1_X2;
                     ResWindowData.VertBar_PosY1 = ResWindowData.AA1_Y1;
                     ResWindowData.VertBar_PosY2 = ResWindowData.AA1_Y2-10;
                    
                     // L'area attiva di livello nr. 2 è il rettangolo compreso tra le barre di scrolling della ROI.
        
                     ResWindowData.AA2_X1=ResWindowData.AA1_X1;
                     ResWindowData.AA2_Y1=ResWindowData.AA1_Y1;
                     ResWindowData.AA2_X2=ResWindowData.AA1_X2-10;
                     ResWindowData.AA2_Y2=ResWindowData.AA1_Y2-10;
        
                     ResWindowData.AA2_LenX=ResWindowData.AA2_X2-ResWindowData.AA2_X1;
                     ResWindowData.AA2_LenY=ResWindowData.AA2_Y2-ResWindowData.AA2_Y1;
                 
                     ResWindowData.AA2_LenX=ResWindowData.AA2_X2-ResWindowData.AA2_X1;
                     ResWindowData.AA2_LenY=ResWindowData.AA2_Y2-ResWindowData.AA2_Y1;       
                         
                     // L'area attiva di livello nr. 3 è il rettangolo dove può avvenire effettivamente la visualizzazione
                     // delle immagini e dei caratteri
            
                     ResWindowData.AA3_X1=ResWindowData.AA2_X1+1;
                     ResWindowData.AA3_Y1=ResWindowData.AA2_Y1+1;
                     ResWindowData.AA3_X2=ResWindowData.AA2_X2-1;
                     ResWindowData.AA3_Y2=ResWindowData.AA2_Y2-1;
            
                     ResWindowData.AA3_LenX=ResWindowData.AA3_X2-ResWindowData.AA3_X1;
                     ResWindowData.AA3_LenY=ResWindowData.AA3_Y2-ResWindowData.AA3_Y1;
            
                     // ATTENZIONE: Le variabili di tipo AAx esprimono la posizione di pixel con riferimento al
                     // primo pixel in alto a sinistra della finestra. Infatti, il primo pixel dell'area attiva di
                     // livello 1 ha coordinate (2, 13) proprio perchè si trova due pixel a destra e 13 pixel in
                     // basso rispetto al primo pixel della finestra.
            
                     ResWindowData.ROI_LenX = ResWindowData.AA3_LenX;
                     ResWindowData.ROI_LenY = ResWindowData.AA3_LenY;
                     
                     // Le variabili ROI_PosX e ROI_PosY sono vincolate dalle condizioni:
                     // 0 < ROI_PosX < WSWndLength - ROI_LenX
                     // 0 < ROI_PosY < WSWndLength - ROI_LenY
                     //
                     // Fino a questo momento abbiamo modificato ROI_LenX e ROI_LenY, ma non
                     // abbiamo modificato ROI_PosX e ROI_PosY, che restano quelli fissati in
                     // precedenza. Conseguenza di questo: è necessario ricontrollare che
                     // ROI_PosX e ROI_PosY rispondano ancora alle condizioni e se necessario
                     // correggerne in automatico il valore
                  
                     if (! (ResWindowData.ROI_PosX <= (ResWindowData.WSWndLength - ResWindowData.ROI_LenX) ) )
                     {
                        ResWindowData.ROI_PosX = ResWindowData.WSWndLength - ResWindowData.ROI_LenX;
                     }
                  
                     if (! (ResWindowData.ROI_PosY <= (ResWindowData.WSWndHeight - ResWindowData.ROI_LenY) ) )
                     {
                        ResWindowData.ROI_PosY = ResWindowData.WSWndHeight - ResWindowData.ROI_LenY;
                     }
               
                     // Esegui il calcolo delle coordinate della ROI
                     ResWindowData.ROI_PosX1 = ResWindowData.ROI_PosX;
                     ResWindowData.ROI_PosY1 = ResWindowData.ROI_PosY;
                     ResWindowData.ROI_PosX2 = ResWindowData.ROI_PosX + ResWindowData.ROI_LenX;
                     ResWindowData.ROI_PosY2 = ResWindowData.ROI_PosY + ResWindowData.ROI_LenY;
            
                     // La Window Space non viene toccata nella fase di resize, nè vengono toccate la charmap e
                     // la color char map
                     
                                             
                     if (ResWindowData.MenuSupported) 
                     {                
                          ResWindowData.SS_Supported = 1;
                          ResWindowData.SS_Enabled   = 1;
                          ResWindowData.SS_LenX      = ResWindowData.ROI_LenX;
                          ResWindowData.SS_LenY      = ResWindowData.ROI_LenY;
                       
                          ResWindowData._ss = malloc  (ResWindowData.SS_LenX * ResWindowData.SS_LenY * sizeof (TypeColor) );
                       
                          // Resettiamo il nuovo service space
                          
                          if (ResWindowData._ss > 0)
                          {
                             for (CounterY=0; CounterY<ResWindowData.SS_LenY; CounterY++)
                             {
                                 for (CounterX=0; CounterX<ResWindowData.SS_LenX; CounterX++)
                                 {
                                     (*( ResWindowData._ss + (ResWindowData.SS_LenX * (CounterY) + (CounterX) ) )) = 0;
                                 }
                             }
                          }
                          
                          
                       }
                       else
                       {
                          ResWindowData.SS_Supported = 0;
                          ResWindowData.SS_Enabled   = 0;
                          ResWindowData.SS_LenX      = 0;
                          ResWindowData.SS_LenY      = 0;
                          
                          ResWindowData._ss=0; 
                       }
                       
                       if ( (!(ResWindowData.MenuSupported)) || (ResWindowData._ss>0) )
                       {
                          // Arrivati a questo punto nel codice, significa che i processi di 
                          // allocazione sono riusciti. Non c'è più motivo di continuare ad
                          // usare il buffer registri temporaneo. Per prima cosa disallochiamo
                          // le vecchie aree di memoria che non servono più
                          
                          free (WindowData [WndHandle]._p);
                          
                          if (WindowData [WndHandle].MenuSupported) 
                            free (WindowData [WndHandle]._ss);
                            
                          // Ora provvediamo a copiare il buffer registri temporaneo nella struct
                          // WindowData definitiva
                          
                          // Per prima cosa eseguiamo il lock del Phoenix Graph Thread in modo
                          // che non intervenga durante tutto il processo....
                          ndHAL_LockPhoenixGraphThread ();
                          
                          { 
                          
                              // Eseguiamo la copia dei registri: il Phoenix Graph Thread è ancora bloccato 
                              memcpy ( &(WindowData [WndHandle]), &ResWindowData, sizeof (struct WindowDataType));    
                              
                              // Le seguenti righe di codice costruiscono la finestra nella relativa pagina logica.
                              ndLP_DrawRectangle (0, 0, WindowData [WndHandle].WndLength, WindowData [WndHandle].WndHeight, 
                                  WindowData [WndHandle].ColorBGWS, WindowData [WndHandle].ColorBorder, WndHandle, NORENDER);
                                                    
                              ndLP_DrawRectangle (0, 0, WindowData [WndHandle].WndLength, 12, 
                                  WindowData [WndHandle].ColorBGTitle, WindowData [WndHandle].ColorBorder, WndHandle, NORENDER);
    
                              // Ora scriviamo il titolo della finestra: lo poniamo al centro dello schermo.
    
                              LunghezzaTitolo = strlen (WindowData [WndHandle].WindowTitle);
    
                              if ( (LunghezzaTitolo*8) <= (WindowData [WndHandle].WndLength - 30) )
                              {
                                 PosTitoloX=30+( (WindowData [WndHandle].WndLength - 30) - LunghezzaTitolo*8)/2;
                                 PosTitoloY=2;
    
                                 ndLP_Write (PosTitoloX, PosTitoloY, WindowData [WndHandle].WindowTitle,
                                 WindowData [WndHandle].ColorTitle, WindowData [WndHandle].ColorBGTitle, WndHandle, NORENDER);
                              }
                              else
                              {
                                 PosTitoloX=30+( (WindowData [WndHandle].WndLength - 30) - 3*8)/2;
                                 PosTitoloY=2;
    
                                 ndLP_Write (PosTitoloX, PosTitoloY, "...",
                                 WindowData [WndHandle].ColorTitle, WindowData [WndHandle].ColorBGTitle, WndHandle, NORENDER);
                              }
    
                              // Ora provvediamo a disegnare sullo schermo il pulsante di chiusura finestra
                              Draw_CloseWindow_Button (5, 2, WndHandle);
                              Draw_MinimizeWindow_Button (20, 2, WndHandle);
                              Draw_Title_Button (30, 2, WindowData [WndHandle].WndLength, 12, WndHandle);
    
                              // e richiamiamo la funzione che si occupa di scrivere le barre di scrolling sullo schermo:
                              ndLP_DrawScrollerRectangles (WndHandle);
                           
                              // Se i menù di finestra sono supportati, provvedi a disegnare la menubar
                              if (WindowData [WndHandle].MenuSupported) INTERNAL_CreateClearMenuBar (WndHandle);
                   
                              // Segnaliamo che la MinWindowsBar potrebbe essere cambiata
                              MinWindowsBarIsChanged=1;

                          }

                          // Riattiviamo il Phoenix Graph Thread
                          ndHAL_UnlockPhoenixGraphThread ();

                          // Eseguiamo un XWindowsRender: provvederà anche ad eseguire un nuovo render
                          // dei menù L1WMI ed L2WMI
                          ndHAL_UpdateWindowsRenderRegisters();                 
                          XWindowRender (WndHandle);
               
                          // Ed alla fine si ritorna il valore 0 per informare la routine chiamante che l'operazione
                          // è riuscita
                          return 0;
                     }
                     else  // Il processo di allocazione del service space è fallito
                     {
                          free (ResWindowData._p);             // Disalloca la pagina logica
                          
                          return ERR_MEMORY_OVERFLOW;        
                     }                
                  }
                  else      // Il processo di allocazione della pagina logica è fallito
                     return ERR_MEMORY_OVERFLOW;                         
                                                      
              }
              else return ERR_WINDOW_OVER_FUNC_AREA;
           }
           else return ERR_WINDOW_IS_TOO_SMALL;
    }
    else return ERR_WINDOW_IS_NOT_RESIZEABLE;            
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;
}




char ndLP_ChangeWindowParameters      (unsigned short int _PosWindowX1,
                                       unsigned short int _PosWindowY1,
                                       unsigned short int _PosWindowX2,
                                       unsigned short int _PosWindowY2,
                                       char *_WindowTitle,
                                       TypeColor _ColorTitle,
                                       TypeColor _ColorBGTitle,
                                       TypeColor _ColorWindow,
                                       TypeColor _ColorBorderWindow,
                                       unsigned short int _WSLength,
                                       unsigned short int _WSHeight,
                                       unsigned char WndHandle,
                                       unsigned short int PutInTheFirstPlane,
                                       unsigned short int Attribute)
/*
  Questa routine cambia i parametri della finestra identificata da WndHandle.
  La procedura è distruttiva: nel cambiamento di parametri il contenuto della
  finestra sarà sempre, irrimediabilmente perso, salvo i bytes dell'user area
  che invece vengono preservati.

  Quando la routine restituisce un valore negativo, allora siamo in presenza
  di un errore. I codici di errore sono:

        ERR_WINDOWS_IS_NOT_ALLOCATED       Non ci sono più finestre disponibili
        ERR_WINDOW_IS_TOO_SMALL            Finestra troppo piccola
        ERR_WS_SPACE_REQUEST_IS_TOO_LARGE  L'estensione della Window Space richiesta è
                                           troppo grande
        ERR_WINDOW_OVER_THE_FUNC_AREA      La finestra coprirebbe l'area funzionale
        ERR_MEMORY_OVERFLOW                Overflow della memoria. 
        
  Qualora l'utente richiesta una window space "più piccola" dell'area ROI della
  finestra, la routine adatta automaticamente la richiesta per la WindowSpace
  al valore della ROI della finestra, non essendo consentito di lavorare con
  WindowSpace che siano più piccole dell'area ROI della finestra.

  Il parametro PutInTheFirstPlane, fa sì che la finestra dopo le modifiche
  distruttive sia posta in primo piano (se posto a 1).
  
  Il parametro Attribute ha lo stesso significato visto in AllocateWindowComplex.
  
  Quando la routine riporta un errore, la vecchia finestra non viene modificata. 
*/

{
 unsigned short int LunghezzaTitolo,
                    PosTitoloX,
                    PosTitoloY;
                           
 int Counter, CounterX, CounterY;                  
 char NoResizeable, AutoFix, MenuSupported;
 char NeedToKeepWinMenu;
 
 unsigned short int ThWndLength, ThWndHeight;
   
 struct WindowDataType NewWindowData;      // Destinato ad accogliere temporaneamente i dati della nuova finestra

 if (!ndInitSystemExecuted) ndInitSystem();
 
 if (WindowData [WndHandle].WindowIsActive)   
 {
             if ((Attribute & NO_RESIZEABLE) == NO_RESIZEABLE)  NoResizeable=1;
                else NoResizeable=0;
                
             if ((Attribute & AUTOFIX) == AUTOFIX) AutoFix=1;
                else AutoFix=0;
                
             if ((Attribute & MENUSUPPORTED) == MENUSUPPORTED) MenuSupported=1;
                else MenuSupported=0;
                
             // Se la vecchia finestra aveva supporto per i menù, ed anche la
             // nuova ha supporto per i menù, è necessario tenere i vecchi
             // menù di sistema, altrimenti questi ultimi devono essere
             // cancellati e la lista di WMI deve essere resettata (insieme
             // ai relativi registri della menu bar).
             
             if ((WindowData [WndHandle].MenuSupported==1) && (MenuSupported==1))
             {
                NeedToKeepWinMenu=1;
             }
             else
             {
                NeedToKeepWinMenu=0;
             }
             
             if ((_WSLength >= MAXDIM_WSLENGTH) || (_WSHeight >= MAXDIM_WSHEIGHT)) 
                return ERR_WS_SPACE_REQUEST_IS_TOO_LARGE;
            
             ThWndLength = abs (_PosWindowX2 - _PosWindowX1);
             ThWndHeight = abs (_PosWindowY2 - _PosWindowY1);
 
             if (   (  ThWndLength>=ndLP_GetMinWndSizeX (MenuSupported) ) ||
                    (  ThWndHeight>=ndLP_GetMinWndSizeY (MenuSupported) )    )
             {
               
               if (  (_PosWindowX1<Windows_MaxScreenX) && (_PosWindowX2<Windows_MaxScreenX) &&
                  (_PosWindowY1<Windows_MaxScreenY) && (_PosWindowY2<Windows_MaxScreenY)  )
               {
                  NewWindowData.WindowTitle=_WindowTitle;          // Qui viene memorizzato l'indirizzo del titolo finestra (che è ciò che viene
                  NewWindowData.ColorTitle=_ColorTitle;            // passato alla funzione tramite stack
                  NewWindowData.ColorBGTitle=_ColorBGTitle;
            
                  NewWindowData.ColorBorder=_ColorBorderWindow;
                  NewWindowData.ColorBGWS=_ColorWindow;
                  
                  if ((Attribute & USE_POSGEN)==USE_POSGEN)
                  {
                     ndPOSGEN_GenPosition (ThWndLength, ThWndHeight, &_PosWindowX1, &_PosWindowY1, &_PosWindowX2, &_PosWindowY2);
                  }

                  NewWindowData.WndLimitX1=_PosWindowX1;
                  NewWindowData.WndLimitY1=_PosWindowY1;
                  NewWindowData.WndLimitX2=_PosWindowX2;
                  NewWindowData.WndLimitY2=_PosWindowY2;
                               
                  NewWindowData.WndLength=ThWndLength;
                  NewWindowData.WndHeight=ThWndHeight;
                  
                  // Gli acceleratori grafici di alcune piattaforme hanno un'unità che pretende che i rettangoli
                  // copiati dal blitter siano con dimensioni multiple di un certo valore. Tale valore viene 
                  // impostato da un costante definita nel codice del HAL, e detta NDHAL_VIDEOACCELERATOR_ALLIGNMENT
                  // Per usare l'accelerazione video 2d, Nanodesktop deve impostare delle dimensioni virtuali delle
                  // LogicalPage. Le logical page avranno una dimensione maggiore di quanto necessario, per
                  // rispettare le richieste degli acceleratori grafici.
      
                  NewWindowData.HwWndLength = INTERNAL_AllignValue (NewWindowData.WndLength+1, 16);
                  NewWindowData.HwWndHeight = INTERNAL_AllignValue (NewWindowData.WndHeight+1, 1);
      
                  NewWindowData._p = malloc  ( (NewWindowData.HwWndLength) * (NewWindowData.HwWndHeight) * sizeof (TypeColor) );
                  // Alloca lo spazio necessario per la pagina logica. Le routine considerano il pixel di coordinate
                  // WndLength e WndHeight come facenti parte della finestra, quindi bisogna modificare di 1 la richiesta
                  // di memoria su entrambe le direzioni
                               
                  if ( NewWindowData._p > 0)   // Il processo di allocazione della pagina logica è andato a buon fine 
                  {
                     // Provvediamo a reinizializzare la pagina logica
                                  
                     for (CounterY=0; CounterY<=NewWindowData.WndHeight; CounterY++)      
                     {
                        for (CounterX=0; CounterX<=NewWindowData.WndLength; CounterX++)
                        {
                           (*( NewWindowData._p + ( (NewWindowData.WndLength+1) * (CounterY) + (CounterX) ) ))=0;
                        }
                     }
                     
                     // Eseguiamo dei calcoli per determinare le dimensioni del Window-Space
                     
                     // L'area attiva di livello nr. 1 ha due dimensioni diverse a seconda che MenuSupported sia
                     // attivo o meno. 
                     // Quando MenuSupported non è attivato, l'area attiva di livello 1 è il rettangolo che 
                     // inizia subito sotto la barra del titolo, termina due pixel prima del limite LP 
                     // della finestra, mentre orizzontalmente inizia due pixel dopo il limite
                     // LP sinistro della finestra e finisce due pixel prima del limite LP destro della finestra.
                     // Quando MenuSupported è attivato, l'area attiva di livello 1 inizia dopo la menubar
                     
                     if (!MenuSupported)
                     {                
                        NewWindowData.MenuSupported = 0;
                        
                        NewWindowData.MenuBar_PosX1 = 0;
                        NewWindowData.MenuBar_PosY1 = 0;
                        NewWindowData.MenuBar_PosX2 = 0;
                        NewWindowData.MenuBar_PosY2 = 0;
                        
                        NewWindowData.MenuBar_LenX  = 0;
                        NewWindowData.MenuBar_LenY  = 0;
                        
                        NewWindowData.AA1_X1=2;                        
                        NewWindowData.AA1_Y1=13;
                        NewWindowData.AA1_X2=NewWindowData.WndLength-2;
                        NewWindowData.AA1_Y2=NewWindowData.WndHeight-2;
            
                        NewWindowData.AA1_LenX=NewWindowData.AA1_X2-NewWindowData.AA1_X1;  // e le relative dimensioni
                        NewWindowData.AA1_LenY=NewWindowData.AA1_Y2-NewWindowData.AA1_Y1;
                       
                     }
                     else
                     {      
                        NewWindowData.MenuSupported = 1;
                        
                        NewWindowData.MenuBar_PosX1 = 2;
                        NewWindowData.MenuBar_PosY1 = 14;
                        NewWindowData.MenuBar_PosX2 = NewWindowData.WndLength - 2;
                        NewWindowData.MenuBar_PosY2 = 30;
                        
                        NewWindowData.MenuBar_LenX  = NewWindowData.MenuBar_PosX2 - NewWindowData.MenuBar_PosX1;
                        NewWindowData.MenuBar_LenY  = NewWindowData.MenuBar_PosY2 - NewWindowData.MenuBar_PosY1;
                        
                        NewWindowData.MenuBar_LA_PosX1 = NewWindowData.MenuBar_PosX1 + 34;
                        NewWindowData.MenuBar_LA_PosY1 = NewWindowData.MenuBar_PosY1;
                        NewWindowData.MenuBar_LA_PosX2 = NewWindowData.MenuBar_PosX2 - 17;
                        NewWindowData.MenuBar_LA_PosY2 = NewWindowData.MenuBar_PosY2;
            
                        NewWindowData.MenuBar_LA_LenX  = NewWindowData.MenuBar_LA_PosX2 - NewWindowData.MenuBar_LA_PosX1;
                        NewWindowData.MenuBar_LA_LenY  = NewWindowData.MenuBar_LA_PosY2 - NewWindowData.MenuBar_LA_PosY1;
            
                        // Reinizializza MenuBarIsChanged
                        if (!NeedToKeepWinMenu)
                           NewWindowData.MenuBarIsChanged=0;
                        else
                           NewWindowData.MenuBarIsChanged=1;   // Così il menù sarà rivisualizzato da XWindowRender
                        
                        // Reinizializza ZString
                        strcpy (NewWindowData.ZString, "");
            
                        // Il primo elemento da visualizzare è il nr. 0
                        NewWindowData.NrFirstL1WMIButton=0;
            
                        // Al momento dell'allocazione della finestra, non ci sono menù di secondo livello aperti
                        NewWindowData.OpenL2WMIMenu=0;
                        NewWindowData.NrL1WMIButtonOpened=0;
            
                        NewWindowData.AA1_X1=2;                        
                        NewWindowData.AA1_Y1=32;
                        NewWindowData.AA1_X2=NewWindowData.WndLength-2;
                        NewWindowData.AA1_Y2=NewWindowData.WndHeight-2;
            
                        NewWindowData.AA1_LenX=NewWindowData.AA1_X2-NewWindowData.AA1_X1;  // e le relative dimensioni
                        NewWindowData.AA1_LenY=NewWindowData.AA1_Y2-NewWindowData.AA1_Y1;
            
                        // Reinizializza la lista di voci del menù in quella che sarà la nuova finestra
                        
                        if (!NeedToKeepWinMenu)
                        {
                            NewWindowData.MainWinMenu.FirstElementAddr=0;
                            NewWindowData.MainWinMenu.LastElementAddr=0;
                            NewWindowData.MainWinMenu.NrElements=0;
                            NewWindowData.MainWinMenu.Dynamic=0;
                        }
                        else
                        {
                            NewWindowData.MainWinMenu.FirstElementAddr = WindowData [WndHandle].MainWinMenu.FirstElementAddr;
                            NewWindowData.MainWinMenu.LastElementAddr  = WindowData [WndHandle].MainWinMenu.LastElementAddr;
                            NewWindowData.MainWinMenu.NrElements       = WindowData [WndHandle].MainWinMenu.NrElements;
                            NewWindowData.MainWinMenu.Dynamic          = WindowData [WndHandle].MainWinMenu.Dynamic;
                        }
                     }
                     
                     if (!NoResizeable)
                     {
                        NewWindowData.NoResizeable=0;
                     
                        // Coordinate della barra di scrolling orizzontale
                        NewWindowData.OrizzBar_PosX1 = NewWindowData.AA1_X1;
                        NewWindowData.OrizzBar_PosX2 = NewWindowData.AA1_X2-10;
                        NewWindowData.OrizzBar_PosY1 = NewWindowData.AA1_Y2-10;
                        NewWindowData.OrizzBar_PosY2 = NewWindowData.AA1_Y2;
                 
                        // Coordinate della barra di scrolling verticale
                        NewWindowData.VertBar_PosX1 = NewWindowData.AA1_X2-10;
                        NewWindowData.VertBar_PosX2 = NewWindowData.AA1_X2;
                        NewWindowData.VertBar_PosY1 = NewWindowData.AA1_Y1;
                        NewWindowData.VertBar_PosY2 = NewWindowData.AA1_Y2-10;
                        
                        // L'area attiva di livello nr. 2 è il rettangolo compreso tra le barre di scrolling della ROI.
            
                        NewWindowData.AA2_X1=NewWindowData.AA1_X1;
                        NewWindowData.AA2_Y1=NewWindowData.AA1_Y1;
                        NewWindowData.AA2_X2=NewWindowData.AA1_X2-10;
                        NewWindowData.AA2_Y2=NewWindowData.AA1_Y2-10;
            
                        NewWindowData.AA2_LenX=NewWindowData.AA2_X2-NewWindowData.AA2_X1;
                        NewWindowData.AA2_LenY=NewWindowData.AA2_Y2-NewWindowData.AA2_Y1;
                     }
                     else      // Attivata la funzionalità No-Resizeable
                     {
                        NewWindowData.NoResizeable=1;
                     
                        // In questo caso, le coordinate della barra di scrolling non hanno significato
                        NewWindowData.OrizzBar_PosX1 = 0;
                        NewWindowData.OrizzBar_PosX2 = 0;
                        NewWindowData.OrizzBar_PosY1 = 0;
                        NewWindowData.OrizzBar_PosY2 = 0;
                        NewWindowData.VertBar_PosX1  = 0;
                        NewWindowData.VertBar_PosX2  = 0;
                        NewWindowData.VertBar_PosY1  = 0;
                        NewWindowData.VertBar_PosY2  = 0;
                        
                        // L'area attiva di livello nr. 2 coincide con l'area attiva di livello 1.
            
                        NewWindowData.AA2_X1=NewWindowData.AA1_X1;
                        NewWindowData.AA2_Y1=NewWindowData.AA1_Y1;
                        NewWindowData.AA2_X2=NewWindowData.AA1_X2;
                        NewWindowData.AA2_Y2=NewWindowData.AA1_Y2;
                     }   
            
                     NewWindowData.AA2_LenX=NewWindowData.AA2_X2-NewWindowData.AA2_X1;
                     NewWindowData.AA2_LenY=NewWindowData.AA2_Y2-NewWindowData.AA2_Y1;       
                         
                     // L'area attiva di livello nr. 3 è il rettangolo dove può avvenire effettivamente la visualizzazione
                     // delle immagini e dei caratteri
            
                     NewWindowData.AA3_X1=NewWindowData.AA2_X1+1;
                     NewWindowData.AA3_Y1=NewWindowData.AA2_Y1+1;
                     NewWindowData.AA3_X2=NewWindowData.AA2_X2-1;
                     NewWindowData.AA3_Y2=NewWindowData.AA2_Y2-1;
            
                     NewWindowData.AA3_LenX=NewWindowData.AA3_X2-NewWindowData.AA3_X1;
                     NewWindowData.AA3_LenY=NewWindowData.AA3_Y2-NewWindowData.AA3_Y1;
            
                     // ATTENZIONE: Le variabili di tipo AAx esprimono la posizione di pixel con riferimento al
                     // primo pixel in alto a sinistra della finestra. Infatti, il primo pixel dell'area attiva di
                     // livello 1 ha coordinate (2, 13) proprio perchè si trova due pixel a destra e 13 pixel in
                     // basso rispetto al primo pixel della finestra.
            
                     // La posizione della ROI all'inizio è sempre posta al massimo valore in alto a sinistra:
                     
                     NewWindowData.ROI_PosX=0;
                     NewWindowData.ROI_PosY=0;
            
                     NewWindowData.ROI_LenX = NewWindowData.AA3_LenX;
                     NewWindowData.ROI_LenY = NewWindowData.AA3_LenY;
                     
                     // Esegui il calcolo delle coordinate della ROI
                     NewWindowData.ROI_PosX1 = NewWindowData.ROI_PosX;
                     NewWindowData.ROI_PosY1 = NewWindowData.ROI_PosY;
                     NewWindowData.ROI_PosX2 = NewWindowData.ROI_PosX + NewWindowData.ROI_LenX;
                     NewWindowData.ROI_PosY2 = NewWindowData.ROI_PosY + NewWindowData.ROI_LenY;
            
                     // I pixel della Window space hanno coordinate che sono riferite al primo punto della
                     // Window Space (NON, come le coordinate AAx, al primo punto della finestra).
            
                     // Di conseguenza, se la Window Space fosse un rettangolo avrebbe coordinate
                     // (0, 0, WSWndLength, WSWndHeight). Sono necessari sono due parametri per descrivere le
                     // dimensioni della Window Space
            
                     if ( (!NoResizeable) && (!AutoFix) )
                     {
                        if (_WSLength >= NewWindowData.ROI_LenX)
                           NewWindowData.WSWndLength=_WSLength;
                        else
                           NewWindowData.WSWndLength=NewWindowData.ROI_LenX;
            
                        if (_WSHeight >= NewWindowData.ROI_LenY)
                           NewWindowData.WSWndHeight=_WSHeight;
                        else
                           NewWindowData.WSWndHeight=NewWindowData.ROI_LenY;
                     }
                     else     // Modalità non resizeable o noautofix
                     {
                        NewWindowData.WSWndLength = NewWindowData.ROI_LenX;
                        NewWindowData.WSWndHeight = NewWindowData.ROI_LenY;
                     }
            
                     // Provvediamo ad allocare lo spazio relativo al window space
                     // N.B. Per il WindowSpace non è necessario allocare 1 elemento in più su entrambe le dimensioni
                     // perchè le routine WS non considerano il pixel di coordinate NewWindowData.WSWndLength,
                     // NewWindowData.WSWndHeight come facente parte del Window Space
                     
                     NewWindowData._ws=malloc  ( NewWindowData.WSWndLength * NewWindowData.WSWndHeight * sizeof (TypeColor) );
                     
                     if (NewWindowData._ws > 0)   // Il processo di allocazione del window space è andato a buon fine
                     {           
                         // Provvediamo a reinizializzare il Window Space
                                  
                         for (CounterY=0; CounterY<NewWindowData.WSWndHeight; CounterY++)      
                         {
                             for (CounterX=0; CounterX<NewWindowData.WSWndLength; CounterX++)
                             {
                                (*( NewWindowData._ws + (NewWindowData.WSWndLength * (CounterY) + (CounterX) ) )) = _ColorWindow;
                             }
                         }
                         
                         // Calcoliamo il numero di caratteri contenibili
            
                         NewWindowData.MargineCharX = ((NewWindowData.WSWndLength) % 8)/2;
                         NewWindowData.MargineCharY = ((NewWindowData.WSWndHeight) % 8)/2;
                         
                         // Ricalcoliamo le coordinate della char-area
                         NewWindowData.CharArea_PosX1 = NewWindowData.MargineCharX;
                         NewWindowData.CharArea_PosY1 = NewWindowData.MargineCharY;
                         NewWindowData.CharArea_PosX2 = NewWindowData.WSWndLength - NewWindowData.MargineCharX - 1;
                         NewWindowData.CharArea_PosY2 = NewWindowData.WSWndHeight - NewWindowData.MargineCharY - 1;
                         // Supponiamo che il margine sia 0. CharArea_PosX2 e CharArea_PosY2 devono avere valore pari
                         // a WSWndLength-1 e WSWndHeight-1.

                         NewWindowData.WS_MaxCursX = NewWindowData.WSWndLength/8;
                         NewWindowData.WS_MaxCursY = NewWindowData.WSWndHeight/8;
            
                         // Adesso tentiamo di allocare la mappa caratteri
                         
                         NewWindowData._CharMap =malloc  (NewWindowData.WS_MaxCursX * NewWindowData.WS_MaxCursY * sizeof (unsigned char));
                         
                         if (NewWindowData._CharMap>0)       // Il processo di allocazione della char map è riuscito
                         {                                  
                                NewWindowData._ColorCharMap = malloc  (NewWindowData.WS_MaxCursX * NewWindowData.WS_MaxCursY * sizeof (TypeColor));
                                
                                if (NewWindowData._ColorCharMap>0)    // Il processo di allocazione della color char map è riuscito
                                {
                                       for (CounterY=0; CounterY<NewWindowData.WS_MaxCursY; CounterY++)          // E' ora viene ripulita la memoria caratteri
                                       {
                                              for (CounterX=0; CounterX<NewWindowData.WS_MaxCursX; CounterX++)
                                              {
                                                  (*( NewWindowData._CharMap + (NewWindowData.WS_MaxCursX * (CounterY) + (CounterX) ) )) = '\x00';
                                                  (*( NewWindowData._ColorCharMap + (NewWindowData.WS_MaxCursX * (CounterY) + (CounterX) ) )) = 0;   
                                              }
                                       }
                                             
                                       if (MenuSupported) 
                                       {                
                                          NewWindowData.SS_Supported = 1;
                                          NewWindowData.SS_Enabled   = 1;
                                          NewWindowData.SS_LenX      = NewWindowData.ROI_LenX;
                                          NewWindowData.SS_LenY      = NewWindowData.ROI_LenY;
                                       
                                          NewWindowData._ss = malloc  (NewWindowData.SS_LenX * NewWindowData.SS_LenY * sizeof (TypeColor) );
                                       
                                          // Eseguiamo la reinizializzazione del service space
                                          
                                          if (NewWindowData._ss > 0)
                                          {
                                             for (CounterY=0; CounterY<NewWindowData.SS_LenY; CounterY++)
                                             {
                                                 for (CounterX=0; CounterX<NewWindowData.SS_LenX; CounterX++)
                                                 {
                                                     (*( NewWindowData._ss + (NewWindowData.SS_LenX * (CounterY) + (CounterX) ) )) = 0;
                                                 }
                                             }
                                          }
                                          
                                          NewWindowData.PrevOpenL2WMIMenu=0;
                                          // Necessario per il corretto funzionamento del renderer WMIL2
                                       }
                                       else
                                       {
                                          NewWindowData.SS_Supported = 0;
                                          NewWindowData.SS_Enabled   = 0;
                                          NewWindowData.SS_LenX      = 0;
                                          NewWindowData.SS_LenY      = 0;
                                          
                                          NewWindowData._ss=0; 
                                       }
                                       
                                       if ( (!MenuSupported) || (NewWindowData._ss>0) )
                                       {
                                          // E' necessario azzerare tutti gli handle per le trackbar. AllocateWindowComplex
                                          // può permettersi di non farlo, perchè tanto gli handles sono già stati azzerati
                                          // o da InitGraph o da DestroyWindow, ma la struct NewWindowData può essere
                                          // localizzata in qualsiasi punto della ram, quindi dobbiamo provvedere a
                                          // reinizializzare.
                                          
                                          NewWindowData.NrTrackBarsAllocated = 0;
                                          
                                          for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
                                              NewWindowData.TrackBar [Counter].IsActive=0;
                                              
                                          // Stesso ragionamento deve essere fatto per le textbox
                                              
                                          NewWindowData.NrTextBoxAllocated = 0;
                                          
                                          for (Counter=0; Counter<__NRTEXTBOXALLOCABLE; Counter++)
                                              NewWindowData.TextBox [Counter].IsActive=0;
                                              
                                          // e per le listbox
                                          
                                          NewWindowData.NrListBoxAllocated = 0;
                                          
                                          for (Counter=0; Counter<__NRLISTBOXALLOCABLE; Counter++)
                                              NewWindowData.ListBox [Counter].IsActive=0;
                                              
                                              
                                          // Stesso ragionamento deve essere fatto per i campi dei pulsanti
                                          
                                          NewWindowData.NrButtonsAllocated   = 0;
                                          
                                          for (Counter=0; Counter<__NRBUTTONSALLOCABLE; Counter++)
                                              NewWindowData.ButtonClass4_Data [Counter].ButtonIsActivated=0;
                                          
                                          // I pulsanti di classe 3 nella struct NewWindow devono essere disattivati
                                          // all'inizio
      
                                          for (Counter=0; Counter<7; Counter++)
                                              NewWindowData.ButtonClass3_Data[Counter].ButtonIsActivated=0;
                                          
                                          // Settiamo ai valori di default i parametri font
                                          NewWindowData.ndLP_CurrentFntHandle=_FntHandle_Console;
                                          NewWindowData.ndWS_CurrentFntHandle=_FntHandle_Console;
                                          NewWindowData.ndMB_CurrentFntHandle=_FntHandle_Console;

                                          // Impostiamo i valori per la posizione cursore
                                          NewWindowData.WS_PosCursX=0;
                                          NewWindowData.WS_PosCursY=0;
            
                                          NewWindowData.WindowIsActive=1;                         // All'inizio la finestra viene considerata attiva
                                          NewWindowData.Minimize=WindowData [WndHandle].Minimize; 
                                          
                                          /* 
                                          ATTENZIONE:
                                                        
                                          Se il programma è giunto fin qui, allora significa che il processo di
                                          allocazione della nuova finestra è andato a buon fine. Dobbiamo allora
                                          cancellare le infomazioni relative alla vecchia finestra
                                          */
                                          
                                          free (WindowData [WndHandle]._p);   // Disalloca lo spazio riservato alla pagina logica   
                                          free (WindowData [WndHandle]._ws);  // Disalloca lo spazio riservato al Window Space
      
                                          free (WindowData [WndHandle]._CharMap);        // Disalloca CharMap e ColorCharMap
                                          free (WindowData [WndHandle]._ColorCharMap);
      
                                          if (WindowData [WndHandle].MenuSupported) 
                                          {
                                              free (WindowData [WndHandle]._ss);
                                          }
                                          
                                          // Se i nuovi parametri non prevedono l'esistenza di un menù, mentre
                                          // i vecchi parametri la prevedevano, le eventuali voci WMI1 e
                                          // WMI2 vanno cancellate dalla memoria
                                          
                                          if (!NeedToKeepWinMenu)
                                          {
                                              ndWMENU_DestroyL1SubMenu (WndHandle, "");
                                          }             
                                          
                                          // Dobbiamo anche disattivare tutti i pulsanti che erano stati 
                                          // allocati e liberare la memoria contenente le relative informazioni
                                          
                                          // Innanzitutto disattiviamo le trackbar allocate nel sistema
      
                                          for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
                                          {
                                              if (WindowData [WndHandle].TrackBar [Counter].IsActive) 
                                                 ndTBAR_DestroyTrackBar (Counter, WndHandle, 1, 0, NORENDER);
                                                 // Parametro Only-Logical posto a 1 in modo da cancellare solo logicamente
                                          }
      
                                          // Ora cancelliamo gli altri pulsanti di classe 4 usati dall'utente
      
                                          for (Counter=0; Counter<__NRBUTTONSALLOCABLE; Counter++)
                                          {
                                              if (WindowData[WndHandle].ButtonClass4_Data[Counter].ButtonIsActivated)
                                                 ndCTRL_DestroyButton (Counter, WndHandle, 1, 0, NORENDER);
                                                 // Parametro Only-Logical posto a 1 in modo da cancellare solo logicamente
                                          }
                                          
                                          // Cancelliamo anche tutte le textarea presenti
                                          
                                          for (Counter=0; Counter<__NRTEXTBOXALLOCABLE; Counter++)
                                          {
                                              if (WindowData[WndHandle].TextBox[Counter].IsActive)
                                                  ndTBOX_DestroyTextArea (Counter, WndHandle, 1, 0, NORENDER);
                                                  // Parametro Only-Logical posto a 1 in modo da cancellare solo logicamente
                                          }
                                          
                                          
                                          
                                          // Proteggiamo i valori della vecchia user area.
                                          
                                          memcpy ( &(NewWindowData.UserArea[0]), &(WindowData [WndHandle].UserArea[0]), __USERAREASIZE); 
                                          
                                          // Proteggiamo l'indirizzo della vecchia CloseWndCallback
                                          
                                          NewWindowData.PntToCloseWndCallback  = WindowData [WndHandle].PntToCloseWndCallback;
                                          NewWindowData.WndInfoField           = WindowData [WndHandle].WndInfoField;
                                          NewWindowData.NoNotifyToMouseControl = WindowData [WndHandle].NoNotifyToMouseControl;

                                          
                                          
                                          /* 
                                          PROVVEDIAMO ORA A COPIARE I DATI NELLA NORMALE STRUTTURA
                                          WINDOWDATA
                                          */
                                          
                                          // Provvediamo ad eseguire il lock del thread
                                          ndHAL_LockPhoenixGraphThread ();
                                          
                                          {
                                          
                                              // Copia dei registri
                                              memcpy ( &(WindowData [WndHandle]), &(NewWindowData), sizeof (struct WindowDataType));
    
                                              /* 
                                                 Da questo momento tutte le chiamate a routine di disegno
                                                 potranno funzionare
                                              */
                                              
                                              
                                              
                                              // Le seguenti righe di codice costruiscono la finestra nella relativa pagina logica.
                                              ndLP_DrawRectangle (0, 0, WindowData[WndHandle].WndLength, WindowData[WndHandle].WndHeight, _ColorWindow, _ColorBorderWindow, WndHandle, NORENDER);
                                              ndLP_DrawRectangle (0, 0, WindowData[WndHandle].WndLength, 12, _ColorBGTitle, _ColorBorderWindow, WndHandle, NORENDER);
                
                                              // Provvediamo al salvataggio del titolo nel buffer interno
                                              LunghezzaTitolo=strlen (_WindowTitle);
                
                                              if (LunghezzaTitolo>58) LunghezzaTitolo=58;
                                              // In caso di caratteri in più essi vengono semplicemente troncati
                
                                              for (CounterX=0; CounterX<LunghezzaTitolo; CounterX++)
                                                  WindowData[WndHandle].WindowTitleChar[CounterX]=*(_WindowTitle+CounterX);
                                              // Copia nel buffer interno WindowTitleChar il titolo fornito dalla
                                              // routine chiamante
                
                                              WindowData[WndHandle].WindowTitleChar[CounterX]=0;
                                              // Assicura che la stringa termini con un carattere 0 anche il caso di
                                              // troncamento
                
                                              WindowData[WndHandle].WindowTitle=&(WindowData[WndHandle].WindowTitleChar[0]);
                
                                              // Ora scriviamo il titolo della finestra: lo poniamo al centro dello schermo.
                
                                              if ( (LunghezzaTitolo*8) <= (WindowData[WndHandle].WndLength - 30) )
                                              {
                                                 PosTitoloX=30+( (WindowData[WndHandle].WndLength - 30) - LunghezzaTitolo*8)/2;
                                                 PosTitoloY=2;
                
                                                 ndLP_Write (PosTitoloX, PosTitoloY, WindowData[WndHandle].WindowTitle,
                                                 WindowData[WndHandle].ColorTitle, WindowData[WndHandle].ColorBGTitle, WndHandle, NORENDER);
                                              }
                                              else
                                              {
                                                 PosTitoloX=30+( (WindowData[WndHandle].WndLength - 30) - 3*8)/2;
                                                 PosTitoloY=2;
                
                                                 ndLP_Write (PosTitoloX, PosTitoloY, "...",
                                                 WindowData[WndHandle].ColorTitle, WindowData[WndHandle].ColorBGTitle, WndHandle, NORENDER);
                                              }
                
                                              // Ora provvediamo a disegnare sullo schermo il pulsante di chiusura finestra
                                              Draw_CloseWindow_Button (5, 2, WndHandle);
                                              Draw_MinimizeWindow_Button (20, 2, WndHandle);
                                              Draw_Title_Button (30, 2, WindowData[WndHandle].WndLength, 12, WndHandle);
                
                                              // e richiamiamo la funzione che si occupa di scrivere le barre di scrolling sullo schermo:
                                              ndLP_DrawScrollerRectangles (WndHandle);
                                           
                                              // Se i menù di finestra sono supportati, provvedi a disegnare la menubar
                                              if (MenuSupported) INTERNAL_CreateClearMenuBar (WndHandle);
                                   
                                              // Segnaliamo che la MinWindowsBar potrebbe essere cambiata
                                              MinWindowsBarIsChanged=1;
                
                                              // Poni la finestra in primo piano se viene segnalato
                                              if (PutInTheFirstPlane) ndLP_PutElementAtTheTopOfWV (WndHandle);
                                    
                                          }
                                
                                          // Riattiva il Phoenix Graph Thread
                                          ndHAL_UnlockPhoenixGraphThread ();
                                
                                          // Eseguiamo un XWindowsRender: non rappresenta la finestra, poichè essa è
                                          // minimizzata, ma farà comparire l'icona nella MinWindowsBar
                                          ndHAL_UpdateWindowsRenderRegisters();                 
                               
                                          XWindowRender (WndHandle);
                               
                                          return 0;
                                          // Ritorna il valore 0 alla routine chiamante per segnalare che non ci
                                          // sono errori
                                      }
                                      else  // Il processo di allocazione del service space è fallito
                                      {
                                          free (NewWindowData._p);             // Disalloca la pagina logica
                                          free (NewWindowData._ws);            // Disalloca il window space
                                          free (NewWindowData._CharMap);       // Disalloca la CharMap
                                          free (NewWindowData._ColorCharMap);  // Disalloca la ColorCharMap
                            
                                          return ERR_MEMORY_OVERFLOW;        
                                      }
                                            
                                }
                                else  // Il processo di allocazione della color char map è fallito
                                {
                                   free (NewWindowData._p);       // Disalloca la pagina logica
                                   free (NewWindowData._ws);      // Disalloca il window space
                                   free (NewWindowData._CharMap); // Disalloca la CharMap
                            
                                   return ERR_MEMORY_OVERFLOW;      
                                }
                         
                         }
                         else  // Il processo di allocazione della char map è fallito
                         {
                            free (NewWindowData._p);       // Disalloca la pagina logica
                            free (NewWindowData._ws);      // Disalloca il window space
                            
                            return ERR_MEMORY_OVERFLOW;   
                         }   
                                    
                     }
                     else    // Il processo di allocazione del window space è fallito
                     {
                        free (NewWindowData._p);  // Disalloca la pagina logica
                        return ERR_MEMORY_OVERFLOW;     
                     }
                     
                  }
                  else      // Il processo di allocazione della pagina logica è fallito
                     return ERR_MEMORY_OVERFLOW;        
                }
                else         // La finestra andrebbe fuori dall'area funzionale
                  return  ERR_WINDOW_OVER_FUNC_AREA;
             }
             else    // La finestra richiesta sarebbe troppo piccola
               return (ERR_WINDOW_IS_TOO_SMALL);  
                             
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
       
}




char ndLP_MoveWindow (unsigned short int NewPosX, unsigned short int NewPosY, unsigned char WndHandle)
/*
     Questa funzione ha il compito di spostare la finestra nella nuova posizione. 
     Sono previsti i seguenti codici di errore:
     
            WINDOW_IS_NOT_ALLOCATED    se la finestra non è assegnata
            ERR_WRONG_PARAMETERS       parametri non validi
*/
{
  int IntPosX1, IntPosX2, IntPosY1, IntPosY2;
  
  if (!ndInitSystemExecuted) ndInitSystem();

  if (WindowData [WndHandle].WindowIsActive)
  {
     if ((NewPosX<Windows_MaxScreenX) && (NewPosY<Windows_MaxScreenY))
     {
             IntPosX1=NewPosX;
             IntPosY1=NewPosY;
             IntPosX2=NewPosX + WindowData [WndHandle].WndLength;
             IntPosY2=NewPosY + WindowData [WndHandle].WndHeight;
        
             if ( (IntPosX2<Windows_MaxScreenX) && (IntPosY2<Windows_MaxScreenY) )
             {
                  ndHAL_LockPhoenixGraphThread ();
                  
                  {
                       WindowData [WndHandle].WndLimitX1=IntPosX1;
                       WindowData [WndHandle].WndLimitX2=IntPosX2;
            
                       WindowData [WndHandle].WndLimitY1=IntPosY1;
                       WindowData [WndHandle].WndLimitY2=IntPosY2;
                  }
                  
                  ndHAL_UnlockPhoenixGraphThread ();
        
                  // Una volta che sono stati cambiati i parametri all'interno di WindowData, è
                  // sufficiente eseguire un nuovo rendering perchè la finestra sia spostata
        
                  ndHAL_UpdateWindowsRenderRegisters ();     
                  WindowsRender();
                  
                  return 0;
             }
             else    // Parametri di coordinata errati (PosX2 e PosY2)
             {
                  return ERR_WRONG_PARAMETERS;   
             }
     }
     else    // Parametri di coordinata errati (PosX1 e PosY1)
     {
         return ERR_WRONG_PARAMETERS;
     }
  }
  else
  {
      return ERR_WINDOW_IS_NOT_ALLOCATED;
  }
}


char ndLP_MoveWindowWithAutoResize (unsigned short int NewPosX, unsigned short int NewPosY, unsigned char WndHandle)
/*
     Questa funzione ha il compito di spostare la finestra nella nuova posizione. 
     
     Questa routine ha un sistema di autoresize che cerca di correggere eventuali
     errori. Se i parametri sono errati, il sistema tenterà di ridimensionare la
     finestra per rispettare le nuove coordinate
     
     Sono previsti i seguenti codici di errore:
     
            WINDOW_IS_NOT_ALLOCATED    se la finestra non è assegnata
            ERR_WRONG_PARAMETERS       parametri non validi
*/
{
  int IntPosX1, IntPosX2, IntPosY1, IntPosY2;
  int NewLenX, NewLenY;
  char ErrRep;

  if (!ndInitSystemExecuted) ndInitSystem();

  if (WindowData [WndHandle].WindowIsActive)
  {
     if ((NewPosX<Windows_MaxScreenX) && (NewPosY<Windows_MaxScreenY))
     {
         IntPosX1=NewPosX;
         IntPosY1=NewPosY;
         IntPosX2=NewPosX + WindowData [WndHandle].WndLength;
         IntPosY2=NewPosY + WindowData [WndHandle].WndHeight;
    
         if ( (IntPosX2<Windows_MaxScreenX) && (IntPosY2<Windows_MaxScreenY) )
         {
              ndHAL_LockPhoenixGraphThread ();
              
              {
                   WindowData [WndHandle].WndLimitX1=IntPosX1;
                   WindowData [WndHandle].WndLimitX2=IntPosX2;
        
                   WindowData [WndHandle].WndLimitY1=IntPosY1;
                   WindowData [WndHandle].WndLimitY2=IntPosY2;
              }
              
              ndHAL_UnlockPhoenixGraphThread ();
    
              // Una volta che sono stati cambiati i parametri all'interno di WindowData, è
              // sufficiente eseguire un nuovo rendering perchè la finestra sia spostata
    
              ndHAL_UpdateWindowsRenderRegisters ();     
              WindowsRender();
              
              return 0;
         }
         else
         {
             if (IntPosX2>Windows_MaxScreenX-1) IntPosX2=(Windows_MaxScreenX-1);
             if (IntPosY2>Windows_MaxScreenY-1) IntPosY2=(Windows_MaxScreenY-1);
    
             NewLenX=IntPosX2-IntPosX1;
             NewLenY=IntPosY2-IntPosY1;
    
             // E' necessario prima tentare il ridimensionamento della finestra
             
             ErrRep=ndLP_ResizeWindow (NewLenX, NewLenY, WndHandle);
             if (ErrRep!=0) return ErrRep;
             
             // Ridimensionamento effettuato. Ora sposta la finestra
             return ndLP_MoveWindow (IntPosX1, IntPosY1, WndHandle);
         }
         
     }
     else    // Parametri di coordinata errati (PosX1 e PosY1)
     {
         return ERR_WRONG_PARAMETERS;
     }   
  }
  else
  {
      return ERR_WINDOW_IS_NOT_ALLOCATED;
  }
}



char ndLP_PutWindowInFirstPlane (unsigned char WndHandle)
/*
   Questa funzione pone la finestra identificata da WndHandle in primo piano.

   La funzione può ritornare il codice di errore
      ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
    if (!ndInitSystemExecuted) ndInitSystem();

    if (WindowData [WndHandle].WindowIsActive)
    {
       ndHAL_LockPhoenixGraphThread ();
       
       {
            ndLP_PutElementAtTheTopOfWV (WndHandle);
            MinWindowsBarIsChanged=1;
       }
       
       ndHAL_UnlockPhoenixGraphThread ();

       ndHAL_UpdateWindowsRenderRegisters ();
       WindowsRender();
       return 0;
    }
    else
        return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndLP_DestroyWindow (unsigned char WndHandle)
/* Ha la funzione di distruggere una finestra. Il relativo Handle viene disallocato
   per usi successivi.

   La funzione ritorna il codice di errore:

      ERR_WINDOW_WAS_NOT_ACTIVE         La finestra era già non attiva
*/
{
   unsigned short int Counter;

   if (!ndInitSystemExecuted) ndInitSystem();

   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WndHandle==StdOutWindowHandle)
         StdOutWindowActivated=0;        // Se si cerca di chiudere la finestra stdout/stderr
                                         // è necessario portare a zero l'apposita variabile in
                                         // modo da segnalarlo a NanoLibc.
                                         
      // Disattiviamo il Phoenix Graph Thread
      ndHAL_LockPhoenixGraphThread ();
      
      {                                  
          // Innanzitutto disattiviamo le trackbar allocate nel sistema
          
          for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
          {
              if (WindowData [WndHandle].TrackBar [Counter].IsActive) 
                ndTBAR_DestroyTrackBar (Counter, WndHandle, 1, 0, NORENDER);
                  // Parametro Only-Logical posto a 1 in modo da cancellare solo logicamente
          }
          
          // Ora cancelliamo gli altri pulsanti di classe 4 usati dall'utente
          
          for (Counter=0; Counter<__NRBUTTONSALLOCABLE; Counter++)
          {
             if (WindowData[WndHandle].ButtonClass4_Data[Counter].ButtonIsActivated)
                ndCTRL_DestroyButton (Counter, WndHandle, 1, 0, NORENDER);
                  // Parametro Only-Logical posto a 1 in modo da cancellare solo logicamente
          }
          
          // Cancelliamo anche tutte le textarea presenti
                                              
          for (Counter=0; Counter<__NRTEXTBOXALLOCABLE; Counter++)
          {
             if (WindowData[WndHandle].TextBox[Counter].IsActive)
                ndTBOX_DestroyTextArea (Counter, WndHandle, 1, 0, NORENDER);
                // Parametro Only-Logical posto a 1 in modo da cancellare solo logicamente
          }
    
          // Disattiviamo i pulsanti di classe 3
          
          for (Counter=0; Counter<7; Counter++) WindowData [WndHandle].ButtonClass3_Data[Counter].ButtonIsActivated=0;
                      
          free (WindowData [WndHandle]._p);   // Disalloca lo spazio riservato alla pagina logica   
          free (WindowData [WndHandle]._ws);  // Disalloca lo spazio riservato al Window Space
          
          free (WindowData [WndHandle]._CharMap);        // Disalloca CharMap e ColorCharMap
          free (WindowData [WndHandle]._ColorCharMap);
          
          if (WindowData [WndHandle].MenuSupported) 
          {
             free (WindowData [WndHandle]._ss);
             ndWMENU_DestroyL1SubMenu (WndHandle, "");
          }
                      
          WindowData [WndHandle].WindowIsActive=0;
          NrWindowsAllocated--;
    
          ndLP_RemoveElementFromWV (WndHandle);
          MinWindowsBarIsChanged=1;
          
      }
      
      // Riattiviamo il Phoenix Graph Thread
      ndHAL_UnlockPhoenixGraphThread ();

      ndHAL_UpdateWindowsRenderRegisters ();
      WindowsRender();
      return 0;
   }
      else
        return ERR_WINDOW_WAS_NOT_ACTIVE;
}


char ndLP_ChangeWndTitle (char *NewTitle, TypeColor NewColorTitle, TypeColor NewBGColorTitle, unsigned char WndHandle)
/*
     Questa funzione provvede a cambiare il titolo della finestra individuata da WndHandle,
     ovvero il colore della stessa.

     La funzione restituisce un codice di errore
        ERR_WINDOW_IS_NOT_ALLOCATED
*/
{
    unsigned short int PosTitoloX, PosTitoloY, LunghezzaTitolo, CounterX;

    if (!ndInitSystemExecuted) ndInitSystem();

    if (WindowData [WndHandle].WindowIsActive)
    {
       // Provvediamo al salvataggio del titolo nel buffer interno
       LunghezzaTitolo=strlen (NewTitle);

       if (LunghezzaTitolo>58) LunghezzaTitolo=58;
       // In caso di caratteri in più essi vengono semplicemente troncati

       for (CounterX=0; CounterX<LunghezzaTitolo; CounterX++)
         WindowData [WndHandle].WindowTitleChar[CounterX]=*(NewTitle+CounterX);
         // Copia nel buffer interno WindowTitleChar il titolo fornito dalla
         // routine chiamante

       WindowData [WndHandle].WindowTitleChar[CounterX]=0;
         // Assicura che la stringa termini con un carattere 0 anche in caso di
         // troncamento

       WindowData [WndHandle].WindowTitle=&(WindowData [WndHandle].WindowTitleChar[0]);

       WindowData [WndHandle].ColorTitle   =NewColorTitle;     // Cambiato anche il colore del titolo
       WindowData [WndHandle].ColorBGTitle =NewBGColorTitle;   // Ed il colore di background del titolo

       // Adesso modifichiamo la barra del titolo
       ndLP_DrawRectangle (30, 0, WindowData[WndHandle].WndLength, 12, NewBGColorTitle, WindowData[WndHandle].ColorBorder, WndHandle, RENDER);

       // Ora scriviamo il titolo della finestra: lo poniamo al centro dello schermo.
          LunghezzaTitolo=strlen (WindowData [WndHandle].WindowTitle);

          if ( (LunghezzaTitolo*8)<=WindowData [WndHandle].WndLength )
          {
             PosTitoloX=((WindowData [WndHandle].WndLength)-LunghezzaTitolo*8)/2;
             PosTitoloY=2;

             ndLP_Write (PosTitoloX, PosTitoloY, WindowData [WndHandle].WindowTitle,
             WindowData[WndHandle].ColorTitle, WindowData[WndHandle].ColorBGTitle, WndHandle, NORENDER);
          }
          else
          {
             PosTitoloX=((WindowData [WndHandle].WndLength)-3*8)/2;
             PosTitoloY=2;

             ndLP_Write (PosTitoloX, PosTitoloY, "...",
             WindowData[WndHandle].ColorTitle, WindowData[WndHandle].ColorBGTitle, WndHandle, NORENDER);
          }

       MinWindowsBarIsChanged=1;

       // Una render window rende evidente la modifica
       ndHAL_UpdateWindowsRenderRegisters ();
       WindowsRender();
       return 0;
    }
    else
       return ERR_WINDOW_IS_NOT_ALLOCATED;
}




float ndLP_GetWindowROIPosX (char Frac, char WndHandle)
/*
     Questa funzione fornisce la posizione X della ROI all'interno della finestra WndHandle
     
     Se viene settato ad 1 il parametro Frac, la routine restituirà la posizione
     X sotto forma di percentuale (0-100). 
     
     La routine restituisce i seguenti codici di errore
       ERR_WINDOW_IS_NOT_ALLOCATED      La finestra non è allocata    
*/
{
    if (!ndInitSystemExecuted) ndInitSystem();

    if (WindowData [WndHandle].WindowIsActive)
    {
        if (!Frac)
        {
            return ( (float)(WindowData [WndHandle].ROI_PosX) );      
        }
        else
        {
            /*
                Dalla relazione 
                
                RectNewROIPosX = (FracNewROIPosX/100) * (WsWndLength - ROILenX)
                
                risolvendo rispetto a FracNewROIPosX
                
                FracNewROIPosX = 100*(RectNewROIPosX)/(WsWndLength - ROILenX)
            */
            
            return ( (float)( (100*WindowData [WndHandle].ROI_PosX)/(WindowData [WndHandle].WSWndLength - WindowData [WndHandle].ROI_LenX) ) );
        }           
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;   
}



float ndLP_GetWindowROIPosY (char Frac, char WndHandle)
/*
     Questa funzione fornisce la posizione Y della ROI all'interno della finestra WndHandle
     
     Se viene settato ad 1 il parametro Frac, la routine restituirà la posizione
     Y sotto forma di percentuale (0-100). 
     
     La routine restituisce i seguenti codici di errore
       ERR_WINDOW_IS_NOT_ALLOCATED      La finestra non è allocata    
*/
{
    if (!ndInitSystemExecuted) ndInitSystem();

    if (WindowData [WndHandle].WindowIsActive)
    {
        if (!Frac)
        {
            return ( (float)(WindowData [WndHandle].ROI_PosY) );      
        }
        else
        {
            return ( (float)( (100*WindowData [WndHandle].ROI_PosY)/(WindowData [WndHandle].WSWndHeight - WindowData [WndHandle].ROI_LenY) ) );
        }           
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;   
}








char ndLP_ChangeWindowROI (float NewROIPosX, float NewROIPosY, int Attribute, unsigned char WndHandle)
/*
     Questa funzione provvede a cambiare la posizione della ROI all'interno della finestra 
     WndHandle. 
     
     La funzione ha un comportamento diverso a seconda dei bit settati nel parametro a 32 bit
     Attribute. 
     
     La routine può cambiare la sola coordinata X della ROI, la sola coordinata Y, o 
     entrambe. L'utente segnala quali dimensioni devono essere modificate settando ad
     1 opportuni bit nel parametro Attribute. Per farlo, usate le costanti manifeste
     ROI_POSX o ROI_POSY o entrambe. Se l'utente non usa nessuna di queste due
     costanti manifeste, allora il sistema sottointende che è necessario 
     agire su entrambe le coordinate (comportamento di default).
          
     La routine, in ogni caso, accetta in ingresso sempre due valori per X ed Y: il 
     valore da non modificare viene semplicemente ignorato. 
     
     Se l'utente non specifica l'opzione "fractional", i parametri NewROIPosX e NewROIPosY
     contengono le nuove coordinate X ed Y del pixel in alto a sinistra della ROI.
     Tali nuove coordinate devono sottostare ai seguenti vincoli: 
            
     NewROIPosX>0 NewROIPosY>0

       NewROIPosX + ROILenX <= WSWndLength   >>>>>>   NewROIPosX <= WSWndLength - ROILenX
       NewROIPosY + ROILenY <= WSWndHeight   >>>>>>   NewROIPosY <= WSWndHeight - ROILenY

     Quindi, in ultima analisi, valgono le condizioni

       0 < NewROIPosX < WSWndLength - ROILenX
       0 < NewROIPosY < WSWndLength - ROILenY
       
     Se tali condizioni non sono rispettate, si otterrà in uscita un codice di 
     errore ERR_WRONG_ROI_COORD.
     
     Si noti che per usare correttamente la routine in tale modalità, è necessario
     che il codice conosca in anticipo le dimensioni di WSWndLength, WSWndHeight, 
     ROILenX, ROILenY, e la posizione della ROI attuale. 
     
     Tale problema può essere risolto usando l'opzione fractional. Tale opzione
     può essere attivata attivando un opportuno bit nel parametro attribute:
     usate la costante manifesta ROI_FRAC.
     
     Quando l'opzione fractional è attivata, i parametri NewROIPosX e NewROIPosY
     possono assumere solo i valori compresi tra 0 e 100 (è consentito l'uso 
     di valori decimali)
     
     In queste condizioni, se si pone NewROIPosX a 0, il cursore sulla scroll-bar
     orizzontale si sposterà completamente a sinistra, mentre se si pone NewROIPosY
     a 100, il cursore sulla scroll-bar orizzontale si sposterà completamente 
     a destra. Internamente, la routine non fa altro che eseguire l'operazione:
               
       RectNewROIPosX = (FracNewROIPosX/100) * (WsWndLength - ROILenX)
       RectNewROIPosY = (FracNewROIPosY/100) * (WsWndLength - ROILenY)
            
     Infine, l'opzione smart. Tale opzione può essere attivata eseguendo l'or del
     parametro attribute con la costante manifesta ROI_SMART.
     
     In questo caso, quando il valore immesso dall'utente è più piccolo di quello
     che può essere accettato in base ai vincoli precedenti, la routine corregge
     automaticamente il valore richiesto a 0. Quando invece il valore immesso
     dall'utente è più alto di quello che può essere accettato, la routine
     corregge automaticamente il valore richiesto al massimo consentito. 
     
     ndLP_ChangeWindowROI restituisce i seguenti codici di errore:
     
       ERR_WINDOW_IS_NOT_ALLOCATED      La finestra non è allocata
       ERR_WRONG_ROI_COORD              La ROI è errata
   
*/

{
    int  L0_NewROIPosX, L0_NewROIPosY;
    int  L1_NewROIPosX, L1_NewROIPosY;
    char BoolA,   BoolB;
    char ExecOnX, ExecOnY;
    
    if (!ndInitSystemExecuted) ndInitSystem();

    if (WindowData [WndHandle].WindowIsActive)
    {
        // Se il sistema non vede settati i bit ExecOnX ed ExecOnY, suppone che 
        // occorra agire su entrambe le coordinate
        
        ExecOnX = ( (Attribute & ROI_POSX) == ROI_POSX );
        ExecOnY = ( (Attribute & ROI_POSY) == ROI_POSY );
        
        if ((!ExecOnX) && (!ExecOnY))
        {
            ExecOnX=1;
            ExecOnY=1;           
        }
        
        // Operiamo sulla coordinata X
        
        if (ExecOnX)
        {
            // Esegui la correzione fractional
            
            if ( !((Attribute & ROI_FRAC)==ROI_FRAC) )
            {
                L0_NewROIPosX = (int)(NewROIPosX);
            }         
            else    // Modalità fractional attivata
            {
                L0_NewROIPosX = (int)(NewROIPosX/100.0F)*(WindowData [WndHandle].WSWndLength - WindowData [WndHandle].ROI_LenX);           
            } 
            
            // Esegui la correzione smart
            
            if ( !((Attribute & ROI_SMART)==ROI_SMART) )
            {
                L1_NewROIPosX = L0_NewROIPosX;
            }
            else    // Modalità smart attivata            
            {                
                if (L0_NewROIPosX<0)
                {
                    L1_NewROIPosX = 0;               
                }
                else
                {
                    if (L0_NewROIPosX > (WindowData [WndHandle].WSWndLength - WindowData [WndHandle].ROI_LenX))
                    {
                        L1_NewROIPosX = (WindowData [WndHandle].WSWndLength - WindowData [WndHandle].ROI_LenX);
                    }
                    else    // Nessuna correzione richiesta perchè i valori sono corretti
                    {
                        L1_NewROIPosX = L0_NewROIPosX;    
                    }
                }            
            }   
        }
        else    // Non è stato richiesto di operare sulla coordinata X: prendi il valore attuale
        {
            L1_NewROIPosX = WindowData [WndHandle].ROI_PosX;    
        }
        
        
        
        // Operiamo sulla coordinata Y
        
        if (ExecOnY)
        {
            // Esegui la correzione fractional
            
            if ( !((Attribute & ROI_FRAC)==ROI_FRAC) )
            {
                L0_NewROIPosY = (int)(NewROIPosY);      
            }         
            else    // Modalità fractional attivata
            {
                L0_NewROIPosY = (int)(NewROIPosY/100.0F)*(WindowData [WndHandle].WSWndHeight - WindowData [WndHandle].ROI_LenY);           
            } 
            
            // Esegui la correzione smart
            
            if ( !((Attribute & ROI_SMART)==ROI_SMART) )
            {
                L1_NewROIPosY = L0_NewROIPosY;
            }
            else    // Modalità smart attivata            
            {                
                if (L0_NewROIPosY<0)
                {
                    L1_NewROIPosY = 0;               
                }
                else
                {
                    if (L0_NewROIPosY > (WindowData [WndHandle].WSWndHeight - WindowData [WndHandle].ROI_LenY))
                    {
                        L1_NewROIPosY = (WindowData [WndHandle].WSWndHeight - WindowData [WndHandle].ROI_LenY);
                    }
                    else    // Nessuna correzione richiesta perchè i valori sono corretti
                    {
                        L1_NewROIPosY = L0_NewROIPosY;    
                    }
                }            
            }   
        }
        else    // Non è stato richiesto di operare sulla coordinata Y: prendi il valore attuale
        {
            L1_NewROIPosY = WindowData [WndHandle].ROI_PosY;    
        }
        
        // Eseguiamo il cambiamento di coordinate
        
        BoolA = (( L1_NewROIPosX + WindowData [WndHandle].ROI_LenX) <= WindowData [WndHandle].WSWndLength);
        BoolB = (( L1_NewROIPosY + WindowData [WndHandle].ROI_LenY) <= WindowData [WndHandle].WSWndHeight);

        if ( BoolA && BoolB )                  // Se la condizione che abbiamo detto è verificata
        {           
            // Blocca il Phoenix Graph Thread
            ndHAL_LockPhoenixGraphThread ();
            
            {
                WindowData [WndHandle].ROI_PosX = L1_NewROIPosX;
                WindowData [WndHandle].ROI_PosY = L1_NewROIPosY;
               
                // Esegui il ricalcolo delle coordinate della ROI
                WindowData [WndHandle].ROI_PosX1 = WindowData [WndHandle].ROI_PosX;
                WindowData [WndHandle].ROI_PosY1 = WindowData [WndHandle].ROI_PosY;
                WindowData [WndHandle].ROI_PosX2 = WindowData [WndHandle].ROI_PosX + WindowData [WndHandle].ROI_LenX;
                WindowData [WndHandle].ROI_PosY2 = WindowData [WndHandle].ROI_PosY + WindowData [WndHandle].ROI_LenY;
            }

            // Riattiva il Phoenix Graph Thread
            ndHAL_UnlockPhoenixGraphThread ();

            ndHAL_UpdateWindowsRenderRegisters ();
            XWindowRender (WndHandle);
            // Questa aggiornerà i pixel nell'area attiva nr. 3, immettendogli quelli della nuova ROI

            return 0;
        }
        else
          return ERR_WRONG_ROI_COORD;
        
    }
    else
       return ERR_WINDOW_IS_NOT_ALLOCATED;   
}















char ndLP_SetupCloseWndCallback (void *CloseWndCallback, ndint64 WndInfoField, char NoNotifyToMouseControl, char WndHandle)
/*
    Questa routine permette di impostare la CloseWndCallback relativa ad una finestra.
    
    Questa callback viene richiamata ogni qualvolta l'utente preme il tasto di 
    chiusura finestra. Se si impone CloseWndCallback pari a 0, viene disattivata
    la callback relativa a quella finestra. 
    
    Ricordiamo che una CloseWndCallback ha prototipo: 
               
       ndint64 CloseWndCallback (char WndHandle, ndint64 WndInfoField);
       
    Il parametro a 64 bit WndInfoField costituisce un valore che verrà automaticamente
    passato alla Callback quando si cercherà di chiudere la finestra. 
    
    Il parametro NoNotifyToMouseControl condiziona il comportamento di MouseControl:
    se viene impostato a 1, il sistema esegue la CloseCallback ma non esce dal loop
    MouseControl, altrimenti la Callback sarà eseguita e MouseControl uscirà dal
    proprio loop restituendo alla routine chiamante un messaggio che indica
    un evento di tipo CLOSEWNDCALLBACK_HAS_BEEN_EXEC
    
    Sono previsti i seguenti codici di errore:
      ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata    
*/
{
    if (!ndInitSystemExecuted) ndInitSystem();

    if (WindowData [WndHandle].WindowIsActive)
    {
       WindowData [WndHandle].PntToCloseWndCallback  = CloseWndCallback;
       WindowData [WndHandle].WndInfoField           = WndInfoField;
       WindowData [WndHandle].NoNotifyToMouseControl = NoNotifyToMouseControl;
       
       return 0;
    }
    else
       return ERR_WINDOW_IS_NOT_ALLOCATED; 
}


char ndLP_GetUserAreaInfo (char WndHandle, int *Addr, int *Size)
/* -----------------------------------------------------------------------------
   Nanodesktop mette a disposizione di ogni utente un area di 32 bytes (ossia,
   il valore previsto dalla costante manifesta __USERAREASIZE) che può
   essere utilizzata dai programmi applicativi per memorizzare nella finestra
   delle informazioni di servizio. 
   
   E' necessario ricordare che queste informazioni vengono comunque perse quando
   la finestra viene distrutta. 
   
   Questa routine restituisce l'indirizzo a 32 bit del primo byte della user area
   e le dimensioni.
   
   Sono previsti i seguenti codici di errore:
        ERR_WINDOW_IS_NOT_ALLOCATED          La finestra non è allocata
   ------------------------------------------------------------------------------
*/
{
   if (WindowData [WndHandle].WindowIsActive)
   {
       *Addr = &(WindowData [WndHandle].UserArea [0]);
       *Size = __USERAREASIZE;
       
       return 0;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndLP_SetFont (unsigned char FntHandle, char WndHandle)
/*
   Questa routine provvede a settare il font per le routines di tipo LP.
   
   Le routines di tipo LP supportano esclusivamente i font di tipo TEXT e non
   i font di tipo GRAPHICS.
   
   Cambiando questo valore, è possibile settare il font con cui verrà visualizzato
   il titolo della prossima finestra. La routine agisce esclusivamente 
   sulla finestra WndHandle 
   
   Sono previsti i seguenti codici di errore:
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata  
        ERR_FONTHDL_IS_NOT_ACTIVE     Il font handle non è attivo
        ERR_GRAPHFONT_NOT_SUPPORTED   I graphfont non sono supportati
*/
{
   if (!ndInitSystemExecuted) ndInitSystem();

   if (WindowData [WndHandle].WindowIsActive)
   {
           if (FontHandlesArray [FntHandle].IsActive)
           {                   
                  if ( (FontHandlesArray [FntHandle].features & NDTEXTFONT) == NDTEXTFONT )
                  {
                     WindowData [WndHandle].ndLP_CurrentFntHandle = FntHandle;
                     // Registra il nuovo text-font corrente
                     
                     return 0;
                  }
                  else return ERR_GRAPHFONT_NOT_SUPPORTED;
           }
           else
               return ERR_FONTHDL_IS_NOT_ACTIVE;          
   }
    else
       return ERR_WINDOW_IS_NOT_ALLOCATED; 
}





/*
    --------------------------------------------------------------------------
    --------------------------------------------------------------------------
    OLD ROUTINES
    --------------------------------------------------------------------------
    --------------------------------------------------------------------------
*/



//   Vecchie routines di allocazione finestra (deprecate e mantenute solo
//   per ragioni di compatibilità
   

char ndLP_AllocateWindowComplex      (unsigned short int _PosWindowX1,
                                      unsigned short int _PosWindowY1,
                                      unsigned short int _PosWindowX2,
                                      unsigned short int _PosWindowY2,
                                      char *_WindowTitle,
                                      TypeColor _ColorTitle,
                                      TypeColor _ColorBGTitle,
                                      TypeColor _ColorWindow,
                                      TypeColor _ColorBorderWindow,
                                      unsigned short int _WSLength,
                                      unsigned short int _WSHeight,
                                      unsigned short int Attribute)
/*
  Questa routine si occupa di allocare una finestra. La routine deve prima
  verificare qual è la finestra che in quel momento è libera, poi deve
  reinizializzarla, ed infine deve fornire alla routine chiamante un
  handle per la finestra.
  
  Il parametro Attribute, è un parametro a 16 bit che fornisce informazioni
  sul comportamento della finestra. I seguenti valori rappresentano bit del
  campo che modificano il comportamento della finestra
  
  1     AUTOFIX           Le dimensioni del window space sono calcolate 
                          automaticamente
  2     NO_RESIZEABLE     La finestra non può essere ridimensionata
  4     MENUSUPPORTED     I menù di finestra sono supportati
  8     USE_POSGEN        Usa il generatore PosGen

  Se il flag AUTOFIX è settato i parametri _WSLength e _WSHeight
  vengono ignorati e le dimensioni del window space saranno settate 
  automaticamente alle dimensioni della finestra sullo schermo. Tuttavia,
  la finestra potrà essere comunque ridimensionata tramite GUI o tramite
  una chiamata a ndLP_ResizeWindow.
  
  Se il flag NO_RESIZEABLE è settato, non solo le dimensioni del window
  space saranno determinate automaticamente, ma sarà anche impossibile
  ridimensionare in seguito la finestra. 
  
  Se il flag USE_POSGEN è attivato, la routine determinerà le quantità
  _LenX = PosWindowX2 - PosWindowX1
  _LenY = PosWindowY2 - PosWindowY1
  
  e dopo si farà fornire le coordinate necessarie dal generatore POSGEN.

  Quando la routine restituisce un valore negativo, allora siamo in presenza
  di un errore. I codici di errore sono:

        ERR_NO_WINDOW_IS_AVAILABLE         Non ci sono più finestre disponibili
        ERR_WINDOW_IS_TOO_SMALL            Finestra troppo piccola
        ERR_WS_SPACE_REQUEST_IS_TOO_LARGE  L'estensione della Window Space richiesta è
                                           troppo grande
        ERR_WINDOW_OVER_THE_FUNC_AREA      La finestra coprirebbe l'area funzionale
        ERR_MEMORY_OVERFLOW                Overflow della memoria

  Questa routine pone la finestra in primo piano, ma minimizzata. E' necessaria
  la routine ndLP_MaximizeWindow per mostrare la finestra.

  Qualora l'utente richiesta una window space "più piccola" dell'area ROI della
  finestra, la routine adatta automaticamente la richiesta per la WindowSpace
  al valore della ROI della finestra, non essendo consentito di lavorare con
  WindowSpace che siano più piccole dell'area ROI della finestra

*/

{
   ndint64 Attribute64 = (Attribute | NDKEY_SETWSLENGTH (_WSLength) | NDKEY_SETWSHEIGHT (_WSHeight));
   
   return ndLP_CreateWindow (_PosWindowX1, _PosWindowY1,  _PosWindowX2, _PosWindowY2, _WindowTitle,
               _ColorTitle,  _ColorBGTitle, _ColorWindow, _ColorBorderWindow, Attribute64);         
}

  




char ndLP_AllocateWindow             (unsigned short int _PosWindowX1,
                                      unsigned short int _PosWindowY1,
                                      unsigned short int _PosWindowX2,
                                      unsigned short int _PosWindowY2,
                                      char *_WindowTitle,
                                      TypeColor _ColorTitle,
                                      TypeColor _ColorBGTitle,
                                      TypeColor _ColorWindow,
                                      TypeColor _ColorBorderWindow)

{
   return ndLP_CreateWindow (_PosWindowX1, _PosWindowY1,  _PosWindowX2, _PosWindowY2, _WindowTitle,
                 _ColorTitle,  _ColorBGTitle, _ColorWindow, _ColorBorderWindow, 0);                                  
}



char ndLP_AllocateWindow_MENU        (unsigned short int _PosWindowX1,
                                      unsigned short int _PosWindowY1,
                                      unsigned short int _PosWindowX2,
                                      unsigned short int _PosWindowY2,
                                      char *_WindowTitle,
                                      TypeColor _ColorTitle,
                                      TypeColor _ColorBGTitle,
                                      TypeColor _ColorWindow,
                                      TypeColor _ColorBorderWindow)

{
    return ndLP_CreateWindow (_PosWindowX1, _PosWindowY1,  _PosWindowX2, _PosWindowY2, _WindowTitle,
               _ColorTitle,  _ColorBGTitle, _ColorWindow, _ColorBorderWindow, MENUSUPPORTED);                                
}





char ndLP_AllocateWindow_NORESIZE    (unsigned short int _PosWindowX1,
                                      unsigned short int _PosWindowY1,
                                      unsigned short int _PosWindowX2,
                                      unsigned short int _PosWindowY2,
                                      char *_WindowTitle,
                                      TypeColor _ColorTitle,
                                      TypeColor _ColorBGTitle,
                                      TypeColor _ColorWindow,
                                      TypeColor _ColorBorderWindow)
{
    return ndLP_CreateWindow (_PosWindowX1, _PosWindowY1,  _PosWindowX2, _PosWindowY2, _WindowTitle,
              _ColorTitle,  _ColorBGTitle, _ColorWindow, _ColorBorderWindow, NO_RESIZEABLE);          
}












