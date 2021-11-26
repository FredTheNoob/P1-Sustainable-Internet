CFLAGS=-g -Wall

make_file:
	gcc $(CFLAGS) -c src/lib/website.c
	gcc $(CFLAGS) -c src/lib/user.c
	gcc $(CFLAGS) -c src/lib/simulation.c
	gcc $(CFLAGS) -c src/main.c

	gcc $(CFLAGS) main.o simulation.o website.o user.o

	rm main.o
	rm simulation.o
	rm user.o
	rm website.o