#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>

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

int process_regex(regex_t* reg_exp, const char* string);
int compile_regex(regex_t* reg_exp, const char* pattern);
void delete_memory(Options* options);


int compile_regex(regex_t* reg_exp, const char* pattern){
  if(!regcomp(reg_exp, pattern, REG_EXTENDED)){
    printf("Unable to compile regular expression");
    return 1; 
  }
  return 0;
}

void delete_memory(Options* options){
  free(options->pattern);
}

int read_file(int argc, char *argv[], Options *options) {
  FILE *f = NULL;
  regex_t reg_exp;
  if (compile_regex(&reg_exp, options->pattern) == 1){ return 0;}
  for (int i = optind; i < argc; ++i) {
    f = fopen(argv[i], "r");
    if (f == NULL) {
      return 1;
    }

    if (options->e == 1) {
      char* line = NULL;
      size_t len_str = 0;
      int check;
      while((check = getline(&line,&len_str, f)) != -1 ){
        
        int result = process_regex(&reg_exp, line);
        if (result == 1){
          printf("%s",line);
        }else if(result == -1){
          return 1;
        }
      }
    }

    fclose(f);
  }
  delete_memory(options);
  return 0;
}

int process_regex(regex_t* reg_exp, const char* string){
  regmatch_t match[1];
  
  int result = regexec(reg_exp,string,0,NULL,0);
  if(!result){ return 1; } // found
  else if(result == REG_NOMATCH){ return 0; }
  else{ return -1; }
  regfree(reg_exp);
}
char *add_pattern(char *pattern) {
  if (pattern != NULL) {
    char *new_pattern = strcat(pattern, "|");
    new_pattern = strcat(pattern, optarg);
    pattern = realloc(pattern, strlen(new_pattern) + 1);
  } else {
    pattern = realloc(pattern, strlen(optarg) + 1);
  }
  return pattern;
}

int parser(int argc, char *argv[],
           Options *options) { // grep -e "pattern 1" -e "pattern 2" file.txt
  int opt;
  while ((opt = getopt(argc, argv, "e:ivcln")) !=
         -1) { // grep -i "pattern2" file.txt
    switch (opt) {
    case 'e':
      options->e = 1;
      char *pattern = add_pattern(options->pattern);
      if (pattern == NULL) {
        return 1;
      }
      options->pattern = pattern;
      break;
    case 'i':
      options->i = 1;
      break;
    case 'v':
      options->v = 1;
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
    case '?':
      printf("Unknnown option");
      return 1;
    }
  }
  if (options->e == 0) {
    add_pattern(argv[optind]);
    ++optind;
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