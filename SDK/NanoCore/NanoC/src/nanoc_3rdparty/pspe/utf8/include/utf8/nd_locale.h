/* Copyright (C) 1999 Free Software Foundation, Inc.
   This file is part of the GNU UTF-8 Library.

   The GNU UTF-8 Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU UTF-8 Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU UTF-8 Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */
   
/*
    Questo file è una versione modificata di utf8/stdio.h. La versione è
    stata realizzata allo scopo di evitare problemi di compilazione al
    resto del sistema
*/


#ifndef _UTF8_LOCALE_FOR_ND_H
#define _UTF8_LOCALE_FOR_ND_H

#define LC_ALL	    0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5
#define LC_MESSAGES 6
               
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



#ifdef __cplusplus
extern "C" {
#endif

/* Set and/or return the current locale. */
extern char* setlocale (int category, const char* locale);

/* Nonzero if the current locale is UTF-8. */
extern int locale_is_utf8;

/* Returns address of system lconv struct */
extern struct lconv *localeconv (void);

#ifdef __cplusplus
}
#endif

#endif /* _UTF8_LOCALE_FOR_ND_H */
