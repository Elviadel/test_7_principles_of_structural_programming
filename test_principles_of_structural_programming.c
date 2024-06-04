#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int StrLength(FILE *file);
void extractWordBeforeBracket(const char *str);
void SerchStr(char *file_name);
void strrev(char *str);

int main(int argc, char *argv[]) {
  for(int i = 1; i < argc; i++)
    SerchStr(argv[i]);
  return 1;
}

void SerchStr(char *file_name) {
  char *a = NULL;
  int countLine = 0;
  int number = 0;
  int enclosure = 0;
  int count_return = 0;
  int count_break = 0;
  int max_enclosure = 0;
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "grep: %s: No such file or directory", file_name);
  }
  while (number != EOF) {
    int end_function = 0;
    countLine++;
    int length = StrLength(file);
    a = calloc((length + 2), sizeof(char));
    fgets(a, length + 1, file);
    number = fgetc(file);
    if (strchr(a, '}') != NULL) {
      if (enclosure > max_enclosure) {
        max_enclosure = enclosure;
      }
      enclosure--;
      if (enclosure == 0) {
        end_function = 1;
      }
    }
    if (strchr(a, '{') != NULL) {
      if (strchr(a, '(') != NULL && !enclosure) {
        extractWordBeforeBracket(a);
        countLine = 0;
      }
      enclosure++;
    }

    if (strstr(a, "return") != NULL) {
      count_return++;
    }

    if (strstr(a, "break") != NULL) {
      count_break++;
    }

    if (end_function) {
      printf("\t%d максимальная вложенность,\n ", max_enclosure - 1);
      max_enclosure = 0;
      printf("\t%d строк,\n ", countLine);
      countLine = 0;
      printf("\t%d ретернов,\n ", count_return);
      count_return = 0;
      printf("\t%d бриков\n", count_break);
      count_break = 0;
      end_function = 0;
      max_enclosure = 0;
    }
    free(a);
  }
  printf("\n");
  fclose(file);
}
int StrLength (FILE *file) {
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
void extractWordBeforeBracket (const char *str) {
  char word[100];
  int i, j = 0;
  int bracketPos = -1;
  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] == '(') {
      bracketPos = i;
      break;
    }
  }
  for (i = bracketPos - 1; i >= 0; i--) {
    if (str[i] == ' ' || i == 0) {
      if (i == 0) {
        word[j++] = str[i];
      }
      break;
    }
    word[j++] = str[i];
  }
  word[j] = '\0';
  strrev(word);

  printf("функция %s содержит:\n ", word);
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