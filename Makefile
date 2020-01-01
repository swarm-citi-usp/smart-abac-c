
main: clean
	cc -g -o main main.c abac_them.h abac_them.c pdp.h util.h -ljansson

run: main
	./main

test: clean
	cc -g -o tests tests.c abac_them.h abac_them.c pdp.h util.h -ljansson && ./tests

clean:
	rm -f *.o
	rm -f main

