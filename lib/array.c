/* SPDX-License-Identifier: MIT */
#include "interface.h"
#include "internals.h"
#include "id-errno.h"

int id_array_size(struct id_data *data, struct id_cookie *cookie)
{
	idenum_t type;
	int err;
	int count = 0;
	unsigned int size;
	struct id_cookie a_cookie;

	a_cookie = *cookie;

	/* It has to be an array */
	err = id_whatis(&a_cookie, &type);
	if (type != IDENUM_ARRAY)
		return -ID_EINVAL;

	size = extract_unsigned_pnum(&a_cookie, 5, &err);
	if (err != ID_EOK)
		return err;

	a_cookie.size = size;
	a_cookie.start_offset = a_cookie.offset;
	while (a_cookie.offset < a_cookie.start_offset+a_cookie.size) {
		/* Get the size of the object */
		size = id_extract_size(&a_cookie, &err);
		if (err != ID_EOK)
			return err;

		/* Move the offset forward by the object size */
		a_cookie.offset += size;

		/* Increment the count */
		count++;
	}
	return count;
}
