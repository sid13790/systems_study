
FLAGS = -Wall -pthread -g

all: concurrent_counter better_concurrent_counter

clean:
	rm -f concurrent_counter better_concurrent_counter

concurrent_counter: concurrent_counter.c mythreads.h concurrent_counter.h
	gcc -o concurrent_counter concurrent_counter.h concurrent_counter.c $(FLAGS)

better_concurrent_counter: better_concurrent_counter.c mythreads.h better_concurrent_counter.h
	gcc -o better_concurrent_counter better_concurrent_counter.h better_concurrent_counter.c $(FLAGS)


