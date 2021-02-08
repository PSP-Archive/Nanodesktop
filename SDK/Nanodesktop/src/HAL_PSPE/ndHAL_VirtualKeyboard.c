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
    API GENERALE VIRTUAL KEYBOARD (PSPE)
    ---------------------------------------------------------------------------
    Questa API si occupa della gestione delle tastiere virtuali ad alto livello.
    
    In questo file non sono presenti le routine relative ai vari specifici tipi
    di tastiera: per queste ultime, vedere i file ndHAL_Keyboard_1.c o
    successive
    ---------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>

void ndHAL_InitKeyboards (void)
{
     int Counter;
     
     for (Counter=0; Counter<__NRVIRTUALKEYBOARDS_ALLOWED; Counter++)
     {
        SysKeyboardsRegister [Counter].IsEnabled = 0; 
     }
     
     ndDefaultKeybType = ND_VIDEO_KEYBOARD;
}

char ndHAL_VKB_ThisKeybTypeIsAvailable (int Type)
/*
    Questa routine restituisce 1 se una tastiera del tipo indicato è supportata
    dalla piattaforma su cui gira Nanodesktop e 0 in caso contrario
*/
{
    switch (Type)
    {
           case ND_DEFAULT_KEYBOARD:
           case ND_VIDEO_KEYBOARD:
                return 1;
           default:
                return 0;
    }
}


int ndHAL_VKB_AssignMeKeyboard (char *MessageToUser, int Type, ndint64 SystemOptions, ndint64 KeyboardStyle, 
                                   void *KeyCallback,   void *KeyCallbackData,
                                   void *EndCallback,   void *EndCallbackData,
                                   void *BreakCallback, void *BreakCallbackData)
/*
    Questa routine provvede a farsi assegnare una tastiera da parte del gestore
    tastiere di Nanodesktop. A seconda della piattaforma utilizzata, Nanodesktop
    può mettere a disposizione tastiere virtuali (ossia tastiere che vengono
    visualizzate sullo schermo) oppure tastiere fisiche (a cui comunque deve
    essere associato un handle di sistema, ed a cui devono essere associate
    delle callbacks in grado di processare i tasti in arrivo). 
     
    Il tipo di tastiera che deve essere assegnato viene specificato dal 
    parametro a 32 bit Type: ci sono delle costanti manifeste, che sono
    unificate tra le varie piattaforme supportate da Nanodesktop, e che
    identificano i diversi dispositivi fisici o virtuali che sono disponibili
    per la digitazione di un testo. Il Type 0 indica la tastiera di default
    del sistema.
    
    Quando si lancia questa routine, per prima cosa il gestore delle tastiere 
    verifica se è disponibile un record nel registro tastiere in gestione e 
    se è presente, provvede ad impegnarlo ed ad attivare la tastiera richiesta. 
    
    L'attivazione della tastiera avviene nel seguente modo: in caso di tastiere
    virtuali, la tastiera viene disegnata sullo schermo; in caso di tastiere
    fisiche, viene lanciato un thread che consente di gestire i tasti che
    vengono premuti sulla tastiera fisica. 
    
    Il parametro MessageToUser ha significato solo per le tastiere virtuali:
    esso contiene l'indirizzo della stringa che viene visualizzata sulla 
    VirtualKeyboard in modo da informare l'utente riguardo all'elemento 
    che viene scritto in quel momento dalla tastiera. Per le tastiere fisiche
    è del tutto ignorato. 
    
    Il campo a 64 bit SystemOptions contiene una serie di opzioni necessarie
    per gestire la tastiera. Tali informazioni sono unificate tra tastiere
    virtuali e tastiere reali. Parleremo tra poco delle opzioni legate al
    campo a 64 bit SystemOptions. 
    
    Le informazioni necessarie per il corretto funzionamento del singolo
    tipo di tastiera, possono essere passate tramite il parametro a 64-bit 
    Keyboard Style. Tali informazioni sono specifiche per il tipo di 
    tastiera che si sta scegliendo.  
    
    I parametri KeyCallback:KeyCallbackData, EndCallback:EndCallbackData,
    BreakCallback:BreakCallbackData, rappresentano rispettivamente tre 
    callbacks distinte che devono essere definite dall'utente per usare le 
    tastiere virtuali o fisiche, gestendo i tasti in arrivo, e tre
    puntatori che vengono passati alle callback e che consentono di
    passare una serie di informazioni alle callback, per esempio allo
    scopo di usare il codice della medesima callback con 2 tastiere 
    virtuali distinte. L'utente può disattivare ciascuna di queste
    callback, semplicemente ponendo a 0 il relativo parametro.
    
    Veniamo adesso al parametro SystemOptions. 
    
    Il campo a 64 bit SystemOptions contiene una serie di opzioni necessarie
    per gestire la tastiera virtuale. Esse sono:
            
    ND_KEYBOARD_CALLBACK_BASED        Crea una tastiera callback-based
    ND_ASK_ONLY_A_KEY                 Chiedi solo un carattere    
    ND_KEYBOARD_NO_AUTOCLOSE          Non chiudere la tastiera alla fine
                                      delle operazioni    
        
    Il funzionamento della routine è diverso a seconda che si sia o meno
    specificata l'opzione ND_KEYBOARD_CALLBACK_BASED 
    
    a) Se non si è specificata l'opzione ND_KEYBOARD_CALLBACK_BASED,
       il sistema ricerca un keyboard handle libero tra quelli disponibili. 
    
       Se lo trova, provvede ad attivare la tastiera. Per le tastiere virtuali 
       questo significa disegnare la tastiera sullo schermo. Per le tastiere
       fisiche, questo significa lanciare un thread che deve gestire i vari
       tasti che vengono premuti.
       
       Quando non si richiede che la tastiera sia callback-based, il thread 
       che ha chiamato ndHAL_VKB_AssignMeKeyboard, viene bloccato fin quando
       non si verifica la corretta condizione di uscita per la tastiera. 
       Durante questa fase, il thread non prosegue la sua elaborazione: la
       tastiera invece può gestire i vari tasti che vengono premuti per
       mezzo delle callbacks KeyCallback, BreakCallback, EndCallback.  
       
       Ogni volta che viene premuto un tasto, viene richiamata la
       KeyCallback: questa procedura viene decisa dall'utente ed ha il compito
       di processare il tasto che è stato fornito. Il prototipo della 
       KeyCallback è una funzione del tipo:
                
       void KeyCallback (int KeyPressedCode, void *KeyCallbackData, int VKBHandle, 
                        char WndHandle)
                        
       Il parametro KeyCallbackData è un indirizzo di un'area dati che può essere
       usata dalla KeyCallback per eseguire l'elaborazione dei dati o per 
       decidere le operazioni da eseguire. Allocando diverse aree dati, è possibile
       utilizzare più istanze contemporanee della medesima key callback, ossia
       è possibile usare più tastiere fisiche o virtuali contemporaneamente.
    
       Il tasto premuto viene comunicato attraverso un codice a 32 bit 
       chiamato KeyPressedCode. Di questo parleremo in seguito.
    
       Dopo la pressione di ogni tasto (e dopo che questo è stato processato
       eseguendo la richiesta KeyCallback), il sistema deve verificare se
       è necessario disallocare il thread chiamante, disattivare la tastiera
       (per le tastiere virtuali significa cancellarle dallo schermo), e
       liberare l'handle che era stato impegnato.   
       
       A tale scopo, esso esegue un'altra callback, detta EndCallback. Questa
       routine ha prototipo:
               
       char EndCallback (void *EndCallbackData, int VKBHandle, char WndHandle)
    
       Se tale funzione, dopo l'elaborazione, restituisce il valore 1, la tastiera
       sarà disattivata, il thread che ha chiamato ndHAL_VKB_AssignMeKeyboard sarà
       sbloccato ed il controllo gli sarà restituito. 
    
       E' possibile anche evitare di eseguire una end-callback che ci dica quando
       restituire il controllo alla routine chiamante: se l'utente setta 
       l'opzione ND_ASK_ONLY_A_KEY, il sistema richiederà solo un tasto e poi
       ritornerà il controllo. 
       
       Il prototipo della funzione prevede anche una callback detta BreakCallback.
       Questa callback ha prototipo:
              
       void BreakCallback (void *BreakCallbackData, int VKBHandle, char WndHandle);
              
       Essa viene eseguita automaticamente quando l'utente cerca di uscire dalla
       tastiera virtuale usando il pulsante di chiusura finestra. La callback
       è completamente ignorata, invece, quando l'utente sta usando una tastiera
       fisica. 
       
       La disattivazione di una tastiera virtuale implica che questa sia chiusa
       automaticamente: questo può essere evitato settando l'opzione 
       ND_KEYBOARD_NO_AUTOCLOSE. Tale opzione è ignorata nel caso di tastiere
       fisiche. 
       
   b)  Se si è specificata l'opzione ND_KEYBOARD_CALLBACK_BASED, il thread
       chiamante non verrà bloccato fin quando il loop di richiesta caratteri
       non sarà terminato, ma verrà invece fatto continuare liberamente. 
       
       Quando si chiede una tastiera callback based, accadono due cose: 
       
       1) per le tastiere virtuali, il sistema si limita a creare una nuova
          tastiera virtuale sullo schermo, dopodichè restituisce subito 
          il controllo alla routine chiamante che prosegue. La tastiera
          virtuale gestirà i tasti (e chiuderà sè stessa), solo per mezzo
          delle apposite callbacks    
       
       2) per le tastiere fisiche, il sistema avvia un thread a parte che
          riceve i dati dalla tastiera, ed esegue le relative callbacks.
       
       In ogni caso, si ribadisce che ***Non vi sarà nessuna attesa da
       parte del thread chiamante *** che riceverà in uscita l'handle della tastiera
       aperta e proseguirà subito l'elaborazione. 
       
       La callback BreakCallback verrà eseguita quando l'utente tenterà di
       chiudere la tastiera virtuale usando il tasto di chiusura finestra,
       oppure quando l'utente premerà un opportuna combinazione di
       ESCAPE sulla tastiera fisica
       
       Quando si crea una tastiera callback based, i parametri EndCallback e 
       EndCallbackData vengono ignorati. 
    
       Inoltre sono ignorate le eventuali opzioni ND_KEYBOARD_NO_AUTOCLOSE
       e ND_ASK_ONLY_A_KEY
       
    Codici KeyPressed:
    -----------------
       Veniamo ora ai codici KeyPressed. Ogni tasto ha associato un codice 
       a 32 bit. Questi 32 bit sono divisi in tre campi distinti: un campo 
       detto FIELDSK, costituito dai 16 più significativi, e due campi 
       detti Hi (dal bit 15 al bit 8) e Lo (dal bit 8 al bit 0)
       
                     FIELDSK   BitsHi  BitsLo
                     
       Il campo FIELDSK è bit-mapped. I bit rappresentano la pressione
       di un tasto speciale come CTRL-ALT-ALTGR sulla tastiera ad
       infrarossi fisica. Le tastiere virtuali non presentano questi
       bits, che sono sempre posti a 0. Perciò se l'utente vuole 
       scrivere codice generico, deve ignorare i 16 bit più significativi,
       che sono specifici delle varie tastiere. 
       
       Il campo ad 8 bit BitsHi contiene un codice, che ci dice qual è
       il tipo di tasto che è stato premuto. Questi codici sono unificati
       tra le tastiere fisiche e quelle virtuali. 
       
       ND_GENERIC_SPECIALKEY
       E' stato premuto un tasto speciale (CTRL-ALT-CANC-INS)
       
       ND_GENERIC_FUNCKEY
       Tasto funzione (FUNC+A, FUNC+B ecc. ecc.). Le tastiere fisiche assegnano
       a tasti come CTRL la funzione di fare da tasti FUNC
       
       ND_KEY_HAS_BEEN_RELEASED
       Alcune tastiere fisiche generano un codice anche quando una tasto
       viene rilasciato e non solo quando viene premuto. Le tastiere virtuali
       non generano questi codici 
       
       ND_NORMAL_KEY
       Il tasto è normale
       
       Il campo a 8 bit BitsLo è il codice ASCII del tasto. In alcuni casi, 
       esso rappresenta un codice che ci dice quale tasto speciale è stato
       premuto (ESC, CANC, ENTER ecc. ecc.)
       
    Codici di uscita:
    -----------------
    
    La routine riporta in uscita un codice a 32 bit che contiene una serie di
    informazioni relative al funzionamento di questa routine. 
    
    Il significato di questo parametro è abbastanza complesso: il codice
    deve innanzitutto distinguere il caso in cui il codice di uscita è
    positivo, dal caso in cui il codice di uscita è negativo (caso tipico
    della condizione di errore). 
    
    a)
    
    Se il codice di uscita è positivo, allora vuol dire che il processo di
    creazione della tastiera virtuale e l'eventuale processo di lettura
    dei tasti è andato a buon fine. I bit del codice di uscita sono 
    mappati nel seguente modo:
            
    7-0         AlphaField:         contiene il VKBHandle che è stato assegnato dal
                                    sistema alla tastiera. L'handle che viene restituito 
                                    in uscita, è sempre un numero maggiore od uguale
                                    ad 0, ed identifica la tastiera per scopi futuri. 
       
    23-8        BetaGammaField:     contiene il numero di caratteri letto, 
                                    (per le tastiere callback based vale sempre 0)
    
    
    b)
    
    Se il codice di uscita è negativo, allora vuol dire che il processo di
    creazione della tastiera virtuale è stato fermato da un errore. I bit del
    codice di uscita sono mappati nel seguente modo:
           
    7-0         AlphaField:   contiene un codice di errore primario
    
    15-8        BetaField:    contiene il codice di errore secondario, che è
                              specifico per il tipo di tastiera scelto
    

    Sono previsti i seguenti codici di errore primari:
         
    ERR_NO_VKB_AVAILABLE     Non ci sono più tastiere virtuali disponibili  
    ERR_VKB_WRONG_TYPE       Tipo tastiera errato
    ERR_I_CANNOT_CREATE_VKB  Non posso creare la tastiera virtuale 
                             (viene restituito anche il codice secondario).
    
    
*/

{
    char IHaveFound, NrVKBRecordFound;
    int  Counter; 
    
    int  StartKeyb_ExitCode;
    
    char MainError = 0;
    char SecError  = 0;
    
    int CallbackBased, AskOnlyAKey, NoKeybAutoClose;
    

    // Ricerchiamo un record libero nel registro delle tastiere
    
    IHaveFound=0;
    
    for (Counter=0; Counter<__NRVIRTUALKEYBOARDS_ALLOWED; Counter++)
    {
       if ( !SysKeyboardsRegister [Counter].IsEnabled )          // Abbiamo trovato un record libero
       {
            IHaveFound       = 1;
            NrVKBRecordFound = Counter;
            break;
       }
    }      
    
    if (IHaveFound)
    {
         // Provvediamo a registrare il Type
         
         if (Type==0) Type=ndDefaultKeybType;
         
         SysKeyboardsRegister [NrVKBRecordFound].TypeKeyboard   = Type;
         
         // Provvediamo a copiare la stringa MessageToUser
         
         ndStrCpy (SysKeyboardsRegister [NrVKBRecordFound].MessageToUser, MessageToUser, 255, 0);
         
         // Provvediamo a registrare i codici delle callback e delle rispettive aree dati
         
         SysKeyboardsRegister [NrVKBRecordFound].SystemOptions     = SystemOptions;
         SysKeyboardsRegister [NrVKBRecordFound].KeyboardStyle     = KeyboardStyle;
         SysKeyboardsRegister [NrVKBRecordFound].KeyCallback       = KeyCallback;
         SysKeyboardsRegister [NrVKBRecordFound].KeyCallbackData   = KeyCallbackData;
         SysKeyboardsRegister [NrVKBRecordFound].EndCallback       = EndCallback;
         SysKeyboardsRegister [NrVKBRecordFound].EndCallbackData   = EndCallbackData;
         SysKeyboardsRegister [NrVKBRecordFound].BreakCallback     = BreakCallback;
         SysKeyboardsRegister [NrVKBRecordFound].BreakCallbackData = BreakCallbackData;
         
         // Resettiamo i contatori ed i flag
         
         SysKeyboardsRegister [NrVKBRecordFound].NrCharsRead     = 0;
         SysKeyboardsRegister [NrVKBRecordFound].YouCanExit      = 0;
         
         // Settiamo le variabili SystemOptions
         
         CallbackBased    = ( (SystemOptions & ND_KEYBOARD_CALLBACK_BASED) == ND_KEYBOARD_CALLBACK_BASED ); 
         AskOnlyAKey      = ( (SystemOptions & ND_ASK_ONLY_A_KEY) == ND_ASK_ONLY_A_KEY );
         NoKeybAutoClose  = ( (SystemOptions & ND_KEYBOARD_NO_AUTOCLOSE) == ND_KEYBOARD_NO_AUTOCLOSE );
         
         // Verifichiamo che il tipo di tastiera virtuale sia corretto
         
         switch (Type)
         {
                case ND_VIDEO_KEYBOARD:
                {
                     // E' stata scelta la tastiera virtuale codice 1
                     // Provvede a lanciare la tastiera
                     
                     StartKeyb_ExitCode = INTERNAL_VKB1_StartKeyboard ( NrVKBRecordFound, KeyboardStyle, CallbackBased );
                     
                     if (StartKeyb_ExitCode==0)            // Nessun errore nella creazione della tastiera
                     {
                          //    Avvia il loop di richiesta caratteri previsto per la tastiera codice 1
                          return INTERNAL_VKB1_BeginReadCharLoop (NrVKBRecordFound, CallbackBased, AskOnlyAKey, NoKeybAutoClose);                                 
                     }
                     else                                  // Errore nella creazione della tastiera
                     {
                          MainError = ERR_I_CANNOT_CREATE_VKB;
                          SecError  = StartKeyb_ExitCode;
                     
                          return (-1)*(abs(SecError)*256 + abs(MainError)); 
                     }
                                            
                     break;
                }
                
                default:   // Il codice tastiera non è fra quelli attualmente supportati
                {
                     MainError = ERR_VKB_WRONG_TYPE;
                     SecError  = 0;
                     return (-1)*(abs(SecError)*256 + abs(MainError)); 
                }    
         }
           
    }
    else    // Non ci sono VKBRecords liberi
    {
        MainError = ERR_NO_VKB_AVAILABLE;
        SecError  = 0;
        return (-1)*(abs(SecError)*256 + abs(MainError)); 
    }
    
    
} 



int ndHAL_VKB_ReUseAnOpenedKeyboard (ndint64 SystemOptions, int VKBHandle)
/*
    Questa routine provvede a riutilizzare una tastiera virtuale di cui sia stato
    fornito il VKBHandle, e che sia attualmente aperta. 
    
    Una tastiera virtuale può essere rimasta aperta per diverse ragioni: un caso
    classico è la tastiera che era stata assegnata usando l'opzione 
    ND_KEYBOARD_NO_AUTOCLOSE.
    
    Il parametro SystemOptions ha lo stesso significato che per la routine
    ndHAL_VKB_AssignMeKeyboard 
    
    Sono previsti i seguenti codici di errore: 
         ERR_INVALID_VKBHANDLE      L'handle si riferisce ad una keyboard che in 
                                    realtà è chiusa

    Il codice di uscita della funzione è identico a quello previsto per 
    ndHAL_VKB_AssignMeKeyboard 
*/
{
     int CallbackBased, AskOnlyAKey, NoKeybAutoClose;
     
     char MainError = 0;
     char SecError  = 0;

     if (SysKeyboardsRegister [VKBHandle].IsEnabled)
     {
             // Determiniamo le costanti di configurazione
             
             CallbackBased    = ( (SystemOptions & ND_KEYBOARD_CALLBACK_BASED) == ND_KEYBOARD_CALLBACK_BASED ); 
             AskOnlyAKey      = ( (SystemOptions & ND_ASK_ONLY_A_KEY) == ND_ASK_ONLY_A_KEY );
             NoKeybAutoClose  = ( (SystemOptions & ND_KEYBOARD_NO_AUTOCLOSE) == ND_KEYBOARD_NO_AUTOCLOSE );
         
             // Resettiamo i contatori
             
             SysKeyboardsRegister [VKBHandle].NrCharsRead     = 0;
             SysKeyboardsRegister [VKBHandle].YouCanExit      = 0;
         
             // Iniziamo il loop 
             
             switch (SysKeyboardsRegister [VKBHandle].TypeKeyboard)
             {
                case ND_VIDEO_KEYBOARD:
                {
                     // E' stata scelta la tastiera virtuale codice 1
                     //    Avvia il loop di richiesta caratteri previsto per la tastiera codice 1
                          
                     return INTERNAL_VKB1_BeginReadCharLoop (VKBHandle, CallbackBased, AskOnlyAKey, NoKeybAutoClose);                                 
                }
                
                default:   // Il codice tastiera non è fra quelli attualmente supportati
                {
                     MainError = ERR_VKB_WRONG_TYPE;
                     SecError  = 0;
                     return (-1)*(abs(SecError)*256 + abs(MainError)); 
                }    
             }
             
     }
     else 
       return ERR_INVALID_VKBHANDLE; 
}



char ndHAL_VKB_DisplayInVKeyboard (char *NewMessageToUser, int VKBHandle, char RenderNow)
/*
   Questa funzione provvede a visualizzare un nuovo messaggio nell'area messaggi
   della Virtual Keyboard

   Sono previsti i seguenti codici di errore:
     ERR_INVALID_VKBHANDLE  L'handle VKB non è valido   
*/
{
   if (VKBHandle<__NRVIRTUALKEYBOARDS_ALLOWED)
   {
   
           if (SysKeyboardsRegister [VKBHandle].IsEnabled)
           {
                switch (SysKeyboardsRegister [VKBHandle].TypeKeyboard)
                {
                       case ND_VIDEO_KEYBOARD:
                       {
                           return INTERNAL_VKB1_DisplayInVKeyboard (NewMessageToUser, VKBHandle, RenderNow); 
                       }
                       
                       default:
                       {
                           return ERR_VKB_WRONG_TYPE;
                       }
                }                    
           }
           else 
             return  ERR_INVALID_VKBHANDLE;    
   
   }
   else 
     return  ERR_INVALID_VKBHANDLE;      
}
                                


char ndHAL_VKB_DestroyKeyboard (char VKBHandle)
/*
     Questa funzione provvede a distruggere una tastiera virtuale ed a disimpegnare
     il rispettivo handle. E' usata per le Virtual Keyboard di tipo callback based
     o per quelle che non sono state chiuse all'uscita a causa del fatto che si è
     scelta l'opzione ND_KEYBOARD_NO_AUTOCLOSE
     
     Sono previsti i seguenti codici di errore
          ERR_INVALID_VKBHANDLE      Handle VirtualKeyboard invalido 
          
*/
{
     if (VKBHandle<__NRVIRTUALKEYBOARDS_ALLOWED)
     {
         if (SysKeyboardsRegister [VKBHandle].IsEnabled)
         {
             switch (SysKeyboardsRegister [VKBHandle].TypeKeyboard)
             {
                 case ND_VIDEO_KEYBOARD:
                 {
                      return INTERNAL_VKB1_DestroyKeyboard (VKBHandle);   
                 }
                 
                 default:
                 {
                      return ERR_VKB_WRONG_TYPE;
                 }
             } 
         
         }
         else                // L'handle era stato già disallocato 
           return 0;  
     }
     else
       return ERR_INVALID_VKBHANDLE;
}





void INTERNAL_GetASingleKeyb_KeyCallback (int KeyPressedCode, void *KeyCallbackData, int VKBHandle, char WndHandle)
{
    int *KeyCode = KeyCallbackData;
    *KeyCode = KeyPressedCode; 
}

void INTERNAL_GetASingleKeyb_Callback (int KeyPressedCode, void *KeyCallbackData, int VKBHandle, char WndHandle)
{
    int *KeyCode = KeyCallbackData;
    *KeyCode = KeyPressedCode; 
}


int ndHAL_VKB_GetASingleKey (char *MessageToUser, int Type)
/*
    Questa routine è la routine standard per ottenere un singolo tasto. Essa 
    restituisce un codice a 32 bit che è il CodeKey del tasto che è stato
    premuto. 
    
    Se la routine restituisce un valore negativo, vuol dire che c'è stato
    un errore. Sono previsti i seguenti codici di errore:
    
    ERR_NO_VKB_AVAILABLE     Non ci sono più tastiere virtuali disponibili  
    ERR_VKB_WRONG_TYPE       Tipo tastiera errato
    ERR_I_CANNOT_CREATE_VKB  Non posso creare la tastiera virtuale 
                             (viene restituito anche il codice secondario).       
*/
{
    int KeyCode;
    int ErrRep;
    
    ErrRep = ndHAL_VKB_AssignMeKeyboard (MessageToUser, Type, ND_ASK_ONLY_A_KEY, 0, 
                                   &INTERNAL_GetASingleKeyb_KeyCallback, &KeyCode,
                                   0, 0, 0, 0);
                                   
    if (ErrRep<0) return ErrRep;
    // Se c'è stato un errore riportalo in uscita
    
    return KeyCode;
    // Riporta in uscita il codice del tasto rilevato
}
