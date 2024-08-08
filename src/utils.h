char* generate_random_uuid();
char **list_files(const char* path, int* num_files);
void free_file_list(char** file_list, int num_files);
int copy_file(char* source_name, char* commit_hash);
