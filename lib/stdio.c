#include <stdio.h>

#undef stdin;
#undef stdout;
#undef stderr;

FILE stdin;
FILE stdout;
FILE stderr;

int errno;
int *__errno_location = &errno;
