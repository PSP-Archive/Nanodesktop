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
    FILE MANAGER DI TIPO 1
    ----------------------------------------------------------------------------
    Le seguenti routine riguardano il file manager di tipo 1
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>




void INTERNAL_T1_SetFMWindowParams (struct T1FMData_Type *MyFMData)
/*
     Questa routine è esclusivamente per uso interno.

     Si occupa di eseguire dei calcoli per determinare le variabili interne della finestra
     del File Manager.

     Poichè sono consentite diverse istanze del File Manager, ogni istanza alloca nel proprio
     stack una struct locale di tipo T1FMData_Type atta a contenere i dati relativi alla
     finestra del FileManager.

     L'indirizzo di tale struct viene passato, sotto forma di parametro, a tutte le routine
     che sono richiamate da FileManager.
*/
{
 unsigned short int w1;
 unsigned short int Counter;

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

 MyFMData->LenActiveAreaX = MyFMData->B3_X2 - MyFMData->B3_X1;
 MyFMData->LenActiveAreaY = MyFMData->B3_Y2 - MyFMData->B3_Y1;
 
 MyFMData->C1_X1 = MyFMData->B3_X1+16;       // Area attiva per i file (livello 2 o BoxArea)
 MyFMData->C1_X2 = MyFMData->B3_X2-1;
 MyFMData->C1_Y1 = MyFMData->B3_Y1+1;
 MyFMData->C1_Y2 = MyFMData->B3_Y2-1;
 
 MyFMData->LenBoxAreaX    = MyFMData->C1_X2 - MyFMData->C1_X1;
 MyFMData->LenBoxAreaY    = MyFMData->C1_Y2 - MyFMData->C1_Y1;
 
 MyFMData->LenBoxX = (MyFMData->LenBoxAreaX)-2;
 MyFMData->LenBoxY = __DIMY_BOXFMT1;
 
 MyFMData->MargineBoxX = (MyFMData->LenBoxAreaX - MyFMData->LenBoxX) / 2;
 
 MyFMData->NrItemsVisualizable = MyFMData->LenBoxAreaY / MyFMData->LenBoxY;
 
 MyFMData->MargineBoxY = (MyFMData->LenBoxAreaY - MyFMData->LenBoxY * MyFMData->NrItemsVisualizable)/2;
 
 MyFMData->C2_X1 = MyFMData->C1_X1 + MyFMData->MargineBoxX;         // Area attiva per i file (livello 3)
 MyFMData->C2_X2 = MyFMData->C1_X2 - MyFMData->MargineBoxX;
 MyFMData->C2_Y1 = MyFMData->C1_Y1 + MyFMData->MargineBoxY;
 MyFMData->C2_Y2 = MyFMData->C1_Y2 - MyFMData->MargineBoxY;
  
 MyFMData->PosIconBoxX=10;
 MyFMData->PosIconBoxY=3;
 MyFMData->DimIconBox = MyFMData->LenBoxY - 6; 
  
 MyFMData->VCharX1 = ((MyFMData->B3_X1 - WindowData [w1].MargineCharX)/8)+1;
 MyFMData->VCharX2 = ((MyFMData->B3_X2 - WindowData [w1].MargineCharX)/8)-1;

 MyFMData->MaxLenPathAllowed = MyFMData->VCharX2 - MyFMData->VCharX1;

 MyFMData->GraphArea_X1 = 10;
 MyFMData->GraphArea_Y1 = 0;
 MyFMData->GraphArea_X2 = MyFMData->DimIconBox+10;
 MyFMData->GraphArea_Y2 = MyFMData->LenBoxY;
   
 MyFMData->SystemArea_X1   = MyFMData->GraphArea_X2;
 MyFMData->SystemArea_X2   = MyFMData->LenBoxX - 10;
 MyFMData->SystemArea_Y1   = MyFMData->GraphArea_Y1;
 MyFMData->SystemArea_Y2   = MyFMData->GraphArea_Y2;
   
 MyFMData->MaxLenNmFile = ((MyFMData->SystemArea_X2 - MyFMData->SystemArea_X1)/8 - 1); 

 MyFMData->FirstRowPosY    = (MyFMData->LenBoxY - 16)/2;
 MyFMData->SecondRowPosY   = MyFMData->FirstRowPosY + 11;
 MyFMData->FirstColumnPosX = MyFMData->SystemArea_X1 + 10;

 MyFMData->PrevStatusChDirButton=-1;                 // Segnala che è la prima volta

 MyFMData->PrevStatusArrowUp   = 0;
 MyFMData->PrevStatusArrowDown = 0;
 
 MyFMData->ThumbnailCachePointer=0;
 
 for (Counter=0; Counter<20; Counter++)
 {
     strcpy (MyFMData->NameThumbnailCached [Counter], "");
     ndIMG_InitImage ( &(MyFMData->ThumbnailCached [Counter]) );
 }
 
}



void INTERNAL_T1_CreateShortPath (struct T1FMData_Type *MyFMData, char *SysPathAddr, char *NewSysPathAddr)
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





void INTERNAL_T1_DrawSysPathBarVoid (struct T1FMData_Type *MyFMData)
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





void INTERNAL_T1_DrawSysPathBar (struct T1FMData_Type *MyFMData)
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
     INTERNAL_T1_CreateShortPath (MyFMData, &ndFileManCurrPath, &NewSysPath);
     ndWS_GrWriteLn (MyFMData->B2_X1 + 4, MyFMData->B2_Y1 + 4, NewSysPath, COLOR_WHITE, COLOR_GRAY, XWndHandle, NORENDER);
  }

}







void INTERNAL_T1_DrawMainButton (struct T1FMData_Type *MyFMData, char ChangeDirMode)
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






void INTERNAL_T1_DrawSecondaryButton (struct T1FMData_Type *MyFMData, char SaveMode)
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




void INTERNAL_T1_DrawButton (struct T1FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Essa disegna i pulsanti principali di un file manager di tipo 1.
*/

{
 if (! (MyFMData->SaveMode) )
 {
    INTERNAL_T1_DrawMainButton (MyFMData, 0);

 }
 else
 {
    INTERNAL_T1_DrawMainButton (MyFMData, 0);
    INTERNAL_T1_DrawSecondaryButton (MyFMData, 0);
 }
}





void INTERNAL_T1_DrawActiveArea (struct T1FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Provvede a disegnare l'area attiva sullo schermo.
*/
{
  unsigned short int Counter;         
           
  ndWS_DrawRectangle (MyFMData->B3_X1, MyFMData->B3_Y1, MyFMData->B3_X2, MyFMData->B3_Y2,
                           COLOR_BLACK, COLOR_WHITE, MyFMData->WndHandle1, NORENDER);

  ndWS_DrawSpRectangle (MyFMData->C1_X1, MyFMData->C1_Y1, MyFMData->C1_X2, MyFMData->C1_Y2,
                             0, COLOR_WHITE, ND_VOID, MyFMData->WndHandle1, NORENDER);

  ndWS_DrawSpRectangle (MyFMData->C2_X1, MyFMData->C2_Y1, MyFMData->C2_X2, MyFMData->C2_Y2,
                             0, COLOR_WHITE, ND_VOID, MyFMData->WndHandle1, NORENDER);

  for (Counter=0; Counter<MyFMData->NrItemsVisualizable; Counter++)
  {
      ndWS_DrawSpRectangle (MyFMData->C2_X1, MyFMData->C2_Y1 + MyFMData->LenBoxY*(Counter+0), 
                                 MyFMData->C2_X2, MyFMData->C2_Y1 + MyFMData->LenBoxY*(Counter+1),
                                 0, COLOR_WHITE, ND_VOID, MyFMData->WndHandle1, NORENDER);
  }
  
  
}


void INTERNAL_T1_DrawLogicalButton (struct T1FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Provvede ad allocare dei pulsanti di tipo ONLY_LOGICAL sopra agli slot per le
  voci dei files, in modo che sia possibile cliccare su uno slot e selezionare
  la voce.
*/
{
  unsigned short int Counter;         
  char StringID [32];
   
  char Temp     [32];
  
  // Questo tipo di pulsanti viene identificato dallo stringID utilizzato. Tutte le
  // stringID sono date da un codice #KEYITEM e da un numero progressivo. In questo
  // modo la routine principale può identificare il numero di pulsante e reagire di
  // conseguenza: questo evita che si debba allocare un array per contenere gli
  // handles dei pulsanti.
  
  for (Counter=0; Counter<MyFMData->NrItemsVisualizable; Counter++)
  {
      strcpy (StringID, "#KEYITEM");
  
      _itoa (Counter, &Temp, 10);
      // Converte il numero Counter in una stringa e lo deposita in Temp
      
      strcat (&StringID, &Temp);
      // In StringID ora c'è la label corretta
      
      ndCTRL_CreateButton     (MyFMData->C2_X1, MyFMData->C2_Y1 + MyFMData->LenBoxY*(Counter+0), 
                               MyFMData->C2_X2, MyFMData->C2_Y1 + MyFMData->LenBoxY*(Counter+1),
                               MyFMData->WndHandle1,
                               &StringID, "", "",
                               0, 0, 0,                         // Colori ignorati perchè usati in modo only-logical
                               ND_BUTTON_LOGIC_MODE, 0, 0, 
                               NORENDER);
      
  }
  
  
}





void INTERNAL_T1_DrawButtonForFileScrollUp (struct T1FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Essa disegna il pulsante per lo scroll dei file verso l'alto nei file
  manager di tipo 1.
*/
{
 int CounterX, CounterY;

 MyFMData->ButtonHandle_ScrollFileUp   = ndCTRL_CreateButton (MyFMData->B3_X1-4-16, MyFMData->B3_Y2-42-24,
                                                              MyFMData->B3_X1-4,    MyFMData->B3_Y2-42,
                                                              MyFMData->WndHandle1,
                                                              "#ScrollDown", "", "", 
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





void INTERNAL_T1_DrawButtonForFileScrollDown (struct T1FMData_Type *MyFMData)
/*
  Questa routine è esclusivamente per uso interno.

  Essa disegna il pulsante per lo scroll dei file verso il basso nei file
  manager di tipo 1.
*/
{
 int CounterX, CounterY;

 MyFMData->ButtonHandle_ScrollFileDown = ndCTRL_CreateButton (MyFMData->B3_X1-4-16, MyFMData->B3_Y2-38,
                                                              MyFMData->B3_X1-4,    MyFMData->B3_Y2-14,
                                                              MyFMData->WndHandle1,
                                                              "#ScrollUp", "", "", 
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


void INTERNAL_T1_DrawButtonForChDirPointPoint (struct T1FMData_Type *MyFMData)
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
     
void INTERNAL_T1_UpdateChDirPointPointButton (struct T1FMData_Type *MyFMData)
/* 
    Il compito di questa routine è evidenziare o meno il tasto per il chdir .. a seconda
    che questo abbia o meno senso nel contesto in cui si opera
*/
{
    if (strcmp (ndFileManCurrPath, SysRootPath)!=0)
       ndCTRL_DeInhibitButton (MyFMData->ButtonHandle_ChDirPointPoint, MyFMData->WndHandle1, NORENDER);
    else
       ndCTRL_InhibitButton (MyFMData->ButtonHandle_ChDirPointPoint, MyFMData->WndHandle1, NORENDER);
        
}


char INTERNAL_T1_DrawFMWindows (struct T1FMData_Type *MyFMData, char *Title)
/*
   Questa routine provvede a disegnare la finestra del file manager di tipo 1 sullo
   schermo. Se si verifica un errore viene fornito in uscita un codice di errore
   pari a -1, altrimenti viene fornito 0
*/

{
  MyFMData->WndHandle1=ndLP_CreateWindow (10, 10, Windows_MaxScreenX-10, Windows_MaxScreenY-10,
                                                 Title, COLOR_WHITE, COLOR_GREEN, COLOR_BLUE, COLOR_WHITE, 0);


  if ( (MyFMData->WndHandle1)>=0 )            // Non si sono verificati errori durante l'allocazione
  {

     INTERNAL_T1_SetFMWindowParams (MyFMData);
     ndLP_MaximizeWindow (MyFMData->WndHandle1);
     
     INTERNAL_T1_DrawSysPathBarVoid (MyFMData);
     // Disegna la barra per il path lasciandola vuota
     
     INTERNAL_T1_DrawActiveArea (MyFMData);
     // Disegna contorno area attiva
     
     INTERNAL_T1_DrawLogicalButton (MyFMData);
     // Disegna i pulsanti logici
     
     INTERNAL_T1_DrawButton (MyFMData);
     // Disegna i tasti principali
     
     INTERNAL_T1_DrawButtonForFileScrollUp (MyFMData);
     // Disegna il pulsante per lo scrolling in alto

     INTERNAL_T1_DrawButtonForFileScrollDown (MyFMData);
     // Disegna il pulsante per lo scrolling in basso
     
     INTERNAL_T1_DrawButtonForChDirPointPoint (MyFMData);
     // Disegna il pulsante per il chdir..
     
     XWindowRender (MyFMData->WndHandle1);
     
     return 0;
  }
  else
      return -1;
}





void INTERNAL_T1_ClrActiveArea (struct T1FMData_Type *MyFMData)
/*
   Questa routine è esclusivamente per uso interno.
*/

{
  unsigned short int Counter;         
           
  ndWS_DrawRectangle (MyFMData->B3_X1, MyFMData->B3_Y1, MyFMData->B3_X2, MyFMData->B3_Y2,
                           COLOR_BLACK, COLOR_WHITE, MyFMData->WndHandle1, NORENDER);

  ndWS_DrawSpRectangle (MyFMData->C1_X1, MyFMData->C1_Y1, MyFMData->C1_X2, MyFMData->C1_Y2,
                             0, COLOR_WHITE, ND_VOID, MyFMData->WndHandle1, NORENDER);

  ndWS_DrawSpRectangle (MyFMData->C2_X1, MyFMData->C2_Y1, MyFMData->C2_X2, MyFMData->C2_Y2,
                             0, COLOR_WHITE, ND_VOID, MyFMData->WndHandle1, NORENDER);

  for (Counter=0; Counter<MyFMData->NrItemsVisualizable; Counter++)
  {
      ndWS_DrawSpRectangle (MyFMData->C2_X1, MyFMData->C2_Y1 + MyFMData->LenBoxY*(Counter+0), 
                                 MyFMData->C2_X2, MyFMData->C2_Y1 + MyFMData->LenBoxY*(Counter+1),
                                 0, COLOR_WHITE, ND_VOID, MyFMData->WndHandle1, NORENDER);
  }
}

void INTERNAL_T1_ClrBoxArea (struct T1FMData_Type *MyFMData)
/*
   Questa routine è esclusivamente per uso interno.
*/

{
           
  ndWS_DrawRectangle     (MyFMData->C1_X1, MyFMData->C1_Y1, MyFMData->C1_X2, MyFMData->C1_Y2,
                           COLOR_BLACK, COLOR_WHITE, MyFMData->WndHandle1, NORENDER);

  ndWS_DrawSpRectangle (MyFMData->C2_X1, MyFMData->C2_Y1, MyFMData->C2_X2, MyFMData->C2_Y2,
                             0, COLOR_WHITE, ND_VOID, MyFMData->WndHandle1, NORENDER);

}




void INTERNAL_T1_DrawArrowUpInListFile (struct T1FMData_Type *MyFMData, char ArrowUp)
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





void INTERNAL_T1_DrawArrowDownInListFile (struct T1FMData_Type *MyFMData, char ArrowDown)
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



void INTERNAL_T1_ShowThumbnailOnScreen (struct ndImage_Type *MyImage, unsigned short int TPosX, unsigned short int TPosY,
                                        unsigned short int XWndHandle)
{
  unsigned short int TLenX, TLenY;
  
  TLenX = MyImage->LenX;
  TLenY = MyImage->LenY;

  ndWS_DrawRectangle     (TPosX, TPosY, TPosX + TLenX, TPosY + TLenY, COLOR_BLACK, COLOR_RED, XWndHandle, NORENDER);

  ndWS_DrawLine          (TPosX + 4,          TPosY + TLenY + 1, 
                          TPosX + TLenX,      TPosY + TLenY + 1, COLOR_RED, XWndHandle, NORENDER);

  ndWS_DrawLine          (TPosX + TLenX + 1,  TPosY + 4, 
                          TPosX + TLenX + 1,  TPosY + TLenY + 1, COLOR_RED, XWndHandle, NORENDER);   

  ndIMG_ShowImage (MyImage, TPosX, TPosY, XWndHandle, NORENDER);
  
}


void INTERNAL_T1_SaveThumbnailInCache (struct T1FMData_Type *MyFMData, struct ndImage_Type *NewThumb, char *CompleteName)
{
  char Pointer;                               
                                 
  MyFMData->ThumbnailCachePointer = MyFMData->ThumbnailCachePointer+1;
  if ((MyFMData->ThumbnailCachePointer)==20) MyFMData->ThumbnailCachePointer=0;
  
  Pointer = MyFMData->ThumbnailCachePointer;
  
  if (strcmp (MyFMData->NameThumbnailCached [Pointer], "")!=0)
  {
     ndIMG_ReleaseImage ( &(MyFMData->ThumbnailCached [Pointer]) );
     // Disalloca lo spazio impegnato da questa vecchia thumbnail
  }
  
  strcpy (MyFMData->NameThumbnailCached [Pointer], CompleteName);
  // Copia il nome della nuova thumbnail
  
  MyFMData->ThumbnailCached [Pointer].LenX        = NewThumb->LenX;
  MyFMData->ThumbnailCached [Pointer].LenY        = NewThumb->LenY;
  MyFMData->ThumbnailCached [Pointer].ColorFormat = NewThumb->ColorFormat;
  MyFMData->ThumbnailCached [Pointer].Data        = NewThumb->Data;
  // Copia i dati relativi alla Thumbnail

} 


char IsPresentThumbInCache (struct T1FMData_Type *MyFMData, struct ndImage_Type *ThumbImage, char *CompleteName)
{
  char Pointer = 0;
    
  while (Pointer<20)
  {
     if (strcmp (MyFMData->NameThumbnailCached [Pointer], CompleteName)==0)
     {
        ThumbImage->LenX        = MyFMData->ThumbnailCached [Pointer].LenX;
        ThumbImage->LenY        = MyFMData->ThumbnailCached [Pointer].LenY;
        ThumbImage->ColorFormat = MyFMData->ThumbnailCached [Pointer].ColorFormat;
        ThumbImage->Data        = MyFMData->ThumbnailCached [Pointer].Data;
        
        return 1;
     }     
          
     Pointer++;
  }
  
  return 0;
}


void INTERNAL_DestroyThumbNailCache (struct T1FMData_Type *MyFMData)
{
  char Counter;
  
  for (Counter=0; Counter<20; Counter++)
  {
      if (strcmp (MyFMData->NameThumbnailCached [Counter], "") != 0)
      {
         ndIMG_ReleaseImage ( &(MyFMData->ThumbnailCached [Counter]) );
      }
  }   
}


void INTERNAL_T1_DrawThumbnailOrIcon (struct T1FMData_Type *MyFMData, 
                                      unsigned short int TPosX, unsigned short int TPosY, 
                                      char *CompleteName, char *Ext)
{
  struct ELEMENT_Class_Type *ClassInfoAddr;
  struct ELEMENT_Icon_Type  *InternalIconAddr;
  
  int PointerToRamClass;
  int PointerToRamIcon;
  
  struct ndImage_Type NewThumb;
  
  char  (*CodeThumbFunc) (struct ndImage_Type *ThumbImage, char *CompleteNameFile,
                          unsigned short int KLenX, unsigned short int KLenY);

  char XWndHandle, IsCached;
  char CanMakeThumb;
  
  static char ErrRep;    // Deve essere dichiarato static altrimenti si crea un malfunzionamento
  
  XWndHandle = MyFMData->WndHandle1;
    
  ErrRep = ndDB_FindClassAddrFromExtension (&SystemClasses, Ext, &PointerToRamClass);
  // Previene un malfunzionamento del compilatore
  
  ClassInfoAddr = PointerToRamClass;
  // Ora in ClassInfoAddr c'è l'indirizzo delle informazioni sulla classe richieste

  if (!ErrRep)                // E' stata trovata una classe compatibile
  {  
     if ((ClassInfoAddr->ThumbFunction!=0) && (ThumbnailEnabled))
     {
        IsCached = IsPresentThumbInCache (MyFMData, &NewThumb, CompleteName);
  
        if (IsCached == 0)         // La Thumbnail non è cacherizzata
        {
             ndIMG_InitImage (&NewThumb);
             // Impedisce che CreateThumbnailForImage, vedendo che NewThumb non è vuota, disallochi dei
             // dati che sono puntati dai puntatori in cache
          
             CodeThumbFunc = ClassInfoAddr->ThumbFunction;
             // Copia dentro il puntatore a funzione CodeThumbFunc l'indirizzo della funzione di thumb
             // della classe trovata                            
          
             ErrRep=(*CodeThumbFunc) (&NewThumb, CompleteName, MyFMData->DimIconBox-1, MyFMData->DimIconBox-1);
             // Tenta di creare la nuova Thumbnail  
             
             if (ErrRep==0)     // Sono riuscito a creare la Thumbnail
             {
                INTERNAL_T1_SaveThumbnailInCache (MyFMData, &NewThumb, CompleteName); 
 
                INTERNAL_T1_ShowThumbnailOnScreen (&NewThumb, TPosX + MyFMData->PosIconBoxX, 
                                                TPosY + MyFMData->PosIconBoxY, XWndHandle);
                // Visualizza l'immagine
             
                CanMakeThumb=1;
             }
             else               // Non è stato possibile creare la thumbs. 
             {                
                ndIMG_ReleaseImage (&NewThumb);                
                CanMakeThumb=0;            
             }
        }     
        else   // Opera sulla Thumbnail cacherizzata
        {
               INTERNAL_T1_ShowThumbnailOnScreen (&NewThumb, TPosX + MyFMData->PosIconBoxX, 
                                                  TPosY + MyFMData->PosIconBoxY, XWndHandle);
               // Visualizza l'immagine
               
               CanMakeThumb=1;
        } 
     }
     else
     {
         CanMakeThumb=0;               // Se il puntatore thumb è a 0, ovviamente CanMakeThumb è FALSE
     }
     
     /*
         Se non è stato possibile creare una thumb, si visualizza un'icona
     */
             
     if (!CanMakeThumb)                // Se non è stato possibile creare una thumb
     {        
        ErrRep=ndDB_GetIconAddrFromList (&SystemIcons, ClassInfoAddr->StringIDIcon, &PointerToRamIcon);
        // Previene un malfunzionamento del compilatore
        
        InternalIconAddr=PointerToRamIcon;
        //Copia in InternalIcon le informazioni relative all'icona
     
        if (ErrRep==0)             // E' stata trovata un'icona con la StringID corretta
        {  
           ndIMG_ShowIcon (InternalIconAddr, TPosX + 5 + MyFMData->PosIconBoxX, TPosY + 5 + MyFMData->PosIconBoxY, XWndHandle); 
        }
     }
       
  }                             
  else        // Non è stata trovata una classe compatibile
  {
       ErrRep=ndDB_GetIconAddrFromList (&SystemIcons, IDQuestionPoint, &PointerToRamIcon);
       // Previene un malfunzionamento del compilatore
       
       InternalIconAddr = PointerToRamIcon;
       //Ora in PointerToIcon c'è il puntatore all'icona
     
       if (!ErrRep)
       {
          ndIMG_ShowIcon  (InternalIconAddr, TPosX + 5 + MyFMData->PosIconBoxX, TPosY + 5 + MyFMData->PosIconBoxY, XWndHandle);                     
       }          
  }   
}


void INTERNAL_T1_DrawFolderIcon (struct T1FMData_Type *MyFMData, unsigned short int TPosX, unsigned short int TPosY)
{
  struct ELEMENT_Icon_Type *InternalIconAddr;                                    
  char XWndHandle;
  
  XWndHandle = MyFMData->WndHandle1;
        
  ndDB_GetIconAddrFromList (&SystemIcons, IDFolderIcon, &InternalIconAddr);
  ndIMG_ShowIcon  (InternalIconAddr, TPosX + 5 + MyFMData->PosIconBoxX, TPosY + 5 + MyFMData->PosIconBoxY, XWndHandle);                       
}  


void INTERNAL_T1_DrawFolderExitIcon (struct T1FMData_Type *MyFMData, unsigned short int TPosX, unsigned short int TPosY)
{
  struct ELEMENT_Icon_Type *InternalIconAddr;                                    
  char XWndHandle;
  
  
  XWndHandle = MyFMData->WndHandle1;
        
  ndDB_GetIconAddrFromList (&SystemIcons, IDFolderExitIcon, &InternalIconAddr);
  ndIMG_ShowIcon  (InternalIconAddr, TPosX + 5 + MyFMData->PosIconBoxX, TPosY + 5 + MyFMData->PosIconBoxY, XWndHandle);                       
}  



void INTERNAL_T1_ShowSingleElement (struct T1FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList,
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
   char YouCanExit;
   char CharToWrite;

   char InternalBuffer [255];
   char CompleteNameBuffer [255];
   unsigned char Counter;

   unsigned short int Box_X1, Box_Y1, Box_X2, Box_Y2;
   unsigned short int ReferX, ReferY;
   
   unsigned short int PosVCursX, PosVCursY;
   unsigned short int FileInKb;

   XWndHandle = MyFMData->WndHandle1;

   ndWS_DrawRectangle     (MyFMData->C2_X1, MyFMData->C2_Y1 + MyFMData->LenBoxY*(Offset+0), 
                           MyFMData->C2_X2, MyFMData->C2_Y1 + MyFMData->LenBoxY*(Offset+1),
                           XColor, COLOR_WHITE, MyFMData->WndHandle1, NORENDER);

   Box_X1 = MyFMData->C2_X1;
   Box_Y1 = MyFMData->C2_Y1 + MyFMData->LenBoxY*(Offset+0);
   Box_X2 = MyFMData->C2_X2;
   Box_Y2 = MyFMData->C2_Y2 + MyFMData->LenBoxY*(Offset+1);
    
   ReferX = Box_X1;
   ReferY = Box_Y1;
                       
   ndDB_GetFileFromList (MyFileList, FirstElement+Offset, &FileInList);
   // Preleva i dati relativi al file di interesse
             
   switch (FileInList.IsDir)
   {
        case 0:             // L'elemento non è una directory
        {
             strcpy (InternalBuffer, FileInList.Name);                  // Copia il nome del file nel buffer interno
             fssAppendExtension (InternalBuffer, FileInList.Ext);          // Appende l'estensione
     
             strcpy (CompleteNameBuffer, ndFileManCurrPath);
             fssPutSlashAtEnd (CompleteNameBuffer);
             strcat (CompleteNameBuffer, InternalBuffer);               // In complete name buffer c'è il nome 
                                                                        // completo di estensione
     
             INTERNAL_T1_DrawThumbnailOrIcon (MyFMData, ReferX, ReferY, CompleteNameBuffer, 
                                              FileInList.Ext);
     
             YouCanExit=0;
             PosVCursX = MyFMData->FirstColumnPosX;
             PosVCursY = MyFMData->FirstRowPosY;
     
             for (Counter=0; ((Counter<MyFMData->MaxLenNmFile) && (YouCanExit==0)); Counter++)
             {
                CharToWrite = InternalBuffer [Counter];
           
                ndWS_GrWriteChar (ReferX + PosVCursX, ReferY + PosVCursY, CharToWrite, COLOR_WHITE, XColor, XWndHandle, NORENDER);
                PosVCursX=PosVCursX+8;
           
                if (CharToWrite==0) YouCanExit=1;
             }
       
             PosVCursX = MyFMData->FirstColumnPosX;
             PosVCursY = MyFMData->SecondRowPosY;
       
             if (FileInList.DateTime.available)
             {
                fssStringDate ( (FileInList.DateTime.day), (FileInList.DateTime.month), (FileInList.DateTime.year), &StrData);
                strcpy (InternalBuffer, "Date ");
                strcat (InternalBuffer, StrData);
                ndWS_GrWriteLn (ReferX + PosVCursX, ReferY + PosVCursY, InternalBuffer, COLOR_BLUE, XColor, XWndHandle, NORENDER);
             }
       
             if (FileInList.Size>1024)
             {
                FileInKb=FileInList.Size/1024;
         
                StringNumber (FileInKb, StrNumero);
                strcpy (InternalBuffer, StrNumero);
                strcat (InternalBuffer, " kb"); 
         
                PosVCursX = MyFMData->SystemArea_X2 - strlen (InternalBuffer)*8;
                PosVCursY = MyFMData->SecondRowPosY;
         
                ndWS_GrWriteLn (ReferX + PosVCursX, ReferY + PosVCursY, InternalBuffer, COLOR_BLUE, XColor, XWndHandle, NORENDER);
             }
             else
             {
                StringNumber (FileInList.Size, StrNumero);
                strcpy (InternalBuffer, StrNumero);
         
                PosVCursX = MyFMData->SystemArea_X2 - strlen (InternalBuffer)*8;
                PosVCursY = MyFMData->SecondRowPosY;
         
                ndWS_GrWriteLn (ReferX + PosVCursX, ReferY + PosVCursY, InternalBuffer, COLOR_BLUE, XColor, XWndHandle, NORENDER);  
             }  
         
             break;
        }
   
        case 1:
        {
              strcpy (InternalBuffer, FileInList.Name);                  // Copia il nome del file nel buffer interno
              fssAppendExtension (InternalBuffer, FileInList.Ext);          // Appende l'estensione
   
              INTERNAL_T1_DrawFolderIcon (MyFMData, ReferX, ReferY);
     
              YouCanExit=0;
              PosVCursX = MyFMData->FirstColumnPosX;
              PosVCursY = MyFMData->FirstRowPosY;
     
              for (Counter=0; ((Counter<MyFMData->MaxLenNmFile) && (YouCanExit==0)); Counter++)
              {
                  CharToWrite = InternalBuffer [Counter];
           
                  ndWS_GrWriteChar (ReferX + PosVCursX, ReferY + PosVCursY, CharToWrite, COLOR_WHITE, XColor, XWndHandle, NORENDER);
                  PosVCursX=PosVCursX+8;
           
                  if (CharToWrite==0) YouCanExit=1;
              }
       
              PosVCursX = MyFMData->FirstColumnPosX;
              PosVCursY = MyFMData->SecondRowPosY;
       
              if (FileInList.DateTime.available)
              {
                 fssStringDate ( (FileInList.DateTime.day), (FileInList.DateTime.month), (FileInList.DateTime.year), &StrData);
                 strcpy (InternalBuffer, "Date ");
                 strcat (InternalBuffer, StrData);
                 ndWS_GrWriteLn (ReferX + PosVCursX, ReferY + PosVCursY, InternalBuffer, COLOR_BLUE, XColor, XWndHandle, NORENDER);
              }

              break;
        }  
   
        case 2:
        {
             INTERNAL_T1_DrawFolderExitIcon (MyFMData, ReferX, ReferY);
     
             PosVCursX = MyFMData->FirstColumnPosX;
             PosVCursY = MyFMData->FirstRowPosY;
  
             ndWS_GrWriteLn (ReferX + PosVCursX, ReferY + PosVCursY, "Previous directory", COLOR_BLUE, XColor, XWndHandle, NORENDER); 

             break;
        }
   }            
   
   if (ReDrawButton)
  {
     if (FileInList.IsDir==0)
        INTERNAL_T1_DrawMainButton (MyFMData, 0);
     else
        INTERNAL_T1_DrawMainButton (MyFMData, 1);
  }
   
}



void INTERNAL_T1_ShowFileList (struct T1FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int FirstElement)
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
 
  // Ed ora eseguiamo dei calcoli

  if (FirstElement > 0) ArrowUp=1;
     else ArrowUp=0;

  LastElement=FirstElement + MyFMData->NrItemsVisualizable - 1;

  if (LastElement >= (  ((MyFileList->NrElements)-1)   )  )
  {
     LastElement=(MyFileList->NrElements - 1);
     ArrowDown=0;
  }
  else ArrowDown=1;

  MyFMData->LastElementVisualized = LastElement;

  // Disegniamo le freccie in alto ed in basso.
  INTERNAL_T1_DrawArrowUpInListFile   (MyFMData, ArrowUp);
  INTERNAL_T1_DrawArrowDownInListFile (MyFMData, ArrowDown);

  NrItemsVisualizable = LastElement - FirstElement + 1;

  for (Counter=0; Counter<NrItemsVisualizable; Counter++)
  {
    INTERNAL_T1_ShowSingleElement (MyFMData, MyFileList, FirstElement, Counter, COLOR_GRAY, 0);
  }

  XWindowRender (XWndHandle);
}


void INTERNAL_T1_EvidenceFileItem (struct T1FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int PointerToItem, int FirstItemInFocus, char ReDrawButton)
/*
   Usata solo per usi interni e non accessibile dall'esterno.

   Si occupa di evidenziare un singolo elemento del menù dei file
*/

{
  int ItemPos;

  ItemPos=PointerToItem - FirstItemInFocus;
  INTERNAL_T1_ShowSingleElement (MyFMData, MyFileList, FirstItemInFocus, ItemPos, COLOR_LBLUE, ReDrawButton);

  XWindowRender (MyFMData->WndHandle1);
}


void INTERNAL_T1_DeEvidenceFileItem (struct T1FMData_Type *MyFMData, struct HEADLIST_File_Type *MyFileList, int PointerToItem, int FirstItemInFocus, char ReDrawButton)
/*
   Usata solo per usi interni e non accessibile dall'esterno.

   Si occupa di deevidenziare un singolo elemento del menù dei file
*/

{
  int ItemPos;

  ItemPos=PointerToItem - FirstItemInFocus;
  INTERNAL_T1_ShowSingleElement (MyFMData, MyFileList, FirstItemInFocus, ItemPos, COLOR_GRAY, ReDrawButton);

  XWindowRender (MyFMData->WndHandle1);
}




char INTERNAL_T1_FileManagerChDir (char *ShortNameNewDir, char *ShortExtNewDir)
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

       SlashPos=Counter;
       // In Counter c'è la posizione del penultimo slash

       for (Counter=SlashPos+1; Counter<OldPathLen; Counter++)
       {
           ndFileManCurrPath [Counter] = 0;
       }

       return 0;
   }

}


char INTERNAL_T1_FileManagerChooseFile (char *ShortNameDir, char *ShortExtDir, char *OutputFileName)
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
   strcat (InternalString, ShortNameDir);                 // Esegue l'append dentro InternalString del nome file
   fssAppendExtension (InternalString, ShortExtDir);  // Appende l'estensione

   strcpy (OutputFileName, InternalString);               // Copia la stringa completa nell'output
   return 0;
}




char INTERNAL_T1_FileManagerNewFile (char *FileOutputAddr)
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


char INTERNAL_T1_FileManagerEraseFile (char *ShortNameFile, char *ShortExtFile)
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


char INTERNAL_T1_FileManagerRmDir (char *ShortNameDir, char *ShortExtDir)
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







char INTERNAL_T1_FileManagerMkDir (void)
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








char INTERNAL_T1_FileManagerRename (char *ShortNameFile, char *ShortExtFile, char IsDir)
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


char INTERNAL_T1FileManager (char *Title, unsigned char SaveMode, char *StringOutputAddr)
{
 struct T1FMData_Type       FMData;
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
 
 char CheckStringID  [32];
 char CheckNrItemStr [32];
 int  LogicalButtonPressed, ItemPressed;
 
 FMData.SaveMode=SaveMode;
 // Memorizza il parametro SaveMode per le routine che disegnano i pulsanti sullo schermo

 ErrWndAlloc = INTERNAL_T1_DrawFMWindows (&FMData, Title);
 // Disegna il file manager con i pulsanti
 
 if (ErrWndAlloc<0)
 {
   return ERR_NO_WINDOW_IS_AVAILABLE;              // Tronca subito l'esecuzione se non ci sono finestre
 }                                                 // disponibili

 IsFirstChDir=1;
 ErrOutput=0;
 YouCanExitLev1=0;
 
 INTERNAL_T1_ClrActiveArea (&FMData);
     
           
 while (!YouCanExitLev1)
 {
       INTERNAL_T1_DrawSysPathBar (&FMData);

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

           FirstItemInFocus=0;
           PointerToItem=0;

           INTERNAL_T1_ClrBoxArea (&FMData);      
           
           INTERNAL_T1_ShowFileList (&FMData, &FileList, FirstItemInFocus);
           INTERNAL_T1_UpdateChDirPointPointButton (&FMData);
           INTERNAL_T1_EvidenceFileItem (&FMData, &FileList, 0, FirstItemInFocus, 1);

           YouCanExitLev2=0;

           while (!YouCanExitLev2)
           {  
              MouseControl (&Event);
              
              if ( (Event.__TypeEvent==CUSTOM_BUTTON_PRESSED) && (Event.__WindowID==FMData.WndHandle1))
              {

                      if (Event.__ButtonID==FMData.ButtonHandle_ScrollFileUp)
                      {
                         // E' stato premuto il pulsante per scrollare in alto i file
        
                         HypPointerToItem=PointerToItem-1;
        
                         if (   HypPointerToItem>=0   )
                         {
                             INTERNAL_T1_DeEvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus, 1);
        
                             if (HypPointerToItem >= FirstItemInFocus )
                             {
                                 PointerToItem = HypPointerToItem;
                                 INTERNAL_T1_EvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus, 1);
                             }
                             else
                             {
                                 FirstItemInFocus--;
                                 INTERNAL_T1_ShowFileList (&FMData, &FileList, FirstItemInFocus);
        
                                 PointerToItem = HypPointerToItem;
                                 INTERNAL_T1_EvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus, 1);
                             }
                         }
                      }
        
        
                      if ( Event.__ButtonID==FMData.ButtonHandle_ScrollFileDown )
        		      {
                         // E' stato premuto il pulsante per scrollare in basso i file
        
                         HypPointerToItem=PointerToItem+1;
        
                         if (   HypPointerToItem<=( (FileList.NrElements)-1 )    )
                         {
                            INTERNAL_T1_DeEvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus, 1);
        
                            if (HypPointerToItem <= FMData.LastElementVisualized )
                            {
                               PointerToItem = HypPointerToItem;
                               INTERNAL_T1_EvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus, 1);
                            }
                            else
                            {
                               FirstItemInFocus++;
                               INTERNAL_T1_ShowFileList (&FMData, &FileList, FirstItemInFocus);
        
                               PointerToItem = HypPointerToItem;
                               INTERNAL_T1_EvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus, 1);
                            }
                         }
                      }
                      
                      
                      if ( Event.__ButtonID==FMData.ButtonHandle_ChDirPointPoint )
                      {
                         INTERNAL_T1_FileManagerChDir ("..", "");
                         YouCanExitLev2=1;        // Questo provocherà l'uscita dal livello di loop più interno
                                                  // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                                  // data dal nuovo ndFileManCurrPath  
                      }     
                      
                      if (SaveMode)
                      {
                                   
                              if ( Event.__ButtonID==FMData.ButtonHandle_New )
                              {
                                 ErrRep2 = INTERNAL_T1_FileManagerNewFile (StringOutputAddr);
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
                                 ErrRep2 = INTERNAL_T1_FileManagerMkDir ();
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
                
                                    ErrRep2 = INTERNAL_T1_FileManagerEraseFile (&SingleFile.Name, &SingleFile.Ext);
                
                                    if (ErrRep2==0)
                                    {
                                       YouCanExitLev2=1;     // Questo provocherà l'uscita dal livello di loop più interno
                                                             // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                                             // con i cambiamenti aggiornati
                                    }
                                 }
                
                                 if (SingleFile.IsDir==1)      // Si sta cercando di cancellare una directory
                                 {
                
                                    ErrRep2 = INTERNAL_T1_FileManagerRmDir (&SingleFile.Name, &SingleFile.Ext);
                
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
                                    ErrRep2 = INTERNAL_T1_FileManagerRename (&SingleFile.Name, &SingleFile.Ext, SingleFile.IsDir);
                
                                    if (ErrRep2==0)
                                    {
                                       YouCanExitLev2=1;     // Questo provocherà l'uscita dal livello di loop più interno
                                                             // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                                             // con i cambiamenti aggiornati
                                    }
                                 }
                              }
        
                      }       // End of (if SaveMode)
              
                      // ROUTINE DI SCELTA
                      // Verifichiamo se è stato premuto il MainButton
                      
                      if ( Event.__ButtonID==FMData.ButtonHandle_MainButton )
                      {
                         ndDB_GetFileFromList (&FileList, PointerToItem, &SingleFile);
        
                         if (SingleFile.IsDir!=0)      // Si sta cercando di cambiare una directory
                         {
                            INTERNAL_T1_FileManagerChDir (&SingleFile.Name, &SingleFile.Ext);
                            YouCanExitLev2=1;     // Questo provocherà l'uscita dal livello di loop più interno
                                                  // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                                  // data dal nuovo ndFileManCurrPath
                         }
                         else                        // Si è selezionato un file
                         {
                            INTERNAL_T1_FileManagerChooseFile (&SingleFile.Name, &SingleFile.Ext, StringOutputAddr);
                            YouCanExitLev2=1;
                            YouCanExitLev1=1;
                            ErrOutput=0;             // Codice di errore 0 in uscita dal File Manager
                         }
                      }
                      
                      // Adesso vediamo se è stato premuto un pulsante logico situato su uno slot
                      
                      ndStrCpyBounded (CheckStringID, Event.__StringID, 0, 7, 0);
                      
                      if (strcmp (CheckStringID, "#KEYITEM")==0)        // E' stato premuto        
                      {
                         ndStrCpyBounded (CheckNrItemStr, Event.__StringID, 8, strlen (CheckStringID), 0);
                         LogicalButtonPressed = atoi (&CheckNrItemStr);
                         
                         // Questo è l'elemento che è stato scelto      
                         ItemPressed = FirstItemInFocus + LogicalButtonPressed;
                         
                         // Se l'elemento che è stato premuto è diverso da quello che era correntemente
                         // puntato, deevidenzia l'elemento che era selezionato ed evidenzia il
                         // nuovo elemento
                         
                         if (ItemPressed!=PointerToItem) 
                         {
                            INTERNAL_T1_DeEvidenceFileItem (&FMData, &FileList, PointerToItem, FirstItemInFocus, 0);
                            INTERNAL_T1_EvidenceFileItem (&FMData, &FileList, ItemPressed, FirstItemInFocus, 0);
                         }
                         
                         INTERNAL_T1_DeEvidenceFileItem (&FMData, &FileList, ItemPressed, FirstItemInFocus, 0);
                         // Evidenziare e deevidenziare da un feedback visivo della avvenuta scelta all'utente
                           
                         ndDB_GetFileFromList (&FileList, ItemPressed, &SingleFile);
        
                         if (SingleFile.IsDir!=0)      // Si sta cercando di cambiare una directory
                         {
                            INTERNAL_T1_FileManagerChDir (&SingleFile.Name, &SingleFile.Ext);
                            YouCanExitLev2=1;     // Questo provocherà l'uscita dal livello di loop più interno
                                                  // In tal modo sarà eseguito il loop di tipo 1 e sarà visualizzata la directory
                                                  // data dal nuovo ndFileManCurrPath
                         }
                         else                        // Si è selezionato un file
                         {
                            INTERNAL_T1_FileManagerChooseFile (&SingleFile.Name, &SingleFile.Ext, StringOutputAddr);
                            YouCanExitLev2=1;
                            YouCanExitLev1=1;
                            ErrOutput=0;             // Codice di errore 0 in uscita dal File Manager
                         }  
                      }
                      
                      
              
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


  INTERNAL_DestroyThumbNailCache (&FMData);       // Disalloca la cache Thumbnail


  ndLP_DestroyWindow (FMData.WndHandle1);         // Cancella la finestra del file manager

  if (ErrOutput!=0) strcpy (StringOutputAddr, "");    // Se c'è un errore la stringa restituita è comunque vuota per sicurezza

  return ErrOutput;

}
