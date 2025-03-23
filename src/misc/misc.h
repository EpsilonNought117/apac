#ifndef APAC_MISC_H
#define APAC_MISC_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "getCPUSpec/getCPUSpec.h"
#include "../apacExport.h"

typedef enum apac_err
{
    APAC_OK,
    APAC_OOM,
    APAC_MATH_ERR

}   apac_err;

#ifndef APAC_ASSERT
    #define APAC_ASSERT(x) assert(x)
#endif

#ifndef APAC_REPORT_ERR
    #define APAC_REPORT_ERR(x) fprintf(stderr, "APAC ERROR: %s\n", x)
#endif

// for setMemFuncs()

APAC_API extern void* (*apac_malloc)(size_t);
APAC_API extern void* (*apac_realloc)(void*, size_t);
APAC_API extern void (*apac_free)(void*);

APAC_API void setMemFuncs(
    void* (*ptr1)(size_t),
    void* (*ptr2)(void*, size_t),
    void (*ptr3)(void*)
);

// for apacInitDefault() which auto initializes above functions with default behaviour

APAC_API void apacInitDefault(void);

#endif