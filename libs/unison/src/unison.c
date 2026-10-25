#include "unison.h"
#include <hidapi.h>
#include <stdlib.h>
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

#define BD_UNISON_MAX_ENUMERATED 16

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
// Bits match BD_UNISON_LIGHT_BTN_*.
#define BD_UNISON_REPORT_BUTTONS_MASK       0x1F

// Pad lights are mapped on P1 custom lights 01-04.
#define BD_UNISON_REPORT_PAD_LEFT           0x10
#define BD_UNISON_REPORT_PAD_RIGHT          0x20
#define BD_UNISON_REPORT_PAD_UP             0x40
#define BD_UNISON_REPORT_PAD_DOWN           0x80

struct bd_unison_device {
  hid_device *handle;
  uint8_t last_report[BD_UNISON_REPORT_SIZE];
  int last_report_valid;
};

static bd_unison_device_t *device = NULL;

static int bd_unison_is_lights_interface(const struct hid_device_info *info) {
  if(info->product_string == NULL || wcscmp(info->product_string, BD_UNISON_PRODUCT) != 0) {
    return 0;
  }

  return info->usage_page == BD_UNISON_VENDOR_USAGE_PAGE ||
         info->interface_number == BD_UNISON_VENDOR_INTERFACE;
}

static int bd_unison_compare_paths(const void *a, const void *b) {
  return strcmp(*(const char * const *)a, *(const char * const *)b);
}

int bd_unison_count_devices(void) {
  hid_init();

  int count = 0;
  struct hid_device_info *devs = hid_enumerate(BD_UNISON_VID, BD_UNISON_PID);
  for(struct hid_device_info *cur = devs; cur != NULL; cur = cur->next) {
    if(bd_unison_is_lights_interface(cur)) {
      count++;
    }
  }
  hid_free_enumeration(devs);

  return count;
}

int bd_unison_open_devices(bd_unison_device_t **devices, int max_devices) {
  hid_init();

  const char *paths[BD_UNISON_MAX_ENUMERATED];
  int path_count = 0;

  struct hid_device_info *devs = hid_enumerate(BD_UNISON_VID, BD_UNISON_PID);
  for(struct hid_device_info *cur = devs; cur != NULL && path_count < BD_UNISON_MAX_ENUMERATED; cur = cur->next) {
    if(bd_unison_is_lights_interface(cur)) {
      paths[path_count++] = cur->path;
    }
  }

  qsort(paths, path_count, sizeof(paths[0]), bd_unison_compare_paths);

  int count = 0;
  for(int i = 0; i < path_count && count < max_devices; i++) {
    hid_device *handle = hid_open_path(paths[i]);
    if(handle == NULL) {
      continue;
    }

    bd_unison_device_t *dev = calloc(1, sizeof(*dev));
    if(dev == NULL) {
      hid_close(handle);
      break;
    }

    dev->handle = handle;
    devices[count++] = dev;
  }
  hid_free_enumeration(devs);

  return count;
}

void bd_unison_close_device(bd_unison_device_t *dev) {
  if(dev == NULL) {
    return;
  }

  hid_close(dev->handle);
  free(dev);
}

int bd_unison_write_lights(bd_unison_device_t *dev, const bd_unison_lights_t *lights) {
  if(dev == NULL) {
    return -1;
  }

  uint8_t report[BD_UNISON_REPORT_SIZE];
  memset(report, 0, sizeof(report));
  report[1] = BD_UNISON_CMD_SET_LIGHTS;

  uint8_t *cab = &report[BD_UNISON_REPORT_CAB_BYTE];
  if(lights->cab & BD_UNISON_LIGHT_CAB_MARQUEE_UP_LEFT)  *cab |= BD_UNISON_REPORT_MARQUEE_UP_LEFT;
  if(lights->cab & BD_UNISON_LIGHT_CAB_MARQUEE_UP_RIGHT) *cab |= BD_UNISON_REPORT_MARQUEE_UP_RIGHT;
  if(lights->cab & BD_UNISON_LIGHT_CAB_MARQUEE_LR_LEFT)  *cab |= BD_UNISON_REPORT_MARQUEE_LR_LEFT;
  if(lights->cab & BD_UNISON_LIGHT_CAB_MARQUEE_LR_RIGHT) *cab |= BD_UNISON_REPORT_MARQUEE_LR_RIGHT;
  if(lights->cab & BD_UNISON_LIGHT_CAB_BASS_LEFT)        *cab |= BD_UNISON_REPORT_BASS_LEFT;
  if(lights->cab & BD_UNISON_LIGHT_CAB_BASS_RIGHT)       *cab |= BD_UNISON_REPORT_BASS_RIGHT;

  report[BD_UNISON_REPORT_P1_BTN_BYTE] = lights->p1_buttons & BD_UNISON_REPORT_BUTTONS_MASK;
  report[BD_UNISON_REPORT_P2_BTN_BYTE] = lights->p2_buttons & BD_UNISON_REPORT_BUTTONS_MASK;

  uint8_t *pad = &report[BD_UNISON_REPORT_P1_PAD_BYTE];
  if(lights->pad & BD_UNISON_LIGHT_PAD_LEFT)  *pad |= BD_UNISON_REPORT_PAD_LEFT;
  if(lights->pad & BD_UNISON_LIGHT_PAD_RIGHT) *pad |= BD_UNISON_REPORT_PAD_RIGHT;
  if(lights->pad & BD_UNISON_LIGHT_PAD_UP)    *pad |= BD_UNISON_REPORT_PAD_UP;
  if(lights->pad & BD_UNISON_LIGHT_PAD_DOWN)  *pad |= BD_UNISON_REPORT_PAD_DOWN;

  if(dev->last_report_valid && memcmp(report, dev->last_report, sizeof(report)) == 0) {
    return 0;
  }

  if(hid_write(dev->handle, report, sizeof(report)) < 0) {
    dev->last_report_valid = 0;
    return -1;
  }

  memcpy(dev->last_report, report, sizeof(report));
  dev->last_report_valid = 1;
  return 0;
}

int bd_unison_open(void) {
  if(device != NULL) {
    return 0;
  }

  if(bd_unison_open_devices(&device, 1) != 1) {
    device = NULL;
    return -1;
  }

  return 0;
}

int bd_unison_close(void) {
  if(device == NULL) {
    return 0;
  }

  bd_unison_close_device(device);
  device = NULL;
  hid_exit();

  return 0;
}

int bd_unison_is_open(void) {
  return device != NULL;
}

int bd_unison_set_lights(uint8_t cab_state, uint8_t pad_state) {
  bd_unison_lights_t lights;
  lights.cab = cab_state;
  lights.p1_buttons = (cab_state & BD_UNISON_LIGHT_CAB_BUTTONS_P1) ? BD_UNISON_LIGHT_BTN_ALL : 0;
  lights.p2_buttons = (cab_state & BD_UNISON_LIGHT_CAB_BUTTONS_P2) ? BD_UNISON_LIGHT_BTN_ALL : 0;
  lights.pad = pad_state;

  return bd_unison_write_lights(device, &lights);
}
