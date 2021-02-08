/* ------------------------------------------------------------------------
   USB Driver PRX for Nanodesktop PSP - Version for CF
   Version 1.0
   ------------------------------------------------------------------------
   This is the driver for USB support in Nanodesktop PSP (ndUsbDriver.PRX).
   
   This driver is partially based on USB HostFS Driver of PSPLINK. 
   
   PSPLink is a component under BSD license:         
   Copyright (c) 2006 James Forshaw <tyranid@gmail.com> 
   file:///home/svn/psp/trunk/psplink/usbhostfs/

   Filippo Battaglia has introduced many modifications to the Forshaw original 
   code, obtaining this Nanodesktop USB Driver. He recognizes to James Forshaw,
   all rights about the original code. 
   
   This Nanodesktop USB driver is released under BSD license. 
   ----------------------------------------------------------------------------
*/


#include <pspkernel.h>
#include <pspdebug.h>
#include <pspkdebug.h>
#include <pspsdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspusb.h>
#include <pspusbbus.h>
#include <nanodesktop.h>


/*
   ---------------------------------------------------------------------------
   PUNTATORI PER LA GESTIONE DELLA COMM AREA
   ---------------------------------------------------------------------------
*/

struct ndUsb_CommArea_Type *PntCommArea;
int    CommAreaInit = 0;

char   SenderIsBusy = 0;


/*
   ----------------------------------------------------------------------------
   ROUTINES PER LA GESTIONE DELLA MEMORIA
   ----------------------------------------------------------------------------
   Durante il build di un PRX tutte le dipendenze devono essere soddisfatte,
   o essere importate dal driver. Se il codice usasse le normali memset e
   memcpy fornite dalla NanoC, questo implicherebbe la necessità di includere
   il codice della NanoC nel driver, ma si è visto che questo provoca problemi
   di compatibilità. Per tale ragione, si riporta qui una copia delle due
   routines.
   ----------------------------------------------------------------------------
*/


void *xmemset(void *b, int c, size_t len)
{
	register void *pRet = b;
	register unsigned char *ub = (unsigned char *) b;

	while(len > 0)
	{
		*ub++ = (unsigned char) c;
		len--;
	}

	return pRet;
}

void *xmemcpy(void *dst, const void *src, size_t len)
{
	register void *pRet = dst;
	register char *usrc; 
	register char *udst;

    usrc = (const char *) src;
    udst = (char *) dst;

	while(len > 0)
	{
		*udst++ = *usrc++;
		len--;
	}

	return pRet;
}







/* 
   -----------------------------------------------------------------------------
   ROUTINES INTERNE AL PRX
   -----------------------------------------------------------------------------
*/


void USB_PRX_INTERNAL_ConditionalWindowsRender ()
/*
    Quando avviene un cambiamento dello stato della porta USB, la visualizzazione della
    nuova icona può avvenire esclusivamente nell'ambito di un WindowsRender, perchè
    in quella fase viene eseguita una nuova DrawMinWindowsBar. 
    
    Però, il driver PRX USB viene inizializzato ed avviato prima dell'esecuzione di
    ndInitSystem, quindi se all'avvio è mantenuto inserito il connettore della presa USB,
    il driver PRX USB potrebbe tentare di eseguire un WindowsRender prima dell'esecuzione
    del sistema grafico, e questo potrebbe provocare un blocco. 
    
    Per tale ragione, la CommArea contiene un campo GraphSysIsLocked: quando tale campo
    è posto ad 1, significa che il sistema grafico non è ancora stato reinizializzato, 
    e quindi il driver PRX attende che quest'ultimo sia pronto prima di richiedere
    un WindowsRender.
*/
{
    void (*PntToWindowsRender)(void);
    
    if (PntCommArea->GraphSysIsLocked)
    {
       while (PntCommArea->GraphSysIsLocked) sceDisplayWaitVblankStart();
       // Ciclo di attesa valido finchè il sistema grafico non si è reinizializzato
    
       sceKernelDelayThread (100000);
       // Ciclo di ritardo necessario per stabilizzare il sistema
    }
    
    PntToWindowsRender = PntCommArea->PntWindowsRender;
    // Copia il puntatore: si prepara alla chiamata di funzione
    
    (*PntToWindowsRender)();
    // Richiama la funzione WindowsRender consentendo la visualizzazione della nuova icona
}



void USB_PRX_INTERNAL_SignalUsbAttach ()
{
     while (!CommAreaInit) sceDisplayWaitVblankStart();
     // Questo loop impedirà che il driver PRX acceda alla CommArea, prima che
     // Nanodesktop abbia comunicato al driver il suo esatto indirizzo
     
     PntCommArea->UsbState          = USBSTATE_ATTACHED;
     PntCommArea->UsbStateIsChanged = 1; 
     
     USB_PRX_INTERNAL_ConditionalWindowsRender ();
}



void USB_PRX_INTERNAL_SignalUsbDetach ()
{
     while (!CommAreaInit) sceDisplayWaitVblankStart();
     // Questo loop impedirà che il driver PRX acceda alla CommArea, prima che
     // Nanodesktop abbia comunicato al driver il suo esatto indirizzo
     
     PntCommArea->UsbState          = USBSTATE_DETACHED;
     PntCommArea->UsbStateIsChanged = 1; 
     
     USB_PRX_INTERNAL_ConditionalWindowsRender ();
}



void USB_PRX_INTERNAL_SendMessageToNd (int MessageCode, const char *format, ...)
/*
    Questa routine viene utilizzata dal driver PRX per inviare un messaggio
    al sistema grafico di Nanodesktop. Un messaggio è composto da un numero
    a 32 bit, che indica il tipo di messaggio, seguito da una stringa
    complessa (simile a quella che viene normalmente usata nelle routines
    printf e derivate), che contiene un testo. 
    
    La routine memorizzerà il codice di messaggio e la stringa complessa
    dentro la communication area. Dopo, però, **non** viene richiamata
    la routine grafica di Nanodesktop per visualizzare il messaggio. 
    
    Il motivo è semplice: la presente routine fa parte del contesto dell'usb-thread,
    e deve potere essere eseguita molto velocemente, se si vogliono evitare
    blocchi di sistema. 
    
    Supponiamo infatti che una prima callback USB cerchi di visualizzare un
    proprio messaggio di debug sullo schermo. 
    
    Il processo di visualizzazione non è a tempo nullo. Richiede un certo tempo
    T per essere completato. 
    
    Se durante questo tempo T intervenisse una seconda callback per visualizzare
    un secondo messaggio, si verificherebbe un blocco di sistema.
    
    Per evitare questo, è previsto un meccanismo di arbitraggio che consente
    la visualizzazione di quasi tutti i messaggi di debug USB senza provocare
    blocchi di sistema. 
    
    Quando l'utente avvia il debug dei messaggi USB usando la funzione 
    ndHAL_USB_EnableDriverMonitor, il sistema avvia un thread utente che verifica
    costantemente la CommArea alla ricerca di nuovi messaggi da visualizzare. 
    
    Nel frattempo, ogni volta che una callback USB richiama la routine 
    USB_PRX_INTERNAL_SendMessageToNd, il sistema controlla la variabile
    MemoryLockState nella CommArea. Solo se il suo valore è 0, la callback
    USB provvede a portarne il valore a 2 (impedendo ad altre callback di
    visualizzare il proprio messaggio finchè il processo di visualizzazione
    non è stato completato). Quando USB_PRX_INTERNAL_SendMessageToNd ha
    finito di scrivere il messaggio nella CommArea, esso porta il valore
    di MemoryLockState ad 1 (le callback USB continuano a *non* potere
    visualizzare alcunchè), in modo che il thread UsbMonitor nel sistema
    grafico di Nanodesktop noti che c'è un nuovo messaggio in arrivo, e
    provveda a visualizzarlo. 
    
    Quando poi il thread USB Monitor di Nanodesktop termina il processo di
    visualizzazione, esso porta a 0 la variabile MemoryLockState nella
    CommArea abilitando la visualizzazione di un nuovo messaggio da parte
    del sistema USB. 
    
    Le callback USB che trovano il sistema di visualizzazione grafica 
    impegnato (MemoryLockState = 1 o = 2), non attendono che il sistema
    grafico si liberi (altrimenti il bus USB sarebbe rallentato), ma
    semplicemente skippano la visualizzazione del messaggio USB. 
*/
{
      
   va_list args;                // Lista degli argomenti da passare. Il tipo va_list indica una
                                // lista di parametri passati alla funzione per mezzo di un 
                                // ellissi. La funzione che vuole accedere ai singoli parametri
                                // deve usare la macro va_arg (puntatore alla lista, tipo parametro)
                                // per ottenere i vari parametri uno alla volta
                                      
   struct s_strargument xarg;   // Struttura argomenti: contiene i parametri necessari
                                // per la corretta programmazione del funzionamento di
                                // vxprintf (ad esempio, l'indirizzo della stringa dove
                                // è necessario scrivere e l'eventuale limite dei
                                // caratteri da scrivere.
  
   // Puntatori a funzione
  
   int    (*PntToVxPrintf)(void (*func)(char *, int, void *), void *arg, const char *format, va_list ap);
          
   int ErrRep1;
    
   if (CommAreaInit)            // Se la CommArea non è inizializzata skip 
   {   
       if (PntCommArea->MonitorEnabled)   // Se il monitor non è abilitato skip
       {
                                  
               if (!PntCommArea->GraphSysIsLocked)    // Se il sistema grafico è ancora locked skip    
               {
                           
                           if (PntCommArea->MemoryLockState==0)      // Provvedi a scrivere nella CommArea solo se il sistema di visualizzazione
                           {                                         // è libero
                                      PntCommArea->MemoryLockState=2;              
                                          
                                      PntCommArea->MessageCode = MessageCode;
                                      
                                      
                                      xarg.next = &(PntCommArea->StrMessage[0]);    // Preparati a programmare la snprintf in modo che scriva
                                                                                    // nella communication area, campo StrMessage. 
                                      
                                      xarg.last = &(PntCommArea->StrMessage[500]);  // setta l'indirizzo dell'ultimo elemento
                                                                                    // del buffer in modo da evitare buffer overflow.       
                                                                      
                                      va_start(args, format);                       // Copia in args la lista di parametri a passati alla routine
                                                                                    // a partire da format
                                                                    
                                      
                                      PntToVxPrintf = PntCommArea->PntVxPrintf;     // Copia il puntatore
                                      
                                      
                                      ErrRep1 = (*PntToVxPrintf)(PntCommArea->PntSout, &xarg, format, args);   
                                                                                     
                                     // Il fatto che venga richiamato vxprintf usando
                                     // _INTERNAL_WriteFncToString_  come routine di scrittura, significa che
                                     // stiamo eseguendo una snprintf. xarg fornirà alla
                                     // snprintf le informazioni necessarie (locazione in
                                     // memoria della stringa da scrivere e numero di 
                                     // caratteri necessari. Stringa fornirà alla routine
                                     // l'indirizzo del primo argomento e args fornirà la
                                     // lista degli argomenti successivi.
                            
                                      va_end(args);         // Disalloca la lista degli argomenti 
                            
                                     // Avverti il thread USB in UserMode che c'è un messaggio
                                     // USB in attesa: le callback USB continuano ad avere inibito
                                     // l'accesso al sistema  
                                      
                                      PntCommArea->MemoryLockState = 1;
                               
                                       
                           }
                   
               }     
                 
       }
   }
}



/* 
   -----------------------------------------------------------------------------
   ROUTINES PUBBLICHE (queste routines sono esportate dal prx)
   -----------------------------------------------------------------------------
*/

void USB_PRX_PassCommAreaAdr (struct ndUsb_CommArea_Type *AdrArea)
{
   PntCommArea  = AdrArea;
   CommAreaInit = 1;  
}
