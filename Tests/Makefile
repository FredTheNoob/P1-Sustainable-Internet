SRC=$(wildcard Files/*.c ../src/lib/*.c)

test: $(SRC)
	gcc AllTests.c CuTest.c $^ -o ../Tests.out