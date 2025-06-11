#pragma once
#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__) // MSVC-specific pragma
#include <Windows.h>

BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

short int WINAPI IsDriverInstalled();
void WINAPI PortOut(short int, char);
char WINAPI PortIn(short int);
void WINAPI ClrPortBit(short int Port, int Bit);
short int WINAPI GetPortBit(short int Port, int Bit);
void WINAPI LeftPortShift(short int Port, int ShiftAmount);
void WINAPI NotPortBit(short int Port, int Bit);
unsigned long WINAPI PortDWordIn(short int Port);
void WINAPI PortDWordOut(short int Port, unsigned long Data);
short int WINAPI PortWordIn(short int Port);
void WINAPI PortWordOut(short int Port, short int Data);
void WINAPI ReleasePort(short int Port);
void WINAPI RightPortShift(short int Port, int ShiftAmount);
void WINAPI SetPortBit(short int Port, int Bit);