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

    This is a demo that shows the capabilities of ndSQLLite layer in nd. 
    The code for shell.c has been provided by SQLLite team. 
    ----------------------------------------------------------------------------
    NANODESKTOP SQLLITE SUPPORT DEMO 
    ----------------------------------------------------------------------------
*/



#include <nanodesktop.h>
#include <stdio.h>
#include <sqlite3.h>

/*
   -----------------------------------------------------------------------------
   VARIABILI
   -----------------------------------------------------------------------------
*/

char input_string [1024];
int  YouCanExit;

int  WndHandle0;
int  TBoxHandle0;
int  BtnHandleEnter0, BtnHandleExit0;
int  YouCanExit0;

int  WndHandle1;
int  TBoxHandle1;
int  BtnHandleEnter1;
int  YouCanExit1;

char AboutWndIsOpen = 0;
char WndAboutDialogBox;



// Riferimenti a routines definite nel seguito
int qmain (int argc, char **argv);

/*
   -----------------------------------------------------------------------------
   INIZIALIZZAZIONE
   -----------------------------------------------------------------------------
*/

char Predefined [5][50] =
{ "", "create table agenda (",
  "id integer primary key,",
  "nome varchar(200)",
  ");" };
  
int Pred=0;



/*
   -----------------------------------------------------------------------------
   DIALOG BOX 1 
   -----------------------------------------------------------------------------
*/

ndint64 cbdb1_Enter (char *StringID, ndint64 InfoField, char WndHandle)
{
   YouCanExit1=1;    
}

void DialogBox1 (void)
{
     memset (input_string, 0, 1023);
    
     WndHandle1 = ndLP_CreateWindow (Windows_MaxScreenX-320, 0, Windows_MaxScreenX-1, 62, 
          "Enter command", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY06, COLOR_GRAY06, NO_RESIZEABLE);

     if (WndHandle1>=0)
     {
        ndLP_MaximizeWindow (WndHandle1);               
     
        ndWS_GrWriteLn (10, 07, "Enter the command and press OK", COLOR_WHITE, COLOR_GRAY06, WndHandle1, NORENDER);
        
        TBoxHandle1 = ndTBOX_CreateTextArea    (10, 20, 270, 40, "Textarea", NULL, &input_string, 
                              COLOR_BLACK,  COLOR_WHITE, COLOR_WHITE,  COLOR_GRAY05, COLOR_BLACK, COLOR_GRAY05,
                              0, 0, 0, 0, WndHandle1, NORENDER);
        
        BtnHandleEnter1 = ndCTRL_CreateButton   (280, 20, 310, 40, WndHandle1, "BtnHandleOk1", 
			  	                                "OK", "", COLOR_WHITE, COLOR_GRAY07, COLOR_BLACK, 
				                                ND_BUTTON_ROUNDED, &cbdb1_Enter, 0, NORENDER);
        
        XWindowRender (WndHandle1);
        
        YouCanExit1=0;
        ndProvideMeTheMouse_Until (&YouCanExit1, ND_IS_GREATER, 0, 0, 0);
     
        // Arrivati a questo punto l'utente ha premuto un tasto
     
        ndLP_DestroyWindow (WndHandle1);
        
     }     
}





/*
   -----------------------------------------------------------------------------
   LIVELLO 1 E GETS MANAGER
   -----------------------------------------------------------------------------
*/


char *INTERNAL_MyFGetsManager (char *buf, int n, FILE *stream)
{
    
    DialogBox1 (); 
 
    #ifndef NDDEMO5_AUTOPILOT
            ndStrCpy (buf, input_string, n, 0);
    #else
            if (Pred<5)
            {
               strcpy (buf, &(Predefined [Pred][50]));
               Pred++;
            }
            else
            {
               ndStrCpy (buf, input_string, n, 0);
            }
    #endif
    
    strcat (buf, "\n");          // Necessario per segnalare il fine stringa al server SQLLite
    
    printf (buf);
    return buf;
}


void PassToLevel1 (void)
{
     #ifndef NDDEMO5_AUTOPILOT
          pseudoExec (&qmain, "FLITE", &input_string, 0);
     #else
          printf ("%s \n", input_string);
          pseudoExec (&qmain, "FLITE", "ms0:/age.db", 0);
     #endif
}



/*
   -----------------------------------------------------------------------------
   ABOUT WINDOW
   -----------------------------------------------------------------------------
*/

ndint64 cbAboutWnd_CLOSE (char WndHandle, ndint64 WndInfoField)
{
     ndLP_DestroyWindow (WndAboutDialogBox);
     AboutWndIsOpen=0;
}


void CreateAboutWindow ()
{
     if (!AboutWndIsOpen)
     {
         WndAboutDialogBox = ndLP_CreateWindow (20, 20, 410, 200, "About", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0); 
         ndLP_MaximizeWindow (WndAboutDialogBox);
         
         if (WndAboutDialogBox>=0)
         {
             ndLP_SetupCloseWndCallback (cbAboutWnd_CLOSE, 0, 1, WndAboutDialogBox);
             
             ndWS_CarriageReturn (WndAboutDialogBox);
             ndWS_WriteLn (" Nanodesktop Demo 5 - Version 1.00", COLOR_BLUE, WndAboutDialogBox, NORENDER);
             ndWS_WriteLn (" Copyright (C) 2007-2011 Filippo Battaglia", COLOR_BLUE, WndAboutDialogBox, NORENDER);
             ndWS_CarriageReturn (WndAboutDialogBox);
             ndWS_WriteLn (" This application is based on Nanodesktop ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
             ndWS_WriteLn (" library version 0.3 ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
             ndWS_CarriageReturn (WndAboutDialogBox);
             ndWS_WriteLn (" The code for SQLLite command parser is ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
             ndWS_WriteLn (" derived by shell.c by Hwaci ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
             ndWS_CarriageReturn (WndAboutDialogBox);
             ndWS_WriteLn (" See SQLLite website for copyright ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
             ndWS_WriteLn (" informations ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
             
             XWindowRender (WndAboutDialogBox);                  
             AboutWndIsOpen=1; 
         }        
     }  
}  





/*
   -----------------------------------------------------------------------------
   DIALOG BOX 0
   -----------------------------------------------------------------------------
*/



ndint64 cbdb0_Enter (char *StringID, ndint64 InfoField, char WndHandle)
{
   YouCanExit0=1;    
}

ndint64 cbdb0_Exit (char *StringID, ndint64 InfoField, char WndHandle)
{
   YouCanExit0=2;     
}

char cbwmi_About (char WndHandle)
{
   CreateAboutWindow ();  
}

char cbwmi_Exit (char WndHandle)
{
   YouCanExit0=2;    
}

void DialogBox0 (void)
{
     memset (input_string, 0, 1023);
     
     WndHandle0 = ndLP_CreateWindow (10, 40, Windows_MaxScreenX-10, Windows_MaxScreenY-123, 
          "ndSQLLite Demo", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY06, COLOR_GRAY06, MENUSUPPORTED);

     if (WndHandle0>=0)
     {
        ndWMENU_DefineMenuItem (WndHandle0, "File",            "File",     0, 0,           RENDER);
        ndWMENU_DefineMenuItem (WndHandle0, "File.About",      "About",    0, &cbwmi_About,    RENDER);
        ndWMENU_DefineMenuItem (WndHandle0, "File.Exit",       "Exit",     0, &cbwmi_Exit,     RENDER); 
      
        ndLP_MaximizeWindow (WndHandle0);               
     
        ndWS_GrWriteLn (10, 07, "Enter the command line parameters for SQLLite server", COLOR_WHITE, COLOR_GRAY06, WndHandle0, NORENDER);
        
        TBoxHandle0 = ndTBOX_CreateTextArea    (10, 20, 270, 40, "Textarea", NULL, &input_string, 
                              COLOR_BLACK,  COLOR_WHITE, COLOR_WHITE,  COLOR_GRAY05, COLOR_BLACK, COLOR_GRAY05,
                              0, 0, 0, 0, WndHandle0, NORENDER);
        
        BtnHandleEnter0 = ndCTRL_CreateButton   (280, 20, 350, 40, WndHandle0, "BtnHandleOk0", 
			  	                                "OK", "", COLOR_WHITE, COLOR_GRAY07, COLOR_BLACK, 
				                                ND_BUTTON_ROUNDED, cbdb0_Enter, 0, NORENDER);
        
        BtnHandleExit0 = ndCTRL_CreateButton   (355, 20, 425, 40, WndHandle0, "BtnHandleExit0", 
			  	                                "Exit", "", COLOR_WHITE, COLOR_GRAY07, COLOR_BLACK, 
				                                ND_BUTTON_ROUNDED, cbdb0_Exit, 0, NORENDER); 
        
        XWindowRender (WndHandle0);
        
        YouCanExit0=0;
        ndProvideMeTheMouse_Until (&YouCanExit0, ND_IS_GREATER, 0, 0, 0);
     
        // Arrivati a questo punto l'utente ha premuto un tasto
     
        ndLP_DestroyWindow (WndHandle0);
        
        switch (YouCanExit0)
        {
               case 1:
               {
                    PassToLevel1 ();
                    break;
               }
        
               case 2:
               {
                   YouCanExit=1;    // Provocherà l'uscita generale dal sistema
                   break;
               }
        }
     }     
}



/*
   -----------------------------------------------------------------------------
   MAIN
   -----------------------------------------------------------------------------
*/


int main ()
{
    ndInitSystem ();
    
    ndEnableStdOutCustomDims (500, 600);
    ndLP_EnableTrasparency ();
    
    YouCanExit=0;
    
    while (!YouCanExit)
    {
          DialogBox0 ();
    }
    
    ndSystemHalt (0);
}
