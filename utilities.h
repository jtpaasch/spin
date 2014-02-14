#ifndef _SPIN_UTILITIES_H
#define _SPIN_UTILITIES_H

#define SPIN_DEFAULT_DIRECTORY_PERMISSIONS 0755
#define SPIN_VERBOSE_OUTPUT 1

void check_for_ansible();
void check_for_git();
void check_for_vagrant();
void check_for_virtualbox();
void copy_file(const char *source_file, const char *destination_file);
void create_directory(char *name);
int find_file(char *path, const char *target);
void print_log(const char *message);

#endif
