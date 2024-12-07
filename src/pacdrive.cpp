#include <pacdrive.h>
#include <hidapi.h>

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
  switch (nReason) {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

int PacInitialize(void) {
  return 5;
}

void PacShutdown(void) {

}

bool PacSetLEDStates(int deviceId, short int data) {
  return true;
}


