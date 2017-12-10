OPTS = -Wall -Wextra -std=c99
TYPE_OBJS = integer.o
UTIL_OBJS = scanner.o kruskal.o
STRUCT_OBJS = da.o set.o bst.o rbt.o queue.o cda.o
OBJECT_BUNDLE = $(TYPE_OBJS) $(STRUCT_OBJS) $(UTIL_OBJS)

kruskal: $(OBJECT_BUNDLE)
	gcc $(OPTS) $(OBJECT_BUNDLE) -o ./kruskal

test: kruskal
	./kruskal g1

kruskal.o: kruskal.c
	gcc $(OPTS) kruskal.c -c

set.o: set.c set.h
	gcc $(OPTS) set.c -c

da.o: da.c da.h
	gcc $(OPTS) da.c -c

scanner.o: scanner.c scanner.h
	gcc $(OPTS) scanner.c -c

integer.o: integer.c integer.h
	gcc $(OPTS) integer.c -c

rbt.o: rbt.c rbt.h
	gcc $(OPTS) rbt.c -c

bst.o: bst.c bst.h
	gcc $(OPTS) bst.c -c

queue.o: queue.c queue.h
	gcc $(OPTS) queue.c -c

cda.o: cda.c cda.h
	gcc $(OPTS) cda.c -c


clean:
	rm -rf *.o ./a.out kruskal