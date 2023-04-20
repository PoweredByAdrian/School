# Makefile
# Reseni IJC-DU2, 20.4. 2019
# Autor: Dominik Nejedly, FIT
# Slouzi pro preklad danych souboru

# prekladace
CC = gcc
CPP = g++

# parametry prekladu
CFLAGS = -std=c99 -pedantic -Wall -Wextra
CPPFLAGS = -std=c++11 -pedantic -Wall

# cesta k dynamicke sdilene knihovne
LD_LIBRARY_PATH = "."

# vsechny objektove soubory nalezici knihovne rozptylove tabulky
HTAB_OBJECT_FILES = htab_hash_function.o htab_init.o htab_move.o htab_size.o htab_bucket_count.o htab_lookup_add.o htab_begin.o htab_end.o htab_iterator_next.o inline.o htab_iterator_get_key.o htab_iterator_get_value.o htab_iterator_set_value.o htab_clear.o htab_free.o

# preklad a vytvoreni danych programu
all: tail tail2 wordcount wordcount-dynamic

# 1) ============================================

tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail

tail2: tail2.cc
	$(CPP) $(CPPFLAGS) -g tail2.cc -o tail2

# 2) ============================================

wordcount: libhtab.a io.o wordcount.o
	$(CC) $(CFLAGS) io.o wordcount.o libhtab.a -o wordcount

wordcount-dynamic: libhtab.so io.o wordcount.o
	$(CC) $(CFLAGS) -Wl,-rpath=$(LD_LIBRARY_PATH) -L. io.o wordcount.o libhtab.so -o wordcount-dynamic

wordcount.o: wordcount.c
	$(CC) $(CFLAGS) wordcount.c -c

io.o: io.c
	$(CC) $(CFLAGS) io.c -c

libhtab.a: $(HTAB_OBJECT_FILES)
	ar -rc libhtab.a $(HTAB_OBJECT_FILES)

libhtab.so: $(HTAB_OBJECT_FILES)
	$(CC) -shared -fPIC $(HTAB_OBJECT_FILES) -o libhtab.so

htab_hash_function.o: htab_hash_function.c
	$(CC) $(CFLAGS) -fPIC htab_hash_function.c -c

htab_init.o: htab_init.c
	$(CC) $(CFLAGS) -fPIC htab_init.c -c

htab_move.o: htab_move.c
	$(CC) $(CFLAGS) -fPIC htab_move.c -c

htab_size.o: htab_size.c
	$(CC) $(CFLAGS) -fPIC htab_size.c -c

htab_bucket_count.o: htab_bucket_count.c
	$(CC) $(CFLAGS) -fPIC htab_bucket_count.c -c

htab_lookup_add.o: htab_lookup_add.c
	$(CC) $(CFLAGS) -fPIC htab_lookup_add.c -c

htab_begin.o: htab_begin.c
	$(CC) $(CFLAGS) -fPIC htab_begin.c -c

htab_end.o: htab_end.c
	$(CC) $(CFLAGS) -fPIC htab_end.c -c

htab_iterator_next.o: htab_iterator_next.c
	$(CC) $(CFLAGS) -fPIC htab_iterator_next.c -c

inline.o: inline.c
	$(CC) $(CFLAGS) -fPIC inline.c -c

htab_iterator_get_key.o: htab_iterator_get_key.c
	$(CC) $(CFLAGS) -fPIC htab_iterator_get_key.c -c

htab_iterator_get_value.o: htab_iterator_get_value.c
	$(CC) $(CFLAGS) -fPIC htab_iterator_get_value.c -c

htab_iterator_set_value.o: htab_iterator_set_value.c
	$(CC) $(CFLAGS) -fPIC htab_iterator_set_value.c -c

htab_clear.o: htab_clear.c
	$(CC) $(CFLAGS) -fPIC htab_clear.c -c

htab_free.o: htab_free.c
	$(CC) $(CFLAGS) -fPIC htab_free.c -c

# -----------------------------------------------
# Prelozi dane moduly s definovanym makrem.
# Pro zapojeni prelozit jeste jednou pomoci make.

TEST_MOVE_FUNCTION: wordcount.c
	$(CC) $(CFLAGS) -DTEST wordcount.c -c

TEST_DIFFERENT_HTAB: htab_hash_function.c
	$(CC) $(CFLAGS) -DHASH_TEST htab_hash_function.c -c

# -----------------------------------------------

# smazani vsech vytvorenych souboru
clean:
	rm -f tail tail2 libhtab.a libhtab.so wordcount wordcount-dynamic *.o

# zabaleni
pack:
	zip xnejed09.zip *.c *.cc *.h Makefile
