#include <stdarg.h>

#ifdef I_AM_EXEC_CONFIG
   extern "C" 
   {
          int cfgprintf(const char *format, ...);
          int cfgvprintf (const char *format, va_list args);
   }
#endif
