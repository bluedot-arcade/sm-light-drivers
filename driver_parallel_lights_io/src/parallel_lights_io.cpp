#include <parallel_lights_io.h>
#include <unilight.h>

#define PORT_LPT1 0x378
#define PORT_LPT2 0x278
#define PORT_LPT3 0x3BC

#define IS_BIT_SET(data, bit) ((data & (1 << bit)) != 0)

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
  switch (nReason) {
  case DLL_PROCESS_ATTACH:
    if(!bd_unilight_is_open()) {
      if(bd_unilight_open() != 0) {
        return FALSE;
      }
    }
    break;
  case DLL_PROCESS_DETACH:
    if(bd_unilight_is_open()) {
      bd_unilight_close();
    }
    break;
  }
  return TRUE;
}

void SetCabLights(char Data) {
  uint8_t cab_lights_state = 0;
  if(IS_BIT_SET(Data, 0)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_LEFT;
  if(IS_BIT_SET(Data, 1)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_RIGHT;
  if(IS_BIT_SET(Data, 2)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_LEFT;
  if(IS_BIT_SET(Data, 3)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_RIGHT;
  if(IS_BIT_SET(Data, 4)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BUTTONS_P1;
  if(IS_BIT_SET(Data, 5)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BUTTONS_P2;
  if(IS_BIT_SET(Data, 6)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BASS;
  if(IS_BIT_SET(Data, 7)) cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BASS;
  if(bd_unilight_set_cab_lights(cab_lights_state) != 0) {
    // No error handling in this implementation
  }
}

void SetPad1Lights(char Data) {
  uint8_t pad_lights_state = 0;
  if(IS_BIT_SET(data, 0)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_LEFT;
  if(IS_BIT_SET(data, 1)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_RIGHT;
  if(IS_BIT_SET(data, 2)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_UP;
  if(IS_BIT_SET(data, 3)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_DOWN;
  if(bd_unilight_set_pad_lights(pad_lights_state) != 0) {
    // No error handling in this implementation
  }
}

void SetPad2Lights(char Data) {
  uint8_t pad_lights_state = 0;
  if(IS_BIT_SET(data, 0)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_LEFT;
  if(IS_BIT_SET(data, 1)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_RIGHT;
  if(IS_BIT_SET(data, 2)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_UP;
  if(IS_BIT_SET(data, 3)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_DOWN;
  if(bd_unilight_set_pad_lights(pad_lights_state) != 0) {
    // No error handling in this implementation
  }
}

void PortOut(short int Port, char Data) {
  switch(Port) {
    case PORT_LPT1:
      SetCabLights(Data);
      break;
    case PORT_LPT2:
      SetPad1Lights(Data);
      break;
    case PORT_LPT3:
      SetPad2Lights(Data);
      break;
    default:
      // Invalid port
      break;
  }
}

short int IsDriverInstalled() {
  return true;
}

