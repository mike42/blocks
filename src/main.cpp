#include "blocks_game.h"

#include <ncurses.h>
#include <iostream>

#define BORDER_WIDTH 2
#define GAP_WIDTH 2
#define SHAPE_WIDTH BLOCKS_SHAPE_WIDTH * 2
#define SHAPE_HEIGHT BLOCKS_SHAPE_HEIGHT
#define BORDER_COLOUR COLOR_WHITE

int game_width;
int game_height;
int game_left;
int game_top;

void draw_border() {
    int x, y;
    int colour = BORDER_COLOUR;

    /* Top border */
    if(game_top >= 1) {
        move(game_top - 1, game_left);
        attron(COLOR_PAIR(colour));
        for(x = 0; x < game_width; x++) {
            addch(' ');
        }
        attroff(COLOR_PAIR(colour));
    }

    /* Left & right borders */
    if(game_left >= 2 && COLS >= game_width + 4)
    for(y = 0; y < game_height; y++) {
        move(game_top + y, game_left - 2);
        attron(COLOR_PAIR(colour));
        addch(' ');
        addch(' ');
        attroff(COLOR_PAIR(colour));

        move(game_top + y, game_left + game_width);
        attron(COLOR_PAIR(colour));
        addch(' ');
        addch(' ');
        attroff(COLOR_PAIR(colour));
    }
}

void shape_changed(blocks_game *game) {
    /* Draw next shape if we have space */
    if(COLS < (game_width + BORDER_WIDTH * 2 + GAP_WIDTH + SHAPE_WIDTH + 2)) {
        /* Box will not fit here */
        return;
    }

    /* Figure out what to draw */
    int y, x, box_top, box_left;
    int colour;
    blocks_shape next = game -> next();

    box_top = game_top + 2;
    box_left = game_width + BORDER_WIDTH * 2 + GAP_WIDTH;
    mvprintw(box_top, box_left, "%s", "Next shape:");
    box_top += 2;
    box_left += 2;

    /* Shape */
    move(box_top, box_left);
    for(y = 0; y < BLOCKS_SHAPE_HEIGHT; y++) {
        move(box_top + y, box_left);
        for(x = 0; x < BLOCKS_SHAPE_WIDTH; x++) {
            colour = next.point[y][x];
            attron(COLOR_PAIR(colour));
            addch(' ');
            addch(' ');
            attroff(COLOR_PAIR(colour));
        }
    }
}

void cells_changed(blocks_game *game) {
	/* Draw each block */
	int x, y, colour;
	for(y = 0; y < game -> height(); y++) {
		move(y + game_top, game_left);
		for(x = 0; x < game -> width(); x++) {
			colour = game -> cell[y][x];
			/* Draw two spaces there */
			attron(COLOR_PAIR(colour));
			addch(' ');
			addch(' ');
			attroff(COLOR_PAIR(colour));
		}
	}

	move(LINES - 1, COLS - 1);

	refresh();
}

void score_changed(blocks_game *game) {
    int box_top, box_left;
    box_top = game_top + BLOCKS_SHAPE_HEIGHT + 5;
    box_left = game_width + BORDER_WIDTH * 2 + GAP_WIDTH;
    mvprintw(box_top, box_left, "%s", "Score:");
    mvprintw(box_top + 1, box_left + 4, "%d", game -> score());
}

void level_changed(blocks_game *game) {
    int box_top, box_left;
    box_top = game_top + BLOCKS_SHAPE_HEIGHT + 8;
    box_left = game_width + BORDER_WIDTH * 2 + GAP_WIDTH;
    mvprintw(box_top, box_left, "%s", "Level:");
    mvprintw(box_top + 1, box_left + 4, "%d", game -> level());
}

void check_for_input(blocks_game *game) {
	char inChar;
	inChar = getch();
	if(inChar != ERR) {
		switch(inChar) {
			case 'q':
				game -> end();
				break;
			case 5: /* Right */
                game -> right();
				break;
			case 4: /* Left */
				game -> left();
				break;
			case 3:
				game -> rotate();
				break;
			case 2: /* Down */
				game -> down();
				break;
            case ' ':
                game -> drop();
                break;
			default:
				break;
		}
	}
}

int main() {
    /* Initialise ncurses */
    initscr();
	if(has_colors() == FALSE) {
		/* Need colour terminal */
		endwin();
		std :: cout << "Your terminal does not support colours" << std :: endl;
		return 1;
    }

    /* Figure out dimensions */
    game_width = 14 * 2;
    game_height = 24;
    game_left = 2; //(COLS - game_width) / 2;
    game_top = LINES - game_height;

    if(LINES < game_height) {
        endwin();
        std :: cout << "Terminal must be " << game_height << " characters tall." << std :: endl;
        return 1;
    }
    if(COLS < game_width + 4) {
        endwin();
        std :: cout << "Terminal must be " << game_width << " characters wide" << std :: endl;
        return 1;
    }

    /* Input options */
	cbreak();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
    curs_set(0); // Hide cursor

    /* Define all colours */
	/*	COLOR_BLACK   0
		COLOR_RED     1
		COLOR_GREEN   2
		COLOR_YELLOW  3
		COLOR_BLUE    4
		COLOR_MAGENTA 5
		COLOR_CYAN    6
		COLOR_WHITE   7 */
	start_color();
	int i;
	init_pair(0, COLOR_WHITE, COLOR_BLACK); /* For text */
	for(i = 1; i < 8; i++) {
		init_pair(i, i, i); /* Solid blocks for blocks */
	}
    draw_border();

    /* Start up blocks game */
    blocks_game *blocks = new blocks_game(game_width / 2, game_height);
    blocks -> check_for_input = check_for_input;
    blocks -> cells_changed = cells_changed;
    blocks -> shape_changed = shape_changed;
    blocks -> score_changed = score_changed;
    blocks -> level_changed = level_changed;
    blocks -> run();
    delete blocks;

    /* Clean up */
    endwin();
	return 0;
}
