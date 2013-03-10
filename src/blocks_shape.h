#include <vector>
#include <string>

#define BLOCKS_SHAPE_WIDTH 4
#define BLOCKS_SHAPE_HEIGHT 4

class blocks_shape {
public:
	int point[BLOCKS_SHAPE_HEIGHT][BLOCKS_SHAPE_WIDTH];
	bool fourRotations;
	int width;
	int height;

	blocks_shape(std::string inp, int setWidth, int setHeight);
	blocks_shape(const blocks_shape& other);

	static std :: vector<blocks_shape> standard_shapes();
	void rotate();
};
