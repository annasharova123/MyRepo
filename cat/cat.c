#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
} Options;

int parser(int argc, char* argv[], Options* options);
void read_file(FILE* f);

int main(int argc, char* argv[]) {
  Options options = {0};
  if (argc > 1) {
    if (parser(argc, argv, &options) != 0) {
      return 1;
    }
  } else {
    printf("No file");
    return 1;
  }
}

int parser(int argc, char* argv[], Options* options) {
  FILE* f = NULL;
  int opt;
    while ((opt = getopt(argc, argv, "benst") != -1)) {
      switch (opt) {
        case 'b':
          options->b = 1;
          break;
        case 'e':
          options->e = 1;
          break;
        case 'n':
          options->n = 1;
          break;
        case 's':
          options->s = 1;
          break;
        case 't':
          options->t = 1;
          break;
        default:
          fprintf(stderr,"unknown option");
          return 1;
      }
    }
  for (int i = optind; i < argc; ++i) {
    f = fopen(argv[i], "r");
    if (f == NULL) {
      printf("Unable to open file");
      return 1;
    }
    
    read_file(f);
  }
  return 0;
}

void read_file(FILE* f) {
  int c;  // Исопльзуем int, а не char, тк 255 в двоичной системе 1111 1111
          // также как и -1(EOF), через доп код, и соответсвенно getc() можнт
          // воспринять
  // его за конец файла, а в случае int сивол хрнаится 32 битах, что ползволяет
  // различать -1(где все 1) и 255, где всего 8 единиц
  while ((c = fgetc(f)) != EOF) {
    printf("%c", c);
  }
}