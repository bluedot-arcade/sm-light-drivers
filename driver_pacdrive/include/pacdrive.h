#ifndef PACDRIVE_H_
#define PACDRIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT __declspec(dllexport)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

EXPORT BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

/**
 * @brief Initialize the PacDrive devices.
 * @return The number of PacDrive devices found.
 */
EXPORT int PacInitialize(void);

/**
 * @brief Shutdown the PacDrive devices.
 */
EXPORT void PacShutdown(void);

/**
 * @brief Set the state of the LEDs on the PacDrive device.
 */
EXPORT bool PacSetLEDStates(int deviceId, short int data);

#ifdef __cplusplus
}
#endif

#endif // PACDRIVE_H_
