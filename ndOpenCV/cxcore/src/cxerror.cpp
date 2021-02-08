/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//            Intel License Agreement
//        For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/


#include "_cxcore.h"



typedef struct
{
    const char* file;
    int         line;
}
CvStackRecord;

typedef struct CvContext
{
    int  err_code;
    int  err_mode;
    CvErrorCallback error_callback;
    void*  userdata;
    char  err_msg[4096];
    CvStackRecord  err_ctx;
} CvContext;



#define CV_DEFAULT_ERROR_CALLBACK  cvNDGuiBoxReport

static CvContext*
icvCreateContext(void)
{
    CvContext *context;   

    context = (CvContext *)malloc( sizeof(*context) );

    context->err_mode = CV_ErrModeLeaf;
    context->err_code = CV_StsOk;

    context->error_callback = CV_DEFAULT_ERROR_CALLBACK;
    context->userdata = 0;

    return context;
}

static void
icvDestroyContext(CvContext* context)
{
    free (context);
}


static CvContext*
icvGetContext(void)
{
// static single-thread library case 
    static CvContext *context;
    
    if( !context )
        context = icvCreateContext();

    return context;
}


CV_IMPL int
cvStdErrReport( int code, const char *func_name, const char *err_msg,
                const char *file, int line, void* )
{
    if( code == CV_StsBackTrace || code == CV_StsAutoTrace )
        fprintf( stderr, "\tcalled from " );
    else
        fprintf( stderr, "OpenCV ERROR: %s (%s)\n\tin function ",
                 cvErrorStr(code), err_msg ? err_msg : "no description" );

    fprintf( stderr, "%s, %s(%d)\n", func_name ? func_name : "<unknown>",
             file != NULL ? file : "", line );

    if( cvGetErrMode() == CV_ErrModeLeaf )
    {
        fprintf( stderr, "Terminating the application...\n" );
        return 1;
    }
    else
        return 0;
}




CV_IMPL int
cvNDGuiBoxReport ( int code, const char *func_name, const char *err_msg,
                    const char *file, int line, void* )
{
    unsigned char WndHeader;

    unsigned short int PosX1, PosY1, PosX2, PosY2;
    unsigned short int B1_Y1, B1_Y2;
    
    PosX1=(Windows_MaxScreenX - __CVERRORBOX_LENX)/2;
    PosY1=(Windows_MaxScreenY - __CVERRORBOX_LENY)/2;
    PosX2=(PosX1 + __CVERRORBOX_LENX);
    PosY2=(PosY1 + __CVERRORBOX_LENY);

    WndHeader=ndLP_CreateWindow (PosX1, PosY1, PosX2, PosY2, "OPENCV ERROR", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);
    ndLP_MaximizeWindow (WndHeader);

    if( code == CV_StsBackTrace || code == CV_StsAutoTrace )
    {
        ndWS_WriteLn ("\tcalled from ", 0x0000, WndHeader, NORENDER);
    }
    else
    {
        ndWS_WriteLn ("Internal error:", 0x0000, WndHeader, NORENDER);
        ndWS_WriteLn ( (char *) (cvErrorStr(code)) , 0x0000, WndHeader, NORENDER);
        ndWS_CarriageReturn (WndHeader);
        ndWS_WriteLn ( (char *) (err_msg), 0x0000, WndHeader, NORENDER);
    }

    ndWS_CarriageReturn (WndHeader);
    ndWS_WriteLn ( (char *) (func_name), 0x0000, WndHeader, NORENDER);
    ndWS_WriteLn ( (char *) (file), 0x0000, WndHeader, NORENDER);
    ndWS_CarriageReturn (WndHeader);


    if( cvGetErrMode() == CV_ErrModeLeaf )
    {
        ndWS_WriteLn ("Terminating the application...", 0x0000, WndHeader, RENDER );
    }
    
    B1_Y1 = WindowData [WndHeader].WSWndHeight - 25;
    B1_Y2 = WindowData [WndHeader].WSWndHeight - 5;

    ndCTRL_MakeOneButton ("OK", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHeader, NOAUTOCLOSE);
    // Consenti all'utente di premere il tasto OK
    
    ndLP_DestroyWindow (WndHeader);
    
   if( cvGetErrMode() == CV_ErrModeLeaf )
     return 1;
   else
     return 0;

}




CV_IMPL int cvNulDevReport( int /*code*/, const char* /*func_name*/,
    const char* /*err_msg*/, const char* /*file*/, int /*line*/, void* )
{
    return cvGetErrMode() == CV_ErrModeLeaf;
}


CV_IMPL CvErrorCallback
cvRedirectError( CvErrorCallback func, void* userdata, void** prev_userdata )
{
    CvContext* context = icvGetContext();

    CvErrorCallback old = context->error_callback;

    if( prev_userdata )
        *prev_userdata = context->userdata;

    if( func )
    {
        context->error_callback = func;
        context->userdata = userdata;
    }
    else
    {
        context->error_callback = CV_DEFAULT_ERROR_CALLBACK;
        context->userdata = 0;
    }

    return old;
}


CV_IMPL int cvGetErrInfo( const char** errorcode_desc, const char** description,
                          const char** filename, int* line )
{
    int code = cvGetErrStatus();

    if( errorcode_desc )
        *errorcode_desc = cvErrorStr( code );

    if( code >= 0 )
    {
        if( description )
            *description = 0;
        if( filename )
            *filename = 0;
        if( line )
            *line = 0;
    }
    else
    {
        CvContext* ctx = icvGetContext();

        if( description )
            *description = ctx->err_msg;
        if( filename )
            *filename = ctx->err_ctx.file;
        if( line )
            *line = ctx->err_ctx.line;
    }

    return code;
}


CV_IMPL const char* cvErrorStr( int status )
{
    static char buf[256];

    switch (status)
    {
    case CV_StsOk :        return "No Error";
    case CV_StsBackTrace : return "Backtrace";
    case CV_StsError :     return "Unspecified error";
    case CV_StsInternal :  return "Internal error";
    case CV_StsNoMem :     return "Insufficient memory";
    case CV_StsBadArg :    return "Bad argument";
    case CV_StsNoConv :    return "Iterations do not converge";
    case CV_StsAutoTrace : return "Autotrace call";
    case CV_StsBadSize :   return "Incorrect size of input array";
    case CV_StsNullPtr :   return "Null pointer";
    case CV_StsDivByZero : return "Divizion by zero occured";
    case CV_BadStep :      return "Image step is wrong";
    case CV_StsInplaceNotSupported : return "Inplace operation is not supported";
    case CV_StsObjectNotFound :      return "Requested object was not found";
    case CV_BadDepth :     return "Input image depth is not supported by function";
    case CV_StsUnmatchedFormats : return "Formats of input arguments do not match";
    case CV_StsUnmatchedSizes :  return "Sizes of input arguments do not match";
    case CV_StsOutOfRange : return "One of arguments\' values is out of range";
    case CV_StsUnsupportedFormat : return "Unsupported format or combination of formats";
    case CV_BadCOI :      return "Input COI is not supported";
    case CV_BadNumChannels : return "Bad number of channels";
    case CV_StsBadFlag :   return "Bad flag (parameter or structure field)";
    case CV_StsBadPoint :  return "Bad parameter of type CvPoint";
    case CV_StsBadMask : return "Bad type of mask argument";
    case CV_StsParseError : return "Parsing error";
    case CV_StsNotImplemented : return "The function/feature is not implemented";
    case CV_StsBadMemBlock :  return "Memory block has been corrupted";
    };

    sprintf(buf, "Unknown %s code %d", status >= 0 ? "status":"error", status);
    return buf;
}

CV_IMPL int cvGetErrMode(void)
{
    return icvGetContext()->err_mode;
}

CV_IMPL int cvSetErrMode( int mode )
{
    CvContext* context = icvGetContext();
    int prev_mode = context->err_mode;
    context->err_mode = mode;
    return prev_mode;
}

CV_IMPL int cvGetErrStatus()
{
    return icvGetContext()->err_code;
}

CV_IMPL void cvSetErrStatus( int code )
{
    icvGetContext()->err_code = code;
}


CV_IMPL void cvError( int code, const char* func_name,
                      const char* err_msg,
                      const char* file_name, int line )
{
    if( code == CV_StsOk )
        cvSetErrStatus( code );
    else
    {
        CvContext *context = icvGetContext();

        if( code != CV_StsBackTrace && code != CV_StsAutoTrace )
        {
            char* message = context->err_msg;
            context->err_code = code;

            strcpy( message, err_msg );
            context->err_ctx.file = file_name;
            context->err_ctx.line = line;
        }

        if( context->err_mode != CV_ErrModeSilent )
        {
            int terminate = context->error_callback( code, func_name, err_msg,
                                                    file_name, line, context->userdata );
            if( terminate )
                exit(-abs(terminate));
        }

    }
}


/******************** End of implementation of profiling stuff *********************/


/**********************DllMain********************************/


/* function, which converts int to int */
CV_IMPL int
cvErrorFromIppStatus( int status )
{
    switch (status)
    {
    case CV_BADSIZE_ERR: return CV_StsBadSize;
    case CV_BADMEMBLOCK_ERR: return CV_StsBadMemBlock;
    case CV_NULLPTR_ERR: return CV_StsNullPtr;
    case CV_DIV_BY_ZERO_ERR: return CV_StsDivByZero;
    case CV_BADSTEP_ERR: return CV_BadStep ;
    case CV_OUTOFMEM_ERR: return CV_StsNoMem;
    case CV_BADARG_ERR: return CV_StsBadArg;
    case CV_NOTDEFINED_ERR: return CV_StsError;
    case CV_INPLACE_NOT_SUPPORTED_ERR: return CV_StsInplaceNotSupported;
    case CV_NOTFOUND_ERR: return CV_StsObjectNotFound;
    case CV_BADCONVERGENCE_ERR: return CV_StsNoConv;
    case CV_BADDEPTH_ERR: return CV_BadDepth;
    case CV_UNMATCHED_FORMATS_ERR: return CV_StsUnmatchedFormats;
    case CV_UNSUPPORTED_COI_ERR: return CV_BadCOI;
    case CV_UNSUPPORTED_CHANNELS_ERR: return CV_BadNumChannels;
    case CV_BADFLAG_ERR: return CV_StsBadFlag;
    case CV_BADRANGE_ERR: return CV_StsBadArg;
    case CV_BADCOEF_ERR: return CV_StsBadArg;
    case CV_BADFACTOR_ERR: return CV_StsBadArg;
    case CV_BADPOINT_ERR: return CV_StsBadPoint;

    default: return CV_StsError;
    }
}
/* End of file */


