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
    ROUTINE DI TEST PER I NOMI
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

char INTERNAL_TEST_NameVoid (char *Name)
// Verifica che il nome non sia una stringa vuota
{
   if ((strcmp (Name, ""))==0)   // Se sono uguali
   {
      INTERNAL_T12_VoidNameErrorBox ();
      return -1;
   }
   else
      return 0;
}

char INTERNAL_TEST_TestName (char *Name)
/*
   Questa routine è esclusivamente per uso interno.

   Essa si occupa di testare il nome generato dal New Name Manager in modo da
   verificare la validità sulla base delle regole del FileSystem utilizzato.

   A tale scopo, TestName esegue in successione una serie di test.

   Quando uno dei test fallisce, esso provvede anche a mostrare sullo schermo
   la relativa dialog box.

   Se tutti i test riescono correttamente, fornisce in uscita 0, altrimenti
   fornisce in uscita -1.
*/

{
   if ( INTERNAL_TEST_NameVoid (Name)==-1) return -1;

   return 0;    // Se non ci sono errori
}
