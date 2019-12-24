
FLAGS = -Wall -pthread -g

all: concurrent_counter better_concurrent_counter hand_over_hand_ll

clean:
	rm -f concurrent_counter better_concurrent_counter hand_over_hand_ll

concurrent_counter: concurrent_counter.c mythreads.h concurrent_counter.h
	gcc -o concurrent_counter concurrent_counter.h concurrent_counter.c $(FLAGS)

better_concurrent_counter: better_concurrent_counter.c mythreads.h better_concurrent_counter.h
	gcc -o better_concurrent_counter better_concurrent_counter.h better_concurrent_counter.c $(FLAGS)

hand_over_hand_ll: hand_over_hand_ll.c mythreads.h hand_over_hand_ll.h
	gcc -o hand_over_hand_ll hand_over_hand_ll.h hand_over_hand_ll.c $(FLAGS)


