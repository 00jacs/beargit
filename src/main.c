// Libraries necessary for handling system files
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uuid/uuid.h>

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

char **list_files(char* path, int* num_files) {
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
  char* source_path = (char*) malloc(snprintf(0, 0, "./tracked-project-example/%s", source_name) + 1);
  sprintf(source_path, "./tracked-project-example/%s", source_name);

  FILE *source_file_ptr = fopen(source_path, "r");

  if (source_file_ptr == NULL) {
    printf("Cannot open file: %s\n", source_name);
    return 1;
  }

  char* dest_path = (char*) malloc(snprintf(0, 0, "./.beargit/%s/%s", commit_hash, source_name) + 1);
  sprintf(dest_path, "./.beargit/%s/%s", commit_hash, source_name);

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

int handle_commit() {
  struct stat st = { 0 };

  if (stat(".beargit", &st) == -1) {
    printf("The beargit project has not been initiated. Initiate it first with 'beargit init'.\n");
    return 1;
  }

  char* commit_hash = generate_random_uuid();
  printf("A new commit has been created <%s>.\n", commit_hash);

  // Allocate a proper size for the new_dir string with malloc
  char* new_dir = (char*) malloc(snprintf(0, 0, "./.beargit/%s", commit_hash) + 1);

  // If memory of this size cannot be allocated, return error
  if (new_dir == NULL) {
    printf("error: not enough memory to allocate for new_dir.\n");
    return 1;
  }

  // @tothink: for safety reasons, consider switching to snprintf
  sprintf(new_dir, "./.beargit/%s", commit_hash);

  // @todo: Think about a better way to handle this error, most likely
  // by making 3 attempts to generate a new uuid (which should not happen, but who knows?)
  if (stat(new_dir, &st) != -1) {
    printf("Strange error: the commit with the same hash %s has already been created.\n", commit_hash);
    return 1;
  }

  // Make a directory for our new commit
  mkdir(new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  int num_files;
  char **files = list_files("./tracked-project-example", &num_files);

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

/**
  * This method handles the init of the beargit repository locally.
  **/
int handle_init() {
  struct stat st = { 0 };

  if (stat(".beargit", &st) == -1) {
    // grant all permissions
    mkdir(".beargit", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    printf("Successfully initiated a beargit project. Feel free to use 'beargit commit' to commit all your current changes.\n");
    return 0;
  } else {
    printf("The beargit project has been initiated. The .beargit directory has already been created.\n");
    return 1;
  }
}

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
