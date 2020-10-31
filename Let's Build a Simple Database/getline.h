#ifndef _LETS_BUILD_A_SIMPLE_DATABASE_ZH_GETLINE_H_
#define _LETS_BUILD_A_SIMPLE_DATABASE_ZH_GETLINE_H_

#include <malloc.h>
#include <stdio.h>

#if !(defined linux) && !(defined __linux) && !(defined __linux__)
typedef intptr_t ssize_t;
#endif

#ifndef __GUNC__
ssize_t getline(char** line, size_t* n, FILE* fp)
{
    char* buf = *line;
    ssize_t c, i = 0;
    if (buf == NULL || *n == 0)
    {
        *line = malloc(10);
        buf = *line;
        *n = 10;
    }

    while ((c = fgetc(fp)) != '\n')
    {
        if (c == EOF)
            return -1;
        if (i < (int)*n - 2)
        {
            *(buf + i++) = c;
        }
        else
        {
            *n = *n + 10;
            buf = realloc(buf, *n);
            *(buf + i++) = c;
        }
    }
    *(buf + i++) = '\n';
    *(buf + i) = '\0';
    return i;
}
#endif // __GUNC__

#endif // _LETS_BUILD_A_SIMPLE_DATABASE_ZH_GETLINE_H_