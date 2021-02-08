/* 
   ------------------------------------------------------------------------
   USB Driver PRX for Nanodesktop PSP - Version for CF
   Version 1.0
   ------------------------------------------------------------------------
   This is the driver for USB support in Nanodesktop PSP (ndUsbDriver.PRX).
   
   This driver is partially based on USB HostFS Driver of PSPLINK. 
   
   PSPLink is a component under BSD license:         
   Copyright (c) 2006 James Forshaw <tyranid@gmail.com> 
   file:///home/svn/psp/trunk/psplink/usbhostfs/

   Filippo Battaglia has introduced many modifications to the Forshaw original 
   code, obtaining this Nanodesktop USB Driver. He recognizes to James Forshaw,
   all rights about the original code. 
   
   This Nanodesktop USB driver is released under BSD license. 
   ----------------------------------------------------------------------------
   FILE HEADER
   ----------------------------------------------------------------------------
*/

            

#ifndef __NDUSBDRIVER
        #define __NDUSBDRIVER
        

/*
        ------------------------------------------------------------------------
        STRUCTURES FOR HOST FILE SYSTEM
        ------------------------------------------------------------------------
        These structures need to access to Host File System (HFS), the original
        function provided by Tyranid's PSPLINK driver and that we have mantained 
        with some changes.
        ------------------------------------------------------------------------
*/

            #define ND001_MODULE_NAME "ndUSBDriver"
            #define HOSTFSDRIVER_NAME "ndUSBDriverFs"
            #define HOSTFSDRIVER_PID  (0x1C9)
            #define SONY_VID (0x54C)
            
            #define HOSTFS_MAGIC 0x782F0812
            #define ASYNC_MAGIC  0x782F0813
            #define BULK_MAGIC   0x782F0814
            
            #define HOSTFS_PATHMAX (4096)
            
            #define HOSTFS_MAX_BLOCK (64*1024)
            
            #define HOSTFS_RENAME_BUFSIZE (1024)
            
            #define HOSTFS_BULK_MAXWRITE  (1024*1024)
            
            #define HOSTFS_BULK_OPEN      (1 << 24)
            
            #define DEVCTL_GET_INFO       0x02425818
            
            struct DevctlGetInfo
            {
                    /* Total number of blocks */
                    unsigned int btotal;
                    /* Total number of free blocks */
                    unsigned int bfree;
                    /* Unknown */
                    unsigned int unk;
                    /* Sector size */
                    unsigned int ssize;
                    /* Number of sectors per block */
                    unsigned int sects;
            };
            
            enum USB_ASYNC_CHANNELS
            {
                    ASYNC_SHELL    = 0,
                    ASYNC_GDB      = 1,
                    ASYNC_STDOUT   = 2,
                    ASYNC_STDERR   = 3,
            };
            
            #define MAX_ASYNC_CHANNELS 8
            
            enum HostFsCommands
            {
                    HOSTFS_CMD_HELLO   = 0x8FFC0000,
                    HOSTFS_CMD_BYE     = 0x8FFC0001,
                    HOSTFS_CMD_OPEN    = 0x8FFC0002,
                    HOSTFS_CMD_CLOSE   = 0x8FFC0003,
                    HOSTFS_CMD_READ    = 0x8FFC0004,
                    HOSTFS_CMD_WRITE   = 0x8FFC0005,
                    HOSTFS_CMD_LSEEK   = 0x8FFC0006,
                    HOSTFS_CMD_REMOVE  = 0x8FFC0007,
                    HOSTFS_CMD_MKDIR   = 0x8FFC0008,
                    HOSTFS_CMD_RMDIR   = 0x8FFC0009,
                    HOSTFS_CMD_DOPEN   = 0x8FFC000A,
                    HOSTFS_CMD_DREAD   = 0x8FFC000B,
                    HOSTFS_CMD_DCLOSE  = 0x8FFC000C,
                    HOSTFS_CMD_GETSTAT = 0x8FFC000D,
                    HOSTFS_CMD_CHSTAT  = 0x8FFC000E,
                    HOSTFS_CMD_RENAME  = 0x8FFC000F,
                    HOSTFS_CMD_CHDIR   = 0x8FFC0010,
                    HOSTFS_CMD_IOCTL   = 0x8FFC0011,
                    HOSTFS_CMD_DEVCTL  = 0x8FFC0012
            };
            
            struct HostFsTimeStamp
            {
                    uint16_t	year;
                    uint16_t 	month;
                    uint16_t 	day;
                    uint16_t	hour;
                    uint16_t	minute;
                    uint16_t	second;
            } __attribute__((packed));
            
            struct HostFsCmd
            {
                    uint32_t magic;
                    uint32_t command;
                    uint32_t extralen;
            } __attribute__((packed));
            
            struct HostFsHelloCmd
            {
                    struct HostFsCmd cmd;
            } __attribute__((packed));
            
            struct HostFsHelloResp
            {
                    struct HostFsCmd cmd;
            } __attribute__((packed));
            
            struct HostFsByeCmd
            {
                    struct HostFsCmd cmd;
            } __attribute__((packed));
            
            struct HostFsByeResp
            {
                    struct HostFsCmd cmd;
            } __attribute__((packed));
            
            struct HostFsOpenCmd
            {
                    struct HostFsCmd cmd;
                    uint32_t mode;
                    uint32_t mask;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsOpenResp
            {
                    struct HostFsCmd cmd;
                    int32_t    res;
            } __attribute__((packed));
            
            struct HostFsCloseCmd
            {
                    struct HostFsCmd cmd;
                    int32_t    fid;
            } __attribute__((packed));
            
            struct HostFsCloseResp
            {
                    struct HostFsCmd cmd;
                    int32_t    res;
            } __attribute__((packed));
            
            struct HostFsReadCmd
            {
                    struct HostFsCmd cmd;
                    int32_t    fid;
                    int32_t    len;
            } __attribute__((packed));
            
            struct HostFsReadResp
            {
                    struct HostFsCmd cmd;
                    int32_t    res;
            } __attribute__((packed));
            
            struct HostFsWriteCmd
            {
                    struct HostFsCmd cmd;
                    int32_t    fid;
            } __attribute__((packed));
            
            struct HostFsWriteResp
            {
                    struct HostFsCmd cmd;
                    int32_t    res;
            } __attribute__((packed));
            
            struct HostFsLseekCmd
            {
                    struct HostFsCmd cmd;
                    int32_t    fid;
                    int64_t ofs;
                    int32_t    whence;
            } __attribute__((packed));
            
            struct HostFsLseekResp
            {
                    struct HostFsCmd cmd;
                    int32_t    res;
                    int64_t ofs;
            } __attribute__((packed));
            
            struct HostFsIoctlCmd
            {
                    struct HostFsCmd cmd;
                    int32_t fid;
                    uint32_t cmdno;
                    int32_t outlen;
            } __attribute__((packed));
            
            struct HostFsIoctlResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsDevctlCmd
            {
                    struct HostFsCmd cmd;
                    uint32_t cmdno;
                    uint32_t fsnum;
                    int32_t outlen;
            } __attribute__((packed));
            
            struct HostFsDevctlResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsRemoveCmd
            {
                    struct HostFsCmd cmd;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsRemoveResp
            {
                    struct HostFsCmd cmd;
                    int32_t    res;
            } __attribute__((packed));
            
            struct HostFsDopenCmd
            {
                    struct HostFsCmd cmd;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsDopenResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsDreadCmd
            {
                    struct HostFsCmd cmd;
                    int32_t did;
            } __attribute__((packed));
            
            struct HostFsDreadResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsDcloseCmd
            {
                    struct HostFsCmd cmd;
                    int32_t  did;
            } __attribute__((packed));
            
            struct HostFsDcloseResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsMkdirCmd
            {
                    struct HostFsCmd cmd;
                    unsigned int mode;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsMkdirResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsRmdirCmd
            {
                    struct HostFsCmd cmd;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsRmdirResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsGetstatCmd
            {
                    struct HostFsCmd cmd;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsGetstatResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsChstatCmd
            {
                    struct HostFsCmd cmd;
                    int32_t bits;
                    uint32_t 	mode;
                    int64_t		size;
                    /** Access time. */
                    struct HostFsTimeStamp atime;
                    /** Modification time. */
                    struct HostFsTimeStamp	mtime;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsChstatResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsRenameCmd
            {
                    struct HostFsCmd cmd;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsRenameResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct HostFsChdirCmd
            {
                    struct HostFsCmd cmd;
                    uint32_t fsnum;
            } __attribute__((packed));
            
            struct HostFsChdirResp
            {
                    struct HostFsCmd cmd;
                    int32_t res;
            } __attribute__((packed));
            
            struct AsyncCommand
            {
                    uint32_t magic;
                    uint32_t channel;
            } __attribute__((packed));
            
            struct BulkCommand
            {
                    uint32_t magic;
                    uint32_t size;
            } __attribute__((packed));
            
            #ifndef PC_SIDE
            
                #define MODPRINTF(fmt, ...) Kprintf("%s: " fmt, ND000_MODULE_NAME, ## __VA_ARGS__)
                
                #ifdef DEBUG
                       #define DEBUG_PRINTF(fmt, ...) Kprintf("%s: " fmt, ND000_MODULE_NAME, ## __VA_ARGS__)
                #else
                       #define DEBUG_PRINTF(fmt, ...)
                #endif
                
            #endif
            
            
 
 
 
 
            
/*
        ------------------------------------------------------------------------
        STRUCTURES AND PROTOTYPES FOR ASYNC ACCESS 
        ------------------------------------------------------------------------
        These structures need to async access to PC. 
        
        This is a function provided by Tyranid's PSPLINK driver and that we 
        have mantained with some changes.
        ------------------------------------------------------------------------
*/
        
            #define MAX_ASYNC_BUFFER   4096
            
            #define ASYNC_USER 4
            
            struct AsyncEndpoint
            {
                    unsigned char buffer[MAX_ASYNC_BUFFER];
                    int read_pos;
                    int write_pos;
                    int size;
            };
            
            #define MAX_ASYNC_CHANNELS 8
            #define ASYNC_ALLOC_CHAN ((unsigned int) (-1))
            
            
            
            
            /**
              * Wait for the USB connection to be established
              *
              * @return 0 on connected, < 0 on error
              */
            int     USB_PRX_WaitForConnect (int TimeOut);
            
            
            
            
            /**
              * Register an asyncronous provider
              * @param chan - The channel number to register (0->3 are reserved for psplink use)
              * If you dont care about what channel number to provide then pass ASYNC_ALLOC_CHAN
              * @param endp - Pointer to an AsyncEndpoint structure
              * 
              * @return channel number on success, < 0 on error
              */
            int     USB_PRX_AsyncRegister(unsigned int chan, struct AsyncEndpoint *endp);
            
            
            
            
            
            /**
              * Unregister an asyncronous provider
              * 
              * @param chan - The channel to unregister
              *
              * @return 0 on success, < 0 on error
              */
            int     USB_PRX_AsyncUnregister(unsigned int chan);
            
            
            
            
            
            /**
              * Write data to the specified async channel
              * 
              * @param chan - The channel to write to
              * @param data - The data set to write
              * @param len - The length of the data
              * 
              * @return The number of bytes written, < 0 on error
              */
            int     USB_PRX_AsyncWrite(unsigned int chan, const void *data, int len);
            
            
            
            
            
            
            /**
              * Write data to the specified async channel
              * 
              * @param chan - The channel to read from
              * @param data - The data block to read into
              * @param len -  The length of the data to read
              * 
              * @return The number of bytes read, < 0 on error
              */
            int     USB_PRX_AsyncRead(unsigned int chan, unsigned char *data, int len);
            
            
            
            
            
            
            /**
              * Flush the read buffer of an async channel
              * 
              * @param chan - The channel to flush
              */
            void    USB_PRX_AsyncFlush(unsigned int chan);
            


/*
        ------------------------------------------------------------------------
        STRUCTURES AND PROTOTYPES FOR BULK ACCESS 
        ------------------------------------------------------------------------
        These structures need to bulk access to PC file system. 
        
        This is a function provided by Tyranid's PSPLINK driver. We have only
        copied here the prototypes, so Nanodesktop USB API can use it
        ------------------------------------------------------------------------
*/
 
            // PROTOTYPES
            
            // (INCOMPLETE)
            
            int usbWriteBulkData(const void *data, int len);
            
            int32_t        USB_PRX_IsUsbConnected(void);
            
            int            USB_PRX_ReadData(void *data, int size, int TimeOut);
            int            USB_PRX_WriteData(const void *data, int size, int TimeOut);
            
            int32_t        USB_PRX_CommXChg(void *outcmd, int32_t outcmdlen, void *incmd, int32_t incmdlen, const void *outdata, 
                                                 int32_t outlen, void *indata, int32_t inlen, int TimeOut);
            
            int32_t        USB_PRX_hfsInit(void);
            void           USB_PRX_hfsTerminate(void);
 
 
            
            
/*      ------------------------------------------------------------------------
        STRUCTURES AND PROTOTYPES FOR KERNEL MODE ACCESS 
        ------------------------------------------------------------------------
        This functions are ASM routines defined in kMode.S
        ------------------------------------------------------------------------
*/
        
        int psplinkSetK1(int k1);


/*
        ------------------------------------------------------------------------
        STRUCTURES AND PROTOTYPES FOR NANODESKTOP ACCESS 
        ------------------------------------------------------------------------
        Prototypes and structures for interaction with Nanodesktop API. 
        
        It contains also service functions as xmemcpy ...
        ------------------------------------------------------------------------
*/

        

        extern struct ndUsb_CommArea_Type *PntCommArea;
        extern int    CommAreaInit;

        // INTERNAL PROTOTYPES
        
        extern void *xmemset(void *b, int c, size_t len);
        extern void *xmemcpy(void *dst, const void *src, size_t len);
        
        extern void USB_PRX_INTERNAL_SendMessageToNd (int MessageCode, const char *format, ...);
        
        extern void USB_PRX_INTERNAL_SignalUsbAttach ();
        extern void USB_PRX_INTERNAL_SignalUsbDetach ();

        // EXPORTED PROTOTYPES
        
        extern void USB_PRX_PassCommAreaAdr (struct ndUsb_CommArea_Type *AdrArea);
        
        
        
        
        
        
#endif
