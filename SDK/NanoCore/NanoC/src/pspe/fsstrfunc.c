/*
   --------------------------------------------------------------------------
   fsstrfunc.c                                      NanoC Files (PSPE)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di manipolazione stringa per il supporto fs nanodesktop (header)
   ----------------------------------------------------------------------------
*/   
 
#include <pspe/$$_ded_fsstrfunc.h>


/*
   ----------------------------------------------------------------------------
   PROTOTIPI DI ROUTINES NANODESKTOP CHE SARANNO UTILIZZATE 
   ----------------------------------------------------------------------------
*/

    extern char Magn (char Carattere); 
    extern char DeMagn (char Carattere); 
    
    extern void ndStrCpy (char *StringaDest, char *StringaSrc, unsigned char Limit, char MakeMagn); 
    extern void ndStrCpyBounded (char *StringaDest, char *StringaSrc, unsigned char LimitX1, unsigned char LimitX2, char MakeMagn); 
    extern void StringNumber (long int Numero, char *StrPointer); 
    
/*
   ----------------------------------------------------------------------------
   ROUTINES
   ----------------------------------------------------------------------------
*/



void fssAppendExtension (char *NameFile, char *Ext)
/*
   Questa routine verifica se l'estensione non è vuota e, se è così, provvede a fare
   l'append su NameFile.
*/
{
   if ( (strcmp (Ext, "")!=0) )
   {
        strcat (NameFile, ".");
        strcat (NameFile, Ext);
   }
}


void fssPutSlashAtEnd (char *NameFile)
/*
    Verifica se c'è uno slash alla fine e, qualora
    non esista, lo aggiunge alla stringa NameFile.
*/
{
  unsigned char NrCaratteri;
  unsigned char Carattere;

  NrCaratteri = strlen (NameFile);

  Carattere = *(NameFile+NrCaratteri-1);
  // Preleverà il carattere immediatamente precedente il NULL di fine stringa

  if (Carattere != '/')
     strcat (NameFile, "/");
}

void fssStringDate (int day, int month, int year, char *Stringa)
{
   char StrDay   [12];
   char StrMonth [12];
   char StrYear  [12];
         
   StringNumber (day,   StrDay);
   StringNumber (month, StrMonth);
   StringNumber (year,  StrYear);
      
   if (day<=9) *(Stringa+0)='0'; 
     else *(Stringa+0)=StrDay [9];
   
   *(Stringa+1)=StrDay   [10];
   
   *(Stringa+2)='/';
   
   if (month<=9) *(Stringa+3)='0'; 
     else *(Stringa+3)=StrMonth [9];
   
   *(Stringa+4)=StrMonth [10];
   
   *(Stringa+5)='/';
   
   *(Stringa+6)=StrYear   [9];
   *(Stringa+7)=StrYear  [10];
   *(Stringa+8)=0;
}

void fssSplitNameFile (char *NomeCompleto, char *Percorso, char *Nome, char *Ext)
{
 unsigned char LenStringa;
 unsigned char BeginSearch;
 int           Counter;
 char          Carattere;
 
 unsigned char IHaveFoundSlash; 
 unsigned char PosSlash = 0;
 unsigned char IHaveFoundPoint; 
 unsigned char PosPoint = 0;
 
 char InternalString [255];
 
 LenStringa = strlen (NomeCompleto);
 
 if (*(NomeCompleto+LenStringa-1)=='/')
    BeginSearch=LenStringa-2;
 else
    BeginSearch=LenStringa-1;
    
 IHaveFoundSlash=0;  
    
 for (Counter=BeginSearch; ((Counter>=0) && (IHaveFoundSlash==0)); Counter--)
 {
     Carattere=*(NomeCompleto+Counter);
     
     if (Carattere=='/')
     {
       IHaveFoundSlash=1;
       PosSlash=Counter;
     }
 }                
 
 if (IHaveFoundSlash)
 {
   ndStrCpyBounded (Percorso,       NomeCompleto, 0,          PosSlash, 0);        // Copia dentro percorso il nome del percorso con lo slash finale
   ndStrCpyBounded (InternalString, NomeCompleto, PosSlash+1, LenStringa-1, 0);    // Copia il resto dentro InternalString
 }
 else
 {
   ndStrCpyBounded (InternalString, NomeCompleto, 0, LenStringa-1, 0);      // Copia tutto dentro InternalString
 }
 
 if ( (strcmp (InternalString, ".")!=0) && (strcmp (InternalString, "..")!=0) )
 {
      LenStringa = strlen (InternalString);
      
      IHaveFoundPoint=0;
      BeginSearch=strlen (InternalString)-1;
         
      for (Counter=BeginSearch; ((Counter>=0) && (IHaveFoundPoint==0)); Counter--)
      {
        Carattere=*(InternalString+Counter);
     
        if (Carattere=='.')
        {
           IHaveFoundPoint=1;
           PosPoint=Counter;
        }
      }
         
      if (IHaveFoundPoint)      // Il punto è stato trovato
      {
         if ( (PosPoint>0) && (PosPoint<LenStringa-1) )                  // Il punto non è nè all'inizio, nè alla fine
         {
            ndStrCpyBounded (Nome, InternalString, 0, PosPoint-1, 0);
            ndStrCpyBounded (Ext,  InternalString, PosPoint+1, LenStringa-1, 0);
         }
         
         if (PosPoint==0)          // Il punto è all'inizio
         {
            strcpy (Nome, "");
            ndStrCpyBounded (Ext, InternalString, 1, LenStringa-1, 0);
         }
         
         if (PosPoint==LenStringa-1)   // Il punto è alla fine
         {
            ndStrCpyBounded (Nome, InternalString, 0, LenStringa-2, 0);
            strcpy (Ext, "");
         }     
      }
      else     // Non è stato trovato il punto nel nome file
      {
         strcpy (Nome, InternalString);
         strcpy (Ext, "");
      }
            
 }                
 else
 {
     strcpy (Nome, InternalString);
     strcpy (Ext, "");
 } 
                                     
}


int fssIsAbsoluteName (char *LongName)
{
    int Counter, Len;
    
    Len = strlen (LongName);
    
    if (Len>=2)
    {
       for (Counter=0; Counter<Len-2; Counter++)
       {
           if ((LongName [Counter]==':') && (LongName [Counter+1]=='/'))
           {
               return 1;
           }
       }
       
       // Se il sistema giunge a questo punto, significa che la sequenza :/
       // che indica un nome assoluto non è presente
       
       return 0;        
    }
    else      // Ci sono meno di 2 caratteri: questo non è un nome assoluto
    {
       return 0;
    }
}


int fssGenerateLongName (char *Path, char *Name, char *Dest)
/*
    Lo scopo di questa routine è generare un nome lungo, ossia un
    nome che contenga sia il Path della piattaforma, che il nome 
    e l'estensione del singolo file. La routine è progettata
    per supportare anche names del tipo ..\..\abc.txt. In 
    questo caso, la routine terrà conto dei simboli di change
    dir impliciti nel generare il nome lungo

    La routine restituisce 0 in caso di successo e -1 in caso
    di errore
*/
{
    int Counter, LenName, LenPath;
    char IHaveFound;
    
    char __Path  [128];
    char __Name  [128];
    char __Temp0 [128];
    char __Temp1 [128];
    
    // Lunghezza len
    
    LenName = strlen (Name);
    LenPath = strlen (Path);
    
    // Copia Path e Name nei due buffer interni in attesa di
    // elaborazione
    
    ndStrCpy ( (char *)(&__Path), Path, LenPath, 0);
    ndStrCpy ( (char *)(&__Name), Name, LenName, 0);
    
    // Per prima cosa, è necessario verificare che Name non
    // sia un nome assoluto es. ms0:/pippo/pluto.txt
    
    if (!fssIsAbsoluteName (   (char *)(&__Name)   ))
    {
        // Ora cerchiamo la sequenza ./ iniziale
        
        if (LenName>=2)
        {
            if ((__Name [0]=='.') && (__Name [1]=='/'))
            {
                 // Ok, è presente un ./ iniziale: toglilo 
                 ndStrCpyBounded ( (char *)(&__Temp0), (char *)(&__Name), 2, LenName, 0);  
                 
                 fssPutSlashAtEnd (__Path);
                 return fssGenerateLongName ( (char *)(&__Path), (char *)(&__Temp0), Dest);
                   // Richiama ricorsivamente la medesima routine. Si noti che ora
                   // in __Path c'è un percorso che termina con /, mentre in
                   // __Temp0 ci dovrebbe essere un nome semplice. La routine
                   // richiamata eseguirà il cat
            }       
        }                 
        
        if (LenName>=3)
        {
            if ( (__Name [0]=='.') && (__Name [1]=='.') && (__Name [2]=='/') )
            {
                 // Ok, è presente un ../ iniziale: intanto toglilo da name 
                 ndStrCpyBounded ( (char *)(&__Temp0), (char *)(&__Name), 3, LenName, 0);  
                 
                 // Adesso è necessario eseguire un chdir .. su __Path
                 Counter = LenPath-1;                     // Ok: punta sul penultimo carattere di path
                 
                 if (__Path [Counter]=='/') Counter--;    // Se l'ultimo carattere di path è uno slash punta
                                                          // ancora indietro
                                                          
                 IHaveFound=0;
                 
                 while (Counter>=0)
                 {
                       if (__Path [Counter]==':')
                       {
                             // Stai tentando di eseguire un chdir .. nella directory radice. 
                             // Ritorna -1 alla routine chiamante, perchè c'è un errore     
                       
                             return -1;
                       }
                       
                       if (__Path [Counter]=='/')
                       {
                             IHaveFound=1;
                             break;              // In Counter ora c'è la posizione del penultimo slash     
                       }
                       
                       Counter--;
                 }
                 
                 if (IHaveFound)                 // Ha trovato uno slash
                 {
                      ndStrCpyBounded ( (char *)(&__Temp1), (char *)(&__Path), 0, Counter, 0);
                      // In Temp1, in questo momento, c'è il path che termina con uno slash
                      
                      return fssGenerateLongName ( (char *)(&__Temp1), (char *)(&__Temp0), Dest);
                      // Richiama ricorsivamente la medesima funzione. Si noti che ora
                      // in path c'è un percorso che è di ordine inferiore rispetto
                      // al precedente. La routine richiamata eseguirà il cat                   
                 }
                 else
                 {
                      // Non è stato trovato uno slash: è una condizione anomala che 
                      // indica un qualche errore
                      
                      return -2;
                 }
            }             
        }
        
        // Ok, il nome non inizia nè con ./ nè con ../
        
        strcpy (  (char *)(&__Temp0), Path  );
        
        fssPutSlashAtEnd (  (char *)(&__Temp0)  );
        strcat (   (char *)(&__Temp0),  (char *)(&__Name)   );
        
        strcpy (Dest, (char *)(&__Temp0) );
        return 0;
    }
    else     // E' un nome assoluto
    {
        strcpy (Dest, (char *)(&__Name) );   
        return 0;
    }
}




