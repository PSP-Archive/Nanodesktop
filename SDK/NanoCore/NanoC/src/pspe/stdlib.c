/*
   --------------------------------------------------------------------------
   stdlib.c                                      NanoC Files (PSPE)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines standard di libreria 
   ----------------------------------------------------------------------------
*/   
 
     #include <pspe/$$_ded_stdlib.h>
     
 
/*     
 * -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * stdlib.c
 *
 * Copyright (c) 2002-2004 PS2DEV
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: stdlib.c 1095 2005-09-27 21:02:16Z jim $
   -----------------------------------------------------------------------
*/


/* 
   -----------------------------------------------------------------------
   SIMBOLI INTERNI DI LIBRERIA
   -----------------------------------------------------------------------
*/

        
        extern void (* __stdlib_exit_func[32])(void);
        extern int     __stdlib_exit_index;
        
        /*
        // This function is missing...
        // char *__stdlib_ecvt(double, size_t, int *, int *);
        
        extern environvariable_t __stdlib_env[32];
        extern int __stdlib_mb_shift;
        extern unsigned int __stdlib_rand_seed;
        */                                             // Non decommentare
        
        
        #define MBCHAR_BUF_SIZE 24
        
        /* stdlib data variables. */
        environvariable_t    __stdlib_env[32];
        void                 (* __stdlib_exit_func[32])(void);
        int                  __stdlib_exit_index = 0;
        int                  __stdlib_mb_shift = 0;
        static unsigned int  __stdlib_rand_seed = 1;

        static void swapfunc (char *a, char *b, int n, int swaptype); 
        static char *med3 (char *a, char *b, char *c, cmp_t (*cmp), void *thunk); 
        
        unsigned short __rand48_seed[3] = 
        {
        	RAND48_SEED_0,
        	RAND48_SEED_1,
        	RAND48_SEED_2
        };
        
        unsigned short __rand48_mult[3] = 
        {
        	RAND48_MULT_0,
        	RAND48_MULT_1,
        	RAND48_MULT_2
        };
        
        unsigned short __rand48_add = RAND48_ADD;


/* 
   -----------------------------------------------------------------------
   RIFERIMENTI A ROUTINES NANODESKTOP CHE SARANNO RICHIAMATE IN SEGUITO
   -----------------------------------------------------------------------
*/

        extern void ndSystemHalt (int ExitCode); 


/* 
   -----------------------------------------------------------------------
   ROUTINES DI INIZIALIZZAZIONE/DEINIZIALIZZAZIONE
   -----------------------------------------------------------------------
*/

void _pspsdk_stdlib_init()
{
    int i;                   // Reinizializza i buffer di ambiente
     
    for (i=0; i<32; i++)
    {
         memset ( &(__stdlib_env[i]), 0, sizeof (environvariable_t) );
    }
    
    // Impostazioni di base per LOCALE
    
    setenv ("LC_CTYPE",     "en_GB.ISO8859-1", 1);
    setenv ("LC_COLLATE",   "en_GB.ISO8859-1", 1);
    setenv ("LC_TIME",      "en_GB.ISO8859-1", 1);
    setenv ("LC_NUMERIC",   "en_GB.ISO8859-1", 1);
    setenv ("LC_MONETARY",  "en_GB.ISO8859-1", 1);
    setenv ("LC_MESSAGES",  "en_GB.ISO8859-1", 1);
    setenv ("decimal_point", ".", 1);
    setenv ("thousands_sep", ",", 1);
}

void _pspsdk_stdlib_deinit()
{
	int i;

	for (i = (__stdlib_exit_index - 1); i >= 0; --i) 
	{
		(__stdlib_exit_func[i])();
	}
}


/* 
   -----------------------------------------------------------------------
   ROUTINES VARIE
   -----------------------------------------------------------------------
*/


/*
**
**  [func] - atexit.
**  [desc] - if the current amount of registered exit() functions has not
**           been reached then registers the func parameter function to the
**           list and returns 0. else returns non-zero.
**  [entr] - void (*func)(void); the pointer to the exit function.
**  [exit] - int; 0 if albe to register the func exit() function. else non-zero.
**  [prec] - func is a valid function pointer.
**  [post] - the atexit() function list is modified.
**
*/
int atexit(void (*func)(void))
{
  int ret;
  
  if (__stdlib_exit_index < 32) {
    /* register func to the exit() function list. */
    __stdlib_exit_func[__stdlib_exit_index++] = func;
    ret = 0;
  }
  else ret = -1;
  return (ret);
}


/*
**
**  [func] - exit.
**  [desc] - calls all the register exit() functions and returns to PlayStation2
**           OSD.
**  [entr] - int status; the exit status code.
**  [exit] - this function deos not return.
**  [prec] - none.
**  [post] - none.
**
*/
void exit(int status)
{
  int i;

  for (i=(__stdlib_exit_index-1); i>=0; i--)
	__stdlib_exit_func[i]();

  ndSystemHalt (status);
}

void done (int status) 
{
  exit (status);
}


/*
**
**  [func] - abs.
**  [desc] - returns the absolute value of the integer c.
**  [entr] - int c; the integer value.
**  [exit] - int; the absolute value of the integer c.
**  [prec] - none.
**  [post] - none.
**
*/

// shouldn't we rather put that as a macro... ?
int abs(int c)
{
  return ((c >= 0) ? c : -c);
}


/*
**
**  [func] - labs.
**  [desc] - returns the absolute value of the long integer n.
**  [entr] - long n; the long integer value.
**  [exit] - long; the absolute value of the long integer n.
**  [prec] - none.
**  [post] - none.
**
*/
long labs(long n)
{
  return ((n >= 0) ? n : -n);
}


/*
**
**  [func] - llabs.
**  [desc] - returns the absolute value of the long long integer n.
**  [entr] - long n; the long long integer value.
**  [exit] - long; the absolute value of the long long integer n.
**  [prec] - none.
**  [post] - none.
**
*/
long long llabs(long long n)
{
  return ((n >= 0) ? n : -n);
}



/*
**
**  [func] - bsearch.
**  [desc] -
**  [entr] - const void *key; the pointer to the search key object.
**           const void *base; the pointer to the base of the search data.
**           size_t count; the number of elements in the search data.
**           size_t size; the size of the search elements.
**           int (* compare)(const void *, const void *); the pointer to the compare function.
**  [exit] - void *;
**  [prec] -
**  [post] -
**
*/
void *bsearch(const void *key, const void *base, size_t count, size_t size, int (* compare)(const void *, const void *))
{
  int        comparison;
  size_t     l, u, idx;
  void       *ret = NULL;
  const void *p;

  /* perform a binary search of a sorted array. */
  for (l = 0, u = count; l < u; ) {
    idx = ((l + u) / 2);
    /* calculate the pointer index. */
    p = (const void *)((const char *)base + (idx * size));
    comparison = (*compare)(key, p);
    if (comparison < 0) u = idx;
    else if (comparison > 0) l = (idx + 1);
    else {
      /* return the pointer. */
      ret = (void *)p;
      break;
    }
  }
  return (ret);
}


/*
**
**  [func] - div.
**  [desc] -
**  [entr] - int n; the integer numerator.
**           int d; the integer divisor.
**  [exit] - div_t;
**  [prec] - none.
**  [post] - none.
**
*/
div_t div(int n, int d)
{
  div_t ret;

  /* calculate the quotient and remainder. */
  // duh... can't this be written with some asm "mfhi/mflo" ?
  ret.quot = (n / d);
  ret.quot = (n % d);
  ret.rem = 0; // set to 0 so it won't be uninitialized
  return (ret);
}





/*
**
**  [func] - ldiv.
**  [desc] -
**  [entr] - long n; the long integer numerator.
**           long d; the long integer denominator.
**  [exit] - ldiv_t;
**  [prec] -
**  [post] -
**
*/
ldiv_t ldiv(long n, long d)
{
  ldiv_t ret;

  ret.quot = (n / d);
  ret.rem = (n % d);
  return (ret);
}


/*
**
**  [func] - lldiv.
**  [desc] -
**  [entr] - long long n; the long long integer numerator.
**           long long d; the long long integer denominator.
**  [exit] - ldiv_t;
**  [prec] -
**  [post] -
**
*/
lldiv_t lldiv(long long n, long long d)
{
  lldiv_t ret;

  ret.quot = (n / d);
  ret.rem = (n % d);
  return (ret);
}


/*
**
**  [func] - _itoa.
**  [desc] -
**  [entr] - int n; the integer value to convert.
**           char *buf; the pointer to the destination memory buffer.
**           int radix; the conversion number base.
**  [exit] - char *; buf.
**  [prec] - buf is a valid memory pointer.
**  [post] - the memory pointed to by buf is modified.
**
*/
char *_itoa(int n, char *buf, int radix)
{
  char         *ret = buf;
  char         tmp[33];
  int          i = 0, j, r;

  /* validate the conversion number base. */
  if ((radix >= 2) && (radix <= 36)) {
    if ((radix == 10) && (n < 0)) {
      /* negative integer value. */
      *buf++ = '-';
      n = -n;
    }
    do {
      /* calculate the current digit. */
      r = (int)((unsigned int)n % radix);
      tmp[i++] = ((r < 10) ? (r + '0') : (r - 10 + 'a'));
    } while ((n /= radix) != 0);
    /* reverse the buffer string. */
    for (--i, j = 0; (i >= 0); --i, ++j) buf[j] = tmp[i];
    buf[j] = 0;
  }
  return (ret);
}



/*
**
**  [func] - _lltoa.
**  [desc] -
**  [entr] - long long n; the long long integer value to convert.
**           char *buf; the pointer to the destination memory buffer.
**           int radix; the conversion number base.
**  [exit] - char *; buf.
**  [prec] - buf is a valid memory pointer.
**  [post] - the memory pointed to by buf is modified.
**
*/
char *_lltoa(long long n, char *buf, int radix)
{
  char         *ret = buf;
  char         tmp[65];
  int          i = 0, j;
  long long    r;

  /* validate the conversion number base. */
  if ((radix >= 2) && (radix <= 36)) {
    if ((radix == 10) && (n < 0)) {
      /* negative integer value. */
      *buf++ = '-';
      n = -n;
    }
    do {
      /* calculate the current digit. */
      r = (long long)((unsigned long long)n % radix);
      tmp[i++] = ((r < 10) ? (r + '0') : (r - 10 + 'a'));
    } while ((n /= radix) != 0);
    /* reverse the buffer string. */
    for (--i, j = 0; (i >= 0); --i, ++j) buf[j] = tmp[i];
    buf[j] = 0;
  }
  return (ret);
}


/*
**
**  [func] - _ltoa.
**  [desc] -
**  [entr] - long n; the long integer value to convert.
**           char *buf; the pointer to the destination memory buffer.
**           int radix; the conversion number base.
**  [exit] - char *; buf.
**  [prec] - buf is a valid memory pointer.
**  [post] - the memory pointed to by buf is modified.
**
*/
char *_ltoa(long n, char *buf, int radix)
{
  char         *ret = buf;
  char         tmp[33];
  int          i = 0, j;
  long         r;

  /* validate the conversion number base. */
  if ((radix >= 2) && (radix <= 36)) {
    if ((radix == 10) && (n < 0)) {
      /* negative integer value. */
      *buf++ = '-';
      n = -n;
    }
    do {
      /* calculate the current digit. */
      r = (long)((unsigned long)n % radix);
      tmp[i++] = ((r < 10) ? (r + '0') : (r - 10 + 'a'));
    } while ((n /= radix) != 0);
    /* reverse the buffer string. */
    for (--i, j = 0; (i >= 0); --i, ++j) buf[j] = tmp[i];
    buf[j] = 0;
  }
  return (ret);
}








/*
**
**  [func] - rand.
**  [desc] - returns the random number generated from the current stdlib random
**           seed.
**  [entr] - none.
**  [exit] - int; the random number generated from the current stdlib random seed.
**  [prec] - none.
**  [post] - the stdlib random seed is modified.
**
*/
int rand(void)
{
    __stdlib_rand_seed = (161140751 * __stdlib_rand_seed + 13) % 219441163;

	return __stdlib_rand_seed;
}


/*
**
**  [func] - srand.
**  [desc] - sets the current stdlib random seed to seed.
**  [entr] - unsigned int seed; the stdlib random seed.
**  [exit] - none.
**  [prec] - none.
**  [post] - none.
**
*/
void srand(unsigned int seed)
{
  __stdlib_rand_seed = seed;
}


/*
**
**  [func] - getenv.
**  [desc] - if name is an existing environment variable name then returns the
**           poiinter to the corresponding environment variable string value.
**           else returns NULL.
**  [entr] - const char *name; the environment name string pointer.
**  [exit] - char *; the ptr. to the corres. environment variable string. else NULL.
**  [prec] - name is a valid string pointer.
**  [post] - none.
**
*/
char *getenv(const char *name)
{
  int  i;
  char *ret = NULL;

  /* search for matching environment variable name. */
  for (i = 0; i < 32; ++i) {
    if (strcmp(name, __stdlib_env[i].name) == 0) {
      /* return the environment variable value. */
      ret = (char *)__stdlib_env[i].value;
      break;
    }
  }
  return (ret);
}




/*
**
**  [func] - setenv.
**  [desc] - if name is an existing environment variable and rewrite is non-zero
**           then overwrites the name environment variable value with value and
**           returns 0. else if name is not an existring environment variable and
**           there is a free environment variable slot available then sets the
**           name environment variable and returns 0. else returns -1.
**  [entr] - const char *name; the environment variable name string pointer.
**           const char *value; the environment variable value string pointer.
**           int rewrite; the overwrite flag.
**  [exit] - int; 0 if able to set the environment variable successfully. else -1.
**  [prec] - name and value are valid string pointers.
**  [post] - the name environment variable is set.
**
*/
int setenv(const char *name, const char *value, int rewrite)
{
  int done, i, ret = -1;

  if (name)
  {
      /* search for matching environment variable name. */
      for (i = 0, done = 0; i < 32; ++i) 
      {
        if (strcmp(name, __stdlib_env[i].name) == 0) 
        {
              if (rewrite) 
              {
                /* overwrite the current environment variable value. */
                strncpy(__stdlib_env[i].value, value, 255);
                __stdlib_env[i].value[255] = 0;
                ret = 0;
              }
              
              done = 1;
              break;
        }
      }
      
      if (!done) 
      {
            /* search for a free environment variable slot. */
            for (i = 0; i < 32; ++i) 
            {
                  if (__stdlib_env[i].name[0] == '\0') 
                  {
                    /* set the name environment variable. */
                    strncpy(__stdlib_env[i].name, name, 255);
                    __stdlib_env[i].name[255] = 0;
                    strncpy(__stdlib_env[i].value, value, 255);
                    __stdlib_env[i].value[255] = 0;
                    ret = 0;
                    break;
                  }
            }
      }     
  }
  
  return (ret);
}



int putenv (const char *string)
/* Put STRING, which is of the form "NAME=VALUE", in the environment.
   If STRING contains no `=', then remove STRING from the environment.  */
{
  char _name [256], _value [256];
  
  char *name_end = strchr (string, '=');
  // Determina se c'è un carattere = e dove si trova
  
  if (name_end == NULL)            // Non c'è alcun carattere =: cancella la variabile
  { 
      return unsetenv (string);
  }
  
  // Isola nome e valore
  
  strncpy (_name,   string, name_end - string);
  strcpy  (_value,  name_end+1);
  
  // Provvedi a modificare il valore
  
  return (setenv (_name, _value, 1));
}


int unsetenv (const char *name)
{
   return ( setenv (name, "", 1) ); 
}





int __assert_fail (const char *assertion, const char *file, unsigned int line)
{
    fprintf(stderr, "Error: assertion `%s' failed in %s:%i\n", assertion, file, line);
	return 0;
}



/* 
   ----------------------------------------------------------------------------
   Routines sostitutive
   ----------------------------------------------------------------------------
*/


long strtol(const char *nptr, char **endptr, int base)
/*
**  [func] - strtol.
**  [desc] - if s is a valid long integer string then converts the string to
**           it's corresponding long integer value and returns the value. else
**           returns the long integer huge value. if eptr is not NULL then
**           stores the pointer to the last processed character in the string.
**  [entr] - const char *s; the source string pointer.
**           char **eptr; the pointer to store the string end pointer.
**           int b; the long integer base.
**  [exit] - long; the converted long integer value. else the long integer huge value.
**  [prec] - s is a valid string pointer and eptr is a valid string pointer
**           pointer.
**  [post] - the memory pointed to by eptr is modified.
**
** -----------------------------------------------------------------------------------
  * Copyright (c) 1990, 1993
  *      The Regents of the University of California.  All rights reserved.
  *
  * This code is derived from software contributed to Berkeley by
  * Chris Torek.
  *
  
  * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. [rescinded 22 July 1999]
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 ------------------------------------------------------------------------------
*/ 

{
         const char *s = nptr;
         unsigned long acc;
         unsigned char c;
         unsigned long cutoff;
         int neg = 0, any, cutlim;
 
         /*
          * Skip white space and pick up leading +/- sign if any.
          * If base is 0, allow 0x for hex and 0 for octal, else
          * assume decimal; if base is already 16, allow 0x.
          */
         do {
                 c = *s++;
         } while (isspace(c));
         
         if (c == '-') 
         {
                 neg = 1;
                 c = *s++;
         } 
         else 
         if (c == '+')
                 c = *s++;
         
         if ((base == 0 || base == 16) && (c == '0') && (*s == 'x' || *s == 'X')) 
         {
                 c = s[1];
                 s += 2;
                 base = 16;
         }
         
         if (base == 0)
                 base = c == '0' ? 8 : 10;
 
         /*
          * Compute the cutoff value between legal numbers and illegal
          * numbers.  That is the largest legal value, divided by the
          * base.  An input number that is greater than this value, if
          * followed by a legal input character, is too big.  One that
          * is equal to this value may be valid or not; the limit
          * between valid and invalid numbers is then based on the last
          * digit.  For instance, if the range for longs is
          * [-2147483648..2147483647] and the input base is 10,
          * cutoff will be set to 214748364 and cutlim to either
          * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
          * a value > 214748364, or equal but the next digit is > 7 (or 8),
          * the number is too big, and we will return a range error.
          *
          * Set any if any `digits' consumed; make it negative to indicate
          * overflow.
          */
         cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
         cutlim = cutoff % (unsigned long)base;
         cutoff /= (unsigned long)base;
         for (acc = 0, any = 0;; c = *s++) {
                 if (!isascii(c))
                         break;
                 if (isdigit(c))
                         c -= '0';
                 else if (isalpha(c))
                         c -= isupper(c) ? 'A' - 10 : 'a' - 10;
                 else
                         break;
                 if (c >= base)
                         break;
                 if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
                         any = -1;
                 else {
                         any = 1;
                         acc *= base;
                         acc += c;
                 }
         }
         if (any < 0) {
                 acc = neg ? LONG_MIN : LONG_MAX;
         } else if (neg)
                 acc = -acc;
         if (endptr != 0)
                 *((const char **)endptr) = any ? s - 1 : nptr;
         return (acc);
}


/*
  ----------------------------------------------------------------------------
  DESCRIPTION
   <<atoi>> converts the initial portion of a string to an <<int>>.
   <<atol>> converts the initial portion of a string to a <<long>>.

   <<atoi(s)>> is implemented as <<(int)strtol(s, NULL, 10).>>
   <<atol(s)>> is implemented as <<strtol(s, NULL, 10).>>

  RETURNS
   The functions return the converted value, if any. If no conversion was
   made, <<0>> is returned.

  PORTABILITY
  <<atoi>> is ANSI.
  -----------------------------------------------------------------------------
  Copyright Andy Wilson, 2-Oct-89.
  -----------------------------------------------------------------------------
*/


int atoi (const char *s)
{
  return (int) strtol (s, NULL, 10);
}

long atol (const char *s)
{
  return strtol (s, NULL, 10);
}


unsigned long strtoul (const char *nptr, char **endptr, int base)
/*
**
**  [func] - strtoul.
**  [desc] - if s is a valid long integer string then converts the string to
**           it's corresponding long integer value and returns the value. else
**           returns the long integer huge value. if eptr is not NULL then
**           stores the pointer to the last processed character in the string.
**  [entr] - const char *s; the source string pointer.
**           char **eptr; the pointer to store the string end pointer.
**           int b; the long integer base.
**  [exit] - long; the converted long integer value. else the long integer huge value.
**  [prec] - s is a valid string pointer and eptr is a valid string pointer
**           pointer.
**  [post] - the memory pointed to by eptr is modified.
**
 ---------------------------------------------------------------------------------
 * Copyright (c) 1990 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. [rescinded 22 July 1999]
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * --------------------------------------------------------------------------
*/

{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	
	if (c == '-') 
    {
		neg = 1;
		c = *s++;
	} 
    else if (c == '+') c = *s++;
    
	if ((base == 0 || base == 16) && (c == '0') && (*s == 'x' || *s == 'X')) 
    {
		c = s[1];
		s += 2;
		base = 16;
	}
	
    if (base == 0)
		base = c == '0' ? 8 : 10;
	
    cutoff = (unsigned long)ULONG_MAX / (unsigned long) base;
	cutlim = (unsigned long)ULONG_MAX % (unsigned long) base;
	
    for (acc = 0, any = 0;; c = *s++) 
    {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	
	if (any < 0) {
		acc = ULONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	
    if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	
    return (acc);
}



double strtod (const char *str, char **endptr)
/*
**
**  [func] - strtod.
**  [desc] - if s is a valid floating point number string then converts the
**           string to it's corresponding float point value and returns the
**           value. else returns 0.0. if eptr is not NULL then stores the
**           pointer to the last processed character in the string.
**  [entr] - const char *s; the source string pointer.
**           char **endptr; the pointer to the store string end pointer.
**  [exit] - double; the converted 64-bit float value. else 0.0.
**  [prec] - s is a valid string pointer and eptr is a valid string pointer
**           pointer.
**  [post] - the memory pointed to by eptr is modified.
**
// ------------------------------------------------------------------------------
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.  
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.  
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
// SUCH DAMAGE.
// 
// ---------------------------------------------------------------------------------
*/


{                                                                  
  double number;
  int exponent;
  int negative;
  char *p = (char *) str;
  double p10;
  int n;
  int num_digits;
  int num_decimals;

  // Skip leading whitespace
  while (isspace(*p)) p++;

  // Handle optional sign
  negative = 0;
  switch (*p) 
  {             
    case '-': negative = 1; // Fall through to increment position
    case '+': p++;
  }

  number = 0.;
  exponent = 0;
  num_digits = 0;
  num_decimals = 0;

  // Process string of digits
  while (isdigit(*p))
  {
    number = number * 10. + (*p - '0');
    p++;
    num_digits++;
  }

  // Process decimal part
  if (*p == '.') 
  {
    p++;

    while (isdigit(*p))
    {
      number = number * 10. + (*p - '0');
      p++;
      num_digits++;
      num_decimals++;
    }

    exponent -= num_decimals;
  }

  if (num_digits == 0)
  {
    errno = ERANGE;
    return 0.0;
  }

  // Correct for sign
  if (negative) number = -number;

  // Process an exponent string
  if (*p == 'e' || *p == 'E') 
  {
    // Handle optional sign
    negative = 0;
    switch(*++p) 
    {   
      case '-': negative = 1;   // Fall through to increment pos
      case '+': p++;
    }

    // Process string of digits
    n = 0;
    while (isdigit(*p)) 
    {   
      n = n * 10 + (*p - '0');
      p++;
    }

    if (negative) 
      exponent -= n;
    else
      exponent += n;
  }

  if (exponent < DBL_MIN_EXP  || exponent > DBL_MAX_EXP)
  {
    errno = ERANGE;
    return HUGE_VAL;
  }

  // Scale the result
  p10 = 10.;
  n = exponent;
  if (n < 0) n = -n;
  while (n) 
  {
    if (n & 1) 
    {
      if (exponent < 0)
        number /= p10;
      else
        number *= p10;
    }
    n >>= 1;
    p10 *= p10;
  }

  if (number == HUGE_VAL) errno = ERANGE;
  if (endptr) *endptr = p;

  return number;
}


float strtof (const char *str, char **endptr)
{
      return (float)(strtod (str, endptr));
}


double atof (const char *str)
/*
**
**  [func] - atof.
**  [desc] - if the string s begins with a valid floating point string then
**           returns the floating point value of the string s. else returns 0.**  
**  [entr] - const char *s; the source string pointer.
**  [exit] - double; the floating point value of the string s. else 0.
**  [prec] - s is a valid string pointer.
**  [post] - none.
*
*/

{
  return strtod(str, NULL);
}

/* --------------------------- Fine del codice Ringaard ---------------------- */





/*
   ----------------------------------------------------------------------------
   ROUTINES DI RICERCA
   ----------------------------------------------------------------------------
*/

void *lfind(const void *key, const void *base, size_t *nelp,
         size_t width, int (*compar)(const void *, const void *))
{
	char *char_base = (char *)base;
	int i;
	for(i=0;i<*nelp;i++) {
		if ( compar(key,char_base) == 0)
			return char_base;
		char_base += width;
	}
	return NULL;
}




void *lsearch(const void *key, void *base, size_t *nelp, size_t width,
         int (*compar)(const void *, const void *))
{
	void *ret_find = lfind(key,base,nelp,width,compar);
	if ( ret_find != NULL )
		return ret_find;

	memcpy( base + (*nelp*width), key, width );
        (*nelp)++;
        return base ;
}


/*
   ----------------------------------------------------------------------------
   CHIAMATE A CONSOLE DI SISTEMA
   ----------------------------------------------------------------------------
*/



int system (const char *string)
{
    return -1;
    // Non è ancora disponibile
}


/*
   ----------------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org       QSORT
 * ----------------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * qsort.c - QSort algorithm implementation.
 *
 * Copyright (c) 1992, 1993 The Regents of the University of California.
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: qsort.c 540 2005-07-08 19:35:10Z warren $
 
                                  BSD License:
 
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------
 */


#define min(a, b)	(a) < (b) ? (a) : (b)

/*
 * Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
 */
#define swapcode(TYPE, parmi, parmj, n) { 		\
	long i = (n) / sizeof (TYPE); 			\
	register TYPE *pi = (TYPE *) (parmi); 		\
	register TYPE *pj = (TYPE *) (parmj); 		\
	do { 						\
		register TYPE	t = *pi;		\
		*pi++ = *pj;				\
		*pj++ = t;				\
        } while (--i > 0);				\
}

#define SWAPINIT(a, es) swaptype = ((char *)a - (char *)0) % sizeof(long) || \
	es % sizeof(long) ? 2 : es == sizeof(long)? 0 : 1;

static __inline void
swapfunc(char *a, char *b, int n, int swaptype)
{
	if(swaptype <= 1)
		swapcode(long, a, b, n)
	else
		swapcode(char, a, b, n)
}

#define swap(a, b)					\
	if (swaptype == 0) {				\
		long t = *(long *)(a);			\
		*(long *)(a) = *(long *)(b);		\
		*(long *)(b) = t;			\
	} else						\
		swapfunc(a, b, es, swaptype)

#define vecswap(a, b, n) 	if ((n) > 0) swapfunc(a, b, n, swaptype)

#define	CMP(t, x, y) (cmp((x), (y)))

static __inline char *
med3(char *a, char *b, char *c, cmp_t *cmp, void *thunk
)
{
	return CMP(thunk, a, b) < 0 ?
	       (CMP(thunk, b, c) < 0 ? b : (CMP(thunk, a, c) < 0 ? c : a ))
              :(CMP(thunk, b, c) > 0 ? b : (CMP(thunk, a, c) < 0 ? a : c ));
}

#define	thunk NULL
void
qsort(void *a, size_t n, size_t es, cmp_t *cmp)
{
	char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
	int d, r, swaptype, swap_cnt;

loop:	SWAPINIT(a, es);
	swap_cnt = 0;
	if (n < 7) {
		for (pm = (char *)a + es; pm < (char *)a + n * es; pm += es)
			for (pl = pm; pl > (char *)a && CMP(thunk, pl - es, pl) > 0;
			     pl -= es)
				swap(pl, pl - es);
		return;
	}
	pm = (char *)a + (n / 2) * es;
	if (n > 7) {
		pl = a;
		pn = (char *)a + (n - 1) * es;
		if (n > 40) {
			d = (n / 8) * es;
			pl = med3(pl, pl + d, pl + 2 * d, cmp, thunk);
			pm = med3(pm - d, pm, pm + d, cmp, thunk);
			pn = med3(pn - 2 * d, pn - d, pn, cmp, thunk);
		}
		pm = med3(pl, pm, pn, cmp, thunk);
	}
	swap(a, pm);
	pa = pb = (char *)a + es;

	pc = pd = (char *)a + (n - 1) * es;
	for (;;) {
		while (pb <= pc && (r = CMP(thunk, pb, a)) <= 0) {
			if (r == 0) {
				swap_cnt = 1;
				swap(pa, pb);
				pa += es;
			}
			pb += es;
		}
		while (pb <= pc && (r = CMP(thunk, pc, a)) >= 0) {
			if (r == 0) {
				swap_cnt = 1;
				swap(pc, pd);
				pd -= es;
			}
			pc -= es;
		}
		if (pb > pc)
			break;
		swap(pb, pc);
		swap_cnt = 1;
		pb += es;
		pc -= es;
	}
	if (swap_cnt == 0) {  /* Switch to insertion sort */
		for (pm = (char *)a + es; pm < (char *)a + n * es; pm += es)
			for (pl = pm; pl > (char *)a && CMP(thunk, pl - es, pl) > 0;
			     pl -= es)
				swap(pl, pl - es);
		return;
	}

	pn = (char *)a + n * es;
	r = min(pa - (char *)a, pb - pa);
	vecswap(a, pb - r, r);
	r = min(pd - pc, pn - pd - es);
	vecswap(pb, pn - r, r);
	if ((r = pb - pa) > es)
		qsort(a, r / es, es, cmp);
	if ((r = pd - pc) > es) {
		/* Iterate rather than recurse to save stack space */
		a = pn - r;
		n = r / es;
		goto loop;
	}
}


/* ---------------------------------------------------------------------	
   Origin: $OpenBSD: _rand48.c,v 1.3 2005/08/08 08:05:36 espie Exp $ 
   ---------------------------------------------------------------------
 * Copyright (c) 1993 Martin Birgmeier
 * All rights reserved.
 *
 * You may redistribute unmodified or modified versions of this source
 * code provided that the above copyright notice and this and the
 * following conditions are retained.
 *
 * This software is provided ``as is'', and comes with no warranties
 * of any kind. I shall in no event be liable for anything that happens
 * to anyone/anything when using this software.
   ---------------------------------------------------------------------
*/


void __dorand48(unsigned short xseed[3])
{
	unsigned long accu;
	unsigned short temp[2];

	accu = (unsigned long) __rand48_mult[0] * (unsigned long) xseed[0] +
	 (unsigned long) __rand48_add;
	temp[0] = (unsigned short) accu;	/* lower 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += (unsigned long) __rand48_mult[0] * (unsigned long) xseed[1] +
	 (unsigned long) __rand48_mult[1] * (unsigned long) xseed[0];
	temp[1] = (unsigned short) accu;	/* middle 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += __rand48_mult[0] * xseed[2] + __rand48_mult[1] * xseed[1] + __rand48_mult[2] * xseed[0];
	xseed[0] = temp[0];
	xseed[1] = temp[1];
	xseed[2] = (unsigned short) accu;
}


long nrand48(unsigned short xseed[3])
{
	__dorand48(xseed);
	return ((long) xseed[2] << 15) + ((long) xseed[1] >> 1);
}

int sgn (int x)
{
    if (x<0)  return -1;
    if (x==0) return 0;
    if (x>0)  return +1;
}
