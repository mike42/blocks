#include "blocks_game.h"

/** @brief test_location
  *
  * @todo: document this function
  */
bool blocks_game::test_location(int point[BLOCKS_SHAPE_HEIGHT][BLOCKS_SHAPE_WIDTH], int top, int left){
    int x, y;
    int normX, normY;

    for(y = 0; y < BLOCKS_SHAPE_HEIGHT; y++) {
        for(x = 0; x < BLOCKS_SHAPE_WIDTH; x++) {
            if(point[y][x] != 0) {
                normX = x + left;
                normY = y + top;
                if(normX < 0 || normY < 0 || normX >= _width || normY >= _height) {
                    return false;
                }
                if(board[normY][normX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

/** @brief write_shape
  *
  * @todo: document this function
  */
void blocks_game::write_shape() {
    /* Set shape in at current location */
    if(falling_shape == nullptr) {
        return;
    }

    int x, y;
    int normX, normY;
    for(y = 0; y < falling_shape -> height; y++) {
        for(x = 0; x < falling_shape -> width; x++) {
            if(falling_shape -> point[y][x] != 0) {
                normX = x + falling_left;
                normY = y + falling_top;
                if(normX >= 0 && normY >= 0 && normX < _width && normY < _height){
                    board[normY][normX] = falling_shape -> point[y][x];
                }
            }
        }
    }
}

/** @brief new_shape
  *
  * @todo: document this function
  */
void blocks_game::new_shape() {
    /* Delete old shape and get the next one */
    if(falling_shape != nullptr) {
        delete falling_shape;
    }
    int id = shapeDist(gen);
    falling_shape = next_shape;

    /* Generate a new shape to come up next */
    next_shape = new blocks_shape(shapes[id]);
    int i, spinTimes;
    spinTimes = rotateDist(gen) % 4;
    for(i = 0; i < spinTimes; i++) {
        next_shape -> rotate();
    }

    if(falling_shape == nullptr) {
        /* Do a second loop (for when there were no shapes defined) */
        new_shape();
    } else {
        /* Good to go */
        falling_top = 0;
        falling_left = (_width - falling_shape -> width) / 2;

        render();
        /* Clear screen if current location cannot work */
        if(!test_location(falling_shape -> point, falling_top, falling_left)) {
            end();
        } else {
            notify_shape_changed();
        }
    }
}

/** @brief render
  *
  * @todo: document this function
  */
void blocks_game::render() {
    int x, y;
    for(y = 0; y < _height; y++) {
        for(x = 0; x < _width; x++) {
            cell[y][x] = board[y][x];
        }
    }

    int normX, normY;
    if(falling_shape != nullptr) {
        for(y = 0; y < falling_shape -> height; y++) {
            for(x = 0; x < falling_shape -> width; x++) {
                normX = x + falling_left;
                normY = y + falling_top;
                if(normX >= 0 && normY >= 0 && normX < _width && normY < _height) { // Bounds check
                    if(falling_shape -> point[y][x] != 0) { // Dont draw black squares over existing shapes
                        cell[normY][normX] = falling_shape -> point[y][x];
                    }
                }
            }
        }
    }

    notify_cells_changed();
}

/** @brief height
  *
  * @todo: document this function
  */
int blocks_game::height()
{
    return _height;
}

/** @brief width
  *
  * @todo: document this function
  */
int blocks_game::width()
{
    return _width;
}

/** @brief score
  *
  * @todo: document this function
  */
long blocks_game::score()
{
    return _score;
}

/** @brief rotate
  *
  * @todo: document this function
  */
void blocks_game::rotate() {
    if(falling_shape == nullptr || state != GAME_RUNNING) {
        return;
    }

    /* Spin a test copy first and see if it overlaps */
    blocks_shape test = blocks_shape(*falling_shape);
    test.rotate();

    /* Kick-back off walls up to 1 block if needed */
    bool okay = false;
    if(test_location(test.point, falling_top, falling_left)) {
        okay = true;
    } else if(test_location(test.point, falling_top, falling_left - 1)) {
        falling_left--;
        okay = true;
    } else if(test_location(test.point, falling_top, falling_left + 1)) {
        falling_left++;
        okay = true;
    }

    if(okay) {
        falling_shape -> rotate(); // All okay
        render();
    }
}

/** @brief right
  *
  * @todo: document this function
  */
void blocks_game::right() {
    if(falling_shape == nullptr || state != GAME_RUNNING) {
        return;
    }

    if(test_location(falling_shape -> point, falling_top, falling_left + 1)) {
        falling_left++;
        render();
    }
}

/** @brief left
  *
  * @todo: document this function
  */
void blocks_game::left() {
    if(falling_shape == nullptr || state != GAME_RUNNING) {
        return;
    }

    if(test_location(falling_shape -> point, falling_top, falling_left - 1)) {
        falling_left--;
        render();
    }
}

/** @brief down
  *
  * @todo: document this function
  */
void blocks_game::down() {
    if(falling_shape == nullptr || state != GAME_RUNNING) {
        return;
    }

    if(!test_location(falling_shape -> point, falling_top + 1, falling_left)) {
        write_shape();

        /* Check for full rows */
        int y, y2, x, full_count = 0;
        bool row_is_full;
        for(y = 0; y < _height; y++) {
            row_is_full = true;
            for(x = 0; x < _width; x++) {
                if(board[y][x] == 0) {
                    row_is_full = false;
                }
            }

            if(row_is_full) {
                /* Push every row above this onedown */
                for(y2 = y; y2 > 0; y2--) {
                    for(x = 0; x < _width; x++) {
                        board[y2][x] = board[y2 - 1][x];
                    }
                }

                /* Clear top row */
                for(x = 0; x < _width; x++) {
                    board[0][x] = 0;
                }

                full_count++;
            }
        }

        if(full_count != 0) {
            /* Scoring numbers from http://tetris.wikia.com/wiki/Scoring */
            switch(full_count) {
                case 1:
                    _score += 100 * _level;
                    break;
                case 2:
                    _score += 300 * _level;
                    break;
                case 3:
                    _score += 400 * _level;
                    break;
                default:
                    _score += 800 * _level;
            }
            notify_score_changed();
        }

        new_shape();
    } else {
        falling_top++;
        render();
    }
}

/** @brief drop
  *
  * @todo: document this function
  */
void blocks_game::drop() {
    int i = 0;
    while(test_location(falling_shape -> point, falling_top + i + 1, falling_left)) {
          /* See how far we can fall */
        i++;
    }

    if(i != 0) {
        falling_top += i;
        render();
    } else {
        /* Treat this like down key and lock the shape */
        down();
    }
}

/** @brief begin
  *
  * @todo: document this function
  */
void blocks_game::begin() {
    clear();

    if(state != GAME_RUNNING) {
        state = GAME_RUNNING;
        notify_state_changed();
    }

    _score = 0;
    notify_score_changed();
    _level = 1;
    notify_level_changed();

    /* Send the first shape */
    new_shape();
}

/** @brief ~blocks_game
  *
  * @todo: document this function
  */
 blocks_game::~blocks_game() {
    /* Free shapes */
    if(falling_shape != NULL) {
        delete falling_shape;
    }

    if(next_shape != NULL) {
        delete next_shape;
    }

    int y;
    for(y = 0; y < _height; y++ ) {
        /* Free columns */
        delete [] cell[y];
        delete [] last_cell[y];
        delete [] board[y];
        delete [] cell_haschanged[y];
    }

    /* Free rows */
    delete [] cell;
    delete [] last_cell;
    delete [] board;
    delete [] cell_haschanged;
}

/** @brief blocks_game
  *
  * @todo: document this function
  */
 blocks_game::blocks_game(int setWidth, int setHeight) {
    int y, x;
    if(setWidth < 5 || setHeight < 5) {
        throw 1;
    }

    /* All callbacks are null */
    check_for_input = nullptr;
    score_changed = nullptr;
    shape_changed = nullptr;
    state_changed = nullptr;
    cells_changed = nullptr;
    level_changed = nullptr;

    /* Set up all the buffers */
    _width = setWidth;
    _height = setHeight;
    _score = 0;
    _level = 1;

    /* Rows */
    cell        = new int *[_height];
    last_cell   = new int *[_height];
    board       = new int *[_height];
    cell_haschanged= new bool *[_height];

    for(y = 0; y < _height ; y++ ) {
        /* Columns */
        cell[y]         = new int[_width];
        last_cell[y]    = new int[_width];
        board[y]        = new int[_width];
        cell_haschanged[y] = new bool[_width];
        for(x = 0; x < _width; x++) {
            /* Blank everything */
            cell[y][x] = last_cell[y][x] = board[y][x] = 0;
            cell_haschanged[y][x] = false;
        }
    }

    /* Load shapes */
    shapes = blocks_shape :: standard_shapes();
    falling_shape = nullptr;
    next_shape = nullptr;
    falling_top = 0;
    falling_left = 0;

    /* Set up random engine */
    std :: random_device rd;
    gen.seed(rd());
    shapeDist =  std :: uniform_int_distribution<int> (0, shapes.size() - 1);
    rotateDist = std :: uniform_int_distribution<int> (0, 4);

    state = GAME_STOPPED;
}

void blocks_game :: clear() {
    /* Clear all boards */
    int y, x;
    for(y = 0; y < _height; y++ ) {
        for(x = 0; x < _width; x++) {
            cell[y][x] = last_cell[y][x] = board[y][x] = 0;
            cell_haschanged[y][x] = false;
        }
    }

    /* Clear shapes and whatnot */
    if(falling_shape != nullptr) {
        delete falling_shape;
    }

    if(next_shape != nullptr) {
        delete next_shape;
    }
}

/** @brief end
  *
  * @todo: document this function
  */
void blocks_game::end() {
    clear();

    if(state != GAME_STOPPED) {
        state = GAME_STOPPED;
        notify_state_changed();
    }
}

/** @brief run
  *
  * @todo: document this function
  */
void blocks_game::run() {
    /* Start a game if we don't have one yet */
    if(state != GAME_RUNNING) {
        begin();
    }

    /* Speed variables */
    int initial_speed = 400;
    int speedup_rate = 15; // Number of realistically playable levels.
    int frame_count = 0;

    /* Poll for input until game is over */
    struct timespec ts, now;
    long diff, frame_ns; /* Really only good to within ~5ms, but gettime works in nanoseconds */
    frame_ns = initial_speed * 1000000; // Initial speed here. Lower numbers are harder

    clock_gettime(CLOCK_MONOTONIC, &now);
    while(state == GAME_RUNNING) {
        ts = now;
        diff = 0;
        while(state == GAME_RUNNING && diff < frame_ns) {
            notify_check_for_input();
            usleep(5000);
            clock_gettime(CLOCK_MONOTONIC, &now);
            diff = (now.tv_sec - ts.tv_sec) * 1000000000 + (now.tv_nsec - ts.tv_nsec);
        }
        down();
        frame_count++;
        if(frame_count % 300 == 0) {
            // Increment levels
            _level++;
            notify_level_changed();
            frame_ns -= (frame_ns / speedup_rate); // Speed-up rate here. Lower number becomes more difficult very quickly
        }
    }
}

/** @brief notify_check_for_input
  *
  * @todo: document this function
  */
void blocks_game::notify_check_for_input() {
    if(check_for_input != nullptr) {
        check_for_input(this);
    }
}

/** @brief notify_state_changed
  *
  * @todo: document this function
  */
void blocks_game::notify_state_changed() {
    if(state_changed != nullptr) {
        state_changed(this);
    }
}

/** @brief notify_shape_changed
  *
  * @todo: document this function
  */
void blocks_game::notify_shape_changed() {
    if(shape_changed != nullptr) {
        shape_changed(this);
    }
}

/** @brief notify_score_changed
  *
  * @todo: document this function
  */
void blocks_game::notify_score_changed() {
    if(score_changed != nullptr) {
        score_changed(this);
    }
}

/** @brief notify_cells_changed
  *
  * @todo: document this function
  */
void blocks_game::notify_cells_changed() {
    if(cells_changed != nullptr) {
        cells_changed(this);
    }
}

/** @brief notify_level_changed
  *
  * @todo: document this function
  */
void blocks_game::notify_level_changed()
{
    if(level_changed != nullptr) {
        level_changed(this);
    }
}

