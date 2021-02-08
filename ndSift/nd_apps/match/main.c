#include <nanodesktop.h>
#include <ndhighgui.h>

int sift_main( int argc, char** argv );
// Riferimento main

int ErrorLevel;

int ndMain (void)
{
   cvInitSystem (0, 0);
   pseudoExec (&sift_main, "match", "beaver.png beaver_xform.png", &ErrorLevel); 
}
