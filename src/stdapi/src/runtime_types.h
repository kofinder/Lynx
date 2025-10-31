#ifndef LYNX_RUNTIME_TYPES_H
#define LYNX_RUNTIME_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>  // Needed for FILE*

typedef struct {
    int year, month, day;
    int hour, minute, second;
    int ms, tz;
} datetime_t;

typedef struct {
    const char* path;
    FILE* handle;
} file_t;

#ifdef __cplusplus
}
#endif

#endif // LYNX_RUNTIME_TYPES_H
