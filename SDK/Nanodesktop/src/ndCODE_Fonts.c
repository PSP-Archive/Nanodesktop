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
    GESTIONE FONT
    ----------------------------------------------------------------------------
*/                   

#include <$_NDforND.h>

short int INTERNAL_NTF_RegisterFont (char *Data, char System, char Graphic, char Static);
short int INTERNAL_TTF_RegisterFont (char *FntName);

/*
    ----------------------------------------------------------------------------
    INIZIALIZZAZIONE DEL SISTEMA
    ----------------------------------------------------------------------------
*/  




void ndInitFont (void)
{
     int Counter;
     
     #ifdef ND_ENABLE_TTF_SUPPORT
             FT_Init_FreeType( &TTF_Library ); 
     #endif
    
     
     _FntHandle_Console=INTERNAL_NTF_RegisterFont (&CONSOLE_font, 1, 0, 1);
   
     #ifndef NO_TERMINATOR_FONT
          _FntHandle_Terminator=INTERNAL_NTF_RegisterFont (&TERMINATOR_font, 1, 0, 1);
     #else
          _FntHandle_Terminator=0;
     #endif
     
     #ifndef NO_SASTEROIDS1_FONT
          _FntHandle_SAsteroids1=INTERNAL_NTF_RegisterFont (&SASTEROIDS1_font, 1, 0, 1);
     #else
          _FntHandle_SAsteroids1=0;
     #endif
         
     #ifndef NO_SASTEROIDS2_FONT
          _FntHandle_SAsteroids2=INTERNAL_NTF_RegisterFont (&SASTEROIDS2_font, 1, 0, 1);
     #else
          _FntHandle_SAsteroids2=0;
     #endif
     
     
     // Registra i fonts di default
     
     ndLL_CurrentFntHandle = _FntHandle_Console; 
     ndBS_CurrentFntHandle = _FntHandle_Console;
     
     for (Counter=0; Counter<__NRWINDOWSALLOCABLE; Counter++)
     {
         WindowData [Counter].ndLP_CurrentFntHandle = _FntHandle_Console;  
     }
}


/*
    ----------------------------------------------------------------------------
    SUPPORTO FONTS NTF
    ----------------------------------------------------------------------------
*/  

short int INTERNAL_NTF_LoadFont (char *FntName)
/*
   Questa routine è esclusivamente per usi interni. 
   
   Essa provvede a caricare un font di tipo NTF. La routine restituisce l'handle
   che il sistema ha assegnato al font.
   
   Se il valore restituito è negativo, ciò significa che si è verificato un errore.
   Sono previsti i seguenti codici di errore:
        
   ERR_FILE_NOT_FOUND              Il file NTF non è stato trovato
   ERR_INTERNAL_ERROR              C'è stato un errore nella lettura del file NTF
   ERR_IS_NOT_VALID_NTF_FILE       Non è un file NTF valido
   ERR_MEMORY_OVERFLOW             La memoria non è sufficiente     
*/
{
    int FileHandle;
    char NTFHandleStr [30];
    int Counter;
   
    static unsigned char CharRead;
    char NrCharsRead;
   
    char *MemoryHandle;
    short int RetFntHandle;
   
   
    FileHandle = fopen (FntName, "r");
   
    if (FileHandle<=0) goto FileError0;       // Problemi nell'apertura del file
   
    strcpy (NTFHandleStr, "NANODESKTOP_TEXT_FONT");
    
    for (Counter=0; Counter<strlen (NTFHandleStr); Counter++)
    {
        NrCharsRead=fread (&CharRead, 1, 1, FileHandle);
        if (NrCharsRead!=1) goto FileError1;
        
        // Adesso confronta il carattere che hai letto con il corrispondente nella
        // NTFHandleStr
        
        if ( *(NTFHandleStr+Counter) != CharRead ) goto FileError2;
        // Il file non ha l'handle corretto: forse non è un NTF?           
    }
    
    // Arrivati a questo punto, vuol dire che l'handle è corretto
    
    MemoryHandle = malloc  (2048);
    
    if ( (int)(MemoryHandle)<0 ) goto FileError3;
    // Problemi con l'allocazione della memoria
    
    for (Counter=0; Counter<2048; Counter++)
    {
        NrCharsRead=fread (&CharRead, 1, 1, FileHandle);
        if (NrCharsRead!=1) goto FileError4;
        
        *(MemoryHandle+Counter)=CharRead;
        // Copia il carattere nel buffer appena creato
    }
    
    // Se il sistema è arrivato a questo punto, vuol dire che il font è
    // stato interamente portato in memoria
    
    fclose (FileHandle);        // Intanto chiudiamo il file
    
    RetFntHandle = INTERNAL_NTF_RegisterFont (MemoryHandle, 0, 0, 0);
    // Cerchiamo di registrare il nuovo font. Vediamo che handle ha restituito 
    // Nanodesktop
    
    if (RetFntHandle<0) goto FntRegisterError;        
    // C'è stato un errore nella registrazione del font
    
    return RetFntHandle;         // Ritorna il font handle alla routine chiamante
    

   // Da questo punto c'è il codice per la gestione degli errori
   
   FileError0:
      return ERR_FILE_NOT_FOUND;      

   FileError1:
     fclose (FileHandle);
     return ERR_INTERNAL_ERROR;
     
   FileError2:
     fclose (FileHandle);
     return ERR_IS_NOT_VALID_NTF_FILE;     
     
   FileError3:                             // Errore di memoria
     fclose (FileHandle);
     return ERR_MEMORY_OVERFLOW;
     
   FileError4:                             // C'è stato un errore nella lettura dell'NTF
     free (MemoryHandle);
     fclose (FileHandle);
     return ERR_INTERNAL_ERROR;
     
   FntRegisterError:                       // C'è stato un errore nella registrazione del font
     free (MemoryHandle);            // Libera la memoria che era stata riservata al candidato font
     return RetFntHandle;                  // In realtà riporterà alla routine chiamante il codice di
                                           // errore della routine di registrazione
}


short int INTERNAL_NTF_RegisterFont (char *Data, char System, char Graphic, char Static)
/*
   Questa routine è esclusivamente per usi interni. 
   
   Essa provvede a registrare nel sistema Nanodesktop un nuovo font. In uscita,
   viene restituito il FontHandle assegnato da Nanodesktop.
   
   Il font handle del sistema è sempre un numero maggiore o uguale a 1. A 
   differenza di ndFONT_RegisterNtfFont, questa routine è in grado di registrare
   anche font di sistema. Sono previsti i seguenti codici di errore.
   
   ERR_NO_FONT_RECORD_IS_AVAILABLE      Non sono disponibili ulteriori font records
*/

{
   char Counter;
   char IHaveFound;
   
   IHaveFound=0;
   
   for (Counter=1; (Counter<NRFONTS_USABLE) && (!IHaveFound); Counter++)
   {
       if (!FontHandlesArray [Counter].IsActive)
       {
          IHaveFound=1;
          break;
       }
   }
   
   if (!IHaveFound) return ERR_NO_FONT_RECORD_IS_AVAILABLE;
   
   // Registra le informazioni necessarie
   
   FontHandlesArray [Counter].IsActive=1;
   FontHandlesArray [Counter].features=0;
   
   if (Graphic) FontHandlesArray [Counter].features = FontHandlesArray [Counter].features | NDGRAPHICFONT;
   if (System)  FontHandlesArray [Counter].features = FontHandlesArray [Counter].features | NDSYSTEMFONT;
   if (Static)  FontHandlesArray [Counter].features = FontHandlesArray [Counter].features | NDSTATICFONT;
   
   FontHandlesArray [Counter].Data       = Data;
   FontHandlesArray [Counter].IsTrueType = 0;
   
   return Counter;  // Ritorna l'handle al programma chiamante
}


char INTERNAL_NTF_DestroyFont (char FntHandle)
/*
    Questa routine si occupa di deregistrare un font NTF dalla memoria. L'handle del font 
    viene disattivato, e la memoria che contiene il font viene cancellata. 
    
    All'utente non è consentito deregistrare i font di sistema. Gli unici font che 
    possono essere deregistrati sono quelli caricati con ndFONT_LoadFont. 
    
    La routine restituisce i seguenti codici di errore:
               
      ERR_FONTHDL_IS_NOT_ACTIVE       Il font handle non è attivo
      ERR_I_CANNOT_DESTROY_THE_FONT   Il font è statico o è un font di sistema
                  
*/
{
   char Static, System; 
   
   if ((FontHandlesArray [FntHandle].features & NDSTATICFONT) == NDSTATICFONT)
      Static=1;
   else
      Static=0;
      
   if ((FontHandlesArray [FntHandle].features & NDSYSTEMFONT) == NDSYSTEMFONT)
      System=1;
   else
      System=0;   
                     
   if ( (!Static) && (!System) )
   {
        free (FontHandlesArray [FntHandle].Data);
        // Libera l'area di memoria che prima conteneva i dati relativi al font
        
        FontHandlesArray [FntHandle].IsActive=0;
        // Disattiva il font liberando il font handle
        
        return 0;
        // Segnala alla routine chiamante che non ci sono stati errori
   }
   else return ERR_I_CANNOT_DESTROY_THE_FONT; 
}



/*
    ----------------------------------------------------------------------------
    SUPPORTO FONTS TRUE-TYPE
    ----------------------------------------------------------------------------
*/  


short int INTERNAL_TTF_LoadFont (char *FntName)
/*
   Questa routine è esclusivamente per usi interni. 
   
   Essa provvede a caricare un font di tipo True Type. La routine restituisce l'handle
   che il sistema ha assegnato al font.
   
   Se il valore restituito è negativo, ciò significa che si è verificato un errore.
   Sono previsti i seguenti codici di errore:
        
   ERR_FILE_NOT_FOUND              Il file NTF non è stato trovato
   ERR_INTERNAL_ERROR              C'è stato un errore nella lettura del file NTF
   ERR_IS_NOT_VALID_NTF_FILE       Non è un file NTF valido
   ERR_MEMORY_OVERFLOW             La memoria non è sufficiente     
*/
{
    #ifdef ND_ENABLE_TTF_SUPPORT
        
        short int RetFntHandle;
       
        if (fexist (FntName))
        {
            RetFntHandle = INTERNAL_TTF_RegisterFont (FntName);
            // Cerchiamo di registrare il nuovo font. Vediamo che handle ha restituito 
            // Nanodesktop
            
            if (RetFntHandle<0) return ERR_INTERNAL_ERROR;        
            // C'è stato un errore nella registrazione del font
            
            return RetFntHandle;         // Ritorna il font handle alla routine chiamante
        }
        else 
          return ERR_FILE_NOT_FOUND; 
            
    #else
    
        return ERR_FONT_FORMAT_NOT_SUPPORTED;
    
    #endif
}

short int INTERNAL_TTF_RegisterFont (char *FntName)
/*
   Questa routine è esclusivamente per usi interni. 
   
   Essa provvede a registrare nel sistema Nanodesktop un nuovo font. In uscita,
   viene restituito il FontHandle assegnato da Nanodesktop.
   
   Il font handle del sistema è sempre un numero maggiore o uguale a 1. A 
   differenza di ndFONT_RegisterNtfFont, questa routine è in grado di registrare
   anche font di sistema. Sono previsti i seguenti codici di errore.
   
   ERR_NO_FONT_RECORD_IS_AVAILABLE      Non sono disponibili ulteriori font records
   ERR_INTERNAL_ERROR                   Errore interno
*/

{
   #ifdef ND_ENABLE_TTF_SUPPORT
   
           char Counter;
           char IHaveFound;
           int  TTFError;
           
           IHaveFound=0;
           
           for (Counter=1; (Counter<NRFONTS_USABLE) && (!IHaveFound); Counter++)
           {
               if (!FontHandlesArray [Counter].IsActive)
               {
                  IHaveFound=1;
                  break;
               }
           }
           
           if (!IHaveFound) return ERR_NO_FONT_RECORD_IS_AVAILABLE;
           
           // Tenta di aprire il font con la libreria TrueType
           
           TTFError = FT_New_Face( TTF_Library, FntName, 0, &(FontHandlesArray [Counter].TTF_Face) );
           
           // Registra le informazioni necessarie
           
           if (!TTFError)
           {
              FontHandlesArray [Counter].features = NDGRAPHICFONT;
           
              FontHandlesArray [Counter].Data       = 0;
              FontHandlesArray [Counter].IsTrueType = 1;
           
              FT_Set_Pixel_Sizes ( FontHandlesArray [Counter].TTF_Face, 16, 16);
              FontHandlesArray [Counter].TTF_Size = 16;
              
              // Ritorna l'handle al programma chiamante
              FontHandlesArray [Counter].IsActive=1;
              return Counter; 
           } 
           else
             return ERR_INTERNAL_ERROR; 
              
   #else
           return ERR_FONT_FORMAT_NOT_SUPPORTED;       
   #endif
}

char INTERNAL_TTF_DestroyFont (char FntHandle)
/*
    Questa routine si occupa di deregistrare un font TrueType dalla memoria. L'handle del font 
    viene disattivato, e la memoria che contiene il font viene cancellata. 
    
    La routine restituisce i seguenti codici di errore:
               
      ERR_FONTHDL_IS_NOT_ACTIVE       Il font handle non è attivo
      ERR_I_CANNOT_DESTROY_THE_FONT   Il font è statico o è un font di sistema
                  
*/
{
    #ifdef ND_ENABLE_TTF_SUPPORT
    
        FT_Done_Face ( FontHandlesArray [FntHandle].TTF_Face );
        // Disattiva il face TTF del font 
        
        FontHandlesArray [FntHandle].IsActive=0;
        // Disattiva il font liberando il font handle
        
        return 0;
        // Segnala alla routine chiamante che non ci sono stati errori
        
    #else
       
        return ERR_FONT_FORMAT_NOT_SUPPORTED;
    
    #endif
}




/*
    ----------------------------------------------------------------------------
    API AD ALTO LIVELLO
    ----------------------------------------------------------------------------
*/  

short int ndFONT_LoadFont (char *FntName)
/*
    Questa routine si occupa di caricare in memoria un font. Attualmente sono supportati
    esclusivamente i TextFont in formato Nanodesktop TextFont (.NTF). Questi font sono
    bitmap font in formato 8x8 pixel. Se l'utente riesce a generare un array contenente
    le informazioni necessarie, può usare l'utility NTFCreator nella cartella /Tools
    per generare il file .NTF necessario. 
    
    La routine restituisce il nr. di handle che è stato assegnato al font. Se il
    valore restituito è negativo, allora vuol dire che c'è stato un errore. 
    
    Sono previsti i seguenti codici di errore:
      ERR_FONT_FORMAT_NOT_SUPPORTED    Il font è in un formato non supportato
*/

{
    char Path [255];
    char Name [255];
    char Ext [10];
    char ExtM [10];
    
    if (!ndInitSystemExecuted) ndInitSystem ();
    
    fssSplitNameFile (FntName, Path, Name, Ext);
    ndStrCpy (ExtM, Ext, strlen (Ext), 1);     // Copia l'estensione facendone il maiuscolo
    
    if (strcmp (ExtM, "NTF")==0) return INTERNAL_NTF_LoadFont (FntName);
    if (strcmp (ExtM, "TTF")==0) return INTERNAL_TTF_LoadFont (FntName);
    if (strcmp (ExtM, "FON")==0) return INTERNAL_TTF_LoadFont (FntName);
       
    // Se il controllo arriva in questo punto vuol dire che il file ha un tipo non supportato
    return ERR_FONT_FORMAT_NOT_SUPPORTED;
}

short int ndFONT_RegisterNtfFont (char *Data, char Graphic, char Static)
/*
   Questa routine consente all'utente di registrare un certo font: di esso
   deve essere presente una copia in memoria. 
   
   L'utilità di questa routine è questa: attraverso di essa, il programmatore
   può registrare un c-font che venga definito da lui come array nel proprio
   codice. In tal caso sarà utile portare ad 1 il parametro Static in modo
   da impedire a Nanodesktop la deregistrazione del font. 
   
   In uscita, viene restituito il FontHandle assegnato da Nanodesktop.
   
   Il font handle del sistema è sempre un numero maggiore o uguale a 1. Si 
   noti che, per come è stato definito il prototipo di questa routine, non
   è consentito all'utente registrare font di sistema.
   
   Sono previsti i seguenti codici di errore.
   
   ERR_NO_FONT_RECORD_IS_AVAILABLE      Non sono disponibili ulteriori font records
*/
{
   return INTERNAL_NTF_RegisterFont (Data, 0, Graphic, Static);
}

char ndFONT_DestroyFont (char FntHandle)
/*
    Questa routine si occupa di deregistrare un font dalla memoria. L'handle del font 
    viene disattivato, e la memoria che contiene il font viene cancellata. 
    
    All'utente non è consentito deregistrare i font di sistema. Gli unici font che 
    possono essere deregistrati sono quelli caricati con ndFONT_LoadFont. 
    
    La routine restituisce i seguenti codici di errore:
               
      ERR_FONTHDL_IS_NOT_ACTIVE       Il font handle non è attivo
      ERR_I_CANNOT_DESTROY_THE_FONT   Il font è statico o è un font di sistema
                  
*/
{
   if (!ndInitSystemExecuted) ndInitSystem ();
    
   if (FontHandlesArray [FntHandle].IsActive)
   {
       if (FontHandlesArray [FntHandle].IsTrueType)
       {
           return INTERNAL_TTF_DestroyFont (FntHandle);                
       }
       else
       {
           return INTERNAL_NTF_DestroyFont (FntHandle); 
       }
   }
   else return ERR_FONTHDL_IS_NOT_ACTIVE;
}




