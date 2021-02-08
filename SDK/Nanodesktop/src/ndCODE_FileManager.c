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
    FILE MANAGER (ROUTINE SWITCH)
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

char FileManager (char *Title, unsigned char TypeManager, unsigned char SaveMode, char *StringOutputAddr)
/*
     Provvede a selezionare un filemanager. La string Title è il titolo che viene posto
     nella finestra del file manager. Il parametro TypeManager seleziona il tipo di
     file manager desiderato. 
     
     Il parametro SaveMode viene posto ad 1 se si vuole che il file manager sia in
     modalità salvataggio
     
     Se l'utente sceglie effettivamente un file, la routine restituisce 0 in uscita
     ed il nome viene memorizzato nella stringa *StringOutputAddr. 
     
     Se invece l'utente interrompe il file manager senza scegliere alcun file,
     la routine restituisce in uscita il codice di errore 
     ERR_OPERATION_INTERRUPTED.
     
     Sono previsti i seguenti codici di errore:
       ERR_ACCESS_SYSTEM_PATH        Non riesce ad accedere alla path di sistema
       ERR_OPERATION_INTERRUPTED     L'utente ha interrotto l'operazione
       ERR_WRONG_PARAMETERS          Il parametro TypeManager non corrisponde ad
                                     un FileManager supportato
*/
{
  switch (TypeManager)
  {
         case 0:
              {
                   return INTERNAL_T1FileManager (Title, SaveMode, StringOutputAddr);   
              }
              
         case 1:
              {
                   return INTERNAL_T2FileManager (Title, SaveMode, StringOutputAddr);
              }
              
         default:
              {
                   return ERR_WRONG_PARAMETERS; 
              }
  } 
}
