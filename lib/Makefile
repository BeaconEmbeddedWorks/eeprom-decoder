# SPDX-License-Identifier: MIT
LIB=id-lib.a
OBJS=startup.o extract.o keys.o dump.o type.o crc-15.o \
	array.o size.o
CFLAGS=-O2 -Wall -Werror -Wmissing-prototypes

all: $(LIB)

type.o startup.o size.o keys.o extract.o dump.o dict.o: interface.h

$(LIB):	$(OBJS)
	$(AR) -rv $(LIB) $(OBJS)

keys.o: internals.h id-errno.h crc-15.h keys.h

clean:
	rm -f $(LIB) $(OBJS)
