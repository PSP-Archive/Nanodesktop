/*
   --------------------------------------------------------------------------
   locale.h               SDK File Headers
   --------------------------------------------------------------------------
   This file is part of PSPSDK. If the user is compiling using Nanodesktop,
   this file will be replaced by compiler with nanoc_locale.h
   --------------------------------------------------------------------------
	locale.h
	Values appropriate for the formatting of monetary and other
	numberic quantities.
   --------------------------------------------------------------------------
*/


#ifndef _LOCALE_H_
#define _LOCALE_H_

        #if (defined PSP_PLATFORM || defined PSPE_PLATFORM)     
             #include <nanoc_locale.h>
        #else  
                    
                #include "_ansi.h"
                
                #ifndef NULL
                #define NULL    0
                #endif
                
                #define LC_ALL	    0
                #define LC_COLLATE  1
                #define LC_CTYPE    2
                #define LC_MONETARY 3
                #define LC_NUMERIC  4
                #define LC_TIME     5
                #define LC_MESSAGES 6
                
                _BEGIN_STD_C
                
                struct lconv
                {
                  char *decimal_point;
                  char *thousands_sep;
                  char *grouping;
                  char *int_curr_symbol;
                  char *currency_symbol;
                  char *mon_decimal_point;
                  char *mon_thousands_sep;
                  char *mon_grouping;
                  char *positive_sign;
                  char *negative_sign;
                  char int_frac_digits;
                  char frac_digits;
                  char p_cs_precedes;
                  char p_sep_by_space;
                  char n_cs_precedes;
                  char n_sep_by_space;
                  char p_sign_posn;
                  char n_sign_posn;
                };
                
                #ifndef _REENT_ONLY
                char *_EXFUN(setlocale,(int category, const char *locale));
                struct lconv *_EXFUN(localeconv,(void));
                #endif
                
                struct _reent;
                char *_EXFUN(_setlocale_r,(struct _reent *, int category, const char *locale));
                struct lconv *_EXFUN(_localeconv_r,(struct _reent *));
                
                _END_STD_C
                
        #endif

#endif /* _LOCALE_H_ */
