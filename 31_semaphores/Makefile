
BINARIES = fork-join rendezvous
HEADERS = common_threads.h

all: $(BINARIES)

clean:
	rm -f $(BINARIES)

fork-join: fork-join.c $(HEADERS)
	gcc -o fork-join fork-join.c -Wall -pthread

rendezvous: rendezvous.c $(HEADERS)
	gcc -o rendezvous rendezvous.c -Wall -pthread

