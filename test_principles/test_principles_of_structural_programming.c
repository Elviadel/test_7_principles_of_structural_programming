#include "test_principles_of_structural_programming.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int handler_string(composition_of_Function *variables, char *line,
                   int count_line);

int StrLength(FILE *file);
char *extractWordBeforeBracket(const char *str, char *previous_str);
void SerchStr(char *file_name);
void strrev(char *str);

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    printf("\n");
    SerchStr(argv[i]);
  }
  return 1;
}

void SerchStr(char *file_name) {
  int number = 0;
  composition_of_Function *variables =
      calloc(1, sizeof(composition_of_Function));
  char *a = NULL;
  int fail_func = 0;
  int all_function = 0;
  int count_line = 0;
  int check_line = 0;
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "grep: %s: No such file or directory", file_name);
    return;
  }
  while (number != EOF) {
    count_line++;
    int length = StrLength(file);
    a = calloc((length + 2), sizeof(char));
    fgets(a, length + 1, file);
    number = fgetc(file);
    check_line = handler_string(variables, a, count_line);
    if (check_line >= 0) {
      if (check_line) {
        fail_func++;
      }
      free(variables);
      variables = calloc(1, sizeof(composition_of_Function));
      count_line = 0;
      all_function++;
    }
    count_line = (check_line == -2) ? 1 : count_line;
  }
  printf("IN FILE %s: ALL FUNCTION  %d , SUCCESS %d, FAIL %d", file_name,
         all_function, all_function - fail_func, fail_func);
  printf("\n");
  fclose(file);
}

int handler_string(composition_of_Function *variables, char *line,
                   int count_line) {
  variables->count_line = count_line;

  // 0 - fail func , 1 - succes func, -1 no end func, -2 start func
  int is_func_succes = -1;
  int end_function = 0;
  static char *previous_line = NULL;
  static char *func_name = NULL;

  if (strchr(line, '}') != NULL) {
    if (variables->enclosure > variables->max_enclosure) {
      variables->max_enclosure = variables->enclosure;
    }
    variables->enclosure--;
    if (variables->enclosure == 0) {
      end_function = 1;
    }
  }
  if (strchr(line, '{') != NULL) {
    if ((strchr(line, '(') != NULL || strchr(previous_line, '(') != NULL) &&
        !variables->enclosure) {
      func_name = extractWordBeforeBracket(line, previous_line);
      is_func_succes = -2;
    }
    variables->enclosure++;
  }

  if (strstr(line, "return") != NULL)
    variables->count_return++;

  if (strstr(line, "break") != NULL)
    variables->count_break++;

  if (end_function) {
    if (func_name ) {
      if (variables->max_enclosure > 4 || variables->count_line > 50 ||
          variables->count_return >= 2 || variables->count_break > 0) {
        printf("function %s :\n\tenclouse: %d\n\tcount string: %d "
               "\n\treturns: %d\n\tbreaks: %d\n",
               func_name, variables->max_enclosure - 1, variables->count_line,
               variables->count_return, variables->count_break);
        printf("-------------------------------------------------------------"
               "---------------------\n");
               is_func_succes = 1;
      }
      else{
        is_func_succes = 0;
      }
    } 
  }
  free(previous_line);
  previous_line = calloc(strlen(line) + 1, sizeof(char));
  strcpy(previous_line, line);
  return is_func_succes;
}

int StrLength(FILE *file) {
  int number;
  int count = 0;
  long set = ftell(file);
  while (((number = fgetc(file)) != '\n') && number != EOF) {
    count++;
  }
  rewind(file);
  fseek(file, set, SEEK_CUR);
  return count;
}

char *extractWordBeforeBracket(const char *str, char *previous_str) {
  char *line = NULL;
  char *word = NULL;
  if (strchr(str, '(') == NULL) {
    line = calloc(strlen(previous_str) + 1, sizeof(char));
    strcpy(line, previous_str);
    word = calloc(30, sizeof(char));
  } else {
    line = calloc(strlen(str) + 1, sizeof(char));
    strcpy(line, str);
    word = calloc(30, sizeof(char));
  }
  int i, j = 0;
  int bracketPos = -1;
  for (i = 0; line[i] != '\0'; i++) {
    if (line[i] == '(') {
      bracketPos = i;
      break;
    }
  }
  for (i = bracketPos - 1; i >= 0; i--) {
    if (line[i] == ' ' || i == 0) {
      if (i == 0) {
        word[j++] = line[i];
      }
      break;
    }
    word[j++] = line[i];
  }
  word[j] = '\0';
  strrev(word);
  if (line) {
    free(line);
  }
  return word;
}

void strrev(char *str) {
  if (!str) {
    return;
  }
  int i = 0;
  int j = strlen(str) - 1;

  while (i < j) {
    char c = str[i];
    str[i] = str[j];
    str[j] = c;
    i++;
    j--;
  }
}