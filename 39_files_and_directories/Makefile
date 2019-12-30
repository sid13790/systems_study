
FLAGS = -Wall -g

all: stat list tail recursearch

clean:
	rm -f stat list tail recursearch

stat: stat.c
	gcc -o stat stat.c $(FLAGS)

list: list.c
	gcc -o list list.c $(FLAGS)

tail: tail.c
	gcc -o tail tail.c $(FLAGS)

recursearch: recursearch.c
	gcc -o recursearch recursearch.c $(FLAGS)