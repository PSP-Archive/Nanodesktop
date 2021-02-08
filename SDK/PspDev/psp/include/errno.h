/*
   --------------------------------------------------------------------------
   errno.h               SDK File Headers
   --------------------------------------------------------------------------
   This file is part of PSPSDK. If the user is compiling using Nanodesktop,
   this file will be replaced by compiler with nanoc_errno.h
   --------------------------------------------------------------------------
*/


#ifndef __ERRNO_H__
            #define __ERRNO_H__

            #if (defined PSP_PLATFORM || defined PSPE_PLATFORM)     
                    #include <nanoc_errno.h>
            #else 
 
                    #ifndef __error_t_defined
                    typedef int error_t;
                    #define __error_t_defined 1
                    #endif
            
                    #include <sys/errno.h>
                    
            #endif

#endif /* !__ERRNO_H__ */
