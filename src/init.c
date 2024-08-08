#include <stdio.h>
#include <sys/stat.h>

#include "commit.h"
#include "shared.h"

/**
  * This method handles the init of the beargit repository locally.
  **/
int handle_init() {
  struct stat st = { 0 };

  if (stat(BEARGIT_DIR, &st) == -1) {
    // grant all permissions
    mkdir(BEARGIT_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    printf("Successfully initiated a beargit project. Feel free to use 'beargit commit' to commit all your current changes.\n");
    return 0;
  } else {
    printf("The beargit project has been initiated. The .beargit directory has already been created.\n");
    return 1;
  }
}
