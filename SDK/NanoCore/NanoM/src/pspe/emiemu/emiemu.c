/*  
    ---------------------------------------------------------------------------------------------------
    EMIEMU - Enhanced Mathematical Interface (emulated) 
    Copyright 2007-2012 Filippo Battaglia 
    ---------------------------------------------------------------------------------------------------
    Version for PSPE 
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

#include <nanodesktop.h>

/*
    ---------------------------------------------------------------------------
    Load16FloatsToMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a caricare 16 elementi di tipo float
    nella matrice NrMatrix. I 16 floats devono essere memorizzati
    a partire dall'indirizzo Data
    ---------------------------------------------------------------------------
*/ 
 

void ndEMIEMU_Load16FloatsToMatrix (int NrMatrix, float *Data) 
{                                                                   
    register int CounterX, CounterY;                                         
                                                                    
    for (CounterY=0; CounterY<4; CounterY++)                        
    {                                                               
        for (CounterX=0; CounterX<4; CounterX++)                    
        {                                                           
            XFPUMatrix [ NrMatrix ].value [CounterY][CounterX] = *Data;   
            Data++;                                                 
        }                                                           
    }                                                               
}


/*
   ---------------------------------------------------------------------------
   Load16IntsToMatrix
   ---------------------------------------------------------------------------
   Questa routine provvede a caricare 16 elementi di tipo int
   nella matrice NrMatrix. I 16 ints devono essere memorizzati
   a partire dall'indirizzo Data
   ---------------------------------------------------------------------------
*/ 

void ndEMIEMU_Load16IntsToMatrix (int NrMatrix, int *Data) \
{                                                                   
    register int CounterX, CounterY;                                         
                                                                    
    for (CounterY=0; CounterY<4; CounterY++)                        
    {                                                               
        for (CounterX=0; CounterX<4; CounterX++)                    
        {                                                           
            XFPUMatrix [ NrMatrix ].value [CounterY][CounterX] = (float)(*Data);   
            Data++;                                                 
        }                                                           
    }                                                               
}


/*
    ---------------------------------------------------------------------------
    Load16UCharsToMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a caricare 16 elementi di tipo uchar
    nella matrice NrMatrix. I 16 uchar devono essere memorizzati
    a partire dall'indirizzo Data
    ---------------------------------------------------------------------------
*/

void ndEMIEMU_Load16UCharsToMatrix (int NrMatrix, unsigned char *Data) 
{                                                                   
    register int CounterX, CounterY;                                         
                                                                    
    for (CounterY=0; CounterY<4; CounterY++)                        
    {                                                               
        for (CounterX=0; CounterX<4; CounterX++)                    
        {                                                           
            XFPUMatrix [ NrMatrix ].value [CounterY][CounterX] = (float)(*Data);   
            Data++;                                                 
        }                                                           
    }                                                               
}



/*
    ---------------------------------------------------------------------------
    Store16FloatsFromMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a memorizzare 16 elementi di tipo float
    dalla matrice NrMatrix. I 16 floats devono essere memorizzati
    a partire dall'indirizzo Data
    ---------------------------------------------------------------------------
*/

void ndEMIEMU_Store16FloatsFromMatrix (int NrMatrix, float *Data) 
{                                                                   
    register int CounterX, CounterY;                                         
                                                                    
    for (CounterY=0; CounterY<4; CounterY++)                        
    {                                                               
        for (CounterX=0; CounterX<4; CounterX++)                    
        {                                                           
            *Data = XFPUMatrix [ NrMatrix ].value [CounterY][CounterX]; 
            Data++;                                                 
        }                                                           
    }                                                               
}


/*
    ---------------------------------------------------------------------------
    Store16IntsFromMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a memorizzare 16 elementi di tipo int
    dalla matrice NrMatrix. I 16 ints devono essere memorizzati
    a partire dall'indirizzo Data
    ---------------------------------------------------------------------------
*/

void ndEMIEMU_Store16IntsFromMatrix (int NrMatrix, int *Data) 
{                                                                   
    register int CounterX, CounterY;                                         
                                                                    
    for (CounterY=0; CounterY<4; CounterY++)                        
    {                                                               
        for (CounterX=0; CounterX<4; CounterX++)                    
        {                                                           
            *Data = XFPUMatrix [ NrMatrix ].value [CounterY][CounterX]; 
            Data++;                                                 
        }                                                           
    }                                                               
}


/*
    ---------------------------------------------------------------------------
    Store16UCharsFromMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a memorizzare 16 elementi di tipo uchar
    dalla matrice NrMatrix. I 16 unsigned chars devono essere memorizzati
    a partire dall'indirizzo Data
    ---------------------------------------------------------------------------
*/

void ndEMIEMU_Store16UCharsFromMatrix (int NrMatrix, unsigned char *Data) 
{                                                                   
    register int CounterX, CounterY;                                         
                                                                    
    for (CounterY=0; CounterY<4; CounterY++)                        
    {                                                               
        for (CounterX=0; CounterX<4; CounterX++)                    
        {                                                           
            *Data = XFPUMatrix [ NrMatrix ].value [CounterY][CounterX]; 
            Data++;                                                 
        }                                                           
    }                                                               
}


/*
    ---------------------------------------------------------------------------
     NullMatrix
    ---------------------------------------------------------------------------
     Riempie di zeri la matrice 
     
            matrix0 [i;j] = 0   
    ---------------------------------------------------------------------------
*/

void ndEMIEMU_NullMatrix (int NrMatrix)                 
{                                                                                 
    register int   CounterX, CounterY;                                                                                                                    
                                                                                  
    for (CounterY=0; CounterY<4; CounterY++)                                      
    {                                                                             
        for (CounterX=0; CounterX<4; CounterX++)                                  
        {                                                                         
            XFPUMatrix [ NrMatrix ].value [CounterY][CounterX] = 0; 
        }                                                                         
    }                                                                             
}                                                                                   



/*
   ---------------------------------------------------------------------------
   MatrixSum
   ---------------------------------------------------------------------------
   Esegue la somma di due matrici secondo la formula
     
            MatrixDest [i;j] = matrix0 [i;j] + matrix1 [i;j]   
   ---------------------------------------------------------------------------
*/ 


void ndEMIEMU_MatrixSum (int NrMatrix0, int NrMatrix1, int NrMatrixDest)   
{                                                                                 
    register int   CounterX, CounterY;                                                     
                                                                                  
    for (CounterY=0; CounterY<4; CounterY++)                                      
    {                                                                             
        for (CounterX=0; CounterX<4; CounterX++)                                  
        {                                                                         
            XFPUMatrix [ NrMatrixDest ].value [CounterY][CounterX] = 
               XFPUMatrix [ NrMatrix0 ].value [CounterY][CounterX] + XFPUMatrix [ NrMatrix1 ].value [CounterY][CounterX]; 
        }                                                                         
    }                                                                             
}                                                                                 



/*
   ---------------------------------------------------------------------------
   MatrixSub
   ---------------------------------------------------------------------------
   Esegue la differenza di due matrici secondo la formula
     
            MatrixDest [i;j] = matrix0 [i;j] - matrix1 [i;j]   
   ---------------------------------------------------------------------------
*/ 


void ndEMIEMU_MatrixSub (int NrMatrix0, int NrMatrix1, int NrMatrixDest)   
{                                                                                 
    register int   CounterX, CounterY;                                                     
                                                                                  
    for (CounterY=0; CounterY<4; CounterY++)                                      
    {                                                                             
        for (CounterX=0; CounterX<4; CounterX++)                                  
        {                                                                         
            XFPUMatrix [ NrMatrixDest ].value [CounterY][CounterX] = 
               XFPUMatrix [ NrMatrix0 ].value [CounterY][CounterX] + XFPUMatrix [ NrMatrix1 ].value [CounterY][CounterX]; 
        }                                                                         
    }                                                                             
}   


/*
   ---------------------------------------------------------------------------
   MatrixTrvMul
   ---------------------------------------------------------------------------
   Esegue lo pseudoprodotto di due matrici secondo la formula
     
            MatrixDest [i;j] = matrix0 [i;j] * matrix1 [i;j]   
   ---------------------------------------------------------------------------
*/ 


void ndEMIEMU_MatrixTrvMul (int NrMatrix0, int NrMatrix1, int NrMatrixDest)   
{                                                                                 
    register int   CounterX, CounterY;                                                     
                                                                                  
    for (CounterY=0; CounterY<4; CounterY++)                                      
    {                                                                             
        for (CounterX=0; CounterX<4; CounterX++)                                  
        {                                                                         
            XFPUMatrix [ NrMatrixDest ].value [CounterY][CounterX] = 
               XFPUMatrix [ NrMatrix0 ].value [CounterY][CounterX] * XFPUMatrix [ NrMatrix1 ].value [CounterY][CounterX]; 
        }                                                                         
    }                                                                             
}                

/*
   ---------------------------------------------------------------------------
   MatrixTrvDiv
   ---------------------------------------------------------------------------
   Esegue la pseudodivisione di due matrici secondo la formula
     
            MatrixDest [i;j] = matrix0 [i;j] / matrix1 [i;j]   
   ---------------------------------------------------------------------------
*/ 


void ndEMIEMU_MatrixTrvDiv (int NrMatrix0, int NrMatrix1, int NrMatrixDest)   
{                                                                                 
    register int   CounterX, CounterY;                                                     
                                                                                  
    for (CounterY=0; CounterY<4; CounterY++)                                      
    {                                                                             
        for (CounterX=0; CounterX<4; CounterX++)                                  
        {                                                                         
            XFPUMatrix [ NrMatrixDest ].value [CounterY][CounterX] = 
               XFPUMatrix [ NrMatrix0 ].value [CounterY][CounterX] / XFPUMatrix [ NrMatrix1 ].value [CounterY][CounterX]; 
        }                                                                         
    }                                                                             
}                




/*
    ---------------------------------------------------------------------------
     MatrixCpy 
    ---------------------------------------------------------------------------
     Copia due matrici:
          
          MatrixDest [i;j] = MatrixSrc [i;j]
    ---------------------------------------------------------------------------
*/


void ndEMIEMU_MatrixCpy (int NrMatrixSrc, int NrMatrixDest)   
{                                                                                 
    register int   CounterX, CounterY;                                                     
                                                                                  
    for (CounterY=0; CounterY<4; CounterY++)                                      
    {                                                                             
        for (CounterX=0; CounterX<4; CounterX++)                                  
        {                                                                         
            XFPUMatrix [ NrMatrixDest ].value [CounterY][CounterX] = XFPUMatrix [ NrMatrixSrc ].value [CounterY][CounterX]; 
        }                                                                         
    }                                                                             
}                
                                                                 

/*
    ---------------------------------------------------------------------------
        SequentialAdder
    ---------------------------------------------------------------------------
        Questa routine è un sommatore sequenziale. E' in grado di eseguire somme di 16 
        elementi per volta. 
        
        Supponiamo che la matrice si possa vedere come un vettore di 4x4=16 elementi. 
        
        Per il primo elemento della matrice, il sistema preleva il float puntato da
        CarryIn e lo porta nel registro [0;0] della matrice NrMatrix0. 
        
        Dopo, esegue la somma del corrispondente elemento della matrice NrMatrix1 in 
        posizione [0;0] e memorizza il risultato nella matrice NrMatrix0 [0;0]. 
        
        In altre parole, per il primo elemento della matrice viene eseguita la
        formula
        
               Matrix0 [n] = *CarryIn + Matrix1 [n];
               
        Per gli altri elementi della matrice viene applicata invece la formula
        
               Matrix0 [n] = Matrix0 [n-1] + Matrix1 [n];
               
        L'ultimo elemento della Matrix0 viene memorizzato nell'indirizzo puntato da
        CarryOut 
    ---------------------------------------------------------------------------
*/


                                                                                    
void ndEMIEMU_SequentialAdder (int NrMatrix0, int NrMatrix1, float *CarryIn, float *CarryOut)   
{                                                                                    
                                                           
    XFPUMatrix [NrMatrix0].value [0][0] = *CarryIn + XFPUMatrix [NrMatrix1].value [0][0]; 
                                                                                          
    XFPUMatrix [NrMatrix0].value [0][1] = XFPUMatrix [NrMatrix0].value [0][0] + XFPUMatrix [NrMatrix1].value [0][1]; 
    XFPUMatrix [NrMatrix0].value [0][2] = XFPUMatrix [NrMatrix0].value [0][1] + XFPUMatrix [NrMatrix1].value [0][2]; 
    XFPUMatrix [NrMatrix0].value [0][3] = XFPUMatrix [NrMatrix0].value [0][2] + XFPUMatrix [NrMatrix1].value [0][3]; 
                                                                                                                     
    XFPUMatrix [NrMatrix0].value [1][0] = XFPUMatrix [NrMatrix0].value [0][3] + XFPUMatrix [NrMatrix1].value [1][0]; 
    XFPUMatrix [NrMatrix0].value [1][1] = XFPUMatrix [NrMatrix0].value [1][0] + XFPUMatrix [NrMatrix1].value [1][1]; 
    XFPUMatrix [NrMatrix0].value [1][2] = XFPUMatrix [NrMatrix0].value [1][1] + XFPUMatrix [NrMatrix1].value [1][2]; 
    XFPUMatrix [NrMatrix0].value [1][3] = XFPUMatrix [NrMatrix0].value [1][2] + XFPUMatrix [NrMatrix1].value [1][3]; 
                                                                                                                     
    XFPUMatrix [NrMatrix0].value [2][0] = XFPUMatrix [NrMatrix0].value [1][3] + XFPUMatrix [NrMatrix1].value [2][0]; 
    XFPUMatrix [NrMatrix0].value [2][1] = XFPUMatrix [NrMatrix0].value [2][0] + XFPUMatrix [NrMatrix1].value [2][1]; 
    XFPUMatrix [NrMatrix0].value [2][2] = XFPUMatrix [NrMatrix0].value [2][1] + XFPUMatrix [NrMatrix1].value [2][2]; 
    XFPUMatrix [NrMatrix0].value [2][3] = XFPUMatrix [NrMatrix0].value [2][2] + XFPUMatrix [NrMatrix1].value [2][3]; 
                                                                                                                     
    XFPUMatrix [NrMatrix0].value [3][0] = XFPUMatrix [NrMatrix0].value [2][3] + XFPUMatrix [NrMatrix1].value [3][0]; 
    XFPUMatrix [NrMatrix0].value [3][1] = XFPUMatrix [NrMatrix0].value [3][0] + XFPUMatrix [NrMatrix1].value [3][1]; 
    XFPUMatrix [NrMatrix0].value [3][2] = XFPUMatrix [NrMatrix0].value [3][1] + XFPUMatrix [NrMatrix1].value [3][2]; 
    XFPUMatrix [NrMatrix0].value [3][3] = XFPUMatrix [NrMatrix0].value [3][2] + XFPUMatrix [NrMatrix1].value [3][3]; 
                                                                                                                     
    *CarryOut = XFPUMatrix [NrMatrix0].value [3][3];     
}
                                                                             
