CC = g++
CFLAGS = -Wall -std=c++11
RM = rm -rf

main: main.o level.o player.o tiles.o
	$(CC) $(CFLAGS) -o game main.o level.o player.o tiles.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lboost_system -lpthread
main.o: main.cpp gamestructs.h message_generated.h player.h level.h
	$(CC) $(CFLAGS) -c main.cpp
level.o: level.cpp gamestructs.h message_generated.h player.h tiles.h level.h
	$(CC) $(CFLAGS) -c level.cpp
player.o: player.cpp gamestructs.h message_generated.h player.h
	$(CC) $(CFLAGS) -c player.cpp
tiles.o: tiles.cpp tiles.h
	$(CC) $(CFLAGS) -c tiles.cpp
clean:
	$(RM) main *.o game