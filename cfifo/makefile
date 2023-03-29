test : cfifo.c cfifo_unit.c
	gcc -std=c11 -Wall -Werror -pedantic -g -o test cfifo.c cfifo_unit.c -lcunit
	valgrind --leak-check=yes ./test
	rm -rf test 

cfifo_example: cfifo.c cfifo_example.c
	gcc -std=c11 -Wall -Werror -pedantic -o cfifo_example cfifo.c cfifo_example.c

cfifo_unit: cfifo.c cfifo_unit.c
	gcc -std=c11 -Wall -Werror -pedantic -o cfifo_unit cfifo.c cfifo_unit.c -lcunit

clean: 
	rm -rf cfifo_example cfifo_unit test a.out