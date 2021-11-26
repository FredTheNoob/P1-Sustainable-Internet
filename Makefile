make_file:
	gcc -c src/lib/website.c
	gcc -c src/lib/user.c
	gcc -c src/lib/simulation.c
	gcc -c src/main.c

	gcc main.o simulation.o website.o user.o

	rm main.o
	rm simulation.o
	rm user.o
	rm website.o