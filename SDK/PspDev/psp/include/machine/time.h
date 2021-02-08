#ifndef	_MACHTIME_H_
#define	_MACHTIME_H_

#if defined(__rtems__)
#define _CLOCKS_PER_SEC_  sysconf(_SC_CLK_TCK)
#else  /* !__rtems__ */
#if defined(__arm__) || defined(__thumb__)
#define _CLOCKS_PER_SEC_ 100
#endif
#endif /* !__rtems__ */

#if defined(__psp__)
    #ifdef PSPE_PLATFORM
        #define _CLOCKS_PER_SEC_  2000000
    #elif defined PSP_PLATFORM
        #define _CLOCKS_PER_SEC_  1000000
    #else
        #define _CLOCKS_PER_SEC_  1000000
    #endif
#endif

#endif	/* _MACHTIME_H_ */


