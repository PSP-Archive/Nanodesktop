/***************************************************************************
 *
 * instance.cpp - Instantiations of the C++ Standard Library templates.
 *
 * $Id: instance.cpp 405913 2006-05-12 22:40:27Z sebor $
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

#define _RWSTD_INSTANTIATE_TEMPLATES      1
#define _RWSTD_INSTANTIATE_STATIC_MUTEX   1
#define _RWSTD_INSTANTIATE_INT            1

#include <string>
#include <rw/_mutex.h>

#ifdef _RWSTD_NO_COLLAPSE_TEMPLATE_STATICS

_RWSTD_NAMESPACE (__rw) {

#ifndef _RWSTD_NO_WCHAR_T

typedef _STD::char_traits<wchar_t> Traits;
typedef _STD::allocator<wchar_t>   Allocator;

typedef __string_ref<wchar_t, Traits, Allocator> StringRef;

#else   // if defined (_RWSTD_NO_WCHAR_T)

typedef _STD::char_traits<char> Traits;
typedef _STD::allocator<char>   Allocator;

typedef __string_ref<char, Traits, Allocator> StringRef;

#endif   // _RWSTD_NO_WCHAR_T

// FIXME: this won't work at all for other (non-char) specializations
// try to allocate enough space for std::wstring plus some overhead
_RWSTD_EXPORT unsigned long
__nullref [4 * sizeof (StringRef) / sizeof (long) + 2];

}   // namespace __rw

#endif   // _RWSTD_NO_COLLAPSE_TEMPLATE_STATICS
