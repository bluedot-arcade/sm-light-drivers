#pragma once
#include <windows.h>

#ifdef _MSC_VER
#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__) // MSVC-specific pragma
#define API
#else
// MinGW ignores the MSVC pragma, so functions are exported with dllexport instead.
// Undecorated stdcall names on 32-bit are produced by the --kill-at linker flag.
#define EXPORT
#define API extern "C" __declspec(dllexport)
#endif

BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

API short int WINAPI IsDriverInstalled();
API void WINAPI PortOut(short int, char);
API char WINAPI PortIn(short int);
API void WINAPI ClrPortBit(short int Port, int Bit);
API short int WINAPI GetPortBit(short int Port, int Bit);
API void WINAPI LeftPortShift(short int Port, int ShiftAmount);
API void WINAPI NotPortBit(short int Port, int Bit);
API unsigned long WINAPI PortDWordIn(short int Port);
API void WINAPI PortDWordOut(short int Port, unsigned long Data);
API short int WINAPI PortWordIn(short int Port);
API void WINAPI PortWordOut(short int Port, short int Data);
API void WINAPI ReleasePort(short int Port);
API void WINAPI RightPortShift(short int Port, int ShiftAmount);
API void WINAPI SetPortBit(short int Port, int Bit);