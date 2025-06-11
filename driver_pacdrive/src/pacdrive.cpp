#include <pacdrive.h>
#include <unilight.h>

#define IS_BIT_SET(data, bit) ((data & (1 << bit)) != 0)

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
  switch (nReason) {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    if(bd_unilight_is_open()) {
      bd_unilight_close();
    }
    break;
  }
  return TRUE;
}

int PacInitialize(void) {
  if(bd_unilight_open() == 0) {
    // The return values is the number of PacDrive devices found.
    // Here we only have 1 device.
    return 1; 
  }
  return 0; 
}

void PacShutdown(void) {
  bd_unilight_close();
}

bool PacSetLEDStates(int deviceId, short int data) {
  bool success = true;

  uint8_t pad_lights_state = 0;
  if(IS_BIT_SET(data, 0)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_UP;
  if(IS_BIT_SET(data, 1)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_DOWN;
  if(IS_BIT_SET(data, 2)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_LEFT;
  if(IS_BIT_SET(data, 3)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_RIGHT;
  if(IS_BIT_SET(data, 4)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_UP;
  if(IS_BIT_SET(data, 5)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_DOWN;
  if(IS_BIT_SET(data, 6)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_LEFT;
  if(IS_BIT_SET(data, 7)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_RIGHT;
  if(bd_unilight_set_pad_lights(pad_lights_state) != 0) {
    success = false;
  }
  
  uint8_t cab_lights_state = 0;
  if(IS_BIT_SET(data, 8)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_LEFT;
  if(IS_BIT_SET(data, 9)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_RIGHT;
  if(IS_BIT_SET(data, 10)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_LEFT;
  if(IS_BIT_SET(data, 11)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_RIGHT;
  if(IS_BIT_SET(data, 12)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BUTTONS_P1;
  if(IS_BIT_SET(data, 13)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BUTTONS_P2;
  if(IS_BIT_SET(data, 14)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BASS;
  if(bd_unilight_set_cab_lights(cab_lights_state) != 0) {
    success = false;
  }

  return success;
}


