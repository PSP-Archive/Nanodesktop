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
    ---------------------------------------------------------------------------
    ROUTINE DI SUPPORTO PER IL FILE-SYSTEM
    ---------------------------------------------------------------------------
*/

#include <$_NDforND.h>
    
char ndFS_GetLongInt (int FileHandle, int *Value)
{
   unsigned char Byte1, Byte2, Byte3, Byte4;
   int NrBytesRead;
   
   NrBytesRead=ndHAL_Read (FileHandle, &Byte4, 1);
   if (NrBytesRead!=1) return -1;
   
   NrBytesRead=ndHAL_Read (FileHandle, &Byte3, 1);
   if (NrBytesRead!=1) return -1;
   
   NrBytesRead=ndHAL_Read (FileHandle, &Byte2, 1);
   if (NrBytesRead!=1) return -1;
   
   NrBytesRead=ndHAL_Read (FileHandle, &Byte1, 1);
   if (NrBytesRead!=1) return -1;
   
   *Value = 65536*256*Byte1 + 65536*Byte2 + 256*Byte3 + Byte4;       
   
   return 0;
}


inline char ndFS_GetWord (int FileHandle, unsigned short int *Value)
{
   unsigned char Byte1, Byte2;
   int NrBytesRead;
   
   NrBytesRead=ndHAL_Read (FileHandle, &Byte2, 1);
   if (NrBytesRead!=1) return -1;
   
   NrBytesRead=ndHAL_Read (FileHandle, &Byte1, 1);
   if (NrBytesRead!=1) return -1;
   
   *Value = 256*Byte1 + Byte2;       
   
   return 0;       
   
} 


inline char ndFS_GetByte (int FileHandle, char *Value)
{
   unsigned char Byte1;
   int NrBytesRead;
   
   NrBytesRead=ndHAL_Read (FileHandle, &Byte1, 1);
   if (NrBytesRead!=1) return -1;
   
   *Value = Byte1;       
   
   return 0;       
}


inline int ndFS_WriteChar (int FileHandle, char Carattere)
/*
   Questa routine scrive su disco il byte fornito. 
  
   Se non sono presenti errori, restituisce in uscita NO_FILE_ERR, altrimenti
   restituisce FILE_ERR (=1)
*/

{
    int NrBytesWr;
    
    NrBytesWr = ndHAL_Write (FileHandle, &Carattere, 1);
    
    if (NrBytesWr == 1)     return NO_FILE_ERR;
       else return FILE_ERR;
}

inline int ndFS_WriteWord (int FileHandle, unsigned short int Word)
/* 
   Questa routine scrive su disco il numero intero a 16 bit fornito. La
   scrittura avviene in notazione little-endian, compatibile con lo standard
   usato dai processori x86.
  
   Se non sono presenti errori, restituisce in uscita NO_FILE_ERR, altrimenti
   restituisce FILE_ERR (=1)
*/

{
    unsigned char ByteHi, ByteLo;
    
    ByteHi = Word / 256;
    ByteLo = Word % 256;

    if (ndFS_WriteChar (FileHandle, ByteLo)) return FILE_ERR;
    if (ndFS_WriteChar (FileHandle, ByteHi)) return FILE_ERR;
    
    return NO_FILE_ERR;
}



inline int ndFS_WriteDWord (int FileHandle, unsigned int DWord)
/* 
   Questa routine scrive su disco il numero intero a 32 bit fornito. La
   scrittura avviene in notazione little-endian, compatibile con lo standard
   usato dai processori x86.
  
   Se non sono presenti errori, restituisce in uscita NO_FILE_ERR, altrimenti
   restituisce FILE_ERR (=1)
*/
{
    unsigned char Byte1, Byte2, Byte3, Byte4;
    
    Byte1 = (DWord / (256*256)) / 256;
    Byte2 = (DWord / (256*256)) % 256;
    Byte3 = (DWord % (256*256)) / 256;
    Byte4 = (DWord % (256*256)) % 256;
    
    if (ndFS_WriteChar (FileHandle, Byte4)) return FILE_ERR;
    if (ndFS_WriteChar (FileHandle, Byte3)) return FILE_ERR;
    if (ndFS_WriteChar (FileHandle, Byte2)) return FILE_ERR;
    if (ndFS_WriteChar (FileHandle, Byte1)) return FILE_ERR;
    
    return NO_FILE_ERR;
}

unsigned int ndMEM_ReadDWordFromBuffer (char *Buffer, int Offset)
{
   unsigned char Byte1, Byte2, Byte3, Byte4;
    
   Byte4 = (unsigned char)(*(Buffer + Offset + 0));
   Byte3 = (unsigned char)(*(Buffer + Offset + 1));
   Byte2 = (unsigned char)(*(Buffer + Offset + 2));
   Byte1 = (unsigned char)(*(Buffer + Offset + 3));
          
   return (65536*256*Byte1 + 65536*Byte2 + 256*Byte3 + Byte4);
}


unsigned int ndMEM_ReadWordFromBuffer (char *Buffer, int Offset)
{
   unsigned char Byte1, Byte2;
    
   Byte2 = (unsigned char)(*(Buffer + Offset + 0));
   Byte1 = (unsigned char)(*(Buffer + Offset + 1));
          
   return (256*Byte2 + Byte1);
} 


unsigned char ndMEM_ReadByteFromBuffer (char *Buffer, int Offset)
{
   unsigned char Byte1;
   
   Byte1 = (unsigned char)(*(Buffer + Offset + 0));
   return Byte1;
}


