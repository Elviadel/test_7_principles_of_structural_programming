#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_principles_of_structural_programming.h"

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
  char *a = NULL;
  composition_of_Function variables = {0};
  int number = 0;
  int enclosure = 0;
  char *previous_line = NULL;
  char *func_name = NULL;
  int all_function = 0;
  int succes_function = 0;

  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "grep: %s: No such file or directory", file_name);
    return;
  }
  while (number != EOF) {
    int end_function = 0;
    variables.count_line++;
    int length = StrLength(file);
    a = calloc((length + 2), sizeof(char));
    fgets(a, length + 1, file);
    number = fgetc(file);
    if (strchr(a, '}') != NULL) {
      if (enclosure > variables.max_enclosure) {
        variables.max_enclosure = enclosure;
      }
      enclosure--;
      if (enclosure == 0) {
        all_function++;
        end_function = 1;
      }
    }
    if (strchr(a, '{') != NULL) {
      if ((strchr(a, '(') != NULL || strchr(previous_line, '(') != NULL) &&
          !enclosure) {
        func_name = extractWordBeforeBracket(a, previous_line);
        variables.count_line = 0;
      }
      enclosure++;
    }

    if (strstr(a, "return") != NULL) {
      variables.count_return++;
    }

    if (strstr(a, "break") != NULL) {
      variables.count_break++;
    }

    if (end_function) {
      if (variables.max_enclosure >= 5 || variables.count_line > 50 ||
          variables.count_return >= 3 || variables.count_break > 0)
        if (func_name) {
          printf("function %s :\n\tenclouse: %d\n\tcount string: %d "
                 "\n\treturns: %d\n\tbreaks: %d\n",
                 func_name, variables.max_enclosure - 1, variables.count_line,
                 variables.count_return, variables.count_break);
          printf("-------------------------------------------------------------"
                 "---------------------\n");
          succes_function++;
        }
      end_function = 0;
      composition_of_Function side_struct = {0};
      variables = side_struct;
    }
    free(previous_line);
    previous_line = calloc(strlen(a) + 1, sizeof(char));
    strcpy(previous_line, a);
    free(a);
  }
  printf("IN FILE %s: ALL FUNCTION  %d , SUCCESS %d, FAIL %d", file_name,
         all_function, all_function - succes_function, succes_function);
  printf("\n");
  fclose(file);
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
  if (strchr(str, '(') == NULL) {
    line = calloc(strlen(previous_str) + 1, sizeof(char));
    strcpy(line, previous_str);
  } else {
    line = calloc(strlen(str) + 1, sizeof(char));
    strcpy(line, str);
  }
  char *word = calloc(30, sizeof(char));
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