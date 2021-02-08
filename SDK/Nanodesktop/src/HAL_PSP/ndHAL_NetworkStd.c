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
    Network standard function (PSP)
    ----------------------------------------------------------------------------
    Questo file contiene funzioni standard per la gestione della rete come
    bind, listen, connect ecc. ecc.
    
    Le funzioni iniziano con il suffisso ndHAL_NET: questa API ha la funzione
    di coprire la differenza di prototipi tra le differenti soluzioni usate
    per il TCP-IP (software o moduli nel firmware come accade nel caso
    della PSP). Un macro wrapper in $$_NetWrapper.h provvede a wrappare le
    funzioni di tale API su un name space standard. 
    
    N.B.
    La modalità PSP non supporta, a differenza della modalità KSU, le
    funzionalità di rete, pertanto l'API viene qui mantenuta esclusivamente
    per ragioni di simmetria
    ----------------------------------------------------------------------------
*/

#include <HAL_PSP\$_HAL_NDforND.h>

static char inet_ntoa_buffer [128];

// ---------------------------------------------------------------------------
// Routines di conversione stringa/IPV4
// ---------------------------------------------------------------------------


in_addr_t ndHAL_NET_InetInetAddr (char *String)
/*
    Questa funzione provvede a convertire una string che contiene un indirizzo
    IP v4 nel corrispondente codice a 32 bit
*/
{
    return htonl (sceNetInetInetAddr (String));
    // Il firmware della PSP restituisce un indirizzo che è invertito rispetto
    // a come dovrebbe essere. Per tale ragione, si deve richiamare la routine
    // htonl per invertire l'ordine dei byte nel valore restituito    
}


char *ndHAL_NET_InetToNtoa (struct in_addr in)
{
    /*
        Deve restituire l'indirizzo di rete (è invertito rispetto all'indirizzo
        di presentazione che si presenta nel classico modo (192.168.0.1)
    */
    
    unsigned char Value1, Value2, Value3, Value4;
    
    Value1 = (in.s_addr >> 24) & 0xFF;
    Value2 = (in.s_addr >> 16) & 0xFF;
    Value3 = (in.s_addr >>  8) & 0xFF;
    Value4 = (in.s_addr >>  0) & 0xFF;
    
    sprintf (inet_ntoa_buffer, "%d.%d.%d.%d", Value4, Value3, Value2, Value1);  
    return &(inet_ntoa_buffer); 
}



// ---------------------------------------------------------------------------
// Routines di conversione non standard
// ---------------------------------------------------------------------------

void ndHAL_NET_ConvertIPv4ToString (union ndIPV4 *IPV4, char *String)
/*
    Questa routine provvede a convertire un indirizzo IPv4 in una stringa
*/
{
    sprintf (String, "%d.%d.%d.%d", IPV4->Value[3], IPV4->Value[2], IPV4->Value[1], IPV4->Value[0]);
} 

void ndHAL_NET_ConvertStringToIPv4 (char *String, union ndIPV4 *IPV4)
/*
    Questa routine provvede a convertire una stringa in un indirizzo IPv4
*/
{
    IPV4->Code32 = ndHAL_NET_InetInetAddr (String);
}

void ndHAL_NET_Convert_SockAddr__to__SockAddrIn (struct sockaddr *address, struct sockaddr_in *address_in)
/*
    Le librerie come libcurl richiamano la routine connect() passando il parametro
    per mezzo della struct sockaddr (definito in sys/socket.h), ma il firmware della 
    PSP fa uso di routines che ottengono i parametri per mezzo della struttura 
    sockaddr_in (definito in netinet/in.h). 
    
    Pertanto, è necessaria una routine che provveda alla conversione tra le due
    strutture
*/ 
{
    unsigned char ByteP0, ByteP1, ByteA0, ByteA1, ByteA2, ByteA3;
    int           Port, Address32;
    
    ByteP0 = ((unsigned char)(address->sa_data [0]));
    ByteP1 = ((unsigned char)(address->sa_data [1]));
    ByteA0 = ((unsigned char)(address->sa_data [2]));
    ByteA1 = ((unsigned char)(address->sa_data [3]));
    ByteA2 = ((unsigned char)(address->sa_data [4]));
    ByteA3 = ((unsigned char)(address->sa_data [5]));
    
    Port       = (ByteP0 <<  8) + ByteP1;
    Address32  = (ByteA0 << 24) + (ByteA1 << 16) + (ByteA2 << 8) + ByteA3;
    
    memset (address_in, 0, sizeof (struct sockaddr_in));
    address_in->sin_family      = AF_INET;
    address_in->sin_port        = htons (Port);
    address_in->sin_addr.s_addr = htonl (Address32);    
}



// ---------------------------------------------------------------------------
// Routines di gestione rete (BSD like)
// ---------------------------------------------------------------------------

int ndHAL_NET_SetupNoBlockingConnection(SOCKET s, u32 NoBlock)
/* 
    Questa routine si occupa di attivare o disattivare il trasferimento a blocchi
    sul socket s. Se si pone NoBlock pari ad 1 il trasferimento a blocchi
    viene disattivato
*/
{ 
    return -1;    
}

void ndHAL_NET_SetupStdNetworkTimeOutValue (int Limit)
/*
    Questa funzione consente di settare il valore di timeout per funzioni come connect
    ad un valore arbitrario diverso da quello di default
*/
{
    ndStdNetworkTimeOut = Limit;
}

void ndHAL_NET_EnableNetworkMonitor ()
/*
    Abilita il monitor di rete
*/
{
    return;
    // In realtà non lo attiva/disattiva. La rete è disattivata quindi non ci sarebbe
    // niente di monitorare
}

void ndHAL_NET_DisableNetworkMonitor ()
/*
    Disabilita il monitor di rete
*/
{
    return;
    // In realtà non lo attiva/disattiva. La rete è disattivata quindi non ci sarebbe
    // niente di monitorare
}




int ndHAL_NET_Accept (SOCKET s, struct sockaddr * sockaddr, int* addrlen)
{
    return -1;
}

int ndHAL_NET_Bind (SOCKET s, const struct sockaddr * addr, int namelen)
{
    return -1;
}





int ndHAL_NET_ConnectWithTimeOut (SOCKET s, const struct sockaddr *address, int addrlen, int timeout)
{ 
    return -1;
}


int ndHAL_NET_Connect (SOCKET s, const struct sockaddr *address, int addrlen)
{
    return -1;
}

int	ndHAL_NET_GetSockOpt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
    return -1;
}

int	ndHAL_NET_Listen (SOCKET s, int backlog)
{
    return -1;
}

int ndHAL_NET_Recv (SOCKET s, u8* buf, int len, int flags)
{
    return -1;
}

int ndHAL_NET_RecvFrom (SOCKET s, u8* buf, int len, int flags, void* sockaddr_from, int* fromlen)
{
    return -1;
}

ssize_t ndHAL_NET_RecvMsg (int s, struct msghdr *msg, int flags)
{
    return -1;
}

int ndHAL_NET_Send (SOCKET s, const void* buf, int len, int flags)
{
    return -1;
}

int ndHAL_NET_SendTo (SOCKET s, const void* buf, int len, int flags, const struct sockaddr *sockaddr_to, int tolen)
{
    return -1;
}

ssize_t ndHAL_NET_SendMsg (int s, const struct msghdr *msg, int flags)
{
    return -1;
}

int	ndHAL_NET_SetSockOpt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
    return -1;
}

int	ndHAL_NET_Shutdown(int s, int how)
{
    return -1;
}

int	ndHAL_NET_Socket (int domain, int type, int protocol)
{
    return -1;
}

int ndHAL_NET_Close (int s)
{
    return -1;
}

int	ndHAL_NET_Select (int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
    return -1;
}

int ndHAL_NET_GetErrno (void)
{
    return -1;
}

int ndHAL_NET_GetSockName (int s, struct sockaddr *name, socklen_t *namelen)
{
    return -1;
}

int ndHAL_NET_GetPeerName(int s, struct sockaddr *name, socklen_t *namelen)
{
    return -1;
}






int ndHAL_NET_RecvUntilFull (SOCKET s, u8* buf, int len)
/*
     Questa routine provvede a ricevere dal socket indicato fin quando 
     non viene riempito completamente il buffer di lunghezza len. 
     
     In uscita vengono restituiti i dati effettivamente letti.
*/
{
    return -1;
}


#define MAX_NAME 512

struct hostent *ndHAL_NET_GetHostByAddr (const void *addr, int len, int type)
{
	return -1;
}



struct hostent *ndHAL_NET_GetHostByName(const char *name)
{
	return -1;
}



