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
    ROUTINES PER LA GENERAZIONE DI CHIAVI
    ----------------------------------------------------------------------------
    Queste routines si occupano di generare chiavi a 64 bit per le routine di
    Nanodesktop, come ndWS_DrawSpRectangle, che ricevono parametri 
    attraverso questa tecnica.
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

#define BIT10 0x0200

inline ndint64 NDKEY_ROTATE (float Degree)
/*
    Genera il codice a 64 bit che imporrà la rotazione desiderata a routines come 
    ndDrawRectangle o come ndDrawEllipse e derivate. 
    
    Il parametro Degree deve essere compreso tra -180 e +180. Se si usano valori
    differenti, allora verrà emesso un codice che non provocherà alcuna rotazione.
*/

{
    ndint64 Temp1, Temp2;
    unsigned short int AbsCode;
    
    if ((Degree>=-180.0) && (Degree<=+180.0))
    {
       AbsCode = (unsigned short int) ( fabs (Degree) * 511.0/180.0);
       
       Temp1=0;
       
       Temp2 = AbsCode;
       if (Degree<0) Temp2=Temp2 | BIT10;
       
       Temp2 = Temp2 << (63-1-9);
       Temp1 = Temp1 | Temp2; 
       
       return Temp1;
    }
    else
     return 0;
}

inline ndint64 NDKEY_BORDER_SIZE (char Border)
/*
    Genera il codice a 64 bit che imporrà una certa larghezza al bordo dell'elemento.
    
    Border deve avere un valore compreso tra 0 e 15. Se si usa un valore differente,
    la routine restituirà zero: questo farà sì che l'elemento assuma automaticamente
    un valore di ampiezza per il bordo esterno di 1 pixel. 
*/

{
    ndint64 Temp1, Temp2;
    
    if ((Border>1) && (Border<=15))
    {
        Temp1 = 0;
     
        Temp2 = Border;
        Temp2 = Temp2 << (63-1-9-4);
        
        Temp1 = Temp1 | Temp2;
        return Temp1;
    }
    else
     return 0;
}



inline ndint64 NDKEY_FONT (unsigned char FntHandle)
/*
    Genera il codice a 64 bit che imporrà un certo font.
*/

{
    ndint64 Temp1;
    
    Temp1 = FntHandle;
    Temp1 = Temp1 << (31-8);
    return Temp1;
}


inline ndint64 NDKEY_POSX (unsigned short int PosX)
/*
     Genera il codice a 64 che è destinato a contenere una posizione. 
     La posizione consentita è un numero tra 0 e 2046 
*/
{
  unsigned short int _PosX;
  ndint64 Temp;
  
  if (PosX<2046)
  {
     _PosX = PosX + 1;        // Il codice key pari a 0 è riservato
  
     Temp = PosX & 0x7FF;
     Temp = Temp << 11;
     return Temp;
  }
  else           // Condizione di errore: restituire codice 0
    return 0;     
}

inline ndint64 NDKEY_POSY (unsigned short int PosY)
/*
     Genera il codice a 64 che è destinato a contenere una posizione. 
     La posizione consentita è un numero tra 0 e 2046 
*/
{
  unsigned short int _PosY;
  ndint64 Temp;
  
  if (PosY<2046)
  {
     _PosY = PosY + 1;        // Il codice key pari a 0 è riservato
  
     Temp = PosY & 0x7FF;
     return Temp;
  }
  else           // Condizione di errore: restituire codice 0
    return 0;     
}

ndint64 NDKEY_SETTEXTAREA (int PosX, int PosY)
{
   ndint64 A, B;
   
   if ((PosX>=0) && (PosX<65536))
      A = PosX & 0xFFFF;
   else
      A = 0;
   
   if ((PosY>=0) && (PosY<1048576))
      B = (ndint64)(PosY & 0x1FFFFFF) << 31;
   else
      B = 0;
  
   return (A | B);
}

inline ndint64 NDKEY_SETFONT (unsigned char FntHandle)
/*
    Genera il codice a 64 bit che imporrà il font testuale di una finestra
    in ndLP_CreateWindow
*/

{
    ndint64 Temp1;
    
    Temp1 = FntHandle;
    Temp1 = Temp1 << (31-8);
    return Temp1;
}

inline ndint64 NDKEY_SETTITLEFONT (unsigned char FntHandle)
/*
    Genera il codice a 64 bit che imporrà il font del titolo di una finestra
    in ndLP_CreateWindow
*/

{
    ndint64 Temp1;
    
    Temp1 = FntHandle;
    Temp1 = Temp1 << (23-8);
    return Temp1;
}

inline ndint64 NDKEY_SETMENUFONT (unsigned char FntHandle)
/*
    Genera il codice a 64 bit che imporrà il font del menu di una finestra
    in ndLP_CreateWindow
*/

{
    ndint64 Temp1;
    
    Temp1 = FntHandle;
    Temp1 = Temp1 << (15-8);
    return Temp1;
}


inline ndint64 NDKEY_SETWSLENGTH (unsigned short int WSLength)
/*
    Genera il codice a 64 bit che imporrà la lunghezza del windows space in una finestra
    in ndLP_CreateWindow
*/

{
    ndint64 Temp1;
    
    Temp1 = WSLength & 2047;
    Temp1 = Temp1 << (63-11);
    return Temp1;
}


inline ndint64 NDKEY_SETWSHEIGHT (unsigned short int WSHeight)
/*
    Genera il codice a 64 bit che imporrà la larghezza del windows space in una finestra
    in ndLP_CreateWindow
*/

{
    ndint64 Temp1;
    
    Temp1 = WSHeight & 2047;
    Temp1 = Temp1 << (63-11-11);
    return Temp1;
}


inline ndint64 NDKEY_SETFNTROTATE (int Rotation)
/*
    Questa il codice a 64 bit che imporrà la larghezza del font. Il codice
    è compreso tra -180 e 180 gradi
*/
{
    ndint64 Temp1;
    
    Temp1 = abs (Rotation) & 0xFF; 
    if (Rotation<0) Temp1 = Temp1 | 0x100;
    
    Temp1 = Temp1 << (63-9);
    return Temp1;
}

inline ndint64 NDKEY_SETFNTSIZE (unsigned char Size)
/*
    Questa il codice a 64 bit che imporrà la larghezza del font. Il codice
    è compreso tra 0 e 256
*/
{
    ndint64 Temp1;
    
    Temp1 = Size;
    Temp1 = Temp1 << (63-9-8);
    
    return Temp1;
}
