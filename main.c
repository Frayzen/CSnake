#define _XOPEN_SOURCE 500
#include "tools.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static WINDOW *main_win = NULL;
static int gameSpeed = 300;
static struct snake s = {
    .posX = WIN_WIDTH / 2,
    .posY = WIN_HEIGHT / 2,
    .size = INIT_TAIL_LEN,
    .going = LEFT,
};

void printchar(int x, int y, char c) {
  wmove(main_win, y, x);
  wprintw(main_win, "%c", c);
}

void move_coord(int *x, int *y, enum dir d) {
  switch (d) {
  case TOP:
    *y = *y - 1;
    break;
  case BOT:
    *y = *y + 1;
    break;
  case LEFT:
    *x = *x - 1;
    break;
  case RIGHT:
    *x = *x + 1;
    break;
  }
}

int fruitX = -1;
int fruitY = -1;

void spawn_fruit(void) {
  gameSpeed = gameSpeed * (0.8);
  fruitX = rand() % WIN_WIDTH;
  fruitY = rand() % WIN_HEIGHT;
  printchar(fruitX, fruitY, fruit);
}

void rectangle(int y1, int x1, int y2, int x2) {
  mvhline(y1, x1, 0, x2 - x1);
  mvhline(y2, x1, 0, x2 - x1);
  mvvline(y1, x1, 0, y2 - y1);
  mvvline(y1, x2, 0, y2 - y1);
  mvaddch(y1, x1, ACS_ULCORNER);
  mvaddch(y2, x1, ACS_LLCORNER);
  mvaddch(y1, x2, ACS_URCORNER);
  mvaddch(y2, x2, ACS_LRCORNER);
}

void init() {
  main_win = initscr();
  EXIT_ON_ERROR(main_win, "The window could not be loaded");
  timeout(0);
  curs_set(0);
  noecho();
  for (int i = 0; i < WIN_WIDTH; i++)
    for (int j = 0; j < WIN_HEIGHT; j++)
      printchar(i, j, empty);
  for (int i = 0; i < MAX_TAIL_LEN; i++)
    s.tail[i] = RIGHT;
  int i = 0;
  int x = s.posX;
  int y = s.posY;
  while (i < s.size) {
    printchar(x, y, body);
    move_coord(&x, &y, s.tail[i++]);
  }
  rectangle(0, 0, WIN_HEIGHT, WIN_WIDTH);
  printchar(s.posX, s.posY, head);
  spawn_fruit();
}

enum dir inverse(enum dir d) {
  switch (d) {
  case TOP:
    return BOT;
  case LEFT:
    return RIGHT;
  case RIGHT:
    return LEFT;
  default:
    return TOP;
  }
}

void lose() {
  wclear(main_win);
  move(0, 0);
  printf("YOU LOST !");
  endwin();
  exit(1);
}
void check_lose() {
  int curX = s.posX;
  int curY = s.posY;
  move_coord(&curX, &curY, s.going);
  if (curX <= 0 || curX >= WIN_WIDTH)
    lose();
  if (curY <= 0 || curY >= WIN_HEIGHT)
    lose();
  char cur = mvinch(curY, curX);
  if (cur == body)
    lose();
}

void update_screen() {
  check_lose();
  if (s.posX == fruitX && s.posY == fruitY) {
    s.tail[s.size] = s.tail[s.size - 1];
    s.size++;
    spawn_fruit();
  }
  // UPDATE TAIL LIST
  for (int i = s.size; i > 0; i--)
    s.tail[i] = s.tail[i - 1];
  s.tail[0] = inverse(s.going);
  // PRINT HEAD FORWARD
  printchar(s.posX, s.posY, body);
  move_coord(&s.posX, &s.posY, s.going);
  printchar(s.posX, s.posY, head);
  // CLEAR TAIL
  int curX = s.posX;
  int curY = s.posY;
  for (int i = 0; i < s.size; i++)
    move_coord(&curX, &curY, s.tail[i]);
  printchar(curX, curY, empty);
  
  // SHOW SCORE
  wmove(main_win, WIN_HEIGHT + 1, 0);
  wprintw(main_win, "SCORE: %u", s.size);
  refresh();
}

void input(char i) {
#define INPUT_HANDLE(Char, Dir)                                                \
  case Char:                                                                   \
    if (s.going != inverse(Dir))                                               \
      s.going = Dir;                                                           \
    break;

  switch (i) {
    INPUT_HANDLE('w', TOP)
    INPUT_HANDLE('s', BOT)
    INPUT_HANDLE('d', RIGHT)
    INPUT_HANDLE('a', LEFT)
  }
}

void run() {
  while (1) {
    char pressed = getch();
    input(pressed);
    update_screen();
    usleep(1000 * gameSpeed);
  }
}

int main(void) {
  init();
  run();
  return 0;
}
