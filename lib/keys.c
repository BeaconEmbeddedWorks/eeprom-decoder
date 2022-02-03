/* SPDX-License-Identifier: MIT */
#include <string.h>
#include "interface.h"
#include "internals.h"
#include "id-errno.h"
#include "crc-15.h"

/* Get a list of key strings in "id_keys" */
#define ID_KEY_STRINGS
#include "keys.h"

int id_get_key(struct id_data *data, int keyval, struct id_key *key)
{
	if (!key)
		return -ID_EINVAL;

	if (keyval < 0)
		return -ID_ERANGE;

	if (keyval < sizeof(id_keys)/sizeof(id_keys[0])) {
		key->ptr = (unsigned char *)id_keys[keyval];
		key->size = strlen(id_keys[keyval]);
		return ID_EOK;
	}

	return -ID_ERANGE;
}
