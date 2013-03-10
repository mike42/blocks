#include <random>
#include <unistd.h>
#include <sys/time.h>

#include "blocks_shape.h"

enum blocks_game_state {
	GAME_STOPPED, // Game is over (eg. not started yet or blocks stacked to top)
	GAME_RUNNING // Game is in progress
};

class blocks_game {
public:
	blocks_game(int setWidth, int setHeight);
	~blocks_game();

	blocks_game_state state;

    void (*check_for_input)(blocks_game *game);
    void (*score_changed)(blocks_game *game);
    void (*shape_changed)(blocks_game *game);
    void (*state_changed)(blocks_game *game);
    void (*cells_changed)(blocks_game *game);
    void (*level_changed)(blocks_game *game);

	/* Basic game control commands */
	void begin();   // Start a new game
	void end();     // End current game
    void run();     // Poll for input until game finishes

	void down();
	void left();
	void right();
	void rotate();
    void drop();

	/* Info */
	long score();
    int level() {return _level;};
	int width();
	int height();
    const blocks_shape falling() {return *falling_shape;};
    const blocks_shape next() {return *next_shape;};

	/* Rendered board goes here */
	int **cell;
	bool **cell_haschanged;

private:
	void render();
    void clear();
	void new_shape();	/* Drop a new shape from the top */
	void write_shape(); /* Write the current shape to the board here */

	long _score;
	int _width;
	int _height;
	int _level;

	int **board;
	int **last_cell; // Last rendered board

	/* Shapes falling */
	int falling_top;
	int falling_left;
	blocks_shape *falling_shape;
    blocks_shape *next_shape;
	bool test_location(int point[BLOCKS_SHAPE_HEIGHT][BLOCKS_SHAPE_WIDTH], int top, int left);
	std :: vector<blocks_shape> shapes;

	/* For random number generation */
	std :: default_random_engine gen;
	std :: uniform_int_distribution<int> shapeDist;
	std :: uniform_int_distribution<int> rotateDist;

	/* Wrappers for callbacks */
    void notify_cells_changed();
    void notify_score_changed();
    void notify_shape_changed();
    void notify_state_changed();
    void notify_check_for_input();
    void notify_level_changed();
};
