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
    FUNZIONI HARDWARE DEPENDENT PER GESTIONE GRAFICA (PSPE)
    ---------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>

void ndHAL_InitGraph (void)
/*
   E' la routine che reinizializza la sezione hardware del sottosistema grafico.
*/   
   
  //  Per reinizializzare il sottosistema grafico della PSP, vengono
  //  richiamate due funzioni del kernel. La prima, pspDisplaySetMode
  //  deve contenere i parametri 0, la lunghezza reale dello
  //  schermo, e l'altezza reale dello schermo.

  //  La seconda, pspDisplaySetFrameBuf, contiene come primo parametro,
  //  l'indirizzo del primo pixel sullo schermo, il secondo parametro
  //  è la lunghezza virtuale dello schermo, il terzo ed il quarto sono
  //  sempre posti pari ad 1. L'indirizzo del primo pixel sullo schermo
  //  è sempre l'indirizzo virtuale, e non quello reale.
{
    unsigned short int  CounterX, CounterY, Page;
    TypeColor *PointerToVRam;                                                   // Il puntatore alla Ram
    
    pspDisplaySetMode     (0, MaxScreenX, MaxScreenY);
    pspDisplaySetFrameBuf (XAddrVideoMemory, VMaxScreenX, 1, 1);
    
    AddrVideoMemory=PSPEBaseRegister+XAddrVideoMemory;
    
    ActiveFBPage=0;
    
    LastFBPageWritten=0;
    WRCurrentPageAddr=AddrVideoMemory;
    LastLineWritten=0;
    WRCurrentLineAddr=AddrVideoMemory;
    
    LastFBPageRead=0;
    RDCurrentPageAddr=AddrVideoMemory;
    LastLineRead=0;
    RDCurrentLineAddr=AddrVideoMemory;
    
    for (Page=0; Page<=1; Page++)
    {
        for (CounterY=0; CounterY<=VMaxScreenY; CounterY++)                        // La Ram video viene sempre completamente
        {                                                                          // svuotata in entrambe le pagine
             for (CounterX=0; CounterX<=MaxScreenX; CounterX++)
            {
                PointerToVRam=(TypeColor *)(AddrVideoMemory+Page*PageSize+sizeof(TypeColor)*(CounterY*VMaxScreenX+CounterX));
                *PointerToVRam=0x0000;
            }
        }
    }

}

inline void ndHAL_PutPixel (unsigned short int PosPixelX, unsigned short int PosPixelY, TypeColor Color)
/*
     Questa funzione si occupa di inserire un pixel nella memoria video. Il pixel viene scritto
     sempre nella pagina del framebuffer corrente. La routine inoltre è accelerata.
*/

{
     register TypeColor *PointerToVRam;

     if ( !ndInitSystemExecuted ) ndInitSystem();

     if (ActiveFBPage!=LastFBPageWritten)                                               // La pagina dove PutPixel scrive è quella determinata dalla
     {                                                                                  // variabile ActiveFBPage. Tuttavia, poichè esistono le funzioni PutPixelToPage
         LastFBPageWritten=ActiveFBPage;                                                // e GetPixelFromPage non è detto che essa sia coincidente con l'ultima pagina
         WRCurrentPageAddr=AddrVideoMemory+ActiveFBPage*PageSize;                       // della memoria video con cui si è scritto, contenuta in LastFBPageWritten.

         WRCurrentLineAddr=WRCurrentPageAddr+sizeof(TypeColor)*VMaxScreenX*PosPixelY;   // Se è presente discrepanza tra la pagina attiva (dove si deve scrivere) e
         LastLineWritten=PosPixelY;                                                     // l'ultima pagina scritta, vengono ricalcolati sia gli indirizzi di pagina,
                                                                                        // sia l'indirizzo di linea
     }
     else
     {
         if (LastLineWritten!=PosPixelY)                                                // Se non vi è discrepanza a livello di pagina, si esegue il controllo
         {                                                                              // solo a livello di linea
             WRCurrentLineAddr=WRCurrentPageAddr+sizeof(TypeColor)*VMaxScreenX*PosPixelY;
             LastLineWritten=PosPixelY;
         }
     }

     PointerToVRam = (TypeColor *)(WRCurrentLineAddr+sizeof(TypeColor)*PosPixelX);
     *PointerToVRam = Color;

}


void ndHAL_ChangeFrameBufferPage()
/*
     Questa funzione provvede a cambiare la pagina corrente del framebuffer. Il cambiamento
     provoca anche la commutazione della pagina da parte del chip grafico. Per leggere o
     scrivere nelle due pagine del chip grafico senza eseguire la commutazione dello schermo,
     usare le routine ndHAL_GetPixelFromPage e ndHAL_PutPixelToPage
*/

{
         if ( !ndInitSystemExecuted ) ndInitSystem();

         ActiveFBPage=!ActiveFBPage;                                                                   // Cambia il valore della pagina framebuffer attiva.

         LastFBPageWritten=ActiveFBPage;                                                               // Inoltre, fa in modo che la routine PutPixel consideri
                                                                                                       // la pagina corrente come appena scritta (risparmia il ricalcolo
                                                                                                       // dell'indirizzo la prima volta che si scrive un pixel sullo
                                                                                                       // schermo.
         WRCurrentPageAddr=AddrVideoMemory+ActiveFBPage*PageSize;

         pspDisplayWaitVblankStart();                                                                  // Attende il segnale della routine di rinfresco
                                                                                                       // (impedisce il tearing dell'immagine)

         pspDisplaySetFrameBuf ((XAddrVideoMemory+ActiveFBPage*PageSize), VMaxScreenX, 1, 1);           // Questa routine provoca la commutazione dello schermo
                                                                                                        // Bisogna ricordare che alle routine del kernel della psp
                                                                                                        // si mandano indirizzi che si riferiscono all'indirizzamento
                                                                                                        // virtuale (quello senza il PSPEBaseRegister: ecco perchè si
                                                                                                        // usa XAddrVideoMemory e non AddrVideoMemory

         LastLineWritten=0;
         WRCurrentLineAddr=WRCurrentPageAddr;                                                               // Occorre aggiornare anche la variabile che tiene conto
                                                                                                        // dell'indirizzo di riga
}


inline void ndHAL_PutPixelToPage (unsigned short int PosPixelX, unsigned short int PosPixelY, TypeColor Color, unsigned char Page)
/*
     Questa funzione consente di scrivere un pixel, ma non nella pagina corrente, ma nella pagina
     specificata, che può essere ben diversa dalla pagina corrente. Anche questa funzione è
     accelerata.
*/

{
     register TypeColor *PointerToVRam;

     if ( !ndInitSystemExecuted ) ndInitSystem();

     if (Page!=LastFBPageWritten)                                                       // La pagina dove PutPixelToPage scrive è quella determinata dalla
     {                                                                                  // variabile Page. Si noti che l'avere aggiornato LastPageWritten, ma non
         LastFBPageWritten=Page;                                                        // ActiveFBPage porta a quella discrepanza che porta poi PutPixel a
         WRCurrentPageAddr=AddrVideoMemory+Page*PageSize;                                 // ricalcolare automaticamente tutti gli indirizzi.

         WRCurrentLineAddr=WRCurrentPageAddr+sizeof(TypeColor)*VMaxScreenX*PosPixelY;
         LastLineWritten=PosPixelY;

     }
     else
     {
         if (LastLineWritten!=PosPixelY)                                                // Se non vi è discrepanza a livello di pagina, si esegue il controllo
         {                                                                              // solo a livello di linea
             WRCurrentLineAddr=WRCurrentPageAddr+sizeof(TypeColor)*VMaxScreenX*PosPixelY;
             LastLineWritten=PosPixelY;
         }
     }

     PointerToVRam = (TypeColor *)(WRCurrentLineAddr+sizeof(TypeColor)*PosPixelX);
     *PointerToVRam = Color;
}

inline TypeColor ndHAL_GetPixel (unsigned short int PosPixelX, unsigned short int PosPixelY)
/*
      Questa funzione restituisce al programma chiamante il contenuto del pixel alle coordinate PosPixelX,
      PosPixelY. La funzione è sempre riferita alla pagina corrente del framebuffer, contenuta in
      ActiveFBPage;
*/

{
     register TypeColor *PointerToVRam;

     if ( !ndInitSystemExecuted ) ndInitSystem();

     if (ActiveFBPage!=LastFBPageRead)                                                  // La pagina da cui GetPixel legge è quella determinata dalla
     {                                                                                  // variabile ActiveFBPage. Tuttavia, poichè esistono le funzioni PutPixelToPage
         LastFBPageRead=ActiveFBPage;                                                   // e GetPixelFromPage non è detto che essa sia coincidente con l'ultima pagina
         RDCurrentPageAddr=AddrVideoMemory+ActiveFBPage*PageSize;                       // della memoria video da cui si è letto, contenuta in LastFBPageRead.

         RDCurrentLineAddr=RDCurrentPageAddr+sizeof(TypeColor)*VMaxScreenX*PosPixelY;   // Se è presente discrepanza tra la pagina attiva (dove si deve scrivere) e
         LastLineRead=PosPixelY;                                                        // l'ultima pagina letta, vengono ricalcolati sia gli indirizzi di pagina,
                                                                                        // sia l'indirizzo di linea
     }
     else
     {
         if (LastLineRead!=PosPixelY)                                                   // Se non vi è discrepanza a livello di pagina, si esegue il controllo
         {                                                                              // solo a livello di linea
             RDCurrentLineAddr=RDCurrentPageAddr+sizeof(TypeColor)*VMaxScreenX*PosPixelY;
             LastLineRead=PosPixelY;
         }   
     }

     PointerToVRam = (TypeColor *)(RDCurrentLineAddr+sizeof(TypeColor)*PosPixelX);
     return (*PointerToVRam);
}

inline TypeColor ndHAL_GetPixelFromPage (unsigned short int PosPixelX, unsigned short int PosPixelY, unsigned short int Page)
/*
      Questa funzione restituisce al programma chiamante il contenuto del pixel alle coordinate PosPixelX,
      PosPixelY. La funzione è riferita alla pagina passata come parametro. La funzione non è accelerata.
*/

{
     register TypeColor *PointerToVRam;

     if ( !ndInitSystemExecuted ) ndInitSystem();

     if (Page!=LastFBPageRead)                                                          // La pagina da cui GetPixelToPage legge è quella determinata dalla
     {                                                                                  // variabile Page. Si noti che l'avere aggiornato LastPageRead, ma non
         LastFBPageRead=Page;                                                           // ActiveFBPage porta a quella discrepanza che porta poi GetPixel a
         RDCurrentPageAddr=AddrVideoMemory+Page*PageSize;                               // ricalcolare automaticamente tutti gli indirizzi.

         RDCurrentLineAddr=RDCurrentPageAddr+sizeof(TypeColor)*VMaxScreenX*PosPixelY;
         LastLineRead=PosPixelY;

     }
     else
     {
         if (LastLineRead!=PosPixelY)                                                // Se non vi è discrepanza a livello di pagina, si esegue il controllo
         {                                                                           // solo a livello di linea
             RDCurrentLineAddr=RDCurrentPageAddr+sizeof(TypeColor)*VMaxScreenX*PosPixelY;
             LastLineRead=PosPixelY;
         }
     }

     PointerToVRam = (TypeColor *)(RDCurrentLineAddr+sizeof(TypeColor)*PosPixelX);
     return (*PointerToVRam);
}

