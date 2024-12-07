#ifndef INCLUDE_UNILIGHT_H_
#define INCLUDE_UNILIGHT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_LEFT   0x01
#define BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_RIGHT  0x02
#define BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_LEFT   0x04
#define BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_RIGHT  0x08
#define BD_UNILIGHT_LIGHT_CAB_BUTTONS_P1        0x10
#define BD_UNILIGHT_LIGHT_CAB_BUTTONS_P2        0x20
#define BD_UNILIGHT_LIGHT_CAB_BASS              0x40
#define BD_UNILIGHT_LIGHT_CAB_OPTIONAL          0x80

#define BD_UNILIGHT_LIGHT_PAD_P1_LEFT           0x01
#define BD_UNILIGHT_LIGHT_PAD_P1_RIGHT          0x02
#define BD_UNILIGHT_LIGHT_PAD_P1_UP             0x04
#define BD_UNILIGHT_LIGHT_PAD_P1_DOWN           0x08
#define BD_UNILIGHT_LIGHT_PAD_P2_LEFT           0x10
#define BD_UNILIGHT_LIGHT_PAD_P2_RIGHT          0x20
#define BD_UNILIGHT_LIGHT_PAD_P2_UP             0x40
#define BD_UNILIGHT_LIGHT_PAD_P2_DOWN           0x80

/**
 * @brief Open the connection to the unilight device.
 * @return 0 on success, -1 on failure.
 */
int bd_unilight_open(void);

/**
 * @brief Close the connection to the unilight device.
 * @return 0 on success, -1 on failure.
 */
int bd_unilight_close(void);

/**
 * @brief Check if the connection to the unilight device is open.
 * @return 1 if the connection is open, 0 if it is closed.
 */
int bd_unilight_is_open(void);

/**
 * @brief Set the state of the cab lights.
 * @param state The state of the cab lights. 
 * It is a bitmask of the following values:
 * - BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_LEFT
 * - BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_RIGHT
 * - BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_LEFT
 * - BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_RIGHT
 * - BD_UNILIGHT_LIGHT_CAB_BUTTONS_P1
 * - BD_UNILIGHT_LIGHT_CAB_BUTTONS_P2
 * - BD_UNILIGHT_LIGHT_CAB_BASS
 * - BD_UNILIGHT_LIGHT_CAB_OPTIONAL
 * @return 0 on success, -1 on failure.
 */
int bd_unilight_set_cab_lights(uint8_t state);

/**
 * @brief Set the state of the pad lights.
 * @param state The state of the pad lights. 
 * It is a bitmask of the following values:
 * - BD_UNILIGHT_LIGHT_PAD_P1_LEFT
 * - BD_UNILIGHT_LIGHT_PAD_P1_RIGHT
 * - BD_UNILIGHT_LIGHT_PAD_P1_UP
 * - BD_UNILIGHT_LIGHT_PAD_P1_DOWN
 * - BD_UNILIGHT_LIGHT_PAD_P2_LEFT
 * - BD_UNILIGHT_LIGHT_PAD_P2_RIGHT
 * - BD_UNILIGHT_LIGHT_PAD_P2_UP
 * - BD_UNILIGHT_LIGHT_PAD_P2_DOWN
 * @return 0 on success, -1 on failure.
 */
int bd_unilight_set_pad_lights(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif // INCLUDE_UNILIGHT_H_