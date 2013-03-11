default:
	mkdir -p bin
	g++ src/main.cpp src/blocks_game.cpp src/blocks_shape.cpp -o bin/blocks -lcurses -lrt -std=c++11 -Wall

doc:
	mkdir -p doc
	doxygen doxygen.conf

clean:
	rm bin/blocks
	rm -R doc/
