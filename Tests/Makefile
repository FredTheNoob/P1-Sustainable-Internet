SRC=$(wildcard Files/*.c ../src/lib/*.c)

test: $(SRC)
	gcc -g AllTests.c CuTest.c $^ -o ../Tests.out