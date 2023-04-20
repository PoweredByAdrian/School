LD_LIBRARY_PATH="." ./wordcount-dynamic file

CC=gcc
CFLAGS= -g -std=c99 -pedantic -Wall -Wextra
OBJECTFILES=wordcount.o hash_function.o htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free.o htab_init.o htab_lookup_add.o htab_move.o htab_size.o 


all: tail wordcount wordcount-dynamic

tail: tail.c
		$(CC) $(CFLAGS) tail.c -o tail

wordcount: wordcount.o libhtab.a
		$(CC) $(CFLAGS) -o wordcount wordcount.o -L . libhtab.a

wordcount-dynamic: wordcount.o libhtab.so
	$(CC) $(CFLAGS) -o wordcount-dynamic wordcount.o libhtab.so
	
# Staticka kniznica
libhtab.a: $(OBJECTFILES)
	ar -rcs libhtab.a $(OBJECTFILES)
	
# Dynamicka kniznica
libhtab.so:	$(OBJECTFILES)
	$(CC) $(CFLAGS) -shared -fPIC -o libhtab.so $(OBJECTFILES)

wordcount.o: wordcount.c
		$(CC) $(CFLAGS) -fPIC -c wordcount.c

hash_function.o: hash_function.c
		$(CC) $(CFLAGS) -fPIC -c hash_function.c

htab_init.o: htab_init.c
		$(CC) $(CFLAGS) -fPIC -c htab_init.c

htab_move.o: htab_move.c htab_lookup_add.o htab_find.o htab_clear.o
		$(CC) $(CFLAGS) -fPIC -c htab_move.c

htab_size.o: htab_size.c
		$(CC) $(CFLAGS) -fPIC -c htab_size.c

htab_bucket_count.o: htab_bucket_count.c
		$(CC) $(CFLAGS) -fPIC -c htab_bucket_count.c

htab_lookup_add.o: htab_lookup_add.c hash_function.o htab_find.o
		$(CC) $(CFLAGS) -fPIC -c htab_lookup_add.c

htab_clear.o: htab_clear.c
		$(CC) $(CFLAGS) -fPIC -c htab_clear.c

htab_erase.o: htab_erase.c hash_function.o
		$(CC) $(CFLAGS) -fPIC -c htab_erase.c

htab_find.o: htab_find.c hash_function.o
		$(CC) $(CFLAGS) -fPIC -c htab_find.c

htab_for_each.o: htab_for_each.c
		$(CC) $(CFLAGS) -fPIC -c htab_for_each.c

htab_free.o: htab_free.c htab_clear.o
		$(CC) $(CFLAGS) -fPIC -c htab_free.c