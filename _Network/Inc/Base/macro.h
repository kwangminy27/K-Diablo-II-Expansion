#pragma once

#ifdef K_NETWORK_EXPORT
#define K_NETWORK_DLL __declspec(dllexport)
#else
#define K_NETWORK_DLL __declspec(dllimport)
#endif
