/*
   ----------------------------------------------------------------------------
   This file is a bridge that allows the execution of ISpell under nd 
   environment
   ----------------------------------------------------------------------------
*/

#include <nanodesktop.h>
#include "config.h"
#include <ispell.h>

int ISpellWindow;
int UseISpellTerminal = 1;
int ISpellTerminalIsActive;
int ISpellTerminalColor;

void ndISpell_DisableTerminal (void)
{
    UseISpellTerminal = 0; 
}

void ndISpell_EnableTerminal (void)
{
    UseISpellTerminal = 1;   
}


void erase ()
{
    if (ISpellTerminalIsActive)
    {
       ndWS_ClrScr (ISpellWindow);
       ndWS_GoCursor (0, 0, ISpellWindow);
    }
}

void move (int row, int col)
{
    if (ISpellTerminalIsActive)
    {
       if ((row>=0) && (col>=0))
       {
          // Sembra che le righe e le colonne siano invertite nella logica di chiamata del programma          
          ndWS_GoCursor (col, row, ISpellWindow);             
       }
    }
}

void inverse ()
{
    ISpellTerminalColor = COLOR_LBLUE;
}

void normal ()
{
    ISpellTerminalColor = COLOR_WHITE;
}

void backup ()
{
}

static int putch (int c)
{
    if (ISpellTerminalIsActive)
    {    
        if (c!=0x0D)
        {
           ndWS_WriteLetter (c, ISpellTerminalColor, ISpellWindow, NORENDER);
        }
        else
        {
           XWindowRender (ISpellWindow);
           ndWS_CarriageReturn (ISpellWindow);
        } 
    }
}

void terminit ()
{
    if (UseISpellTerminal)
    {
        ISpellWindow = ndLP_CreateWindow (3, 3, Windows_MaxScreenX-3, Windows_MaxScreenY-3, "Nanodesktop ISpell Terminal", 
                                          COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_WHITE, NDKEY_SETWSLENGTH (640) | NDKEY_SETWSHEIGHT (200));
        
        if (ISpellWindow>=0)
        {
            ndLP_MaximizeWindow (ISpellWindow);
            ndLP_EnableTrasparency ();
            
            ISpellTerminalColor=COLOR_WHITE;
            ISpellTerminalIsActive=1;
            
            co         = 40;
            li         = 25;
            maxposslen = 14;
            contextsize = MINCONTEXT;
        }
        else
        {
            ISpellTerminalIsActive=0;
        }
    }
}

void stop ()
{
    if (ISpellTerminalIsActive)
    {
       ndHAL_Delay (5.0);

       ndLP_DestroyWindow (ISpellWindow);
       ISpellTerminalIsActive=0;
    }
}

int shellescape	(int buf)
{
    char DialogBox;
    
    if (ISpellTerminalIsActive)
    {    
        DialogBox = ndLP_CreateWindow (Windows_MaxScreenX-140, 0, Windows_MaxScreenX-3, 50, "Message", 
                                              COLOR_WHITE, COLOR_BLUE, COLOR_GRAY, COLOR_GRAY, NO_RESIZEABLE);
            
        if (DialogBox>=0)
        {
            ndLP_MaximizeWindow (DialogBox);
            ndWS_GrWriteLn (5, 10, "Shellescape not", COLOR_BLACK, COLOR_GRAY, DialogBox, NORENDER);
            ndWS_GrWriteLn (5, 18, "supported", COLOR_BLACK, COLOR_GRAY, DialogBox, RENDER);                 
        }
    }                    
}
