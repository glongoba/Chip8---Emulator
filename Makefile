SDL_CFLAGS = `sdl2-config --cflags`
SDL_LIBS   = `sdl2-config --libs`

all: ex.exe

ex.exe: chip8.o display.o main.o
	gcc chip8.o display.o main.o -o ex.exe $(SDL_LIBS)

chip8.o: chip8.c
	gcc -c chip8.c

display.o: display.c
	gcc -c display.c $(SDL_CFLAGS)

main.o: main.c
	gcc -c main.c $(SDL_CFLAGS)

clean:
	rm -f *.o ex.exe