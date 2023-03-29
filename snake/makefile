CURSESLIBS=$(shell pkg-config --libs ncurses)
snake: ../vararray/vararray.c util.c ../cfifo/cfifo.c screen.c snake_object.c border_object.c food_object.c worm_hole_object.c square_wall_object.c snake_game.c main.c
	gcc -std=c11 -Wall -Werror -pedantic ../vararray/vararray.c util.c ../cfifo/cfifo.c screen.c snake_object.c border_object.c food_object.c worm_hole_object.c square_wall_object.c snake_game.c main.c $(CURSESLIBS) -o snake
clean:
	rm -rf snake