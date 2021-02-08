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


#ifndef _UTF8_STDIO_FOR_ND_H
#define _UTF8_STDIO_FOR_ND_H


#include <nanoc_3rdparty/pspe/utf8/include/utf8/types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Write formatted output to STREAM. */
extern int utf8_fprintf (FILE* stream, const char* format, ...);

/* Write formatted output to stdout. */
extern int utf8_printf (const char* format, ...);

/* Write formatted output to string S. */
extern int utf8_sprintf (char* s, const char* format, ...);

/* Write formatted output to string S, at most MAXLEN bytes. */
extern int utf8_snprintf (char* s, size_t maxlen, const char* format, ...);

/* Write formatted output to STREAM. */
extern int utf8_vfprintf (FILE* stream, const char* format, va_list args);

/* Write formatted output to stdout. */
extern int utf8_vprintf (const char* format, va_list args);

/* Write formatted output to string S. */
extern int utf8_vsprintf (char* s, const char* format, va_list args);

/* Write formatted output to string S, at most MAXLEN bytes. */
extern int utf8_vsnprintf (char* s, size_t maxlen, const char* format, va_list args);


/* Write formatted output to STREAM. */
extern int fwprintf (FILE* stream, const wchar_t* format, ...);

/* Write formatted output to stdout. */
extern int wprintf (const wchar_t* format, ...);

/* Write formatted output to string S, at most MAXLEN bytes. */
extern int swprintf (wchar_t* s, size_t maxlen, const wchar_t* format, ...);

/* Write formatted output to STREAM. */
extern int vfwprintf (FILE* stream, const wchar_t* format, va_list args);

/* Write formatted output to stdout. */
extern int vwprintf (const wchar_t* format, va_list args);

/* Write formatted output to string S, at most MAXLEN bytes. */
extern int vswprintf (wchar_t* s, size_t maxlen, const wchar_t* format, va_list args);


#ifdef __cplusplus
}
#endif

#endif /* _UTF8_STDIO_FOR_ND_H */
