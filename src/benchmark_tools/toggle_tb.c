#include "benchmarkTools.h"

#include <Windows.h>
#include <powrprof.h>
#pragma comment (lib, "powrprof")

static GUID* CurrentScheme;
static DWORD CurrentMode;

void DisableTurboBoost(void)
{
    PowerGetActiveScheme(NULL, &CurrentScheme);
    PowerReadACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, &CurrentMode);
    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, PROCESSOR_PERF_BOOST_MODE_DISABLED);
    PowerSetActiveScheme(NULL, CurrentScheme);
}

void RestoreTurboBoost(void)
{
    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, CurrentMode);
    PowerWriteACValueIndex(NULL, CurrentScheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, &GUID_PROCESSOR_PERF_BOOST_MODE, PROCESSOR_PERF_BOOST_MODE_ENABLED);
    PowerSetActiveScheme(NULL, CurrentScheme);
    LocalFree(CurrentScheme);
    CurrentScheme = NULL;
}