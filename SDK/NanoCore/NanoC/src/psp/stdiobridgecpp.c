/*
   --------------------------------------------------------------------------
   $$_ded_stdiobridgecpp.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   NanoCPP usa una particolare versione delle routines di accesso al disco:
   in pratica, prevede che routines come read, write, lseek ecc. possano
   riconoscere come fd i codici degli streams stdin, stdout, stderr. 
   
   Questo non è il comportamento standard della NanoC. Pertanto, è stato
   realizzato un particolare bridge che ha la funzione di porsi tra
   le routines di accesso al disco di NanoCPP e le routines della NanoC.
   ----------------------------------------------------------------------------
*/   


#include <psp/$$_ded_stdiobridgecpp.h>

int brdcpp_open (const char *NameFile, int flags, int mode)
{
    if (!strcmp (NameFile, "stdin"))
    {
        return (int)(stdin);        
    }
    
    if (!strcmp (NameFile, "stdout"))
    {
        return (int)(stdout);        
    }
    
    if (!strcmp (NameFile, "stderr"))
    {
        return (int)(stderr);        
    }
    
    return open (NameFile, flags, mode);
} 

int brdcpp_close (int VFHNumber)
{
    if ( (VFHNumber==(int)(stdin)) || (VFHNumber==(int)(stdout)) || (VFHNumber==(int)(stderr)) )
    {
        return 0;        
    }
    
    return close (VFHNumber);
} 

int brdcpp_read (int VFHNumber, const void *Data, int Size)
{
    if (VFHNumber==(int)(stdin) )
       return fread ((void *)(Data), Size, 1, stdin);
       
    if (VFHNumber==(int)(stdout) )
       return -1;
       
    if (VFHNumber==(int)(stderr) )
       return -1;
       
    // Non si tratta degli streams standard
    
    return read (VFHNumber, (void *)(Data), Size);
}

int brdcpp_write (int VFHNumber, const void *Data, int Size)
{
    if (VFHNumber==(int)(stdin) )
       return -1;
       
    if (VFHNumber==(int)(stdout) )
       return fwrite ((void *)(Data), Size, 1, stdout);
       
    if (VFHNumber==(int)(stderr) )
       return fwrite ((void *)(Data), Size, 1, stderr);
       
    // Non si tratta degli streams standard
    
    return write (VFHNumber, (void *)(Data), Size);
}

int _fcntl (int fd, int cmd, va_list args);

int _brdcpp_fcntl (int VFHNumber, int cmd, va_list args)
{	
	if (VFHNumber==(int)(stdin) )
	{
         switch (cmd) 
         {    
              case 0x03:            // F_GETFL
              {
                   return O_RDONLY;
              }
         }
    } 
    
    if (VFHNumber==(int)(stdout) )
	{
         switch (cmd) 
         {    
              case 0x03:            // F_GETFL
              {
                   return O_WRONLY;
              }
         }                
    } 
    
    if (VFHNumber==(int)(stderr) )
	{
         switch (cmd) 
         {    
              case 0x03:            // F_GETFL
              {
                   return O_WRONLY;
              }
         }                
    }
    
    // Non si tratta degli streams standard
    
    return _fcntl (VFHNumber, cmd, args);
}    


int brdcpp_fcntl (int VFHNumber, int cmd, ...)
{
    va_list args;
	int ret;

	va_start(args, cmd);
	ret = _brdcpp_fcntl (VFHNumber, cmd, args);
	va_end(args);

	return ret;  
}

int brdcpp_fileno (FILE * f)
{
    if ((f==stdin) || (f==stdout) || (f==stderr))  return (int)(f);
    return fileno (f);
} 

int brdcpp_seek (int VFHNumber, int Offset, int Whence)
{
    if (VFHNumber==(int)(stdin) )  return -1;
    if (VFHNumber==(int)(stdout) ) return -1;
    if (VFHNumber==(int)(stderr) ) return -1;
    
    return lseek (VFHNumber, Offset, Whence); 
} 

