#ifndef INCLUDE_UNISON_H_
#define INCLUDE_UNISON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define BD_UNISON_LIGHT_CAB_MARQUEE_UP_LEFT     0x01
#define BD_UNISON_LIGHT_CAB_MARQUEE_UP_RIGHT    0x02
#define BD_UNISON_LIGHT_CAB_MARQUEE_LR_LEFT     0x04
#define BD_UNISON_LIGHT_CAB_MARQUEE_LR_RIGHT    0x08
#define BD_UNISON_LIGHT_CAB_BUTTONS_P1          0x10
#define BD_UNISON_LIGHT_CAB_BUTTONS_P2          0x20
#define BD_UNISON_LIGHT_CAB_BASS_LEFT           0x40
#define BD_UNISON_LIGHT_CAB_BASS_RIGHT          0x80

/* The UNISON board drives a single pad. */
#define BD_UNISON_LIGHT_PAD_LEFT                0x01
#define BD_UNISON_LIGHT_PAD_RIGHT               0x02
#define BD_UNISON_LIGHT_PAD_UP                  0x04
#define BD_UNISON_LIGHT_PAD_DOWN                0x08

/* Cabinet button lights of a player. */
#define BD_UNISON_LIGHT_BTN_MENU_LEFT           0x01
#define BD_UNISON_LIGHT_BTN_MENU_RIGHT          0x02
#define BD_UNISON_LIGHT_BTN_MENU_UP             0x04
#define BD_UNISON_LIGHT_BTN_MENU_DOWN           0x08
#define BD_UNISON_LIGHT_BTN_START               0x10
#define BD_UNISON_LIGHT_BTN_ALL                 0x1F

/**
 * @brief Full light state of a UNISON board.
 */
typedef struct {
  /** Bitmask of BD_UNISON_LIGHT_CAB_MARQUEE_* and BD_UNISON_LIGHT_CAB_BASS_*.
   *  The BD_UNISON_LIGHT_CAB_BUTTONS_* bits are ignored, use p1_buttons and p2_buttons. */
  uint8_t cab;
  /** Bitmask of BD_UNISON_LIGHT_BTN_* for player 1. */
  uint8_t p1_buttons;
  /** Bitmask of BD_UNISON_LIGHT_BTN_* for player 2. */
  uint8_t p2_buttons;
  /** Bitmask of BD_UNISON_LIGHT_PAD_*. */
  uint8_t pad;
} bd_unison_lights_t;

/**
 * @brief An open UNISON board.
 */
typedef struct bd_unison_device bd_unison_device_t;

/**
 * @brief Count the connected UNISON boards.
 * @return The number of connected boards.
 */
int bd_unison_count_devices(void);

/**
 * @brief Open the connected UNISON boards.
 * Boards are sorted by device path, so the order is stable as long as
 * the boards stay connected to the same USB ports.
 * @param devices Array receiving the opened boards.
 * @param max_devices Size of the devices array.
 * @return The number of opened boards.
 */
int bd_unison_open_devices(bd_unison_device_t **devices, int max_devices);

/**
 * @brief Close a board opened with bd_unison_open_devices.
 */
void bd_unison_close_device(bd_unison_device_t *device);

/**
 * @brief Write the full light state to a board.
 * Nothing is sent if the state did not change since the last successful write.
 * @return 0 on success, -1 on failure (e.g. the board was disconnected).
 */
int bd_unison_write_lights(bd_unison_device_t *device, const bd_unison_lights_t *lights);

/**
 * @brief Open the connection to the UNISON device.
 * @return 0 on success, -1 on failure.
 */
int bd_unison_open(void);

/**
 * @brief Close the connection to the UNISON device.
 * @return 0 on success, -1 on failure.
 */
int bd_unison_close(void);

/**
 * @brief Check if the connection to the UNISON device is open.
 * @return 1 if the connection is open, 0 if it is closed.
 */
int bd_unison_is_open(void);

/**
 * @brief Set the state of all the lights.
 * The board receives cab and pad lights in a single packet, so both
 * states are always sent together. Nothing is sent if the state did not
 * change since the last successful write.
 * @param cab_state The state of the cab lights.
 * It is a bitmask of the following values:
 * - BD_UNISON_LIGHT_CAB_MARQUEE_UP_LEFT
 * - BD_UNISON_LIGHT_CAB_MARQUEE_UP_RIGHT
 * - BD_UNISON_LIGHT_CAB_MARQUEE_LR_LEFT
 * - BD_UNISON_LIGHT_CAB_MARQUEE_LR_RIGHT
 * - BD_UNISON_LIGHT_CAB_BUTTONS_P1 (all P1 button lights)
 * - BD_UNISON_LIGHT_CAB_BUTTONS_P2 (all P2 button lights)
 * - BD_UNISON_LIGHT_CAB_BASS_LEFT
 * - BD_UNISON_LIGHT_CAB_BASS_RIGHT
 * @param pad_state The state of the pad lights.
 * It is a bitmask of the following values:
 * - BD_UNISON_LIGHT_PAD_LEFT
 * - BD_UNISON_LIGHT_PAD_RIGHT
 * - BD_UNISON_LIGHT_PAD_UP
 * - BD_UNISON_LIGHT_PAD_DOWN
 * @return 0 on success, -1 on failure.
 */
int bd_unison_set_lights(uint8_t cab_state, uint8_t pad_state);

#ifdef __cplusplus
}
#endif

#endif // INCLUDE_UNISON_H_
