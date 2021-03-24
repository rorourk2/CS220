all: estimator

estimator:estimator.c nop.S ret.S
	gcc estimator.c nop.S ret.S -std=c89 -g -c

.PHONY: clean
clean: 
	rm -fv sorter estimator ret.o nop.o estimator.o sorter.o toggle.o
	rm -fv *~ #*#

include test.make
