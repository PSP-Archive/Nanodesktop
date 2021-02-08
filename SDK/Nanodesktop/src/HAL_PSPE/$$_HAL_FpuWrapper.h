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
    FILE SPECIALE: WRAPPER FUNZIONI FPU (PSPE)
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
        Sotto PSPE, non è disponibile emulazione della VFPU e così le 
        funzioni vengono rimappate sulle funzioni matematiche 
        standard della NanoM
    */
    
    #define ndHAL_XFPU_sinf   sinf
    #define ndHAL_XFPU_cosf   cosf
    #define ndHAL_XFPU_tanf   tanf
    #define ndHAL_XFPU_asinf  asinf
    #define ndHAL_XFPU_acosf  acosf
    #define ndHAL_XFPU_atanf  atanf
    #define ndHAL_XFPU_atan2f atan2f
    #define ndHAL_XFPU_sinhf  sinhf
    #define ndHAL_XFPU_coshf  coshf
    #define ndHAL_XFPU_tanhf  tanhf
    #define ndHAL_XFPU_sincos sincos
    #define ndHAL_XFPU_expf   expf
    #define ndHAL_XFPU_logf   logf
    #define ndHAL_XFPU_fabsf  fabsf
    #define ndHAL_XFPU_sqrtf  sqrtf
    #define ndHAL_XFPU_powf   powf
    #define ndHAL_XFPU_fmodf  fmodf
    #define ndHAL_XFPU_fminf  fminf
    #define ndHAL_XFPU_fmaxf  fmaxf
    

/*
    ---------------------------------------------------------------------------
    FUNZIONI DI GESTIONE DELLE MATRICI
    ---------------------------------------------------------------------------
     Sotto PSPE le funzioni ndHAL_XFPU per la gestione delle matrici
     vengono reindirizzate dal macro wrapper verso le funzionalità software
     EmiEmu che emulano via software il comportamento dell'interfaccia EMI
     (EmiEmu è in NanoM).
    ---------------------------------------------------------------------------  
*/
     
// Load

#define ndHAL_XFPU_Load16FloatsToMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Load16FloatsToMatrix ( (_mpNrMatrix_), (_mpData_) ); \
                                                                  \
                                                                  
#define ndHAL_XFPU_Load16IntsToMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Load16IntsToMatrix ( (_mpNrMatrix_), (_mpData_) ); \
                                                              \
                                                                   
#define ndHAL_XFPU_Load16UCharsToMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Load16UCharsToMatrix ( (_mpNrMatrix_), (_mpData_) ); \
                                                                \
// Store                                                                

#define ndHAL_XFPU_Store16FloatsFromMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Store16FloatsFromMatrix ( (_mpNrMatrix_), (_mpData_) ); \
                                                                  \
                                                                  
#define ndHAL_XFPU_Store16IntsFromMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Store16IntsFromMatrix ( (_mpNrMatrix_), (_mpData_) ); \
                                                                 \
                                                                 
#define ndHAL_XFPU_Store16UCharsFromMatrix(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Store16UCharsFromMatrix ( (_mpNrMatrix_), (_mpData_) ); \
                                                                 \
// Store (Destructive)

#define ndHAL_XFPU_Store16FloatsFromMatrixDestr(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Store16FloatsFromMatrix ( (_mpNrMatrix_), (_mpData_) );\
                                                                  \
                                                                  
#define ndHAL_XFPU_Store16IntsFromMatrixDestr(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Store16IntsFromMatrix ( (_mpNrMatrix_), (_mpData_) );\
                                                                 \
                                                                 
#define ndHAL_XFPU_Store16UCharsFromMatrixDestr(_mpNrMatrix_,_mpData_)  \
        ndEMIEMU_Store16UCharsFromMatrix ( (_mpNrMatrix_), (_mpData_) );\
                                                                 \
                                                                 
// NullMatrix

#define ndHAL_XFPU_NullMatrix(_mpNrMatrix_)  \
        ndEMIEMU_NullMatrix( (_mpNrMatrix_) ) ;\
                                                                 \
 
// MatrixSum

#define ndHAL_XFPU_MatrixSum(_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)  \
        ndEMIEMU_MatrixSum( (_mpNrMatrix0_), (_mpNrMatrix1_), (_mpNrMatrixDest_) ); \
        
                                                                  
                                                                    
// MatrixSub

#define ndHAL_XFPU_MatrixSub(_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)  \
        ndEMIEMU_MatrixSub( (_mpNrMatrix0_), (_mpNrMatrix1_), (_mpNrMatrixDest_) );\
                                                                 \
                                                                 
// MatrixTrvMul

#define ndHAL_XFPU_MatrixTrvMul(_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)  \
        ndEMIEMU_MatrixTrvMul( (_mpNrMatrix0_), (_mpNrMatrix1_), (_mpNrMatrixDest_) );\
  

// MatrixTrvDiv

#define ndHAL_XFPU_MatrixTrvDiv(_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)  \
        ndEMIEMU_MatrixTrvDiv( (_mpNrMatrix0_), (_mpNrMatrix1_), (_mpNrMatrixDest_) );\
  
                                                                                                                                  
          
// MatrixCpy

#define ndHAL_XFPU_MatrixCpy(_mpNrMatrixSrc_,_mpNrMatrixDest_)  \
        ndEMIEMU_MatrixCpy( (_mpNrMatrixSrc_), (_mpNrMatrixDest_) );\
                                                                   
                                                                 
// SequentialAdder

#define ndHAL_XFPU_SequentialAdder(_mpNrMatrix0_, _mpNrMatrix1_, _mpCarryIn_, _mpCarryOut_)           \
        ndEMIEMU_SequentialAdder( (_mpNrMatrix0_), (_mpNrMatrix1_), (_mpCarryIn_), (_mpCarryOut_) );     \



                                                                  


