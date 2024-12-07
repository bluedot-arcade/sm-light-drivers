#include "unilight.h"
#include <hidapi.h>

#define BD_UNILIGHT_VID 0x04BD
#define BD_UNILIGHT_PID 0x00BD

static uint8_t cab_lights_state[3];
static uint8_t pad_lights_state[3];
static hid_device *handle = NULL;

static void bd_unilight_init_cab_lights(void) {
  cab_lights_state[0] = 0x00;
  cab_lights_state[1] = 0x01;
  cab_lights_state[2] = 0x00;
}

static void bd_unilight_init_pad_lights(void) {
  pad_lights_state[0] = 0x00;
  pad_lights_state[1] = 0x02;
  pad_lights_state[2] = 0x00;
}

int bd_unilight_open(void) {
  if(handle != NULL) {
    return 0;
  }

  hid_init();
  handle = hid_open(BD_UNILIGHT_VID, BD_UNILIGHT_PID, NULL);
  if(handle == NULL) {
    return -1;
  }

  bd_unilight_init_cab_lights();
  bd_unilight_init_pad_lights();
  return 0;
}

int bd_unilight_close(void) {
  if(handle == NULL) {
    return 0;
  }

  hid_close(handle);
  handle = NULL;
  hid_exit();

  return 0;
}

int bd_unilight_is_open(void) {
  return handle != NULL;
}

int bd_unilight_set_cab_lights(uint8_t state) {
  if(handle == NULL) {
    return -1;
  }

  cab_lights_state[2] = state;

  if(hid_write(handle, cab_lights_state, sizeof(cab_lights_state)) < 0) {
    return -1;
  }

  return 0;
}

int bd_unilight_set_pad_lights(uint8_t state) {
  if(handle == NULL) {
    return -1;
  }

  pad_lights_state[2] = state;

  if(hid_write(handle, pad_lights_state, sizeof(pad_lights_state)) < 0) {
    return -1;
  }

  return 0;
}