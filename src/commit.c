#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "commit.h"
#include "utils.h"
#include "shared.h"

int handle_commit() {
  struct stat st = { 0 };

  if (stat(BEARGIT_DIR, &st) == -1) {
    printf("The beargit project has not been initiated. Initiate it first with 'beargit init'.\n");
    return 1;
  }

  char* commit_hash = generate_random_uuid();
  printf("A new commit has been created <%s>.\n", commit_hash);

  // Allocate a proper size for the new_dir string with malloc
  char* new_dir = (char*) malloc(snprintf(0, 0, "%s/%s", BEARGIT_DIR, commit_hash) + 1);

  // If memory of this size cannot be allocated, return error
  if (new_dir == NULL) {
    printf("error: not enough memory to allocate for new_dir.\n");
    return 1;
  }

  // @tothink: for safety reasons, consider switching to snprintf
  sprintf(new_dir, "%s/%s", BEARGIT_DIR, commit_hash);

  // @todo: Think about a better way to handle this error, most likely
  // by making 3 attempts to generate a new uuid (which should not happen, but who knows?)
  if (stat(new_dir, &st) != -1) {
    printf("Strange error: the commit with the same hash %s has already been created.\n", commit_hash);
    return 1;
  }

  // Make a directory for our new commit
  mkdir(new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  int num_files;
  char **files = list_files(SOURCE_DIR, &num_files);

  if (files == NULL) {
    printf("No files are there to be tracked. Cannot commit.\n");
  }
  
  for (int i = 0; i < num_files; i++) {
    int error = copy_file(files[i], commit_hash);

    if (error) {
      printf("\t - COULD NOT COMMIT %s\n", files[i]);
      return 1;
    } else {
      printf("\t - commited %s\n", files[i]);
    }
  }

  free_file_list(files, num_files);
  return 0;
}
