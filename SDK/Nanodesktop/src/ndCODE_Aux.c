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
    FUNZIONI DI SERVIZIO
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void StringNumber (long int Numero, char *StrPointer)
// Questa funzione trasforma un numero di tipo longint in una stringa.
// Alla funzione deve essere passato il puntatore alla stringa, che
// deve obbligatoriamente essere un array di char di 12
// caratteri

{
   long int XNumero;
   int i, PrimaCifraTrovata;
   long int b, cifra;

   XNumero=abs(Numero);

   PrimaCifraTrovata=0;
   b=1000000000;

   for (i=1; i<11; i++)
   {
       cifra=XNumero/b;     // Questa funzione estrae la cifra voluta
       XNumero=XNumero % b; // A numero sostituisce il resto

       b=b/10;              // Scala il valore di B

       if (cifra!=0)
       {
          PrimaCifraTrovata=1;
          *(StrPointer+i)=48+cifra;
       }
       else
       {
             if (PrimaCifraTrovata==0)
             {
                if (i<10) *(StrPointer+i)=32;
                if (i==10) *(StrPointer+i)=48;
             }
             else
             {
                *(StrPointer+i)=48;
             }
       }
   }

   *(StrPointer+11)=0;

   if (Numero<0) *(StrPointer)=0x2D;    //Anteponi il segno - al numero
   else
       *(StrPointer)=32;                //Altrimenti metti uno spazio bianco per valori positivi
}


void ndDelay (float t)                  // Mantenuta solo per ragioni di compatibilità
{
  ndHAL_Delay (t);    
}

char INTERNAL_RettangoliDisgiunti (unsigned short aX1, unsigned short aY1, unsigned short aX2, unsigned short aY2,
                          unsigned short bX1, unsigned short bY1, unsigned short bX2, unsigned short bY2)
/* Questa funzione restituisce 1 se i due rettangoli di cui vengono fornite le coordinate sono
   disgiunti, altrimenti restituisce 0 */
{
  char DisgX, DisgY;

  DisgX = ( ( (aX2<bX1)&&(aX2<bX2) ) || ( (aX1>bX1)&&(aX1>bX2) ) );
  DisgY = ( ( (aY2<bY1)&&(aY2<bY2) ) || ( (aY1>bY1)&&(aY1>bY2) ) );

  return (DisgX || DisgY);
}



int ndRand(void)
{
	return rand();
} 



void UShort_Swap (unsigned short int *a, unsigned short int *b)
{
    unsigned short int Temp0, Temp1;
    
    Temp0=*a;
    Temp1=*b;
    
    *a=Temp1;
    *b=Temp0;
}


void Int_Swap (int *a, int *b)
{
    int Temp0, Temp1;
    
    Temp0=*a;
    Temp1=*b;
    
    *a=Temp1;
    *b=Temp0;
}


 
inline char PixelInRect (unsigned short int x,  unsigned short int y, 
                        unsigned short int x1, unsigned short int y1,
                        unsigned short int x2, unsigned short int y2)
{
   return ( (x>=x1) && (x<=x2) && (y>=y1) && (y<=y2) );
}




inline void INTERNAL_Rotate (int CenterX, int CenterY, float ZPixelX, float ZPixelY, float Rot_Radiant, int *ResultX, int *ResultY)
{
     if (Rot_Radiant==0)           // Non è stata richiesta alcuna rotazione
     {
         *ResultX = CenterX + (int)(ZPixelX);
         *ResultY = CenterY + (int)(ZPixelY);
     }
     else
     {
         *ResultX = CenterX + (int)(cosf (Rot_Radiant)*ZPixelX + sinf (Rot_Radiant)*ZPixelY + 0.5);
         *ResultY = CenterY + (int)(-sinf (Rot_Radiant)*ZPixelX + cosf (Rot_Radiant)*ZPixelY + 0.5); 
     }
} 

inline int INTERNAL_AllignValue (int Value, int BankSize)
{
   if ((Value % BankSize) == 0)
      return Value;
   else
      return (Value + (BankSize - (Value % BankSize))); 
}
