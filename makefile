ifeq ($(OS),Windows_NT)
OSDIR			= _windows
STATIC_BUILD	= $(OSDIR)/lib/libSSGE.a
DLL_BUILD		= $(OSDIR)/bin/SSGE.dll
IMPLIB_BUILD	= $(OSDIR)/lib/libSSGE.dll.a
else
OSDIR			= _linux
STATIC_BUILD	= $(OSDIR)/lib/libSSGE.all
DLL_BUILD		= $(OSDIR)/bin/libSSGE.so
IMPLIB_BUILD	= $(OSDIR)/lib/libSSGE.so
endif

SRC				= $(wildcard src/*.c)
OBJ				= $(subst src,$(OSDIR)/build,$(patsubst %.c, %.o, $(SRC)))
OBJ_STATIC		= $(subst src,$(OSDIR)/build_static,$(patsubst %.c, %.o, $(SRC)))

INCLUDE			= -I include
LIB				= -L $(OSDIR)/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
EXTRA			= -Werror -Wall -O3 -flto=auto -fPIC -DSSGE_BUILD

# Build mode: release, cpuSpecific
BUILD_MODE		?= release
ifeq ($(BUILD_MODE),cpuSpecific)
OPTIMIZE		= -march=native -mtune=native
else
OPTIMIZE		= -O3 -mtune=generic
endif

all: create_dirs static dll
	@echo Static library:    $(STATIC_BUILD)
	@echo Dynamic library:   $(DLL_BUILD)
	@echo Imp. library:      $(IMPLIB_BUILD)

remake: clean all

$(OSDIR)/build/%.o: src/%.c
	@echo [DYNAMIC] Compiling $*.c...
	@gcc $(INCLUDE) -c src/$*.c -o $(OSDIR)/build/$*.o $(EXTRA) $(OPTIMIZE)

$(OSDIR)/build_static/%.o: src/%.c
	@echo [STATIC] Compiling $*.c...
	@gcc $(INCLUDE) -c src/$*.c -o $(OSDIR)/build_static/$*.o $(EXTRA) $(OPTIMIZE) -DSSGE_STATIC -static

clean:
	@echo Cleaning up...
	@rm -f $(OBJ) $(OBJ_STATIC)

create_dirs:
	@echo Creating directories...
	@mkdir -p $(OSDIR)/bin $(OSDIR)/build $(OSDIR)/build_static $(OSDIR)/lib

static: $(OBJ_STATIC)
	@echo [STATIC] Creating static library...
	@ar rcs $(STATIC_BUILD) $(OBJ_STATIC)

dll: $(OBJ)
	@echo [DYNAMIC] Creating dynamic library...
ifeq ($(OS),Windows_NT)
	@gcc -shared -o $(DLL_BUILD) $(OBJ) $(LIB) -Wl,--out-implib=$(IMPLIB_BUILD)
else
	@gcc -shared -fPIC -o $(DLL_BUILD) $(OBJ) $(LIB)
	@cp $(DLL_BUILD) $(IMPLIB_BUILD)
endif

ubuntu-installsdl:
	@echo Installing SDL2 (Ubuntu)...
	@sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev -yq
	@echo Done

arch-installsdl2:
	@echo Installing SDL2 (Arch)...
	@sudo pacman -Sq --noconfirm sdl2 sdl2_image sdl2_ttf sdl2_mixer
	@echo Done
	
.PHONY: all remake clean create_dirs static dll