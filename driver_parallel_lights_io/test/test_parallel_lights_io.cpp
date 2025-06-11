#include <windows.h>
#include <iostream>
#include <thread>

#define PORT_LPT1 0x378
#define PORT_LPT2 0x278
#define PORT_LPT3 0x3BC

typedef void (WINAPI *PortOutFunc)(short int, char);
typedef short int (WINAPI *IsDriverInstalledFunc)();

int main() {
    HMODULE hModule = LoadLibrary("parallel_lights_io.dll");
    if (!hModule) {
        std::cerr << "Failed to load DLL" << std::endl;
        return 1;
    }

    PortOutFunc PortOut = (PortOutFunc)GetProcAddress(hModule, "PortOut");
    IsDriverInstalledFunc IsDriverInstalled = (IsDriverInstalledFunc)GetProcAddress(hModule, "IsDriverInstalled");

    if (!PortOut || !IsDriverInstalled) {
        std::cerr << "Failed to get function addresses" << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    short int initResult = IsDriverInstalled();
    std::cout << "IsDriverInstalled result: " << initResult << std::endl;

    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        PortOut(PORT_LPT1, static_cast<char>(0xFF));
        PortOut(PORT_LPT2, static_cast<char>(0xFF));
        PortOut(PORT_LPT3, static_cast<char>(0xFF));
        std::this_thread::sleep_for(std::chrono::seconds(3));
        PortOut(PORT_LPT1, 0);
        PortOut(PORT_LPT2, 0);
        PortOut(PORT_LPT3, 0);
    }

    FreeLibrary(hModule);

    return 0;
}