/*
   --------------------------------------------------------------------------
   $$_ded_time.h               NanoC File Headers (PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di base di gestione ora e data (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_TIME_H_
       #define _DED_TIME_H_
       
       /*
          ---------------------------------------------------------------------
          INCLUSIONE DI ALTRI HEADERS
          ---------------------------------------------------------------------
       */
       
            
            #include <stddef.h>
            
            #include <sys/time.h> 
                                   
            // Un file del SDK originario della PSP che fornisce le definizioni di clock_t, 
            // time_t, struct time_val, struct_time_zone,
            
            // L'header precedente però non fornisce la definizione della struct tm
            
            #include <machine/time.h>
            // Un file del SDK originario della PSP che fornisce la macro _CLOCKS_PER_SEC_
            
            
       /*
          ---------------------------------------------------------------------
          STRUTTURE PUBBLICHE DEFINITE DALLA LIBRERIA
          ---------------------------------------------------------------------
       */
          
            // Strutture non definite dagli include precedenti
            
            struct tm
            {
              int	tm_sec;
              int	tm_min;
              int	tm_hour;
              int	tm_mday;
              int	tm_mon;
              int	tm_year;
              int	tm_wday;
              int	tm_yday;
              int	tm_isdst;
            };
            
            // Variabili tz
            
            extern char *_tzname[2];
            extern int  _daylight;
            extern time_t _timezone;

            extern int __tzyear;
            extern int __tznorth;

       /*
          ---------------------------------------------------------------------
          PROTOTIPI
          ---------------------------------------------------------------------
       */
        
            
        #ifdef __cplusplus
        extern "C" {
        #endif
            
            
             extern void __tz_lock (void); 
             extern void __tz_unlock (void); 
             extern int  __tzcalc_limits (int year); 
            
             extern int gettimeofday (struct timeval *tp, struct timezone *tzp); 
             
             extern clock_t clock (void); 
             extern time_t time (time_t *t); 
             
             extern double difftime (time_t tim1, time_t tim2); 
             extern time_t mktime (struct tm *tim_p); 
             extern char *asctime_r (struct tm *tim_p, char *result); 
             extern char *asctime (struct tm *tim_p); 
             
             extern struct tm *_mktm_r (const time_t *tim_p, struct tm *res, int is_gmtime); 
             extern struct tm *localtime_r (const time_t *tim_p, struct tm *res); 
             extern struct tm *localtime (const time_t *tim_p); 
             
             extern char *ctime (time_t *tim_p); 
             extern char *ctime_r (time_t *tim_p, char *result); 
             
             extern struct tm *gmtime_r (const time_t *tim_p, struct tm *res); 
             extern struct tm *gmtime (const time_t *tim_p); 
             
             extern size_t strftime (char *s, size_t maxsize, char *format, struct tm *tim_p); 
             extern char *strptime (const char *buf, const char *format, struct tm *timeptr); 
             
             extern int DetDayOfWeek (int y, int m, int d); 
             extern int DetDayOfYear (int Year, int Month, int Day); 
             
             extern void tzset(void);
             
                      
       #ifdef __cplusplus
       }
       #endif
                
        
#endif             // ifdef _DED_TIME_H_






















       
