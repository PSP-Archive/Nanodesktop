/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.2)
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
    ENGINE WINDOWS RENDER (PSP)
    ---------------------------------------------------------------------------
    Questo modulo fornisce le routine per il rendering delle finestre. Nelle 
    versioni di Nanodesktop che usano accelerazione hardware, queste routine
    sono specifiche per l'hardware e quindi vengono mantenute a livello HAL
    e non più al vecchio livello livello Logical Page. 

    L'HAL di Nanodesktop fornisce quattro funzioni per il render della finestra:
          ndHAL_UpdateWindowsRenderRegisters ();
          ndHAL_DoWindowsRenderNoSwap ();
          ndHAL_DoWindowsRender();
          ndHAL_WindowsRender_CLIENT ();
          
    La prima di queste funzioni viene richiamata ogni volta che viene cambiata
    la posizione delle finestre (minimizzazione, massimizzazione ecc. ecc.). 
    Essa aggiorna i registri interni usati da HAL per la visualizzazione: tali
    registri posso dipendere dall'hardware. 
    
    ndHAL_DoWindowsRender, invece, contiene il codice relativo al renderer vero
    e proprio. La versione NoSwap si distingue per il fatto che non esegue lo
    swap del frame video, alla fine del processo di rendering, ma lascia tale
    compito alla routine chiamante.
    
    ndHAL_WindowsRender_CLIENT, è il client di WindowsRender che provvede alla
    serializzazione delle chiamate a WindowsRender in ambiente multithreading.
    ---------------------------------------------------------------------------
*/

#include <HAL_PSP\$_HAL_NDforND.h>

static unsigned int __attribute__((aligned(16))) ndlist[262144];
// Buffer destinato ad accogliere le richieste per l'acceleratore grafico della PSP

static unsigned short int __attribute__((aligned(16))) BufferAlpha [32][__VMAXSCREENX];
static unsigned short int __attribute__((aligned(16))) BufferBeta  [__VMAXSCREENY][32];
static unsigned short int __attribute__((aligned(16))) BufferGamma [32][32];         
// Buffers destinati ad accogliere le copie delle zone alpha, beta e gamma della
// minwindows bar                                
                               
/*
   ----------------------------------------------------------------------------------
   Routines ausiliarie del Windows Render
   ----------------------------------------------------------------------------------
   Queste routines si occupano di eseguire funzionalità di supporto a WindowsRender.
   Le routines sono per uso interno, e sono progettate per ottenere la massima
   velocità possibile
   ---------------------------------------------------------------------------------- 
*/

inline TypeColor INTERNAL_MathBlend (TypeColor Color1, TypeColor Color2)
/*
   Ricava il colore che si ottiene dal blend di due finestre sovrapposte.
   
   E' identica ad ndHAL_MathBlend solo che, per aumentare la velocità del
   sistema, questa routine è definita nello stesso modulo del WindowsRender:
   in questo modo il compilatore è obbligato ad inserirla inline. 
   
   Ciò ha effetti straordinariamente benefici sulla velocità 
*/
{
   register unsigned short int cRT, cGT, cBT;
   
   cRT =  ((Color1 & 0x001F)      )  +  ((Color2 & 0x001F) >> 3 );
   cGT =  ((Color1 & 0x03E0) >> 5 )  +  ((Color2 & 0x03E0) >> 8 );
   cBT =  ((Color1 & 0x7C00) >> 10)  +  ((Color2 & 0x7C00) >> 13);
   
   if (cRT>0x1F) cRT=0x1F;
   if (cGT>0x1F) cGT=0x1F;
   if (cBT>0x1F) cBT=0x1F;

   return ( (cBT<<10)+(cGT<<5)+cRT+0x8000 );
}


inline unsigned char INTERNAL_IsPixelInWindow (unsigned short int PosPixelX, unsigned short int PosPixelY, unsigned char WndHandle)
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




inline void INTERNAL_FindOwnerOfPixel (unsigned short int PosPixelX, unsigned short int PosPixelY, unsigned char *OwnerID)
/*
     Questa routine ha lo scopo di individuare qual è la finestra che "possiede" un certo pixel, ossia
     a quale finestra massimizzata sullo schermo appartiene quel pixel in quel dato momento.

     La routine risponde con l'handle della finestra. Se la routine risponde con un valore
     255, allora vuol dire che il pixel non appartiene a nessuna finestra in quel momento.
     
     La routine chiamante deve allocare una variabile di tipo unsigned char nel proprio
     stack, atta ad ospitare l'informazione richiesta. Di tale variabile viene passato
     l'indirizzo.

     Le coordinate del pixel sono espresse sempre in modo assoluto.
     
     La routine ha la stessa funzione di ndLP_FindOwnerOfPixel, ma a differenza di quest'ultima
     non esegue controlli di sicurezza, essendo progettata per la velocità.
*/

{
     register int Counter, IDWindow;
     
     for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
     {
         if (WindowsVector[Counter]!=255)
         {
             IDWindow=WindowsVector[Counter];    // La prima (o seconda, terza ecc. ecc.) finestra nel Windows Vector;

             if ( !(WindowData [IDWindow].Minimize) )       // Il processo di analisi deve andare avanti solo se la finestra non
                                                            // è minimizzata
             {                        
                 if (INTERNAL_IsPixelInWindow(PosPixelX, PosPixelY, IDWindow))
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

     return;
}





inline void INTERNAL_FindTwoOwnersOfPixel (unsigned short int PosPixelX, unsigned short int PosPixelY,
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
  register unsigned short int Counter;
  register char IHaveFound1, IHaveFound2;
  
  register int IDWindow;

  Counter=0;
  IHaveFound1=0;
  IHaveFound2=0;

  *OwnerID1=255;            // Impostiamo a 255 per sicurezza
  *OwnerID2=255;

  
  while (  (Counter<__NRWINDOWSALLOCABLE) && (! (IHaveFound1 && IHaveFound2) )  )
  {
     IDWindow = WindowsVector [Counter];

     if (IDWindow==255) break;          // Siamo a fine vettore. Esegui break

     if (!IHaveFound1)                // Non è stata ancora trovata la prima finestra
     {
            if  ( (!WindowData [IDWindow].Minimize) && ( INTERNAL_IsPixelInWindow (PosPixelX, PosPixelY, IDWindow) ) )
            {
                 IHaveFound1=1;
                 *OwnerID1=IDWindow;
            }
     }
     else        // Se la prima finestra è stata già trovata, stiamo cercando la seconda
     {
            if (!(WindowData [IDWindow].Minimize) && ( INTERNAL_IsPixelInWindow (PosPixelX, PosPixelY, IDWindow) ) )
            {
                 IHaveFound2=1;
                 *OwnerID2=IDWindow;
            }
     }
     
     Counter++;
  }
       
}


/*
   ----------------------------------------------------------------------------------
   Routine DrawMinWindowsBar
   ----------------------------------------------------------------------------------
*/


inline void INTERNAL_DrawMinWindowsBar ()
{
    register unsigned short int CounterX, CounterY, CounterA;
    register unsigned short int NrWindowsMinimized;
    register unsigned short int NotActiveFBPage;
    register unsigned short int CounterWindow;
    register unsigned short int ButtonLength;
    register unsigned short int CharsInButton;
    register unsigned char Carattere;
    register unsigned short int PosButtonX, PosButtonY;
    register TypeColor ButtonColor;
    register int ADR;

    unsigned short int ButtonX1, ButtonY1, ButtonX2, ButtonY2;

    NotActiveFBPage=!ActiveFBPage;
  
               
    if (MinWindowsBarIsChanged)            
    {
                    // Procediamo innanzitutto pulendo l'area: questo cancellerà l'eventuale
                    // puntatore del mouse che si trovi in questo punto
    
                    // Disegna la linea verticale ed orizzontale bianca sullo schermo
    
                    for (CounterX=0; CounterX<MaxScreenX; CounterX++)
                       ndHAL_PutPixelToPage (CounterX, Windows_MaxScreenY, 0xFFFF, NotActiveFBPage);
                
                    for (CounterY=0; CounterY<MaxScreenY; CounterY++)
                       ndHAL_PutPixelToPage (Windows_MaxScreenX, CounterY, 0xFFFF, NotActiveFBPage);
                 
                    // Pulisci area alpha
                 
                    for (CounterY=Windows_MaxScreenY+1; CounterY<MaxScreenY; CounterY++)
                       for (CounterX=0; CounterX<=Windows_MaxScreenX-1; CounterX++)
                           ndHAL_PutPixelToPage (CounterX, CounterY, 0, NotActiveFBPage);
                
                    // Pulisci area beta
                   
                    for (CounterY=0; CounterY<=Windows_MaxScreenY-1; CounterY++)
                       for (CounterX=Windows_MaxScreenX+1; CounterX<MaxScreenX; CounterX++)
                           ndHAL_PutPixelToPage (CounterX, CounterY, 0, NotActiveFBPage);
                
                    // Pulisci area gamma
                   
                    for (CounterY=Windows_MaxScreenY+1; CounterY<MaxScreenY; CounterY++)
                       for (CounterX=Windows_MaxScreenX+1; CounterX<MaxScreenX; CounterX++)
                           ndHAL_PutPixelToPage (CounterX, CounterY, 0, NotActiveFBPage);
                
                    // Iniziamo contando il numero di finestre minimizzate
                                   
                    NrWindowsMinimized=0;
                        
                    for (CounterX=0; CounterX<__NRWINDOWSALLOCABLE; CounterX++)
                       if ( (WindowData [CounterX].WindowIsActive) && (WindowData [CounterX].Minimize) )
                           NrWindowsMinimized++;
                
                    // Occupiamoci dei pulsanti di classe 1: il comportamento è diverso a seconda che ci siano 
                    // o meno finestre minimizzate
                     
                    if (NrWindowsMinimized>0)
                    {
                               ButtonLength = MinWindowsBar_LenX / NrWindowsMinimized;
                               CounterWindow=0;
                        
                               for (CounterA=0; CounterA<__NRWINDOWSALLOCABLE; CounterA++)
                               {
                                  if ( (WindowData [CounterA].WindowIsActive) && (WindowData [CounterA].Minimize) )
                                  {
                                       ButtonX1 = MinWindowsBar_x1 + CounterWindow*ButtonLength;
                                       ButtonY1 = MinWindowsBar_y1 + 1;
                                       ButtonX2 = MinWindowsBar_x1 + (CounterWindow+1)*ButtonLength - 2;
                                       ButtonY2 = MinWindowsBar_y2 - 1;
                        
                                       ButtonColor = WindowData [CounterA].ColorBGTitle;
                        
                                       for (CounterY=ButtonY1; CounterY<=ButtonY2; CounterY++)
                                       {
                                           for (CounterX=ButtonX1; CounterX<=ButtonX2; CounterX++)
                                           {
                                               ndHAL_PutPixelToPage (CounterX, CounterY, ButtonColor, NotActiveFBPage);
                                           }
                                       }
                        
                                       CharsInButton = (ButtonLength - 4) / 8;
                                       
                                       for (CounterX=0; (CounterX<CharsInButton); CounterX++)
                                       {
                                           Carattere = WindowData [CounterA].WindowTitleChar [CounterX];
                        
                                           if (Carattere!=0)
                                              ndLL_WriteCharSpecial ( ButtonX1 + 2 + CounterX*8, ButtonY1 + 2, Carattere, COLOR_WHITE, NotActiveFBPage);
                                           else
                                              break;
                                       }
                        
                                       ButtonClass1_Data [CounterA].ButtonID=CounterA;
                                       ButtonClass1_Data [CounterA].ButtonIsActivated=1;
                                       ButtonClass1_Data [CounterA].x1=ButtonX1;
                                       ButtonClass1_Data [CounterA].y1=ButtonY1;
                                       ButtonClass1_Data [CounterA].x2=ButtonX2;
                                       ButtonClass1_Data [CounterA].y2=ButtonY2;
                        
                                       CounterWindow++;
                                  }
                                  else
                                  {
                                      ButtonClass1_Data [CounterA].ButtonIsActivated=0;
                                  }
                               }
                        
                    }
                    else // Se non ci sono finestre minimizzate, allora disattiva tutti i pulsanti di classe 1
                    {
                               for (CounterA=0; CounterA<__NRWINDOWSALLOCABLE; CounterA++)
                               {
                                      ButtonClass1_Data [CounterA].ButtonIsActivated=0;
                               }
                    }
                    
                    // Adesso occupiamoci dei pulsanti di classe 2
                    
                    if ( (NrWindowsAllocated>0) && (NrWindowsAllocated>NrWindowsMinimized) )
                    {
                              // Disegniamo il pulsante di allargamento finestra in orizzontale
                        
                              PosButtonX=Windows_MaxScreenX - 60;
                              PosButtonY=Windows_MaxScreenY + 3;
                        
                              for (CounterY=0; CounterY<8; CounterY++)
                              {
                                 for (CounterX=0; CounterX<20; CounterX++)
                                 {
                                     if (Button_Enlarge [CounterY][CounterX] == 'B')
                                        ndHAL_PutPixelToPage (PosButtonX+CounterX, PosButtonY+CounterY, 0, NotActiveFBPage);
                        
                                     if (Button_Enlarge [CounterY][CounterX] == 'W')
                                        ndHAL_PutPixelToPage (PosButtonX+CounterX, PosButtonY+CounterY, COLOR_LBLUE, NotActiveFBPage);
                                 }
                              }
                        
                              // e memorizziamo i parametri nel buffer per i pulsanti di classe 2.
                        
                              ButtonClass2_Data [0].ButtonID = ENLARGE_X;
                              ButtonClass2_Data [0].ButtonIsActivated = 1;
                              ButtonClass2_Data [0].x1 = PosButtonX;
                              ButtonClass2_Data [0].y1 = PosButtonY;
                              ButtonClass2_Data [0].x2 = PosButtonX+20;
                              ButtonClass2_Data [0].y2 = PosButtonY+8;
                        
                              // Disegniamo il pulsante di restringimento finestra in orizzontale
                        
                              PosButtonX=Windows_MaxScreenX - 30;
                              PosButtonY=Windows_MaxScreenY + 3;
                        
                              for (CounterY=0; CounterY<8; CounterY++)
                              {
                                 for (CounterX=0; CounterX<20; CounterX++)
                                 {
                                     if (Button_Smaller [CounterY][CounterX] == 'B')
                                        ndHAL_PutPixelToPage (PosButtonX+CounterX, PosButtonY+CounterY, 0, NotActiveFBPage);
                        
                                     if (Button_Smaller [CounterY][CounterX] == 'W')
                                        ndHAL_PutPixelToPage (PosButtonX+CounterX, PosButtonY+CounterY, COLOR_LBLUE, NotActiveFBPage);
                                 }
                              }
                        
                              // e memorizziamo i parametri nel buffer per i pulsanti di classe 2.
                        
                              ButtonClass2_Data [1].ButtonID = SMALLER_X;
                              ButtonClass2_Data [1].ButtonIsActivated = 1;
                              ButtonClass2_Data [1].x1 = PosButtonX;
                              ButtonClass2_Data [1].y1 = PosButtonY;
                              ButtonClass2_Data [1].x2 = PosButtonX+20;
                              ButtonClass2_Data [1].y2 = PosButtonY+8;
                        
                              // Disegniamo il pulsante di allargamento finestra in verticale
                        
                              PosButtonX=Windows_MaxScreenX + 3;
                              PosButtonY=Windows_MaxScreenY - 60;
                        
                              for (CounterY=0; CounterY<8; CounterY++)
                              {
                                 for (CounterX=0; CounterX<20; CounterX++)
                                 {
                                     if (Button_Enlarge [CounterY][CounterX] == 'B')
                                        ndHAL_PutPixelToPage (PosButtonX+CounterY, PosButtonY+CounterX, 0, NotActiveFBPage);
                        
                                     if (Button_Enlarge [CounterY][CounterX] == 'W')
                                        ndHAL_PutPixelToPage (PosButtonX+CounterY, PosButtonY+CounterX, COLOR_YELLOW, NotActiveFBPage);
                                 }
                              }
                        
                              // e memorizziamo i parametri nel buffer per i pulsanti di classe 2.
                        
                              ButtonClass2_Data [2].ButtonID = ENLARGE_Y;
                              ButtonClass2_Data [2].ButtonIsActivated = 1;
                              ButtonClass2_Data [2].x1 = PosButtonX;
                              ButtonClass2_Data [2].y1 = PosButtonY;
                              ButtonClass2_Data [2].x2 = PosButtonX+8;
                              ButtonClass2_Data [2].y2 = PosButtonY+20;
                        
                              // Disegniamo il pulsante di restringimento finestra in verticale
                        
                              PosButtonX=Windows_MaxScreenX + 3;
                              PosButtonY=Windows_MaxScreenY - 30;
                        
                              for (CounterY=0; CounterY<8; CounterY++)
                              {
                                 for (CounterX=0; CounterX<20; CounterX++)
                                 {
                                     if (Button_Smaller [CounterY][CounterX] == 'B')
                                        ndHAL_PutPixelToPage (PosButtonX+CounterY, PosButtonY+CounterX, 0, NotActiveFBPage);
                        
                                     if (Button_Smaller [CounterY][CounterX] == 'W')
                                        ndHAL_PutPixelToPage (PosButtonX+CounterY, PosButtonY+CounterX, COLOR_YELLOW, NotActiveFBPage);
                                 }
                              }
                        
                              // e memorizziamo i parametri nel buffer per i pulsanti di classe 2.
                        
                              ButtonClass2_Data [3].ButtonID = SMALLER_Y;
                              ButtonClass2_Data [3].ButtonIsActivated = 1;
                              ButtonClass2_Data [3].x1 = PosButtonX;
                              ButtonClass2_Data [3].y1 = PosButtonY;
                              ButtonClass2_Data [3].x2 = PosButtonX+8;
                              ButtonClass2_Data [3].y2 = PosButtonY+20;
                        
                    }
                    else           // Se il numero di finestre allocato è nullo, o se le finestre sono tutte minimizzate
                                   // occorre disattivare i pulsanti di classe 2
                    {
                              ButtonClass2_Data [0].ButtonIsActivated = 0;
                              ButtonClass2_Data [1].ButtonIsActivated = 0;
                              ButtonClass2_Data [2].ButtonIsActivated = 0;
                              ButtonClass2_Data [3].ButtonIsActivated = 0;
                    }
                
                    // Adesso provvediamo a salvare una copia della MinWindowsBar nei
                    // buffer alpha, beta e gamma: in questo modo non sarà necessario
                    // ridisegnare tutta la barra al frame successivo
                    
                    sceGuStart(GU_DIRECT,ndlist);   // Preparati ad inviare comandi al chip grafico della PSP
                    sceKernelDcacheWritebackAll();  // Pulisci la cache: necessario su PSP per evitare la corruzione dell'immagine 
  
                    ADR=AddrVideoMemory+NotActiveFBPage*PageSize;
                    
                    // Esegui la copia dell'area alpha usando il chip video della PSP
                 
                    sceGuCopyImage(GU_PSM_5551, 0, Windows_MaxScreenY,                              
                                                Windows_MaxScreenX, MaxScreenY-Windows_MaxScreenY,  
                                                __VMAXSCREENX, 
                                                ADR, 
                                                0, 0, __VMAXSCREENX, 
                                                BufferAlpha);
             
                    // Esegui la copia dell'area beta usando il chip video della PSP
             
                    sceGuCopyImage(GU_PSM_5551, Windows_MaxScreenX, 0,                              
                                                MaxScreenX-Windows_MaxScreenX, Windows_MaxScreenY,  
                                                __VMAXSCREENX, 
                                                ADR, 
                                                0, 0, 32, 
                                                BufferBeta);
             
             
                    // Esegui la copia dell'area gamma usando il chip video della PSP
             
                    sceGuCopyImage(GU_PSM_5551, Windows_MaxScreenX, Windows_MaxScreenY,      
                                                MaxScreenX-Windows_MaxScreenX, MaxScreenY-Windows_MaxScreenY,                      
                                                __VMAXSCREENX, 
                                                ADR, 
                                                0, 0, 32, 
                                                BufferGamma);
             
                    
                    sceGuTexSync();
                    sceGuFinish ();
                    sceGuSync (0,0);                           
                 
                    MinWindowsBarIsChanged=0;
                    return;
    }
    else  // La MinWindowsBar non è stata cambiata
    {
                    // Preparati ad eseguire il ripristino delle aree alpha, beta e gamma
                    // usando il chip grafico della PSP
 
                    sceGuStart(GU_DIRECT,ndlist);   // Preparati ad inviare comandi al chip grafico della PSP
                    sceKernelDcacheWritebackAll();  // Pulisci la cache: necessario su PSP per evitare la corruzione dell'immagine 
  
                    ADR=AddrVideoMemory+NotActiveFBPage*PageSize;
                    
                    // Ripristina l'area alpha usando il chip video della PSP
                 
                    sceGuCopyImage(GU_PSM_5551, 0, 0,                                             
                                                __VMAXSCREENX, MaxScreenY-Windows_MaxScreenY,     
                                                __VMAXSCREENX, 
                                                BufferAlpha, 
                                                0, Windows_MaxScreenY, __VMAXSCREENX, 
                                                ADR);
             
                    // Ripristina l'area beta usando il chip video della PSP
             
                    sceGuCopyImage(GU_PSM_5551, 0, 0,                                                
                                                MaxScreenX-Windows_MaxScreenX, Windows_MaxScreenY,   
                                                32, 
                                                BufferBeta, 
                                                Windows_MaxScreenX, 0, __VMAXSCREENX, 
                                                ADR);
             
                    // Ripristina l'area gamma usando il chip video della PSP
             
                    sceGuCopyImage(GU_PSM_5551, 0, 0,                                                          
                                                MaxScreenX-Windows_MaxScreenX, MaxScreenY-Windows_MaxScreenY,  
                                                32, 
                                                BufferGamma, 
                                                Windows_MaxScreenX, Windows_MaxScreenY, __VMAXSCREENX, 
                                                ADR);
             
                    
                    sceGuTexSync();
                    sceGuFinish ();
                    sceGuSync (0,0);                           
                 
                    return;
    }
                    
}







/*
   ----------------------------------------------------------------------------------
   Engine del Windows Render
   ----------------------------------------------------------------------------------
*/

void INTERNAL_FatalSystemErrorInWindowRender ()  
{
     /*
     ndLL_WriteLn ("Fatal Error: Window Render XY Vertex Overflow", COLOR_WHITE);
     ndDelay (1);
     ndSystemHalt (-1);
     */
}

void INTERNAL_ReInitVertexBuffer()
{
   int Counter;
   
   for (Counter=0; Counter<__MAXVERTEXALLOCABLE; Counter++)
   {
       ndXVertexBuffer [Counter].Enabled=0;
       ndYVertexBuffer [Counter].Enabled=0;
   }  
   
   NrXVertexEnabled=0;
   NrYVertexEnabled=0;    
}

void INTERNAL_PushElemToXVertexBuffer (unsigned short int X)
{
   int Counter;  
   char IHaveFound;  
     
   if (NrXVertexEnabled<__MAXVERTEXALLOCABLE)
   {
      IHaveFound=0;                                       
                                             
      if (NrXVertexEnabled>0)                // Verifica che questo vertex non sia già presente 
      {
         for (Counter=0; Counter<NrXVertexEnabled; Counter++)
         {
             if ((ndXVertexBuffer [Counter].Enabled) && (ndXVertexBuffer [Counter].PosX==X)) IHaveFound=1; 
         }
      }
      
      if (!IHaveFound)            // Se non hai trovato un vertex analogo registra il nuovo vertex
      {
         ndXVertexBuffer [NrXVertexEnabled].PosX = X;
         ndXVertexBuffer [NrXVertexEnabled].Enabled = 1;
      
         NrXVertexEnabled++;
      }
   }
   else
       INTERNAL_FatalSystemErrorInWindowRender ();  
}

void INTERNAL_PushElemToYVertexBuffer (unsigned short int Y)
{
   int Counter;  
   char IHaveFound;  
     
   if (NrYVertexEnabled<__MAXVERTEXALLOCABLE)
   {
      IHaveFound=0;                                       
                                             
      if (NrYVertexEnabled>0)                // Verifica che questo vertex non sia già presente 
      {
         for (Counter=0; Counter<NrYVertexEnabled; Counter++)
         {
             if ((ndYVertexBuffer [Counter].Enabled) && (ndYVertexBuffer [Counter].PosY==Y)) IHaveFound=1; 
         }
      }
      
      if (!IHaveFound)            // Se non hai trovato un vertex analogo registra il nuovo vertex
      {
         ndYVertexBuffer [NrYVertexEnabled].PosY = Y;
         ndYVertexBuffer [NrYVertexEnabled].Enabled = 1;
      
         NrYVertexEnabled++;
      }
   }
   else
       INTERNAL_FatalSystemErrorInWindowRender ();  
}


void INTERNAL_PushRectToVertexBuffer (unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2)
/*
   Questa routine è esclusivamente per uso interno. Essa provvede ad immettere
   le coordinate dei nuovi vertici all'interno dei buffers ndXVertex e ndYVertex
*/
{
   INTERNAL_PushElemToXVertexBuffer (x1);
   INTERNAL_PushElemToXVertexBuffer (x2);
   INTERNAL_PushElemToYVertexBuffer (y1);
   INTERNAL_PushElemToYVertexBuffer (y2);
}

void INTERNAL_OrderXVertexBuffer ()
/* 
   Questa routine provvede a riordinare il buffer X. Viene usato un algoritmo di
   tipo bubble-sort 
*/
{
   char IsChanged;
   int Counter;
   
   do
   {
      IsChanged=0;
       
      if (NrXVertexEnabled>=2)          // L'ordinamento ha senso solo se ci sono almeno 2 vertex attivi
      {
         for (Counter=0; Counter<(NrXVertexEnabled-1); Counter++)
         {
             if ( (ndXVertexBuffer [Counter].PosX) > (ndXVertexBuffer [Counter+1].PosX) )
             {
                  UShort_Swap ( &(ndXVertexBuffer [Counter].PosX), &(ndXVertexBuffer [Counter+1].PosX) );
                  IsChanged=1;
             }
         }
      }
   }
   while (IsChanged);
   
}


void INTERNAL_OrderYVertexBuffer ()
/* 
   Questa routine provvede a riordinare il buffer Y. Viene usato un algoritmo di
   tipo bubble-sort 
*/
{
   char IsChanged;
   int Counter;
   
   do
   {
      IsChanged=0;
       
      if (NrYVertexEnabled>=2)          // L'ordinamento ha senso solo se ci sono almeno 2 vertex attivi
      {
         for (Counter=0; Counter<(NrYVertexEnabled-1); Counter++)
         {
             if ( (ndYVertexBuffer [Counter].PosY) > (ndYVertexBuffer [Counter+1].PosY) )
             {
                  UShort_Swap ( &(ndYVertexBuffer [Counter].PosY), &(ndYVertexBuffer [Counter+1].PosY) );
                  IsChanged=1;
             }
         }
      }
   }
   while (IsChanged);
   
}


void INTERNAL_RemakeNanoTileMatrix()
/*
    Questa routine provvede a reinizializzare la matrice NanoTile, dopodichè la
    ricostruisce sulla base della nuova posizione delle finestre. 
*/
{
   int Counter, CounterX, CounterY;
   char IHaveFound1, IHaveFound2;
   unsigned char IDWindow;
   unsigned short int TileLenX, TileLenY;
   
   // Reinizializziamo la matrice NanoTile
   for (Counter=0; Counter<__MAXTILESALLOCABLE; Counter++) ndNanoTile [Counter].Enabled=0;
   
   // Reinizializziamo il contatore NanoTiles
   NrNanoTilesEnabled=0;
   
   // Provvediamo a ricostruire la matrice NanoTile
   for (CounterY=0; CounterY<(NrYVertexEnabled-1); CounterY++)
   {
        for (CounterX=0; CounterX<(NrXVertexEnabled-1); CounterX++)
        {
            TileLenX = ndXVertexBuffer [CounterX+1].PosX - ndXVertexBuffer [CounterX].PosX;
            TileLenY = ndYVertexBuffer [CounterY+1].PosY - ndYVertexBuffer [CounterY].PosY;
            
            // Le tile devono sempre avere una dimensione maggiore o uguale a 2, sia sull'asse x
            // che sull'asse y. Diversamente non vengono registrate
            
            if ((TileLenX>=2) && (TileLenY>=2))
            {
                        ndNanoTile [NrNanoTilesEnabled].PosX1 = ndXVertexBuffer [CounterX].PosX;
                        ndNanoTile [NrNanoTilesEnabled].PosX2 = ndXVertexBuffer [CounterX+1].PosX;
                        ndNanoTile [NrNanoTilesEnabled].PosY1 = ndYVertexBuffer [CounterY].PosY;
                        ndNanoTile [NrNanoTilesEnabled].PosY2 = ndYVertexBuffer [CounterY+1].PosY; 
                        
                        ndNanoTile [NrNanoTilesEnabled].Enabled=1;
                        
                        // Adesso dobbiamo provvedere all'analisi dei livelli: per prima cosa partiamo dal
                        // presupposto che la tile faccia parte dello sfondo
                        ndNanoTile [NrNanoTilesEnabled].Level1 = 255;
                        ndNanoTile [NrNanoTilesEnabled].Level2 = 255;
                        
                        // Poniamo a zero gli indicatori found
                        IHaveFound1=0;
                        IHaveFound2=0;
                        
                        Counter=0;
                        
                        // Controlliamo finestra per finestra
                        while (  (Counter<__NRWINDOWSALLOCABLE) && (! (IHaveFound1 && IHaveFound2) )  )
                        {
                              IDWindow = WindowsVector [Counter];
                              // Questo è l'handle della finestra che si trova in primo piano (proseguendo
                              // il ciclo, si passerà al secondo piano, terzo ecc. ecc.
                              
                              if (IDWindow==255) break;          // Siamo a fine vettore. Esegui break
                              
                              if (!WindowData [IDWindow].Minimize)      // La finestra non è minimizzata
                              {            
                                  if ( (ndNanoTile [NrNanoTilesEnabled].PosX1 >= WindowData [IDWindow].WndLimitX1) &&
                                       (ndNanoTile [NrNanoTilesEnabled].PosX2 <= WindowData [IDWindow].WndLimitX2) &&
                                       (ndNanoTile [NrNanoTilesEnabled].PosY1 >= WindowData [IDWindow].WndLimitY1) &&
                                       (ndNanoTile [NrNanoTilesEnabled].PosY2 <= WindowData [IDWindow].WndLimitY2) )
                                  {
                                       // Ok, questa tile si trova nello spazio della finestra
                                   
                                       if (!IHaveFound1)     // Ancora il primo piano non era stato trovato
                                       {
                                          ndNanoTile [NrNanoTilesEnabled].Level1 = IDWindow;
                                          IHaveFound1=1;
                                       }
                                       else                  // L'elemento in primo piano era già stato trovato: questo è il secondo livello
                                       {
                                          ndNanoTile [NrNanoTilesEnabled].Level2 = IDWindow;
                                          IHaveFound2=1;                         // Questo provocherà l'uscita dal loop
                                       }
                                  
                                  }
                              }
            
                              Counter++;
                         }                       // Fine ciclo while di scansione del window vector
              
                         NrNanoTilesEnabled++;
                         
             } // End if (LenTileX>=2) e (LenTileY>=2)
        } 
   }
     
}


void INTERNAL_RemakeOwnerPixelMatrixForContours ()
/*
    L'algoritmo che visualizza l'immagine tile per tile, funziona bene salvo che nei
    bordi della tile. Per questo motivo, sui bordi si continua a visualizzare con il
    vecchio metodo ownerpixel usato nelle prime versioni di NanoDesktop.
*/
{
    register int CounterX, CounterY;
    register unsigned short int PosPixelX, PosPixelY; 
    
    // Iniziamo ad operare sui bordi verticali
    
    for (CounterX=0; CounterX<NrXVertexEnabled; CounterX++)     
    {
        for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
        {
          PosPixelX = ndXVertexBuffer [CounterX].PosX;
          PosPixelY = CounterY;  
            
          if (!(TrasparencyEnabled))
              INTERNAL_FindOwnerOfPixel (PosPixelX, PosPixelY, &(OwnerArray [PosPixelX][PosPixelY][0]) );
          else
              INTERNAL_FindTwoOwnersOfPixel (PosPixelX, PosPixelY, &(OwnerArray [PosPixelX][PosPixelY][0]), &(OwnerArray [PosPixelX][PosPixelY][1]) ); 
        }
    }
    
    // Ed ora operiamo sui bordi orizzontali
    
    for (CounterY=0; CounterY<NrYVertexEnabled; CounterY++)     
    {
        for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
        {
          PosPixelX = CounterX;  
          PosPixelY = ndYVertexBuffer [CounterY].PosY;
            
          if (!(TrasparencyEnabled))
              INTERNAL_FindOwnerOfPixel (PosPixelX, PosPixelY, &(OwnerArray [PosPixelX][PosPixelY][0]) );
          else
              INTERNAL_FindTwoOwnersOfPixel (PosPixelX, PosPixelY, &(OwnerArray [PosPixelX][PosPixelY][0]), &(OwnerArray [PosPixelX][PosPixelY][1]) ); 
        }
    }
}



inline void ndHAL_UpdateWindowsRenderRegisters_Core (void)
{
   int Counter;
   
   // Azzera il due vertex buffer (dimensione X e dimensione Y)
   INTERNAL_ReInitVertexBuffer (); 
   
   // Immette le coordinate dello sfondo
   INTERNAL_PushRectToVertexBuffer (0, 0, Windows_MaxScreenX, Windows_MaxScreenY);
   
   // Immette le coordinate di tutte le finestre che non sono minimizzate
   for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
   {
       if ((WindowData [Counter].WindowIsActive) && (!WindowData [Counter].Minimize))
       {
           INTERNAL_PushRectToVertexBuffer ( WindowData [Counter].WndLimitX1, WindowData [Counter].WndLimitY1,
                                             WindowData [Counter].WndLimitX2, WindowData [Counter].WndLimitY2 );
       }
   }
   
   INTERNAL_OrderXVertexBuffer ();
   INTERNAL_OrderYVertexBuffer ();
   INTERNAL_RemakeNanoTileMatrix ();
   
   INTERNAL_RemakeOwnerPixelMatrixForContours (); 
}






inline void INTERNAL_WindowsRender_NOTRASP_NOBACKGR ()
/*
    Questa subroutine di WindowsRender esegue il render di tutto lo schermo. E' ottimizzata per
    funzionare nel caso in cui non vi siano trasparenze ed il background sia disattivato
*/
{
  register unsigned short int PosPixelX, PosPixelY;
  register unsigned short int RgPosX1, RgPosY1, RgPosX2, RgPosY2;
  
  register unsigned short int NotActiveFBPage;
    
  register unsigned short int RPosX, RPosY;
  register unsigned char OwnerOfPixel;
  
  register int Counter, CounterX, CounterY;
  
  register int ADR;


  NotActiveFBPage=!ActiveFBPage;        // Tutte le operazioni di scrittura vengono eseguite nella pagina che
                                        // in quel momento non è attiva

  // Provvedi alla visualizzazione delle tiles
  
  sceGuStart(GU_DIRECT,ndlist);   // Preparati ad inviare comandi al chip grafico della PSP
  sceKernelDcacheWritebackAll();  // Pulisci la cache: necessario su PSP per evitare la corruzione dell'immagine 
  
  ADR=AddrVideoMemory+NotActiveFBPage*PageSize;
  
  for (Counter=0; Counter<NrNanoTilesEnabled; Counter++)
  {
     RgPosX1 = ndNanoTile [Counter].PosX1+1;    // Forza questi valori dentro ai registri
     RgPosX2 = ndNanoTile [Counter].PosX2-1;
     RgPosY1 = ndNanoTile [Counter].PosY1+1;
     RgPosY2 = ndNanoTile [Counter].PosY2-1;
     
     OwnerOfPixel = ndNanoTile [Counter].Level1;   
   
     if (OwnerOfPixel!=255)    // Il tile appartiene a qualche finestra
     {
             RPosX=RgPosX1-(WindowData [OwnerOfPixel].WndLimitX1);                  // Viene calcolata la coordinata relativa per
             RPosY=RgPosY1-(WindowData [OwnerOfPixel].WndLimitY1);                  // accesso alla ram delle pagine logiche
                     
             sceGuCopyImage(GU_PSM_5551, RPosX,              RPosY,                 // Provvede a salvare la tile sullo schermo 
                                         RgPosX2-RgPosX1+1,  RgPosY2-RgPosY1+1,     // usando il chip grafico della PSP
                                         WindowData [OwnerOfPixel].HwWndLength, 
                                         WindowData [OwnerOfPixel]._p, 
                                         RgPosX1, RgPosY1, 512, 
                                         ADR);
                                         
	         sceGuTexSync();
     }  
     else    // Il tile appartiene allo sfondo
     {
             for (PosPixelY=RgPosY1; PosPixelY<=RgPosY2; PosPixelY++)
             {
                for (PosPixelX=RgPosX1; PosPixelX<=RgPosX2; PosPixelX++)
                {
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, 0, NotActiveFBPage);             
                }
             }      
     }
         
  }
  
  sceGuFinish();
  sceGuSync(0,0);
             
  // Provvedi alla visualizzazione dei contorni delle tiles: prima operiamo sui contorni verticali
  
  for (CounterX=0; CounterX<NrXVertexEnabled; CounterX++)     
  {
        for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
        {
              PosPixelX = ndXVertexBuffer [CounterX].PosX;
              PosPixelY = CounterY;  
                
              OwnerOfPixel=OwnerArray [PosPixelX][PosPixelY][0];
    
              if (OwnerOfPixel!=255)                     // Il pixel appartiene a qualche finestra
              {
                 RPosX=PosPixelX-(WindowData[OwnerOfPixel].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                 RPosY=PosPixelY-(WindowData[OwnerOfPixel].WndLimitY1);                  // accesso alla ram delle pagine logiche
    
                 ndHAL_PutPixelToPage (PosPixelX, PosPixelY, WINDOWDATA_P (OwnerOfPixel, RPosX, RPosY), NotActiveFBPage);
              }
              else                                       // Il pixel appartiene allo sfondo
              {
                 ndHAL_PutPixelToPage (PosPixelX, PosPixelY, 0, NotActiveFBPage);                 
              }      
        }
  }
    
  // Ed ora provvediamo a visualizzare i bordi orizzontali
    
  for (CounterY=0; CounterY<NrYVertexEnabled; CounterY++)     
  {
        for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
        {
              PosPixelX = CounterX;  
              PosPixelY = ndYVertexBuffer [CounterY].PosY;
                
              OwnerOfPixel=OwnerArray [PosPixelX][PosPixelY][0];
        
              if (OwnerOfPixel!=255)                     // Il pixel appartiene a qualche finestra
              {
                 RPosX=PosPixelX-(WindowData[OwnerOfPixel].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                 RPosY=PosPixelY-(WindowData[OwnerOfPixel].WndLimitY1);                  // accesso alla ram delle pagine logiche
    
                 ndHAL_PutPixelToPage (PosPixelX, PosPixelY, WINDOWDATA_P (OwnerOfPixel, RPosX, RPosY), NotActiveFBPage);
              }
              else                                       // Il pixel appartiene allo sfondo
              {
                 ndHAL_PutPixelToPage (PosPixelX, PosPixelY, 0, NotActiveFBPage); 
              }
        }
  }  
  
}


inline void INTERNAL_WindowsRender_NOTRASP_BACKGR ()
/*
    Questa subroutine di WindowsRender esegue il render di tutto lo schermo. E' ottimizzata per
    funzionare nel caso in cui non vi siano trasparenze ed il background sia attivato
*/
{
  register unsigned short int PosPixelX, PosPixelY;
  register unsigned short int RgPosX1, RgPosY1, RgPosX2, RgPosY2;
  
  register unsigned short int NotActiveFBPage;
    
  register unsigned short int RPosX, RPosY;
  register unsigned char OwnerOfPixel;

  register int Counter, CounterX, CounterY;
  
  register int ADR;
  

  NotActiveFBPage=!ActiveFBPage;        // Tutte le operazioni di scrittura vengono eseguite nella pagina che
                                        // in quel momento non è attiva
  
  // Provvedi alla visualizzazione delle tiles
  
  sceGuStart(GU_DIRECT,ndlist);   // Preparati ad inviare comandi al chip grafico della PSP
  sceKernelDcacheWritebackAll();  // Pulisci la cache: necessario su PSP per evitare la corruzione dell'immagine 
  
  ADR=AddrVideoMemory+NotActiveFBPage*PageSize;
  
  for (Counter=0; Counter<NrNanoTilesEnabled; Counter++)
  {
     RgPosX1 = ndNanoTile [Counter].PosX1+1;    // Forza questi valori dentro ai registri
     RgPosX2 = ndNanoTile [Counter].PosX2-1;
     RgPosY1 = ndNanoTile [Counter].PosY1+1;
     RgPosY2 = ndNanoTile [Counter].PosY2-1;
     
     OwnerOfPixel = ndNanoTile [Counter].Level1;   
   
     if (OwnerOfPixel!=255)    // Il tile appartiene a qualche finestra
     {
             RPosX=RgPosX1-(WindowData [OwnerOfPixel].WndLimitX1);                  // Viene calcolata la coordinata relativa per
             RPosY=RgPosY1-(WindowData [OwnerOfPixel].WndLimitY1);                  // accesso alla ram delle pagine logiche
                     
             sceGuCopyImage(GU_PSM_5551, RPosX,              RPosY,                 // Provvede a salvare la tile sullo schermo 
                                         RgPosX2-RgPosX1+1,  RgPosY2-RgPosY1+1,     // usando il chip grafico della PSP
                                         WindowData [OwnerOfPixel].HwWndLength, 
                                         WindowData [OwnerOfPixel]._p, 
                                         RgPosX1, RgPosY1, 512, 
                                         ADR);
                                         
	         sceGuTexSync();    
     }  
     else    // Il tile appartiene allo sfondo, che però in questo caso non è vuoto
     {
             sceGuCopyImage(GU_PSM_5551, RgPosX1,            RgPosY1,               // Provvede a salvare la tile sullo schermo 
                                         RgPosX2-RgPosX1+1,  RgPosY2-RgPosY1+1,     // usando il chip grafico della PSP
                                         __BASESCREENX,                             // BaseScreen è un multiplo di 16 qui 
                                         &BaseScreen,                               // Indirizzo del basescreen 
                                         RgPosX1, RgPosY1, 512, 
                                         ADR);
                                         
	         sceGuTexSync();     
     }
         
  }
  
  sceGuFinish();
  sceGuSync(0,0);
  
  // Provvedi alla visualizzazione dei contorni delle tiles: prima operiamo sui contorni verticali
  
  for (CounterX=0; CounterX<NrXVertexEnabled; CounterX++)     
  {
        for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
        {
              PosPixelX = ndXVertexBuffer [CounterX].PosX;
              PosPixelY = CounterY;  
                
              OwnerOfPixel=OwnerArray [PosPixelX][PosPixelY][0];
    
              if (OwnerOfPixel!=255)                     // Il pixel appartiene a qualche finestra
              {
                 RPosX=PosPixelX-(WindowData[OwnerOfPixel].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                 RPosY=PosPixelY-(WindowData[OwnerOfPixel].WndLimitY1);                  // accesso alla ram delle pagine logiche
    
                 ndHAL_PutPixelToPage (PosPixelX, PosPixelY, WINDOWDATA_P (OwnerOfPixel, RPosX, RPosY), NotActiveFBPage);
              }
              else                                       // Il pixel appartiene allo sfondo
              {
                 ndHAL_PutPixelToPage (PosPixelX, PosPixelY, BaseScreen [PosPixelY][PosPixelX], NotActiveFBPage);                 
              }      
        }
  }
    
  // Ed ora provvediamo a visualizzare i bordi orizzontali
    
  for (CounterY=0; CounterY<NrYVertexEnabled; CounterY++)     
  {
        for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
        {
              PosPixelX = CounterX;  
              PosPixelY = ndYVertexBuffer [CounterY].PosY;
                
              OwnerOfPixel=OwnerArray [PosPixelX][PosPixelY][0];
        
              if (OwnerOfPixel!=255)                     // Il pixel appartiene a qualche finestra
              {
                 RPosX=PosPixelX-(WindowData[OwnerOfPixel].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                 RPosY=PosPixelY-(WindowData[OwnerOfPixel].WndLimitY1);                  // accesso alla ram delle pagine logiche
    
                 ndHAL_PutPixelToPage (PosPixelX, PosPixelY, WINDOWDATA_P (OwnerOfPixel, RPosX, RPosY), NotActiveFBPage);
              }
              else                                       // Il pixel appartiene allo sfondo
              {
                 ndHAL_PutPixelToPage (PosPixelX, PosPixelY, BaseScreen [PosPixelY][PosPixelX], NotActiveFBPage); 
              }
        }
  } 
  
   
}


inline void INTERNAL_WindowsRender_TRASP_NOBACKGR ()
/*
    Questa subroutine di WindowsRender esegue il render di tutto lo schermo. E' ottimizzata per
    funzionare nel caso in cui vi siano trasparenze ed il background sia disattivato
*/
{
  register unsigned short int PosPixelX, PosPixelY;
  register unsigned short int RgPosX1, RgPosY1, RgPosX2, RgPosY2;
  
  register unsigned short int NotActiveFBPage;
    
  register unsigned short int RPosX, RPosY, SPosX, SPosY;
  register unsigned char OwnerOfPixel1, OwnerOfPixel2;

  register int Counter, CounterX, CounterY;
  register TypeColor Color1, Color2, ColorOut;

  NotActiveFBPage=!ActiveFBPage;        // Tutte le operazioni di scrittura vengono eseguite nella pagina che
                                        // in quel momento non è attiva

  // Provvedi alla visualizzazione delle tiles
  
  for (Counter=0; Counter<NrNanoTilesEnabled; Counter++)
  {
     RgPosX1 = ndNanoTile [Counter].PosX1+1;    // Forza questi valori dentro ai registri
     RgPosX2 = ndNanoTile [Counter].PosX2-1;
     RgPosY1 = ndNanoTile [Counter].PosY1+1;
     RgPosY2 = ndNanoTile [Counter].PosY2-1;
     
     OwnerOfPixel1 = ndNanoTile [Counter].Level1; 
     OwnerOfPixel2 = ndNanoTile [Counter].Level2;  
   
     if ((OwnerOfPixel1!=255) && (OwnerOfPixel2!=255))    // Il tile appartiene a due finestre sovrapposte
     {
             for (PosPixelY=RgPosY1; PosPixelY<=RgPosY2; PosPixelY++)
             {
                for (PosPixelX=RgPosX1; PosPixelX<=RgPosX2; PosPixelX++)
                {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY);
                     
                    SPosX=PosPixelX-(WindowData [OwnerOfPixel2].WndLimitX1);                  
                    SPosY=PosPixelY-(WindowData [OwnerOfPixel2].WndLimitY1);                  
                    Color2=WINDOWDATA_P (OwnerOfPixel2, SPosX, SPosY);
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage);
                }
             }
     }
     
     if ((OwnerOfPixel1!=255) && (OwnerOfPixel2==255))   // Il tile appartiene alla prima finestra, dietro c'è un background nero
     {
             for (PosPixelY=RgPosY1; PosPixelY<=RgPosY2; PosPixelY++)
             {
                for (PosPixelX=RgPosX1; PosPixelX<=RgPosX2; PosPixelX++)
                {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                      
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY), NotActiveFBPage);
                }
             }                        
     }
      
     if ((OwnerOfPixel1==255) && (OwnerOfPixel2==255))   // Il tile non appartiene ad alcuna finestra
     {
             for (PosPixelY=RgPosY1; PosPixelY<=RgPosY2; PosPixelY++)
             {
                for (PosPixelX=RgPosX1; PosPixelX<=RgPosX2; PosPixelX++)
                {
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, 0, NotActiveFBPage);             
                }
             }      
     }
         
  }
  
  // Provvedi alla visualizzazione dei contorni delle tiles: prima operiamo sui contorni verticali
  
  for (CounterX=0; CounterX<NrXVertexEnabled; CounterX++)     
  {
        for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
        {
              PosPixelX = ndXVertexBuffer [CounterX].PosX;
              PosPixelY = CounterY;  
                
              OwnerOfPixel1=OwnerArray [PosPixelX][PosPixelY][0];
              OwnerOfPixel2=OwnerArray [PosPixelX][PosPixelY][1];
              
              if ((OwnerOfPixel1!=255) && (OwnerOfPixel2!=255))    // Il pixel appartiene a due finestre sovrapposte
              {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY);
                     
                    SPosX=PosPixelX-(WindowData [OwnerOfPixel2].WndLimitX1);                  
                    SPosY=PosPixelY-(WindowData [OwnerOfPixel2].WndLimitY1);                  
                    Color2=WINDOWDATA_P (OwnerOfPixel2, SPosX, SPosY);
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage);
              }
     
              if ((OwnerOfPixel1!=255) && (OwnerOfPixel2==255))   // Il pixel appartiene alla prima finestra, dietro c'è un background nero
              {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                      
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY), NotActiveFBPage);
              }
      
              if ((OwnerOfPixel1==255) && (OwnerOfPixel2==255))   // Il pixel non appartiene a nessuna finestra
              {
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, 0, NotActiveFBPage);                  
              }   
        }
  }
    
  // Ed ora provvediamo a visualizzare i bordi orizzontali
    
  for (CounterY=0; CounterY<NrYVertexEnabled; CounterY++)     
  {
        for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
        {
              PosPixelX = CounterX;  
              PosPixelY = ndYVertexBuffer [CounterY].PosY;
                
              OwnerOfPixel1=OwnerArray [PosPixelX][PosPixelY][0];
              OwnerOfPixel2=OwnerArray [PosPixelX][PosPixelY][1];
              
              if ((OwnerOfPixel1!=255) && (OwnerOfPixel2!=255))    // Il pixel appartiene a due finestre sovrapposte
              {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY);
                     
                    SPosX=PosPixelX-(WindowData [OwnerOfPixel2].WndLimitX1);                  
                    SPosY=PosPixelY-(WindowData [OwnerOfPixel2].WndLimitY1);                  
                    Color2=WINDOWDATA_P (OwnerOfPixel2, SPosX, SPosY);
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage);
              }
     
              if ((OwnerOfPixel1!=255) && (OwnerOfPixel2==255))   // Il pixel appartiene alla prima finestra, dietro c'è un background nero
              {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                      
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY), NotActiveFBPage);
              }
      
              if ((OwnerOfPixel1==255) && (OwnerOfPixel2==255))   // Il pixel non appartiene a nessuna finestra
              {
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, 0, NotActiveFBPage);                  
              }
        }
  }  
}

inline void INTERNAL_WindowsRender_TRASP_BACKGR ()
/*
    Questa subroutine di WindowsRender esegue il render di tutto lo schermo. E' ottimizzata per
    funzionare nel caso in cui vi siano trasparenze ed il background sia attivato
*/
{
  register unsigned short int PosPixelX, PosPixelY;
  register unsigned short int RgPosX1, RgPosY1, RgPosX2, RgPosY2;
  
  register unsigned short int NotActiveFBPage;
    
  register unsigned short int RPosX, RPosY, SPosX, SPosY;
  register unsigned char OwnerOfPixel1, OwnerOfPixel2;

  register int Counter, CounterX, CounterY;
  register TypeColor Color1, Color2, ColorOut;

  NotActiveFBPage=!ActiveFBPage;        // Tutte le operazioni di scrittura vengono eseguite nella pagina che
                                        // in quel momento non è attiva

  // Provvedi alla visualizzazione delle tiles
  
  for (Counter=0; Counter<NrNanoTilesEnabled; Counter++)
  {
     RgPosX1 = ndNanoTile [Counter].PosX1+1;    // Forza questi valori dentro ai registri
     RgPosX2 = ndNanoTile [Counter].PosX2-1;
     RgPosY1 = ndNanoTile [Counter].PosY1+1;
     RgPosY2 = ndNanoTile [Counter].PosY2-1;
     
     OwnerOfPixel1 = ndNanoTile [Counter].Level1; 
     OwnerOfPixel2 = ndNanoTile [Counter].Level2;  
   
     if ((OwnerOfPixel1!=255) && (OwnerOfPixel2!=255))    // Il tile appartiene a due finestre sovrapposte
     {
             for (PosPixelY=RgPosY1; PosPixelY<=RgPosY2; PosPixelY++)
             {
                for (PosPixelX=RgPosX1; PosPixelX<=RgPosX2; PosPixelX++)
                {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY);
                     
                    SPosX=PosPixelX-(WindowData [OwnerOfPixel2].WndLimitX1);                  
                    SPosY=PosPixelY-(WindowData [OwnerOfPixel2].WndLimitY1);                  
                    Color2=WINDOWDATA_P (OwnerOfPixel2, SPosX, SPosY);
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage);
                }
             }
     }
     
     if ((OwnerOfPixel1!=255) && (OwnerOfPixel2==255))   // Il tile appartiene alla prima finestra, dietro c'è un background che è attivato
     {
             for (PosPixelY=RgPosY1; PosPixelY<=RgPosY2; PosPixelY++)
             {
                for (PosPixelX=RgPosX1; PosPixelX<=RgPosX2; PosPixelX++)
                {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY); 
                    Color2=BaseScreen [PosPixelY][PosPixelX];
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage); 
                }
             }                        
     }
      
     if ((OwnerOfPixel1==255) && (OwnerOfPixel2==255))   // Il tile non appartiene a nessuna finestra, ma al backgroud attivato
     {
             for (PosPixelY=RgPosY1; PosPixelY<=RgPosY2; PosPixelY++)
             {
                for (PosPixelX=RgPosX1; PosPixelX<=RgPosX2; PosPixelX++)
                {
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, BaseScreen [PosPixelY][PosPixelX], NotActiveFBPage);             
                }
             }      
     }
         
  }
  
  // Provvedi alla visualizzazione dei contorni delle tiles: prima operiamo sui contorni verticali
  
  for (CounterX=0; CounterX<NrXVertexEnabled; CounterX++)     
  {
        for (CounterY=0; CounterY<Windows_MaxScreenY; CounterY++)
        {
              PosPixelX = ndXVertexBuffer [CounterX].PosX;
              PosPixelY = CounterY;  
                
              OwnerOfPixel1=OwnerArray [PosPixelX][PosPixelY][0];
              OwnerOfPixel2=OwnerArray [PosPixelX][PosPixelY][1];
              
              if ((OwnerOfPixel1!=255) && (OwnerOfPixel2!=255))    // Il pixel appartiene a due finestre sovrapposte
              {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY);
                     
                    SPosX=PosPixelX-(WindowData [OwnerOfPixel2].WndLimitX1);                  
                    SPosY=PosPixelY-(WindowData [OwnerOfPixel2].WndLimitY1);                  
                    Color2=WINDOWDATA_P (OwnerOfPixel2, SPosX, SPosY);
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage);
              }
     
              if ((OwnerOfPixel1!=255) && (OwnerOfPixel2==255))   // Il pixel appartiene alla prima finestra, dietro c'è un background attivato
              {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY); 
                    Color2=BaseScreen [PosPixelY][PosPixelX];
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage);        
              }
      
              if ((OwnerOfPixel1==255) && (OwnerOfPixel2==255))   // Il pixel appartiene al background non vuoto
              {
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, BaseScreen [PosPixelY][PosPixelX], NotActiveFBPage);                  
              }   
        }
  }
    
  // Ed ora provvediamo a visualizzare i bordi orizzontali
    
  for (CounterY=0; CounterY<NrYVertexEnabled; CounterY++)     
  {
        for (CounterX=0; CounterX<Windows_MaxScreenX; CounterX++)
        {
              PosPixelX = CounterX;  
              PosPixelY = ndYVertexBuffer [CounterY].PosY;
                
              OwnerOfPixel1=OwnerArray [PosPixelX][PosPixelY][0];
              OwnerOfPixel2=OwnerArray [PosPixelX][PosPixelY][1];
              
              if ((OwnerOfPixel1!=255) && (OwnerOfPixel2!=255))    // Il pixel appartiene a due finestre sovrapposte
              {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY);
                     
                    SPosX=PosPixelX-(WindowData [OwnerOfPixel2].WndLimitX1);                  
                    SPosY=PosPixelY-(WindowData [OwnerOfPixel2].WndLimitY1);                  
                    Color2=WINDOWDATA_P (OwnerOfPixel2, SPosX, SPosY);
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage);
              }
     
              if ((OwnerOfPixel1!=255) && (OwnerOfPixel2==255))   // Il pixel appartiene alla prima finestra, dietro c'è un background attivato
              {
                    RPosX=PosPixelX-(WindowData [OwnerOfPixel1].WndLimitX1);                  // Viene calcolata la coordinata relativa per
                    RPosY=PosPixelY-(WindowData [OwnerOfPixel1].WndLimitY1);                  // accesso alla ram delle pagine logiche
                    
                    Color1=WINDOWDATA_P (OwnerOfPixel1, RPosX, RPosY); 
                    Color2=BaseScreen [PosPixelY][PosPixelX];
                     
                    ColorOut = INTERNAL_MathBlend (Color1, Color2);
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, ColorOut, NotActiveFBPage);        
              }
      
              if ((OwnerOfPixel1==255) && (OwnerOfPixel2==255))   // Il pixel appartiene al background non vuoto
              {
                    ndHAL_PutPixelToPage (PosPixelX, PosPixelY, BaseScreen [PosPixelY][PosPixelX], NotActiveFBPage);                  
              }
        }
  }  
}



inline void ndHAL_DoWindowsRenderNoSwap ()
/*
     Questa routine è una delle più importanti della libreria NANODesktop.

     Essa, esegue il rendering dell'insieme di finestre: a partire dalle varie Logical Page,
     e considerati i dati nel Windows Vector, viene disegnata la pagina video.

     La pagina video viene disegnata in una pagina off-screen del framebuffer.
     
     Questa routine non esegue la commutazione del video alla fine del processo di rendering,
     ma lascia tale compito alla routine chiamante, che deve eseguire un ndHAL_ChangeFrameBufferPage()
     subito dopo avere richiamato questa routine.
*/

{
     if (!TrasparencyEnabled)
     {
       if (!BaseScreenIsActive)
          INTERNAL_WindowsRender_NOTRASP_NOBACKGR ();
       else
          INTERNAL_WindowsRender_NOTRASP_BACKGR (); 
     }
     else  // Le trasparenze sono abilitate
     {
       if (!BaseScreenIsActive)
          INTERNAL_WindowsRender_TRASP_NOBACKGR ();
       else
          INTERNAL_WindowsRender_TRASP_BACKGR (); 
     }
     
     INTERNAL_DrawMinWindowsBar ();
     
     ndNrVideoFrame++;                            // Aggiorna il nr di video frame
     
     // Nessuna commutazione del video
}


inline void ndHAL_DoWindowsRender ()
/*
     Questa routine è una delle più importanti della libreria NANODesktop.

     Essa, esegue il rendering dell'insieme di finestre: a partire dalle varie Logical Page,
     e considerati i dati nel Windows Vector, viene disegnata la pagina video.

     La pagina video viene prima disegnata in una pagina off-screen del framebuffer, e poi
     avviene la commutazione del video.
*/

{
   ndHAL_DoWindowsRenderNoSwap ();     // Richiama la funzione per eseguire il rendering   
   ndHAL_ChangeFrameBufferPage();      // Esegue la commutazione del video
}


void ndHAL_WindowsRender_CLIENT (void)
/*
    Questa routine costituisce il client del sistema client/server che serializza
    le chiamate WindowsRender in modo da rendere possibile l'uso di più threads
    in una applicazione Nanodesktop. 
    
    Nei sistemi diversi da PSPE, questa routine provvede ad inserire le opportune
    chiamate in modo che esse possano essere intercettate dal PhoenixGraph 
    thread. 
    
*/ 
{
   register int  ThreadID;
   register int  Counter;
   register int  IHaveFound;
   
   int  EvResult;
   
   
   
   if (!ND_SWMODE_DisablePhoenixSubsystem)   // Il Phoenix Subsystem non è stato disattivato
   {      
           sceKernelWaitEventFlag  (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR , &EvResult, 0); 
           // Attende che il buffer sia libero e dopo azzera il flag

           // Ponendo ad 0 questo flag, ci si assicura che nessun altro
           // thread tenti di scrivere
        
           IHaveFound = 0;                     
           Counter    = 0;
           ThreadID   = sceKernelGetThreadId();
           
           while (Counter<__NR_THREADS_AVAILABLE) 
           {
                 if (!phnGraph_Callers [Counter].IsEnabled)
                 {
                     phnGraph_Callers [Counter].IsEnabled = 1;
                     phnGraph_Callers [Counter].ThreadID  = ThreadID;   
                     
                     IHaveFound=1;
                     break;
                 }
                 
                 Counter++;
           }
           
           sceKernelSetEventFlag (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY);
           // Adesso è di nuovo possibile scrivere nel registro dei Phoenix callers
          
           if (IHaveFound)
           {
              phnGraph_WindowsRender_ReqOn  = 1;
              sceKernelSleepThread ();
           }
           
   }
   else                     
   {
       // Phoenix Subsystem disattivato. In questo caso non c'è più l'architettura client/server
       // tipica di Nanodesktop Phoenix e quindi il client richiama direttamente il server                  
                            
       ndHAL_DoWindowsRender ();
   }
}


void ndHAL_UpdateWindowsRenderRegisters ()
/*
   Questa routine provvede a ricostruire il buffer dei vertici sia sull'asse X
   che sull'asse Y. La routine è solo per usi interni, e deve essere richiamata
   tutte le volte che si verifica un cambiamento nella posizione o dimensione
   delle finestre
*/
{  
   register int  ThreadID;
   register int  Counter;
   register int  IHaveFound;
   
   int  EvResult;
   
   
   
   if (!ND_SWMODE_DisablePhoenixSubsystem)       // Phoenix subsystem attivato
   {
           sceKernelWaitEventFlag  (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR , &EvResult, 0); 
           // Attende che il buffer sia libero e dopo azzera il flag

           // Ponendo ad 0 questo flag, ci si assicura che nessun altro
           // thread tenti di scrivere
          
           IHaveFound = 0;                     
           Counter    = 0;
           ThreadID   = sceKernelGetThreadId();
           
           while (Counter<__NR_THREADS_AVAILABLE)
           {
                 if (!phnGraph_Callers [Counter].IsEnabled)
                 {
                     phnGraph_Callers [Counter].IsEnabled = 1;
                     phnGraph_Callers [Counter].ThreadID  = ThreadID;   
                     
                     IHaveFound=1;
                     break;
                 }
                 
                 Counter++;
           }
           
           sceKernelSetEventFlag (phnGraph_Ev0_ReqRegister, PHNGRAPH_SVR_IS_NOT_BUSY);
           // Adesso è di nuovo possibile scrivere nel registro dei Phoenix callers
          
           if (IHaveFound)
           {
              phnGraph_UpdWindowsRenderRegs_ReqOn = 1;
              sceKernelSleepThread ();
           }
           
   }
   else
   {
           ndHAL_UpdateWindowsRenderRegisters_Core ();
   }
}






