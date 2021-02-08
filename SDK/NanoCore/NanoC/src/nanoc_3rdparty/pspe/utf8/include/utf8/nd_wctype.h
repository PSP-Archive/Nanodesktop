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
    Questo file è una versione modificata di utf8/wctype.h. La versione è
    stata realizzata allo scopo di evitare problemi di compilazione al
    resto del sistema
*/



#ifndef _UTF8_WCTYPE_FOR_ND_H
#define _UTF8_WCTYPE_FOR_ND_H

/* Define wchar_t and wint_t. */
#include <nanoc_3rdparty/pspe/utf8/include/utf8/types.h>

/* Scalar type that can hold values which represent locale-specific
   character classifications. */
typedef unsigned long int wctype_t;


#ifdef __cplusplus
extern "C" {
#endif


/* Test for any wide character for which `iswalpha' or `iswdigit' is true. */
extern int iswalnum (wint_t wc);

/* Test for any wide character for which `iswupper' or 'iswlower' is true,
   or any wide character that is one of a locale-specific set of
   wide-characters for which none of `iswcntrl', `iswdigit', `iswpunct',
   or `iswspace' is true. */
extern int iswalpha (wint_t wc);

/* Test for any control wide character. */
extern int iswcntrl (wint_t wc);

/* Test for any wide character that corresponds to a decimal-digit
   character. */
extern int iswdigit (wint_t wc);

/* Test for any wide character for which `iswprint' is true and `iswspace'
   is false. */
extern int iswgraph (wint_t wc);

/* Test for any wide character that corresponds to a lowercase letter or is
   one of a locale-specific set of wide characters for which none of
   `iswcntrl', `iswdigit', `iswpunct', or `iswspace' is true. */
extern int iswlower (wint_t wc);

/* Test for any printing wide character. */
extern int iswprint (wint_t wc);

/* Test for any printing wide character that is one of a locale-specific set
   of wide characters for which neither `iswspace' nor `iswalnum' is true. */
extern int iswpunct (wint_t wc);

/* Test for any wide character that corresponds to a locale-specific set of
   wide characters for which none of `iswalnum', `iswgraph', or `iswpunct'
   is true. */
extern int iswspace (wint_t wc);

/* Test for any wide character that corresponds to an uppercase letter or is
   one of a locale-specific set of wide character for which none of
   `iswcntrl', `iswdigit', `iswpunct', or `iswspace' is true. */
extern int iswupper (wint_t wc);

/* Test for any wide character that corresponds to a hexadecimal-digit
   character equivalent to that performed be the functions described in the
   previous subclause. */
extern int iswxdigit (wint_t wc);

/* GNU extension. */
/* Test for any wide character that corresponds to a standard blank wide
   character or a locale-specific set of wide characters for which `iswalnum'
   is false. */
extern int iswblank (wint_t wc);


/* Construct value that describes a class of wide characters identified
   by the string argument PROPERTY. */
extern wctype_t wctype (const char* property);

/* Determine whether the wide-character WC has the property described by
   DESC. */
extern int iswctype (wint_t wc, wctype_t desc);


/* Scalar type that can hold values which represent locale-specific character
   mappings. */
typedef const short * const * wctrans_t;

/* Converts an uppercase letter to the corresponding lowercase letter. */
extern wint_t towlower (wint_t wc);

/* Converts a lowercase letter to the corresponding uppercase letter.  */
extern wint_t towupper (wint_t wc);


/* Scalar type that can hold values which represent locale-specific character
   mappings. */
typedef const short * const * utf8_wctrans_t;
#define wctrans_t utf8_wctrans_t


/* Construct value that describes a mapping between wide characters
   identified by the string argument PROPERTY. */
extern wctrans_t wctrans (const char* property);

/* Map the wide character WC using the mapping described by DESC. */
extern wint_t towctrans (wint_t wc, wctrans_t desc);





#ifdef __cplusplus
}
#endif

#endif /* _UTF8_WCTYPE_FOR_ND_H */
