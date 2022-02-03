/* SPDX-License-Identifier: MIT */
#include "../id_config.h"
#if ID_COMPILER_FUNCTIONAL_TEST_MODE
#include "stdio.h"
#else
#include <stdio.h>
#endif
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "../lib/interface.h"
#include "../lib/id-errno.h"
#include "lib-tool.h"

static unsigned char *data_buf;
static unsigned int data_buf_size;

unsigned char id_fetch_byte(int offset, int *oor)
{
	if (!data_buf) {
		fprintf(stderr, "%s: attempt to read data before buffer initialized\n", __FUNCTION__);
		exit(-4);
	}

	if (offset >= data_buf_size) {
		fprintf(stderr, "Attempt to read past end of buffer (offset %d >= size %d\n", offset, data_buf_size);
		*oor = -ID_ERANGE;
		return 0;  /* Force upper laer to recover */
	}

	*oor = ID_EOK;
	return data_buf[offset];
}

void initialize_data_buffer(char *file)
{
	FILE *ifile;
	struct stat sb;
	int size;
	int ret;

	ret = stat(file, &sb);
	if (ret < 0) {
		fprintf(stderr, "%s: failed to stat '%s'\n", __FUNCTION__, file);
		exit(-1);
	}

	data_buf = malloc(sb.st_size);
	if (!data_buf) {
		fprintf(stderr, "%s: failed to malloc %lu bytes for buffer\n", __FUNCTION__, sb.st_size);
		exit(-1);
	}
	ifile = fopen(file, "r");
	if (!ifile) {
		fprintf(stderr, "%s: Failed to open '%s' for reading\n", __FUNCTION__, file);
		exit(-1);
	}
	size = fread(data_buf, 1, sb.st_size, ifile);
	if (size != sb.st_size) {
		fprintf(stderr, "%s: Failed to read %lu bytes (got %u) for reading, errno %d\n", __FUNCTION__, sb.st_size, size, errno);
		exit(-1);
	}
	fclose(ifile);
	data_buf_size = sb.st_size;
}

int id_printf(const char *fmt, ...)
{
	int ret;
	va_list vl;

	va_start(vl,  fmt);
	ret = vprintf(fmt, vl);
	va_end(vl);

	return ret;
}

void id_error(const char *fmt, ...)
{
	va_list vl;

	va_start(vl, fmt);
#if ID_COMPILER_FUNCTIONAL_TEST_MODE
	ftcfprintf(stderr, fmt, vl);
#else
	vfprintf(stderr, fmt, vl);
#endif
	va_end(vl);
}
