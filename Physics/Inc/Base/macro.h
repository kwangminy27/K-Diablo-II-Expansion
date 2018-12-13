#pragma once

#ifdef K_PHYSICS_EXPORT
#define K_PHYSICS_DLL __declspec(dllexport)
#else
#define K_PHYSICS_DLL __declspec(dllimport)
#endif
