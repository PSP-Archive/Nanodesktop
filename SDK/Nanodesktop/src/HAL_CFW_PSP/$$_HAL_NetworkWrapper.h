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
    Wrapper per funzionalità di rete (PSP CFW mode)
    ----------------------------------------------------------------------------
*/

#ifndef _ND_NETWORK_WRAPPER_H_
      #define _ND_NETWORK_WRAPPER_H_ 
      
              #define inet_addr   ndHAL_NET_InetInetAddr
              #define inet_ntoa   ndHAL_NET_InetToNtoa 
              
              #define accept      ndHAL_NET_Accept 
              #define bind        ndHAL_NET_Bind  
              #define connect     ndHAL_NET_Connect  
              #define getsockopt  ndHAL_NET_GetSockOpt  
              #define listen      ndHAL_NET_Listen  
              #define recv        ndHAL_NET_Recv  
              #define recvfrom    ndHAL_NET_RecvFrom  
              #define recvmsg     ndHAL_NET_RecvMsg  
              #define send        ndHAL_NET_Send  
              #define sendto      ndHAL_NET_SendTo  
              #define sendmsg     ndHAL_NET_SendMsg  
              #define setsockopt  ndHAL_NET_SetSockOpt  
              #define shutdown    ndHAL_NET_Shutdown  
              #define socket      ndHAL_NET_Socket  
              #define getsockname ndHAL_NET_GetSockName  
              #define getpeername ndHAL_NET_GetPeerName  
              #define closesocket ndHAL_NET_Close
              #define select      ndHAL_NET_Select
              #define recvfull    ndHAL_NET_RecvUntilFull 
              
              #define gethostbyaddr ndHAL_NET_GetHostByAddr 
              #define gethostbyname ndHAL_NET_GetHostByName
              
              // #define ndHAL_NET_GetErrno 
                                
      
#endif     // ifndef _ND_NETWORK_WRAPPER_H
