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

/*
 *  CONSTANTS
 */
const char *devbox_folder = ".devbox";
const char *vagrantfile_source = ".spin/config/vagrant/Vagrantfile";
const char *vagrantfile_destination = ".devbox/Vagrantfile";
const char *playbook_folder = ".devbox/ansible";
const char *playbook_source = ".spin/config/ansible/playbook.yml";
const char *playbook_destination = ".devbox/ansible/playbook.yml";

/*
 *  SOME NON-CONSTANT VARIABLES
 *  THAT WILL BE INITIALIZED IN THE MAIN() FUNCTION
 */
char home_dir[HOME_DIR_MAX_LENGTH];
char vagrantfile_source_path[SOURCE_PATH_MAX_LENGTH];
char playbook_source_path[SOURCE_PATH_MAX_LENGTH];

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
  playbook_source_path[0] = '\0';
  vagrantfile_source_path[0] = '\0';

  strcat(home_dir, getenv("HOME"));

  strcat(playbook_source_path, home_dir);
  strcat(playbook_source_path, "/");
  strcat(playbook_source_path, playbook_source);

  strcat(vagrantfile_source_path, home_dir);
  strcat(vagrantfile_source_path, "/");
  strcat(vagrantfile_source_path, vagrantfile_source);

}

/*
 *  Print usage instructions.
 *
 *  @return void
 */
void print_usage() {

  printf("Usage: spin [command]\n");
  printf("\n");
  printf("\t-- spin up        : creates and boots up a VM\n");
  printf("\t-- spin down      : shut down the VM\n");
  printf("\t-- spin ssh       : SSH into the VM\n");
  printf("\t-- spin provision : re-provision the VM\n");
  printf("\t-- spin destroy   : remove the VM from your system\n");

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

  // Create a playbook folder (if absent).
  create_directory(playbook_folder);

  // Copy over the default playbook (if absent).
  copy_file(playbook_source_path, playbook_destination);

  // Make sure virtualbox is installed.
  check_for_virtualbox();

  // Make sure the right version of vagrant is installed.
  check_for_vagrant();

  // Make sure ansible is installed.
  check_for_ansible();

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
 *  Re-provision the VM.
 *
 *  @return void
 */
void provision_box() {

  // Run `vagrant provision`.
  system("cd .devbox && vagrant provision");

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
