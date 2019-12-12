all:
	gcc -std=c99 -o ser ser2.c -pthread
	gcc -o cli cli2.c -pthread
