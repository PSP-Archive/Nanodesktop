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
    Codice per la gestione dei menu di finestra (parte grafica)
    ----------------------------------------------------------------------------
*/                   

#include <$_NDforND.h>

/* 
   -----------------------------------------------------------------------------
   ROUTINES A BASSO LIVELLO PER LA SCRITTURA NEI MENU BAR
   -----------------------------------------------------------------------------
*/

char ndMB_PutPixel (unsigned short int RPosPixelX, unsigned short int RPosPixelY, TypeColor Color, unsigned char WndHandle)
/*
     Questa funzione si occupa di inserire un pixel nella menù bar di una determinata finestra.
     
     Le coordinate devono essere fornite con riferimento al primo pixel in alto a sinistra della
     menù bar.
     
     Sono previsti i seguenti errori:
           ERR_WINDOW_IS_NOT_ALLOCATED     Finestra non allocata
           ERR_PIXEL_IS_OUT_OF_WINDOW      Posizione del pixel fuori finestra
           ERR_WINMENU_NOT_SUPPORTED       WinMenu non supportati

*/

{
  unsigned short int PosPixelX, PosPixelY;

  if (!ndInitSystemExecuted) ndInitSystem();

  if (WindowData [WndHandle].WindowIsActive)
  {
     if (WindowData [WndHandle].MenuSupported)
     {
        if ((RPosPixelX<WindowData [WndHandle].MenuBar_LenX) && (RPosPixelY<WindowData [WndHandle].MenuBar_LenY))
        {
             PosPixelX = WindowData [WndHandle].MenuBar_PosX1 + RPosPixelX;
             PosPixelY = WindowData [WndHandle].MenuBar_PosY1 + RPosPixelY;
             
             return ndLP_PutPixel (PosPixelX, PosPixelY, Color, WndHandle, NORENDER);
        }
        else return 0;            // Il tentativo di scrivere fuori barra viene ignorato
     }
     else return ERR_WINMENU_NOT_SUPPORTED;
  }
  else return ERR_WINDOW_IS_NOT_ALLOCATED;
                 
}





char ndMB_WriteChar (unsigned short int RPosPixelCharX, unsigned short int RPosPixelCharY, char Carattere, TypeColor Color, TypeColor BGColor, unsigned char WndHandle)
{
    unsigned char *FontData;		//puntatore al font

	unsigned char BitFilter, BitExtract;
	unsigned char ByteInChar;
	unsigned char FntHandle;
    int IntX, IntY;

	char ErroreRiportato;

	if (!ndInitSystemExecuted) ndInitSystem();

	if (Carattere!=0)
	{
		FntHandle = WindowData [WndHandle].ndMB_CurrentFntHandle;
        FontData=FontHandlesArray [FntHandle].Data+Carattere*8;	
        //Inizializza il puntatore alla corretta lettera nel buffer dei font

		ByteInChar=0;

		for (IntY=0; IntY<8; IntY++)
		{
			BitFilter=0x80;

			for (IntX=0; IntX<8; IntX++)
			{
				BitExtract=(*(FontData+ByteInChar)&BitFilter);

				if (BitExtract!=0)
                   ErroreRiportato=ndMB_PutPixel (RPosPixelCharX+IntX, RPosPixelCharY+IntY, Color, WndHandle);
                else
                   ErroreRiportato=ndMB_PutPixel (RPosPixelCharX+IntX, RPosPixelCharY+IntY, BGColor, WndHandle);

                if (ErroreRiportato!=0) return ErroreRiportato;

				BitFilter=BitFilter>>1;		//La maschera BitFilter è stata spostata di un bit a dx
			}

			ByteInChar++;

		}

    }

    return 0;
}

char ndMB_Write (unsigned short int RPosPixelX, unsigned short int RPosPixelY, char *str, TypeColor Color, TypeColor BGColor, unsigned char WndHandle)
{
	int MaxLenStr=255;
	unsigned short int t, kx, ky;
	char *strpointer;			//Puntatore alla stringa

    char ErroreRiportato;

    if (!ndInitSystemExecuted) ndInitSystem();

	strpointer=str;
	t=0;

    ky=RPosPixelY;

	while (*(strpointer+t)!=0 && t<MaxLenStr)
	{
        kx=RPosPixelX+t*8;

		ErroreRiportato=ndMB_WriteChar (kx, ky, *(strpointer+t), Color, BGColor, WndHandle);
		if (ErroreRiportato!=0) return (ErroreRiportato);

       	t++;
	}
    return 0;

}





char ndMB_DrawRectangle (unsigned short int PosX1, unsigned short int PosY1, unsigned short int PosX2, unsigned short int PosY2, TypeColor Color, TypeColor BorderColor, unsigned char WndHandle)
{
	int IntX, IntY;

	unsigned short int KDiffX, KDiffY;
    char ErroreRiportato;

    if (!ndInitSystemExecuted) ndInitSystem();
	
	if (PosX1>PosX2) UShort_Swap (&PosX1, &PosX2);		
	if (PosY1>PosY2) UShort_Swap (&PosY1, &PosY2);
		
	KDiffX=PosX2-PosX1;
    KDiffY=PosY2-PosY1;
	
	
	//Disegniamo prima l'interno del rettangolo

	for (IntY=0; IntY<=KDiffY; IntY++)
	{
			for (IntX=0; IntX<=KDiffX; IntX++)
			{
					ErroreRiportato=ndMB_PutPixel (IntX+PosX1, IntY+PosY1, Color, WndHandle);
				    if (ErroreRiportato!=0) return ErroreRiportato;
            }
	}

	//Ora disegniamo il border del rettangolo: lato superiore
	//ed inferiore


	for (IntX=0; IntX<=KDiffX; IntX++)
	{
			ErroreRiportato=ndMB_PutPixel (IntX+PosX1, PosY1, BorderColor, WndHandle);
			if (ErroreRiportato!=0) return ErroreRiportato;

            ErroreRiportato=ndMB_PutPixel (IntX+PosX1, PosY2, BorderColor, WndHandle);
		    if (ErroreRiportato!=0) return ErroreRiportato;
    }

    //Ora disegniamo il border del rettangolo: lato destro e
	//e sinistro


	for (IntY=0; IntY<=KDiffY; IntY++)
	{

			ErroreRiportato=ndMB_PutPixel (PosX1, IntY+PosY1, BorderColor, WndHandle);
			if (ErroreRiportato!=0) return ErroreRiportato;

            ErroreRiportato=ndMB_PutPixel (PosX2, IntY+PosY1, BorderColor, WndHandle);
            if (ErroreRiportato!=0) return ErroreRiportato;
    }

   // Se tutto è andato a buon fine, restituiamo codice di errore 0

   return 0;
}



char ndMB_DrawRectangleVoid (unsigned short int PosX1, unsigned short int PosY1, unsigned short int PosX2, unsigned short int PosY2, TypeColor BorderColor, unsigned char WndHandle)
/*
     Questa funzione scrive un rettangolo nella pagina logica della finestra identificata da
     WndHandle. Le coordinate devono essere fornite in modo relativo, con riferimento al primo
     pixel in alto a sinistra della finestra

     L'ultimo parametro dice alla routine se il carattere deve essere renderizzato immediatamente

     A differenza della normale DrawRectangle, il rettangolo che viene creato qui è vuoto.
*/
   {
	int IntX, IntY;

	unsigned short int KDiffX, KDiffY;
    char ErroreRiportato;

    if (!ndInitSystemExecuted) ndInitSystem();	
	
    if (PosX1>PosX2) UShort_Swap (&PosX1, &PosX2);	
	if (PosY1>PosY2) UShort_Swap (&PosY1, &PosY2);
		
	KDiffX=PosX2-PosX1;
    KDiffY=PosY2-PosY1;
	
	//Disegniamo il border del rettangolo: lato superiore
	//ed inferiore

	for (IntX=0; IntX<=KDiffX; IntX++)
	{
			ErroreRiportato=ndMB_PutPixel (IntX+PosX1, PosY1, BorderColor, WndHandle);
			if (ErroreRiportato!=0) return ErroreRiportato;

            ErroreRiportato=ndMB_PutPixel (IntX+PosX1, PosY2, BorderColor, WndHandle);
		    if (ErroreRiportato!=0) return ErroreRiportato;
    }

    //Ora disegniamo il border del rettangolo: lato destro e
	//e sinistro


	for (IntY=0; IntY<=KDiffY; IntY++)
	{

			ErroreRiportato=ndMB_PutPixel (PosX1, IntY+PosY1, BorderColor, WndHandle);
			if (ErroreRiportato!=0) return ErroreRiportato;

            ErroreRiportato=ndMB_PutPixel (PosX2, IntY+PosY1, BorderColor, WndHandle);
            if (ErroreRiportato!=0) return ErroreRiportato;
    }

    // Se tutto è andato a buon fine, restituiamo codice di errore 0

    return 0;
}


char ndMB_SetFont (unsigned char FntHandle, char WndHandle)
/*
   Questa routine provvede a settare il font per le routines che si occupano
   della visualizzazione dei menù di tipo 1 e di tipo 2.
   
   Le routines di tipo MB supportano esclusivamente i font di tipo TEXT e non
   i font di tipo GRAPHICS. 
   
   Sono previsti i seguenti codici di errore:
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
        ERR_SS_NOT_SUPPORTED          Il service space non è supportato
        ERR_FONTHDL_IS_NOT_ACTIVE     Il font handle non è attivo
        ERR_GRAPHFONT_NOT_SUPPORTED   I graphfont non sono supportati
*/
{
   if ( !ndInitSystemExecuted ) ndInitSystem();
           
   if ((WindowData [WndHandle].WindowIsActive))
   {  
       if (WindowData [WndHandle].SS_Supported)
       {           
           if (FontHandlesArray [FntHandle].IsActive)
           {                   
              if ( (FontHandlesArray [FntHandle].features & NDTEXTFONT) == NDTEXTFONT )
              {
                 WindowData [WndHandle].ndMB_CurrentFntHandle = FntHandle;
                 // Registra il nuovo text-font corrente
                 
                 XWindowRender (WndHandle);
                 
                 return 0;
                 // e ritorna il codice di controllo 0
              }
              else 
                 return ERR_GRAPHFONT_NOT_SUPPORTED;
                 
           }
           else
               return ERR_FONTHDL_IS_NOT_ACTIVE; 
       }
       else
         return ERR_SS_NOT_SUPPORTED;  
   }
   else
     return ERR_WINDOW_IS_NOT_ALLOCATED;
}





/*
   -----------------------------------------------------------------------------
   RENDERING DELLA MENUBAR
   -----------------------------------------------------------------------------
*/


void INTERNAL_CreateClearMenuBar (unsigned char WndHandle)
/*
    Crea una barra dei menù vuota nella finestra di handle WndHandle.
*/
{
    ndLP_DrawRectangle (WindowData [WndHandle].MenuBar_PosX1, WindowData [WndHandle].MenuBar_PosY1,
                        WindowData [WndHandle].MenuBar_PosX2, WindowData [WndHandle].MenuBar_PosY2,
                        COLOR_GRAY14, COLOR_WHITE, WndHandle, NORENDER);
}

void INTERNAL_ClearLogicalButtonBuffer (unsigned char WndHandle)
{
   char Counter;
   
   for (Counter=0; Counter<4; Counter++)
       WindowData [WndHandle].ButtonWMI1Serv [Counter].ButtonIsActivated=0;
       
   WindowData [WndHandle].NrWMI1ButtonsAllocated = 0;
   
   for (Counter=0; Counter<NRMAX_WMI1_BUTTON_ALLOCABLE; Counter++)
       WindowData [WndHandle].ButtonWMI1 [Counter].ButtonIsActivated=0;
       
   for (Counter=0; Counter<2; Counter++)
       WindowData [WndHandle].ButtonWMI2Serv [Counter].ButtonIsActivated=0;
       
   WindowData [WndHandle].NrWMI2ButtonsAllocated = 0;
   
   for (Counter=0; Counter<NRMAX_WMI2_BUTTON_ALLOCABLE; Counter++)
       WindowData [WndHandle].ButtonWMI2 [Counter].ButtonIsActivated=0;
}


void ndMB_CreateButton_BackMenu (unsigned char WndHandle)
{
   unsigned short int rX1, rY1, rX2, rY2;
   unsigned short int CounterX, CounterY;
   
   if (strcmp (WindowData [WndHandle].ZString, "") != 0)
   {
       rX1 = 2;
       rY1 = 2;
       rX2 = 2+15;
       rY2 = WindowData [WndHandle].MenuBar_LenY-2;
       
       ndMB_DrawRectangle (rX1+1, rY1+1, rX2-1, rY2-1, COLOR_GRAY12, COLOR_BLACK, WndHandle);
  
       for (CounterY=0; CounterY<8; CounterY++)
       {
           for (CounterX=0; CounterX<8; CounterX++)
           {
               if (Button_PrevMenu [CounterY][CounterX] == 'x')
                  ndMB_PutPixel (rX1+CounterX+4, rY1+CounterY+2, COLOR_BLACK, WndHandle);
               
           }
       }
       
       WindowData [WndHandle].ButtonWMI1Serv[WMI_GO_BACK].ButtonIsActivated=1;
       WindowData [WndHandle].ButtonWMI1Serv[WMI_GO_BACK].MBPosX1 = rX1 + 1;
       WindowData [WndHandle].ButtonWMI1Serv[WMI_GO_BACK].MBPosX2 = rX2 - 1;
       WindowData [WndHandle].ButtonWMI1Serv[WMI_GO_BACK].MBPosY1 = rY1 + 1;
       WindowData [WndHandle].ButtonWMI1Serv[WMI_GO_BACK].MBPosY2 = rY2 - 1;
       
   }
   else           // Registra che il pulsante è disattivato
   {   
      WindowData [WndHandle].ButtonWMI1Serv[WMI_GO_BACK].ButtonIsActivated=0;
   }
}

void ndMB_CreateButton_ScrollMenuToLeft (unsigned char WndHandle)
{
   unsigned short int rX1, rY1, rX2, rY2;
   unsigned short int CounterX, CounterY;
   
   if (WindowData [WndHandle].NrFirstL1WMIButton>0)
   {
       rX1 = 15+4;
       rY1 = 2;
       rX2 = 15+4+15;
       rY2 = WindowData [WndHandle].MenuBar_LenY-2;
           
       ndMB_DrawRectangle (rX1+1, rY1+1, rX2-1, rY2-1, COLOR_GRAY12, COLOR_BLACK, WndHandle);
       
       for (CounterY=0; CounterY<8; CounterY++)
       {
           for (CounterX=0; CounterX<8; CounterX++)
           {
               if (Button_ScrollLeft [CounterY][CounterX] == 'x')
                  ndMB_PutPixel (rX1+CounterX+4, rY1+CounterY+3, COLOR_BLACK, WndHandle);
               else
                  ndMB_PutPixel (rX2+CounterX+4, rY1+CounterY+3, COLOR_GRAY12, WndHandle);

           }
       }
       
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_LEFT].ButtonIsActivated=1;
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_LEFT].MBPosX1 = rX1 + 1;
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_LEFT].MBPosX2 = rX2 - 1;
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_LEFT].MBPosY1 = rY1 + 1;
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_LEFT].MBPosY2 = rY2 - 1;
   }
   else
   {
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_LEFT].ButtonIsActivated=0;
   }
}

void ndMB_CreateButton_ScrollMenuToRight (unsigned short int LastElementShown, unsigned short int ElementsInTotal, unsigned char WndHandle)
{
   unsigned short int rX1, rY1, rX2, rY2;
   unsigned short int CounterX, CounterY;
   
   if ( LastElementShown<(ElementsInTotal-1) )
   {
       rX1 = WindowData [WndHandle].MenuBar_LenX - 2 - 15;
       rY1 = 2;
       rX2 = WindowData [WndHandle].MenuBar_LenX - 2;
       rY2 = WindowData [WndHandle].MenuBar_LenY - 2;
       
       ndMB_DrawRectangle (rX1+1, rY1+1, rX2-1, rY2-1, COLOR_GRAY12, COLOR_BLACK, WndHandle);
   
       for (CounterY=0; CounterY<8; CounterY++)
       {
           for (CounterX=0; CounterX<8; CounterX++)
           {
               if (Button_ScrollRight [CounterY][CounterX] == 'x')
                  ndMB_PutPixel (rX1+CounterX+4, rY1+CounterY+3, COLOR_BLACK, WndHandle);
               else
                  ndMB_PutPixel (rX2+CounterX+4, rY1+CounterY+3, COLOR_GRAY12, WndHandle);

           }
       }
       
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_RIGHT].ButtonIsActivated=1;
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_RIGHT].MBPosX1 = rX1 + 1;
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_RIGHT].MBPosX2 = rX2 - 1;
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_RIGHT].MBPosY1 = rY1 + 1;
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_RIGHT].MBPosY2 = rY2 - 1;
   }
   else
   {
       WindowData [WndHandle].ButtonWMI1Serv[SCROLL_WMI1_TO_RIGHT].ButtonIsActivated=0;
   }
}


void ndMB_CreateButton_for_L1WMI (unsigned short int RPosX1, unsigned short int RPosX2, char *NameItem, 
                                  unsigned char Inhibited, unsigned char Activated, 
                                  unsigned char WndHandle)
{
   unsigned short int rX1, rY1, rX2, rY2;
   
   unsigned short int LenButton;
   unsigned short int NrCharsInButton;
   unsigned short int Margin;
   
   TypeColor Color, BackColor, BorderColor;
   
   char StringBuffer [512];
   char xA;
   
   if (WindowData [WndHandle].NrWMI1ButtonsAllocated<NRMAX_WMI1_BUTTON_ALLOCABLE)
   {
           xA = WindowData [WndHandle].NrWMI1ButtonsAllocated;
   
           LenButton       = RPosX2 - RPosX1 - 8;
           NrCharsInButton = LenButton / 8;
           Margin          = LenButton - (NrCharsInButton * 8) ;
           
           ndStrCpy (StringBuffer, NameItem, NrCharsInButton, 0);
            
           rX1 = RPosX1;
           rY1 = 2;
           rX2 = RPosX2;
           rY2 = WindowData [WndHandle].MenuBar_LenY - 2;
           
           if (!Inhibited)
           {
              if (!Activated)
              {
                 BackColor   = COLOR_GRAY12;
                 Color       = COLOR_BLACK;
                 BorderColor = COLOR_GRAY12;           
              }
              else
              {
                 BackColor   = COLOR_BLUE;
                 Color       = COLOR_WHITE;
                 BorderColor = COLOR_GRAY12;
              } 
           
               WindowData [WndHandle].ButtonWMI1[xA].ButtonIsActivated=1;
               WindowData [WndHandle].ButtonWMI1[xA].MBPosX1 = rX1;
               WindowData [WndHandle].ButtonWMI1[xA].MBPosX2 = rX2;
               WindowData [WndHandle].ButtonWMI1[xA].MBPosY1 = rY1;
               WindowData [WndHandle].ButtonWMI1[xA].MBPosY2 = rY2;
               WindowData [WndHandle].NrWMI1ButtonsAllocated++;
           }
           else      // Il pulsante è inibito
           {
               WindowData [WndHandle].ButtonWMI1[xA].ButtonIsActivated=0;
               WindowData [WndHandle].NrWMI1ButtonsAllocated++;
               
               BackColor   = COLOR_GRAY12;
               Color       = COLOR_GRAY09;
               BorderColor = COLOR_GRAY09;
           }
           
           ndMB_DrawRectangle (rX1, rY1, rX2, rY2, BackColor, BorderColor, WndHandle);
           ndMB_Write (rX1+5+Margin/2, rY1+2, StringBuffer, Color, BackColor, WndHandle);
                 
   }

}


char INTERNAL_StrCatZString (char *TempBuffer, char *Str1, char *Str2)
{
   strcpy (TempBuffer, "");  
     
   if (strcmp (Str1, "")==0)      // Se Str1 è vuota
   {
     strcpy (TempBuffer, Str2);
   }
   else
   {
     strcpy (TempBuffer, Str1);
     strcat (TempBuffer, "/");
     strcat (TempBuffer, Str2);
   }  
}


void ndMB_CreateButton_for_L2WMI (unsigned short int SSPosX1, unsigned short int SSPosY1, 
                                  unsigned short int SSPosX2, unsigned short int SSPosY2, 
                                  char *NameItem, unsigned char Inhibited, unsigned char WndHandle)
{
   unsigned short int rX1, rY1;
   
   unsigned short int LenButton;
   unsigned short int NrCharsInButton;
   unsigned short int Margin;
   
   TypeColor Color, BackColor;
   unsigned short int xA;
   
   char StringBuffer [512];
                   
   if (WindowData [WndHandle].NrWMI2ButtonsAllocated<NRMAX_WMI2_BUTTON_ALLOCABLE)
   {
           xA = WindowData [WndHandle].NrWMI2ButtonsAllocated;
        
           LenButton       = SSPosX2 - SSPosX1 - 4;
           NrCharsInButton = LenButton / 8;
           Margin          = LenButton - (NrCharsInButton * 8) ;
        
           rX1 = SSPosX1 + 2 + Margin/2;
           rY1 = SSPosY1 + 4;
        
           ndStrCpy (StringBuffer, NameItem, NrCharsInButton, 0);
            
           if (!Inhibited)
           {
              Color       = COLOR_SSBLACK;
           
              WindowData [WndHandle].ButtonWMI2[xA].ButtonIsActivated=1;
              WindowData [WndHandle].ButtonWMI2[xA].SSPosX1 = SSPosX1;
              WindowData [WndHandle].ButtonWMI2[xA].SSPosX2 = SSPosX2;
              WindowData [WndHandle].ButtonWMI2[xA].SSPosY1 = SSPosY1;
              WindowData [WndHandle].ButtonWMI2[xA].SSPosY2 = SSPosY2;
              WindowData [WndHandle].NrWMI2ButtonsAllocated++;
           }
           else  
           {    
              Color       = COLOR_GRAY08;
              
              WindowData [WndHandle].ButtonWMI2[xA].ButtonIsActivated=0;
              WindowData [WndHandle].NrWMI2ButtonsAllocated++;
           }
           
           BackColor = COLOR_GRAY10;    
   
           ndSS_DrawRectangle    (SSPosX1, SSPosY1, SSPosX2, SSPosY2, COLOR_GRAY10, COLOR_GRAY11, WndHandle, NORENDER);
           ndSS_WriteStringInSS  (rX1,     rY1,     StringBuffer, Color, BackColor, WndHandle, NORENDER);
   }
}


void ndMB_CreateButton_for_ScrollUp_L2WMI (unsigned short int SSPosX1, unsigned short int SSPosY1, 
                                              unsigned short int SSPosX2, unsigned short int SSPosY2, 
                                              unsigned char WndHandle)
{     
   unsigned short int CounterX, CounterY;
   unsigned short int rX1, rY1;
   
   rX1 = ((SSPosX2-SSPosX1)/2)-4;
   rY1 = 2;
   
   ndSS_DrawRectangle    (SSPosX1+1, SSPosY1+1, SSPosX2-1, SSPosY2-1, COLOR_GRAY07, COLOR_SSBLACK, WndHandle, NORENDER);
   
   for (CounterY=0; CounterY<8; CounterY++)
   {
       for (CounterX=0; CounterX<8; CounterX++)
       {
           if (Button_ScrollUp [CounterY][CounterX] == 'x')
              ndSS_PutPixel (SSPosX1+1+rX1+CounterX, SSPosY1+1+rY1+CounterY, COLOR_SSBLACK, WndHandle, NORENDER);
           else
              ndSS_PutPixel (SSPosX1+1+rX1+CounterX, SSPosY1+1+rY1+CounterY, COLOR_GRAY07, WndHandle, NORENDER);

       }
   }

   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_UP].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_UP].SSPosX1 = SSPosX1+1;
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_UP].SSPosX2 = SSPosX2-1;
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_UP].SSPosY1 = SSPosY1+1;
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_UP].SSPosY2 = SSPosY2-1;
         
}


void ndMB_CreateButton_for_ScrollDown_L2WMI  (unsigned short int SSPosX1, unsigned short int SSPosY1, 
                                              unsigned short int SSPosX2, unsigned short int SSPosY2, 
                                              unsigned char WndHandle)
{  
   unsigned short int CounterX, CounterY;
   unsigned short int rX1, rY1;
   
   rX1 = ((SSPosX2-SSPosX1)/2)-4;
   rY1 = 2;
   
   ndSS_DrawRectangle    (SSPosX1+1, SSPosY1+1, SSPosX2-1, SSPosY2-1, COLOR_GRAY07, COLOR_SSBLACK, WndHandle, NORENDER);
   
   for (CounterY=0; CounterY<8; CounterY++)
   {
       for (CounterX=0; CounterX<8; CounterX++)
       {
           if (Button_ScrollDown [CounterY][CounterX] == 'x')
              ndSS_PutPixel (SSPosX1+1+rX1+CounterX, SSPosY1+1+rY1+CounterY, COLOR_SSBLACK, WndHandle, NORENDER);
           else
              ndSS_PutPixel (SSPosX1+1+rX1+CounterX, SSPosY1+1+rY1+CounterY, COLOR_GRAY07, WndHandle, NORENDER);

       }
   }
   
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_DOWN].ButtonIsActivated=1;
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_DOWN].SSPosX1 = SSPosX1+1;
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_DOWN].SSPosX2 = SSPosX2-1;
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_DOWN].SSPosY1 = SSPosY1+1;
   WindowData [WndHandle].ButtonWMI2Serv [SCROLL_WMI2_TO_DOWN].SSPosY2 = SSPosY2-1;
         
}



/* 
   --------------------------------------------------------------------------------------------
   SISTEMA DI RENDER DEI MENU DI FINESTRA (Routine di livello superiore)
   --------------------------------------------------------------------------------------------
*/


void ndL2WinMenuRender (char *ZString,  unsigned short int Theoretical_PosX1, 
                           unsigned short int Theoretical_PosX2, unsigned char WndHandle)
/*
    Questa routine si occupa di disegnare il menù di tipo 2
*/

{
    struct ELEMENT_L2WMI_Type *PointerToL2WMI;
    
    unsigned short int  L2MenuX1,  L2MenuX2;
    unsigned short int  L2MenuY1,  L2MenuY2;
    
    unsigned short int  SBMenuX1, SBMenuY1; 
    unsigned short int  SBMenuX2, SBMenuY2;
    
    unsigned short int RequestLenX, RequestLenY;
    
    short int TryPosX1, TryPosX2;
    short int OverflowTry1, OverflowTry2;
    
    unsigned short int NrL2Elements;
    unsigned short int MaxLen, StrLen;
    unsigned short int Go;
    
    unsigned short int Counter;
    unsigned short int GetElement;
    
    unsigned short int NrL2ElementsVisualizable, MarginY;
    char NewL2Menu;
    
    ndSS_ClrBufferSpace (WndHandle, NORENDER);         // Per prima cosa puliamo il service space
        
    if ((WindowData [WndHandle].PrevOpenL2WMIMenu==0) && (WindowData [WndHandle].OpenL2WMIMenu==1))
    {
       ndHAL_RequestDelay (ND_REQDELAY_WINMENUGRAPH);
       // Sotto PSP è necessario un lieve ritardo all'apertura della nuova
       // finestra per lasciare il tempo all'utente di rilasciare il tasto.
                    
       NewL2Menu=1;
    }
    else
       NewL2Menu=0;
       
    WindowData [WndHandle].PrevOpenL2WMIMenu = WindowData [WndHandle].OpenL2WMIMenu;
    // Salva il valore precedente del registro 
    
    if (WindowData [WndHandle].OpenL2WMIMenu!=0)
    { 
            // Per prima cosa bisogna determinare la RequestLenY.
           
            NrL2Elements = ndWMENU_NumberOfL2WMIs (WndHandle, ZString);
                 
            RequestLenY = 10 + ( NrL2Elements*15 );
            
            L2MenuY1 = 0;
            
            if (RequestLenY < WindowData [WndHandle].SS_LenY)
               L2MenuY2=RequestLenY;
            else
               L2MenuY2=WindowData [WndHandle].SS_LenY-1;
            
            // Adesso è necessario determinare le coordinate X per il menù. 
            
            MaxLen=0;
            
            for (Counter=0; Counter<NrL2Elements; Counter++)
            {
               ndWMENU_EnumL2WMI ( WndHandle, ZString, Counter, &(PointerToL2WMI) ); 
            
               StrLen = strlen (PointerToL2WMI->NameItem);
               if (StrLen>=MaxLen) MaxLen=StrLen;
            }
            
            RequestLenX = MaxLen * 8 + 20 + 12;
            
            TryPosX1 = Theoretical_PosX1;
            TryPosX2 = Theoretical_PosX1 + RequestLenX;
            OverflowTry1 = WindowData [WndHandle].SS_LenX - TryPosX2;
            
            TryPosX1 = Theoretical_PosX2 - RequestLenX;
            TryPosX2 = Theoretical_PosX2;
            OverflowTry2 = TryPosX1;
            
            if (OverflowTry1>=0)     // OverflowTry1 positivo implica che il menù entri nella SS
            {
               L2MenuX1 = Theoretical_PosX1;
               L2MenuX2 = Theoretical_PosX1 + RequestLenX;
               Go=1;
            }
            else        // OverflowTry1 è negativo, vediamo se OverflowTry2 è positivo
            {
                if (OverflowTry2>=0)
                {  
                   L2MenuX1 = Theoretical_PosX2 - RequestLenX;
                   L2MenuX2 = Theoretical_PosX2;
                   Go=1;
                }
                else        // OverflowTry1 e OverflowTry2 sono entrambi negativi: è necessario un ridimensionamento
                {
                   // Deve scegliere il tipo di ridimensionamento che minimizza il taglio eseguito al menù
                            
                   if ( abs(OverflowTry1) <= abs(OverflowTry2) )
                   {
                        L2MenuX1 = Theoretical_PosX1;
                        L2MenuX2 = WindowData [WndHandle].SS_LenX;
                   }
                   else
                   {
                        L2MenuX1 = 0;
                        L2MenuX2 = Theoretical_PosX2;
                   }
                   
                   if ((L2MenuX2-L2MenuX1)>64) Go=1;
                    else Go=0;
                }
            }
               
            if (Go)      // Abbiamo la possibilità di visualizzare un menù
            {
               ndSS_DrawRectangle (L2MenuX1, L2MenuY1, L2MenuX2, L2MenuY2, COLOR_GRAY12, COLOR_SSBLACK, WndHandle, NORENDER); 
               ndSS_DrawRectangle (L2MenuX1+2, L2MenuY1+2, L2MenuX1+2+8, L2MenuY2-2, COLOR_BLUE, COLOR_BLUE, WndHandle, NORENDER);
            
               NrL2ElementsVisualizable = (L2MenuY2 - L2MenuY1 - 10)/15;
               MarginY = ((L2MenuY2 - L2MenuY1 - 10) - (NrL2ElementsVisualizable*15))/2;
               
               if (NrL2Elements <= NrL2ElementsVisualizable)     // Non è necessario attivare il sistema scrolling
               {
                  WindowData [WndHandle].L2WMIMenuScrollActive=0;
                  // Segnala che l'ultima volta che è stata chiamata la routine scroll active era disattivato 
                                
                  for (Counter=0; Counter<NrL2Elements; Counter++)
                  {   
                      ndWMENU_EnumL2WMI (WndHandle, ZString, Counter, &(PointerToL2WMI));
                  
                      ndMB_CreateButton_for_L2WMI (L2MenuX1+15, L2MenuY1+MarginY+5+Counter*15, 
                                                   L2MenuX2-5,  L2MenuY1+MarginY+5+(Counter+1)*15, 
                                                   PointerToL2WMI->NameItem, 
                                                   PointerToL2WMI->Item_Inhibited, 
                                                   WndHandle);  
                  }               
                   
               }
               else    // Bisogna utilizzare il sistema scrolling
               {
                  ndSS_DrawRectangle (L2MenuX1, L2MenuY1, L2MenuX2, L2MenuY2, COLOR_GRAY12, COLOR_SSBLACK, WndHandle, NORENDER); 
                  ndSS_DrawRectangle (L2MenuX1+2, L2MenuY1+2, L2MenuX1+2+8, L2MenuY2-2, COLOR_BLUE, COLOR_BLUE, WndHandle, NORENDER);
                     
                  WindowData [WndHandle].L2WMIMenuScrollActive =1;
                  
                  if (NewL2Menu) WindowData [WndHandle].L2WMIScrollElement=0;
                  // Se il menù L2 è stato aperto ora, è necessario resettare L2WMIScrollElement
                  
                  SBMenuX1 = L2MenuX1 + 15;
                  SBMenuY1 = L2MenuY1 + MarginY + 5;
                  SBMenuX2 = L2MenuX2 - 5;
                  SBMenuY2 = L2MenuY1 + MarginY + 5 + 15;
                  
                  if (WindowData [WndHandle].L2WMIScrollElement>0)
                     ndMB_CreateButton_for_ScrollUp_L2WMI (SBMenuX1, SBMenuY1, SBMenuX2, SBMenuY2, WndHandle);
                  
                  for (Counter=0; Counter<NrL2ElementsVisualizable-2; Counter++)
                  {   
                      GetElement = WindowData [WndHandle].L2WMIScrollElement + Counter;
                      
                      ndWMENU_EnumL2WMI (WndHandle, ZString, GetElement, &(PointerToL2WMI));
                  
                      ndMB_CreateButton_for_L2WMI (L2MenuX1+15, L2MenuY1+5+MarginY+(1+Counter)*15, 
                                                   L2MenuX2-5,  L2MenuY1+5+MarginY+(2+Counter)*15, 
                                                   PointerToL2WMI->NameItem, 
                                                   PointerToL2WMI->Item_Inhibited, 
                                                   WndHandle);  
                  }           
                   
                  SBMenuX1 = L2MenuX1 + 15;
                  SBMenuY1 = L2MenuY1 + 5 + MarginY + (NrL2ElementsVisualizable-1)*15;
                  SBMenuX2 = L2MenuX2 - 5;
                  SBMenuY2 = L2MenuY1 + 5 + MarginY + (NrL2ElementsVisualizable)*15;
                  
                  if ( (WindowData [WndHandle].L2WMIScrollElement + NrL2ElementsVisualizable - 2) < NrL2Elements)
                     ndMB_CreateButton_for_ScrollDown_L2WMI (SBMenuX1, SBMenuY1, SBMenuX2, SBMenuY2, WndHandle);
                           
               }  
            }
    }        
    
}  


char ndWinMenuRender (unsigned char WndHandle)
/* 
   Questa routine si occupa di disegnare la barra dei menù con i pulsanti per i vari
   L1WMI e si occupa di disegnare i menù per gli elementi L2WMI
*/
{
   struct ELEMENT_L1WMI_Type  *PointerToL1WMI;
   
   unsigned short int NrL1WMI;
   unsigned short int Counter;
   unsigned short int LenName;
   
   unsigned short int Theoretical_PosX1Button;
   unsigned short int Theoretical_PosX2Button;
   
   unsigned short int Theoretical_PixelLenButton;
   
   unsigned char YouCanExit;
   unsigned char NoDrawLastButton;
   
   char ErrRep, EvidenceItem;
   
   unsigned short int Theoretical_PosL2MenuX1; 
   unsigned short int Theoretical_PosL2MenuX2;
   
   unsigned short int LastElementShown; 
   
   char TempBuffer [512];
        
         
   if (WindowData [WndHandle].WindowIsActive)
   {
      if (WindowData [WndHandle].MenuSupported)
      {   
         // Provvedi a cancellare la barra di menù, in modo da avere uno spazio pulito
         INTERNAL_CreateClearMenuBar (WndHandle);
         INTERNAL_ClearLogicalButtonBuffer (WndHandle);
         
         Theoretical_PosX1Button = WindowData [WndHandle].MenuBar_LA_PosX1;
         
         NrL1WMI = ndWMENU_NumberOfL1WMIs ( WndHandle, &(WindowData [WndHandle].ZString) );
       
         if (NrL1WMI>0)             // Ci sono degli L1WMI da visualizzare 
         {
            YouCanExit=0;  
            
            ndMB_CreateButton_BackMenu (WndHandle);  
            ndMB_CreateButton_ScrollMenuToLeft (WndHandle);   
                                    
            for (Counter=WindowData [WndHandle].NrFirstL1WMIButton; (Counter<NrL1WMI) && (!YouCanExit); Counter++)
            {
                ErrRep = ndWMENU_EnumL1WMI ( WndHandle, &(WindowData [WndHandle].ZString), Counter, &(PointerToL1WMI) );
                
                if (!ErrRep)
                {
                   LenName = strlen (PointerToL1WMI->NameItem);
                   
                   Theoretical_PixelLenButton = 8 + LenName*8;
                   Theoretical_PosX2Button = Theoretical_PosX1Button + Theoretical_PixelLenButton;
                   
                   if (Theoretical_PosX2Button >= WindowData [WndHandle].MenuBar_LA_PosX2 - 4)
                   {
                      NoDrawLastButton=1;
                      YouCanExit=1;
                   }
                   else (NoDrawLastButton=0);
                   
                   if (!NoDrawLastButton)
                   {
                         if ( (WindowData [WndHandle].OpenL2WMIMenu) && (WindowData [WndHandle].NrL1WMIButtonOpened==Counter) )
                         {
                             EvidenceItem=1; 
                             Theoretical_PosL2MenuX1 = Theoretical_PosX1Button;
                             Theoretical_PosL2MenuX2 = Theoretical_PosX2Button;
                   
                             INTERNAL_StrCatZString (TempBuffer, &(WindowData [WndHandle].ZString), PointerToL1WMI->StringID);
                             
                             // Questa stringa sarà passata al renderer dei menù di livello 2          
                         }
                         else
                         {
                             EvidenceItem=0;
                         }   
                         
                                         
                         ndMB_CreateButton_for_L1WMI (Theoretical_PosX1Button, Theoretical_PosX2Button, PointerToL1WMI->NameItem, 
                                                      PointerToL1WMI->Item_Inhibited, EvidenceItem, WndHandle);
                       
                         Theoretical_PosX1Button = Theoretical_PosX1Button + Theoretical_PixelLenButton + 4; 
                       
                         LastElementShown = Counter;
                         // Memorizza l'ultimo elemento che è stato disegnato
                   }
                }
                else      // C'è stato un errore: interrompi le operazioni
                {
                   YouCanExit=1;
                }
            }
            
            // A questo punto, in Counter c'è il codice dell'ultimo elemento visualizzato
            
            ndMB_CreateButton_ScrollMenuToRight (LastElementShown, NrL1WMI, WndHandle); 
       
            // Se è necessario, provvediamo a disegnare il menù di tipo 2
            
            ndL2WinMenuRender (TempBuffer, Theoretical_PosL2MenuX1, Theoretical_PosL2MenuX2, WndHandle);
                               
         } // Non ci sono items da visualizzare
         
         WindowData [WndHandle].MenuBarIsChanged=0;
         // Questo farà in modo che XWindowRender non esegua il rendering del menù finchè
         // non vengono effettuati nuovi cambiamenti.
      }
      else return ERR_WINMENU_NOT_SUPPORTED;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;
}



