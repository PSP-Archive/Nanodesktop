/*
   --------------------------------------------------------------------------
   unistd.h               SDK File Headers
   --------------------------------------------------------------------------
   This file is part of PSPSDK. If the user is compiling using Nanodesktop,
   this file will be replaced by compiler with nanoc_unistd.h
   --------------------------------------------------------------------------
*/


#ifndef _UNISTD_H_
        
        #define _UNISTD_H_
        
        #if (defined PSP_PLATFORM || defined PSPE_PLATFORM)     
              #include <nanoc_unistd.h>
        #else 

              // Original code of unistd.h

              # include <sys/unistd.h>

        #endif   //#if (defined PSP_PLATFORM || defined PSPE_PLATFORM)

#endif /* _UNISTD_H_ */
