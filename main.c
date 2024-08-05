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


int main(int argc, char *argv[]) {
  char* uuid = generate_random_uuid();
  printf("uuid: %s\n", uuid);


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
