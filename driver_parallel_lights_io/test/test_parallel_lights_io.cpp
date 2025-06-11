#include <windows.h>
#include <iostream>
#include <thread>

#define PORT_LPT1 0x378
#define PORT_LPT2 0x278
#define PORT_LPT3 0x3BC

typedef void (WINAPI *PortOutFunc)(short int, char);
typedef short int (WINAPI *IsDriverInstalledFunc)();

int main() {
    const HMODULE hModule = LoadLibrary("parallel_lights_io.dll");
    if (!hModule) {
        const DWORD error = GetLastError(); // Get the last error code
        std::cerr << "Failed to load DLL. Error code: " << error << " (" << std::system_category().message(error) << ")" << std::endl;
        return 1;
    }

    const auto PortOut = reinterpret_cast<PortOutFunc>(GetProcAddress(hModule, "PortOut"));
    const auto IsDriverInstalled = reinterpret_cast<IsDriverInstalledFunc>(GetProcAddress(hModule, "IsDriverInstalled"));

    if (!PortOut || !IsDriverInstalled) {
        const DWORD error = GetLastError(); // Get the last error code for GetProcAddress failure
        std::cerr << "Failed to get function addresses. Error code: " << error << " (" << std::system_category().message(error) << ")" << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    const short int initResult = IsDriverInstalled();
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