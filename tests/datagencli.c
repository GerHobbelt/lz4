/*
    datagencli.c
    compressible data command line generator
    Copyright (C) Yann Collet 2012-2020

    GPL v2 License

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    You can contact the author at :
   - LZ4 source repository : https://github.com/lz4/lz4
   - Public forum : https://groups.google.com/forum/#!forum/lz4c
*/


/**************************************
*  Includes
**************************************/
#include "util.h"      /* U32 */
#include <stdio.h>     /* fprintf, stderr */
#include "datagen.h"   /* RDG_generate */
#include "lz4.h"       /* LZ4_VERSION_STRING */


/**************************************
*  Compiler specific
**************************************/
#ifdef _MSC_VER    /* Visual Studio */
#pragma warning(disable : 4127)    /* disable: C4127: conditional expression is constant */
#define strtoull    _strtoui64  /* https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtoui64-wcstoui64-strtoui64-l-wcstoui64-l */
#endif


/**************************************
*  Constants
**************************************/
#define KB *(1 <<10)
#define MB *(1 <<20)
#define GB *(1U<<30)

#define SIZE_DEFAULT (64 KB)
#define SEED_DEFAULT 0
#define COMPRESSIBILITY_DEFAULT 50


/**************************************
*  Macros
**************************************/
#define DISPLAY(...)         fprintf(stderr, __VA_ARGS__)
#define DISPLAYLEVEL(l, ...) do { if (displayLevel>=(l)) DISPLAY(__VA_ARGS__); } while (0)
static unsigned displayLevel = 2;


/*********************************************************
*  Command line
*********************************************************/
static int usage(const char* programName)
{
    DISPLAY( "Compressible data generator\n");
    DISPLAY( "Usage :\n");
    DISPLAY( "      %s [size] [args]\n", programName);
    DISPLAY( "\n");
    DISPLAY( "Arguments :\n");
    DISPLAY( " -g#    : generate # data (default:%i)\n", SIZE_DEFAULT);
    DISPLAY( " -s#    : Select seed (default:%i)\n", SEED_DEFAULT);
    DISPLAY( " -P#    : Select compressibility in %% (default:%i%%)\n", COMPRESSIBILITY_DEFAULT);
    DISPLAY( " -h     : display help and exit\n");
    DISPLAY( "Special values :\n");
    DISPLAY( " -P0    : generate incompressible noise\n");
    DISPLAY( " -P100  : generate sparse files\n");
    return 0;
}




#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      lz4_datagen_example_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
    int argNb;
    double proba = (double)COMPRESSIBILITY_DEFAULT / 100;
    double litProba = 0.0;
    U64 size = SIZE_DEFAULT;
    U32 seed = SEED_DEFAULT;
    const char* programName;

    /* Check command line */
    programName = argv[0];
    for(argNb=1; argNb<argc; argNb++)
    {
        const char* argument = argv[argNb];

        if(!argument) continue;   /* Protection if argument empty */

        /* Handle commands. Aggregated commands are allowed */
        if (*argument=='-')
        {
            argument++;
            while (*argument!=0)
            {
                switch(*argument)
                {
                case 'h':
                    return usage(programName);
                case 'g':
                    argument++;
                    size = strtoull(argument, &argument, 10);
                    if (*argument=='K') { size <<= 10; argument++; }
                    if (*argument=='M') { size <<= 20; argument++; }
                    if (*argument=='G') { size <<= 30; argument++; }
                    if (*argument=='B') { argument++; }
                    break;
                case 's':
                    argument++;
                    seed = (U32) strtoul(argument, &argument, 10);
                    break;
                case 'P':
                    argument++;
                    proba = (double) strtoull(argument, &argument, 10);
                    proba /= 100.;
                    break;
                case 'L':   /* hidden argument : Literal distribution probability */
                    argument++;
                    litProba = (double) strtoull(argument, &argument, 10);
                    if (litProba>100.) litProba=100.;
                    litProba /= 100.;
                    break;
                case 'v':
                    displayLevel = 4;
                    argument++;
                    break;
                default:
                    return usage(programName);
                }
            }

        }
    }

    DISPLAYLEVEL(4, "Data Generator %s \n", LZ4_VERSION_STRING);
    DISPLAYLEVEL(3, "Seed = %u \n", seed);
    if (proba!=COMPRESSIBILITY_DEFAULT) DISPLAYLEVEL(3, "Compressibility : %i%%\n", (U32)(proba*100));

    RDG_genOut(size, proba, litProba, seed);
    DISPLAYLEVEL(1, "\n");

    return 0;
}
