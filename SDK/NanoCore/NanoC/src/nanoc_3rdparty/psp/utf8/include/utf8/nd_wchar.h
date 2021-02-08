/* Copyright (C) 1999-2000 Free Software Foundation, Inc.
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
    Questo file è una versione modificata di utf8/wchar.h. La versione è
    stata realizzata allo scopo di evitare problemi di compilazione al
    resto del sistema
*/

#ifndef _UTF8_WCHAR_FOR_ND_H
#define _UTF8_WCHAR_FOR_ND_H

/* Get FILE definition. */
#include <stdio.h>

/* Define wchar_t and wint_t. */
#include <nanoc_3rdparty/pspe/utf8/include/utf8/types.h>

/* Get errno declaration and values. */
#include <errno.h>
/* Some systems, like SunOS 4, don't have EILSEQ. On these systems, define
   EILSEQ ourselves, but don't define it as EINVAL, because iconv() callers
   want to distinguish EINVAL and EILSEQ. */
#ifndef EILSEQ
#define EILSEQ ENOENT
#endif

/* Conversion state information. */
/*
   In ISO C 89 + Amendment 1, a state was *not* a partially accumulated
   UTF-8 character, or empty, because the Dinkumware C library manual for
   mbsrtowcs and wcsrtombs says that these functions proceed by treating
   entire wide characters one at a time:
   "The function effectively increments dst by x and *src by one after each
    call to wcrtomb that stores a complete converted multibyte character in
    the remaining space available."
   And neither 8-bit locales nor UTF-8 have "shift state", i.e. state between
   complete converted characters.

   But in ISO C 99, this has changed:
   7.24.6 paragraph 4 says:
   "On entry, each function takes the described conversion state (either
    internal or pointed to by an argument) as current. The conversion state
    described by the pointed-to object is altered as needed to track the shift
    state, and the position within a multibyte character, for the associated
    multibyte character sequence."
   7.24.6.3.2, the description of mbrtowc's return value, says:
   "(size_t)(-2)  if the next n bytes contribute to an incomplete (but
    potentially valid) multibyte character, and all n bytes have been
    processed (no value is stored)."
   Note the words "contribute to" (not "start"!) and "processed" (not
   "examined"!).
 */

/* This struct must not be larger than the platform's native mbstate_t. */
typedef struct
{
#if WCHAR_T_BITS == 32
  unsigned int count;        /* number of bytes remaining to be processed */
  unsigned int value;        /* if count > 0: partial wide character */
#else
  unsigned int count : 16;   /* number of bytes remaining to be processed */
  unsigned int value : 16;   /* if count > 0: partial wide character */
#endif
/* If WCHAR_T_BITS == 32, need 3 bits for count,
   30 bits for value (25 for mbstowcs direction, 30 for wcstombs direction).
   If WCHAR_T_BITS == 16, need 2 bits for count,
   12 bits for value (10 for mbstowcs direction, 12 for wcstombs direction). */
} mbstate_t;

#define utf8_mbstate_t mbstate_t



#ifdef __cplusplus
extern "C" {
#endif


/* Copy SRC to DEST. */
extern wchar_t* wcscpy (wchar_t* dest, const wchar_t* src);

/* Copy no more than N wide-characters of SRC to DEST. */
extern wchar_t* wcsncpy (wchar_t* dest, const wchar_t* src, size_t n);

/* Append SRC onto DEST. */
extern wchar_t* wcscat (wchar_t* dest, const wchar_t* src);

/* Append no more than N wide-characters of SRC onto DEST. */
extern wchar_t* wcsncat (wchar_t* dest, const wchar_t* src, size_t n);

/* Compare S1 and S2. */
extern int wcscmp (const wchar_t* s1, const wchar_t* s2);

/* Compare no more than N wide-characters of S1 and S2. */
extern int wcsncmp (const wchar_t* s1, const wchar_t* s2, size_t n);

/* GNU extension. */
/* Compare S1 and S2, ignoring case. */
extern int wcscasecmp (const wchar_t* s1, const wchar_t* s2);

/* GNU extension. */
/* Compare no more than N chars of S1 and S2, ignoring case. */
extern int wcsncasecmp (const wchar_t* s1, const wchar_t* s2, size_t n);


/* Compare S1 and S2, both interpreted as appropriate to the LC_COLLATE
   category of the current locale. */
extern int wcscoll (const wchar_t* s1, const wchar_t* s2);

/* Transform S2 into array pointed to by S1 such that if wcscmp is applied
   to two transformed strings the result is the as applying `wcscoll' to the
   original strings. */
extern size_t wcsxfrm (wchar_t* s1, const wchar_t* s2, size_t n);


/* GNU extension. */
/* Duplicate S, returning an identical malloc'd string. */
extern wchar_t* wcsdup (const wchar_t* s);

/* Find the first occurrence of WC in WCS. */
extern wchar_t* wcschr (const wchar_t* wcs, wchar_t wc);

/* Find the last occurrence of WC in WCS. */
extern wchar_t* wcsrchr (const wchar_t* wcs, wchar_t wc);

/* Return the length of the initial segmet of WCS which consists entirely
   of wide characters not in REJECT. */
extern size_t wcscspn (const wchar_t* wcs, const wchar_t* reject);

/* Return the length of the initial segmet of WCS which consists entirely
   of wide characters in ACCEPT. */
extern size_t wcsspn (const wchar_t* wcs, const wchar_t* accept);

/* Find the first occurrence in WCS of any character in ACCEPT. */
extern wchar_t* wcspbrk (const wchar_t* wcs, const wchar_t* accept);

/* Find the first occurrence of NEEDLE in HAYSTACK. */
extern wchar_t* wcsstr (const wchar_t* haystack, const wchar_t* needle);

/* Divide WCS into tokens separated by characters in DELIM. */
extern wchar_t* wcstok (wchar_t* wcs, const wchar_t* delim, wchar_t** ptr);

/* Return the number of wide characters in S. */
extern size_t wcslen (const wchar_t* s);

/* GNU extension. */
/* Return the number of wide characters in S, but at most MAXLEN. */
extern size_t wcsnlen (const wchar_t* s, size_t maxlen);


/* Search N wide characters of S for C. */
extern wchar_t* wmemchr (const wchar_t* s, wchar_t c, size_t n);

/* Compare N wide characters of S1 and S2. */
extern int wmemcmp (const wchar_t* s1, const wchar_t* s2, size_t n);

/* Copy N wide characters of SRC to DEST.  */
extern wchar_t* wmemcpy (wchar_t* dest, const wchar_t* src, size_t n);

/* Copy N wide characters of SRC to DEST, guaranteeing correct behavior for
   overlapping memory areas.  */
extern wchar_t* wmemmove (wchar_t* dest, const wchar_t* src, size_t n);

/* Set N wide characters of S to C. */
extern wchar_t* wmemset (wchar_t* s, wchar_t c, size_t n);


/* Determine whether C constitutes a valid (one-byte) multibyte character. */
extern wint_t btowc (int c);

/* Determine whether C corresponds to a member of the extended character set
   whose multibyte representation is a single byte. */
extern int wctob (wint_t c);

/* Determine whether PS points to an object representing the initial state. */
extern int mbsinit (const mbstate_t* ps);

/* Write wide character representation of multibyte character pointed to by S
   to PWC. */
extern size_t mbrtowc (wchar_t* pwc, const char* s, size_t n, mbstate_t* ps);

/* Write multibyte representation of wide character WC to S. */
extern size_t wcrtomb (char* s, wchar_t wc, mbstate_t* ps);

/* Return number of bytes in multibyte character pointed to by S. */
extern size_t mbrlen (const char* s, size_t n, mbstate_t* ps);

/* Write wide character representation of multibyte character string SRC
   to DEST. */
extern size_t mbsrtowcs (wchar_t* dest, const char** src, size_t len, mbstate_t* ps);

/* Write multibyte character representation of wide character string SRC
   to DEST. */
extern size_t wcsrtombs (char* dest, const wchar_t** src, size_t len, mbstate_t* ps);

/* GNU extension. */
/* Write wide character representation of at most NMC bytes of the multibyte
   character string SRC to DEST. */
extern size_t mbsnrtowcs (wchar_t* dest, const char** src, size_t nms, size_t len, mbstate_t* ps);

/* GNU extension. */
/* Write multibyte character representation of at most NWC characters from
   the wide character string SRC to DEST. */
extern size_t wcsnrtombs (char* dest, const wchar_t** src, size_t nwc, size_t len, mbstate_t* ps);

/* X/Open extension. */
/* Determine number of column positions required for C. */
extern int wcwidth (const wchar_t c);

/* X/Open extension. */
/* Determine number of column positions required for first N wide characters
   (or fewer if S ends before this) in S. */
extern int wcswidth (const wchar_t* s, size_t n);

/* Convert initial portion of the wide string NPTR to `float'
   representation. */
extern float wcstof (const wchar_t* nptr, wchar_t** endptr);

/* Convert initial portion of the wide string NPTR to `double'
   representation. */
extern double wcstod (const wchar_t* nptr, wchar_t** endptr);

#if HAVE_LONG_DOUBLE
/* Convert initial portion of the wide string NPTR to `long double'
   representation. */
extern long double wcstold (const wchar_t* nptr, wchar_t** endptr);
#endif

/* Convert initial portion of wide string NPTR to `long int'
   representation. */
extern long int wcstol (const wchar_t* nptr, wchar_t** endptr, int base);

/* Convert initial portion of wide string NPTR to `unsigned long int'
   representation. */
extern unsigned long int wcstoul (const wchar_t* nptr, wchar_t** endptr, int base);

#if HAVE_LONG_LONG
/* Convert initial portion of wide string NPTR to `long long int'
   representation. */
extern long long int wcstoll (const wchar_t* nptr, wchar_t** endptr, int base);
#endif

#if HAVE_LONG_LONG
/* Convert initial portion of wide string NPTR to `unsigned long long int'
   representation. */
extern unsigned long long int wcstoull (const wchar_t* nptr, wchar_t** endptr, int base);
#endif

/* GNU extension. */
/* Copy SRC to DEST, returning the address of the terminating L'\0' in DEST. */
extern wchar_t* wcpcpy (wchar_t* dest, const wchar_t* src);

/* GNU extension. */
/* Copy no more than N characters of SRC to DEST, returning the address of
   the last character written into DEST. */
extern wchar_t* wcpncpy (wchar_t* dest, const wchar_t* src, size_t n);


/* Reads the next wide character from STREAM, and returns it. Returns WEOF
   when end-of-file is reached or an error is encountered. */
extern wint_t fgetwc (FILE* stream);
#define getwc fgetwc

/* Reads the next wide character from stdin, and returns it. Returns WEOF
   when end-of-file is reached or an error is encountered. */
extern wint_t getwchar (void);

/* Reads a line of wide characters from STREAM. WS points to a buffer for
   N wide characters. At most N-1 wide characters will be read, stopping
   after the first seen newline character. A null wide character will be
   appended. Then WS is returned. If no wide character could be read from
   STREAM, or if an error is encountered, NULL is returned.
   Note: This function is unreliable, because it does not permit to deal
   properly with null wide characters that may be present in the input. */
extern wchar_t* fgetws (wchar_t* ws, int n, FILE* stream);

/* Pushes back a wide character onto STREAM. Returns WC if successful, or
   WEOF if WC is WEOF or when an error is encountered. */
extern wint_t ungetwc (wint_t wc, FILE* stream);


/* Writes WC to STREAM. Returns WC if successful, or WEOF when an error is
   encountered. */
extern wint_t fputwc (wchar_t wc, FILE* stream);
#define putwc fputwc

/* Writes WC to stdout. Returns WC if successful, or WEOF when an error is
   encountered. */
extern wint_t putwchar (wchar_t wc);

/* Writes the contents of the wide string WS, up to but not including the
   terminating null wide character, to STREAM. Returns a nonnegative integer
   if successful, or -1 in case of error. */
extern int fputws (const wchar_t* ws, FILE* stream);


/* If MODE != 0, attempts to change the orientation of STREAM. Then returns
   the orientation of STREAM. */
extern int fwide (FILE* stream, int mode);


#ifdef __cplusplus
}
#endif

#endif /* _UTF8_WCHAR_FOR_ND_H */
