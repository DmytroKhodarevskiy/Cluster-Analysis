CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -DNDEBUG
LIBS = -lm

cluster: cluster.c
	$(CC) $(CFLAGS) cluster.c -o cluster $(LIBS)

.PHONY: clean

clean:
	rm -f cluster
