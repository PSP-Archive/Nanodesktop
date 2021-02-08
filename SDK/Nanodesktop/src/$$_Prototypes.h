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
    ---------------------------------------------------------------------------
    FILE SPECIALE DI DEFINIZIONE DEI PROTOTIPI (Livello CODE)
    ---------------------------------------------------------------------------
    Questo file speciale può essere richiamato in due casi:
           a) dai moduli di Nanodesktop durante la fase di compilazione: in tal
              caso esso fornisce tutti i prototipi delle varie funzioni 
              presenti nei moduli di livello code di Nanodesktop. I moduli di
              solito richiamano questo file dopo avere definito la costante
              manifesta I_NEED_TO_DEFINE_ALL_PROTOTYPES, che rende 
              disponibili anche i prototipi delle routine INTERNAL
              
           b) da Nanodesktop.h durante la compilazione delle normali applicazioni.
              In tal caso i prototipi delle routines internal non saranno
              forniti al compilatore.
    --------------------------------------------------------------------------- 
*/

#ifndef _PROTOTYPES_CONTAINER_
      #define  _PROTOTYPES_CONTAINER_
      
      // Impedirà che questo file possa essere incluso due volte nella compilazione


                                    /*
                                    ------------------------------------------------------------
                                    FUNZIONI AUSILIARIE
                                    ------------------------------------------------------------
                                    */
                                    
                                    extern void StringNumber (long int Numero, char *StrPointer); 
                                    extern void ndDelay (float t); 
                                    
                                    extern int  ndRand (void); 
                                    
                                    extern void UShort_Swap (short unsigned int *a, short unsigned int *b); 
                                    extern void Int_Swap (int *a, int *b); 
                                    
                                    extern char PixelInRect (short unsigned int x, short unsigned int y, short unsigned int x1, short unsigned int y1, short unsigned int x2, short unsigned int y2); 
                                    
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES 
                                    extern char INTERNAL_RettangoliDisgiunti (short unsigned int aX1, short unsigned int aY1, short unsigned int aX2, short unsigned int aY2, short unsigned int bX1, short unsigned int bY1, short unsigned int bX2, short unsigned int bY2); 
                                    extern void INTERNAL_Rotate (int CenterX, int CenterY, float ZPixelX, float ZPixelY, float Rot_Radiant, int *ResultX, int *ResultY); 
                                    extern int  INTERNAL_AllignValue (int Value, int BankSize); 
                              #endif
                                    
                                    extern char Magn (char Carattere); 
                                    extern char DeMagn (char Carattere); 
                                    
                                    extern void ndStrCpy (char *StringaDest, char *StringaSrc, unsigned char Limit, char MakeMagn); 
                                    extern void ndStrCpyBounded (char *StringaDest, char *StringaSrc, unsigned char LimitX1, unsigned char LimitX2, char MakeMagn); 
                                    
                                    
                              /*
                                 ------------------------------------------------------------
                                 FUNZIONI DI GESTIONE FONT
                                 ------------------------------------------------------------
                              */
                              
                              
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES       
                                    extern short int INTERNAL_NTF_LoadFont (char *FntName);
                                    extern short int INTERNAL_NTF_RegisterFont (char *Data, char System, char Graphic, char Static);
                                    extern char      INTERNAL_NTF_DestroyFont (char FntHandle);
                                    extern short int INTERNAL_TTF_LoadFont (char *FntName);
                                    extern short int INTERNAL_TTF_RegisterFont (char *FntName);
                                    extern char      INTERNAL_TTF_DestroyFont (char FntHandle);
                              #endif      
                                    
                                    extern void ndInitFont (void);  
                                    extern short int ndFONT_LoadFont (char *FntName); 
                                    extern short int ndFONT_RegisterNtfFont (char *Data, char Graphic, char Static); 
                                    extern char ndFONT_DestroyFont (char FntHandle); 
                                    
                              /*
                                 -------------------------------------------------------------
                                 FUNZIONI AUSILIARIE PER ACCESSO AL FILE SYSTEM
                                 -------------------------------------------------------------
                              */
                                    
                                    extern char ndFS_GetLongInt (int FileHandle, int *Value); 
                                    extern char ndFS_GetWord (int FileHandle, short unsigned int *Value); 
                                    extern char ndFS_GetByte (int FileHandle, char *Value); 
                                    extern int  ndFS_WriteChar (int FileHandle, char Carattere); 
                                    extern int  ndFS_WriteWord (int FileHandle, short unsigned int Word); 
                                    extern int  ndFS_WriteDWord (int FileHandle, unsigned int DWord); 
                                    extern unsigned int ndMEM_ReadDWordFromBuffer (char *Buffer, int Offset); 
                                    extern unsigned int ndMEM_ReadWordFromBuffer (char *Buffer, int Offset); 
                                    extern unsigned char ndMEM_ReadByteFromBuffer (char *Buffer, int Offset); 
                                    
                                    
                              /*
                                 -------------------------------------------------------------
                                 FUNZIONI KEYS
                                 -------------------------------------------------------------
                              */
                                      
                                    extern ndint64 NDKEY_ROTATE (float Degree); 
                                    extern ndint64 NDKEY_BORDER_SIZE (char Border); 
                                    extern ndint64 NDKEY_FONT (unsigned char FntHandle); 
                                    extern ndint64 NDKEY_POSX (unsigned short int PosX);
                                    extern ndint64 NDKEY_POSY (unsigned short int PosY);
                                    extern ndint64 NDKEY_SETTEXTAREA (int PosX, int PosY);
                                    
                                    extern ndint64 NDKEY_SETFONT (unsigned char FntHandle);
                                    extern ndint64 NDKEY_SETTITLEFONT (unsigned char FntHandle);
                                    extern ndint64 NDKEY_SETMENUFONT (unsigned char FntHandle);
                                    extern ndint64 NDKEY_SETWSLENGTH (unsigned short int WSLength);
                                    extern ndint64 NDKEY_SETWSHEIGHT (unsigned short int WSHeight);
                                    extern ndint64 NDKEY_SETFNTSIZE (unsigned char Size);
                                    extern ndint64 NDKEY_SETFNTROTATE (int Rotation);
                                    
                              /*
                                 -------------------------------------------------------------
                                 INITGRAPH a livello CODE
                                 -------------------------------------------------------------
                              */
                                    
                                    extern void ndCODE_InitGraph (void); 
                                    
                              /*
                                 -------------------------------------------------------------
                                 ROUTINES DI TIPO LOW LEVEL
                                 -------------------------------------------------------------
                              */
                                    #define ndLL_PutPixel                         ndHAL_PutPixel
                                    #define ndLL_GetPixel                         ndHAL_GetPixel
                                    #define ndLL_PutPixelToPage                   ndHAL_PutPixelToPage
                                    #define ndLL_GetPixelFromPage                 ndHAL_GetPixelFromPage
                                    #define ndLL_ChangeFrameBufferPage            ndHAL_ChangeFrameBufferPage
                                 
                                    extern void ndLL_DrawRectangle (unsigned short int RRPosX1, unsigned short int RRPosY1, unsigned short int RRPosX2, unsigned short int RRPosY2, TypeColor Color);
                                    extern void ndLL_WriteChar (short int ChPosX, short int ChPosY, char Carattere, TypeColor Color); 
                                    extern void ndLL_GoCursor (short unsigned int PosX, short unsigned int PosY); 
                                    extern void ndLL_CarriageReturn (void); 
                                    extern void ndLL_WriteLetter (char Lettera, TypeColor Color); 
                                    extern void ndLL_Write (char *str, TypeColor color); 
                                    extern void ndLL_WriteLn (char *str, TypeColor color); 
                                    extern void ndLL_ClrScr (void); 
                                    extern void ndLL_WriteCharSpecial (short int PosPixelCharX, short int PosPixelCharY, char Carattere, TypeColor Color, short unsigned int Page); 
                                    extern void ndLL_PrintLn (TypeColor Color, char *FirstParam, ...); 
                                    extern void ndLL_Print (TypeColor Color, char *FirstParam, ...); 
                                    extern char ndLL_SetFont (unsigned char FntHandle); 
                                    
                              /*
                                 ------------------------------------------------------------
                                 ROUTINES DEL POSITION GENERATOR
                                 ------------------------------------------------------------
                              */
                              
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern char INTERNAL_TheFirstWindow (short unsigned int LenX, short unsigned int LenY, short unsigned int *ResX1, short unsigned int *ResY1, short unsigned int *ResX2, short unsigned int *ResY2); 
                                    extern int  INTERNAL_Calculate_Intersect_Len (int Ax1, int Ax2, int Bx1, int Bx2); 
                                    extern void INTERNAL_Calculate_Epsilon_Sigma (int *Epsilon, int *Sigma, char WndHandle, int Zx1, int Zy1, int Zx2, int Zy2); 
                                    extern void INTERNAL_Calculate_Total_Epsilon_Sigma (ndint64 *TotalEpsilon, ndint64 *TotalSigma, int ThPosX1, int ThPosY1, int ThPosX2, int ThPosY2); 
                                    extern void INTERNAL_Register_Best_Epsilon_Sigma (struct PosGenData_Type *PosGenData, int ThPosX1, int ThPosY1, int ThPosX2, int ThPosY2, ndint64 TotalEpsilon, ndint64 TotalSigma); 
                                    extern void INTERNAL_MakeFirstPassage (struct PosGenData_Type *PosGenData, short unsigned int LenX, short unsigned int LenY); 
                                    extern void INTERNAL_MakeSecondPassage (struct PosGenData_Type *PosGenData, short unsigned int LenX, short unsigned int LenY); 
                              #endif
                                    
                                    extern char ndPOSGEN_GenPosition (short unsigned int LenX, short unsigned int LenY, short unsigned int *ResX1, short unsigned int *ResY1, short unsigned int *ResX2, short unsigned int *ResY2); 
                                    extern char ndPOSGEN_ExplodePosition (short unsigned int PosX, short unsigned int PosY, short unsigned int LenX, short unsigned int LenY, short unsigned int *ResX1, short unsigned int *ResY1, short unsigned int *ResX2, short unsigned int *ResY2); 
                                    
                              /*
                                 ------------------------------------------------------------
                                 ROUTINES LOGICAL PAGE
                                 ------------------------------------------------------------
                              */
                                    
                                    extern unsigned char ndLP_IsPixelInWindow (short unsigned int PosPixelX, short unsigned int PosPixelY, unsigned char WndHandle); 
                                    extern char ndLP_ShootElementInTheWV (unsigned char Element); 
                                    extern char ndLP_PutElementAtTheTopOfWV (unsigned char Element); 
                                    extern char ndLP_RemoveElementFromWV (unsigned char Element); 
                                    extern void ndLP_FindOwnerOfPixel (short unsigned int PosPixelX, short unsigned int PosPixelY, unsigned char *OwnerID); 
                                    extern void ndLP_FindTwoOwnersOfPixel (short unsigned int PosPixelX, short unsigned int PosPixelY, unsigned char *OwnerID1, unsigned char *OwnerID2); 
                                    extern unsigned char ndLP_MyPixelIsInFirstPlane (unsigned int RPosPixelX, unsigned int RPosPixelY, unsigned char WndHandle); 
                                    
                                    extern void ndLP_EnableTrasparency (void); 
                                    extern void ndLP_DisableTrasparency (void); 
                                    extern char ndLP_LoadWallPaperFromFile (char *NameFile); 
                                    extern char ndLP_LoadWallPaperFromNDImage (struct ndImage_Type *MyImage); 
                                    extern void ndLP_EnableBaseScreen (void); 
                                    extern void ndLP_DisableBaseScreen (void); 
                                    extern void ndLP_EnableWallPaper (void); 
                                    extern void ndLP_DisableWallPaper (void); 
                                    extern char ndLP_PutPixel (short unsigned int RPosPixelX, short unsigned int RPosPixelY, TypeColor Color, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern void ndLP_PutPixel_NORENDER_FAST (short unsigned int RPosPixelX, short unsigned int RPosPixelY, TypeColor Color, unsigned char WndHandle); 
                                    extern char ndLP_WriteChar (short unsigned int RPosPixelCharX, short unsigned int RPosPixelCharY, char Carattere, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndLP_Write (short unsigned int RPosPixelX, short unsigned int RPosPixelY, char *str, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndLP_DrawRectangle (short unsigned int PosX1, short unsigned int PosY1, short unsigned int PosX2, short unsigned int PosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndLP_DrawRectangleVoid (short unsigned int PosX1, short unsigned int PosY1, short unsigned int PosX2, short unsigned int PosY2, TypeColor BorderColor, unsigned char WndHandle, short unsigned int RenderNow); 
                                         
                                    extern short unsigned int ndLP_GetMinWndSizeX (char MenuSupported); 
                                    extern short unsigned int ndLP_GetMinWndSizeY (char MenuSupported); 
                                    extern char ndLP_ValidateNewSizeX (int NewSizeX, int WSWndLength, char MenuSupported, int *NewSizeSuggested); 
                                    extern char ndLP_ValidateNewSizeY (int NewSizeY, int WSWndHeight, char MenuSupported, int *NewSizeSuggested); 
                                     
                                    extern char ndLP_CreateWindow (unsigned short int _PosWindowX1, unsigned short int _PosWindowY1, unsigned short int _PosWindowX2, unsigned short int _PosWindowY2, char *_WindowTitle, TypeColor _ColorTitle, TypeColor _ColorBGTitle, TypeColor _ColorWindow, TypeColor _ColorBorderWindow, ndint64 Attribute);
                                    extern char ndLP_MaximizeWindow (unsigned char WndHandle); 
                                    extern char ndLP_MinimizeWindow (unsigned char WndHandle); 
                                    extern char ndLP_ResizeWindow (short unsigned int NewSizeX, short unsigned int NewSizeY, unsigned char WndHandle); 
                                    extern char ndLP_ChangeWindowParameters (short unsigned int _PosWindowX1, short unsigned int _PosWindowY1, short unsigned int _PosWindowX2, short unsigned int _PosWindowY2, char *_WindowTitle, TypeColor _ColorTitle, TypeColor _ColorBGTitle, TypeColor _ColorWindow, TypeColor _ColorBorderWindow, short unsigned int _WSLength, short unsigned int _WSHeight, unsigned char WndHandle, short unsigned int PutInTheFirstPlane, short unsigned int Attribute); 
                                    extern char ndLP_MoveWindow (short unsigned int NewPosX, short unsigned int NewPosY, unsigned char WndHandle); 
                                    extern char ndLP_MoveWindowWithAutoResize (unsigned short int NewPosX, unsigned short int NewPosY, unsigned char WndHandle);
                                    extern char ndLP_PutWindowInFirstPlane (unsigned char WndHandle); 
                                    extern char ndLP_DestroyWindow (unsigned char WndHandle); 
                                    extern char ndLP_ChangeWndTitle (char *NewTitle, TypeColor NewColorTitle, TypeColor NewBGColorTitle, unsigned char WndHandle); 
                                    extern float ndLP_GetWindowROIPosX (char Frac, char WndHandle);
                                    extern float ndLP_GetWindowROIPosY (char Frac, char WndHandle);
                                    extern char ndLP_ChangeWindowROI (float NewROIPosX, float NewROIPosY, int Attribute, unsigned char WndHandle); 
                                    extern char ndLP_SetupCloseWndCallback (void *CloseWndCallback, ndint64 WndInfoField, char NoNotifyToMouseControl, char WndHandle);
                                    extern char ndLP_GetUserAreaInfo (char WndHandle, int *Addr, int *Size); 
                                    extern char ndLP_SetFont (unsigned char FntHandle, char WndHandle);
                                    extern void WindowsRender (void); 
                                    
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern void Draw_CloseWindow_Button (short unsigned int RPosX, short unsigned int RPosY, unsigned char WndHandle); 
                                    extern void Draw_MinimizeWindow_Button (short unsigned int RPosX, short unsigned int RPosY, unsigned char WndHandle); 
                                    extern void Draw_Title_Button (short unsigned int RPosX1, short unsigned int RPosY1, short unsigned int RPosX2, short unsigned int RPosY2, unsigned char WndHandle); 
                                    extern void Draw_ScrollToLeft_Button (short unsigned int RPosX1, short unsigned int RPosY1, short unsigned int RPosX2, short unsigned int RPosY2, TypeColor Color, TypeColor BGColor, short unsigned int WndHandle); 
                                    extern void Draw_ScrollToRight_Button (short unsigned int RPosX1, short unsigned int RPosY1, short unsigned int RPosX2, short unsigned int RPosY2, TypeColor Color, TypeColor BGColor, short unsigned int WndHandle); 
                                    extern void Draw_ScrollToUp_Button (short unsigned int RPosX1, short unsigned int RPosY1, short unsigned int RPosX2, short unsigned int RPosY2, TypeColor Color, TypeColor BGColor, short unsigned int WndHandle); 
                                    extern void Draw_ScrollToDown_Button (short unsigned int RPosX1, short unsigned int RPosY1, short unsigned int RPosX2, short unsigned int RPosY2, TypeColor Color, TypeColor BGColor, short unsigned int WndHandle); 
                                    extern void INTERNAL_NoOrizzontalScroll (short unsigned int WndHandle); 
                                    extern void INTERNAL_NoVerticalScroll (short unsigned int WndHandle); 
                                    extern char ndLP_DrawScrollerRectangles (unsigned int WndHandle); 
                              #endif
                              
                              // Vecchie routines di allocazione finestra: deprecate e mantenute
                              // solo per ragioni di compatibilità
                              
                                    extern char ndLP_AllocateWindowComplex (short unsigned int _PosWindowX1, short unsigned int _PosWindowY1, short unsigned int _PosWindowX2, short unsigned int _PosWindowY2, char *_WindowTitle, TypeColor _ColorTitle, TypeColor _ColorBGTitle, TypeColor _ColorWindow, TypeColor _ColorBorderWindow, short unsigned int _WSLength, short unsigned int _WSHeight, short unsigned int Attribute); 
                                    extern char ndLP_AllocateWindow (short unsigned int _PosWindowX1, short unsigned int _PosWindowY1, short unsigned int _PosWindowX2, short unsigned int _PosWindowY2, char *_WindowTitle, TypeColor _ColorTitle, TypeColor _ColorBGTitle, TypeColor _ColorWindow, TypeColor _ColorBorderWindow); 
                                    extern char ndLP_AllocateWindow_MENU (short unsigned int _PosWindowX1, short unsigned int _PosWindowY1, short unsigned int _PosWindowX2, short unsigned int _PosWindowY2, char *_WindowTitle, TypeColor _ColorTitle, TypeColor _ColorBGTitle, TypeColor _ColorWindow, TypeColor _ColorBorderWindow); 
                                    extern char ndLP_AllocateWindow_NORESIZE (short unsigned int _PosWindowX1, short unsigned int _PosWindowY1, short unsigned int _PosWindowX2, short unsigned int _PosWindowY2, char *_WindowTitle, TypeColor _ColorTitle, TypeColor _ColorBGTitle, TypeColor _ColorWindow, TypeColor _ColorBorderWindow); 
                                    
                                   
                               
                              /*
                                 ------------------------------------------------------------
                                 ROUTINES WINDOWS SPACE
                                 ------------------------------------------------------------
                              */
                                     
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern char INTERNAL_ndWS_IsPixelInWS (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle); 
                                    extern char INTERNAL_ndWS_IsPixelInROI (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle); 
                                    extern char INTERNAL_ndWS_IsPixelInCharArea (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle); 
                                    extern unsigned char INTERNAL_ndWS_IsPixelCovered_NOSS (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle, TypeColor *ColoreCoprente); 
                                    extern unsigned char INTERNAL_ndWS_IsPixelCovered_SS (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle, TypeColor *ColoreCoprente); 
                                    extern unsigned char INTERNAL_ndWS_IsPixelCovered (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle, TypeColor *ColoreCoprente); 
                                    extern char INTERNAL_XWindowRender_NO_SS (unsigned char WndHandle); 
                                    extern char INTERNAL_XWindowRender_SS (unsigned char WndHandle); 
                                    extern char INTERNAL_ndWS_FillArea (int PosX, int PosY, TypeColor Color, TypeColor BorderColor, char WndHandle); 
                                    extern char INTERNAL_ndWS_EllipseEngine (short unsigned int CenterX, short unsigned int CenterY, float RayA, float RayB, float RayX, float DegreeA, float DegreeB, float Rot, TypeColor Colore, char DrawRay, char WndHandle, char RenderNow); 
                              #endif 
                              
                                    extern char ndWS_IsPixelInWS (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle); 
                                    extern char ndWS_IsPixelInROI (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle); 
                                    extern char ndWS_IsPixelInCharArea (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle); 
                                    extern char ndWS_IsPixelCovered (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle, TypeColor *ColoreCoprente); 
                                    extern char ndWS_WriteChar (unsigned int PosCursX, unsigned int PosCursY, char Carattere, TypeColor Colore, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndWS_CharScrolling (unsigned char WndHandle); 
                                    extern char ndWS_CarriageReturn (unsigned char WndHandle); 
                                    extern char ndWS_GoCursor (short unsigned int PosCursX, short unsigned int PosCursY, unsigned char WndHandle); 
                                    extern char ndWS_ClrScr (unsigned char WndHandle); 
                                    extern char ndWS_WriteLetter (char Carattere, TypeColor Colore, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndWS_Write (char *str, TypeColor Colore, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndWS_WriteLn (char *str, TypeColor Colore, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndWS_PrintLn (char WndHandle, TypeColor Color, char RenderNow, char *FirstParam, ...); 
                                    extern char ndWS_Print (char WndHandle, TypeColor Color, char RenderNow, char *FirstParam, ...); 
                                    extern int  ndWS_GetPixel (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle); 
                                    extern char ndWS_PutPixel (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, TypeColor Color, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern void ndWS_PutPixel_NORENDER_FAST (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, TypeColor Color, unsigned char WndHandle); 
                                    extern char ndWS_DrawLine (int RRPosX1, int RRPosY1, int RRPosX2, int RRPosY2, TypeColor Color, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndWS_DrawPoly (int WndHandle, TypeColor Color, char RenderNow, int NrPixels, ...); 
                                    extern char ndWS_FillArea (int PosX, int PosY, TypeColor Color, TypeColor BorderColor, char WndHandle, char RenderNow); 
                                    extern char ndWS_DrawSpRectangle (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor Color, TypeColor BorderColor, ndint64 CodeStyle, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndWS_DrawRectangle (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndWS_DrawRtRectangle (int CenterRectX, int CenterRectY, int UserDimX, int UserDimY, TypeColor Color, TypeColor BorderColor, ndint64 CodeStyle, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndWS_DrawArcOfEllipse (short unsigned int CenterX, short unsigned int CenterY, float RayA, float RayB, float DegreeA, float DegreeB, TypeColor Color, TypeColor ColorBorder, ndint64 CodeStyle, char WndHandle, char RenderNow); 
                                    extern char ndWS_DrawEllipse (short unsigned int CenterX, short unsigned int CenterY, float RayA, float RayB, TypeColor Color, TypeColor ColorBorder, ndint64 CodeStyle, char WndHandle, char RenderNow); 
                                    extern char ndWS_DrawArcOfCircle (short unsigned int CenterX, short unsigned int CenterY, float Ray, float DegreeA, float DegreeB, TypeColor Color, TypeColor ColorBorder, ndint64 CodeStyle, char WndHandle, char RenderNow); 
                                    extern char ndWS_DrawCircle (short unsigned int CenterX, short unsigned int CenterY, float Ray, TypeColor Color, TypeColor ColorBorder, ndint64 CodeStyle, char WndHandle, char RenderNow); 
                                    extern char ndWS_ClrBufferSpace (unsigned char WndHandle); 
                                    extern char ndWS_ClrALL (unsigned char WndHandle); 
                                    extern char ndWS_ChangeWndColor (TypeColor NewColor, short unsigned int WndHandle); 
                                    extern char ndLP_ChangeWndColor (TypeColor NewColor, short unsigned int WndHandle); 
                                    extern char ndWS_GrWriteChar (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char Carattere, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, ndint64 TextCode); 
                                    extern char ndWS_GrWriteLn (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, char *str, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, ndint64 TextCode); 
                                    extern char ndWS_GrPrintLn (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, ndint64 TextCode, char *FirstParam, ...); 
                                    extern char ndWS_SetFont (unsigned char FntHandle, char WndHandle); 
                                    extern char XWindowRender (unsigned char WndHandle); 
                                    
                              /*
                                 ------------------------------------------------------------
                                 ROUTINES SERVICE SPACE
                                 ------------------------------------------------------------
                              */      
                                    
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern char INTERNAL_ndSS_IsPixelInSS (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char WndHandle); 
                              #endif
                                    
                                    extern void ndSS_PutPixel_NORENDER_FAST (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, TypeColor Color, unsigned char WndHandle); 
                                    extern char ndSS_PutPixel (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, TypeColor Color, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndSS_DrawRectangle (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndSS_DrawRectangleVoid (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndSS_DrawRectangleRounded (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndSS_DrawRectangleRoundedVoid (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor BorderColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndSS_DrawLine (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor Color, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndSS_ClrBufferSpace (unsigned char WndHandle, char RenderNow); 
                                    extern char ndSS_WriteCharInSS (unsigned int RRPosPixelX, unsigned int RRPosPixelY, char Carattere, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndSS_WriteStringInSS (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, char *str, TypeColor Color, TypeColor BGColor, unsigned char WndHandle, unsigned char RenderNow); 
                                    extern char ndSS_EnableServiceSpace (unsigned char WndHandle); 
                                    extern char ndSS_DisableServiceSpace (unsigned char WndHandle);
                               
                              /*
                                 ------------------------------------------------------------
                                 ROUTINES BASE SCREEN
                                 ------------------------------------------------------------
                              */     
                                    
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES       
                                    extern char INTERNAL_ndBS_IsPixelInBS (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY); 
                                    extern char INTERNAL_ndBS_FillArea (int PosX, int PosY, TypeColor Color, TypeColor BorderColor); 
                                    extern char INTERNAL_ndBS_EllipseEngine (short unsigned int CenterX, short unsigned int CenterY, float RayA, float RayB, float RayX, float DegreeA, float DegreeB, float Rot, TypeColor Colore, char DrawRay, char RenderNow); 
                              #endif
                                    
                                    extern int  ndBS_GetPixel (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY); 
                                    extern void ndBS_PutPixel_NORENDER_FAST (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, TypeColor Color); 
                                    extern char ndBS_PutPixel (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, TypeColor Color, unsigned char RenderNow); 
                                    extern char ndBS_DrawLine (int RRPosX1, int RRPosY1, int RRPosX2, int RRPosY2, TypeColor Color, unsigned char RenderNow); 
                                    extern char ndBS_DrawPoly (TypeColor Color, char RenderNow, int NrPixels, ...); 
                                    extern char ndBS_FillArea (int PosX, int PosY, TypeColor Color, TypeColor BorderColor, char RenderNow); 
                                    extern char ndBS_DrawSpRectangle (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor Color, TypeColor BorderColor, ndint64 CodeStyle, unsigned char RenderNow); 
                                    extern char ndBS_DrawRectangle (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor Color, TypeColor BorderColor, unsigned char RenderNow); 
                                    extern char ndBS_DrawRtRectangle (int CenterRectX, int CenterRectY, int UserDimX, int UserDimY, TypeColor Color, TypeColor BorderColor, ndint64 CodeStyle, unsigned char RenderNow); 
                                    extern char ndBS_DrawArcOfEllipse (short unsigned int CenterX, short unsigned int CenterY, float RayA, float RayB, float DegreeA, float DegreeB, TypeColor Color, TypeColor ColorBorder, ndint64 CodeStyle, char RenderNow); 
                                    extern char ndBS_DrawEllipse (short unsigned int CenterX, short unsigned int CenterY, float RayA, float RayB, TypeColor Color, TypeColor ColorBorder, ndint64 CodeStyle, char RenderNow); 
                                    extern char ndBS_DrawArcOfCircle (short unsigned int CenterX, short unsigned int CenterY, float Ray, float DegreeA, float DegreeB, TypeColor Color, TypeColor ColorBorder, ndint64 CodeStyle, char RenderNow); 
                                    extern char ndBS_DrawCircle (short unsigned int CenterX, short unsigned int CenterY, float Ray, TypeColor Color, TypeColor ColorBorder, ndint64 CodeStyle, char RenderNow); 
                                    extern char ndBS_ClrBaseScreen (void); 
                                    extern char ndBS_GrWriteChar (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, unsigned char Carattere, TypeColor Color, TypeColor BGColor, ndint64 TextCode); 
                                    extern char ndBS_GrWriteLn (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, char *str, TypeColor Color, TypeColor BGColor, ndint64 TextCode); 
                                    extern void ndBS_GrPrintLn (short unsigned int RRPosPixelX, short unsigned int RRPosPixelY, TypeColor Color, TypeColor BGColor, ndint64 TextCode, char *FirstParam, ...); 
                                    extern char ndBS_SetFont (unsigned char FntHandle);
                                    extern void BaseScreenRender (void); 
                                    
                                    
                                    
                              /*
                                 ------------------------------------------------------------
                                 ROUTINES DI MANIPOLAZIONE IMMAGINI
                                 ------------------------------------------------------------
                              */       
                                    
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern char INTERNAL_LEGACY_ndIMG_LoadImage_BMP (struct ndImage_Type *MyImage, char *NameFile, char ColorFormat); 
                                    extern char INTERNAL_LEGACY_ndIMG_LoadImage_General (struct ndImage_Type *MyImage, char *NameFile, char ColorFormat); 
                                    extern char INTERNAL_LEGACY_ndIMG_SaveImage_BMP (struct ndImage_Type *MyImage, char *NameFile); 
                                    extern char INTERNAL_LEGACY_ndIMG_SaveImage_General (struct ndImage_Type *MyImage, char *NameFile); 
                              #endif
                                     
                                    extern char ndIMG_GetPixelFromImage (struct ndImage_Type *MyImage, short unsigned int PosX, short unsigned int PosY, TypeColor *MagicNumber); 
                                    extern char ndIMG_GetPixelFromImage_RGB (struct ndImage_Type *MyImage, short unsigned int PosX, short unsigned int PosY, unsigned char *ChannelR, unsigned char *ChannelG, unsigned char *ChannelB); 
                                    extern char ndIMG_PutPixelToImage (struct ndImage_Type *MyImage, short unsigned int PosX, short unsigned int PosY, TypeColor MagicNumber); 
                                    extern char ndIMG_PutPixelToImage_RGB (struct ndImage_Type *MyImage, short unsigned int PosX, short unsigned int PosY, unsigned char ChannelR, unsigned char ChannelG, unsigned char ChannelB); 
                                    extern void ndIMG_InitImage (struct ndImage_Type *ndImgDest); 
                                    extern char ndIMG_CreateImage (struct ndImage_Type *ndImgDest, int LenX, int LenY, char ColorFormat); 
                                    extern char ndIMG_LoadImageFromNDFArray (struct ndImage_Type *ndImgDest, int LenX, int LenY, short unsigned int *PointerToFirstWord, char ColorFormat); 
                                    extern char ndIMG_LoadImage (struct ndImage_Type *MyImage, char *NameFile, char ColorFormat); 
                                    extern char ndIMG_SaveImage (struct ndImage_Type *MyImage, char *NameFile); 
                                    extern char ndIMG_ShowImageScaled (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, float ScaleX, float ScaleY, unsigned char WndHandle, char RenderNow); 
                                    extern char ndIMG_ShowImage (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, unsigned char WndHandle, char RenderNow); 
                                    extern char ndIMG_ShowImageScaled_InBS (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, float ScaleX, float ScaleY, char RenderNow); 
                                    extern char ndIMG_ShowImage_InBS (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, char RenderNow); 
                                    extern void ndIMG_ReleaseImage (struct ndImage_Type *MyImage); 
                                    
                              /*
                                 ------------------------------------------------------------
                                 ROUTINES DI GESTIONE PULSANTI
                                 ------------------------------------------------------------
                              */
                              
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern TypeColor INTERNAL_ConvertToDarkGray (TypeColor Colore); 
                                    extern void INTERNAL_ShowImageBNDark (struct ndImage_Type *MyImage, short int RRPosX, short int RRPosY, unsigned char WndHandle); 
                                    extern void INTERNAL_ButtonRenderer (char ButtonHandle, char WndHandle, char IsInhibited, char Rounded, char RenderNow); 
                              #endif      
                                    
                                    extern char ndCTRL_FindButtonHandleByStringID (char *StringID, char WndHandle); 
                                    extern char ndCTRL_CreateButtonComplex (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, char WndHandle, char *StringID, char *LabelButton1, char *LabelButton2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, short unsigned int Q1PosX1, short unsigned int Q1PosY1, short unsigned int Q1PosX2, short unsigned int Q1PosY2, struct ndImage_Type *IconImage, short unsigned int Q2PosX1, short unsigned int Q2PosY1, short unsigned int Q2PosX2, short unsigned int Q2PosY2, ndint64 ButtonStyle, void* Callback, ndint64 InfoField, char RenderNow); 
                                    extern char ndCTRL_CreateButton (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, char WndHandle, char *StringID, char *LabelButton1, char *LabelButton2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, ndint64 ButtonStyle, void* Callback, ndint64 InfoField, char RenderNow); 
                                    extern char ndCTRL_CreateButtonSmart (short unsigned int RRPosX1, short unsigned int RRPosY1, char WndHandle, char *StringID, char *LabelButton1, char *LabelButton2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, struct ndImage_Type *ImageIcon, ndint64 ButtonStyle, void* Callback, ndint64 InfoField, char RenderNow); 
                                    extern char ndCTRL_DestroyButton (char ButtonHandle, char WndHandle, char OnlyLogical, TypeColor Color, char RenderNow); 
                                    extern char ndCTRL_DestroyButtonUsingStringID (char *StringID, char WndHandle, char OnlyLogical, TypeColor Color, char RenderNow); 
                                    extern char ndCTRL_ChangeButtonParameters_Complex (char ButtonHandle, char WndHandle, char *NewStringID, char *NewLabelButton1, char *NewLabelButton2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, short unsigned int Q1PosX1, short unsigned int Q1PosY1, short unsigned int Q1PosX2, short unsigned int Q1PosY2, struct ndImage_Type *IconImage, short unsigned int Q2PosX1, short unsigned int Q2PosY1, short unsigned int Q2PosX2, short unsigned int Q2PosY2, ndint64 ButtonStyle, char RenderNow); 
                                    extern char ndCTRL_ChangeButtonParametersUsingStringID_Complex (char *StringID, char WndHandle, char *NewStringID, char *NewLabelButton1, char *NewLabelButton2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, short unsigned int Q1PosX1, short unsigned int Q1PosY1, short unsigned int Q1PosX2, short unsigned int Q1PosY2, struct ndImage_Type *IconImage, short unsigned int Q2PosX1, short unsigned int Q2PosY1, short unsigned int Q2PosX2, short unsigned int Q2PosY2, ndint64 ButtonStyle, char RenderNow); 
                                    extern char ndCTRL_ChangeButtonParameters (char ButtonHandle, char WndHandle, char *NewStringID, char *NewLabelButton1, char *NewLabelButton2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, ndint64 ButtonStyle, char RenderNow); 
                                    extern char ndCTRL_ChangeButtonParametersUsingStringID (char *StringID, char WndHandle, char *NewStringID, char *NewLabelButton1, char *NewLabelButton2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, ndint64 ButtonStyle, char RenderNow); 
                                    extern char ndCTRL_InhibitButton (char ButtonHandle, char WndHandle, char RenderNow); 
                                    extern char ndCTRL_DeInhibitButton (char ButtonHandle, char WndHandle, char RenderNow); 
                                    extern char ndCTRL_InhibitButtonUsingStringID (char *StringID, char WndHandle, char RenderNow); 
                                    extern char ndCTRL_DeInhibitButtonUsingStringID (char *StringID, char WndHandle, char RenderNow); 
                                    extern char ndCTRL_ChangeCallBack (int Callback, ndint64 InfoField, char ButtonHandle, char WndHandle, char cbOptions); 
                                    extern char ndCTRL_ChangeCallBackUsingStringID (int Callback, ndint64 InfoField, char *StringID, char WndHandle, char cbOptions); 
                                    extern char ndCTRL_MakeOneButton (char *Message, short unsigned int PosY1, short unsigned int PosY2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, short unsigned int WndHandle, char AutoClose); 
                                    extern char ndCTRL_MakeTwoButton (char *Message1, char *Message2, short unsigned int PosY1, short unsigned int PosY2, TypeColor ColorText, TypeColor ColorButton, TypeColor ColorBorderButton, short unsigned int WndHandle, char AutoClose); 
                                    
                              /*
                                 ------------------------------------------------------------
                                 ROUTINES DI GESTIONE CONTROLLER
                                 ------------------------------------------------------------
                              */     
                                    
                                  
                                    extern char ndCTRL_MoveMousePointerToPos (short unsigned int PosX, short unsigned int PosY); 
                                    
                                    extern void ndCTRL_HideMousePointer (void);
                                    extern void ndCTRL_ShowMousePointer (void); 
                                    
                                    extern void MouseControl (struct ndEvent_Type *EventAddress); 
                                    
                                    void ndProvideMeTheMouse_Until (int *CtrlValue, unsigned char CondCode0, int TheValue0, 
                                                unsigned char CondCode1, int TheValue1);
                                                
                                    void ndProvideMeTheMouse_Check (void *CheckCallbackAdr, ndint64 CodeCheck);
                                    
                              /*
                                 ------------------------------------------------------------
                                 NEW NAME MANAGER
                                 ------------------------------------------------------------
                              */      
                                    
                              #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern void INTERNAL_NNM_InitVar (struct NNMData_Type *MyNNMData); 
                                    extern void INTERNAL_NNM_DrawDisplayVoid (struct NNMData_Type *MyNNMData); 
                                    extern void INTERNAL_NNM_DrawArrowLeftGeneric (struct NNMData_Type *MyNNMData, TypeColor Color); 
                                    extern void INTERNAL_NNM_DrawArrowLeft (struct NNMData_Type *MyNNMData); 
                                    extern void INTERNAL_NNM_UpdateDisplay (struct NNMData_Type *MyNNMData); 
                                    extern void INTERNAL_NNM_AddCharToDisplay (struct NNMData_Type *MyNNMData, char Carattere); 
                                    extern void INTERNAL_NNM_DoBackSpaceToDisplay (struct NNMData_Type *MyNNMData); 
                                    extern void INTERNAL_NNM_DoCancelToDisplay (struct NNMData_Type *MyNNMData);
                                    extern void INTERNAL_NNM_EnterString (struct NNMData_Type *MyNNMData); 
                                    extern void INTERNAL_NNM_NullString (struct NNMData_Type *MyNNMData); 
                                    extern char INTERNAL_GetNewName (char *TitleDisplay, char *FileOutputAddr); 
                              #endif
                                    
                              /*
                                 ------------------------------------------------------------
                                 GESTIONE THUMBNAIL PER IL FILE MANAGER T1
                                 ------------------------------------------------------------
                              */       
                                    
                                    extern char ndIMG_CreateThumbnailFromImage (struct ndImage_Type *ThumbImage, char *CompleteNameFile, short unsigned int KLenX, short unsigned int KLenY); 
                                    extern void ndFM_EnableThumbnail (void); 
                                    extern void ndFM_DisableThumbnail (void); 
                                    
                               /*
                                 ------------------------------------------------------------
                                 ROUTINES GENERICHE PER LA GESTIONE DI LISTE DI INDIRIZZI
                                 ------------------------------------------------------------
                               */     
                                    
                                    extern void ndDB_InitList (struct HEADLIST_Generic_Type *MyListHandle); 
                                    extern char ndDB_AppendElementToList (struct HEADLIST_Generic_Type *MyListHandle, int GenericalAddr); 
                                    extern char ndDB_GetElementFromList (struct HEADLIST_Generic_Type *MyListHandle, int NrElement, int AddressGet); 
                                    extern char ndDB_GetLastElementFromList (struct HEADLIST_Generic_Type *MyListHandle, int AddressGet); 
                                    extern char ndDB_SwapElementInList (struct HEADLIST_Generic_Type *MyListHandle, int NrElement); 
                                    extern char ndDB_RemoveLastElementFromList (struct HEADLIST_Generic_Type *MyListHandle); 
                                    extern void ndDB_DestroyList (struct HEADLIST_Generic_Type *MyListHandle); 
                                    
                                /*
                                 ------------------------------------------------------------
                                 ROUTINES GENERICHE PER LA GESTIONE DI LISTE DI FILE
                                 ------------------------------------------------------------
                                */    
                                    
                                    extern void ndDB_InitListOfFile (struct HEADLIST_File_Type *MyListHandle); 
                                    extern char ndDB_AppendFileToList (struct HEADLIST_File_Type *MyListHandle, char *Namefile, char *Ext, int Size, char IsDir, struct ndFS_DateTime_Type *MyDateTime); 
                                    extern char ndDB_GetFileFromList (struct HEADLIST_File_Type *MyListHandle, int NrElement, struct ELEMENT_File_Type *MyFile); 
                                    extern char ndDB_GetFileAddrFromList (struct HEADLIST_File_Type *MyListHandle, int NrElement, struct ELEMENT_File_Type *AddressGet); 
                                    extern char ndDB_SwapFileInList (struct HEADLIST_File_Type *MyListHandle, int NrElement); 
                                    extern char ndDB_RemoveLastFileFromList (struct HEADLIST_File_Type *MyListHandle); 
                                    extern void ndDB_DestroyListOfFile (struct HEADLIST_File_Type *MyListHandle); 
                                    extern char ndDB_CompareFile (char *Name1, char *Ext1, char IsDir1, char *Name2, char *Ext2, char IsDir2); 
                                    extern char ndDB_FillFileList (char *DirPath, struct HEADLIST_File_Type *MyListHandle); 
                                    
                                /*
                                 ------------------------------------------------------------
                                 ROUTINES GENERICHE PER LA GESTIONE DI LISTE DI ICONE
                                 ------------------------------------------------------------
                                */    
                                
                                #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern char INTERNAL_ndDB_AppendIconToList (struct HEADLIST_Icon_Type *MyListHandle, char *StringID, char Format, struct ndImage_Type *MyImage, char SystemAuth); 
                                    extern char INTERNAL_RemoveLastIconFromList (struct HEADLIST_Icon_Type *MyListHandle); 
                                #endif   
                                    
                                    extern void ndDB_InitListOfIcons (struct HEADLIST_Icon_Type *MyListHandle); 
                                    extern char ndDB_IsPresentIconStringID (struct HEADLIST_Icon_Type *MyListHandle, char *StringID); 
                                    extern char ndDB_AppendIconToList (struct HEADLIST_Icon_Type *MyListHandle, char *StringID, char Format, struct ndImage_Type *MyImage); 
                                    extern char ndDB_GetIconFromList (struct HEADLIST_Icon_Type *MyListHandle, char *StringID, struct ELEMENT_Icon_Type *MyIcon); 
                                    extern char ndDB_GetIconAddrFromList (struct HEADLIST_Icon_Type *MyListHandle, char *StringID, int *AddrMyIconPointer); 
                                    extern char ndDB_RemoveIconFromList (struct HEADLIST_Icon_Type *MyListHandle, char *StringID); 
                                    extern void ndDB_DestroyListOfIcon (struct HEADLIST_Icon_Type *MyListHandle); 
                                    extern void ndIMG_ShowIcon (struct ELEMENT_Icon_Type *MyIcon, short unsigned int RRPosX, short unsigned int RRPosY, char XWndHandle); 
                                    extern void InitializeStandardIconList (void); 
                                    
                               /*
                                 ------------------------------------------------------------
                                 ROUTINES GENERICHE PER LA GESTIONE DI LISTE DI CLASSI
                                 ------------------------------------------------------------
                               */     
                                    
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern char INTERNAL_RemoveLastClassFromList (struct HEADLIST_Class_Type *MyListHandle); 
                               #endif  
                               
                                    extern void ndDB_InitListOfClasses (struct HEADLIST_Class_Type *MyListHandle); 
                                    extern char ndDB_IsPresentSameNameClass (struct HEADLIST_Class_Type *MyListHandle, char *NameClass); 
                                    extern char ndDB_AppendClassToList (struct HEADLIST_Class_Type *MyListHandle, char *NameClass, char *Extensions, char *StringIDIcon, int PointerToThumbFunc); 
                                    extern char ndDB_GetClassFromList (struct HEADLIST_Class_Type *MyListHandle, char *NameClass, struct ELEMENT_Class_Type *MyClass); 
                                    extern char ndDB_GetClassAddrFromList (struct HEADLIST_Class_Type *MyListHandle, char *NameClass, int *PointerToClassAddr); 
                                    extern char ndDB_RemoveClassFromList (struct HEADLIST_Class_Type *MyListHandle, char *NameClass); 
                                    extern void ndDB_DestroyListOfClass (struct HEADLIST_Class_Type *MyListHandle); 
                                    extern char ndDB_FindClassFromExtension (struct HEADLIST_Class_Type *MyListHandle, char *Ext, struct ELEMENT_Class_Type *MyClass); 
                                    extern char ndDB_FindClassAddrFromExtension (struct HEADLIST_Class_Type *MyListHandle, char *Ext, int *MyClassAddr); 
                                    extern char ndDB_ModifyClass (struct HEADLIST_Class_Type *MyListHandle, char *OldNameClass, char *NewNameClass, char *Extensions, char *StringIDIcon, int PointerToThumbFunc); 
                                    extern void InitializeStandardClassList (void); 
                                
                               /*
                                 ------------------------------------------------------------
                                 ROUTINES GENERICHE PER LA GESTIONE DELLE LISTE DI L2WMI
                                 ------------------------------------------------------------
                               */
                                    
                                    extern void ndDB_InitList_of_L2WMI (struct HEADLIST_L2WMI_Type *MyListHandle, char Dynamic); 
                                    extern char ndDB_IsPresentSameStringID_in_L2WMI_List (struct HEADLIST_L2WMI_Type *MyListHandle, char *StringID); 
                                    extern char ndDB_Append_L2WMI_ToList (struct HEADLIST_L2WMI_Type *MyListHandle, char *StringID, char *NameItem, char Item_Inhibited, char ExecInNewContext, void *CallbackFunction); 
                                    extern char ndDB_GetL2WMIFromList (struct HEADLIST_L2WMI_Type *MyListHandle, int NrElement, struct ELEMENT_L2WMI_Type *MyL2WMI); 
                                    extern char ndDB_GetL2WMIAddrFromList (struct HEADLIST_L2WMI_Type *MyListHandle, int NrElement, int *AddressGet); 
                                    extern char ndDB_GetL2WMIFromList_UsingStringID (struct HEADLIST_L2WMI_Type *MyListHandle, char *StringID, struct ELEMENT_L2WMI_Type *MyL2WMI); 
                                    extern char ndDB_GetL2WMIAddrFromList_UsingStringID (struct HEADLIST_L2WMI_Type *MyListHandle, char *StringID, int *AddressGet); 
                                    extern char ndDB_RemoveLastL2WMIFromList (struct HEADLIST_L2WMI_Type *MyListHandle); 
                                    extern void ndDB_DestroyListOfL2WMI (struct HEADLIST_L2WMI_Type *MyListHandle); 
                                    
                               /*
                                 ------------------------------------------------------------
                                 ROUTINES GENERICHE PER LA GESTIONE DELLE LISTE DI L1WMI
                                 ------------------------------------------------------------
                               */     
                                    
                                    extern void ndDB_InitList_of_L1WMI (struct HEADLIST_L1WMI_Type *MyListHandle, char Dynamic); 
                                    extern char ndDB_IsPresentSameStringID_in_L1WMI_List (struct HEADLIST_L1WMI_Type *MyListHandle, char *StringID); 
                                    extern char ndDB_Append_L1WMI_ToList (struct HEADLIST_L1WMI_Type *MyListHandle, char *StringID, char *NameItem, char Item_Inhibited, char ExecInNewContext, char Item_Expanded, void *CallbackFunction); 
                                    extern char ndDB_GetL1WMIFromList (struct HEADLIST_L1WMI_Type *MyListHandle, int NrElement, struct ELEMENT_L1WMI_Type *MyL1WMI); 
                                    extern char ndDB_GetL1WMIAddrFromList (struct HEADLIST_L1WMI_Type *MyListHandle, int NrElement, int *AddressGet); 
                                    extern char ndDB_GetL1WMIFromList_UsingStringID (struct HEADLIST_L1WMI_Type *MyListHandle, char *StringID, struct ELEMENT_L1WMI_Type *MyL1WMI); 
                                    extern char ndDB_GetL1WMIAddrFromList_UsingStringID (struct HEADLIST_L1WMI_Type *MyListHandle, char *StringID, int *AddressGet); 
                                    extern char ndDB_RemoveLastL1WMIFromList (struct HEADLIST_L1WMI_Type *MyListHandle); 
                                    extern void ndDB_DestroyListOfL1WMI (struct HEADLIST_L1WMI_Type *MyListHandle); 
                                    
                               /*
                                 ------------------------------------------------------------
                                 API PER LA GESTIONE DEI MENU
                                 ------------------------------------------------------------
                               */    
                                    
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern char INTERNAL_ndDB_MenuParser_Read (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString, int *AddressGet); 
                                    extern char INTERNAL_ndDB_MenuParser_Write (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString, char *NameItem, char Item_Inhibited, char ExecInNewContext, char Item_Expanded, void *CallbackFunction); 
                                    extern int  INTERNAL_ndDB_NumberOfL1WMIs (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString); 
                                    extern int  INTERNAL_ndDB_NumberOfL2WMIs (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString); 
                                    extern char INTERNAL_ndDB_EnumL1WMI (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString, int NrElement, int *AddressGet); 
                                    extern char INTERNAL_ndDB_EnumL2WMI (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString, int NrElement, int *AddressGet); 
                               #endif
                                    
                                    extern char ndWMENU_DefineMenuItem (unsigned char WndHandle, char *ZString, char *NameItem, char Features, void *CallbackFunction, unsigned char RenderNow);
                                    extern char ndWMENU_FeaturesMenuItem (unsigned char WndHandle, char *ZString, int *AddressGet); 
                                    extern int  ndWMENU_NumberOfL1WMIs (char WndHandle, char *ZString); 
                                    extern int  ndWMENU_NumberOfL2WMIs (char WndHandle, char *ZString); 
                                    extern char ndWMENU_EnumL1WMI (char WndHandle, char *ZString, int NrElement, int *AddressGet); 
                                    extern char ndWMENU_EnumL2WMI (char WndHandle, char *ZString, int NrElement, int *AddressGet); 
                                    extern char ndWMENU_InhibitL1WMI (char WndHandle, char *ZString, char RenderNow); 
                                    extern char ndWMENU_DeInhibitL1WMI (char WndHandle, char *ZString, char RenderNow); 
                                    extern char ndWMENU_InhibitL2WMI (char WndHandle, char *ZString, char RenderNow); 
                                    extern char ndWMENU_DeInhibitL2WMI (char WndHandle, char *ZString, char RenderNow); 
                                    extern char ndWMENU_ChangeCallback_For_L1WMI (char WndHandle, char *ZString, int NewCallback); 
                                    extern char ndWMENU_ChangeCallback_For_L2WMI (char WndHandle, char *ZString, int NewCallback); 
                                    extern char ndWMENU_DestroyL1SubMenu (char WndHandle, char *ZString); 
                                    extern char ndWMENU_DestroyL2SubMenu (char WndHandle, char *ZString); 
                                   
                               /*
                                 ------------------------------------------------------------
                                 API PER LA GESTIONE DEI MENU BAR
                                 ------------------------------------------------------------
                               */  
                                    
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern void INTERNAL_CreateClearMenuBar (unsigned char WndHandle); 
                                    extern void INTERNAL_ClearLogicalButtonBuffer (unsigned char WndHandle); 
                                    extern char INTERNAL_StrCatZString (char *TempBuffer, char *Str1, char *Str2); 
                               #endif       
                                    
                                    extern char ndMB_PutPixel (short unsigned int RPosPixelX, short unsigned int RPosPixelY, TypeColor Color, unsigned char WndHandle); 
                                    extern char ndMB_WriteChar (short unsigned int RPosPixelCharX, short unsigned int RPosPixelCharY, char Carattere, TypeColor Color, TypeColor BGColor, unsigned char WndHandle); 
                                    extern char ndMB_Write (short unsigned int RPosPixelX, short unsigned int RPosPixelY, char *str, TypeColor Color, TypeColor BGColor, unsigned char WndHandle); 
                                    extern char ndMB_DrawRectangle (short unsigned int PosX1, short unsigned int PosY1, short unsigned int PosX2, short unsigned int PosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle); 
                                    extern char ndMB_DrawRectangleVoid (short unsigned int PosX1, short unsigned int PosY1, short unsigned int PosX2, short unsigned int PosY2, TypeColor BorderColor, unsigned char WndHandle); 
                                    extern char ndMB_SetFont (unsigned char FntHandle, char WndHandle); 
                                    extern void ndMB_CreateButton_BackMenu (unsigned char WndHandle); 
                                    extern void ndMB_CreateButton_ScrollMenuToLeft (unsigned char WndHandle); 
                                    extern void ndMB_CreateButton_ScrollMenuToRight (short unsigned int LastElementShown, short unsigned int ElementsInTotal, unsigned char WndHandle); 
                                    extern void ndMB_CreateButton_for_L1WMI (short unsigned int RPosX1, short unsigned int RPosX2, char *NameItem, unsigned char Inhibited, unsigned char Activated, unsigned char WndHandle); 
                                    extern void ndMB_CreateButton_for_L2WMI (short unsigned int SSPosX1, short unsigned int SSPosY1, short unsigned int SSPosX2, short unsigned int SSPosY2, char *NameItem, unsigned char Inhibited, unsigned char WndHandle); 
                                    extern void ndMB_CreateButton_for_ScrollUp_L2WMI (short unsigned int SSPosX1, short unsigned int SSPosY1, short unsigned int SSPosX2, short unsigned int SSPosY2, unsigned char WndHandle); 
                                    extern void ndMB_CreateButton_for_ScrollDown_L2WMI (short unsigned int SSPosX1, short unsigned int SSPosY1, short unsigned int SSPosX2, short unsigned int SSPosY2, unsigned char WndHandle); 
                                    extern void ndL2WinMenuRender (char *ZString, short unsigned int Theoretical_PosX1, short unsigned int Theoretical_PosX2, unsigned char WndHandle); 
                                    extern char ndWinMenuRender (unsigned char WndHandle); 
                                 
                               /*
                                 ------------------------------------------------------------
                                 FUNZIONALITA' COMUNI A FILE MANAGER DI TIPO 1 E DI TIPO 2
                                 ------------------------------------------------------------
                               */  
                                    
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES     
                                    extern char INTERNAL_T12_RecursiveDestroyDir (char *ShortNameDir, char NoGenWnd, char EredWndHandle); 
                                    extern void INTERNAL_T12_VoidNameErrorBox (void); 
                                    extern void INTERNAL_T12_MkDirErrorBox (char *NameDir); 
                                    extern char INTERNAL_T12_ConfirmErase (char *NameElement, char IsDir); 
                                    extern void INTERNAL_T12_ErrorOnErase (char *NameElement, char IsDir); 
                                    extern void INTERNAL_T12_SuccessOnErase (char *NameElement, char IsDir); 
                                    extern char INTERNAL_T12_ConfirmRename (char *OldNameElement, char *NewNameElement, char IsDir); 
                                    extern void INTERNAL_T12_ErrorOnRename (char *NameElement, char IsDir); 
                                    extern void INTERNAL_T12_SuccessOnRename (char *NameElement, char IsDir); 
                                    extern char INTERNAL_TEST_NameVoid (char *Name); 
                                    extern char INTERNAL_TEST_TestName (char *Name); 
                               #endif
                               
                               /*
                                 ------------------------------------------------------------
                                 FILE MANAGER DI TIPO 1
                                 ------------------------------------------------------------
                               */
                               
                                    extern char IsPresentThumbInCache (struct T1FMData_Type *MyFMData, struct ndImage_Type *ThumbImage, char *CompleteName); 
                                                                                             
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern void INTERNAL_T1_SetFMWindowParams (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_CreateShortPath (struct T1FMData_Type *MyFMData, char *SysPathAddr, char *NewSysPathAddr); 
                                    extern void INTERNAL_T1_DrawSysPathBarVoid (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_DrawSysPathBar (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_DrawMainButton (struct T1FMData_Type *MyFMData, char ChangeDirMode); 
                                    extern void INTERNAL_T1_DrawSecondaryButton (struct T1FMData_Type *MyFMData, char SaveMode); 
                                    extern void INTERNAL_T1_DrawButton (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_DrawActiveArea (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_DrawLogicalButton (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_DrawButtonForFileScrollUp (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_DrawButtonForFileScrollDown (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_DrawButtonForChDirPointPoint (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_UpdateChDirPointPointButton (struct T1FMData_Type *MyFMData); 
                                    extern char INTERNAL_T1_DrawFMWindows (struct T1FMData_Type *MyFMData, char *Title); 
                                    extern void INTERNAL_T1_ClrActiveArea (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_ClrBoxArea (struct T1FMData_Type *MyFMData); 
                                    extern void INTERNAL_T1_DrawArrowUpInListFile (struct T1FMData_Type *MyFMData, char ArrowUp); 
                                    extern void INTERNAL_T1_DrawArrowDownInListFile (struct T1FMData_Type *MyFMData, char ArrowDown); 
                                    extern void INTERNAL_T1_ShowThumbnailOnScreen (struct ndImage_Type *MyImage, short unsigned int TPosX, short unsigned int TPosY, short unsigned int XWndHandle); 
                                    extern void INTERNAL_T1_SaveThumbnailInCache (struct T1FMData_Type *MyFMData, struct ndImage_Type *NewThumb, char *CompleteName); 
                                    extern void INTERNAL_DestroyThumbNailCache (struct T1FMData_Type *MyFMData); 
                                    
                                    extern void INTERNAL_T1_DrawThumbnailOrIcon (struct T1FMData_Type *MyFMData, short unsigned int TPosX, short unsigned int TPosY, char *CompleteName, char *Ext); 
                                    extern void INTERNAL_T1_DrawFolderIcon (struct T1FMData_Type *MyFMData, short unsigned int TPosX, short unsigned int TPosY); 
                                    extern void INTERNAL_T1_DrawFolderExitIcon (struct T1FMData_Type *MyFMData, short unsigned int TPosX, short unsigned int TPosY); 
                                    extern void INTERNAL_T1_ShowSingleElement (struct T1FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int FirstElement, int Offset, TypeColor XColor, char ReDrawButton); 
                                    extern void INTERNAL_T1_ShowFileList (struct T1FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int FirstElement); 
                                    extern void INTERNAL_T1_EvidenceFileItem (struct T1FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int PointerToItem, int FirstItemInFocus, char ReDrawButton); 
                                    extern void INTERNAL_T1_DeEvidenceFileItem (struct T1FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int PointerToItem, int FirstItemInFocus, char ReDrawButton); 
                                    extern char INTERNAL_T1_FileManagerChDir (char *ShortNameNewDir, char *ShortExtNewDir); 
                                    extern char INTERNAL_T1_FileManagerChooseFile (char *ShortNameDir, char *ShortExtDir, char *OutputFileName); 
                                    extern char INTERNAL_T1_FileManagerNewFile (char *FileOutputAddr); 
                                    extern char INTERNAL_T1_FileManagerEraseFile (char *ShortNameFile, char *ShortExtFile); 
                                    extern char INTERNAL_T1_FileManagerRmDir (char *ShortNameDir, char *ShortExtDir); 
                                    extern char INTERNAL_T1_FileManagerMkDir (void); 
                                    extern char INTERNAL_T1_FileManagerRename (char *ShortNameFile, char *ShortExtFile, char IsDir); 
                                    
                                    extern char INTERNAL_T1FileManager (char *Title, unsigned char SaveMode, char *StringOutputAddr); 
                               #endif
                               
                               /*
                                 ------------------------------------------------------------
                                 FILE MANAGER DI TIPO 2
                                 ------------------------------------------------------------
                               */
                                    
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern void INTERNAL_T2_SetFMWindowParams (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_CreateShortPath (struct T2FMData_Type *MyFMData, char *SysPathAddr, char *NewSysPathAddr); 
                                    extern void INTERNAL_T2_DrawSysPathBarVoid (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_DrawSysPathBar (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_DrawMainButton (struct T2FMData_Type *MyFMData, char ChangeDirMode); 
                                    extern void INTERNAL_T2_DrawSecondaryButton (struct T2FMData_Type *MyFMData, char SaveMode); 
                                    extern void INTERNAL_T2_DrawButton (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_DrawActiveArea (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_DrawButtonForFileScrollUp (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_DrawButtonForFileScrollDown (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_DrawButtonForChDirPointPoint (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_UpdateChDirPointPointButton (struct T2FMData_Type *MyFMData); 
                                    extern char INTERNAL_T2_DrawFMWindows (struct T2FMData_Type *MyFMData, char *Title); 
                                    extern void INTERNAL_T2_ClrMemoryChar (struct T2FMData_Type *MyFMData); 
                                    extern void INTERNAL_T2_DrawArrowUpInListFile (struct T2FMData_Type *MyFMData, char ArrowUp); 
                                    extern void INTERNAL_T2_DrawArrowDownInListFile (struct T2FMData_Type *MyFMData, char ArrowDown); 
                                    extern void INTERNAL_T2_ShowSingleElement (struct T2FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int FirstElement, int Offset, TypeColor XColor, char ReDrawButton); 
                                    extern void INTERNAL_T2_ShowFileList (struct T2FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int FirstElement); 
                                    extern void INTERNAL_T2_EvidenceFileItem (struct T2FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int PointerToItem, int FirstItemInFocus); 
                                    extern void INTERNAL_T2_DeEvidenceFileItem (struct T2FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int PointerToItem, int FirstItemInFocus); 
                                    extern char INTERNAL_T2_FileManagerChDir (char *ShortNameNewDir, char *ShortExtNewDir); 
                                    extern char INTERNAL_T2_FileManagerChooseFile (char *ShortNameDir, char *ShortExtDir, char *OutputFileName); 
                                    extern char INTERNAL_T2_FileManagerNewFile (char *FileOutputAddr); 
                                    extern char INTERNAL_T2_FileManagerEraseFile (char *ShortNameFile, char *ShortExtFile); 
                                    extern char INTERNAL_T2_FileManagerRmDir (char *ShortNameDir, char *ShortExtDir); 
                                    extern char INTERNAL_T2_FileManagerMkDir (void); 
                                    extern char INTERNAL_T2_FileManagerRename (char *ShortNameFile, char *ShortExtFile, char IsDir); 
                                    extern char INTERNAL_T2FileManager (char *Title, unsigned char SaveMode, char *StringOutputAddr); 
                               #endif
                               
                               /*
                                 ------------------------------------------------------------
                                 FILE MANAGER DI TIPO 1 E DI TIPO 2
                                 ------------------------------------------------------------
                               */
                                    
                                    extern char FileManager (char *Title, unsigned char TypeManager, unsigned char SaveMode, char *StringOutputAddr); 
                                    
                               /*
                                 ------------------------------------------------------------
                                 TRACKBARS
                                 ------------------------------------------------------------
                               */
                                    
                                    extern char  ndTBAR_DrawProgressBar (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, float Value, float MinValue, float MaxValue, TypeColor StringColor, TypeColor BarColor, TypeColor BGBarColor, TypeColor BorderColor, ndint64 Attribute, char WndHandle, char RenderNow); 
                                    
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES     
                                    extern void  INTERNAL_ElabLabel (char *Stringa1, char *Stringa2, char Number); 
                                    extern float INTERNAL_GetStep (char StepCode); 
                                    extern void  INTERNAL_GetStepString (char StepCode, char *Str); 
                                    extern char  INTERNAL_GetStepCodeFromTrackAttribute (ndint64 TrackAttribute); 
                                    extern short unsigned int INTERNAL_CalcLengthBar (char TBHandle, char WndHandle); 
                                    extern short unsigned int INTERNAL_CalcLengthStepDisplay (char TBHandle, char WndHandle); 
                                    
                                    extern void INTERNAL_DrawTrackBarBody (char TBHandle, char WndHandle); 
                                    extern void INTERNAL_DrawMinusButton (char TBHandle, char WndHandle, int TB_CallBack, int TB_InfoField); 
                                    extern void INTERNAL_RenderProgressBar (char TBHandle, char WndHandle, char RenderNow); 
                                    extern void INTERNAL_DrawPlusButton (char TBHandle, char WndHandle, int TB_CallBack, int TB_InfoField); 
                                    extern void INTERNAL_DrawInfoButton (char TBHandle, char WndHandle, int InfoCallBack); 
                                    extern void INTERNAL_DrawStepDisplay (char TBHandle, char WndHandle, char RenderNow); 
                                    extern void INTERNAL_DrawPlusStepButton (char TBHandle, char WndHandle); 
                                    extern void INTERNAL_DrawMinusStepButton (char TBHandle, char WndHandle); 
                                    extern ndint64 INTERNAL_EngineChangeStep (char *StringID, ndint64 InfoField, char WndHandle); 
                                    extern ndint64 INTERNAL_EngineChangeValue (char *StringID, ndint64 InfoField, char WndHandle); 
                                    extern char INTERNAL_TrackBarRenderer (char TBHandle, char WndHandle, int TB_CallBack, int TB_InfoField, int TB_InfoCallBack, char RenderNow); 
                               #endif
                                    
                                    extern short unsigned int ndTBAR_ValidateLenXTrackBar (char ChangeStepEnabled); 
                                    extern short unsigned int ndTBAR_ValidateLenYTrackBar (char ChangeStepEnabled); 
                                    
                                    extern char ndTBAR_CreateTrackBar (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor TextColor, TypeColor PBarColor, TypeColor PBarBkgrColor, TypeColor PBarBorderColor, TypeColor TBarColor, TypeColor TBarBorderColor, float Value, float MinValue, float MaxValue, ndint64 TrackAttribute, int TB_CallBack, int InfoCallBack, char WndHandle, char RenderNow); 
                                    extern char ndTBAR_CreateLabeledTrackBar (short unsigned int RRPosX1, short unsigned int RRPosY1, short unsigned int RRPosX2, short unsigned int RRPosY2, TypeColor TextColor, TypeColor PBarColor, TypeColor PBarBkgrColor, TypeColor PBarBorderColor, TypeColor TBarColor, TypeColor TBarBorderColor, char *Label, TypeColor LabelColor, TypeColor ExoRectColor, TypeColor ExoRectBorderColor, float Value, float MinValue, float MaxValue, ndint64 TrackAttribute, int TB_CallBack, int InfoCallBack, char WndHandle, char RenderNow); 
                                    extern char ndTBAR_GetValue (float *Value, char TBHandle, char WndHandle); 
                                    extern char ndTBAR_SetValue (float Value, char TBHandle, char WndHandle, char RenderNow); 
                                    extern char ndTBAR_InhibitTrackBar (char TBHandle, char WndHandle, char RenderNow); 
                                    extern char ndTBAR_DeInhibitTrackBar (char TBHandle, char WndHandle, char RenderNow); 
                                    extern char ndTBAR_DestroyTrackBar (char TBHandle, char WndHandle, char OnlyLogical, TypeColor Color, char RenderNow); 
                                    
                               /*
                                 ------------------------------------------------------------
                                 TEXTBOX/TEXTAREA
                                 ------------------------------------------------------------
                               */
                                
                                #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES     
                                    extern char *INTERNAL_GetCharAddr (int PosX, int PosY, int TextBoxHandle, char WndHandle, char Reserve); 
                                    extern char *INTERNAL_GetRowAddr (int PosY, int TextBoxHandle, char WndHandle, char Reserve); 
                                    extern void INTERNAL_TextAreaRenderer (char State, char CursorOn, int TextBoxHandle, char WndHandle, char Reserve, char ToBegin, char RenderNow); 
                                    extern char INTERNAL_EnableTestArea (int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_DisableTestArea (int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_SignalCursorPos (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_LastRowIsFull (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_TestArea_AddCharToDisplay (unsigned char LoCode, int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_TestArea_MoveCursor (int DiffX, int DiffY, int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_TestArea_CarriageReturn (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_TestArea_DeleteKey (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_TestArea_CancKey (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_TestArea_OkKey (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_TestArea_EscKey (int VKBHandle, int TextBoxHandle, char TextBoxWndHandle); 
                                    extern void INTERNAL_TestArea_BreakCallback (void *BreakCallbackData, int VKBHandle, char WndHandle); 
                                    extern void INTERNAL_TextArea_KeyCallback (int KeyPressedCode, void *KeyCallbackData, int VKBHandle, char WndHandle); 
                                    extern ndint64 INTERNAL_TextAreaManager (char *StringID, ndint64 InfoField, char WndHandle); 
                                #endif    
                                    
                                    extern char *ndTBOX_GetCharAddr (int PosX, int PosY, int TextBoxHandle, char WndHandle); 
                                    extern char *ndTBOX_GetRowAddr (int PosY, int TextBoxHandle, char WndHandle); 
                                    extern int  ndTBOX_GetNrCharsX (int TextBoxHandle, char WndHandle); 
                                    extern int  ndTBOX_GetNrCharsY (int TextBoxHandle, char WndHandle); 
                                    extern int  ndTBOX_CreateTextArea (short unsigned int PosX1, short unsigned int PosY1, short unsigned int PosX2, short unsigned int PosY2, char *DescriptorString, ndint64 Options, void *TextData, TypeColor TextColorOn, TypeColor BgTextColorOn, TypeColor TextColorOff, TypeColor BgTextColorOff, TypeColor TextColorInh, TypeColor BgTextColorInh, void *cbProcessValueBefore, void *cbProcessValueForValidation, void *cbProcessValueAfter, void *ProcessValueData, char WndHandle, char RenderNow); 
                                    extern char ndTBOX_InhibitTextArea (int TextBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndTBOX_DeInhibitTextArea (int TextBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndTBOX_ChangeTextAreaContent (char TextBoxHandle, char WndHandle, void *NewContent, char RenderNow); 
                                    extern char ndTBOX_TextAreaUpdate (char TextBoxHandle, char WndHandle, char RenderNow);
                                    extern char ndTBOX_DestroyTextArea (char TextBoxHandle, char WndHandle, char OnlyLogical, TypeColor CoverColor, char RenderNow); 

                               /*
                                 ------------------------------------------------------------
                                 LISTBOX
                                 ------------------------------------------------------------
                               */     
                               
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES      
                                    extern void ndDB_InitListOfLBoxItem (struct HEADLIST_LBoxItem_Type *MyListHandle); 
                                    extern char ndDB_AppendLBoxItemToList (struct HEADLIST_LBoxItem_Type *MyListHandle, char *NameLBoxItem, void *LBoxCallback, ndint64 LBoxCode, char IsInhibited); 
                                    extern char ndDB_GetLBoxItemFromList (struct HEADLIST_LBoxItem_Type *MyListHandle, int NrElement, struct ELEMENT_LBoxItem_Type *MyLBoxItem); 
                                    extern char ndDB_GetLBoxItemAddrFromList (struct HEADLIST_LBoxItem_Type *MyListHandle, int NrElement, void *AddressGet); 
                                    extern char ndDB_SwapLBoxItemInList (struct HEADLIST_LBoxItem_Type *MyListHandle, int NrElement); 
                                    extern char ndDB_RemoveLastLBoxItemFromList (struct HEADLIST_LBoxItem_Type *MyListHandle); 
                                    extern char ndDB_RemoveSpecificLBoxItemFromList (struct HEADLIST_LBoxItem_Type *MyListHandle, int NrElement); 
                                    extern void ndDB_DestroyListOfLBoxItem (struct HEADLIST_LBoxItem_Type *MyListHandle); 
                                    
                                    extern void INTERNAL_ListBoxRenderer (int, char, char);
                                    extern ndint64 INTERNAL_ListBoxManager (char *StringID, ndint64 InfoField, char WndHandle); 
                                    extern ndint64 INTERNAL_ListBoxManagerForScrollUp (char *StringID, ndint64 InfoField, char WndHandle); 
                                    extern ndint64 INTERNAL_ListBoxManagerForScrollDown (char *StringID, ndint64 InfoField, char WndHandle); 
                                    extern void INTERNAL_ListBox_DrawArrowUp (int PosX, int PosY, TypeColor Color, int ListBoxHandle, char WndHandle); 
                                    extern void INTERNAL_ListBox_DrawArrowDown (int PosX, int PosY, TypeColor Color, int ListBoxHandle, char WndHandle); 
                                    extern void INTERNAL_CreateKeysOfListBox (int ListBoxHandle, char WndHandle, char CallbacksInNewContext); 
                                    extern void INTERNAL_DestroyKeysOfListBox (char OnlyLogical, TypeColor CoverColor, int ListBoxHandle, char WndHandle); 
                                    extern void INTERNAL_ListBoxRenderer (int ListBoxHandle, char WndHandle, char RenderNow); 
                               #endif
                                    
                                    // API pubblica
                                    
                                    extern int ndLBOX_CreateListBox (short unsigned int PosX1, short unsigned int PosY1, short unsigned int PosX2, short unsigned int PosY2, ndint64 Options, struct ndListBoxColors_Type *ListBoxColor, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_AddItemToListBox (char *NameLBoxItem, void *LBoxCallback, ndint64 LBoxCode, char IsInhibited, int ListBoxHandle, char WndHandle); 
                                    extern char ndLBOX_UpdateListBox (int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern int ndLBOX_FindListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle); 
                                    extern char ndLBOX_InhibitListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_InhibitListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_DeInhibitListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_DeInhibitListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_EvidenceListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_EvidenceListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_DeEvidenceListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_DeEvidenceListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndLBOX_RemoveListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow); /* (NrElement, ListBoxHandle, WndHandle, RenderNow) int NrElement; int ListBoxHandle; char WndHandle; char RenderNow; */
                                    extern char ndLBOX_RemoveListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow); /* (Name, ListBoxHandle, WndHandle, RenderNow) char *Name; int ListBoxHandle; char WndHandle; char RenderNow; */
                                    extern char ndLBOX_DestroyListBox (char ListBoxHandle, char WndHandle, char OnlyLogical, TypeColor CoverColor, char RenderNow); /* (ListBoxHandle, WndHandle, OnlyLogical, CoverColor, RenderNow) char ListBoxHandle; char WndHandle; char OnlyLogical; TypeColor CoverColor; char RenderNow; */
   
                               /*
                                 ------------------------------------------------------------
                                 SUPPORTO CBOX
                                 ------------------------------------------------------------
                               */ 
                               
                               #ifdef I_NEED_TO_DEFINE_ALL_PROTOTYPES
                                    extern void INTERNAL_CBoxRenderer (short unsigned int PosX, short unsigned int PosY, char IsOn, char IsInhibited, TypeColor MainColor, char WndHandle, char RenderNow); 
                                    extern ndint64 INTERNAL_CBoxEngine (char *StringID, ndint64 InfoField, char WndHandle); 
                               #endif     
                                    
                                    extern int  ndCBOX_CreateCBox (short unsigned int PosX, short unsigned int PosY, char *StringID, char IsOn, ndint64 Features, int *AdrVariable, TypeColor MainColor, void *CBoxCallback, char WndHandle, char RenderNow); 
                                    extern char ndCBOX_DestroyCBox (int CBoxHandle, char WndHandle, char OnlyLogical, TypeColor Color, char RenderNow); 
                                    extern char ndCBOX_InhibitCBox (int CBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndCBOX_DeInhibitCBox (int CBoxHandle, char WndHandle, char RenderNow); 
                                    extern char ndCBOX_SetCboxOn (int CBoxHandle, char WndHandle, char ExecCallBack, char RenderNow); 
                                    extern char ndCBOX_SetCboxOff (int CBoxHandle, char WndHandle, char ExecCallBack, char RenderNow); 
                                    extern char ndCBOX_ChangeCallback (int CBoxHandle, char WndHandle, void *NewCallBack); 

                               
                               
                               /*
                                 ------------------------------------------------------------
                                 STD-IN/STD-OUT
                                 ------------------------------------------------------------
                               */     
                                    
                                    extern void ndInitStdInSystem (void); 
                                    extern void ndSET_SystemKeyboardAutoClose (char Value); 
                                    extern char ndSET_SystemKeyboardType (int Type); 
                                    extern int  ndGetChar (void); 
                                    extern void ndCloseSystemKeyboard (void); 
                                    
                                    extern void ndInitStdOutSystem (void); 
                                    extern int  ndActivateStdOutDialogBox (void); 
                                    extern int  ndWriteToStdOut (char Carattere, char TypeChannel, char RenderNow);      
                                    extern void ndDisableStdOutDialogBox (void); 
                                    extern char ndEnableStdOutCustomDims (int NewDimX, int NewDimY);
                                    extern void ndDisableStdOutCustomDims (void);
                                    extern char ndChangeStdOutPosition (unsigned short int PosX1, unsigned short int PosY1, unsigned short int PosX2, unsigned short int PosY2);
                                    extern void ndRestoreStdOutStandardPosition ();
                                    extern void ndStdOutRender (void);
                                    
                                /*
                                 ------------------------------------------------------------
                                 INFO API
                                 ------------------------------------------------------------
                                */    
                                    
                                    extern ndint64 ndINFO_GetValue (int Code, int Param1, int Param2, int Param3); 
                                    
                                /*
                                 ------------------------------------------------------------
                                 ROUTINE DI INIZIALIZZAZIONE
                                 ------------------------------------------------------------
                                */ 
                                    
                                    extern void ndInitSystem (void); 
                                    
                                /*
                                 ------------------------------------------------------------
                                 ROUTINES DI SISTEMA
                                 ------------------------------------------------------------
                                */
                                    
                                    extern void ndSystemHalt (int ExitCode); 
                                    

#endif
