CC=g++
CFLAGS=--std=c++14 -L/lib -I/usr/include
CLINK= -lsfml-graphics -lsfml-window -lsfml-system

game: player.o src/main.cpp
	$(CC) $(CFLAGS) player.o src/main.cpp -o game $(CLINK)

player.o: src/player.cpp inc/player.h
	$(CC) $(CFLAGS) src/player.cpp -c $(CLINK)
