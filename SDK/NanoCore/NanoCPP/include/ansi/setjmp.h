/***************************************************************************
 *
 * setjmp.h
 *
 * $Id: setjmp.h 550991 2007-06-26 23:58:07Z sebor $
 *
 ***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  "License"); you may  not use  this file
 * except in  compliance with the License.   You may obtain  a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the  License is distributed on an  "AS IS" BASIS,
 * WITHOUT  WARRANTIES OR CONDITIONS  OF ANY  KIND, either  express or
 * implied.   See  the License  for  the  specific language  governing
 * permissions and limitations under the License.
 *
 * Copyright 1994-2006 Rogue Wave Software.
 * 
 **************************************************************************/

#ifndef _RWSTD_SETJMP_H_INCLUDED
#define _RWSTD_SETJMP_H_INCLUDED


#include <rw/_defs.h>


#ifndef _RWSTD_NO_DEPRECATED_C_HEADERS

#  include <csetjmp>

#  ifndef _RWSTD_NO_NAMESPACE

using std::jmp_buf;

using std::longjmp;

#  endif   // _RWSTD_NO_NAMESPACE
#else
#  include _RWSTD_ANSI_C_SETJMP_H
#endif   // _RWSTD_NO_DEPRECATED_C_HEADERS

#endif   // _RWSTD_SETJMP_H_INCLUDED
