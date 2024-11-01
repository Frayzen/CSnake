#ifndef TOOLS_H
#define TOOLS_H

#include <ncurses.h>
#include <stdlib.h>

#define WIN_WIDTH 40
#define WIN_HEIGHT 20
#define MAX_TAIL_LEN 100
#define INIT_TAIL_LEN 4
#define EXIT_ON_ERROR(Check, Message)                                          \
  if (!(Check)) {                                                              \
    fprintf(stderr, "%s", (Message));                                          \
    exit(1);                                                                   \
  }

enum UI {
  body = '#',
  empty = ' ',
  head = 'O',
  fruit = 'X',
};

enum dir { TOP, BOT, RIGHT, LEFT };

struct snake {
  int posX;
  int posY;
  int size;
  enum dir going;
  enum dir tail[MAX_TAIL_LEN];
};

#endif /* !TOOLS_H */
