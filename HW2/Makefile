CFLAGS=-g -Wall -std=c99 -g
CC=gcc
SRCS= shell.c tokenizer.c utils.c LinkedList.c redirect.c modHash.c
OBJS= shell.o tokenizer.o utils.o LinkedList.o redirect.o modHash.o
LDFLAGS=
LIBS=

all:    shell

$(SRCS):
	$(CC) $(CFLAGS) -c $*.c

shell: $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o shell $(OBJS)

clean:
	rm -f *.o test
