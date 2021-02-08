/*  
    ---------------------------------------------------------------------------------------------------
    EMIEMU - Enhanced Mathematical Interface (emulated) 
    Copyright 2007-2012 Filippo Battaglia 
    ---------------------------------------------------------------------------------------------------
    Version for PSPE (file header)
    ---------------------------------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
        
    By downloading, copying, installing or using the software you agree to this license.
    If you do not agree to this license, do not download, install,
    copy or use the software.
    
    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    
      * Redistribution's of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
    
      * Redistribution's in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
    
      * The name of the Visilab research center may not be used to endorse or promote products
        derived from this software without specific prior written permission.
    
    This software is provided by the copyright holders and contributors "as is" and
    any express or implied warranties, including, but not limited to, the implied
    warranties of merchantability and fitness for a particular purpose are disclaimed.
    In no event shall the author of the software, the Visilab Research Center or 
    contributors be liable for any direct, indirect, incidental, special, exemplary, 
    or consequential damages (including, but not limited to, procurement of substitute 
    goods or services; loss of use, data, or profits; or business interruption) 
    however caused and on any theory of liability, whether in contract, strict liability,
    or tort (including negligence or otherwise) arising in any way out of
    the use of this software, even if advised of the possibility of such damage.
    ------------------------------------------------------------------------------------------------------
    
    PSPE emulator doesn't support VFPU emulation, so EMI is emulated via software.
    This library provides a set of routines that helps to emulate EMI. 
    ------------------------------------------------------------------------------------------------------
*/

#ifndef _EMIEMU_H
    #define _EMIEMU_H

        #ifdef __cplusplus
        extern "C" {
        #endif

        /* ./emiemu/emiemu.c:58:NF  */ extern void ndEMIEMU_Load16FloatsToMatrix (int NrMatrix, float *Data); /* (NrMatrix, Data) int NrMatrix; float *Data; */
        /* ./emiemu/emiemu.c:82:NF  */ extern void ndEMIEMU_Load16IntsToMatrix (int NrMatrix, int *Data); /* (NrMatrix, Data) int NrMatrix; int *Data; */
        /* ./emiemu/emiemu.c:108:NF */ extern void ndEMIEMU_Load16UCharsToMatrix (int NrMatrix, unsigned char *Data); /* (NrMatrix, Data) int NrMatrix; unsigned char *Data; */
        /* ./emiemu/emiemu.c:134:NF */ extern void ndEMIEMU_Store16FloatsFromMatrix (int NrMatrix, float *Data); /* (NrMatrix, Data) int NrMatrix; float *Data; */
        /* ./emiemu/emiemu.c:159:NF */ extern void ndEMIEMU_Store16IntsFromMatrix (int NrMatrix, int *Data); /* (NrMatrix, Data) int NrMatrix; int *Data; */
        /* ./emiemu/emiemu.c:184:NF */ extern void ndEMIEMU_Store16UCharsFromMatrix (int NrMatrix, unsigned char *Data); /* (NrMatrix, Data) int NrMatrix; unsigned char *Data; */
        /* ./emiemu/emiemu.c:209:NF */ extern void ndEMIEMU_NullMatrix (int NrMatrix); /* (NrMatrix) int NrMatrix; */
        /* ./emiemu/emiemu.c:235:NF */ extern void ndEMIEMU_MatrixSum (int NrMatrix0, int NrMatrix1, int NrMatrixDest); /* (NrMatrix0, NrMatrix1, NrMatrixDest) int NrMatrix0; int NrMatrix1; int NrMatrixDest; */
        /* ./emiemu/emiemu.c:262:NF */ extern void ndEMIEMU_MatrixSub (int NrMatrix0, int NrMatrix1, int NrMatrixDest); /* (NrMatrix0, NrMatrix1, NrMatrixDest) int NrMatrix0; int NrMatrix1; int NrMatrixDest; */
        /* ./emiemu/emiemu.c:288:NF */ extern void ndEMIEMU_MatrixTrvMul (int NrMatrix0, int NrMatrix1, int NrMatrixDest); /* (NrMatrix0, NrMatrix1, NrMatrixDest) int NrMatrix0; int NrMatrix1; int NrMatrixDest; */
        /* ./emiemu/emiemu.c:288:NF */ extern void ndEMIEMU_MatrixTrvDiv (int NrMatrix0, int NrMatrix1, int NrMatrixDest); /* (NrMatrix0, NrMatrix1, NrMatrixDest) int NrMatrix0; int NrMatrix1; int NrMatrixDest; */
        /* ./emiemu/emiemu.c:314:NF */ extern void ndEMIEMU_MatrixCpy (int NrMatrixSrc, int NrMatrixDest); /* (NrMatrixSrc, NrMatrixDest) int NrMatrixSrc; int NrMatrixDest; */
        /* ./emiemu/emiemu.c:359:NF */ extern void ndEMIEMU_SequentialAdder (int NrMatrix0, int NrMatrix1, float *CarryIn, float *CarryOut); /* (NrMatrix0, NrMatrix1, CarryIn, CarryOut) int NrMatrix0; int NrMatrix1; float *CarryIn; float *CarryOut; */

        #ifdef __cplusplus
        }
        #endif
        
   #endif    /* _EMI_H */
