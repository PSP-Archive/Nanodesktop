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
    GESTIONE THUMBNAIL
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>
 
char  ndIMG_CreateThumbnailFromImage (struct ndImage_Type *ThumbImage, char *CompleteNameFile,
                                 unsigned short int KLenX, unsigned short int KLenY)

/* 
   Questa routine provvede a trasformare il file il cui nome completo di path è CompleteNameFile
   in una thumbnail.

   Questa thumbnail viene memorizzata nella struct ThumbImage. Deve essere la routine chiamante ad
   allocare una struct di tipo ndImage_Type nel proprio stack ed a passare l'indirizzo come
   parametro.
   
   La routine restituisce i seguenti codici di errore:
     ERR_I_CANNOT_CREATE_THUMBS      Non è possibile creare la Thumbnail
*/

{                     
  struct ndImage_Type MyImage;
  char ErrRep;
  unsigned short int CounterX, CounterY;
  unsigned short int PosImgX, PosImgY;
  TypeColor MagicNumber;
  
  float ScaleX, ScaleY;
  
  ErrRep=ndIMG_LoadImage (&MyImage, CompleteNameFile, NDMGKNB);
  
  if (ErrRep==0)             // L'immagine è stata caricata
  {                           
     if (ThumbImage->Data!=0) ndIMG_ReleaseImage (ThumbImage);
     
     ErrRep=ndIMG_CreateImage (ThumbImage, KLenX, KLenY, NDMGKNB);
     
     if (ErrRep==0)
     {              
        ScaleX = ( (float) MyImage.LenX )/( (float) KLenX );
        ScaleY = ( (float) MyImage.LenY )/( (float) KLenY );
     
        for (CounterY=0; CounterY<KLenY; CounterY++)
        {
            for (CounterX=0; CounterX<KLenX; CounterX++)
            {
                PosImgX = (int) ( ScaleX * (float) CounterX );
                PosImgY = (int) ( ScaleY * (float) CounterY );
            
                ndIMG_GetPixelFromImage (&MyImage, PosImgX, PosImgY, &MagicNumber);
                ndIMG_PutPixelToImage   (ThumbImage, CounterX, CounterY, MagicNumber);
            }
        }   
        
        ndIMG_ReleaseImage (&MyImage);
        return 0;
     }
     else
        return ERR_I_CANNOT_CREATE_THUMBS;  
  }
  else
     return ERR_I_CANNOT_CREATE_THUMBS;  
} 

void ndFM_EnableThumbnail ()
{
   ThumbnailEnabled=1;  
}

void ndFM_DisableThumbnail ()
{
   ThumbnailEnabled=0;  
}

