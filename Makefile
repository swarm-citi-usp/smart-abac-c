main_src = $(filter-out tests.c, $(wildcard *.c))
test_src = $(filter-out main.c, $(wildcard *.c))
main_obj = $(main_src:.c=.o)
test_obj = $(test_src:.c=.o)

LDFLAGS = -ljansson

main: $(main_obj)
	$(CC) -o $@ $^ $(LDFLAGS)

test: $(test_obj)
	$(CC) -o $@ $^ $(LDFLAGS) && ./test

.PHONY: clean
clean:
	rm -f $(main_obj) main
	rm -f $(test_obj) test


run: clean main
	./main

