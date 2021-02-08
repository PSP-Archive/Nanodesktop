/* Copyright (C) 1999-2001 Free Software Foundation, Inc.
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

#include <nanoc_3rdparty/psp/utf8/include/utf8/nd_wctype.h>

#include <nanoc_3rdparty/psp/utf8/include/utf8/nd_locale.h>
#include <ctype.h>
#include "noppage.h"
#include "toupper.h"

wint_t towupper (wint_t wc)
#undef towupper /* identifier used inside toupper macro on OSF/1 4.0 */
{
  if (!locale_is_utf8) {
    if ((u_wchar_t) wc < 0x100)
      return toupper((unsigned char) wc);
    else
      return wc;
  } else {
    if ((unsigned int) wc < 0x10800u)
      return (unsigned short) (wc + toupper_table[((unsigned int) wc) >> 8][((unsigned int) wc) & 0xff]);
    else
      return wc;
  }
}
