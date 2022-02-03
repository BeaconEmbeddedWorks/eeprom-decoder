/* SPDX-License-Identifier: MIT */
#include <ctype.h>
#include "interface.h"
#include "internals.h"
#include "id-errno.h"

static int id_iterate_array(struct id_data *data, struct id_cookie *cookie);
static int id_extract_entry(struct id_data *data, struct id_cookie *cookie);

static int id_extract_str(struct id_cookie *cookie, idenum_t type)
{
	int err, size, i;
	unsigned char byte;

	if (type != IDENUM_HEXSTR)
		id_printf("\"");

	size = extract_unsigned_pnum(cookie, 5, &err);
	if (err)
		return err;

	for (i=0; i<size; ++i) {
		byte = id_fetch_byte(cookie->offset++, &err);
		if (err)
			return err;
		if (type == IDENUM_HEXSTR) {
			id_printf("%02x", byte);
			if (i < (size-1))
				id_printf(":");
		} else {
			if (isprint(byte))
				id_printf("%c", byte);
			else
				id_printf("\\%o", byte);
		}
	}

	if (type == IDENUM_HEXSTR) {
		id_printf("\n");
	} else {
		id_printf("\"\n");
	}
	return err;
}


static int id_extract_num(struct id_cookie *cookie, idenum_t type)
{
	int err;
	unsigned int numval;

	numval = extract_unsigned_pnum(cookie, 5, &err);
	id_printf("%s%#x\n", (type == IDENUM_NEG_NUM ? "-" : ""), numval);
	return err;
}


static int id_extract_key(struct id_data *data, struct id_cookie *cookie)
{
	int err;
	int keyval;
	struct id_key key;

	keyval = extract_unsigned_pnum(cookie, 5, &err);
	if (err != ID_EOK)
		return err;
	err = id_get_key(data, keyval, &key);
	id_printf("%*s: ", key.size, key.ptr);
	return err;
}

static int id_extract_entry(struct id_data *data, struct id_cookie *cookie)
{
	idenum_t type;
	int err;

	err = id_whatis(cookie, &type);
	if (err)
		return err;

	switch(type) {
	case IDENUM_DICT:
		err = id_iterate_dict(data, cookie);
		break;
	case IDENUM_ARRAY:
		err = id_iterate_array(data, cookie);
		break;
	case IDENUM_STR:
	case IDENUM_HEXSTR:
		err = id_extract_str(cookie, type);
		break;
	case IDENUM_POS_NUM:
	case IDENUM_NEG_NUM:
		err = id_extract_num(cookie, type);
		break;
	case IDENUM_KEY:
		err = id_extract_key(data, cookie);
		break;
	default:
		err = -ID_EDOM;
		break;
	}
	return err;
}

int id_iterate_dict(struct id_data *data, struct id_cookie *cookie)
{
	unsigned int size;
	int err;
	idenum_t type;
	struct id_cookie d_cookie;

	d_cookie = *cookie;

	/* Cookie is expected to be on top of dictionary */
	err = id_whatis(cookie, &type);
	if (err != ID_EOK)
		return err;

	/* Header has to be a dictionary */
	if (type != IDENUM_DICT)
		return -ID_EINVAL;

	/* Extract size of the dictionary */
	size = extract_unsigned_pnum(&d_cookie, 5, &err);
	if (err != ID_EOK)
		return err;

	d_cookie.size = size;
	d_cookie.start_offset = d_cookie.offset;

	/* cookie->offset now is at the first key */
	while (d_cookie.offset < d_cookie.start_offset+d_cookie.size) {

		/* Extract the key */
		err = id_extract_key(data, &d_cookie);
		if (err != ID_EOK)
			return err;
		/* Next is a generic value */
		err = id_extract_entry(data, &d_cookie);
		if (err != ID_EOK)
			return err;
	}
	/* move forward to cover the dictionary */
	cookie->offset = d_cookie.offset;

	return ID_EOK;
}

int id_iterate_array(struct id_data *data, struct id_cookie *cookie)
{
	unsigned int size;
	int err;
	idenum_t type;
	struct id_cookie t_cookie = *cookie;
	struct id_cookie a_cookie;

	a_cookie = *cookie;

	/* Cookie is expected to be on top of dictionary */
	err = id_whatis(cookie, &type);
	if (err != ID_EOK)
		return err;

	/* Header has to be an array */
	if (type != IDENUM_ARRAY)
		return -ID_EINVAL;

	size = id_array_size(data, &a_cookie);
	id_printf("[ # offset %u, %d elements\n", t_cookie.offset, size);

	/* Extract size of the dictionary */
	size = extract_unsigned_pnum(&a_cookie, 5, &err);
	if (err != ID_EOK)
		return err;

	a_cookie.size = size;
	a_cookie.start_offset = a_cookie.offset;

	/* cookie->offset now is at the first key */
	while (a_cookie.offset < a_cookie.start_offset+a_cookie.size) {
		/* each entry is a generic value */
		err = id_extract_entry(data, &a_cookie);
		if (err != ID_EOK)
			return err;
	}
	/* move forward to cover the dictionary */
	cookie->offset = a_cookie.offset;

	id_printf("]\n");

	return ID_EOK;
}
