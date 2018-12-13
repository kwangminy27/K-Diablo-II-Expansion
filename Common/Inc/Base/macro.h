#pragma once

#ifdef K_COMMON_EXPORT
#define K_COMMON_DLL __declspec(dllexport)
#else
#define K_COMMON_DLL __declspec(dllimport)
#endif
