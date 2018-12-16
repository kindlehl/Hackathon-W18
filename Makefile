CC=g++
CFLAGS=--std=c++14 -L/lib -L/usr/local/lib -I/usr/include -g
CLINK= -lsfml-graphics -lsfml-window -lsfml-system -lenet

all: game server

game: bullet.o player.o environment.o connection.o rain.o src/main.cpp 
	$(CC) $(CFLAGS) *.o src/main.cpp -o game $(CLINK)

environment.o: src/environment.cpp inc/environment.h hitbox.o
	$(CC) $(CFLAGS) src/environment.cpp -c $(CLINK)

bullet.o: src/bullet.cpp inc/bullet.h
	$(CC) $(CFLAGS) src/bullet.cpp -c $(CLINK)

rain.o: src/rain.cpp inc/rain.h
	$(CC) $(CFLAGS) src/rain.cpp -c $(CLINK)

#bullet.o: src/bullet.cpp inc/bullet.h
	#$(CC) $(CFLAGS) src/bullet.cpp -c $(CLINK)

player.o: src/player.cpp inc/player.h hitbox.o
	$(CC) $(CFLAGS) src/player.cpp -c $(CLINK)

hitbox.o: src/hitbox.cpp inc/hitbox.h
	$(CC) $(CFLAGS) src/hitbox.cpp -c $(CLINK)

server: src/server.cpp 
	$(CC) $(CFLAGS) src/server.cpp -o server $(CLINK)

connection.o: src/connection.cpp inc/connection.h
	$(CC) $(CFLAGS) src/connection.cpp -c $(CLINK)

clean:
	rm -rf *.o
