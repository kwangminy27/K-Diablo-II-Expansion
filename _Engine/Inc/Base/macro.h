#pragma once

#ifdef K_ENGINE_EXPORT
#define K_ENGINE_DLL __declspec(dllexport)
#else
#define K_ENGINE_DLL __declspec(dllimport)
#endif
