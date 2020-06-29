#include "gfxutils.h"
#include "shapes.h"
#include <stdarg.h>
#include <switch.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *CopyTextUtil(const char *in){
    char *out;
    out = calloc(strlen(in) + 1, 1);
    strcpy(out, in);
    return out;
}

char *CopyTextLenUtil(const char *in, int len){
    char *out;
    int locallen = len;

    if (locallen > strlen(in))
        locallen = strlen(in);

    out = calloc(locallen + 1, 1);
    memcpy(out, in, locallen);
    return out;
}

char *CopyTextArgsUtil(const char *in, ...){
    char *buff, *out;
    buff = calloc(0x200, 1);
    va_list args;
    va_start(args, in);
    vsprintf(buff, in, args);
    va_end(args);

    out = CopyTextUtil(buff);
    free(buff);
    return out;
}

void Log(char *text){
    static FILE *file = NULL;
    if (file == NULL)
        file = fopen("fuck.txt", "w");

    fprintf(file, text);
}