SRC=bst/bst.c queue/queue.c main.c
CFLAGS+=-g -Wall
INCLUDE=-I.

all:
	gcc -o bstest $(SRC) $(CFLAGS) $(INCLUDE)
clean:
	rm bstest
