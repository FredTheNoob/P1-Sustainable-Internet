make_file:
	mkdir build
	gcc -c src/lib/website.c -o build/website.o
	gcc -c src/lib/user.c -o build/user.o
	gcc -c src/lib/simulation.c -o build/simulation.o
	gcc -c src/main.c -o build/main.o
	gcc build/main.o build/simulation.o build/website.o build/user.o
	rmdir /Q /S build