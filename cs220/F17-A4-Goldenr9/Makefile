CFLAGS:= -g -Wall -std=c99

all: main

main: main.c functions.c
	gcc main.c -o main
	gcc functions.c -c

.PHONY: clean
clean:
	rm -fv main *.o
	rm -fv *~ \#*\# *.swp

# Rule to build a test executable
# DO NOT MODIFY
test: test.c functions.c
	$(CC) $(CFLAGS) $^ -o $@

# Rule to run a testing script and produce an output file
# DO NOT MODIFY
.PHONY: test-run
results.txt: test test.py
	/usr/bin/python3.5 test.py

# Rule to run *your* clean rule AND remove the test components
# DO NOT MODIFY
.PHONY: really-clean
really-clean: clean
	rm -fv test*
