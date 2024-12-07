#ifndef PACDRIVE_H_
#define PACDRIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT __declspec(dllexport)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

EXPORT BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

EXPORT int PacInitialize(void);
EXPORT void PacShutdown(void);
EXPORT bool PacSetLEDStates(int deviceId, short int data);

#ifdef __cplusplus
}
#endif

#endif // PACDRIVE_H_
