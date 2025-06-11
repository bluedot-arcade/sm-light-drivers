#ifndef PARALLEL_LIGHTS_IO_H_
#define PARALLEL_LIGHTS_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT __declspec(dllexport)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

EXPORT BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

EXPORT void WINAPI PortOut(short int Port, char Data);

EXPORT short int WINAPI IsDriverInstalled();

#ifdef __cplusplus
}
#endif

#endif // PARALLEL_LIGHTS_IO_H_
