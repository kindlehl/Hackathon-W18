CC=g++
CFLAGS=--std=c++14 -L/lib -I/usr/include
CLINK= -lsfml-graphics -lsfml-window -lsfml-system

game: player.o environment.o src/main.cpp
	$(CC) $(CFLAGS) *.o src/main.cpp -o game $(CLINK)

environment.o: src/environment.cpp inc/environment.h
	$(CC) $(CFLAGS) src/environment.cpp -c $(CLINK)

player.o: src/player.cpp inc/player.h
	$(CC) $(CFLAGS) src/player.cpp -c $(CLINK)
