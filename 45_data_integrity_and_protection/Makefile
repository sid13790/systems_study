
FLAGS = -Wall -g

all: check-xor check-fletcher

clean:
	rm -f check-xor check-fletcher

check-xor: check-xor.c 
	gcc -o check-xor check-xor.c $(FLAGS)

check-fletcher: check-fletcher.c 
	gcc -o check-fletcher check-fletcher.c $(FLAGS)
