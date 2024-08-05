#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <command>\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "init") == 0) {
    printf("Hello world\n");
    return 0;
  } else {
    printf("Unknown command: %s\n", argv[1]);
    return 1;
  }
}
