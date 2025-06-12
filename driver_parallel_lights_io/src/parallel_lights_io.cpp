#define WIN32_LEAN_AND_MEAN

#include "parallel_lights_io.h"
#include <unilight.h>

#define PORT_LPT1 0x378
#define PORT_LPT2 0x278
#define PORT_LPT3 0x3BC

#define BD_UNILIGHT_LIGHT_PAD_P1_ALL ( \
  BD_UNILIGHT_LIGHT_PAD_P1_LEFT  | \
  BD_UNILIGHT_LIGHT_PAD_P1_RIGHT | \
  BD_UNILIGHT_LIGHT_PAD_P1_UP    | \
  BD_UNILIGHT_LIGHT_PAD_P1_DOWN)

#define BD_UNILIGHT_LIGHT_PAD_P2_ALL ( \
  BD_UNILIGHT_LIGHT_PAD_P2_LEFT  | \
  BD_UNILIGHT_LIGHT_PAD_P2_RIGHT | \
  BD_UNILIGHT_LIGHT_PAD_P2_UP    | \
  BD_UNILIGHT_LIGHT_PAD_P2_DOWN)

#define IS_BIT_SET(data, bit) ((data & (1 << bit)) != 0)

static uint8_t g_cab_lights_state = 0;
static uint8_t g_pad_lights_state = 0;

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

void WriteCabLights() {
  if(bd_unilight_set_cab_lights(g_cab_lights_state) != 0) {
    // No error handling in this implementation
  }
}

void WritePadLights() {
  if(bd_unilight_set_pad_lights(g_pad_lights_state) != 0) {
    // No error handling in this implementation
  }
}

void SetCabLights(char Data) {
  g_cab_lights_state = 0;
  if(IS_BIT_SET(Data, 0)) g_cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_LEFT;
  if(IS_BIT_SET(Data, 1)) g_cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_RIGHT;
  if(IS_BIT_SET(Data, 2)) g_cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_LEFT;
  if(IS_BIT_SET(Data, 3)) g_cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_RIGHT;
  if(IS_BIT_SET(Data, 4)) g_cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BUTTONS_P1;
  if(IS_BIT_SET(Data, 5)) g_cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BUTTONS_P2;
  if(IS_BIT_SET(Data, 6)) g_cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BASS;
  if(IS_BIT_SET(Data, 7)) g_cab_lights_state |= BD_UNILIGHT_LIGHT_CAB_BASS;
}

void SetPad1Lights(char Data) {
  g_pad_lights_state &= ~(BD_UNILIGHT_LIGHT_PAD_P1_ALL);
  if(IS_BIT_SET(Data, 0)) g_pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_LEFT;
  if(IS_BIT_SET(Data, 1)) g_pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_RIGHT;
  if(IS_BIT_SET(Data, 2)) g_pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_UP;
  if(IS_BIT_SET(Data, 3)) g_pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P1_DOWN;
}

void SetPad2Lights(char Data) {
  g_pad_lights_state &= ~(BD_UNILIGHT_LIGHT_PAD_P2_ALL);
  if(IS_BIT_SET(Data, 0)) g_pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_LEFT;
  if(IS_BIT_SET(Data, 1)) g_pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_RIGHT;
  if(IS_BIT_SET(Data, 2)) g_pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_UP;
  if(IS_BIT_SET(Data, 3)) g_pad_lights_state |= BD_UNILIGHT_LIGHT_PAD_P2_DOWN;
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
      WriteCabLights();
      break;
    case PORT_LPT2:
      SetPad1Lights(data);
      break;
    case PORT_LPT3:
      SetPad2Lights(data);
      WritePadLights(); // Write only after both pads are set.
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