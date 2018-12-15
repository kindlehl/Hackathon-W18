CC=g++
CFLAGS=--std=c++14 -L/lib -L/usr/local/lib -I/usr/include
CLINK= -lsfml-graphics -lsfml-window -lsfml-system

game: player.o environment.o src/main.cpp
	$(CC) $(CFLAGS) *.o src/main.cpp -o game $(CLINK)

environment.o: src/environment.cpp inc/environment.h
	$(CC) $(CFLAGS) src/environment.cpp -c $(CLINK)

player.o: src/player.cpp inc/player.h
	$(CC) $(CFLAGS) src/player.cpp -c $(CLINK)

server: src/server.cpp inc/server.h
	$(CC) $(CFLAGS) src/server.cpp -o server -lenet 
