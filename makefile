STATIC_BUILD	= ./lib/libSSGE.a
DLL_BUILD		= ./bin/SSGE.dll
IMPLIB_BUILD	= ./lib/libSSGE.dll.a

SRC				= $(wildcard src/*.c)
OBJ				= $(subst src,build,$(patsubst %.c, %.o, $(SRC)))

INCLUDE			= -I ./include
LIB				= -L lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
EXTRA			= -Werror -Wall -O3

all: create_dirs static dll

remake: clean all

clean:
	rm -f $(OBJ)

create_dirs:
	if not exist bin mkdir bin
	if not exist build mkdir build
	if not exist lib mkdir lib

build/%.o: src/%.c
	gcc $(INCLUDE) -c src/$*.c -o build/$*.o $(EXTRA)

static: $(OBJ)
	ar rcs $(STATIC_BUILD) $(OBJ)

dll: $(OBJ)
	gcc -shared -o $(DLL_BUILD) $(OBJ) $(LIB) -Wl,--out-implib,$(IMPLIB_BUILD)
