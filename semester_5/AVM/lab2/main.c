#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printFile(FILE *file) {
  int current;

  while (current = fgetc(file), current != EOF) {
    while (current != '\n' && current != EOF) {
      putchar(current);
      current = fgetc(file);
    }
    putchar('\n');
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("No options is specified\n");
    return 1;
  } else if ((argc < 3) && (argc == 2)) {
    printf("The file name is not specified\n");
    return 1;
  }
  char *fileName = argv[2];

  FILE *fptr = NULL;

  if (argc == 3) {
    if (((strcmp(argv[1], "--show") == 0))) {
      fptr = fopen(fileName, "r");
      if (fptr == NULL) {
        printf("Can't open the file.\n");
        return 1;
      }
      printFile(fptr);
      fclose(fptr);
      int flag = 1;
      while (flag) {
        char c = getchar();
        if (c == 'q') {
          flag = 0;
        }
      }
    }
    return 0;
  }
}