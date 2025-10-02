ifeq ($(OS),Windows_NT)
OSDIR		= _windows
else
OSDIR		= _linux
endif

EXE			= $(OSDIR)/bin/test
SRC			= $(wildcard src/*.c)
OBJ			= $(subst src, $(OSDIR)/build, $(patsubst %.c, %.o, $(SRC)))

ifeq ($(OS),Windows_NT)
LIB			= -lmingw32 # other libraries
else
LIB			= -lm # other libraries
endif

SSGE_LIB	= -L $(OSDIR)/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSSGE # must include when using SSGE
INCLUDE		= -I ../include
EXTRA		= -Werror -Wall #-mwindows

all: create_dirs copySSGE link
	@echo Done

remake: clean all

copySSGE:
	@echo Setting up SSGE...
	@echo Copying SSGE files to the project directory...
	@echo This may take a while, please wait...
ifeq ($(OS),Windows_NT)
	@cp -f ../$(OSDIR)/bin/SSGE.dll ./$(OSDIR)/bin/SSGE.dll
	@cp -f ../$(OSDIR)/bin/SDL2.dll ./$(OSDIR)/bin/SDL2.dll
	@cp -f ../$(OSDIR)/bin/SDL2_image.dll ./$(OSDIR)/bin/SDL2_image.dll
	@cp -f ../$(OSDIR)/bin/SDL2_ttf.dll ./$(OSDIR)/bin/SDL2_ttf.dll
	@cp -f ../$(OSDIR)/bin/SDL2_mixer.dll ./$(OSDIR)/bin/SDL2_mixer.dll
else
	@cp -f ../$(OSDIR)/bin/libSSGE.so ./$(OSDIR)/bin/libSSGE.so
endif
	@rm -rf $(OSDIR)/lib
	@cp -rf ../$(OSDIR)/lib $(OSDIR)/lib

clean:
	@echo Cleaning up...
	@rm -f $(OBJ)

create_dirs:
	@echo Creating necessary directories...
	@mkdir -p $(OSDIR) $(OSDIR)/bin $(OSDIR)/build

$(OSDIR)/build/%.o: src/%.c
	@echo Compiling $*.c...
	@gcc $(INCLUDE) -c src/$*.c -o $(OSDIR)/build/$*.o $(EXTRA)

link: $(OBJ)
	@echo Linking object files...
ifeq ($(OS),Windows_NT)
	@gcc $(OBJ) -o $(EXE) $(LIB) $(SSGE_LIB) $(EXTRA)
else
	@gcc $(OBJ) -o $(EXE) $(LIB) $(SSGE_LIB) -Wl,-rpath='$$ORIGIN' $(EXTRA)
endif