/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ALLAFFIXES = 258,
     ALTSTRINGCHAR = 259,
     ALTSTRINGTYPE = 260,
     BOUNDARYCHARS = 261,
     COMPOUNDMIN = 262,
     COMPOUNDWORDS = 263,
     CONTROLLED = 264,
     DEFSTRINGTYPE = 265,
     FLAG = 266,
     FLAGMARKER = 267,
     NROFFCHARS = 268,
     OFF = 269,
     ON = 270,
     PREFIXES = 271,
     RANGE = 272,
     SUFFIXES = 273,
     STRING = 274,
     STRINGCHAR = 275,
     TEXCHARS = 276,
     WORDCHARS = 277
   };
#endif
/* Tokens.  */
#define ALLAFFIXES 258
#define ALTSTRINGCHAR 259
#define ALTSTRINGTYPE 260
#define BOUNDARYCHARS 261
#define COMPOUNDMIN 262
#define COMPOUNDWORDS 263
#define CONTROLLED 264
#define DEFSTRINGTYPE 265
#define FLAG 266
#define FLAGMARKER 267
#define NROFFCHARS 268
#define OFF 269
#define ON 270
#define PREFIXES 271
#define RANGE 272
#define SUFFIXES 273
#define STRING 274
#define STRINGCHAR 275
#define TEXCHARS 276
#define WORDCHARS 277




/* Copy the first part of user declarations.  */
#line 1 "parse.y"

#ifndef lint
static char Rcs_Id[] =
    "$Id: parse.y,v 1.52 1994/11/21 07:03:03 geoff Exp $";
#endif

/*
 * Copyright 1992, 1993, Geoff Kuenning, Granada Hills, CA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All modifications to the source code must be clearly marked as
 *    such.  Binary redistributions based on modified source code
 *    must be clearly marked as modified versions in the documentation
 *    and/or other materials provided with the distribution.
 * 4. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgment:
 *      This product includes software developed by Geoff Kuenning and
 *      other unpaid contributors.
 * 5. The name of Geoff Kuenning may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEOFF KUENNING AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL GEOFF KUENNING OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * $Log: parse.y,v $
 * Revision 1.52  1994/11/21  07:03:03  geoff
 * Get rid of the last vestiges of the "+" flag option.
 *
 * Revision 1.51  1994/11/02  06:56:13  geoff
 * Remove the anyword feature, which I've decided is a bad idea.
 *
 * Revision 1.50  1994/10/25  05:46:22  geoff
 * Add support for the "+" (any word) flag modifier.
 *
 * Revision 1.49  1994/09/16  04:48:30  geoff
 * Allow more than 128 string characters, by using different types in the
 * appropriate places.
 *
 * Revision 1.48  1994/05/24  05:31:25  geoff
 * Remember to convert the flag bit from a character to a bit number
 * before putting it in the hash file.
 *
 * Revision 1.47  1994/05/24  04:54:33  geoff
 * Improve the error checking for affix flag names, detecting bad flags
 * and duplicates.
 *
 * Revision 1.46  1994/05/22  01:38:02  geoff
 * Don't force flags to uppercase if lowercase flags are legal
 *
 * Revision 1.45  1994/05/17  06:44:17  geoff
 * Add support for controlled compound formation and the COMPOUNDONLY
 * option to affix flags.
 *
 * Revision 1.44  1994/02/07  05:51:03  geoff
 * Fix a place where atoi got the wrong argument type (lint error only)
 *
 * Revision 1.43  1994/01/25  07:12:01  geoff
 * Get rid of all old RCS log lines in preparation for the 3.1 release.
 *
 */

#include <ctype.h>
#include "config.h"
#include "ispell.h"
#include "proto.h"
#include "msgs.h"



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 92 "parse.y"
{
    int			simple;		/* Simple char or lval from yylex */
    struct
	{
	char *		set;		/* Character set */
	int		complement;	/* NZ if it is a complement set */
	}
			charset;
    unsigned char *	string;		/* String */
    ichar_t *		istr;		/* Internal string */
    struct flagent *	entry;		/* Flag entry */
    }
/* Line 187 of yacc.c.  */
#line 243 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 105 "parse.y"


static int	yylex P ((void)); /* Trivial lexical analyzer */
static int	kwanalyze P ((int backslashed, unsigned char * str));
				/* Analyze a possible keyword */
static void	getqstring P ((void));
				/* Get (double-)quoted string */
static void	getrange P ((void)); /* Get a lexical character range */
static int	backch P ((void)); /* Process a backslashed character */
static void	yyerror P ((char * msg));
				/* Print out an error message */
int		yyopen P ((char * file));
				/* Open a table file */
void		yyinit P ((void)); /* Initialize for parsing */
static int	grabchar P ((void));
				/* Get a character and track line number */
static void	ungrabchar P ((int ch));
				/* Unget a character, tracking line numbers */
static int	sufcmp P ((struct flagent * flag1, struct flagent * flag2));
				/* Compare suffix flags for qsort */
static int	precmp P ((struct flagent * flag1, struct flagent * flag2));
				/* Compare prefix flags for qsort */
static int	addstringchar P ((unsigned char * str, int lower, int upper));
				/* Add a string character to the table */
static int	stringcharcmp P ((char * a, char * b));
				/* Strcmp() done right, for Sun 4's */
#ifdef TBLDEBUG
static void	tbldump P ((struct flagent * flagp, int numflags));
				/* Dump a flag table */
static void	entdump P ((struct flagent * flagp));
				/* Dump one flag entry */
static void	setdump P ((char * setp, int mask));
				/* Dump a set specification */
static void	subsetdump P ((char * setp, int mask, int dumpval));
				/* Dump part of a set spec */
#endif

struct kwtab
    {
    char *	kw;		/* Syntactic keyword */
    int		val;		/* Lexical value */
    };

#define TBLINC		10		/* Size to allocate table by */

static FILE *		aff_file = NULL; /* Input file pointer */
static int		centnum;	/* Number of entries in curents */
static int		centsize = 0;	/* Size of alloc'ed space in curents */
static int		ctypechars;	/* Size of string in current strtype */
static int		ctypenum = 0;	/* Number of entries in chartypes */
static int		ctypesize = 0;	/* Size of alloc'ed spc in chartypes */
static struct flagent * curents;	/* Current flag entry collection */
static char *		fname = "(stdin)"; /* Current file name */
static char		lexungrab[MAXSTRINGCHARLEN * 2]; /* Spc for ungrabch */
static int		lineno = 1;	/* Current line number in file */
static struct flagent * table;		/* Current table being built */
static int		tblnum;		/* Numer of entries in table */
static int		tblsize = 0;	/* Size of the flag table */
static int		ungrablen;	/* Size of ungrab area */


/* Line 216 of yacc.c.  */
#line 316 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  45
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   115

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  30
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNRULES -- Number of states.  */
#define YYNSTATES  102

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     8,     2,     5,     3,     7,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,     2,
       2,     2,     4,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     9,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     8,    11,    15,    17,    20,    22,
      25,    28,    30,    33,    36,    38,    41,    45,    48,    52,
      55,    58,    62,    65,    69,    73,    75,    78,    80,    82,
      85,    89,    92,    95,    98,   101,   105,   108,   111,   113,
     115,   117,   119,   121,   124,   127,   129,   131,   134,   137,
     139,   142,   147,   153,   155,   157,   160,   162,   164,   166,
     169,   173,   180,   187,   193,   194,   196,   198,   201
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      31,     0,    -1,    32,    47,    -1,    47,    -1,    33,    34,
      -1,    33,    34,    36,    -1,    34,    -1,    34,    36,    -1,
      44,    -1,    33,    44,    -1,    35,    37,    -1,    37,    -1,
      34,    37,    -1,    17,    39,    -1,    38,    -1,    36,    38,
      -1,    29,    45,    45,    -1,    29,    45,    -1,    13,    45,
      45,    -1,    13,    45,    -1,    27,    26,    -1,    27,    26,
      26,    -1,    12,    39,    -1,    12,    39,    42,    -1,    26,
      26,    40,    -1,    41,    -1,    40,    41,    -1,    26,    -1,
      43,    -1,    42,    43,    -1,    11,    26,    26,    -1,    20,
      26,    -1,    28,    26,    -1,    14,    26,    -1,    15,    46,
      -1,    15,    16,    26,    -1,    10,    46,    -1,    19,    26,
      -1,     7,    -1,    26,    -1,    24,    -1,    22,    -1,    21,
      -1,    48,    49,    -1,    49,    48,    -1,    48,    -1,    49,
      -1,    23,    50,    -1,    25,    50,    -1,    51,    -1,    50,
      51,    -1,    18,    26,     6,    54,    -1,    18,    52,    26,
       6,    54,    -1,     1,    -1,    53,    -1,    52,    53,    -1,
       8,    -1,     9,    -1,    55,    -1,    54,    55,    -1,    56,
       4,    58,    -1,    56,     4,     3,    58,     5,    58,    -1,
      56,     4,     3,    58,     5,     3,    -1,    56,     4,     3,
       5,     3,    -1,    -1,    57,    -1,    45,    -1,    57,    45,
      -1,    26,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   224,   224,   225,   228,   229,   230,   231,   234,   235,
     238,   239,   240,   243,   246,   247,   250,   312,   337,   399,
     424,   439,   478,   479,   482,   506,   542,   574,   578,   579,
     582,   623,   634,   644,   658,   662,   688,   692,   702,   718,
     744,   747,   751,   757,   758,   759,   760,   763,   785,   804,
     834,   857,   889,   921,   925,   926,   932,   934,   938,   956,
     976,  1006,  1034,  1061,  1090,  1105,  1108,  1150,  1190
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'-'", "'>'", "','", "':'", "'.'", "'*'",
  "'~'", "ALLAFFIXES", "ALTSTRINGCHAR", "ALTSTRINGTYPE", "BOUNDARYCHARS",
  "COMPOUNDMIN", "COMPOUNDWORDS", "CONTROLLED", "DEFSTRINGTYPE", "FLAG",
  "FLAGMARKER", "NROFFCHARS", "OFF", "ON", "PREFIXES", "RANGE", "SUFFIXES",
  "STRING", "STRINGCHAR", "TEXCHARS", "WORDCHARS", "$accept", "file",
  "headers", "option_group", "charset_group", "deftype_stmt",
  "altchar_group", "charset_stmt", "altchar_stmt", "stringtype_info",
  "filesuf_list", "filesuf", "altchar_spec_group", "altchar_spec",
  "option_stmt", "char_set", "on_or_off", "tables", "prefix_table",
  "suffix_table", "table", "flagdef", "flagoptions", "flagoption", "rules",
  "affix_rule", "cond_or_null", "conditions", "ichar_string", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    45,    62,    44,    58,    46,    42,   126,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    30,    31,    31,    32,    32,    32,    32,    33,    33,
      34,    34,    34,    35,    36,    36,    37,    37,    37,    37,
      37,    37,    38,    38,    39,    40,    40,    41,    42,    42,
      43,    44,    44,    44,    44,    44,    44,    44,    45,    45,
      45,    46,    46,    47,    47,    47,    47,    48,    49,    50,
      50,    51,    51,    51,    52,    52,    53,    53,    54,    54,
      55,    55,    55,    55,    56,    56,    57,    57,    58
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     3,     1,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     3,     2,     3,     2,
       2,     3,     2,     3,     3,     1,     2,     1,     1,     2,
       3,     2,     2,     2,     2,     3,     2,     2,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     2,     2,     1,
       2,     4,     5,     1,     1,     2,     1,     1,     1,     2,
       3,     6,     6,     5,     0,     1,     1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     0,    11,     8,
       3,    45,    46,    42,    41,    36,    38,    40,    39,    19,
      33,     0,    34,     0,    13,    37,    31,    53,     0,     0,
      49,     0,    20,    32,    17,     1,     2,     4,     9,     0,
       7,    12,    14,    10,    43,    44,    18,    35,     0,    56,
      57,     0,     0,    54,    50,    21,    16,     5,    22,    15,
      27,    24,    25,    64,     0,    55,     0,    23,    28,    26,
      66,    51,    58,     0,    65,    64,     0,    29,    59,     0,
      67,    52,    30,     0,    68,    60,     0,     0,    63,     0,
      62,    61
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    50,    18,    52,    34,
      71,    72,    77,    78,    19,    80,    25,    20,    21,    22,
      39,    40,    62,    63,    81,    82,    83,    84,    95
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -70
static const yytype_int8 yypact[] =
{
      26,    58,    43,   -12,    36,     4,    51,    55,    67,    67,
      57,    60,    43,    18,    -8,    46,    35,    -4,   -70,   -70,
     -70,    59,    64,   -70,   -70,   -70,   -70,   -70,   -70,    43,
     -70,    65,   -70,    66,   -70,   -70,   -70,   -70,    11,     3,
     -70,     6,    68,   -70,    43,   -70,   -70,    35,   -70,     4,
      78,   -70,   -70,   -70,   -70,   -70,   -70,   -70,    69,   -70,
     -70,    87,    62,   -70,   -70,   -70,   -70,    78,    85,   -70,
     -70,    69,   -70,    43,    91,   -70,    72,    85,   -70,   -70,
     -70,     9,   -70,    95,    43,    43,    74,   -70,   -70,     5,
     -70,     9,   -70,     0,   -70,   -70,    98,    97,   -70,     8,
     -70,   -70
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -70,   -70,   -70,   -70,    88,   -70,    61,   -15,    22,    56,
     -70,    33,   -70,    29,    92,    -2,   105,    96,    89,    93,
     103,    37,   -70,    53,    28,   -69,   -70,   -70,   -55
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -65
static const yytype_int8 yytable[] =
{
      29,    51,    53,   -47,    37,    96,   -48,    37,    93,     2,
      44,   100,    88,   -64,    30,     8,    26,     9,    45,    59,
      60,    38,    88,    10,    38,    12,    94,    56,   -47,   -48,
      33,    94,    51,    27,    94,    28,     1,    61,    97,     2,
       3,     4,    66,     5,   101,     6,     7,    49,     2,     8,
      26,     9,    31,    10,    11,    12,     1,    23,    24,     2,
       3,     4,    10,     5,    12,     6,     7,    27,    37,    28,
      59,    60,    69,    10,    11,    12,    64,    35,    64,    23,
      24,    36,    90,    42,     9,    38,    43,     8,    74,    69,
      49,    57,    58,    73,    65,    70,    76,    85,    86,    89,
      92,    98,    99,    47,    79,    68,    87,    48,    67,    32,
      46,    55,    41,    91,    54,    75
};

static const yytype_uint8 yycheck[] =
{
       2,    16,    17,     0,     1,     5,     0,     1,     3,    13,
      12,     3,    81,     4,    26,    23,     7,    25,     0,     8,
       9,    18,    91,    27,    18,    29,    26,    29,    25,    23,
      26,    26,    47,    24,    26,    26,    10,    26,    93,    13,
      14,    15,    44,    17,    99,    19,    20,    12,    13,    23,
       7,    25,    16,    27,    28,    29,    10,    21,    22,    13,
      14,    15,    27,    17,    29,    19,    20,    24,     1,    26,
       8,     9,    50,    27,    28,    29,    39,    26,    41,    21,
      22,    26,    84,    26,    25,    18,    26,    23,    26,    67,
      12,    26,    26,     6,    26,    26,    11,     6,    26,     4,
      26,     3,     5,    15,    71,    49,    77,    15,    47,     4,
      14,    22,     9,    85,    21,    62
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    10,    13,    14,    15,    17,    19,    20,    23,    25,
      27,    28,    29,    31,    32,    33,    34,    35,    37,    44,
      47,    48,    49,    21,    22,    46,     7,    24,    26,    45,
      26,    16,    46,    26,    39,    26,    26,     1,    18,    50,
      51,    50,    26,    26,    45,     0,    47,    34,    44,    12,
      36,    37,    38,    37,    49,    48,    45,    26,    26,     8,
       9,    26,    52,    53,    51,    26,    45,    36,    39,    38,
      26,    40,    41,     6,    26,    53,    11,    42,    43,    41,
      45,    54,    55,    56,    57,     6,    26,    43,    55,     4,
      45,    54,    26,     3,    26,    58,     5,    58,     3,     5,
       3,    58
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 16:
#line 251 "parse.y"
    {
			    int nextlower;
			    int nextupper;

			    for (nextlower = SET_SIZE + hashheader.nstrchars;
			      --nextlower > SET_SIZE;
			      )
				{
				if ((yyvsp[(2) - (3)].charset).set[nextlower] != 0
				  ||  (yyvsp[(3) - (3)].charset).set[nextlower] != 0)
				    {
				    yyerror (PARSE_Y_NO_WORD_STRINGS);
				    break;
				    }
				}
			    for (nextlower = 0;
			      nextlower < SET_SIZE;
			      nextlower++)
				{
				hashheader.wordchars[nextlower]
				  |= (yyvsp[(2) - (3)].charset).set[nextlower] | (yyvsp[(3) - (3)].charset).set[nextlower];
				hashheader.lowerchars[nextlower]
				  |= (yyvsp[(2) - (3)].charset).set[nextlower];
				hashheader.upperchars[nextlower]
				  |= (yyvsp[(3) - (3)].charset).set[nextlower];
				}
			    for (nextlower = nextupper = 0;
			      nextlower < SET_SIZE;
			      nextlower++)
				{
				if ((yyvsp[(2) - (3)].charset).set[nextlower])
				    {
				    for (  ;
				      nextupper < SET_SIZE
					&&  !(yyvsp[(3) - (3)].charset).set[nextupper];
				      nextupper++)
					;
				    if (nextupper == SET_SIZE)
					yyerror (PARSE_Y_UNMATCHED);
				    else
					{
					hashheader.lowerconv[nextupper]
					  = (ichar_t) nextlower;
					hashheader.upperconv[nextlower]
					  = (ichar_t) nextupper;
					hashheader.sortorder[nextupper]
					  = hashheader.sortval++;
					hashheader.sortorder[nextlower]
					  = hashheader.sortval++;
					nextupper++;
					}
				    }
				}
			    for (  ;  nextupper < SET_SIZE;  nextupper++)
				{
				if ((yyvsp[(3) - (3)].charset).set[nextupper])
				    yyerror (PARSE_Y_UNMATCHED);
				}
			    free ((yyvsp[(2) - (3)].charset).set);
			    free ((yyvsp[(3) - (3)].charset).set);
			    }
    break;

  case 17:
#line 313 "parse.y"
    {
			    int i;

			    for (i = SET_SIZE + hashheader.nstrchars;
			      --i > SET_SIZE;
			      )
				{
				if ((yyvsp[(2) - (2)].charset).set[i] != 0)
				    {
				    yyerror (PARSE_Y_NO_WORD_STRINGS);
				    break;
				    }
				}
			    for (i = 0;  i < SET_SIZE;  i++)
				{
				if ((yyvsp[(2) - (2)].charset).set[i])
				    {
				    hashheader.wordchars[i] = 1;
				    hashheader.sortorder[i]
				      = hashheader.sortval++;
				    }
				}
			    free ((yyvsp[(2) - (2)].charset).set);
			    }
    break;

  case 18:
#line 338 "parse.y"
    {
			    int nextlower;
			    int nextupper;

			    for (nextlower = SET_SIZE + hashheader.nstrchars;
			      --nextlower > SET_SIZE;
			      )
				{
				if ((yyvsp[(2) - (3)].charset).set[nextlower] != 0
				  ||  (yyvsp[(3) - (3)].charset).set[nextlower] != 0)
				    {
				    yyerror (PARSE_Y_NO_BOUNDARY_STRINGS);
				    break;
				    }
				}
			    for (nextlower = 0;
			      nextlower < SET_SIZE;
			      nextlower++)
				{
				hashheader.boundarychars[nextlower]
				  |= (yyvsp[(2) - (3)].charset).set[nextlower] | (yyvsp[(3) - (3)].charset).set[nextlower];
				hashheader.lowerchars[nextlower]
				  |= (yyvsp[(2) - (3)].charset).set[nextlower];
				hashheader.upperchars[nextlower]
				  |= (yyvsp[(3) - (3)].charset).set[nextlower];
				}
			    for (nextlower = nextupper = 0;
			      nextlower < SET_SIZE;
			      nextlower++)
				{
				if ((yyvsp[(2) - (3)].charset).set[nextlower])
				    {
				    for (  ;
				      nextupper < SET_SIZE
					&&  !(yyvsp[(3) - (3)].charset).set[nextupper];
				      nextupper++)
					;
				    if (nextupper == SET_SIZE)
					yyerror (PARSE_Y_UNMATCHED);
				    else
					{
					hashheader.lowerconv[nextupper]
					  = (ichar_t) nextlower;
					hashheader.upperconv[nextlower]
					  = (ichar_t) nextupper;
					hashheader.sortorder[nextupper]
					  = hashheader.sortval++;
					hashheader.sortorder[nextlower]
					  = hashheader.sortval++;
					nextupper++;
					}
				    }
				}
			    for (  ;  nextupper < SET_SIZE;  nextupper++)
				{
				if ((yyvsp[(3) - (3)].charset).set[nextupper])
				    yyerror (PARSE_Y_UNMATCHED);
				}
			    free ((yyvsp[(2) - (3)].charset).set);
			    free ((yyvsp[(3) - (3)].charset).set);
			    }
    break;

  case 19:
#line 400 "parse.y"
    {
			    int i;

			    for (i = SET_SIZE + hashheader.nstrchars;
			      --i > SET_SIZE;
			      )
				{
				if ((yyvsp[(2) - (2)].charset).set[i] != 0)
				    {
				    yyerror (PARSE_Y_NO_BOUNDARY_STRINGS);
				    break;
				    }
				}
			    for (i = 0;  i < SET_SIZE;  i++)
				{
				if ((yyvsp[(2) - (2)].charset).set[i])
				    {
				    hashheader.boundarychars[i] = 1;
				    hashheader.sortorder[i]
				      = hashheader.sortval++;
				    }
				}
			    free ((yyvsp[(2) - (2)].charset).set);
			    }
    break;

  case 20:
#line 425 "parse.y"
    {
			    int len;

			    len = strlen ((char *) (yyvsp[(2) - (2)].string));
			    if (len > MAXSTRINGCHARLEN)
				yyerror (PARSE_Y_LONG_STRING);
			    else if (len == 0)
				yyerror (PARSE_Y_NULL_STRING);
			    else if (hashheader.nstrchars >= MAXSTRINGCHARS)
				yyerror (PARSE_Y_MANY_STRINGS);
			    else
				(void) addstringchar ((yyvsp[(2) - (2)].string), 0, 0);
			    free ((char *) (yyvsp[(2) - (2)].string));
			    }
    break;

  case 21:
#line 440 "parse.y"
    {
			    int lcslot;
			    int len;
			    int ucslot;

			    len = strlen ((char *) (yyvsp[(2) - (3)].string));
			    if (strlen ((char *) (yyvsp[(3) - (3)].string)) != len)
				yyerror (PARSE_Y_LENGTH_MISMATCH);
			    else if (len > MAXSTRINGCHARLEN)
				yyerror (PARSE_Y_LONG_STRING);
			    else if (len == 0)
				yyerror (PARSE_Y_NULL_STRING);
			    else if (hashheader.nstrchars >= MAXSTRINGCHARS)
				yyerror (PARSE_Y_MANY_STRINGS);
			    else
				{
				/*
				 * Add the uppercase character first, so that
				 * it will sort first.
				 */
				lcslot = ucslot = addstringchar ((yyvsp[(3) - (3)].string), 0, 1);
				if (ucslot >= 0)
				    lcslot = addstringchar ((yyvsp[(2) - (3)].string), 1, 0);
				if (ucslot >= 0  &&  lcslot >= 0)
				    {
				    if (ucslot >= lcslot)
					ucslot++;
				    hashheader.lowerconv[ucslot] =
				      (ichar_t) lcslot;
				    hashheader.upperconv[lcslot] =
				      (ichar_t) ucslot;
				    }
				}
			    free ((char *) (yyvsp[(2) - (3)].string));
			    free ((char *) (yyvsp[(3) - (3)].string));
			    }
    break;

  case 24:
#line 483 "parse.y"
    {
			    chartypes[ctypenum].name = (char *) (yyvsp[(1) - (3)].string);
			    chartypes[ctypenum].deformatter = (char *) (yyvsp[(2) - (3)].string);
			    /*
			     * Implement a few common synonyms.  This should
			     * be generalized.
			     */
			    if (strcmp ((char *) (yyvsp[(2) - (3)].string), "TeX") == 0)
				(void) strcpy ((char *) (yyvsp[(2) - (3)].string), "tex");
			    else if (strcmp ((char *) (yyvsp[(2) - (3)].string), "troff") == 0)
				(void) strcpy ((char *) (yyvsp[(2) - (3)].string), "nroff");
			    /*
			     * Someday, we'll accept generalized deformatters.
			     * Then we can get rid of this test.
			     */
			    if (strcmp ((char *) (yyvsp[(2) - (3)].string), "nroff") != 0
			      &&  strcmp ((char *) (yyvsp[(2) - (3)].string), "tex") != 0)
				yyerror (PARSE_Y_BAD_DEFORMATTER);
			    ctypenum++;
			    hashheader.nstrchartype = ctypenum;
			    }
    break;

  case 25:
#line 507 "parse.y"
    {
			    if (ctypenum >= ctypesize)
				{
				if (ctypesize == 0)
				    chartypes = (struct strchartype *)
				      malloc (TBLINC
					* sizeof (struct strchartype));
				else
				    chartypes = (struct strchartype *)
				      realloc ((char *) chartypes,
					(ctypesize + TBLINC)
					  * sizeof (struct strchartype));
				if (chartypes == NULL)
				    {
				    yyerror (PARSE_Y_NO_SPACE);
				    exit (1);
				    }
				ctypesize += TBLINC;
				}
			    ctypechars =
			      TBLINC * (strlen ((char *) (yyvsp[(1) - (1)].string)) + 1) + 1;
			    chartypes[ctypenum].suffixes =
			      malloc ((unsigned int) ctypechars);
			    if (chartypes[ctypenum].suffixes == NULL)
				{
				yyerror (PARSE_Y_NO_SPACE);
				exit (1);
				}
			    (void) strcpy (chartypes[ctypenum].suffixes,
			     (char *) (yyvsp[(1) - (1)].string));
			    chartypes[ctypenum].suffixes
				[strlen ((char *) (yyvsp[(1) - (1)].string)) + 1]
			      = '\0';
			    free ((char *) (yyvsp[(1) - (1)].string));
			    }
    break;

  case 26:
#line 543 "parse.y"
    {
			    char *	nexttype;
			    int		offset;

			    for (nexttype = chartypes[ctypenum].suffixes;
			      *nexttype != '\0';
			      nexttype += strlen (nexttype) + 1)
				;
			    offset = nexttype - chartypes[ctypenum].suffixes;
			    if ((int) (offset + strlen ((char *) (yyvsp[(2) - (2)].string)) + 1)
			       >= ctypechars)
				{
				ctypechars +=
				  TBLINC * (strlen ((char *) (yyvsp[(2) - (2)].string)) + 1);
				chartypes[ctypenum].suffixes =
				  realloc (chartypes[ctypenum].suffixes,
				    (unsigned int) ctypechars);
				if (chartypes[ctypenum].suffixes == NULL)
				    {
				    yyerror (PARSE_Y_NO_SPACE);
				    exit (1);
				    }
				nexttype =
				  chartypes[ctypenum].suffixes + offset;
				}
			    (void) strcpy (nexttype, (char *) (yyvsp[(2) - (2)].string));
			    nexttype[strlen ((char *) (yyvsp[(2) - (2)].string)) + 1] = '\0';
			    free ((char *) (yyvsp[(2) - (2)].string));
			    }
    break;

  case 30:
#line 583 "parse.y"
    {
			    int i;
			    int len;
			    int slot;

			    len = strlen ((char *) (yyvsp[(2) - (3)].string));
			    if (len > MAXSTRINGCHARLEN)
				yyerror (PARSE_Y_LONG_STRING);
			    else if (len == 0)
				yyerror (PARSE_Y_NULL_STRING);
			    else if (hashheader.nstrchars >= MAXSTRINGCHARS)
				yyerror (PARSE_Y_MANY_STRINGS);
			    else if (!isstringch ((char *) (yyvsp[(3) - (3)].string), 1))
				yyerror (PARSE_Y_NO_SUCH_STRING);
			    else
				{
				slot = addstringchar ((yyvsp[(2) - (3)].string), 0, 0) - SET_SIZE;
				if (laststringch >= slot)
				    laststringch++;
				hashheader.stringdups[slot] = laststringch;
				for (i = hashheader.nstrchars;  --i >= 0;  )
				    {
				    if (hashheader.stringdups[i]
				      == laststringch)
					hashheader.dupnos[slot]++;
				    }
				/*
				 * The above code sets dupnos one too high,
				 * because it counts the character itself.
				 */
				if (hashheader.dupnos[slot]
				  != hashheader.nstrchartype)
				    yyerror (PARSE_Y_MULTIPLE_STRINGS);
				hashheader.dupnos[slot]--;
				}
			    free ((char *) (yyvsp[(2) - (3)].string));
			    free ((char *) (yyvsp[(3) - (3)].string));
			    }
    break;

  case 31:
#line 624 "parse.y"
    {
			    if (strlen ((char *) (yyvsp[(2) - (2)].string))
			      == sizeof (hashheader.nrchars))
				(void) bcopy ((char *) (yyvsp[(2) - (2)].string), hashheader.nrchars,
				  sizeof (hashheader.nrchars));
			    else
				yyerror (PARSE_Y_WRONG_NROFF);
			    free ((char *) (yyvsp[(2) - (2)].string));
			    }
    break;

  case 32:
#line 635 "parse.y"
    {
			    if (strlen ((char *) (yyvsp[(2) - (2)].string))
			      == sizeof (hashheader.texchars))
				(void) bcopy ((char *) (yyvsp[(2) - (2)].string), hashheader.texchars,
				  sizeof (hashheader.texchars));
			    else
				yyerror (PARSE_Y_WRONG_TEX);
			    free ((char *) (yyvsp[(2) - (2)].string));
			    }
    break;

  case 33:
#line 645 "parse.y"
    {
			    unsigned char * digitp; /* Pointer to next digit */

			    for (digitp = (yyvsp[(2) - (2)].string);  *digitp != '\0';  digitp++)
				{
				if (*digitp <= '0'  ||  *digitp >= '9')
				    {
				    yyerror (PARSE_Y_BAD_NUMBER);
				    break;
				    }
				}
			    hashheader.compoundmin = atoi ((char *) (yyvsp[(2) - (2)].string));
			    }
    break;

  case 34:
#line 659 "parse.y"
    {
			    hashheader.compoundflag = (yyvsp[(2) - (2)].simple);
			    }
    break;

  case 35:
#line 663 "parse.y"
    {
			    if (strlen ((char *) (yyvsp[(3) - (3)].string)) != 1)
				yyerror (PARSE_Y_LONG_FLAG);
			    else if (hashheader.compoundbit >= 0)
				yyerror (PARSE_Y_DOUBLE_COMPOUND);
			    else
				{
				hashheader.compoundbit = (unsigned char) (yyvsp[(3) - (3)].string)[0];
#if MASKBITS <= 128
				hashheader.compoundbit &= 0x7f;
#endif /* MASKBITS */
#if MASKBITS <= 32
				if (islower (hashheader.compoundbit))
				    hashheader.compoundbit =
				      toupper (hashheader.compoundbit);
#endif /* MASKBITS */
#if MASKBITS <= 64
				if (!isalpha (hashheader.compoundbit))
				    yyerror (PARSE_Y_BAD_FLAG);
#endif /* MASKBITS */
				hashheader.compoundbit =
				  CHARTOBIT (hashheader.compoundbit);
				}
			    hashheader.compoundflag = COMPOUND_CONTROLLED;
			    }
    break;

  case 36:
#line 689 "parse.y"
    {
			    hashheader.defhardflag = (yyvsp[(2) - (2)].simple);
			    }
    break;

  case 37:
#line 693 "parse.y"
    {
			    if (strlen ((char *) (yyvsp[(2) - (2)].string)) != 1)
				yyerror (PARSE_Y_LONG_FLAG);
			    else
				hashheader.flagmarker = (yyvsp[(2) - (2)].string)[0];
			    free ((char *) (yyvsp[(2) - (2)].string));
			    }
    break;

  case 38:
#line 703 "parse.y"
    {
			    int		i;
			    char *	set;

			    set = malloc (SET_SIZE + MAXSTRINGCHARS);
			    if (set == NULL)
				{
				yyerror (PARSE_Y_NO_SPACE);
				exit (1);
				}
			    (yyval.charset).set = set;
			    for (i = SET_SIZE + MAXSTRINGCHARS;  --i >= 0;  )
				*set++ = 1;
			    (yyval.charset).complement = 0;
			    }
    break;

  case 39:
#line 719 "parse.y"
    {
			    int		setlen;

			    (yyval.charset).set = malloc (SET_SIZE + MAXSTRINGCHARS);
			    if ((yyval.charset).set == NULL)
				{
				yyerror (PARSE_Y_NO_SPACE);
				exit (1);
				}
			    (void) bzero ((yyval.charset).set, SET_SIZE + MAXSTRINGCHARS);
			    if (l1_isstringch ((char *) (yyvsp[(1) - (1)].string), setlen, 1))
				{
				if (setlen != strlen ((char *) (yyvsp[(1) - (1)].string)))
				    yyerror (PARSE_Y_NEED_BLANK);
				(yyval.charset).set[SET_SIZE + laststringch] = 1;
				}
			    else
				{
				if (strlen ((char *) (yyvsp[(1) - (1)].string)) != 1)
				    yyerror (PARSE_Y_NEED_BLANK);
				(yyval.charset).set[*(yyvsp[(1) - (1)].string)] = 1;
				}
			    free ((char *) (yyvsp[(1) - (1)].string));
			    (yyval.charset).complement = 0;
			    }
    break;

  case 41:
#line 748 "parse.y"
    {
			    (yyval.simple) = 1;
			    }
    break;

  case 42:
#line 752 "parse.y"
    {
			    (yyval.simple) = 0;
			    }
    break;

  case 47:
#line 764 "parse.y"
    {
			    pflaglist = table;
			    numpflags = tblnum;
			    /*
			     * Sort the flag table.  This is critical so
			     * that ispell can build a correct index
			     * table.  The idea is to put similar affixes
			     * together.
			     */
			    qsort ((char *) table, (unsigned) tblnum,
			      sizeof (*table),
			      (int (*) P ((const void *, const void *)))
				precmp);
#ifdef TBLDEBUG
			    (void) fprintf (stderr, "prefixes\n");
			    tbldump (table, tblnum);
#endif
			    tblsize = 0;
			    }
    break;

  case 48:
#line 786 "parse.y"
    {
			    sflaglist = table;
			    numsflags = tblnum;
			    /*
			     * See comments on the prefix sort.
			     */
			    qsort ((char *) table, (unsigned) tblnum,
			      sizeof (*table),
			      (int (*) P ((const void *, const void *)))
				sufcmp);
#ifdef TBLDEBUG
			    (void) fprintf (stderr, "suffixes\n");
			    tbldump (table, tblnum);
#endif
			    tblsize = 0;
			    }
    break;

  case 49:
#line 805 "parse.y"
    {
			    if (tblsize == 0)
				{
				tblsize = centnum + TBLINC;
				tblnum = 0;
				table = (struct flagent *)
				  malloc (tblsize * (sizeof (struct flagent)));
				if (table == NULL)
				    {
				    yyerror (PARSE_Y_NO_SPACE);
				    exit (1);
				    }
				}
			    else if (tblnum + centnum >= tblsize)
				{
				tblsize = tblnum + centnum + TBLINC;
				table = (struct flagent *)
				  realloc ((char *) table,
				    tblsize * (sizeof (struct flagent)));
				if (table == NULL)
				    {
				    yyerror (PARSE_Y_NO_SPACE);
				    exit (1);
				    }
				}
			    for (tblnum = 0;  tblnum < centnum;  tblnum++)
				table[tblnum] = curents[tblnum];
			    centnum = 0;
			    }
    break;

  case 50:
#line 835 "parse.y"
    {
			    int i;

			    if (tblnum + centnum >= tblsize)
				{
				tblsize = tblnum + centnum + TBLINC;
				table = (struct flagent *)
				  realloc ((char *) table,
				    tblsize * (sizeof (struct flagent)));
				if (table == NULL)
				    {
				    yyerror (PARSE_Y_NO_SPACE);
				    exit (1);
				    }
				}
			    for (i = 0;  i < centnum;  i++)
				table[tblnum + i] = curents[i];
			    tblnum += centnum;
			    centnum = 0;
			    }
    break;

  case 51:
#line 858 "parse.y"
    {
			    int flagbit;
			    int i;

			    if (strlen ((char *) (yyvsp[(2) - (4)].string)) != 1)
				yyerror (PARSE_Y_LONG_FLAG);
			    flagbit = (unsigned char) (yyvsp[(2) - (4)].string)[0];
#if MASKBITS <= 128
			    flagbit &= 0x7f;
#endif /* MASKBITS */
#if MASKBITS <= 32
			    if (islower (flagbit))
				flagbit = toupper (flagbit);
#endif /* MASKBITS */
#if MASKBITS <= 64
			    if (!isalpha (flagbit))
				yyerror (PARSE_Y_BAD_FLAG);
#endif /* MASKBITS */
			    flagbit = CHARTOBIT (flagbit);
			    for (i = 0;  i < tblnum;  i++)
				{
				if (table[i].flagbit == flagbit)
				    yyerror (PARSE_Y_DUP_FLAG);
				}
			    for (i = 0;  i < centnum;  i++)
				{
				curents[i].flagbit = flagbit;
				curents[i].flagflags = 0;
				}
			    free ((char *) (yyvsp[(2) - (4)].string));
			    }
    break;

  case 52:
#line 890 "parse.y"
    {
			    int flagbit;
			    int i;

			    if (strlen ((char *) (yyvsp[(3) - (5)].string)) != 1)
				yyerror (PARSE_Y_LONG_FLAG);
			    flagbit = (unsigned char) (yyvsp[(3) - (5)].string)[0];
#if MASKBITS <= 128
			    flagbit &= 0x7f;
#endif /* MASKBITS */
#if MASKBITS <= 32
			    if (islower (flagbit))
				flagbit = toupper (flagbit);
#endif /* MASKBITS */
#if MASKBITS <= 64
			    if (!isalpha (flagbit))
				yyerror (PARSE_Y_BAD_FLAG);
#endif /* MASKBITS */
			    flagbit = CHARTOBIT (flagbit);
			    for (i = 0;  i < tblnum;  i++)
				{
				if (table[i].flagbit == flagbit)
				    yyerror (PARSE_Y_DUP_FLAG);
				}
			    for (i = 0;  i < centnum;  i++)
				{
				curents[i].flagbit = flagbit;
				curents[i].flagflags = (yyvsp[(2) - (5)].simple);
				}
			    free ((char *) (yyvsp[(3) - (5)].string));
			    }
    break;

  case 53:
#line 922 "parse.y"
    { (yyval.simple) = 0; }
    break;

  case 55:
#line 927 "parse.y"
    {
			    (yyval.simple) = (yyvsp[(1) - (2)].simple) | (yyvsp[(2) - (2)].simple);
			    }
    break;

  case 56:
#line 933 "parse.y"
    { (yyval.simple) = FF_CROSSPRODUCT; }
    break;

  case 57:
#line 935 "parse.y"
    { (yyval.simple) = FF_COMPOUNDONLY; }
    break;

  case 58:
#line 939 "parse.y"
    {
			    if (centsize == 0)
				{
				curents = (struct flagent *)
				  malloc (TBLINC * (sizeof (struct flagent)));
				if (curents == NULL)
				    {
				    yyerror (PARSE_Y_NO_SPACE);
				    exit (1);
				    }
				centsize = TBLINC;
				}
			    curents[0] = *(yyvsp[(1) - (1)].entry);
			    centnum = 1;
			    free ((char *) (yyvsp[(1) - (1)].entry));
			    (yyval.simple) = 0;
			    }
    break;

  case 59:
#line 957 "parse.y"
    {
			    if (centnum >= centsize)
				{
				centsize += TBLINC;
				curents = (struct flagent *)
				  realloc ((char *) curents,
				    centsize * (sizeof (struct flagent)));
				if (curents == NULL)
				    {
				    yyerror (PARSE_Y_NO_SPACE);
				    exit (1);
				    }
				}
			    curents[centnum] = *(yyvsp[(2) - (2)].entry);
			    centnum++;
			    free ((char *) (yyvsp[(2) - (2)].entry));
			    }
    break;

  case 60:
#line 977 "parse.y"
    {
			    int		i;

			    (yyvsp[(1) - (3)].entry)->stripl = 0;
			    (yyvsp[(1) - (3)].entry)->strip = NULL;
			    (yyvsp[(1) - (3)].entry)->affl = icharlen ((yyvsp[(3) - (3)].istr));
			    (yyvsp[(1) - (3)].entry)->affix = (yyvsp[(3) - (3)].istr);
			    upcase ((yyvsp[(3) - (3)].istr));
			    /*
			     * As a special optimization (and a
			     * concession to those who prefer the syntax
			     * that way), convert any single condition
			     * that accepts all characters into no
			     * condition at all.
			     */
			    if ((yyvsp[(1) - (3)].entry)->numconds == 1)
				{
				for (i = SET_SIZE + hashheader.nstrchars;
				  --i >= 0;
				  )
				    {
				    if (((yyvsp[(1) - (3)].entry)->conds[i] & 1) == 0)
					break;
				    }
				if (i < 0)
				    (yyvsp[(1) - (3)].entry)->numconds = 0;
				}
			    (yyval.entry) = (yyvsp[(1) - (3)].entry);
			    }
    break;

  case 61:
#line 1007 "parse.y"
    {
			    int		    i;

			    (yyvsp[(1) - (6)].entry)->stripl = icharlen ((yyvsp[(4) - (6)].istr));
			    (yyvsp[(1) - (6)].entry)->strip = (yyvsp[(4) - (6)].istr);
			    upcase ((yyvsp[(4) - (6)].istr));
			    (yyvsp[(1) - (6)].entry)->affl = icharlen ((yyvsp[(6) - (6)].istr));
			    (yyvsp[(1) - (6)].entry)->affix = (yyvsp[(6) - (6)].istr);
			    upcase ((yyvsp[(6) - (6)].istr));
			    /*
			     * Convert the syntax ". > -xxx,yyy" into
			     * " > -xxx,yyy", as in the code just above.
			     */
			    if ((yyvsp[(1) - (6)].entry)->numconds == 1)
				{
				for (i = SET_SIZE + hashheader.nstrchars;
				  --i >= 0;
				  )
				    {
				    if (((yyvsp[(1) - (6)].entry)->conds[i] & 1) == 0)
					break;
				    }
				if (i < 0)
				    (yyvsp[(1) - (6)].entry)->numconds = 0;
				}
			    (yyval.entry) = (yyvsp[(1) - (6)].entry);
			    }
    break;

  case 62:
#line 1035 "parse.y"
    {
			    int		    i;

			    (yyvsp[(1) - (6)].entry)->stripl = icharlen ((yyvsp[(4) - (6)].istr));
			    (yyvsp[(1) - (6)].entry)->strip = (yyvsp[(4) - (6)].istr);
			    upcase ((yyvsp[(4) - (6)].istr));
			    (yyvsp[(1) - (6)].entry)->affl = 0;
			    (yyvsp[(1) - (6)].entry)->affix = NULL;
			    /*
			     * Convert the syntax ". > -xxx," into
			     * " > -xxx,", as in the code just above.
			     */
			    if ((yyvsp[(1) - (6)].entry)->numconds == 1)
				{
				for (i = SET_SIZE + hashheader.nstrchars;
				  --i >= 0;
				  )
				    {
				    if (((yyvsp[(1) - (6)].entry)->conds[i] & 1) == 0)
					break;
				    }
				if (i < 0)
				    (yyvsp[(1) - (6)].entry)->numconds = 0;
				}
			    (yyval.entry) = (yyvsp[(1) - (6)].entry);
			    }
    break;

  case 63:
#line 1062 "parse.y"
    {
			    int		    i;

			    (yyvsp[(1) - (5)].entry)->stripl = 0;
			    (yyvsp[(1) - (5)].entry)->strip = NULL;
			    (yyvsp[(1) - (5)].entry)->affl = 0;
			    (yyvsp[(1) - (5)].entry)->affix = NULL;
			    /*
			     * Convert the syntax ". > -,-" into
			     * " > -,-", as in the code just above.
			     */
			    if ((yyvsp[(1) - (5)].entry)->numconds == 1)
				{
				for (i = SET_SIZE + hashheader.nstrchars;
				  --i >= 0;
				  )
				    {
				    if (((yyvsp[(1) - (5)].entry)->conds[i] & 1) == 0)
					break;
				    }
				if (i < 0)
				    (yyvsp[(1) - (5)].entry)->numconds = 0;
				}
			    (yyval.entry) = (yyvsp[(1) - (5)].entry);
			    }
    break;

  case 64:
#line 1090 "parse.y"
    {
			    struct flagent *	ent;

			    ent = (struct flagent *)
			      malloc (sizeof (struct flagent));
			    if (ent == NULL)
				{
				yyerror (PARSE_Y_NO_SPACE);
				exit (1);
				}
			    ent->numconds = 0;
			    (void) bzero (ent->conds,
			       SET_SIZE + MAXSTRINGCHARS);
			    (yyval.entry) = ent;
			    }
    break;

  case 66:
#line 1109 "parse.y"
    {
			    struct flagent *	ent;
			    int			i;

			    ent = (struct flagent *)
			      malloc (sizeof (struct flagent));
			    if (ent == NULL)
				{
				yyerror (PARSE_Y_NO_SPACE);
				exit (1);
				}
			    ent->numconds = 1;
			    (void) bzero (ent->conds,
			       SET_SIZE + MAXSTRINGCHARS);
			    /*
			     * Copy conditions to the new entry, making
			     * sure that uppercase versions are generated
			     * for lowercase input.
			     */
			    for (i = SET_SIZE + MAXSTRINGCHARS;  --i >= 0;  )
				{
				if ((yyvsp[(1) - (1)].charset).set[i])
				    {
				    ent->conds[i] = 1;
				    if (!(yyvsp[(1) - (1)].charset).complement)
					ent->conds[mytoupper ((ichar_t) i)] = 1;
				    }
				}
			    if ((yyvsp[(1) - (1)].charset).complement)
				{
				for (i = SET_SIZE + MAXSTRINGCHARS;
				--i >= 0;
				)
				    {
				    if ((yyvsp[(1) - (1)].charset).set[i] == 0)
					ent->conds[mytoupper ((ichar_t) i)] = 0;
				    }
				}
			    free ((yyvsp[(1) - (1)].charset).set);
			    (yyval.entry) = ent;
			    }
    break;

  case 67:
#line 1151 "parse.y"
    {
			    int			i;
			    int			mask;

			    if ((yyvsp[(1) - (2)].entry)->numconds >= 8)
				{
				yyerror (PARSE_Y_MANY_CONDS);
				(yyvsp[(1) - (2)].entry)->numconds = 7;
				}
			    mask = 1 << (yyvsp[(1) - (2)].entry)->numconds;
			    (yyvsp[(1) - (2)].entry)->numconds++;
			    for (i = SET_SIZE + MAXSTRINGCHARS;
			      --i >= 0;
			      )
				{
				if ((yyvsp[(2) - (2)].charset).set[i])
				    {
				    (yyvsp[(1) - (2)].entry)->conds[i] |= mask;
				    if (!(yyvsp[(2) - (2)].charset).complement)
					(yyvsp[(1) - (2)].entry)->conds[mytoupper ((ichar_t) i)]
					  |= mask;
				    }
				}
			    if ((yyvsp[(2) - (2)].charset).complement)
				{
				mask = ~mask;
				for (i = SET_SIZE + MAXSTRINGCHARS;
				  --i >= 0;
				  )
				    {
				    if ((yyvsp[(2) - (2)].charset).set[i] == 0)
					(yyvsp[(1) - (2)].entry)->conds[mytoupper ((ichar_t) i)]
					  &= mask;
				    }
				}
			    free ((yyvsp[(2) - (2)].charset).set);
			    }
    break;

  case 68:
#line 1191 "parse.y"
    {
			    ichar_t *tichar;

			    tichar = strtosichar ((char *) (yyvsp[(1) - (1)].string), 1);
			    (yyval.istr) = (ichar_t *) malloc (sizeof (ichar_t)
			      * (icharlen (tichar) + 1));
			    if ((yyval.istr) == NULL)
				{
				yyerror (PARSE_Y_NO_SPACE);
				exit (1);
				}
			    (void) icharcpy ((yyval.istr), tichar);
			    free ((char *) (yyvsp[(1) - (1)].string));
			    }
    break;


/* Line 1267 of yacc.c.  */
#line 2630 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1206 "parse.y"

static struct kwtab			/* Table of built-in keywords */
		keywords[] =
    {
    {"allaffixes", ALLAFFIXES},
    {"altstringchar", ALTSTRINGCHAR},
    {"altstringtype", ALTSTRINGTYPE},
    {"boundarychars", BOUNDARYCHARS},
    {"compoundmin", COMPOUNDMIN},
    {"compoundwords", COMPOUNDWORDS},
    {"controlled", CONTROLLED},
    {"defstringtype", DEFSTRINGTYPE},
    {"flag", FLAG},
    {"flagmarker", FLAGMARKER},
    {"nroffchars", NROFFCHARS},
    {"troffchars", NROFFCHARS},
    {"on", ON},
    {"off", OFF},
    {"prefixes", PREFIXES},
    {"stringchar", STRINGCHAR},
    {"suffixes", SUFFIXES},
    {"TeXchars", TEXCHARS},
    {"texchars", TEXCHARS},
    {"wordchars", WORDCHARS},
    {NULL, 0}
    };

/*
 * Trivial lexical analyzer.
 */
static int yylex ()
    {
    int			backslashed; /* NZ if backslash appeared */
    register int	ch;	/* Next character seen */
    register unsigned char *
			lexp;	/* Pointer into lexstring */
    unsigned char	lexstring[256];	/* Space for collecting strings */

    while ((ch = grabchar ()) != EOF  &&  (isspace (ch)  ||  ch == '#'))
	{			/* Skip whitespace and comments */
	if (ch == '#')
	    {
	    while ((ch = grabchar ()) != EOF  &&  ch != '\n')
		;
	    }
	}
    switch (ch)
	{
	case EOF:
	    return EOF;
	case '"':
	    getqstring ();
	    return STRING;
	case '-':
	case '>':
	case ',':
	case ':':
	case '.':
	case '*':
	case '~':
	    yylval.simple = ch;
	    return ch;
	case '[':		/* Beginning of a range set ] */
	    getrange ();	/* Get the range */
	    return RANGE;
	}
    /*
     * We get here if the character is an ordinary one;  note that
     * this includes backslashes.
     */
    backslashed = 0;
    lexp = lexstring;
    for (  ;  ;  )
	{
	switch (ch)
	    {
	    case EOF:
		*lexp = '\0';
		return kwanalyze (backslashed, lexstring);
	    case '\\':
		backslashed = 1;
		ch = backch ();
		*lexp++ = (char) ch;
		break;
	    case ' ':
	    case '\t':
	    case '\n':
	    case '\f':
	    case '\r':
		*lexp = '\0';
		return kwanalyze (backslashed, lexstring);
	    case '#':
	    case '>':
	    case ':':
	    case '-':
	    case ',':
	    case '[':			/* ] */
		ungrabchar (ch);
		*lexp = '\0';
		return kwanalyze (backslashed, lexstring);
	    default:
		*lexp++ = (char) ch;
#ifdef NO8BIT
		if (ch & 0x80)
		    yyerror (PARSE_Y_8_BIT);
#endif /* NO8BIT */
		break;
	    }
	ch = grabchar ();
	}
    }

static int kwanalyze (backslashed, str)
    int			backslashed;	/* NZ if string had a backslash */
    register unsigned char *
			str;		/* String to analyze */
    {
    register struct kwtab *
			kwptr;		/* Pointer into keyword table */

    yylval.simple = 0;
    if (!backslashed)			/* Backslash means not keyword */
	{
	for (kwptr = keywords;  kwptr->kw != NULL;  kwptr++)
	    {
	    if (strcmp (kwptr->kw, (char *) str) == 0)
		return (yylval.simple = kwptr->val);
	    }
	}
    yylval.string =
      (unsigned char *) malloc ((unsigned) strlen ((char *) str) + 1);
    if (yylval.string == NULL)
	{
	yyerror (PARSE_Y_NO_SPACE);
	exit (1);
	}
    (void) strcpy ((char *) yylval.string, (char *) str);
#ifdef NO8BIT
    while (*str != '\0')
	{
	if (*str++ & 0x80)
	    yyerror (PARSE_Y_8_BIT);
	}
#endif /* NO8BIT */
    return STRING;
    }

/*
 * Analyze a string in double quotes.  The leading quote has already
 * been processed.
 */
static void getqstring ()
    {
    register int	ch;		/* Next character read */
    char		lexstring[256];	/* Room to collect the string */
    register char *	lexp;		/* Pointer into lexstring */

    for (lexp = lexstring;
      (ch = grabchar ()) != EOF  &&  ch != '"'
	&&  lexp < &lexstring[sizeof lexstring - 1];
      )
	{
	if (ch == '\\')
	    ch = backch ();
	*lexp++ = (char) ch;
	}
    *lexp++ = '\0';
    if (ch == EOF)
	yyerror (PARSE_Y_EOF);
    else if (ch != '"')
	{
	yyerror (PARSE_Y_LONG_QUOTE);
	while ((ch = grabchar ()) != EOF  &&  ch != '"')
	    {
	    if (ch == '\\')
		ch = backch ();
	    }
	}
    yylval.string = (unsigned char *) malloc ((unsigned) (lexp - lexstring));
    if (yylval.string == NULL)
	{
	yyerror (PARSE_Y_NO_SPACE);
	exit (1);
	}
    (void) strcpy ((char *) yylval.string, lexstring);
#ifdef NO8BIT
    for (lexp = lexstring;  *lexp != '\0';  )
	{
	if (*lexp++ & 0x80)
	    yyerror (PARSE_Y_8_BIT);
	}
#endif /* NO8BIT */
    }

/*
 * Analyze a range (e.g., [A-Za-z]).  The left square bracket
 * has already been processed.
 */
static void getrange ()			/* Parse a range set */
    {
    register int	ch;		/* Next character read */
    register int	lastch;		/* Previous char, for ranges */
    char		stringch[MAXSTRINGCHARLEN];
    int			stringchlen;

    yylval.charset.set = malloc (SET_SIZE + MAXSTRINGCHARS);
    if (yylval.charset.set == NULL)
	{
	yyerror (PARSE_Y_NO_SPACE);
	exit (1);
	}

    /* Start with a null set */
    (void) bzero (yylval.charset.set, SET_SIZE + MAXSTRINGCHARS);
    yylval.charset.complement = 0;

    lastch = -1;
    ch = grabchar ();
    if (ch == '^')
	{
	yylval.charset.complement = 1;
	ch = grabchar ();
	}
    /* [ */
    if (ch == ']')
	{
	/* [[ */
	lastch = ']';
	yylval.charset.set[']'] = 1;
	}
    else
	ungrabchar (ch);
    /* [ */
    while ((ch = grabchar ()) != EOF  &&  ch != ']')
	{
	if (isstringstart (ch))		/* Handle a possible string character */
	    {
	    stringch[0] = (char) ch;
	    for (stringchlen = 1;
	      stringchlen < MAXSTRINGCHARLEN;
	      stringchlen++)
		{
		stringch[stringchlen] = '\0';
		if (isstringch (stringch, 1))
		    {
		    yylval.charset.set[SET_SIZE + laststringch] = 1;
		    stringchlen = 0;
		    break;
		    }
		ch = grabchar ();
		if (ch == EOF)
		    break;
		else
		    stringch[stringchlen] = (char) ch;
		}
	    if (stringchlen == 0)
		{
		lastch = -1;		/* String characters can't be ranges */
		continue;		/* We found a string character */
		}
	    /*
	     * Not a string character - put it back
	     */
	    while (--stringchlen > 0)
		ungrabchar (stringch[stringchlen] & 0xFF);
	    ch = stringch[0] & 0xFF;
	    }
	if (ch == '\\')
	    {
	    lastch = ch = backch ();
	    yylval.charset.set[ch] = 1;
	    continue;
	    }
#ifdef NO8BIT
	if (ch & 0x80)
	    {
	    yyerror (PARSE_Y_8_BIT);
	    ch &= 0x7F;
	    }
#endif /* NO8BIT */
	if (ch == '-')			/* Handle a range */
	    {
	    if (lastch == -1)
		{
		lastch = ch = '-';	/* Not really a range */
		yylval.charset.set['-'] = 1;
		}
	    else
		{
		ch = grabchar ();
		/* [ */
		if (ch == EOF  ||  ch == ']')
		    {
		    lastch = ch = '-';	/* Not really range */
		    yylval.charset.set['-'] = 1;
		    if (ch != EOF)
			ungrabchar (ch);
		    }
		else
		    {
#ifdef NO8BIT
		    if (ch & 0x80)
			{
			yyerror (PARSE_Y_8_BIT);
			ch &= 0x7F;
			}
#endif /* NO8BIT */
		    if (ch == '\\')
			ch = backch ();
		    while (lastch <= ch)
			yylval.charset.set[lastch++] = 1;
		    lastch = -1;
		    }
		}
	    }
	else
	    {
	    lastch = ch;
	    yylval.charset.set[ch] = 1;
	    }
	}
    if (yylval.charset.complement)
	{
	for (ch = 0;  ch < SET_SIZE + MAXSTRINGCHARS;  ch++)
	    yylval.charset.set[ch] = !yylval.charset.set[ch];
	}
    }

static int backch ()			/* Process post-backslash characters */
    {
    register int	ch;		/* Next character read */
    register int	octval;		/* Budding octal value */

    ch = grabchar ();
    if (ch == EOF)
	return '\\';
    else if (ch >= '0'  &&  ch <= '7')
	{
	octval = ch - '0';
	ch = grabchar ();
	if (ch >= '0'  &&  ch <= '7')
	    {
	    octval = (octval << 3) + ch - '0';
	    ch = grabchar ();
	    if (ch >= '0'  &&  ch <= '7')
		octval = (octval << 3) + ch - '0';
	    else
		ungrabchar (ch);
	    }
	else if (ch != EOF)
	    ungrabchar (ch);
	ch = octval;
	}
    else if (ch == 'x')
	{
	ch = grabchar ();
	octval = 0;
	if ((ch >= '0'  &&  ch <= '9')
	  ||  (ch >= 'a'  &&  ch <= 'f')
	  ||  (ch >= 'A'  &&  ch <= 'F'))
	    {
	    if (ch >= '0'  &&  ch <= '9')
		octval = ch - '0';
	    else if (ch >= 'a'  &&  ch <= 'f')
		octval = ch - 'a' + 0xA;
	    else if (ch >= 'A'  &&  ch <= 'F')
		octval = ch - 'A' + 0xA;
	    ch = grabchar ();
	    octval <<= 4;
	    if (ch >= '0'  &&  ch <= '9')
		octval |= ch -'0';
	    else if (ch >= 'a'  &&  ch <= 'f')
		octval |= ch - 'a' + 0xA;
	    else if (ch >= 'A'  &&  ch <= 'F')
		octval |= ch - 'A' + 0xA;
	    else if (ch != EOF)
		{
		octval >>= 4;
		ungrabchar (ch);
		}
	    }
	else if (ch != EOF)
	    ungrabchar (ch);
	ch = octval;
	}
    else
	{
	switch (ch)
	    {
	    case 'n':
		ch = '\n';
		break;
	    case 'f':
		ch = '\f';
		break;
	    case 'r':
		ch = '\r';
		break;
	    case 'b':
		ch = '\b';
		break;
	    case 't':
		ch = '\t';
		break;
	    case 'v':
		ch = '\v';
		break;
	    }
	}
#ifdef NO8BIT
    if (ch & 0x80)
	{
	yyerror (PARSE_Y_8_BIT);
	ch &= 0x7F;
	}
#endif /* NO8BIT */
    return ch;
    }

static void yyerror (str)
    char *		str;	/* Error string */
    {
    (void) fflush (stdout);
    (void) fprintf (stderr, PARSE_Y_ERROR_FORMAT(fname, lineno, str));
    (void) fflush (stderr);
    }

int yyopen (file)
    register char *	file;	/* File name to be opened */
    {
    fname = malloc ((unsigned) strlen (file) + 1);
    if (fname == NULL)
	{
	(void) fprintf (stderr, PARSE_Y_MALLOC_TROUBLE);
	exit (1);
	}
    (void) strcpy (fname, file);
    aff_file = fopen (file, "r");
    if (aff_file == NULL)
	{
	(void) fprintf (stderr, CANT_OPEN, file);
	perror ("");
	return 1;
	}
    lineno = 1;
    return 0;
    }

void yyinit ()
    {
    register unsigned int i;	/* Loop counter */

    if (aff_file == NULL)
	aff_file = stdin;	/* Must be dynamically initialized on Amigas */
    for (i = 0;  i < SET_SIZE + MAXSTRINGCHARS;  i++)
	{
	hashheader.lowerconv[i] = (ichar_t) i;
	hashheader.upperconv[i] = (ichar_t) i;
	hashheader.wordchars[i] = 0;
	hashheader.lowerchars[i] = 0;
	hashheader.upperchars[i] = 0;
	hashheader.boundarychars[i] = 0;
	/*
	 * The default sort order is a big value so that there is room
	 * to insert "underneath" it.  In this way, special characters
	 * will sort last, but in ASCII order.
	 */
	hashheader.sortorder[i] = i + 1 + 2 * SET_SIZE;
	}
    for (i = 0;  i < SET_SIZE;  i++)
	hashheader.stringstarts[i] = 0;
    for (i = 0;  i < MAXSTRINGCHARS;  i++)
	{
	hashheader.stringdups[i] = i;
	hashheader.dupnos[i] = 0;
	}
    
    hashheader.sortval = 1;	/* This is so 0 can mean uninitialized */
    (void) bcopy (NRSPECIAL, hashheader.nrchars, sizeof hashheader.nrchars);
    (void) bcopy (TEXSPECIAL, hashheader.texchars, sizeof hashheader.texchars);
    hashheader.compoundflag = COMPOUND_NEVER; /* Dflt is report missing blks */
    hashheader.defhardflag = 0; /* Default is to try hard only if failures */
    hashheader.nstrchars = 0;	/* No string characters to start with */
    hashheader.flagmarker = '/'; /* Default flag marker is slash */
    hashheader.compoundmin = 3;	/* Dflt is at least 3 chars in cmpnd parts */
    hashheader.compoundbit = -1; /* Dflt is no compound bit */
    /* Set up magic numbers and compile options */
    hashheader.magic = hashheader.magic2 = MAGIC;
    hashheader.compileoptions = COMPILEOPTIONS;
    hashheader.maxstringchars = MAXSTRINGCHARS;
    hashheader.maxstringcharlen = MAXSTRINGCHARLEN;
    }

static int grabchar ()		/* Get a character and count lines */
    {
    int			ch;	/* Next input character */

    if (ungrablen > 0)
	ch = lexungrab[--ungrablen] & 0xFF;
    else
	ch = getc (aff_file);
    if (ch == '\n')
	lineno++;
    return ch;
    }

static void ungrabchar (ch)	/* Unget a character, tracking line numbers */
    int			ch;	/* Character to put back */
    {

    if (ch == '\n')
	lineno--;
    if (ch != EOF)
	{
	if (ungrablen == sizeof (lexungrab))
	    yyerror (PARSE_Y_UNGRAB_PROBLEM);
	else
	    lexungrab[ungrablen++] = (char) ch;
	}
    }

static int sufcmp (flag1, flag2)	/* Compare suffix flags for qsort */
    register struct flagent *	flag1;	/* Flags to be compared */
    register struct flagent *	flag2;	/* ... */
    {
    register ichar_t *		cp1;	/* Pointer into flag1's suffix */
    register ichar_t *		cp2;	/* Pointer into flag2's suffix */

    if (flag1->affl == 0  ||  flag2->affl == 0)
	return flag1->affl - flag2->affl;
    cp1 = flag1->affix + flag1->affl;
    cp2 = flag2->affix + flag2->affl;
    while (*--cp1 == *--cp2  &&  cp1 > flag1->affix  &&  cp2 > flag2->affix)
	;
    if (*cp1 == *cp2)
	{
	if (cp1 == flag1->affix)
	    {
	    if (cp2 == flag2->affix)
		return 0;
	    else
		return -1;
	    }
	else
	    return 1;
	}
    return *cp1 - *cp2;
    }

static int precmp (flag1, flag2)	/* Compare prefix flags for qsort */
    register struct flagent *	flag1;	/* Flags to be compared */
    register struct flagent *	flag2;	/* ... */
    {

    if (flag1->affl == 0  ||  flag2->affl == 0)
	return flag1->affl - flag2->affl;
    else
	return icharcmp (flag1->affix, flag2->affix);
    }

static int addstringchar (str, lower, upper) /* Add a string character */
    register unsigned char *	str;	/* String character to be added */
    int				lower;	/* NZ if a lower string */
    int				upper;	/* NZ if an upper string */
    {
    int				len;	/* Length of the string */
    register unsigned int	mslot;	/* Slot being moved or modified */
    register unsigned int	slot;	/* Where to put it */

    len = strlen ((char *) str);
    if (len > MAXSTRINGCHARLEN)
	{
	yyerror (PARSE_Y_LONG_STRING);
	}
    else if (len == 0)
	{
	yyerror (PARSE_Y_NULL_STRING);
	return -1;
	}
    else if (hashheader.nstrchars >= MAXSTRINGCHARS)
	{
	yyerror (PARSE_Y_MANY_STRINGS);
	return -1;
	}

    /*
     * Find where to put the new character
     */
    for (slot = 0;  slot < hashheader.nstrchars;  slot++)
	{
	if (stringcharcmp (&hashheader.stringchars[slot][0], (char *) str) > 0)
	    break;
	}
    /*
     * Fix all duplicate numbers to reflect the new slot.
     */
    for (mslot = 0;  mslot < hashheader.nstrchars;  mslot++)
	{
	if (hashheader.stringdups[mslot] >= slot)
	    hashheader.stringdups[mslot]++;
	}
    /*
     * Fix all characters before it so that their case conversion reflects
     * the new locations of the characters that will follow the new one.
     */
    slot += SET_SIZE;
    for (mslot = SET_SIZE;  mslot < slot;  mslot++)
	{
	if (hashheader.lowerconv[mslot] >= (ichar_t) slot)
	    hashheader.lowerconv[mslot]++;
	if (hashheader.upperconv[mslot] >= (ichar_t) slot)
	    hashheader.upperconv[mslot]++;
	}
    /*
     * Slide up all the other characters to make room for the new one, also
     * making the appropriate changes in the case-conversion tables.
     */
    for (mslot = hashheader.nstrchars + SET_SIZE;  --mslot >= slot;  )
	{
	(void) strcpy (&hashheader.stringchars[mslot + 1 - SET_SIZE][0],
	  &hashheader.stringchars[mslot - SET_SIZE][0]);
	hashheader.lowerchars[mslot + 1] = hashheader.lowerchars[mslot];
	hashheader.upperchars[mslot + 1] = hashheader.upperchars[mslot];
	hashheader.wordchars[mslot + 1] = hashheader.wordchars[mslot];
	hashheader.boundarychars[mslot + 1] = hashheader.boundarychars[mslot];
	if (hashheader.lowerconv[mslot] >= (ichar_t) slot)
	    hashheader.lowerconv[mslot]++;
	if (hashheader.upperconv[mslot] >= (ichar_t) slot)
	    hashheader.upperconv[mslot]++;
	hashheader.lowerconv[mslot + 1] = hashheader.lowerconv[mslot];
	hashheader.upperconv[mslot + 1] = hashheader.upperconv[mslot];
	hashheader.sortorder[mslot + 1] = hashheader.sortorder[mslot];
	hashheader.stringdups[mslot + 1 - SET_SIZE] =
	  hashheader.stringdups[mslot - SET_SIZE];
	hashheader.dupnos[mslot + 1 - SET_SIZE] =
	  hashheader.dupnos[mslot - SET_SIZE];
	}
    /*
     * Insert the new string character into the slot we made.  The
     * caller may choose to change the case-conversion field.
     */
    (void) strcpy (&hashheader.stringchars[slot - SET_SIZE][0], (char *) str);
    hashheader.lowerchars[slot] = (char) lower;
    hashheader.upperchars[slot] = (char) upper;
    hashheader.wordchars[slot] = 1;
    hashheader.boundarychars[slot] = 0;
    hashheader.sortorder[slot] = hashheader.sortval++;
    hashheader.lowerconv[slot] = (ichar_t) slot;
    hashheader.upperconv[slot] = (ichar_t) slot;
    hashheader.stringdups[slot - SET_SIZE] = slot - SET_SIZE;
    hashheader.dupnos[slot - SET_SIZE] = 0;
    /*
     * Add the first character of the string to the string-starts table, and
     * bump the count.
     */
    hashheader.stringstarts[str[0]] = 1;
    hashheader.nstrchars++;
    return slot;
    }

/*
 * This routine is a reimplemention of strcmp(), needed because the
 * idiots at Sun managed to screw up the implementation of strcmp on
 * Sun 4's (they used unsigned comparisons, even though characters
 * default to signed).  I hate hate HATE putting in this routine just
 * to support the stupidity of one programmer who ought to find a new
 * career digging ditches, but there are a lot of Sun 4's out there,
 * so I don't really have a lot of choice.
 */
static int stringcharcmp (a,  b)
    register char *		a;
    register char *		b;
    {

#ifdef NO8BIT
    while (*a != '\0')
	{
	if (((*a++ ^ *b++) & NOPARITY) != 0)
	    return (*--a & NOPARITY) - (*--b & NOPARITY);
	}
    return (*a & NOPARITY) - (*b & NOPARITY);
#else /* NO8BIT */
    while (*a != '\0')
	{
	if (*a++ != *b++)
	    return *--a - *--b;
	}
    return *a - *b;
#endif /* NO8BIT */
    }

#ifdef TBLDEBUG
static void tbldump (flagp, numflags)	/* Dump a flag table */
    register struct flagent *	flagp;	/* First flag entry to dump */
    register int		numflags; /* Number of flags to dump */
    {
    while (--numflags >= 0)
	entdump (flagp++);
    }

static void entdump (flagp)		/* Dump one flag entry */
    register struct flagent *	flagp;	/* Flag entry to dump */
    {
    register int		cond;	/* Condition number */

    (void) fprintf (stderr, "flag %s%c:\t",
      (flagp->flagflags & FF_CROSSPRODUCT) ? "*" : "",
      BITTOCHAR (flagp->flagbit));
    for (cond = 0;  cond < flagp->numconds;  cond++)
	{
	setdump (flagp->conds, 1 << cond);
	if (cond < flagp->numconds - 1)
	    (void) putc (' ', stderr);
	}
    if (cond == 0)			/* No conditions at all? */
	(void) putc ('.', stderr);
    (void) fprintf (stderr, "\t> ");
    (void) putc ('\t', stderr);
    if (flagp->stripl)
	(void) fprintf (stderr, "-%s,", ichartosstr (flagp->strip, 1));
    (void) fprintf (stderr, "%s\n",
      flagp->affl ? ichartosstr (flagp->affix, 1) : "-");
    }

static void setdump (setp, mask)	/* Dump a set specification */
    register char *		setp;	/* Set to be dumped */
    register int		mask;	/* Mask for bit to be dumped */
    {
    register int		cnum;	/* Next character's number */
    register int		firstnz; /* Number of first NZ character */
    register int		numnz;	/* Number of NZ characters */

    numnz = 0;
    for (cnum = SET_SIZE + hashheader.nstrchars;  --cnum >= 0;  )
	{
	if (setp[cnum] & mask)
	    {
	    numnz++;
	    firstnz = cnum;
	    }
	}
    if (numnz == 1)
	{
	if (cnum < SET_SIZE)
	    (void) putc (firstnz, stderr);
	else
	    (void) fputs (hashheader.stringchars[cnum - SET_SIZE], stderr);
	}
    else if (numnz == SET_SIZE)
	(void) putc ('.', stderr);
    else if (numnz > SET_SIZE / 2)
	{
	(void) fprintf (stderr, "[^");
	subsetdump (setp, mask, 0);
	(void) putc (']', stderr);
	}
    else
	{
	(void) putc ('[', stderr);
	subsetdump (setp, mask, mask);
	(void) putc (']', stderr);
	}
    }

static void subsetdump (setp, mask, dumpval) /* Dump part of a set spec */
    register char *		setp;	/* Set to be dumped */
    register int		mask;	/* Mask for bit to be dumped */
    register int		dumpval; /* Value to be printed */
    {
    register int		cnum;	/* Next character's number */
    register int		rangestart; /* Value starting a range */

    for (cnum = 0;  cnum < SET_SIZE;  setp++, cnum++)
	{
	if (((*setp ^ dumpval) & mask) == 0)
	    {
	    for (rangestart = cnum;  cnum < SET_SIZE;  setp++, cnum++)
		{
		if ((*setp ^ dumpval) & mask)
		    break;
		}
	    if (cnum == rangestart + 1)
		(void) putc (rangestart, stderr);
	    else if (cnum <= rangestart + 3)
		{
		while (rangestart < cnum)
		    {
		    (void) putc (rangestart, stderr);
		    rangestart++;
		    }
		}
	    else
		(void) fprintf (stderr, "%c-%c", rangestart, cnum - 1);
	    }
	}
    for (  ;  cnum < SET_SIZE + hashheader.nstrchars;  setp++, cnum++)
	{
	if (((*setp ^ dumpval) & mask) == 0)
	    (void) fputs (hashheader.stringchars[cnum - SET_SIZE], stderr);
	}
    }
#endif

