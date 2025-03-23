#ifndef APAC_EXPORT_H
#define APAC_EXPORT_H

#if defined(BUILD_SHARED_LIB)
    // Export symbols when building the DLL
    #define APAC_API __declspec(dllexport)
#elif defined(LIBAPAC_DLL)
    // Import symbols when using the DLL
    #define APAC_API __declspec(dllimport)
#else
    // Static library, no import/export needed
    #define APAC_API
#endif

#endif