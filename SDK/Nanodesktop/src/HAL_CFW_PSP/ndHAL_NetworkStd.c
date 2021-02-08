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
    Network standard function (PSP CFW mode)
    ----------------------------------------------------------------------------
    Questo file contiene funzioni standard per la gestione della rete come
    bind, listen, connect ecc. ecc.
    
    Le funzioni iniziano con il suffisso ndHAL_NET: questa API ha la funzione
    di coprire la differenza di prototipi tra le differenti soluzioni usate
    per il TCP-IP (software o moduli nel firmware come accade nel caso
    della PSP). Un macro wrapper in $$_NetWrapper.h provvede a wrappare le
    funzioni di tale API su un name space standard. 
    ----------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>

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
// Routines del monitor di rete (BSD like)
// ---------------------------------------------------------------------------

#define PRINT_VALUE1   1
#define PRINT_VALUE12  2
#define PRINT_STRING   16

void INTERNAL_ShowNetMonitorMessage (char Code, char *String, int Value1, int Value2, char *StrCustom)
/*
   Questa routine provvede a visualizzare sullo schermo il messaggio per il
   monitor di rete (se attivato)
*/
{
   if (!ndNetworkMonitorIsOpen)
   {
        ndNetworkMonitorHandle = ndLP_CreateWindow (100, 100, Windows_MaxScreenX-5, Windows_MaxScreenY-5, "Network Monitor", COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_WHITE, 0);                            
        
        if (ndNetworkMonitorHandle>=0) 
        {
           ndLP_MaximizeWindow (ndNetworkMonitorHandle); 
        }
        else
        {
           return;   // Non siamo riusciti ad allocare la finestra esci          
        }
        
        ndNetworkMonitorIsOpen=1;          
   }        
   
   // Inizia a visualizzare il messaggio sullo schermo
   {
      ndWS_Print (ndNetworkMonitorHandle, COLOR_WHITE, RENDER, "%s ", String); 
      
      if (Code & PRINT_VALUE1)  ndWS_Print (ndNetworkMonitorHandle, COLOR_WHITE, RENDER, "%d ", Value1);
      if (Code & PRINT_VALUE12)  ndWS_Print (ndNetworkMonitorHandle, COLOR_WHITE, RENDER, "%d ", Value2);
      if (Code & PRINT_STRING)  ndWS_Print (ndNetworkMonitorHandle, COLOR_WHITE, RENDER, "%s ", StrCustom);
      
      ndWS_CarriageReturn (ndNetworkMonitorHandle);        
   }
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
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (PRINT_VALUE1, "BLOCK CONNECTION: ", NoBlock, 0, 0);
    
    return sceNetInetSetsockopt(s, SOL_SOCKET, 0x1009, (const char*)&NoBlock, sizeof(u32));
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
    ndNetMonitorEnabled=1;
}

void ndHAL_NET_DisableNetworkMonitor ()
/*
    Disabilita il monitor di rete
*/
{
    if (ndNetworkMonitorIsOpen) 
    {
       ndLP_DestroyWindow (ndNetworkMonitorHandle);
       ndNetworkMonitorIsOpen=0;
    }
    
    ndNetMonitorEnabled=0;
}




int ndHAL_NET_Accept (SOCKET s, struct sockaddr * sockaddr, int* addrlen)
{
    static struct sockaddr_in address_in;
    
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "ACCEPT ", 0, 0, 0);
    ndHAL_NET_Convert_SockAddr__to__SockAddrIn (sockaddr, &address_in);
    
    return sceNetInetAccept(s, &address_in, addrlen);
}

int ndHAL_NET_Bind (SOCKET s, const struct sockaddr * addr, int namelen)
{
    static struct sockaddr_in address_in;
    
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "BIND ", 0, 0, 0);
    ndHAL_NET_Convert_SockAddr__to__SockAddrIn (addr, &address_in);
    
    return sceNetInetBind (s, &address_in, namelen);
}





int ndHAL_NET_ConnectWithTimeOut (SOCKET s, const struct sockaddr *address, int addrlen, int timeout)
{ 
    static struct sockaddr_in address_in;
    int err;
    
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "CONNECT (attempt) ", 0, 0, 0);
    
    ndHAL_NET_Convert_SockAddr__to__SockAddrIn (address, &address_in);
     
    ndHAL_NET_SetupNoBlockingConnection(s, 1);
    err = sceNetInetConnect ( s, &address_in, sizeof (struct sockaddr_in) );

    if (err == 0)           // La connessione è avvenuta immediatamente: raro, ma può capitare
    {
        ndHAL_NET_SetupNoBlockingConnection(s, 0); // riattiva la connessione a blocchi
        return 0;                                  // segnala alla routine chiamante che non ci sono errori
    }
    else
    {
            if ((err == -1) && (sceNetInetGetErrno() == 0x77))
            {
                // Il processo di connessione è avviato, ma non è ancora concluso (errore 0x77).
                // E' necessario attendere fino alla fine del processo di connessione
                
                int timer;
                for (timer = 0; timer < timeout; timer += 100000)
                {
                    // Esegui nuovamente il processo di connessione, fin quando non
                    // si trova errore di connessione apparente e codice di errore
                    // 0x7F (hack del firmware PSP) oppure codice di errore 0
                    
                    err = sceNetInetConnect(s, &address_in, sizeof (struct sockaddr_in));
                    
                    if (err == 0 || (err == -1 && sceNetInetGetErrno() == 0x7F))
                    {
                        if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "CONNECT (success) ", 0, 0, 0);
                        
                        // La connessione adesso è avvenuta
                        ndHAL_NET_SetupNoBlockingConnection (s, 0); // Riattiva trasmissione a blocchi per ricezione 
                        return 0;                                   // Segnala al socket chiamante il successo
                    }
                    
                    sceKernelDelayThread (20000);
                }
                
                // Se il sistema giunge qui, c'è il TimeOut: segnalalo alla routine chiamante
                if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "CONNECT (failed) ", 0, 0, 0);
                return -1;
            }
            else
            {
               if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (PRINT_VALUE1, "CONNECT (failed): Fw error code is ", sceNetInetGetErrno(), 0, 0);
               return err;
            }
    }
}


int ndHAL_NET_Connect (SOCKET s, const struct sockaddr *address, int addrlen)
{
    return (ndHAL_NET_ConnectWithTimeOut (s, address, addrlen, ndStdNetworkTimeOut));
}

int	ndHAL_NET_GetSockOpt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "GETSOCKOPT ", 0, 0, 0);
    
    return sceNetInetGetsockopt (s, level, optname, optval, optlen);
}

int	ndHAL_NET_Listen (SOCKET s, int backlog)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "LISTEN ", 0, 0, 0);
    
    return sceNetInetListen(s, backlog);
}

int ndHAL_NET_Recv (SOCKET s, u8* buf, int len, int flags)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "RECV ", 0, 0, 0);
    
    return sceNetInetRecv (s, buf, len, flags);
}

int ndHAL_NET_RecvFrom (SOCKET s, u8* buf, int len, int flags, void* sockaddr_from, int* fromlen)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "RECV FROM ", 0, 0, 0);
    
    return sceNetInetRecvfrom (s, buf, len, flags, sockaddr_from, fromlen);
}

ssize_t ndHAL_NET_RecvMsg (int s, struct msghdr *msg, int flags)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "RECMSG ", 0, 0, 0);
    
    return sceNetInetRecvmsg (s, msg, flags);
}

int ndHAL_NET_Send (SOCKET s, const void* buf, int len, int flags)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "SEND ", 0, 0, 0);
    
    return sceNetInetSend (s, buf, len, flags);
}

int ndHAL_NET_SendTo (SOCKET s, const void* buf, int len, int flags, const struct sockaddr *sockaddr_to, int tolen)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "SENDTO ", 0, 0, 0);
    
    return sceNetInetSendto (s, buf, len, flags, sockaddr_to, tolen);
}

ssize_t ndHAL_NET_SendMsg (int s, const struct msghdr *msg, int flags)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "SENDMSG ", 0, 0, 0);
    
    return sceNetInetSendmsg (s, msg, flags);
}

int	ndHAL_NET_SetSockOpt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "SETSOCKOPT ", 0, 0, 0);
    
    return sceNetInetSetsockopt (s, level, optname, optval, optlen);
}

int	ndHAL_NET_Shutdown(int s, int how)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "SHUTDOWN ", 0, 0, 0);
    
    return sceNetInetShutdown (s, how);
}

int	ndHAL_NET_Socket (int domain, int type, int protocol)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "SOCKET ", 0, 0, 0);
    
    return sceNetInetSocket (domain, type, protocol);
}

int ndHAL_NET_Close (int s)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "CLOSE SOCKET ", 0, 0, 0);
    
    return sceNetInetClose (s);
}

int	ndHAL_NET_Select (int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "SELECT ", 0, 0, 0);
    
    return sceNetInetSelect (n, readfds, writefds, exceptfds, timeout);  
}

int ndHAL_NET_GetErrno (void)
{
    int ErrRep = sceNetInetGetErrno ();
    
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (PRINT_VALUE1, "GET ERRNO ", ErrRep, 0, 0);
    return ErrRep;
}

int ndHAL_NET_GetSockName (int s, struct sockaddr *name, socklen_t *namelen)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "GET SOCK NAME ", 0, 0, 0);
    
    return sceNetInetGetsockname (s, name, namelen);
}

int ndHAL_NET_GetPeerName(int s, struct sockaddr *name, socklen_t *namelen)
{
    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "GET PEER NAME ", 0, 0, 0);
    
    return sceNetInetGetpeername (s, name, namelen);
}






int ndHAL_NET_RecvUntilFull (SOCKET s, u8* buf, int len)
/*
     Questa routine provvede a ricevere dal socket indicato fin quando 
     non viene riempito completamente il buffer di lunghezza len. 
     
     In uscita vengono restituiti i dati effettivamente letti.
*/
{
    int n, nTotal = 0;
    
    while (len > 0)
    {
        n = sceNetInetRecv(s, buf, len, 0);  // ricevi un blocco dati
        if (n <= 0) return n;                // Se c'è un errore esci
        
        nTotal += n;
        buf += n;
        len -= n;
    }
    
    return nTotal;
}


#define MAX_NAME 512

struct hostent *ndHAL_NET_GetHostByAddr (const void *addr, int len, int type)
{
	static struct hostent ent;
	char buf[1024];
	static char sname[MAX_NAME] = "";
	static struct in_addr saddr = { 0 };
	static char *addrlist[2] = { (char *) &saddr, NULL };
	int rid;
	int err;

    if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "GET HOST BY ADDR ", 0, 0, 0);

	if((len != sizeof(struct in_addr)) || (type != AF_INET) || (addr == NULL))
	{
		h_errno = HOST_NOT_FOUND;
		return NULL;
	}

	memcpy(&saddr, addr, len);

	if(sceNetResolverCreate(&rid, buf, sizeof(buf)) < 0)
	{
		h_errno = NO_RECOVERY;
		return NULL;
	}

	err = sceNetResolverStartAtoN(rid, &saddr, sname, sizeof(sname), 2, 10);
	sceNetResolverDelete(rid);
	if(err < 0)
	{
		h_errno = HOST_NOT_FOUND;
		return NULL;
	}

	ent.h_name = sname;
	ent.h_aliases = 0;
	ent.h_addrtype = AF_INET;
	ent.h_length = sizeof(struct in_addr);
	ent.h_addr_list = addrlist;
	ent.h_addr = (char *) &saddr;

	return &ent;
}



struct hostent *ndHAL_NET_GetHostByName(const char *name)
{
	static struct hostent ent;
	char buf[1024];
	static char sname[MAX_NAME] = "";
	static struct in_addr saddr = { 0 };
	static char *addrlist[2] = { (char *) &saddr, NULL };
	int rid = -1;

	if (ndNetMonitorEnabled) INTERNAL_ShowNetMonitorMessage (0, "GET HOST BY NAME ", 0, 0, 0);
    
    if(sceNetInetInetAton(name, &saddr) == 0)
	{
		int err;

		if(sceNetResolverCreate(&rid, buf, sizeof(buf)) < 0)
		{
			h_errno = NO_RECOVERY;
			return NULL;
		}

		err = sceNetResolverStartNtoA(rid, name, &saddr, 2, 3);
		sceNetResolverDelete(rid);
		if(err < 0)
		{
			h_errno = HOST_NOT_FOUND;
			return NULL;
		}

	}

	snprintf(sname, MAX_NAME, "%s", name);
	ent.h_name = sname;
	ent.h_aliases = 0;
	ent.h_addrtype = AF_INET;
	ent.h_length = sizeof(struct in_addr);
	ent.h_addr_list = addrlist;
	ent.h_addr = (char *) &saddr;

	return &ent;
}



