#ifndef APAC_MISC_H
#define APAC_MISC_H

#include <stdlib.h>
#include <intrin.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "../apacExport.h"

typedef enum apac_err
{
    APAC_OK,
    APAC_OOM,
    APAC_MATH_ERR

}   apac_err;

#ifndef APAC_ASSERT
    #define APAC_ASSERT(x) asset(x)
#endif

#ifndef APAC_REPORT_ERR
    #define APAC_REPORT_ERR(x) fprintf(stderr, "APAC ERROR: %s\n", x)
#endif

// for getCPUSpec()

extern uint8_t adx_chk;
extern uint8_t bmi2_chk;

APAC_API void getCPUSpec(void);

// for setMemFuncs()

extern APAC_API void* (*apac_malloc)(size_t);
extern APAC_API void* (*apac_realloc)(void*, size_t);
extern APAC_API void (*apac_free)(void*);

APAC_API void setMemFuncs(
    void* (*ptr1)(size_t),
    void* (*ptr2)(void*, size_t),
    void (*ptr3)(void*)
);

// for apacInitDefault() which auto initializes above functions with default behaviour

APAC_API void apacInitDefault(void);

#endif