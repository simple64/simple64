#include "osal_keys.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void CALL osal_keys_init()
{
}

EXPORT void CALL osal_keys_quit()
{
}

EXPORT void CALL osal_keys_update_state()
{
}

EXPORT unsigned int CALL osal_is_key_pressed(unsigned int _key, unsigned int _mask)
{
	return 0;
}

EXPORT unsigned int CALL osal_virtual_key_to_hid(unsigned int _key, unsigned int _hidKey)
{
	return 0;
}

EXPORT const char * CALL osal_keycode_name(unsigned int _hidCode)
{
	return 0;
}

#ifdef __cplusplus
}
#endif
