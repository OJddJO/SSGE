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

INCLUDE			= -I include
LIB				= -L $(OSDIR)/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
EXTRA			= -Werror -Wall -O3 -fPIC

all: create_dirs static dll
	@echo Static library:           $(STATIC_BUILD)
	@echo Dynamic library:          $(DLL_BUILD)
	@echo Implementation library:   $(IMPLIB_BUILD)

remake: clean all

$(OSDIR)/build/%.o: src/%.c
	@echo Compiling $*.c...
	@gcc $(INCLUDE) -c src/$*.c -o $(OSDIR)/build/$*.o $(EXTRA)

clean:
	@echo Cleaning up...
ifeq ($(OS),Windows_NT)
	@erase /q $(subst /,\,$(OBJ))
else
	@rm -f $(OBJ)
endif

create_dirs:
	@echo Creating directories...
ifeq ($(OS),Windows_NT)
	@if not exist $(OSDIR) mkdir $(OSDIR)
	@if not exist $(OSDIR)\bin mkdir $(OSDIR)\bin
	@if not exist $(OSDIR)\build mkdir $(OSDIR)\build
	@if not exist $(OSDIR)\lib mkdir $(OSDIR)\lib
else
	@mkdir -p $(OSDIR)/bin $(OSDIR)/build $(OSDIR)/lib
endif

static: $(OBJ)
	@echo Creating static library...
	@ar rcs $(STATIC_BUILD) $(OBJ)

dll: $(OBJ)
	@echo Creating dynamic library...
ifeq ($(OS),Windows_NT)
	@gcc -shared -o $(DLL_BUILD) $(OBJ) $(LIB) -Wl,--out-implib=$(IMPLIB_BUILD)
else
	@gcc -shared -fPIC -o $(DLL_BUILD) $(OBJ) $(LIB)
	@cp $(DLL_BUILD) $(IMPLIB_BUILD)
endif

.PHONY: all remake clean create_dirs static dll