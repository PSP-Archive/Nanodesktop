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
    DIALOG-BOX COMUNI AI FILE MANAGER DI TIPO 1 E DI TIPO 2
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void INTERNAL_T12_VoidNameErrorBox (void)
{
  char WndHandle;
  unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;
  unsigned short int B1_Y1, B1_Y2;
  unsigned short int MargineX, MargineY;

  MargineX = ( Windows_MaxScreenX - __LENX_FILEMAN_ERRORBOX_VOID )/2;
  MargineY = ( Windows_MaxScreenY - __LENY_FILEMAN_ERRORBOX_VOID )/2;

  A1_X1 = MargineX;
  A1_Y1 = MargineY;
  A1_X2 = Windows_MaxScreenX-MargineX;
  A1_Y2 = Windows_MaxScreenY-MargineY;

  WndHandle = ndLP_CreateWindow (A1_X1, A1_Y1, A1_X2, A1_Y2, "Error", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);

  if (WndHandle>0)
  {
     ndLP_MaximizeWindow (WndHandle);
     ndWS_CarriageReturn (WndHandle);
     ndWS_WriteLn ("  Filename cannot be void.", COLOR_BLACK, WndHandle, NORENDER);

     B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
     B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;

     ndCTRL_MakeOneButton ("OK", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
     // Consenti all'utente di premere il tasto OK
     
     ndLP_DestroyWindow (WndHandle);
  }
}


void INTERNAL_T12_MkDirErrorBox (char *NameDir)
{
  char WndHandle;
  unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;
  unsigned short int B1_Y1, B1_Y2;
  unsigned short int MargineX, MargineY;

  MargineX = ( Windows_MaxScreenX - __LENX_FILEMAN_ERRORBOX_MKDIR )/2;
  MargineY = ( Windows_MaxScreenY - __LENY_FILEMAN_ERRORBOX_MKDIR )/2;

  A1_X1 = MargineX;
  A1_Y1 = MargineY;
  A1_X2 = Windows_MaxScreenX-MargineX;
  A1_Y2 = Windows_MaxScreenY-MargineY;

  WndHandle = ndLP_CreateWindow (A1_X1, A1_Y1, A1_X2, A1_Y2, "Disk error", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);

  if (WndHandle>0)
  {
     ndLP_MaximizeWindow (WndHandle);

     ndWS_GoCursor (2, 1, WndHandle);
     ndWS_WriteLn ("I cannot create directory:", COLOR_BLACK, WndHandle, NORENDER);

     ndWS_GoCursor (2, 3, WndHandle);
     ndWS_WriteLn (NameDir, COLOR_LBLUE, WndHandle, NORENDER);

     B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
     B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;

     ndCTRL_MakeOneButton ("OK", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
     // Consenti all'utente di premere il tasto OK
     
     ndLP_DestroyWindow (WndHandle);
  }
}



char INTERNAL_T12_ConfirmErase (char *NameElement, char IsDir)
/*
   Questa routine è esclusivamente per uso interno.

   Chiede conferma della cancellazione di un file. Se l'utente dà conferma
   viene restituito in uscita 1, altrimenti viene restituito 0.
*/

{
  char WndHandle;
  
  unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;
  unsigned short int B1_Y1, B1_Y2;

  unsigned short int MargineX, MargineY;
  char UserReport;

  MargineX = ( Windows_MaxScreenX - __LENX_FILEMAN_ERRORBOX_CONFIRMDEL )/2;
  MargineY = ( Windows_MaxScreenY - __LENY_FILEMAN_ERRORBOX_CONFIRMDEL )/2;

  A1_X1 = MargineX;
  A1_Y1 = MargineY;
  A1_X2 = Windows_MaxScreenX-MargineX;
  A1_Y2 = Windows_MaxScreenY-MargineY;

  WndHandle = ndLP_CreateWindow (A1_X1, A1_Y1, A1_X2, A1_Y2, "Confirm deletion", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);

  if (WndHandle>0)
  {
     ndLP_MaximizeWindow (WndHandle);

     if (!(IsDir))
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("You are erasing the file", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }
     else
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("You are erasing the directory", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }

     B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
     B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;

     UserReport=ndCTRL_MakeTwoButton ("Confirm", "Cancel", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
     // Consenti all'utente di confermare o cancellare

     ndLP_DestroyWindow (WndHandle);

     if (UserReport==0)             // L'utente ha scelto Accept
        return 1;                   // Informa che l'utente vuole cancellare
        
     if ((UserReport==1) || (UserReport==DIALOG_BOX_EXIT))
        return 0;                   // Informa che l'utente rinuncia alla cancellazione

  }
  return 0;          // Se si è verificato un'errore dì alla routine chiamante che la cancellazione è stata
                     // annullata per sicurezza
}



void INTERNAL_T12_ErrorOnErase (char *NameElement, char IsDir)
/*
   Questa routine è esclusivamente per uso interno.

   Si occupa di notificare all'utente che c'è stato un
   errore nella cancellazione di un file o di una
   directory.
*/

{
  char WndHandle;
  unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;
  unsigned short int B1_Y1, B1_Y2;
  unsigned short int MargineX, MargineY;

  MargineX = ( Windows_MaxScreenX - __LENX_FILEMAN_ERRORBOX_ERRDEL )/2;
  MargineY = ( Windows_MaxScreenY - __LENY_FILEMAN_ERRORBOX_ERRDEL )/2;

  A1_X1 = MargineX;
  A1_Y1 = MargineY;
  A1_X2 = Windows_MaxScreenX-MargineX;
  A1_Y2 = Windows_MaxScreenY-MargineY;

  WndHandle = ndLP_CreateWindow (A1_X1, A1_Y1, A1_X2, A1_Y2, "Disk error", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);

  if (WndHandle>0)
  {
     ndLP_MaximizeWindow (WndHandle);

     if (!(IsDir))
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("I cannot delete file", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }
     else
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("I cannot delete directory", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }

     B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
     B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;

     ndCTRL_MakeOneButton ("OK", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
     // Consenti all'utente di premere il tasto OK

     ndLP_DestroyWindow (WndHandle);
  }
}



void INTERNAL_T12_SuccessOnErase (char *NameElement, char IsDir)
/*
   Questa routine è esclusivamente per uso interno.

   Si occupa di notificare all'utente che l'operazione
   di cancellazione di un file o di una
   directory ha avuto successo.

*/

{
  char WndHandle;
  unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;
  unsigned short int B1_Y1, B1_Y2;
  unsigned short int MargineX, MargineY;

  MargineX = ( Windows_MaxScreenX - __LENX_FILEMAN_ERRORBOX_ERRDEL )/2;
  MargineY = ( Windows_MaxScreenY - __LENY_FILEMAN_ERRORBOX_ERRDEL )/2;

  A1_X1 = MargineX;
  A1_Y1 = MargineY;
  A1_X2 = Windows_MaxScreenX-MargineX;
  A1_Y2 = Windows_MaxScreenY-MargineY;

  WndHandle = ndLP_CreateWindow (A1_X1, A1_Y1, A1_X2, A1_Y2, "Deletion successful", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);

  if (WndHandle>0)
  {
     ndLP_MaximizeWindow (WndHandle);

     if (!(IsDir))
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("I have deleted the file", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }
     else
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("I have deleted directory", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }

     B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
     B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;

     ndCTRL_MakeOneButton ("OK", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
     // Consenti all'utente di premere il tasto OK

     ndLP_DestroyWindow (WndHandle);
  }
}


char INTERNAL_T12_ConfirmRename (char *OldNameElement, char *NewNameElement, char IsDir)
/*
   Questa routine è esclusivamente per uso interno.

   Chiede conferma prima dell'operazione di rename di un file. Se l'utente dà conferma
   viene restituito in uscita 1, altrimenti viene restituito 0.
*/

{
  char WndHandle;
  
  unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;
  unsigned short int B1_Y1, B1_Y2;
  unsigned short int UserReport;

  unsigned short int MargineX, MargineY;

  MargineX = ( Windows_MaxScreenX - __LENX_FILEMAN_ERRORBOX_CONFIRMREN )/2;
  MargineY = ( Windows_MaxScreenY - __LENY_FILEMAN_ERRORBOX_CONFIRMREN )/2;

  A1_X1 = MargineX;
  A1_Y1 = MargineY;
  A1_X2 = Windows_MaxScreenX-MargineX;
  A1_Y2 = Windows_MaxScreenY-MargineY;

  WndHandle = ndLP_CreateWindow (A1_X1, A1_Y1, A1_X2, A1_Y2, "Confirm rename", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);

  if (WndHandle>0)
  {
     ndLP_MaximizeWindow (WndHandle);

     if (!(IsDir))
     {
        ndWS_CarriageReturn (WndHandle);          
        ndWS_WriteLn ("You are changing the name of file", COLOR_BLACK, WndHandle, NORENDER);
        ndWS_CarriageReturn (WndHandle);

        ndWS_Write ("  ", COLOR_LBLUE, WndHandle, NORENDER);
        ndWS_WriteLn (OldNameElement, COLOR_LBLUE, WndHandle, NORENDER);

        ndWS_CarriageReturn (WndHandle);

        ndWS_WriteLn ("To:", COLOR_BLACK, WndHandle, NORENDER);
        ndWS_CarriageReturn (WndHandle);

        ndWS_Write ("  ", COLOR_LBLUE, WndHandle, NORENDER);
        ndWS_WriteLn (NewNameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }
     else
     {
        ndWS_CarriageReturn (WndHandle); 
        ndWS_WriteLn ("You are changing the name of dir", COLOR_BLACK, WndHandle, NORENDER);
        ndWS_CarriageReturn (WndHandle);

        ndWS_Write ("  ", COLOR_LBLUE, WndHandle, NORENDER);
        ndWS_WriteLn (OldNameElement, COLOR_LBLUE, WndHandle, NORENDER);

        ndWS_CarriageReturn (WndHandle);

        ndWS_WriteLn ("To:", COLOR_BLACK, WndHandle, NORENDER);
        ndWS_CarriageReturn (WndHandle);

        ndWS_Write ("  ", COLOR_LBLUE, WndHandle, NORENDER);
        ndWS_WriteLn (NewNameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }

     B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
     B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;

     UserReport=ndCTRL_MakeTwoButton ("Confirm", "Cancel", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
     // Consenti all'utente di scegliere uno dei due tasti

     ndLP_DestroyWindow (WndHandle);

     if (UserReport==0)             // L'utente ha scelto Accept
        return 1;                   // Informa che l'utente vuole rinominare
        
     if ((UserReport==1) || (UserReport==DIALOG_BOX_EXIT))
        return 0;                   // Informa che l'utente rinuncia al rename
     
     ndLP_DestroyWindow (WndHandle);
  }
  return 0;          // Se si è verificato un'errore dì alla routine chiamante che il rename è stato annullato
}


void INTERNAL_T12_ErrorOnRename (char *NameElement, char IsDir)
/*
   Questa routine è esclusivamente per uso interno.

   Si occupa di notificare all'utente che c'è stato un
   errore nel rename di un file o di una
   directory.
*/

{
  char WndHandle;
  
  unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;
  unsigned short int B1_Y1, B1_Y2;
  
  unsigned short int MargineX, MargineY;

  MargineX = ( Windows_MaxScreenX - __LENX_FILEMAN_ERRORBOX_ERRREN )/2;
  MargineY = ( Windows_MaxScreenY - __LENY_FILEMAN_ERRORBOX_ERRREN )/2;

  A1_X1 = MargineX;
  A1_Y1 = MargineY;
  A1_X2 = Windows_MaxScreenX-MargineX;
  A1_Y2 = Windows_MaxScreenY-MargineY;

  WndHandle = ndLP_CreateWindow (A1_X1, A1_Y1, A1_X2, A1_Y2, "Disk error", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);

  if (WndHandle>0)
  {
     ndLP_MaximizeWindow (WndHandle);

     if (!(IsDir))
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("I cannot rename file", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }
     else
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("I cannot rename directory", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }

     B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
     B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;

     ndCTRL_MakeOneButton ("OK", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
     // Consenti all'utente di premere il tasto OK

     ndLP_DestroyWindow (WndHandle);
  }
}


void INTERNAL_T12_SuccessOnRename (char *NameElement, char IsDir)
/*
   Questa routine è esclusivamente per uso interno.

   Si occupa di notificare all'utente che l'operazione
   di rename di un file o di una
   directory ha avuto successo.

*/

{
  char WndHandle;
  
  unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;
  unsigned short int B1_Y1, B1_Y2;
  
  unsigned short int MargineX, MargineY;

  MargineX = ( Windows_MaxScreenX - __LENX_FILEMAN_ERRORBOX_ERRREN )/2;
  MargineY = ( Windows_MaxScreenY - __LENY_FILEMAN_ERRORBOX_ERRREN )/2;

  A1_X1 = MargineX;
  A1_Y1 = MargineY;
  A1_X2 = Windows_MaxScreenX-MargineX;
  A1_Y2 = Windows_MaxScreenY-MargineY;

  WndHandle = ndLP_CreateWindow (A1_X1, A1_Y1, A1_X2, A1_Y2, "Rename successful", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);

  if (WndHandle>0)
  {
     ndLP_MaximizeWindow (WndHandle);

     if (!(IsDir))
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("I have renamed the file", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }
     else
     {
        ndWS_GoCursor (2, 1, WndHandle);
        ndWS_WriteLn ("I have renamed directory", COLOR_BLACK, WndHandle, NORENDER);

        ndWS_GoCursor (2, 3, WndHandle);
        ndWS_WriteLn (NameElement, COLOR_LBLUE, WndHandle, NORENDER);
     }

     B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
     B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;

     ndCTRL_MakeOneButton ("OK", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
     // Consenti all'utente di premere il tasto OK
     
     ndLP_DestroyWindow (WndHandle);
  }
}
