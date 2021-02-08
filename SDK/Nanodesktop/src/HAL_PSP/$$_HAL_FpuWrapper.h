/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2008-2013 Filippo Battaglia 
    ----------------------------------------------------------------------------
    Version for PSP/PSPE
    ----------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

    By downloading, copying, installing or using the software you agree to 
    Nanodesktop license, a modified version of BSD license. Read the file
    license.doc in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    ---------------------------------------------------------------------------
    FILE SPECIALE: WRAPPER FUNZIONI FPU (PSP)
    ---------------------------------------------------------------------------
    Questo file si occupa di creare dei macro wrappers per l'utilizzo della
    VFPU/FPU della piattaforma utilizzata. 
    
    Scopo del macro wrapper è fare in modo che l'utente possa accedere alle
    funzioni VFPU per mezzo di routines di tipo ndHAL_XFPU_ senza doversi
    preoccupare di come realmente tali funzioni matematiche sono 
    implementate
    ---------------------------------------------------------------------------
*/

/*
    ---------------------------------------------------------------------------
    FUNZIONI MATEMATICHE
    ---------------------------------------------------------------------------
*/

    /* 
        Sotto PSP (o PSP KSU), le funzioni matematiche e trigonometriche
        vengono rimappate sulle funzioni vfpu in modo che sia 
        possibile per l'utente usare le funzionalità del 
        coprocessore matematico della PSP
    */
    
    
    #define ndHAL_XFPU_sinf   (double)(vfpu_sinf)
    #define ndHAL_XFPU_cosf   (double)(vfpu_cosf)
    #define ndHAL_XFPU_tanf   (double)(vfpu_tanf)
    #define ndHAL_XFPU_asinf  (double)(vfpu_asinf)
    #define ndHAL_XFPU_acosf  (double)(vfpu_acosf)
    #define ndHAL_XFPU_atanf  (double)(vfpu_atanf)
    #define ndHAL_XFPU_atan2f (double)(vfpu_atan2f)
    #define ndHAL_XFPU_sinhf  (double)(vfpu_sinhf)
    #define ndHAL_XFPU_coshf  (double)(vfpu_coshf)
    #define ndHAL_XFPU_tanhf  (double)(vfpu_tanhf)
    #define ndHAL_XFPU_sincos (double)(vfpu_sincos)
    #define ndHAL_XFPU_expf   (double)(vfpu_expf)
    #define ndHAL_XFPU_logf   (double)(vfpu_logf)
    #define ndHAL_XFPU_fabsf  (double)(vfpu_fabsf)
    #define ndHAL_XFPU_sqrtf  sqrtf                   
    #define ndHAL_XFPU_powf   powf
    #define ndHAL_XFPU_fmodf  (double)(vfpu_fmodf)
    #define ndHAL_XFPU_fminf  (double)(vfpu_fminf)
    #define ndHAL_XFPU_fmaxf  (double)(vfpu_fmaxf)
    
    
    
/*
    ---------------------------------------------------------------------------
    FUNZIONI DI GESTIONE DELLE MATRICI
    ---------------------------------------------------------------------------
     Sotto PSP/PSP KSU le funzioni ndHAL_XFPU per la gestione delle matrici
     vengono reindirizzate dal macro wrapper verso le funzionalità Emi
     equivalenti (Emi è in NanoM)
    ---------------------------------------------------------------------------  
*/
     
// Load

#define ndHAL_XFPU_Load16FloatsToMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMI_Load16FloatsToMatrix_m##_mpNrMatrix_ ( (_mpData_) );\
                                                                  \
                                                                  
#define ndHAL_XFPU_Load16IntsToMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMI_Load16IntsToMatrix_m##_mpNrMatrix_ ( (_mpData_) );\
                                                              \
                                                                   
#define ndHAL_XFPU_Load16UCharsToMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMI_Load16UCharsToMatrix_m##_mpNrMatrix_ ( (_mpData_) );\
                                                                \
// Store                                                                

#define ndHAL_XFPU_Store16FloatsFromMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMI_Store16FloatsFromMatrix_m##_mpNrMatrix_ ( (_mpData_) );\
                                                                  \
                                                                  
#define ndHAL_XFPU_Store16IntsFromMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMI_Store16IntsFromMatrix_m##_mpNrMatrix_ ( (_mpData_) );\
                                                                 \
                                                                 
#define ndHAL_XFPU_Store16UCharsFromMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMI_Store16UCharsFromMatrix_m##_mpNrMatrix_ ( (_mpData_) );\
                                                                 \
// Store (Destructive)

#define ndHAL_XFPU_Store16FloatsFromMatrixDestr(_mpNrMatrix_,_mpData_)  \
        ndEMI_Store16FloatsFromMatrixDestr_m##_mpNrMatrix_ ( (_mpData_) );\
                                                                  \
                                                                  
#define ndHAL_XFPU_Store16IntsFromMatrixDestr(_mpNrMatrix_,_mpData_)  \
        ndEMI_Store16IntsFromMatrixDestr_m##_mpNrMatrix_ ( (_mpData_) );\
                                                                 \
                                                                 
#define ndHAL_XFPU_Store16UCharsFromMatrixDestr(_mpNrMatrix_,_mpData_)  \
        ndEMI_Store16UCharsFromMatrixDestr_m##_mpNrMatrix_ ( (_mpData_) );\
                                                                 \
                                                                 
// NullMatrix

#define ndHAL_XFPU_NullMatrix(_mpNrMatrix_)  \
        ndEMI_NullMatrix_m##_mpNrMatrix_ () );\
                                                                 \
 
// MatrixSum

#define ndHAL_XFPU_MatrixSum(_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)  \
        ndEMI_MatrixSum_m##_mpNrMatrix0_##_m##_mpNrMatrix1_##_to_m##_mpNrMatrixDest_ ();\
                                                                  
                                                                    
// MatrixSub

#define ndHAL_XFPU_MatrixSub(_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)  \
        ndEMI_MatrixSub_m##_mpNrMatrix0_##_m##_mpNrMatrix1_##_to_m##_mpNrMatrixDest_ ();\
                                                                 \
                                                                 
// MatrixTrvMul

#define ndHAL_XFPU_MatrixTrvMul(_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)  \
        ndEMI_MatrixTrvMul_m##_mpNrMatrix0_##_m##_mpNrMatrix1_##_to_m##_mpNrMatrixDest_ ();\

// MatrixTrvDiv

#define ndHAL_XFPU_MatrixTrvDiv(_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)  \
        ndEMI_MatrixTrvDiv_m##_mpNrMatrix0_##_m##_mpNrMatrix1_##_to_m##_mpNrMatrixDest_ ();\
                                                                                                                                            
// MatrixCpy

#define ndHAL_XFPU_MatrixCpy(_mpNrMatrixSrc_,_mpNrMatrixDest_)  \
        ndEMI_MatrixCpy_m##_mpMatrixSrc_##_to_m##_mpMatrixDest_ ();\
                                                                   
                                                                 
// SequentialAdder

#define ndHAL_XFPU_SequentialAdder(_mpNrMatrix0_, _mpNrMatrix1_, _mpCarryIn_, _mpCarryOut_)           \
        ndEMI_SequentialAdder_m##_mpNrMatrix0_##_m##_mpNrMatrix1_ ( (_mpCarryIn_), (_mpCarryOut_) );     \



                                                                  

