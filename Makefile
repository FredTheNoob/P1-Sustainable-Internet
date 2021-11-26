make_file:
	gcc -g -Wall -c src/lib/website.c
	gcc -g -Wall -c src/lib/user.c
	gcc -g -Wall -c src/lib/simulation.c
	gcc -g -Wall -c src/main.c

	gcc main.o simulation.o website.o user.o

	rm main.o
	rm simulation.o
	rm user.o
	rm website.o