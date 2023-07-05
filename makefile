SOURCE = game.cpp

TARGET = dist/game

build:
	g++ $(SOURCE) -lSDL2 -o $(TARGET)

clean:
	rm -rf $(TARGET)
