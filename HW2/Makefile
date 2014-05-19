CFLAGS=-g -Wall
CC=gcc
SRCS=tokenizer.c shell.c utils.c
OBJS=tokenizer.o shell.o utils.o
LDFLAGS=
LIBS=

all:    shell

$(SRCS):
	$(CC) $(CFLAGS) -c $*.c

shell: $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o shell $(OBJS)

clean:
	rm -f *.o shell
