
ifeq ($(OS),Windows_NT)
	REMOVE = del /f
else
	REMOVE = rm
endif

make_file:
	gcc -c src/lib/website.c
	gcc -c src/lib/user.c
	gcc -c src/lib/simulation.c
	gcc -c src/main.c

	# gcc main.o simulation.o website.o user.o

	$(REMOVE) main.o
	$(REMOVE) simulation.o
	$(REMOVE) user.o
	$(REMOVE) website.o