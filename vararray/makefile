test: vararray.c vararray_unit.c
	gcc -std=c11 -Wall -Werror -pedantic -g -o test vararray.c vararray_unit.c -lcunit
	valgrind --leak-check=yes ./test
	rm -rf test

vararray_example: vararray.c vararray_example.c
	gcc -std=c11 -Wall -Werror -pedantic -o vararray_example vararray.c vararray_example.c

vararray_unit: vararray.c vararray_unit.c
	gcc -std=c11 -Wall -Werror -pedantic -o vararray_unit vararray.c vararray_unit.c -lcunit

clean: 
	rm -rf vararray_example vararray_unit test