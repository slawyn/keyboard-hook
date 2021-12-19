
#include <windows.h>
#include <stdint.h>


#ifdef USINGDLL

//typedef EXPORT uint64_t ui64GetCount();
typedef BOOL (CALLBACK *SetupHook)(HWND);
typedef BOOL (CALLBACK *UninstallHook)(void);

#else


#define EXPORT extern "C" __declspec (dllexport)

// Funktonalität sowohl unter C als auch unter C++ gegeben
//EXPORT EXPORT uint64_t ui64GetCount();
EXPORT BOOL CALLBACK SetupHook(HWND);
EXPORT BOOL CALLBACK UninstallHook(void);
#endif;