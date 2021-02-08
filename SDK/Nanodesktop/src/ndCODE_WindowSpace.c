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
    ROUTINES PER IL DISEGNO IN WINDOWS-SPACE
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

#define BIT10 0x0200

/*
   -------------------------------------------------------------------------------------------
   ROUTINES WS CHE ESPLETANO FUNZIONI DI SERVIZIO
   -------------------------------------------------------------------------------------------
*/


inline char INTERNAL_ndWS_IsPixelInWS (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle)
/* 
   Analoga a ndWS_IsPixelInWS, ma progettata per essere veloce (usi interni). La routine non esegue controlli,
   perchè si presume siano inutili nel contesto.
*/

{            
     return ((RRPosPixelX<WindowData[WndHandle].WSWndLength) && (RRPosPixelY<WindowData[WndHandle].WSWndHeight));
}



char ndWS_IsPixelInWS (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle)
/* Questa funzione restituisce 1 se il pixel si trova nella Window Space determinata da WndHandle,
   altrimenti restituisce 0. I parametri relativi alla posizione dei pixel devono sempre essere passati
   relativamente al punto in alto a sinistra del window space.
   
   La routine restituisce un codice di errore:
      ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata 
*/

{            
     if ( !ndInitSystemExecuted ) ndInitSystem();
     
     if (WindowData [WndHandle].WindowIsActive)
     {
         return INTERNAL_ndWS_IsPixelInWS (RRPosPixelX, RRPosPixelY, WndHandle);
     }
     else
       return ERR_WINDOW_IS_NOT_ALLOCATED;
     
}


inline char INTERNAL_ndWS_IsPixelInROI (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle)
/* 
    Analoga a ndWS_IsPixelInROI, ma progettata per essere veloce (usi interni).
    
    La routine non esegue controlli, perchè si presume che essi siano inutili nel
    contesto.
*/
{   
    return ( (RRPosPixelX >= WindowData [WndHandle].ROI_PosX1)  
               && (RRPosPixelX <= WindowData [WndHandle].ROI_PosX2) 
                  && (RRPosPixelY >= WindowData [WndHandle].ROI_PosY1) 
                     && (RRPosPixelY <= WindowData [WndHandle].ROI_PosY2) );                        
}




char ndWS_IsPixelInROI (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle)
/* Questa funzione restituisce 1 se il pixel si trova nella Region of Interest della finestra determinata da WndHandle,
   altrimenti restituisce 0. I parametri relativi alla posizione del pixel devono sempre essere passati
   relativamente al punto in alto a sinistra del window space. 
   
   Per questa routine è previsto un codice di errore:
     ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata  
*/

{   
    if ( !ndInitSystemExecuted ) ndInitSystem();
    
    if (WindowData [WndHandle].WindowIsActive)
    {
         return INTERNAL_ndWS_IsPixelInROI (RRPosPixelX, RRPosPixelY, WndHandle);
    }
    else
       return ERR_WINDOW_IS_NOT_ALLOCATED;                       
}




inline char INTERNAL_ndWS_IsPixelInCharArea (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle)
/* 
   Analoga a ndWS_IsPixelInCharArea ma progettata per essere veloce (usi interni).
   
   Questa routine non esegue controlli perchè si presume che essi siano inutili nel
   contesto
*/
{
    return (  (RRPosPixelX >= WindowData[WndHandle].CharArea_PosX1)  &&
                 (RRPosPixelY >= WindowData[WndHandle].CharArea_PosY1) &&
                     (RRPosPixelX < WindowData[WndHandle].CharArea_PosX2) &&
                          (RRPosPixelY < WindowData[WndHandle].CharArea_PosY2)  );  
           
}



char ndWS_IsPixelInCharArea (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle)
/* 
    Questa routine si occupa di verificare se un pixel può essere coperto da un carattere.
    
    E' previsto un codice di errore:
      ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata          
*/
{
   if ( !ndInitSystemExecuted ) ndInitSystem();
    
   if (WindowData [WndHandle].WindowIsActive)
   {
         return INTERNAL_ndWS_IsPixelInCharArea (RRPosPixelX, RRPosPixelY, WndHandle);
   }
   else
     return ERR_WINDOW_IS_NOT_ALLOCATED;                                   
}


inline unsigned char INTERNAL_ndWS_IsPixelCovered_NOSS (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle, TypeColor *ColoreCoprente)
/* 
   Questa routine si occupa di verificare se il pixel è coperto quando il service space 
   risulta essere disattivato. E' progettata esclusivamente per scopi interni
*/
{
   const unsigned char BitMask [9] = {128, 64, 32, 16, 8, 4, 2, 1};

   unsigned short int PosCharX, PosCharY, BitX, BitY;
   unsigned char  CarattereEstratto;
   unsigned char *ByteInFont;                                              // Puntatore al font
   unsigned char FntHandle;

   // Verifica se questo pixel è in un area che può essere impegnata da caratteri
    
   if ( INTERNAL_ndWS_IsPixelInCharArea (RRPosPixelX, RRPosPixelY, WndHandle) ) 
   {
          FntHandle = WindowData [WndHandle].ndWS_CurrentFntHandle;
        
          PosCharX=(RRPosPixelX - WindowData [WndHandle].MargineCharX)/8;         // Determina la posizione del carattere in CharMap
          PosCharY=(RRPosPixelY - WindowData [WndHandle].MargineCharY)/8;

          BitX=(RRPosPixelX - WindowData [WndHandle].MargineCharX) % 8;           // Il bit del carattere che deve essere controllato
                                                                                  // Si osservi che il bit 0 è quello più a DESTRA

          BitY=(RRPosPixelY - WindowData [WndHandle].MargineCharY) % 8;           // Ci dice il byte (riga bit) del carattere che deve
                                                                                  // essere controllato

          CarattereEstratto=WINDOWDATA_CHARMAP (WndHandle, PosCharX, PosCharY);
                       
          if ((CarattereEstratto==0) || (CarattereEstratto==32))                  // Uno spazio deve essere trattato come se non avesse pixel
          {
             *ColoreCoprente=0;
             return 0;                                                            // Se il carattere è vuoto, non c'è copertura. Ovvio no?
          }
          else
          {
             ByteInFont = FontHandlesArray [FntHandle].Data+CarattereEstratto*8+BitY;                            // Il puntatore ByteFont ora punta al byte corretto
             // L'analisi si fa solo se il carattere non è vuoto

             if ( ((*ByteInFont) & (BitMask[BitX])) !=0)
             {
               *ColoreCoprente = WINDOWDATA_COLORCHARMAP (WndHandle, PosCharX, PosCharY);
               return 1;
             }
             else
             {
               *ColoreCoprente = 0;
               return 0;
             }
          }
   }
   else
   {                                                                            // Se il pixel non è nell'area dove ci sono caratteri,
      *ColoreCoprente=0;                                                        // di sicuro non c'è copertura
      return 0;
   }        
}

inline unsigned char INTERNAL_ndWS_IsPixelCovered_SS (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle, TypeColor *ColoreCoprente)
/*
   Questa routine si occupa di verificare se un pixel risulta coperto quando il service
   space è abilito. Usata esclusivamente per usi interni
*/
{
  const unsigned char BitMask [9] = {128, 64, 32, 16, 8, 4, 2, 1};

  unsigned short int PosCharX, PosCharY, BitX, BitY;
  unsigned char  CarattereEstratto;
  unsigned char *ByteInFont;                                              // Puntatore al font
  unsigned char FntHandle;

  unsigned short int RXPosPixelX, RXPosPixelY;
  TypeColor SSColor;
  
  RXPosPixelX = RRPosPixelX - WindowData [WndHandle].ROI_PosX;
  RXPosPixelY = RRPosPixelY - WindowData [WndHandle].ROI_PosY;
  
  SSColor = WINDOWDATA_SS (WndHandle, RXPosPixelX, RXPosPixelY);
  
  if (SSColor==0)         // Se SS color è a 0, allora bisogna visualizzare il colore della mappa caratteri
                          // o del window space
  {
                          
       if ( INTERNAL_ndWS_IsPixelInCharArea (RRPosPixelX, RRPosPixelY, WndHandle) ) 
       {
          FntHandle = WindowData [WndHandle].ndWS_CurrentFntHandle;
          
          PosCharX=(RRPosPixelX - WindowData [WndHandle].MargineCharX)/8;         // Determina la posizione del carattere in CharMap
          PosCharY=(RRPosPixelY - WindowData [WndHandle].MargineCharY)/8;

          BitX=(RRPosPixelX - WindowData [WndHandle].MargineCharX) % 8;           // Il bit del carattere che deve essere controllato
                                                                                  // Si osservi che il bit 0 è quello più a DESTRA

          BitY=(RRPosPixelY - WindowData [WndHandle].MargineCharY) % 8;           // Ci dice il byte (riga bit) del carattere che deve
                                                                                  // essere controllato

          CarattereEstratto=WINDOWDATA_CHARMAP (WndHandle, PosCharX, PosCharY);
                       
          if ((CarattereEstratto==0) || (CarattereEstratto==32))                  // Uno spazio deve essere trattato come se non avesse pixel
          {
             *ColoreCoprente=0;
             return 0;                                                            // Se il carattere è vuoto, non c'è copertura. Ovvio no?
          }
          else
          {
             ByteInFont=FontHandlesArray [FntHandle].Data+CarattereEstratto*8+BitY;  // Il puntatore ByteFont ora punta al byte corretto
                                                                                  // L'analisi si fa solo se il carattere non è vuoto

             if ( ((*ByteInFont) & (BitMask[BitX])) !=0)
             {
               *ColoreCoprente = WINDOWDATA_COLORCHARMAP (WndHandle, PosCharX, PosCharY);
               return 1;
             }
             else
             {
               *ColoreCoprente = 0;
               return 0;
             }
          }
       }
       else
       {                                                                            // Se il pixel non è nell'area dove ci sono caratteri,
          *ColoreCoprente=0;                                                        // di sicuro non c'è copertura
          return 0;
       }        
                                            
  }
  else    // Il service space è diverso da 0 in quel punto
  {
     *ColoreCoprente = SSColor;
     return 1;
  }
     
}


inline unsigned char INTERNAL_ndWS_IsPixelCovered (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle, TypeColor *ColoreCoprente)
/*
   Analoga ad IsPixelCovered ma progettata per funzionare ad alta velocità (usi interni).
*/

{
   if ( (WindowData [WndHandle].SS_Supported) && (WindowData [WndHandle].SS_Enabled) )
      return INTERNAL_ndWS_IsPixelCovered_SS   (RRPosPixelX, RRPosPixelY, WndHandle, ColoreCoprente);
   else
      return INTERNAL_ndWS_IsPixelCovered_NOSS (RRPosPixelX, RRPosPixelY, WndHandle, ColoreCoprente);          
}





char ndWS_IsPixelCovered (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle, TypeColor *ColoreCoprente)
/*
   Questa funzione deve calcolare se un dato pixel viene o meno coperto da un pixel della mappa caratteri.
   Le coordinate devono sempre essere date in modo relativo al primo carattere in alto a sinistra del
   window space.

   Inoltre, la funzione determina qual è il colore che effettivamente copre il pixel nella mappa caratteri.
   Ciò esonera la routine chiamante dal dovere eseguire una seconda volta i complessi calcoli necessari per
   ottenere il colore una volta che ha avuto conferma che il pixel effettivamente è coperto da un carattere,
   ed incrementa fortemente le prestazioni.

   A tale scopo, la routine chiamante deve passare come quarto parametro della funzione l'indirizzo di una
   variabile di tipo TypeColor. In tale variabile verrà depositato il valore del colore del carattere coprente.

   Qualora il pixel non risultasse coperto da un carattere, nella variabile di cui si passa l'indirizzo viene
   depositato il valore 0.
   
   Per questa routine è previsto un codice di errore:
     ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata  
*/

{
    if ( !ndInitSystemExecuted ) ndInitSystem();
    
    if (WindowData [WndHandle].WindowIsActive)                              // La finestra è attivata
       return INTERNAL_ndWS_IsPixelCovered (RRPosPixelX, RRPosPixelY, WndHandle, ColoreCoprente);
    else 
       return ERR_WINDOW_IS_NOT_ALLOCATED;    
}





inline char INTERNAL_XWindowRender_NO_SS (unsigned char WndHandle)
/*
    Questa versione della routine XWindowRender provvede ad eseguire il rendering
    della finestra, senza attivare il service space.
    
    La routine viene usata esclusivamente per uso interno: viene richiamata da
    XWindowRender, che provvede anche ad eseguire i controlli preliminari sullo
    stato della finestra ed ad attivare ndInitSystem.
    
    Per i codici di errore restituiti, si veda la documentazione di XWindowRender.
*/
{
     register unsigned short int RRPosPixelX, RRPosPixelY;
     register unsigned short int RPosPixelX,  RPosPixelY;
     register unsigned short int RXPosPixelX, RXPosPixelY;
     
     const unsigned char BitMask [9] = {128, 64, 32, 16, 8, 4, 2, 1};

     register unsigned short int PosCharX, PosCharY, BitX, BitY;
     register unsigned char  CarattereEstratto;
     register unsigned char *ByteInFont;   // Puntatore al font
     unsigned char FntHandle;
     
     // La scansione viene eseguita esclusivamente sui pixel della ROI             
        
     for (RXPosPixelY=0; RXPosPixelY<WindowData [WndHandle].ROI_LenY; RXPosPixelY++)
     {
           for (RXPosPixelX=0; RXPosPixelX<WindowData [WndHandle].ROI_LenX; RXPosPixelX++)
           {
              RRPosPixelX=WindowData [WndHandle].ROI_PosX + RXPosPixelX;
              RRPosPixelY=WindowData [WndHandle].ROI_PosY + RXPosPixelY;
              // Le coordinate del pixel rispetto all'origine del window space
              
              RPosPixelX=WindowData [WndHandle].AA3_X1 + RXPosPixelX;
              RPosPixelY=WindowData [WndHandle].AA3_Y1 + RXPosPixelY;
              // Le coordinate del medesimo pixel rispetto all'origine del logical space
              
              // A questo punto è necessario verificare se il pixel è coperto da un carattere
              
              if (  INTERNAL_ndWS_IsPixelInCharArea (RRPosPixelX, RRPosPixelY, WndHandle)  )  
              {
                   FntHandle = WindowData [WndHandle].ndWS_CurrentFntHandle;  
                   
                   PosCharX=(RRPosPixelX - WindowData [WndHandle].MargineCharX)/8;         // Determina la posizione del carattere in CharMap
                   PosCharY=(RRPosPixelY - WindowData [WndHandle].MargineCharY)/8;

                   CarattereEstratto=WINDOWDATA_CHARMAP (WndHandle, PosCharX, PosCharY);

                   if ((CarattereEstratto!=0) && (CarattereEstratto!=32))                  // Il carattere è diverso da 0 o spazio
                   {
                        BitX=(RRPosPixelX - WindowData [WndHandle].MargineCharX) % 8;      // Il bit del carattere che deve essere controllato
                                                                                           // Si osservi che il bit 0 è quello più a DESTRA

                        BitY=(RRPosPixelY - WindowData [WndHandle].MargineCharY) % 8;      // Ci dice il byte (riga bit) del carattere che deve
                                                                                           // essere controllato
  
                                 
                        ByteInFont=FontHandlesArray [FntHandle].Data+CarattereEstratto*8+BitY;                          // Il puntatore ByteFont ora punta al byte corretto
                                                                                           // L'analisi si fa solo se il carattere non è vuoto

                        if ( ((*ByteInFont) & (BitMask[BitX])) !=0)
                        {
                           WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) =
                           WINDOWDATA_COLORCHARMAP (WndHandle, PosCharX, PosCharY);
                      
                           // Il pixel è coperto, copia nella pagina logica il colore del carattere
                        }
                        else
                        {
                           WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) =
                           WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY);
                      
                          // Il pixel non è coperto: copia nella pagina logica il colore del pixel
                        }
                    }
                    else  // Il carattere è vuoto o è uno spazio
                    {
                          WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) =
                          WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY);
                      
                      // Se il carattere è vuoto, non c'è copertura. Ovvio no?
                    }
                   
              }
              else   // Il pixel non è nella zona dove ci sono i caratteri
              {                                                                            // Se il pixel non è nell'area dove ci sono caratteri,
                  WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) =                      // di sicuro non c'è copertura
                  WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY);                        
              }
           }
     }
          
     ndLP_DrawScrollerRectangles (WndHandle);
     
     WindowsRender();     // Il rendering finale rende visibili le modifiche alla pagina 
                          // logica, ma viene eseguito solo se la finestra non è minimizzata
     return 0;

}
 
     



inline char INTERNAL_XWindowRender_SS (unsigned char WndHandle)
/*
    Questa versione della routine XWindowRender provvede ad eseguire il rendering
    della finestra, utilizzando il service space.
    
    Quando un pixel del service space ha un valore superiore a 0, viene visualizzato
    il pixel del service space in luogo di quello del window space.
    
    La routine viene usata esclusivamente per uso interno: viene richiamata da
    XWindowRender, che provvede anche ad eseguire i controlli preliminari sullo
    stato della finestra ed ad attivare ndInitSystem.
    
    Per i codici di errore restituiti, si veda la documentazione di XWindowRender.
*/

{
     register unsigned short int RRPosPixelX, RRPosPixelY;
     register unsigned short int RPosPixelX,  RPosPixelY;
     register unsigned short int RXPosPixelX, RXPosPixelY;
     
     const unsigned char BitMask [9] = {128, 64, 32, 16, 8, 4, 2, 1};

     register unsigned short int PosCharX, PosCharY, BitX, BitY;
     register unsigned char  CarattereEstratto;
     register unsigned char *ByteInFont;                                              // Puntatore al font
     unsigned char FntHandle;

     TypeColor SSPixel;

     if (WindowData [WndHandle].MenuBarIsChanged) 
     {
        ndWinMenuRender (WndHandle);
     }
     
     // Esegue preventivamente un nuovo rendering della finestra   
        
     // La scansione viene eseguita esclusivamente sui pixel della ROI             
        
     for (RXPosPixelY=0; RXPosPixelY<WindowData [WndHandle].ROI_LenY; RXPosPixelY++)
     {
         
           for (RXPosPixelX=0; RXPosPixelX<WindowData [WndHandle].ROI_LenX; RXPosPixelX++)
           {
               
                SSPixel = WINDOWDATA_SS (WndHandle, RXPosPixelX, RXPosPixelY);
                
                if (SSPixel==0)
                {
                    // Il window space deve essere visualizzato
         
                    RRPosPixelX=WindowData [WndHandle].ROI_PosX + RXPosPixelX;
                    RRPosPixelY=WindowData [WndHandle].ROI_PosY + RXPosPixelY;
                    // Le coordinate del pixel rispetto all'origine del window space
              
                    RPosPixelX=WindowData [WndHandle].AA3_X1 + RXPosPixelX;
                    RPosPixelY=WindowData [WndHandle].AA3_Y1 + RXPosPixelY;
                    // Le coordinate del medesimo pixel rispetto all'origine del logical space
              
                    // A questo punto è necessario verificare se il pixel è coperto da un carattere
              
                    if (  INTERNAL_ndWS_IsPixelInCharArea (RRPosPixelX, RRPosPixelY, WndHandle)  )  
                    {
                          FntHandle = WindowData [WndHandle].ndWS_CurrentFntHandle;   
                          
                          // Il pixel è coperto da un carattere
                          
                          PosCharX=(RRPosPixelX - WindowData [WndHandle].MargineCharX)/8;         // Determina la posizione del carattere in CharMap
                          PosCharY=(RRPosPixelY - WindowData [WndHandle].MargineCharY)/8;

                          CarattereEstratto=WINDOWDATA_CHARMAP (WndHandle, PosCharX, PosCharY);

                          if ((CarattereEstratto!=0) && (CarattereEstratto!=32))                  // Il carattere è diverso da 0 o spazio
                          {
                                                     
                             BitX=(RRPosPixelX - WindowData [WndHandle].MargineCharX) % 8;        // Il bit del carattere che deve essere controllato
                                                                                                  // Si osservi che il bit 0 è quello più a DESTRA

                             BitY=(RRPosPixelY - WindowData [WndHandle].MargineCharY) % 8;        // Ci dice il byte (riga bit) del carattere che deve
                                                                                                  // essere controllato
          
                             ByteInFont=FontHandlesArray [FntHandle].Data+CarattereEstratto*8+BitY;                          // Il puntatore ByteFont ora punta al byte corretto
  
                             // L'analisi si fa solo se il carattere non è vuoto

                             if ( ((*ByteInFont) & (BitMask[BitX])) !=0)
                             {
                                WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) =
                                WINDOWDATA_COLORCHARMAP (WndHandle, PosCharX, PosCharY);
                      
                                // Il pixel è coperto, copia nella pagina logica il colore del carattere
                             }
                             else
                             {
                                WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) =
                                WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY);
                      
                                // Il pixel non è coperto: copia nella pagina logica il colore del pixel
                             }
                          }
                          else  // Il carattere è vuoto o è uno spazio
                          {
                             WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) =
                             WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY);
                      
                             // Se il carattere è vuoto, non c'è copertura. Ovvio no?
                          }
                   
                    }
                    else   // Il pixel non è nella zona dove ci sono i caratteri
                    {                                                                               // Se il pixel non è nell'area dove ci sono caratteri,
                        WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) =                          // di sicuro non c'è copertura
                        WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY);                        
                    }
                }
                else   // C'è un pixel del Service Space che copre il Window Space
                {
                   RPosPixelX=WindowData [WndHandle].AA3_X1 + RXPosPixelX;
                   RPosPixelY=WindowData [WndHandle].AA3_Y1 + RXPosPixelY;
                   // Le coordinate del medesimo pixel rispetto all'origine del logical space    
                
                   WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY) = SSPixel;
                }    
           }
     }
          
     ndLP_DrawScrollerRectangles (WndHandle);
     
     WindowsRender();     // Il rendering finale rende visibili le modifiche alla pagina 
                          // logica, ma viene eseguito solo se la finestra non è minimizzata
     return 0;
     
    
}


char XWindowRender (unsigned char WndHandle)
/*
     Questa routine è molto importante, e non va confusa con la normale WindowsRender.

     Concettualmente, lo scopo di questa routine è analizzare i pixel di una finestra
     punto per punto: per ogni pixel, se questo è occupato da un carattere viene
     visualizzato il colore della colorcharmap, mentre se il pixel non è
     occupato da un carattere, viene visualizzato il colore del window space.
     
     Se, tuttavia, il service space della finestra è abilitato, i pixel del service
     space non nulli si sovrapporranno a quelli della colorcharmap e del
     window space.
     
     La routine non esegue internamente il lavoro, ma richiama due routines 
     specializzate che si occupano di operare quando il service space è
     disattivato e quando il service space è attivato. 
     
     Anche questa procedura prevede un codice di errore in uscita

           ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
   if ( !ndInitSystemExecuted ) ndInitSystem();  
     
   if ((WindowData [WndHandle].WindowIsActive))
   {
      if (!((WindowData [WndHandle].SS_Supported) && (WindowData [WndHandle].SS_Enabled)))
        return INTERNAL_XWindowRender_NO_SS (WndHandle);
      else
        return INTERNAL_XWindowRender_SS (WndHandle);
   }  
   else
         return ERR_WINDOW_IS_NOT_ALLOCATED;  
}




/*
   ------------------------------------------------------------------------------------------------
   ROUTINE WS CHE SI OCCUPANO DELLA SCRITTURA DEI CARATTERI
   ------------------------------------------------------------------------------------------------
*/

char ndWS_WriteChar (unsigned int PosCursX, unsigned int PosCursY, char Carattere, TypeColor Colore, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione permette di scrivere un carattere nella finestra definita da WndHandle. La funzione scrive in
     un punto preciso dello schermo, senza dunque spostare il cursore.

     Questa funzione è stata progettata per la scrittura di caratteri singoli. Al termine del processo, richiama
     XWindowRender per disegnare il carattere sullo schermo.

     Se non si vuole che questa avvenga, si può passare come ultimo parametro la costante manifesta NORENDER
     (che è comunemente usato da funzioni come ndWS_WriteLetter o ndWS_WriteLn).

     Questa funzione restituisce il codice di errore:

           ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
     if ( !ndInitSystemExecuted ) ndInitSystem();
           
     if ((WindowData [WndHandle].WindowIsActive))
     {
        WINDOWDATA_CHARMAP (WndHandle, PosCursX, PosCursY)=Carattere;
        WINDOWDATA_COLORCHARMAP (WndHandle, PosCursX, PosCursY)=Colore;

        if (RenderNow) XWindowRender(WndHandle);
        return 0;
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;
}

char ndWS_CharScrolling (unsigned char WndHandle)
/* Questa funzione esegue lo scrolling dei caratteri nel buffer Caratteri del
   Window Space. Si tenga sempre presente che questo scrolling non mostra effetti
   sul video fin quando non viene eseguito da qualcuno un XWindowRender.

   Questa funzione ritorna un codice di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
     unsigned short int CounterX, CounterY;
     unsigned short int LastRow;
     
     if ( !ndInitSystemExecuted ) ndInitSystem();

     if ((WindowData [WndHandle].WindowIsActive)==1)
     {
        for (CounterY=1; CounterY<WindowData [WndHandle].WS_MaxCursY; CounterY++)
        {
            for (CounterX=0; CounterX<WindowData [WndHandle].WS_MaxCursX; CounterX++)
            {
                WINDOWDATA_CHARMAP (WndHandle, CounterX, CounterY-1) = WINDOWDATA_CHARMAP (WndHandle, CounterX, CounterY);
                WINDOWDATA_COLORCHARMAP (WndHandle, CounterX, CounterY-1) = WINDOWDATA_COLORCHARMAP (WndHandle, CounterX, CounterY);
            }
        }

        // La routine di scrolling dei caratteri scatta quando WS_PosCursY è uguale a
        // WS_MaxCharY: ciò significa che non è possibile scrivere nella riga di
        // coordinata WS_MaxCharY. L'ultima riga scrivibile è la WS_MaxCharY-1.

        // Il doppio ciclo iterativo precedente scrive su tutte le righe salvo la
        // prima, ossia per Counter che và da 1 a WS_MaxCursY-1.

        LastRow =(WindowData [WndHandle].WS_MaxCursY)-1;

        for (CounterX=0; CounterX<(WindowData [WndHandle].WS_MaxCursX); CounterX++)
        {
            WINDOWDATA_CHARMAP (WndHandle, CounterX, LastRow)=0;                 // L'ultima riga della mappa caratteri viene riempita
            WINDOWDATA_COLORCHARMAP (WndHandle, CounterX, LastRow)=0;            // di zeri: non comparirà nel prossimo rendering
        }

        return 0;
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndWS_CarriageReturn (unsigned char WndHandle)
/*
  Questa funzione si occupa di eseguire il carriage return della posizione del cursore all'interno
  della finestra definita da WndHandle. Può sembrare una funzione banale, ma occorre tenere presente
  che essa può avviare la routine CharScrolling che si occupa dello scroll dello schermo.

   Anche per questa funzione è previsto il codice di errore di uscita:

         ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
     if ( !ndInitSystemExecuted ) ndInitSystem();
         
     if ((WindowData [WndHandle].WindowIsActive)==1)
     {
        WindowData [WndHandle].WS_PosCursY++;
        WindowData [WndHandle].WS_PosCursX=0;

        if ((WindowData [WndHandle].WS_PosCursY) == (WindowData [WndHandle].WS_MaxCursY))
        {
           ndWS_CharScrolling (WndHandle);     // Se lo schermo è oltre l'ultima riga della finestra, bisogna eseguire lo
           WindowData [WndHandle].WS_PosCursY--;    // scrolling dei caratteri e riportare il cursore ad un valore corretto
        }

         return 0;
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;
}

char ndWS_GoCursor (unsigned short int PosCursX, unsigned short int PosCursY, unsigned char WndHandle)
/*
   Questa routine si occupa di inserire il cursore del window space della finestra determinata da
   WndHandle in un punto specifico dello schermo.

   Sono previsti due codici di errore:
       ERR_WINDOW_IS_NOT_ALLOCATED
       ERR_WSCURSOR_IS_OUT_OF_WS
*/

{
       if ( !ndInitSystemExecuted ) ndInitSystem();
       
       if (WindowData [WndHandle].WindowIsActive)
       {
          if ( (PosCursX<WindowData [WndHandle].WS_MaxCursX) && (PosCursY<WindowData [WndHandle].WS_MaxCursY) )
          {
                WindowData [WndHandle].WS_PosCursX=PosCursX;
                WindowData [WndHandle].WS_PosCursY=PosCursY;
                return 0;
          }
          else
            return ERR_WSCURSOR_IS_OUT_OF_WS;

       }
       else
       return ERR_WINDOW_IS_NOT_ALLOCATED;
}

char ndWS_ClrScr (unsigned char WndHandle)
/*
    Questa routine si occupa di cancellare completamente la mappa caratteri del Window Space e di riportare il
    cursore all'inizio della schermo.

    La funzione non tocca il buffer WS, che quindi continua a funzionare. Per cancellare completamente la
    finestra, si deve usare la funzione ndWS_ClrWindow.

    E' previsto un codice di errore:
      ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
       if ( !ndInitSystemExecuted ) ndInitSystem();
      
       if (WindowData [WndHandle].WindowIsActive)
       {
          unsigned short int CounterX, CounterY;

          for (CounterY=0; CounterY<(WindowData [WndHandle].WS_MaxCursY); CounterY++)
          {
              for (CounterX=0; CounterX<(WindowData [WndHandle].WS_MaxCursX); CounterX++)
              {
                  WINDOWDATA_CHARMAP (WndHandle, CounterX, CounterY)=0;
                  WINDOWDATA_COLORCHARMAP (WndHandle, CounterX, CounterY)=0;
              }
          }

          ndWS_GoCursor (0, 0, WndHandle);
          XWindowRender (WndHandle);                  // E la pagina si cancella

		  return 0;
       }
       else
       return ERR_WINDOW_IS_NOT_ALLOCATED;
}

char ndWS_WriteLetter (char Carattere, TypeColor Colore, unsigned char WndHandle, unsigned char RenderNow)
/*
   Questa funzione scrive un carattere sullo schermo nel punto definito dai campi PosCursX e PosCursY della
   struttura WindowData. La funzione dovrebbe essere usata solo quando si vuole scrivere un singolo carattere
   sullo schermo: dopo avere aggiornato la mappa caratteri essa esegue l' x-rendering in modo che il nuovo
   carattere compaia sullo schermo

   Anche per questa funzione è previsto il codice di errore di uscita:

         ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
     if ( !ndInitSystemExecuted ) ndInitSystem();
         
     if (WindowData [WndHandle].WindowIsActive)
     {
        ndWS_WriteChar (WindowData [WndHandle].WS_PosCursX, WindowData [WndHandle].WS_PosCursY, Carattere, Colore, WndHandle, NORENDER);

        WindowData [WndHandle].WS_PosCursX++;                             // Incrementa la posizione X del cursore

        if ( (WindowData [WndHandle].WS_PosCursX) == (WindowData [WndHandle].WS_MaxCursX) )
           ndWS_CarriageReturn (WndHandle);                       // La funzione CarriageReturn può anche eseguire lo
                                                                       // scrolling dei caratteri ove necessario

        if (RenderNow) XWindowRender (WndHandle);                      // Richiama la funzione per il rendering
        return 0;
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;
}



char ndWS_Write (char *str, TypeColor Colore, unsigned char WndHandle, unsigned char RenderNow)
/*
   Questa funzione, insieme a ndLL_WriteLn si occupa di stampare una stringa nello
   spazio WS della finestra definita da WndHandle. La posizione è quella definita dalle
   coordinate del cursore WS_PosCursX e WS_PosCursY nella relativa struttura
   WindowData.

   Anche per questa funzione è previsto il codice di errore di uscita:

         ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
    int MaxLenStr=255;
	int t=0;
	char *strpointer;			//Puntatore alla stringa

    if ( !ndInitSystemExecuted ) ndInitSystem();

	if ((WindowData [WndHandle].WindowIsActive)==1)
	{
    	strpointer=str;

	    while (*strpointer!=0 && t<MaxLenStr)
	    {
		      ndWS_WriteLetter (*strpointer, Colore, WndHandle, NORENDER);
		      strpointer++;

              t++;
        }

        if (RenderNow) XWindowRender (WndHandle);          // Al termine del processo di copia il rendering fa apparire la stringa
        return 0;
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;

}

char ndWS_WriteLn (char *str, TypeColor Colore, unsigned char WndHandle, unsigned char RenderNow)
/*
   Questa funzione è identica alla precedente ndWS_WriteLn, salvo che per
   il fatto che al termine del processo manda il cursore a capo.

   Anche per questa funzione è previsto il codice di errore di uscita:

         ERR_WINDOW_IS_NOT_ALLOCATED
*/

{
    int MaxLenStr=255;
	int t=0;
	char *strpointer;			//Puntatore alla stringa

    if ( !ndInitSystemExecuted ) ndInitSystem();

	if ((WindowData [WndHandle].WindowIsActive)==1)
	{
    	strpointer=str;

	    while (*strpointer!=0 && t<MaxLenStr)
	    {
		      ndWS_WriteLetter (*strpointer, Colore, WndHandle, NORENDER);
		      strpointer++;

              t++;
        }

        ndWS_CarriageReturn (WndHandle);           // Va a capo (e scrolla i caratteri se necessario)

        if (RenderNow) XWindowRender (WndHandle);          // Al termine del processo di copia il rendering fa apparire la stringa
        return 0;
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;

}

char ndWS_PrintLn (char WndHandle, TypeColor Color, char RenderNow, char *FirstParam, ...)
/*
    Questa routine si occupa di visualizzare una stringa complessa nella mappa
    caratteri del window space della finestra WndHandle.
    
    Il funzionamento è analogo a quello previsto per la printf della libc, salvo
    per il fatto che l'utente non deve inserire un \n alla fine per andare a
    capo (viene fatto automaticamente).
    
    Sono previsti tutti i codici di errore previsti per ndWS_WriteLn, ed in più
    un codice di errore:
       ERR_INTERNAL_ERROR        Un problema con snprintf
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
    
      va_end(args);         // Disalloca la lista degli argomenti 
      
      if (ErrRep1>=0)       // Non ci sono stati problemi nella scrittura
      {
           return ndWS_WriteLn (&IntBufPrintF, Color, WndHandle, RenderNow);
      } 
      else
        return ERR_INTERNAL_ERROR;  
}


char ndWS_Print (char WndHandle, TypeColor Color, char RenderNow, char *FirstParam, ...)
/*
    Questa routine è perfettamente identica a ndWS_PrintLn, ma non viene eseguito
    alcun carriage return alla fine della scrittura    
*/
{
      va_list args;                       // Lista degli argomenti da passare. Il tipo va_list indica una
                                          // lista di parametri passati alla funzione per mezzo di un 
                                          // ellissi. La funzione che vuole accedere ai singoli parametri
                                          // deve usare la macro va_arg (puntatore alla lista, tipo parametro)
                                          // per ottenere i vari parametri uno alla volta
      
      struct s_strargument xarg;          // Struttura argomenti: contiene i parametri necessari
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
            return ndWS_Write (&IntBufPrintF, Color, WndHandle, RenderNow);
      } 
      else
        return ERR_INTERNAL_ERROR;
      
}



/*
  ----------------------------------------------------------------------------------------------
  ROUTINE WS CHE SI OCCUPANO DELLA CREAZIONE DI ELEMENTI GRAFICI SULLO SCHERMO: PUNTI
  ----------------------------------------------------------------------------------------------
*/


inline int ndWS_GetPixel (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, unsigned char WndHandle)
/* Questa funzione restituisce il colore del pixel nel buffer WS. 

   Il colore restituito è sempre quello del buffer WS, anche se il pixel è coperto da un carattere.
   La funzione restituisce i seguenti codici di errore:
               
      ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è stata allocata
      ERR_PIXEL_NOT_IN_WS           Il pixel non è nel Windows Space
*/

{
      if ( !ndInitSystemExecuted ) ndInitSystem();

      if (WindowData[WndHandle].WindowIsActive)                                         // Controlliamo innanzitutto che la finestra sia allocata
      {

            if (INTERNAL_ndWS_IsPixelInWS (RRPosPixelX, RRPosPixelY, WndHandle))                 // Controlliamo inoltre che il pixel sia in WS
            {
               return WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY);              // Inserisce il colore nel buffer WS
            }
            else
               return ERR_PIXEL_NOT_IN_WS;

      }
      else
          return ERR_WINDOW_IS_NOT_ALLOCATED;
}
     


inline char ndWS_PutPixel (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, TypeColor Color, unsigned char WndHandle, unsigned char RenderNow)
/*
   Questa funzione scrive un pixel nel buffer WS. In seguito verifica:

   a) che lo stesso pixel non sia in quel momento al di fuori della regione di interesse della finestra
   b) che lo stesso pixel non sia in quel momento impegnato da un pixel di un carattere.
   c) che lo stesso pixel non sia coperto da un pixel del service space

   Se entrambe queste condizioni non sono verificate, si provvede a lanciare una LP_PutPixel nelle corrette
   coordinate. Le coordinate dei punti devono sempre essere fornite relativamente alla posizione di inizio
   della window space, determinata dal punto in alto a sinistra di quest'ultima

   Si noti che questa procedura è molto più veloce che modificare il buffer WS ed eseguire l'x-render di
   tutta la finestra. Per tale ragione, questa WS_PutPixel è la procedura d'elezione quando si vuole
   scrivere un singolo punto sullo schermo.

   Sono previsti due codici di errore in uscita

      ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è stata allocata
      ERR_PIXEL_NOT_IN_WS           Il pixel non è nel Windows Space
*/

{
      unsigned int RPosPixelX, RPosPixelY;
      unsigned int RXPosPixelX, RXPosPixelY;

      TypeColor ColoreCoprente;

      if ( !ndInitSystemExecuted ) ndInitSystem();

      if (WindowData[WndHandle].WindowIsActive)                                         // Controlliamo innanzitutto che la finestra sia allocata
      {
            if (INTERNAL_ndWS_IsPixelInWS (RRPosPixelX, RRPosPixelY, WndHandle))        // Controlliamo inoltre che il pixel sia in WS
            {
                  WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY)=Color;            // Inserisce il colore nel buffer WS

                  if (RenderNow)
                  {
                     if ( INTERNAL_ndWS_IsPixelInROI (RRPosPixelX, RRPosPixelY, WndHandle) )  // Controlliamo se il pixel in quel momento è nella Region of
                     {                                                                        // of interest della finestra
                         RXPosPixelX = RRPosPixelX - WindowData[WndHandle].ROI_PosX;          // Coordinate del punto sullo schermo rispetto all'angolo
                         RXPosPixelY = RRPosPixelY - WindowData[WndHandle].ROI_PosY;          // superiore della ROI

                         RPosPixelX = WindowData [WndHandle].AA3_X1 + RXPosPixelX;
                         RPosPixelY = WindowData [WndHandle].AA3_Y1 + RXPosPixelY;

                         if (INTERNAL_ndWS_IsPixelCovered (RRPosPixelX, RRPosPixelY, WndHandle, &ColoreCoprente))
                            WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY)=ColoreCoprente;
                         else
                            WINDOWDATA_P (WndHandle, RPosPixelX, RPosPixelY)=Color;
                     }
                  
                     WindowsRender ();   // Esegue la visualizzazione della finestra   
                  }
                  
                  return 0;                  
                  
            }
            else
              return ERR_PIXEL_NOT_IN_WS;
      }
      else
          return ERR_WINDOW_IS_NOT_ALLOCATED;
}

inline void ndWS_PutPixel_NORENDER_FAST (unsigned short int RRPosPixelX, unsigned short int RRPosPixelY, TypeColor Color, unsigned char WndHandle)
/*
   Questa funzione scrive un pixel nel buffer WS. E' la più veloce delle routine WS_PutPixel, ma lascia all'applicazione
   chiamante il compito di verificare se la finestra è attivata e se il pixel si trova nel Window Space della finestra.

   USANDO QUESTA ROUTINE DA UTENTE E' POSSIBILE PROVOCARE DANNI GRAVI AL SISTEMA GRAFICO, PER TALE
   RAGIONE ESSA NON VIENE ESPOSTA IN NANODESKTOP.H.

   E' stata progettata esclusivamente per uso interno.

   Proprio per via della elevata velocità, è utilizzata da tutte le routine di questa libreria che disegnano in background
   rettangoli, o immagini, che la usano diverse migliaia di volte, ed infine eseguono sempre un XWindowRender
   per visualizzare l'immagine.

   Non sono previsti codici di errore in uscita.
*/

{
   WINDOWDATA_WS (WndHandle, RRPosPixelX, RRPosPixelY)=Color;  // Inserisce il colore nel buffer WS
}


/*
  ----------------------------------------------------------------------------------------------
  ROUTINE WS CHE SI OCCUPANO DELLA CREAZIONE DI ELEMENTI GRAFICI SULLO SCHERMO: LINEE
  ----------------------------------------------------------------------------------------------
*/



char ndWS_DrawLine      (int RRPosX1, int RRPosY1, int RRPosX2, int RRPosY2, 
                         TypeColor Color, unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive una linea sullo schermo, tra i due punti le cui coordinate sono
     date da RRPosX1,RRPosY1 e RRPosX2,RRPosY2. 
     
     La funzione accetta anche coordinate negative: in tal caso viene disegnato solo ciò
     che è rappresentabile sullo schermo.
     
     E' previsto un codice di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
   
*/

{
    float CounterR;
    float SignX, SignY;
    float ScaleX, ScaleY;
    float m1, m2;
    int KPosX, KPosY;
    char BoundCheckOk;
       
    if ( !ndInitSystemExecuted ) ndInitSystem();
       
	if (WindowData [WndHandle].WindowIsActive)
	{
       CounterR=0;
       
       ScaleX = (float) (RRPosX2 - RRPosX1);
       ScaleY = (float) (RRPosY2 - RRPosY1);
       
       if ( (INTERNAL_ndWS_IsPixelInWS (RRPosX1, RRPosY1, WndHandle)) && (INTERNAL_ndWS_IsPixelInWS (RRPosX2, RRPosY2, WndHandle))  )
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
                   
                   if (BoundCheckOk) ndWS_PutPixel_NORENDER_FAST (KPosX, KPosY, Color, WndHandle);
                   else ndWS_PutPixel (KPosX, KPosY, Color, WndHandle, NORENDER);
               }
               
               if (RenderNow) XWindowRender (WndHandle);
               return 0;
           }
           else
           {
               for (CounterR=0; CounterR<=fabs(ScaleY); CounterR++)
               {
                   KPosX = RRPosX1 + m2*SignY*CounterR;
                   KPosY = RRPosY1 + SignY*CounterR;
                   
                   if (BoundCheckOk) ndWS_PutPixel_NORENDER_FAST (KPosX, KPosY, Color, WndHandle);
                   else ndWS_PutPixel (KPosX, KPosY, Color, WndHandle, NORENDER);
                   
               }
               
               if (RenderNow) XWindowRender (WndHandle);
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
               
                       if (BoundCheckOk) ndWS_PutPixel_NORENDER_FAST (KPosX, KPosY, Color, WndHandle);
                       else ndWS_PutPixel (KPosX, KPosY, Color, WndHandle, NORENDER);    
                   }
                   
                   if (RenderNow) XWindowRender (WndHandle);
                   return 0;
           }
           
           if ((ScaleX!=0) && (ScaleY==0))      // Linea orizzontale
           {       
                   for (CounterR=0; CounterR<=fabs(ScaleX); CounterR++)
                   {
                       KPosX = RRPosX1 + SignX*CounterR;
                       KPosY = RRPosY1;  
               
                       if (BoundCheckOk) ndWS_PutPixel_NORENDER_FAST (KPosX, KPosY, Color, WndHandle);
                       else ndWS_PutPixel (KPosX, KPosY, Color, WndHandle, NORENDER);
               
                   }
                   
                   if (RenderNow) XWindowRender (WndHandle);
                   return 0;
           }    
           
           if ((ScaleX==0) && (ScaleY==0))     // La linea si riduce ad un punto
           {
              if (BoundCheckOk) ndWS_PutPixel_NORENDER_FAST (RRPosX1, RRPosY1, Color, WndHandle);
                else ndWS_PutPixel (RRPosX1, RRPosY1, Color, WndHandle, NORENDER);
              
              if (RenderNow) XWindowRender (WndHandle);
              return 0;
           }
       }
     
    }
    else
      return ERR_WINDOW_IS_NOT_ALLOCATED;
    
}


char ndWS_DrawPoly (int WndHandle, TypeColor Color, char RenderNow, int NrPixels, ...)
/*
     Questa routine consente di disegnare una linea spezzata o un poligono nella finestra
     caratterizzata da un dato WndHandle. 
     
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
     
     La routine restituisce i seguenti codici di errore:
       ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata          
*/
{
  va_list args;    // Puntatore agli argomenti   
  int Counter;
  
  int CoordX, CoordY;
  int FirstPixelIsSaved;
  
  int FirstPixelX, FirstPixelY;
  int SecondPixelX, SecondPixelY;
  
  if ( !ndInitSystemExecuted ) ndInitSystem();
       
  if (WindowData [WndHandle].WindowIsActive)
  {
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
         
            if (NrPixels==1) return ndWS_PutPixel (CoordX, CoordY, Color, WndHandle, RenderNow);                
            // Caso speciale: c'è un solo pixel nella catena
         }
         else
         {
            SecondPixelX = CoordX;
            SecondPixelY = CoordY;
            
            ndWS_DrawLine (FirstPixelX, FirstPixelY, SecondPixelX, SecondPixelY, Color, WndHandle, NORENDER);
            // Disegna la linea
            
            FirstPixelX = SecondPixelX;
            FirstPixelY = SecondPixelY;
            // Scambio di coordinate: il penultimo punto della spezzata diventa l'ultimo  
         }
     }
     
     va_end(args);     // Rilascia il puntatore argomenti
     
     if (RenderNow) XWindowRender (WndHandle);
     return 0;  
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;   
} 







/*
  ----------------------------------------------------------------------------------------------
  ROUTINE WS CHE SI OCCUPANO DELLA CREAZIONE DI ELEMENTI GRAFICI SULLO SCHERMO: ROUTINE DI FILL
  ----------------------------------------------------------------------------------------------
*/




char INTERNAL_ndWS_FillArea (int PosX, int PosY, TypeColor Color, TypeColor BorderColor, char WndHandle)
/*
    Questa routine è il motore di FillArea. E' ottimizzata per eseguire il minimo numero di
    controlli possibili, in modo da ridurre l'overhead di gestione, in modo che sia possibile
    utilizzarla in un algoritmo ricorsivo.
*/
{
    int SPosX, SPosY;
    TypeColor ScrColor;
    
    ndWS_PutPixel_NORENDER_FAST (PosX, PosY, Color, WndHandle);
        
    SPosX = PosX;
    SPosY = PosY - 1;
    
    if ( (SPosX>=0) && (SPosX<WindowData [WndHandle].WSWndLength) && (SPosY>=0) && (SPosY<WindowData [WndHandle].WSWndHeight) )
    {
        ScrColor = WINDOWDATA_WS (WndHandle, SPosX, SPosY);
        
        if ( (ScrColor!=BorderColor) && (ScrColor!=Color) )
           INTERNAL_ndWS_FillArea (SPosX, SPosY, Color, BorderColor, WndHandle); 
    }
    
    SPosX = PosX - 1;
    SPosY = PosY;
    
    if ( (SPosX>=0) && (SPosX<WindowData [WndHandle].WSWndLength) && (SPosY>=0) && (SPosY<WindowData [WndHandle].WSWndHeight) )
    {
        ScrColor = WINDOWDATA_WS (WndHandle, SPosX, SPosY);
         
        if ( (ScrColor!=BorderColor) && (ScrColor!=Color) )
           INTERNAL_ndWS_FillArea (SPosX, SPosY, Color, BorderColor, WndHandle); 
    }
    
    SPosX = PosX + 1;
    SPosY = PosY;
    
    if ( (SPosX>=0) && (SPosX<WindowData [WndHandle].WSWndLength) && (SPosY>=0) && (SPosY<WindowData [WndHandle].WSWndHeight) )
    {
        ScrColor = WINDOWDATA_WS (WndHandle, SPosX, SPosY);
         
        if ( (ScrColor!=BorderColor) && (ScrColor!=Color) )
           INTERNAL_ndWS_FillArea (SPosX, SPosY, Color, BorderColor, WndHandle); 
    }
    
    
    SPosX = PosX;
    SPosY = PosY + 1;
    
    if ( (SPosX>=0) && (SPosX<WindowData [WndHandle].WSWndLength) && (SPosY>=0) && (SPosY<WindowData [WndHandle].WSWndHeight) )
    {
        ScrColor = WINDOWDATA_WS (WndHandle, SPosX, SPosY);
        
        if ( (ScrColor!=BorderColor) && (ScrColor!=Color) )
           INTERNAL_ndWS_FillArea (SPosX, SPosY, Color, BorderColor, WndHandle); 
    }
    
    return 0;
}

char ndWS_FillArea (int PosX, int PosY, TypeColor Color, TypeColor BorderColor, char WndHandle, char RenderNow)
/*
    Questa routine provvede a riempire una certa zona dello schermo con il colore Color. 
    L'algoritmo di filler si ferma solo quando viene trovato un colore pari a BorderColor.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata   
         ERR_PIXEL_IS_OUT_OF_WINDOW          Il pixel è fuori dalla finestra
*/
{
   if ( !ndInitSystemExecuted ) ndInitSystem();
          
   if (WindowData [WndHandle].WindowIsActive)
   {
      if ( (PosX>=0) && (PosX<WindowData [WndHandle].WSWndLength) && (PosY>=0) && (PosY<WindowData [WndHandle].WSWndHeight) )
      {
          INTERNAL_ndWS_FillArea (PosX, PosY, Color, BorderColor, WndHandle);
          if (RenderNow) XWindowRender (WndHandle);
          
          return 0;
      }
      else
        return ERR_PIXEL_IS_OUT_OF_WINDOW;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}


/*
  ----------------------------------------------------------------------------------------------
  ROUTINE WS CHE SI OCCUPANO DELLA CREAZIONE DI ELEMENTI GRAFICI SULLO SCHERMO: RETTANGOLI
  ----------------------------------------------------------------------------------------------
*/


char ndWS_DrawSpRectangle (unsigned short int RRPosX1, unsigned short int RRPosY1, 
                           unsigned short int RRPosX2, unsigned short int RRPosY2, 
                           TypeColor Color, TypeColor BorderColor, ndint64 CodeStyle,
                           unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo nel Window Space della finestra identificata da WndHandle.
     
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
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
          ERR_RECTANGLE_OUT_OF_WS             Il rettangolo andrebbe fuori dai limiti della WS
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
            
    	if (WindowData [WndHandle].WindowIsActive)
        {
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
                       
                       
           if ( (KDiffX<WindowData [WndHandle].WSWndLength) && (KDiffY<WindowData [WndHandle].WSWndHeight) )
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
                             ndWS_PutPixel_NORENDER_FAST (RRPosX1+IntX, RRPosY1+IntY, BorderColor, WndHandle);
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
                           ndWS_PutPixel_NORENDER_FAST (RRPosX1+PosOffset+IntX, RRPosY1+IntY, BorderColor, WndHandle);
                         
                         for (IntX=0; IntX<NrPixelsChoose; IntX++)
                           ndWS_PutPixel_NORENDER_FAST (RRPosX2-PosOffset-IntX, RRPosY1+IntY, BorderColor, WndHandle);
                         
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
                                ndWS_PutPixel_NORENDER_FAST (IntX, RRPosY1+IntY, Color, WndHandle);
                         }
                         
                    }
                }
                    
                    
                // Ora, eseguiamo il rendering della finestra in modo che possa apparire il
                // rettangolo
    
                if (RenderNow) XWindowRender (WndHandle);
                return 0;
           }
           else
             return ERR_RECTANGLE_OUT_OF_WS;
    }
    else
      return ERR_WINDOW_IS_NOT_ALLOCATED;
                    
}


char ndWS_DrawRectangle (unsigned short int RRPosX1, unsigned short int RRPosY1, 
                         unsigned short int RRPosX2, unsigned short int RRPosY2, 
                         TypeColor Color, TypeColor BorderColor, 
                         unsigned char WndHandle, unsigned char RenderNow)
/*
   Funzione analoga a SpDrawRectangle, ma disegna un semplice rettangolo pieno
   con bordo di spessore 1.
   
   Sono previsti i seguenti codici di errore:
      ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
      ERR_RECTANGLE_OUT_OF_WS             Il rettangolo andrebbe fuori dai limiti della WS 
*/

{
   return  ndWS_DrawSpRectangle (RRPosX1, RRPosY1, RRPosX2, RRPosY2, Color, BorderColor, 0, WndHandle, RenderNow);
}


char ndWS_DrawRtRectangle (int CenterRectX, int CenterRectY, int UserDimX, int UserDimY,
                            TypeColor Color, TypeColor BorderColor, ndint64 CodeStyle,
                            unsigned char WndHandle, unsigned char RenderNow)
/*
     Questa funzione scrive un rettangolo nel Window Space della finestra identificata da WndHandle,
     ma, a differenza delle precedenti, è in grado di ruotare il rettangolo intorno al proprio
     centro di un certo numero di gradi. 
     
     L'utente non fornisce le coordinate dei vertici, come nel caso delle precedenti routine
     DrawRectangle, ma indica semplicemente le coordinate del centro del rettangolo e le
     dimensioni. 
     
     Si faccia attenzione al fatto che DrawRtRectangle esegue un controllo per assicurarsi
     che le dimensioni non siano maggiori delle dimensioni del WindowSpace e che siano
     positive, ma non esegue alcun controllo per assicurarsi che tutto il rettangolo, dopo
     la rotazione, sia ancora contenuto nel window space della finestra. Se una parte del
     rettangolo andasse fuori dalle coordinate del window space, essa sarà semplicemente
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
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
          ERR_RECTANGLE_OUT_OF_WS             Le dimensioni del rettangolo sono troppo grandi
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
            
	if (WindowData [WndHandle].WindowIsActive)
    {
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
           if ( (UserDimX<WindowData [WndHandle].WSWndLength) && (UserDimY<WindowData [WndHandle].WSWndHeight) )
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
                             
                             ndWS_PutPixel (TargetX, TargetY, BorderColor, WndHandle, NORENDER);
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
                                 
                           ndWS_PutPixel (TargetX, TargetY, BorderColor, WndHandle, NORENDER);
                         }
                         
                         for (FlCounterX=0; FlCounterX<NrPixelsChoose; FlCounterX=FlCounterX+0.5)
                         {
                           UpCornerPixelX = UserDimX-PosOffset-FlCounterX;
                           UpCornerPixelY = FlCounterY;
                           
                           CenterPixelX = UpCornerPixelX-HalfDimX;     // Coordinate rispetto al centro del rettangolo
                           CenterPixelY = UpCornerPixelY-HalfDimY;
                             
                           INTERNAL_Rotate (CenterRectX, CenterRectY, CenterPixelX, CenterPixelY, RotateRadiant, &TargetX, &TargetY);
                             
                           ndWS_PutPixel (TargetX, TargetY, BorderColor, WndHandle, NORENDER);
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
                           
                                ndWS_PutPixel (TargetX, TargetY, Color, WndHandle, NORENDER);
                            }
                         
                         }
                                          
                    }
                }
                        
                // Ora, eseguiamo il rendering della finestra in modo che possa apparire il
                // rettangolo
    
                if (RenderNow) XWindowRender (WndHandle);
                return 0;
           }
           else
             return ERR_RECTANGLE_OUT_OF_WS;
        }
        else
          return ERR_WRONG_PARAMETERS;
    }
    else
        return ERR_WINDOW_IS_NOT_ALLOCATED;
                
}





/*
  -----------------------------------------------------------------------------------------------
  ROUTINE WS CHE SI OCCUPANO DELLA CREAZIONE DI ELEMENTI GRAFICI SULLO SCHERMO: CERCHI ED ELLISSI
  -----------------------------------------------------------------------------------------------
*/






char INTERNAL_ndWS_EllipseEngine (unsigned short int CenterX, unsigned short int CenterY, 
                                  float RayA, float RayB, float RayX, float DegreeA, float DegreeB, float Rot,
                                  TypeColor Colore, char DrawRay, char WndHandle, char RenderNow)
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
       ErrRep=ndWS_PutPixel (X, Y, Colore, WndHandle, NORENDER);  
    
       if (!ErrRep)
       {
          PrevX = X;
          PrevY = Y;
          First = 0;
       }
    }
    else
    {
       ndWS_DrawLine (X, Y, PrevX, PrevY, Colore, WndHandle, NORENDER);  
  
       PrevX = X;
       PrevY = Y;
    }
    
    if (RayX!=0)                   // E' stata richiesta la produzione di uno spessore
    {
        if ( (RayX<RayA) && (RayX<RayB) )
        {
            INTERNAL_Rotate (CenterX, CenterY, RayA*ndHAL_XFPU_cosf (t), - RayB*ndHAL_XFPU_sinf (t), Rot_Radiant, &X1, &Y1);
            INTERNAL_Rotate (CenterX, CenterY, (RayA-RayX)*ndHAL_XFPU_cosf (t), - (RayB-RayX)*ndHAL_XFPU_sinf (t), Rot_Radiant, &X2, &Y2);
        
            ndWS_DrawLine (X1, Y1, X2, Y2, Colore, WndHandle, NORENDER);
            ndWS_DrawLine (X1-1, Y1, X2-1, Y2, Colore, WndHandle, NORENDER);
            ndWS_DrawLine (X1+1, Y1, X2+1, Y2, Colore, WndHandle, NORENDER);
            ndWS_DrawLine (X1, Y1-1, X2, Y2-1, Colore, WndHandle, NORENDER);
            ndWS_DrawLine (X1, Y1+1, X2, Y2+1, Colore, WndHandle, NORENDER);
             
        }
    }
  }
  
  if ((DrawRay) && (RayX<RayA) && (RayX<RayB) && (fabs (DegreeA-DegreeB)<360))
  {                   
       INTERNAL_Rotate (CenterX, CenterY, (RayA-RayX)*ndHAL_XFPU_cosf (DegreeA_Radiant), - (RayB-RayX)*ndHAL_XFPU_sinf (DegreeA_Radiant), Rot_Radiant, &X, &Y);     
       ndWS_DrawLine (CenterX, CenterY, X, Y, Colore, WndHandle, NORENDER);
       
       INTERNAL_Rotate (CenterX, CenterY, (RayA-RayX)*ndHAL_XFPU_cosf (DegreeB_Radiant), - (RayB-RayX)*ndHAL_XFPU_sinf (DegreeB_Radiant), Rot_Radiant, &X, &Y);       
       ndWS_DrawLine (CenterX, CenterY, X, Y, Colore, WndHandle, NORENDER);
  }
             
  if (RenderNow) XWindowRender (WndHandle);
   
  return 0;
}

char ndWS_DrawArcOfEllipse (unsigned short int CenterX, unsigned short int CenterY, 
                            float RayA, float RayB, float DegreeA, float DegreeB, 
                            TypeColor Color, TypeColor ColorBorder, 
                            ndint64 CodeStyle, char WndHandle, char RenderNow)
/*
     Questa routine permette di disegnare un arco di ellisse dentro la finestra
     caratterizzata da un certo WndHandle. 
     
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
                      
   Sono previsti i seguenti codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata       
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
          
   if (WindowData [WndHandle].WindowIsActive)
   {
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
          ErrRep = INTERNAL_ndWS_EllipseEngine (CenterX, CenterY, RayA, RayB, BorderCode, DegreeA, DegreeB, RotateDegree, ColorBorder, 0, WndHandle, NORENDER);
          if (ErrRep) return ErrRep;
          
          if (!Void) ErrRep = ndWS_FillArea (CenterX, CenterY, Color, ColorBorder, WndHandle, NORENDER);
          if (ErrRep) return ErrRep;  
      }
      else    // Ha chiesto di visualizzare un arco di circonferenza
      {   
          ErrRep = INTERNAL_ndWS_EllipseEngine (CenterX, CenterY, RayA, RayB, BorderCode, DegreeA, DegreeB, RotateDegree, ColorBorder, 1, WndHandle, NORENDER);
          if (ErrRep) return ErrRep; 
          
          // Qui è necessario calcolare il punto di filler
          
          if ((!Void) && (RayA-BorderCode>3) && (RayB-BorderCode>3))
          {
              InterDegree = (DegreeA + DegreeB)/2;
              InterRadiant = InterDegree*6.28/360.0;
              
              FillPoint_X =   (RayA-BorderCode-2)*ndHAL_XFPU_cosf (InterRadiant);
              FillPoint_Y = - (RayB-BorderCode-2)*ndHAL_XFPU_sinf (InterRadiant);
       
              INTERNAL_Rotate (CenterX, CenterY, FillPoint_X, FillPoint_Y, RotateRadiant, &FillPoint__X, &FillPoint__Y);     
       
              ErrRep = ndWS_FillArea (FillPoint__X, FillPoint__Y, Color, ColorBorder, WndHandle, NORENDER);
              if (ErrRep) return ErrRep; 
          }
      } 
      
      if (RenderNow) XWindowRender (WndHandle);
      return 0;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndWS_DrawEllipse      (unsigned short int CenterX, unsigned short int CenterY, 
                            float RayA, float RayB, TypeColor Color, TypeColor ColorBorder, 
                            ndint64 CodeStyle, char WndHandle, char RenderNow)
/*
     Questa routine permette di disegnare un'ellisse dentro la finestra
     caratterizzata da un certo WndHandle. 
     
     Il significato del parametro StyleCode è lo stesso usato nelle altre routine
     di disegno.
                      
     Sono previsti i seguenti codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata       
*/

{
    return ndWS_DrawArcOfEllipse (CenterX, CenterY, RayA, RayB, 0, 360, 
           Color, ColorBorder, CodeStyle, WndHandle, RenderNow);
}


char ndWS_DrawArcOfCircle  (unsigned short int CenterX, unsigned short int CenterY, 
                            float Ray, float DegreeA, float DegreeB,
                            TypeColor Color, TypeColor ColorBorder, 
                            ndint64 CodeStyle, char WndHandle, char RenderNow)
/*
     Questa routine permette di disegnare un arco di cerchio dentro la finestra
     caratterizzata da un certo WndHandle. 
     
     Il significato del parametro StyleCode è lo stesso usato nelle altre routine
     di disegno.
                      
     Sono previsti i seguenti codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata       
*/

{
    return ndWS_DrawArcOfEllipse (CenterX, CenterY, Ray, Ray, DegreeA, DegreeB, 
           Color, ColorBorder, CodeStyle, WndHandle, RenderNow);
}



char ndWS_DrawCircle       (unsigned short int CenterX, unsigned short int CenterY, 
                            float Ray, TypeColor Color, TypeColor ColorBorder, 
                            ndint64 CodeStyle, char WndHandle, char RenderNow)
/*
     Questa routine permette di disegnare un cerchio o una circonferenza dentro 
     la finestra caratterizzata da un certo WndHandle. 
     
     Il significato del parametro StyleCode è lo stesso usato nelle altre routine
     di disegno.
                      
     Sono previsti i seguenti codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata       
*/

{
    return ndWS_DrawArcOfEllipse (CenterX, CenterY, Ray, Ray, 0, 360, 
           Color, ColorBorder, CodeStyle, WndHandle, RenderNow);
}







/* 
   -----------------------------------------------------------------------------
   ROUTINES PER LA CANCELLAZIONE DEL WINDOW SPACE
   -----------------------------------------------------------------------------
*/



char ndWS_ClrBufferSpace (unsigned char WndHandle)
/*
     Questa funzione pulisce il buffer WS della finestra identificata da WndHandle.
     Al termine dell'operazione viene eseguita una operazione di X-Render per mostrare il risultato.

     La funzione restituisce il codice di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
*/

{
     if ( !ndInitSystemExecuted ) ndInitSystem();
          
     unsigned short int CounterX, CounterY;

     if (WindowData [WndHandle].WindowIsActive)
     {
        for (CounterY=0; CounterY<(WindowData [WndHandle].WSWndHeight); CounterY++)
        {
            for (CounterX=0; CounterX<(WindowData [WndHandle].WSWndLength); CounterX++)
            {
                ndWS_PutPixel_NORENDER_FAST (CounterX, CounterY, WindowData [WndHandle].ColorBGWS, WndHandle);
            }
        }

        XWindowRender (WndHandle);
        return 0;
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;
}

char ndWS_ClrALL (unsigned char WndHandle)
/*
   Questa funzione pulisce il buffer WS della finestra identificata da WndHandle e nel
   contempo cancella completamente la memoria caratteri, riportando il cursore in
   alto a sinistra. Il risultato è una finestra completamente vuota.

   E' previsto in uscita un codice di errore
           ERR_WINDOWS_IS_NOT_ALLOCATED         La finestra non è allocata
*/

{
       if ( !ndInitSystemExecuted ) ndInitSystem();
           
       if (WindowData [WndHandle].WindowIsActive)
       {
          unsigned short int CounterX, CounterY;

          for (CounterY=0; CounterY<(WindowData [WndHandle].WS_MaxCursY); CounterY++)
          {
              for (CounterX=0; CounterX<(WindowData [WndHandle].WS_MaxCursX); CounterX++)
              {
                  WINDOWDATA_CHARMAP (WndHandle, CounterX, CounterY)=0;
                  WINDOWDATA_COLORCHARMAP (WndHandle, CounterX, CounterY)=0;
              }
          }

          ndWS_GoCursor (0, 0, WndHandle);

          for (CounterY=0; CounterY<(WindowData [WndHandle].WSWndHeight); CounterY++)
          {
              for (CounterX=0; CounterX<(WindowData [WndHandle].WSWndLength); CounterX++)
              {
                  ndWS_PutPixel_NORENDER_FAST (CounterX, CounterY, WindowData [WndHandle].ColorBGWS, WndHandle);
              }
          }

          XWindowRender (WndHandle);
          return 0;

       }
       else
         return ERR_WINDOW_IS_NOT_ALLOCATED;
}

char ndWS_ChangeWndColor (TypeColor NewColor, unsigned short int WndHandle)
/* Questa funzione ha il compito di cambiare il colore del window space di una finestra.

   Il cambiamento di colore del window space ha sempre, come conseguenza, che lo stesso
   window space sia cancellato.

   La funzione riporta in uscita il seguente codice di errore:
           ERR_WINDOW_IS_NOT_ALLOCATED         La finestra non è allocata
*/

{      
       unsigned int CounterX, CounterY;

       if ( !ndInitSystemExecuted ) ndInitSystem();

       if (WindowData [WndHandle].WindowIsActive)
       {
          WindowData [WndHandle].ColorBGWS=NewColor;

          for (CounterY=13; CounterY<=(WindowData [WndHandle].WndHeight - 1); CounterY++)
          {
              for (CounterX=1; CounterX<=(WindowData [WndHandle].WndLength - 1); CounterX++)
              {
                  ndLP_PutPixel_NORENDER_FAST (CounterX, CounterY, NewColor, WndHandle);
              }
          }

          /* E' necessario eseguire la copia sia a livello LP che a livello WS. Infatti,
             se fosse eseguita la copia solo a livello WS, gli spazi a margine del WS
             rimarrebbero del colore originale. Il ciclo precedente tiene anche conto
             dei punti di margine della finestra */

          for (CounterY=0; CounterY<(WindowData [WndHandle].WSWndHeight); CounterY++)
          {
              for (CounterX=0; CounterX<(WindowData [WndHandle].WSWndLength); CounterX++)
              {
                  ndWS_PutPixel_NORENDER_FAST (CounterX, CounterY, NewColor, WndHandle);
              }
          }

          XWindowRender (WndHandle);
          return 0;
       }
       else
         return ERR_WINDOW_IS_NOT_ALLOCATED;
}



char ndLP_ChangeWndColor (TypeColor NewColor, unsigned short int WndHandle)
/* Richiama semplicemente la relativa funzione WS. E' usata solo per facilitare il lavoro
   agli sviluppatori, in quanto una ChangeWndColor può essere considerata anche una
   routine di tipo Logical Page, oltre che di tipo Window Space.
*/
{
    char Error;

    Error = (ndWS_ChangeWndColor (NewColor, WndHandle));
    return Error;
}

/* 
   -----------------------------------------------------------------------------
   ROUTINES DI SCRITTURA SPECIALI
   -----------------------------------------------------------------------------
*/

char ndWS_GrWriteChar (unsigned short RRPosPixelX, unsigned short RRPosPixelY, unsigned char Carattere, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, ndint64 TextCode)
/*
     Questa funzione permette di scrivere un carattere nel window space della finestra indicata da WndHandle.
     Il carattere viene scritto come se si trattasse di un semplice elemento grafico: la funzione non
     modifica la mappa caratteri della finestra. Le coordinate RPosPixelX e RPosPixelY rappresentano
     le coordinate del pixel del carattere più in alto a sinistra. 
     
     Le coordinate devono essere espresse rispetto al punto più in alto ed a sinistra del window space.
      
     Questa funzione è stata progettata per la scrittura di caratteri singoli. 
     
     Le caratteristiche del carattere vengono passate per mezzo del parametro a 64 bit TextCode.
     
     I 32 bit più significativi non hanno attualmente significato, essendo riservati ad usi
     futuri. Per quanto riguarda i 32 bit meno significativi, essi sono mappati nel seguente
     modo:
          
     31-23   Indicano l'handle del font da utilizzare. L'utente può ottenere la chiave necessaria
             attraverso la funzione NDKEY_FONT (FontHandle). Se si omette questa chiave, ovvero se
             si stabilisce un handle pari a 0, il sistema imposterà di default il font che è 
             attualmente settato di default nel window space della finestra WndHandle (ossia,
             l'ultimo font impostato con ndWS_SetFont. 
             
     1       Se si imposta questo bit a 1, il sistema visualizzerà il carattere in modalità 
             trasparent. Quando viene attivata questa modalità, il parametro BGColor viene
             ignorato.
             
     0       Se si imposta questo bit a 1, il sistema richiamerà XWindowRender dopo la visualizzazione
             del carattere. Usare la costante manifesta RENDER
     
     Sono previsti i seguenti codici di errore:

     ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
     ERR_FONTHDL_IS_NOT_ACTIVE          L'handle del font non è attivo
*/
{
     unsigned char *FontData;		//puntatore al font
    
     unsigned char BitFilter, BitExtract;
     unsigned char ByteInChar;
     unsigned char FntHandle;
     int IntX, IntY;
    
     char RenderNow, Trasp;
     ndint64 Temp;
    
     static char ErrRep;
     
     int             NewTTFSize, TTFAngle;
     unsigned char   XStr [2];
    
     // Inizio del codice     

     if ( !ndInitSystemExecuted ) ndInitSystem();
           
     if ((WindowData [WndHandle].WindowIsActive))
     {
        
        Temp = TextCode;
        Temp = Temp >> (31-8);   
           
        FntHandle = (unsigned char) Temp;
        
        if (FntHandle==0) FntHandle=WindowData [WndHandle].ndWS_CurrentFntHandle;
        
        // Parametri NoRender e Trasp
        
        if ((TextCode & RENDER) == RENDER) RenderNow=1;
           else RenderNow=0;
           
        if ((TextCode & TRASP) == TRASP) Trasp=1;
           else Trasp=0;
     
        // Inizio dell'analisi
        
        if (FontHandlesArray [FntHandle].IsActive)
        {  
            if (!FontHandlesArray [FntHandle].IsTrueType)
            {                 
                             
                        if (Carattere!=0)
                        {
                           FontData=(FontHandlesArray [FntHandle].Data)+Carattere*8;	//Inizializza il puntatore alla corretta lettera nel buffer dei font
                    
                    	   ByteInChar=0;
                    
                    	   for (IntY=0; IntY<8; IntY++)
                    	   {
                    	   	   BitFilter=0x80;
                    
                    		   for (IntX=0; IntX<8; IntX++)
                    		   {
                    			   BitExtract=(*(FontData+ByteInChar)&BitFilter);
                    
                    			   if (BitExtract!=0)
                    			   {
                                       ErrRep=ndWS_PutPixel (RRPosPixelX+IntX, RRPosPixelY+IntY, Color, WndHandle, NORENDER);
                                   }
                                   else
                                   {
                                       if (!Trasp) 
                                         ErrRep=ndWS_PutPixel (RRPosPixelX+IntX, RRPosPixelY+IntY, BGColor, WndHandle, NORENDER);
                                       else
                                         ErrRep=0;  
                                   }
                                   
                                   if (ErrRep!=0) return ErrRep;
                    
                    			   BitFilter=BitFilter>>1;		//La maschera BitFilter è stata spostata di un bit a dx
                    		    }
                    
                    		    ByteInChar++;
                    
                           }
                    
                           if (RenderNow) XWindowRender (WndHandle);           
                        }
                        
                        return 0;
                        
            }
            else   // Font True Type
            {
                   #ifdef ND_ENABLE_TTF_SUPPORT 
                   
                        TTFAngle   = (unsigned short) ( (TextCode >> 63-9) & 0x00FF );      // Isolato il codice di rotazione (abs)          
                        if ( ( TextCode >> 63-9  ) & 0x0100 ) TTFAngle=-TTFAngle;
                        
                        NewTTFSize = (unsigned char)  ( (TextCode >> 63-9-8) & 0x00FF );    // Isolato il codice di ampiezza TTF
                        
                        // Inserisci il carattere nel buffer
                        XStr [0]=Carattere;
                        XStr [1]=0;
                        
                        // Esegui il renderer
                        ndFLIB_WS_TTFEngine0 (RRPosPixelX, RRPosPixelY, &XStr, Color, BGColor, FntHandle, NewTTFSize, TTFAngle, 
                                              WndHandle, WindowData [WndHandle].WSWndLength, WindowData [WndHandle].WSWndHeight, Trasp);

                        if (RenderNow) XWindowRender (WndHandle);          // Al termine del processo di copia il rendering fa apparire la stringa
                        return 0;
                  
                   #else
                   
                        return ERR_GRAPHFONT_NOT_SUPPORTED;
                        
                   #endif
            }
                        
        }
        else
          return ERR_FONTHDL_IS_NOT_ACTIVE;  
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;  
}


char ndWS_GrWriteLn (unsigned short RRPosPixelX, unsigned short RRPosPixelY, char *str, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, ndint64 TextCode)
/*
   Questa funzione serve per scrivere una stringa nel Window Space di una finestra identificata da
   WndHandle. La stringa viene scritta come un semplice elemento grafico: la mappa caratteri della
   finestra NON viene modificata.
   
   La stringa comparirà nel windowspace a partire dal pixel determinato dalle coordinate RPosPixelX
   e RPosPixelY. Tali coordinate devono essere fornite rispetto al punto in alto a sinistra del
   WindowSpace. 
  
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
        ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
        ERR_FONTHDL_IS_NOT_ACTIVE          L'handle del font non è attivo       
*/

{
    int InternalPosX, InternalPosY;
      
    char *strpointer;			//Puntatore alla stringa  
    int MaxLenStr=255;
	int t=0;
	
	ndint64       Temp, NewCode, Mask;
	char          RenderNow, Trasp;
	unsigned char FntHandle;
	
	int           NewTTFSize, TTFAngle;
	static char   ErrRep;
	
	// Inizio del codice

    if ( !ndInitSystemExecuted ) ndInitSystem();

	if (WindowData [WndHandle].WindowIsActive)
	{
        Temp = TextCode;
        Temp = Temp >> (31-8);   
           
        FntHandle = (unsigned char) Temp;
        
        if (FntHandle==0) FntHandle=WindowData [WndHandle].ndWS_CurrentFntHandle;
        
        // Parametri NoRender e Trasp
        
        if ((TextCode & RENDER) == RENDER) RenderNow=1;
           else RenderNow=0;
           
        if ((TextCode & TRASP) == TRASP) Trasp=1;
           else Trasp=0;
     
        // Inizio del codice
        
        if (FontHandlesArray [FntHandle].IsActive)
        { 
            if (!FontHandlesArray [FntHandle].IsTrueType)
            {               
                            strpointer=str;
                        	
                        	if ((TextCode & RENDER) == RENDER) RenderNow=1;
                               else RenderNow=0;
                               
                            Mask = 0xFFFFFFFFFFFFFFFE;
                            NewCode = TextCode & Mask;   // Cancella il bit meno significativo: serve a fare il NORENDER sui caratteri
                            
                    	    while (*strpointer!=0 && t<MaxLenStr)
                    	    {
                                  InternalPosX=RRPosPixelX + t*8;
                                  InternalPosY=RRPosPixelY;
                                  
                    		      ErrRep=ndWS_GrWriteChar (InternalPosX, InternalPosY, *strpointer, Color, BGColor, WndHandle, NewCode);
                    		      // GrWriteChar erediterà attraverso NewCode tutte le caratteristiche (font, eventuale attributi di
                    		      // trasparenza) passate alla routine salvo l'attributo di rendering che sarà sempre fissato a
                    		      // NORENDER
                    		      
                    		      if (ErrRep!=0) return ErrRep;
                                  
                                  strpointer++;
                                  t++;
                            }
                    
                            
                            if (RenderNow) XWindowRender (WndHandle);          // Al termine del processo di copia il rendering fa apparire la stringa
                            return 0;
            }
            else   // Font TrueType
            {
                   #ifdef ND_ENABLE_TTF_SUPPORT 
                   
                        TTFAngle   = (unsigned short) ( (TextCode >> 63-9) & 0x00FF );      // Isolato il codice di rotazione (abs)          
                        if ( ( TextCode >> 63-9  ) & 0x0100 ) TTFAngle=-TTFAngle;
                        
                        NewTTFSize = (unsigned char)  ( (TextCode >> 63-9-8) & 0x00FF );    // Isolato il codice di ampiezza TTF
                        
                        // Esegui il renderer
                        ndFLIB_WS_TTFEngine0 (RRPosPixelX, RRPosPixelY, str, Color, BGColor, FntHandle, NewTTFSize, TTFAngle, 
                                              WndHandle, WindowData [WndHandle].WSWndLength, WindowData [WndHandle].WSWndHeight, Trasp);

                        if (RenderNow) XWindowRender (WndHandle);          // Al termine del processo di copia il rendering fa apparire la stringa
                        return 0;
                  
                   #else
                   
                        return ERR_GRAPHFONT_NOT_SUPPORTED;
                        
                   #endif
            }
        }
        else
           return ERR_FONTHDL_IS_NOT_ACTIVE;
     }
     else
        return ERR_WINDOW_IS_NOT_ALLOCATED;

}


char ndWS_GrPrintLn (unsigned short RRPosPixelX, unsigned short int RRPosPixelY, 
                    TypeColor Color, TypeColor BGColor, 
                    unsigned char WndHandle, ndint64 TextCode, 
                    char *FirstParam, ...)
/*
    Questa routine si occupa di visualizzare una stringa complessa nella finestra
    WndHandle. A differenza di ndWS_PrintLn e ndWS_Print, queste routine scrivono
    la stringa dentro al window space, come se fosse un qualsiasi elemento grafico.
    
    Il funzionamento è analogo a quello previsto per la printf della libc, salvo
    per il fatto che l'utente non deve inserire un \n alla fine per andare a
    capo.
    
    La stringa comparirà nel windowspace a partire dal pixel determinato dalle 
    coordinate RPosPixelX e RPosPixelY. Tali coordinate devono essere fornite 
    rispetto al punto in alto a sinistra del WindowSpace. 
    
    Sono previsti tutti i codici di errore previsti per ndWS_GrWriteLn, ed 
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
         return ndWS_GrWriteLn (RRPosPixelX, RRPosPixelY, &IntBufPrintF, Color, BGColor, WndHandle, TextCode);
      } 
      else
        return ERR_INTERNAL_ERROR;
        
}


char ndWS_SetFont (unsigned char FntHandle, char WndHandle)
/*
   Questa routine provvede a settare il font per le routines di tipo WS relative
   alla finestra che viene identificata da WndHandle.
   
   Le routines di tipo WS supportano esclusivamente i font di tipo TEXT e non
   i font di tipo GRAPHICS. Questa limitazione sarà rimossa nella prossima
   versione di Nanodesktop. 
   
   Sono previsti i seguenti codici di errore:
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
        ERR_FONTHDL_IS_NOT_ACTIVE     Il font handle non è attivo
        ERR_GRAPHFONT_NOT_SUPPORTED   I graphfont non sono supportati
*/
{
   if ( !ndInitSystemExecuted ) ndInitSystem();
           
   if ((WindowData [WndHandle].WindowIsActive))
   {  
       if (FontHandlesArray [FntHandle].IsActive)
       {                   
          if ( (FontHandlesArray [FntHandle].features & NDTEXTFONT) == NDTEXTFONT )
          {
             WindowData [WndHandle].ndWS_CurrentFntHandle = FntHandle;
             // Registra il nuovo text-font corrente
             
             XWindowRender (WndHandle);
             
             return 0;
             // e ritorna il codice di controllo 0
          }
          else return ERR_GRAPHFONT_NOT_SUPPORTED;
       }
       else
           return ERR_FONTHDL_IS_NOT_ACTIVE;   
   }
   else
     return ERR_WINDOW_IS_NOT_ALLOCATED;
}





