#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <uuid/uuid.h>
#include <string.h>

#include "utils.h"
#include "shared.h"

char* generate_random_uuid() {
  uuid_t binuuid;
  uuid_generate_random(binuuid);

  // each uuid has 36 characters
  // so we need to reserve 37 (include \0)
  char *uuid = malloc(37); 

  // convert UUID from binary representation to a string
  uuid_unparse(binuuid, uuid);
  return uuid;
}

void free_file_list(char** file_list, int num_files) {
  for (int i = 0; i < num_files; i++) {
    free(file_list[i]);
  }

  free(file_list);
}

char **list_files(const char* path, int* num_files) {
  DIR *d;
  struct dirent *dir;

  char **file_list = NULL; // initialize to NULL pointer
  int size = 10;
  int count = 0;

  d = opendir(path);
  if (d == NULL) {
    printf("error: We could not list files from the source directory of the project.\n");
    return NULL;
  }
 

  file_list = malloc(size * sizeof(char *));
  if (file_list == NULL) {
    printf("error (malloc): cannot allocate enough memory for our file_list.\n");
    closedir(d);
    return NULL;
  }

  while((dir = readdir(d)) != NULL) {
    // Check if the path is "." or ".."
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
      continue;
    }

    if (count >= size) {
      size *= 2;
      char **temp = realloc(file_list, size * sizeof(char *));
      if (temp == NULL) {
        printf("error (realloc): cannot reallocate more memory to file_list.\n");

        // Free previously allocated memory
        for (int i = 0; i < count; i++) {
          free(file_list[i]);
        }

        free(file_list);
        closedir(d);
        return NULL;
      }

      file_list = temp;
    }

    file_list[count] = strdup(dir->d_name);
    if (file_list[count] == NULL) {
      printf("error (strdup): could not copy the dir->d_name.\n");
      for (int i = 0; i < count; i++) {
        free(file_list[i]);
      }

      free(file_list);
      closedir(d);
      return NULL;
    }

    count++;
  }

  closedir(d);
  *num_files = count;
  return file_list;
}

int copy_file(char* source_name, char* commit_hash) {
  // Allocate proper size to the source_path string
  char* source_path = (char*) malloc(snprintf(0, 0, "./%s/%s", SOURCE_DIR, source_name) + 1);
  sprintf(source_path, "./%s/%s", SOURCE_DIR, source_name);

  FILE *source_file_ptr = fopen(source_path, "r");

  if (source_file_ptr == NULL) {
    printf("Cannot open file: %s\n", source_name);
    return 1;
  }

  char* dest_path = (char*) malloc(snprintf(0, 0, "%s/%s/%s", BEARGIT_DIR, commit_hash, source_name) + 1);
  sprintf(dest_path, "%s/%s/%s", BEARGIT_DIR, commit_hash, source_name);

  FILE *dest_file_ptr = fopen(dest_path, "w");

  if (dest_file_ptr == NULL) {
    printf("Cannot write a copy for commit %s for file %s.\n", commit_hash, source_name);
    return 1;
  }

  int c;
  while((c = fgetc(source_file_ptr)) != EOF)
    fputc(c, dest_file_ptr);

  fclose(source_file_ptr);
  fclose(dest_file_ptr);
  return 0;
}
