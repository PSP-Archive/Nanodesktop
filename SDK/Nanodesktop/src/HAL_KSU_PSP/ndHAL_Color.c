/*  
    ---------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2008-2013 Filippo Battaglia 
    ---------------------------------------------------------------------------
    Version for PSP/PSPE
    ---------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ---------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

    By downloading, copying, installing or using the software you agree to 
    Nanodesktop license, a modified version of BSD license. Read the file
    license.doc in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    ---------------------------------------------------------------------------
    ROUTINE DIPENDENTI DALLA PIATTAFORMA PER GESTIONE COLORI (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

TypeColor ndHAL_RGBToMagicNumber (unsigned char ChannelR, unsigned char ChannelG, unsigned char ChannelB)
/*
    Questa routine trasforma tre componenti RGB a 8 bit nel Magic Number, ossia
    nel numero che indica il corrispondente colore nella piattaforma utilizzata
    dalla libreria. 
*/

{
   return ((ChannelB & 0xF8)<<7) + ((ChannelG & 0xF8)<<2) + ((ChannelR & 0xF8)>>3) + 0x8000;
}

void ndHAL_MagicNumberToRGB (TypeColor MagicNumber, unsigned char *ChannelR, unsigned char *ChannelG, unsigned char *ChannelB) 
/*
    Questa routine trasforma un pixel di cui è noto il MagicNumber nello standard usato
    dalla piattaforma, nelle tre componenti RGB corrispondenti.
    
    Non sono previsti codici di errore per questa routine.
*/

{
   *ChannelR = (MagicNumber & 0x001F) << 3;
   *ChannelG = (MagicNumber & 0x03E0) >> 2;
   *ChannelB = (MagicNumber & 0x7C00) >> 7;          
}


TypeColor ndHAL_ConvertNDFtoMagicNumber (unsigned short int NDFValue)
/*
   Questa routine converte un colore pixel in formato NDF nel corrispondente
   Magic Number.
   
   Su alcune piattaforme, questo può essere più efficiente che non convertire
   NDF in RGB e poi RGB in MagicNumber.
*/ 

{
    return NDFValue;          // Nella piattaforma PSP gli NDFValue corrispondono ai
                              // MagicNumber.

}


void ndHAL_ConvertNDFToRGB (unsigned short int NDFValue, unsigned char *ChannelR, unsigned char *ChannelG, unsigned char *ChannelB) 
/*
    Questa routine trasforma un pixel di cui è noto il valore in formato NDF nello standard usato
    dalla piattaforma, nelle tre componenti RGB corrispondenti.
    
    Non sono previsti codici di errore per questa routine.
*/

{
   *ChannelR = (NDFValue & 0x001F) << 3;
   *ChannelG = (NDFValue & 0x03E0) >> 2;
   *ChannelB = (NDFValue & 0x7C00) >> 7;              
}



inline unsigned short int ndHAL_MathBlend (TypeColor Color1, TypeColor Color2)
/*
   Ricava il colore che si ottiene dal blend di due finestre sovrapposte.
*/

{
   unsigned short int c1R, c1G, c1B;
   unsigned short int c2R, c2G, c2B;
   unsigned short int cRT, cGT, cBT;
   
   c1R = ((Color1 & 0x001F)      ) & 0x1F;
   c1G = ((Color1 & 0x03E0) >> 5 ) & 0x1F;
   c1B = ((Color1 & 0x7C00) >> 10) & 0x1F;
   
   c2R = ((Color2 & 0x001F) >> 3 ) & 0x1F;
   c2G = ((Color2 & 0x03E0) >> 8 ) & 0x1F;
   c2B = ((Color2 & 0x7C00) >> 13) & 0x1F;
   
   cRT = c1R + c2R;
   cGT = c1G + c2G;
   cBT = c1B + c2B;
   
   if (cRT>0x1F) cRT=0x1F;
   if (cGT>0x1F) cGT=0x1F;
   if (cBT>0x1F) cBT=0x1F;

   return ( (cBT<<10)+(cGT<<5)+cRT+0x8000 );
}


char ndHAL_NDF_Identical_To_MGKNB (void)
/*
    Deve essere settata ad 1 sulle piattaforme in cui gli NDF a 16 bit sono identici ai MagicNumber
*/
{
    return 1;
}


