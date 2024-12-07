#include <windows.h>
#include <iostream>

typedef int (WINAPI *PacInitializeFunc)();
typedef void (WINAPI *PacShutdownFunc)();
typedef bool (WINAPI *PacSetLEDStatesFunc)(int, short int);

int main() {
    HMODULE hModule = LoadLibrary("PacDrive32.dll");
    if (!hModule) {
        std::cerr << "Failed to load DLL" << std::endl;
        return 1;
    }

    PacInitializeFunc PacInitialize = (PacInitializeFunc)GetProcAddress(hModule, "PacInitialize");
    PacShutdownFunc PacShutdown = (PacShutdownFunc)GetProcAddress(hModule, "PacShutdown");
    PacSetLEDStatesFunc PacSetLEDStates = (PacSetLEDStatesFunc)GetProcAddress(hModule, "PacSetLEDStates");

    if (!PacInitialize || !PacShutdown || !PacSetLEDStates) {
        std::cerr << "Failed to get function addresses" << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    int initResult = PacInitialize();
    std::cout << "PacInitialize result: " << initResult << std::endl;

    bool setLEDResult = PacSetLEDStates(0, 0xFFFF);
    std::cout << "PacSetLEDStates result: " << setLEDResult << std::endl;

    PacShutdown();
    FreeLibrary(hModule);

    return 0;
}