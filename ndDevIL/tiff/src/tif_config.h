/* libtiff/tif_config.h.in.  Generated from configure.ac by autoheader.  */

#ifndef _TIFCONFIG_H
  #define _TIFCONFIG_H

/* Support CCITT Group 3 & 4 algorithms */
#undef CCITT_SUPPORT

/* Pick up YCbCr subsampling info from the JPEG data stream to support files
   lacking the tag (default enabled). */
#undef CHECK_JPEG_YCBCR_SUBSAMPLING

/* Support C++ stream API (requires C++ compiler) */
#undef CXX_SUPPORT

/* Treat extra sample as alpha (default enabled). The RGBA interface will
   treat a fourth sample with no EXTRASAMPLE_ value as being ASSOCALPHA. Many
   packages produce RGBA files but don't mark the alpha properly. */
#undef DEFAULT_EXTRASAMPLE_AS_ALPHA

/* Use the Apple OpenGL framework. */
#undef HAVE_APPLE_OPENGL_FRAMEWORK

/* Define to 1 if you have the <assert.h> header file. */
#undef HAVE_ASSERT_H

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you have the <fcntl.h> header file. */
#undef HAVE_FCNTL_H

/* Define to 1 if you have the `floor' function. */
#undef HAVE_FLOOR

/* Define to 1 if you have the `getopt' function. */
#undef HAVE_GETOPT

/* Define as 0 or 1 according to the floating point format suported by the
   machine */
#undef HAVE_IEEEFP

/* Define to 1 if the system has the type `tiff_tiff_int16'. */
#undef HAVE_tiff_tiff_int16

/* Define to 1 if the system has the type `tiff_tiff_int32'. */
#undef HAVE_tiff_tiff_int32

/* Define to 1 if the system has the type `int8'. */
#undef HAVE_INT8

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the `isascii' function. */
#undef HAVE_ISASCII

/* Define to 1 if you have the `lfind' function. */
#undef HAVE_LFIND

/* Define to 1 if you have the `c' library (-lc). */
#define HAVE_LIBC

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM

/* Define to 1 if you have the <limits.h> header file. */
#undef HAVE_LIMITS_H

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE

/* Define to 1 if you have the <memory.h> header file. */
#undef HAVE_MEMORY_H

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET

/* Define to 1 if you have the `mmap' function. */
#undef HAVE_MMAP

/* Define to 1 if you have the `pow' function. */
#undef HAVE_POW

/* Define if you have POSIX threads libraries and header files. */
#undef HAVE_PTHREAD

/* Define to 1 if you have the <search.h> header file. */
#undef HAVE_SEARCH_H

/* Define to 1 if you have the `sqrt' function. */
#define HAVE_SQRT

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H

/* Define to 1 if you have the `strrchr' function. */
#undef HAVE_STRRCHR

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL

/* Define to 1 if you have the `strtoul' function. */
#define HAVE_STRTOUL

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/time.h> header file. */
#undef HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H

/* Define to 1 if you have the <windows.h> header file. */
#undef HAVE_WINDOWS_H

/* Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
   (Intel) */
#undef HOST_BIGENDIAN 

/* Set the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB) */
#undef HOST_FILLORDER

/* Support JPEG compression (requires IJG JPEG library) */
#undef JPEG_SUPPORT

/* Support LogLuv high dynamic range encoding */
#undef LOGLUV_SUPPORT

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#undef LT_OBJDIR

/* Support LZW algorithm */
#undef LZW_SUPPORT

/* Support Microsoft Document Imaging format */
#undef MDI_SUPPORT

/* Support NeXT 2-bit RLE algorithm */
#undef NEXT_SUPPORT

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
#undef NO_MINUS_C_MINUS_O

/* Support Old JPEG compresson (read contrib/ojpeg/README first! Compilation
   fails with unpatched IJG JPEG library) */
#undef OJPEG_SUPPORT

/* Name of package */
#undef PACKAGE

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* Support Macintosh PackBits algorithm */
#undef PACKBITS_SUPPORT

/* Support Pixar log-format algorithm (requires Zlib) */
#undef PIXARLOG_SUPPORT

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
#undef PTHREAD_CREATE_JOINABLE

/* The size of a `int', as computed by sizeof. */
#undef SIZEOF_INT

/* The size of a `long', as computed by sizeof. */
#undef SIZEOF_LONG

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

/* Support strip chopping (whether or not to convert single-strip uncompressed
   images to mutiple strips of specified size to reduce memory usage) */
#undef STRIPCHOP_DEFAULT

/* Default size of the strip in bytes (when strip chopping enabled) */
#undef STRIP_SIZE_DEFAULT

/* Enable SubIFD tag (330) support */
#undef SUBIFD_SUPPORT

/* Support ThunderScan 4-bit RLE algorithm */
#undef THUNDER_SUPPORT

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#undef TIME_WITH_SYS_TIME

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
#undef TM_IN_SYS_TIME

/* Version number of package */
#undef VERSION

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
//#undef WORDS_BIGENDIAN  
#undef WORDS_BIGENDIAN 

/* Define to 1 if the X Window System is missing or not being used. */
//#define X_DISPLAY_MISSING
#define X_DISPLAY_MISSING 1

/* Support Deflate compression */
#undef ZIP_SUPPORT

/* Number of bits in a file offset, on hosts where this is settable. */
#undef _FILE_OFFSET_BITS

/* Define for large files, on AIX-style hosts. */
#undef _LARGE_FILES

/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif

/* Define to `long' if <sys/types.h> does not define. */
#undef off_t

/* Define to `unsigned' if <sys/types.h> does not define. */
#undef size_t

#endif    // ifdef _TIFCONFIG_H
