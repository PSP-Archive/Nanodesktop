/*
   ----------------------------------------------------------------------------
   Nanodesktop ndFLib
   ----------------------------------------------------------------------------
   This library has been freely based on FLib 
   library (Copyright 2006 samedi1971@yahoo.com)
   
   Filippo Battaglia has realized these routines that provide to Nanodesktop
   support for TrueType fonts
   ----------------------------------------------------------------------------
*/

#include <nanodesktop.h>


/*
   ----------------------------------------------------------------------------
   PROTOTIPI DI FUNZIONI ND INTERNE
   ----------------------------------------------------------------------------
*/




/*
   ----------------------------------------------------------------------------
   RENDER FONT PER IL BASE SCREEN
   ----------------------------------------------------------------------------
*/

static inline void ndFLIB_BS_TTFEngine1 (FT_Bitmap *bitmap, int RRPosPixelX, int RRPosPixelY, unsigned short int Color, unsigned short int BGColor, char Trasp)
{
    int i, j, p, q;
	int grey;
	
	for(i = RRPosPixelX, p = 0; i < RRPosPixelX + bitmap->width; i++, p++)
	{
		for(j = RRPosPixelY, q = 0; j < RRPosPixelY + bitmap->rows; j++, q++)
		{
			grey = bitmap->buffer[q * bitmap->width + p];

            if ( (i>=0) && (j>=0) && (i < Windows_MaxScreenX) && (j < Windows_MaxScreenY) )
            {    
                if (grey > 0) 
                {
                  ndBS_PutPixel_NORENDER_FAST (i, j, Color);
                }
                else
                {
                  if (!Trasp) ndBS_PutPixel_NORENDER_FAST (i, j, BGColor);
                }
            }
		}
	}
}


char ndFLIB_BS_TTFEngine0 (unsigned short RRPosPixelX, unsigned short RRPosPixelY, char *str, TypeColor Color, TypeColor BGColor, int FntHandle, int NewTTFSize, int FntAngle, char Trasp)
{   
    FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
	
	int target_height = 272;
	int n, num_chars;
	float FloatAngle;

    // Cambia eventualmente le dimensioni
    
    if (NewTTFSize!=0)
    {
       if (FontHandlesArray [FntHandle].TTF_Size!=NewTTFSize)
       {
            FT_Set_Pixel_Sizes ( FontHandlesArray [FntHandle].TTF_Face, NewTTFSize, NewTTFSize);
            FontHandlesArray [FntHandle].TTF_Size = NewTTFSize;                 
       }
    }

	// Inizia il rendering
	
	slot = FontHandlesArray [FntHandle].TTF_Face->glyph;
	num_chars = strlen(str);

	FloatAngle = ( (float)(FntAngle) / 360.0 ) * 3.14159 * 2;
	matrix.xx = (FT_Fixed)( ndHAL_XFPU_cosf (FloatAngle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-ndHAL_XFPU_sinf (FloatAngle) * 0x10000L);
	matrix.yx = (FT_Fixed)( ndHAL_XFPU_sinf (FloatAngle) * 0x10000L);
	matrix.yy = (FT_Fixed)( ndHAL_XFPU_cosf (FloatAngle) * 0x10000L);

	pen.x = RRPosPixelX * 64;
	pen.y = (target_height - RRPosPixelY) * 64;

	for(n = 0; n < num_chars; n++)
	{
		FT_Set_Transform ( FontHandlesArray [FntHandle].TTF_Face, &matrix, &pen);

		error = FT_Load_Char( FontHandlesArray [FntHandle].TTF_Face, str[n], FT_LOAD_RENDER );
		if(error) continue;                 // ignore errors 

        ndFLIB_BS_TTFEngine1 (&slot->bitmap, slot->bitmap_left, target_height - slot->bitmap_top, Color, BGColor, Trasp);

		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	} 
	
	return 0;
} 


/*
   ----------------------------------------------------------------------------
   RENDER FONT PER IL WINDOWS SPACE
   ----------------------------------------------------------------------------
*/

static inline void ndFLIB_WS_TTFEngine1 (FT_Bitmap *bitmap, int RRPosPixelX, int RRPosPixelY, unsigned short int Color, unsigned short int BGColor, 
                                         int WndHandle, int WSWndLength, int WSWndHeight, char Trasp)
{
    int i, j, p, q;
	int grey;
	
	for(j = RRPosPixelY, q = 0; j < RRPosPixelY + bitmap->rows; j++, q++)
	{  
        for(i = RRPosPixelX, p = 0; i < RRPosPixelX + bitmap->width; i++, p++)
    	{		
			grey = bitmap->buffer[q * bitmap->width + p];

            if ( (i>=0) && (j>=0) && (i < WSWndLength) && (j < WSWndHeight) )
            {    
                if (grey > 0) 
                {
                  ndWS_PutPixel_NORENDER_FAST (i, j, Color, WndHandle);
                }
                else
                {
                  if (!Trasp) ndWS_PutPixel_NORENDER_FAST (i, j, BGColor, WndHandle);
                }
            }
		}
	}
}


char ndFLIB_WS_TTFEngine0 (unsigned short RRPosPixelX, unsigned short RRPosPixelY, char *str, TypeColor Color, TypeColor BGColor, 
                           int FntHandle, int NewTTFSize, int FntAngle, int WndHandle, int WSWndLength, int WSWndHeight, char Trasp)
{   
    FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
	
	int target_height = 272;
	int n, num_chars;
	float FloatAngle;

    
    // Cambia eventualmente le dimensioni
    
    if (NewTTFSize!=0)
    {
       if (FontHandlesArray [FntHandle].TTF_Size!=NewTTFSize)
       {
            FT_Set_Pixel_Sizes ( FontHandlesArray [FntHandle].TTF_Face, NewTTFSize, NewTTFSize);
            FontHandlesArray [FntHandle].TTF_Size = NewTTFSize;                 
       }
    }
    
	// Inizia il rendering
	
	slot = FontHandlesArray [FntHandle].TTF_Face->glyph;
	num_chars = strlen(str);

	FloatAngle = ( (float)(FntAngle) / 360.0 ) * 3.14159 * 2;
	matrix.xx = (FT_Fixed)( ndHAL_XFPU_cosf (FloatAngle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-ndHAL_XFPU_sinf (FloatAngle) * 0x10000L);
	matrix.yx = (FT_Fixed)( ndHAL_XFPU_sinf (FloatAngle) * 0x10000L);
	matrix.yy = (FT_Fixed)( ndHAL_XFPU_cosf (FloatAngle) * 0x10000L);

	pen.x = RRPosPixelX * 64;
	pen.y = (target_height - RRPosPixelY) * 64;

	for(n = 0; n < num_chars; n++)
	{
		FT_Set_Transform ( FontHandlesArray [FntHandle].TTF_Face, &matrix, &pen);

		error = FT_Load_Char( FontHandlesArray [FntHandle].TTF_Face, str[n], FT_LOAD_RENDER );
		if(error) continue;                 // ignore errors 

        ndFLIB_WS_TTFEngine1 (&slot->bitmap, slot->bitmap_left, target_height - slot->bitmap_top, 
                              Color, BGColor, WndHandle, WSWndLength, WSWndHeight, Trasp);

		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	} 
	
	return 0;
} 



/*
   ----------------------------------------------------------------------------
   DETERMINAZIONE LUNGHEZZA STRINGA
   ----------------------------------------------------------------------------
*/

int ndFLIB_StrLength (char *str, int FntHandle, int NewTTFSize, int *ReqMaximumX, int *ReqMaximumY)
{
    FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
	
	int target_height = 272;
	int n, num_chars;
	
    int OldTTFSize;
    int MaximumWidth, MaximumHeight; 
    int LastLeftPixel, LastDownPixel;
    
    char SizeChanged = 0;

    // Registra la vecchia dimensione del vecchio font
    
    OldTTFSize = FontHandlesArray [FntHandle].TTF_Size; 
    
    // Cambia eventualmente le dimensioni
    
    if (NewTTFSize!=0)
    {
        if (FontHandlesArray [FntHandle].TTF_Size!=NewTTFSize)
        {
               FT_Set_Pixel_Sizes ( FontHandlesArray [FntHandle].TTF_Face, NewTTFSize, NewTTFSize);  
               SizeChanged = 1;                 
        }
    }
    
	// Inizia il rendering simulato
	
	slot = FontHandlesArray [FntHandle].TTF_Face->glyph;
	num_chars = strlen(str);

	matrix.xx = (FT_Fixed)(0x10000L);
	matrix.xy = (FT_Fixed)(0);
	matrix.yx = (FT_Fixed)(0);
	matrix.yy = (FT_Fixed)(0x10000L);

	pen.x = 0;
	pen.y = (target_height) * 64;

    MaximumHeight = 0;
    MaximumWidth  = 0;

	for (n = 0; n < num_chars; n++)
	{
		FT_Set_Transform ( FontHandlesArray [FntHandle].TTF_Face, &matrix, &pen);

		error = FT_Load_Char( FontHandlesArray [FntHandle].TTF_Face, str[n], FT_LOAD_RENDER );
		if(error) continue;                 // ignore errors 

        LastLeftPixel = slot->bitmap_left + slot->bitmap.width;
        LastDownPixel = target_height - slot->bitmap_top + slot->bitmap.rows;
        
        if (LastLeftPixel>MaximumWidth)  MaximumWidth  = LastLeftPixel;
        if (LastDownPixel>MaximumHeight) MaximumHeight = LastDownPixel;

		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	} 
	
	// Ripristina la vecchia dimensione dei font se era stata cambiata
	
	if (SizeChanged)
	{
        FT_Set_Pixel_Sizes ( FontHandlesArray [FntHandle].TTF_Face, OldTTFSize, OldTTFSize);                 
    }
	
	// Deposita le informazioni richieste
	*ReqMaximumX = MaximumWidth;
	*ReqMaximumY = MaximumHeight;
	
	return 0;
}
