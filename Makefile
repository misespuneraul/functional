CC=gcc
CFLAGS=-Wall -Wextra -Werror -g

.PHONY: build clean

build: functional

functional: main.o functional.o tests.o tasks.o
	$(CC) $(CFLAGS) -o ../$@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

functional.o: functional.c functional.h
	$(CC) $(CFLAGS) -c -o $@ $<

tests.o: tests.c tests.h
	$(CC) $(CFLAGS) -c -o $@ $<

tasks.o: tasks.c tasks.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o functional