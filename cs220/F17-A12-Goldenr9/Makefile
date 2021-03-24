all: loader hellosh

loader: loader.c
	gcc loader.c -o loader -m32 -Wl,-z,execstack

hellosh: hellosh.c
	gcc hellosh.c -o hellosh
.PHONY: clean
clean:
	rm -f loader hellosh
	rm -f *.o
	rm -f *~ \#*\# *.swp
