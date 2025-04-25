EXE		    = ./bin/tictactoe
SRC         = $(wildcard src/*.c)
OBJ         = $(subst src, build, $(patsubst %.c, %.o, $(SRC)))

INCLUDE     = -I ./include # *.h files in ./include
SSGE_LIB	= -L lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSSGE # must include when using SSGE
LIB         = -L lib -lmingw32 # other libraries
EXTRA       = -Werror -Wall -O3

all: setup create_dirs link

remake: clean all

setup:
	erase /f include\SSGE.h
	erase /f bin\SSGE.dll
	erase /f /q lib
	copy ..\include\SSGE.h .\include\SSGE.h
	copy ..\bin\SSGE.dll .\bin\SSGE.dll
	copy ..\lib .\lib

clean:
	erase $(subst build/, build\, $(OBJ))

create_dirs:
	if not exist bin mkdir bin
	if not exist build mkdir build

build/%.o: src/%.c
	gcc $(INCLUDE) -c src/$*.c -o build/$*.o $(DBG) $(EXTRA)

link: $(OBJ)
	gcc $(OBJ) -o $(EXE) $(LIB) $(SSGE_LIB) $(DBG) $(EXTRA)