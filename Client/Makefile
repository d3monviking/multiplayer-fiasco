CC = g++
CFLAGS = -Wall -std=c++11 -I/opt/homebrew/Cellar/sfml/2.6.1/include \
         -I/opt/homebrew/Cellar/flatbuffers/24.3.25/include \
         -I/opt/homebrew/Cellar/boost/1.86.0_1/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.1/lib \
          -L/opt/homebrew/Cellar/flatbuffers/24.3.25/lib \
          -L/opt/homebrew/Cellar/boost/1.86.0_1/lib \
          -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network \
          -lflatbuffers -lboost_system -lpthread
RM = rm -rf

main: main.o level.o player.o tiles.o
	$(CC) $(CFLAGS) -o game main.o level.o player.o tiles.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lboost_system -lpthread $(LDFLAGS)
main.o: main.cpp gamestructs.h message_generated.h player.h level.h
	$(CC) $(CFLAGS) -c main.cpp
level.o: level.cpp gamestructs.h message_generated.h player.h tiles.h level.h
	$(CC) $(CFLAGS) -c level.cpp
player.o: player.cpp gamestructs.h message_generated.h player.h
	$(CC) $(CFLAGS) -c player.cpp
tiles.o: tiles.cpp tiles.h
	$(CC) $(CFLAGS) -c tiles.cpp
clean:
	$(RM) main *.o