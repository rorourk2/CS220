# OPTIONAL: Set these variables as required.
CFLAGS := -g -Wall -std=c99
LDFLAGS :=
LDLIBS :=

all: static-driver

# OPTIONAL: COMPLETE THIS TARGET
# This target produces an object file for making libraries.
parser.o : parser.c parser.h
	gcc $(CFLAGS) -fpic -I$(PWD) -c parser.c

# COMPLETE THIS TARGET
# This target produces the dynamic library.
lib/libparser.so: lib/libparser.a
	gcc $(CFLAGS) -shared -o ./lib/libparser.so parser.o

# COMPLETE THIS TARGET
# This target produces the executable using the dynamic library
driver:  driver.c
	gcc $(CFLAGS) driver.c -o driver -I$(PWD) -L$(PWD)/lib -lparser
	export LD_LIBRARY_PATH=$(PWD)/lib

# This target produces the executable using static library
static-driver: driver.c lib/libparser.a
	$(CC) $(CFLAGS) driver.c -I$PWD lib/libparser.a -o static-driver

.PHONY: clean
clean:
	rm -fv static-driver driver
	rm -fv lib/libparser.so
	rm -fv *.o *.txt *.bin
	rm -fv *~ \#*\# *.swp


# Add testing targets. See the file.
# Doesn't currently do anything.
include test.make
