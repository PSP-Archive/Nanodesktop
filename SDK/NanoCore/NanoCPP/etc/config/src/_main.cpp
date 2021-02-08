#include <nanodesktop.h>

#include "cfgprintf.h"

FILE *config_file_hdr;


int SIZET_main ();
int UNISTD_DECL_main ();
int ABS_OVERLOADS_main ();
int LIMITS_main ();
int LOCALE_NAME_FMAT_main ();
int WCTYPE_T_main ();
int FLOAT_main ();
int INFINITY_main ();
int NO_SIGNALING_NAN_main ();
int LCONV_main ();
int NEWLINE_main ();
int CTYPE_BITS_main ();
int MUNMAP_main ();
int STRUCT_TM_main ();


int cfgprintf(const char *format, ...)
{
	va_list args;
	int ret;

	va_start(args, format);
	ret = cfgvprintf(format, args);
	va_end(args);

	return ret;
}

int cfgvprintf (const char *format, va_list args)
{
    static char buf[5120];
	int ret;

	ret = vsnprintf(buf, 5120, format, args);

    fwrite (buf, ret, 1, config_file_hdr);
	return ret;
}




int main ()
{
    ndInitSystem ();
    
    config_file_hdr = fopen ("ms0:/configcpp.h", "w");
    
    LIMITS_main ();
    cfgprintf ("\n");
    
    FLOAT_main ();
    cfgprintf ("\n");
    
    SIZET_main ();
    cfgprintf ("\n");
    
    ABS_OVERLOADS_main ();
    cfgprintf ("\n");
    
    UNISTD_DECL_main ();
    cfgprintf ("\n");
    
    LOCALE_NAME_FMAT_main ();
    cfgprintf ("\n");
    
    WCTYPE_T_main ();
    cfgprintf ("\n");
    
    INFINITY_main ();
    cfgprintf ("\n");
    
    NO_SIGNALING_NAN_main ();
    cfgprintf ("\n");
    
    LCONV_main ();
    cfgprintf ("\n");
    
    NEWLINE_main ();
    cfgprintf ("\n");
    
    CTYPE_BITS_main ();
    cfgprintf ("\n");
    
    MUNMAP_main ();
    cfgprintf ("\n");
    
    STRUCT_TM_main ();
    cfgprintf ("\n");
    
    fclose (config_file_hdr);
    printf ("configcfg.h generated in ms0:/ folder of \n");
    printf ("emulator. \n");
    
}

