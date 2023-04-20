# Makefile
# Riešenie IJC-DU1, 24.3.2021
# Autor: Adrián Horváth, FIT


CC=gcc
CFLAGS= -O2 -g -std=c11 -pedantic -Wall -Wextra
SOURCE=primes.o error.o eratosthenes.o
SOURCE2=error.o eratosthenes.o 

all: primes

primes: $(SOURCE) bitset.h
	$(CC) $(CFLAGS) $(SOURCE) -o primes -lm
primes.o: primes.c
	$(CC) -c primes.c
error.o: error.c error.h
	$(CC) -c error.c
eratosthenes.o: eratosthenes.c eratosthenes.h
	$(CC) -c eratosthenes.c
run: primes
	./primes
clean:
	rm primes
	rm primes-i
	rm *.o
zip:
	zip xhorva14.zip *.c *.h Makefile