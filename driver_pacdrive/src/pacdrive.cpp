#include <pacdrive.h>

#if defined(BD_BOARD_UNILIGHT)
#include <unilight.h>
#elif defined(BD_BOARD_UNISON)
#include <unison.h>
#if !defined(BD_UNISON_PLAYER) || (BD_UNISON_PLAYER != 1 && BD_UNISON_PLAYER != 2)
#error "Define BD_UNISON_PLAYER as 1 or 2 to select the pad driven by the UNISON board."
#endif
#else
#error "No target board defined. Define BD_BOARD_UNILIGHT or BD_BOARD_UNISON."
#endif

#define IS_BIT_SET(data, bit) ((data & (1 << (bit))) != 0)

#if defined(BD_BOARD_UNILIGHT)

static int OpenBoard() { return bd_unilight_open(); }
static int CloseBoard() { return bd_unilight_close(); }
static int IsBoardOpen() { return bd_unilight_is_open(); }

static bool SetBoardLEDStates(short int data) {
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

#elif defined(BD_BOARD_UNISON)

static int OpenBoard() { return bd_unison_open(); }
static int CloseBoard() { return bd_unison_close(); }
static int IsBoardOpen() { return bd_unison_is_open(); }

static bool SetBoardLEDStates(short int data) {
  // The UNISON board drives a single pad: P1 pad lights are bits 0-3, P2 pad lights are bits 4-7.
  const int pad_bit = (BD_UNISON_PLAYER - 1) * 4;

  uint8_t pad_lights_state = 0;
  if(IS_BIT_SET(data, pad_bit + 0)) pad_lights_state |= BD_UNISON_LIGHT_PAD_UP;
  if(IS_BIT_SET(data, pad_bit + 1)) pad_lights_state |= BD_UNISON_LIGHT_PAD_DOWN;
  if(IS_BIT_SET(data, pad_bit + 2)) pad_lights_state |= BD_UNISON_LIGHT_PAD_LEFT;
  if(IS_BIT_SET(data, pad_bit + 3)) pad_lights_state |= BD_UNISON_LIGHT_PAD_RIGHT;

  uint8_t cab_lights_state = 0;
  if(IS_BIT_SET(data, 8)) cab_lights_state |= BD_UNISON_LIGHT_CAB_MARQUEE_UP_LEFT;
  if(IS_BIT_SET(data, 9)) cab_lights_state |= BD_UNISON_LIGHT_CAB_MARQUEE_UP_RIGHT;
  if(IS_BIT_SET(data, 10)) cab_lights_state |= BD_UNISON_LIGHT_CAB_MARQUEE_LR_LEFT;
  if(IS_BIT_SET(data, 11)) cab_lights_state |= BD_UNISON_LIGHT_CAB_MARQUEE_LR_RIGHT;
  if(IS_BIT_SET(data, 12)) cab_lights_state |= BD_UNISON_LIGHT_CAB_BUTTONS_P1;
  if(IS_BIT_SET(data, 13)) cab_lights_state |= BD_UNISON_LIGHT_CAB_BUTTONS_P2;
  if(IS_BIT_SET(data, 14)) cab_lights_state |= BD_UNISON_LIGHT_CAB_BASS_LEFT | BD_UNISON_LIGHT_CAB_BASS_RIGHT;

  return bd_unison_set_lights(cab_lights_state, pad_lights_state) == 0;
}

#endif

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
  switch (nReason) {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    if(IsBoardOpen()) {
      CloseBoard();
    }
    break;
  }
  return TRUE;
}

int PacInitialize(void) {
  if(OpenBoard() == 0) {
    // The return values is the number of PacDrive devices found.
    // Here we only have 1 device.
    return 1;
  }
  return 0;
}

void PacShutdown(void) {
  CloseBoard();
}

bool PacSetLEDStates(int deviceId, short int data) {
  return SetBoardLEDStates(data);
}
