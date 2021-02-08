/*
   --------------------------------------------------------------------------
   nanoc_ctype.h               NanoC File Headers
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically
   --------------------------------------------------------------------------
*/


/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * ctype.h
 *
 * Copyright (c) 2002-2004 PS2DEV
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: ctype.h 1095 2005-09-27 21:02:16Z jim $
 */

#ifndef _NANOC_CTYPE_H
#define _NANOC_CTYPE_H

                
                #ifdef __cplusplus
                extern "C" {
                #endif
                 
                int isalnum(int);
                int isalpha(int);
                int iscntrl(int);
                int isdigit(int);
                int isgraph(int);
                int islower(int);
                int isprint(int);
                int ispunct(int);
                int isspace(int);
                int isupper(int);
                int isxdigit(int);
                int tolower(int);
                int toupper(int);
                int isascii(int);
                
                #ifdef __cplusplus
                }
                #endif
                
                /* To be compatible with C++'s ctype_base.h */
                
                #ifdef __cplusplus
                
                
                    namespace std {
                        enum {
                    	_U =	01,
                    	_L =	02,
                    	_N =	04,
                    	_S =	010,
                    	_P =	020,
                    	_C =	040,
                    	_X =	0100,
                    	_B =	0200
                        };
                    };
                    
                #endif
                
                #ifdef ENABLE_NANOC_SYMBOLIC_EXTENSION
                    
                    // E' previsto che sia disattivabile per evitare problemi nella compilazione 
                    // di cvcalibration.cpp
                    
                    #define	_U	01
                    #define	_L	02
                    #define	_N	04
                    #define	_S	010
                    #define _P	020
                    #define _C	040
                    #define _X	0100
                    #define	_B	0200

                #endif                

#endif   // ifdef _NANOC_CTYPE_H
