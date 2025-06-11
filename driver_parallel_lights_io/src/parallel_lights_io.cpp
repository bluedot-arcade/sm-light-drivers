#define WIN32_LEAN_AND_MEAN

#include "parallel_lights_io.h"
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
  default:
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
  if(IS_BIT_SET(Data, 0)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_LEFT;
  if(IS_BIT_SET(Data, 1)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_RIGHT;
  if(IS_BIT_SET(Data, 2)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_UP;
  if(IS_BIT_SET(Data, 3)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_DOWN;
  if(bd_unilight_set_pad_lights(pad_lights_state) != 0) {
    // No error handling in this implementation
  }
}

void SetPad2Lights(char Data) {
  uint8_t pad_lights_state = 0;
  if(IS_BIT_SET(Data, 0)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_LEFT;
  if(IS_BIT_SET(Data, 1)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_RIGHT;
  if(IS_BIT_SET(Data, 2)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_UP;
  if(IS_BIT_SET(Data, 3)) pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_DOWN;
  if(bd_unilight_set_pad_lights(pad_lights_state) != 0) {
    // No error handling in this implementation
  }
}

short int WINAPI IsDriverInstalled(){
    #pragma EXPORT
    return 1;
}

void WINAPI PortOut(short int port, char data)
{
#pragma EXPORT
  switch(port) {
    case PORT_LPT1:
      SetCabLights(data);
      break;
    case PORT_LPT2:
      SetPad1Lights(data);
      break;
    case PORT_LPT3:
      SetPad2Lights(data);
      break;
    default:
      break;
  }
}

// --- STUB IMPLEMENTATIONS FOR OTHER EXPORTED FUNCTIONS (with #pragma EXPORT) ---

char WINAPI PortIn(short int port){
#pragma EXPORT
  return '\0';
}

void WINAPI ClrPortBit(short int Port, int Bit) {
  #pragma EXPORT
}

short int WINAPI GetPortBit(short int Port, int Bit) {
  #pragma EXPORT
  return 0;
}

void WINAPI LeftPortShift(short int Port, int ShiftAmount) {
  #pragma EXPORT
}

void WINAPI NotPortBit(short int Port, int Bit) {
  #pragma EXPORT
}

unsigned long WINAPI PortDWordIn(short int Port) {
  #pragma EXPORT
  return 0;
}

void WINAPI PortDWordOut(short int Port, unsigned long Data) {
  #pragma EXPORT
}

short int WINAPI PortWordIn(short int Port) {
  #pragma EXPORT
  return 0;
}

void WINAPI PortWordOut(short int Port, short int Data) {
  #pragma EXPORT
}

void WINAPI ReleasePort(short int Port) {
  #pragma EXPORT
}

void WINAPI RightPortShift(short int Port, int ShiftAmount) {
  #pragma EXPORT
}

void WINAPI SetPortBit(short int Port, int Bit) {
  #pragma EXPORT
}