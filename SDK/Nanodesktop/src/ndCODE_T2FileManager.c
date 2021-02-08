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
    FILE MANAGER DI TIPO 2
    ----------------------------------------------------------------------------
    Le seguenti routine riguardano il file manager di tipo 2
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>




void INTERNAL_T2_SetFMWindowParams (struct T2FMData_Type *MyFMData)
/*
     Questa routine è esclusivamente per uso interno.

     Si occupa di eseguire dei calcoli per determinare le variabili interne della finestra
     del File Manager.

     Poichè sono consentite diverse istanze del File Manager, ogni istanza alloca nel proprio
     stack una struct locale di tipo T2FMData_Type atta a contenere i dati relativi alla
     finestra del FileManager.

     L'indirizzo di tale struct viene passato, sotto forma di parametro, a tutte le routine
     che sono richiamate da FileManager.
*/
{
 unsigned short int w1;

 w1=MyFMData->WndHandle1;

 MyFMData->B1_X1=10;
 MyFMData->B1_Y1=10;
 MyFMData->B1_X2=WindowData [w1].WSWndLength-10;
 MyFMData->B1_Y2=WindowData [w1].WSWndHeight-10;
 // In questo modo in B1 sono disegnati i vari elementi

 MyFMData->B2_X1 = MyFMData->B1_X1+16+4;
 MyFMData->B2_X2 = MyFMData->B1_X2;
 MyFMData->B2_Y1 = MyFMData->B1_Y1;
 MyFMData->B2_Y2 = MyFMData->B1_Y1+14;
 // Coordinate della barra per i path

 MyFMData->B3_X1=MyFMData->B1_X1+16+4;
 MyFMData->B3_X2=MyFMData->B1_X2;
 MyFMData->B3_Y1=MyFMData->B2_Y2+2;
 MyFMData->B3_Y2=MyFMData->B1_Y2-20-5;
 // In B3 c'è l'area attiva per i file (livello 1)

 MyFMData->VCharX1 = ((MyFMData->B3_X1 - WindowData [w1].MargineCharX)/8)+1;
 MyFMData->VCharY1 = ((MyFMData->B3_Y1 - WindowData [w1].MargineCharY)/8)+3;
 MyFMData->VCharX2 = ((MyFMData->B3_X2 - WindowData [w1].MargineCharX)/8)-1;
 MyFMData->VCharY2 = ((MyFMData->B3_Y2 - WindowData [w1].MargineCharY)/8)-3;

 MyFMData->VLenCharX = MyFMData->VCharX2 - MyFMData->VCharX1 + 1;
 MyFMData->VLenCharY = MyFMData->VCharY2 - MyFMData->VCharY1 + 1;

 MyFMData->MaxLenPathAllowed = MyFMData->VLenCharX - 1;

 MyFMData->Tab4 = MyFMData->VCharX2 - 11;
 MyFMData->Tab3 = MyFMData->Tab4 - 9;
 MyFMData->Tab2 = MyFMData->Tab3 - 4;
 MyFMData->Tab1 = MyFMData->VCharX1;

 MyFMData->MaxLenNmFile = MyFMData->Tab2 - 2;

 MyFMData->PrevStatusChDirButton=-1;

 MyFMData->PrevStatusArrowUp   = 0;
 MyFMData->PrevStatusArrowDown = 0;
}



void INTERNAL_T2_CreateShortPath (struct T2FMData_Type *MyFMData, char *SysPathAddr, char *NewSysPathAddr)
/*
   Questa routine esegue una conversione del path di sistema in un formato che gli consenta di entrare
   all'interno della barra dei path.

   Per esempio, se un path del tipo:
       ms0:/Directory1/Directory2/Directory3/Directory4

   fosse troppo lungo, esso verrebbe convertito in:

       ms0:/../Directory2/Directory3/Directory4

   Le directory di livello inferiore vengono troncate fin quando non si raggiunge una dimensione compatibile
   con le dimensioni della path di sistema.

   La routine chiamante deve passare a CreateShortPath:

   a) L'indirizzo di una struttura FMData
   b) L'indirizzo della stringa che contiene il path di sistema
   c) L'indirizzo del buffer di 256 caratteri dove depositare la nuova stringa che esprime il path corto
*/
{
  char SysPathLength, SysRootLength;
  char IpotLength;
  char Carattere;
  char CounterA, CounterB;
  char YouCanExit, FoundCrSlash, PosCrSlash;

  SysPathLength = strlen (SysPathAddr);
  SysRootLength = strlen (SysRootPath);

  YouCanExit=0;
  CounterA=0;

  while (!(YouCanExit))
  {
     Carattere = *(SysPathAddr+CounterA);

     if (Carattere=='/')
     {
        IpotLength = ((SysPathLength - CounterA + 1) + (SysRootLength) + 3 );
        // E' la lunghezza della nuova stringa se tagliamo al carattere successivo allo slash

        if (IpotLength < MyFMData->MaxLenPathAllowed)
        {
           FoundCrSlash=1;
           PosCrSlash=CounterA;
           break;
        }
     }

     CounterA++;

     if (CounterA>=SysPathLength)
     {
        FoundCrSlash=0;
        YouCanExit=1;
     }

  }

  if (FoundCrSlash)
  {
     strcpy (NewSysPathAddr, SysRootPath);
     // Copia la root path all'inizio dell'indirizzo

     CounterB=SysRootLength;
     // Ora in CounterB c'è il carattere NULL alla fine di SysRootPath

     *(NewSysPathAddr+CounterB)='.'; CounterB++;    // Il carattere NULL viene sovrascritto
     *(NewSysPathAddr+CounterB)='.'; CounterB++;
     *(NewSysPathAddr+CounterB)='/'; CounterB++;

     CounterA=PosCrSlash+1;

     while (CounterA<SysPathLength)
     {
        *(NewSysPathAddr+CounterB) = *(SysPathAddr+CounterA);
        CounterA++;
        CounterB++;
     }

     *(NewSysPathAddr+CounterB)=0;
     // Terminatore stringa

   }
   else       // Non è stato trovato uno slash in grado di ridurre le dimensioni virtuali
   {
     strcpy (NewSysPathAddr, SysRootPath);
     // Copia la root path all'inizio dell'indirizzo

     CounterB=SysRootLength;
     // Ora in CounterB c'è il carattere NULL alla fine di SysRootPath

     *(NewSysPathAddr+CounterB)='.'; CounterB++;
     *(NewSysPathAddr+CounterB)='.'; CounterB++;
     *(NewSysPathAddr+CounterB)='.'; CounterB++;
     *(NewSysPathAddr+CounterB)='.'; CounterB++;

     CounterA=SysPathLength - MyFMData->MaxLenPathAllowed + (SysRootLength) + 4;

     while (CounterA<SysPathLength)
     {
         *(NewSysPathAddr+CounterB) = *(SysPathAddr+CounterA);
         CounterA++;
         CounterB++;
     }

     *(NewSysPathAddr+CounterB)=0;
     // Terminatore stringa
   }
}





void INTERNAL_T2_DrawSysPathBarVoid (struct T2FMData_Type *MyFMData)
/*
   Questa routine è esclusivamente per uso interno.

   Essa provvede a disegnare la barra per la path di sistema.
*/
{
  char XWndHandle;

  XWndHandle = MyFMData->WndHandle1;

  // Disegna la barra del path
  ndWS_DrawRectangle (MyFMData->B2_X1, MyFMData->B2_Y1, MyFMData->B2_X2, MyFMData->B2_Y2,
                                COLOR_GRAY, COLOR_WHITE, XWndHandle, NORENDER);
}





void INTERNAL_T2_DrawSysPathBar (struct T2FMData_Type *MyFMData)
/* Questa routine è esclusivamente per uso interno.

   Essa provvede a disegnare la barra per la path di sistema e
   scrive il path attuale.

   Se il path attuale è troppo lungo per entrare nella barra, viene
   richiamata la routine CreateShortPath che esegue un opportuno algoritmo
   per creare un indirizzo più corto.
*/
{
  char XWndHandle, SysPathLength;
  char NewSysPath [256];

  XWndHandle = MyFMData->WndHandle1;

  // Disegna la barra del path
  ndWS_DrawRectangle (MyFMData->B2_X1, MyFMData->B2_Y1, MyFMData->B2_X2, MyFMData->B2_Y2, COLOR_GRAY, COLOR_WHITE, XWndHandle, NORENDER);
  SysPathLength = strlen (ndFileManCurrPath);

  if (SysPathLength<=MyFMData->MaxLenPathAllowed)        // il percorso non è troppo lungo per la barra
  {
     ndWS_GrWriteLn (MyFMData->B2_X1 + 4, MyFMData->B2_Y1 + 4, ndFileManCurrPath, COLOR_WHITE, COLOR_GRAY, XWndHandle, NORENDER);
  }
  else                                                   // Se il percorso è troppo lungo, applica algoritmo
  {
     INTERNAL_T2_CreateShortPath (MyFMData, &ndFileManCurrPath, &NewSysPath);
     ndWS_GrWriteLn (MyFMData->B2_X1 + 4, MyFMData->B2_Y1 + 4, NewSysPath, COLOR_WHITE, COLOR_GRAY, XWndHandle, NORENDER);
  }

}







void INTERNAL_T2_DrawMainButton (struct T2FMData_Type *MyFMData, char ChangeDirMode)
/*
  Questa routine è esclusivamente per uso interno.

  Provvede a disegnare sullo schermo il pulsante principale (Load o Save). Il pulsante
  che viene realmente visualizzato dipende dal valore del parametro SaveMode nel
  buffer MyFMData.

  Se ChangeDir è posto ad 1, allora la routine disegna il pulsante ChangeDir.
*/
{
   if (MyFMData->PrevStatusChDirButton == -1)         // E' la prima volta che si esegue la routine
   {
        MyFMData->PrevStatusChDirButton = ChangeDirMode;                            
                                       
        if (ChangeDirMode == 0)
        {
            if ( (MyFMData->SaveMode) == 0 )
            {
               MyFMData->ButtonHandle_MainButton = ndCTRL_CreateButton (MyFMData->B1_X2-110, MyFMData->B1_Y2-20,
                                                                        MyFMData->B1_X2,     MyFMData->B1_Y2,
                                                                        MyFMData->WndHandle1,
                                                                        "#MainButton", "Load", "", 
                                                                        COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                                        ND_BUTTON_ROUNDED, 0, 0, NORENDER);
        
          
            }

            if ( (MyFMData->SaveMode) == 1 )
            {
               MyFMData->ButtonHandle_MainButton = ndCTRL_CreateButton (MyFMData->B1_X2-110, MyFMData->B1_Y2-20,
                                                                        MyFMData->B1_X2,     MyFMData->B1_Y2,
                                                                        MyFMData->WndHandle1,
                                                                        "#MainButton", "Save", "", 
                                                                        COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                                        ND_BUTTON_ROUNDED, 0, 0, NORENDER);
            }
        }
        else    // ChangeDirMode è attivato
        {
             MyFMData->ButtonHandle_MainButton = ndCTRL_CreateButton (MyFMData->B1_X2-110, MyFMData->B1_Y2-20,
                                                                      MyFMData->B1_X2,    MyFMData->B1_Y2,
                                                                      MyFMData->WndHandle1,
                                                                      "#MainButton", "Change Dir", "", 
                                                                      COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                                      ND_BUTTON_ROUNDED, 0, 0, NORENDER);
        }   
   }  
   else     // Non è la prima volta che si esegue la routine: si tratta solo di modificare parametri del pulsante
   {                       
      
      if ( (ChangeDirMode == 0) && (MyFMData->PrevStatusChDirButton == 1) )
      {
        MyFMData->PrevStatusChDirButton = ChangeDirMode;

        if ( (MyFMData->SaveMode) == 0 )
        {
           ndCTRL_ChangeButtonParameters (MyFMData->ButtonHandle_MainButton, MyFMData->WndHandle1,
                                          "#MainButton", "Load", "",
                                          COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                          ND_BUTTON_ROUNDED, NORENDER);
        }

        if ( (MyFMData->SaveMode) == 1 )
        {
           ndCTRL_ChangeButtonParameters (MyFMData->ButtonHandle_MainButton, MyFMData->WndHandle1,
                                          "#MainButton", "Save", "",
                                          COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                          ND_BUTTON_ROUNDED, NORENDER);
        }
      }

      if  ( (ChangeDirMode == 1) && (MyFMData->PrevStatusChDirButton == 0) )
      {
         MyFMData->PrevStatusChDirButton   = ChangeDirMode;
         
         ndCTRL_ChangeButtonParameters (MyFMData->ButtonHandle_MainButton, MyFMData->WndHandle1,
                                        "#MainButton", "Change Dir", "",
                                        COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                        ND_BUTTON_ROUNDED, NORENDER);
      } 
   }
   
}






void INTERNAL_T2_DrawSecondaryButton (struct T2FMData_Type *MyFMData, char SaveMode)
/*
   Questa routine è esclusivamente per uso interno.

   Provvede a disegnare i pulsanti New, MkDir e Del quando ci si trova
   con il FileManager in SaveMode
*/

{
    MyFMData->ButtonHandle_New      = ndCTRL_CreateButton (MyFMData->B1_X2-115-60, MyFMData->B1_Y2-20,
                                                           MyFMData->B1_X2-115,    MyFMData->B1_Y2,
                                                           MyFMData->WndHandle1,
                                                           "#New", "New", "", 
                                                           COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                           ND_BUTTON_ROUNDED, 0, 0, NORENDER);

    MyFMData->ButtonHandle_MkDir    = ndCTRL_CreateButton (MyFMData->B1_X2-115-65-60, MyFMData->B1_Y2-20,
                                                           MyFMData->B1_X2-115-65,    MyFMData->B1_Y2,
                                                           MyFMData->WndHandle1,
                                                           "#MkDir", "MkDir", "", 
                                                           COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                           ND_BUTTON_ROUNDED, 0, 0, NORENDER);

    MyFMData->ButtonHandle_Delete   = ndCTRL_CreateButton (MyFMData->B1_X2-115-65-65-60, MyFMData->B1_Y2-20,
                                                           MyFMData->B1_X2-115-65-65,    MyFMData->B1_Y2,
                                                           MyFMData->WndHandle1,
                                                           "#Del", "Del", "", 
                                                           COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                           ND_BUTTON_ROUNDED, 0, 0, NORENDER);

    MyFMData->ButtonHandle_Rename   = ndCTRL_CreateButton (MyFMData->B1_X2-115-65-65-65-60, MyFMData->B1_Y2-20,
                                                           MyFMData->B1_X2-115-65-65-65,    MyFMData->B1_Y2,
                                                           MyFMData->WndHandle1,
                                                           "#Rename", "Rename", "", 
                                                           COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                           ND_BUTTON_ROUNDED, 0, 0, NORENDER);

}




void INTERNAL_T2_DrawButton (struct T2FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Essa disegna i pulsanti principali di un file manager di tipo 2.
*/

{
 if (! (MyFMData->SaveMode) )
 {
    INTERNAL_T2_DrawMainButton (MyFMData, 0);

 }
 else
 {
    INTERNAL_T2_DrawMainButton (MyFMData, 0);
    INTERNAL_T2_DrawSecondaryButton (MyFMData, 0);
 }
}





void INTERNAL_T2_DrawActiveArea (struct T2FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Provvede a disegnare l'area attiva sullo schermo.
*/
{
  ndWS_DrawRectangle (MyFMData->B3_X1, MyFMData->B3_Y1, MyFMData->B3_X2, MyFMData->B3_Y2,
                           COLOR_BLACK, COLOR_LBLUE, MyFMData->WndHandle1, NORENDER);

}





void INTERNAL_T2_DrawButtonForFileScrollUp (struct T2FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Essa disegna il pulsante per lo scroll dei file verso l'alto nei file
  manager di tipo 2.
*/
{
 int CounterX, CounterY;

 MyFMData->ButtonHandle_ScrollFileUp   = ndCTRL_CreateButton (MyFMData->B3_X1-4-16, MyFMData->B3_Y2-42-24,
                                                              MyFMData->B3_X1-4,    MyFMData->B3_Y2-42,
                                                              MyFMData->WndHandle1,
                                                              "#ScrollUp", "", "", 
                                                              COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                              ND_BUTTON_ROUNDED, 0, 0, NORENDER);

 for (CounterY=0; CounterY<8; CounterY++)
 {
    for (CounterX=0; CounterX<8; CounterX++)
    {
        if (Button_ScrollUp [CounterY][CounterX] == 'x')
            ndWS_PutPixel ( (MyFMData->B3_X1-4-16) + 4 + CounterX, (MyFMData->B3_Y2-42-24) + 8 + CounterY,
                                  COLOR_BLACK, MyFMData->WndHandle1, NORENDER);
    }
 }
}





void INTERNAL_T2_DrawButtonForFileScrollDown (struct T2FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Essa disegna il pulsante per lo scroll dei file verso il basso nei file
  manager di tipo 2.
*/
{
 int CounterX, CounterY;

 MyFMData->ButtonHandle_ScrollFileDown = ndCTRL_CreateButton (MyFMData->B3_X1-4-16, MyFMData->B3_Y2-38,
                                                              MyFMData->B3_X1-4,    MyFMData->B3_Y2-14,
                                                              MyFMData->WndHandle1,
                                                              "#ScrollDown", "", "", 
                                                              COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                              ND_BUTTON_ROUNDED, 0, 0, NORENDER);

 for (CounterY=0; CounterY<8; CounterY++)
 {
    for (CounterX=0; CounterX<8; CounterX++)
    {
        if (Button_ScrollDown [CounterY][CounterX] == 'x')
            ndWS_PutPixel ( (MyFMData->B3_X1-4-16) + 4 + CounterX, (MyFMData->B3_Y2-38) + 8 + CounterY,
                                   COLOR_BLACK, MyFMData->WndHandle1, NORENDER);
    }
 }
}


void INTERNAL_T2_DrawButtonForChDirPointPoint (struct T2FMData_Type *MyFMData)
/*
     Questa routine è esclusivamente per uso interno.
     
     Provvede a disegnare il pulsante per eseguire il chdir..
*/
{
    struct ndImage_Type IconImage; 
     
    ndIMG_LoadImageFromNDFArray (&IconImage, FolderDown_Width, FolderDown_Height, &FolderDown, NDMGKNB);
    
    MyFMData->ButtonHandle_ChDirPointPoint = ndCTRL_CreateButtonComplex (MyFMData->B3_X1-4-16, MyFMData->B3_Y1+10,
                                                              MyFMData->B3_X1-4,    MyFMData->B3_Y1+28+10,
                                                              MyFMData->WndHandle1, "#ChDirPP", "", "", 
                                                              COLOR_WHITE, COLOR_GRAY, COLOR_WHITE,
                                                              0, 0, 0, 0, &IconImage, 2, 8, 15, 20,
                                                              ND_BUTTON_ROUNDED, 0, 0, NORENDER); 
    
}

void INTERNAL_T2_UpdateChDirPointPointButton (struct T2FMData_Type *MyFMData)
/* 
    Il compito di questa routine è evidenziare o meno il tasto per il chdir .. a seconda
    che questo abbia o meno senso nel contesto in cui si opera
*/
{
    if (strcmp (ndFileManCurrPath, SysRootPath)!=0)
      ndCTRL_DeInhibitButton (MyFMData->ButtonHandle_ChDirPointPoint, MyFMData->WndHandle1, RENDER);
    else
      ndCTRL_InhibitButton (MyFMData->ButtonHandle_ChDirPointPoint, MyFMData->WndHandle1, RENDER);
        
}



char INTERNAL_T2_DrawFMWindows (struct T2FMData_Type *MyFMData, char *Title)
/*
   Questa routine provvede a disegnare la finestra del file manager di tipo 2 sullo
   schermo. Se si verifica un errore viene fornito in uscita un codice di errore
   pari a -1, altrimenti viene fornito 0
*/

{
  MyFMData->WndHandle1=ndLP_CreateWindow (10, 10, Windows_MaxScreenX-10, Windows_MaxScreenY-10,
                                                 Title, COLOR_WHITE, COLOR_LBLUE, COLOR_BLUE, COLOR_WHITE, 0);

  if ( (MyFMData->WndHandle1)>=0 )            // Non si sono verificati errori durante l'allocazione
  {

     INTERNAL_T2_SetFMWindowParams (MyFMData);
     ndLP_MaximizeWindow (MyFMData->WndHandle1);

     INTERNAL_T2_DrawSysPathBarVoid (MyFMData);
     // Disegna la barra per il path lasciandola vuota

     INTERNAL_T2_DrawActiveArea (MyFMData);
     // Disegna contorno area attiva

     INTERNAL_T2_DrawButton (MyFMData);
     // Disegna i tasti principali

     INTERNAL_T2_DrawButtonForFileScrollUp (MyFMData);
     // Disegna il pulsante per lo scrolling in alto

     INTERNAL_T2_DrawButtonForFileScrollDown (MyFMData);
     // Disegna il pulsante per lo scrolling in basso
     
     INTERNAL_T2_DrawButtonForChDirPointPoint (MyFMData);
     // Disegna il pulsante per il chdir..
     
     XWindowRender (MyFMData->WndHandle1);

     return 0;
  }
  else
      return -1;
}





void INTERNAL_T2_ClrMemoryChar (struct T2FMData_Type *MyFMData)
/*
   Questa routine è esclusivamente per uso interno.

   Provvede a cancellare la memoria caratteri nell'area attiva del file
   manager.
*/

{
  char XWndHandle;
  char CounterA, CounterB;

  XWndHandle=MyFMData->WndHandle1;

  for (CounterA=MyFMData->VCharY1;  CounterA<=MyFMData->VCharY2;  CounterA++)
  {
      for (CounterB=MyFMData->VCharX1;  CounterB<=MyFMData->VCharX2;  CounterB++)
      {
          WINDOWDATA_CHARMAP (XWndHandle, CounterB, CounterA)=0;
      }
  }
}





void INTERNAL_T2_DrawArrowUpInListFile (struct T2FMData_Type *MyFMData, char ArrowUp)
/*
   Questa routine è esclusivamente per uso interno: si occupa di disegnare sullo schermo
   la freccia in alto nella lista dei file.

   La freccia viene effettivamente disegnata solo se è necessario. A tale scopo, la
   routine confronta il valore di ArrowUp con il precedente stato della freccia
   memorizzato in MyFMData->PrevStatusArrowUp.
*/

{
  unsigned short int RRPosX1, RRPosY1;
  unsigned short int CounterX, CounterY;
  char XWndHandle;

  XWndHandle=MyFMData->WndHandle1;

  if ((ArrowUp) && !(MyFMData->PrevStatusArrowUp))
  {
     RRPosX1=MyFMData->B3_X1+4;
     RRPosY1=MyFMData->B3_Y1+4;

     for (CounterY=0; CounterY<8; CounterY++)
     {
        for (CounterX=0; CounterX<8; CounterX++)
        {
            if (Button_ScrollUp [CounterY][CounterX] == 'x')
               ndWS_PutPixel (RRPosX1+CounterX+1, RRPosY1+CounterY+1, COLOR_WHITE, XWndHandle, NORENDER);
        }
     }
  };

  if (!(ArrowUp) && (MyFMData->PrevStatusArrowUp))
  {
      // Se non è più necessario visualizzare la freccia in alto, è comunque necessario controllare
      // se la freccia è attualmente disegnata e cancellarla.

     RRPosX1=MyFMData->B3_X1+4;
     RRPosY1=MyFMData->B3_Y1+4;

     for (CounterY=0; CounterY<8; CounterY++)
     {
        for (CounterX=0; CounterX<8; CounterX++)
        {
            if (Button_ScrollUp [CounterY][CounterX] == 'x')
               ndWS_PutPixel (RRPosX1+CounterX+1, RRPosY1+CounterY+1, COLOR_BLACK, XWndHandle, NORENDER);
        }
     }
  }

  MyFMData->PrevStatusArrowUp = ArrowUp;               // Salva il vecchio stato

}





void INTERNAL_T2_DrawArrowDownInListFile (struct T2FMData_Type *MyFMData, char ArrowDown)
/*
   Questa routine è esclusivamente per uso interno: si occDowna di disegnare sullo schermo
   la freccia in basso nella lista dei file.

   La freccia viene effettivamente disegnata solo se è necessario. A tale scopo, la
   routine confronta il valore di ArrowDown con il precedente stato della freccia
   memorizzato in MyFMData->PrevStatusArrowDown.
*/

{
  unsigned short int RRPosX1, RRPosY1;
  unsigned short int CounterX, CounterY;
  char XWndHandle;

  XWndHandle=MyFMData->WndHandle1;

  if ((ArrowDown) && !(MyFMData->PrevStatusArrowDown))
  {
     RRPosX1=MyFMData->B3_X1+4;
     RRPosY1=MyFMData->B3_Y2-12;

     for (CounterY=0; CounterY<8; CounterY++)
     {
        for (CounterX=0; CounterX<8; CounterX++)
        {
            if (Button_ScrollDown [CounterY][CounterX] == 'x')
               ndWS_PutPixel (RRPosX1+CounterX+1, RRPosY1+CounterY+1, COLOR_WHITE, XWndHandle, NORENDER);
        }
     }
  };

  if (!(ArrowDown) && (MyFMData->PrevStatusArrowDown))
  {
      // Se non è più necessario visualizzare la freccia in basso, è comunque necessario controllare
      // se la freccia è attualmente disegnata e cancellarla.

     RRPosX1=MyFMData->B3_X1+4;
     RRPosY1=MyFMData->B3_Y2-12;

     for (CounterY=0; CounterY<8; CounterY++)
     {
        for (CounterX=0; CounterX<8; CounterX++)
        {
            if (Button_ScrollDown [CounterY][CounterX] == 'x')
               ndWS_PutPixel (RRPosX1+CounterX+1, RRPosY1+CounterY+1, COLOR_BLACK, XWndHandle, NORENDER);
        }
     }
  }

  MyFMData->PrevStatusArrowDown = ArrowDown;               // Salva il vecchio stato
}







void INTERNAL_T2_ShowSingleElement (struct T2FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList,
                                    int FirstElement, int Offset, TypeColor XColor, char ReDrawButton)
/*
   Questa routine è concepita esclusivamente per uso interno.

   La routine provvede a visualizzare una singola riga nell'area attiva del FileManager.
   L'elemento che verrà visualizzato è quello definito dalla relazione FirstElement+Offset.
   La riga che viene scritta è la nr. Offset dell'area attiva del FileManager.

   Il colore usato è quello definito da XColor. La routine è il cuore di ShowFileList,
   EvidenceItem e DeevidenceItem che la richiamano internamente.

   Se il parametro ReDrawButton è posto a 1, la routine provvede anche a commutare il
   MainButton della finestra dallo stato Load/Save a ChDir e viceversa (qualora fosse
   necessario).
*/
{
   struct ELEMENT_File_Type FileInList;
   char StrNumero [12];
   char StrData [8];

   char XWndHandle;
   char CounterB, YouCanExit;
   char CharToWrite;

   XWndHandle = MyFMData->WndHandle1;

   ndDB_GetFileFromList (MyFileList, FirstElement+Offset, &FileInList);
   // Preleva i dati relativi al file di interesse

   if (FileInList.IsDir!=2)
   {
      CounterB=0;
      YouCanExit=0;
      ndWS_GoCursor (MyFMData->Tab1, MyFMData->VCharY1 + Offset, XWndHandle);

      while ( !YouCanExit )
      {
         CharToWrite=FileInList.Name [CounterB];

         if (CharToWrite!=0)
         {
            ndWS_WriteLetter (CharToWrite, XColor, XWndHandle, NORENDER);
            CounterB++;

            if ( (CounterB==64) || (CounterB==MyFMData->MaxLenNmFile) ) YouCanExit=1;
         }
         else
            YouCanExit=1;
      }

      CounterB=0;
      YouCanExit=0;
      ndWS_GoCursor (MyFMData->Tab2, MyFMData->VCharY1 + Offset, XWndHandle);

      while ( !YouCanExit )
      {
         CharToWrite=FileInList.Ext [CounterB];

         if (CharToWrite!=0)
            ndWS_WriteLetter (CharToWrite, XColor, XWndHandle, NORENDER);

         CounterB++;
         if ((CounterB==3) || (CharToWrite==0)) YouCanExit=1;
      }

      if (FileInList.DateTime.available)
      {
          fssStringDate ( (FileInList.DateTime.day), (FileInList.DateTime.month), (FileInList.DateTime.year), &StrData);
          ndWS_GoCursor (MyFMData->Tab3, MyFMData->VCharY1 + Offset, XWndHandle);
          ndWS_Write (StrData, XColor, XWndHandle, NORENDER);
      }
      else
      {
         ndWS_GoCursor (MyFMData->Tab3, MyFMData->VCharY1 + Offset, XWndHandle);
         ndWS_Write ("<NODATE>", XColor, XWndHandle, NORENDER);
      }
      

      if (FileInList.IsDir==0)
      {
         ndWS_GoCursor (MyFMData->Tab4, MyFMData->VCharY1 + Offset, XWndHandle);

         StringNumber (FileInList.Size, StrNumero);
         ndWS_Write (StrNumero, XColor, XWndHandle, NORENDER);
      }

      if (FileInList.IsDir==1)
      {
         ndWS_GoCursor (MyFMData->Tab4+3, MyFMData->VCharY1 + Offset, XWndHandle);
         ndWS_Write ("<dir>", XColor, XWndHandle, NORENDER);
      }
  }
  else   // IsDir è allo stato 2, usato solo per la directory ..
  {
    ndWS_GoCursor (MyFMData->Tab1, MyFMData->VCharY1 + Offset, XWndHandle);
    ndWS_Write ("<Previous directory>", XColor, XWndHandle, NORENDER);
  }

  if (ReDrawButton)
  {
     if (FileInList.IsDir==0)
        INTERNAL_T2_DrawMainButton (MyFMData, 0);
     else
        INTERNAL_T2_DrawMainButton (MyFMData, 1);
  }

}



void INTERNAL_T2_ShowFileList (struct T2FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int FirstElement)
/*
   Questa routine è stata scritta per scopi interni.
   Si occupa di disegnare i caratteri che costituiscono il menù
*/

{
  char Counter;
  int  LastElement, NrItemsVisualizable;

  char ArrowUp, ArrowDown;
  char XWndHandle;


  XWndHandle=MyFMData->WndHandle1;

  INTERNAL_T2_ClrMemoryChar (MyFMData);      // Cancella la memoria carattere nell'area attiva

  // Ed ora eseguiamo dei calcoli

  if (FirstElement > 0) ArrowUp=1;
     else ArrowUp=0;

  LastElement=FirstElement + MyFMData->VLenCharY - 1;

  if (LastElement >= (  ((MyFileList->NrElements)-1)   )  )
  {
     LastElement=(MyFileList->NrElements - 1);
     ArrowDown=0;
  }
  else ArrowDown=1;

  MyFMData->LastElementVisualized = LastElement;

  // Disegniamo le freccie in alto ed in basso.
  INTERNAL_T2_DrawArrowUpInListFile (MyFMData, ArrowUp);
  INTERNAL_T2_DrawArrowDownInListFile (MyFMData, ArrowDown);

  NrItemsVisualizable = LastElement - FirstElement + 1;

  for (Counter=0; Counter<NrItemsVisualizable; Counter++)
  {
    INTERNAL_T2_ShowSingleElement (MyFMData, MyFileList, FirstElement, Counter, COLOR_GRAY, 0);
  }

  XWindowRender (XWndHandle);

}


void INTERNAL_T2_EvidenceFileItem (struct T2FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int PointerToItem, int FirstItemInFocus)
/*
   Usata solo per usi interni e non accessibile dall'esterno.

   Si occupa di evidenziare un singolo elemento del menù dei file
*/

{
  int ItemPos;

  ItemPos=PointerToItem - FirstItemInFocus;
  INTERNAL_T2_ShowSingleElement (MyFMData, MyFileList, FirstItemInFocus, ItemPos, COLOR_WHITE, 1);

  XWindowRender (MyFMData->WndHandle1);
}


void INTERNAL_T2_DeEvidenceFileItem (struct T2FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int PointerToItem, int FirstItemInFocus)
/*
   Usata solo per usi interni e non accessibile dall'esterno.

   Si occupa di deevidenziare un singolo elemento del menù dei file
*/

{
  int ItemPos;

  ItemPos=PointerToItem - FirstItemInFocus;
  INTERNAL_T2_ShowSingleElement (MyFMData, MyFileList, FirstItemInFocus, ItemPos, COLOR_GRAY, 1);

  XWindowRender (MyFMData->WndHandle1);
}




char INTERNAL_T2_FileManagerChDir (char *ShortNameNewDir, char *ShortExtNewDir)
  /*
     Questa routine è solo per uso interno.

     Non provvede a cambiare la directory corrente ma esegue il cambiamento
     della variabile ndFileManCurrPath.

     La funzione restituisce 0 quando l'intera operazione avviene con successo.

     Qualora invece l'operazione fallisca, la routine restituisce il codice di
     errore -1.
  */
{
   char Counter;
   char OldPathLen;
   char YouCanExit, SlashPos;
   char InternalString [256];

   if ((strcmp (ShortNameNewDir, ".."))!=0)           // Se la directory è diversa da ..
   {
      strcpy (InternalString, ShortNameNewDir);           // Copia dentro InternalString il nome
      fssAppendExtension (InternalString, ShortExtNewDir);   // Appende l'estensione
      fssPutSlashAtEnd (InternalString);                     // Pone uno slash alla fine

      strcat (ndFileManCurrPath, InternalString);    // Esegue l'append di ShortNameDir

      return 0;
   }
   else   // E' un chdir ..
   {
       OldPathLen = strlen (ndFileManCurrPath);
       // In OldPathLen c'è la posizione del carattere NULL che chiude ndFileManCurrPath

       Counter=OldPathLen-1;
       // Counter punta sull'ultimo carattere diverso da 0 della stringa

       if (ndFileManCurrPath [Counter] == '/')
       {
         ndFileManCurrPath [Counter]=0;
         Counter--;                // Considera il carattere ancora precedente
       }

       YouCanExit=0;

       while ( !(YouCanExit) )
       {
          if ((ndFileManCurrPath [Counter])=='/')
             YouCanExit=1;
          else
          {
             Counter--;
             if (Counter<0) return -1;              // C'è un qualche errore. Restituisce -1.
          }

       }

       SlashPos=Counter;                            // In Counter c'è la posizione del penultimo slash

       for (Counter=SlashPos+1; Counter<OldPathLen; Counter++)
       {
           ndFileManCurrPath [Counter] = 0;
       }

       return 0;
   }

}


char INTERNAL_T2_FileManagerChooseFile (char *ShortNameDir, char *ShortExtDir, char *OutputFileName)
  /*
     Questa routine è solo per uso interno.

     Questa routine provvede a copiare il nome del file di output che è stato
     selezionato nell'indirizzo OutputFileName. Deve essere la routine
     chiamante ad allocare una stringa di almeno 256 caratteri per
     contenere il nome del file.

     La funzione restituisce 0 quando l'intera operazione avviene con successo.

     Qualora invece l'operazione fallisca, la routine restituisce il codice di
     errore -1.
  */
{
   char InternalString [256];

   strcpy (InternalString, ndFileManCurrPath);
   // Copia dentro InternalString il path attuale

   fssPutSlashAtEnd (InternalString);                 // Pone uno slash alla fine
   strcat (InternalString, ShortNameDir);          // Esegue l'append dentro InternalString del nome file
   fssAppendExtension (InternalString, ShortExtDir);  // Appende l'estensione

   strcpy (OutputFileName, InternalString);    // Copia la stringa completa nell'output
   return 0;
}




char INTERNAL_T2_FileManagerNewFile (char *FileOutputAddr)
/*
     Questa routine è solo per uso interno.

     Essa richiama il New Name Manager in modo che l'utente possa digitare
     un nuovo nome. Dopo, lo copia all'indirizzo FileOutputAddr.

     La funzione restituisce 0 quando l'intera operazione avviene con successo.

     Qualora invece l'operazione fallisca, la routine restituisce il codice di
     errore -1.
*/
{
   char ErrRep;
   char InternalString [255];
   char IntFileOutput  [255];

   ErrRep = INTERNAL_GetNewName ("Choose name for new file", IntFileOutput);       // Richiama il New Name Manager

   if (!ErrRep)
   {
      if ( (INTERNAL_TEST_TestName (IntFileOutput))==0 )    // Se i controlli sul nome file hanno successo
      {
         strcpy (InternalString, ndFileManCurrPath);               // Copia ndFileManCurrPath nella stringa interna

         fssPutSlashAtEnd (InternalString);                    // Pone uno slash alla fine

         strcat (InternalString, IntFileOutput);            // Ottiene il nome completo del nuovo file
         strcpy (FileOutputAddr, InternalString);           // Copia il risultato nel buffer della routine che ha chiamato FileManager

         return 0;
      }
      else
         return -1;                                         // e segnala l'errore al FileManager in modo che non esca
   }
   else
   {
      return -1;                                       // C'è stato qualche errore con il NewNameManager. Restituisci -1
                                                       // al file manager in modo da informarlo che non si può ancora uscire
   }
}


char INTERNAL_T2_FileManagerEraseFile (char *ShortNameFile, char *ShortExtFile)
  /*
     Questa routine è solo per uso interno.

     Questa routine cancella il file scelto dall'utente, ed i cui nomi brevi
     (senza percorso), sono dati da ShortNameFile e ShortExtFile.

     La funzione restituisce 0 quando l'intera operazione avviene con successo.

     Qualora invece l'operazione fallisca, la routine restituisce il codice di
     errore -1.
  */
{
   char InternalString [256];
   char Confirm, ErrorCode;

   strcpy (InternalString, ndFileManCurrPath);
   // Copia dentro InternalString il path attuale

   fssPutSlashAtEnd (InternalString);                  // Pone uno slash alla fine
   strcat (InternalString, ShortNameFile);          // Esegue l'append dentro InternalString del nome file
   fssAppendExtension (InternalString, ShortExtFile);  // Appende l'estensione

   // In InternalString ora c'è il nome completo del file da cancellare

   Confirm=INTERNAL_T12_ConfirmErase (InternalString, 0);

   if (Confirm)                      // L'utente ha confermato la richiesta di cancellazione
   {
      ErrorCode = ndHAL_EraseFile (InternalString);

      if (ErrorCode<0)               // L'operazione di cancellazione ha provocato un'errore
      {
         INTERNAL_T12_ErrorOnErase (InternalString, 0);
         return -1;                  // Segnala alla routine chiamante che non hai potuto cancellare i file
      }
      else
      {
         INTERNAL_T12_SuccessOnErase (InternalString, 0);
         return 0;
      }
   }
   else
       return -1;    // Segnala alla routine chiamante che l'utente ha annullato l'operazione di cancellazione del file
}


char INTERNAL_T2_FileManagerRmDir (char *ShortNameDir, char *ShortExtDir)
  /*
     Questa routine è solo per uso interno.

     Questa routine cancella la directory scelta dall'utente, ed i cui nomi brevi
     (senza percorso), sono dati da ShortNameDir e ShortExtDir.

     La funzione restituisce 0 quando l'intera operazione avviene con successo.

     Qualora invece l'operazione fallisca, la routine restituisce il codice di
     errore -1.
  */
{
   char InternalString [256];
   char Confirm, ErrorCode;

   strcpy (InternalString, ndFileManCurrPath);
   // Copia dentro InternalString il path attuale

   fssPutSlashAtEnd (InternalString);
   strcat (InternalString, ShortNameDir);          // Esegue l'append dentro InternalString del nome directory
   fssAppendExtension (InternalString, ShortExtDir);  // Appendi l'estensione

   // In InternalString ora c'è il nome completo della directory da cancellare

   Confirm=INTERNAL_T12_ConfirmErase (InternalString, 1);

   if (Confirm)                      // L'utente ha confermato la richiesta di cancellazione
   {
      ErrorCode = INTERNAL_T12_RecursiveDestroyDir (InternalString, 0, 0);

      if (ErrorCode<0)               // L'operazione di cancellazione ha provocato un'errore
      {
         INTERNAL_T12_ErrorOnErase (InternalString, 1);
         return -1;                  // Segnala alla routine chiamante che non hai potuto cancellare i file
      }
      else
      {
         INTERNAL_T12_SuccessOnErase (InternalString, 1);
         return 0;
      }
   }
   else
       return -1;    // Segnala alla routine chiamante che l'utente ha annullato l'operazione di cancellazione del file
}







char INTERNAL_T2_FileManagerMkDir (void)
/*
     Questa routine è solo per uso interno.

     Essa richiama il New Name Manager in modo che l'utente possa digitare
     il nome della nuova subdirectory. Dopo, tenta di creare la nuova
     subdirectory nella directory ndFileManCurrPath corrente.

     La funzione restituisce 0 quando l'intera operazione avviene con successo.

     Qualora invece l'operazione fallisca, la routine restituisce il codice di
     errore -1.
*/
{

   char ErrRep;
   char InternalString [255];
   char IntDirNameOutput  [255];
   int  DiskErr;

   ErrRep = INTERNAL_GetNewName ("Choose name for new directory", IntDirNameOutput);       // Richiama il New Name Manager

   if (!ErrRep)
   {
      if ( (INTERNAL_TEST_TestName (IntDirNameOutput))==0 ) // Se i controlli sul nome directory hanno successo
      {
         strcpy (InternalString, ndFileManCurrPath);               // Copia ndFileManCurrPath nella stringa interna

         // Il ndFileManCurrPath deve sempre terminare per /
         // Eseguiamo un controllo sul vecchio ndFileManCurrPath

         fssPutSlashAtEnd (InternalString);
         strcat (InternalString, IntDirNameOutput);            // Ottiene il nome completo della nuova directory

         // Secondo controllo: stavolta sul nome della nuova directory
         fssPutSlashAtEnd (InternalString);

         // Ora tentiamo di creare la nuova directory

         DiskErr=ndHAL_MakeDir (InternalString, O_RDWR);

         if (DiskErr==0) return 0;             // La creazione della nuova directory è riuscita. Ritorna con codice di errore 0.
         else
         {
            INTERNAL_T12_MkDirErrorBox (InternalString);
            return -1;
         }
      }
      else                                                  // Se il test sul nome della nuova directory fallisce
         return -1;                                         // segnala l'errore al FileManager in modo che non esca
   }
   else
      return -1;                                       // C'è stato qualche errore con il NewNameManager. Restituisci -1
                                                       // al file manager in modo da informarlo che non si può ancora uscire
}








char INTERNAL_T2_FileManagerRename (char *ShortNameFile, char *ShortExtFile, char IsDir)
/*
     Questa routine è solo per uso interno.

     Essa richiama il New Name Manager in modo che l'utente possa digitare
     il nuovo nome del file. Dopo, tenta di eseguire il rename del file.

     La funzione restituisce 0 quando l'intera operazione avviene con successo.

     Qualora invece l'operazione fallisca, la routine restituisce il codice di
     errore -1.
*/
{
   char InternalStringOld [256];
   char InternalStringNew [256];

   char NewNameChoosen [256];

   char Confirm, ErrorCode;
   char ErrRep1;

   strcpy (InternalStringOld, ndFileManCurrPath);
   // Copia dentro InternalStringOld il path attuale

   fssPutSlashAtEnd (InternalStringOld);                    // Se l'ultimo carattere non è un / lo aggiungo
   strcat (InternalStringOld, ShortNameFile);            // Esegue l'append dentro InternalStringOld del nome file
   fssAppendExtension (InternalStringOld, ShortExtFile);    // Appende l'estensione

   if (IsDir) fssPutSlashAtEnd (InternalStringOld);         // Se siamo in presenza di una directory dobbiamo appendere un'altro
                                                         // slash

   // In InternalStringOld ora c'è il vecchio nome (completo) del file da rinominare

   if (!(IsDir))
      ErrRep1 = INTERNAL_GetNewName ("Choose new name for file", NewNameChoosen);       // Richiama il New Name Manager
   else
      ErrRep1 = INTERNAL_GetNewName ("Choose new name for directory", NewNameChoosen);       // Richiama il New Name Manager

   if (!ErrRep1)
   {
      if ( (INTERNAL_TEST_TestName (NewNameChoosen))==0 ) // Se i controlli sul nome del nuovo file hanno successo
      {
         strcpy (InternalStringNew, ndFileManCurrPath);               // Copia ndFileManCurrPath in InternalStringNew

         fssPutSlashAtEnd (InternalStringNew);                    // Il ndFileManCurrPath deve sempre terminare per /
                                                               // Eseguiamo un controllo sul vecchio ndFileManCurrPath

         strcat (InternalStringNew, NewNameChoosen);           // Ottiene il nome completo del nuovo file

         if (IsDir) fssPutSlashAtEnd (InternalStringNew);         // Se siamo in presenza di una directory dobbiamo inserire
                                                               // un altro slash

         // Ora chiediamo conferma per il rename del file
         Confirm=INTERNAL_T12_ConfirmRename (InternalStringOld, InternalStringNew, IsDir);

         if (Confirm)                      // L'utente ha confermato la richiesta di rename
         {
            ErrorCode = ndHAL_RenameFile (InternalStringOld, InternalStringNew);

            if (ErrorCode!=0)               // L'operazione di rename ha provocato un'errore
            {
               INTERNAL_T12_ErrorOnRename (InternalStringOld, IsDir);
               return -1;                  // Segnala alla routine chiamante che non hai potuto rinominare i file
            }
            else
            {
               INTERNAL_T12_SuccessOnRename (InternalStringOld, IsDir);
               return 0;
            }
         }
         else
             return -1;    // Segnala alla routine chiamante che l'utente ha annullato l'operazione di rename del file
      }
      else
          return -1;   // L'utente ha immesso un nome non valido
   }
   else
       return -1;   // Segnala alla routine chiamante che l'utente ha annullato l'operazione di rename del file

}


char INTERNAL_T2FileManager (char *Title, unsigned char SaveMode, char *StringOutputAddr)
{
 struct T2FMData_Type       FMData;
 struct HEADLIST_File_Type  FileList;
 struct ELEMENT_File_Type   SingleFile;
 struct ndEvent_Type        Event;

 char ErrRep1, ErrRep2, ErrOutput, ErrWndAlloc;
 
 int  FirstItemInFocus;
 int  PointerToItem;
 int  HypPointerToItem;
 char YouCanExitLev1, YouCanExitLev2;

 char ResSystemPath [255];
 char IsFirstChDir;

 FMData.SaveMode=SaveMode;
 // Memorizza il parametro SaveMode per le routine che disegnano i pulsanti sullo schermo

 ErrWndAlloc = INTERNAL_T2_DrawFMWindows (&FMData, Title);
 // Disegna il file manager con i pulsanti
 
 if (ErrWndAlloc!=0)
 {
   return ERR_NO_WINDOW_IS_AVAILABLE;              // Tronca subito l'esecuzione se non ci sono finestre
 }                                                 // disponibili

 IsFirstChDir=1;
 ErrOutput=0;
 YouCanExitLev1=0;

 while (!YouCanExitLev1)
 {
       INTERNAL_T2_DrawSysPathBar (&FMData);

       ndDB_InitListOfFile (&FileList);
       // Inizializza la lista di file.
    
       ErrRep1=ndDB_FillFileList (ndFileManCurrPath, &FileList);
       // Provvede a spostarsi nella directory attuale ed esegue il sorting degli elementi

       if (!ErrRep1)
       {
           // Siamo riusciti ad entrare in questa directory. Allora salviamo per sicurezza
           // il percorso in modo da potere ritornare

           strcpy (ResSystemPath, ndFileManCurrPath);
           IsFirstChDir=0;

           INTERNAL_T2_ClrMemoryChar (&FMData);

           FirstItemInFocus=0;
           PointerToItem=0;

           INTERNAL_T2_ShowFileList (&FMData, &FileList, FirstItemInFocus);
           INTERNAL_T2_UpdateChDirPointPointButton (&FMData);
           INTERNAL_T2_EvidenceFileItem (&FMData, &FileList, 0, FirstItemInFocus);

           YouCanExitLev2=0;

           while (!YouCanExitLev2)
           {   
              MouseControl (&Event);
              
              if ((Event.__TypeEvent==CUSTOM_BUTTON_PRESSED) && (Event.__WindowID==FMData.WndHandle1))
              {

                  if ( Event.__ButtonID==FMData.ButtonHandle_ScrollFileUp )
                  {
                     // E' stato premuto il pulsante per scrollare in alto i file
    
                     HypPointerToItem=PointerToItem-1;
    
                     if (   HypPointerToItem>=0   )
                     {
                         INTERNAL_T2_DeEvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus);
    
                         if (HypPointerToItem >= FirstItemInFocus )
                         {
                             PointerToItem = HypPointerToItem;
                             INTERNAL_T2_EvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus);
                         }
                         else
                         {
                             FirstItemInFocus--;
                             INTERNAL_T2_ShowFileList (&FMData, &FileList, FirstItemInFocus);
    
                             PointerToItem = HypPointerToItem;
                             INTERNAL_T2_EvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus);
                         }
                     }
                  }
    
    
                  if ( Event.__ButtonID==FMData.ButtonHandle_ScrollFileDown )
    		      {
                     // E' stato premuto il pulsante per scrollare in basso i file
    
                     HypPointerToItem=PointerToItem+1;
    
                     if (   HypPointerToItem<=( (FileList.NrElements)-1 )    )
                     {
                        INTERNAL_T2_DeEvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus);
    
                        if (HypPointerToItem <= FMData.LastElementVisualized )
                        {
                           PointerToItem = HypPointerToItem;
                           INTERNAL_T2_EvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus);
                        }
                        else
                        {
                           FirstItemInFocus++;
                           INTERNAL_T2_ShowFileList (&FMData, &FileList, FirstItemInFocus);
    
                           PointerToItem = HypPointerToItem;
                           INTERNAL_T2_EvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus);
                        }
                     }
                  }
    
                  if ( Event.__ButtonID==FMData.ButtonHandle_ChDirPointPoint )
                  {
                     INTERNAL_T2_FileManagerChDir ("..", "");
                     YouCanExitLev2=1;        // Questo provocherà l'uscita dal livello di loop più interno
                                              // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                              // data dal nuovo ndFileManCurrPath  
                  }     
    
    
    
                  if ( Event.__ButtonID==FMData.ButtonHandle_MainButton )
                  // E' stato premuto il main button
                  {
                     ndDB_GetFileFromList (&FileList, PointerToItem, &SingleFile);
    
                     if (SingleFile.IsDir!=0)      // Si sta cercando di cambiare una directory
                     {
                        INTERNAL_T2_FileManagerChDir (&SingleFile.Name, &SingleFile.Ext);
                        YouCanExitLev2=1;     // Questo provocherà l'uscita dal livello di loop più interno
                                              // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                              // data dal nuovo ndFileManCurrPath
                     }
                     else                        // Si è selezionato un file
                     {
                        INTERNAL_T2_FileManagerChooseFile (&SingleFile.Name, &SingleFile.Ext, StringOutputAddr);
                        YouCanExitLev2=1;
                        YouCanExitLev1=1;
                        ErrOutput=0;             // Codice di errore 0 in uscita dal File Manager
                     }
                  }
    
    
                  if (SaveMode)
                  {
    
                          if ( Event.__ButtonID==FMData.ButtonHandle_New )
                          {
                             ErrRep2 = INTERNAL_T2_FileManagerNewFile (StringOutputAddr);
                             // Seleziona la routine per la creazione di nuovi file
            
                             if (ErrRep2==0)             // Non ci sono stati errori nella scelta del nuovo file
                             {
                                YouCanExitLev2=1;        // Esci da entrambi i cicli di loop
                                YouCanExitLev1=1;
                                ErrOutput=0;             // Codice di errore 0 in uscita dal file manager
                             }
                          }
            
                          if ( Event.__ButtonID==FMData.ButtonHandle_MkDir )
                          {
                             ErrRep2 = INTERNAL_T2_FileManagerMkDir ();
                             // Seleziona la routine per la creazione di una nuova directory
            
                             if (ErrRep2==0)         // La creazione della nuova directory ha avuto successo
                             {
                                YouCanExitLev2=1;    // Questo farà sì che il sistema esca dal loop di secondo livello,
                                                     // ma rimanga nel loop di primo livello, ricaricando così la nuova
                                                     // lista dei file nel path corrente
                             }
                          }
            
                          if ( Event.__ButtonID==FMData.ButtonHandle_Delete )
                          {
                             ndDB_GetFileFromList (&FileList, PointerToItem, &SingleFile);
            
                             if (SingleFile.IsDir==0)      // Si sta cercando di cancellare un file
                             {
            
                                ErrRep2 = INTERNAL_T2_FileManagerEraseFile (&SingleFile.Name, &SingleFile.Ext);
            
                                if (ErrRep2==0)
                                {
                                   YouCanExitLev2=1;     // Questo provocherà l'uscita dal livello di loop più interno
                                                         // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                                         // con i cambiamenti aggiornati
                                }
                             }
            
                             if (SingleFile.IsDir==1)      // Si sta cercando di cancellare una directory
                             {
            
                                ErrRep2 = INTERNAL_T2_FileManagerRmDir (&SingleFile.Name, &SingleFile.Ext);
            
                                if (ErrRep2==0)
                                {
                                   YouCanExitLev2=1;     // Questo provocherà l'uscita dal livello di loop più interno
                                                         // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                                         // con i cambiamenti aggiornati
                                }
                             }
            
                          }
            
                          if ( Event.__ButtonID==FMData.ButtonHandle_Rename )
                          {
                             ndDB_GetFileFromList (&FileList, PointerToItem, &SingleFile);
            
                             if (SingleFile.IsDir!=2)
                             {
                                ErrRep2 = INTERNAL_T2_FileManagerRename (&SingleFile.Name, &SingleFile.Ext, SingleFile.IsDir);
            
                                if (ErrRep2==0)
                                {
                                   YouCanExitLev2=1;     // Questo provocherà l'uscita dal livello di loop più interno
                                                         // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                                         // con i cambiamenti aggiornati
                                }
                             }
                          }
                          
                  }       // End of SaveMode
            }
               
                                            
            if ( (Event.__TypeEvent==A_WINDOW_WANTS_TO_BE_CLOSED) && (Event.__WindowID==FMData.WndHandle1) )
            {
               YouCanExitLev1=1;
               YouCanExitLev2=1;

               ErrOutput = ERR_OPERATION_INTERRUPTED;
            }
                                
          }    // End of while (! YouCanExitLev2)

          ndDB_DestroyListOfFile (&FileList);         // Disalloca i dati contenuti dentro FileList;

      }     // End if !ErrRep1
      else
      {
          if (!(IsFirstChDir))
          {
             strcpy (ndFileManCurrPath, ResSystemPath);
             // Se non è la prima volta che si esegue il ciclo while, torna automaticamente alla
             // ultima directory in cui c'è stato un accesso con successo.
          }
          else
          {
             ErrOutput = ERR_ACCESS_SYSTEM_PATH;
             YouCanExitLev1=1;
          }

      }

       

  } // End of while (! YouCanExitLev1)

  ndLP_DestroyWindow (FMData.WndHandle1);         // Cancella la finestra del file manager

  if (ErrOutput!=0) strcpy (StringOutputAddr, "");    // Se c'è un errore la stringa restituita è comunque vuota per sicurezza

  return ErrOutput;

}




