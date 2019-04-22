install:	
	sudo apt-get update
	sudo apt-get -f install freeglut3 freeglut3-dev binutils-gold g++ cmake libglew-dev mesa-common-dev libglew1.5-dev libglm-dev
	sudo apt-get -f install libsoil-dev
	sudo apt-get -f install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev
	
	
TARGET = main
LIBS = -lglut -lGL -lGLU -lSOIL -lm -lSDL -lSDL_mixer

all: default
    
default: main

main.o: main.c
	gcc -c main.c -o main.o
	
main: main.o
	gcc main.o -o main $(LIBS)

run: main
	./main

clean:
	-rm -f main
