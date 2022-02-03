# SPDX-License-Identifier: MIT
ifneq ($(origin CROSS_COMPILE), undefined)
export CC = $(CROSS_COMPILE)gcc
export AR = $(CROSS_COMPILE)ar
endif

all clean:
	make -C lib $@
	make -C tools $@
