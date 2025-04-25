EXE		    = ./bin/tictactoe
SRC         = $(wildcard src/*.c)
OBJ         = $(subst src, build, $(patsubst %.c, %.o, $(SRC)))

INCLUDE     = -I ./include
LIB         = -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSSGE
EXTRA       = -Werror -O3

all: create_dirs link

remake: clean all

clean:
	erase $(subst build/, build\, $(OBJ))

create_dirs:
	if not exist bin mkdir bin
	if not exist build mkdir build

build/%.o: src/%.c
	gcc $(INCLUDE) -c src/$*.c -o build/$*.o $(DBG) $(EXTRA)

link: $(OBJ)
	gcc $(OBJ) -o $(EXE) $(LIB) $(DBG) $(EXTRA)