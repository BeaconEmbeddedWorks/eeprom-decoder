# SPDX-License-Identifier: MIT
EXEC=beacon-eeprom-decoder
OBJS=main.o lib-interface.o dump-data.o builtin-keys.o
CFLAGS=-g -Wall  -Wmissing-prototypes

LIBS=../lib/id-lib.a

all: $(EXEC)

$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

clean:
	rm -f $(EXEC) $(OBJS)
