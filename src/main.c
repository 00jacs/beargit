// Libraries necessary for handling system files
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "utils.h"
#include "shared.h"
#include "init.h"
#include "commit.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <command>\n\t- init: initialize the beargit repository\n\t- commit: commit all the current files\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "init") == 0) {
    return handle_init();
  } else if (strcmp(argv[1], "commit") == 0){
    // For now, by default, all files are commited;
    return handle_commit();
  } else {
    printf("Unknown command: %s\n", argv[1]);
    return 1;
  }
}
