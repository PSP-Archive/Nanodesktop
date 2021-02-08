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
    FUNZIONI DI MANIPOLAZIONE STRINGHE
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

char Magn (char Carattere)
// Fornisce in uscita il corrispondente carattere maiuscolo
{
   if ( (Carattere>=97) && (Carattere<=122) )     // Se il carattere è minuscolo
      return (Carattere-32);
   else
      return Carattere;                           // Il carattere resta così com'è
}


char DeMagn (char Carattere)
// Fornisce in uscita il corrispondente carattere minuscolo
{
   if ( (Carattere>=65) && (Carattere<=90) )     // Se il carattere è minuscolo
      return (Carattere+32);
   else
      return Carattere;                           // Il carattere resta così com'è
}



void ndStrCpy (char *StringaDest, char *StringaSrc, unsigned char Limit, char MakeMagn)
// Questa routine provvede a copiare una stringa. Viene copiato solo il numero
// di caratteri dati da Limit. Se MakeMagn viene posto ad 1, allora ogni carattere
// della stringa viene convertito da minuscolo a maiuscolo.

{
  char Counter;
  char Carattere;
  
  if (Limit>0)
  {
     Counter=0;         
              
     do
     {
        Carattere=*(StringaSrc+Counter);
        
        if (!(MakeMagn))
        {
          *(StringaDest+Counter)=Carattere;
        }
        else
        {
          *(StringaDest+Counter)=Magn(Carattere);
        }
     
        Counter++;  
     }
     while ( (Carattere!=0) && (Counter<=Limit-1) );
     
     *(StringaDest+Counter)=0;   // La stringa viene terminata
  }
  else
  {
      *StringaDest=0;
  }
}

void ndStrCpyBounded (char *StringaDest, char *StringaSrc, unsigned char LimitX1, unsigned char LimitX2, char MakeMagn)
// Questa routine provvede a copiare una stringa. Vengono copiati solo i caratteri
// di nr. ordine compreso tra LimitX1 e LimitX2.
// Se MakeMagn viene posto ad 1, allora ogni carattere
// della stringa viene convertito da minuscolo a maiuscolo.

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
          *(StringaDest+Counter)=Magn(Carattere);
        }
     
        Counter++;  
  }
  while ( (Carattere!=0) && (Counter<=Limit) );
     
  *(StringaDest+Limit+1)=0;   // La stringa di destinazione viene terminata
  
}


