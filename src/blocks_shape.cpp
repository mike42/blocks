#include "blocks_shape.h"

/** @brief standard_shapes
  *
  * @todo: document this function
  */
std::vector<blocks_shape> blocks_shape::standard_shapes() {
    /* Shapes are modelled to spin with the Super Rotation System: http://tetris.wikia.com/wiki/SRS
        The colours are ncurses colour numbers.
    */
    std::vector<blocks_shape> shapes;
    shapes.push_back(blocks_shape(std :: string("5555"), 2, 2)); // Box, 5 (magenta)
    shapes.push_back(blocks_shape(std :: string("0000666600000000"), 4, 4)); // I, 6 (cyan)
    shapes.push_back(blocks_shape(std :: string("330033000"), 3, 3)); // Z, 3 (yellow)
    shapes.push_back(blocks_shape(std :: string("077770000"), 3, 3)); // S, 7 (white)
    shapes.push_back(blocks_shape(std :: string("001111000"), 3, 3)); // L, 1 (red)
    shapes.push_back(blocks_shape(std :: string("040444000"), 3, 3)); // T, 4 (blue)
    shapes.push_back(blocks_shape(std :: string("200222000"), 3, 3)); // J, 2 (green)
    return shapes;
}

/** @brief blocks_shape
  *
  * @todo: document this function
  */
 blocks_shape::blocks_shape(std::string inp, int setWidth, int setHeight) {
    width = setWidth;
    height = setHeight;

	/* Load up a shape from a string */
	if((signed int)inp.size() != (width * height)) {
		throw 1;
	}

	if(setWidth > BLOCKS_SHAPE_WIDTH || setHeight > BLOCKS_SHAPE_HEIGHT) {
		/* This will make bounds problems later */
		throw 2;
	}

	int y, x;
	for(y = 0; y < BLOCKS_SHAPE_HEIGHT; y++) {
		for(x = 0; x < BLOCKS_SHAPE_WIDTH; x++) {
		    if(y < height && x < width) {
                point[y][x] = inp[y * width + x] - 48;
		    } else {
                point[y][x] = 0; // Blank space outside
		    }
		}
	}
}

/** @brief blocks_shape
  *
  * @todo: document this function
  */
 blocks_shape::blocks_shape(const blocks_shape& other) {
    width = other.width;
    height = other.height;

	int y, x;
	for(y = 0; y < BLOCKS_SHAPE_HEIGHT; y++) {
		for(x = 0; x < BLOCKS_SHAPE_WIDTH; x++) {
			point[y][x] = other.point[y][x];
		}
	}
}

/** @brief rotate
  *
  * @todo: document this function
  */
void blocks_shape::rotate()
{
	int oldPoint[BLOCKS_SHAPE_HEIGHT][BLOCKS_SHAPE_WIDTH];
	int x, y;

	/* Copy current state elsewhere*/
    for(y = 0; y < BLOCKS_SHAPE_HEIGHT; y++) {
		for(x = 0; x < BLOCKS_SHAPE_WIDTH; x++) {
			oldPoint[y][x] = point[y][x];
		}
	}

	/* Zero the space */
	for(y = 0; y < BLOCKS_SHAPE_HEIGHT; y++) {
		for(x = 0; x < BLOCKS_SHAPE_WIDTH; x++) {
			point[y][x] = 0;
		}
	}

	/* Basic 90 degree rotation */
	for(y = 0; y < width; y++) {
		for(x = 0; x < height; x++) {
			point[y][x] = oldPoint[x][width - 1 - y];
		}
	}

    /* Swap width and height */
    int temp;
    temp = width;
    width = height;
    height = temp;
}

