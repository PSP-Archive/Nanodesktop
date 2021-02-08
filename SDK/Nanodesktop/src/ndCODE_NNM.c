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
    NEW NAME MANAGER
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void INTERNAL_NNM_InitVar (struct NNMData_Type *MyNNMData)
/*
   Questa routine è concepita esclusivamente per uso interno.

   Provvede a reinizializzare tutte le variabili del new name manager.
*/
{
  unsigned short int MargineX, MargineY;
  char XWndHandle;
  unsigned char Counter;

  XWndHandle=MyNNMData->WndHandle;

  MyNNMData->A2_X1=10;
  MyNNMData->A2_Y1=10;
  MyNNMData->A2_X2=WindowData [XWndHandle].WSWndLength-10;
  MyNNMData->A2_Y2=WindowData [XWndHandle].WSWndHeight-10;

  MyNNMData->MaxLenChar = (MyNNMData->A2_X2 - MyNNMData->A2_X1 - 40) / 8;
  MargineX = ( (MyNNMData->A2_X2 - MyNNMData->A2_X1) - MyNNMData->MaxLenChar * 8)/2;
  MargineY = ( (MyNNMData->A2_Y2 - MyNNMData->A2_Y1) - 8)/2;

  MyNNMData->A3_X1=MyNNMData->A2_X1+MargineX;
  MyNNMData->A3_Y1=MyNNMData->A2_Y1+MargineY;
  MyNNMData->A3_X2=MyNNMData->A2_X2-MargineX;
  MyNNMData->A3_Y2=MyNNMData->A2_Y2-MargineY;

  for (Counter=0; Counter<255; Counter++)
  {
      (MyNNMData->Display [Counter]=0);
  }

  MyNNMData->FirstCharShown=0;
  MyNNMData->PosCursor=0;
  MyNNMData->PrevArrowLeftStatus=0;

}

void INTERNAL_NNM_DrawDisplayVoid (struct NNMData_Type *MyNNMData)
/*
   Questa routine è esclusivamente per uso interno. Provvede
   a disegnare la GrayBar del New Name Manager.

   La GrayBar visualizzata è sempre vuota.
*/
{
  ndWS_DrawRectangle (MyNNMData->A2_X1, MyNNMData->A2_Y1, MyNNMData->A2_X2, MyNNMData->A2_Y2,
                              COLOR_GRAY, COLOR_LBLUE, MyNNMData->WndHandle, NORENDER);

  XWindowRender (MyNNMData->WndHandle);
}


void INTERNAL_NNM_DrawArrowLeftGeneric (struct NNMData_Type *MyNNMData, TypeColor Color)
{
 unsigned short int CounterX, CounterY;
 unsigned short int RRPosX, RRPosY;

 RRPosX = MyNNMData->A2_X1+4;
 RRPosY = MyNNMData->A2_Y2-14;

 for (CounterY=0; CounterY<8; CounterY++)
 {
    for (CounterX=0; CounterX<8; CounterX++)
    {
        if (Button_ScrollLeft [CounterY][CounterX] == 'x')
            ndWS_PutPixel ( RRPosX + CounterX, RRPosY + CounterY, Color, MyNNMData->WndHandle, NORENDER);
    }
 }

}

void INTERNAL_NNM_DrawArrowLeft (struct NNMData_Type *MyNNMData)
/*
   Questa routine è esclusivamente per uso interno. Provvede a disegnare la
   freccia a sinistra nel display, per segnalare che esso è in scrolling
*/

{
   char ArrowLeft;

   if ((MyNNMData->PosCursor) >= (MyNNMData->MaxLenChar))
     ArrowLeft=1;
   else
     ArrowLeft=0;

   if ((MyNNMData->PrevArrowLeftStatus!=0) && (ArrowLeft==0))
   {
      INTERNAL_NNM_DrawArrowLeftGeneric (MyNNMData, COLOR_GRAY);
      MyNNMData->PrevArrowLeftStatus=ArrowLeft;
   }

   if ((MyNNMData->PrevArrowLeftStatus!=1) && (ArrowLeft==1))
   {
      INTERNAL_NNM_DrawArrowLeftGeneric (MyNNMData, COLOR_WHITE);
      MyNNMData->PrevArrowLeftStatus=ArrowLeft;
   }

}


void INTERNAL_NNM_UpdateDisplay (struct NNMData_Type *MyNNMData)
/*
   Questa routine è solo per uso interno.

   Essa provvede a visualizzare ad aggiornare nella gray bar il
   contenuto del campo Display, consentendo all'utente di capire
   cosa ha digitato sino a quel momento.
*/

{
  unsigned char Counter;
  unsigned char PosChar;
  unsigned char VirtualPosCursor;

  ndWS_DrawRectangle (MyNNMData->A3_X1, MyNNMData->A3_Y1, MyNNMData->A3_X2, MyNNMData->A3_Y2+2,
                              COLOR_GRAY, COLOR_GRAY, MyNNMData->WndHandle, NORENDER);

  INTERNAL_NNM_DrawArrowLeft (MyNNMData);          // Disegna la freccia a sinistra, ove necessario.

  for (Counter=0; Counter<MyNNMData->MaxLenChar; Counter++)
  {
      PosChar = MyNNMData->FirstCharShown + Counter;

      if (   PosChar < (strlen (MyNNMData->Display))   )
      {
         ndWS_GrWriteChar (MyNNMData->A3_X1 + Counter*8, MyNNMData->A3_Y1,
                                  MyNNMData->Display [PosChar],
                                  COLOR_YELLOW, COLOR_GRAY, MyNNMData->WndHandle,
                                  NORENDER);
      }

  }

  VirtualPosCursor = MyNNMData->PosCursor - MyNNMData->FirstCharShown;

  ndWS_DrawLine (MyNNMData->A3_X1 +     VirtualPosCursor*8, MyNNMData->A3_Y2+2,   // Disegna il cursore
                      MyNNMData->A3_X1 + (VirtualPosCursor+1)*8, MyNNMData->A3_Y2+2,
                      COLOR_YELLOW, MyNNMData->WndHandle, NORENDER);

  XWindowRender (MyNNMData->WndHandle);
}



void INTERNAL_NNM_AddCharToDisplay (struct NNMData_Type *MyNNMData, char Carattere)
/*
   Questa routine è solo per uso interno.

   Essa aggiunge un carattere alla stringa interna MyNNMData->Display e
   cambia la posizione del cursore. Successivamente, esegue l'aggiornamento
   della Gray Bar sullo schermo.
*/

{

   if ((MyNNMData->PosCursor)<=250)        // Fai qualcosa solo se il cursore non è a fine stringa
   {
     MyNNMData->Display [ MyNNMData->PosCursor ] = Carattere;
     MyNNMData->PosCursor = MyNNMData->PosCursor + 1;

     if (MyNNMData->PosCursor >= MyNNMData->MaxLenChar)
     {
        MyNNMData->FirstCharShown = MyNNMData->FirstCharShown + 1;
     }

     INTERNAL_NNM_UpdateDisplay (MyNNMData);
   }
}



void INTERNAL_NNM_DoBackSpaceToDisplay (struct NNMData_Type *MyNNMData)
/*
   Questa routine è solo per uso interno.

   Essa esegue l'operazione di backspace sulla stringa interna MyNNMData->Display e
   cambia la posizione del cursore. Successivamente, esegue l'aggiornamento
   della Gray Bar sullo schermo.
*/

{
  int Counter, Index, NrOrderCharDeleted;
  char TempString [250];
  
  if ((MyNNMData->PosCursor)>0)            // Sposta indietro il cursore solo se
                                           // esso non è all'inizio
  {
     NrOrderCharDeleted = MyNNMData->PosCursor - 1;
     Index=0;
     
     for (Counter=0; Counter < strlen (MyNNMData->Display); Counter++)
     {
            if (Counter!=NrOrderCharDeleted)
            {
                TempString [Index] = MyNNMData->Display [Counter];
                Index++;
            }
     }
         
     TempString [Index] = 0;                            // Terminatore stringa    
     strcpy (MyNNMData->Display, TempString);           // Diventa la nuova stringa
     
     // Adesso dobbiamo solo sistemare la posizione del cursore ed occuparci dello
     // scrolling del display
     
     MyNNMData->PosCursor = MyNNMData->PosCursor - 1;

     if (MyNNMData->PosCursor >= MyNNMData->MaxLenChar)
             MyNNMData->FirstCharShown = MyNNMData->FirstCharShown - 1;

     INTERNAL_NNM_UpdateDisplay (MyNNMData);
     return;
  }

}

void INTERNAL_NNM_DoCancelToDisplay (struct NNMData_Type *MyNNMData)
/*
   Questa routine è solo per uso interno.

   Essa esegue l'operazione di backspace sulla stringa interna MyNNMData->Display e
   cambia la posizione del cursore. Successivamente, esegue l'aggiornamento
   della Gray Bar sullo schermo.
*/

{
  int Counter, Index, NrOrderCharDeleted;
  char TempString [250];
  
  if ( MyNNMData->PosCursor < (strlen (MyNNMData->Display)) )    // Non eseguire nulla se sei a fine            
  {                                                              // stringa
     NrOrderCharDeleted = MyNNMData->PosCursor;
     Index=0;
     
     for (Counter=0; Counter < (strlen (MyNNMData->Display)); Counter++)
     {
            if (Counter!=NrOrderCharDeleted)
            {
                TempString [Index] = MyNNMData->Display [Counter];
                Index++;
            }
     }
         
     TempString [Index] = 0;                            // Terminatore stringa    
     
     strcpy (MyNNMData->Display, TempString);           // Diventa la nuova stringa
     
     
     // Adesso dobbiamo solo sistemare la posizione del cursore ed occuparci di
     // aggiornare il display: occorre verificare se la stringa è troppo piccola
     // per la posizione attuale del puntatore e correggere il problema
     
     if ( MyNNMData->PosCursor > (strlen (MyNNMData->Display)) )
     {
          MyNNMData->PosCursor = strlen (MyNNMData->Display);
     } 
     
     
     INTERNAL_NNM_UpdateDisplay (MyNNMData);
     return;
  }
   
}



void  INTERNAL_NNM_MoveCursorToSx (struct NNMData_Type *MyNNMData)
/*
   Questa routine è solo per uso interno.

   Essa provvede a spostare il cursore a sinistra. Successivamente, esegue 
   l'aggiornamento della Gray Bar sullo schermo.
*/

{
  if ((MyNNMData->PosCursor)>0)            // Sposta indietro il cursore solo se
                                           // esso non è all'inizio
  {
     MyNNMData->PosCursor                  = MyNNMData->PosCursor - 1;

     if (MyNNMData->PosCursor >= MyNNMData->MaxLenChar)
             MyNNMData->FirstCharShown = MyNNMData->FirstCharShown - 1;

     INTERNAL_NNM_UpdateDisplay (MyNNMData);
     return;
  }

}


void  INTERNAL_NNM_MoveCursorToDx (struct NNMData_Type *MyNNMData)
/*
   Questa routine è solo per uso interno.

   Essa provvede a spostare il cursore a destra. Successivamente, esegue 
   l'aggiornamento della Gray Bar sullo schermo.
*/

{
  if ((MyNNMData->PosCursor)<=250)         // Sposta in avanti il cursore solo se
                                           // esso non è a fine stringa
  {
     if ( (MyNNMData->PosCursor + 1) <= strlen (MyNNMData->Display) )
        // Non è consentito portare il cursore oltre l'ultimo carattere della
        // stringa attuale
     {
         MyNNMData->PosCursor                  = MyNNMData->PosCursor + 1;
     
         if (MyNNMData->PosCursor >= MyNNMData->MaxLenChar)
                 MyNNMData->FirstCharShown = MyNNMData->FirstCharShown + 1;
    
         INTERNAL_NNM_UpdateDisplay (MyNNMData);
         return;
     }
  }

}



void INTERNAL_NNM_EnterString (struct NNMData_Type *MyNNMData)
{
   strcpy (MyNNMData->FileOutputAddr, MyNNMData->Display);
   // Copia il valore della stringa MyNNMData->Display all'indirizzo richiesto dalla
   // routine chiamante
}



void INTERNAL_NNM_NullString (struct NNMData_Type *MyNNMData)
{
   strcpy (MyNNMData->FileOutputAddr, "");
   // Copia il valore della stringa MyNNMData->Display all'indirizzo richiesto dalla
   // routine chiamante
}




ndint64 INTERNAL_NNM_KeyCallback (int KeyPressedCode, void *KeyCallbackData, int VKBHandle, char WndHandle)
/*
     Questa routine si occupa di gestire la pressione di un tasto sulla tastiera
     virtuale che viene associata al New Name Manager
*/
{
     struct NNMData_Type *PntNNMData;
     
     unsigned char HiCode;
     unsigned char LoCode;
     
     char IsChanged;
     
     IsChanged  = 0;
     PntNNMData = KeyCallbackData;
     
     KeyPressedCode = KeyPressedCode & 0xFFFF;       // Considera solo i 16 bit meno significativi
     
     HiCode = KeyPressedCode / 256;
     LoCode = KeyPressedCode % 256;
     
     if (HiCode==0)          // E' stata scelta una lettera
     {
            INTERNAL_NNM_AddCharToDisplay (PntNNMData, LoCode);
     }   
     
     
     if (HiCode==ND_GENERIC_SPECIALKEY)        // E' stato premuto un tasto speciale
     {
            switch (LoCode)
            {

                   case 0x10:      // Backspace
                   {
                        INTERNAL_NNM_DoBackSpaceToDisplay (PntNNMData);
                        break;
                   }
                   
                   case 0x12:      // Cancel
                   {
                        INTERNAL_NNM_DoCancelToDisplay (PntNNMData);
                        break;
                   }
                   
                   case 0x63:      // Tasto CSR-SX
                   {
                        INTERNAL_NNM_MoveCursorToSx (PntNNMData);
                        break;
                   }

                   case 0x64:      // Tasto CSR-DX
                   {
                        INTERNAL_NNM_MoveCursorToDx (PntNNMData);
                        break;
                   }

                   case 0x20:     // Enter
                   {
                        INTERNAL_NNM_EnterString (PntNNMData);
                        PntNNMData->VKB_YouCanExit       = 1;
                        PntNNMData->VKB_UserInterruption = 0;
                        break;
                   }

                   case 0xFF:     // ESC
                   {
                        INTERNAL_NNM_NullString (PntNNMData);
                        PntNNMData->VKB_YouCanExit       = 1;
                        PntNNMData->VKB_UserInterruption = 1;
                        break;
                   }
                   
                   case 0x21:     // Tasto OK
                   {
                        INTERNAL_NNM_EnterString (PntNNMData);
                        PntNNMData->VKB_YouCanExit       = 1;
                        PntNNMData->VKB_UserInterruption = 0;
                        break;
                   }
            }                    
     }
     
     
     
     return 0;
}

char INTERNAL_NNM_ExitCallback (void *EndCallbackData, int VKBHandle, char WndHandle)
{
     struct NNMData_Type *PntNNMData = EndCallbackData;
     return (PntNNMData->VKB_YouCanExit);
}

void INTERNAL_NNM_BreakCallback (void *BreakCallbackData, int VKBHandle, char WndHandle)
{
     // Questa callback sarà eseguita automaticamente se si preme il pulsante di chiusura
     // finestra della Virtual Keyboard
     
     struct NNMData_Type *PntNNMData = BreakCallbackData;
     
     PntNNMData->VKB_YouCanExit       = 1;
     PntNNMData->VKB_UserInterruption = 1;
                        
     return;
}













char INTERNAL_GetNewName (char *TitleDisplay, char *FileOutputAddr)
/*
   Questa routine è concepita esclusivamente per uso interno.

   Essa permette di scrivere sullo schermo un nome. Una volta che esso è stato
   digitato, il risultato viene posto nell'indirizzo puntato da FileOutputAddr.

   La routine restituisce:
     0  qualora l'utente abbia inserito un nuovo nome
     -1 qualora l'utente abbia rinunciato ad inserire un nuovo nome con il tasto ESC
     -2 qualora si sia verificato un errore interno alla routine.
*/

{
  struct NNMData_Type NNMData;
  
  int KeybReturnCode;
  int ExitCode;

  NNMData.FileOutputAddr   = FileOutputAddr;
  // Salva il parametro dentro NNMData: ciò sarà utile per l'accesso da parte delle callback
  
  
  NNMData.A1_X1=__NNM_POSX1;
  NNMData.A1_Y1=__NNM_POSY1;
  NNMData.A1_X2=__NNM_POSX1 + __NNM_LENX;
  NNMData.A1_Y2=__NNM_POSY1 + __NNM_LENY;

  NNMData.WndHandle=ndLP_CreateWindow (NNMData.A1_X1, NNMData.A1_Y1, NNMData.A1_X2, NNMData.A1_Y2,
                                              TitleDisplay, COLOR_WHITE, COLOR_GREEN, COLOR_BLACK, COLOR_WHITE, 0);

  if (NNMData.WndHandle>=0)                  // E' stato possibile allocare la finestra per la Gray Bar
  {
      ndLP_MaximizeWindow (NNMData.WndHandle);
      
      INTERNAL_NNM_InitVar (&NNMData);             // Provvede a reinizializzare le variabili del New name manager
      INTERNAL_NNM_DrawDisplayVoid (&NNMData);     // Disegna la GrayBar vuota

      INTERNAL_NNM_UpdateDisplay (&NNMData);
      ndLP_PutWindowInFirstPlane (NNMData.WndHandle);
      
      NNMData.VKB_YouCanExit       = 0;
      NNMData.VKB_UserInterruption = 0;

      KeybReturnCode = ndHAL_VKB_AssignMeKeyboard ("Choose a name", 0, 0, 0, &INTERNAL_NNM_KeyCallback, &NNMData, &INTERNAL_NNM_ExitCallback, 
                                                            &NNMData, &INTERNAL_NNM_BreakCallback, &NNMData);
                                                  
      // Fatti assegnare una tastiera virtuale da parte del sistema: si noti che come area dati
      // per le callback scegliamo proprio la struct NNM che è in stack
      
      if (KeybReturnCode>=0)     // Dati digitati o comunque niente errori
      {                                        
           if (!NNMData.VKB_UserInterruption)       // L'utente ha premuto ENTER dopo avere immesso i dati
                ExitCode = 0;                       // Segnala alla routine chiamante che i dati sono stati immessi
           else                                     // L'utente ha premuto ESC o ha cercato di chiudere la finestra
                ExitCode = -1;
      }
      else     // Si è verificato qualche errore nel processo di creazione della finestra
      {
           ExitCode = -2;         // Non è necessario distruggere la finestra della virtualkeyboard perchè
                                  // viene fatto automaticamente dalla routine ndHAL_VKB_AssignMeKeyboard
      }
      
      // Cancella la finestra del NewName Generator
      
      ndLP_DestroyWindow (NNMData.WndHandle); 
      return (ExitCode);
  }
  else
  {
      // C'è stato un errore nell'allocazione della finestra per la GrayBar;

      INTERNAL_NNM_NullString (FileOutputAddr);
      return (-2);            // Segnala l'errore interno alla routine chiamante
  }
}


