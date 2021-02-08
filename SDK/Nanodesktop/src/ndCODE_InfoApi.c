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
    INFO API
    ----------------------------------------------------------------------------
*/                   

#include <$_NDforND.h>



/*
   -------------------------------------------------------------------------
   ROUTINE DELL'API INFO
   -------------------------------------------------------------------------
*/


ndint64 ndINFO_GetValue (int Code, int Param1, int Param2, int Param3)
/*
    Questa routine è uno delle più importanti nella Info API di Nanodesktop.
    
    L'utente richiede una certa informazione fornendo un codice opportuno, e
    la routine la restituisce al programma chiamante. 
    
    I parametri Param1, Param2, Param3 non hanno sempre significato: il loro
    eventuale significato dipende dal codice.
    
    Il parametro Code è un parametro a 32 bit che indica il tipo di
    informazione che viene richiesta. Il primo bit di solito deve
    essere posto a 0: se è posto ad un valore diverso, vuol dire
    che siamo in presenza di un parametro specifico dell'architettura

    La routine segnala un errore nei parametri restituendo un valore in uscita
    NDGET___ERROR.
*/
{
    if ((Code & NDGET_HAL) == NDGET_HAL) 
    {
       return ndINFO_HAL_GetValue (Code, Param1, Param2, Param3);
    }
    
    switch (Code)
    {
           case NDGET_MAXSCREENX:                  { return __MAXSCREENX; };
           case NDGET_MAXSCREENY:                  { return __MAXSCREENY; };
           case NDGET_VMAXSCREENX:                 { return __VMAXSCREENX;   }     
           case NDGET_VMAXSCREENY:                 { return __VMAXSCREENY;   };       
           case NDGET_WINDOWS_MAXSCREENX:          { return Windows_MaxScreenX;   };       
           case NDGET_WINDOWS_MAXSCREENY:          { return Windows_MaxScreenY;   };       
           case NDGET_MAXCURSX:                    { return __MAXCURSX;   };       
           case NDGET_MAXCURSY:                    { return __MAXCURSX;   };  
           
           case NDGET_MINWINDOWSBAR_X1:            { return MinWindowsBar_x1; };                                  
           case NDGET_MINWINDOWSBAR_Y1:            { return MinWindowsBar_y1; };    
           case NDGET_MINWINDOWSBAR_X2:            { return MinWindowsBar_x2; };    
           case NDGET_MINWINDOWSBAR_Y2:            { return MinWindowsBar_y2; };  
           
           case NDGET_MINWINDOWSBARISCHANGED:      { return MinWindowsBarIsChanged; };       
           case NDGET_OTHERFBPAGEISNOTUPGRADED:    { return OtherFBPageIsNotUpgraded; };  

           case NDGET_LL_POSCURSX:                 { return LL_PosCursX; };     
           case NDGET_LL_POSCURSY:                 { return LL_PosCursY; };

           case NDGET_MOUSE_MAXSCREENX:            { return Mouse_MaxScreenX; };
           case NDGET_MOUSE_MAXSCREENY:            { return Mouse_MaxScreenY; };
           
           case NDGET_NRWINDOWSALLOCABLE:          { return __NRWINDOWSALLOCABLE;   };       
           case NDGET_NRBUTTONSALLOCABLE:          { return __NRBUTTONSALLOCABLE;   };       
           case NDGET_NRTRACKBARSALLOCABLE:        { return __NRTRACKBARSALLOCABLE;   };      
           case NDGET_NRFONTS_USABLE:              { return NRFONTS_USABLE;   };       
           case NDGET_USERAREASIZE:                { return __USERAREASIZE;   };                           

           case NDGET_NDLL_CURRENT_FONT_HANDLER:   { return  ndLL_CurrentFntHandle; };        
           case NDGET_NDLP_CURRENT_FONT_HANDLER:   { return  WindowData [Param1].ndLP_CurrentFntHandle; };     
           case NDGET_NDBS_CURRENT_FONT_HANDLER:   { return  ndBS_CurrentFntHandle; };                     
           case NDGET_FONT_INFO_ADR:
           {
                if (Param1<NRFONTS_USABLE) return ( &(FontHandlesArray [Param1]) );
                else return NDGET___ERROR;
           }

           case NDGET_MOUSEPOINTER_POSX:           { return MousePointer_PosX; };     
           case NDGET_MOUSEPOINTER_POSY:           { return MousePointer_PosY; };     
           case NDGET_OLDMOUSEPOINTER_POSX:        { return OLD_MousePointer_PosX; };     
           case NDGET_OLDMOUSEPOINTER_POSY:        { return OLD_MousePointer_PosY; };     
           case NDGET_ISTHEFIRSTTIMEFORMOUSE:      { return IsTheFirstTimeForMouse; };     
           case NDGET_BACKGROUNDMOUSE:                                 
           {
                if ((Param1<16) && (Param2<16)) return BackgroundMouse [Param1][Param2];
                   else return NDGET___ERROR;
           }

           case NDGET_BASESCREENISACTIVE:          { return BaseScreenIsActive; };
           case NDGET_BASESCREEN:
           {
               if ((Param1<__MAXSCREENX - 14) && (Param2<__MAXSCREENY - 14)) return BaseScreen [Param2][Param1];
                   else return NDGET___ERROR; 
           }
           
           case NDGET_TRASPARENCYENABLED:          { return TrasparencyEnabled; };
           
           case NDGET_OWNERARRAY:
           {
               if ((Param1<__VMAXSCREENX) && (Param2<__VMAXSCREENY) && (Param3<2)) return OwnerArray [Param1][Param2][Param3];
                  else return NDGET___ERROR;
           }
           
           case NDGET_WINDOWSVECTOR:
           {
                if (Param1<__NRWINDOWSALLOCABLE) return WindowsVector [Param1];
                  else return NDGET___ERROR;
           } 
           
           
           case NDGET_NRWINDOWSALLOCATED:         { return NrWindowsAllocated; }
           
           case NDGET_MAXDIM_WSLENGTH:
           {
                return MAXDIM_WSLENGTH;
           }
           
           case NDGET_MAXDIM_WSHEIGHT:
           {
                return MAXDIM_WSHEIGHT;
           }
           
           
           case NDGET_OVERDIMX:
                /*
                    Fornisce il sovradimensionamento per le finestre in dimensione X. Param1 deve
                    essere impostato a 0 se la finestra non usa menù, o a 1 se la finestra usa
                    menù.
                */
           {
                return (Param1 ? 16 : 16);
                // 16 per MenuSupported e 16 per Menu not Supported
           }
           
           case NDGET_OVERDIMY:
                /*
                    Fornisce il sovradimensionamento per le finestre in dimensione Y. Param1 deve
                    essere impostato a 0 se la finestra non usa menù, o a 1 se la finestra usa
                    menù.
                */
           {
                return (Param1 ? 46 : 27);
                // 46 per MenuSupported e 27 per Menu Not Supported
           }
           
           case NDGET_THUMBNAILENABLED:               { return ThumbnailEnabled; };

           case NDGET_STDOUTWINDOWACTIVATED:          { return StdOutWindowActivated; };           
           case NDGET_STDOUTPOSX1:                    { return StdOutPosX1; };   
           case NDGET_STDOUTPOSY1:                    { return StdOutPosY1; };   
           case NDGET_STDOUTPOSX2:                    { return StdOutPosX2; };   
           case NDGET_STDOUTPOSY2:                    { return StdOutPosY2; };   
           case NDGET_STDOUTCOLORTITLE:               { return StdOutColorTitle; };   
           case NDGET_STDOUTCOLORBGTITLE:             { return StdOutColorBGTitle; };   
           case NDGET_STDOUTCOLORWINDOW:              { return StdOutColorWindow; };   
           case NDGET_STDOUTCOLORBORDERWINDOW:        { return StdOutColorBorderWindow; };   
           case NDGET_STDOUTCOLORTEXT:                { return StdOutColorText; };   
           case NDGET_STDOUTCOLORTEXTERR:             { return StdOutColorTextErr; };   
           case NDGET_STDOUTWNDHANDLER:               { return StdOutWindowHandle; };   

           case NDGET_SYSTEMKEYBOARDTYPE:             { return SystemKeyboardType; };
           case NDGET_SYSTEMKEYBOARDAUTOCLOSE:        { return SystemKeyboardAutoClose; };
           case NNM_SYSTEMRECORD_ADR:                 { return 0; };

           case NDGET_BUTTONCLASS1_ADR:
           {
                if (Param1<__NRWINDOWSALLOCABLE) return ( &(ButtonClass1_Data [Param1]) );
                   else return NDGET___ERROR;
           }
           
           case NDGET_BUTTONCLASS2_ADR:
           {
                if (Param1<4) return ( &(ButtonClass2_Data [Param1]) );
                   else return NDGET___ERROR;
           }
           
           case NDDEVIL_SUPPORT_IS_OPERATIVE:
           {
                #ifdef I_HAVE_DEV_IL
                   return 1;
                #else
                   return 0;
                #endif
           }
           
           case NDDEVIL_ILGENIMAGE_ADR:
           {
                #ifdef I_HAVE_DEV_IL
                   return (ndint64)(&ilGenImages);
                #else
                   return 0;
                #endif
           }
           
           case NDDEVIL_ILBINDIMAGE_ADR:
           {
                #ifdef I_HAVE_DEV_IL
                   return (ndint64)(&ilBindImage);
                #else
                   return 0;
                #endif
           }
           
           case NDDEVIL_ILLOADL_ADR:
           {
                #ifdef I_HAVE_DEV_IL
                   return (ndint64)(&ilLoadL);
                #else
                   return 0;
                #endif
           }
           
           case NDDEVIL_ILCOPYPIXEL_ADR:
           {
                #ifdef I_HAVE_DEV_IL
                   return (ndint64)(&ilCopyPixels);
                #else
                   return 0;
                #endif
           }   
           
           case NDDEVIL_ILDELETEIMAGES_ADR:
           {
                #ifdef I_HAVE_DEV_IL
                   return (ndint64)(&ilDeleteImages);
                #else
                   return 0;
                #endif
           }  
    }
    
    return NDGET___ERROR;
}




