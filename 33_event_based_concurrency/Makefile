
ALL = server

all: $(ALL)

clean:
	rm -f $(ALL) *~

server: server.c $(COMMON)
	gcc -o server server.c -Wall -O

