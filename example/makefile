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
INCLUDE		= -I include # *.h files in ./include
EXTRA		= -Werror -Wall -O3 #-mwindows

all: create_dirs setup link
	@echo Done

remake: clean all

setup:
	@echo "Setting up SSGE..."
	@echo "Copying SSGE files to the project directory..."
	@echo "This may take a while, please wait..."
ifeq ($(OS),Windows_NT)
	@erase /q $(OSDIR)\SSGE.dll
	@copy /y ..\$(OSDIR)\ $(OSDIR)\ 
	@erase /q include\SSGE\ 
	@xcopy /y /s ..\include\SSGE\ include\SSGE\ 
	@erase /q $(OSDIR)\lib\ 
	@xcopy /y /s ..\$(OSDIR)\lib\ $(OSDIR)\lib\ 
else
	@rm -f $(OSDIR)/libSSGE.so
	@cp ../$(OSDIR)/ $(OSDIR)/
	@rm -rf include/SSGE
	@cp -r ../include/SSGE include/SSGE
	@rm -rf $(OSDIR)/lib
	@cp -r ../$(OSDIR)/lib $(OSDIR)/lib
endif

clean:
	@echo "Cleaning up..."
ifeq ($(OS),Windows_NT)
	@erase $(subst /, \, $(OBJ))
else
	@rm -f $(OBJ)
endif

create_dirs:
	@echo "Creating necessary directories..."
ifeq ($(OS),Windows_NT)
	@if not exist $(OSDIR) mkdir $(OSDIR)
	@if not exist $(OSDIR)\build mkdir $(OSDIR)\build
else
	@mkdir -p $(OSDIR) $(OSDIR)/build
endif

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