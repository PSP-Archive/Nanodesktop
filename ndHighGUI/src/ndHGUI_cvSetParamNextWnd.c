/* 
    ----------------------------------------------------------------------------
    NanoDesktop HighGUI (ndHighGUI) Library - Version 0.2
    Copyright 2007-2012 Filippo Battaglia 
    ----------------------------------------------------------------------------
    This is the porting of HighGUI (the component of Intel (R) OpenCV Libraries
    that allows use of windows environment), for Nanodesktop windows manager.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
        
    By downloading, copying, installing or using the software you agree to this license.
    If you do not agree to this license, do not download, install,
    copy or use the software.
    
    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    
      * Redistribution's of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
    
      * Redistribution's in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
    
      * The name of the Visilab research center may not be used to endorse or promote products
        derived from this software without specific prior written permission.
    
    This software is provided by the copyright holders and contributors "as is" and
    any express or implied warranties, including, but not limited to, the implied
    warranties of merchantability and fitness for a particular purpose are disclaimed.
    In no event shall the author of the software, the Visilab Research Center or 
    contributors be liable for any direct, indirect, incidental, special, exemplary, 
    or consequential damages (including, but not limited to, procurement of substitute 
    goods or services; loss of use, data, or profits; or business interruption) 
    however caused and on any theory of liability, whether in contract, strict liability,
    or tort (including negligence or otherwise) arising in any way out of
    the use of this software, even if advised of the possibility of such damage.
    ---------------------------------------------------------------------------
    ROUTINES DEL GENERATORE COLORI / GENERATORE POSIZIONE FINESTRA
    ---------------------------------------------------------------------------   
*/


#include <$_NDHGUI_for_NDHGUI.h>





void cvChangeDefaultWindowDim (unsigned short int NewLenX, unsigned short int NewLenY)
/* ----------------------------------------------------------------------------
   Routine cvChangeDefaultWindowDim                             Pubblica
   ----------------------------------------------------------------------------
   Questa routine cambia la lunghezza e l'altezza di default delle finestre
   che vengono create con cvNamedWindow.
   
   La routine ha effetto solo a condizione che NewLenX>64 e NewLenY>64.
   
   Sono previsti i seguenti codici di errore: 
        NDHGUI_WRONG_WINDOW_DEFAULT_LENXY      Le dimensioni di NewLenX o NewLenY non
                                               sono maggiori di 64 
   ----------------------------------------------------------------------------
*/
{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  if ((NewLenX>64) && (NewLenY>64))
  {
     HGUI_DefaultLenX=NewLenX;
     HGUI_DefaultLenY=NewLenY;
     
     HGUI_SystemError=0;      
     return;
  }
  else
  {
     HGUI_SystemError=NDHGUI_WRONG_WINDOW_DEFAULT_LENXY;
     return;
  }
}








void INTERNAL_ProvideColor  (TypeColor *ColorTitle, TypeColor *ColorBGTitle, 
                             TypeColor *ColorBGWS, TypeColor *ColorBorder)                            
/* 
   ----------------------------------------------------------------------------
   Routine INTERNAL_ProvideColor                                   Interna
   ----------------------------------------------------------------------------
   Fornisce il colore della prossima finestra da visualizzare. 
   
   Non sono previsti codici di errore per questa routine.
   ----------------------------------------------------------------------------
*/
{
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
   if (!NoDefaultColor)
   {
      *ColorTitle      =HGUI_DefWnd_ColorTitle;
      *ColorBGTitle    =HGUI_DefWnd_ColorBGTitle;
      *ColorBorder     =HGUI_DefWnd_ColorBorder;
      *ColorBGWS       =HGUI_DefWnd_ColorBGWS;
      
      return;
   }
   else
   {
      *ColorTitle      =HGUI_NextWnd_ColorTitle;
      *ColorBGTitle    =HGUI_NextWnd_ColorBGTitle;
      *ColorBorder     =HGUI_NextWnd_ColorBorder;
      *ColorBGWS       =HGUI_NextWnd_ColorBGWS;
      
      NoDefaultColor=0;                 // Dalla prossima volta riusa i colori di default
      
      return;
   }
}




      

void cvSetDefaultColorWnd (TypeColor ColorTitle, TypeColor ColorBGTitle, 
                           TypeColor ColorBGWS,  TypeColor ColorBorder)
/* 
   ------------------------------------------------------------------------------------------
   Routine cvSetDefaultColorWnd                                             Pubblica
   ------------------------------------------------------------------------------------------
   Questa routine consente di settare i colori che vengono normalmente utilizzati per la
   visualizzazione delle finestre.
   
   Non sono previsti codici di errore per questa routine
   ------------------------------------------------------------------------------------------
*/

{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  HGUI_DefWnd_ColorTitle    =ColorTitle;
  HGUI_DefWnd_ColorBGTitle  =ColorBGTitle;
  HGUI_DefWnd_ColorBGWS     =ColorBGWS;
  HGUI_DefWnd_ColorBorder   =ColorBorder;
  
  return;
}







void cvSetDefaultColorTitle (TypeColor ColorTitle)
/* 
   ------------------------------------------------------------------------------------------
   Routine cvSetDefaultColorTitle                                             Pubblica
   ------------------------------------------------------------------------------------------
   Questa routine consente di settare i colori che vengono normalmente utilizzati per la
   visualizzazione delle finestre. La routine cambia il colore di default del titolo.
   
   Non sono previsti codici di errore per questa routine.
   ------------------------------------------------------------------------------------------
*/

{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  HGUI_DefWnd_ColorTitle=ColorTitle;  
  return;
}







void cvSetDefaultColorBGTitle (TypeColor ColorBGTitle)
/* 
   ------------------------------------------------------------------------------------------
   Routine cvSetDefaultColorBGTitle                                             Pubblica
   ------------------------------------------------------------------------------------------
   Questa routine consente di settare i colori che vengono normalmente utilizzati per la
   visualizzazione delle finestre. La routine agisce sul colore di background del titolo.
   
   Non sono previsti codici di errore per questa routine.
   ------------------------------------------------------------------------------------------
*/

{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  HGUI_DefWnd_ColorBGTitle=ColorBGTitle;
  return;
}






void cvSetDefaultColorBGWS (TypeColor ColorBGWS)
/* 
   ------------------------------------------------------------------------------------------
   Routine cvSetDefaultColorBGWS                                             Pubblica
   ------------------------------------------------------------------------------------------
   Questa routine consente di settare i colori che vengono normalmente utilizzati per la
   visualizzazione delle finestre. La routine agisce sul colore del background del window space.
   
   Non sono previsti codici di errore per questa routine.
   ------------------------------------------------------------------------------------------
*/

{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  HGUI_DefWnd_ColorBGWS=ColorBGWS;
  return;
}





void cvSetDefaultColorBorder (TypeColor ColorBorder)
/* 
   ------------------------------------------------------------------------------------------
   Routine cvSetDefaultColorBorder                                             Pubblica
   ------------------------------------------------------------------------------------------
   Questa routine consente di settare i colori che vengono normalmente utilizzati per la
   visualizzazione delle finestre. La routine agisce sul colore del bordo della finestra

   Non sono previsti codici di errore per questa routine.
   ------------------------------------------------------------------------------------------
*/

{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  HGUI_DefWnd_ColorBorder=ColorBorder;
  return;
}





void cvSetNextColorWnd (TypeColor ColorTitle, TypeColor ColorBGTitle, 
                        TypeColor ColorBGWS, TypeColor ColorBorder)
/* 
   ------------------------------------------------------------------------------------------
   Routine cvSetNextColorWnd                                             Pubblica
   ------------------------------------------------------------------------------------------
   Questa routine è utile quando si vuole settare un colore diverso dal set di default solo
   per la successiva finestra che sarà visualizzata. 
   
   Non sono previsti codici di errore per questa routine.
   ------------------------------------------------------------------------------------------
*/

{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  HGUI_NextWnd_ColorTitle   =ColorTitle;
  HGUI_NextWnd_ColorBGTitle =ColorBGTitle;
  HGUI_NextWnd_ColorBGWS    =ColorBGWS;
  HGUI_NextWnd_ColorBorder  =ColorBorder;

  NoDefaultColor=1;
  return;
}






