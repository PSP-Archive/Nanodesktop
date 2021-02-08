/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2007-2012 Filippo Battaglia 
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
    license.txt in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    ----------------------------------------------------------------------------
    NANODESKTOP GRAPHDEMO 1
    ----------------------------------------------------------------------------
*/




#include <nanodesktop.h>

#include <css1.c>
#include <css2.c>
#include <leon.c>




struct ndImage_Type Image1, Image2, ImageWallPaper;

void Window1 ()
{
     unsigned char WndHandle;
     unsigned short int CounterX, CounterY;
     unsigned short int MagicNumber;     
          
     unsigned short int ColorR, ColorG, ColorB;     
          
     char *StrA = "Finestra nr.1: Palette colori";
     
     WndHandle=ndLP_CreateWindow (0, 0, 460, 256, StrA, COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_WHITE, 0);
     ndLP_MaximizeWindow (WndHandle);
     
     
     ndWS_WriteLn ("Questa è una demo della libreria NanoDesktop", COLOR_WHITE, WndHandle, NORENDER);
     ndWS_WriteLn ("creata da Filippo Battaglia.", COLOR_WHITE, WndHandle, NORENDER);
     ndWS_CarriageReturn (WndHandle);
     ndWS_CarriageReturn (WndHandle);
     
     XWindowRender (WndHandle);

     ndDelay (1);
     
     ndWS_ClrALL (WndHandle);

     ndWS_WriteLn ("La libreria Nanodesktop consente ai programmi", COLOR_WHITE, WndHandle, NORENDER);
     ndWS_WriteLn ("l'uso di un semplice ambiente grafico a", COLOR_WHITE, WndHandle, NORENDER);
     ndWS_WriteLn ("finestre. ", COLOR_WHITE, WndHandle, NORENDER);
     XWindowRender (WndHandle);
     
     ndDelay (1);

     ndWS_ClrALL (WndHandle);
     
     ndWS_WriteLn ("Su PSP la libreria gestisce 65.535 colori", COLOR_WHITE, WndHandle, NORENDER);
     
     for (CounterY=0; CounterY<255; CounterY++)
     {
         for (CounterX=0; CounterX<255; CounterX++)
         {
             ColorR = CounterX;
             ColorG = CounterY;
             ColorB = 255*( ((float)CounterX) / 255.0)*( ((float)CounterY) / 255.0);
             
             MagicNumber = ndHAL_RGBToMagicNumber (ColorR, ColorG, ColorB);
             
             ndWS_PutPixel (10+CounterX, 20+CounterY, MagicNumber, WndHandle, NORENDER);
         }
     }
             
     XWindowRender (WndHandle);
     ndDelay (1);
     
     ndWS_ClrALL (WndHandle);
     // ndLP_DestroyWindow (WndHandle);
     
}  


void Window2()
{
     unsigned char WndHandle [24];
     unsigned int Counter;
     
     for (Counter=0; Counter<8; Counter++)
     {
         WndHandle [Counter]=ndLP_CreateWindow (10+10*Counter, 10+5*Counter, 340+10*Counter, 140+5*Counter, "Finestra", COLOR_WHITE, ndWndColorVector [Counter+1], COLOR_BLACK, COLOR_WHITE, 0);
         ndLP_MaximizeWindow (WndHandle [Counter]);
     } 
     
     for (Counter=0; Counter<8; Counter++)
     {
         WndHandle [Counter+12]=ndLP_CreateWindow (10+10*Counter, 52+5*Counter, 340+10*Counter, 182+5*Counter, "Finestra", COLOR_WHITE, ndWndColorVector [Counter+1], COLOR_BLACK, COLOR_WHITE, 0);
         ndLP_MaximizeWindow (WndHandle [Counter+12]);
     } 
     
     ndLP_PutWindowInFirstPlane (3);   
     
     ndWS_WriteLn ("Sono supportate 24 finestre, ma", COLOR_WHITE, 3, NORENDER);
     ndWS_WriteLn ("questo valore può essere cambiato", COLOR_WHITE, 3, NORENDER);
     ndWS_WriteLn ("dal programmatore, compatibilmente", COLOR_WHITE, 3, NORENDER);
     ndWS_WriteLn ("con la memoria disponibile sulla", COLOR_WHITE, 3, NORENDER);
     ndWS_WriteLn ("piattaforma.", COLOR_WHITE, 3, NORENDER);
     XWindowRender (3);
     
     ndDelay (1);
     
     ndLP_MinimizeWindow (3);
     ndLP_MaximizeWindow (3);
                         
     for (Counter=0; Counter<24; Counter++)
     {
         ndLP_DestroyWindow (WndHandle[Counter]);
     } 
     
}

void Window3 (void)
{
     unsigned char WndHandle1, WndHandle2;
     unsigned short int Counter;
     
     WndHandle1=ndLP_CreateWindow (10, 10, 380, 120, "Finestra A", COLOR_WHITE, COLOR_BLUE, COLOR_WHITE, COLOR_BLACK, 0);
     ndLP_MaximizeWindow (WndHandle1);
     
     WndHandle2=ndLP_CreateWindow (100, 100, 430, 220, "Finestra B", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);
     ndLP_MaximizeWindow (WndHandle2);

     ndWS_WriteLn ("Naturalmente non sono obbligato", COLOR_BLACK, WndHandle2, NORENDER);
     ndWS_WriteLn ("ad usare uno sfondo nero per", COLOR_BLACK, WndHandle2, NORENDER);
     ndWS_WriteLn ("per le finestre", COLOR_BLACK, WndHandle2, NORENDER);
     XWindowRender (WndHandle2);
     
     ndDelay (1);
     ndLP_PutWindowInFirstPlane (WndHandle1);
     
     ndWS_WriteLn ("Anzi, posso anche cambiare il colore", COLOR_BLACK, WndHandle1, NORENDER);
     ndWS_WriteLn ("della finestra", COLOR_BLACK, WndHandle1, NORENDER);
     ndWS_CarriageReturn (WndHandle1);
     

     
     ndWS_WriteLn ("Proprio come sto facendo ora", COLOR_BLACK, WndHandle1, NORENDER);
     XWindowRender (WndHandle1);
     
     for (Counter=0; Counter<7; Counter++)
        ndLP_ChangeWndColor (ndWndColorVector [Counter], WndHandle2);

     ndLP_PutWindowInFirstPlane (WndHandle2);
     
     for (Counter=0; Counter<7; Counter++)
        ndLP_ChangeWndColor (ndWndColorVector [Counter], WndHandle2);

     ndLP_DestroyWindow (WndHandle2);
     ndLP_DestroyWindow (WndHandle1);        
}


void Window4()
{
     unsigned char WndHandle1, WndHandle2, WndHandle3;
     unsigned short int Counter;
     
     WndHandle1=ndLP_CreateWindow (0, 0, 330, 230, "Finestra A", COLOR_WHITE, COLOR_BLUE, COLOR_WHITE, COLOR_BLACK, MENUSUPPORTED);
     ndLP_MaximizeWindow (WndHandle1);
     
     int CounterA;
     
     ndIMG_InitImage (&Image1);
     ndIMG_LoadImageFromNDFArray (&Image1, css1Width, css1Height, css1, NDRGB);
     
     ndIMG_ShowImage (&Image1, 0, 0, WndHandle1, RENDER);
     
     ndWS_WriteLn ("La libreria NanoDesktop gestisce", 0xFFFF, WndHandle1, RENDER);
     ndWS_WriteLn ("in modo automatico lo scrolling" , 0xFFFF, WndHandle1, RENDER);
     ndWS_WriteLn ("dei caratteri sullo schermo", 0xFFFF, WndHandle1, RENDER);
     ndWS_CarriageReturn (WndHandle1);
     
     ndWS_WriteLn ("Inoltre gestisce l'overscan", 0xFFFF, WndHandle1, RENDER);
     ndWS_WriteLn ("dei caratteri, come mostra questa", 0xFFFF, WndHandle1, RENDER);
     ndWS_WriteLn ("demo", 0xFFFF, WndHandle1, RENDER);
     
     for (Counter=0; Counter<20; Counter++)
     {
         ndWS_WriteLn ("Guarda lo scrolling....", COLOR_WHITE, WndHandle1, RENDER);
         ndWS_WriteLn ("Guarda lo scrolling....", COLOR_LBLUE, WndHandle1, RENDER);
     }
     
     ndLP_ResizeWindow (250, 100, WndHandle1);
     
     for (Counter=0; Counter<20; Counter++)
     {
         ndWS_WriteLn ("Guarda lo scrolling....", COLOR_GREEN, WndHandle1, RENDER);
         ndWS_WriteLn ("Guarda lo scrolling....", COLOR_RED, WndHandle1, RENDER);
     }
     
     ndLP_ResizeWindow (330, 230, WndHandle1);
     
     for (Counter=0; Counter<10; Counter++)
     {
         ndWS_WriteLn ("Guarda lo scrolling....", COLOR_GREEN, WndHandle1, RENDER);
         ndWS_WriteLn ("Guarda lo scrolling....", COLOR_RED, WndHandle1, RENDER);
     }
     
          
     WndHandle2=ndLP_CreateWindow (200, 50, 360, 170, "Finestra B", COLOR_WHITE, COLOR_GREEN, COLOR_WHITE, COLOR_BLACK, 0);
     ndLP_MaximizeWindow (WndHandle2);
     
     ndIMG_InitImage (&Image2);
     ndIMG_LoadImageFromNDFArray (&Image2, css2Width, css2Height, css2, NDRGB);
     ndIMG_ShowImage (&Image2, 0, 0, WndHandle2, RENDER);
     
     for (Counter=0; Counter<5; Counter++)
     {
         ndWS_WriteLn ("Scrolling....", COLOR_WHITE, WndHandle1, RENDER);
         ndWS_WriteLn ("Scrolling....", COLOR_LGREEN, WndHandle2, RENDER);
         
         ndWS_WriteLn ("Scrolling....", COLOR_LBLUE, WndHandle1, RENDER);
         ndWS_WriteLn ("Scrolling....", COLOR_RED, WndHandle2, RENDER);
     }
     
     
     WndHandle3=ndLP_CreateWindow (20, 180, 440, 250, "", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);
     ndLP_MaximizeWindow (WndHandle3);
      
     ndWS_WriteLn ("Le funzioni di scrolling e overscan", COLOR_BLACK, WndHandle3, RENDER);
     ndWS_WriteLn ("funzionano anche in caso di spostamento", COLOR_BLACK, WndHandle3, RENDER);
     ndWS_WriteLn ("delle finestre.", COLOR_BLACK, WndHandle3, RENDER);
     
     ndLP_MoveWindow (180, 40, WndHandle2);
     
     for (Counter=0; Counter<3; Counter++)
     {
         ndWS_WriteLn ("Scrolling....", COLOR_WHITE, WndHandle1, RENDER);
         ndWS_WriteLn ("Scrolling....", COLOR_LGREEN, WndHandle2, RENDER);
         
         ndWS_WriteLn ("Scrolling....", COLOR_LBLUE, WndHandle1, RENDER);
         ndWS_WriteLn ("Scrolling....", COLOR_RED, WndHandle2, RENDER);
     }
     
     ndLP_MoveWindow (150, 80, WndHandle2);
     
     for (Counter=0; Counter<3; Counter++)
     {
         ndWS_WriteLn ("Scrolling....", COLOR_WHITE, WndHandle1, RENDER);
         ndWS_WriteLn ("Scrolling....", COLOR_LGREEN, WndHandle2, RENDER);
         
         ndWS_WriteLn ("Scrolling....", COLOR_LBLUE, WndHandle1, RENDER);
         ndWS_WriteLn ("Scrolling....", COLOR_RED, WndHandle2, RENDER);
     }
     
     ndLP_MoveWindow (120, 100, WndHandle2);
     
     for (Counter=0; Counter<3; Counter++)
     {
         ndWS_WriteLn ("Scrolling....", COLOR_WHITE, WndHandle1, RENDER);
         ndWS_WriteLn ("Scrolling....", COLOR_LGREEN, WndHandle2, RENDER);
         
         ndWS_WriteLn ("Scrolling....", COLOR_LBLUE, WndHandle1, RENDER);
         ndWS_WriteLn ("Scrolling....", COLOR_RED, WndHandle2, RENDER);
     }
     
     ndLP_DestroyWindow (WndHandle3);
     ndLP_DestroyWindow (WndHandle2);
     ndLP_DestroyWindow (WndHandle1);
     
}

    
void Window5 ()
{
     unsigned char WndHandle;
     unsigned short int Counter;
     
     WndHandle=ndLP_CreateWindow (10, 10, 400, 250, "INGRANDIMENTO", COLOR_WHITE, COLOR_BLUE, COLOR_WHITE, COLOR_BLACK,  
                                  NDKEY_SETWSLENGTH (470) | NDKEY_SETWSHEIGHT (270) );
     
     ndLP_MaximizeWindow (WndHandle);
    
     ndWS_WriteLn ("La libreria Nanodesktop supporta", COLOR_BLACK, WndHandle, NORENDER);
     ndWS_WriteLn ("La visualizzazione delle immagini", COLOR_BLACK, WndHandle, NORENDER);
     ndWS_WriteLn ("con differenti ingrandimenti", COLOR_BLACK, WndHandle, NORENDER);
     XWindowRender (WndHandle);
     
     ndDelay (1);
     ndWS_ClrALL (WndHandle);
     
     ndLP_ChangeWndTitle ("Scala x=1 y=1", COLOR_WHITE, COLOR_BLUE, WndHandle);
     
     ndIMG_LoadImageFromNDFArray (&Image2, css2Width, css2Height, css2, NDRGB);
     ndIMG_ShowImage (&Image2, 0, 0, WndHandle, RENDER);
     
     ndDelay (1);
     ndWS_ClrALL (WndHandle);
     
     ndLP_ChangeWndTitle ("Scala x=0.5 y=0.5", COLOR_WHITE, COLOR_BLUE, WndHandle);
     ndIMG_ShowImageScaled (&Image2, 0, 0, 0.5, 0.5, WndHandle, RENDER);
      
     ndDelay (1);
     ndWS_ClrALL (WndHandle);
     
     ndLP_ChangeWndTitle ("Scala x=2 y=2", COLOR_WHITE, COLOR_BLUE, WndHandle);
     ndIMG_ShowImageScaled (&Image2, 0, 0, 2, 2, WndHandle, RENDER);
     
     for (Counter=0; Counter<=10; Counter++)
     {
         ndLP_ChangeWindowROI (0, Counter*10, ROI_POSY, WndHandle);
     }
     
     
     ndDelay (1);
     ndWS_ClrALL (WndHandle);
     
     ndLP_ChangeWndTitle ("Scala x=3 y=3", COLOR_WHITE, COLOR_BLUE, WndHandle);
     ndIMG_ShowImageScaled (&Image2, 0, 0, 3, 3, WndHandle, RENDER);
     
     for (Counter=0; Counter<=10; Counter++)
     {
         ndLP_ChangeWindowROI (0, Counter*10, ROI_POSY, WndHandle);
     }
     
     
     ndDelay (1);    
     ndIMG_ReleaseImage (&Image1);
     ndIMG_ReleaseImage (&Image2);
     
     ndLP_DestroyWindow (WndHandle);
}

void Window6()
{
  unsigned char WndHandle;   
     
  ndIMG_LoadImageFromNDFArray (&ImageWallPaper, sfondoWidth, sfondoHeight, sfondo, NDRGB);   
  ndLP_LoadWallPaperFromNDImage (&ImageWallPaper);
  ndIMG_ReleaseImage (&ImageWallPaper);
  
  ndLP_EnableTrasparency ();
  
  WndHandle=ndLP_CreateWindow (10, 10, 380, 120, "Trasparenze", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY, COLOR_BLACK, 0);
  ndLP_MaximizeWindow (WndHandle);
  
  ndWS_WriteLn ("La libreria Nanodesktop supporta", COLOR_BLACK, WndHandle, NORENDER);
  ndWS_WriteLn ("la gestione degli sfondi e delle", COLOR_BLACK, WndHandle, NORENDER);
  ndWS_WriteLn ("trasparenze.", COLOR_BLACK, WndHandle, NORENDER);
  XWindowRender (WndHandle);

  ndDelay (1);
  ndLP_DisableTrasparency ();
  ndLP_DisableWallPaper ();  
  ndLP_DestroyWindow (WndHandle);  
}






void ndMain()
{      
    ndInitSystem ();
    
    Window1();
    Window2();  
    Window3();
    Window4();
    Window5(); 
    Window6();    
       
    ndSystemHalt(0);    
}






