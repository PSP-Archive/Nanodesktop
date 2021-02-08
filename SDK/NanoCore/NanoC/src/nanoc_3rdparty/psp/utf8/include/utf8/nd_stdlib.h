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

#ifndef _UTF8_NDSTDLIB_H
#define _UTF8_NDSTDLIB_H



#include <nanoc_3rdparty/psp/utf8/include/utf8/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum length of a multibyte character in the current locale.  */
#if defined(LIBUTF8_PLUG) && defined(__GLIBC__)
#define utf8_mb_cur_max __ctype_get_mb_cur_max
/* glibc defines:  #define MB_CUR_MAX (__ctype_get_mb_cur_max ())  */
#endif
#ifndef LIBUTF8_PLUG
#undef MB_CUR_MAX
#define MB_CUR_MAX (utf8_mb_cur_max())
#endif
#if defined(LIBUTF8_PLUG) && defined(__GLIBC__) && (__GLIBC__ == 2) && (__GLIBC_MINOR__ == 0) /* glibc-2.0.x */
extern int utf8_mb_cur_max (void);
#else
extern size_t utf8_mb_cur_max (void);
#endif

/* Return the length of the multibyte character in S, which is no longer
   than N. */
extern int mblen (const char* s, size_t n);

/* Return the length of the given multibyte character, putting its `wchar_t'
   representation in *PWC. */
extern int mbtowc (wchar_t* pwc, const char* s, size_t n);

/* Put the multibyte character represented by WCHAR in S, returning its
   length. */
extern int wctomb (char* s, wchar_t wc);

/* Convert a multibyte string to a wide char string. */
extern size_t mbstowcs (wchar_t* pwcs, const char* s, size_t n);

/* Convert a wide char string to multibyte string. */
extern size_t wcstombs (char* s, const wchar_t* pwcs, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _UTF8_NDSTDLIB_H */
