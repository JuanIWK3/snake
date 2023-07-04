SOURCE = game.cpp

TARGET = dist/game

all: linux windows

linux:
	g++ $(SOURCE) -lSDL2 -o $(TARGET)

windows:
	g++ -w -lSDL2main -lSDL2 $(SOURCE) -o $(TARGET).exe

clean:
	rm -rf target/*

		
