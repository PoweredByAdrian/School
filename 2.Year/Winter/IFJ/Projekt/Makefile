OBJS	= ifj21.o scanner.o str.o token.o tokenstack.o symtable.o expression.o parser.o codegenerator.o
SOURCE	= ifj21.c scanner.c str.c token.c tokenstack.c symtable.c expression.c parser.c codegenerator.c
HEADER	= scanner.h str.h token.h error.h type.h tokenstack.h symtable.h expression.h parser.h codegenerator.h
ARCHIVE = ifj21.tar.gz
OUT	= ifj21
LFLAGS	 = -Wall -Werror -pedantic -DNDEBUG

all: ifj21

ifj21: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(LFLAGS)

clean:
	rm -f $(OBJS) $(OUT) $(ARCHIVE)

run: $(OUT)
	./$(OUT)

archive:
	tar czvf xgafri00.tgz $(SOURCE) $(HEADER) rozdeleni dokumentace.pdf Makefile


debug: $(OUT)
	valgrind ./$(OUT)

valgrind: $(OUT)
	valgrind ./$(OUT)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full ./$(OUT)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes ./$(OUT)
