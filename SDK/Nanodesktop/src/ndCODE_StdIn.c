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
    GESTORE STD-IN
    ----------------------------------------------------------------------------
    Questo modulo contiene le routines usate dalla libreria NanoC per emulare
    il comportamento di uno stream std-in standard. La necessità di tale
    emulazione nasce dal fatto che alcune piattaforme supportate da Nanodesktop,
    come PSP, non possiedono una tastiera e quindi devono emulare lo stream
    std-in per mezzo di routines opportune.
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void ndInitStdInSystem ()
{
    SystemKeyboardType      = 0;
    SystemKeyboardAutoClose = 1;
    
    SystemKeyboardEnabled   = 0;
    SystemKeyboardHandle    = 0;     
}


void ndSET_SystemKeyboardAutoClose (char Value)
/* Permette di settare a 0 o a 1 la variabile SystemKeyboard AutoClose. Quando
   SystemKeyboard AutoClose è posto a 1, la funzione ndGetChar chiude la finestra
   della Virtual Keyboard dopo avere ricevuto un singolo tasto.
   
   Al contrario, quando SystemKeyboardAutoClose viene posto a 0, l'AutoClose non
   viene eseguito ed è l'utente che deve provvedere con un ndCloseSystemKeyboard.
*/
{
   SystemKeyboardAutoClose = Value;
}


char ndSET_SystemKeyboardType (int Type)
/*
   Questa routine permette di settare il tipo di tastiera che deve essere visualizzata
   quando viene usata una funzione della libc. La routine non ha effetto sulla tastiera
   usata dai FileManager, che continua ad essere la tastiera 0. 
   
   Se Type corrisponde ad un tipo di tastiera non implementata, la routine restituisce
   il codice di errore:
             
   ERR_WRONG_PARAMETERS          -
*/

{  
   if (Type==0)
   {
      SystemKeyboardType = Type;
      return 0;
   }
   else
      return ERR_WRONG_PARAMETERS;      
}

 
 
void INTERNAL_STDIO_KeyCallback (int KeyPressedCode, void *AreaAddr, int VKBHandle, char WndHandle)  
/*
   Questa routine è la KeyCallback usata da ndGetChar: la sua funzione è quella di
   convertire i codici a 32 bit KeyPressedCode in codici ASCII a 8 bit che possano
   poi essere usati dalle routines libc ANSI della libreria NanoC.
*/
{
    struct        StdInDataArea_Type  *StdInData = AreaAddr;
    unsigned char HiCode, LoCode;
                
    StdInData->KeyPressedCode = 0;
    StdInData->AsciiCode      = 0;
    
    // Trovi i codici Hi e Lo
    
    KeyPressedCode = KeyPressedCode & 0xFFFF;       // Considera solo i 16 bit meno significativi
     
    HiCode = KeyPressedCode / 256;
    LoCode = KeyPressedCode % 256;
    
    if (HiCode==0)          // Tasto standard
    {
         StdInData->AsciiCode = LoCode;
    }
    
    if (HiCode==ND_GENERIC_SPECIALKEY)       // Premuto un tasto funzione
    {
         switch (LoCode)
         {
                case 0x10:     // BackSpace
                {
                     StdInData->AsciiCode = 8;
                     break;
                }
                
                case 0x20:     // Enter
                {
                     StdInData->AsciiCode = '\n';
                     break;
                }
                
                case 0x21:     // Tasto OK premuto: viene interpretato come ENTER da STDIN
                {
                     StdInData->AsciiCode = '\n';
                     break;
                }
                
                case 0xFF:     // Tasto ESC premuto
                {
                     StdInData->AsciiCode = 27;
                     break;
                }
                
                default:                  // Altri tasti funzione: la routine restituisce FF
                {
                     StdInData->AsciiCode = 0xFF;
                }                                 
         }                   
    }  
    
    return;
}
      

int ndGetChar ()
/*
     Questa routine viene usata dalla libreria NanoC per ottenere dei caratteri ASCII a 8 bit
     attraverso una tastiera virtuale di Nanodesktop. 
     
     Qualora venga premuto un tasto funzione, la routine assegna un codice ASCII fittizio che
     indica il tasto funzione: 8 per Backspace, \n per ENTER, 27 per ESC, e 255 per tutti
     gli altri tasti funzione (questa soluzione è solo temporanea).
     
     Se si verifica un'errore nella creazione della Virtual Keyboard, la routine riporterà in 
     uscita il valore negativo -1.
*/
{
    struct StdInDataArea_Type StdInData;
    
    int    KeybCodeExit;
    int    AlphaField, BetaField;
    int    VKBHandle;
    
    // Attiviamo la Keyboard di sistema
    
    if (!SystemKeyboardEnabled)         // La tastiera di sistema non è attiva: si deve provare ad allocarla
    {
            
            KeybCodeExit = ndHAL_VKB_AssignMeKeyboard  ("ANSI STD-IN Keyboard", SystemKeyboardType, 
                                                         ND_ASK_ONLY_A_KEY | ND_KEYBOARD_NO_AUTOCLOSE, 0,
                                                         &INTERNAL_STDIO_KeyCallback, &StdInData, 0, 0, 0, 0);                                                     
    }
    else                                // La tastiera di sistema già era attiva: riutilizza quella
    {
            KeybCodeExit = ndHAL_VKB_ReUseAnOpenedKeyboard (ND_ASK_ONLY_A_KEY | ND_KEYBOARD_NO_AUTOCLOSE, 
                                                             SystemKeyboardHandle);
    }
 
 
    // A questo punto la keyboard di sistema è attivata 
                                      
    if (KeybCodeExit>=0)                                // Non ci sono stati errori. 
    {
         AlphaField = KeybCodeExit & 0xFF;              // Contiene l'handle della tastiera
         BetaField  = (KeybCodeExit >> 8) & 0xFFFFFF;   // Contiene il numero di caratteri letti
         
         if (SystemKeyboardAutoClose)
         {
             VKBHandle = AlphaField;                    // Otteniamo l'handle della tastiera
             ndHAL_VKB_DestroyKeyboard (VKBHandle);     // Distruggi la tastiera
             
             SystemKeyboardEnabled = 0;
         }
         else        // Modalità SystemKeyboardAutoClose disattivata: non dobbiamo "chiudere" la tastiera
         {           // ma dobbiamo solo provvedere a salvarne l'handle
                     
             VKBHandle = AlphaField; 
             SystemKeyboardHandle  = VKBHandle;       
             
             SystemKeyboardEnabled = 1;
         }
         
         return StdInData.AsciiCode;
    }
    else                             // Ci sono stati degli errori nell'allocazione della tastiera
         return (-1);                // Segnala l'errore alla routine chiamante    
    
}


void ndCloseSystemKeyboard ()
/*
     Provvede a chiudere la tastiera di sistema. 
     Usato dalla libreria NanoC.
*/
{
    if (SystemKeyboardEnabled) 
    {
        ndHAL_VKB_DestroyKeyboard (SystemKeyboardHandle);
        SystemKeyboardEnabled = 0;
    }
    
    return;
}
