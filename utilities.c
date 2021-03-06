#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "utilities.h"

/*
 *  A NON-CONSTANT 
 *  THAT CAN BE SET BY THE FIND_FILE() FUNCTION
 */
char found_path[1024];

/*
 *  Check for ansible.
 *
 *  @return void
 */
void check_for_ansible() {

  // Print a message
  print_log("checking for ansible");

  // Does ansible exist?
  int exit_code = system("which ansible >/dev/null 2>&1");

  // An exit code of 256 means it does not exist.
  if (exit_code == 256) {
    printf("I cannot find ansible on your computer.\n");
    printf("Please install ansible, then run me again.\n");
    exit(1);
  }

  // An exit code of 0 means it does exist.
  else if (exit_code == 0) {

    // Report the status.
    print_log("-- ansible exists");
    print_log("-- moving on...");

  }

  // Otherwise, provide a generic error message.
  else {
    printf("I could not find ansible on your computer.\n");
    printf("I got an error when I looked for it.\n");
    exit(1);
  }

}

/*
 *  Check for git.
 *
 *  @return void
 */
void check_for_git() {

  // Print a message
  print_log("checking for git");

  // Does git exist?
  int exit_code = system("which git >/dev/null 2>&1");

  // An exit code of 256 means it does not exist.
  if (exit_code == 256) {
    printf("I cannot find git on your computer.\n");
    printf("Please install git, then run me again.\n");
    exit(1);
  }

  // An exit code of 0 means it does exist.
  else if (exit_code == 0) {

    // Report the status.
    print_log("-- git exists");
    print_log("-- moving on...");

  }

  // Otherwise, provide a generic error message.
  else {
    printf("I could not find git on your computer.\n");
    printf("I got an error when I looked for it.\n");
    exit(1);
  }

}

/*
 *  Check for vagrant.
 *
 *  @return void
 */
void check_for_vagrant() {

  // Print a message
  print_log("checking for vagrant");

  // Does vagrant exist?
  int exit_code = system("which vagrant >/dev/null 2>&1");

  // An exit code of 256 means it does not exist.
  if (exit_code == 256) {
    printf("I cannot find Vagrant on your computer.\n");
    printf("Please install Vagrant 1.4.3+, then run me again.\n");
    exit(1);
  }

  // An exit code of 0 means it does exist.
  else if (exit_code == 0) {

    // Report the status.
    print_log("-- vagrant exists");

    // Report that we're checking the version.
    print_log("checking version");

    // Run a command that gets the version number.
    FILE *stream = popen("vagrant -v | grep -o '[0-9]\\.[0-9]\\.[0-9]'", "r");

    // Read the results, character by character, into the string 'output'.
    char character;
    char output[150];
    int i = 0;
    while ((character = fgetc(stream)) != EOF) {
      output[i++] = character;
    }

    // Close the command stream.
    pclose(stream);

    // Find the major, minor, and revision digits in the version.
    int major, minor, revision;
    sscanf(output, "%d.%d.%d", &major, &minor, &revision);

    // Print the version.
    char version_message[24];
    sprintf(version_message, "-- version %d.%d.%d", major, minor, revision);
    print_log(version_message);

    // Version needs to be 1.4.3+
    if (major < 1 || minor < 2 || revision < 3) {
      printf("Vagrant 1.4.3+ is required.\n");
      printf("Upgrade to version 1.4.3+ and try me again.\n");
      exit(1);
    } else {
      print_log("-- moving on...");
    }

  }

  // Otherwise, provide a generic error message.
  else {
    printf("I could not find Vagrant on your computer.\n");
    printf("I got an error when I looked for it.\n");
    exit(1);
  }

}

/*
 *  Check for virtualbox.
 *
 *  @return void
 */
void check_for_virtualbox() {

  // Print a message
  print_log("checking for virtualbox");

  // Does virtualbox exist?
  int exit_code = system("which virtualbox >/dev/null 2>&1");

  // An exit code of 256 means it does not exist.
  if (exit_code == 256) {
    printf("I cannot find virtualbox on your computer.\n");
    printf("Please install virtualbox, then run me again.\n");
    exit(1);
  }

  // An exit code of 0 means it does exist.
  else if (exit_code == 0) {

    // Report the status.
    print_log("-- virtualbox exists");
    print_log("-- moving on...");

  }

  // Otherwise, provide a generic error message.
  else {
    printf("I could not find virtualbox on your computer.\n");
    printf("I got an error when I looked for it.\n");
    exit(1);
  }

}

/*
 *  Copy a file.
 *
 *  @param const char *source_file The path of the source file.
 *  @param const char *destination_file The path of the destination file.
 *  @return void
 */
void copy_file(const char *source_file, const char *destination_file) {

  // Print message.
  char message[100];

  message[0] = '\0';

  print_log("copying file");
  strcat(message, "-- source: ");
  strcat(message, source_file);
  print_log(message);

  message[0] = '\0';

  strcat(message, "-- destination: ");
  strcat(message, destination_file);
  print_log(message);

  // Initialize some pointers to the source and destination files.
  FILE *source, *destination;

  // If the destination file exists, report it and do nothing.
  if (access(destination_file, F_OK) != -1) {
    print_log("-- file already exists");
    print_log("-- moving on...");
  }

  // Otherwise, let's copy it over.
  else {

    // Open the source file for reading.
    source = fopen(source_file, "r");

    // Send a message if we don't have permission to open it.
    if (EACCES == errno) {
      print_log("-- no permission to open source file");
      exit(1);
    }

    // Send a message if the source file isn't there.
    else if (source == NULL) {
      print_log("-- no such source file");
      exit(1);
    }

    // Open (or create) the target file for (over)writing.
    destination = fopen(destination_file, "w");

    // If we don't have permission to open it, report that and exit.
    if (EACCES == errno) {
      print_log("-- no permission to open/create destination file");
      exit(1);
    }

    // If we can't otherwise open (or create it), report that and exit.
    else if (destination == NULL){
      print_log("-- couldn't open/create destination file");
      exit(1);
    }

    // Otherwise, let's copy the file, character by character.
    else {

      char character;
      while ((character = fgetc(source)) != EOF) {
        fputc(character, destination);
      }

      // TO DO: Check for writing errors?

      // Report the status.
      print_log("-- file copied");

    }

    // Close the files.
    fclose(source);
    fclose(destination);

  }

}

/*
 *  Create a folder.
 *
 *  @param char *name The name of the directory.
 *  @return void
 */
void create_directory(char *name) {

  // Print message: "-- creating directory: *name"
  char message[100];
  message[0] = '\0';
  strcat(message, "creating directory: ");
  strcat(message, name);
  print_log(message);

  // Try to create the directory.
  int success = mkdir(name, SPIN_DEFAULT_DIRECTORY_PERMISSIONS);

  // Report the status.
  if (success == 0) {
      print_log("-- directory created");
  }

  else if (EEXIST == errno) {
    print_log("-- directory already exists");
    print_log("-- moving on...");
  }

  else if (EACCES == errno) {
    print_log("-- no permission to create directory");
    exit(1);
  }

  else if (ENOSPC == errno) {
    print_log("-- not enough space to create directory");
    exit(1);
  }

  else {
    print_log("-- could not create directory");
    exit(1);
  }

}

/*
 *  Walk up a directory tree, looking 
 *  for a file or folder.
 *
 *  @param char *path The path to start looking in.
 *  @param char *target The name of the file or folder to find.
 *  @return int
 */
int find_file(char *path, const char *target) {

  DIR *stream;
  struct dirent *item;

  // Open the directory.
  stream = opendir(path);
  if (stream != NULL) {

    // Check each item in the directory.
    while ((item = readdir(stream))) {

      // If this is the item we're looking for, note the path.
      if (strcmp(item->d_name, target) == 0) {
        found_path[0] = '\0';
        strcat(found_path, path);
        strcat(found_path, "/");
        strcat(found_path, target);
        return 1;
      }

    }

    // We didn't find what we're looking for, so let's go up one level.
    // What's the path to the parent directory?
    char parent_directory[strlen(path) + 4];
    char full_path[1024];
    parent_directory[0] = '\0';
    strcat(parent_directory, path);
    strcat(parent_directory, "/../");

    // Check the parent directory.
    int success;
    realpath(parent_directory, full_path);
    if (strcmp(full_path, "/") == 0) {
      success = 0;
    } else {
      success = find_file(full_path, target);
    }

    // Close the stream and return the results.
    (void) closedir(stream);
    return success;

  }

  // We couldn't open the directory.
  else {
    return 0;
  }

}

/*
 *  Print verbose output to screen.
 *
 *  @param char *message The message to print.
 *  @return void
 */
void print_log(const char *message) {

  if (SPIN_VERBOSE_OUTPUT) {
    printf("-- %s\n", message);
  }

}
