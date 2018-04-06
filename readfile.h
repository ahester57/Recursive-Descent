#ifndef READFILE_H_
#define READFILE_H_
#include <stdio.h>

char** getfilestring (FILE* fp);
int getnumwords ();
size_t getfilesize (FILE* fp);

#endif