/* ----------------------------------------------------------------------------
   NANODESKTOP GOCR TEST 1
   ----------------------------------------------------------------------------
   di Andrea Badiali
   ----------------------------------------------------------------------------
   Questo programma è un front-end per il motore di scan ndGOCR fornito a
   partire da Nanodesktop 0.3.3 
   ----------------------------------------------------------------------------
*/




#include <nanodesktop.h>

int YouCanExit=0, TBoxHandle, OpenExist=0, OptionExist=0, ErrRep;
int MoreInfoVar=0, ListChapesVar=0, ListPatternVar=0, ListAllPatternVar=0, LineInfosVar=0,DebugModeVar=0;
int SpacewidthVar=0, GraylevelVar=160, DustsizeVar=20,UseDatabaseVar=0,LayoutAnalysisVar=0,Compare_modeVar=0, Divide_modeVar=0,ContextCorrectionVar=0,CharPackingVar=0, OnlyNumbersVar=0;
struct ndImage_Type Image;
char NameFile[255], FileChoosen[255], command[255], temp[255];

int ErrorLevel;
int AboutWndIsOpen=0;

ndint64 TheCloseWndCallback (char WndHandle, ndint64 WndInfoField)
{
     YouCanExit=1;
     ndHAL_Delay(0.15);
} 

int gocr_main(int argn, char *argv[]);
// Riferimento main

void Scan (char *StringID, ndint64 InfoField, char WndHandle)
{
     if (strlen(NameFile))
     {
        int v=0, m=0;
        strcat(&command,  " -i ");
        strcat(&command, "ms0:/temp.pnm");
        
        v=MoreInfoVar+ListChapesVar*2+ListPatternVar*4+ListAllPatternVar*8+LineInfosVar*16+DebugModeVar*32;
        strcat(&command, " -v ");
        _itoa(v, &temp, 10);
        strcat(&command, &temp);
        
        m=UseDatabaseVar*2+LayoutAnalysisVar*4+Compare_modeVar*8+Divide_modeVar*16+ContextCorrectionVar*32+CharPackingVar*64;
        strcat(&command, " -m ");
        _itoa(m, &temp, 10);
        strcat(&command, &temp);
        
        strcat(&command, " -l ");
        _itoa(GraylevelVar, &temp, 10);
        strcat(&command, &temp);
        
        strcat(&command, " -d ");
        _itoa(DustsizeVar, &temp, 10);
        strcat(&command, &temp);
        
        strcat(&command, " -s ");
        _itoa(SpacewidthVar, &temp, 10);
        strcat(&command, &temp);
         
        if (OnlyNumbersVar) strcat(&command, " -n 1 ");  
        
        // Esegui la scansione
        pseudoExec (&gocr_main, "gocr ", command, &ErrorLevel); 
     }
     else
     {
        char WndHandleLocal;
        WndHandleLocal=ndLP_CreateWindow (75, 50, 370, 100, "Error", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY09, COLOR_WHITE, NO_RESIZEABLE);
        ndLP_SetupCloseWndCallback (&TheCloseWndCallback, 0, 0, WndHandleLocal);    
        ndLP_PutWindowInFirstPlane(WndHandleLocal);
        ndLP_MaximizeWindow (WndHandleLocal);
        ndWS_GrWriteLn(10,10, "Error: please open a image file!", COLOR_BLUE, COLOR_GRAY09, WndHandleLocal, RENDER);
        ndHAL_Delay(3);
        ndLP_DestroyWindow(WndHandleLocal);      
     }
}

void OpenFile (char *StringID, ndint64 InfoField, char WndHandle)
{
     if (!OpenExist)
     {
        char PathFile[255],ExtFile[4];
        ndIMG_InitImage(&Image);
        OpenExist=1;
        
	    //Richiamo il FileManager per selezionare un file immagine
        FileManager ("Choose a image file for OCR", 0, 0, &FileChoosen);
    
        ErrRep=ndIMG_LoadImage(&Image, &FileChoosen, NDMGKNB);
        
        if (ErrRep) 
        {
             char WndHandleLocal;
             WndHandleLocal=ndLP_CreateWindow (75, 50, 370, 100, "Error", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY09, COLOR_WHITE, NO_RESIZEABLE);
             ndLP_SetupCloseWndCallback (&TheCloseWndCallback, 0, 0, WndHandleLocal);    
             ndLP_PutWindowInFirstPlane(WndHandleLocal);
             ndLP_MaximizeWindow (WndHandleLocal);
             ndWS_GrWriteLn(10,10, "Error loading the image file!", COLOR_BLUE, COLOR_GRAY09, WndHandleLocal, RENDER);
             ndHAL_Delay(3);
             ndLP_DestroyWindow(WndHandleLocal);        
        }
        else 
        {
             //Scrivo il nome del file sull'interfaccia
             fssSplitNameFile(&FileChoosen,PathFile,NameFile,ExtFile);
             strncpy(temp,NameFile,strlen(NameFile));
             strcat(temp,".");
             strcat(temp, ExtFile);
             strncpy ( ndTBOX_GetRowAddr (0, TBoxHandle, WndHandle), temp,strlen(temp));
             ndTBOX_TextAreaUpdate (TBoxHandle, WndHandle, RENDER);
             ndIMG_SaveImage(&Image, "ms0:/temp.pnm");
        }
        
        OpenExist=0;
    };
    
    ndHAL_Delay(0.15);     
}

Spacewidth_Callback (float SpacewidthChange, int TB_InfoField, char WndHandle)
{
       SpacewidthVar=SpacewidthChange;
}

Graylevel_Callback (float GraylevelChange, int TB_InfoField, char WndHandle)
{
       GraylevelVar=GraylevelChange;
}

Dustsize_Callback (float DustsizeChange, int TB_InfoField, char WndHandle)
{
       DustsizeVar=DustsizeChange;
}

ndint64 Option(char WndHandle)
//Funzione che apre la finestra contenente le opzioni avanzate
{
    if (!OptionExist)
    {
        OptionExist=1;
        char WndHandleLocal;
        WndHandleLocal=ndLP_CreateWindow (100, 50, 280, 200, "Advanced Option", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY09, COLOR_WHITE, NO_RESIZEABLE);
        ndLP_SetupCloseWndCallback (&TheCloseWndCallback, 0, 0, WndHandleLocal);    
        ndLP_PutWindowInFirstPlane(WndHandleLocal);
        ndLP_MaximizeWindow (WndHandleLocal);
         
        ndCBOX_CreateCBox (10, 5, "MoreInfo", MoreInfoVar, 0, &MoreInfoVar, COLOR_GRAY09, 0, WndHandleLocal, NORENDER);
        ndWS_GrWriteLn (30, 9, "More info", COLOR_WHITE, COLOR_GRAY09, WndHandleLocal, NORENDER);
        
        ndCBOX_CreateCBox (10, 20, "ListChapes", ListChapesVar, 0, &ListChapesVar, COLOR_GRAY09, 0, WndHandleLocal, NORENDER);
        ndWS_GrWriteLn (30, 24, "List chapes", COLOR_WHITE, COLOR_GRAY09, WndHandleLocal, NORENDER);
        
        ndCBOX_CreateCBox (10, 35, "ListPattern", ListPatternVar, 0, &ListPatternVar, COLOR_GRAY09, 0, WndHandleLocal, NORENDER);
        ndWS_GrWriteLn (30, 39, "List pattern", COLOR_WHITE, COLOR_GRAY09, WndHandleLocal, NORENDER);
        
        ndCBOX_CreateCBox (10, 50, "ListAllPattern", ListAllPatternVar, 0, &ListAllPatternVar, COLOR_GRAY09, 0, WndHandleLocal, NORENDER);
        ndWS_GrWriteLn (30, 54, "List all pattern", COLOR_WHITE, COLOR_GRAY09, WndHandleLocal, NORENDER);
        
        ndCBOX_CreateCBox (10, 65, "LineInfos", LineInfosVar, 0, &LineInfosVar, COLOR_GRAY09, 0, WndHandleLocal, NORENDER);
        ndWS_GrWriteLn (30, 69, "Line infos", COLOR_WHITE, COLOR_GRAY09, WndHandleLocal, NORENDER);
        
        ndCBOX_CreateCBox (10, 80, "DebugMode", DebugModeVar, 0, &DebugModeVar, COLOR_GRAY09, 0, WndHandleLocal, NORENDER);
        ndWS_GrWriteLn (30, 84, "Debug mode", COLOR_WHITE, COLOR_GRAY09, WndHandleLocal, NORENDER);
        
        XWindowRender(WndHandleLocal);
        
        ndProvideMeTheMouse_Until (&YouCanExit, ND_IS_EQUAL, 1, 0, 0);
        //Uscita dall'applicazione e ritorno alla finestra base
        
        OptionExist=0;
        YouCanExit=0;
        ndLP_DestroyWindow(WndHandleLocal);
    }
}

ndint64 cbclwnd_About (char WndHandle, ndint64 WndInfoField)
{
    AboutWndIsOpen=0;
    
    ndLP_DestroyWindow (WndHandle);
    return 0;    
}

ndint64 cbAbout (char WndHandle)
{
  int WndAboutDialogBox;
  
  if (!AboutWndIsOpen)
  {
      WndAboutDialogBox = ndLP_AllocateWindow (10, 30, Windows_MaxScreenX-10, Windows_MaxScreenY-30,
                             "About", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK);
            
      if (WndAboutDialogBox>=0)
      {      
                                 
          ndWS_CarriageReturn (WndAboutDialogBox);
          
          ndWS_WriteLn ("GNU OCR Test for Nanodesktop - Ver. 1.00", COLOR_BLUE, WndAboutDialogBox, NORENDER);
          ndWS_WriteLn ("Copyright (C) 2008-2013 Andrea Badiali ", COLOR_BLUE, WndAboutDialogBox, NORENDER);
          ndWS_CarriageReturn (WndAboutDialogBox);
          ndWS_WriteLn ("This application is based on Nanodesktop library", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          ndWS_CarriageReturn (WndAboutDialogBox);
          ndWS_WriteLn ("Routines for loading/saving images are provided by", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          ndWS_WriteLn ("ndDevIL released under LGPL license", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          ndWS_WriteLn ("DevIL: Copyright (C) 2000-2002 by Denton Woods", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          
          ndWS_CarriageReturn (WndAboutDialogBox);
          ndWS_WriteLn ("This application uses also IJG libjpg, pnglib", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          ndWS_WriteLn ("tifflib and libz deflate. ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          ndWS_CarriageReturn (WndAboutDialogBox);
          
          ndWS_WriteLn ("OCR Engine provided by GNU and released under", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          ndWS_WriteLn ("GPL license.            ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          ndWS_CarriageReturn (WndAboutDialogBox);
          
          ndWS_WriteLn ("GOCR: Authors Joerg Schulenburg and ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          ndWS_WriteLn ("Bruno Barberi Gnecco ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
          
          ndLP_MaximizeWindow (WndAboutDialogBox);  
          
          // Crea la close window callback
          ndLP_SetupCloseWndCallback (&cbclwnd_About, 0, 0, WndAboutDialogBox);
          
          XWindowRender (WndAboutDialogBox);
          AboutWndIsOpen = 1;
      }   
  }
}

ndint64 cbExit (char WndHandle)
{
   ndHAL_SystemHalt (0);
   return 0;
}






void InterfacciaMain (int WndHandle)
{
     ndCTRL_CreateButton ( 10, 10, 90, 40, WndHandle, "Open", "Open", "File", COLOR_WHITE, COLOR_BLUE, COLOR_WHITE,
                         ND_BUTTON_ROUNDED | ND_CALLBACK_IN_NEW_CONTEXT, &OpenFile, 0, NORENDER );
     ndCTRL_CreateButton ( 95, 10, 170, 40, WndHandle, "Advance", "Advanced", "Option", COLOR_WHITE, COLOR_BLUE, COLOR_WHITE, 
                         ND_BUTTON_ROUNDED | ND_CALLBACK_IN_NEW_CONTEXT, &Option, 0, NORENDER );
     
     ndWS_GrWriteLn(10,45, "Spacewidth", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     ndTBAR_CreateTrackBar (10, 55, 165, 75,COLOR_WHITE, COLOR_BLUE, COLOR_GRAY09, COLOR_WHITE, COLOR_GRAY09, COLOR_WHITE, 0, 0, 200, NDKEY_FONT (1), &Spacewidth_Callback, 0,WndHandle,NORENDER);

     ndWS_GrWriteLn(10,85, "Graylevel", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     ndTBAR_CreateTrackBar (10, 95, 165, 115,COLOR_WHITE, COLOR_BLUE, COLOR_GRAY09, COLOR_WHITE, COLOR_GRAY09, COLOR_WHITE, 160, 0, 255, NDKEY_FONT (1), &Graylevel_Callback, 0,WndHandle,NORENDER);

     ndWS_GrWriteLn(10,125, "Dustsize", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     ndTBAR_CreateTrackBar (10, 135, 165, 155,COLOR_WHITE, COLOR_BLUE, COLOR_GRAY09, COLOR_WHITE, COLOR_GRAY09, COLOR_WHITE, 20, 0, 60, NDKEY_FONT (1), &Dustsize_Callback, 0,WndHandle,NORENDER);
     
     ndCBOX_CreateCBox (175, 10, "UseDatabase", UseDatabaseVar, 0, &UseDatabaseVar, COLOR_GRAY09, 0, WndHandle, NORENDER);
     ndWS_GrWriteLn (192, 14, "Use Database", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     
     ndCBOX_CreateCBox (175, 25, "LayoutAnalysis", LayoutAnalysisVar, 0, &LayoutAnalysisVar, COLOR_GRAY09, 0, WndHandle, NORENDER);
     ndWS_GrWriteLn (192, 29, "Layout analysis", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     
     ndCBOX_CreateCBox (175, 40, "Compare_mode", Compare_modeVar, 0, &Compare_modeVar, COLOR_GRAY09, 0, WndHandle, NORENDER);
     ndWS_GrWriteLn (192, 44, "Not Compare mode", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     
     ndCBOX_CreateCBox (175,55, "Divide_mode", Divide_modeVar, 0, &Divide_modeVar, COLOR_GRAY09, 0, WndHandle, NORENDER);
     ndWS_GrWriteLn (192, 59, "Not Divide mode", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     
     ndCBOX_CreateCBox (175, 70, "ContextCorrection", ContextCorrectionVar, 0, &ContextCorrectionVar, COLOR_GRAY09, 0, WndHandle, NORENDER);
     ndWS_GrWriteLn (192, 74, "Not Context correction", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     
     ndCBOX_CreateCBox (175, 85, "CharPacking", CharPackingVar, 0, &CharPackingVar, COLOR_GRAY09, 0, WndHandle, NORENDER);
     ndWS_GrWriteLn (192, 89, "Char packing", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     
     ndCBOX_CreateCBox (175, 100, "OnlyNumbers", OnlyNumbersVar, 0, &OnlyNumbersVar, COLOR_GRAY09, 0, WndHandle, NORENDER);
     ndWS_GrWriteLn (192, 104, "Only numbers", COLOR_WHITE, COLOR_GRAY09, WndHandle, NORENDER);
     
     // Creo la voce di menu
        
     ndWMENU_DefineMenuItem (WndHandle, "File", "File", 0, 0, NORENDER);
     ndWMENU_DefineMenuItem (WndHandle, "File.About", "About", ND_CALLBACK_IN_NEW_CONTEXT, &cbAbout, NORENDER);
     ndWMENU_DefineMenuItem (WndHandle, "File.Quit", "Quit", ND_CALLBACK_IN_NEW_CONTEXT, &cbExit, NORENDER);
        
     // Creo la textbox che conterrà  il nome del file caricato
     TBoxHandle = ndTBOX_CreateTextArea(10,160,360,185,"NameBox", NDKEY_SETTEXTAREA (41, 2) | ND_TEXTAREA_INHIBITED | NDKEY_FONT (1), 0, COLOR_WHITE, COLOR_GRAY09, COLOR_WHITE, COLOR_GRAY09, COLOR_WHITE, COLOR_GRAY09, NULL, NULL, NULL, 0, WndHandle, NORENDER);
     
     ndCTRL_CreateButton ( 320, 110, 380, 150, WndHandle, "Scan", "Start", "Scan", COLOR_WHITE, COLOR_BLUE, COLOR_WHITE, 
                         ND_BUTTON_ROUNDED, &Scan, 0, NORENDER );
                         
     XWindowRender(WndHandle);
}

int ndMain()
{
    int WndHandle;
    ndInitSystem();
    
    //Creazione finestra base
    WndHandle=ndLP_CreateWindow (1, 1, 420, 240, "VisualOCR", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY09, COLOR_WHITE, MENUSUPPORTED);
    ndLP_PutWindowInFirstPlane(WndHandle);
    ndLP_MaximizeWindow (WndHandle);
    ndLP_SetupCloseWndCallback (&TheCloseWndCallback, 0, 0, WndHandle);
    ndLP_EnableTrasparency();
    
    InterfacciaMain(WndHandle);
    
    ndProvideMeTheMouse_Until (&YouCanExit, ND_IS_EQUAL, 1, 0, 0);
    
    remove("ms0:/temp.pnm"); 
    ndLP_DestroyWindow(WndHandle);
    ndHAL_SystemHalt (0);
}
