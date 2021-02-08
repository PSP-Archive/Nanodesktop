/*
   --------------------------------------------------------------------------
   stdioexec.c                                      NanoC Files (PSP)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines per esecuzione pseudo-main program 
   ----------------------------------------------------------------------------
*/   


#define DED_STDIOEXEC_I_NEED_VARIABLES
          #include <psp/$$_ded_stdioexec.h>
#undef  DED_STDIOEXEC_I_NEED_VARIABLES




extern void ndStrCpyBounded (char *StringaDest, char *StringaSrc, unsigned char LimitX1, unsigned char LimitX2, char MakeMagn); 
                                    

/* 
  -----------------------------------------------------------------------------------------
  PSEUDOEXEC
  -----------------------------------------------------------------------------------------
*/ 
   
int pseudoExec (void *MainRoutine, char *NameProgram, char *EnvStr, int *ErrorLevel)
/*
    Questa routine provvede ad eseguire una chiamata main emulata. 
    
    La routine MainRoutine, di cui viene passato l'indirizzo, ha
    prototipo
    
     int xmain (int argc, char **argv)
     
    Questa routine è la tipica routine main che apre un programma C.
    Normalmente, argc e **argv vengono passati dal sistema operativo,
    e permettono l'accesso ai parametri di ambiente del programma. 
    
    Nanodesktop permette di emulare questo comportamento, per mezzo
    della presente routine. E' sufficiente inserire in EnvStr
    la stringa da passare alla routine main, e Nanodesktop emulerà
    il passaggio dei parametri di ambiente. 
    
    Si consideri che il codice C standard che contiene una routine
    di start del tipo int xmain (int argc, char **argv) si aspetta
    che il primo termine nella stringa ambiente sia il nome del
    programma. Ad esempio, quando si passa a make la stringa:
               
               make.exe -f -B makefile.gcc
               
    La prima parola nella variabile di ambiente è make.exe. Una
    chiamata equivalente per pseudoExec sarebbe la 
    seguente:
             
               pseudoExec (&xmain, "make.exe", "-f -B makefile.gcc");
               
    La routine main normalmente restituisce al sistema operativo
    un codice di errore int detto, in Dos, ErrorLevel. Questo
    codice di errore viene memorizzato nella variabile int 
    puntata da ErrorLevel, salvo che non si ponga tale puntatore
    a zero. 
    
    pseudoExec restituisce 0 in caso di successo e -1 in caso
    di errore
*/
{
    // Puntatore a funzione
    int (*PntToMainFunc)(int argc, char **argv) = MainRoutine;
    
    struct pseudoExec_ArrayOfTheStrings_Type               *ArrayOfTheStrings;
    struct pseudoExec_ArrayOfThePointersToTheStrings_Type  *ArrayOfThePointersToTheStrings;
    
    char InputString [ND_MAXLEN_OF_ENVSTRING_OF_PSEUDOEXEC];
    int  Counter, NrElements, EL; 
    int  PosInString;
    
    char YouCanExit, StringScannerIsActive;
    int  PosX1, PosX2;
     
    
    ArrayOfTheStrings = malloc ( sizeof (struct pseudoExec_ArrayOfTheStrings_Type) );
    memset (&InputString, 0, ND_MAXLEN_OF_ENVSTRING_OF_PSEUDOEXEC);
    
    if (ArrayOfTheStrings)
    {
          ArrayOfThePointersToTheStrings = malloc ( sizeof(struct pseudoExec_ArrayOfThePointersToTheStrings_Type) );
          
          if (ArrayOfThePointersToTheStrings)
          {
              // Prepara la stringa InputString
              
              strcpy (InputString, NameProgram);
              strcat (InputString, " ");
              strcat (InputString, EnvStr);
              
              // Resetta le due struct
              
              for (Counter=0; Counter<ND_NR_PARAMS_PASSABLE_TO_PSEUDOEXEC; Counter++)
              {
                  strcpy (&(ArrayOfTheStrings->Str[Counter][0]), "");
                  ArrayOfThePointersToTheStrings->PntToStr [Counter] = &(ArrayOfTheStrings->Str[Counter][0]);
              }
              
              // Inizia a riempire la struct ArrayOfTheStrings 
              
              NrElements            = 0;
              PosInString           = 0;
              
              
              // Scarta gli spazi iniziali
              
              while (InputString [PosInString]==' ')
              {
                    PosInString++;
              }
              
              // Inizia l'analisi della stringa
              
              YouCanExit            = 0;
              StringScannerIsActive = 0;
              PosX1                 = 0;
              
              while (!YouCanExit)
              {
                    if (!StringScannerIsActive)       // La scansione della stringa non è attiva
                    {
                         if (InputString [PosInString]==0)          // Siamo a fine stringa
                         {
                            YouCanExit=1;
                            break;
                         } 
                         
                         if (InputString [PosInString]!=' ')        // Carattere diverso da uno spazio
                         {
                            StringScannerIsActive=1;                // Attiva la scansione della stringa
                            PosX1 = PosInString;                    // Registra la posizione della stringa
                         }    
                         
                         PosInString++;
                         continue;                               
                    } 
                    else                                // La scansione della stringa è già attiva
                    {
                         if ((InputString [PosInString]==' ') || (InputString [PosInString]==0))
                         {
                             PosX2 = PosInString-1;
                             ndStrCpyBounded ( &(ArrayOfTheStrings->Str[ NrElements ][0]), (char *)(&InputString), PosX1, PosX2, 0);
                             
                             StringScannerIsActive=0;
                             NrElements++;
                             
                             if (NrElements==ND_NR_PARAMS_PASSABLE_TO_PSEUDOEXEC)
                             {
                                 YouCanExit=1;
                                 break;                                               
                             }             
                         }
                         
                         if (InputString [PosInString]==0)
                         {
                             YouCanExit=1;
                             break;            
                         }
                         
                         PosInString++; 
                         continue;                              
                    }         
              }
                                       
              // ESEGUI LA CHIAMATA RICHIESTA
              
              EL = (*PntToMainFunc)(NrElements, (char **)(ArrayOfThePointersToTheStrings) );
              
              // Adesso libera la memoria dai buffer non più necessari
              free (ArrayOfThePointersToTheStrings);
              free (ArrayOfTheStrings);
              
              // Registra la variabile ErrorLevel in memoria
              if (ErrorLevel!=0) *ErrorLevel = EL;               
              
              // Ritorna 0 alla routine chiamante per segnalare che è tutto ok
              return 0;
          }
          else             // Problemi nella seconda allocazione  
          {
              free (ArrayOfTheStrings);
              return -1;
          }           
    }
    else             // Problemi nella prima allocazione
      return -1;
}





/* 
  -----------------------------------------------------------------------------------------
  GETOPT
  -----------------------------------------------------------------------------------------
*/ 



#define ERR(s, c) \
    if (opterr) {\
        char errbuf[2];\
        errbuf[0] = (char) c; errbuf[1] = '\n';\
        (void) fwrite (argv[0], strlen (argv[0]), 1, stderr);\
        (void) fwrite (s, strlen (s), 1, stderr);\
        (void) fwrite (errbuf, 2, 1, stderr);\
    }


int getopt( int argc, char *argv[], const char *opts )
{
   static int sp = 1;
   int c;
   char *cp;

   if ( sp == 1 )
      if ( optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0' )
         return ( EOF );
      else if ( strcmp( argv[optind], "--" ) == 0 )
      {
         optind++;
         return ( EOF );
      }
   optopt = c = argv[optind][sp];
   if ( c == ':' || ( cp = strchr( opts, c ) ) == NULL )
   {
      ERR( ": illegal option -- ", c );
      if ( argv[optind][++sp] == '\0' )
      {
         optind++;
         sp = 1;
      }
      return ( '?' );
   }
   if ( *++cp == ':' )
   {
      if ( argv[optind][sp + 1] != '\0' )
         optarg = &argv[optind++][sp + 1];
      else if ( ++optind >= argc )
      {
         ERR( ": option requires an argument -- ", c );
         sp = 1;
         return ( '?' );
      }
      else
         optarg = argv[optind++];
      sp = 1;
   }
   else
   {
      if ( argv[optind][++sp] == '\0' )
      {
         sp = 1;
         optind++;
      }
      optarg = NULL;
   }

   return ( c );

}  


/* 
  -----------------------------------------------------------------------------------------
  SLEEP
  -----------------------------------------------------------------------------------------
*/ 

void ndHAL_HardDelay (float DelaySec); 

unsigned sleep(unsigned seconds)
{
   ndHAL_HardDelay ( (float)(seconds) );
   return 0;      
}
