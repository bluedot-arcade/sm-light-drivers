#include "unison.h"
#include <hidapi.h>
#include <string.h>
#include <wchar.h>

// The UNISON firmware uses the LUFA demo VID/PID, so the product string
// is checked as well to avoid opening other LUFA based devices.
#define BD_UNISON_VID 0x03EB
#define BD_UNISON_PID 0x204F
#define BD_UNISON_PRODUCT L"UNISON"

// Lights are received on the vendor defined HID interface.
// The other interface is the pad gamepad.
#define BD_UNISON_VENDOR_USAGE_PAGE 0xFF00
#define BD_UNISON_VENDOR_INTERFACE  0

// Report ID byte (unused by the board) + 8 bytes vendor report.
#define BD_UNISON_REPORT_SIZE 9
#define BD_UNISON_CMD_SET_LIGHTS 0x02

// Offsets of the light bytes in the report, see firmware Lights.h.
// The set lights payload starts after the report ID and command bytes.
#define BD_UNISON_REPORT_CAB_BYTE       2
#define BD_UNISON_REPORT_P1_BTN_BYTE    3
#define BD_UNISON_REPORT_P1_PAD_BYTE    4
#define BD_UNISON_REPORT_P2_BTN_BYTE    7

#define BD_UNISON_REPORT_MARQUEE_UP_LEFT    0x01
#define BD_UNISON_REPORT_MARQUEE_UP_RIGHT   0x02
#define BD_UNISON_REPORT_MARQUEE_LR_LEFT    0x04
#define BD_UNISON_REPORT_MARQUEE_LR_RIGHT   0x08
#define BD_UNISON_REPORT_BASS_LEFT          0x10
#define BD_UNISON_REPORT_BASS_RIGHT         0x20

// Menu left, right, up, down and start lights of a player.
#define BD_UNISON_REPORT_BUTTONS_ALL        0x1F

// Pad lights are mapped on P1 custom lights 01-04.
#define BD_UNISON_REPORT_PAD_LEFT           0x10
#define BD_UNISON_REPORT_PAD_RIGHT          0x20
#define BD_UNISON_REPORT_PAD_UP             0x40
#define BD_UNISON_REPORT_PAD_DOWN           0x80

static uint8_t last_report[BD_UNISON_REPORT_SIZE];
static int last_report_valid = 0;
static hid_device *handle = NULL;

static int bd_unison_is_lights_interface(const struct hid_device_info *info) {
  if(info->product_string == NULL || wcscmp(info->product_string, BD_UNISON_PRODUCT) != 0) {
    return 0;
  }

  return info->usage_page == BD_UNISON_VENDOR_USAGE_PAGE ||
         info->interface_number == BD_UNISON_VENDOR_INTERFACE;
}

int bd_unison_open(void) {
  if(handle != NULL) {
    return 0;
  }

  hid_init();

  struct hid_device_info *devs = hid_enumerate(BD_UNISON_VID, BD_UNISON_PID);
  for(struct hid_device_info *cur = devs; cur != NULL; cur = cur->next) {
    if(bd_unison_is_lights_interface(cur)) {
      handle = hid_open_path(cur->path);
      if(handle != NULL) {
        break;
      }
    }
  }
  hid_free_enumeration(devs);

  if(handle == NULL) {
    return -1;
  }

  last_report_valid = 0;
  return 0;
}

int bd_unison_close(void) {
  if(handle == NULL) {
    return 0;
  }

  hid_close(handle);
  handle = NULL;
  hid_exit();

  return 0;
}

int bd_unison_is_open(void) {
  return handle != NULL;
}

int bd_unison_set_lights(uint8_t cab_state, uint8_t pad_state) {
  if(handle == NULL) {
    return -1;
  }

  uint8_t report[BD_UNISON_REPORT_SIZE];
  memset(report, 0, sizeof(report));
  report[1] = BD_UNISON_CMD_SET_LIGHTS;

  uint8_t *cab = &report[BD_UNISON_REPORT_CAB_BYTE];
  if(cab_state & BD_UNISON_LIGHT_CAB_MARQUEE_UP_LEFT)  *cab |= BD_UNISON_REPORT_MARQUEE_UP_LEFT;
  if(cab_state & BD_UNISON_LIGHT_CAB_MARQUEE_UP_RIGHT) *cab |= BD_UNISON_REPORT_MARQUEE_UP_RIGHT;
  if(cab_state & BD_UNISON_LIGHT_CAB_MARQUEE_LR_LEFT)  *cab |= BD_UNISON_REPORT_MARQUEE_LR_LEFT;
  if(cab_state & BD_UNISON_LIGHT_CAB_MARQUEE_LR_RIGHT) *cab |= BD_UNISON_REPORT_MARQUEE_LR_RIGHT;
  if(cab_state & BD_UNISON_LIGHT_CAB_BASS_LEFT)        *cab |= BD_UNISON_REPORT_BASS_LEFT;
  if(cab_state & BD_UNISON_LIGHT_CAB_BASS_RIGHT)       *cab |= BD_UNISON_REPORT_BASS_RIGHT;

  if(cab_state & BD_UNISON_LIGHT_CAB_BUTTONS_P1) report[BD_UNISON_REPORT_P1_BTN_BYTE] = BD_UNISON_REPORT_BUTTONS_ALL;
  if(cab_state & BD_UNISON_LIGHT_CAB_BUTTONS_P2) report[BD_UNISON_REPORT_P2_BTN_BYTE] = BD_UNISON_REPORT_BUTTONS_ALL;

  uint8_t *pad = &report[BD_UNISON_REPORT_P1_PAD_BYTE];
  if(pad_state & BD_UNISON_LIGHT_PAD_LEFT)  *pad |= BD_UNISON_REPORT_PAD_LEFT;
  if(pad_state & BD_UNISON_LIGHT_PAD_RIGHT) *pad |= BD_UNISON_REPORT_PAD_RIGHT;
  if(pad_state & BD_UNISON_LIGHT_PAD_UP)    *pad |= BD_UNISON_REPORT_PAD_UP;
  if(pad_state & BD_UNISON_LIGHT_PAD_DOWN)  *pad |= BD_UNISON_REPORT_PAD_DOWN;

  if(last_report_valid && memcmp(report, last_report, sizeof(report)) == 0) {
    return 0;
  }

  if(hid_write(handle, report, sizeof(report)) < 0) {
    last_report_valid = 0;
    return -1;
  }

  memcpy(last_report, report, sizeof(report));
  last_report_valid = 1;
  return 0;
}
