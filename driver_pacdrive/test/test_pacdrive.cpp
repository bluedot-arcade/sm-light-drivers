#include <windows.h>
#include <iostream>
#include <thread>

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

    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        PacSetLEDStates(0, static_cast<short int>(0xFFFF));
        std::this_thread::sleep_for(std::chrono::seconds(3));
        PacSetLEDStates(0, 0);
    }

    PacShutdown();
    FreeLibrary(hModule);

    return 0;
}