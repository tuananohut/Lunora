#ifndef CORE_H
#define CORE_H

#ifdef LNR_BUILD_DLL
    #define LUNORA_API __declspec(dllexport)
#else
    #define LUNORA_API __declspec(dllimport)
#endif

#endif
