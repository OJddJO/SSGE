ifeq ($(OS),Windows_NT)
OSDIR		= _windows
else
OSDIR		= _linux
endif

EXE			= $(OSDIR)/tictactoe
SRC			= $(wildcard src/*.c)
OBJ			= $(subst src, $(OSDIR)/build, $(patsubst %.c, %.o, $(SRC)))

ifeq ($(OS),Windows_NT)
LIB			= -lmingw32 # other libraries
else
LIB			= -lm # other libraries
endif

SSGE_LIB	= -L $(OSDIR)/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSSGE # must include when using SSGE
INCLUDE		= -I include -I ../include # *.h files in include
EXTRA		= -Werror -Wall -O3 #-mwindows

all: create_dirs setup link
	@echo Done

remake: clean all

setup:
	@echo Setting up SSGE...
	@echo Copying SSGE files to the project directory...
	@echo This may take a while, please wait...
ifeq ($(OS),Windows_NT)
	@rm -f $(OSDIR)/bin/SSGE.dll
	@cp -f ../$(OSDIR)/bin/SSGE.dll ./$(OSDIR)/SSGE.dll
	@cp -f ../$(OSDIR)/bin/SDL2.dll ./$(OSDIR)/SDL2.dll
	@cp -f ../$(OSDIR)/bin/SDL2_image.dll ./$(OSDIR)/SDL2_image.dll
	@cp -f ../$(OSDIR)/bin/SDL2_ttf.dll ./$(OSDIR)/SDL2_ttf.dll
	@cp -f ../$(OSDIR)/bin/SDL2_mixer.dll ./$(OSDIR)/SDL2_mixer.dll
else
	@rm -f $(OSDIR)/bin/libSSGE.so
	@cp -f ../$(OSDIR)/bin/libSSGE.so ./$(OSDIR)/libSSGE.so
endif
	@rm -rf $(OSDIR)/lib
	@cp -rf ../$(OSDIR)/lib $(OSDIR)/lib
	@echo Copying assets...
	@cp -rf assets -T ./$(OSDIR)/assets

clean:
	@echo Cleaning up...
	@rm -f $(OBJ)

create_dirs:
	@echo Creating necessary directories...
	@mkdir -p $(OSDIR) $(OSDIR)/build

$(OSDIR)/build/%.o: src/%.c
	@echo Compiling $*.c...
	@gcc $(INCLUDE) -c src/$*.c -o $(OSDIR)/build/$*.o $(EXTRA)

link: $(OBJ)
	@echo Linking object files...
ifeq ($(OS),Windows_NT)
	@gcc $(OBJ) -o $(EXE) $(LIB) $(SSGE_LIB) $(EXTRA)
else
	@gcc $(OBJ) -o $(EXE) $(LIB) $(SSGE_LIB) -Wl,-rpath='$$ORIGIN/lib' $(EXTRA)
endif