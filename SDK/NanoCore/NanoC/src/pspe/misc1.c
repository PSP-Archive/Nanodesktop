/*
   --------------------------------------------------------------------------
   misc1.c                                      NanoC Files (PSPE)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines _ntohs _ntohl _htons _htonl
   
   Queste routine sostituiscono le rispettive che operano nel PSPSDK. Sono
   fornite per astrarre il fatto che sotto MIPS2 non sono fornite di 
   default le routines __builtin_allegrex_wsbw e __builtin_allegrex_wsbh
   ----------------------------------------------------------------------------
*/   


#include <stdio.h>

int _ntohs (int x) 
{
    //return __builtin_allegrex_wsbh(x);
    return x;
}

int _ntohl(int x)
{
    //return __builtin_allegrex_wsbw(x);
    return x;
}

int _htons(int x)
{
    //return __builtin_allegrex_wsbh(x);
    return x;
}

int _htonl(int x) 
{
    //return __builtin_allegrex_wsbw(x);
    return x;
}
