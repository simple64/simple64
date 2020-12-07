#if !defined(OSAL_KEYS_H)
#define OSAL_KEYS_H

#include "osal_export.h"
#include "keycode/keycode.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void CALL osal_keys_init();

EXPORT void CALL osal_keys_quit();

EXPORT void CALL osal_keys_update_state();

EXPORT unsigned int CALL osal_is_key_pressed(unsigned int _key, unsigned int _mask);

EXPORT const char * CALL osal_keycode_name(unsigned int _hidCode);

#ifdef __cplusplus
}
#endif

#endif /* #define OSAL_KEYS_H */

