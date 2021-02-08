/*
    semaphore.h
    
    Questo file non era incluso nella dotazione standard degli headers
    del PSPSDK. Viene aggiunto per consentire la coerenza del meccanismo
    di code deviation
*/

#ifndef __PSEMAPHORE_H
#define __PSEMAPHORE_H

    #if (defined PSPE_PLATFORM || defined PSP_PLATFORM)  
      
                        #include <nanoc_semaphore.h>
                        // Code deviation
                        
    #endif
    
#endif
