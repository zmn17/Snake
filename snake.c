#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SNAKE_R '>'
#define SNAKE_L '<'
#define SNAKE_U '^'
#define SNAKE_D 'v'

// Function prototypes
void snake_move(int *y, int *x, char face, int box_height, int box_width);
void seed_foods(WINDOW *win, int *y, int *x, int box_height, int box_width);

int main() {
  srand(time(0));
  int row, col;

  // tracking score points
  int score = 0;

  // Default
  char snake = SNAKE_R;
  // Initialize NCurses
  initscr();
  // Disable cursor
  curs_set(FALSE);
  noecho();
  nodelay(stdscr, TRUE);

  // Background color/pattern for stdscr
  init_pair(1, COLOR_BLACK, COLOR_CYAN);

  // 33 119
  getmaxyx(stdscr, row, col);

  // snake - field size
  int box_height = 10;
  int box_width = 40;

  // coordinates for win
  int start_y = (row - box_height) / 2;
  int start_x = (col - box_width) / 2;

  // command for keypad
  int command;
  // create new window
  WINDOW *field = newwin(box_height, box_width, start_y, start_x);
  keypad(field, TRUE);
  // border
  box(field, 0, 0);
  nodelay(field, TRUE);

  // score
  move(10, COLS - box_width - 8);
  printw("Score: %i", score);
  bkgd('-');
  refresh();

  // snake: character position
  int snake_x = 2;
  int snake_y = 1;
  int food_x = -1, food_y = -1;

  // seed the first food
  seed_foods(field, &food_y, &food_x, box_height, box_width);

  do {
    // clear the snake
    wmove(field, snake_y, snake_x);
    waddch(field, ' ');

    // move the snake, based on its face, direction
    snake_move(&snake_y, &snake_x, snake, box_height, box_width);

    // draw the snake
    wmove(field, snake_y, snake_x);
    waddch(field, snake);
    wrefresh(field);

    // Check if snake eats the food
    if (snake_y == food_y && snake_x == food_x) {
      score += 2;
      mvprintw(1, 1, "Score: %d", score);
      refresh();

      // Clear the eaten food and respawn
      seed_foods(field, &food_y, &food_x, box_height, box_width);
    }

    // .1 sec delay
    napms(100);

    // waiting for command
    command = wgetch(field);

    switch (command) {
    case KEY_DOWN:
      // if (snake_y < box_height - 2) {
      //   snake_y++;
      //   snake = SNAKE_D;
      // }
      snake = SNAKE_D;
      break;
    case KEY_UP:
      // if (snake_y > 1) {
      //   snake_y--;
      //   snake = SNAKE_U;
      // }
      snake = SNAKE_U;
      break;
    case KEY_LEFT:
      // if (snake_x > 1) {
      //   snake_x--;
      //   snake = SNAKE_L;
      // }
      snake = SNAKE_L;
      break;

    case KEY_RIGHT:
      // if (snake_x < box_width - 2) {
      //   snake_x++;
      //   snake = SNAKE_R;
      // }
      snake = SNAKE_R;
      break;

    case 'q':
      return (1);
      break;
    }

  } while (true);

  delwin(field);

  endwin();

  return (0);
}

void snake_move(int *y, int *x, char face, int box_height, int box_width) {
  if (face == SNAKE_R && *x <= box_width - 2) {
    if (*x == (box_width - 2)) {
      *x = 0;
    }
    (*x)++;
  } else if (face == SNAKE_L && *x >= 1) {
    if (*x == 1) {
      *x = box_width - 2;
    }
    (*x)--;
  } else if (face == SNAKE_U && *y >= 1) {
    if (*y == 1) {
      *y = box_height - 2;
    }
    (*y)--;
  } else if (face == SNAKE_D && *y <= box_height - 2) {
    if (*y == (box_height - 2)) {
      *y = 0;
    }
    (*y)++;
  }
}

void seed_foods(WINDOW *win, int *y, int *x, int box_height, int box_width) {
  // Randomly selected food/obstacle characters
  char game_characters[] = {'@', '#', '$', '%', '&', 'X', '+', 'o', '!', '?'};
  int r_idx = rand() % (sizeof(game_characters) / sizeof(game_characters[0]));

  *y = rand() % (box_height - 2) + 1;
  *x = rand() % (box_width - 2) + 1;

  wmove(win, *y, *x);
  waddch(win, game_characters[r_idx]);
  wrefresh(win);

  // Set a timeout for food disappearance
  napms(5000); // 5-second delay
  wmove(win, *y, *x);
  waddch(win, ' ');
  wrefresh(win);
}
