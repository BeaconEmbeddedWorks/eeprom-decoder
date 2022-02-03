/* SPDX-License-Identifier: MIT */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#undef DEBUG

#ifdef DEBUG
#define DPRINTF(fmt, args...) printf(fmt, ## args)
#else
#define DPRINTF(fmt, ...)
#endif

#endif
