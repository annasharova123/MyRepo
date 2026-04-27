#include "options.h"
void enable_v(int c) {
  if (c >= 0 && c < 32 && c != 9) {
    printf("^%c", c + 64);
  } else if (c > 127 && c < 160) {
    printf("M-^%c", c - 64);
  } else if (c == 127 && c != 9) {
    printf("^?");
  } else if (c >= 160) {
    printf("M-%c", c - 128);
  } else {
    printf("%c", c);
  }
}

void option_n(FILE* f) {
  int counter = 1;
  int c, need_to_print = 0, first_line = 1;
  while ((c = fgetc(f)) != EOF) {
    if (first_line) {
      printf("%6d\t", counter);
      first_line = 0;
      ++counter;
    }
    if (need_to_print == 1) {
      printf("%6d\t", counter);
      ++counter;
      need_to_print = 0;
    }
    printf("%c", c);
    if (c == '\n') {
      need_to_print = 1;
    }
  }
}
void option_e(FILE* f) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (c == '\n') {
      printf("%c", '$');
      printf("%c", c);
    } else {
      enable_v(c);
    }
  }
}
void option_b(FILE* f) {
  int counter = 1;
  int c, start_new_line = 1;
  while ((c = fgetc(f)) != EOF) {
    if (c != '\n' && start_new_line == 1) {
      printf("%6d\t", counter);
      ++counter;
      start_new_line = 0;
    }
    printf("%c", c);
    if (c == '\n') {
      start_new_line = 1;
    }
  }
}
void option_t(FILE* f) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (c == '\t') {
      printf("^I");
    } else if (c == '\n') {
      printf("%c", c);
    } else {
      enable_v(c);
    }
  }
}
void option_s(FILE* f) {
  int c;
  int counter_blank = 0;
  int first_line = 1;
  while ((c = fgetc(f)) != EOF) {
    if (c == '\n') {
      ++counter_blank;
    } else {
      if (counter_blank >= 2 && first_line != 1) {
        counter_blank = 0;
        printf("\n\n%c", c);
      } else if (counter_blank == 1 || first_line == 1) {
        counter_blank = 0;
        first_line = 0;
        printf("\n%c", c);
      } else {
        printf("%c", c);
      }
    }
  }
  if (counter_blank != 0) {
    if (counter_blank >= 2) {
      printf("\n\n");
    } else {
      printf("\n");
    }
  }
}
void option_E(FILE* f) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (c == '\n') {
      printf("%c", '$');
      printf("%c", c);
    } else {
      printf("%c", c);
    }
  }
}
void option_T(FILE* f) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (c == '\t') {
      printf("^I");
    } else {
      printf("%c", c);
    }
  }
}
void no_options(FILE* f) {
  int c;  // Исопльзуем int, а не char, тк 255 в двоичной системе 1111 1111
          // также как и -1(EOF), через доп код, и соответсвенно getc()
          // может
  // воспринять его за конец файла, а в случае int сивол хрнаится 32 битах,
  // что ползволяет различать -1(где все 1) и 255, где всего 8 единиц
  while ((c = fgetc(f)) != EOF) {
    printf("%c", c);
  }
}