CFLAGS=-g -Wall -std=c99 -g -D_XOPEN_SOURCE
CC=gcc
SRCS= shell.c tokenizer.c utils.c LinkedList.c redirect.c modHash.c process.c errCheck.c
OBJS= shell.o tokenizer.o utils.o LinkedList.o redirect.o modHash.o process.o errCheck.c
LDFLAGS=
LIBS=

all:    shell

$(SRCS):
	$(CC) $(CFLAGS) -c $*.c

shell: $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o shell $(OBJS)

clean:
	rm -f *.o test
