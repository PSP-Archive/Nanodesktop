/**************************************************************************
 *
 * version.cpp - library versioning information
 *
 * $Id: version.cpp 550991 2007-06-26 23:58:07Z sebor $
 *
 ***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  License); you may  not use  this file
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

#define _RWSTD_LIB_SRC
#include <rw/_defs.h>


_RWSTD_NAMESPACE (__rw) {

#ifdef _RWSTD_VER_STR

extern const char __rw_ident[] = {
    "@(#) Rogue Wave C++ Standard Library version " _RWSTD_VER_STR
};

#endif   // _RWSTD_VER_STR

}   // namespace __rw


#ifdef __HP_aCC
#  pragma COPYRIGHT      "Rogue Wave Software"
#  pragma COPYRIGHT_DATE "1994-2007"
#  pragma VERSIONID      _RWSTD_VER_STR
#elif defined (__IBMCPP__)
#  pragma comment (copyright, "(C) 1994-2007, Rogue Wave Software")
#endif   // __HP_aCC
