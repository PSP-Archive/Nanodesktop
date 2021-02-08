/***************************************************************************
 *
 * ti_wfilebuf.cpp - explicit instantiation directives
 *                   for basic_filebuf<wchar_t>
 *
 * $Id: ti_wfilebuf.cpp 386729 2006-03-17 21:42:20Z sebor $
 *
 ***************************************************************************
 *
 * Copyright 2005-2006 The Apache Software Foundation or its licensors,
 * as applicable.
 *
 * Copyright 1994-2006 Rogue Wave Software.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 **************************************************************************/

#define _RWSTD_LIB_SRC

#define _RWSTD_INSTANTIATE_WCHAR_T         1
#define _RWSTD_INSTANTIATE_TEMPLATES       1
#define _RWSTD_INSTANTIATE_BASIC_FILEBUF   1

#include <rw/_defs.h>

#undef _RWSTD_NO_TEMPLATE_DEFINITIONS

#ifdef _MSC_VER
   // shut up the bogus MSVC warning C4661: no suitable definition
   // provided for explicit template instantiation request
#  pragma warning (disable: 4661)
#endif   // _MSC_VER

// include <cstdio> to define the extended member functions that
// take a FILE* argument
#include <cstdio>
#include <fstream>
