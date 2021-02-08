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
    CODICE VIRTUAL KEYBOARD 1 (PSPE)
    ---------------------------------------------------------------------------
    Questo file contiene il codice relativo alla tastiera virtuale di tipo 1
    (ND_VIDEO_KEYBOARD)
    ---------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>

#define IFAC_NO_KEYB_AUTOCLOSE 0x10000

void INTERNAL_VKB1_ChangeCapsLockStatus (int VKBHandle)
/*
    Provvede al cambiamento dello stato del CapsLock della tastiera
*/
{
    struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
    
    KeybData->kb1.CapsLockOn = !(KeybData->kb1.CapsLockOn);
    INTERNAL_CapsLockDisplay (VKBHandle, KeybData->kb1.CapsLockOn);
    
    return; 
}

void INTERNAL_VKB1_ChangeFuncLockStatus (int VKBHandle)
/*
    Provvede al cambiamento dello stato del FuncLock della tastiera
*/
{
    struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
    
    KeybData->kb1.FuncLockOn = !(KeybData->kb1.FuncLockOn);
    INTERNAL_FuncLockDisplay (VKBHandle, KeybData->kb1.FuncLockOn);
    
    return; 
}



void INTERNAL_VKB1_SendKeyCode (int VKBHandle, int KeyCode)
/*
   Questa routine provvede ad inviare un KeyCode alla UserCallback che è stata
   impostata dall'utente
*/
{
   // Crea il puntatore al registro tastiere 
   struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
   
   // Puntatore a funzione
   void (*PntToKeyCallback) (int KeyPressedCode, void *KeyCallbackData, int VKBHandle, char WndHandle);
    
   // Ritardo necessario su PSP/PSP KSU per evitare pressioni multiple
   ndHAL_RequestDelay (ND_REQDELAY_FOR_VKB0);
   
   if (KeybData->KeyCallback !=0)
   {
      PntToKeyCallback    = KeybData->KeyCallback;
      (*PntToKeyCallback) (KeyCode, KeybData->KeyCallbackData, VKBHandle, KeybData->WndHandle);
   } 
}



ndint64 INTERNAL_VKB1_StdCodeManager (char *StringID, ndint64 InfoField, char WndHandle)
/*
    Questa routine si occupa di convertire i messaggi ottenuti dalla tastiera, che 
    vengono forniti sotto forma di StringID, in codici KeyCode a 16 bit. 
    
    La routine ottiene il VKBHandle attraverso il campo InfoField.
    
    Tali codici KeyCode vengono successivamente inviati alla KeyCallback, che ne
    può eseguire un'elaborazione in base alle specifiche decise dall'utente.
    
    Il codice KeyCode è un codice a 16 bit composto da due campi a 8 bit:
    
    quando il campo Hi è a 0, allora il campo Lo contiene il codice ASCII del
    carattere digitato.
    
    quando il campo Hi è ad un valore diverso, allora vuol dire che è stato 
    premuto un tasto speciale (ESC, CANC, DEL ecc. ecc.)
    
    La routine si occupa anche di gestire le commutazioni di stato della
    tastiera, gestendo internamente i tasti CAPS-LOCK e FUNC 
*/
{
    
    // Variabili
    char TempBuffer [20];
    
    unsigned char LoCode, HiCode;
    int           VKBHandle;
    int           KeyCode;
    
    // Ricava dall'InfoField il VKB Handle
    VKBHandle = InfoField;
    
    // Crea il puntatore
    struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
    
    // Analisi del codice
    
    ndStrCpyBounded (TempBuffer, StringID, 3, 9, 0);
    
    if ( !strcmp (TempBuffer, "-KEY---") )    // E' stato premuto un tasto lettera
    {
           // Viene eseguito un algoritmo per verificare quale simbolo
           // deve essere mostrato
           
           // Per prima cosa verifichiamo se il tasto di primo livello è una lettera A..Z
           
           if ( (StringID[10] >='A') && (StringID[10] <='Z') )
           {
                    // Se il tasto FUNC è disattivato, è necessario semplicemente mostrare la
                    // lettera
                    
                    if (!KeybData->kb1.FuncLockOn)
                    {
                        if (KeybData->kb1.CapsLockOn)    // In questo caso CapsLockOn seleziona maiuscole/minuscole
                        {
                           LoCode = StringID [10];
                           HiCode = 0;
                        }
                        else
                        {
                           LoCode = DeMagn (StringID [10]);
                           HiCode = 0;
                        }
                    }             
                    else          // FuncLock è attivato: è necessario verificare se siamo in presenza di
                    {             // un tasto doppio   
                                  
                         if (StringID[11]!=32)         // Tasto doppio
                         {
                             // In questo caso c'è un tasto A..Z che ha un tasto secondario. Il tasto
                             // FUNC che è attivato seleziona semplicemente il tasto secondario 
                             // (che è un simbolo specifico)                         
                             
                             LoCode = StringID [11];
                             HiCode = 0;
                         }
                         else                          // Tasto singolo
                         {
                             // In questo caso c'è un tasto A..Z che non ha un tasto secondario. Il tasto
                             // FUNC che è attivato provoca l'emissione di un codice FUNC
                                                      
                             LoCode = StringID [10];
                             HiCode = ND_GENERIC_FUNCKEY;
                         }                          
                    }
                    
            }    
            else    
            {
                    // Appurato che il tasto non è una lettera, vediamo se è un numero
                    
                    if ( (StringID[10] >='0') && (StringID[10] <='9') )
                    {
                         
                                 if (!KeybData->kb1.FuncLockOn)
                                 {
                                      // Se il tasto FUNC è disattivato, è necessario semplicemente mostrare il numero
                                      // o il simbolo: 
                             
                                      if (!KeybData->kb1.CapsLockOn)         
                                      {
                                          LoCode = StringID [11];            // Cifra
                                          HiCode = 0;
                                      }
                                      else
                                      {
                                          LoCode = StringID [10];            // Simbolo
                                          HiCode = 0;
                                      }
                                 }
                                 else
                                 {
                                      // Tasto FUNC attivato. Emette codice
                                      
                                      LoCode = StringID [11];                // Cifra
                                      HiCode = ND_GENERIC_FUNCKEY;                         // Codice
                                 }
                                 
                    }    
                    else
                    {
                                 // Abbiamo appurato che il tasto non è nè una lettera nè una cifra
                                 // Il tasto CAPS seleziona la lettera secondaria, mentre il tasto FUNC emette
                                 // un codice funzione
                        
                                 if (!KeybData->kb1.FuncLockOn)
                                 {
                                      if (KeybData->kb1.CapsLockOn)         // Per i tasti simbolo è necessario il caps inverso
                                      {
                                         LoCode = StringID [11];
                                         HiCode = 0;
                                      }
                                      else
                                      {
                                         LoCode = StringID [10];
                                         HiCode = 0;
                                      }    
                                 }
                                 else     // Tasto FUNC attivato: emetti un codice funzione
                                 {
                                      LoCode = StringID [10];
                                      HiCode = ND_GENERIC_FUNCKEY;     
                                 } 
                    }
           }  
           
           KeyCode = 256*HiCode+LoCode;
           
           // Esegui la callback utente
           INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
    
           // Incrementa di 1 il contatore che indica i caratteri letti
           KeybData->NrCharsRead++;
           
           return 1;
    } 
    
    
    
    if ( !strcmp (TempBuffer, "-CAPS--") )    // E' stato premuto il tasto CAPS
    {
         INTERNAL_VKB1_ChangeCapsLockStatus (VKBHandle);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-FUNC--") )    // E' stato premuto il tasto FUNC
    {
         INTERNAL_VKB1_ChangeFuncLockStatus (VKBHandle);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-ESC---") )    // E' stato premuto il tasto ESC
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0xFF;                       // Codice Escape
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         KeybData->NrCharsRead++;
         
         return 1;
    } 
    
    if ( !strcmp (TempBuffer, "-DEL---") )    // E' stato premuto il tasto DEL
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x10;                       // Codice DEL
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    } 
    
    if ( !strcmp (TempBuffer, "-ENTER-") )    // E' stato premuto il tasto ENTER
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x20;                       // Codice ENTER
         KeyCode = 256*HiCode+LoCode;
         
         KeybData->NrCharsRead++;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 1;
    } 
    
    if ( !strcmp (TempBuffer, "-SPACE-") )    // E' stato premuto il tasto SPACE
    {
         HiCode = 0x00;
         LoCode = 0x20;                       // Codice SPACE
         KeyCode = 256*HiCode+LoCode;
         
         KeybData->NrCharsRead++;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 1;
    }
    
    if ( !strcmp (TempBuffer, "-CANC--") )    // E' stato premuto il tasto CANC
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x12;                       // Codice CANC
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "--TAB--") )    // E' stato premuto il tasto TAB
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x08;                       // Codice TAB
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-CSRUP-") )    // E' stato premuto il tasto CSRUP
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x61;                       // Codice CSRUP
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-CSRDW-") )    // E' stato premuto il tasto CSRDW
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x62;                       // Codice CSRDW
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-CSRSX-") )    // E' stato premuto il tasto CSRSX
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x63;                       // Codice CSRSX
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-CSRDX-") )    // E' stato premuto il tasto CSRDX
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x64;                       // Codice CSRDX
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-PAGUP-") )    // E' stato premuto il tasto PAGUP
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x71;                       // Codice PAGUP
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-PAGDW-") )    // E' stato premuto il tasto PAGDW
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x72;                       // Codice PAGDW
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         return 0;
    }
    
    if ( !strcmp (TempBuffer, "-OK---") )    // E' stato premuto il tasto OK
    {
         HiCode = ND_GENERIC_SPECIALKEY;
         LoCode = 0x21;                       // Codice OK
         KeyCode = 256*HiCode+LoCode;
         
         INTERNAL_VKB1_SendKeyCode (VKBHandle, KeyCode);
         KeybData->NrCharsRead++;
         
         return 1;
    }
    
    return 0;
}



ndint64 INTERNAL_VKB1_AutoCloseKeyboard (char WndHandle, ndint64 KeybIncloseInfoField)
{
   // Recupera l'handle della tastiera dal campo KeybIncloseInfoField
   char VKBHandle = (char)(KeybIncloseInfoField & 0xFF);
   
   // Verifica se l'opzione autoclose è attivata
   char NoAutoClose = ( (KeybIncloseInfoField & IFAC_NO_KEYB_AUTOCLOSE) == IFAC_NO_KEYB_AUTOCLOSE );
   
   // Recupera il puntatore all'opportuno KeybData
   struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
   
   // Setup della callback di uscita: puntatore alla break callback
   void (*PntBreakCallback) (void *BreakCallbackData, int VKBHandle, char WndHandle);
   PntBreakCallback = KeybData->BreakCallback;
    
   // Esegue la break callback
   
   if (PntBreakCallback!=0)
   {
       // Esegui l'eventuale BreakCallback
       (*PntBreakCallback) (KeybData->BreakCallbackData, VKBHandle, KeybData->WndHandle);
   }
                        
   // Ritardo necessario per evitare pressioni multiple
   ndHAL_RequestDelay (ND_REQDELAY_CLOSE_VKB);
   
   if (!NoAutoClose)    // Chiudi la finestra se non è attivata l'opzione autoclose
   {
       ndLP_DestroyWindow (KeybData->WndHandle);
       KeybData->IsEnabled = 0;  
   }       
   
   return 0;                
}





char INTERNAL_VKB1_DrawMessageDisplay (int VKBHandle)
/*
    Questa routine provvede a creare il display che conterrà il MessageToUser
    che è stato passato alla tastiera.
*/
{
    unsigned short int RectX1, RectY1, RectX2, RectY2;
    unsigned short int LenX;
    unsigned short int NrCharsVisualizable;
    unsigned short int MargineX;
    
    char StringBuffer [255];
    
    struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
   
    RectX1 = 2;
    RectY1 = 2;
    RectX2 = 218;
    RectY2 = 13;
    
    LenX = RectX2 - RectX1;
    NrCharsVisualizable = LenX/8;
    
    ndWS_DrawRectangle (RectX1, RectY1, RectX2, RectY2, COLOR_GRAY07, COLOR_BLACK, KeybData->WndHandle, NORENDER);

    ndStrCpy (StringBuffer, KeybData->MessageToUser, NrCharsVisualizable, 0);
    
    MargineX = (LenX - strlen (StringBuffer)*8)/2;
    
    ndWS_GrWriteLn (RectX1+MargineX, RectY1+2, StringBuffer, COLOR_GRAY10, COLOR_GRAY07, KeybData->WndHandle, NORENDER);

    return 0;
}


char INTERNAL_CapsLockDisplay (int VKBHandle, char CapsLockOn)
{
    unsigned char GrElem_CAPS [8][40] =
    {
      ".......................",
      "..WWWW.WWWW.WWWW.WWWW..",
      "..W....W..W.W..W.W.....",
      "..W....WWWW.WWWW.WWWW..",
      "..W....W..W.W.......W..",
      "..WWWW.W..W.W....WWWW..",
      "......................."
    };

    struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
    
    unsigned short int RectX1, RectY1, RectX2, RectY2;
    unsigned short int RelPosX, RelPosY;

    char CounterX, CounterY;
    
    RectX1 = 220;
    RectY1 = 2;
    RectX2 = 249;
    RectY2 = 13;
    
    RelPosX = 4;
    RelPosY = 2;
    
    ndWS_DrawRectangle (RectX1, RectY1, RectX2, RectY2, COLOR_GRAY14, COLOR_GRAY07, KeybData->WndHandle, NORENDER);

    if (CapsLockOn)
    {
       for (CounterY=0; CounterY<7; CounterY++)
       {
           for (CounterX=0; CounterX<40; CounterX++)
           {
               if (GrElem_CAPS [CounterY][CounterX]=='W')
                  ndWS_PutPixel (RectX1+RelPosX+CounterX, RectY1+RelPosY+CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           }
       }
    } 
    
    XWindowRender (KeybData->WndHandle);
    return 0;
}


char INTERNAL_FuncLockDisplay (int VKBHandle, char FuncLockOn)
{
    unsigned char GrElem_FUNC [8][40] =
    {
      ".......................",
      "..WWWW.W..W.WW.W.WWWW..",
      "..W....W..W.W.WW.W.....",
      "..WWWW.W..W.W..W.W.....",
      "..W....W..W.W..W.W.....",
      "..W....WWWW.W..W.WWWW..",
      "......................."
    };

    struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
    
    unsigned short int RectX1, RectY1, RectX2, RectY2;
    unsigned short int RelPosX, RelPosY;
    
    char CounterX, CounterY;
    
    RectX1 = 251;
    RectY1 = 2;
    RectX2 = 280;
    RectY2 = 13;
    
    RelPosX = 4;
    RelPosY = 2;
    
    ndWS_DrawRectangle (RectX1, RectY1, RectX2, RectY2, COLOR_GRAY14, COLOR_GRAY07, KeybData->WndHandle, NORENDER);

    if (FuncLockOn)
    {
       for (CounterY=0; CounterY<7; CounterY++)
       {
           for (CounterX=0; CounterX<40; CounterX++)
           {
               if (GrElem_FUNC [CounterY][CounterX]=='W')
                  ndWS_PutPixel (RectX1+RelPosX+CounterX, RectY1+RelPosY+CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           }
       }
    } 
    
    XWindowRender (KeybData->WndHandle);
    return 0;
}



char INTERNAL_VKB1_DrawFirstButtons (int VKBHandle, ndint64 KeyOptions)
/*
    Questa routine provvede a disegnare la prima fila di tasti della tastiera 
    virtuale
*/

{
   struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
     
   unsigned short int BasePosX,    BasePosY;
   unsigned short int BaseSecPosX, BaseSecPosY; 
   unsigned short int KeyPosX1,    KeyPosY1;
   unsigned short int KeyPosX2,    KeyPosY2;
   
   int Counter, CounterX, CounterY;
   
   char SymbolString0 [20]="1234567890?^é";
   char SymbolString1 [20]="! £$%&à()='ìè";
   
   
   char __ButtonID [16];     // Destinato a contenere il ButtonID
   char __Label1   [16];     // Destinato a contenere la Label1 per il pulsante
   char __Label2   [16];     // Destinato a contenere la Label2 per il pulsante
   
   int ButtonHandle;
   
   BasePosX = 2;
   BasePosY = 17;
   
   // Inserisci le virgolette
   
   SymbolString1 [1]=34;
   
   // Attenzione alla StringID. Questa stringa sarà sfruttata
   // dal gestore della tastiera per individuare quale tasto è stato premuto:
   // la struttura della stringa è la seguente: i primi 3 caratteri sono
   // posti a VKB, seguiti da 7 caratteri che contengono codici come ENTER,
   // DEL, CANC, SPACE nel caso di tasti speciali, oppure ------- nel
   // caso di tasti standard, oppure -KEY--- nel caso di tasti doppi
        
   // Subito dopo questo gruppo di caratteri, vi sono due byte che indicano
   // il carattere principale e poi il carattere secondario, seguiti da
   // un carattere NUL di fine stringa. 
   
   // TASTO ESC
   
   
   char GrElem_ESC [12][40];
   
   strcpy (GrElem_ESC [ 0], "                         ");
   strcpy (GrElem_ESC [ 1], " xxxxx xxxxx xxxxx       ");
   strcpy (GrElem_ESC [ 2], " x     x     x           ");
   strcpy (GrElem_ESC [ 3], " xxxx  xxxxx x           ");
   strcpy (GrElem_ESC [ 4], " x         x x           ");
   strcpy (GrElem_ESC [ 5], " xxxxx xxxxx xxxxx       ");
   strcpy (GrElem_ESC [ 6], "                         ");
   
   
   // Inizia a preparare i dati per il pulsante ESC
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-ESC---");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
    
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 26;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY12, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici pulsante ESC
   
   BaseSecPosX = 3;
   BaseSecPosY = 3;
   
   for (CounterY=0; CounterY<6; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_ESC [CounterY][CounterX]=='x')
               ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   // Disegniamo la prima serie di tasti
   
   BasePosX = BasePosX+28;
   
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   for (Counter=0; Counter<10; Counter++)            // Gruppo 0..9
   {
        KeyPosX1 = BasePosX + BaseSecPosX + 16*(Counter);
        KeyPosY1 = BasePosY + BaseSecPosY;
        KeyPosX2 = BasePosX + BaseSecPosX + 16*(Counter+1) - 2;
        KeyPosY2 = BasePosY + BaseSecPosY + 22;
        
        // Inizia a creare la StringID per il pulsante. 
        
        strcpy (__ButtonID, "VKB");
        strcat (__ButtonID, "-KEY---");
        
        __ButtonID [10] = SymbolString1 [Counter];
        __ButtonID [11] = SymbolString0 [Counter];
        __ButtonID [12] = 0;                          // Terminatore stringa
        
        // Prepariamo la label 1
        
        __Label1 [0] = SymbolString0 [Counter];
        __Label1 [1] = 0;
        
        // Prepariamo la label 2
        
        __Label2 [0] = SymbolString1 [Counter];
        __Label2 [1] = 0;
        
    
        ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_LGREEN, COLOR_GRAY08, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
    
        if (ButtonHandle<0) goto ErrorCondition1;
   } 
   
   
   
   for (Counter=10; Counter<13; Counter++)        // Altri tasti della fila
   {
        KeyPosX1 = BasePosX + BaseSecPosX + 16*(Counter);
        KeyPosY1 = BasePosY + BaseSecPosY;
        KeyPosX2 = BasePosX + BaseSecPosX + 16*(Counter+1) - 2;
        KeyPosY2 = BasePosY + BaseSecPosY + 22;
        
        // Inizia a creare la StringID per il pulsante. 
        
        strcpy (__ButtonID, "VKB");
        strcat (__ButtonID, "-KEY---");
        
        __ButtonID [10] = SymbolString1 [Counter];
        __ButtonID [11] = SymbolString0 [Counter];
        __ButtonID [12] = 0;                          // Terminatore stringa
        
        // Prepariamo la label 1
        
        __Label1 [0] = SymbolString0 [Counter];
        __Label1 [1] = 0;
        
        // Prepariamo la label 2
        
        __Label2 [0] = SymbolString1 [Counter];
        __Label2 [1] = 0;
        
    
        ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY08, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
    
        if (ButtonHandle<0) goto ErrorCondition1;
   } 
   
   // TASTO DEL
   
   char GrElem_DEL [12][40];
   
   strcpy (GrElem_DEL [ 0], "                         ");
   strcpy (GrElem_DEL [ 1], " xxxx  xxxxx x           ");
   strcpy (GrElem_DEL [ 2], " x   x x     x           ");
   strcpy (GrElem_DEL [ 3], " x   x xxxx  x           ");
   strcpy (GrElem_DEL [ 4], " x   x x     x           ");
   strcpy (GrElem_DEL [ 5], " xxxx  xxxxx xxxxx       ");
   strcpy (GrElem_DEL [ 6], "                         ");
   
   
   BasePosX = BasePosX+16*13;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-DEL---");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 26;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY12, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
  // Disegno elementi grafici per il pulsante DEL
   
   BaseSecPosX = 3;
   BaseSecPosY = 3;
   
   for (CounterY=0; CounterY<6; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_DEL [CounterY][CounterX]=='x')
               ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
  
  
  
  
  // TASTO OK
   
   char GrElem_OK [12][40];
   
   strcpy (GrElem_OK [ 0], "                         ");
   strcpy (GrElem_OK [ 1], "  xxx  x   x             ");
   strcpy (GrElem_OK [ 2], " x   x x  x              ");
   strcpy (GrElem_OK [ 3], " x   x xxx               ");
   strcpy (GrElem_OK [ 4], " x   x x  x              ");
   strcpy (GrElem_OK [ 5], "  xxx  x   x             ");
   strcpy (GrElem_OK [ 6], "                         ");
   
   
   BasePosX = BasePosX+29;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-OK---");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 16 - 2;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_RED, COLOR_BLACK, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici per il pulsante OK
   
   BaseSecPosX = 1;
   BaseSecPosY = 8;
   
   for (CounterY=0; CounterY<6; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_OK [CounterY][CounterX]=='x')
               ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_WHITE, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   return 0;
   
   ErrorCondition1:    // C'è stato un problema nella creazione di un pulsante
   
   return VKB1_ERR_I_CANNOT_CREATE_BUTTONS;

   
}



char INTERNAL_VKB1_DrawSecondButtons (int VKBHandle, ndint64 KeyOptions)
/*
    Questa routine provvede a disegnare la seconda fila di tasti della tastiera 
    virtuale
*/

{
   struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
   
   unsigned short int BasePosX,    BasePosY;
   unsigned short int BaseSecPosX, BaseSecPosY; 
   unsigned short int KeyPosX1,    KeyPosY1;
   unsigned short int KeyPosX2,    KeyPosY2;
   
   int Counter, CounterX, CounterY;
   
   //char SymbolString0 [20]="~       |\*{}§";
   //char SymbolString1 [20]="QWERTYUIOP+[]ù";
   
   char SymbolString0 [20]="~       |*+-[";
   char SymbolString1 [20]="QWERTYUIOP{}]";
   
   
   char __ButtonID [16];     // Destinato a contenere il ButtonID
   char __Label1   [16];     // Destinato a contenere la Label1 per il pulsante
   char __Label2   [16];     // Destinato a contenere la Label2 per il pulsante
   
   int ButtonHandle;
   
   BasePosX = 2;
   BasePosY = 17+24;
   
   
   // Attenzione alla StringID. Questa stringa sarà sfruttata
   // dal gestore della tastiera per individuare quale tasto è stato premuto:
   // la struttura della stringa è la seguente: i primi 3 caratteri sono
   // posti a VKB, seguiti da 7 caratteri che contengono codici come ENTER,
   // DEL, CANC, SPACE nel caso di tasti speciali, oppure ------- nel
   // caso di tasti standard, oppure -KEY--- nel caso di tasti doppi
        
   // Subito dopo questo gruppo di caratteri, vi sono due byte che indicano
   // il carattere principale e poi il carattere secondario, seguiti da
   // un carattere NUL di fine stringa. 
   
   // TASTO TAB
   
   
   char GrElem_TAB [12][40];
   
   strcpy (GrElem_TAB [ 0], "                         ");
   strcpy (GrElem_TAB [ 1], " xxxxx xxxxx xxxx        ");
   strcpy (GrElem_TAB [ 2], "   x   x   x x   x       ");
   strcpy (GrElem_TAB [ 3], "   x   xxxxx xxxx        ");
   strcpy (GrElem_TAB [ 4], "   x   x   x x   x       ");
   strcpy (GrElem_TAB [ 5], "   x   x   x xxxx        ");
   strcpy (GrElem_TAB [ 6], "                         ");
   
   // Inizia a preparare i dati per il pulsante TAB
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "--TAB--");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
    
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 27;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY12, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici pulsante TAB
   
   BaseSecPosX = 3;
   BaseSecPosY = 3;
   
   for (CounterY=0; CounterY<6; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_TAB [CounterY][CounterX]=='x')
               ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
  
   
   // Disegniamo la seconda serie di tasti
   
   BasePosX = BasePosX+29;
   
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   
   for (Counter=0; Counter<13; Counter++)
   {
        KeyPosX1 = BasePosX + BaseSecPosX + 16*(Counter);
        KeyPosY1 = BasePosY + BaseSecPosY;
        KeyPosX2 = BasePosX + BaseSecPosX + 16*(Counter+1) - 2;
        KeyPosY2 = BasePosY + BaseSecPosY + 22;
        
        // Inizia a creare la StringID per il pulsante. 
        
        strcpy (__ButtonID, "VKB");
        strcat (__ButtonID, "-KEY---");
        
        __ButtonID [10] = SymbolString1 [Counter];
        __ButtonID [11] = SymbolString0 [Counter];
        __ButtonID [12] = 0;                          // Terminatore stringa
        
        // Prepariamo la label 1
        
        __Label1 [0] = SymbolString0 [Counter];
        __Label1 [1] = 0;
        
        // Prepariamo la label 2
        
        __Label2 [0] = SymbolString1 [Counter];
        __Label2 [1] = 0;
        
    
        ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY08, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
    
        if (ButtonHandle<0) goto ErrorCondition1;
   } 
   
   // Disegnamo il tasto CANC
   
   char GrElem_CANC [12][40];
   
   strcpy (GrElem_CANC [ 0], "                         ");
   strcpy (GrElem_CANC [ 1], " xxxx xxxx x  x xxxx     ");
   strcpy (GrElem_CANC [ 2], " x    x  x xx x x        ");
   strcpy (GrElem_CANC [ 3], " x    xxxx x xx x        ");
   strcpy (GrElem_CANC [ 4], " x    x  x x  x x        ");
   strcpy (GrElem_CANC [ 5], " xxxx x  x x  x xxxx     ");
   strcpy (GrElem_CANC [ 6], "                         ");
   
   
   BasePosX = BasePosX+16*13;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-CANC--");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 26;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY12, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici per il pulsante CANC
   
   BaseSecPosX = 3;
   BaseSecPosY = 3;
   
   
   for (CounterY=0; CounterY<6; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_CANC [CounterY][CounterX]=='x')
               ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   // TASTO PAGUP
   
   char GrElem_PGUP [18][40];
   
   strcpy (GrElem_PGUP [ 0], " xxx xxx                 ");
   strcpy (GrElem_PGUP [ 1], " x x x                   ");
   strcpy (GrElem_PGUP [ 2], " xxx x                   ");
   strcpy (GrElem_PGUP [ 3], " x   x x                 ");
   strcpy (GrElem_PGUP [ 4], " x   xxx                 ");
   strcpy (GrElem_PGUP [ 5], "                         ");
   strcpy (GrElem_PGUP [ 6], "                         ");
   strcpy (GrElem_PGUP [ 7], "    x                    ");
   strcpy (GrElem_PGUP [ 8], "   xxx                   ");
   strcpy (GrElem_PGUP [ 9], "  xxxxx                  ");
   strcpy (GrElem_PGUP [10], "    x                    ");
   strcpy (GrElem_PGUP [11], "    x                    ");
   strcpy (GrElem_PGUP [12], "    x                    ");
   strcpy (GrElem_PGUP [13], "    x                    ");
   strcpy (GrElem_PGUP [14], "    x                    ");
   strcpy (GrElem_PGUP [15], "    x                    ");
   
   
   BasePosX = BasePosX+28;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-PAGUP-");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 16 - 2;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_LBLUE, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
  
   
   // Disegno elementi grafici per il pulsante PGUP
   
   BaseSecPosX = 3;
   BaseSecPosY = 3;
   
   for (CounterY=0; CounterY<16; CounterY++)
   {
       for (CounterX=0; CounterX<11; CounterX++)
       {
           if (GrElem_PGUP [CounterY][CounterX]=='x')
               ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
  
   
   
   
   return 0;
   
   
   
   
   
   
   ErrorCondition1:    // C'è stato un problema nella creazione di un pulsante
   
   return VKB1_ERR_I_CANNOT_CREATE_BUTTONS;
   
   
}



char INTERNAL_VKB1_DrawThirdButtons (int VKBHandle, ndint64 KeyOptions)
/*
    Questa routine provvede a disegnare la terza fila di tasti della tastiera 
    virtuale
*/

{  
   struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
   
   unsigned short int BasePosX,    BasePosY;
   unsigned short int BaseSecPosX, BaseSecPosY; 
   unsigned short int KeyPosX1,    KeyPosY1;
   unsigned short int KeyPosX2,    KeyPosY2;
   
   int Counter, CounterX, CounterY;
   
   char SymbolString0 [20]="         ç #";
   char SymbolString1 [20]="ASDFGHJKLò/@";
   
   // Carattere slash inverso
   SymbolString0 [10]=92;
   
   char __ButtonID [16];     // Destinato a contenere il ButtonID
   char __Label1   [16];     // Destinato a contenere la Label1 per il pulsante
   char __Label2   [16];     // Destinato a contenere la Label2 per il pulsante
   
   int ButtonHandle;
   
   BasePosX = 2;
   BasePosY = 17+24+24;
   
   
   // Attenzione alla StringID. Questa stringa sarà sfruttata
   // dal gestore della tastiera per individuare quale tasto è stato premuto:
   // la struttura della stringa è la seguente: i primi 3 caratteri sono
   // posti a VKB, seguiti da 7 caratteri che contengono codici come ENTER,
   // DEL, CANC, SPACE nel caso di tasti speciali, oppure ------- nel
   // caso di tasti standard, oppure -KEY--- nel caso di tasti doppi
    
   
        
   // Subito dopo questo gruppo di caratteri, vi sono due byte che indicano
   // il carattere principale e poi il carattere secondario, seguiti da
   // un carattere NUL di fine stringa. 
   
   // TASTO CAPS
   
   
   char GrElem_CAPS [12][40];
   
   strcpy (GrElem_CAPS [ 0], "                        ");
   strcpy (GrElem_CAPS [ 1], " xxxx xxxxx xxxx   xxxx ");
   strcpy (GrElem_CAPS [ 2], "x     x   x x   x x     ");
   strcpy (GrElem_CAPS [ 3], "x     xxxxx xxxx   xxx  ");
   strcpy (GrElem_CAPS [ 4], "x     x   x x         x ");
   strcpy (GrElem_CAPS [ 5], " xxxx x   x x     xxxx  ");
   strcpy (GrElem_CAPS [ 6], "                        ");
   
   // Inizia a preparare i dati per il pulsante CAPS
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-CAPS--");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
    
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 28;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY12, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici pulsante CAPS
   
   BaseSecPosX = 3;
   BaseSecPosY = 3;
   
   for (CounterY=0; CounterY<6; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_CAPS [CounterY][CounterX]=='x')
               ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
  
   
   
   // Disegniamo la terza serie di tasti
   
   BasePosX = BasePosX+30;
   
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   
   for (Counter=0; Counter<12; Counter++)
   {
        KeyPosX1 = BasePosX + BaseSecPosX + 16*(Counter);
        KeyPosY1 = BasePosY + BaseSecPosY;
        KeyPosX2 = BasePosX + BaseSecPosX + 16*(Counter+1) - 2;
        KeyPosY2 = BasePosY + BaseSecPosY + 22;
        
        // Inizia a creare la StringID per il pulsante. 
        
        strcpy (__ButtonID, "VKB");
        strcat (__ButtonID, "-KEY---");
        
        __ButtonID [10] = SymbolString1 [Counter];
        __ButtonID [11] = SymbolString0 [Counter];
        __ButtonID [12] = 0;                          // Terminatore stringa
        
        // Prepariamo la label 1
        
        __Label1 [0] = SymbolString0 [Counter];
        __Label1 [1] = 0;
        
        // Prepariamo la label 2
        
        __Label2 [0] = SymbolString1 [Counter];
        __Label2 [1] = 0;
        
    
        ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY08, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
    
        if (ButtonHandle<0) goto ErrorCondition1;
   } 
   
   // TASTO ENTER
   
   char GrElem_ENTER [12][40];
   
   strcpy (GrElem_ENTER [ 0], "                         ");
   strcpy (GrElem_ENTER [ 1], "            x            ");
   strcpy (GrElem_ENTER [ 2], "            x            ");
   strcpy (GrElem_ENTER [ 3], "            x            ");
   strcpy (GrElem_ENTER [ 4], "     x      x            ");
   strcpy (GrElem_ENTER [ 5], "    xx      x            ");
   strcpy (GrElem_ENTER [ 6], "   xxx      x            ");
   strcpy (GrElem_ENTER [ 7], "  xxxxxxxxxxx            ");
   strcpy (GrElem_ENTER [ 8], "   xxx                   ");
   strcpy (GrElem_ENTER [ 9], "    xx                   ");
   strcpy (GrElem_ENTER [10], "     x                   ");
   
   BasePosX = BasePosX+16*12;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-ENTER-");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 20;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY12, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici per il pulsante ENTER
   
   BaseSecPosX = 3;
   BaseSecPosY = 6;
   
   
   for (CounterY=0; CounterY<=10; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_ENTER [CounterY][CounterX]=='x')
                 ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   
   // Tasto CURSOR UP
   
   char GrElem_CSRUP [12][40];
   
   strcpy (GrElem_CSRUP [ 0], "        x               ");
   strcpy (GrElem_CSRUP [ 1], "       xxx              ");
   strcpy (GrElem_CSRUP [ 2], "      xxxxx             ");
   strcpy (GrElem_CSRUP [ 3], "     xxxxxxx            ");
   strcpy (GrElem_CSRUP [ 4], "    xx  x  xx           ");
   strcpy (GrElem_CSRUP [ 5], "        x               ");
   strcpy (GrElem_CSRUP [ 6], "        x               ");
   strcpy (GrElem_CSRUP [ 7], "        x               ");
   strcpy (GrElem_CSRUP [ 8], "        x               ");
   strcpy (GrElem_CSRUP [ 9], "        x               ");
   strcpy (GrElem_CSRUP [10], "        x               ");
   
   BasePosX = BasePosX+22;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-CSRUP-");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 16;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_YELLOW, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici per il pulsante CSRUP
   
   BaseSecPosX = 0;
   BaseSecPosY = 6;
   
   
   for (CounterY=0; CounterY<=10; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_CSRUP [CounterY][CounterX]=='x')
                 ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   // TASTO PAGDW
   
   char GrElem_PGDW [18][40];
   
   strcpy (GrElem_PGDW [ 0], " xxx xxx                 ");
   strcpy (GrElem_PGDW [ 1], " x x x                   ");
   strcpy (GrElem_PGDW [ 2], " xxx x                   ");
   strcpy (GrElem_PGDW [ 3], " x   x x                 ");
   strcpy (GrElem_PGDW [ 4], " x   xxx                 ");
   strcpy (GrElem_PGDW [ 5], "                         ");
   strcpy (GrElem_PGDW [ 6], "                         ");
   strcpy (GrElem_PGDW [ 7], "    x                    ");
   strcpy (GrElem_PGDW [ 8], "    x                    ");
   strcpy (GrElem_PGDW [ 9], "    x                    ");
   strcpy (GrElem_PGDW [10], "    x                    ");
   strcpy (GrElem_PGDW [11], "    x                    ");
   strcpy (GrElem_PGDW [12], "    x                    ");
   strcpy (GrElem_PGDW [13], "  xxxxx                  ");
   strcpy (GrElem_PGDW [14], "   xxx                   ");
   strcpy (GrElem_PGDW [15], "    x                    ");
   
   
   BasePosX = BasePosX+21;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-PAGDW-");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 14;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_LBLUE, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
  
   
   // Disegno elementi grafici per il pulsante PGDW
   
   BaseSecPosX = 3;
   BaseSecPosY = 3;
   
   for (CounterY=0; CounterY<16; CounterY++)
   {
       for (CounterX=0; CounterX<11; CounterX++)
       {
           if (GrElem_PGDW [CounterY][CounterX]=='x')
               ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   
   return 0;
   
   ErrorCondition1:    // C'è stato un problema nella creazione di un pulsante
   
   return VKB1_ERR_I_CANNOT_CREATE_BUTTONS;

   
}


char INTERNAL_VKB1_DrawFourthButtons (int VKBHandle, ndint64 KeyOptions)
/*
    Questa routine provvede a disegnare la quarta fila di tasti della tastiera 
    virtuale
*/

{
   struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
    
   unsigned short int BasePosX,    BasePosY;
   unsigned short int BaseSecPosX, BaseSecPosY; 
   unsigned short int KeyPosX1,    KeyPosY1;
   unsigned short int KeyPosX2,    KeyPosY2;
   
   int Counter, CounterX, CounterY;
   
   char SymbolString0 [20]=">       ,._";
   char SymbolString1 [20]="<ZXCVBNM;:ù";
   
   char __ButtonID [16];     // Destinato a contenere il ButtonID
   char __Label1   [16];     // Destinato a contenere la Label1 per il pulsante
   char __Label2   [16];     // Destinato a contenere la Label2 per il pulsante
   
   int ButtonHandle;
   
   BasePosX = 2;
   BasePosY = 17+24+24+24;
   
   
   // Attenzione alla StringID. Questa stringa sarà sfruttata
   // dal gestore della tastiera per individuare quale tasto è stato premuto:
   // la struttura della stringa è la seguente: i primi 3 caratteri sono
   // posti a VKB, seguiti da 7 caratteri che contengono codici come ENTER,
   // DEL, CANC, SPACE nel caso di tasti speciali, oppure ------- nel
   // caso di tasti standard, oppure -KEY--- nel caso di tasti doppi
       
   // Subito dopo questo gruppo di caratteri, vi sono due byte che indicano
   // il carattere principale e poi il carattere secondario, seguiti da
   // un carattere NUL di fine stringa. 
   
   // TASTO FUNC
   
   char GrElem_FUNC [12][40];
   
   strcpy (GrElem_FUNC [ 0], "                         ");
   strcpy (GrElem_FUNC [ 1], " xxxxx x   x  xxxx        ");
   strcpy (GrElem_FUNC [ 2], " x     xx  x x            ");
   strcpy (GrElem_FUNC [ 3], " xxxx  x x x x            ");
   strcpy (GrElem_FUNC [ 4], " x     x  xx x            ");
   strcpy (GrElem_FUNC [ 5], " x     x   x  xxxx        ");
   strcpy (GrElem_FUNC [ 6], "                         ");
   strcpy (GrElem_FUNC [ 7], "                         ");
   strcpy (GrElem_FUNC [ 8], "                         ");
   strcpy (GrElem_FUNC [ 9], "                         ");
   strcpy (GrElem_FUNC [10], "                         ");
   
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-FUNC--");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 20;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_LGREEN, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici per il pulsante FUNC
   
   BaseSecPosX = 1;
   BaseSecPosY = 4;
   
   
   for (CounterY=0; CounterY<=10; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_FUNC [CounterY][CounterX]=='x')
                 ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   
   
   
   // Disegniamo la quarta serie di tasti
   
   BasePosX = BasePosX + 22;
   
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   
   for (Counter=0; Counter<11; Counter++)
   {
        KeyPosX1 = BasePosX + BaseSecPosX + 16*(Counter);
        KeyPosY1 = BasePosY + BaseSecPosY;
        KeyPosX2 = BasePosX + BaseSecPosX + 16*(Counter+1) - 2;
        KeyPosY2 = BasePosY + BaseSecPosY + 22;
        
        // Inizia a creare la StringID per il pulsante. 
        
        strcpy (__ButtonID, "VKB");
        strcat (__ButtonID, "-KEY---");
        
        __ButtonID [10] = SymbolString1 [Counter];
        __ButtonID [11] = SymbolString0 [Counter];
        __ButtonID [12] = 0;                          // Terminatore stringa
        
        // Prepariamo la label 1
        
        __Label1 [0] = SymbolString0 [Counter];
        __Label1 [1] = 0;
        
        // Prepariamo la label 2
        
        __Label2 [0] = SymbolString1 [Counter];
        __Label2 [1] = 0;
        
    
        ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY08, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
    
        if (ButtonHandle<0) goto ErrorCondition1;
   } 
   
   
   
   // TASTO SPACE
   
   BasePosX = BasePosX+16*11;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-SPACE-");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 26;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_GRAY12, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // TASTO CSR-SX
   
   char GrElem_CSRSX [12][40];
   
   strcpy (GrElem_CSRSX [ 0], "                        ");
   strcpy (GrElem_CSRSX [ 1], "      x                 ");
   strcpy (GrElem_CSRSX [ 2], "     xx                 ");
   strcpy (GrElem_CSRSX [ 3], "    xx                  ");
   strcpy (GrElem_CSRSX [ 4], "   xxx                  ");
   strcpy (GrElem_CSRSX [ 5], "  xxxxxxxxxxxx          ");
   strcpy (GrElem_CSRSX [ 6], "   xxx                  ");
   strcpy (GrElem_CSRSX [ 7], "    xx                  ");
   strcpy (GrElem_CSRSX [ 8], "     xx                 ");
   strcpy (GrElem_CSRSX [ 9], "      x                 ");
   strcpy (GrElem_CSRSX [10], "                        ");
   
   BasePosX = BasePosX+28;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-CSRSX-");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 16;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_YELLOW, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici per il pulsante CSRSX
   
   BaseSecPosX = 0;
   BaseSecPosY = 6;
   
   
   for (CounterY=0; CounterY<=10; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_CSRSX [CounterY][CounterX]=='x')
                 ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   // TASTO CSR-DW
   
   char GrElem_CSRDW [12][40];
   
   strcpy (GrElem_CSRDW [ 0], "        x               ");
   strcpy (GrElem_CSRDW [ 1], "        x               ");
   strcpy (GrElem_CSRDW [ 2], "        x               ");
   strcpy (GrElem_CSRDW [ 3], "        x               ");
   strcpy (GrElem_CSRDW [ 4], "        x               "); 
   strcpy (GrElem_CSRDW [ 5], "        x               ");
   strcpy (GrElem_CSRDW [ 6], "    xx  x  xx           ");
   strcpy (GrElem_CSRDW [ 7], "     xxxxxxx            ");
   strcpy (GrElem_CSRDW [ 8], "      xxxxx             ");
   strcpy (GrElem_CSRDW [ 9], "       xxx              ");
   strcpy (GrElem_CSRDW [10], "        x               ");
   
   BasePosX = BasePosX+18;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-CSRDW-");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 16;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_YELLOW, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici per il pulsante CSRDW
   
   BaseSecPosX = 0;
   BaseSecPosY = 6;
   
   
   for (CounterY=0; CounterY<=10; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_CSRDW [CounterY][CounterX]=='x')
                 ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   
   // TASTO CSR-DW
   
   char GrElem_CSRDX [12][40];
   
   strcpy (GrElem_CSRDX [ 0], "                        ");
   strcpy (GrElem_CSRDX [ 1], "         x              ");
   strcpy (GrElem_CSRDX [ 2], "          xx            ");
   strcpy (GrElem_CSRDX [ 3], "           xx           ");
   strcpy (GrElem_CSRDX [ 4], "           xxx          "); 
   strcpy (GrElem_CSRDX [ 5], "   xxxxxxxxxxxx         ");
   strcpy (GrElem_CSRDX [ 6], "           xxx          ");
   strcpy (GrElem_CSRDX [ 7], "           xx           ");
   strcpy (GrElem_CSRDX [ 8], "          xx            ");
   strcpy (GrElem_CSRDX [ 9], "         x              ");
   strcpy (GrElem_CSRDX [10], "                        ");
   
   BasePosX = BasePosX+18;
        
   BaseSecPosX = 0;
   BaseSecPosY = 0;
   
   strcpy (__ButtonID, "VKB");
   strcat (__ButtonID, "-CSRDX-");
        
   strcpy (__Label1, "");         // Prepariamo la label 1   
   strcpy (__Label2, "");         // Prepariamo la label 2
   
   
   KeyPosX1 = BasePosX + BaseSecPosX;
   KeyPosY1 = BasePosY + BaseSecPosY;
   KeyPosX2 = BasePosX + BaseSecPosX + 16;
   KeyPosY2 = BasePosY + BaseSecPosY + 22;
            
   ButtonHandle = ndCTRL_CreateButton (KeyPosX1, KeyPosY1, KeyPosX2, KeyPosY2, KeybData->WndHandle, __ButtonID, __Label1, __Label2, COLOR_WHITE, COLOR_YELLOW, COLOR_GRAY01, KeyOptions, INTERNAL_VKB1_StdCodeManager, VKBHandle, NORENDER);
  
   if (ButtonHandle<0) goto ErrorCondition1;
  
   // Disegno elementi grafici per il pulsante CSRDX
   
   BaseSecPosX = 0;
   BaseSecPosY = 6;
   
   
   for (CounterY=0; CounterY<=10; CounterY++)
   {
       for (CounterX=0; CounterX<30; CounterX++)
       {
           if (GrElem_CSRDX [CounterY][CounterX]=='x')
                 ndWS_PutPixel (BasePosX + BaseSecPosX + CounterX, BasePosY + BaseSecPosY + CounterY, COLOR_BLACK, KeybData->WndHandle, NORENDER);
           
       }
   }
   
   
   return 0;
   
   ErrorCondition1:    // C'è stato un problema nella creazione di un pulsante
   
   return VKB1_ERR_I_CANNOT_CREATE_BUTTONS;
   
   
}


















int INTERNAL_VKB1_DrawKeyboard (int VKBHandle, char CallbackBased)
/*
   Questa routine provvede a disegnare sullo schermo gli elementi grafici che compongono
   la tastiera virtuale. 
   
   Se l'operazione va a buon fine, viene restituito in uscita il valore 0, altrimenti
   viene restituito un codice di errore. 
*/

{
   ndint64 KeyOptions;
   int     Error;
   
   struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
   
    
   KeybData->WndHandle = ndLP_CreateWindow (KeybData->kb1.kbPosX1, KeybData->kb1.kbPosY1, 
                                              KeybData->kb1.kbPosX2, KeybData->kb1.kbPosY2, 
                                              "Virtual Keyboard", COLOR_WHITE, COLOR_BLUE, 
                                              COLOR_GRAY12, COLOR_WHITE, 0);
    
   if (KeybData->WndHandle>=0)    // Il processo di creazione della tastiera è andato a buon fine
   {
        if (!CallbackBased)  KeyOptions = ND_BUTTON_ROUNDED;
           else KeyOptions = ND_BUTTON_ROUNDED | ND_TRASPARENT_CALLBACK;
        
        ndLP_MaximizeWindow (KeybData->WndHandle);
        ndLP_PutWindowInFirstPlane (KeybData->WndHandle);
        
        INTERNAL_VKB1_DrawMessageDisplay (VKBHandle);
        INTERNAL_CapsLockDisplay (VKBHandle, KeybData->kb1.CapsLockOn);
        INTERNAL_FuncLockDisplay (VKBHandle, KeybData->kb1.FuncLockOn);
         
        Error = INTERNAL_VKB1_DrawFirstButtons (VKBHandle, KeyOptions); 
        if (Error) goto ConditionError1;
        
        Error = INTERNAL_VKB1_DrawSecondButtons (VKBHandle, KeyOptions); 
        if (Error) goto ConditionError1;
        
        Error = INTERNAL_VKB1_DrawThirdButtons (VKBHandle, KeyOptions); 
        if (Error) goto ConditionError1;
        
        Error = INTERNAL_VKB1_DrawFourthButtons (VKBHandle, KeyOptions); 
        if (Error) goto ConditionError1;
        
        XWindowRender (KeybData->WndHandle);
        
        // Se il sistema è arrivato qui, vuol dire che non ci sono errori. 
        // Segnalalo alla routine chiamante
        
        return 0;
   }
   else
      return VKB1_ERR_I_CANNOT_CREATE_WINDOW;
      
   ConditionError1:                          // C'è stato un errore nella creazione dei pulsanti
   
   ndLP_DestroyWindow (KeybData->WndHandle);    // Distruggendo la finestra, distruggerò anche eventuali
                                                    // pulsanti già creati
   return Error;
   

}












int INTERNAL_VKB1_StartKeyboard     (int VKBHandle, ndint64 KeyboardStyle, char CallbackBased)
/*
    Questa routine provvede a creare la tastiera tipo 0 sullo schermo. 
    
    Se il sistema non riscontra errori, restituisce in uscita il valore 0, segnalando
    alla routine ndHAL_VKB_AssignMeVirtualKeyboard che è riuscito il processo di
    creazione della tastiera. 
    
    In caso contrario, il sistema restituirà in uscita un codice di errore che poi
    diventerà il codice di errore secondario restituito da ndHAL_VKB_AssignMeKeyboard
    per segnalare alla routine chiamante qual è l'errore riscontrato. 
    
    Il parametro KeyboardStyle è lo stesso che era stato passato ad
    ndHAL_VKB_AssignMeVirtualKeyboard e permette di regolare il comportamento
    della tastiera. 
    
    Se non viene usata alcuna key per il parametro Keyboard Style, il sistema
    provvederà a determinare automaticamente la posizione ottimale per la
    tastiera facendo uso del Position Generator di Nanodesktop. 
    
    Se si vuole settare esplicitamente la posizione della tastiera, occorre
    passare come parametro Keyboard Style la combinazione NDKEY_POSX (x) |
    NDKEY_POSY (y) dove x ed y sono le coordinate del punto in alto a sinistra
    della tastiera.
    
    Sono previsti i seguenti codici di errore:
        VKB1_ERR_I_CANNOT_CREATE_WINDOW       Non posso allocare la finestra 
        VKB1_ERR_I_CANNOT_CREATE_BUTTONS      Non posso creare i pulsanti
*/

{
   struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);

   unsigned short int RPosX1, RPosY1, RPosX2, RPosY2; 
   int ReqPosX, ReqPosY;

   KeybData->kb1.CapsLockOn=1;
   KeybData->kb1.FuncLockOn=0;
   
   ReqPosX = (KeyboardStyle >> 11) & 0x7FF;
   ReqPosY =  KeyboardStyle        & 0x7FF;
   
   if ((ReqPosX!=0) && (ReqPosY!=0)) 
   {
       KeybData->kb1.kbPosX1 = (ReqPosX-1);           // Il codice Key è sempre maggiore di 1 rispetto
       KeybData->kb1.kbPosY1 = (ReqPosY-1);           // alla reale posizione richiesta
       KeybData->kb1.kbPosX2 = (ReqPosX-1)+298;
       KeybData->kb1.kbPosY2 = (ReqPosY-1)+140;
   }   
   else
   {
       ndPOSGEN_GenPosition (298, 140, &RPosX1, &RPosY1, &RPosX2, &RPosY2);
   
       KeybData->kb1.kbPosX1 = RPosX1;
       KeybData->kb1.kbPosY1 = RPosY1;
       KeybData->kb1.kbPosX2 = RPosX2;
       KeybData->kb1.kbPosY2 = RPosY2;
   }
   
   return INTERNAL_VKB1_DrawKeyboard (VKBHandle, CallbackBased);   
}


int INTERNAL_VKB1_BeginReadCharLoop (int VKBHandle, char CallbackBased, char AskOnlyAKey, char NoKeybAutoClose)
{ 
   struct VKB_InfoRec_Type *KeybData;  
   struct ndEvent_Type     MyEvent;
        
   int     AlphaField, BetaField;
   char    YouCanExit;
  
   ndint64 KeybIncloseInfoField;
             
   
   char (*PntEndCallback)(void *EndCallbackData, int VKBHandle, char WndHandle); 
   // Puntatore alla EndCallback
    
   void (*PntBreakCallback) (void *BreakCallbackData, int VKBHandle, char WndHandle);
   // Puntatore alla BreakCallback
   
    
   // Inizio del codice
   
   KeybData = &(SysKeyboardsRegister [VKBHandle]);  // Registra l'indirizzo dell'handle  
   KeybData->IsEnabled = 1;                         // Impegna l'handle
             
   PntEndCallback    = KeybData->EndCallback;       // Prepara il puntatore alla end callback
   PntBreakCallback  = KeybData->BreakCallback;     // Prepara il puntatore alla break callback
     
   if (!CallbackBased)            // Non è stata richiesta una tastiera callback based
   {
             YouCanExit = 0;
             
             while (!YouCanExit)
             {
                 ndLP_PutWindowInFirstPlane (KeybData->WndHandle);
                 MouseControl (&MyEvent);  
                 
                 if (AskOnlyAKey)
                 {
                     if (KeybData->NrCharsRead==1)       
                        YouCanExit=1;            
                 }
                 else   // Non stiamo usando la modalità AskOnlyAKey
                 {
                     if (PntEndCallback!=0)
                     {
                        // Usa la EndCallback per verificare se è necessario uscire dal loop               
                                           
                        if ((*PntEndCallback)(KeybData->EndCallbackData, VKBHandle, KeybData->WndHandle))
                        {
                           YouCanExit=1;
                        }
                     }
                     
                     if ((MyEvent.__TypeEvent==A_WINDOW_WANTS_TO_BE_CLOSED) && (MyEvent.__WindowID==KeybData->WndHandle))
                     {   
                        
                        if (PntBreakCallback!=0)
                        {
                           // Esegui l'eventuale BreakCallback
                           (*PntBreakCallback) (KeybData->BreakCallbackData, VKBHandle, KeybData->WndHandle);
                        }
                        
                        // Ritardo necessario per evitare pressioni multiple
                        ndHAL_RequestDelay (ND_REQDELAY_CLOSE_VKB);
                        
                        // Termina il loop
                        YouCanExit=1;
                     } 
                       
                 }
             }
             
             AlphaField = VKBHandle;
             BetaField  = (unsigned short int)(KeybData->NrCharsRead); 
             
             if (!NoKeybAutoClose)        // Provvedi all'AutoClose se necessario
             {
                 ndLP_DestroyWindow (KeybData->WndHandle);
                 KeybData->IsEnabled = 0;                     // Disimpegna l'handle
             }
             
             return (+1)*(256*BetaField + AlphaField);
   }
   else                           // E' stata richiesta una tastiera callback based
   {
             ndLP_PutWindowInFirstPlane (KeybData->WndHandle);
             
             // Provvedi a settare una WndExitCallback che chiude automaticamente
             // la tastiera. Prepara il campo KeybIncloseInfoField
        
             KeybIncloseInfoField = VKBHandle;
             if (NoKeybAutoClose) KeybIncloseInfoField = KeybIncloseInfoField | IFAC_NO_KEYB_AUTOCLOSE;
             
             // Setup della callback
        
             ndLP_SetupCloseWndCallback (&INTERNAL_VKB1_AutoCloseKeyboard, KeybIncloseInfoField, 1, KeybData->WndHandle);
            
             // Può proseguire nel processo di setup
             
             AlphaField = VKBHandle;
             BetaField  = 0;                          // Nessun tasto premuto
             return (256*BetaField+AlphaField);       // Restituisce il codice di uscita necessario              
   } 
}










char INTERNAL_VKB1_DisplayInVKeyboard (char *NewMessageToUser, int VKBHandle, char RenderNow)
{
    int RectX1, RectY1, RectX2, RectY2;
    int LenX;
    int NrCharsVisualizable;
    int MargineX;
    
    char StringBuffer [255];
    struct VKB_InfoRec_Type *KeybData = &(SysKeyboardsRegister [VKBHandle]);
   
    RectX1 = 2;
    RectY1 = 2;
    RectX2 = 218;
    RectY2 = 13;
    
    LenX = RectX2 - RectX1;
    NrCharsVisualizable = LenX/8;
    
    ndWS_DrawRectangle (RectX1, RectY1, RectX2, RectY2, COLOR_GRAY07, COLOR_BLACK, KeybData->WndHandle, NORENDER);

    ndStrCpy (StringBuffer, NewMessageToUser, NrCharsVisualizable, 0);
    
    MargineX = (LenX - strlen (StringBuffer)*8)/2;
    
    if (MargineX>=0)
    {
       ndWS_GrWriteLn (RectX1+MargineX, RectY1+2, StringBuffer, COLOR_GRAY10, COLOR_GRAY07, KeybData->WndHandle, NORENDER);
       if (RenderNow) XWindowRender (KeybData->WndHandle);
    }

    return 0; 
} 



char INTERNAL_VKB1_DestroyKeyboard (int VKBHandle)
{
    ndLP_DestroyWindow (SysKeyboardsRegister [VKBHandle].WndHandle);
    SysKeyboardsRegister [VKBHandle].IsEnabled = 0; 
         
    return 0; 
}


