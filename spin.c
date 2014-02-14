/************************************************
 *
 *  SPIN
 *
 *  Sets up a vagrant box VM.
 *  Copyright (c) 2013 JT Paasch
 *
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

#define HOME_DIR_MAX_LENGTH 50
#define SOURCE_PATH_MAX_LENGTH 100
#define DESTINATION_PATH_MAX_LENGTH 150

/*
 *  CONSTANTS
 */
const char *devbox_folder = ".devbox";
const char *vagrantfile_source = ".spin/config/vagrant/Vagrantfile";
const char *vagrantfile_filename = "Vagrantfile";
const char *playbook_source = ".spin/config/ansible/playbook.yml";
const char *playbook_folder = "ansible";
const char *playbook_filename = "playbook.yml";

/*
 *  SOME NON-CONSTANT VARIABLES
 *  THAT WILL BE INITIALIZED IN THE INIT() FUNCTION
 */
char home_dir[HOME_DIR_MAX_LENGTH];
char devbox_path[DESTINATION_PATH_MAX_LENGTH];
char vagrantfile_source_path[SOURCE_PATH_MAX_LENGTH];
char vagrantfile_destination_path[DESTINATION_PATH_MAX_LENGTH];
char playbook_source_path[SOURCE_PATH_MAX_LENGTH];
char playbook_folder_path[DESTINATION_PATH_MAX_LENGTH];
char playbook_destination_path[DESTINATION_PATH_MAX_LENGTH];

/*
 *  FROM UTILITIES.H: A NON-CONSTANT
 *  THAT GETS SET BY THE FIND_FILE() FUNCTION.
 */
extern char found_path[1024];

/*
 *  FUNCTION PROTOTYPES
 */
void destroy_box();
void init();
void print_usage();
void provision_box();
void spindown_box();
void spinup_box();
void ssh_box();

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

    // For "provision" commands.
    else if (strcmp(argument[1], "provision") == 0) {
      provision_box();
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
  home_dir[0] = '\0';
  devbox_path[0] = '\0';
  vagrantfile_source_path[0] = '\0';
  vagrantfile_destination_path[0] = '\0';
  playbook_source_path[0] = '\0';
  playbook_folder_path[0] = '\0';
  playbook_destination_path[0] = '\0';

  // Is there a devbox folder somewhere up the directory tree? 
  int has_folder = find_file(".", devbox_folder);
  if (has_folder) {
    strcat(devbox_path, found_path);
  } else {
    strcat(devbox_path, devbox_folder);
  }

  // Generate the HOME path.
  strcat(home_dir, getenv("HOME"));

  // Generate the path to the vagrant file source.
  strcat(vagrantfile_source_path, home_dir);
  strcat(vagrantfile_source_path, "/");
  strcat(vagrantfile_source_path, vagrantfile_source);

  // Generate the path to the vagrant file destination.
  strcat(vagrantfile_destination_path, devbox_path);
  strcat(vagrantfile_destination_path, "/");
  strcat(vagrantfile_destination_path, vagrantfile_filename);

  // Generate the path to the playbook source.
  strcat(playbook_source_path, home_dir);
  strcat(playbook_source_path, "/");
  strcat(playbook_source_path, playbook_source);

  // Generate the path to the playbook folder.
  strcat(playbook_folder_path, devbox_path);
  strcat(playbook_folder_path, "/");
  strcat(playbook_folder_path, playbook_folder);

  // Generate the path to the playbook destination.
  strcat(playbook_destination_path, playbook_folder_path);
  strcat(playbook_destination_path, "/");
  strcat(playbook_destination_path, playbook_filename);

}

/*
 *  Print usage instructions.
 *
 *  @return void
 */
void print_usage() {

  printf("Usage: spin [command]\n");
  printf("\n");
  printf("-- spin up        : creates and boots up a VM\n");
  printf("-- spin down      : shut down the VM\n");
  printf("-- spin ssh       : SSH into the VM\n");
  printf("-- spin provision : re-provision the VM\n");
  printf("-- spin destroy   : remove the VM from your system\n");

}

/*
 *  Spin up the VM.
 *
 *  @return void
 */
void spinup_box() {

  // Create a hidden .devbox folder (if absent).
  create_directory(devbox_path);

  // Copy over a Vagrantfile (if absent).
  copy_file(vagrantfile_source_path, vagrantfile_destination_path);

  // Create a playbook folder (if absent).
  create_directory(playbook_folder_path);

  // Copy over the default playbook (if absent).
  copy_file(playbook_source_path, playbook_destination_path);

  // Make sure virtualbox is installed.
  check_for_virtualbox();

  // Make sure the right version of vagrant is installed.
  check_for_vagrant();

  // Make sure ansible is installed.
  check_for_ansible();

  // Make sure git is installed.
  check_for_git();

  // Run `vagrant up`.
  char command[150];
  command[0] = '\0';
  strcat(command, "cd ");
  strcat(command, devbox_path);
  strcat(command, " && vagrant up");
  system(command);

}

/*
 *  Spin down the VM.
 *
 *  @return void
 */
void spindown_box() {

  // Run `vagrant halt`.
  char command[150];
  command[0] = '\0';
  strcat(command, "cd ");
  strcat(command, devbox_path);
  strcat(command, " && vagrant halt");
  system(command);

}

/*
 *  SSH into the VM.
 *
 *  @return void
 */
void ssh_box() {

  // Run `vagrant ssh`.
  char command[150];
  command[0] = '\0';
  strcat(command, "cd ");
  strcat(command, devbox_path);
  strcat(command, " && vagrant ssh");
  system(command);

}

/*
 *  Re-provision the VM.
 *
 *  @return void
 */
void provision_box() {

  // Run `vagrant provision`.
  char command[150];
  command[0] = '\0';
  strcat(command, "cd ");
  strcat(command, devbox_path);
  strcat(command, " && vagrant provision");
  system(command);

}

/*
 *  Destroy the whole VM.
 *
 *  @return void
 */
void destroy_box() {

  // Run `vagrant destroy`.
  char command[150];
  command[0] = '\0';
  strcat(command, "cd ");
  strcat(command, devbox_path);
  strcat(command, " && vagrant destroy");
  system(command);

  // Remove the folder.
  print_log("removing .devbox");
  command[0] = '\0';
  strcat(command, "rm -r ");
  strcat(command, devbox_path);
  system(command);

}
