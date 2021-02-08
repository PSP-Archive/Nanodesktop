/*
   --------------------------------------------------------------------------
   time.c                                      NanoC Files (PSPE)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di base per gestione ora e data
   ----------------------------------------------------------------------------
*/   
 
/*
   ----------------------------------------------------------------------------
   Inclusione di altri headers
   ----------------------------------------------------------------------------
*/

        #include <malloc.h>
        #include <stdarg.h>
        #include <string.h>
        #include <stdlib.h>
        #include <time.h>
        #include <sys/stat.h>
        #include <sys/time.h>
        #include <sys/types.h>
        #include <sys/unistd.h>
        #include <sys/lock.h>
        
        #include <local.h>
        
        
/*
   ----------------------------------------------------------------------------
   Variabili tz
   ----------------------------------------------------------------------------
*/

        /* default to GMT */
        char *_tzname[2] = {"GMT" "GMT"};
        int _daylight = 0;
        
        time_t _timezone = (time_t)0;
        int __tzyear = 0;
        int __tznorth = 1;
        
        __tzrule_type __tzrule[2] = { {'J', 0, 0, 0, 0, (time_t)0, 0 }, {'J', 0, 0, 0, 0, (time_t)0, 0 } };



/*
 * -------------------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org        TIME CORE
 * -------------------------------------------------------------------------------
 * time.c
 * Original Author:	
 * -------------------------------------------------------------------------------
        FUNCTION
        <<time>>---get current calendar time (as single number)
        
        INDEX
        	time
        
        ANSI_SYNOPSIS
        	#include <time.h>
        	time_t time(time_t *<[t]>);
        
        TRAD_SYNOPSIS
        	#include <time.h>
        	time_t time(<[t]>)
        	time_t *<[t]>;
        
        DESCRIPTION
        <<time>> looks up the best available representation of the current
        time and returns it, encoded as a <<time_t>>.  It stores the same
        value at <[t]> unless the argument is <<NULL>>.
        
        RETURNS
        A <<-1>> result means the current time is not available; otherwise the
        result represents the current time.
        
        PORTABILITY
        ANSI C requires <<time>>.
        
        Supporting OS subroutine required: Some implementations require
        <<gettimeofday>>.
   -------------------------------------------------------------------------------
*/


int gettimeofday (struct timeval *tp, struct timezone *tzp)
{
	   return sceKernelLibcGettimeofday(tp, tzp);
}

clock_t clock(void)
{
       return sceKernelLibcClock();
}


time_t time (time_t * t)
{
      struct timeval  now;
      struct timezone tzp;
    
      if (gettimeofday (&now, &tzp) >= 0)
      {
          if (t) *t = now.tv_sec;
          return now.tv_sec;
      }
     
      return -1;
}


/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * difftime.c
 * Original Author:	G. Haley
 * -----------------------------------------------------------------------

        FUNCTION
        <<difftime>>---subtract two times
        
        INDEX
        	difftime
        
        ANSI_SYNOPSIS
        	#include <time.h>
        	double difftime(time_t <[tim1]>, time_t <[tim2]>);
        
        TRAD_SYNOPSIS
        	#include <time.h>
        	double difftime(<[tim1]>, <[tim2]>)
        	time_t <[tim1]>;
        	time_t <[tim2]>;
        
        DESCRIPTION
        Subtracts the two times in the arguments: `<<<[tim1]> - <[tim2]>>>'.
        
        RETURNS
        The difference (in seconds) between <[tim2]> and <[tim1]>, as a <<double>>.
        
        PORTABILITY
        ANSI C requires <<difftime>>, and defines its result to be in seconds
        in all implementations.
        
        <<difftime>> requires no supporting OS subroutines.
* -----------------------------------------------------------------------
*/


double difftime (time_t tim1, time_t tim2)
{
   return (double)(tim1 - tim2);     
}


/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * mktime
 * Original Author:	G. Haley
 * -----------------------------------------------------------------------
 * Converts the broken-down time, expressed as local time, in the structure
 * pointed to by tim_p into a calendar time value. The original values of the
 * tm_wday and tm_yday fields of the structure are ignored, and the original
 * values of the other fields have no restrictions. On successful completion
 * the fields of the structure are set to represent the specified calendar
 * time. Returns the specified calendar time. If the calendar time can not be
 * represented, returns the value (time_t) -1.
 */

        /*
        FUNCTION
        <<mktime>>---convert time to arithmetic representation
        
        INDEX
        	mktime
        
        ANSI_SYNOPSIS
        	#include <time.h>
        	time_t mktime(struct tm *<[timp]>);
        
        TRAD_SYNOPSIS
        	#include <time.h>
        	time_t mktime(<[timp]>)
        	struct tm *<[timp]>;
        
        DESCRIPTION
        <<mktime>> assumes the time at <[timp]> is a local time, and converts
        its representation from the traditional representation defined by
        <<struct tm>> into a representation suitable for arithmetic.
        
        <<localtime>> is the inverse of <<mktime>>.
        
        RETURNS
        If the contents of the structure at <[timp]> do not form a valid
        calendar time representation, the result is <<-1>>.  Otherwise, the
        result is the time, converted to a <<time_t>> value.
        
        PORTABILITY
        ANSI C requires <<mktime>>.
        
        <<mktime>> requires no supporting OS subroutines.
* -----------------------------------------------------------------------
*/

#define _SEC_IN_MINUTE 60L
#define _SEC_IN_HOUR 3600L
#define _SEC_IN_DAY 86400L

static _CONST int DAYS_IN_MONTH[12] =
{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define _DAYS_IN_MONTH(x) ((x == 1) ? days_in_feb : DAYS_IN_MONTH[x])

static _CONST int _DAYS_BEFORE_MONTH[12] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

#define _ISLEAP(y) (((y) % 4) == 0 && (((y) % 100) != 0 || (((y)+1900) % 400) == 0))
#define _DAYS_IN_YEAR(year) (_ISLEAP(year) ? 366 : 365)

static void 
validate_structure (tim_p)
     struct tm *tim_p;
{
  div_t res;
  int days_in_feb = 28;

  /* calculate time & date to account for out of range values */
  if (tim_p->tm_sec < 0 || tim_p->tm_sec > 59)
    {
      res = div (tim_p->tm_sec, 60);
      tim_p->tm_min += res.quot;
      if ((tim_p->tm_sec = res.rem) < 0)
	{
	  tim_p->tm_sec += 60;
	  --tim_p->tm_min;
	}
    }

  if (tim_p->tm_min < 0 || tim_p->tm_min > 59)
    {
      res = div (tim_p->tm_min, 60);
      tim_p->tm_hour += res.quot;
      if ((tim_p->tm_min = res.rem) < 0)
	{
	  tim_p->tm_min += 60;
	  --tim_p->tm_hour;
        }
    }

  if (tim_p->tm_hour < 0 || tim_p->tm_hour > 23)
    {
      res = div (tim_p->tm_hour, 24);
      tim_p->tm_mday += res.quot;
      if ((tim_p->tm_hour = res.rem) < 0)
	{
	  tim_p->tm_hour += 24;
	  --tim_p->tm_mday;
        }
    }

  if (tim_p->tm_mon > 11)
    {
      res = div (tim_p->tm_mon, 12);
      tim_p->tm_year += res.quot;
      if ((tim_p->tm_mon = res.rem) < 0)
        {
	  tim_p->tm_mon += 12;
	  --tim_p->tm_year;
        }
    }

  if (_DAYS_IN_YEAR (tim_p->tm_year) == 366)
    days_in_feb = 29;

  if (tim_p->tm_mday <= 0)
    {
      while (tim_p->tm_mday <= 0)
	{
	  if (--tim_p->tm_mon == -1)
	    {
	      tim_p->tm_year--;
	      tim_p->tm_mon = 11;
	      days_in_feb =
		((_DAYS_IN_YEAR (tim_p->tm_year) == 366) ?
		 29 : 28);
	    }
	  tim_p->tm_mday += _DAYS_IN_MONTH (tim_p->tm_mon);
	}
    }
  else
    {
      while (tim_p->tm_mday > _DAYS_IN_MONTH (tim_p->tm_mon))
	{
	  tim_p->tm_mday -= _DAYS_IN_MONTH (tim_p->tm_mon);
	  if (++tim_p->tm_mon == 12)
	    {
	      tim_p->tm_year++;
	      tim_p->tm_mon = 0;
	      days_in_feb =
		((_DAYS_IN_YEAR (tim_p->tm_year) == 366) ?
		 29 : 28);
	    }
	}
    }
}

time_t mktime (struct tm *tim_p)
{
  time_t tim = 0;
  long days = 0;
  int year, isdst;

  /* validate structure */
  validate_structure (tim_p);

  /* compute hours, minutes, seconds */
  tim += tim_p->tm_sec + (tim_p->tm_min * _SEC_IN_MINUTE) +
    (tim_p->tm_hour * _SEC_IN_HOUR);

  /* compute days in year */
  days += tim_p->tm_mday - 1;
  days += _DAYS_BEFORE_MONTH[tim_p->tm_mon];
  if (tim_p->tm_mon > 1 && _DAYS_IN_YEAR (tim_p->tm_year) == 366)
    days++;

  /* compute day of the year */
  tim_p->tm_yday = days;

  if (tim_p->tm_year > 10000
      || tim_p->tm_year < -10000)
    {
      return (time_t) -1;
    }

  /* compute days in other years */
  if (tim_p->tm_year > 70)
    {
      for (year = 70; year < tim_p->tm_year; year++)
	days += _DAYS_IN_YEAR (year);
    }
  else if (tim_p->tm_year < 70)
    {
      for (year = 69; year > tim_p->tm_year; year--)
	days -= _DAYS_IN_YEAR (year);
      days -= _DAYS_IN_YEAR (year);
    }

  /* compute day of the week */
  if ((tim_p->tm_wday = (days + 4) % 7) < 0)
    tim_p->tm_wday += 7;

  /* compute total seconds */
  tim += (days * _SEC_IN_DAY);

  isdst = tim_p->tm_isdst;

  if (_daylight)
    {
      int y = tim_p->tm_year + YEAR_BASE;
      if (y == __tzyear || __tzcalc_limits (y))
	{
	  /* calculate start of dst in dst local time and 
	     start of std in both std local time and dst local time */
	     
      time_t startdst_dst = __tzrule[0].change - __tzrule[1].offset;
	  time_t startstd_dst = __tzrule[1].change - __tzrule[1].offset;
	  time_t startstd_std = __tzrule[1].change - __tzrule[0].offset;
	  
      /* if the time is in the overlap between dst and std local times */
	  if (tim >= startstd_std && tim < startstd_dst)
	    ; /* we let user decide or leave as -1 */
          else
	    {
	      isdst = (__tznorth
		       ? (tim >= startdst_dst && tim < startstd_std)
		       : (tim >= startdst_dst || tim < startstd_std));
	      /* if user committed and was wrong, perform correction */
	      if ((isdst ^ tim_p->tm_isdst) == 1)
		{
		  /* we either subtract or add the difference between
		     time zone offsets, depending on which way the user got it wrong */
		  int diff = __tzrule[0].offset - __tzrule[1].offset;
		  if (!isdst)
		    diff = -diff;
		  tim_p->tm_sec += diff;
		  validate_structure (tim_p);
		  tim += diff;  /* we also need to correct our current time calculation */
		}
	    }
	}
    }

  /* add appropriate offset to put time in gmt format */
  if (isdst == 1)
    tim += __tzrule[1].offset;
  else /* otherwise assume std time */
    tim += __tzrule[0].offset;

  /* reset isdst flag to what we have calculated */
  tim_p->tm_isdst = isdst;

  return tim;
}


/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * asctime/asctime_r
 * Original Author:	G. Haley
 * -----------------------------------------------------------------------

        FUNCTION
        <<asctime>>---format time as string
        
        INDEX
        	asctime
        INDEX
        	_asctime_r
        
        ANSI_SYNOPSIS
        	#include <time.h>
        	char *asctime(const struct tm *<[clock]>);
        	char *asctime_r(const struct tm *<[clock]>, char *<[buf]>);
        
        TRAD_SYNOPSIS
        	#include <time.h>
        	char *asctime(<[clock]>)
        	struct tm *<[clock]>;
        	char *asctime_r(<[clock]>)
        	struct tm *<[clock]>;
        	char *<[buf]>;
        
        DESCRIPTION
        Format the time value at <[clock]> into a string of the form
        . Wed Jun 15 11:38:07 1988\n\0
        The string is generated in a static buffer; each call to <<asctime>>
        overwrites the string generated by previous calls.
        
        RETURNS
        A pointer to the string containing a formatted timestamp.
        
        PORTABILITY
        ANSI C requires <<asctime>>.
        
        <<asctime>> requires no supporting OS subroutines.
    ---------------------------------------------------------------------------
*/

static char asctime_buffer [128];

char *asctime_r (struct tm *tim_p, char *result)	
{
  static _CONST char day_name[7][3] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  static _CONST char mon_name[12][3] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  sprintf (result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
	   day_name[tim_p->tm_wday], 
	   mon_name[tim_p->tm_mon],
	   tim_p->tm_mday, tim_p->tm_hour, tim_p->tm_min,
	   tim_p->tm_sec, 1900 + tim_p->tm_year);
  return result;
}

char *asctime (struct tm *tim_p)
{
   return asctime_r (tim_p, (char *)(&asctime_buffer) );  
}


/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * mktm_r.c
 * Original Author:	Adapted from tzcode maintained by Arthur David Olson.
 * Modifications:       Changed to mktm_r and added __tzcalc_limits - 04/10/02, Jeff Johnston
 *                      Fixed bug in mday computations - 08/12/04, Alex Mogilnikov <alx@intellectronika.ru>
 *                      Fixed bug in __tzcalc_limits - 08/12/04, Alex Mogilnikov <alx@intellectronika.ru>
 *
 * Converts the calendar time pointed to by tim_p into a broken-down time
 * expressed as local time. Returns a pointer to a structure containing the
 * broken-down time.
 * -----------------------------------------------------------------------
*/

static const int mon_lengths[2][MONSPERYEAR] = {
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
} ;

static const int year_lengths[2] = {
  365,
  366
} ;

struct tm *_mktm_r (const time_t *tim_p, struct tm *res, int is_gmtime)
{
  long days, rem;
  time_t lcltime;
  int y;
  int yleap;
  _CONST int *ip;

  /* base decision about std/dst time on current time */
  lcltime = *tim_p;
   
  days = ((long)lcltime) / SECSPERDAY;
  rem = ((long)lcltime) % SECSPERDAY;
  while (rem < 0) 
    {
      rem += SECSPERDAY;
      --days;
    }
  while (rem >= SECSPERDAY)
    {
      rem -= SECSPERDAY;
      ++days;
    }
 
  /* compute hour, min, and sec */  
  res->tm_hour = (int) (rem / SECSPERHOUR);
  rem %= SECSPERHOUR;
  res->tm_min = (int) (rem / SECSPERMIN);
  res->tm_sec = (int) (rem % SECSPERMIN);

  /* compute day of week */
  if ((res->tm_wday = ((EPOCH_WDAY + days) % DAYSPERWEEK)) < 0)
    res->tm_wday += DAYSPERWEEK;

  /* compute year & day of year */
  y = EPOCH_YEAR;
  if (days >= 0)
    {
      for (;;)
	{
	  yleap = isleap(y);
	  if (days < year_lengths[yleap])
	    break;
	  y++;
	  days -= year_lengths[yleap];
	}
    }
  else
    {
      do
	{
	  --y;
	  yleap = isleap(y);
	  days += year_lengths[yleap];
	} while (days < 0);
    }

  res->tm_year = y - YEAR_BASE;
  res->tm_yday = days;
  ip = mon_lengths[yleap];
  for (res->tm_mon = 0; days >= ip[res->tm_mon]; ++res->tm_mon)
    days -= ip[res->tm_mon];
  res->tm_mday = days + 1;

  if (!is_gmtime)
    {
      int offset;
      int hours, mins, secs;

      TZ_LOCK;
      if (_daylight)
	{
	  if (y == __tzyear || __tzcalc_limits (y))
	    res->tm_isdst = (__tznorth 
			     ? (*tim_p >= __tzrule[0].change && *tim_p < __tzrule[1].change)
			     : (*tim_p >= __tzrule[0].change || *tim_p < __tzrule[1].change));
	  else
	    res->tm_isdst = -1;
	}
      else
	res->tm_isdst = 0;

      offset = (res->tm_isdst == 1 ? __tzrule[1].offset : __tzrule[0].offset);

      hours = offset / SECSPERHOUR;
      offset = offset % SECSPERHOUR;
      
      mins = offset / SECSPERMIN;
      secs = offset % SECSPERMIN;

      res->tm_sec -= secs;
      res->tm_min -= mins;
      res->tm_hour -= hours;

      if (res->tm_sec >= SECSPERMIN)
	{
	  res->tm_min += 1;
	  res->tm_sec -= SECSPERMIN;
	}
      else if (res->tm_sec < 0)
	{
	  res->tm_min -= 1;
	  res->tm_sec += SECSPERMIN;
	}
      if (res->tm_min >= MINSPERHOUR)
	{
	  res->tm_hour += 1;
	  res->tm_min -= MINSPERHOUR;
	}
      else if (res->tm_min < 0)
	{
	  res->tm_hour -= 1;
	  res->tm_min += MINSPERHOUR;
	}
      if (res->tm_hour >= HOURSPERDAY)
	{
	  ++res->tm_yday;
	  ++res->tm_wday;
	  if (res->tm_wday > 6)
	    res->tm_wday = 0;
	  ++res->tm_mday;
	  res->tm_hour -= HOURSPERDAY;
	  if (res->tm_mday > ip[res->tm_mon])
	    {
	      res->tm_mday -= ip[res->tm_mon];
	      res->tm_mon += 1;
	      if (res->tm_mon == 12)
		{
		  res->tm_mon = 0;
		  res->tm_year += 1;
		  res->tm_yday = 0;
		}
	    }
	}
       else if (res->tm_hour < 0)
	{
	  res->tm_yday -= 1;
	  res->tm_wday -= 1;
	  if (res->tm_wday < 0)
	    res->tm_wday = 6;
	  res->tm_mday -= 1;
	  res->tm_hour += 24;
	  if (res->tm_mday == 0)
	    {
	      res->tm_mon -= 1;
	      if (res->tm_mon < 0)
		{
		  res->tm_mon = 11;
		  res->tm_year -= 1;
		  res->tm_yday = 365 + isleap(res->tm_year);
		}
	      res->tm_mday = ip[res->tm_mon];
	    }
	}
      TZ_UNLOCK;
    }
  else
    res->tm_isdst = 0;

  return (res);
}

int __tzcalc_limits (int year)
{
  int days, year_days, years;
  int i, j;

  if (year < EPOCH_YEAR)
    return 0;

  __tzyear = year;

  years = (year - EPOCH_YEAR);

  year_days = years * 365 +
    (years - 1 + EPOCH_YEARS_SINCE_LEAP) / 4 - (years - 1 + EPOCH_YEARS_SINCE_CENTURY) / 100 + 
    (years - 1 + EPOCH_YEARS_SINCE_LEAP_CENTURY) / 400;
  
  for (i = 0; i < 2; ++i)
    {
      if (__tzrule[i].ch == 'J')
	days = year_days + __tzrule[i].d + (isleap(year) && __tzrule[i].d >= 60);
      else if (__tzrule[i].ch == 'D')
	days = year_days + __tzrule[i].d;
      else
	{
	  int yleap = isleap(year);
	  int m_day, m_wday, wday_diff;
	  _CONST int *ip = mon_lengths[yleap];

	  days = year_days;

	  for (j = 1; j < __tzrule[i].m; ++j)
	    days += ip[j-1];

	  m_wday = (EPOCH_WDAY + days) % DAYSPERWEEK;
	  
	  wday_diff = __tzrule[i].d - m_wday;
	  if (wday_diff < 0)
	    wday_diff += DAYSPERWEEK;
	  m_day = (__tzrule[i].n - 1) * DAYSPERWEEK + wday_diff;

	  while (m_day >= ip[j-1])
	    m_day -= DAYSPERWEEK;

	  days += m_day;
	}

      /* store the change-over time in GMT form by adding offset */
      __tzrule[i].change = days * SECSPERDAY + __tzrule[i].s + __tzrule[i].offset;
    }

  __tznorth = (__tzrule[0].change < __tzrule[1].change);

  return 1;
}


/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
     FUNCTION
    <<localtime>>---convert time to local representation
    
    INDEX
    	localtime
    
    ANSI_SYNOPSIS
    	#include <time.h>
    	struct tm *localtime(time_t *<[clock]>);
    	struct tm *localtime_r(time_t *<[clock]>, struct tm *<[res]>);
    
    TRAD_SYNOPSIS
    	#include <time.h>
    	struct tm *localtime(<[clock]>)
    	time_t *<[clock]>;
    	struct tm *localtime(<[clock]>, <[res]>)
    	time_t *<[clock]>;
    	struct tm *<[res]>;
    
    DESCRIPTION
    <<localtime>> converts the time at <[clock]> into local time, then
    converts its representation from the arithmetic representation to the
    traditional representation defined by <<struct tm>>.
    
    <<localtime>> constructs the traditional time representation in static
    storage; each call to <<gmtime>> or <<localtime>> will overwrite the
    information generated by previous calls to either function.
    
    <<mktime>> is the inverse of <<localtime>>.
    
    RETURNS
    A pointer to the traditional time representation (<<struct tm>>).
    
    PORTABILITY
    ANSI C requires <<localtime>>.
    
    <<localtime>> requires no supporting OS subroutines.        
----------------------------------------------------------------------- 
*/

static struct tm localtime_buffer;

struct tm *localtime_r (const time_t *tim_p, struct tm *res)
{
  return _mktm_r (tim_p, res, 0);
}

struct tm *localtime (const time_t *tim_p)
{
   return localtime_r (tim_p, &(localtime_buffer) );
}
 

/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * ctime.c
 * Original Author:	G. Haley
 * -----------------------------------------------------------------------
     FUNCTION
    <<ctime>>---convert time to local and format as string
    
    INDEX
    	ctime
    
    ANSI_SYNOPSIS
    	#include <time.h>
    	char *ctime(const time_t *<[clock]>);
    	char *ctime_r(const time_t *<[clock]>, char *<[buf]>);
    
    TRAD_SYNOPSIS
    	#include <time.h>
    	char *ctime(<[clock]>)
    	time_t *<[clock]>;
    
    	char *ctime_r(<[clock]>, <[buf]>)
    	time_t *<[clock]>;
    	char *<[buf]>;
    
    DESCRIPTION
    Convert the time value at <[clock]> to local time (like <<localtime>>)
    and format it into a string of the form
    . Wed Jun 15 11:38:07 1988\n\0
    (like <<asctime>>).
    
    RETURNS
    A pointer to the string containing a formatted timestamp.
    
    PORTABILITY
    ANSI C requires <<ctime>>.
    
    <<ctime>> requires no supporting OS subroutines.
  * -----------------------------------------------------------------------
*/

char *ctime (time_t * tim_p)
{
  return asctime (localtime (tim_p));
}
  
char *ctime_r (time_t *tim_p, char *result)
{
  struct tm tm;
  return asctime_r (localtime_r (tim_p, &tm), result);
}

  

/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * gmtime/gmtime_r
 * Original Author:	G. Haley
 * -----------------------------------------------------------------------
    FUNCTION
    <<gmtime>>---convert time to UTC traditional form
    
    INDEX
    	gmtime
    
    ANSI_SYNOPSIS
    	#include <time.h>
    	struct tm *gmtime(const time_t *<[clock]>);
    	struct tm *gmtime_r(const time_t *<[clock]>, struct tm *<[res]>);
    
    TRAD_SYNOPSIS
    	#include <time.h>
    	struct tm *gmtime(<[clock]>)
    	const time_t *<[clock]>;
    	struct tm *gmtime_r(<[clock]>, <[res]>)
    	const time_t *<[clock]>;
    	struct tm *<[res]>;
    
    DESCRIPTION
    <<gmtime>> assumes the time at <[clock]> represents a local time.
    <<gmtime>> converts it to UTC (Universal Coordinated Time, also known in some
    countries as GMT, Greenwich Mean time), then converts the
    representation from the arithmetic representation to
    the traditional representation defined by <<struct tm>>.
    
    <<gmtime>> constructs the traditional time representation in static
    storage; each call to <<gmtime>> or <<localtime>> will overwrite the
    information generated by previous calls to either function.
    
    RETURNS
    A pointer to the traditional time representation (<<struct tm>>).
    
    PORTABILITY
    ANSI C requires <<gmtime>>.
    
    <<gmtime>> requires no supporting OS subroutines.
    ---------------------------------------------------------------------------
*/

static struct tm gmtime_buffer;

struct tm *gmtime_r (const time_t *tim_p, struct tm *res)
{
   return (_mktm_r (tim_p, res, 1));
}

struct tm *gmtime (const time_t *tim_p)
{
   return gmtime_r (tim_p, &(gmtime_buffer));    
}


/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * strftime
 * Original Author:	G. Haley
 * -----------------------------------------------------------------------
     FUNCTION
    <<strftime>>---flexible calendar time formatter
    
    INDEX
    	strftime
    
    ANSI_SYNOPSIS
    	#include <time.h>
    	size_t strftime(char *<[s]>, size_t <[maxsize]>,
    			const char *<[format]>, const struct tm *<[timp]>);
    
    TRAD_SYNOPSIS
    	#include <time.h>
    	size_t strftime(<[s]>, <[maxsize]>, <[format]>, <[timp]>)
    	char *<[s]>;
    	size_t <[maxsize]>;
    	char *<[format]>;
    	struct tm *<[timp]>;
    
    DESCRIPTION
    <<strftime>> converts a <<struct tm>> representation of the time (at
    <[timp]>) into a string, starting at <[s]> and occupying no more than
    <[maxsize]> characters.
    
    You control the format of the output using the string at <[format]>.
    <<*<[format]>>> can contain two kinds of specifications: text to be
    copied literally into the formatted string, and time conversion
    specifications.  Time conversion specifications are two-character
    sequences beginning with `<<%>>' (use `<<%%>>' to include a percent
    sign in the output).  Each defined conversion specification selects a
    field of calendar time data from <<*<[timp]>>>, and converts it to a
    string in one of the following ways:
    
    o+
    o %a
    An abbreviation for the day of the week.
    
    o %A
    The full name for the day of the week.
    
    o %b
    An abbreviation for the month name.
    
    o %B
    The full name of the month.
    
    o %c
    A string representing the complete date and time, in the form
    . Mon Apr 01 13:13:13 1992
    
    o %d
    The day of the month, formatted with two digits.
    
    o %e
    The day of the month, formatted with leading space if single digit.
    
    o %H
    The hour (on a 24-hour clock), formatted with two digits.
    
    o %I
    The hour (on a 12-hour clock), formatted with two digits.
    
    o %j
    The count of days in the year, formatted with three digits
    (from `<<001>>' to `<<366>>').
    
    o %m
    The month number, formatted with two digits.
    
    o %M
    The minute, formatted with two digits.
    
    o %p
    Either `<<AM>>' or `<<PM>>' as appropriate.
    
    o %S
    The second, formatted with two digits.
    
    o %U
    The week number, formatted with two digits (from `<<00>>' to `<<53>>';
    week number 1 is taken as beginning with the first Sunday in a year).
    See also <<%W>>.
    
    o %w
    A single digit representing the day of the week: Sunday is day <<0>>.
    
    o %W
    Another version of the week number: like `<<%U>>', but counting week 1
    as beginning with the first Monday in a year.
    
    o
    o %x
    A string representing the complete date, in a format like
    . Mon Apr 01 1992
    
    o %X
    A string representing the full time of day (hours, minutes, and
    seconds), in a format like
    . 13:13:13
    
    o %y
    The last two digits of the year.
    
    o %Y
    The full year, formatted with four digits to include the century.
    
    o %Z
    The time zone name.  If tm_isdst is -1, no output is generated.
    Otherwise, the time zone name based on the TZ environment variable
    is used.
    
    o %%
    A single character, `<<%>>'.
    o-
    
    RETURNS
    When the formatted time takes up no more than <[maxsize]> characters,
    the result is the length of the formatted string.  Otherwise, if the
    formatting operation was abandoned due to lack of room, the result is
    <<0>>, and the string starting at <[s]> corresponds to just those
    parts of <<*<[format]>>> that could be completely filled in within the
    <[maxsize]> limit.
    
    PORTABILITY
    ANSI C requires <<strftime>>, but does not specify the contents of
    <<*<[s]>>> when the formatted string would require more than
    <[maxsize]> characters.
    
    <<strftime>> requires no supporting OS subroutines.
  ----------------------------------------------------------------------------
*/

static const int dname_len[7] =
{6, 6, 7, 9, 8, 6, 8};

static const char *dname[7] =
{"Sunday", "Monday", "Tuesday", "Wednesday",
 "Thursday", "Friday", "Saturday"};

static const int mname_len[12] =
{7, 8, 5, 5, 3, 4, 4, 6, 9, 7, 8, 8};

static const char *mname[12] =
{"January", "February", "March", "April",
 "May", "June", "July", "August", "September", "October", "November",
 "December"};

size_t strftime (char *s, size_t maxsize, char *format, struct tm *tim_p)
{
  size_t count = 0;
  int i;

  for (;;)
    {
      while (*format && *format != '%')
	{
	  if (count < maxsize - 1)
	    s[count++] = *format++;
	  else
	    return 0;
	}

      if (*format == '\0')
	break;

      format++;
      switch (*format)
	{
	case 'a':
	  for (i = 0; i < 3; i++)
	    {
	      if (count < maxsize - 1)
		s[count++] =
		  dname[tim_p->tm_wday][i];
	      else
		return 0;
	    }
	  break;
	case 'A':
	  for (i = 0; i < dname_len[tim_p->tm_wday]; i++)
	    {
	      if (count < maxsize - 1)
		s[count++] =
		  dname[tim_p->tm_wday][i];
	      else
		return 0;
	    }
	  break;
	case 'b':
	case 'h':
	  for (i = 0; i < 3; i++)
	    {
	      if (count < maxsize - 1)
		s[count++] =
		  mname[tim_p->tm_mon][i];
	      else
		return 0;
	    }
	  break;
	case 'B':
	  for (i = 0; i < mname_len[tim_p->tm_mon]; i++)
	    {
	      if (count < maxsize - 1)
		s[count++] =
		  mname[tim_p->tm_mon][i];
	      else
		return 0;
	    }
	  break;
	case 'c':
	  if (count < maxsize - 24)
	    {
	      for (i = 0; i < 3; i++)
		s[count++] =
		  dname[tim_p->tm_wday][i];
	      s[count++] = ' ';
	      for (i = 0; i < 3; i++)
		s[count++] =
		  mname[tim_p->tm_mon][i];

	      sprintf (&s[count],
		       " %.2d %2.2d:%2.2d:%2.2d %.4d",
		       tim_p->tm_mday, tim_p->tm_hour,
		       tim_p->tm_min,
		       tim_p->tm_sec, 1900 +
		       tim_p->tm_year);
	      count += 17;
	    }
	  else
	    return 0;
	  break;
	case 'd':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%.2d",
		       tim_p->tm_mday);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'e':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2d",
		       tim_p->tm_mday);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'H':
	case 'k':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], *format == 'k' ? "%2d" : "%2.2d",
		       tim_p->tm_hour);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'I':
	case 'l':
	  if (count < maxsize - 2)
	    {
	      if (tim_p->tm_hour == 0 ||
		  tim_p->tm_hour == 12)
		{
		  s[count++] = '1';
		  s[count++] = '2';
		}
	      else
		{
		  sprintf (&s[count], (*format == 'I') ? "%.2d" : "%2d",
			   tim_p->tm_hour % 12);
		  count += 2;
		}
	    }
	  else
	    return 0;
	  break;
	case 'j':
	  if (count < maxsize - 3)
	    {
	      sprintf (&s[count], "%.3d",
		       tim_p->tm_yday + 1);
	      count += 3;
	    }
	  else
	    return 0;
	  break;
	case 'm':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%.2d",
		       tim_p->tm_mon + 1);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'M':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2.2d",
		       tim_p->tm_min);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'p':
	  if (count < maxsize - 2)
	    {
	      if (tim_p->tm_hour < 12)
		s[count++] = 'A';
	      else
		s[count++] = 'P';

	      s[count++] = 'M';
	    }
	  else
	    return 0;
	  break;
	case 'S':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2.2d",
		       tim_p->tm_sec);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'U':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2.2d",
		       (tim_p->tm_yday + 7 -
			tim_p->tm_wday) / 7);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'w':
	  if (count < maxsize - 1)
	    {
	      sprintf (&s[count], "%1.1d",
		       tim_p->tm_wday);
	      count++;
	    }
	  else
	    return 0;
	  break;
	case 'W':
	  if (count < maxsize - 2)
	    {
	      int wday = (tim_p->tm_wday) ? tim_p->tm_wday - 1 : 6;
	      sprintf (&s[count], "%2.2d",
		       (tim_p->tm_yday + 7 -
			wday) / 7);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'x':
	  if (count < maxsize - 15)
	    {
	      for (i = 0; i < 3; i++)
		s[count++] =
		  dname[tim_p->tm_wday][i];
	      s[count++] = ' ';
	      for (i = 0; i < 3; i++)
		s[count++] =
		  mname[tim_p->tm_mon][i];

	      sprintf (&s[count],
		       " %.2d %.4d", tim_p->tm_mday,
		       1900 + tim_p->tm_year);
	      count += 8;
	    }
	  else
	    return 0;
	  break;
	case 'X':
	  if (count < maxsize - 8)
	    {
	      sprintf (&s[count],
		       "%2.2d:%2.2d:%2.2d",
		       tim_p->tm_hour, tim_p->tm_min,
		       tim_p->tm_sec);
	      count += 8;
	    }
	  else
	    return 0;
	  break;
	case 'y':
	  if (count < maxsize - 2)
	    {
	      /* The year could be greater than 100, so we need the value
		 modulo 100.  The year could be negative, so we need to
		 correct for a possible negative remainder.  */
	      sprintf (&s[count], "%2.2d",
		       (tim_p->tm_year % 100 + 100) % 100);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'Y':
	  if (count < maxsize - 4)
	    {
	      sprintf (&s[count], "%.4d",
		       1900 + tim_p->tm_year);
	      count += 4;
	    }
	  else
	    return 0;
	  break;
	case 'Z':
	  if (tim_p->tm_isdst >= 0)
	    {
	      int size;
	      TZ_LOCK;
	      size = strlen(_tzname[tim_p->tm_isdst]);
	      for (i = 0; i < size; i++)
		{
		  if (count < maxsize - 1)
		    s[count++] = _tzname[tim_p->tm_isdst][i];
		  else
		    {
		      TZ_UNLOCK;
		      return 0;
		    }
		}
	      TZ_UNLOCK;
	    }
	  break;
	case '%':
	  if (count < maxsize - 1)
	    s[count++] = '%';
	  else
	    return 0;
	  break;
	}
      if (*format)
	format++;
      else
	break;
    }
  s[count] = '\0';

  return count;
}

/*
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * strptime
 * Original Author:	1999 Kungliga Tekniska Högskolan
 * -----------------------------------------------------------------------
 * Copyright (c) 1999 Kungliga Tekniska Högskolan
 * (Royal Institute of Technology, Stockholm, Sweden). 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *
 * 3. Neither the name of KTH nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY KTH AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL KTH OR ITS CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 ------------------------------------------------------------------------------
*/

static const char *abb_weekdays[] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
    NULL
};

static const char *full_weekdays[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    NULL
};

static const char *abb_month[] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec",
    NULL
};

static const char *full_month[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
    NULL,
};

static const char *ampm[] = {
    "am",
    "pm",
    NULL
};

/*
 * tm_year is relative this year 
 */
const int tm_year_base = 1900;

/*
 * Return TRUE iff `year' was a leap year.
 * Needed for strptime.
 */
static int
is_leap_year (int year)
{
    return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}

/* Needed for strptime. */
static int
match_string (const char **buf, const char **strs)
{
    int i = 0;

    for (i = 0; strs[i] != NULL; ++i) {
	int len = strlen (strs[i]);

	if (strncasecmp (*buf, strs[i], len) == 0) {
	    *buf += len;
	    return i;
	}
    }
    return -1;
}

/* Needed for strptime. */
static int
first_day (int year)
{
    int ret = 4;

    for (; year > 1970; --year)
	ret = (ret + 365 + is_leap_year (year) ? 1 : 0) % 7;
    return ret;
}

/*
 * Set `timeptr' given `wnum' (week number [0, 53])
 * Needed for strptime
 */

static void
set_week_number_sun (struct tm *timeptr, int wnum)
{
    int fday = first_day (timeptr->tm_year + tm_year_base);

    timeptr->tm_yday = wnum * 7 + timeptr->tm_wday - fday;
    if (timeptr->tm_yday < 0) {
	timeptr->tm_wday = fday;
	timeptr->tm_yday = 0;
    }
}

/*
 * Set `timeptr' given `wnum' (week number [0, 53])
 * Needed for strptime
 */

static void
set_week_number_mon (struct tm *timeptr, int wnum)
{
    int fday = (first_day (timeptr->tm_year + tm_year_base) + 6) % 7;

    timeptr->tm_yday = wnum * 7 + (timeptr->tm_wday + 6) % 7 - fday;
    if (timeptr->tm_yday < 0) {
	timeptr->tm_wday = (fday + 1) % 7;
	timeptr->tm_yday = 0;
    }
}

/*
 * Set `timeptr' given `wnum' (week number [0, 53])
 * Needed for strptime
 */
static void
set_week_number_mon4 (struct tm *timeptr, int wnum)
{
    int fday = (first_day (timeptr->tm_year + tm_year_base) + 6) % 7;
    int offset = 0;

    if (fday < 4)
	offset += 7;

    timeptr->tm_yday = offset + (wnum - 1) * 7 + timeptr->tm_wday - fday;
    if (timeptr->tm_yday < 0) {
	timeptr->tm_wday = fday;
	timeptr->tm_yday = 0;
    }
}

/* strptime: roken */

char *strptime (const char *buf, const char *format, struct tm *timeptr)
{
    char c;

    for (; (c = *format) != '\0'; ++format) {
	char *s;
	int ret;

	if (isspace (c)) {
	    while (isspace (*buf))
		++buf;
	} else if (c == '%' && format[1] != '\0') {
	    c = *++format;
	    if (c == 'E' || c == 'O')
		c = *++format;
	    switch (c) {
	    case 'A' :
		ret = match_string (&buf, full_weekdays);
		if (ret < 0)
		    return NULL;
		timeptr->tm_wday = ret;
		break;
	    case 'a' :
		ret = match_string (&buf, abb_weekdays);
		if (ret < 0)
		    return NULL;
		timeptr->tm_wday = ret;
		break;
	    case 'B' :
		ret = match_string (&buf, full_month);
		if (ret < 0)
		    return NULL;
		timeptr->tm_mon = ret;
		break;
	    case 'b' :
	    case 'h' :
		ret = match_string (&buf, abb_month);
		if (ret < 0)
		    return NULL;
		timeptr->tm_mon = ret;
		break;
	    case 'C' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_year = (ret * 100) - tm_year_base;
		buf = s;
		break;
	    case 'c' :
		abort ();
	    case 'D' :		/* %m/%d/%y */
		s = strptime (buf, "%m/%d/%y", timeptr);
		if (s == NULL)
		    return NULL;
		buf = s;
		break;
	    case 'd' :
	    case 'e' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_mday = ret;
		buf = s;
		break;
	    case 'H' :
	    case 'k' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_hour = ret;
		buf = s;
		break;
	    case 'I' :
	    case 'l' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		if (ret == 12)
		    timeptr->tm_hour = 0;
		else
		    timeptr->tm_hour = ret;
		buf = s;
		break;
	    case 'j' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_yday = ret - 1;
		buf = s;
		break;
	    case 'm' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_mon = ret - 1;
		buf = s;
		break;
	    case 'M' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_min = ret;
		buf = s;
		break;
	    case 'n' :
		if (*buf == '\n')
		    ++buf;
		else
		    return NULL;
		break;
	    case 'p' :
		ret = match_string (&buf, ampm);
		if (ret < 0)
		    return NULL;
		if (timeptr->tm_hour == 0) {
		    if (ret == 1)
			timeptr->tm_hour = 12;
		} else
		    timeptr->tm_hour += 12;
		break;
	    case 'r' :		/* %I:%M:%S %p */
		s = strptime (buf, "%I:%M:%S %p", timeptr);
		if (s == NULL)
		    return NULL;
		buf = s;
		break;
	    case 'R' :		/* %H:%M */
		s = strptime (buf, "%H:%M", timeptr);
		if (s == NULL)
		    return NULL;
		buf = s;
		break;
	    case 'S' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_sec = ret;
		buf = s;
		break;
	    case 't' :
		if (*buf == '\t')
		    ++buf;
		else
		    return NULL;
		break;
	    case 'T' :		/* %H:%M:%S */
	    case 'X' :
		s = strptime (buf, "%H:%M:%S", timeptr);
		if (s == NULL)
		    return NULL;
		buf = s;
		break;
	    case 'u' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_wday = ret - 1;
		buf = s;
		break;
	    case 'w' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_wday = ret;
		buf = s;
		break;
	    case 'U' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		set_week_number_sun (timeptr, ret);
		buf = s;
		break;
	    case 'V' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		set_week_number_mon4 (timeptr, ret);
		buf = s;
		break;
	    case 'W' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		set_week_number_mon (timeptr, ret);
		buf = s;
		break;
	    case 'x' :
		s = strptime (buf, "%Y:%m:%d", timeptr);
		if (s == NULL)
		    return NULL;
		buf = s;
		break;
	    case 'y' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		if (ret < 70)
		    timeptr->tm_year = 100 + ret;
		else
		    timeptr->tm_year = ret;
		buf = s;
		break;
	    case 'Y' :
		ret = strtol (buf, &s, 10);
		if (s == buf)
		    return NULL;
		timeptr->tm_year = ret - tm_year_base;
		buf = s;
		break;
	    case 'Z' :
		abort ();
	    case '\0' :
		--format;
		/* FALLTHROUGH */
	    case '%' :
		if (*buf == '%')
		    ++buf;
		else
		    return NULL;
		break;
	    default :
		if (*buf == '%' || *++buf == c)
		    ++buf;
		else
		    return NULL;
		break;
	    }
	} else {
	    if (*buf == c)
		++buf;
	    else
		return NULL;
	}
    }
    return (char *)buf;
}



void __tz_lock (void)
{
  // __lock_acquire (__tz_lock_object);
  // No effect under PSP
}

void __tz_unlock (void)
{
  //__lock_release (__tz_lock_object);
  // No effect under PSP
}



/*
 * -----------------------------------------------------------------------
 * Service Date Functions
 * -----------------------------------------------------------------------
*/

 

int DetDayOfWeek (int y, int m, int d) 
/* 0 = Sunday ; 1 <= m <= 12, y > 1752 or so */
/* (Copyright 1993, Tomohiko Sakamoto) */ 
{
   static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
   
   y -= m < 3;
   return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

int DetDayOfYear (int Year, int Month, int Day) 
{ 
    int Counter;
    int DaysOfMonth [13]={0,31,28,31,30,31,30,31,31,30,31,30,31}; 

    // controlla se l'Year e' bisestile 

    if ( ((Year%4)==0 && (Year%100)!=0) || ((Year%400)==0) ) 
    {
         DaysOfMonth[2]=29; 
    }

    Counter=0;
    
    while (Month<=12) 
    { 
          while (Day<=DaysOfMonth[Month]) 
          { 
            Counter++;
            Day++; 
          } 
          
          Month++; 
          Day=1; 
    } 

    return Counter;
} 


/*
   ----------------------------------------------------------------------------
   TZSET
   ----------------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * init.c
 *
 * Copyright (c) 2002-2004 PS2DEV
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: init.c 1095 2005-09-27 21:02:16Z jim $
   ----------------------------------------------------------------------
*/


static char __tzname_std[11];
static char __tzname_dst[11];
static char *prev_tzenv = NULL;

_VOID
_DEFUN (_tzset_r, (reent_ptr),
        struct _reent *reent_ptr)
{
  char *tzenv;
  unsigned short hh, mm, ss, m, w, d;
  int sign, n;
  int i, ch;

  if ((tzenv = getenv ("TZ")) == NULL)
      {
	TZ_LOCK;
	_timezone = (time_t)0;
	_daylight = 0;
	_tzname[0] = "GMT";
	_tzname[1] = "GMT";
	TZ_UNLOCK;
	return;
      }

  TZ_LOCK;

  if (prev_tzenv != NULL && strcmp(tzenv, prev_tzenv) == 0)
    {
      TZ_UNLOCK;
      return;
    }

  free(prev_tzenv);
  prev_tzenv = malloc (strlen(tzenv) + 1);
  if (prev_tzenv != NULL)
    strcpy (prev_tzenv, tzenv);

  /* ignore implementation-specific format specifier */
  if (*tzenv == ':')
    ++tzenv;  

  if (sscanf (tzenv, "%10[^0-9,+-]%n", __tzname_std, &n) <= 0)
    {
      TZ_UNLOCK;
      return;
    }
 
  tzenv += n;

  sign = 1;
  if (*tzenv == '-')
    {
      sign = -1;
      ++tzenv;
    }
  else if (*tzenv == '+')
    ++tzenv;

  mm = 0;
  ss = 0;
 
  if (sscanf (tzenv, "%hu%n:%hu%n:%hu%n", &hh, &n, &mm, &n, &ss, &n) < 1)
    {
      TZ_UNLOCK;
      return;
    }
  
  __tzrule[0].offset = sign * (ss + SECSPERMIN * mm + SECSPERHOUR * hh);
  _tzname[0] = __tzname_std;
  tzenv += n;
  
  if (sscanf (tzenv, "%10[^0-9,+-]%n", __tzname_dst, &n) <= 0)
    {
      _tzname[1] = _tzname[0];
      TZ_UNLOCK;
      return;
    }
  else
    _tzname[1] = __tzname_dst;

  tzenv += n;

  /* otherwise we have a dst name, look for the offset */
  sign = 1;
  if (*tzenv == '-')
    {
      sign = -1;
      ++tzenv;
    }
  else if (*tzenv == '+')
    ++tzenv;

  hh = 0;  
  mm = 0;
  ss = 0;
  
  if (sscanf (tzenv, "%hu%n:%hu%n:%hu%n", &hh, &n, &mm, &n, &ss, &n) <= 0)
    __tzrule[1].offset = __tzrule[0].offset - 3600;
  else
    __tzrule[1].offset = sign * (ss + SECSPERMIN * mm + SECSPERHOUR * hh);

  tzenv += n;

  for (i = 0; i < 2; ++i)
    {
      if (*tzenv == ',')
        ++tzenv;

      if (*tzenv == 'M')
	{
	  if (sscanf (tzenv, "M%hu%n.%hu%n.%hu%n", &m, &n, &w, &n, &d, &n) != 3 ||
	      m < 1 || m > 12 || w < 1 || w > 5 || d > 6)
	    {
	      TZ_UNLOCK;
	      return;
	    }
	  
	  __tzrule[i].ch = 'M';
	  __tzrule[i].m = m;
	  __tzrule[i].n = w;
	  __tzrule[i].d = d;
	  
	  tzenv += n;
	}
      else 
	{
	  char *end;
	  if (*tzenv == 'J')
	    {
	      ch = 'J';
	      ++tzenv;
	    }
	  else
	    ch = 'D';
	  
	  d = strtoul (tzenv, &end, 10);
	  
	  /* if unspecified, default to US settings */
	  if (end == tzenv)
	    {
	      if (i == 0)
		{
		  __tzrule[0].ch = 'M';
		  __tzrule[0].m = 4;
		  __tzrule[0].n = 1;
		  __tzrule[0].d = 0;
		}
	      else
		{
		  __tzrule[1].ch = 'M';
		  __tzrule[1].m = 10;
		  __tzrule[1].n = 5;
		  __tzrule[1].d = 0;
		}
	    }
	  else
	    {
	      __tzrule[i].ch = ch;
	      __tzrule[i].d = d;
	    }
	  
	  tzenv = end;
	}
      
      /* default time is 02:00:00 am */
      hh = 2;
      mm = 0;
      ss = 0;
      n = 0;
      
      if (*tzenv == '/')
	sscanf (tzenv, "/%hu%n:%hu%n:%hu%n", &hh, &n, &mm, &n, &ss, &n);

      __tzrule[i].s = ss + SECSPERMIN * mm + SECSPERHOUR  * hh;
      
      tzenv += n;
    }

  __tzcalc_limits (__tzyear);
  _timezone = (time_t)(__tzrule[0].offset);  
  _daylight = __tzrule[0].offset != __tzrule[1].offset;

  TZ_UNLOCK;
}



void tzset(void)
{
    static int initialized = 0;

	if (!initialized)
	{
		initialized = 1;

		// Don't init if TZ has already been set once, this probably means the user
		//   wanted to override what we would set below. 
		if (getenv("TZ") == NULL)
		{
			// Initialize timezone from PSP configuration 
			int tzOffset = 0;
			
            // sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_TIMEZONE, &tzOffset);
			// Sotto PSPE non è implementato. Poniamo tzOffset=1
            
            tzOffset=1;
            
            int tzOffsetAbs = tzOffset < 0 ? -tzOffset : tzOffset;
			int hours = tzOffsetAbs / 60;
			int minutes = tzOffsetAbs - hours * 60;
			int pspDaylight = 0;
			
            //sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_DAYLIGHTSAVINGS, &pspDaylight);
			// Sotto PSPE non è implementato. Poniamo pspDayLight=1
			
			pspDaylight=1;
            
            static char tz[18];
			sprintf(tz, "GMT%s%02i:%02i%s", tzOffset < 0 ? "+" : "-", hours, minutes, pspDaylight ? "daylight" : "");
			setenv("TZ", tz, 1);
		}
	}

	_tzset_r(_REENT);
	
	
}



