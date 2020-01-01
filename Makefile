
main:
	cc -g -o main main.c pdp.h util.h -ljansson

test:
	cc -g -o tests tests.c pdp.h util.h -ljansson && ./tests

clean:
	rm -f *.o
	rm -f main
