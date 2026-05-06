#ifndef APAC_UTILITIES_H
#define APAC_UTILITIES_H

#include "../include/apac.h"

#if (defined(_WIN32) && defined(_MSC_VER))
    #include <powrprof.h>
    #pragma comment(lib, "powrprof")
#endif

#define APAC_TEST_START(name) printf("\nTEST START: %s()\n", name)
#define APAC_TEST_END(name) printf("TEST END: %s()\n", name);

uint64_t cpu_timer(void);
uint64_t os_timer(void);

int pin_curr_thread_to_core(uint32_t core_id);

void apac_disable_dfs(void);
void apac_restore_dfs(void);

#endif