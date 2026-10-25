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
 * - BD_UNISON_LIGHT_CAB_BUTTONS_P1
 * - BD_UNISON_LIGHT_CAB_BUTTONS_P2
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
