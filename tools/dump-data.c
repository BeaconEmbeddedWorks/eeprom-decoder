/* SPDX-License-Identifier: MIT */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "../lib/interface.h"
#include "lib-tool.h"

void dump_data(struct id_data *data)
{
	struct id_cookie cookie;
	extern int id_iterate_dict(struct id_data *data, struct id_cookie *cookie);

	id_init_cookie(data, &cookie);
	id_iterate_dict(data, &cookie);
}
