#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  int e;
  char *pattern;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Options;
int is_dir(const char *filename);
int process_regex(regex_t *reg_exp, const char *string);
int compile_regex(regex_t *reg_exp, const char *pattern, int flag);
void delete_memory(Options *options);
char *add_pattern(char *pattern, char *argv[]);
void process_pattern_file(char *pattern, char **old_pattern);

int compile_regex(regex_t *reg_exp, const char *pattern, int flag) {
  if (regcomp(reg_exp, pattern, flag)) {
    printf("Unable to compile regular expression");
    return 1;
  }
  return 0;
}

int is_dir(const char *filename) {
  struct stat file_stat;
  stat(filename, &file_stat);
  return S_ISDIR(file_stat.st_mode);
}
void delete_memory(Options *options) { free(options->pattern); }

int enable_f(const char *pattern_file, Options *options) {
  FILE *f = NULL;
  f = fopen(pattern_file, "r");
  if (f == NULL) {
    return 1;
  }
  ssize_t read;
  size_t len = 0;
  char *line = NULL;
  while ((read = getline(&line, &len, f)) != -1) {
    if (read > 0 && line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    char *pattern_copy = strdup(line);
    process_pattern_file(pattern_copy, &(options->pattern));
    free(pattern_copy);
  }

  free(line);
  fclose(f);

  return 0;
}
void process_pattern_file(char *pattern, char **old_pattern) {
  if (*old_pattern != NULL) {
    *old_pattern =
        realloc(*old_pattern, strlen(*old_pattern) + 1 + strlen(pattern) + 1);
    strcat(*old_pattern, "|");
    strcat(*old_pattern, pattern);
  } else {
    *old_pattern = realloc(*old_pattern, strlen(pattern) + 1);
    strcpy(*old_pattern, pattern);
  }
}

int read_file(int argc, char *argv[], Options *options) {
  FILE *f = NULL;
  regex_t reg_exp;
  int regex_flags = REG_EXTENDED;
  if (options->i == 1) {
    regex_flags |= REG_ICASE;
  }
  if (compile_regex(&reg_exp, options->pattern, regex_flags) == 1) {
    return 0;
  }

  int exit_status = 0;
  for (int i = optind; i < argc; ++i) {
    f = fopen(argv[i], "r");
    if (f == NULL) {
      if (options->s == 0) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[i]);
      }
      continue;
    }
    if (is_dir(argv[i]) == 1) {
      if (options->s == 0) {
        fprintf(stderr, "s21_grep: %s: Is a directory\n", argv[i]);
      }
      continue;
    }
    int file_number = argc - optind;
    int print_file_name = 1;
    if (file_number == 1 | options->h == 1) {
      print_file_name = 0;
    }
    char *line = NULL;
    size_t len_str = 0;
    int check;
    int count_line = 0;
    int org_line_number = 0;
    int new_line = 1;
    while ((check = getline(&line, &len_str, f)) != -1) {
      ++org_line_number;
      if (options->c == 0 && options->l == 0) {
        int result = process_regex(&reg_exp, line);
        if (result == 1 && options->v == 0) {
          if (print_file_name) {
            printf("%s:", argv[i]);
          }
          (options->n == 1) ? printf("%d:%s", org_line_number, line)
                            : printf("%s", line);
          new_line = (line[check - 1] == '\n');
        } else if (result == 0 && options->v == 1) {
          if (print_file_name) {
            printf("%s:", argv[i]);
          }
          (options->n == 1) ? printf("%d:%s", org_line_number, line)
                            : printf("%s", line);
          new_line = (line[check - 1] == '\n');
        } else if (result == -1) {
          return 1;
        }
      } else if (options->c == 1 && options->l == 0) {
        int result = process_regex(&reg_exp, line);
        if (result == 1 && options->v == 0) {
          ++count_line;
        } else if (result == 0 && options->v == 1) {
          ++count_line;
        }
      } else if (options->c == 1 && options->l == 1) {
        if (file_number == 1) {
          printf("%d\n", 1);
          printf("%s\n", argv[i]);
        } else {
          printf("%s:%d\n", argv[i], 1);
          printf("%s\n", argv[i]);
        }
        break;
      } else if (options->l == 1) {

        printf("%s\n", argv[i]);
        break;
      }
    }
    if (options->c == 1 && options->l == 0) {
      if (print_file_name) {
        printf("%s:", argv[i]);
      }
      printf("%d\n", count_line);
    }
    if (new_line == 0 && options->c == 0 && options->l == 0) {
      printf("\n");
    }
    fclose(f);
    free(line);
  }
  delete_memory(options);
  regfree(&reg_exp);

  return 0;
}

int process_regex(regex_t *reg_exp, const char *string) {
  regmatch_t match[1];
  int result = regexec(reg_exp, string, 0, NULL, 0);
  if (!result) {
    return 1;
  } // found
  else if (result == REG_NOMATCH) {
    return 0;
  } else {
    return -1;
  }
}
char *add_pattern(char *pattern, char *argv[]) {
  char *src = (optarg != NULL) ? optarg : argv[optind];
  if (src == NULL)
    return pattern;
  if (pattern != NULL) {
    pattern = realloc(pattern, strlen(pattern) + 1 + strlen(src) + 1);
    strcat(pattern, "|");
    strcat(pattern, src);
  } else {
    pattern = realloc(pattern, strlen(src) + 1);
    strcpy(pattern, src);
  }
  return pattern;
}

int parser(int argc, char *argv[],
           Options *options) { // grep -e "pattern 1" -e "pattern 2" file.txt
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclonhsf:")) !=
         -1) { // grep -i "pattern2" file.txt
    switch (opt) {
    case 'e':
      options->e = 1;
      char *pattern = add_pattern(options->pattern, argv);
      if (pattern == NULL) {
        return 1;
      }
      options->pattern = pattern;
      break;
    case 'i':
      options->i = 1;
      break;
    case 'f':
      options->f = 1;
      if (enable_f(optarg, options) == 1) {
        return 1;
      }
      break;
    case 'v':
      options->v = 1;
      break;
    case 'o':
      options->o = 1;
      break;
    case 's':
      options->s = 1;
      break;
    case 'c':
      options->c = 1;
      break;
    case 'l':
      options->l = 1;
      break;
    case 'n':
      options->n = 1;
      break;
    case 'h':
      options->h = 1;
      break;
    case '?':
      printf("Unknnown option");
      return 1;
    }
  }
  if (options->e == 0) {
    if (options->f == 0) {
      char *pattern = add_pattern(options->pattern, argv);
      if (pattern == NULL) {
        return 1;
      }
      options->pattern = pattern;
      ++optind;
    }
  }
  if (read_file(argc, argv, options) != 0) {
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    Options options = {0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if (parser(argc, argv, &options) == 1) {
      return 1;
    };
  } else {
    printf("No pattern or file");
    return 1;
  }
}