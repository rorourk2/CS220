# Rule to build a test executable
test: test.c sorter.c toggle.S
	$(eval CFLAGS+=-g -Wall -std=c99)
	$(eval LDFLAGS+= -lcunit)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to run *your* clean rule
# AND remove the test executable and output results
.PHONY: really-clean
really-clean: clean
	rm -fv test
