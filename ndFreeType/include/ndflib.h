/*
   ----------------------------------------------------------------------------
   Nanodesktop ndFLib - File headers
   ----------------------------------------------------------------------------
   This library has been freely based on FLib 
   library (Copyright 2006 samedi1971@yahoo.com)
   
   Filippo Battaglia has realized these routines that provide to Nanodesktop
   support for TrueType fonts
   ----------------------------------------------------------------------------
*/

  // Motore di rendering per il base screen
  extern char ndFLIB_BS_TTFEngine0 (unsigned short RRPosPixelX, unsigned short RRPosPixelY, char *str, unsigned short int Color, unsigned short int BGColor, 
                                    int FntHandle, int NewTTFSize, int FntAngle, char Trasp);
 
  // Motore di rendering per il windows space 
  extern char ndFLIB_WS_TTFEngine0 (unsigned short RRPosPixelX, unsigned short RRPosPixelY, char *str, unsigned short int Color, unsigned short int BGColor, 
                                    int FntHandle, int NewTTFSize, int FntAngle, int WndHandle, int WSWndLength, int WSWndHeight, char Trasp);

  // Routine per determinazione lunghezza stringa
  extern int ndFLIB_StrLength (char *str, int FntHandle, int NewTTFSize, int *ReqMaximumX, int *ReqMaximumY);
