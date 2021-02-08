#include <nanodesktop.h>
#include <ndhighgui.h>

int sift_main( int argc, char** argv );
// Riferimento main

int ErrorLevel;

int ndMain (void)
{
   cvInitSystem (0, 0);
   printf ("FIND Features \n");
   
   pseudoExec (&sift_main, "siftfeatures", "ms0:/beaver.png", &ErrorLevel); 
}
