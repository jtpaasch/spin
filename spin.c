/************************************************
 *
 *    SPIN
 *
 *    Sets up a vagrant box VM.
 *    Copyright 2013 JT Paasch
 *
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


/*
 *  CONSTANTS
 */
const int folder_permissions = 0755;   
const int verbose = 1;                 
const char *devbox_folder = ".devbox";
const char *vagrantfile_source = ".spin/config/vagrant/Vagrantfile";
const char *vagrantfile_destination = ".devbox/Vagrantfile";
const char *manifests_folder = ".devbox/manifests";
const char *manifest_source = ".spin/config/puppet/default.pp";
const char *manifest_destination = ".devbox/manifests/default.pp";


/*
 *  SOME NON-CONSTANT VARIABLES
 *  THAT WILL BE INITIALIZED IN THE MAIN() FUNCTION 
 */
char home_dir[50];
char vagrantfile_source_path[100];
char manifest_source_path[100];


/*
 *  FUNCTION PROTOTYPES
 */
void init();
void print_usage();
void print_message(const char *message);
void print_log(const char *message);
void create_directory(const char *name);
void copy_file(const char *source_file, const char *destination_file);
void check_for_virtualbox();
void check_for_vagrant();
void check_for_puppet();
void check_for_git();
void spinup_box();
void spindown_box();
void ssh_box();
void destroy_box();


/*
 *  The MAIN entry point to the program. 
 */
int main(int number_arguments, char *argument[]) {

  // Initialize things.
  init();

  // If no arguments were provided, display a "usage" message.
  if (number_arguments < 2) {
    print_usage();
  }

  // Otherwise, let's process the arguments.
  else {

    // For "up" commands.
    if (strcmp(argument[1], "up") == 0) {
      spinup_box();
    }

    // For "down" commands.
    else if (strcmp(argument[1], "down") == 0) {
      spindown_box();
    }

    // For "ssh" commands.
    else if (strcmp(argument[1], "ssh") == 0) {
      ssh_box();
    }

    // For "destroy" commands.
    else if (strcmp(argument[1], "destroy") == 0) {
      destroy_box();
    }

    // For anything else...i.e., any unrecognized command:
    else {
      printf("I don't recognize the command: %s\n", argument[1]);
      print_usage();
    }

  }

  // Exit cleanly.
  return 0;

}

/*
 *  Do anything needed to initialize the program.
 *
 *  @return void
 */
void init() {

  // Initialize some variables.
  sprintf(home_dir, "%s", getenv("HOME"));
  sprintf(vagrantfile_source_path, "%s/%s", home_dir, vagrantfile_source);
  sprintf(manifest_source_path, "%s/%s", home_dir, manifest_source);

}

/*
 *  Print usage instructions.
 *
 *  @return void
 */
void print_usage() {
  printf("Usage: spin [command]\n");
  printf("-- spin up      : creates and boots up a VM\n");
  printf("-- spin down    : shut down the VM\n");
  printf("-- spin ssh     : SSH into the VM\n");
  printf("-- spin destroy : remove the VM from your system\n");
}

/*
 *  Print verbose output to screen.
 *
 *  @param char *message The message to print.
 *  @return void
 */
void print_log(const char *message) {
  if (verbose) {
      printf("-- %s\n", message);
  }
}

/*
 *  Create a folder.
 *
 *  @param char *name The name of the directory.
 *  @return void
 */
void create_directory(const char *name) {

  // Print message: "-- creating directory: *name"
  char message[50];
  sprintf(message, "creating directory: %s", name);
  print_log(message);

  // Try to create the directory.
  int success = mkdir(name, folder_permissions);

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
 *  Copy a file.
 *
 *  @param const char *source_file The path of the source file.
 *  @param const char *destination_file The path of the destination file.
 *  @return void
 */
void copy_file(const char *source_file, const char *destination_file) {

  // Print message.
  char message[100];
  print_log("copying file");
  sprintf(message, "-- source: %s", source_file);
  print_log(message);
  sprintf(message, "-- destination: %s", destination_file);
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
    printf("Please install Vagrant 1.2+, then run me again.\n");
    printf("NOTE: Vagrant 1.1 or lower will not work.\n");
    exit(1);
  } 

  // An exit code of 0 means it does exist.
  else if (exit_code == 0) {

    // Report the status.
    print_log("-- vagrant exists");

    // Check if it's version 1.2 or higher.
    print_log("checking version");    

    // Run a command that gets the version number.
    FILE *stream = popen("vagrant -v | grep -o '[0-9]\\.[0-9]'", "r");

    // Read the results, character by character, into the string 'output'.
    char character;
    char output[150];
    int i = 0;
    while ((character = fgetc(stream)) != EOF) {
      output[i++] = character;
    }

    // Remove the ending \n character from the output.
    output[strlen(output)-1] = 0;

    // Close the command stream.
    pclose(stream);

    // Print the version.
    char version_message[18];
    sprintf(version_message, "-- version %s", output);
    print_log(version_message);

    // Version needs to be 1.2.
    int major, minor;
    sscanf(output, "%d.%d", &major, &minor);
    if (major < 1 || minor < 2) {
      printf("Vagrant 1.2+ is required. 1.1 or lower will not work.\n");
      printf("Upgrade to version 1.2+ and try me again.\n");
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
 *  Check for puppet.
 *
 *  @return void
 */
void check_for_puppet() {

  // Print a message
  print_log("checking for puppet");

  // Does puppet exist?
  int exit_code = system("which puppet >/dev/null 2>&1");

  // An exit code of 256 means it does not exist.
  if (exit_code == 256) {
    printf("I cannot find puppet on your computer.\n");
    printf("Please install puppet, then run me again.\n");
    exit(1);
  } 

  // An exit code of 0 means it does exist.
  else if (exit_code == 0) {

    // Report the status.
    print_log("-- puppet exists");
    print_log("-- moving on...");

  }

  // Otherwise, provide a generic error message.
  else {
    printf("I could not find puppet on your computer.\n");
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
 *  Spin up the VM.
 *
 *  @return void
 */
void spinup_box() {

  // Create a hidden .devbox folder (if absent).
  create_directory(devbox_folder);

  // Copy over a Vagrantfile (if absent).
  copy_file(vagrantfile_source_path, vagrantfile_destination);

  // Create a manifests folder (if absent).
  create_directory(manifests_folder);

  // Copy over the default manifest (if absent).
  copy_file(manifest_source_path, manifest_destination);

  // Make sure virtualbox is installed.
  check_for_virtualbox();

  // Make sure the right version of vagrant is installed.
  check_for_vagrant();

  // Make sure puppet is installed.
  check_for_puppet();

  // Make sure git is installed.
  check_for_git();

  // Run `vagrant up`.
  system("cd .devbox && vagrant up");

}

/*
 *  Spin down the VM.
 *
 *  @return void
 */
void spindown_box() {

  // Run `vagrant halt`.
  system("cd .devbox && vagrant halt");

}

/*
 *  SSH into the VM.
 *
 *  @return void
 */
void ssh_box() {

  // Run `vagrant ssh`.
  system("cd .devbox && vagrant ssh");

}

/*
 *  Destroy the whole VM.
 *
 *  @return void
 */
void destroy_box() {

  // Run `vagrant destroy`.
  system("cd .devbox && vagrant destroy");
  system("cd ..");

  // Remove the folder.
  print_log("removing .devbox");
  system("rm -r .devbox");

}
