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
    ROUTINES PER LA GESTIONE DEL MOUSE
    ----------------------------------------------------------------------------
    Queste routine si occupano della gestione del mouse.
    
    Tutte le routines che si occupano della gestione del mouse hanno un prefisso
    un prefisso ndCTRL_ 
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>



char ndCTRL_MoveMousePointerToPos (unsigned short int PosX, unsigned short int PosY)
/*
   Questa funzione provvede a spostare il puntatore del mouse in una determinata 
   posizione che viene passata alla funzione esplicitamente. 
   
   A partire dalla versione 0.3 di Nanodesktop il codice è stato spostato nell'HAL
   e questa funzione viene mantenuta esclusivamente per ragioni di retrocompatibilità.
   
   Sono previsti i seguenti codici di errore:
        ERR_WRONG_PARAMETERS       Le coordinate del puntatore sono errate
*/

{
     return ndHAL_MoveMousePointerToPos (PosX, PosY);   
}




void ndCTRL_HideMousePointer (void)
/* 
   Cancella il puntatore del mouse. Questa routine fa scomparire il puntatore del
   mouse, ma non arresta l'eventuale Phoenix Mouse Thread che continua a girare
   in background nel frattempo. 
   
   Se si vuole cancellare il puntatore e nel contempo arrestare il Phoenix Mouse
   Thread, usare la funzione ndHAL_DisableMousePointer()
  
   Non sono previsti codici di errore per questa routine.
*/
{  
   ndHAL_HideMousePointer (); 
}




void ndCTRL_ShowMousePointer (void)
/* 
   Mostra il puntatore del mouse nella sua attuale posizione

*/
{
   ndHAL_ShowMousePointer ();  
}






void MouseControl (struct ndEvent_Type *EventAddress)
/*
   -----------------------------------------------------------------------------------------------
   Questa routine è una delle più importanti del sistema Nanodesktop. Qualsiasi applicazione che
   faccia uso del mouse deve passare obbligatoriamente per questa routine. 
   
   (OBSOLETO: a partire da Nanodesktop 0.4, l'uso diretto di MouseControl è mantenuto solo
    per ragioni di compatibilità. L'utente può usare con maggiore profitto ndProvideMeTheMouse_Until
    e ndProvideMeTheMouse_Check)
   
   Quando si richiama MouseControl, la routine
       
   - Fa comparire un puntatore del mouse sullo schermo e permette di muoverlo
   - Quando l'utente esegue un clic su un oggetto la funzione verifica il tipo di oggetto.
     
   Se l'oggetto è di tipo "riservato" (pulsanti di minimizzazione, scrolling, massimizzazione
   ecc. ecc.), allora MouseControl provvede a richiamare le relative routine di gestione.
     
   Se l'oggetto è di tipo "user-defined" (un pulsante definito dall'utente), allora MouseControl
   provvede a restituire alla routine chiamante una struct di tipo ndEvent_Type che descrive 
   l'oggetto che è stato premuto. A tale scopo, le routine chiamante deve passare a
   MouseControl l'indirizzo di una struct di tipo ndEvent_Type che sia già stata allocata
   in precedenza

   Il thread verrà arrestato fin quando non sarà recapitato un messaggio. Per quanto riguarda
   i messaggi, si applicano le convenzioni seguenti: ad occuparsi della generazione dei
   messaggi è la routine ndHAL_MouseEventManager. 
     
   Questa funzione si occupa di gestire gli eventi del sistema grafico. 
   
   In pratica, essa viene richiamata da MouseControl quando viene premuto uno dei quattro
   pulsanti previsti dalla routine ndHAL_PadCtrl. 
   
   La routine verifica qual è il pulsante che è stato premuto dall'utente 
   ed esegue la relativa operazione di minimizzazione della 
   finestra, o di spostamento della stessa.
   
   Qualora l'utente clicchi su un pulsante di classe 4 (pulsanti definiti 
   dall'utente), su un pulsante di classe 5 (pulsanti del menù WMI1), 
   su un pulsante di classe 6 (pulsante del menù WMI2), la routine
   verifica innanzitutto che sia stata registrata una opportuna callback.
   
   Nel caso dei pulsanti di classe 4 (o pulsanti definiti dall'utente), una
   callback è una routine con prototipo:
            
       ndint64 Callback (char *StringID, ndint64 InfoField, char WndHandle);
       
   Il parametro StringID è l'indirizzo della StringID del pulsante che è stato
   premuto. In tal modo, la callback può avere un'indicazione su quale 
   pulsante è stato premuto all'interno della finestra. 
   
   Il campo InfoField è il numero a 64 bit che viene registrato nel pulsante
   da CreateButtonComplex. Il significato è quello che viene attribuito dal
   programmatore.
   
   Per i pulsanti del menù WMI1 e WMI2, invece, una callback è una routine con 
   prototipo:
       
       ndint64 CallBack (char WndHandle);
       
   che restituisce un risultato a 64 bit. EventManager esegue la routine
   Callback e restituisce il risultato nel campo __CallbackAnswer della
   struttura ndEvent_Type puntata da *EventAddress.
   
   Dopo avere eseguito l'eventuale Callback, la routine inoltre lascia un
   opportuno messaggio nella struttura ndEvent.  
   
   Qualora l'utente abbia cercato di chiudere una finestra, ci sono due 
   possibilità:
               
   a) sulla finestra non è stata mai usata la routine ndLP_SetupCloseWndCallback
      allo scopo di settare la callback di chiusura finestra
      In tal caso, ndCTRL_EventManager, si limita a restituire un messaggio 
      apposito alla routine chiamante attraverso una struttura di
      tipo ndEvent: è l'applicazione chiamante che deve provvedere a chiudere
      la finestra in conseguenza del messaggio. 
   
   b) sulla finestra era stata usata la routine ndLP_SetupCloseWndCallback
      allo scopo di settare la callback di chiusura finestra. 
      Tale callback ha un prototipo del tipo:
           
      ndint64 CloseWndCallback (char WndHandle, ndint64 WndInfoField);
      
      Se nell'usare la routine ndLP_SetupCloseWndCallback era stata attivata 
      l'opzione NoNotifyToMouseControl, il sistema eseguirà la callback 
      internamente a MouseControl senza uscire dallo stesso.
      
      Al contrario, se è stata disabilitata l'opzione NoNotifyToMouseControl,
      il sistema eseguirà la callback e restituirà alla routine chiamante un
      messaggio.   
   
   Si ritiene opportuno riportare qui, per migliore comprensione del codice, il
   significato dei campi della struttura ndEvent_Type:
               
    Una struct di tipo ndEvent_Type possiede i seguenti campi:
   
    Questo int contiene informazioni sul tipo di evento che
    si è verificato: i valori possibili sono:
                   
    __TypeEvent:      0 NO_EVENT
                      
                      Gli eventi sono stati tutti gestiti internamente dal 
                      sottosistema grafico. 
                      
                      -3 SPECIAL_KEY_PRESSED
                      
                      E' stato premuto un tasto speciale della tastiera o della
                      console: in genere significa che siamo in presenza di un
                      evento speciale. __WindowID viene sempre posto a 0, mentre
                      in __ButtonID c'è un codice specifico della piattaforma
                      che identifica il tasto premuto 
                      
                      -2 CLOSEWNDCALLBACK_HAS_BEEN_EXEC
                      
                      E' stata eseguita una CloseWindow callback. Questo
                      messaggio viene recapitato a condizione che l'utente
                      non abbia impostato ad 1 il flag NoNotifyToMouseControl
                      relativo alla finestra per mezzo della routine 
                      ndLP_SetupCloseWndCallback.
                      
                      Nel campo __WindowID è presente l'handle della finestra
                      
                      Inoltre, verrà posto ad 1 il campo __CallbackExecuted 
                      ed il campo a 64 bit __CallbackAnswer conterrà il risultato a 64 bit della
                      Callback (può eventualmente essere un indirizzo). 
                      
                      -1 A_WINDOW_WANTS_TO_BE_CLOSED
                      
                      Una finestra ha chiesto di essere chiusa. Il numero
                      della finestra è contenuto nel campo __WindowID
                      della struct ndEvent_Type
                      
                      1 CUSTOM_BUTTON_PRESSED
                      
                      E' stato premuto un pulsante definito dall'utente.
                      Nel campo __WindowID è presente l'handle della finestra
                      e nel campo __ButtonID è presente l'handle del 
                      pulsante. Nel campo __StringID viene posto l'identificatore
                      stringa del pulsante. 
                      
                      Se il sistema ha eseguito una callback prima di riportare
                      il controllo alla routine chiamante, verrà posto ad 1
                      il campo __CallbackExecuted ed il campo a 64 bit
                      __CallbackAnswer conterrà il risultato a 64 bit della
                      Callback (può eventualmente essere un indirizzo). 
                      
                      2 L1WMI_BUTTON_PRESSED
                      
                      E' stato premuto un elemento del menù L1WMI. Nel campo
                      __WindowID è presente l'handle della finestra. Nel
                      campo __ZString è sempre presente una copia della
                      ZString associata al pulsante.
                      
                      Se il sistema ha eseguito una callback prima di riportare
                      il controllo alla routine chiamante, verrà posto ad 1
                      il campo __CallbackExecuted ed il campo a 64 bit
                      __CallbackAnswer conterrà il risultato a 64 bit della
                      Callback (può eventualmente essere un indirizzo). 
                      
                      3 L2WMI_BUTTON_PRESSED
                      
                      E' stato premuto un elemento del menù L2WMI. Nel campo
                      __WindowID è presente l'handle della finestra. Nel
                      campo __ZString è sempre presente una copia della
                      ZString associata al pulsante.
                      
                      Se il sistema ha eseguito una callback prima di riportare
                      il controllo alla routine chiamante, verrà posto ad 1
                      il campo __CallbackExecuted ed il campo a 64 bit
                      __CallbackAnswer conterrà il risultato a 64 bit della
                      Callback (può eventualmente essere un indirizzo).  
                                                                             
                           
  __WindowID          L'ID della finestra dove è avvenuto l'evento
  
  __ButtonID          L'ID del pulsante definito dall'utente
  
  __StringID          L'identificatore stringa del pulsante
  
  __CallbackExecuted  Dice alla routine chiamante se è stata eseguita una callback
                      e quindi se il risultato a 64 bit in CallbackAnswer deve 
                      essere interpretato oppure no.
                      
  __CallbackAnswer    La risposta a 64 bit della Callback che è stata eseguita
  
  __ZString           La ZString che identifica l'elemento L1WMI/L2WMI che è 
                      stato premuto.
   
                 
  ------------------------------------------------------------------------------------------------   
*/
{
   ndHAL_MouseControl (EventAddress);             // Richiama la nuova funzione                                                                                        
}


void ndProvideMeTheMouse_Until (int *CtrlValue, unsigned char CondCode0, int TheValue0, 
                                                unsigned char CondCode1, int TheValue1)
/*
    Questa routine permette di creare con facilità un ciclo mouse. 
    
    Quando l'utente usa ndProvideMeTheMouse_Until, Nanodesktop interrompe l'esecuzione
    del thread corrente ed entra in un proprio loop MouseControl.
    
    Durante tale loop, viene richiamato MouseControl facendo apparire un puntatore
    sullo schermo. L'utente può eseguire le callback di pulsante ed usare i menù.
    
    Ogni volta che viene premuto un elemento per il quale non è stata prevista una
    callback di tipo "trasparente", il sistema esegue la callback e poi verifica
    la condizione di controllo stabilita all'inizio del ciclo per mezzo della
    terna di valori CtrlValue/ConditionCode/TheValue. 
    
    Se il controllo riesce, Nanodesktop esce dal ciclo mouse e restituisce il controllo
    al thread chiamante. 
    
    La terna CtrlValue/ConditionCode/TheValue funziona nel seguente modo: CtrlValue
    è un puntatore ad una variabile di controllo di tipo int, TheValue è il valore
    che la variabile dovrà assumere (o non assumere), perchè la routine esca dal
    ciclo mouse. 
    
    Il ConditionCode specifica il tipo di controllo che deve essere eseguito sulla
    variabile CtrlValue. E' possibile usare le seguenti costanti manifeste:
              
    #define ND_IS_EQUAL              1
    #define ND_IS_DIFFERENT          2
    #define ND_IS_GREATER            3
    #define ND_IS_GREATER_OR_EQUAL   4
    #define ND_IS_SMALLER            5
    #define ND_IS_SMALLER_OR_EQUAL   6
    #define ND_NOT                 128
    
    Per esempio, la chiamata
    
    ndProvideMeTheMouse_Until (&Exit, ND_IS_EQUAL, 1 ....);
    
    Determina l'uscita dal ciclo mouse se Exit è pari a 1. 
    
    Nanodesktop prevede due gruppi parametri (Cond0/Value0, Cond1/Value1), perchè
    in tal modo è possibile subordinare l'uscita ad una condizione doppia. 
    
    Per esempio:
        
    ndProvideMeTheMouse_Until (&Exit, ND_IS_GREATER, 10, ND_IS_LESSER 25);     

    esce se Exit è compresa tra 10 e 25. Se non si vuole il secondo controllo, è
    sufficiente porre a 0 il relativo parametro Cond0 o Cond1. 
*/

{
    struct ndEvent_Type MyEvent;
    
    char   YouCanExit;
    char   IsVerified0, IsVerified1;
    
    
    YouCanExit=0;
    
    while (!YouCanExit)
    {
        MouseControl (&MyEvent);
        
        IsVerified0=0;
        IsVerified1=0;
        
        // Analisi della prima condizione
        
        switch (CondCode0 & 0x7F)
        {
               case 0:
                    {
                        IsVerified0=1;   // Se non è specificato nulla, il semaforo è on di default
                        break;
                    }
               case ND_IS_EQUAL:
                    {
                        IsVerified0 = (  (*CtrlValue) == TheValue0  );
                        break;
                    }
               case ND_IS_DIFFERENT:
                    {
                        IsVerified0 = (  (*CtrlValue) != TheValue0  );
                        break;            
                    }
               case ND_IS_GREATER:
                    {
                        IsVerified0 = (  (*CtrlValue) > TheValue0  );
                        break;          
                    }
               case ND_IS_GREATER_OR_EQUAL:
                    {
                        IsVerified0 = (  (*CtrlValue) >= TheValue0  );
                        break;          
                    }
               case ND_IS_SMALLER:
                    {
                        IsVerified0 = (  (*CtrlValue) < TheValue0  );
                        break;          
                    }
               case ND_IS_SMALLER_OR_EQUAL:
                    {
                        IsVerified0 = (  (*CtrlValue) <= TheValue0  );
                        break;          
                    }          
        }
        
        if ((CondCode0 & ND_NOT)==ND_NOT)    IsVerified0=!IsVerified0;
        
        // Analisi della seconda condizione
    
        switch (CondCode1 & 0x7F)
        {
               case 0:
                    {
                        IsVerified1=1;  // Se non è specificato nulla, il semaforo è on di default
                        break;
                    }
               case ND_IS_EQUAL:
                    {
                        IsVerified1 = (  (*CtrlValue) == TheValue1  );
                        break;
                    }
               case ND_IS_DIFFERENT:
                    {
                        IsVerified1 = (  (*CtrlValue) != TheValue1  );
                        break;            
                    }
               case ND_IS_GREATER:
                    {
                        IsVerified1 = (  (*CtrlValue) > TheValue1  );
                        break;          
                    }
               case ND_IS_GREATER_OR_EQUAL:
                    {
                        IsVerified1 = (  (*CtrlValue) >= TheValue1  );
                        break;          
                    }
               case ND_IS_SMALLER:
                    {
                        IsVerified1 = (  (*CtrlValue) < TheValue1  );
                        break;          
                    }
               case ND_IS_SMALLER_OR_EQUAL:
                    {
                        IsVerified1 = (  (*CtrlValue) <= TheValue1  );
                        break;          
                    }          
        }
        
        if ((CondCode1 & ND_NOT)==ND_NOT)    IsVerified1=!IsVerified1;
        
        if (IsVerified0 && IsVerified1) 
           YouCanExit=1;
           
    }
}


void ndProvideMeTheMouse_Check (void *CheckCallbackAdr, ndint64 CodeCheck)
/*
     Questa routine è simile alle precedenti, solo che il meccanismo che
     consente l'uscita dal loop MouseControl è diverso. 
     
     Ogni volta che avviene un mouse event che non può essere gestito
     internamente da MouseControl, il sistema esegue una "check callback".
     
     Una CheckCallback è una callback con prototipo:
         
     char CheckCallback (ndint64 CodeCheck, char TypeEvent, char WindowID, 
                         char ButtonID, char *StringID, char CallbackExecuted,
                         ndint64 CallbackAnswer, char ZString)
                         
     Questa callback deve riceve in ingresso tutte le informazioni sull'evento
     che si è verificato nel sistema grafico, e deve decidere quando uscire
     dal loop MouseControl. 
     
     Se la callback restituisce 1, questa routine esce dal loop MouseControl
     ed il controllo viene restituito al thread chiamante. 
     
     Il codice CodeCheck è un codice a 64 bit, che permette di usare la
     stessa Callback con usi contemporanei di diversi loop MouseControl.

*/
{
    struct ndEvent_Type MyEvent;
    char   YouCanExit;
    
    char (*CheckCallback) (ndint64 CodeCheck, char TypeEvent, char WindowID, 
                           char ButtonID, char *StringID, char CallbackExecuted,
                           ndint64 CallbackAnswer, char *ZString);
    
    CheckCallback = CheckCallbackAdr;
    
    // Inizio del codice
    
    YouCanExit=0;
    
    while (!YouCanExit)
    {
        MouseControl (&MyEvent);
        
        if  ((*CheckCallback) (CodeCheck, MyEvent.__TypeEvent, MyEvent.__WindowID, 
                                         MyEvent.__ButtonID,  &(MyEvent.__StringID), 
                                         MyEvent.__CallbackExecuted, MyEvent.__CallbackAnswer, 
                                         &(MyEvent.__ZString)    ))                                
        {
             YouCanExit=1;
        }
    }
}
