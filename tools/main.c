/* SPDX-License-Identifier: MIT */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "../lib/interface.h"
#include "lib-tool.h"
#include "../lib/id-errno.h"

static const struct option long_options[] = {
	{ "debug", 1, 0, 'd' },
	{ "input", 1, 0, 'i' },
	{ NULL, 0, 0, 0 },
};

int id_debug_mask = 0;

int main(int argc, char *argv[])
{
	int opt;
	char *ifile=NULL;
	extern char *optarg;
	int long_idx;
	char *p;
	int ret;
	struct id_data id_data;

	while ((opt = getopt_long(argc, argv, "d::i:", long_options, &long_idx)) != -1) {
		switch(opt) {
		case 'd':
			if (optarg)
				id_debug_mask = strtol(optarg, &p, 0);
			break;
		case 'i':
			ifile = optarg;
			break;
		default: /* '?' */
			fprintf(stderr, "Usage: %s [-i input-file] [-o output-file] [-d debug level]\n", argv[0]);
			exit(-1);
		}
	}

	if (!ifile) {
		fprintf(stderr, "%s: No input data file specified\n", argv[0]);
		exit(-2);
	}

	initialize_data_buffer(ifile);

	ret = id_startup(&id_data);
	if (ret != ID_EOK) {
		fprintf(stderr, "%s: id_startup returned %d\n", __FUNCTION__, ret);
		exit(-1);
	}

	dump_data(&id_data);

	return 0;
}
