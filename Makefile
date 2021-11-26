make_file:
	gcc -g -c src/lib/website.c
	gcc -g -c src/lib/user.c
	gcc -g -c src/lib/simulation.c
	gcc -g -c src/main.c

	gcc -g main.o simulation.o website.o user.o

	rm main.o
	rm simulation.o
	rm user.o
	rm website.o