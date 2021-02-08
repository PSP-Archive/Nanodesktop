/*  
    ---------------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2008-2013 Filippo Battaglia 
    ----------------------------------------------------------------------------------
    Version for PSP/PSPE
    ----------------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

    By downloading, copying, installing or using the software you agree to 
    Nanodesktop license, a modified version of BSD license. Read the file
    license.doc in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    ----------------------------------------------------------------------------------
    ROUTINES PER LA GESTIONE DELLE IMMAGINI
    ----------------------------------------------------------------------------------
    Le immagini di NanoDesktop vengono memorizzate per mezzo di aree allocate 
    dinamicamente.
   
    Le informazioni sull'immagine vengono memorizzate dentro una struttura di tipo
    ndImage_Type. La medesima struttura contiene anche un campo dove viene memorizzato
    l'indirizzo dei dati in memoria.
   
    Le immagini di Nanodesktop possono essere di due tipi. In quelle di tipo NDMGKNB,
    vengono memorizzati esclusivamente i MagicNumber di ogni pixel. Ciò è di importanza
    fondamentale, perchè su alcune piattaforme ciò consente una riduzione del 30% dello
    spazio occupato.
   
    Nelle immagini di tipo NDRGB, invece, vengono memorizzati i valori dei canali R,G,B
    (con profondità di colore di 8 bit per canale).  
    -----------------------------------------------------------------------------------

*/

#include <$_NDforND.h>
    
char ndIMG_GetPixelFromImage (struct ndImage_Type *MyImage, unsigned short int PosX, unsigned short int PosY, TypeColor *MagicNumber)
/*
  Questa routine si occupa di fornire alla routine chiamante il colore del pixel di coordinate
  PosX, PosY dell'immagine MyImage. 
  
  Il colore del pixel viene fornito per mezzo del corrispondente
  Magic Number.
   
  Sono previsti i seguenti codici di errore.
  
    ERR_WRONG_PARAMETERS           PosX o PosY sono fuori immagine
    ERR_IMAGE_IS_NOT_ALLOCATED     L'immagine non è stata ancora allocata
*/

{
    unsigned short int LenX, LenY;
     
    unsigned char ChannelR, ChannelG, ChannelB;
    unsigned char *PointerToRam; 
    
    if ((MyImage->Data)!=0)
    {
       LenX = MyImage->LenX;
       LenY = MyImage->LenY;
    
       if ( (PosX<LenX) && (PosY<LenY)  )
       {
          switch (MyImage->ColorFormat)
          {
             case NDMGKNB:
             {
               *(MagicNumber) = *(MyImage->Data + PosY*LenX + PosX);  
               
               return 0;
             }
             
             case NDRGB:
             {
               PointerToRam = MyImage->Data;
               
               ChannelR = *(PointerToRam + 3*(PosY*LenX + PosX) + 0);
               ChannelG = *(PointerToRam + 3*(PosY*LenX + PosX) + 1);
               ChannelB = *(PointerToRam + 3*(PosY*LenX + PosX) + 2);
               
               *(MagicNumber) = ndHAL_RGBToMagicNumber (ChannelR, ChannelG, ChannelB);
               return 0;
             }
          
             default:
               return ERR_WRONG_PARAMETERS;
          }
          
       }
       else
          return ERR_WRONG_PARAMETERS;
    }
    else
     return ERR_IMAGE_IS_NOT_ALLOCATED;
}
    

char ndIMG_GetPixelFromImage_RGB (struct ndImage_Type *MyImage, unsigned short int PosX, unsigned short int PosY, 
                                  unsigned char *ChannelR, unsigned char *ChannelG, unsigned char *ChannelB)
/*
  Questa routine si occupa di fornire alla routine chiamante il colore del pixel di coordinate
  PosX, PosY dell'immagine MyImage. 
  
  Il colore del pixel viene fornito per mezzo delle sue coordinate RGB. 
  Sono previsti i seguenti codici di errore.
  
    ERR_WRONG_PARAMETERS        PosX o PosY sono fuori immagine
*/

{
    unsigned short int LenX, LenY;
    unsigned char *PointerToRam; 
    TypeColor MagicNumber;
    
    if ((MyImage->Data)!=0)
    {
       LenX = MyImage->LenX;
       LenY = MyImage->LenY;
    
       if ( (PosX<LenX) && (PosY<LenY)  )
       {
          switch (MyImage->ColorFormat)
          {
             case NDMGKNB:
             {
               MagicNumber = *(MyImage->Data + PosY*LenX + PosX);  
               ndHAL_MagicNumberToRGB (MagicNumber, ChannelR, ChannelG, ChannelB);
    
               return 0; 
             }
             
             case NDRGB:
             {
               PointerToRam = (unsigned char *) MyImage->Data;
               
               *(ChannelR) = *(PointerToRam + 3*(PosY*LenX + PosX) + 0);
               *(ChannelG) = *(PointerToRam + 3*(PosY*LenX + PosX) + 1);
               *(ChannelB) = *(PointerToRam + 3*(PosY*LenX + PosX) + 2);
               
               return 0;
             }
          
             default:
               return ERR_WRONG_PARAMETERS;
          }
    
       }
       else
          return ERR_WRONG_PARAMETERS;
    }
    else
     return ERR_IMAGE_IS_NOT_ALLOCATED;  
}



char ndIMG_PutPixelToImage (struct ndImage_Type *MyImage, unsigned short int PosX, unsigned short int PosY, TypeColor MagicNumber)
/*
  Questa routine permette alla routine chiamante di scrivere un pixel
  nell'immagine MyImage. 
    
  Il colore del pixel viene fornito per mezzo del corrispondente
  Magic Number.
   
  Sono previsti i seguenti codici di errore.
  
    ERR_WRONG_PARAMETERS           PosX o PosY sono fuori immagine
    ERR_IMAGE_IS_NOT_ALLOCATED     L'immagine non è stata ancora allocata
*/

{
    unsigned short int LenX, LenY;
    
    unsigned char *PointerToMem;
    unsigned char ChannelR, ChannelG, ChannelB; 
    
    if ((MyImage->Data)!=0)
    {
       LenX = MyImage->LenX;
       LenY = MyImage->LenY;
    
       if ( (PosX<LenX) && (PosY<LenY)  )
       {
          switch (MyImage->ColorFormat)
          {
                 case NDMGKNB:
                 {
          
                      *(MyImage->Data + PosY*LenX + PosX) = MagicNumber;
                      // Non è necessaria moltiplicazione per sizeof (TypeColor)
                      // perchè MyImage->Data non è di tipo char e quindi ci
                      // pensa il compilatore
                
                      return 0;
                 }
                
                 case NDRGB:
                 {
                      ndHAL_MagicNumberToRGB (MagicNumber, &ChannelR, &ChannelG, &ChannelB);
                      
                      PointerToMem = (unsigned char *)(MyImage->Data);
                      
                      *(PointerToMem + 3*(PosY*LenX + PosX) + 0) = ChannelR;
                      *(PointerToMem + 3*(PosY*LenX + PosX) + 1) = ChannelG;
                      *(PointerToMem + 3*(PosY*LenX + PosX) + 2) = ChannelB;
                 
                      return 0;
                 }
                
                 default:
                   return ERR_WRONG_PARAMETERS;
          }
          
       }
       else
          return ERR_WRONG_PARAMETERS;
    }
    else
     return ERR_IMAGE_IS_NOT_ALLOCATED;
}


char ndIMG_PutPixelToImage_RGB (struct ndImage_Type *MyImage, unsigned short int PosX, unsigned short int PosY, 
                                unsigned char ChannelR, unsigned char ChannelG, unsigned char ChannelB)
/*
  Questa routine permette alla routine chiamante di scrivere un pixel
  nell'immagine MyImage. 
    
  Il colore del pixel viene fornito per mezzo dei rispettivi canali
  RGB.
   
  Sono previsti i seguenti codici di errore.
  
    ERR_WRONG_PARAMETERS               PosX o PosY sono fuori immagine
    ERR_IMAGE_IS_NOT_ALLOCATED         L'immagine non è stata ancora allocata
*/

{
    unsigned short int LenX, LenY;
    
    unsigned char *PointerToMem;
    TypeColor MagicNumber; 
    
    if ((MyImage->Data)!=0)
    {
       LenX = MyImage->LenX;
       LenY = MyImage->LenY;
    
       if ( (PosX<LenX) && (PosY<LenY)  )
       {
          switch (MyImage->ColorFormat)
          {
                 case NDMGKNB:
                 {
          
                      MagicNumber=ndHAL_RGBToMagicNumber (ChannelR, ChannelG, ChannelB);
          
                      *(MyImage->Data + PosY*LenX + PosX) = MagicNumber;
                      // Non è necessaria moltiplicazione per sizeof (TypeColor)
                      // perchè MyImage->Data non è di tipo char e quindi ci
                      // pensa il compilatore
                
                      return 0;
                 }
                
                 case NDRGB:
                 {
                      
                      PointerToMem = (unsigned char *)(MyImage->Data);
                      
                      *(PointerToMem + 3*(PosY*LenX + PosX) + 0) = ChannelR;
                      *(PointerToMem + 3*(PosY*LenX + PosX) + 1) = ChannelG;
                      *(PointerToMem + 3*(PosY*LenX + PosX) + 2) = ChannelB;
                 
                      return 0;
                 }
                
                 default:
                   return ERR_WRONG_PARAMETERS;
          }
          
       }
       else
          return ERR_WRONG_PARAMETERS;
    }
    else
     return ERR_IMAGE_IS_NOT_ALLOCATED;

}


void ndIMG_InitImage (struct ndImage_Type *ndImgDest)
{
   ndImgDest->Data=0;        // Informa le routine successive che il puntatore non punta attualmente
                             // alcuna immagine.
}

char ndIMG_CreateImage           (struct ndImage_Type *ndImgDest, int LenX, int LenY, char ColorFormat)
/* 
   Questa routine alloca i bytes necessari per contenere un immagine con le dimensioni
   LenX, LenY. L'immagine all'inizio contiene pixel totalmente casuali: la routine
   chiamante eseguirà una serie di ndPutPixelToImage per riempire l'immagine. 
   
   Il parametro ColorFormat deve avere il valore NDMGKNB se si vuole un'immagine di tipo
   MagicNumber, mentre deve avere il valore NDRGB se si vuole un'immagine di tipo RGB.
   
   La routine restituisce i seguenti codici di errore:
       __ERR_MEMORY_OVERFLOW        Non c'è abbastanza memoria per contenere l'immagine
       __ERR_WRONG_PARAMETERS       I parametri di larghezza o altezza immagine sono sbagliati
                                    oppure ColorFormat non è riconosciuto
*/       

{
   TypeColor *NewPointerToMemory;
   TypeColor MagicNumber;
   
   int BytesNeed; 
   
   switch (ColorFormat)
   {
          case NDMGKNB:
          {
               if ((LenX>0) && (LenY>0)) 
               {
                  BytesNeed = LenY*LenX*sizeof(MagicNumber);
   
                  if ((ndImgDest->Data!=0) && (ndImgDest->IsStatic==0)) free (ndImgDest->Data);
                  // Disalloca la precedente immagine allocata nel puntatore se l'utente non lo ha già fatto
                  // esplicitamente
         
                  NewPointerToMemory = malloc  (BytesNeed);
   
                  if (NewPointerToMemory!=0)
                  {
                     ndImgDest -> LenX = LenX;
                     ndImgDest -> LenY = LenY;
                     ndImgDest -> ColorFormat = NDMGKNB;
                     ndImgDest -> Data = NewPointerToMemory;
                     ndImgDest -> IsStatic=0;
   
                     return 0;
                  }
                  else
                     return ERR_MEMORY_OVERFLOW;
               }
               else
                   return ERR_WRONG_PARAMETERS;
           
               break;
           }
           
           case NDRGB:
           {
               if ((LenX>0) && (LenY>0)) 
               {
                  BytesNeed = LenY*LenX*3;
   
                  if (ndImgDest->Data!=0) free (ndImgDest->Data);
                  // Disalloca la precedente immagine allocata nel puntatore se l'utente non lo ha già fatto
                  // esplicitamente
         
                  NewPointerToMemory = malloc  (BytesNeed);
   
                  if (NewPointerToMemory!=0)
                  {
                     ndImgDest -> LenX = LenX;
                     ndImgDest -> LenY = LenY;
                     ndImgDest -> ColorFormat = NDRGB;
                     ndImgDest -> Data = NewPointerToMemory;
                     ndImgDest -> IsStatic=0;
   
                     return 0;
                  }
                  else
                     return ERR_MEMORY_OVERFLOW;
               }
               else
                   return ERR_WRONG_PARAMETERS;
          
               break;
           }
                
           default:
             return ERR_WRONG_PARAMETERS;
   }
   
   
} 



char ndIMG_LoadImageFromNDFArray (struct ndImage_Type *ndImgDest, int LenX, int LenY, unsigned short int *PointerToFirstWord, char ColorFormat)
/* 
   Questa routine alloca i bytes necessari per contenere un immagine che viene fornita per
   mezzo di un array NDF.
   
   Un array NDF è un array in codice C in cui ogni pixel dell'immagine viene rappresentato
   per mezzo di un campo a 16 bit. Il primo bit viene sempre ignorato, i successivi 5 bit
   rappresentano l'intensità di colore del rosso, i successivi 5 bit rappresentano 
   l'intensità di colore del verde, ed i successivi 5 bit rappresentano l'intensità 
   di colore del blu.
   
   In memoria l'immagine viene memorizzata per mezzo del MagicNumber associato alla
   piattaforma se si fornisce il parametro ColorFormat=NDMGKNB, mentre vengono 
   memorizzati i tre canali RGB se si fornisce il parametro ColorFormat=NDRGB
   
   La routine restituisce i seguenti codici di errore:
       ERR_MEMORY_OVERFLOW        Non c'è abbastanza memoria per contenere l'immagine
       ERR_WRONG_PARAMETERS       I parametri di larghezza o altezza immagine sono sbagliati
                                    o ColorFormat ha un valore non consentito
*/       

{
   char ErrRep;
   TypeColor MagicNumber;
   unsigned char ChannelR, ChannelG, ChannelB;
   
   unsigned short int NDFValue;
   unsigned short int CounterX, CounterY;
   
   if (! ((ColorFormat==NDMGKNB) && ( ndHAL_NDF_Identical_To_MGKNB() )) )
   {
           ErrRep = ndIMG_CreateImage (ndImgDest, LenX, LenY, ColorFormat);
           // Alloca in memoria lo spazio necessario
           
           if (!ErrRep)
           {              
                 for (CounterY=0; CounterY<LenY; CounterY++)
                 {
                     for (CounterX=0; CounterX<LenX; CounterX++)
                     {
                         NDFValue = *(PointerToFirstWord + CounterY*LenX + CounterX);           // Preleva i dati necessari dall'array NDF
                   
                         switch (ColorFormat)
                         {
                            case NDMGKNB:
                            {
                                 MagicNumber = ndHAL_ConvertNDFtoMagicNumber (NDFValue);                 // Converte il valore nel corrispondente MagicNumber
                                                        
                                 ndIMG_PutPixelToImage (ndImgDest, CounterX, CounterY, MagicNumber);              
                                 break;
                            }
                            
                            case NDRGB:
                            {
                                 ndHAL_ConvertNDFToRGB (NDFValue, &ChannelR, &ChannelG, &ChannelB);
                                 ndIMG_PutPixelToImage_RGB (ndImgDest, CounterX, CounterY, ChannelR, ChannelG, ChannelB);
                                 break;
                            }
                         }
                     }
                 }
           
                 return 0;
           }
           else
             return ErrRep;
   }
   else // Siamo in una piattaforma dove i MagicNumber sono identici agli NDF. Tantovale puntare direttamente agli array
   {
        ndImgDest->LenX = LenX;
        ndImgDest->LenY = LenY;
        ndImgDest->ColorFormat = NDMGKNB;
        ndImgDest->Data = PointerToFirstWord;
        ndImgDest->IsStatic = 1;
        return 0;
   }
} 








#ifndef I_HAVE_DEV_IL
  
char INTERNAL_LEGACY_ndIMG_LoadImage_BMP (struct ndImage_Type *MyImage, char *NameFile, char ColorFormat)
/* 
    Questa routine si occupa di aprire un'immagine bitmap. Viene usata quando non è
    disponibile l'interfaccia Dev-IL.
*/ 
{    
    char FileHandle; 
    
    int BMP_Size;  
    int BMP_DataOffset;
    int BMP_InfoHandleSize;
    int BMP_ImageLenX;
    int BMP_ImageLenY;
    int BMP_ColorDepth;
        
    unsigned int Palette [256][3];
    unsigned int NrElementiPalette; 

    unsigned short int ImagePosX, ImagePosY;
   
    char ColorBMP1, ColorBMP2, ColorBMP3;
    unsigned short int CharFromBMP, ColorFromBMP, BitColor;
   
    unsigned char ChannelR, ChannelG, ChannelB;
   
    int NrBytes, BankSize;
    int CounterX, CounterY, CounterA, CounterB;
    
    char MemErrorCode;
    int WantedSeek;
    
    // Inizio del codice  

    if (!ndInitSystemExecuted) ndInitSystem();                  

    FileHandle=ndHAL_Open (NameFile, O_RDONLY, 0777);
                      
    if (FileHandle>=0)        // E' stato possibile aprire il file
    {
       if (MyImage->Data!=0) free (MyImage->Data);
       // Disalloca la precedente immagine allocata nel puntatore se l'utente non lo ha già fatto
       // esplicitamente
          
       WantedSeek=2;
       if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)  goto FileIOErrorLev1;
       if (ndFS_GetLongInt (FileHandle, &BMP_Size)<0)                  goto FileIOErrorLev1;
          
       WantedSeek=10;
       if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)  goto FileIOErrorLev1;
       if (ndFS_GetLongInt (FileHandle, &BMP_DataOffset)<0)            goto FileIOErrorLev1;
          
       WantedSeek=14;
       if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)  goto FileIOErrorLev1;
       if (ndFS_GetLongInt (FileHandle, &BMP_InfoHandleSize)<0)        goto FileIOErrorLev1;
          
       WantedSeek=18;
       if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)  goto FileIOErrorLev1;
       if (ndFS_GetLongInt (FileHandle, &BMP_ImageLenX)<0)             goto FileIOErrorLev1;
          
       WantedSeek=22;
       if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)  goto FileIOErrorLev1;
       if (ndFS_GetLongInt (FileHandle, &BMP_ImageLenY)<0)             goto FileIOErrorLev1;
          
       WantedSeek=28;
       if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)  goto FileIOErrorLev1;
       if (ndFS_GetLongInt (FileHandle, &BMP_ColorDepth)<0)            goto FileIOErrorLev1;
                       
       goto ContinueProcess1;           // Per saltare la prima condizione di errore
       
       FileIOErrorLev1:                 // Errore durante la lettura dell'handle del file 
                
           ndHAL_Close (FileHandle);
           return ERR_LOGICAL_IN_IMAGE ;
    
       ContinueProcess1:
                              
           MemErrorCode=ndIMG_CreateImage (MyImage, BMP_ImageLenX, BMP_ImageLenY, ColorFormat);
           // Alloca la memoria necessaria
             
           if (MemErrorCode==0)   // Il processo di allocazione è andato a buon fine
           {              
                // Adesso occorre differenziale il trattamente a seconda del numero di bit
                
                
                switch (BMP_ColorDepth)
                {
                       case 24:            // Valido per immagini a 24 bit per pixel (8 per canale)
                       {                 
                           for (CounterY=0; CounterY<BMP_ImageLenY; CounterY++)
                           {
                                
                                for (CounterX=0; CounterX<BMP_ImageLenX; CounterX++)
                                {
                                    ImagePosX=CounterX;
                                    ImagePosY=(BMP_ImageLenY-1)-CounterY;
                              
                                    NrBytes = BMP_ImageLenX*3;
                              
                                    if ((NrBytes % 4)==0)
                                       BankSize=NrBytes;
                                    else
                                       BankSize = NrBytes + (4 - (NrBytes % 4));
                              
                                    WantedSeek = BMP_DataOffset + CounterY*BankSize + CounterX*3;
                                    if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek) 
                                       goto FileIOErrorLev2A;
                                    
                                    if (ndFS_GetByte (FileHandle, &ChannelB)<0) goto FileIOErrorLev2A;
                                    // Canale Blue
                     
                                    if (ndFS_GetByte (FileHandle, &ChannelG)<0) goto FileIOErrorLev2A;
                                    // Canale Green
                     
                                    if (ndFS_GetByte (FileHandle, &ChannelR)<0) goto FileIOErrorLev2A;
                                    // Canale Red
                     
                                    // Si consideri che le .BMP di Windows hanno i canali nell'ordine blue, green, red.
                                         
                                    ndIMG_PutPixelToImage_RGB (MyImage, ImagePosX, ImagePosY, ChannelR, ChannelG, ChannelB);                
                                }
                           }
                      
                           //Ok
                                
                              ndHAL_Close (FileHandle);
                              return 0;
                           
                           FileIOErrorLev2A:
                              ndIMG_ReleaseImage (MyImage);
                              ndHAL_Close (FileHandle);
                              return ERR_IO_IMAGE;
                                             
                       }                 // End of case 24
    
                       
                       case 4:   // Valido per immagini a 16 colori
                       {
                           // Per le immagini a 16 colori è necessario determinare la palette. 
             
                           NrElementiPalette=16;
                           
                           for (CounterA=0; CounterA<16; CounterA++)
                           {
                               for (CounterB=0; CounterB<3; CounterB++)
                               {
                                   WantedSeek = 54 + CounterA*4 + CounterB;
                           
                                   if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)
                                     goto FileIOErrorLev2B;
                                     
                                   if (ndFS_GetByte (FileHandle, &Palette [CounterA][CounterB])<0) 
                                      goto FileIOErrorLev2B;
                                  }
                           }
               
                      
                           NrBytes = BMP_ImageLenX/2;
                              
                           if ((NrBytes % 4)==0)  BankSize=NrBytes;
                              else BankSize = NrBytes + (4 - (NrBytes % 4));
                      
                           
                           for (CounterY=0; CounterY<BMP_ImageLenY; CounterY++)
                           {
                               for (CounterX=0; CounterX<BMP_ImageLenX; CounterX++)
                               {
                                   WantedSeek = BMP_DataOffset + CounterY*BankSize + CounterX/2;
                                   
                                   if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)
                                     goto FileIOErrorLev2B;
                                     
                                   if (ndFS_GetByte (FileHandle, &CharFromBMP)<0)
                                     goto FileIOErrorLev2B; 
                                   
                                   if (( CounterX % 2) == 0) 
                                      ColorFromBMP = (CharFromBMP >> 4) & 15;
                                   else
                                      ColorFromBMP = CharFromBMP & 15; 
                     
                                   ColorBMP1=Palette [ColorFromBMP][2];
                                   ColorBMP2=Palette [ColorFromBMP][1];
                                   ColorBMP3=Palette [ColorFromBMP][0];
                     
                                   ImagePosX=CounterX;
                                   ImagePosY=(BMP_ImageLenY-1)-CounterY;
                     
                                   ChannelB = ColorBMP3;
                                   // Canale Blue
                     
                                   ChannelG = ColorBMP2;
                                   // Canale Green
                     
                                   ChannelR = ColorBMP1;
                                   // Canale Red
                     
                                   // Si consideri che le .BMP di Windows hanno i canali nell'ordine blue, green, red.
                                         
                                   ndIMG_PutPixelToImage_RGB (MyImage, ImagePosX, ImagePosY, ChannelR, ChannelG, ChannelB);   
                              
                               }
                           }
                           
                           // Ok
                           
                              ndHAL_Close (FileHandle);
                              return 0;
                           
                           FileIOErrorLev2B:
                              ndIMG_ReleaseImage (MyImage);
                              ndHAL_Close (FileHandle);
                              return ERR_IO_IMAGE;
                            
                       
                       }   // End of case 4    
                  
                       case 8:            // Valido per immagini a 256 colori
                       {
                           // Per le immagini a 256 colori è necessario determinare la palette. 
             
                           NrElementiPalette=256;
                          
                          for (CounterA=0; CounterA<256; CounterA++)
                          {
                              for (CounterB=0; CounterB<3; CounterB++)
                              {
                                  WantedSeek = 54 + CounterA*4 + CounterB;
                                  
                                  if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)
                                    goto FileIOErrorLev2C;
                                    
                                  if (ndFS_GetByte (FileHandle, &Palette [CounterA][CounterB])<0)
                                    goto FileIOErrorLev2C;
                              }
                          }
               
                      
                          NrBytes=BMP_ImageLenX;
                      
                          if ((NrBytes % 4)==0) BankSize=NrBytes;
                             else BankSize = NrBytes + (4 - (NrBytes % 4));        
             
                          for (CounterY=0; CounterY<BMP_ImageLenY; CounterY++)
                          {
                             for (CounterX=0; CounterX<BMP_ImageLenX; CounterX++)
                             {
                                 WantedSeek = BMP_DataOffset + ( CounterY * BankSize ) + CounterX;
                                 
                                 if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)
                                   goto FileIOErrorLev2C;
                                   
                                 if (ndFS_GetByte (FileHandle, &ColorFromBMP)<0)
                                   goto FileIOErrorLev2C;
                                 
                                 ColorBMP1=Palette [ColorFromBMP][2];
                                 ColorBMP2=Palette [ColorFromBMP][1];
                                 ColorBMP3=Palette [ColorFromBMP][0];
                     
                                 ImagePosX=CounterX;
                                 ImagePosY=(BMP_ImageLenY-1)-CounterY;
                     
                                 ChannelB = ColorBMP3;
                                 // Canale Blue
                     
                                 ChannelG = ColorBMP2;
                                 // Canale Green
                     
                                 ChannelR = ColorBMP1;
                                 // Canale Red
                     
                                 // Si consideri che le .BMP di Windows hanno i canali nell'ordine blue, green, red.
                                         
                                 ndIMG_PutPixelToImage_RGB (MyImage, ImagePosX, ImagePosY, ChannelR, ChannelG, ChannelB);              
                             }
                          }
                      
                          // Ok
                           
                              ndHAL_Close (FileHandle);
                              return 0;
                           
                           FileIOErrorLev2C:
                              ndIMG_ReleaseImage (MyImage);
                              ndHAL_Close (FileHandle);
                              return ERR_IO_IMAGE;
                            
                       }    // End of case 8
          
                       case 1:            // Valido per immagini in bianco e nero
                       {
                            // Per le immagini in bianco e nero non è necessario determinare la palette. 
                      
                            NrBytes = (BMP_ImageLenX/8);
                      
                            if ((NrBytes % 4)==0) BankSize=NrBytes;
                               else BankSize=NrBytes + (4 - (NrBytes % 4) );
                      
                            for (CounterY=0; CounterY<BMP_ImageLenY; CounterY++)
                            {
                               for (CounterX=0; CounterX<BMP_ImageLenX; CounterX++)
                               {
                                   WantedSeek = BMP_DataOffset + ( CounterY * BankSize ) + CounterX/8;
                                 
                                   if (ndHAL_Lseek (FileHandle, WantedSeek, SEEK_SET)!=WantedSeek)
                                     goto FileIOErrorLev2D;
                                   
                                   if (ndFS_GetByte (FileHandle, &ColorFromBMP)<0) 
                                     goto FileIOErrorLev2D;  
                                 
                                   BitColor = ColorFromBMP >> (7 - CounterX % 8) & 1;
                     
                                   ColorBMP1=BitColor*255;
                                   ColorBMP2=BitColor*255;
                                   ColorBMP3=BitColor*255;
                     
                                   ImagePosX=CounterX;
                                   ImagePosY=(BMP_ImageLenY-1)-CounterY;
                     
                                   ChannelB = ColorBMP3;
                                   // Canale Blue
                     
                                   ChannelG = ColorBMP2;
                                   // Canale Green
                     
                                   ChannelR = ColorBMP1;
                                   // Canale Red
                     
                                   // Si consideri che le .BMP di Windows hanno i canali nell'ordine blue, green, red.
                                         
                                   ndIMG_PutPixelToImage_RGB (MyImage, ImagePosX, ImagePosY, ChannelR, ChannelG, ChannelB);   
                                 
                                }
                            }
                          
                            // Ok
                           
                              ndHAL_Close (FileHandle);
                              return 0;
                           
                            FileIOErrorLev2D:
                              ndIMG_ReleaseImage (MyImage);
                              ndHAL_Close (FileHandle);
                              return ERR_IO_IMAGE;
                                 
                       }    // End of case 2
              
                       default:    // Se si arriva a questo punto, vuol dire che il formato non è supportato
                       
                       {
                             ndIMG_ReleaseImage (MyImage);
                             ndHAL_Close (FileHandle);
                             return ERR_LOGICAL_IN_IMAGE;
                       }
                       
                }    // End of switch
                   
           }
           else                // Il processo di allocazione non è andato a buon fine     
           {
              ndHAL_Close (FileHandle);
              return MemErrorCode;
           }            
      
    }  
    else      // Non è stato possibile aprire il file o il file non è stato trovato  
    {
       return ERR_FILE_NOT_FOUND;     
    }
    
}


char INTERNAL_LEGACY_ndIMG_LoadImage_General (struct ndImage_Type *MyImage, char *NameFile, char ColorFormat)
{
  char Percorso [255];
  char Nome [255];
  char Ext [255];
  char ExtMagn [255];
  unsigned char LenExt;
  
  fssSplitNameFile (NameFile, Percorso, Nome, Ext);          // Splitta nome, estensione e percorso.
  LenExt = strlen (Ext);
  ndStrCpy (ExtMagn, Ext, LenExt, 1);                     // Crea una copia maiuscola dell'estensione
  
  if (strcmp (ExtMagn, "BMP")==0)
     return INTERNAL_LEGACY_ndIMG_LoadImage_BMP (MyImage, NameFile, ColorFormat);   // Carica il file corrispondente
      
  return ERR_FORMAT_NOT_SUPPORTED;                        // Se si è arrivati fin qui, vuol dire che il formato             
                                                          // non è riconosciuto
      
}


#else


char BRIDGE_LoadImage_DevIL (struct ndImage_Type *MyImage, char *NameFile, char ColorFormat)
{
  
  ILuint DevIL_Image;        // Codice immagine DevIL 
  ILuint DevIL_LenX, DevIL_LenY;
  int    DevIL_DataAddr, DevIL_PixelAddr;
  char   DevIL_Success;
  int    DevIL_Error;
  
  int CounterX, CounterY;
  int PosPixelX, PosPixelY;
  
  char InvX, InvY;

  unsigned char *ChannelR, *ChannelG, *ChannelB;
  
  char Percorso [255];
  char Nome [255];
  char Ext [255];
  char ExtMagn [255];
  unsigned char LenExt;
  
  fssSplitNameFile (NameFile, Percorso, Nome, Ext);          // Splitta nome, estensione e percorso.
  LenExt = strlen (Ext);
  ndStrCpy (ExtMagn, Ext, LenExt, 1);                     // Crea una copia maiuscola dell'estensione
  
  // Eseguiamo l'analisi di inversione. Per ragioni non ben chiare, Dev-IL fornisce delle
  // immagini invertite per alcuni formati, mentre per altri l'origine dell'immagine è
  // corretta. Nanodesktop compensa questo problema in modo da nasconderlo alla routine
  // chiamante.

  InvX=0;                           // Valori di default
  InvY=0;
  
  if (strcmp (ExtMagn, "BMP")==0)
  {
     InvX=0;
     InvY=1;
  }

  if (strcmp (ExtMagn, "ICO")==0)
  {
     InvX=0;
     InvY=1;
  }

  if (strcmp (ExtMagn, "TGA")==0)
  {
     InvX=0;
     InvY=1;
  }

  if (strcmp (ExtMagn, "SGI")==0)
  {
     InvX=0;
     InvY=1;
  }

  ilGenImages (1, &DevIL_Image);
  // Genera il codice che identifica l'immagine DevIL 
  
  ilBindImage (DevIL_Image);
  // Predispone la libreria DevIL per ricevere le successive chiamate ed usarle
  // sull'immagine DevIL
  
  DevIL_Success = ilLoadImage (NameFile);
  // Carica l'immagine con la libreria DevIL
  
  if (DevIL_Success)
  {
     ilConvertImage (IL_RGB, IL_UNSIGNED_BYTE);

     DevIL_LenX = ilGetInteger (IL_IMAGE_WIDTH);
     DevIL_LenY = ilGetInteger (IL_IMAGE_HEIGHT); 
    
     ndIMG_CreateImage (MyImage, DevIL_LenX, DevIL_LenY, ColorFormat);
     // Crea un'immagine con le medesime dimensioni usando l'API Nanodesktop
     
     DevIL_DataAddr = (int) ( ilGetData () ); 
  
     for (CounterY=0; CounterY<=(DevIL_LenY - 1); CounterY++)
     {
        for (CounterX=0; CounterX<=(DevIL_LenX - 1); CounterX++)
        {
        
           if (!InvX)  PosPixelX=CounterX;
              else PosPixelX=DevIL_LenX - 1 - CounterX;
              
           if (!InvY)  PosPixelY=CounterY;
              else PosPixelY=DevIL_LenY - 1 - CounterY;
              
  
           DevIL_PixelAddr = DevIL_DataAddr + 3*(PosPixelY*DevIL_LenX + PosPixelX);
                   
           ChannelR = (unsigned char *)(DevIL_PixelAddr+2);
           ChannelG = (unsigned char *)(DevIL_PixelAddr+1);
           ChannelB = (unsigned char *)(DevIL_PixelAddr+0);
        
           ndIMG_PutPixelToImage_RGB (MyImage, CounterX, CounterY, *ChannelB, *ChannelG, *ChannelR);
        
        }
     }    
     
     ilDeleteImages (1, &DevIL_Image);
     
     return 0;
  }
  else
  {
      DevIL_Error=ilGetError();
      ilDeleteImages (1, &DevIL_Image);
      
      if (DevIL_Error==IL_FORMAT_NOT_SUPPORTED)
         return ERR_FORMAT_NOT_SUPPORTED;    
      
      if (DevIL_Error==IL_COULD_NOT_OPEN_FILE)
         return ERR_FILE_NOT_FOUND;
         
      if (DevIL_Error==IL_FILE_READ_ERROR)
         return ERR_IO_IMAGE;
      
      return ERR_LOGICAL_IN_IMAGE;
  }
}



#endif


char ndIMG_LoadImage (struct ndImage_Type *MyImage, char *NameFile, char ColorFormat)
/* 
   Questa routine consente di caricare un'immagine dal file-system della
   piattaforma utilizzata.
   
   I dati estratti dal file grafico vengono riposti in un'area
   di memoria che viene allocata automaticamente: l'indirizzo di
   tale area di memoria sarà restituito nella struttura
   MyImage.
   
   Il formato dei dati caricati in memoria dipende dal parametro
   ColorFormat: se esso è pari a NDMGKNB, allora il sistema
   memorizza in ram esclusivamente i MagicNumber dei pixel
   dell'immagine. Se invece si usa il parametro NDRGB vengono
   memorizzati i valori di intensità dei tre canali RGB con
   profondità a 8 bit.
   
   Al termine dell'operazione, la routine restituisce i seguenti
   codici di errore:
   
   ERR_IMAGE_IS_NOT_ALLOCATED  L'immagine non è allocata
   ERR_FILE_NOT_FOUND          Il file non è stato trovato
   ERR_FORMAT_NOT_SUPPORTED    Formato dell'immagine non supportato
   ERR_IO_IMAGE                Un errore non identificato nel 
                               caricamento dell'immagine
   ERR_MEMORY_OVERFLOW         Overflow della memoria
   ERR_LOGICAL_IN_IMAGE        Errore logico nell'immagine
*/
{
  #ifdef I_HAVE_DEV_IL
     return BRIDGE_LoadImage_DevIL (MyImage, NameFile, ColorFormat);
  #else
     return INTERNAL_LEGACY_ndIMG_LoadImage_General (MyImage, NameFile, ColorFormat);  
  #endif
}
     
#ifndef I_HAVE_DEV_IL
  
char INTERNAL_LEGACY_ndIMG_SaveImage_BMP (struct ndImage_Type *MyImage, char *NameFile)
{
  int FileHandle;
  int BytesPerRiga1, BytesPerRiga2;
  int ImageDataRaw;
  unsigned short int PosPixelX, PosPixelY;
  unsigned short int CounterX, CounterY;
  unsigned char ChannelR, ChannelG, ChannelB;
  
  
  if (MyImage->Data==0) return ERR_IMAGE_IS_NOT_ALLOCATED; 
  
  FileHandle=ndHAL_Open (NameFile, O_CREAT | O_WRONLY | O_TRUNC , 0777);
  
  if (FileHandle>=0)
  {
    // Un algoritmo calcola il numero di byte dell'immagine
   
    BytesPerRiga1 = MyImage->LenX*3;
   
    if ((BytesPerRiga1 % 4)==0)
       BytesPerRiga2=BytesPerRiga1;
    else
       BytesPerRiga2=BytesPerRiga1 + (4 - (BytesPerRiga1 % 4));
   
    ImageDataRaw=MyImage->LenY*BytesPerRiga2;
    
    
    if (ndFS_WriteChar (FileHandle, 'B')) goto FileErrorLev1;
    if (ndFS_WriteChar (FileHandle, 'M')) goto FileErrorLev1;
   
    if (ndFS_WriteDWord (FileHandle, ImageDataRaw+54)) goto FileErrorLev1;
    // La dimensione del file in bytes
   
    if (ndFS_WriteWord (FileHandle, 0)) goto FileErrorLev1;
    // campo bfReservedWord1
   
    if (ndFS_WriteWord (FileHandle, 0)) goto FileErrorLev1;
    // campo bfReservedWord2
   
    if (ndFS_WriteDWord (FileHandle, 54)) goto FileErrorLev1;
    // campo bfOffBits: nelle condizioni date si verifica che è sempre 54
   
    if (ndFS_WriteDWord (FileHandle, 40)) goto FileErrorLev1;
    // campo biSize: nelle condizioni date è sempre 40
   
    if (ndFS_WriteDWord (FileHandle, MyImage->LenX)) goto FileErrorLev1;
    // campo biWidth
   
    if (ndFS_WriteDWord (FileHandle, MyImage->LenY)) goto FileErrorLev1;
    // campo biHeight
   
    if (ndFS_WriteWord (FileHandle, 1)) goto FileErrorLev1;
    // campo biPlanes: va sempre settato a uno
   
    if (ndFS_WriteWord (FileHandle, 24)) goto FileErrorLev1;
    // campo bBitCount: settato a 24 bit per pixel (8 bit per canale)
   
    if (ndFS_WriteDWord (FileHandle, 0)) goto FileErrorLev1;
    // campo DCompression. Settato a 0.
   
    if (ndFS_WriteDWord (FileHandle, ImageDataRaw)) goto FileErrorLev1;
    // campo biSizeImage. 
     
    if (ndFS_WriteDWord (FileHandle, 0)) goto FileErrorLev1;
    // campo biXPelsPerMeter. Settato a 0 per convenzione.
  
    if (ndFS_WriteDWord (FileHandle, 0)) goto FileErrorLev1;
    // campo biYPelsPerMeter. Settato a 0 per convenzione.
   
    if (ndFS_WriteDWord (FileHandle, 0)) goto FileErrorLev1;
    //biClrUsed
   
    if (ndFS_WriteDWord (FileHandle, 0)) goto FileErrorLev1;
    //biClrImportant
   
    for (CounterY=0; CounterY<MyImage->LenY; CounterY++)
    {
       for (CounterX=0; CounterX<MyImage->LenX; CounterX++)
       {
           PosPixelX = CounterX;
           PosPixelY = (MyImage->LenY-1) - CounterY;
           
           // Le righe nelle bitmap di windows sono in ordine inverso.
           
           ndIMG_GetPixelFromImage_RGB (MyImage, PosPixelX, PosPixelY, &ChannelR, &ChannelG, &ChannelB);
           // Ricava i valori RGB relativi all'immagine
           
           if (ndFS_WriteChar (FileHandle, ChannelB )) goto FileErrorLev1;
           if (ndFS_WriteChar (FileHandle, ChannelG )) goto FileErrorLev1;
           if (ndFS_WriteChar (FileHandle, ChannelR )) goto FileErrorLev1;
           // L'immagine deve essere salvata nell'ordine riconosciuto dalle bitmap di Windows: prima il canale
           // blu, dopo il verde ed infine il rosso.
       }
       
       if ((BytesPerRiga1 % 4) != 0)
       {
          for (CounterX=0; CounterX<(4 - (BytesPerRiga1 % 4)); CounterX++)
          {
              ndFS_WriteChar (FileHandle, 0);
              // Inseriamo i byte per mantenere la convenzione Windows che i byte che compongono una riga 
              // devono sempre essere multipli di 4.
          }
       }
   }
   
   ndHAL_Close (FileHandle);
   return 0;
   
   FileErrorLev1:
      ndHAL_Close (FileHandle);
      return ERR_IO_IMAGE;   
  }   
  else       // Non è stato possibile creare un handle per il nuovo file
    return ERR_CANNOT_CREATE_FILE;   
   
}

char INTERNAL_LEGACY_ndIMG_SaveImage_General (struct ndImage_Type *MyImage, char *NameFile)
{
  char Percorso [255];
  char Nome [255];
  char Ext [255];
  char ExtMagn [255];
  unsigned char LenExt;
  
  fssSplitNameFile (NameFile, Percorso, Nome, Ext);   // Splitta nome, estensione e percorso.
  LenExt = strlen (Ext);
  ndStrCpy (ExtMagn, Ext, LenExt, 1);                     // Crea una copia maiuscola dell'estensione
  
  if (strcmp (ExtMagn, "BMP")==0)
     return INTERNAL_LEGACY_ndIMG_SaveImage_BMP (MyImage, NameFile);   // Carica il file corrispondente
      
  return ERR_FORMAT_NOT_SUPPORTED;                        // Se si è arrivati fin qui, vuol dire che il formato             
                                                          // non è riconosciuto   
}

#else

char BRIDGE_SaveImage_DevIL (struct ndImage_Type *MyImage, char *NameFile)
{
  ILuint DevIL_ImageCode1;        // Codice immagine DevIL 
  
  struct ndImage_Type NDSavingImage;
  
  int FileHandle, ErrRep1, ErrRep2;
  int DevIL_Success, DevIL_Error;
  
  unsigned short int CounterX, CounterY;
  unsigned short int PosX, PosY;
  
  unsigned char ChannelR, ChannelG, ChannelB;
 
  FileHandle = ndHAL_Open (NameFile, O_RDONLY, 0777);
 
  if (FileHandle>=0)     // Il file già esiste
  {
     ndHAL_Close (FileHandle);
     ErrRep1=ndHAL_EraseFile (NameFile);
  }
  else
  {
     ErrRep1=0; 
  }
   
  if (MyImage->Data!=0)
  {                 
     if (ErrRep1==0)
     {                 
        ErrRep2=ndIMG_CreateImage (&NDSavingImage, MyImage->LenX, MyImage->LenY, NDRGB);
        // Crea un'immagine temporanea per il salvataggio
     
        if (ErrRep2==0)
        {
           for (CounterY=0; CounterY<MyImage->LenY; CounterY++)
           {
               for (CounterX=0; CounterX<MyImage->LenX; CounterX++)
               {
                   ndIMG_GetPixelFromImage_RGB (MyImage, CounterX, CounterY, &ChannelR, &ChannelG, &ChannelB);
                   
                   PosX = CounterX;
                   PosY = MyImage->LenY - CounterY;
                   // E' necessario invertire l'immagine prima di fare intervenire le routine Dev-IL
                   
                   ndIMG_PutPixelToImage_RGB (&NDSavingImage, PosX, PosY, ChannelR, ChannelG, ChannelB);
                   // Salva l'immagine RGB invertita
               }
           }
           
           ilGenImages (1, &DevIL_ImageCode1);
           // Genera il codice che identifica l'immagine DevIL nr. 1
  
           ilBindImage (DevIL_ImageCode1);
           // Predispone la libreria DevIL per ricevere le successive chiamate ed usarle
           // sull'immagine DevIL_ImageCode1
  
           ilTexImage (NDSavingImage.LenX, NDSavingImage.LenY, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NDSavingImage.Data);
           // Setta la libreria Dev-IL per usare i dati dell'immagine RGB fornita
  
           DevIL_Success=ilSaveImage (NameFile);
           // Salva l'immagine con la libreria DevIL
              
           if (DevIL_Success)
           {
              ndIMG_ReleaseImage (&NDSavingImage);
              // Rimuove dalla memoria l'immagine di salvataggio temporaneo
              
              ilDeleteImages (1, &DevIL_ImageCode1);
                                     
              return 0;
           }
           else
           {
              ndIMG_ReleaseImage (&NDSavingImage);
              // Rimuove dalla memoria l'immagine di salvataggio temporaneo
            
              ilDeleteImages (1, &DevIL_ImageCode1);
      
              
              DevIL_Error=ilGetError();
                    
              if (DevIL_Error==IL_FORMAT_NOT_SUPPORTED)
                 return ERR_FORMAT_NOT_SUPPORTED;
                 
              return ERR_IO_IMAGE;
           }
 
         }
         else
           return ERR_MEMORY_OVERFLOW;
                      
     }
     else
         return  ERR_CANNOT_CREATE_FILE; 
  
  }
  else
    return ERR_IMAGE_IS_NOT_ALLOCATED; 
    
}


#endif

char ndIMG_SaveImage (struct ndImage_Type *MyImage, char *NameFile)
/* 
   Questa routine consente di salvare un'immagine su disco.
   
   Se non è presente il supporto Dev-IL, l'immagine potrà essere
   salvata esclusivamente in formato BMP con profondità
   colore RGB 24 bit (8 per pixel).
   
   Se invece è presente il supporto Dev-IL, sarà possibile 
   salvare in uno dei formati supportati da Dev-IL.
   
   Al termine dell'operazione, la routine restituisce i seguenti
   codici di errore:
   
   ERR_IMAGE_IS_NOT_ALLOCATED  L'immagine non è allocata
   
   ERR_FORMAT_NOT_SUPPORTED    Il formato destinazione dell'immagine
                               non è supportato
   ERR_IO_IMAGE                Un errore non identificato nel 
                               salvataggio dell'immagine
   ERR_CANNOT_CREATE_FILE      Impossibile creare il file
   ERR_LOGICAL_IN_IMAGE        Errore logico nell'immagine
   ERR_MEMORY_OVERFLOW         Overflow della memoria
*/

{
  #ifdef I_HAVE_DEV_IL
     return BRIDGE_SaveImage_DevIL (MyImage, NameFile);
  #else
     return INTERNAL_LEGACY_ndIMG_SaveImage_General (MyImage, NameFile);  
  #endif  
}




char ndIMG_ShowImageScaled (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, float ScaleX, 
                            float ScaleY, unsigned char WndHandle, char RenderNow)
/*
  -------------------------------------------------------------------------------------------------------
  La presente routine ha lo scopo di visualizzare un immagine sullo schermo nella posizione relativa
  definita da RRPosX e RRPosY, e nella finestra definita da WndHandle. La routine accetta in ingresso
  un puntatore ad una struttura ndImage_Type già esistente.

  L'immagine può essere disegnata con diversi ingrandimenti. Il coefficiente di ingrandimento è
  dato dai fattori ScaleX e ScaleY.
  
  Se un immagine va fuori dalla finestra verrà restituito in uscita un errore ERR_IMAGE_OUT_OF_WS, 
  ma verrà comunque tracciata la parte di immagine che effettivamente ricade all'interno.

  Questo, insieme al fatto che sono ammesse RRPosX e RRPosY negative, consente di eseguire un vero e
  proprio zoom dell'immagine.

  E' possibile anche disegnare un'immagine senza eseguire subito il render. In tal caso è necessario 
  porre RenderNow=NORENDER.

  Sono previsti due codici di errore in uscita
     ERR_IMAGE_OUT_OF_WS
     ERR_WINDOW_IS_NOT_ALLOCATED
  --------------------------------------------------------------------------------------------------------
*/

{
    unsigned short int CounterX, CounterY;
    TypeColor ColorPixel;
    
    unsigned short int OrigX, OrigY;
    unsigned short int ScaledLenX, ScaledLenY;
    
    unsigned short int BoolA, BoolB;
    unsigned short int ErroreInUscita;

    if (!ndInitSystemExecuted) ndInitSystem();

    if (WindowData [WndHandle].WindowIsActive)
    {
       ScaledLenX = ScaleX*(MyImage->LenX);
       ScaledLenY = ScaleY*(MyImage->LenY);            
                   
       BoolA=(RRPosX + ScaledLenX)<=(WindowData [WndHandle].WSWndLength);
       BoolB=(RRPosY + ScaledLenY)<=(WindowData [WndHandle].WSWndHeight);

       if (BoolA && BoolB)
          ErroreInUscita=0;
       else
          ErroreInUscita=ERR_IMAGE_OUT_OF_WS;

       for (CounterY=0; CounterY<ScaledLenY; CounterY++)
       {
          for (CounterX=0; CounterX<ScaledLenX; CounterX++)
          {
              OrigX = (int) (CounterX/ScaleX);
              OrigY = (int) (CounterY/ScaleY);
              
              ndIMG_GetPixelFromImage (MyImage, OrigX, OrigY, &ColorPixel);
              // Poni il MagicNumber in ColorPixel 
              
              ndWS_PutPixel (RRPosX + CounterX, RRPosY + CounterY, ColorPixel, WndHandle, NORENDER);
          }
       }

       if (RenderNow) XWindowRender(WndHandle);
       return ErroreInUscita;
     }
     else
       return ERR_WINDOW_IS_NOT_ALLOCATED;
}


char ndIMG_ShowImage (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY,  
                            unsigned char WndHandle, char RenderNow)
/*
  -------------------------------------------------------------------------------------------------------
  La presente routine ha lo scopo di visualizzare un immagine sullo schermo nella posizione relativa
  definita da RRPosX e RRPosY, e nella finestra definita da WndHandle. La routine accetta in ingresso
  un puntatore ad una struttura ndImage_Type già esistente.

  Se un immagine va fuori dalla finestra verrà restituito in uscita un errore ERR_IMAGE_OUT_OF_WS, 
  ma verrà comunque tracciata la parte di immagine che effettivamente ricade all'interno.

  Sono previsti due codici di errore in uscita
     ERR_IMAGE_OUT_OF_WS
     ERR_WINDOW_IS_NOT_ALLOCATED
  --------------------------------------------------------------------------------------------------------
*/

{
   return ndIMG_ShowImageScaled (MyImage, RRPosX, RRPosY, 1, 1, WndHandle, RenderNow);
};                                                                                                   



char ndIMG_ShowImageScaled_InBS (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, float ScaleX, 
                                 float ScaleY, char RenderNow)
/*
  -------------------------------------------------------------------------------------------------------
  La presente routine ha lo scopo di visualizzare un immagine sullo schermo nella posizione relativa
  definita da RRPosX e RRPosY. La visualizzazione avviene nel base screen. La routine riceve in ingresso
  un puntatore ad una struttura ndImage_Type già esistente.

  L'immagine può essere disegnata con diversi ingrandimenti. Il coefficiente di ingrandimento è
  dato dai fattori ScaleX e ScaleY.
  
  Se un immagine va fuori dal basescreen, verrà restituito in uscita un errore ERR_IMAGE_OUT_OF_BS, 
  ma verrà comunque tracciata la parte di immagine che effettivamente ricade nel base screen.

  Questo, insieme al fatto che sono ammesse RRPosX e RRPosY negative, consente di eseguire un vero e
  proprio zoom dell'immagine.

  E' possibile anche disegnare una immagine senza eseguire subito il render. In tal caso è necessario 
  porre RenderNow=NORENDER.

  E' previsto un solo codice di errore in uscita
     ERR_IMAGE_OUT_OF_BS
  --------------------------------------------------------------------------------------------------------
*/

{
    unsigned short int CounterX, CounterY;
    TypeColor ColorPixel;
    
    unsigned short int OrigX, OrigY;
    unsigned short int ScaledLenX, ScaledLenY;
    
    unsigned short int BoolA, BoolB;
    unsigned short int ErroreInUscita;

    if (!ndInitSystemExecuted) ndInitSystem();

       ScaledLenX = ScaleX*(MyImage->LenX);
       ScaledLenY = ScaleY*(MyImage->LenY);            
                   
       BoolA=(RRPosX + ScaledLenX)<=(Windows_MaxScreenX);
       BoolB=(RRPosY + ScaledLenY)<=(Windows_MaxScreenY);

       if (BoolA && BoolB)
          ErroreInUscita=0;
       else
          ErroreInUscita=ERR_IMAGE_OUT_OF_BS;

       for (CounterY=0; CounterY<ScaledLenY; CounterY++)
       {
          for (CounterX=0; CounterX<ScaledLenX; CounterX++)
          {
              OrigX = (int) (CounterX/ScaleX);
              OrigY = (int) (CounterY/ScaleY);
              
              ndIMG_GetPixelFromImage (MyImage, OrigX, OrigY, &ColorPixel);
              // Poni il MagicNumber in ColorPixel 
              
              ndBS_PutPixel (RRPosX + CounterX, RRPosY + CounterY, ColorPixel, NORENDER);
          }
       }

       if (RenderNow) BaseScreenRender();
       return ErroreInUscita;    
}


char ndIMG_ShowImage_InBS (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, char RenderNow)
/*
  -------------------------------------------------------------------------------------------------------
  La presente routine ha lo scopo di visualizzare un immagine sullo schermo nella posizione relativa
  definita da RRPosX e RRPosY, ed all'interno del base screen. La routine accetta in ingresso
  un puntatore ad una struttura ndImage_Type già esistente.

  Se un immagine va fuori dal base screen verrà restituito in uscita un errore ERR_IMAGE_OUT_OF_BS, 
  ma verrà comunque tracciata la parte di immagine che effettivamente ricade nello schermo.

  E' previsto il seguente codice di errore:
     ERR_IMAGE_OUT_OF_BS
  --------------------------------------------------------------------------------------------------------
*/

{
   return ndIMG_ShowImageScaled_InBS (MyImage, RRPosX, RRPosY, 1, 1, RenderNow);
};                                                                                                   





void ndIMG_ReleaseImage (struct ndImage_Type *MyImage)
/* --------------------------------------------------------------------------------------------------------
   La presente routine provvede a rilasciare la memoria allocata per contenere l'immagine MyImage
   
   Non sono previsti codici di errore per questa routine.
   --------------------------------------------------------------------------------------------------------
*/

{
  if ( ((MyImage->Data)!=0) && (!(MyImage->IsStatic)) )
  {
     free (MyImage->Data);     
     MyImage->Data=0;
  }
}






