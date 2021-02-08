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
    ROUTINES PER LA GESTIONE DELLE TEXTBOX/TEXTAREA
    ----------------------------------------------------------------------------
    Queste routine si occupano della gestione delle textbox e delle textarea.
    Sono caratterizzate dal fatto che il loro nome inizia sempre per ndTBOX
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

// -------------------------------- Simboli interni ----------------------------

#define STDBUFFER       0
#define RESERVE         1

#define TXAREA_DISABLED   0     // Attenzione: questi sono solo i codici interni per il renderer
#define TXAREA_ENABLED    1
#define TXAREA_INHIBITED -1

#define CSR_DISABLED      0
#define CSR_ENABLED       1

#define FROM_START        1


/* *****************************************************************************
 *******************************************************************************
 **************************** Renderer interno *********************************
 *******************************************************************************
 ******************************************************************************* */



inline char *INTERNAL_GetCharAddr (int PosX, int PosY, int TextBoxHandle, char WndHandle, char Reserve)
/*
   Questa funzione restituisce l'indirizzo di un carattere della TextArea o del buffer di
   riserva. La funzione è ottimizzata per la velocità e non esegue controlli 
*/
{
   char *PointerToChar;   
      
   if (!Reserve)
   {
        PointerToChar = WindowData [WndHandle].TextBox [TextBoxHandle].TextData + 
                   (WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsX+1) * PosY + PosX;
   }
   else
   {
        PointerToChar = WindowData [WndHandle].TextBox [TextBoxHandle].ReserveBuffer + 
                   (WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsX+1) * PosY + PosX;
   }
                 
   return PointerToChar;         
 
}
   

inline char *INTERNAL_GetRowAddr (int PosY, int TextBoxHandle, char WndHandle, char Reserve)
/*
   Questa funzione restituisce l'indirizzo del primo carattere di una riga della TextArea 
   o del buffer di riserva. La funzione è ottimizzata per la velocità e non esegue controlli  
*/
{
   char *PointerToChar;   
      
   if (!Reserve)
   {
        PointerToChar = WindowData [WndHandle].TextBox [TextBoxHandle].TextData + 
                   (WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsX+1) * PosY;
   }
   else
   {
        PointerToChar = WindowData [WndHandle].TextBox [TextBoxHandle].ReserveBuffer + 
                   (WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsX+1) * PosY;
   }
                 
   return PointerToChar;         

}
  

//    ----------------------- Renderer della TextArea ----------------------------------------

   
void INTERNAL_TextAreaRenderer (char State, char CursorOn, int TextBoxHandle, char WndHandle, char Reserve, char ToBegin, char RenderNow)
/*
   Questa routine provvede ad eseguire il rendering della TextBox. 
*/
{
   int CounterX, CounterY;
   int PosX, PosY, CharX, CharY;
   
   char Red, Green, Blue;
   
   char *PointerToChar;
   
   int MetaPosCursX, MetaPosCursY;
   int FirstCharX, FirstCharY;
   
   TypeColor _Color, _BgColor, _BgChColor;
   
   // Stabiliamo i colori
   
   switch (State)
   {
          case TXAREA_DISABLED:
          {
               _Color     = WindowData [WndHandle].TextBox [TextBoxHandle].TextColorOff;
               _BgColor   = WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorOff;  
               _BgChColor = WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorOff;   
               
               break;
          }
          
          case TXAREA_ENABLED:
          {
               _Color     = WindowData [WndHandle].TextBox [TextBoxHandle].TextColorOn;
               _BgColor   = WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorOn;  
       
               ndHAL_MagicNumberToRGB (WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorOn, &Red, &Green, &Blue);
       
               if ( (Red-16)>=0 )            Red = Red - 16;
               if ( (Green-16)>=0 )          Green = Green - 16;
               if ( (Blue-16)>=0 )           Blue = Blue - 16;
       
               _BgChColor = ndHAL_RGBToMagicNumber (Red, Green, Blue);  
               
               break;
          }
          
          case TXAREA_INHIBITED:
          {
               _Color     = WindowData [WndHandle].TextBox [TextBoxHandle].TextColorInh;
               _BgColor   = WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorInh;  
               _BgChColor = WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorInh;   
               
               break;
          }
   }
   
   // Stabiliamo l'inizio della visualizzazione
   
   if (!ToBegin)
   {
       FirstCharX = WindowData [WndHandle].TextBox [TextBoxHandle].FirstCharX;
       FirstCharY = WindowData [WndHandle].TextBox [TextBoxHandle].FirstCharY;         
   }
   else
   {
       FirstCharX = 0;
       FirstCharY = 0;
   }
   
   // Cancella tutta la vecchia area
   
   ndWS_DrawRectangle (WindowData [WndHandle].TextBox [TextBoxHandle].PosX1, WindowData [WndHandle].TextBox [TextBoxHandle].PosY1,
                       WindowData [WndHandle].TextBox [TextBoxHandle].PosX2, WindowData [WndHandle].TextBox [TextBoxHandle].PosY2,
                       _BgColor, _Color, WndHandle, NORENDER);
   
   // Visualizza i caratteri
   
   for (CounterY=0; CounterY<WindowData [WndHandle].TextBox [TextBoxHandle].ShownCharsY; CounterY++)
   {
      for (CounterX=0; CounterX<WindowData [WndHandle].TextBox [TextBoxHandle].ShownCharsX; CounterX++)
      {
              PosX = 2 + WindowData [WndHandle].TextBox [TextBoxHandle].PosX1 + 
                         WindowData [WndHandle].TextBox [TextBoxHandle].MargineX +
                             WindowData [WndHandle].TextBox [TextBoxHandle].sDimCharX * CounterX;
                             
              PosY = 2 + WindowData [WndHandle].TextBox [TextBoxHandle].PosY1 + 
                         WindowData [WndHandle].TextBox [TextBoxHandle].MargineY +
                             WindowData [WndHandle].TextBox [TextBoxHandle].sDimCharY * CounterY;
              
              CharX = FirstCharX + CounterX;
              CharY = FirstCharY + CounterY;
              
              PointerToChar = INTERNAL_GetCharAddr (CharX, CharY, TextBoxHandle, WndHandle, Reserve);
              
              ndWS_GrWriteChar (PosX, PosY, *PointerToChar, _Color, _BgChColor, WndHandle,  
                                    NDKEY_FONT (WindowData [WndHandle].TextBox [TextBoxHandle].FntHandle) | NORENDER );
              
                          
      } 
   }
   
   // Provvedi eventualmente a disegnare il cursore
   
   if (CursorOn)
   {
      MetaPosCursX = WindowData [WndHandle].TextBox [TextBoxHandle].PosCursX - FirstCharX;
      MetaPosCursY = WindowData [WndHandle].TextBox [TextBoxHandle].PosCursY - FirstCharY;
      
      PosX = 2 + WindowData [WndHandle].TextBox [TextBoxHandle].PosX1 + 
                         WindowData [WndHandle].TextBox [TextBoxHandle].MargineX +
                             WindowData [WndHandle].TextBox [TextBoxHandle].sDimCharX * MetaPosCursX;
                             
      PosY = 2 + WindowData [WndHandle].TextBox [TextBoxHandle].PosY1 + 
                         WindowData [WndHandle].TextBox [TextBoxHandle].MargineY +
                             WindowData [WndHandle].TextBox [TextBoxHandle].sDimCharY * MetaPosCursY;
                             
      ndWS_DrawLine   (PosX, 
                       PosY + WindowData [WndHandle].TextBox [TextBoxHandle].DimCharY, 
                       PosX + WindowData [WndHandle].TextBox [TextBoxHandle].DimCharX - 1,
                       PosY + WindowData [WndHandle].TextBox [TextBoxHandle].DimCharY, _Color, WndHandle, NORENDER);
                        
   }
   
   if (RenderNow) XWindowRender (WndHandle);
}




//    ----------------------- Enable/Disable TextArea ---------------------------------



char INTERNAL_EnableTestArea (int TextBoxHandle, char TextBoxWndHandle)
/*
    Questa routine è utilizzata per abilitare la TestArea. Restituisce 0 in caso di
    successo e -1 in caso di errore 
*/
{
    int MemorySize;
    
    // Creiamo il buffer di riserva
           
    MemorySize = (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX+1)*(WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY+1); 
    WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ReserveBuffer = malloc  (MemorySize);
           
    // Abbiamo allocato con successo il buffer di riserva
           
    if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ReserveBuffer)
    {
         // Provvediamo a salvare una copia del testo attuale nel buffer di riserva
                   
         memcpy (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ReserveBuffer, 
                    WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].TextData, MemorySize);
                                  
         // e provvediamo a salvare la vecchia posizione del cursore
                   
         WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ResPosCursX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX;
         WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ResPosCursY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY;
                   
         // Esegui una render della textarea: cursore attivato, area attivata
                   
         INTERNAL_TextAreaRenderer (TXAREA_ENABLED, CSR_ENABLED, TextBoxHandle, TextBoxWndHandle, RESERVE, 0, RENDER);
                   
         // Setta a 1 TextAreaIsOn per informare le altre routines che la TextArea è attiva
                  
         WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].TextAreaIsOn = 1;
         
         return 0;    // Segnala che non ci sono errori
    }
    else    // Processo di allocazione del reserve buffer fallito
    {
         return -1;   
    }      
}


void INTERNAL_DisableTestArea (int TextBoxHandle, char TextBoxWndHandle)
{
    if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].TextAreaIsOn)
    {
        INTERNAL_TextAreaRenderer (TXAREA_DISABLED, CSR_DISABLED, TextBoxHandle, TextBoxWndHandle, STDBUFFER, FROM_START, RENDER);
        // Rendering della textarea: colori off e cursore disattivato
        
        free (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ReserveBuffer);
        // Disalloca il buffer di riserva
        
        if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].KeybIsOpened)
           ndHAL_VKB_DestroyKeyboard (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].VKBHandle);
        
        // Distrugge la tastiera virtuale che era stata usata
        
        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].KeybIsOpened=0;
        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].TextAreaIsOn=0;
    }  
}



//    ----------------------- Segnala la posizione del cursore ---------------------------------


void INTERNAL_SignalCursorPos (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{
    char InternalBuffer  [255];
    char InternalBuffer0 [255];
    
    strcpy (InternalBuffer, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].DescriptorString);
    strcat (InternalBuffer, " (");
    _itoa   (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX, InternalBuffer0, 10);
    strcat (InternalBuffer, InternalBuffer0);
    strcat (InternalBuffer, ":");
    _itoa   (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, InternalBuffer0, 10);
    strcat (InternalBuffer, InternalBuffer0);
    strcat (InternalBuffer, ")");
    
    ndHAL_VKB_DisplayInVKeyboard (InternalBuffer, VKBHandle, RENDER); 
}


void INTERNAL_LastRowIsFull (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{
    ndHAL_VKB_DisplayInVKeyboard ("[Last row is full]", VKBHandle, RENDER); 
}


//    ----------------------- Aggiungi caratteri al display ---------------------------------


void INTERNAL_TestArea_AddCharToDisplay (unsigned char LoCode, int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{
    char *PointerToChar;
    int  NewPosCursX, NewPosCursY;
    
    int  CounterX, CounterY;
    char YouCanExit;
        
    PointerToChar = INTERNAL_GetCharAddr (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX, 
                                          WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, 
                                          TextBoxHandle, TextBoxWndHandle, RESERVE);
    
    *PointerToChar = LoCode;      // Registra il nuovo dato
    
    // Provvedi a controllare a ritroso tutte le precedenti posizioni della TextArea: se
    // ci sono degli zeri, devono essere sostituiti da degli spazi (codice char 32).
    // Dall'analisi è necessario escludere l'ultimo carattere appena inserito
    
    YouCanExit=0;
    
    CounterX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX-1;
    CounterY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY;
    
    while (!YouCanExit)
    {
       if (CounterX>=0)
       {
           PointerToChar = INTERNAL_GetCharAddr (CounterX, CounterY, TextBoxHandle, TextBoxWndHandle, RESERVE);
        
           if (*PointerToChar==0) 
           {
               *PointerToChar=32;
           }
           else
           {
              break;     // Una volta trovato il primo carattere diverso da zero, l'analisi può essere terminata                       
           }
       } 
       
       CounterX--;
       
       if (CounterX<0)
       {
           CounterY--;
           CounterX=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1;            
       }
       
       if (CounterY<0)         // Siamo all'inizio. Possiamo terminare di andare a ritroso
       {
           break;
       }
    }
    
    // Aggiorna la nuova posizione del cursore
                                          
    NewPosCursX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX + 1;
    
    if (NewPosCursX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX)
    {
         WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = NewPosCursX;
    }
    else  // Vai a capo
    {
        NewPosCursX = 0;
        NewPosCursY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY + 1;
        
        if (NewPosCursY<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY)
        {
            WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = NewPosCursX;
            WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = NewPosCursY;
        }
        else   // Siamo a fine area: esci senza registrare il nuovo dato e senza cambiare la posizione del mouse
        {
            return;
        }
    }
    
    // Aggiorna FirstChar
    
    if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX)<0)
    {
       WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX;            
    }
    
    if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY)<0)
    {
       WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY;              
    }
    
    if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX)
           >WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsX-1)
    {
       WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsX;    
    }
    
    if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY)
           >WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsY-1)
    {
       WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsY;    
        
    }

    // Esegui l'aggiornamento sullo schermo
    INTERNAL_SignalCursorPos  (VKBHandle, TextBoxHandle, TextBoxWndHandle);
    INTERNAL_TextAreaRenderer (TXAREA_ENABLED, CSR_ENABLED, TextBoxHandle, TextBoxWndHandle, RESERVE, 0, RENDER);
} 


void INTERNAL_TestArea_MoveCursor (int DiffX, int DiffY, int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{
    int  NewPosCursX, NewPosCursY;
                                          
    NewPosCursX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX + DiffX;
    NewPosCursY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY + DiffY;
    
    if (DiffX!=0)
    {
            if (NewPosCursX<0)
            {
                    NewPosCursX=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1;
                    NewPosCursY=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY-1;
                    
                    if (NewPosCursY>=0)    // Posso andare sulla riga precedente
                    {
                        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = NewPosCursX;
                        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = NewPosCursY;                   
                    }
                    else                   // Non posso andare sulla riga precedente
                    {
                        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = 0;
                        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = 0;
                    }
            }
            else
            {
                if (NewPosCursX<=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1)
                {
                    WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = NewPosCursX;                        
                }
                else           // Devo provare ad andare sulla riga successiva
                {
                    NewPosCursX=0;
                    NewPosCursY=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY+1;
                    
                    if (NewPosCursY<=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1)
                    {                      // Posso andare sulla riga successiva
                        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = NewPosCursX;  
                        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = NewPosCursY;  
                    }
                    else
                    {                      // Non posso andare sulla riga successiva
                        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1;
                        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1; 
                    }                               
                }
                
            }       
    }
    
    
    if (DiffY!=0)
    {
            if (NewPosCursY<0)
            {
                WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = 0;
            }
            else
            {
                if (NewPosCursY>WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1)
                {
                    WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1;                        
                }
                else
                {
                    WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = NewPosCursY;
                }
            }
    }
    
    
    // Aggiorna FirstCharX e FirstCharY
    
    if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX)<0)
    {
       WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX;            
    }
    
    if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY)<0)
    {
       WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY;              
    }
    
    if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX)
           >WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsX-1)
    {
       WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsX-1);    
    }
    
    if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY)
           >WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsY-1)
    {
       WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsY-1);    
        
    }

    // Esegui l'aggiornamento sullo schermo
    INTERNAL_SignalCursorPos  (VKBHandle, TextBoxHandle, TextBoxWndHandle);
    INTERNAL_TextAreaRenderer (TXAREA_ENABLED, CSR_ENABLED, TextBoxHandle, TextBoxWndHandle, RESERVE, 0, RENDER);
}

void INTERNAL_TestArea_CarriageReturn (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{
    int  NewPosCursX, NewPosCursY;
    char LastRowIsVoid;
    int  Counter;
    char *PntToChar;
    
    NewPosCursX = 0;
    NewPosCursY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY + 1;
        
    // Verifica se nuova posizione del cursore è ammessa
    
    if (NewPosCursY<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY)
    {
             // Provvedi a copiare i dati sulla riga successiva: per prima cosa è necessario verificare
             // se l'ultima riga è vuota
        
             LastRowIsVoid=1;
            
             for (Counter=0; Counter<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; Counter++)
             {
                PntToChar = INTERNAL_GetCharAddr (Counter, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1, TextBoxHandle, TextBoxWndHandle, RESERVE); 
            
                if (*PntToChar!=0)
                {
                   LastRowIsVoid=0;            // Non è vero che l'ultima riga è vuota
                   break;               
                }
             }
            
             // Se l'ultima riga è vuota....
        
             if ((LastRowIsVoid) && (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY>=2))
             {
                         int CounterX, CounterY, CounterBX;
                         char *PntToCharSrc, *PntToCharDest;
                         
                         // ....  provvede a copiare tutte le righe di un elemento in basso
                         
                         for (CounterY=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-2; CounterY>WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY; CounterY--)
                         {
                             for (CounterX=0; CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; CounterX++)
                             {
                                PntToCharSrc  = INTERNAL_GetCharAddr (CounterX, CounterY,   TextBoxHandle, TextBoxWndHandle, RESERVE); 
                                PntToCharDest = INTERNAL_GetCharAddr (CounterX, CounterY+1, TextBoxHandle, TextBoxWndHandle, RESERVE);
                                
                                *PntToCharDest = *PntToCharSrc;    // Copia il carattere  
                             }
                         } 
                         
                         // .... costruisci la riga immediatamente sotto la posizione attuale del cursore:
                         // prima riempi di zeri la riga immediatamente sotto la posizione del cursore ....
                         
                         for (CounterX=0; CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; CounterX++)
                         {
                             PntToCharDest = INTERNAL_GetCharAddr (CounterX, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY+1, TextBoxHandle, TextBoxWndHandle, RESERVE);      
                             *PntToCharDest = 0;    // Poni uno zero nel carattere  
                         }     
                         
                         // .... e successivamente copia gli ultimi caratteri della riga precedente
                         
                         CounterX  =0;
                         CounterBX =WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX;
                                     
                         while (CounterBX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX)
                         {
                            PntToCharSrc  = INTERNAL_GetCharAddr (CounterBX, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY,   TextBoxHandle, TextBoxWndHandle, RESERVE); 
                            PntToCharDest = INTERNAL_GetCharAddr (CounterX,  WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY+1, TextBoxHandle, TextBoxWndHandle, RESERVE);    
                            
                            *PntToCharDest = *PntToCharSrc;
                            *PntToCharSrc  = 0;
                            
                            CounterX++;
                            CounterBX++;   
                         }         
             }
             else        // L'ultima riga è piena: segnalalo all'utente
             {
                 INTERNAL_LastRowIsFull  (VKBHandle, TextBoxHandle, TextBoxWndHandle); 
                 ndDelay (0.1);
             }
        
             // Provvede a modificare PosCursX e PosCursY con i nuovi valori
        
             WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = NewPosCursX;
             WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = NewPosCursY;
        
             // Adesso aggiorniamo FirstCharX e FirstCharY
        
             if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX)<0)
             {
                WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX;            
             }
        
             if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY)<0)
             {
                WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY;              
             }
        
             if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX)
                    >WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsX-1)
             {
                WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsX-1);    
             }
        
             if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY)
                    >WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsY-1)
             {
                WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsY-1);    
             }
    
             // Esegui l'aggiornamento sullo schermo
             INTERNAL_SignalCursorPos  (VKBHandle, TextBoxHandle, TextBoxWndHandle);
             INTERNAL_TextAreaRenderer (TXAREA_ENABLED, CSR_ENABLED, TextBoxHandle, TextBoxWndHandle, RESERVE, 0, RENDER);
    }
    
}


void INTERNAL_TestArea_DeleteKey (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{
    int  NewPosCursX, NewPosCursY;
    char ActualRowIsVoid;
    int  CounterX, CounterY;
    char *PntToChar, *PntToCharSrc, *PntToCharDest;
    
    NewPosCursX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX-1;
    NewPosCursY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY;
        
    // Verifica se nuova posizione del cursore è ammessa
    
    if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX>=1)   
    {
             // Il cursore non è sulla prima colonna:
                   
             // Provvedi a copiare tutti i caratteri che sono a sinistra della nuova posizione:
             // questo cancellerà il carattere a destra della posizione attuale del cursore
             
             char *PntToCharSrc, *PntToCharDest;
             int  CounterX;
                         
             CounterX  = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX;
             
             while (CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX)
             {
                 PntToCharSrc  = INTERNAL_GetCharAddr (CounterX,   WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, TextBoxHandle, TextBoxWndHandle, RESERVE); 
                 PntToCharDest = INTERNAL_GetCharAddr (CounterX-1, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, TextBoxHandle, TextBoxWndHandle, RESERVE);                  
             
                 *PntToCharDest = *PntToCharSrc;    // Copia il carattere
                 
                 CounterX++;
             }
             
             // Inserisci uno zero come ultimo carattere
             
             PntToCharDest = INTERNAL_GetCharAddr (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1, 
                                                 WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, 
                                                 TextBoxHandle, TextBoxWndHandle, RESERVE);                  
             
             *PntToCharDest=0;
             
             // Provvede a modificare PosCursX e PosCursY con i nuovi valori
        
             WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = NewPosCursX;
             WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = NewPosCursY;
        
             
    }
    else     // Cursore sulla prima colonna
    {
        if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX==0)
        {
                 // Controlla che il cursore non sia sull'ultima riga.....
                 
                 if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1)
                 {
                        // .... e che non sia sulla prima riga
                        
                        if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY>0)
                        {
                        
                                // Verifichiamo se la riga corrente è completamente vuota
                               
                                ActualRowIsVoid = 1;
                                
                                for (CounterX=0; CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; CounterX++)
                                {
                                    PntToChar = INTERNAL_GetCharAddr (CounterX,   WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, TextBoxHandle, TextBoxWndHandle, RESERVE); 
                                    
                                    if (*PntToChar!=0)
                                    {
                                        ActualRowIsVoid=0;           // Non è vero che la riga corrente è vuota
                                        break;                       // Termina il ciclo for
                                    }
                                }  
                                
                                if (ActualRowIsVoid)
                                {
                                        // Se la riga presente è completamente vuota, provvedi a copiare tutte le righe
                                        // di un elemento in alto
                                    
                                        for (CounterY=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY; CounterY<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1; CounterY++)
                                        {
                                            for (CounterX=0; CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; CounterX++)
                                            {
                                                PntToCharSrc  = INTERNAL_GetCharAddr (CounterX, CounterY+1,   TextBoxHandle, TextBoxWndHandle, RESERVE); 
                                                PntToCharDest = INTERNAL_GetCharAddr (CounterX, CounterY,     TextBoxHandle, TextBoxWndHandle, RESERVE);
                                            
                                                *PntToCharDest = *PntToCharSrc;    // Copia il carattere  
                                            }
                                        } 
                                     
                                        // Adesso riempi di zero l'ultima riga 
                                     
                                        for (CounterX=0; CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; CounterX++)
                                        {
                                            PntToCharDest = INTERNAL_GetCharAddr (CounterX, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1, TextBoxHandle, TextBoxWndHandle, RESERVE);      
                                            *PntToCharDest = 0;    // Poni uno zero nel carattere  
                                        }  
                                        
                                        // Setta la nuova posizione del cursore: controlla qual'è l'ultimo carattere
                                        // non vuoto sulla riga precedente
                                        
                                        for (CounterX=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1; CounterX>=0; CounterX--)
                                        {
                                            PntToCharSrc = INTERNAL_GetCharAddr (CounterX, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY-1, TextBoxHandle, TextBoxWndHandle, RESERVE);      
                                        
                                            if (*PntToCharSrc!=0)
                                            {
                                                break;                 
                                            }
                                        }      
                                        
                                        // Ok, ora in CounterX c'è la posizione dell'ultimo carattere della riga precedente che
                                        // è diverso da zero
                                        
                                        if (CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1)
                                        {
                                            WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = CounterX+1;
                                            WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY--;                    
                                        }
                                        else           // La vecchia riga era tutta piena
                                        {
                                            WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1;
                                            WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY--;         
                                        }   
                                        
                                         
                                       
                                }      // End (if ActualRowIsVoid)    
                        }              // End (if PosCursY>0)
                 }                     // End (if PosCursY<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1)
        }                              // End else (if PosCursX==0)
    
    
    }                                  // End (if PosCursX>=1)
    
    // Adesso aggiorniamo FirstCharX e FirstCharY
        
     if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX)<0)
     {
        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX;            
     }

     if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY)<0)
     {
        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY;              
     }

     if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX)
            >WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsX-1)
     {
        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX - (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsX-1);    
     }

     if ((WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY)
            >WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsY-1)
     {
        WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].FirstCharY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY - (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ShownCharsY-1);    
     }

     // Esegui l'aggiornamento sullo schermo
     INTERNAL_SignalCursorPos  (VKBHandle, TextBoxHandle, TextBoxWndHandle);
     INTERNAL_TextAreaRenderer (TXAREA_ENABLED, CSR_ENABLED, TextBoxHandle, TextBoxWndHandle, RESERVE, 0, RENDER);

     return;   
}


void INTERNAL_TestArea_CancKey (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{
    int  CounterX, CounterY;
    char *PntToChar, *PntToCharSrc, *PntToCharDest;
    char ActualRowIsVoid;
            
    // Controlla che il cursore non sia sull'ultimo carattere della riga
    
    if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1)
    {         
            // Scroll in alto delle righe
            // ----------------------------------------------------------------
            // Iniziamo controllando che il cursore non sia sulla prima colonna,
            // non sull'ultima riga, in una riga che è completamente vuota
            
            if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX==0)
            {
                if (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1)
                {
                        // Verifichiamo se la riga corrente è completamente vuota
                       
                        ActualRowIsVoid = 1;
                        
                        for (CounterX=0; CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; CounterX++)
                        {
                            PntToChar = INTERNAL_GetCharAddr (CounterX,   WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, TextBoxHandle, TextBoxWndHandle, RESERVE); 
                            
                            if (*PntToChar!=0)
                            {
                                ActualRowIsVoid=0;           // Non è vero che la riga corrente è vuota
                                break;                       // Termina il ciclo for
                            }
                        }        
                        
                        if (ActualRowIsVoid)                 // La riga attuale è vuota 
                        { 
                                 // Se la riga presente è completamente vuota, provvedi a copiare tutte le righe
                                 // di un elemento in alto
                                
                                 for (CounterY=WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY; CounterY<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1; CounterY++)
                                 {
                                     for (CounterX=0; CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; CounterX++)
                                     {
                                        PntToCharSrc  = INTERNAL_GetCharAddr (CounterX, CounterY+1,   TextBoxHandle, TextBoxWndHandle, RESERVE); 
                                        PntToCharDest = INTERNAL_GetCharAddr (CounterX, CounterY,     TextBoxHandle, TextBoxWndHandle, RESERVE);
                                        
                                        *PntToCharDest = *PntToCharSrc;    // Copia il carattere  
                                     }
                                 } 
                                 
                                 // Adesso riempi di zero l'ultima riga 
                                 
                                 for (CounterX=0; CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX; CounterX++)
                                 {
                                     PntToCharDest = INTERNAL_GetCharAddr (CounterX, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY-1, TextBoxHandle, TextBoxWndHandle, RESERVE);      
                                     *PntToCharDest = 0;    // Poni uno zero nel carattere  
                                 }    
                                 
                                 // Dopo lo scroll in alto di una riga, renderizza ed esci
                                 INTERNAL_SignalCursorPos  (VKBHandle, TextBoxHandle, TextBoxWndHandle);
                                 INTERNAL_TextAreaRenderer (TXAREA_ENABLED, CSR_ENABLED, TextBoxHandle, TextBoxWndHandle, RESERVE, 0, RENDER); 
                                 return;
                        } 
                           
                }          
            }              
      
            // Termine della sezione che si occupa dell'eventuale scroll in alto delle righe 
            // -------------------------------------------------------------------------------- 
            // Provvedi a copiare tutti i caratteri che sono a sinistra della nuova posizione:
            // questo cancellerà il carattere che si trova nella posizione attuale del cursore
             
            CounterX  = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX+1;
             
            while (CounterX<WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX)
            {
                 PntToCharSrc  = INTERNAL_GetCharAddr (CounterX,   WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, TextBoxHandle, TextBoxWndHandle, RESERVE); 
                 PntToCharDest = INTERNAL_GetCharAddr (CounterX-1, WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, TextBoxHandle, TextBoxWndHandle, RESERVE);                  
             
                 *PntToCharDest = *PntToCharSrc;    // Copia il carattere
                 
                 CounterX++;
            }
    }
             
    // Inserisci uno zero come ultimo carattere: questa è l'unica cosa da fare se il cursore si
    // trova nell'ultima colonna
             
    PntToCharDest = INTERNAL_GetCharAddr (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX-1, 
                                                 WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY, 
                                                 TextBoxHandle, TextBoxWndHandle, RESERVE);                          
    *PntToCharDest=0;
    
            
    // Esegui l'aggiornamento sullo schermo
    INTERNAL_SignalCursorPos  (VKBHandle, TextBoxHandle, TextBoxWndHandle);
    INTERNAL_TextAreaRenderer (TXAREA_ENABLED, CSR_ENABLED, TextBoxHandle, TextBoxWndHandle, RESERVE, 0, RENDER);
    return;
}


//    ---------- Gestione OK/ESC/BreakCallBack per la tastiera --------------------------


void INTERNAL_TestArea_OkKey (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{
    void (*PntToCbProcessValueBefore)(void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle);
    char (*PntToCbProcessValueForValidation)(void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle);
    void (*PntToCbProcessValueAfter)(void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle);
    
    void *PntToProcessValueData, *PntToTextData, *PntToResTextData;
    char DataHasBeenValidated;
    
    // Setta il puntatore ai dati
    PntToProcessValueData = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ProcessValueData;
    PntToTextData         = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].TextData; 
    PntToResTextData      = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ReserveBuffer;   
        
    // E' stato premuto il tasto OK: per prima cosa eseguiamo la callback before 
    PntToCbProcessValueBefore = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].cbProcessValueBefore;
    
    if (PntToCbProcessValueBefore!=0)
    {
         (*PntToCbProcessValueBefore)(PntToProcessValueData, PntToResTextData, TextBoxHandle, TextBoxWndHandle);
    }
    
    // Esegui la procedura di validazione: essa deve essere eseguita sui dati del reserve buffer
    PntToCbProcessValueForValidation = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].cbProcessValueForValidation;
    
    if (PntToCbProcessValueForValidation!=0)
    {
         DataHasBeenValidated = (*PntToCbProcessValueForValidation)(PntToProcessValueData, PntToResTextData, TextBoxHandle, TextBoxWndHandle);   
    }
    else     // Se non è stata stabilita una procedura di validazione, vuol dire che i dati sono validati di default 
    {
         DataHasBeenValidated = 1;    
    }
        
    if (DataHasBeenValidated)
    {
            // Adesso il contenuto del buffer di riserva deve essere copiato nel campo TextData. 
            
            int Size  = (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsX+1)*
                        (WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].NrCharsY+1);
                   
            memcpy (PntToTextData, PntToResTextData, Size);
             
            // Adesso eseguiamo la callback after: questo si esegue sulla text-area
            PntToCbProcessValueAfter = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].cbProcessValueAfter;
            
            if (PntToCbProcessValueAfter!=0)
            {
                 (*PntToCbProcessValueAfter)(PntToProcessValueData, PntToTextData, TextBoxHandle, TextBoxWndHandle);   
            }
    }
            
    // Adesso disabilita la TextArea. Questo provvederà anche a disallocare il ReserveBuffer 
     
    INTERNAL_DisableTestArea (TextBoxHandle, TextBoxWndHandle); 
}

void INTERNAL_TestArea_EscKey (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle)
{   
    // Ripristiniamo la vecchia posizione del cursore
                   
    WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursX = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ResPosCursX;
    WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].PosCursY = WindowData [TextBoxWndHandle].TextBox [TextBoxHandle].ResPosCursY;
                   
    // Disattiva la testarea e disalloca il reserve buffer        
    
    INTERNAL_DisableTestArea (TextBoxHandle, TextBoxWndHandle); 
}

void INTERNAL_TestArea_BreakCallback (void *BreakCallbackData, int VKBHandle, char WndHandle)
{   
    // La callback è concepita in modo che BreakCallbackData corrisponda all'indirizzo
    // della struct di tipo TextBox_Type che contiene le informazioni relative alla
    // TextArea
    
    struct TextBox_Type *TextBoxRecord = BreakCallbackData;
    
    // Ripristiniamo la vecchia posizione del cursore
                   
    TextBoxRecord->PosCursX = TextBoxRecord->ResPosCursX;
    TextBoxRecord->PosCursY = TextBoxRecord->ResPosCursY;
                   
    // Disattiva la testarea e disalloca il reserve buffer        
    
    INTERNAL_DisableTestArea (TextBoxRecord->TextBoxHandle, TextBoxRecord->TextBoxWndHandle); 
}






//    ------------------------------- KeyCallback principale per la tastiera --------------------------

void INTERNAL_TextArea_KeyCallback (int KeyPressedCode, void *KeyCallbackData, int VKBHandle, char WndHandle)
/*
   Questa è la callback che gestisce i tasti che devono essere inviati alla textarea. Il sistema
   è progettato in modo che KeyCallbackData equivalga all'indirizzo della struttura di tipo
   Textbox_Type che contiene i dati relativi alla TextArea. In questo modo, però, la callback
   non potrebbe conoscere il numero di handle che nanodesktop aveva assegnato a questa 
   tastiera. Per tale ragione, nella struttura è previsto un campo che riporta proprio
   tale informazione. Inoltre, si faccia attenzione al fatto che qui WndHandle è l'handle
   della VirtualKeyboard (questa è una keyboard callback), e non è l'handle della finestra
   che contiene la text-box/text-area. 
   
   Quindi, questa callback non saprebbe nemmeno l'handle della finestra che contiene la TextArea.
   Per questo motivo, è presente un campo apposito dentro la struttura di tipo
   Textbox_Type, che contiene la TextBoxWndHandle. 
*/
{
   struct TextBox_Type *PntToTextBoxData = KeyCallbackData;
   // Recuperata la possibilità di puntare alle informazioni sulla textbox/textarea in memoria   
   
   int TextBoxHandle = PntToTextBoxData->TextBoxHandle;
   // Recuperato il text-box handle
   
   char TextBoxWndHandle = PntToTextBoxData->TextBoxWndHandle;
   // Recuperato l'handle della finestra che gestisce la TextBox
   
   unsigned char HiCode;
   unsigned char LoCode;
   
   KeyPressedCode = KeyPressedCode & 0xFFFF;       // Considera solo i 16 bit meno significativi
     
   HiCode = KeyPressedCode / 256;
   LoCode = KeyPressedCode % 256;
   
   switch (HiCode)
   {
          case 0:          // E' stata premuta una lettera normale
          {
              INTERNAL_TestArea_AddCharToDisplay (LoCode, VKBHandle, TextBoxHandle, TextBoxWndHandle);
              break; 
          }
          
          case ND_GENERIC_SPECIALKEY:
          {
               switch (LoCode)
               {
                      case 0x61:
                      {
                           INTERNAL_TestArea_MoveCursor (0, -1, VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break; 
                      }
                      
                      case 0x62:
                      {
                           INTERNAL_TestArea_MoveCursor (0, +1, VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break; 
                      }
                      
                      case 0x63:
                      {
                           INTERNAL_TestArea_MoveCursor (-1, 0, VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break; 
                      }
                           
                      case 0x64:
                      {
                           INTERNAL_TestArea_MoveCursor (+1, 0, VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break; 
                      }
                      
                      case 0x71:                        // PageUp
                      {
                           INTERNAL_TestArea_MoveCursor (0, -4, VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break; 
                      }
                      
                      case 0x72:                        // PageDown
                      {
                           INTERNAL_TestArea_MoveCursor (0, +4, VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break; 
                      }
                      
                      case 0x08:                        // Tab
                      {
                           INTERNAL_TestArea_MoveCursor (+4, 0, VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break; 
                      }
                      
                      case 0x20:                        // Enter
                      {
                           if (PntToTextBoxData->NrCharsY>1)
                           {
                               INTERNAL_TestArea_CarriageReturn (VKBHandle, TextBoxHandle, TextBoxWndHandle);
                               break;
                           }
                           else    // E' una textbox e non una textarea
                           {
                               INTERNAL_TestArea_OkKey (VKBHandle, TextBoxHandle, TextBoxWndHandle);
                               break;
                           }          
                      }   
                      
                      case 0x10:                        // Delete
                      {
                           INTERNAL_TestArea_DeleteKey (VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break;
                      }    
                      
                      case 0x12:                        // Canc
                      {
                           INTERNAL_TestArea_CancKey (VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break;
                      }
                      
                      case 0x21:                        // Tasto OK
                      {
                           INTERNAL_TestArea_OkKey (VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break;
                      }  
                      
                      case 0xFF:                        // Tasto ESC
                      {
                           INTERNAL_TestArea_EscKey (VKBHandle, TextBoxHandle, TextBoxWndHandle);
                           break;
                      }
                           
               }
          }
   } 
       
}



//    ---------- Manager per il pulsante virtuale della TextArea --------------------------


ndint64 INTERNAL_TextAreaManager (char *StringID, ndint64 InfoField, char WndHandle)
{
    /*
       Questa routine si occupa di gestire l'immissione di caratteri all'interno della
       textarea
    */
    
    int  TextBoxHandle = (int)(InfoField);        // InfoField qui contiene l'handle della text-area
    int  VKBHandle;
    char ErrRep;
    
    if (!WindowData [WndHandle].TextBox [TextBoxHandle].TextAreaIsOn)
    {
        ErrRep = INTERNAL_EnableTestArea (TextBoxHandle, WndHandle);
                    
        if (!ErrRep)
        {                         
                   // Crea una tastiera callback based sullo schermo
           
                   VKBHandle = ndHAL_VKB_AssignMeKeyboard (WindowData [WndHandle].TextBox [TextBoxHandle].DescriptorString,
                                                             0, ND_KEYBOARD_CALLBACK_BASED, 0, 
                                                             INTERNAL_TextArea_KeyCallback, &(WindowData [WndHandle].TextBox [TextBoxHandle]), 
                                                             0, 0, 
                                                             INTERNAL_TestArea_BreakCallback, &(WindowData [WndHandle].TextBox [TextBoxHandle])  );
                                                      
                   if (VKBHandle>=0)                       // Non ci sono stati errori nell'assegnazione della tastiera
                   {
                       WindowData [WndHandle].TextBox [TextBoxHandle].VKBHandle = VKBHandle;
                       WindowData [WndHandle].TextBox [TextBoxHandle].KeybIsOpened = 1;
                       return 0;        
                   }
                   else        // Problemi nel visualizzare la tastiera
                   {
                       INTERNAL_DisableTestArea (TextBoxHandle, WndHandle); // Provvede a disabilitare la TestArea
                       return -1;      
                   }
           
        }
        else        // Fallito il tentativo di attivare la TextArea (il reserve buffer non è stato allocato ?)
           return ErrRep;               
    }
    else
      return 0;
}



/* *****************************************************************************
 *******************************************************************************
 **************************** API pubblica     *********************************
 *******************************************************************************
 ******************************************************************************* */


char *ndTBOX_GetCharAddr (int PosX, int PosY, int TextBoxHandle, char WndHandle)
/*
   Questa funzione restituisce l'indirizzo di un carattere della TextArea. 
   Se c'è qualche errore, la routine restituisce 0
*/
{
   char *PointerToChar;   
   
   if (!ndInitSystemExecuted) ndInitSystem();
      
   if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
   {
        if ((PosX>=0) && (PosX<WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsX))
        {
              if ((PosY>=0) && (PosY<WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsY))
              {
                   PointerToChar = INTERNAL_GetCharAddr (PosX, PosY, TextBoxHandle, WndHandle, STDBUFFER);     
                   return PointerToChar;         
              }
              else return 0;
        }
        else return 0;
   }
   else return 0;   
}
   

char *ndTBOX_GetRowAddr (int PosY, int TextBoxHandle, char WndHandle)
/*
   Questa funzione restituisce l'indirizzo del primo carattere di una riga della TextArea 
   Se c'è qualche errore, la routine restituisce 0
*/
{
   char *PointerToChar;   
   
   if (!ndInitSystemExecuted) ndInitSystem();
      
   if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
   {
          if ((PosY>=0) && (PosY<WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsY))
          {
               PointerToChar = INTERNAL_GetRowAddr (PosY, TextBoxHandle, WndHandle, STDBUFFER);              
               return PointerToChar;         
          }
          else return 0;
   }
   else return 0;   
}
  
int ndTBOX_GetNrCharsX (int TextBoxHandle, char WndHandle)
/*
   Questa routine provvede a fornire il numero di colonne della TextArea. 
   
   Se il valore restituito è negativo, siamo in presenza di un errore. 
   Sono previsti i seguenti codici di errore:
        
          ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
          ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE   L'handle della TextBox non è attivo
*/
{
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
        if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
        {
               return (WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsX);
        }
        else return ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED; 
}


int ndTBOX_GetNrCharsY (int TextBoxHandle, char WndHandle)
/*
   Questa routine provvede a fornire il numero di righe della TextArea. 
   
   Se il valore restituito è negativo, siamo in presenza di un errore. 
   Sono previsti i seguenti codici di errore:
        
          ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
          ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE   L'handle della TextBox non è attivo
*/
{
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
        if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
        {
               return (WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsY);
        }
        else return ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED; 
}



int ndTBOX_CreateTextArea    (unsigned short int PosX1, unsigned short int PosY1, 
                              unsigned short int PosX2, unsigned short int PosY2, 
                              char *DescriptorString, ndint64 Options, void *TextData, 
                              TypeColor TextColorOn,  TypeColor BgTextColorOn,
                              TypeColor TextColorOff, TypeColor BgTextColorOff, 
                              TypeColor TextColorInh, TypeColor BgTextColorInh,
                              void *cbProcessValueBefore, void *cbProcessValueForValidation, 
                              void *cbProcessValueAfter,  void *ProcessValueData, 
                              char WndHandle, char RenderNow)
/*
     Questa routine crea una textarea sullo schermo. Le coordinate della TextArea
     sono in PosX1, PosY1, PosX2, PosY2. 
     
     Il campo DescriptorString è un puntatore ad una stringa, che sarà quella 
     che verrà visualizzata nella VirtualKeyboard associata alla TextArea
     quando si tenterà di immettere dei caratteri
     
     Il parametro Options è un parametro a 64 bit che contiene una serie di
     informazioni. 
     
     I bit sono mappati nel seguente modo:
           
           31-24        Contiene l'handle del font usato dalla TextArea
                        L'utente può settare questo valore utilizzando la key
                        NDKEY_FONT (WndHandle). Se questi bit sono 
                        impostati a zero, il sistema userà il font standard
                        della finestra WndHandle
           
           56-32, 15-0  Contiene il numero di caratteri sull'asse Y (codice a
                        25 bit, è consentito un valore non superiore a 
                        1048576) ed il numero di caratteri sull'asse X (codice
                        a 16 bit, è consentito un valore non superiore a
                        65535). L'utente può ottenere questo codice usando la
                        key NDKEY_SETTEXTAREA (PosX, PosY). Se questo parametro
                        viene posto a 0, il sistema userà il numero di caratteri
                        X ed Y che viene determinata in base alle dimensioni 
                        della TextArea.
                        
           16           Se viene impostato ad 1, Nanodesktop creerà una textarea
                        inibita. Usare la costante manifesta ND_TEXTAREA_INHIBITED
                        
                        
     Il parametro TextData può avere 2 valori. Se è posto a 0, CreateTextArea
     creerà automaticamente un proprio buffer di caratteri di NrCharX*NrCharY bytes: 
     l'indirizzo di questo buffer può essere determinato con la funzione 
     ndTXTAREA_GetTextBufferAdr. 
     
     Se invece ha un valore diverso da 0, allora vuol dire che il programma chiamante
     ha già allocato il buffer con una propria malloc (esso viene detto, in questo 
     caso, buffer esterno), e verrà usato quello dalla routine
     
     Gli indirizzi cbProcessValueBefore, cbProcessValueForValidation, 
     cbProcessValueAfter si riferiscono a tre callbacks che vengono richiamate 
     quando l'utente preme il tasto OK confermando una certa stringa dentro 
     la TextArea. Queste callbacks hanno il seguente prototipo:
                 
     void cbProcessValueBefore (void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle);
     char cbProcessValueForValidation (void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle);
     void cbProcessValueAfter (void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle);
     
     Quando l'utente preme il tasto OK, il sistema esegue la callback ProcessValueBefore.
     Questa callback ha la funzione di eseguire un compito ogni volta che è stato premuto
     il tasto OK. 
     
     Successivamente, viene eseguita la callback cbProcessValueForValidation, che ha
     la funzione di validare il contenuto della TestArea. L'utente deve programmare
     questa routine in modo che essa restituisca 1 qualora i nuovi dati che sono
     stati immessi dall'utente sono corretti e 0 qualora non sono accettabili. 
     Se l'utente non specifica una cbProcessValueForValidation, i dati si intendono
     validati di default.
     
     Se i nuovi dati sono stati validati, viene copiato il contenuto del ReserveBuffer 
     in TextData.
     
     Infine, viene eseguita la callback ProcessValueAfter.
     
     L'utente può disattivare ciascuna delle tre callback, impedendone l'esecuzione
     automatica, semplicemente ponendo a 0 il relativo parametro nella chiamata
     a ndTBOX_CreateTextArea. Ciò assicura la massima flessibilità. 
     
     Il parametro ProcessValueData che viene passato alle due callback, consente
     all'utente di passare alle proprie callback personalizzate l'indirizzo di
     un record, che può contenere delle informazioni atte a modificare il
     comportamento della stessa callback 
     
     La routine restituisce l'handle textbox assegnato. Se il risultato è negativo,
     siamo in presenza di un errore
      
     Sono previsti i seguenti codici di errore:
          ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
          ERR_FONTHDL_IS_NOT_ACTIVE     Il font handle non è attivo
          ERR_NO_TEXTBOX_HANDLE_IS_AVAILABLE    Non ci sono textbox handle disponibili
          ERR_WRONG_PARAMETERS          I parametri di dimensione sono fuori dal WS oppure 
                                        le dimensioni dell'area sono inferiori a 4x4 
          ERR_MEMORY_ERROR              Non è riuscito ad allocare il buffer interno 
          ERR_NO_BUTTON_RECORD_IS_AVAILABLE  Nessun button record utilizzabile 
          ERR_TEXTBOX_IS_TOO_SMALL      Le dimensioni del font sono tali che la textbox non 
                                        avrebbe alcuna riga o colonna        
*/
{
   ndint64 ButtonOptions;
   
   char StrButton0 [512];
   char StrButton1 [512];
   
   unsigned char FntHandle;
   int  NrCharsX, NrCharsY;
   int  ShownCharsX, ShownCharsY;
   char IHaveFound;
   int  TextBoxHandle, BtnHandle;
   int  Counter;
   
   int  DimCharX, DimCharY;
   int  sDimCharX, sDimCharY;
   
   int  LenX, LenY, mLenX, mLenY;
   int  IUseInternalBuffer;
   char IsInhibited;
   char ErrRep; 
   
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
       LenX = abs (PosX2 - PosX1);
       LenY = abs (PosY2 - PosY2);
              
       if ( (PosX2<WindowData [WndHandle].WSWndLength) &&
            (PosY2<WindowData [WndHandle].WSWndHeight) )
       {        
              mLenX = abs (PosX2 - PosX1) - 4;
              mLenY = abs (PosY2 - PosY1) - 4;
              
              if ((mLenX>4) && (mLenY>4))
              {
      
                  FntHandle = (unsigned char)((Options >> (31-8)) & 0xFF); 
                  if (FntHandle==0) FntHandle = WindowData [WndHandle].ndWS_CurrentFntHandle;
                  
                  if (FontHandlesArray [FntHandle].IsActive)
                  {
                       DimCharX = 8;
                       DimCharY = 8;
                       
                       sDimCharX = DimCharX;
                       sDimCharY = DimCharY + 2;
                       
                       // Caratteri visualizzabili sullo schermo
                               
                       ShownCharsX = (mLenX / sDimCharX) - 1;
                       ShownCharsY = (mLenY / sDimCharY);
                               
                       if ((ShownCharsX>=1) && (ShownCharsY>=1))
                       {
                               // Cerchiamo un handle per le textarea che sia libero
                               
                               IHaveFound = 0;
                               
                               for (Counter=0; Counter<__NRTEXTBOXALLOCABLE; Counter++)
                               {
                                   if (!WindowData [WndHandle].TextBox [Counter].IsActive)
                                   {
                                       TextBoxHandle  = Counter;
                                       IHaveFound = 1;
                                       
                                       break;
                                   } 
                               }
                               
                               if (IHaveFound)      // Ho trovato un handle
                               {
                                       // Caratteri della TextArea
                                       
                                       NrCharsX = Options & 0xFFFF;
                                       NrCharsY = (Options >> 31) & 0x1FFFFFF;
                                       
                                       if ((NrCharsX==0) && (NrCharsY==0))
                                       {
                                           NrCharsX = ShownCharsX;
                                           NrCharsY = ShownCharsY;
                                       }  
                                       
                                       if ((NrCharsX>0) && (NrCharsY>0))
                                       {       
                                               // Provvediamo a lavorare sul buffer caratteri
                                               
                                               if (TextData==0)   
                                               {
                                                   TextData = malloc  ( (NrCharsX+1)*(NrCharsY+1) );
                                                   
                                                   if (TextData)
                                                   {
                                                       memset (TextData, 0, (NrCharsX+1)*(NrCharsY+1));  
                                                       
                                                       IUseInternalBuffer = 1;      
                                                   }   
                                                   else                 // Non sono riuscito ad allocare il buffer interno
                                                   {
                                                       return ERR_MEMORY_ERROR;                 
                                                   }      
                                               }
                                               else     // Buffer esterno
                                               {
                                                   IUseInternalBuffer = 0;    
                                               } 
                                               
                                               // Controlli preliminari effettuati. Possiamo iniziare a lavorare
                                               // Provvediamo a salvare i dati 
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].PosX1 = PosX1;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].PosX2 = PosX2;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].PosY1 = PosY1;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].PosY2 = PosY2;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].LenX  = LenX;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].LenY  = LenY;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].mLenX = mLenX;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].mLenY = mLenY;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].DescriptorString = DescriptorString;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].TextColorOff     = TextColorOff;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorOff   = BgTextColorOff;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].TextColorOn      = TextColorOn;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorOn    = BgTextColorOn;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].TextColorInh     = TextColorInh;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].BgTextColorInh   = BgTextColorInh;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].PosCursX = 0;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].PosCursY = 0;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].FirstCharX = 0;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].FirstCharY = 0;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsX = NrCharsX;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsY = NrCharsY; 
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].ShownCharsX = ShownCharsX;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].ShownCharsY = ShownCharsY; 
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].MargineX = 2 + ( mLenX % sDimCharX)/2;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].MargineY = 2 + ( mLenY % sDimCharY)/2;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].FntHandle = FntHandle;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].DimCharX  = DimCharX;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].DimCharY  = DimCharY;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].sDimCharX = sDimCharX;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].sDimCharY = sDimCharY;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].TextData = TextData;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].IUseInternalBuffer = IUseInternalBuffer;
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].TextBoxHandle = TextBoxHandle; 
                                               WindowData [WndHandle].TextBox [TextBoxHandle].TextBoxWndHandle = WndHandle; 
                                               
                                               // Registra le callbacks di gestione
                                               
                                               WindowData [WndHandle].TextBox [TextBoxHandle].cbProcessValueBefore           = cbProcessValueBefore; 
                                               WindowData [WndHandle].TextBox [TextBoxHandle].cbProcessValueForValidation    = cbProcessValueForValidation;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].cbProcessValueAfter            = cbProcessValueAfter;
                                               WindowData [WndHandle].TextBox [TextBoxHandle].ProcessValueData               = ProcessValueData;
                                               
                                               // Prepara la StringButton
                                               
                                               strcpy (StrButton0, "#TxtAreaBtn_");
                                               _itoa  (TextBoxHandle, StrButton1, 10);
                                               strcat (StrButton0, StrButton1);
                                               strcat (StrButton0, "_");
                                               _itoa  (WndHandle, StrButton1, 10);
                                               strcat (StrButton0, StrButton1);
                                               
                                               // Verifica se si è richiesta l'inibizione
                                               
                                               if ((Options & ND_TEXTAREA_INHIBITED)==ND_TEXTAREA_INHIBITED)
                                                   IsInhibited=1;
                                               else
                                                   IsInhibited=0;
                                                   
                                               // Costruiamo il parametro a 64 bit ButtonOptions
                                              
                                               ButtonOptions = ND_BUTTON_LOGIC_MODE | ND_TRASPARENT_CALLBACK;
                                               if (IsInhibited) ButtonOptions = ButtonOptions | ND_BUTTON_INHIBITED; 
                                               
                                               // Crea il pulsante
                                               
                                               BtnHandle = ndCTRL_CreateButton (PosX1, PosY1, PosX2, PosY2, WndHandle, StrButton0, "", "", 0, 0, 0, 
                                                                                ButtonOptions, &(INTERNAL_TextAreaManager), TextBoxHandle, NORENDER);
                                               
                                               if (BtnHandle>=0)        // Assegnazione dell'handle riuscita
                                               {
                                                      WindowData [WndHandle].TextBox [TextBoxHandle].ButtonHandle = BtnHandle;
                                                      
                                                      WindowData [WndHandle].TextBox [TextBoxHandle].TextAreaIsOn = 0;
                                                      WindowData [WndHandle].TextBox [TextBoxHandle].KeybIsOpened = 0;
                                                      WindowData [WndHandle].TextBox [TextBoxHandle].IsActive     = 1;
                                                   
                                                      // Esegui il rendering della text-area. 
                                                      
                                                      if (!IsInhibited)
                                                      {
                                                           // La TextArea non è stata inibita: Status OFF e cursore disattivato
                                                           INTERNAL_TextAreaRenderer (TXAREA_DISABLED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER); 
                                                           WindowData [WndHandle].TextBox [TextBoxHandle].IsInhibited = 0;
                                                           
                                                           if (RenderNow) XWindowRender (WndHandle);
                                                      }
                                                      else
                                                      {
                                                           // La TextArea è stata inibita: Status TXAREA_INHIBITED e cursore disattivato
                                                           INTERNAL_TextAreaRenderer (TXAREA_INHIBITED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER); 
                                                           WindowData [WndHandle].TextBox [TextBoxHandle].IsInhibited = 1;
                                                           
                                                           if (RenderNow) XWindowRender (WndHandle);
                                                      }
                                                     
                                                      WindowData [WndHandle].NrTextBoxAllocated++;
                                                      
                                                      return TextBoxHandle;        // Restituisce la TextBoxHandle alla routine chiamante
                                               }
                                               else      // Problemi a creare il pulsante logico
                                               {
                                                      if (IUseInternalBuffer) free (TextData);
                                                      
                                                      ErrRep=BtnHandle;
                                                      return ErrRep;       
                                               }
                                       }
                                       else return ERR_WRONG_PARAMETERS;
                               }
                               else return ERR_NO_TEXTBOX_HANDLE_IS_AVAILABLE;
                       }
                       else return ERR_TEXTBOX_IS_TOO_SMALL;
                  }
                  else return ERR_FONTHDL_IS_NOT_ACTIVE;    
              }
              else return ERR_WRONG_PARAMETERS;        
       }
       else return ERR_WRONG_PARAMETERS;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED; 
}
                            

char ndTBOX_InhibitTextArea (int TextBoxHandle, char WndHandle, char RenderNow)
/*
    Questa routine si occupa di inibire una TextArea. L'inibizione impedisce
    che l'utente possa cliccare sulla TextArea attivandola

    Sono previsti i seguenti codici di errore
          ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
          ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE   L'handle della TextBox non è attivo
*/
{
   int ButtonHandle;
   
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
        if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
        {
              ButtonHandle = WindowData [WndHandle].TextBox [TextBoxHandle].ButtonHandle;
              ndCTRL_InhibitButton (ButtonHandle, WndHandle, NORENDER);
              
              if (WindowData [WndHandle].TextBox [TextBoxHandle].KeybIsOpened)
              {
                  ndHAL_VKB_DestroyKeyboard (WindowData [WndHandle].TextBox [TextBoxHandle].VKBHandle);
                  WindowData [WndHandle].TextBox [TextBoxHandle].KeybIsOpened = 0;
              }
              
              if (WindowData [WndHandle].TextBox [TextBoxHandle].TextAreaIsOn)
              {
                  INTERNAL_DisableTestArea (TextBoxHandle, WndHandle);
                  WindowData [WndHandle].TextBox [TextBoxHandle].TextAreaIsOn = 0;           
              }
              
              INTERNAL_TextAreaRenderer (TXAREA_INHIBITED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER); 
              if (RenderNow) XWindowRender (WndHandle);
              
              WindowData [WndHandle].TextBox [TextBoxHandle].IsInhibited = 1;
              return 0;
        }
        else return ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED; 
}




char ndTBOX_DeInhibitTextArea (int TextBoxHandle, char WndHandle, char RenderNow)
/*
    Questa routine si occupa di deinibire una TextArea. 

    Sono previsti i seguenti codici di errore
          ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
          ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE   L'handle della TextBox non è attivo
*/
{
   int ButtonHandle;
   
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
        if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
        {
              ButtonHandle = WindowData [WndHandle].TextBox [TextBoxHandle].ButtonHandle;
              ndCTRL_DeInhibitButton (ButtonHandle, WndHandle, NORENDER);
              
              INTERNAL_TextAreaRenderer (TXAREA_DISABLED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER); 
              if (RenderNow) XWindowRender (WndHandle);
              
              WindowData [WndHandle].TextBox [TextBoxHandle].IsInhibited = 0;
              return 0;
        }
        else return ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED; 
}

char ndTBOX_ChangeTextAreaContent (char TextBoxHandle, char WndHandle, void *NewContent, char RenderNow)
/*
    Questa routine provvede a cambiare il contenuto della TextArea. 
    I dati vengono presi dall'indirizzo NewContent. 
    
    Attenzione perchè deve essere l'utente ad allocare correttamente l'area di memoria
    NewContent in modo che Nanodesktop possa prelevarvi i dati. Se la TextArea ha x righe
    ed y colonne, sarà necessario eseguire un NewContent=malloc ((y+1)*(x+1)) prima
    di richiamare ChangeTextAreaContent. 
    
    Sono previsti i seguenti codici di errore:
        ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
        ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE   L'handle della TextBox non è attivo    
*/
{
   char *PntToSrc, *PntToDest, *PntToTextData;
   int CounterX, CounterY;
   int NrCharsX, NrCharsY;
   
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
        if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
        {
              NrCharsX = WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsX;
              NrCharsY = WindowData [WndHandle].TextBox [TextBoxHandle].NrCharsY;
              
              // Provvedi a cancellare tutta la TextArea
              memset (WindowData [WndHandle].TextBox [TextBoxHandle].TextData, 0, (NrCharsX+1)*(NrCharsY+1)); 
              
              // Adesso copia i caratteri che servono da NewContent
              PntToTextData = WindowData [WndHandle].TextBox [TextBoxHandle].TextData;
              
              for (CounterY=0; CounterY<NrCharsY; CounterY++)
              {
                  for (CounterX=0; CounterX<NrCharsX; CounterX++)
                  {
                     PntToSrc  = NewContent    + (NrCharsX+1) * CounterY + CounterX; 
                     PntToDest = PntToTextData + (NrCharsX+1) * CounterY + CounterX;
                  
                     *PntToDest = *PntToSrc;   // Copia i byte
                  }
              }
              
              // Esegui un nuovo rendering della TextArea
              
              if (!WindowData [WndHandle].TextBox [TextBoxHandle].IsInhibited)
                 INTERNAL_TextAreaRenderer (TXAREA_DISABLED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER);  
              else
                 INTERNAL_TextAreaRenderer (TXAREA_INHIBITED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER);   
              
              if (RenderNow) XWindowRender (WndHandle);
              return 0;
        }
        else return ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}

char ndTBOX_TextAreaUpdate (char TextBoxHandle, char WndHandle, char RenderNow)
/*
    Questa routine provvede ad aggiornare il contenuto grafico della TextBox. 
    
    Normalmente, il programmatore può cambiare il contenuto dell'area di 
    memoria associata alla textbox (l'indirizzo passato tramite il parametro
    TextData al momento della creazione della TextBox oppure l'area
    allocata automaticamente da Nanodesktop se il parametro TextData era
    stato posto a 0). 
    
    Dopo avere cambiato il contenuto dell'area di memoria, è necessario
    l'aggiornamento grafico della TextBox. Questa routine consente di
    ottenerlo. 
    
    Sono previsti i seguenti codici di errore:
        ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
        ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE   L'handle della TextBox non è attivo 
*/
{
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
        if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
        {
              if (!WindowData [WndHandle].TextBox [TextBoxHandle].IsInhibited)
                 INTERNAL_TextAreaRenderer (TXAREA_DISABLED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER);  
              else
                 INTERNAL_TextAreaRenderer (TXAREA_INHIBITED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER);   
              
              if (RenderNow) XWindowRender (WndHandle);
              return 0;          
        }
        else return ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndTBOX_DestroyTextArea (char TextBoxHandle, char WndHandle, char OnlyLogical, TypeColor CoverColor, char RenderNow)
/*
    Questa routine ha lo scopo di distruggere una TextArea. L'elemento viene identificato 
    dalla coppia TextBoxHandle/WndHandle. 
    
    Se si pone ad 1 il parametro OnlyLogical, la TextArea verrà cancellata solo dal
    punto di vista logico: il disegno dell'area rimarrà sullo schermo, anche se
    cliccandoci sopra MouseControl non farà nulla. 
   
    Se invece si pone a 0 il parametro OnlyLogical, l'area verrà cancellata
    anche graficamente: questo viene fatto riempiendo lo spazio dove si trovava il
    pulsante con il colore Color (che è in genere il colore di sfondo della finestra.
   
    Sono previsti i seguenti codici di errore
          ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
          ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE   L'handle della TextBox non è attivo   
*/
{
   int ButtonHandle;
   
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
        if (WindowData [WndHandle].TextBox [TextBoxHandle].IsActive)
        {
              // Cancellazione del pulsante (viene cancellato solo dal punto di vista logico)
              ButtonHandle = WindowData [WndHandle].TextBox [TextBoxHandle].ButtonHandle;
              ndCTRL_DestroyButton (ButtonHandle, WndHandle, 1, 0, NORENDER);
              
              if (WindowData [WndHandle].TextBox [TextBoxHandle].KeybIsOpened)
              {
                   ndHAL_VKB_DestroyKeyboard (WindowData [WndHandle].TextBox [TextBoxHandle].VKBHandle);
                   WindowData [WndHandle].TextBox [TextBoxHandle].KeybIsOpened = 0;
              }
              
              if (WindowData [WndHandle].TextBox [TextBoxHandle].TextAreaIsOn)
              {
                   INTERNAL_DisableTestArea (TextBoxHandle, WndHandle);
                   WindowData [WndHandle].TextBox [TextBoxHandle].TextAreaIsOn = 0;           
              }
              
              if (!OnlyLogical)
              {
                   // Cancella tutta la vecchia area
   
                   ndWS_DrawRectangle (WindowData [WndHandle].TextBox [TextBoxHandle].PosX1, WindowData [WndHandle].TextBox [TextBoxHandle].PosY1,
                                       WindowData [WndHandle].TextBox [TextBoxHandle].PosX2, WindowData [WndHandle].TextBox [TextBoxHandle].PosY2,
                                       CoverColor, CoverColor, WndHandle, NORENDER);
              }
              
              if (WindowData [WndHandle].TextBox [TextBoxHandle].IUseInternalBuffer)
              {
                   // Se l'area in memoria è stata allocata internamente da Nanodesktop, il
                   // sistema provvede automaticamente a disallocarla  
                   
                   free (WindowData [WndHandle].TextBox [TextBoxHandle].TextData);             
              }
              
              INTERNAL_TextAreaRenderer (TXAREA_INHIBITED, CSR_DISABLED, TextBoxHandle, WndHandle, STDBUFFER, FROM_START, NORENDER); 
              if (RenderNow) XWindowRender (WndHandle);
              
              WindowData [WndHandle].TextBox [TextBoxHandle].IsActive=0;
              WindowData [WndHandle].NrTextBoxAllocated--;
              return 0;
        }
        else return ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;        
}












