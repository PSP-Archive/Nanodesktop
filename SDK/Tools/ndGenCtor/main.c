/*
     UTILITY NDGENCTOR
     by Filippo Battaglia
     
     Questa routine si occupa di risolvere un problema di compatibilità del C++
     in ambiente PSPE. Il problema è il seguente: il compilatore non provvede
     a reinizializzare i costruttori ed i distruttori globali del C++. 
     
     Non è stato possibile risolvere il problema neanche con collect2, pertanto
     si è deciso di scrivere un manager dedicato in grado di gestire in modo
     corretto tali costruttori. 
     
     In fase di compilazione, per ogni file C++ compilato per PSPE, viene
     inserita una coppia di routines all'interno del flusso codice. Questa
     coppia di routines costituisce l'extramanager per i costruttori e
     distruttori globali. L'extramanager è costituito sempre dalla medesime
     due routines (CTORMGR_PSPE_do_global_ctors_aux e CTORMGR_PSPE_do_global_dtors_aux).
     
     Ogni file oggetto .o contiene una coppia di tali routines, sempre caratterizzate
     dallo stesso nome. Le routines sono statiche, pertanto non si verificano 
     problema di linkaggio, visto che ogni routine resta interna al namespace
     del singolo file oggetto .o.
     
     Il codice delle due routines viene inoculato automaticamente in fase di
     compilazione: è contenuto nel file ndHAL_CTORExtraManager.hpp.
     
     L'inoculazione può essere comunque inibita attraverso la direttiva al
     compilatore #define NDHAL_NO_CTOR_DTOR_EXTRAMANAGER, da porre prima 
     di #include <nanodesktop.h>  
     
     In fase di compilazione, MakeFile_PSPE esegue le seguenti operazioni
     
     a) esegue la compilazione dei vari sorgenti che costituiscono il progetto
     b) richiama l'utility ndGenCTor in modalità --fake. In questa modalità, 
        l'utility genera un semplice file C chiamato ndpspe_ctor_matrix.c. 
        
        Questo file .c si limita a contenere la definizione di un array 
        e di una funzione fake (InitializeConstructorMatrix), che in realtà
        non espletano nessuna funzione
        
     c) il file precedente viene ricompilato in modo da ottenere un file .o.
        Questo viene linkato una prima volta agli altri moduli, in modo da
        soddisfare le dipendenze del linker ed ottenere una versione provvisoria
        dell'eseguibile C++.
        
     d) MakeFile_PSPE.mak richiama automaticamente l'utility psp-nm, che fornisce
        in output gli indirizzi delle varie routines di costruttori e distruttori
        che fanno parte dell'eseguibile. In genere, per ogni oggetto CPP c'è sempre
        almeno una routine che gestisce i costruttori globali ed una routine che
        gestisce i distruttori globali. 
        
     e) MakeFile_PSPE.mak richiama di nuovo questa utility (NDGENCTOR), che provvede a leggere
        il file mappa, ed a generare un nuovo file ndpspe_ctor_matrix.c. Stavolta
        questo file non contiene solo la definizione degli array RELADR_Cpp_Constructors
        e RELADR_Cpp_Destructors insieme ad una piccola chiamata a memset per
        portare a 0 tutto l'array. In questo caso, nel sorgente C sono riportati
        anche gli indirizzi di ciascun extramanager relativo ai vari oggetti. Questi
        indirizzi (sia per l'extramanager che richiama tutti i costruttori globali
        C++, che per l'extramanager che richiama tutti i distruttori globali),
        sono inseriti per assegnazione dentro l'array. 
        
        Il risultato di questo processo, è che si forma un file ndpspe_ctor_matrix.c 
        che contiene la routine C InitializeConstructorMatrix. Quando Nanodesktop
        richiamerà questa routine, quest'ultima provvederà a riportare in un
        array di indirizzi, l'indirizzo dei vari extramanager legati ai differenti
        objects che costituiscono il programma  
        
     f) Il file C ndpspe_ctor_matrix.c viene compilato automaticamente da psp-gcc
        ed il relativo file oggetto viene linkato ai moduli che costituiscono
        l'applicazione PSPE finale
        
     g) All'avvio dell'homebrew per PSPE, ndInitSystem, poco prima di terminare
        e tornare al codice chiamante, richiama il gestore statico di costruttori
        globali (ndHAL_ExecuteCPPGlobalConstructors. E' definito dentro 
        ndHAL_CppConstructors.c). Questo manager legge nella matrice definita
        da ndpspe_ctor_matrix.c l'indirizzo dei vari extramanager associati 
        a vari files oggetto in C++ che fanno parte del programma. E, sulla base
        di tale tabella, ndInitSystem inizializza i vari costruttori globali
        ad uno ad uno. 
        
     h) L'avvio dei distruttori globali si verifica quando l'utente esce dal
        programma per mezzo della routine ndHAL_SystemHalt (oppure, per mezzo
        della routine exit(x) che la richiama internamente
        
     L'utente può disabilitare questo funzionamento inserendo, prima del
     #include <nanodesktop.h> la direttiva
     
     #define ND_NO_GLOBAL_CTOR_DTOR_INIT
     
     Un caso tipico in cui si rende necessario fare questo è quando PSPE
     crasha durante l'esecuzione di ndInitSystem
*/


#include <stdio.h>
#include <stdlib.h>


char StringBuffer  [512];
char StringBuffer0 [512];
char StringBuffer1 [512];
char StringBuffer2 [512];

int  YouCanExit;
int  Results;



void ndStrCpyBounded (char *StringaDest, char *StringaSrc, unsigned char LimitX1, unsigned char LimitX2, char MakeMagn)
{
  char Counter;
  char Carattere;
  char Limit;
  
  Limit=LimitX2-LimitX1;
  
  Counter=0;         
              
  do
  {
        Carattere=*(StringaSrc+LimitX1+Counter);
        
        if (!(MakeMagn))
        {
          *(StringaDest+Counter)=Carattere;
        }
        else
        {
          *(StringaDest+Counter)=toupper(Carattere);
        }
     
        Counter++;  
  }
  while ( (Carattere!=0) && (Counter<=Limit) );
     
  *(StringaDest+Limit+1)=0;   // La stringa di destinazione viene terminata
  
}

/*
**
**  [func] - _itoa.
**  [desc] -
**  [entr] - int n; the integer value to convert.
**           char *buf; the pointer to the destination memory buffer.
**           int radix; the conversion number base.
**  [exit] - char *; buf.
**  [prec] - buf is a valid memory pointer.
**  [post] - the memory pointed to by buf is modified.
**
*/
char *_itoa(int n, char *buf, int radix)
{
  char         *ret = buf;
  char         tmp[33];
  int          i = 0, j, r;

  /* validate the conversion number base. */
  if ((radix >= 2) && (radix <= 36)) {
    if ((radix == 10) && (n < 0)) {
      /* negative integer value. */
      *buf++ = '-';
      n = -n;
    }
    do {
      /* calculate the current digit. */
      r = (int)((unsigned int)n % radix);
      tmp[i++] = ((r < 10) ? (r + '0') : (r - 10 + 'a'));
    } while ((n /= radix) != 0);
    /* reverse the buffer string. */
    for (--i, j = 0; (i >= 0); --i, ++j) buf[j] = tmp[i];
    buf[j] = 0;
  }
  return (ret);
}


int main(int argc, char *argv[])
{
  FILE *SrcFile;
  FILE *DestFile;
  char StrBuffer [16];
  int  NrConstrDestr;
  int  IsFake = 0;

  if ((argc==2) && (!strcmp (argv [1], "--fake"))) IsFake=1;

  DestFile = fopen ("ndpspe_ctor_matrix.c", "w");
      
  if (DestFile!=0)
  {
          // Inizia a creare il file matrice di indirizzi per i costruttori e distruttori
          
          fprintf (DestFile, " // Nanodesktop PSP Constructor and Destructor Matrix \n");
          fprintf (DestFile, " // Source Code (generated by ndGENCTOR)");
          fprintf (DestFile, "\n");
          fprintf (DestFile, "#include <nanodesktop.h> \n");
          
          fprintf (DestFile, " \n");
          fprintf (DestFile, "int RELADR_ndInitSystem; \n");
          fprintf (DestFile, " \n");
          fprintf (DestFile, "int RELADR_Cpp_Constructors [256]; \n");
          fprintf (DestFile, "int RELADR_Cpp_Destructors [256]; \n");
          fprintf (DestFile, " \n");
          
          fprintf (DestFile, "void InitializeConstructorMatrix (void) \n");
          fprintf (DestFile, "{ \n");
          fprintf (DestFile, "  RELADR_ndInitSystem = 0; \n");
          fprintf (DestFile, "  memset (&RELADR_Cpp_Constructors, 0, sizeof (RELADR_Cpp_Constructors)); \n");
          fprintf (DestFile, "  memset (&RELADR_Cpp_Destructors, 0, sizeof (RELADR_Cpp_Destructors)); \n");
          fprintf (DestFile, "\n");
          
          if (!IsFake)
          {
              SrcFile = fopen ("ndpspe_ctor.map", "r");
              
              if (SrcFile!=0)
              {
                  YouCanExit=0;
                  
                  while (!YouCanExit)
                  {
                        Results = fgets (StringBuffer, 511, SrcFile);
                  
                        if (Results==0) break;
                        
                        ndStrCpyBounded (StringBuffer0, StringBuffer, 0, 8, 0);
                        ndStrCpyBounded (StringBuffer1, StringBuffer, 11, strlen(StringBuffer)-2, 0);
                  
                        if (!strcmp (&StringBuffer1, "ndInitSystem"))
                        {
                            fprintf (DestFile, "  RELADR_ndInitSystem = 0x");
                            
                            //Togli lo spazio finale
                            ndStrCpyBounded (StringBuffer2, StringBuffer0, 0, strlen(StringBuffer0)-2, 0);
                            
                            fprintf (DestFile, "%s", StringBuffer2);
                            fprintf (DestFile, "; \n");   
                            break;
                        }
                  }
                  
                  fprintf (DestFile, "\n");
                  
                  // Adesso ricava gli indirizzi dei costruttori
                        
                  fseek (SrcFile, 0, SEEK_SET);
                  
                  YouCanExit=0;
                  NrConstrDestr=0;
                  
                  while (!YouCanExit)
                  {
                        Results = fgets (StringBuffer, 511, SrcFile);
                  
                        if (Results==0) break;
                        
                        ndStrCpyBounded (StringBuffer0, StringBuffer, 0, 8, 0);
                        ndStrCpyBounded (StringBuffer1, StringBuffer, 11, strlen(StringBuffer)-2, 0);
                  
                        if (!strcmp (&StringBuffer1, "CTORMGR_PSPE_do_global_ctors_aux"))
                        {
                            fprintf (DestFile, "  RELADR_Cpp_Constructors [");
                            
                            _itoa (NrConstrDestr, &StrBuffer, 10);
                            
                            fprintf (DestFile, StrBuffer);
                            fprintf (DestFile, "]=0x");
                  
                            //Togli lo spazio finale
                            ndStrCpyBounded (StringBuffer2, StringBuffer0, 0, strlen(StringBuffer0)-2, 0);
                            
                            fprintf (DestFile, "%s", StringBuffer2);
                            fprintf (DestFile, "; \n");   
                            
                            // Continua l'analisi
                            NrConstrDestr++;
                        }
                  }
                        
                  fprintf (DestFile, "\n");
                        
                  // Adesso ricava gli indirizzi dei distruttori
                        
                  fseek (SrcFile, 0, SEEK_SET);
                  
                  YouCanExit=0;
                  NrConstrDestr=0;
                  
                  while (!YouCanExit)
                  {
                        Results = fgets (StringBuffer, 511, SrcFile);
                  
                        if (Results==0) break;
                        
                        ndStrCpyBounded (StringBuffer0, StringBuffer, 0, 8, 0);
                        ndStrCpyBounded (StringBuffer1, StringBuffer, 11, strlen(StringBuffer)-2, 0);
                  
                        if (!strcmp (&StringBuffer1, "CTORMGR_PSPE_do_global_dtors_aux"))
                        {
                            fprintf (DestFile, "  RELADR_Cpp_Destructors [");
                            
                            _itoa (NrConstrDestr, &StrBuffer, 10);
                            
                            fprintf (DestFile, StrBuffer);
                            fprintf (DestFile, "]=0x");
                  
                            //Togli lo spazio finale
                            ndStrCpyBounded (StringBuffer2, StringBuffer0, 0, strlen(StringBuffer0)-2, 0);
                            
                            fprintf (DestFile, "%s", StringBuffer2);
                            fprintf (DestFile, "; \n");   
                            
                            // Continua l'analisi
                            NrConstrDestr++;
                        }
                  } 
                  
                  close (SrcFile);          
              }
              else
              {
                  close (DestFile);
                  exit (-2);
              }        
          
              
          }     
          
          fprintf (DestFile, "} \n");
          
          // Terminiamo il file di output        
      
          close (DestFile);
          exit (0);     
  }
  else
     exit (-1);
}
