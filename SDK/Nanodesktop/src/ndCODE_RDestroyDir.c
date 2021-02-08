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
    RECURSIVE DESTROY DIRECTORY
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

char INTERNAL_T12_RecursiveDestroyDir (char *ShortNameDir, char NoGenWnd, char EredWndHandle)
/*
   Questa routine è concepita esclusivamente per uso interno.

   Su molte piattaforme, la normale routine di cancellazione delle directory è
   in grado di operare esclusivamente qualora la directory bersaglio sia
   vuota.

   La RecursiveDestroyDir distrugge tutti i file in una directory e poi agisce
   ricorsivamente sulle sottodirectory fino alla completa cancellazione della
   directory radice.

   Il parametro NoGenWnd viene posto a 0 solo alla prima chiamata, per informare
   la routine che deve creare una finestra in cui riversare le informazioni
   di tipo diagnostico. Quando la routine richiama sè stessa per cancellare
   le sottodirectory, il parametro GenWnd viene posto a 1, ed il parametro
   WndHandle informa le successive istanze della procedura del nr. di
   identificatore della finestra diagnostica.

   Se l'operazione di cancellazione ha avuto successo, la routine restituisce
   0 altrimenti restituisce -1.
*/

{
 struct HEADLIST_File_Type  LocalFileList;
 struct ELEMENT_File_Type   SingleFile;

 char InternalBuffer1 [255];
 char InternalBuffer2 [255];

 int Counter;

 char WndHandle, YouCanProceed;
 char ErrorOnExit, ErrRep1, ErrRep2; 

 unsigned short int aX1, aX2, aY1, aY2;

 YouCanProceed=0;

 if (NoGenWnd==0)
 {
   aX1=30;
   aY1=30;
   aX2=Windows_MaxScreenX-30;
   aY2=Windows_MaxScreenY-30;

   WndHandle = ndLP_CreateWindow (aX1, aY1, aX2, aY2, "Deleting in progress...", COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_WHITE, 0);

   if (WndHandle>=0)       // E' riuscito ad allocare una nuova finestra
   {
      ndLP_MaximizeWindow (WndHandle);
      YouCanProceed=1;
   }
 }
 else
 {
   WndHandle=EredWndHandle;
   YouCanProceed=1;
 }

 if (YouCanProceed)
 {
   ErrorOnExit=0;

   strcpy (InternalBuffer1, ShortNameDir);
   // Copia ShortNameDir in InternalBuffer1

   fssPutSlashAtEnd (InternalBuffer1);        // Se l'ultimo carattere non è uno slash lo aggiunge

   ndDB_InitListOfFile (&LocalFileList);
   // Reinizializza la lista

   ErrRep1=ndDB_FillFileList (InternalBuffer1, &LocalFileList);
   // Riempe il buffer FileList locale con i nomi dei file.

   if (ErrRep1==0)               // Non ci sono stati problemi con il riempimento della lista locale
   {
      ndWS_WriteLn ("Attempt to access to directory", COLOR_GREEN, WndHandle, RENDER);
      ndWS_WriteLn (InternalBuffer1, COLOR_GREEN, WndHandle, RENDER);
      ndWS_CarriageReturn (WndHandle);

      if (LocalFileList.NrElements!=0)     // Se la lista file non è vuota
      {
         for (Counter=0; Counter<=LocalFileList.NrElements-1; Counter++)
         {
             ndDB_GetFileFromList (&LocalFileList, Counter, &SingleFile);
             // Estrae un elemento dal buffer

             switch (SingleFile.IsDir)
             {
                 case 0:               // Siamo in presenza di un file
                 {
                      strcpy (InternalBuffer2, ShortNameDir);
                      // Copia il nome della directory in InternalBuffer2;

                      fssPutSlashAtEnd (InternalBuffer2);
                      strcat (InternalBuffer2, SingleFile.Name);          // Esegue l'append dentro InternalString del nome file
                      fssAppendExtension (InternalBuffer2, SingleFile.Ext);  // Esegue l'append dell'estensione

                      // In InternalBuffer2 ora c'è il nome completo del file da cancellare

                      ndWS_Write ("Deleting.... ", COLOR_LBLUE, WndHandle, RENDER);
                      ndWS_WriteLn (InternalBuffer2, COLOR_WHITE, WndHandle, RENDER);

                      ErrRep2 = ndHAL_EraseFile (InternalBuffer2);
                      // Cancella il file. Codice di errore del kernel in IOErrRep

                      if (ErrRep2==0)     // Non ci sono stati errori
                      {
                         ndWS_GoCursor (WindowData [WndHandle].WS_MaxCursX - 10, WindowData [WndHandle].WS_PosCursY, WndHandle);
                         ndWS_WriteLn ("Deleted", COLOR_GREEN, WndHandle, RENDER);
                      }
                      else
                      {
                         ndWS_GoCursor (WindowData [WndHandle].WS_MaxCursX - 10, WindowData [WndHandle].WS_PosCursY, WndHandle);
                         ndWS_WriteLn ("Failed", COLOR_RED, WndHandle, RENDER);
                         ErrorOnExit=-1;      // Segnala l'errore in uscita per le successive elaborazioni
                      }

                      break;
                  }

                  case 1:         // Siamo in presenza di una directory
                  {
                      strcpy (InternalBuffer2, ShortNameDir);
                      fssPutSlashAtEnd (InternalBuffer2);                 // Aggiunge uno slash alla fine

                      strcat (InternalBuffer2, SingleFile.Name);
                      // Aggiunge il nome che era stato trovato per la directory

                      fssAppendExtension (InternalBuffer2, SingleFile.Ext);
                      // Appende l'estensione

                      // Secondo controllo....
                      fssPutSlashAtEnd (InternalBuffer2);

                      // Ora la routine richiama sè stessa, allo scopo di cancellare le sotto-directory

                      ndWS_WriteLn ("Access to directory: ", COLOR_GREEN, WndHandle, RENDER);
                      ndWS_WriteLn (InternalBuffer2, COLOR_GREEN, WndHandle, RENDER);
                      ndWS_CarriageReturn (WndHandle);

                      ErrRep2=INTERNAL_T12_RecursiveDestroyDir (InternalBuffer2, 1, WndHandle);

                      if (ErrRep2!=0)         // Ci sono stati errori
                         ErrorOnExit=-1;      // Segnala l'errore in uscita per le successive elaborazioni
                   }

              }    // End of switch
         }       // End del ciclo For
      }          // End dell'if NrElements!=0

      // Alla fine del ciclo for la directory attuale dovrebbe essere vuota. Ora è possibile
      // cancellarla.

      ndWS_CarriageReturn (WndHandle);

      ndWS_WriteLn ("Attempt to delete void directory:", COLOR_GREEN, WndHandle, RENDER);
      ndWS_WriteLn (InternalBuffer1, COLOR_GREEN, WndHandle, RENDER);

      ErrRep2=ndHAL_RemoveDir (InternalBuffer1);
      // Cancella la directory

      if (ErrRep2==0)
      {
        ndWS_GoCursor (WindowData [WndHandle].WS_MaxCursX - 10, WindowData [WndHandle].WS_PosCursY, WndHandle);
        ndWS_WriteLn ("Success", COLOR_YELLOW, WndHandle, RENDER);
        ndWS_CarriageReturn (WndHandle);
      }
      else
      {
        ndWS_GoCursor (WindowData [WndHandle].WS_MaxCursX - 10, WindowData [WndHandle].WS_PosCursY, WndHandle);
        ndWS_WriteLn ("Failed", COLOR_RED, WndHandle, RENDER);
        ndWS_CarriageReturn (WndHandle);

        ErrorOnExit=-1;       // Segnala l'errore in uscita per le successive elaborazioni
      }

      ndDB_DestroyListOfFile (&LocalFileList);          // Disalloca la lista locale
   }                                              // End del if ErrRep1==0;

   if (NoGenWnd==0)
   {
      ndLP_DestroyWindow (WndHandle);
   }

   return ErrorOnExit;         // Ritorni alle autoistanze inferiori della routine il codice di errore

 }       // End di if YouCanProceed
 else   // Non è stato possibile iniziare perchè c'è stato un problema nell'allocazione di una nuova finestra
 {
    return -1;   // Codice di errore -1
 }

}

